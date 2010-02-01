/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    AVC Payload Format Write .. Payloadizes incoming frames into RTP packets
*
*/






// INCLUDE FILES
#include <e32base.h>
#include <mmf/common/mmffourcc.h>
#include <sysutil.h>
#include "avcpayloadformatwrite.h"
#include "mccrtpdatasink.h"
#include "mccinternaldef.h"
#include "mmcccodecavc.h"
#include "avcpayloadformatlogs.h"


// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CAvcPayloadFormatWrite::CAvcPayloadFormatWrite
//
// CAvcPayloadFormatWrite default constructor, can NOT contain any code,
// that might leave
// Phase #1 of 2-phase constructor
// -----------------------------------------------------------------------------
//
CAvcPayloadFormatWrite::CAvcPayloadFormatWrite ( )
    {
    iFrameTimeInterval = 0;
    iSourceBuffer = NULL;
    }

// -----------------------------------------------------------------------------
// CAvcPayloadFormatWrite::NewL
//
// Two-phased constructor.
// Static function for creating and constructing an instance of the AVC formatter.
//
// Returns:  CAvcPayloadFormatWrite* : pointer to created instance
// -----------------------------------------------------------------------------
//
CAvcPayloadFormatWrite* CAvcPayloadFormatWrite::NewL( MDataSink* aSink )
    {
    CAvcPayloadFormatWrite* self = new ( ELeave ) CAvcPayloadFormatWrite;
    CleanupStack::PushL( self );
    self->ConstructL( aSink );
    CleanupStack::Pop();
    return self;
    }
    
// -----------------------------------------------------------------------------
// CAvcPayloadFormatWrite::ConstructL
//
// Symbian 2nd phase constructor can leave.
// Phase #2 of 2-phase constructor
// Initializes the Encoder State Machine and Encoder
// Parameters:
//  aSink				:	data sink
// -----------------------------------------------------------------------------
//
void CAvcPayloadFormatWrite::ConstructL( MDataSink* aSink )
    {
    __AVCPLFORMAT_CONTROLL("CAvcPayloadFormatWrite::ConstructL")
    
    __ASSERT_ALWAYS( aSink, User::Leave ( KErrArgument ) );
    
	// Set data sink
    iClip = aSink;
      
    iIsRtpSink = ( KMccRtpSinkUid == iClip->DataSinkType() );

    // Initialize state machine
    iStateMachine = CFormatEncodeStateMachine::NewL( this );
    iStateMachine->ChangeState( EEncodeIdle );
    iCurDataSink = aSink;
    
    iEncoder = CRFC3984Encode::NewL();
    
    __AVCPLFORMAT_CONTROLL("CAvcPayloadFormatWrite::ConstructL, exit")
    }


// -----------------------------------------------------------------------------
// CAvcPayloadFormatWrite::~CAvcPayloadFormatWrite
//
// Destructor.
// -----------------------------------------------------------------------------
//
CAvcPayloadFormatWrite::~CAvcPayloadFormatWrite ( )
    {
    __AVCPLFORMAT_CONTROLL("CAvcPayloadFormatWrite::~CAvcPayloadFormatWrite")
    
    delete iSourceBuffer;
    delete iSinkBuffer;
    delete iStateMachine;
    delete iEncoder;
    
    __AVCPLFORMAT_CONTROLL("CAvcPayloadFormatWrite::~CAvcPayloadFormatWrite, exit")
    }

// -----------------------------------------------------------------------------
// CAvcPayloadFormatWrite::SetPayloadType
// Set PayloadType
// -----------------------------------------------------------------------------
//
void CAvcPayloadFormatWrite::SetPayloadType( TUint8 aPayloadType )
    {
	iCInfo.iPayloadType = aPayloadType;
    }


// -----------------------------------------------------------------------------
// CAvcPayloadFormatWrite::FrameTimeInterval
//
// Return the frame time interval for the given media
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CAvcPayloadFormatWrite::FrameTimeInterval( TMediaId aMediaId ) const
    {
    if ( aMediaId.iMediaType == KUidMediaTypeVideo )
        {
        return iFrameTimeInterval;
        }
    else
        {
        return TTimeIntervalMicroSeconds( 0 );
        }
    }

// -----------------------------------------------------------------------------
// CAvcPayloadFormatWrite::CreateSinkBufferL
//
// Create a source buffer for the given media and indicate in aReference if buffer
// is created.
// -----------------------------------------------------------------------------
//
CMMFBuffer* CAvcPayloadFormatWrite::CreateSinkBufferL( TMediaId aMediaId, 
                                                       TBool &aReference )
    {
  
    if ( aMediaId.iMediaType != KUidMediaTypeVideo )
        {
        User::Leave( KErrNotSupported );
        }
	
    aReference = ETrue;
   	return CreateSinkBufferOfSizeL( iCInfo.iFrameSize );
    }

// -----------------------------------------------------------------------------
// CAvcPayloadFormatWrite::CreateSinkBufferOfSizeL
//
// Create a sink buffer of the given size.
// -----------------------------------------------------------------------------
//
CMMFDataBuffer* CAvcPayloadFormatWrite::CreateSinkBufferOfSizeL( TUint aSize )
    {
	// the buffer is created once and the component retains ownership in order to delete the resource

	if(NULL == iSourceBuffer)
	    {
		//needs to create source buffer
		iSourceBuffer = CMMFDataBuffer::NewL( aSize );
		iSourceBuffer->Data().FillZ( aSize );
		iSourceBuffer->SetRequestSizeL( aSize );
	    }
	
    return iSourceBuffer;
    }

// -----------------------------------------------------------------------------
// CAvcPayloadFormatWrite::SinkDataTypeCode
//
// Return the sink data type ( four CC code ) for the given media
// -----------------------------------------------------------------------------
//
TFourCC CAvcPayloadFormatWrite::SinkDataTypeCode( TMediaId aMediaId )
    {
    if ( aMediaId.iMediaType == KUidMediaTypeVideo ) 
        {
        return iCInfo.iFourCC;
        }
    else 
        {
        return TFourCC( ); //defaults to 'NULL' fourCC
        }
    }

// -----------------------------------------------------------------------------
// CAvcPayloadFormatWrite::SetSinkDataTypeCode
//
// Set the sink data type to the given four CC code for the given media
// -----------------------------------------------------------------------------
//
TInt CAvcPayloadFormatWrite::SetSinkDataTypeCode( TFourCC aSinkFourCC, 
                                                  TMediaId aMediaId )
    {
    if ( aMediaId.iMediaType != KUidMediaTypeVideo )
        {
        return KErrNotSupported;
        }
    else 
        {
        iCInfo.iFourCC = aSinkFourCC;
        }

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CAvcPayloadFormatWrite::SinkThreadLogon
//
// Passes the logon command to the sink clip
// -----------------------------------------------------------------------------
//
TInt CAvcPayloadFormatWrite::SinkThreadLogon( MAsyncEventHandler& aEventHandler )
    {
    iCurDataSink->SinkThreadLogon( aEventHandler );
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CAvcPayloadFormatWrite::SinkThreadLogoff
//
// Log out of the sink thread.
// -----------------------------------------------------------------------------
//
void CAvcPayloadFormatWrite::SinkThreadLogoff ( )
    {
    iCurDataSink->SinkThreadLogoff( );
    }

// -----------------------------------------------------------------------------
// CAvcPayloadFormatWrite::ProcessFramesL
//
// Packetize the AVC frames received from AVC codec and deliver the packets.
// The AVC frames are stored in "iSourceBuffer".
// return value - Current talk spurt finished, ETrue. Otherwise, EFalse
// -----------------------------------------------------------------------------
//
TBool CAvcPayloadFormatWrite::ProcessFramesL( )
    {
    __AVCPLFORMAT_CONTROLL("CAvcPayloadFormatWrite::ProcessFramesL")
    
    __ASSERT_ALWAYS( iSourceBuffer, User::Leave( KErrNotReady ) );
    
	TInt nalCount = 0;
    TUint32 markerBit = 0;
    TDes8& srcDes = iSourceBuffer->Data();
    HBufC8* bufPtr = NULL;
    TInt count = 0;
    TBool retVal = EFalse;
    
    if ( !iSinkBuffer )
        {
        __AVCPLFORMAT_CONTROLL(
            "CAvcPayloadFormatWrite::ProcessFramesL, sink not ready, exit")
        return retVal;
        }	
    
    // Get timestamp to RTP header. conversion to 90kHz clock
	iRtpSendHeader.iTimestamp = iSourceBuffer->TimeToPlay().Int64() / 100 * 9; 
    
    iEncoder->PayloadizeFrameL( srcDes, iRtpSendHeader.iTimestamp, markerBit, nalCount );
        
   	for ( count = 0; count < nalCount; count++ )
   	    {
   	    __AVCPLFORMAT_CONTROLL("looping")
   	    
   		// getting data
   		bufPtr = iEncoder->GetNalUnitsInOrder( count );
   		User::LeaveIfNull( bufPtr );
   		TPtr8 ptr = bufPtr->Des();
   		
   		// copying data to sink
   		TDes8& dataDes = iSinkBuffer->Data();
   		
   		dataDes.Copy( ptr.Ptr(), ptr.Length() );
   		
		// filling RTP Header structure
		
		iSinkBuffer->SetFrameNumber( iSourceBuffer->FrameNumber() + iSeqNumIncrementer );

		// Setting marker bit only for last packet if more than one NAL unit
		// for same ts, marker bit is never set for sps or pps
		if ( nalCount > 1 && ( count != nalCount-1 ) )
		    {
		    iRtpSendHeader.iMarker = 0;
		    
		    // If frame is divided in several packets, sequence numbers of following
	        // packets have to be modified
		    iSeqNumIncrementer++;
		    }
		else if ( TMccCodecInfo::IsAvcPpsOrSpsData( ptr, ETrue ) )
		    {
		    iRtpSendHeader.iMarker = 0;
		    }
	    else
	        {
	        iRtpSendHeader.iMarker = 1;
	        }
	        
		iRtpSendHeader.iPayloadType = iCInfo.iPayloadType; // as per configured
   		
   		if ( iIsRtpSink )
    	    {
        	CMccRtpDataSink* rtpSink = static_cast<CMccRtpDataSink*>( iCurDataSink );
    	    rtpSink->EmptyBufferL( iSinkBuffer, this, iMediaId, iRtpSendHeader );
    	    }
    	else
    	    {
    	    iSinkBuffer->SetTimeToPlay( iRtpSendHeader.iTimestamp );
		    iSinkBuffer->SetLastBuffer( iRtpSendHeader.iMarker );
    	    iCurDataSink->EmptyBufferL( iSinkBuffer, this, iMediaId );
    	    }
	    }
   	
   	// this clears the buffers in the encoder filled with data, necessary to do this
   	iEncoder->ClearNalBuffers();	
    
    __AVCPLFORMAT_CONTROLL("CAvcPayloadFormatWrite::ProcessFramesL, exit")
    // probably it always returns false because in TRUE case, S/M goes IDLE,
    // which is not desired in video
    return retVal;
    }

// -----------------------------------------------------------------------------
// CAvcPayloadFormatWrite::EmptyBufferL
//
// Empty the given source buffer by formatting the AVC frames into RTP payload.
// -----------------------------------------------------------------------------
//
void CAvcPayloadFormatWrite::EmptyBufferL( CMMFBuffer* aBuffer, 
                                           MDataSource* aSupplier, 
                                           TMediaId aMediaId )
    {
 
    if ( aMediaId.iMediaType != KUidMediaTypeVideo )
        {
        User::Leave( KErrNotSupported );
        }
    
    // MMF buffer must never be NULL
    __ASSERT_ALWAYS (aBuffer, User::Leave(KErrArgument));
    
    // Make sure that the very buffer is passed on, which we created once...
    // This is necessary to avoid double-deletion problems etc.
    __ASSERT_ALWAYS (aBuffer == iSourceBuffer, User::Leave(KErrArgument));
    
    // if same buffer is passed then no need to save the pointer; we already have it

    // Save source buffer parameters and change the state.
    iDataPath = aSupplier;
    iMediaId = aMediaId;
    
    // we request S/M to call our EmptySourceBufferL() function
    iStateMachine->ChangeState( EEmptySourceBuffer );

    }

// -----------------------------------------------------------------------------
// CAvcPayloadFormatWrite::EmptySourceBufferL
//
// Empty the given source buffer by formatting the AMR frames into RTP payload.
// Source buffer is given in "iSourceBuffer".
// Called by the state machine.
// -----------------------------------------------------------------------------
//
void CAvcPayloadFormatWrite::EmptySourceBufferL( )
    {
  
    if ( this->ProcessFramesL( ) )
        {
        // Current talk spurt is finished. Either last buffer
        iStateMachine->ChangeState( EEncodeIdle );
        }
    else
        {
        // Just one step in a talk spurt
        iStateMachine->ChangeState( ESourceBufferEmptied );
        }
    }

// -----------------------------------------------------------------------------
// CAvcPayloadFormatWrite::SourceBufferEmptiedL
//
// Hanlde the event that source buffer has been emptied.
// Source buffer is given in "iSourceBuffer".
// Called by the state machine.
// -----------------------------------------------------------------------------
//
void CAvcPayloadFormatWrite::SourceBufferEmptiedL( )
    {
    __ASSERT_ALWAYS( iDataPath, User::Leave( KErrNotReady ) );
    iDataPath->BufferEmptiedL( iSourceBuffer );
    }

// -----------------------------------------------------------------------------
// CAvcPayloadFormatWrite::BufferEmptiedL
//
// -----------------------------------------------------------------------------
//
void CAvcPayloadFormatWrite::BufferEmptiedL( CMMFBuffer* /*aBuffer*/ )
    {
    // We're using synchronous EmptyBufferL( aBuffer ) function call to RTP Sink.
    // The aBuffer is ready to be used again when the EmptyBufferL returns back.
    // So BufferEmptiedL( aBuffer ) is not really used.
    }

// -----------------------------------------------------------------------------
// CAvcPayloadFormatWrite::Duration
//
// Return the clip duration for the given media. Returns a big value, 
// which is not usable since this function is not being used for video.
// The return value should be ignored.
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CAvcPayloadFormatWrite::Duration( TMediaId /*aMediaType*/ ) const
    {
    return TTimeIntervalMicroSeconds( 1000000000 );
    }


// -----------------------------------------------------------------------------
// CAvcPayloadFormatWrite::CancelUlRequest
// Cancel UL Request
// -----------------------------------------------------------------------------
//
void CAvcPayloadFormatWrite::CancelUlRequest( )
    {
    __AVCPLFORMAT_CONTROLL("CAvcPayloadFormatWrite::CancelUlRequest")
     
    iStateMachine->Cancel( );
    iStateMachine->ChangeState( EEncodeIdle );

     // Reset the payload buffer
    if ( iSinkBuffer )
        {
        TDes8& dataDes = iSinkBuffer->Data();
        dataDes.SetLength( 0 );
        }
    }

// -----------------------------------------------------------------------------
// CAvcPayloadFormatWrite::SinkPrimeL( )
// Primes sink
// -----------------------------------------------------------------------------
//
void CAvcPayloadFormatWrite::SinkPrimeL( )
    {
    __AVCPLFORMAT_CONTROLL("CAvcPayloadFormatWrite::SinkPrimeL")
    
    iCurDataSink->SinkPrimeL();
    
    __AVCPLFORMAT_CONTROLL("CAvcPayloadFormatWrite::SinkPrimeL, exit")
    }

// -----------------------------------------------------------------------------
// CAvcPayloadFormatWrite::SinkPlayL( )
// Plays sink
// -----------------------------------------------------------------------------
//
void CAvcPayloadFormatWrite::SinkPlayL( )
    {
    __AVCPLFORMAT_CONTROLL("CAvcPayloadFormatWrite::SinkPlayL")
    
    // Allocate buffer for data transfer between 
    // FormatWrite - MDataSink AND FormatWrite - redundancy payload encoder
	
    delete iSinkBuffer;
    iSinkBuffer = NULL;
    iSinkBuffer = CMMFDataBuffer::NewL( iCInfo.iFrameSize );

    // Initialize payload encoder
    TDes8& dataDes = iSinkBuffer->Data();
    dataDes.SetLength( 0 );   // for first packet length
    
    iSinkBuffer->SetLastBuffer( EFalse );
    
    iCurDataSink->SinkPlayL();
    
    __AVCPLFORMAT_CONTROLL("CAvcPayloadFormatWrite::SinkPlayL, exit")
    }

// -----------------------------------------------------------------------------
// CAvcPayloadFormatWrite::SinkPauseL( )
// Pauses sink
// -----------------------------------------------------------------------------
//
void CAvcPayloadFormatWrite::SinkPauseL( )
    {
    __AVCPLFORMAT_CONTROLL("CAvcPayloadFormatWrite::SinkPauseL")
    
    iCurDataSink->SinkPauseL( );
    
    __AVCPLFORMAT_CONTROLL("CAvcPayloadFormatWrite::SinkPauseL, exit")
    }

// -----------------------------------------------------------------------------
// CAvcPayloadFormatWrite::SinkStopL( )
// Stops sink
// -----------------------------------------------------------------------------
//
void CAvcPayloadFormatWrite::SinkStopL( )
    {
    __AVCPLFORMAT_CONTROLL("CAvcPayloadFormatWrite::SinkStopL")
     
    if ( EEmptySourceBuffer == iStateMachine->CurrentState( ) )
        {
        return;
        }

    // Stop state machine
    CancelUlRequest( );

    iCurDataSink->SinkStopL( );
    
    __AVCPLFORMAT_CONTROLL("CAvcPayloadFormatWrite::SinkStopL, exit")
    }

// -----------------------------------------------------------------------------
// CAvcPayloadFormatWrite::ConfigurePayloadFormatL
// Configure payload encoding parameters.
// -----------------------------------------------------------------------------
//
void CAvcPayloadFormatWrite::ConfigurePayloadFormatL(
    const TDesC8& aConfigParams,
    CMccRtpMediaClock& /*aClock*/ )
    {
	__AVCPLFORMAT_CONTROLL("CAvcPayloadFormatWrite::ConfigurePayloadFormatL")
	
	if ( aConfigParams.Size() == sizeof( TMccCodecInfo ) )
        {
        TMccCodecInfoBuffer infoBuffer;
        infoBuffer.Copy( aConfigParams );
        iCInfo = infoBuffer();
        
        // use values passed in to set parameter variables
        __AVCPLFORMAT_CONTROLL_INT1( "packetization mode:", iCInfo.iCodecMode )
        __ASSERT_ALWAYS( iCInfo.iCodecMode == KAvcFormatModeSingleNal || 
                         iCInfo.iCodecMode == KAvcFormatModeNonInterleaved,
                         User::Leave( KErrNotSupported ) );
        
        __ASSERT_ALWAYS( iCInfo.iPayloadType < KMccPayloadTypeMax, 
                         User::Leave( KErrArgument ) );
        
        __ASSERT_ALWAYS( iCInfo.iFrameSize > 0, User::Leave( KErrArgument ) );
        __ASSERT_ALWAYS( iCInfo.iMTUSize > 0, User::Leave( KErrArgument ) );
        
        iEncoder->SetPacketizationMode( iCInfo.iCodecMode );

        iEncoder->SetFrameRate( static_cast<TInt>( iCInfo.iFramerate ) );

        iEncoder->SetMTUSize( iCInfo.iMTUSize );
        }
    else
        {
        User::Leave( KErrArgument );
    	}
    	
    __AVCPLFORMAT_CONTROLL("CAvcPayloadFormatWrite::ConfigurePayloadFormatL, exit")
    }

// -----------------------------------------------------------------------------

