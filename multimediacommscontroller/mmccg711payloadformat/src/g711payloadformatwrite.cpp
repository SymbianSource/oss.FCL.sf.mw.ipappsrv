/*
* Copyright (c) 2004-2008 Nokia Corporation and/or its subsidiary(-ies).
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
*                containing G711 audio.
*
*/




// INCLUDE FILES
#include "rtpheader.h"
#include    "g711payloadformatwrite.h"
#include    "mccrtpdatasink.h"
#include    "g711payloadformatdefs.h"
#include    "mccuids.hrh"
#include    "mccdef.h"
#include    "mccrtpmediaclock.h"
#include    "mccredpayloadwrite.h"
#include    "mccinternaldef.h"

// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CG711PayloadFormatRead::CG711PayloadFormatRead
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CG711PayloadFormatWrite::CG711PayloadFormatWrite()
    {
    }
    
// -----------------------------------------------------------------------------
// CG711PayloadFormatWrite::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CG711PayloadFormatWrite::ConstructL ( MDataSink* aSink )
    {
    DP_G711_WRITE( "CG711PayloadFormatWrite::ConstructL()" );
    
    __ASSERT_ALWAYS( aSink, User::Leave( KErrArgument ) );
    
    // Set default values
    iFourCC.Set( KMccFourCCIdG711 );
    
    iIsRtpSink = ( KMccRtpSinkUid  == aSink->DataSinkType() );
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
        DP_G711_WRITE( "CG711PayloadFormatWrite::ConstructL, sink not RTP one" );
        }
    
    iClip = aSink;
    
    // Initialize state machine
    iStateMachine = CFormatEncodeStateMachine::NewL( this );
    iStateMachine->ChangeState( EEncodeIdle );
    }
    
// -----------------------------------------------------------------------------
// CG711PayloadFormatWrite::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CG711PayloadFormatWrite* CG711PayloadFormatWrite::NewL ( MDataSink* aSink )
    {
    DP_G711_WRITE( "CG711PayloadFormatWrite::NewL()" );
    
    __ASSERT_ALWAYS( aSink, User::Leave( KErrArgument ) );
    
    CG711PayloadFormatWrite* self = new (ELeave) CG711PayloadFormatWrite;
    CleanupStack::PushL( self );
    self->ConstructL( aSink );
    CleanupStack::Pop( self );
    return self;
    }

// Destructor
CG711PayloadFormatWrite::~CG711PayloadFormatWrite ()
    {
    DP_G711_WRITE( "CG711PayloadFormatWrite::~CG711PayloadFormatWrite()" );
    if ( iStateMachine )
        {
        iStateMachine->Cancel();
        }
    
    // Media clock is not owned
    if ( iRtpMediaClock )
        {
        iRtpMediaClock->UnregisterMediaFormat( iKey );
        }
    
    delete iSourceBuffer;
    delete iSinkBuffer;
    delete iStateMachine;
    }

// -----------------------------------------------------------------------------
// CG711PayloadFormatWrite::SinkThreadLogon
// Passes the logon command to the sink clip
// -----------------------------------------------------------------------------
//
TInt CG711PayloadFormatWrite::SinkThreadLogon(MAsyncEventHandler& aEventHandler)
    {
    iClip->SinkThreadLogon( aEventHandler );
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CG711PayloadFormatWrite::SinkThreadLogoff
// Passes the logoff command to the sink clip
// -----------------------------------------------------------------------------
void CG711PayloadFormatWrite::SinkThreadLogoff()
    {
    iClip->SinkThreadLogoff();
    }

// -----------------------------------------------------------------------------
// CG711PayloadFormatWrite::CreateSinkBufferL
// Create a sink buffer
// -----------------------------------------------------------------------------
//
CMMFBuffer* CG711PayloadFormatWrite::CreateSinkBufferL( TMediaId aMediaId,
                                                        TBool &aReference )
    {
    DP_G711_WRITE( "CG711PayloadFormatWrite::CreateSinkBufferL()" );
    
    if ( KUidMediaTypeAudio != aMediaId.iMediaType )
        {
        User::Leave( KErrNotSupported );
        }

    aReference = ETrue;

    // Create buffer for data transfer between ULDataPath and FormatWrite
    return CreateSinkBufferOfSizeL( iCInfo.iFrameSize + KVoIPHeaderLength );
    }

// -----------------------------------------------------------------------------
// CG711PayloadFormatWrite::CreateSinkBufferOfSizeL
// Create a sink buffer of the given size.
// -----------------------------------------------------------------------------
//
CMMFDataBuffer* CG711PayloadFormatWrite::CreateSinkBufferOfSizeL( TUint aSize )
    {
    DP_G711_WRITE( "CG711PayloadFormatWrite::CreateSinkBufferOfSizeL()" );
    
    // Needs to create source buffer
    if ( !iSourceBuffer )
	    {  
	    iSourceBuffer = CMMFDataBuffer::NewL( aSize );
    	iSourceBuffer->Data().FillZ( aSize );
    	iSourceBuffer->SetRequestSizeL( aSize );
	    }
	    
    return iSourceBuffer;
    }

// -----------------------------------------------------------------------------
// CG711PayloadFormatWrite::EmptyBufferL
// Empty the given source buffer
// -----------------------------------------------------------------------------
//
void CG711PayloadFormatWrite::EmptyBufferL( CMMFBuffer* aBuffer, 
                                            MDataSource* aSupplier, 
                                            TMediaId aMediaId )
    {
    DP_G711_WRITE( "CG711PayloadFormatWrite::EmptyBufferL()" );
    
    __ASSERT_ALWAYS( aBuffer, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( aBuffer == iSourceBuffer, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( aSupplier, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( KUidMediaTypeAudio == aMediaId.iMediaType,
        User::Leave( KErrNotSupported ) );
    
    // Save source buffer parameters and change the state.
    iDataPath = aSupplier;
    iSourceBuffer = static_cast<CMMFDataBuffer*>( aBuffer );
    
    if ( !iSinkBuffer )
        {
        DP_G711_WRITE( "CG711PayloadFormatWrite::EmptyBufferL, sink not ready!" );
        
        iStateMachine->ChangeState( ESourceBufferEmptied );
        return;
        }
    
    // Check if hw has sent different length buffer compared to last time.
    // This means that we need to update the media clock. This allows G.711
    // codec to change dynamically the buffer size.
    const TInt dataLen( iSourceBuffer->Data().Length() - KVoIPHeaderLength );
    const TInt audioLen( dataLen / KBitsPerByte );
    if ( audioLen != iCInfo.iHwFrameTime )
        {
        // Re-register to RTP clock.
        iCInfo.iHwFrameTime = audioLen;
        iRtpMediaClock->UnregisterMediaFormat( iKey );
        iKey = iRtpMediaClock->RegisterMediaFormat( KDefaultSampleRate, iCInfo.iHwFrameTime );
        }
        
    DP_G711_WRITE( "CG711PayloadFormatWrite::EmptyBufferL, SetTimeToPlay" );
    
    TUint32 ts = 0;
    User::LeaveIfError( iRtpMediaClock->GetTimeStamp( iKey, ts ) );
    
    if ( iRtpMediaClock->TimeBasedIncrement() )
    	{
    	DP_G711_WRITE( 
    		"CG711PayloadFormatWrite::EmptyBufferL(), New stream begins" );
    	iFirstPacketFinished = EFalse;
    	}
    
    if ( !iSinkBuffer->Data().Length() )
        {
        iSinkBuffer->SetTimeToPlay( TInt64( ts ) );
        iSinkBuffer->SetFrameNumber( aBuffer->FrameNumber() );
        }
    else
        {
        iSourceBuffer->SetTimeToPlay( TInt64( ts ) );
        }
    
    iStateMachine->ChangeState( EEmptySourceBuffer );
    }

// -----------------------------------------------------------------------------
// CG711PayloadFormatWrite::SetSinkDataTypeCode
// Set the sink data type to the given four CC code for the given media
// -----------------------------------------------------------------------------
//
TInt CG711PayloadFormatWrite::SetSinkDataTypeCode( TFourCC aSinkFourCC,
                                                   TMediaId aMediaId )
    {
    DP_G711_WRITE( "CG711PayloadFormatWrite::SetSinkDataTypeCode()" );
    
    if ( KUidMediaTypeAudio != aMediaId.iMediaType ) 
        {
        return KErrNotSupported;
        }
    
    iFourCC = aSinkFourCC;
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CG711PayloadFormatWrite::SinkDataTypeCode
// Return the sink data type (four CC code) for the given media ID
// -----------------------------------------------------------------------------
//
TFourCC CG711PayloadFormatWrite::SinkDataTypeCode( TMediaId aMediaId )
    {
    if ( KUidMediaTypeAudio == aMediaId.iMediaType )
        {
        return iFourCC;
        }
    else
        {
        return TFourCC(); //defaults to 'NULL' fourCC
        }
    }

// -----------------------------------------------------------------------------
// CG711PayloadFormatWrite::BufferEmptiedL
// Called after payload buffer is completely emptied by RtpDataSink.
// -----------------------------------------------------------------------------
//
void CG711PayloadFormatWrite::BufferEmptiedL( CMMFBuffer* /*aBuffer*/ )
    {
        
    }

// -----------------------------------------------------------------------------
// CG711PayloadFormatWrite::NumChannels
// Returns number of channels
// -----------------------------------------------------------------------------
//
TUint CG711PayloadFormatWrite::NumChannels()
    {
    return KMono;
    }

// -----------------------------------------------------------------------------
// CG711PayloadFormatWrite::SampleRate
// Returns SampleRate
// -----------------------------------------------------------------------------
//
TUint CG711PayloadFormatWrite::SampleRate()
    {
    return KDefaultSampleRate;
    }

// -----------------------------------------------------------------------------
// CG711PayloadFormatWrite::SetSampleRate
// Set SampleRate
// -----------------------------------------------------------------------------
//
TInt CG711PayloadFormatWrite::SetSampleRate( TUint aSampleRate )
    {
    if ( KDefaultSampleRate != aSampleRate )
        {
        return KErrNotSupported;
        }
    else
        {
        return KErrNone;
        }
    }

// -----------------------------------------------------------------------------
// CG711PayloadFormatWrite::FrameTimeInterval
// Return the frame time interval for the given media
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds 
    CG711PayloadFormatWrite::FrameTimeInterval( TMediaId aMediaId ) const
    {
    if ( KUidMediaTypeAudio == aMediaId.iMediaType )
        {
        return TTimeIntervalMicroSeconds( TInt64( 0 ) );
        }
    else
        {
        return TTimeIntervalMicroSeconds( TInt64( 0 ) );
        }
    }

// -----------------------------------------------------------------------------
// CG711PayloadFormatWrite::Duration
// Return the frame time interval for the given media
// NOT SUPPORTED
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds 
    CG711PayloadFormatWrite::Duration( TMediaId /*aMediaType*/ ) const
    {
    return TTimeIntervalMicroSeconds( TInt64( 0 ) );
    }

// -----------------------------------------------------------------------------
// CG711PayloadFormatWrite::EmptySourceBufferL
// Empty the given sourcebuffer
// Sourcebuffer is given in iSourceBuffer
// Called by statemachine
// -----------------------------------------------------------------------------
//
void CG711PayloadFormatWrite::EmptySourceBufferL()
	{
    const TDesC8& srcDes( iSourceBuffer->Data() );
    
    DP_G711_WRITE2( "CG711PayloadFormatWrite::EmptySourceBufferL - SRC BUF SIZE: %d", srcDes.Size() );
    DP_G711_WRITE2( "CG711PayloadFormatWrite::EmptySourceBufferL - AUDIO SIZE: %d", srcDes.Size() - KVoIPHeaderLength );
    
    TPtrC8 audioFrame( srcDes.Mid( KVoIPHeaderLength ) );
    TPtrC8 frameHeader( srcDes.Left( KVoIPHeaderLength ) );
    TDes8& destDes( iSinkBuffer->Data() );

    if ( audioFrame.Size() > destDes.MaxSize() - destDes.Size() || !frameHeader.Length() )
        {
        DP_G711_WRITE( "CG711PayloadFormatWrite::EmptySourceBufferL WARNING SRCBUF LARGER THAN SINKBUF" );
        }
    else
        {
        const TUint8 frmHdrByte( frameHeader[0] );
        if ( KVoIPAudioFrame == frmHdrByte )
            {
            DP_G711_WRITE( "CG711PayloadFormatWrite::EmptySourceBufferL - AUDIO FRAME" );
            if ( iCNModeON )
                {
                iCNModeON = EFalse;
                iFirstPacketFinished = EFalse;
                }

            destDes.Append( audioFrame );
            }
        else if ( KVoIPCNFrame == frmHdrByte )
            {
            DP_G711_WRITE( "CG711PayloadFormatWrite::EmptySourceBufferL - CNOISE FRAME" );
            if ( destDes.Length() )
                {
                // Send audio frames first
                DeliverPacketL( *iSinkBuffer );
                iSinkBuffer->SetTimeToPlay( iSourceBuffer->TimeToPlay() );
                }

            iCNModeON = ETrue;
            destDes.Append( audioFrame );
            }
        else
            {
            DP_G711_WRITE( "CG711PayloadFormatWrite::EmptySourceBufferL - UNVOICED NON-SID FRAME" );
            }
        }        
    
    // If we have filled the buffer enough or the frame is a CN packet
    // then send it.
    const TInt sendBytes( iCInfo.iPtime * KBitsPerByte );
    if ( sendBytes == destDes.Size() || iCNModeON )
        {
        this->DeliverPacketL( *iSinkBuffer );
        }

    iStateMachine->ChangeState( ESourceBufferEmptied );
    }

// -----------------------------------------------------------------------------
// CG711PayloadFormatWrite::DeliverPacketL
// Prepare the packet header and deliver the packet to the datasink.
// -----------------------------------------------------------------------------
//
void CG711PayloadFormatWrite::DeliverPacketL( CMMFDataBuffer& aPayload )
    {
    DP_G711_WRITE2( "CG711PayloadFormatWrite::DeliverPacketL - TSTAMP: %u",
        static_cast<TUint32>( aPayload.TimeToPlay().Int64() ) );
    
    if ( !iClip )
        {
        DP_G711_WRITE( "CG711PayloadFormatWrite::DeliverPacketL NO DATASINK!" );

        User::Leave( KErrNotReady );
        }

    // Construct RTP header. Note that CN frames must not have the marker bit
    // set, only first audioframe after silence period.
    TRtpSendHeader sendHdr;
    if ( !iFirstPacketFinished && !iCNModeON )
        {
        sendHdr.iMarker = 1;
        iFirstPacketFinished = ETrue;
        }
    else
        {
        sendHdr.iMarker = 0;
        }

    sendHdr.iTimestamp 
        = static_cast<TUint32>( aPayload.TimeToPlay().Int64() );
    
    const TMediaId mediaId( KUidMediaTypeAudio );
    const TInt dataSize( aPayload.Data().Size() );
    if ( iCNModeON )
        {
        if ( iCInfo.iComfortNoiseGenerationPt != KPayloadTypeUndefined )
            {
            sendHdr.iPayloadType = iCInfo.iComfortNoiseGenerationPt;
            if ( dataSize )
                {  
                if ( iIsRtpSink )
                    {
                    iRtpDataSink->EmptyBufferL( &aPayload, this, mediaId, sendHdr );
                    }
                else
                    {
        		    aPayload.SetLastBuffer( sendHdr.iMarker );
                    iClip->EmptyBufferL( &aPayload, this, mediaId );
                    }
                }
            }
        }
    else
        {
        sendHdr.iPayloadType = iCInfo.iPayloadType;
        
        if ( dataSize )
            {  
            if ( iIsRtpSink )
                {
                iRtpDataSink->EmptyBufferL( &aPayload, this, mediaId, sendHdr );
                }
            else
                {
    		    aPayload.SetLastBuffer( sendHdr.iMarker );
                iClip->EmptyBufferL( &aPayload, this, mediaId );
                }
            }
        }        
    
    // Reset the payload buffer -- only if previous EmptyBufferL() is a 
    // synchronous call.
    aPayload.Data().Zero();
    }

// -----------------------------------------------------------------------------
// CG711PayloadFormatWrite::SourceBufferEmptiedL
// Handle the event that sourcebuffer has been emptied.
// Sourcebuffer is given in "iSourceBuffer".
// Called by the statemachine.
// -----------------------------------------------------------------------------
//
void CG711PayloadFormatWrite::SourceBufferEmptiedL()
    {
    DP_G711_WRITE( "CG711PayloadFormatWrite::SourceBufferEmptiedL()" );
    iDataPath->BufferEmptiedL( iSourceBuffer );
    }

// -----------------------------------------------------------------------------
// CG711PayloadFormatWrite::SinkPrimeL
// Prime sink
// -----------------------------------------------------------------------------
//
void CG711PayloadFormatWrite::SinkPrimeL()
    {
    DP_G711_WRITE( "CG711PayloadFormatWrite::SinkPrimeL()"  );
    iClip->SinkPrimeL();
    }

// -----------------------------------------------------------------------------
// CG711PayloadFormatWrite::SinkPlayL
// Start playing.
// -----------------------------------------------------------------------------
//
void CG711PayloadFormatWrite::SinkPlayL()
    {
    DP_G711_WRITE( "CG711PayloadFormatWrite::SinkPlayL()" );
    // Allocate buffer for data transfer between 
    // FormatWrite - MDataSink AND FormatWrite - redundancy payload encoder
    delete iSinkBuffer;
    iSinkBuffer = NULL;
    iSinkBuffer = CMMFDataBuffer::NewL( iCInfo.iPtime * KBitsPerByte );
    
    // Start state machine
    iStateMachine->ChangeState( EEncodeIdle ); 
    iFirstPacketFinished = EFalse;

    // Start a new cycle of frame collecting
    if ( iSinkBuffer )
        {
        iSinkBuffer->SetLastBuffer( EFalse );
        }

    iClip->SinkPlayL();
    }

// -----------------------------------------------------------------------------
// CG711PayloadFormatWrite::SinkPauseL
// Pause sink
// -----------------------------------------------------------------------------
//
void CG711PayloadFormatWrite::SinkPauseL()
    {
    DP_G711_WRITE( "CG711PayloadFormatWrite::SinkPauseL()" );
    iStateMachine->Cancel();
    iStateMachine->ChangeState( EEncodeIdle );
    
    iClip->SinkPauseL();
    }

// -----------------------------------------------------------------------------
// CG711PayloadFormatWrite::SinkStopL
// Stop sink
// -----------------------------------------------------------------------------
//
void CG711PayloadFormatWrite::SinkStopL()
    {
    DP_G711_WRITE( "CG711PayloadFormatWrite::SinkStopL" );
    
    // Stop state machine
    iStateMachine->Cancel();
    iStateMachine->ChangeState( EEncodeIdle );
    
    iClip->SinkStopL();
    }

// ---------------------------------------------------------------------------
// CG711PayloadFormatWrite::ConfigurePayloadFormatL
// Configure payload encoding parameters
// ---------------------------------------------------------------------------
//
void CG711PayloadFormatWrite::ConfigurePayloadFormatL( const TDesC8& aConfigParams, 
    CMccRtpMediaClock& aClock  )
    {
    DP_G711_WRITE( "CG711PayloadFormatWrite::ConfigurePayloadFormatL" );
    
    __ASSERT_ALWAYS( aConfigParams.Size() == sizeof( TMccCodecInfo ),
        User::Leave( KErrArgument ) );
    
    TMccCodecInfoBuffer infoBuffer;
    infoBuffer.Copy( aConfigParams );
    
    if ( !infoBuffer().iIsUpdate )
        {
        iCInfo = infoBuffer();
        iRtpMediaClock = &aClock;  
        iKey = iRtpMediaClock->RegisterMediaFormat( KDefaultSampleRate, iCInfo.iHwFrameTime );
        
        if( iCInfo.iHwFrameTime )
            {
            iCInfo.iFrameSize = TUint( iCInfo.iHwFrameTime * KDefaultSampleRateInkHz );
            }
        else
            {
            DP_G711_WRITE( "CG711PayloadFormatWrite::ConfigurePayloadFormatL KErrArgument" );
            User::Leave( KErrArgument );
            }
        
        DP_G711_WRITE2( "CG711PayloadFormatWrite::ConfigurePayloadFormatL FrameSize: %d", iCInfo.iFrameSize );
        
        if ( EGenRedUsed == iCInfo.iAlgoUsed )
            {
            DP_G711_WRITE2( "CG711PayloadFormatWrite::ConfigurePayloadFormatL, RED LEVEL: %d",
                iCInfo.iRedundancyCount );
            
            CPayloadFormatWrite* redEncoder
                = static_cast<CMccRedPayloadWrite*>( iClip );
            
            TMccRedPayloadWriteConfig config;
            config.iRedBlockCount = iCInfo.iRedundancyCount;
            config.iMaxPayloadSize = iCInfo.iPtime * KBitsPerByte;
            config.iNumOfEncodings = 1;
            config.iRedPayloadType = iCInfo.iRedundantPayload;
            config.InitPayloadTypes();
            config.iEncPayloadTypes[0] = iCInfo.iPayloadType;
            TMccRedPayloadWritePckg pckg( config );
            redEncoder->ConfigurePayloadFormatL( pckg, *iRtpMediaClock );
            }
        }
    else
        {
        UpdateConfigurationL( infoBuffer() ); 
        }
    }

// -----------------------------------------------------------------------------
// CG711PayloadFormatWrite::UpdateConfigurationL
// Update payload encoding parameters
// -----------------------------------------------------------------------------
//
void CG711PayloadFormatWrite::UpdateConfigurationL( TMccCodecInfo& aCodecInfo )
    {
    DP_G711_WRITE( "CG711PayloadFormatWrite::UpdateConfigurationL" );
    
    iCInfo.iPtime = aCodecInfo.iPtime;
    iCInfo.iPayloadType = aCodecInfo.iPayloadType;
    iCInfo.iComfortNoiseGenerationPt = aCodecInfo.iComfortNoiseGenerationPt;
    
    // Allocate buffer for data transfer between 
    // FormatWrite - MDataSink AND FormatWrite - redundancy payload encoder
    delete iSinkBuffer;
    iSinkBuffer = NULL;
    iSinkBuffer = CMMFDataBuffer::NewL( iCInfo.iPtime * KBitsPerByte );
    
    DP_G711_WRITE( "CG711PayloadFormatWrite::UpdateConfigurationL, exit" );
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File
