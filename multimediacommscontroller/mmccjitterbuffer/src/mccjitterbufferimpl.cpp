/*
* Copyright (c) 2004-2007 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:    Implementation of Mcc Jitterbuffer
*
*/




// INCLUDE FILES
#include "mmcccodecinformation.h"
#include "mccjitterbufferimpl.h"
#include "mcccngenerator.h"
#include "mccinternalevents.h"
#include "mccinternaldef.h"
#include "mccjitterbufferlogs.h"
#include "mccjitterbufferobserver.h"

// LOCAL CONSTANTS
const TInt KDefaultSampleRateInkHz = 8;
const TInt KWbSampleRateInkHz = 16;
const TInt KLowBufferLimit = 2;
const TInt KOverflowAlarmLimit = 20;
const TInt KSequentialLateFramesLimit = 20;

// Codec frame sizes
const TInt KAMRNBFrameSize = 32;
const TInt KAMRWBFrameSize = 64;
const TInt KILBCFrameSize = 52;
const TInt KG729FrameSize = 24; // 10 octets + 2 for header

// Maximum possible sequence number of RTP packet with standard RTP header.
const TInt KMaxSeqNumber = 65535;        // 16-bit value

// Codec frame times
const TInt KAMRNBFrameTime = 20;

// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// TJitterBufferElement::CompareSeqNum
// Compare SequenceNumber
// -----------------------------------------------------------------------------    
//
TInt TJitterBufferElement::CompareSeqNum( const TJitterBufferElement& aElem1, 
        const TJitterBufferElement& aElem2 )
    {
    if ( aElem1.iSequenceNumber > aElem2.iSequenceNumber )
        {
        return (-1);
        }
    else if ( aElem1.iSequenceNumber < aElem2.iSequenceNumber )
        {
        return (1);
        }
    else
        {
        return (0);
        }
    }

// -----------------------------------------------------------------------------
// TJitterBufferElement::CompareStamp
// Compare Stamp
// -----------------------------------------------------------------------------        
//
TInt TJitterBufferElement::CompareStamp( const TJitterBufferElement& aElem1, 
        const TJitterBufferElement& aElem2 )
    {
    if ( aElem1.iStamp > aElem2.iStamp )
        {
        return (-1);
        }
    else if ( aElem1.iStamp < aElem2.iStamp )
        {
        return (1);
        }
    else
        {
        return (0);
        }
    }

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccJitterBufferImpl::CMccJitterBufferImpl
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMccJitterBufferImpl::CMccJitterBufferImpl( MJitterBufferObserver* aObserver): 
		iPacketsInBuffer( 0 ), 
        iLastGetSeqNum( -1 ), 
        iBufStampSorter( 
            TLinearOrder<TJitterBufferElement>( TJitterBufferElement::CompareStamp ) 
            ),
        iBufSequenceSorter( 
            TLinearOrder<TJitterBufferElement>( TJitterBufferElement::CompareSeqNum ) 
            ),
        iSeqNumIncrement( 0 ),
        iPlay( EFalse ),
        iObserver( aObserver ),
        iOverflowCounter( 0 ),
        iPlayToneInterval( 0 ),
        iNotifyUser( ETrue ),
        iLatestNotifiedEvent( KMccEventNone ),
        iSampleRate( KDefaultSampleRateInkHz ),
        iBufLenMultiplier( 1 )
    {
    iTonePlayTime.UniversalTime();
    }

// -----------------------------------------------------------------------------
// CMccJitterBufferImpl::NewL
// Static constructor.
// -----------------------------------------------------------------------------
//
CMccJitterBufferImpl* CMccJitterBufferImpl::NewL( MJitterBufferObserver* aObserver )
    {
    __JITTER_BUFFER( "CMccJitterBufferImpl::NewL 64bit version" )

    CMccJitterBufferImpl* self = new( ELeave ) CMccJitterBufferImpl( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// -----------------------------------------------------------------------------
// CMccJitterBufferImpl::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMccJitterBufferImpl::ConstructL()
    {
    SetBufferState( EIdle );
    SendStreamEventToClient( KMccStreamIdle, ETrue );
    }

// -----------------------------------------------------------------------------
// CMccJitterBufferImpl::~CMccJitterBufferImpl
// Destructor deallocate memory.
// -----------------------------------------------------------------------------
//
CMccJitterBufferImpl::~CMccJitterBufferImpl()
    {
    __JITTER_BUFFER( "CMccJitterBufferImpl::~CMccJitterBufferImpl entry" )

    delete iCnGenerator;    
        
    // Deallocate payload memory of jitter buffer elements.
    const TInt count = iBuffer.Count();
    for ( TInt i = 0; i < count; i++ )
        {
        delete iBuffer[i].iDataFrame;
        } 

    iBuffer.Close();
    iObserver = NULL;
    
    __JITTER_BUFFER( "CMccJitterBufferImpl::~CMccJitterBufferImpl exit" )
    }
    
// -----------------------------------------------------------------------------
// CMccJitterBufferImpl::SetupL
// Setup Jitterbuffer
// -----------------------------------------------------------------------------
//
void CMccJitterBufferImpl::SetupL( 
    TInt aPlayoutThreshold, 
    const TMccCodecInfo& aCInfo, 
    CMMFDevSound& aDevSound, 
    MAsyncEventHandler* aEventHandler,
    TUint32 aEndpointId )
    {
    __JITTER_BUFFER_INT1( "CMccJitterBufferImpl::SetupL aCInfo.iJitterBufBufferLength:",
                          aCInfo.iJitterBufBufferLength )
    __JITTER_BUFFER_INT1( "CMccJitterBufferImpl::SetupL aPlayoutThreshold:",
                          aPlayoutThreshold )
    __JITTER_BUFFER_INT1( "CMccJitterBufferImpl::SetupL aCInfo.iJitterBufInactivityTimeOut:",
                          aCInfo.iJitterBufInactivityTimeOut )
    __JITTER_BUFFER_INT1( "CMccJitterBufferImpl::SetupL aCInfo.iJitterBufPlayToneTimeout:",
                          aCInfo.iJitterBufPlayToneTimeout )
    __JITTER_BUFFER_INT1( "CMccJitterBufferImpl::SetupL aCInfo.iJitterBufPlayToneFrequency:",
                          aCInfo.iJitterBufPlayToneFrequency )
    __JITTER_BUFFER_INT1( "CMccJitterBufferImpl::SetupL aCInfo.iJitterBufPlayToneDuration:",
                          aCInfo.iJitterBufPlayToneDuration )
    __JITTER_BUFFER_INT1( "CMccJitterBufferImpl::SetupL aCInfo.iHwFrameTime:",
                          aCInfo.iHwFrameTime )
    __JITTER_BUFFER_INT1( "CMccJitterBufferImpl::SetupL aCInfo.iPtime:",
                          aCInfo.iPtime )
    
    // Changed if-statement to asserts. Added also check agains 0 hwframetime
    __ASSERT_ALWAYS( aCInfo.iPtime, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( aCInfo.iJitterBufBufferLength, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( aCInfo.iHwFrameTime, User::Leave( KErrArgument ) );
    
    if( aCInfo.iJitterBufInactivityTimeOut 	!= iCInfo.iJitterBufInactivityTimeOut 
    	|| aCInfo.iJitterBufBufferLength 	!= iCInfo.iJitterBufBufferLength
    	|| aCInfo.iHwFrameTime 				!= iCInfo.iHwFrameTime 
    	|| aCInfo.iFourCC 					!= iCInfo.iFourCC   
    	|| aPlayoutThreshold 				!= iOriginalPlayThreshold / iBufLenMultiplier )
	    {
	    // Save the original HW frame time because we may need it in case of
	    // dynamic G.711 adjustment.
	    const TUint8 origHwtime = iCInfo.iHwFrameTime;
	    iCInfo = aCInfo;
	    iEventHandler = aEventHandler;
	    iEndpointId = aEndpointId;
	    
	    __JITTER_BUFFER_INT1( "CMccJitterBufferImpl::SetupL origHwtime:", origHwtime )
	    
	    if( iCInfo.iJitterBufInactivityTimeOut )
	        {
	        if( ( iCInfo.iJitterBufPlayToneFrequency > 0 ) && 
	            ( iCInfo.iJitterBufPlayToneDuration > 0 ) )
	            {
	            iPlayToneInterval = iCInfo.iJitterBufPlayToneTimeout;            
	            iPlay = ETrue;
	            }
	        }
	        
	    TInt bufLenMultiplier = 1;
	    if ( iCInfo.iFourCC == KMMFFourCCCodeAMR )
	        {
	        iFrameSize = KAMRNBFrameSize;
	        iFrameTime = KAMRNBFrameTime;
	        }
	    else if ( iCInfo.iFourCC == KMMFFourCCCodeAWB )
	        {   
	        iFrameSize = KAMRWBFrameSize;
	        iFrameTime = KAMRNBFrameTime;
	        iSampleRate = KWbSampleRateInkHz;
	        }
	    else if( iCInfo.iFourCC == KMccFourCCIdG711 )
	        {
	        // G.711 is configured dynamically. Take voip headerlength also in to
	        // account. G.711 hwframetime is in milliseconds, so need to multiply.
	        iFrameSize = ( iCInfo.iHwFrameTime * KDefaultSampleRateInkHz )
	            + KVoIPHeaderLength;
	        iFrameTime = 0;
	        
	        // In case of G.711 codec dynamic configuration, we may need to double
	        // the jitterbuffer length if HW frame time is changed from 20ms to
	        // 10ms. 
	        if ( origHwtime )
	            {
	            bufLenMultiplier = origHwtime / iCInfo.iHwFrameTime;
	            if ( !bufLenMultiplier )
	                {
	                bufLenMultiplier = 1;
	                }
	            }
	        }
	    else if( iCInfo.iFourCC == KMccFourCCIdILBC )
	        {
	        iFrameSize = KILBCFrameSize;
	        iFrameTime = 0;
	        }
	    else if( iCInfo.iFourCC == KMccFourCCIdG729 )
	        {
	        iFrameSize = KG729FrameSize;
	        iFrameTime = 0;
	        // Multiply G.729 also by two...
	        bufLenMultiplier = 2;
	        }
	    else
	        {
	        __JITTER_BUFFER( "CMccJitterBufferImpl::SetupL KErrNotSupported" )
	        
	        User::Leave( KErrNotSupported );
	        }
	        
	    // Save the bufLenMultiplier value, so we can use it later in comparison   
		iBufLenMultiplier = bufLenMultiplier;
	            
	    // Delete old buffer & reset it
	    const TInt elems = iBuffer.Count();
	    for( TInt i = 0; i < elems; i++ )
	        {
	        delete iBuffer[i].iDataFrame;
	        iBuffer[i].iDataFrame = NULL;
	        }
	    
	    iBuffer.Reset();
	    
	    // Calculate needed elements
	    TInt bufLen = iCInfo.iJitterBufBufferLength * bufLenMultiplier;
	    
	    __JITTER_BUFFER_INT1( "CMccJitterBufferImpl::SetupL G.711 bufLenMultiplier ",
	        bufLenMultiplier )
	    __JITTER_BUFFER_INT1( "CMccJitterBufferImpl::SetupL iBufferLength: ",
	        bufLen )
	    
	    __ASSERT_ALWAYS( aPlayoutThreshold < bufLen, User::Leave( KErrTooBig ) );
	    
	    // if differences between bufferlength and treshold set by client
	    // is less than 10, increase bufferlength so the differences is 10
	    // this is to help buffer goes to overflow easily.
	    // Also possible G.711/729 multiplier needs to be taken care of.
	    CheckThresholdBufferLength( bufLen, aPlayoutThreshold );
	    iCurrentPlayThreshold = aPlayoutThreshold * bufLenMultiplier;
	    iOriginalPlayThreshold = aPlayoutThreshold * bufLenMultiplier;
	    
	    if( iCnGenerator )
	        {
	        delete iCnGenerator;
	        iCnGenerator = NULL;
	        }

	    iCnGenerator = CMccCnGenerator::NewL( iCInfo.iFourCC, aDevSound );
	    
	    // Create the elements in the Buffer
	    for( TInt k = 0; k < bufLen; k++ )
	        {
	        CMMFDataBuffer* buf = CMMFDataBuffer::NewL( iFrameSize );
	        CleanupStack::PushL( buf );
	        TJitterBufferElement newElement;
	        newElement.iDataFrame = buf;
	        newElement.iSequenceNumber = -1;
	        newElement.iStamp = -1;
	        iBuffer.AppendL( newElement );
	        CleanupStack::Pop( buf );
	        }
	    
	    // Zero the statistic members
	    iFramesLost = 0;
	    iFramesReceived = 0;
	    iNumOfLateFrames = 0;
	    iFramesRemoved = 0;
	    iFramesPlayed = 0;
	    iPacketsInBuffer = 0;
	    
	    // Calculate the sequence number increment
	    iSeqNumIncrement = iSampleRate * iCInfo.iHwFrameTime;
	    }
    } 
        
// -----------------------------------------------------------------------------
// CMccJitterBufferImpl::ResetBufferL
// Reset Jitterbuffer
// -----------------------------------------------------------------------------
//

void CMccJitterBufferImpl::ResetBuffer( TBool aPlayTone, TBool aNotifyUser )
    {
    const TInt bufLen = BufferLength();
    __JITTER_BUFFER_INT1( "CMccJitterBufferImpl::ResetBuffer bufLen: ", bufLen )
    __JITTER_BUFFER_INT1( "CMccJitterBufferImpl::ResetBuffer pktsIn: ", iPacketsInBuffer )
    
    for( TInt i = 0; i < bufLen; i++ )
        {
        iBuffer[i].iSequenceNumber = -1;
        iBuffer[i].iStamp = -1;
        }
    
    iLastGetSeqNum = -1;
    iCurrentPlayThreshold = iOriginalPlayThreshold;
    iPacketsInBuffer = 0; 
    iPlay = aPlayTone; 
    
    if ( BufferState() != EIdle && 
         aNotifyUser && 
         iLatestNotifiedEvent != KMccStreamIdle )
        {
        SendStreamEventToClient( KMccStreamIdle, ETrue );
        }
    
    SetBufferState( EIdle );
    iDropNextFrame = EFalse;
    }

// -----------------------------------------------------------------------------
// CMccJitterBufferImpl::CurrentCodec
// Return current codec.
// -----------------------------------------------------------------------------
//
TFourCC CMccJitterBufferImpl::CurrentCodec() const
    {
    return iCInfo.iFourCC;
    }
    
// -----------------------------------------------------------------------------
// CMccJitterBufferImpl::DelayUpL
// Adds a buffer element into the jitter buffer
// -----------------------------------------------------------------------------
//
void CMccJitterBufferImpl::DelayUpL()
    {
    CMMFDataBuffer* buf = CMMFDataBuffer::NewL( iFrameSize );
    CleanupStack::PushL( buf );
    TJitterBufferElement newElement;
    newElement.iDataFrame = buf;
    newElement.iSequenceNumber = -1;
    newElement.iStamp = -1;
    iBuffer.AppendL( newElement );
    CleanupStack::Pop( buf );
    
    // Insert one NO_DATA frame into the audio stream, so the jitterbuffer has
    // the possibility to grow at least one frame from the current status. 
    // If the current playout threshold is zero, then there is playout ongoing or
    // a DTX period. This means that we should buffer at least one frame before
    // starting playout.
    if( 0 == iCurrentPlayThreshold )
        {
        __JITTER_BUFFER( "CMccJitterBufferImpl::DelayUpL Adjusting Playout Threshold" )
        
        iCurrentPlayThreshold = iPacketsInBuffer + 1;
        }
    
    __JITTER_BUFFER_INT1( "CMccJitterBufferImpl::DelayUpL BUF_LEN:", BufferLength() )
    __JITTER_BUFFER_INT1( "CMccJitterBufferImpl::DelayUpL CUR_FRAMES:", iPacketsInBuffer )
    __JITTER_BUFFER_INT1( "CMccJitterBufferImpl::DelayUpL PLAY_TH:", iCurrentPlayThreshold )
    }
    
// -----------------------------------------------------------------------------
// CMccJitterBufferImpl::DelayDownL
// Removes an buffer element from the jitter buffer
// -----------------------------------------------------------------------------
//
void CMccJitterBufferImpl::DelayDownL()
    {
    // We need to remove one frame from the jitterbuffer. If currently we are
    // in a DTX period, we're in luck as we do not affect voice quality at all.
    
    // DURING DTX:
    // During DTX periods the jitterbuffer can be empty, so we cannot remove
    // anything from nothing. Then we basically can remove one element from the
    // buffer so it is shorter.
    
    // DURING SPEECH:
    // We'll need to remove one audio frame and mark the one preceding the 
    // removed frame as bad. Then we'll continue as usual.
    const TInt bufLen = BufferLength();
    __JITTER_BUFFER_INT1( "CMccJitterBufferImpl::DelayDownL BUF_LEN:", bufLen )
    __JITTER_BUFFER_INT1("CMccJitterBufferImpl::DelayDownL CUR_FRAMES:", iPacketsInBuffer )
    
    if( this->IsEmpty() )
        {
        // Cannot do anything for empty buffer
        if( KLowBufferLimit < bufLen )
            {
            __JITTER_BUFFER( "CMccJitterBufferImpl::DelayDownL Empty buffer" )
            
            delete iBuffer[bufLen - 1].iDataFrame;
            iBuffer.Remove( bufLen - 1 );
            }
        else if( 0 < iCurrentPlayThreshold )
            {
            __JITTER_BUFFER( "CMccJitterBufferImpl::DelayDownL Playthreshold modification" )
                        
            iCurrentPlayThreshold--;
            }
        else
            {
            __JITTER_BUFFER( "CMccJitterBufferImpl::DelayDownL Buffer empty, cant do anything" )
            // Cannot do anything currently
            return;
            }
        }
    else if( this->IsFull() )
        {
        // If there is sufficiently data in the buffer, then remove one
        // frame and mark the one preceding it bad
        if( KLowBufferLimit < iPacketsInBuffer )
            {
            __JITTER_BUFFER( "CMccJitterBufferImpl::DelayDownL Buffer full, doing removal" )
            
            // Remove the 2nd oldest frame and mark the oldest as bad
            delete iBuffer[1].iDataFrame;
            iBuffer.Remove( 1 );
            
            // MARK THE FIRST AS BAD FRAME!!!
            iPacketsInBuffer--;
            iFramesRemoved++;
            }
        else
            {
            // Cant do removing, see if playthreshold can be adjusted
            if( 0 < iCurrentPlayThreshold )
                {
                __JITTER_BUFFER( "CMccJitterBufferImpl::DelayDownL Buffer full, cant do anything" )
                
                iCurrentPlayThreshold--;
                }
            }
        }
    else
        {
        if( KLowBufferLimit < bufLen )
            {
            __JITTER_BUFFER( "CMccJitterBufferImpl::DelayDownL Some frames, adjusting length" )
            
            // Adjust the length of the buffer
            delete iBuffer[bufLen - 1].iDataFrame;
            iBuffer.Remove( bufLen - 1 );
            }
        else
            {
            __JITTER_BUFFER( "CMccJitterBufferImpl::DelayDownL Some frames, too small buffer" )
            }
        }
    }
    
// -----------------------------------------------------------------------------
// CMccJitterBufferImpl::CalculateDelay
// Calculates the current delay that jitterbuffer has in the playback
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds32 CMccJitterBufferImpl::CalculateDelay() const
    {
    __JITTER_BUFFER_INT1( "CMccJitterBufferImpl::CalculateDelay:", iCInfo.iPtime * iPacketsInBuffer )

    return iCInfo.iPtime * iPacketsInBuffer;
    }        
        
// -----------------------------------------------------------------------------
// CMccJitterBufferImpl::AddDataFrameL
// Adds a audio frame into the jitterbuffer passed in the aDataBuffer
// -----------------------------------------------------------------------------
//
void CMccJitterBufferImpl::AddDataFrameL( CMMFBuffer* aDataBuffer )
    {
    User::LeaveIfNull( aDataBuffer );
    const TInt64 seqNum = aDataBuffer->FrameNumber();
    iFramesReceived++;
    
    const TInt num( FindLargestSeqNum() );
    
    __JITTER_BUFFER_INT1( "CMccJitterBufferImpl::AddDataFrameL FRAM_SEQNUM:", seqNum )
    __JITTER_BUFFER_INT1( "CMccJitterBufferImpl::AddDataFrameL LAST_SEQNUM:", iLastGetSeqNum )
    __JITTER_BUFFER_INT1( "CMccJitterBufferImpl::AddDataFrameL PACKETS:", iPacketsInBuffer )
    
    const CMMFDataBuffer* dataBuffer( static_cast<CMMFDataBuffer*>( aDataBuffer ) );
    
    if( seqNum > iLastGetSeqNum )
        {
        iNumOfSequentialLateFrames = 0;
        if( IsFull() )
            {
            __JITTER_BUFFER_INT1( "CMccJitterBufferImpl::AddDataFrameL OFLOW dropnext: ",
                iDropNextFrame );
            
            iFramesRemoved++;
			iOverflowCounter++;
			if ( iObserver && iOverflowCounter > KOverflowAlarmLimit )
                {
	            __JITTER_BUFFER( "CMccJitterBufferImpl::AddDataFrameL OFLOW reported" )
	            
                iOverflowCounter = 0;
                iObserver->JitterBufferError( MJitterBufferObserver::EBufferOverflow );
                ResetBuffer( ETrue, EFalse );
                
                // Do not inform resource unavailability several times. Once playback
                // is ok again, resource unavailability can be informed if it occurs.
                SendStreamEventToClient( KMccResourceNotAvailable, ETrue );
                iNotifyUser = EFalse;
                }
            
            // Check that we actually have packets to remove from the buffer.
            if ( !iDropNextFrame && iPacketsInBuffer )
                {
                // Remove the newest frame in the buffer
                (void) iBuffer[ iPacketsInBuffer - 1 ].iSequenceNumber;
                iBuffer[ iPacketsInBuffer - 1 ].iSequenceNumber = -1;
                iBuffer[ iPacketsInBuffer - 1 ].iStamp = -1;
                iPacketsInBuffer--;
                
                iDropNextFrame = ETrue;
                }
            else
                {
                iDropNextFrame = EFalse;
                }
            
            // If we dropped this frame, then return from here.
            if ( !iDropNextFrame )
                {
                return;
                }
            }
        else if( IsEmpty() )
            {
            // Buffering
            HandleNotifications( ETrue );
            
    		TTimeIntervalSeconds interval;
    		TTime currentTime;
    		currentTime.UniversalTime();
            currentTime.SecondsFrom( iTonePlayTime, interval );
            
            if( iPlay && interval.Int() >= iPlayToneInterval )
                {    
                __JITTER_BUFFER( "CMccJitterBufferImpl::AddDataFrameL tone to be played" )                
    			iTonePlayTime.UniversalTime();
                iPlay = EFalse;
                }
            
            const TInt64 nextGetSeqNum( iLastGetSeqNum + iSeqNumIncrement );
            if( seqNum > nextGetSeqNum )
                {
                // We are empty and there is frames missing between last get frame
                // and frame sequence number. This might be because of DTX period
                // so we need to adjust to the situation
                iLastGetSeqNum = seqNum - iSeqNumIncrement;
                
                __JITTER_BUFFER( "CMccJitterBufferImpl::AddDataFrameL MISSING & EMPTY" )
                __JITTER_BUFFER_INT2( "CMccJitterBufferImpl::AddDataFrameL ADJ_LAST:", iLastGetSeqNum,
                                      " FRM_SEQ:", seqNum )
                }
            }
        else
            {
            __JITTER_BUFFER( "CMccJitterBufferImpl::AddDataFrameL NORMAL" )
            
			iOverflowCounter = 0;
            }
        
        // Reset inactivity timing. NB. all branches need to reset inactivity
        // and insert the new element.
        iInactivityTime = 0;
        InsertBufferElement( dataBuffer->Data(), num, seqNum );
        }
    else
        {
        iNumOfLateFrames++;
        if(++iNumOfSequentialLateFrames > KSequentialLateFramesLimit )
             iLastGetSeqNum = -1;
        
        __JITTER_BUFFER( "CMccJitterBufferImpl::AddDataFrameL TOO LATE" )
        }
    }
    
// -----------------------------------------------------------------------------
// CMccJitterBufferImpl::GetDataFrameL
// Get DataFrame
// -----------------------------------------------------------------------------
//
void CMccJitterBufferImpl::GetDataFrameL( CMMFBuffer* aBuffer )
    {
    User::LeaveIfNull( aBuffer );
    CMMFDataBuffer& dataBuffer = static_cast<CMMFDataBuffer&>( *aBuffer );
    TDes8& playBuffer( dataBuffer.Data() );
    iFramesPlayed++;
    
    __JITTER_BUFFER_INT1( "CMccJitterBufferImpl::GetDataFrameL PACKETS:", iPacketsInBuffer )
    
    if( IsEmpty() )
        {
        // Buffer is empty, so we need to generate CN frames. During DTX period
        // this is where we'll end up. If we are empty because of DTX period,
        // the CN generator will do the error concealment.
        iCnGenerator->GenerateSidPacket( playBuffer, dataBuffer.RequestSize() );
        iCurrentPlayThreshold = iOriginalPlayThreshold;
        
        // Used for talkburst, if talkburst over, reset buffer and sequence numbers
        if( iCInfo.iJitterBufInactivityTimeOut > 0 && iInactivityTime >= 0 )
            {
            iInactivityTime = iInactivityTime + iFrameTime;
            if( iInactivityTime >= (TInt) iCInfo.iJitterBufInactivityTimeOut )
                {
                __JITTER_BUFFER_INT2("CMccJitterBufferImpl::Inactivity timeout:",
                                     iCInfo.iJitterBufInactivityTimeOut,
                                     " time:", iInactivityTime )
                
                iInactivityTime = -1;
                ResetBuffer();
                }
            }
        }
    else 
        {
        // This is where we buffer some data before starting the playout so we can
        // take some jitter by nature without adaptation.
        if( iPacketsInBuffer < iCurrentPlayThreshold )
            {
            // Give comfort noise when we are buffering for playback start
            __JITTER_BUFFER( "CMccJitterBufferImpl::GetDataFrameL THRESHOLD NOT REACHED" )
            __JITTER_BUFFER_INT1( "CMccJitterBufferImpl::GetDataFrameL iCurrentPlayThreshold",
                iCurrentPlayThreshold )
            
            iCnGenerator->GenerateSidPacket(
                playBuffer, dataBuffer.RequestSize() );
            }
        else
            {
            // Not buffering
            HandleNotifications( EFalse );
            
            // Set the playout threshold to zero so we play the current talkspurt to the
            // end and do not affect the speech quality. This means that we will play the
            // buffer until it is empty. After that it is either DTX period or it is packet
            // loss.
            iCurrentPlayThreshold = 0;
            
            // Next sequence number that should be in the buffer
            TInt64 nextSeqNum = iLastGetSeqNum + iSeqNumIncrement;
            if( -1 == iLastGetSeqNum )
                {
                // Last get sequence number was -1, so add one
                __JITTER_BUFFER( "CMccJitterBufferImpl::GetDataFrameL LAST WAS -1 ADJUSTING" )

                nextSeqNum += 1;
                }
                
            // The actual sequence number which is in the buffer
            const TInt64 nextFrameSeqNum(
                iBuffer[ iPacketsInBuffer - 1 ].iSequenceNumber );
            
            __JITTER_BUFFER_INT1( "CMccJitterBufferImpl::GetDataFrameL NEXT_GET_SEQ_NUM:", nextSeqNum )
            __JITTER_BUFFER_INT1( "CMccJitterBufferImpl::GetDataFrameL FRAME_SEQ_NUM:", nextFrameSeqNum )
            __JITTER_BUFFER_INT1( "CMccJitterBufferImpl::GetDataFrameL PACKETS:", iPacketsInBuffer )
            __JITTER_BUFFER_INT1( "CMccJitterBufferImpl::GetDataFrameL LAST_GET_SEQ_NUM:", iLastGetSeqNum )
            
            // Check if the next sequence number is smaller than the frame number of the next
            // frame to be played out. This will mean that a frame was lost and we need to do
            // error concealment. 
            // Also we need to take account the start of the playout when iLastGetSeqNum == -1 
            // so we play the first frame in the buffer regardless of it's sequence number.
            // This should also take the sequence number wrap around situation.
            
            // As if(0) is intentional, PC-lint warning disabled:
            /*lint -e506 */
            //if ( nextSeqNum < nextFrameSeqNum )
            if ( 0 )
                {
                __JITTER_BUFFER( "CMccJitterBufferImpl::GetDataFrameL LOST FRAME" )

                iFramesLost++;
                iLastGetSeqNum = iLastGetSeqNum + iSeqNumIncrement;
                
                __JITTER_BUFFER_INT1( "CMccJitterBufferImpl::GetDataFrameL 1. LAST_GET_SEQ_NUM IS NOW:", iLastGetSeqNum )
                
                iCnGenerator->GenerateSidPacket( playBuffer,
                    dataBuffer.RequestSize() );
                }
            else
                {
                __JITTER_BUFFER( "CMccJitterBufferImpl::GetDataFrameL NORMAL" )
                
                TJitterBufferElement& bufferElement = iBuffer[ iPacketsInBuffer - 1 ];
                if ( playBuffer.MaxLength() >= bufferElement.iDataFrame->Data().Length() )
                    {
                    playBuffer.Copy( bufferElement.iDataFrame->Data() );
                    }
                else
                    {
                    __JITTER_BUFFER_INT1( "CMccJitterBufferImpl::GetDataFrameL ERROR - output buffer too small, maxlen:", 
                                          playBuffer.MaxLength() )

                    iCnGenerator->GenerateSidPacket( playBuffer, dataBuffer.RequestSize() );
                    }
                
                iLastGetSeqNum = bufferElement.iSequenceNumber;
                bufferElement.iSequenceNumber = -1;
                bufferElement.iStamp = -1;
                iPacketsInBuffer--;
                iInactivityTime = -1;
                
                // Lets see if the new buffer will lead into a DTX period
                iCnGenerator->DoDtxDecision( playBuffer );
                }
            }
        }
    }

// -----------------------------------------------------------------------------
// CMccJitterBufferImpl::FindLargestSeqNum
// Find Largest SequenceNumber
// -----------------------------------------------------------------------------
//
TInt CMccJitterBufferImpl::FindLargestSeqNum() const
    {
    TInt64 num = 0;
    TInt pos = 0;
    const TInt len( BufferLength() );
    
    for( TInt i = 0; i < len; i++ )
        {
        if( iBuffer[i].iSequenceNumber > num ) 
            {
            num = iBuffer[i].iSequenceNumber;
            pos = i;
            }
        }

    return pos;
    }
    
// -----------------------------------------------------------------------------
// CMccJitterBufferImpl::InsertBufferElement
// Insert Buffer Element
// -----------------------------------------------------------------------------
//
void CMccJitterBufferImpl::InsertBufferElement( const TDesC8& aBuffer, 
    TInt64 aLargestSeqNum, TInt64 aFrameNumber )
    {
    __JITTER_BUFFER_INT1( "CMccJitterBufferImpl::InsertBufferElement aFrameNumber:",
        aFrameNumber )
    
    if ( !aBuffer.Length() || aBuffer.Length() > iFrameSize )
        {
        __JITTER_BUFFER( "CMccJitterBufferImpl::InsertBufferElement INVALID DATA, IGNORING" )
        return;
        }
    
    const TInt len( this->BufferLength() );
    iBuffer[len-1].iDataFrame->Data().Copy( aBuffer );
    iBuffer[len-1].iSequenceNumber = aFrameNumber;
    
    if ( IsSeqNumWrappedAround( iBuffer[aLargestSeqNum].iSequenceNumber, aFrameNumber ) )
        {
        for ( TInt i = 0; i < len; i++ )
            {
            if( iBuffer[i].iStamp > aLargestSeqNum ) 
                {
                aLargestSeqNum = iBuffer[i].iStamp;
                }
            }
        
        iBuffer[len-1].iStamp = aLargestSeqNum + 1;
        iBuffer.Sort( iBufStampSorter );
        iLastGetSeqNum = -1;
        }
    else 
        {
        iBuffer[len-1].iStamp = aFrameNumber - BufferLength();
        iBuffer.Sort( iBufSequenceSorter );
        }
    
    iPacketsInBuffer++;
    }

// -----------------------------------------------------------------------------
// CMccJitterBufferImpl::BufferLength()
// Return bufferlength.
// -----------------------------------------------------------------------------
//
TInt CMccJitterBufferImpl::BufferLength() const
    {
    return iBuffer.Count();
    }

// -----------------------------------------------------------------------------
// CMccJitterBufferImpl::IsFull()
// Return:    True if full
//            False if not full
// -----------------------------------------------------------------------------
//
TBool CMccJitterBufferImpl::IsFull() const
    {
    return ( iPacketsInBuffer == BufferLength() );
    }

// -----------------------------------------------------------------------------
// CMccJitterBufferImpl::IsEmpty()
// Return:    True if empty
//            False if not empty
// -----------------------------------------------------------------------------
//
TBool CMccJitterBufferImpl::IsEmpty() const
    {
    return ( iPacketsInBuffer == 0 );    
    }

// -----------------------------------------------------------------------------
// CMccJitterBufferImpl::IsSeqNumWrappedAround
// Checks if sequence number wrapped around from aSeqNum1 to aSeqNum2
// The sequence number wrap-around condition cannot be reliably detected by 
// checking for exact numbers, i.e., aSeqNum1==KMaxSeqNumber and aSeqNum2==0, 
// because, due to packet losses in network, sequence numbers may have gaps 
// between them.
// -----------------------------------------------------------------------------
//
TBool CMccJitterBufferImpl::IsSeqNumWrappedAround( TInt64 aSeqNum1, 
    TInt64 aSeqNum2 ) const
    {
    __JITTER_BUFFER_INT1( "CMccJitterBufferImpl::IsSeqNumWrappedAround aSeqNum1:", aSeqNum1 )
    __JITTER_BUFFER_INT1( "CMccJitterBufferImpl::IsSeqNumWrappedAround aSeqNum2:", aSeqNum2 )
    
    if ( aSeqNum1 > ( KMaxSeqNumber - BufferLength() ) && 
         aSeqNum2 < BufferLength() )
        {
        __JITTER_BUFFER( "CMccJitterBufferImpl::IsSeqNumWrappedAround ETrue" )
        return ETrue;
        }
    else
        {
        __JITTER_BUFFER( "CMccJitterBufferImpl::IsSeqNumWrappedAround EFalse" )
        return EFalse;
        }
    }

// -----------------------------------------------------------------------------
// CMccJitterBufferImpl::GenerateStatistics
// Generates the statistics from the jitterbuffer
// -----------------------------------------------------------------------------
//
void CMccJitterBufferImpl::GenerateStatistics( TMccJitterBufferEventData& aEvent ) const
    {
    aEvent.iFramesReceived = iFramesReceived;
    aEvent.iBufferLength = BufferLength();
    aEvent.iFramesInBuffer = iPacketsInBuffer;
    aEvent.iFrameLoss = iFramesLost;
    aEvent.iLateFrames = iNumOfLateFrames;
    aEvent.iFramesRemoved = iFramesRemoved;
    aEvent.iFramesPlayed = iFramesPlayed;
    }

// -----------------------------------------------------------------------------
// CMccJitterBufferImpl::HandleNotifications
// -----------------------------------------------------------------------------
//
void CMccJitterBufferImpl::HandleNotifications( TBool aBuffering )
    {
    if ( aBuffering )
        {
        if ( BufferState() < EPlaying )
            {
            __JITTER_BUFFER( "CMccJitterBufferImpl::HandleNotifications, to buffering state" )
            
            SetBufferState( EBuffering );
            SendStreamEventToClient( KMccStreamBuffering, ETrue );
            }
        }
    else
        {
        if ( BufferState() >= EBuffering )
            {
            // Do not notify about streaming until we know that speaker
            // is really playing provided data. This cannot be known when
            // first buffer is sent to speaker.
            if ( BufferState() == EDetermining )
                {
                __JITTER_BUFFER( "CMccJitterBufferImpl::HandleNotifications, to playing state" )

                SetBufferState( EPlaying );
                iNotifyUser = ETrue;
                
                // As we are hiding buffer state changes when doing several
                // speaker starting attempts in row:
                // idle->buffering->streaming->idle->noresources->(buffering)->
                // (streaming)->(idle)->(noresources)->(buffering)-> etc.
                // We have to "fake" the buffering event when we are
                // sure that playback is fine.
                if ( KMccStreamBuffering != iLatestNotifiedEvent )
                    {
                    SendStreamEventToClient( KMccStreamBuffering, ETrue );
                    }
                
                SendStreamEventToClient( KMccStreamPlaying, ETrue );
                }
            else
                {
                SetBufferState( EDetermining );
                }
            }
        }
    }
  
// -----------------------------------------------------------------------------
// CMccJitterBufferImpl::CheckThresholdBufferLength
// -----------------------------------------------------------------------------
//  
void CMccJitterBufferImpl::CheckThresholdBufferLength(
    TInt& aBufferLength, TInt aTreshhold ) const
    {
    const TInt numTen = 10;
    if ( ( aBufferLength - aTreshhold ) < numTen )  
        {
        aBufferLength = aTreshhold + numTen;
        }
    }

// -----------------------------------------------------------------------------
// CMccJitterBufferImpl::SendStreamEventToClient
// -----------------------------------------------------------------------------
//	
void CMccJitterBufferImpl::SendStreamEventToClient( 
    TMccEventType aEventType,
    TBool aAllEndpoints )
    {
    __JITTER_BUFFER( "CMccJitterBufferImpl::SendStreamEventToClient" )
    
    if ( iEventHandler && iNotifyUser )
	    {
	    const TUint32 endpointId = aAllEndpoints ? 0 : iEndpointId;
	    // Clear old event
	    { 
	    iMccEvent = TMccEvent();
	    }
	    
	    iMccEvent.iEndpointId = endpointId;
	    iMccEvent.iEventCategory = KMccEventCategoryStream;
	    iMccEvent.iEventType = aEventType;

		TMccInternalEvent internalEvent( KMccJitterBufferUid, 
		                                 EMccInternalEventNone,
		                                 iMccEvent );
		                         
		iEventHandler->SendEventToClient( internalEvent );
		iLatestNotifiedEvent = aEventType;
	    }
	else
		{
		__JITTER_BUFFER( "CMccJitterBufferImpl::SendStreamEventToClient, sending ignored" )
		}
    }

// -----------------------------------------------------------------------------
// CMccJitterBufferImpl::SetBufferState
// -----------------------------------------------------------------------------
//		
void CMccJitterBufferImpl::SetBufferState( TMccJitterBufferImplState aState )
    {
    iBufferState = aState;
    }

// -----------------------------------------------------------------------------
// CMccJitterBufferImpl::BufferState
// -----------------------------------------------------------------------------
//   
CMccJitterBufferImpl::TMccJitterBufferImplState 
    CMccJitterBufferImpl::BufferState() const
    {
    return iBufferState;
    }
        
// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File 

