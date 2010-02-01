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
* Description:    Payload format component capable to write RTP payload
*                containing AMR audio.
*
*/




// INCLUDE FILES
#include <e32base.h>
#include <mmf/common/mmffourcc.h>
#include "amrpayloadformatwrite.h"
#include "amrpayloadencoder.h"
#include "amrpayloadencoderoa.h"
#include "amrpayloaddecoder.h"
#include "amrcommonutil.h"
#include "mccrtpdatasink.h"
#include "mccuids.hrh"
#include "mccdef.h"
#include "mccinternaldef.h"
#include "amrpayloadformatter.h"
#include "mccrtpmediaclock.h" 
#include "mccredpayloadwrite.h"

const TUint KSampleRate8kHz = 8;
const TUint KSampleRate16kHz = 16;
const TReal KTimeMultiplier = 0.001;

// MACROS

#define MCC_AUDIO_TIMESTAMP( a ) \
TUint32( iFramesPerPacket * a * iChannels * \
         (iIsNb?KSampleRate8kHz:KSampleRate16kHz) * KTimeMultiplier )


// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::CAmrPayloadFormatWrite
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CAmrPayloadFormatWrite::CAmrPayloadFormatWrite( )
    : iIsNb( ETrue ), iSamplingRate( KAmrNbSampleRate ), iChannels( 1 )
    {
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatWrite::ConstructL ( MDataSink* aSink )
    {
    // Set default values
    iFramesPerPacket = 1;
    iFourCC.Set( TFourCC( ' ','A','M','R' ) );
    
    // Set data sink
    iIsRtpSink = ( KMccRtpSinkUid == aSink->DataSinkType() );
    TBool isRedEncoder 
        = ( TUid::Uid( KImplUidRedPayloadFormatEncode ) == aSink->DataSinkType() );
    
    if ( iIsRtpSink )
        {
        CMccRtpDataSink* tmp = static_cast<CMccRtpDataSink*>( aSink );
        iRtpDataSink = static_cast<MMccRtpDataSink*>( tmp );
        }
    else if ( isRedEncoder )
        {
        CMccRedPayloadWrite* tmp = static_cast<CMccRedPayloadWrite*>( aSink );
        iRtpDataSink = static_cast<MMccRtpDataSink*>( tmp );
        iIsRtpSink = ETrue;
        }
    else
        {
        AMR_PAYLOAD_FORMAT_WRITE( "CAmrPayloadFormatWrite::ConstructL, not RTP sink" )
        }
    
    iClip = aSink;
    
    // Set default set of modes
    for ( TInt i = 0; i < KNumberOfNbModes; i++ )
        {
        iModes.Append( KAmrNbModes[i] );
        }
    
    // Initialize state machine
    iStateMachine = CFormatEncodeStateMachine::NewL( this );
    iStateMachine->ChangeState( EEncodeIdle );
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CAmrPayloadFormatWrite* CAmrPayloadFormatWrite::NewL( MDataSink* aSink )
    {
    #ifdef TRACE_AMR_PAYLOAD_FORMAT_WRITE
        RDebug::Print( _L("CAmrPayloadFormatWrite::NewL") );
    #endif
    
    __ASSERT_ALWAYS( aSink, User::Leave( KErrArgument ) );

    CAmrPayloadFormatWrite* self = new ( ELeave ) CAmrPayloadFormatWrite;
    CleanupStack::PushL( self );
    self->ConstructL( aSink );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::~CAmrPayloadFormatWrite
// Destructor.
// -----------------------------------------------------------------------------
//
CAmrPayloadFormatWrite::~CAmrPayloadFormatWrite ( )
    {
    delete iSourceBuffer;
    delete iSinkBuffer;
    delete iStateMachine;
    delete iPayloadEncoder;
    iModes.Close();
    
    // Media clock is not owned
    if ( iRtpMediaClock )
        {
        iRtpMediaClock->UnregisterMediaFormat( iKey );
        }
    }

 
// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::FrameTimeInterval
// Return the frame time interval for the given media
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CAmrPayloadFormatWrite::FrameTimeInterval( 
        TMediaId aMediaId ) const
    {
    if ( KUidMediaTypeAudio == aMediaId.iMediaType )
        {
        TInt hwFrametime = static_cast<TInt>( iCInfo.iHwFrameTime );
        return TTimeIntervalMicroSeconds( TInt64( hwFrametime ) );
        }
    else
        {
        return TTimeIntervalMicroSeconds( TInt64( 0 ) );
        }
    }


// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::CreateSinkBufferL
//
// Create a source buffer for the given media and indicate in aReference if buffer
// is created.
// -----------------------------------------------------------------------------
//
CMMFBuffer* CAmrPayloadFormatWrite::CreateSinkBufferL( TMediaId aMediaId, 
                                                       TBool &aReference )
    {
    #ifdef TRACE_AMR_PAYLOAD_FORMAT_WRITE
        AMR_PAYLOAD_FORMAT_WRITE ( "CAmrPayloadFormatWrite::CreateSinkBufferL( )" );
    #endif

    if ( KUidMediaTypeAudio != aMediaId.iMediaType )
        {
        User::Leave( KErrNotSupported );
        }
    // the ownership of iSourceBuffer is in CAmrPayloadFormatWrite
    aReference = ETrue;
    return CreateSinkBufferOfSizeL( iCInfo.iFrameSize );
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::SinkDataTypeCode
// Return the sink data type ( four CC code ) for the given media
// -----------------------------------------------------------------------------
//
TFourCC CAmrPayloadFormatWrite::SinkDataTypeCode( TMediaId aMediaId )
    {
    if ( KUidMediaTypeAudio == aMediaId.iMediaType ) 
        {
         #ifdef TRACE_AMR_PAYLOAD_FORMAT_WRITE
        	AMR_PAYLOAD_FORMAT_WRITE2( 
        		"CAmrPayloadFormatWrite::SinkDataTypeCode: 0x%x", iFourCC.FourCC() );
        #endif
        return iFourCC;
        }
    else 
        {
        return TFourCC( ); //defaults to 'NULL' fourCC
        }
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::SetSinkDataTypeCode
// Set the sink data type to the given four CC code for the given media
// -----------------------------------------------------------------------------
//
TInt CAmrPayloadFormatWrite::SetSinkDataTypeCode( TFourCC aSinkFourCC, 
                                                  TMediaId aMediaId )
    {
    #ifdef TRACE_AMR_PAYLOAD_FORMAT_WRITE
        	AMR_PAYLOAD_FORMAT_WRITE2( 
        		"CAmrPayloadFormatWrite::SetSinkDataTypeCode: 0x%x", 
        		aSinkFourCC.FourCC() );
    #endif
    if ( KUidMediaTypeAudio != aMediaId.iMediaType )
        {
        return KErrNotSupported;
        }
    else if ( aSinkFourCC == KMccFourCCIdAMRNB )
        {
        iFourCC = aSinkFourCC;
        }
    else if ( aSinkFourCC == KMccFourCCIdAMRWB )
        {
        iFourCC = aSinkFourCC;
        // Nb modes were set in the construtor. We need to replace them with wb modes.
        iIsNb = EFalse; 
        iModes.Reset();
	    for ( TInt i = 0; i < KNumberOfWbModes; i++ )
	        {
	        iModes.Append( KAmrWbModes[i] );
	        }
	    SetSampleRate( KAmrWbSampleRate );
        }
    else
        {
        return KErrNotSupported;
        }

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::SinkThreadLogon
// Passes the logon command to the sink clip
// -----------------------------------------------------------------------------
//
TInt CAmrPayloadFormatWrite::SinkThreadLogon( MAsyncEventHandler& aEventHandler )
    {
    iClip->SinkThreadLogon( aEventHandler );
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::SinkThreadLogoff
// Log out of the sink thread.
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatWrite::SinkThreadLogoff( )
    {
    #ifdef TRACE_AMR_PAYLOAD_FORMAT_WRITE
        AMR_PAYLOAD_FORMAT_WRITE( "CAmrPayloadFormatWrite::SinkThreadLogoff " );
    #endif   
    
    iClip->SinkThreadLogoff( );
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::ProcessFramesL
// Packetize the AMR frames received from AMR codec and deliver the packets.
// The AMR frames are stored in "iSourceBuffer".
// return value - Current talk spurt finished, ETrue. Otherwise, EFalse.
// -----------------------------------------------------------------------------
//
TBool CAmrPayloadFormatWrite::ProcessFramesL()
    {
    #ifdef TRACE_AMR_PAYLOAD_FORMAT_WRITE
        AMR_PAYLOAD_FORMAT_WRITE ( "CAmrPayloadFormatWrite::ProcessFramesL( ) Begin" );
    #endif

    TBool payloadReady( EFalse );
    TBool isDone( EFalse );
    TAmrFrameInfo frameInfo;

    CMMFBuffer* buffer = iSourceBuffer;
    TDes8& srcDes = ( static_cast<CMMFDataBuffer*>( buffer )->Data() );
    TInt readLen = srcDes.Length();
    TInt byteIndex = 0;
    TInt seekLen = readLen - byteIndex;

    const TUint8* seekPtr = srcDes.Ptr() + byteIndex;

    while ( seekLen >= CAmrCommonUtility::FrameHeaderSize() )
        {
        // Scan frame by frame until free format frame or EOB detected.
        TInt length = CAmrCommonUtility::FrameInfo( seekPtr, seekLen, frameInfo, iIsNb );
        if ( length > 0 && frameInfo.iBitrate > 0 ) // got a frame
            {
            TStreamDecoder decoder;
            decoder.Initialize( seekPtr, 0, 0 );
            decoder.Decode( 1 );                              // Skip first Padding bit
            TInt frameType( decoder.Decode( KNumValue4 ) ); // FT value
            TUint8 qualityInd = TUint8( decoder.Decode( 1 ) );// QualityInd value

            // Encode a frame, speech data starts from the second byte in a frame
            TPtr8 dataPtr( const_cast<TUint8*>( seekPtr+1 ), seekLen - 1, seekLen - 1 );
            
            #ifdef TRACE_AMR_PAYLOAD_FORMAT_WRITE
                RDebug::Print( _L("CAmrPayloadFormatWrite::ProcessFramesL frameType: %d"), frameType );
            #endif
            
            // frameType 8 (SID) or 15 (NO_DATA) tells us that we can set the 
            // CMR to NO_REQUEST, otherwise we can keep the CMR in which ever
            // the frameType is as they map directly to each other.
            // NOTE: There is no clear information on how to set the CMR field
            // regarding SID or NO_DATA frames. So there might be a possibility
            // that we should keep the original CMR also for SID and NO_DATA 
            // frames.
            
            // In the old implementation, the iPayloadEncoder used always the
            // same (moderequest for 12.2kbps) but this should be the correct
            // way of things.
            TAmrModeRequest modeReq;
            if ( ( iIsNb && ( frameType == EAmrFrameNoData || frameType == EAmrFrameSid ) ) ||
                 ( !iIsNb && ( frameType == EAmrWbFrameNoData || frameType == EAmrWbFrameSid  ) ) )
                {
                #ifdef TRACE_AMR_PAYLOAD_FORMAT_WRITE
                    RDebug::Print( _L("CAmrPayloadFormatWrite::ProcessFramesL CMR NO_REQ") );
                #endif
            
                modeReq = EAmrModeReqNone;
                }
            else
                {
                #ifdef TRACE_AMR_PAYLOAD_FORMAT_WRITE
                    RDebug::Print( _L("CAmrPayloadFormatWrite::ProcessFramesL CMR: %d"), frameType );
                #endif

                modeReq = static_cast<TAmrModeRequest>( frameType );
                }
                
            iPayloadEncoder->SetModeRequest( modeReq );
            payloadReady = iPayloadEncoder->EncodeFrame( TInt( iCurrentChannel ),
                    TAmrFrameType( frameType ), qualityInd, dataPtr );
                    
     		#ifdef TRACE_AMR_PAYLOAD_FORMAT_WRITE
        		AMR_PAYLOAD_FORMAT_WRITE2 ( "CAmrPayloadFormatWrite::ProcessFramesL( ): payloadReady1 = %d ", payloadReady );
    		#endif

            // Increase current channel
            iCurrentChannel++;
            if ( iCurrentChannel == iChannels )
                {
                // All channel frames for a frame block are received
                iCurrentChannel = 0;
                }

            seekPtr += length; // update byte positions
            seekLen -= length; // update seek length
            byteIndex += length;
            
            iFramesEncoded++;
            }
        else
            {
            if ( frameInfo.iBitrate < 0 )
                {
                #ifdef TRACE_AMR_PAYLOAD_FORMAT_WRITE
        		AMR_PAYLOAD_FORMAT_WRITE ( "CAmrPayloadFormatWrite::ProcessFramesL( ): frameInfo.iBitrate < 0 ");
    			#endif
                break;
                }
            else
                {
                //  No data frame
                TStreamDecoder decoder;
                decoder.Initialize( seekPtr, 0, 0 );
                decoder.Decode( 1 );                        // Skip first Padding bit
                TInt frameType = decoder.Decode( KNumValue4 );       // FT value

                if ( frameType != TInt( EAmrFrameNoData ) ) // bad frame
                    {
                    #ifdef TRACE_AMR_PAYLOAD_FORMAT_WRITE
        				AMR_PAYLOAD_FORMAT_WRITE ( "CAmrPayloadFormatWrite::ProcessFramesL( ): bad frame ");
    				#endif
                    break;
                    }

                // QualityIndicator value
                TUint8 qualityInd = TUint8( decoder.Decode( 1 ) );
                TPtr8 dummy( 0,0,0 );
                payloadReady = iPayloadEncoder->EncodeFrame( TInt( iCurrentChannel ),
                            TAmrFrameType( frameType ), qualityInd, dummy );
                            
                #ifdef TRACE_AMR_PAYLOAD_FORMAT_WRITE
        				AMR_PAYLOAD_FORMAT_WRITE2 ( "CAmrPayloadFormatWrite::ProcessFramesL( ): payloadReady2 = %d ", payloadReady);
    			#endif   
                // Increase current channel
                iCurrentChannel++;
                if ( iCurrentChannel == iChannels )
                    {
                    // All channel frames for a frame block are received
                    iCurrentChannel = 0;
                    }

                seekPtr++;
                seekLen--;
                byteIndex++;

                iFramesEncoded++;
                }
            }

        if ( payloadReady )
            {
            AMR_PAYLOAD_FORMAT_WRITE2( "CAmrPayloadFormatWrite::ProcessFramesL: iSinkBuffer1 = %X ", iSinkBuffer );
            AMR_PAYLOAD_FORMAT_WRITE2( "CAmrPayloadFormatWrite::ProcessFramesL: iSourceBuffer = %X ", iSourceBuffer );
    	    
    	    if ( !iSinkBuffer )
                {
                return EFalse;
                }
                
            // Payload has been constructed; deliver it to RTP datasink
            // Update length info of payload buffer.
            if ( seekLen < CAmrCommonUtility::FrameHeaderSize( ) 
                &&  ( iSourceBuffer->LastBuffer( ) || iNeedToFlush ) )
                {
                // No more AMR frames
                iSinkBuffer->SetLastBuffer( ETrue );    
                isDone = ETrue;
                }

            TDes8& dataDes = static_cast<CMMFDataBuffer*>( iSinkBuffer )->Data( );
            TDesC8& payloadDes = iPayloadEncoder->PayloadBuffer( );
            dataDes.SetLength( payloadDes.Length( ) );
            DeliverPacketL( *iSinkBuffer );
            }            
        }

    // If this is the last buffer, 
    // Set from datapath or by AmrPayloadFormatWrite itself
    if ( iSourceBuffer->LastBuffer( ) || iNeedToFlush ) 
        {
        AMR_PAYLOAD_FORMAT_WRITE2( "CAmrPayloadFormatWrite::ProcessFramesL: iSinkBuffer2 = %X ", iSinkBuffer );
        
        isDone = ETrue;
        if ( iPayloadEncoder->ReEncodeFrameL( ) )
            {
            // added for last buffer  
            // length can not be zero.
            if ( !iSinkBuffer )
                {
                return EFalse;
                }

            TDes8& dataDes = static_cast<CMMFDataBuffer*>( iSinkBuffer )->Data( );
            TDesC8& payloadDes = iPayloadEncoder->PayloadBuffer( );
            dataDes.SetLength( payloadDes.Length( ) );
            iSinkBuffer->SetLastBuffer( ETrue );   
            DeliverPacketL( *iSinkBuffer );
            }
        }

    return isDone;
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::DeliverPacketL
//
// Prepare packet header and deliver the packet to the datasink.
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatWrite::DeliverPacketL( CMMFDataBuffer& aPayload )
    {
    AMR_PAYLOAD_FORMAT_WRITE2( "CAmrPayloadFormatWrite::DeliverPacketL - TSTAMP: %u",
        static_cast<TUint32>( aPayload.TimeToPlay().Int64() ) );
    
    // Construct RTP header.
    if ( !iFirstPacketFinished )
        {
        iRtpSendHeader.iMarker = 1;
        iFirstPacketFinished = ETrue;
        }
    else
        {
        iRtpSendHeader.iMarker = 0;
        }
    
    iRtpSendHeader.iPayloadType = iCInfo.iPayloadType;
    iRtpSendHeader.iTimestamp 
        = static_cast<TUint32>( aPayload.TimeToPlay().Int64() );
    
    if ( aPayload.BufferSize() )
        {  
        if ( iIsRtpSink )
            {
            iRtpDataSink->EmptyBufferL( &aPayload, this, iMediaId, iRtpSendHeader );
            }
        else
            {
		    aPayload.SetLastBuffer( iRtpSendHeader.iMarker );
            iClip->EmptyBufferL( &aPayload, this, iMediaId );
            }
        }
    
    // Reset the payload buffer as previous EmptyBufferL() is a synchronous call.
    aPayload.Data().Zero();
    iPayloadEncoder->ResetPayloadBuffer();
    }


// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::EmptyBufferL
// Empty the given source buffer by formatting the AMR frames into RTP payload.
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatWrite::EmptyBufferL( CMMFBuffer* aBuffer, 
                                           MDataSource* aSupplier, 
                                           TMediaId aMediaId )
    {
    AMR_PAYLOAD_FORMAT_WRITE2( "CAmrPayloadFormatWrite::EmptyBufferL aBuffer = 0x%x", aBuffer );
    
    __ASSERT_ALWAYS( aBuffer, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( aBuffer == iSourceBuffer, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( aSupplier, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( KUidMediaTypeAudio == aMediaId.iMediaType, 
        User::Leave( KErrNotSupported ) );
    
    // Save source buffer parameters and change the state.
    iDataPath = aSupplier;
    iMediaId = aMediaId;
    iSourceBuffer = static_cast<CMMFDataBuffer*>( aBuffer );
    
    TUint32 ts = 0;
    if ( iCInfo.iSourceDefinedTimeStamps )
        {
        AMR_PAYLOAD_FORMAT_WRITE( "CAmrPayloadFormatWrite::EmptyBufferL source has defined timestamp" );
        
        ts = MCC_AUDIO_TIMESTAMP( iSourceBuffer->TimeToPlay().Int64() );
        iSinkBuffer->SetTimeToPlay( TInt64( ts ) );
        iSinkBuffer->SetFrameNumber( aBuffer->FrameNumber() );
        }
    else if ( iPayloadEncoder->IsStartOfPeriod() )
        {
        AMR_PAYLOAD_FORMAT_WRITE( "CAmrPayloadFormatWrite::EmptyBufferL SetTimeToPlay" );
        
        User::LeaveIfError( iRtpMediaClock->GetTimeStamp( iKey, ts ) );

        if ( iRtpMediaClock->TimeBasedIncrement() )
	    	{
    		iFirstPacketFinished = EFalse;
    		}
        
        iSinkBuffer->SetTimeToPlay( TInt64( ts ) );
        iSinkBuffer->SetFrameNumber( aBuffer->FrameNumber() );
        }
    else if( !iCInfo.iSourceDefinedTimeStamps &&
             !iPayloadEncoder->IsStartOfPeriod() )
        {
        User::LeaveIfError( iRtpMediaClock->GetTimeStamp( iKey, ts ) );
        
        if ( iRtpMediaClock->TimeBasedIncrement() )
	    	{
    		iFirstPacketFinished = EFalse;
    		}
        
        }
    else
        {
        // nop
        }
    
    iStateMachine->ChangeState( EEmptySourceBuffer );
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::EmptySourceBufferL
// Empty the given source buffer by formatting the AMR frames into RTP payload.
// Source buffer is given in "iSourceBuffer".
// Called by the state machine.
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatWrite::EmptySourceBufferL( )
    {
    AMR_PAYLOAD_FORMAT_WRITE2( "CAmrPayloadFormatWrite::EmptySourceBufferL SRCBUFLEN:%d", iSourceBuffer->Data().Length() );

    if ( ProcessFramesL() )
        {
        // Current talk spurt is finished. Either last buffer or iNeedToFlush
        iStateMachine->ChangeState( EEncodeIdle );
        iClip->SinkStopL();
        }
    else
        {
        // Just one step in a talk spurt
        iStateMachine->ChangeState( ESourceBufferEmptied );
        }
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::SourceBufferEmptiedL
//
// Hanlde the event that source buffer has been emptied.
// Source buffer is given in "iSourceBuffer".
// Called by the state machine.
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatWrite::SourceBufferEmptiedL()
    {
    AMR_PAYLOAD_FORMAT_WRITE ( "CAmrPayloadFormatWrite::SourceBufferEmptiedL" );

    iDataPath->BufferEmptiedL( iSourceBuffer );
    }


// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::BufferEmptiedL
// Called after payload buffer is completely emptied by RtpDataSink.
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatWrite::BufferEmptiedL( CMMFBuffer* /*aBuffer*/ )
    {
    /* 
    * We're using synchronous EmptyBufferL( aBuffer ) function call to RTP Sink.
    * The aBuffer is ready to be used again when the EmptyBufferL returns back.
    * So BufferEmptiedL( aBuffer ) is not really used.
    */
    }


// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::NumChannels
// Get the number of channels
// -----------------------------------------------------------------------------
//
TUint CAmrPayloadFormatWrite::NumChannels( )
    {
    return KMono;
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::SampleRate
// Returns the samplerate
// -----------------------------------------------------------------------------
//
TUint CAmrPayloadFormatWrite::SampleRate( )
    {
    return iSamplingRate;
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::SetSampleRate
// Sets the sample rate, AMR-NB is always 8000Hz
// -----------------------------------------------------------------------------
//
TInt CAmrPayloadFormatWrite::SetSampleRate ( TUint aSampleRate )
    {
    if( (iIsNb && KAmrNbSampleRate != aSampleRate) ||
        (!iIsNb && KAmrWbSampleRate != aSampleRate) )
        {
        return KErrNotSupported;
        }
    else
        {
        iSamplingRate = aSampleRate;
        return KErrNone;
        }
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::Duration
// Return the clip duration for the given media.
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CAmrPayloadFormatWrite::Duration( 
        TMediaId /*aMediaType*/ ) const
    {
    return TTimeIntervalMicroSeconds( TInt64( 0 ) );
    }


// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::CreateSinkBufferOfSizeL
// Create a sink buffer of the given size.
// -----------------------------------------------------------------------------
//
CMMFDataBuffer* CAmrPayloadFormatWrite::CreateSinkBufferOfSizeL( TUint aSize )
    {
    AMR_PAYLOAD_FORMAT_WRITE ( "CAmrPayloadFormatWrite::CreateSinkBufferOfSizeL" );

    if ( !iSourceBuffer )
	    {  
	    iSourceBuffer = CMMFDataBuffer::NewL( aSize );
    	iSourceBuffer->Data().FillZ( aSize );
    	iSourceBuffer->SetRequestSizeL( aSize );
	    }
    return iSourceBuffer;
    }


// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::IsBitrateChangeValid
// Checks if codec mode change request is valid.
// -----------------------------------------------------------------------------
//
TBool CAmrPayloadFormatWrite::IsBitrateChangeValid( TInt aBitRate ) 
    {
    AMR_PAYLOAD_FORMAT_WRITE( "CAmrPayloadFormatWrite::IsBitrateChangeValid" );
    
    TInt currentModeIndex = iModes.Find( iCInfo.iBitrate ); 
    TInt desiredModeIndex = iModes.Find( aBitRate );  
    
    TBool modechangeperiodvalid( EFalse );
    
    if ( iFirstCmrHandled )
        {
        modechangeperiodvalid = !TBool( iFramesEncoded % iCInfo.iModeChangePeriod );
        }
    else
        {
        // Initialize counter
        iFramesEncoded = 0;
        }
     
    if ( KErrNotFound == currentModeIndex )            
        {                        
        if ( ( modechangeperiodvalid || !iFirstCmrHandled ) &&
            ( KErrNotFound != desiredModeIndex ) )        
            {                                         
            iCInfo.iBitrate = aBitRate;
            iFirstCmrHandled = ETrue;
            return ETrue;
            }
        else                                          
            {
            return EFalse;        
            }            
        }
         
    if ( ( modechangeperiodvalid || !iFirstCmrHandled ) && ( ( ( iCInfo.iNeighbor ) &&
        ( desiredModeIndex == currentModeIndex + 1 ) ||
        ( desiredModeIndex == currentModeIndex - 1 && currentModeIndex != 0 ) ) ||
        ( !iCInfo.iNeighbor && KErrNotFound != desiredModeIndex  ) ) )
        {
        iCInfo.iBitrate = aBitRate;
        iFirstCmrHandled = ETrue;
        return ETrue;
        }
    else
        {
        return EFalse;
        }               
    }
    
    
// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::CancelUlRequest
// Cancel UL Request
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatWrite::CancelUlRequest( )
    {
    AMR_PAYLOAD_FORMAT_WRITE( "CAmrPayloadFormatWrite::CancelUlRequest" );

    iStateMachine->Cancel();
    iStateMachine->ChangeState( EEncodeIdle );

    // Reset the payload buffer
    if ( iSinkBuffer )
        {
        TDes8& dataDes = static_cast<CMMFDataBuffer*>( iSinkBuffer )->Data();
        dataDes.SetLength( 0 );
        // Reset iFrameEncoder
        iPayloadEncoder->InitializeFrameEncoder();
        iPayloadEncoder->ResetPayloadBuffer();
        }
    };

// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::SinkPrimeL()
// Primes sink
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatWrite::SinkPrimeL()
    {
    iClip->SinkPrimeL();
    };

// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::SinkPlayL()
// Plays sink
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatWrite::SinkPlayL()
    {
    AMR_PAYLOAD_FORMAT_WRITE2( "CAmrPayloadFormatWrite::SinkPlayL RedCount: %d", iCInfo.iRedundancyCount );

    // Allocate buffer for data transfer between
    // FormatWrite - MDataSink AND FormatWrite - redundancy payload encoder
    delete iSinkBuffer;
    iSinkBuffer = NULL;
    iSinkBuffer = CMMFDataBuffer::NewL( iCInfo.iFrameSize * iFramesPerPacket
        * ( 1 + iCInfo.iRedundancyCount ) + KNumValue2 );

    // Initialize payload encoder
    iPayloadEncoder->Initialize();
    TDes8& dataDes = static_cast<CMMFDataBuffer*>( iSinkBuffer )->Data( );
    dataDes.SetLength( 0 );   // for first packet length
    iPayloadEncoder->SetPayloadBuffer( dataDes );
	iPayloadEncoder->SetChannelCount( iChannels );

    // Reset flag
    iNeedToFlush = EFalse;

    // Start state machine
    iStateMachine->ChangeState( EEncodeIdle );
    iFirstPacketFinished = EFalse;

    if ( iSinkBuffer )
        {
        iSinkBuffer->SetLastBuffer( EFalse );
        }

    iClip->SinkPlayL();
    };

// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::SinkPauseL()
// Pauses sink
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatWrite::SinkPauseL()
    {
    this->CancelUlRequest();
    iClip->SinkPauseL();
    };

// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::SinkStopL()
// Stops sink
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatWrite::SinkStopL()
    {
    if ( EEmptySourceBuffer == iStateMachine->CurrentState() )
        {
        iNeedToFlush = ETrue; 
        return;
        }

    if ( ESourceBufferEmptied == iStateMachine->CurrentState() )
        {
        // Finished emptying current buffer to RTP
        // check whether there is internal buffer left
        if ( iPayloadEncoder->ReEncodeFrameL() )
            {
            // length can not be zero.
            if ( !iSinkBuffer )
                {
                return;
                }
                
            TDes8& dataDes = static_cast<CMMFDataBuffer*>( iSinkBuffer )->Data();
            TDesC8& payloadDes = iPayloadEncoder->PayloadBuffer();
            dataDes.SetLength( payloadDes.Length() );
            iSinkBuffer->SetLastBuffer( ETrue );

            this->DeliverPacketL( *iSinkBuffer );

            AMR_PAYLOAD_FORMAT_WRITE( "CAmrPayloadFormatWrite::StopUlStream ReEncodeFrame done" );
            }
        }

    // Stop state machine
    this->CancelUlRequest();
    iClip->SinkStopL();
    };

// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::ConfigurePayloadFormatL
// Configure payload encoding parameters.
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatWrite::ConfigurePayloadFormatL( 
        const TDesC8& aConfigParams, 
        CMccRtpMediaClock& aClock  )
    {
    AMR_PAYLOAD_FORMAT_WRITE( "CAmrPayloadFormatWrite::ConfigurePayloadFormatL (NB/WB)" )
    __ASSERT_ALWAYS( aConfigParams.Size() == sizeof( TMccCodecInfo ),
        User::Leave( KErrArgument ) );
    
    TMccCodecInfoBuffer infoBuffer;
    infoBuffer.Copy( aConfigParams );  
        
    if ( !infoBuffer().iIsUpdate )
        {
        __ASSERT_ALWAYS( infoBuffer().iHwFrameTime, User::Leave( KErrArgument ) );
        __ASSERT_ALWAYS( infoBuffer().iModeChangePeriod, User::Leave( KErrArgument ) );
        
        iCInfo = infoBuffer();
        iRtpMediaClock = &aClock;

        TUint tempMask(0x0001);
        // Reset old iModes array if new modes exist
        if ( iCInfo.iBitrateMask != 0x0000 )
            {
            iModes.Reset();
            }
        
        if ( iIsNb )
        	{
        	iKey = iRtpMediaClock->RegisterMediaFormat( KAmrNbSampleRate,
        	                                            iCInfo.iHwFrameTime );

	        for ( TInt ii = KNumValue0; ii < KNumberOfNbModes; ii++ )
	            {
	            if( iCInfo.iBitrateMask & tempMask )
	                {
	                iModes.AppendL( KAmrNbModes[ii] );
	                }
	            tempMask*=2;
	            }           
        	}
        else // Wb
        	{
        	iKey = iRtpMediaClock->RegisterMediaFormat( KAmrWbSampleRate, 
        	                                            iCInfo.iHwFrameTime );

	        for ( TInt ii = 0; ii < KNumberOfWbModes; ii++ )
	            {
	            if( iCInfo.iBitrateMask & tempMask )
	                {
	                iModes.AppendL( KAmrWbModes[ii] );
	                }
	            tempMask*=2;
	            }           
        	}
                                    
        if ( !iPayloadEncoder )
            {
            if( KAmrCodecModeBandwidthEfficient == iCInfo.iCodecMode )
                {
                iPayloadEncoder = CAmrPayloadEncoder::NewL( iIsNb );
                }
            else if( KAmrCodecModeOctetAlign == iCInfo.iCodecMode )
                {
                iPayloadEncoder = CAmrPayloadEncoderOA::NewL( iIsNb );
                }
            else
                {
                User::Leave( KErrArgument );
                }    
            }
        
        iFramesPerPacket = iCInfo.iPtime / iCInfo.iHwFrameTime;
        iPayloadEncoder->SetFrameBlockCount( iFramesPerPacket );
  
        if ( EAmrFecUsed == iCInfo.iAlgoUsed && iCInfo.iRedundancyCount > 0 )
            {
            AMR_PAYLOAD_FORMAT_WRITE( "CAmrPayloadFormatWrite::ConfigurePayloadFormatL - Using AMR FEC" );
            iPayloadEncoder->Initialize();
            User::LeaveIfError( iPayloadEncoder
                ->SetRedFrameBlockCount( iCInfo.iRedundancyCount ) );
            }
        else if ( EGenRedUsed == iCInfo.iAlgoUsed )
            {
            AMR_PAYLOAD_FORMAT_WRITE2( "CAmrPayloadFormatWrite::ConfigurePayloadFormatL, RED LEVEL: %d",
                iCInfo.iRedundancyCount );
            
            CPayloadFormatWrite* redEncoder
                = static_cast<CMccRedPayloadWrite*>( iClip );
            
            TMccRedPayloadWriteConfig config;
            config.iRedBlockCount = iCInfo.iRedundancyCount;
            config.iMaxPayloadSize = iCInfo.iFrameSize * iFramesPerPacket;
            config.iNumOfEncodings = 1;
            config.iRedPayloadType = iCInfo.iRedundantPayload;
            config.InitPayloadTypes();
            config.iEncPayloadTypes[0] = iCInfo.iPayloadType;
            TMccRedPayloadWritePckg pckg( config );
            redEncoder->ConfigurePayloadFormatL( pckg, *iRtpMediaClock );
            }
        else
            {
            // NOP
            }
        }
    else
        {
        UpdateConfigurationL( infoBuffer() );            
        } 
               
    AMR_PAYLOAD_FORMAT_WRITE( "CAmrPayloadFormatWrite::ConfigurePayloadFormatL OUT" );
    }


// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::UpdateConfigurationL
// Update payload encoding parameters
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatWrite::UpdateConfigurationL( const TMccCodecInfo& aCodecInfo )
    {
    AMR_PAYLOAD_FORMAT_WRITE( "CAmrPayloadFormatWrite::UpdateConfigurationL" )
    
    __ASSERT_ALWAYS( aCodecInfo.iModeChangePeriod, User::Leave( KErrArgument ) );
    
    iCInfo.iNeighbor = aCodecInfo.iNeighbor;                            
    iCInfo.iModeChangePeriod = aCodecInfo.iModeChangePeriod;          
    iCInfo.iRedundancyCount = aCodecInfo.iRedundancyCount;
    iCInfo.iPtime = aCodecInfo.iPtime;
    iCInfo.iSourceDefinedTimeStamps = aCodecInfo.iSourceDefinedTimeStamps;
    iCInfo.iPayloadType = aCodecInfo.iPayloadType;
    
    TUint tempMask(0x0001);
    
    // Reset old iModes array if new modes exist
    if ( iCInfo.iBitrateMask != 0x0000 )
        {
        iModes.Reset();
        }
    
    if ( iIsNb )
    	{
	    for ( TInt ii = 0; ii < KNumberOfNbModes; ii++ )
	        {
	        if( iCInfo.iBitrateMask & tempMask )
	            {
	            iModes.AppendL( KAmrNbModes[ii] );
	            }
	        tempMask*=2;
	        } 
    	}
	else // Wb
		{
	    for ( TInt ii = 0; ii < KNumberOfWbModes; ii++ )
	        {
	        if( iCInfo.iBitrateMask & tempMask )
	            {
	            iModes.AppendL( KAmrWbModes[ii] );
	            }
	        tempMask*=2;
	        } 
		}	        
    
    iFramesPerPacket = iCInfo.iPtime / iCInfo.iHwFrameTime;
    iPayloadEncoder->SetFrameBlockCount( iFramesPerPacket );
    
    if ( EAmrFecUsed == iCInfo.iAlgoUsed )
        {                           
        iPayloadEncoder->Initialize();
        User::LeaveIfError( iPayloadEncoder
            ->SetRedFrameBlockCount( iCInfo.iRedundancyCount ) );
        }
    else
        {
        AMR_PAYLOAD_FORMAT_WRITE( "CAmrPayloadFormatWrite::ConfigurePayloadFormatL - Not using AMR FEC" );
        }
    
    // Allocate buffer for data transfer between
    // FormatWrite - MDataSink AND FormatWrite - redundancy payload encoder
    delete iSinkBuffer;
    iSinkBuffer = NULL;
    iSinkBuffer = CMMFDataBuffer::NewL( iCInfo.iFrameSize * iFramesPerPacket
        * ( 1 + iCInfo.iRedundancyCount ) + KNumValue2 );
    
    iPayloadEncoder->Initialize();
    TDes8& dataDes = static_cast<CMMFDataBuffer*>( iSinkBuffer )->Data( );
    dataDes.SetLength( 0 );   // for first packet length
    iPayloadEncoder->SetPayloadBuffer( dataDes );
    iPayloadEncoder->SetChannelCount( iChannels );
    
    // Reset flag
    iNeedToFlush = EFalse;
    
    AMR_PAYLOAD_FORMAT_WRITE( "CAmrPayloadFormatWrite::UpdateConfigurationL OUT" )
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File
