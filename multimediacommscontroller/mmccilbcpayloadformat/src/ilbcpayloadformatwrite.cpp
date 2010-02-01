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
*                containing ILBC audio.
*
*/




// INCLUDE FILES
#include    "ilbcpayloadformatwrite.h"
#include    "mccrtpdatasink.h"
#include    "ilbcpayloadformatutil.h"
#include    "mccuids.hrh"
#include    "mccdef.h"
#include    "mccinternaldef.h"
#include    "mccrtpmediaclock.h" 
#include    "mccredpayloadwrite.h"

// MACROS
#ifdef _DEBUG
#define TRACE_ILBC_PAYLOAD_FORMAT_WRITE
#endif

// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CIlbcPayloadFormatWrite::CIlbcPayloadFormatWrite
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CIlbcPayloadFormatWrite::CIlbcPayloadFormatWrite()
    {
    
    };

// -----------------------------------------------------------------------------
// CIlbcPayloadFormatWrite::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CIlbcPayloadFormatWrite::ConstructL( MDataSink* aSink )
    {
    #ifdef TRACE_ILBC_PAYLOAD_FORMAT_WRITE
        RDebug::Print( _L("CIlbcPayloadFormatWrite::ConstructL") );
    #endif
    
    __ASSERT_ALWAYS( aSink, User::Leave( KErrArgument ) );
    
    // Set default values
    iFramesPerPacket = 1;
    iFourCC.Set( KMccFourCCIdILBC );

    // Set data sink
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
        #ifdef TRACE_ILBC_PAYLOAD_FORMAT_WRITE
            RDebug::Print( _L("CG711PayloadFormatWrite::ConstructL, sink not RTP one") );
        #endif
        }
    
    iClip = aSink;

    // Initialize state machine
    iStateMachine = CFormatEncodeStateMachine::NewL( this );
    iStateMachine->ChangeState( EEncodeIdle );
    };
        
// -----------------------------------------------------------------------------
// CIlbcPayloadFormatWrite::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CIlbcPayloadFormatWrite* CIlbcPayloadFormatWrite::NewL( MDataSink* aSink )
    {
    #ifdef TRACE_ILBC_PAYLOAD_FORMAT_WRITE
        RDebug::Print( _L("CIlbcPayloadFormatWrite::NewL") );
    #endif

    __ASSERT_ALWAYS( aSink, User::Leave( KErrArgument ) );
    
    CIlbcPayloadFormatWrite* self = new ( ELeave ) CIlbcPayloadFormatWrite();
    CleanupStack::PushL( self );
    self->ConstructL ( aSink );
    CleanupStack::Pop( self );
    return self;
    };

// Destructor
CIlbcPayloadFormatWrite::~CIlbcPayloadFormatWrite()
    {
    #ifdef TRACE_ILBC_PAYLOAD_FORMAT_WRITE
        RDebug::Print( _L("CIlbcPayloadFormatWrite::~CIlbcPayloadFormatWrite") );
    #endif

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
// CIlbcPayloadFormatWrite::SinkThreadLogon
// Passes the logon command to the sink clip
// -----------------------------------------------------------------------------
//
TInt CIlbcPayloadFormatWrite::SinkThreadLogon( 
    MAsyncEventHandler& aEventHandler )
    {
    iClip->SinkThreadLogon( aEventHandler );
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CIlbcPayloadFormatWrite::SinkThreadLogoff
// Passes the logoff command to the sink clip
// -----------------------------------------------------------------------------
//
void CIlbcPayloadFormatWrite::SinkThreadLogoff()
    {
    iClip->SinkThreadLogoff();
    }

// -----------------------------------------------------------------------------
// CIlbcPayloadFormatWrite::CreateSinkBufferL
// Create a sink buffer
// -----------------------------------------------------------------------------
//
CMMFBuffer* CIlbcPayloadFormatWrite::CreateSinkBufferL( TMediaId aMediaId, 
        TBool &aReference )
    {
    if ( KUidMediaTypeAudio != aMediaId.iMediaType )
        {
        #ifdef TRACE_ILBC_PAYLOAD_FORMAT_WRITE
            RDebug::Print (_L ("CIlbcPayloadFormatWrite::CreateSinkBufferL KErrNotSupported"));
        #endif
        
        User::Leave( KErrNotSupported );
        return NULL;
        }
    else
        {
        aReference = ETrue;
        
        // Create buffer for data transfer between UL datapth and FormatWrite.
        // 30ms mode buffers can be used also for 20ms mode.
        if ( KiLBCBitrateWith30ms == iCInfo.iBitrate ||
             KiLBCBitrateWith20ms == iCInfo.iBitrate )
            {
            return CreateSinkBufferOfSizeL( 
                    KiLBCNumOfHeaderBytes + KiLBCFrameSize30ms );
            }
        else
            {
            #ifdef TRACE_ILBC_PAYLOAD_FORMAT_WRITE
                RDebug::Print (_L ("CIlbcPayloadFormatWrite::CreateSinkBufferL KErrNotReady"));
            #endif
        
            User::Leave( KErrNotReady );
            return NULL;
            }
        }
    }

// -----------------------------------------------------------------------------
// CIlbcPayloadFormatWrite::CreateSinkBufferOfSizeL
// Create a sink buffer of the given size.
// -----------------------------------------------------------------------------
//
CMMFDataBuffer* CIlbcPayloadFormatWrite::CreateSinkBufferOfSizeL( TUint aSize )
    {
    #ifdef TRACE_ILBC_PAYLOAD_FORMAT_WRITE
        RDebug::Print( _L("CIlbcPayloadFormatWrite::CreateSinkBufferOfSizeL") );
    #endif
    
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
// CIlbcPayloadFormatWrite::EmptyBufferL
// Empty the given source buffer
// -----------------------------------------------------------------------------
//
void CIlbcPayloadFormatWrite::EmptyBufferL( CMMFBuffer* aBuffer, 
        MDataSource* aSupplier, TMediaId aMediaId )
    {
    #ifdef TRACE_ILBC_PAYLOAD_FORMAT_WRITE
        RDebug::Print( _L("CIlbcPayloadFormatWrite::EmptyBufferL aBuffer = 0x%x"),
            aBuffer );
    #endif
    
    __ASSERT_ALWAYS( aBuffer, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS ( aBuffer == iSourceBuffer, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( aSupplier, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( KUidMediaTypeAudio == aMediaId.iMediaType,
        User::Leave( KErrNotSupported ) );
    
    // Save source buffer parameters and change the state.
    iDataPath = aSupplier;
    iMediaId = aMediaId;
    iSourceBuffer = static_cast<CMMFDataBuffer*>( aBuffer );
    
    if ( !iSinkBuffer )
        {
        #ifdef TRACE_ILBC_PAYLOAD_FORMAT_WRITE
            RDebug::Print( _L("CIlbcPayloadFormatWrite::EmptyBufferL, sink not ready!") );
        #endif
        iStateMachine->ChangeState( ESourceBufferEmptied );
        return;
        }
    
    TUint32 ts = 0;
    User::LeaveIfError( iRtpMediaClock->GetTimeStamp( iKey, ts ) );

    if ( iRtpMediaClock->TimeBasedIncrement() )
    	{
    	iFirstPacketFinished = EFalse;
    	}
    
    if ( 0 == iFrameIndex )
        {
        #ifdef TRACE_ILBC_PAYLOAD_FORMAT_WRITE
            RDebug::Print( _L("CIlbcPayloadFormatWrite::EmptyBufferL, SetTimeToPlay") );
        #endif
        
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
// CIlbcPayloadFormatWrite::SetSinkDataTypeCode
// Set the sink data type to the given four CC code for the given media
// -----------------------------------------------------------------------------
//
TInt CIlbcPayloadFormatWrite::SetSinkDataTypeCode( TFourCC aSinkFourCC, 
    TMediaId aMediaId )
    {
    #ifdef TRACE_ILBC_PAYLOAD_FORMAT_WRITE
        RDebug::Print (_L ("CIlbcPayloadFormatWrite::SetSinkDataTypeCode()"));
    #endif

    if ( KUidMediaTypeAudio != aMediaId.iMediaType ) 
        {
        return KErrNotSupported;
        }
    
    iFourCC = aSinkFourCC;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CIlbcPayloadFormatWrite::SinkDataTypeCode
// Return the sink data type (four CC code) for the given media
// -----------------------------------------------------------------------------
//
TFourCC CIlbcPayloadFormatWrite::SinkDataTypeCode( TMediaId aMediaId )
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
// CIlbcPayloadFormatWrite::BufferEmptiedL
// Called after payload buffer is completely emptied by RtpDataSink.
// -----------------------------------------------------------------------------
//
void CIlbcPayloadFormatWrite::BufferEmptiedL( CMMFBuffer* /*aBuffer*/ )
    {
    
    }

// -----------------------------------------------------------------------------
// CIlbcPayloadFormatWrite::NumChannels
// Returns number of channels
// -----------------------------------------------------------------------------
//
TUint CIlbcPayloadFormatWrite::NumChannels()
    {
    return KMono;
    }

// -----------------------------------------------------------------------------
// CIlbcPayloadFormatWrite::SampleRate
// Returns SampleRate
// -----------------------------------------------------------------------------
//
TUint CIlbcPayloadFormatWrite::SampleRate()
    {
    return KiLBCSampleRate;
    }

// -----------------------------------------------------------------------------
// CIlbcPayloadFormatWrite::SetSampleRate
// Set SampleRate
// -----------------------------------------------------------------------------
//
TInt CIlbcPayloadFormatWrite::SetSampleRate( TUint aSampleRate )
    {
    return ( KiLBCSampleRate == aSampleRate ) ? KErrNone : KErrNotSupported;
    }

// -----------------------------------------------------------------------------
// CIlbcPayloadFormatWrite::FrameTimeInterval
// Return the frame time interval for the given media
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CIlbcPayloadFormatWrite::FrameTimeInterval(
    TMediaId aMediaId ) const
    {
    if ( KUidMediaTypeAudio == aMediaId.iMediaType )
        {
        return iFrameTimeInterval;
        }
    else
        {
        return TTimeIntervalMicroSeconds( TInt64( 0 ) );
        }
    }

// -----------------------------------------------------------------------------
// CIlbcPayloadFormatWrite::Duration
// Return the frame time interval for the given media
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CIlbcPayloadFormatWrite::Duration(
    TMediaId /*aMediaType*/) const
    {
    return TTimeIntervalMicroSeconds( TInt64( 0 ) );
    }


// -----------------------------------------------------------------------------
// CIlbcPayloadFormatWrite::EmptySourceBufferL
// Empty the given sourcebuffer
// Sourcebuffer is given in iSourceBuffer
// Called by statemachine
// -----------------------------------------------------------------------------
//
void CIlbcPayloadFormatWrite::EmptySourceBufferL()
    {
    #ifdef TRACE_ILBC_PAYLOAD_FORMAT_WRITE
        RDebug::Print( _L("CIlbcPayloadFormatWrite::EmptySourceBufferL - SIZE OF SRC BUF: %d"), 
            iSourceBuffer->Data().Size() );
    #endif

    TPtrC8 audioFrame( iSourceBuffer->Data().Mid( KiLBCNumOfHeaderBytes ) );
    TPtrC8 frameHeader = iSourceBuffer->Data().Left( KiLBCNumOfHeaderBytes );
    TDes8& destDes = iSinkBuffer->Data();
    
    if ( audioFrame.Size() > destDes.MaxSize() - destDes.Size() || !frameHeader.Length() )
        {
        #ifdef TRACE_ILBC_PAYLOAD_FORMAT_WRITE
        RDebug::Print( _L("CIlbcPayloadFormatWrite::EmptySourceBufferL \
            AUDIOFRAME DOES NOT FIT TO SINK BUF") );
        RDebug::Print( _L("%d > %d || !%d"), audioFrame.Size(), destDes.MaxSize()-destDes.Size(),
            !frameHeader.Length() );
        #endif
        }
    else
        {
        if ( KAudioFrameHeaderByte == frameHeader[0] )
            {
            #ifdef TRACE_ILBC_PAYLOAD_FORMAT_WRITE
                RDebug::Print( _L("CIlbcPayloadFormatWrite::EmptySourceBufferL AUDIO FRAME") );
            #endif
            if ( iCNModeON )
                {
                iCNModeON = EFalse;
                iFirstPacketFinished = EFalse;
                }

            destDes.Append( audioFrame );
            iFrameIndex++;
            }
        else if ( KCNoiseFrameHeaderByte == frameHeader[0] )
            {
            #ifdef TRACE_ILBC_PAYLOAD_FORMAT_WRITE
                RDebug::Print( _L("CIlbcPayloadFormatWrite::EmptySourceBufferL CNOISE FRAME") );
            #endif
            if ( destDes.Size() )
                {
                // Send audio frames first
                DeliverPacketL( *iSinkBuffer );
                iSinkBuffer->SetTimeToPlay( iSourceBuffer->TimeToPlay() );
                }
            
            iCNModeON = ETrue;
            destDes.Append( audioFrame );

            // Only one comfort noise frame per RTP packet
            iFrameIndex = iFramesPerPacket;
            }
        else
            {
            #ifdef TRACE_ILBC_PAYLOAD_FORMAT_WRITE
                RDebug::Print( _L("CIlbcPayloadFormatWrite::EmptySourceBufferL NULL BIT RATE FRAME") );
            #endif
            }        
        }
    
    if ( iFrameIndex == iFramesPerPacket )
        {
        // Reset variables
        iFrameIndex = 0;
        this->DeliverPacketL( *iSinkBuffer );
        destDes.Zero();
        }

    iStateMachine->ChangeState( ESourceBufferEmptied );
    }

// -----------------------------------------------------------------------------
// CIlbcPayloadFormatWrite::DeliverPacketL
// Prepare the packet header and deliver the packet to the datasink.
// -----------------------------------------------------------------------------
//
void CIlbcPayloadFormatWrite::DeliverPacketL( CMMFDataBuffer& aPayload )
    {
    #ifdef TRACE_ILBC_PAYLOAD_FORMAT_WRITE
        RDebug::Print( _L("CIlbcPayloadFormatWrite::DeliverPacketL - TSTAMP: %u"),
            static_cast<TUint32>( aPayload.TimeToPlay().Int64() ) );
    #endif
    
    if ( !iClip )
        {
        #ifdef TRACE_ILBC_PAYLOAD_FORMAT_WRITE
            RDebug::Print( _L("CIlbcPayloadFormatWrite::DeliverPacketL - NO DATASINK") );
        #endif
        User::Leave( KErrNotReady );
        }
    
    // Construct RTP header.
    if ( !iFirstPacketFinished && !iCNModeON )
        {
        iRtpSendHeader.iMarker = 1;
        iFirstPacketFinished = ETrue;
        }
    else
        {
        iRtpSendHeader.iMarker = 0;
        }
    
    if ( !iRtpDataSink )
        {
        #ifdef TRACE_ILBC_PAYLOAD_FORMAT_WRITE
            RDebug::Print( _L("CIlbcPayloadFormatWrite::DeliverPacketL - NO DATASINK") );
        #endif
        User::Leave( KErrNotReady );
        }
    
    iRtpSendHeader.iTimestamp 
        = static_cast<TUint32>( aPayload.TimeToPlay().Int64() );
    
    if ( iCNModeON )
        {
        if ( iCInfo.iComfortNoiseGenerationPt != KPayloadTypeUndefined )
            {
            iRtpSendHeader.iPayloadType = iCInfo.iComfortNoiseGenerationPt;
            
            if ( aPayload.Data().Size() )
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
            }
        }
    else
        {
        iRtpSendHeader.iPayloadType = iCInfo.iPayloadType;
        
        if ( aPayload.Data().Size() )
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

        }          

    // Reset the payload buffer -- only if previous EmptyBufferL() is a 
    // synchronous call
    aPayload.Data().Zero();
    }

// -----------------------------------------------------------------------------
// CIlbcPayloadFormatWrite::SourceBufferEmptiedL
// Handle the event that sourcebuffer has been emptied.
// Sourcebuffer is given in "iSourceBuffer".
// Called by the statemachine.
// -----------------------------------------------------------------------------
//
void CIlbcPayloadFormatWrite::SourceBufferEmptiedL()
    {
    #ifdef TRACE_ILBC_PAYLOAD_FORMAT_WRITE
        RDebug::Print( _L("CIlbcPayloadFormatWrite::SourceBufferEmptiedL") );
    #endif

    iDataPath->BufferEmptiedL( iSourceBuffer );
    }

// -----------------------------------------------------------------------------
// CIlbcPayloadFormatWrite::SinkPrimeL
// Prime sink
// -----------------------------------------------------------------------------
//
void CIlbcPayloadFormatWrite::SinkPrimeL()
    {
    #ifdef TRACE_ILBC_PAYLOAD_FORMAT_WRITE
        RDebug::Print (_L ("CIlbcPayloadFormatWrite::SinkPrimeL()"));
    #endif
    
    iClip->SinkPrimeL();
    }

// -----------------------------------------------------------------------------
// CIlbcPayloadFormatWrite::SinkPlayL
// Start playing
// -----------------------------------------------------------------------------
//
void CIlbcPayloadFormatWrite::SinkPlayL()
    {
    #ifdef TRACE_ILBC_PAYLOAD_FORMAT_WRITE
        RDebug::Print (_L ("CIlbcPayloadFormatWrite::SinkPlayL()"));
    #endif
    
    // Allocate buffer for data transfer between 
    // FormatWrite - MDataSink AND FormatWrite - redundancy payload encoder
    delete iSinkBuffer;
    iSinkBuffer = NULL;
    iSinkBuffer = CMMFDataBuffer::NewL( iCInfo.iFrameSize * iFramesPerPacket );
    
    // Start state machine
    iStateMachine->ChangeState( EEncodeIdle );
    // RTP Header marker-bit must be set to 1 when starting stream
    iFirstPacketFinished = EFalse;
    
    // Start a new cycle of frame collecting
    iFrameIndex = 0;
    
    if ( iSinkBuffer )
        {
        iSinkBuffer->SetLastBuffer( EFalse );
        }

    iClip->SinkPlayL();
    }

// -----------------------------------------------------------------------------
// CIlbcPayloadFormatWrite::SinkPauseL
// Pause sink
// -----------------------------------------------------------------------------
//
void CIlbcPayloadFormatWrite::SinkPauseL()
    {
    #ifdef TRACE_ILBC_PAYLOAD_FORMAT_WRITE
        RDebug::Print( _L("CIlbcPayloadFormatWrite::SinkPauseL") );
    #endif
    
    iStateMachine->Cancel();
    iStateMachine->ChangeState( EEncodeIdle );
    
    iClip->SinkPauseL();
    }

// -----------------------------------------------------------------------------
// CIlbcPayloadFormatWrite::SinkStopL
// Stop sink
// -----------------------------------------------------------------------------
//
void CIlbcPayloadFormatWrite::SinkStopL()
    {
    #ifdef TRACE_ILBC_PAYLOAD_FORMAT_WRITE
        RDebug::Print( _L("CIlbcPayloadFormatWrite::SinkStopL") );
    #endif
    
    // Stop state machine
    iStateMachine->Cancel();
    iStateMachine->ChangeState( EEncodeIdle );
    iClip->SinkStopL();
    }
 
// -----------------------------------------------------------------------------
// CIlbcPayloadFormatWrite::ConfigurePayloadFormatL
// Configure payload encoding parameters.
// -----------------------------------------------------------------------------
//
void CIlbcPayloadFormatWrite::ConfigurePayloadFormatL( const TDesC8& aConfigParams, 
    CMccRtpMediaClock& aClock )
    {
    #ifdef TRACE_ILBC_PAYLOAD_FORMAT_WRITE
        RDebug::Print( _L("CIlbcPayloadFormatWrite::ConfigurePayloadFormatL IN") );
    #endif    
    __ASSERT_ALWAYS( aConfigParams.Size() == sizeof( TMccCodecInfo ),
        User::Leave( KErrArgument ) );
        
    iRtpMediaClock = &aClock; 
    
    TMccCodecInfoBuffer infoBuffer;
    infoBuffer.Copy( aConfigParams );
    
    if ( !infoBuffer().iIsUpdate )
        {
        DoConfigurePayloadFormatL( infoBuffer() );
        }
    else
        {
        UpdateConfigurationL( infoBuffer() );
        }  
    
    #ifdef TRACE_ILBC_PAYLOAD_FORMAT_WRITE
    RDebug::Print( _L("CIlbcPayloadFormatWrite::ConfigurePayloadFormatL OUT") );
    #endif 
    }

// -----------------------------------------------------------------------------
// CIlbcPayloadFormatWrite::UpdateConfigurationL
// Update payload encoding parameters
// -----------------------------------------------------------------------------
//
void CIlbcPayloadFormatWrite::UpdateConfigurationL( const TMccCodecInfo& aCodecInfo )
    {
    #ifdef TRACE_ILBC_PAYLOAD_FORMAT_WRITE
        RDebug::Print( _L("CIlbcPayloadFormatWrite::UpdateConfigurationL") );
    #endif
    
    iCInfo.iPayloadType = aCodecInfo.iPayloadType;
    
    if ( iCInfo.iMaxPtime != aCodecInfo.iMaxPtime ||
         iCInfo.iPtime != aCodecInfo.iPtime ||
         iCInfo.iHwFrameTime != aCodecInfo.iHwFrameTime ||
         iCInfo.iBitrate != aCodecInfo.iBitrate ||
         iCInfo.iComfortNoiseGenerationPt != aCodecInfo.iComfortNoiseGenerationPt )
        {
        DoConfigurePayloadFormatL( aCodecInfo );
        
        // Allocate buffer for data transfer between 
        // FormatWrite - MDataSink AND FormatWrite - redundancy payload encoder
        delete iSinkBuffer;
        iSinkBuffer = NULL;
        iSinkBuffer = CMMFDataBuffer::NewL( iCInfo.iFrameSize * iFramesPerPacket );
        
        // Start a new cycle of frame collecting
        iFrameIndex = 0; 
        }

    #ifdef TRACE_ILBC_PAYLOAD_FORMAT_WRITE
    RDebug::Print( _L("CIlbcPayloadFormatWrite::UpdateConfigurationL OUT") );
    #endif
    } 

// -----------------------------------------------------------------------------
// CIlbcPayloadFormatWrite::DoConfigurePayloadFormatL
// -----------------------------------------------------------------------------
//   
void CIlbcPayloadFormatWrite::DoConfigurePayloadFormatL( const TMccCodecInfo& aCodecInfo )
    {
    __ASSERT_ALWAYS( aCodecInfo.iHwFrameTime != 0, User::Leave( KErrArgument ) );
    
    iCInfo = aCodecInfo;   
    iKey = iRtpMediaClock->RegisterMediaFormat( KiLBCSampleRate, iCInfo.iHwFrameTime ); 

    if ( KiLBCBitrateWith20ms == iCInfo.iBitrate )
        {
        iCInfo.iHwFrameTime = KiLBCFrameTime20ms;
        // Maximum number of frames in RTP payload
        iFramesPerPacket = iCInfo.iPtime / iCInfo.iHwFrameTime;
        iCInfo.iFrameSize = KiLBCFrameSize20ms;
        iFrameTimeInterval = TInt64( KiLBCFrameTime20ms * TUint8( iFramesPerPacket ) );
        }
    else if ( KiLBCBitrateWith30ms == iCInfo.iBitrate )
        {
        iCInfo.iHwFrameTime = KiLBCFrameTime30ms;
        // Maximum number of frames in RTP payload
        iFramesPerPacket = iCInfo.iPtime / iCInfo.iHwFrameTime;
        iCInfo.iFrameSize = KiLBCFrameSize30ms;
        iFrameTimeInterval = TInt64( KiLBCFrameTime30ms * TUint8( iFramesPerPacket ) );
        }
    else
        {
        #ifdef TRACE_ILBC_PAYLOAD_FORMAT_WRITE
            RDebug::Print( _L("CIlbcPayloadFormatWrite::ConfigurePayloadFormatL - BITRATE INVALID: %d"), iCInfo.iBitrate );
        #endif 
        User::Leave( KErrArgument );
        }

    if ( EGenRedUsed == iCInfo.iAlgoUsed )
        {
        #ifdef TRACE_ILBC_PAYLOAD_FORMAT_WRITE
            RDebug::Print( _L("CIlbcPayloadFormatWrite::ConfigurePayloadFormatL, RED LEVEL: %d"), 
                iCInfo.iRedundancyCount );
        #endif 
        
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
    }
    
// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File
