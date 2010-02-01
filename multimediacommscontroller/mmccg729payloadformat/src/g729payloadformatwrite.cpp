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
*                containing G729 audio.
*
*/




// INCLUDE FILES
#include    "g729payloadformatwrite.h"
#include    "g729payloadformatutil.h"
#include    "mccrtpdatasink.h"
#include    "mmccinterfacedef.h"
#include    "mccuids.hrh"
#include    "mccdef.h"
#include    "streamformatter.h"
#include    "mccrtpmediaclock.h" 
#include    "mccredpayloadwrite.h"


// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CG729PayloadFormatWrite::CG729PayloadFormatWrite
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CG729PayloadFormatWrite::CG729PayloadFormatWrite()
    {
    
    }

// -----------------------------------------------------------------------------
// CG729PayloadFormatWrite::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CG729PayloadFormatWrite::ConstructL (MDataSink* aSink)
    {
    #ifdef TRACE_G729_PAYLOAD_FORMAT_WRITE
        RDebug::Print( _L("CG729PayloadFormatWrite::ConstructL()") );
    #endif

    // Set default values
    iFramesPerPacket = 2;
    iFourCC.Set( KMccFourCCIdG729 );

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
        #ifdef TRACE_G729_PAYLOAD_FORMAT_WRITE
        RDebug::Print( _L("CG729PayloadFormatWrite::ConstructL, sink not RTP one" ) );
        #endif
        }
    
    iClip = aSink;

    // Initialize state machine
    iStateMachine = CFormatEncodeStateMachine::NewL( this );
    }

// -----------------------------------------------------------------------------
// CG729PayloadFormatWrite::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CG729PayloadFormatWrite* CG729PayloadFormatWrite::NewL (MDataSink* aSink)
    {
    #ifdef TRACE_G729_PAYLOAD_FORMAT_WRITE
            RDebug::Print (_L ("CG729PayloadFormatWrite::NewL()"));
    #endif

    __ASSERT_ALWAYS( aSink, User::Leave( KErrArgument ) );
    
    CG729PayloadFormatWrite* self = new (ELeave) CG729PayloadFormatWrite;
    CleanupStack::PushL ( self );
    self->ConstructL ( aSink );
    CleanupStack::Pop( self );
    return self;
    }
    

// -----------------------------------------------------------------------------
// CG729PayloadFormatWrite::~CG729PayloadFormatWrite
// Destructor..
// -----------------------------------------------------------------------------
//
CG729PayloadFormatWrite::~CG729PayloadFormatWrite ()
    {
    #ifdef TRACE_G729_PAYLOAD_FORMAT_WRITE
        RDebug::Print( _L("CG729PayloadFormatWrite::~CG729PayloadFormatWrite()") );
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
// CG729PayloadFormatWrite::SinkThreadLogon
// Passes the logon command to the sink clip
// -----------------------------------------------------------------------------
//
TInt CG729PayloadFormatWrite::SinkThreadLogon(
    MAsyncEventHandler& aEventHandler )
    {
    iClip->SinkThreadLogon( aEventHandler );
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CG729PayloadFormatWrite::SinkThreadLogoff
// Passes the logoff command to the sink clip
// -----------------------------------------------------------------------------
//
void CG729PayloadFormatWrite::SinkThreadLogoff()
    {
    iClip->SinkThreadLogoff();
    }

// -----------------------------------------------------------------------------
// CG729PayloadFormatWrite::CreateSinkBufferL
// Create a sink buffer
// -----------------------------------------------------------------------------
//
CMMFBuffer* CG729PayloadFormatWrite::CreateSinkBufferL( TMediaId aMediaId, 
                                                        TBool &aReference )
    {
    #ifdef TRACE_G729_PAYLOAD_FORMAT_WRITE
        RDebug::Print( _L("CG729PayloadFormatWrite::CreateSinkBufferL()") );
    #endif
    
    if ( KUidMediaTypeAudio != aMediaId.iMediaType )
        {
        User::Leave( KErrNotSupported );
        }

    aReference = ETrue;

    // Create buffer for data transfer between ULDataPath and FormatWrite
    return CreateSinkBufferOfSizeL( KG729CodecEncBufSize );
    }

// -----------------------------------------------------------------------------
// CG729PayloadFormatWrite::CreateSinkBufferOfSizeL
// Create a sink buffer of the given size.
// -----------------------------------------------------------------------------
//
CMMFDataBuffer* CG729PayloadFormatWrite::CreateSinkBufferOfSizeL( TUint aSize )
    {
    #ifdef TRACE_G729_PAYLOAD_FORMAT_WRITE
        RDebug::Print( _L("CG729PayloadFormatWrite::CreateSinkBufferOfSizeL()") );
    #endif
    
    // Need to create a source buffer
    if ( !iSourceBuffer )
	    {  
	    iSourceBuffer = CMMFDataBuffer::NewL( aSize );
    	iSourceBuffer->Data().FillZ( aSize );
    	iSourceBuffer->SetRequestSizeL( aSize );
	    }
	    
    return iSourceBuffer;
    }

// -----------------------------------------------------------------------------
// CG729PayloadFormatWrite::EmptyBufferL
// Empty the given source buffer
// -----------------------------------------------------------------------------
//
void CG729PayloadFormatWrite::EmptyBufferL( CMMFBuffer* aBuffer, 
    MDataSource* aSupplier, TMediaId aMediaId )
    {
    #ifdef TRACE_G729_PAYLOAD_FORMAT_WRITE
        RDebug::Print( _L("CG729PayloadFormatWrite::EmptyBufferL aBuffer[0x%x]"), aBuffer);
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
        #ifdef TRACE_G729_PAYLOAD_FORMAT_WRITE
            RDebug::Print( _L("CG729PayloadFormatWrite::EmptyBufferL, sink not ready!") );
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
        #ifdef TRACE_G729_PAYLOAD_FORMAT_WRITE
            RDebug::Print( _L("CG729PayloadFormatWrite::EmptyBufferL, SetTimeToPlay") );
        #endif
        
        iSinkBuffer->SetTimeToPlay( TInt64( ts ) );
        iSinkBuffer->SetFrameNumber( aBuffer->FrameNumber() );
        }
    
    #ifdef TRACE_G729_PAYLOAD_FORMAT_WRITE
    
    TDesC8& data = iSourceBuffer->Data();
    RDebug::Print( _L("CG729PayloadFormatWrite::EmptyBufferL DATALEN: %d"), data.Length() );
    RDebug::Print( _L("CG729PayloadFormatWrite::EmptyBufferL DATA_SZ: %d"), data.Size() );
    
    if( data.Length() > KG729NumOfHeaderBytes )
        {
        RDebug::Print( _L("CG729PayloadFormatWrite::EmptyBufferL DATA[0]: %u"), data[0] );
        RDebug::Print( _L("CG729PayloadFormatWrite::EmptyBufferL DATA[1]: %u"), data[1] );
        }
    
    #endif
    
    iStateMachine->ChangeState( EEmptySourceBuffer );
    }

// -----------------------------------------------------------------------------
// CG729PayloadFormatWrite::SetSinkDataTypeCode
// Set the sink data type to the given four CC code for the given media
// -----------------------------------------------------------------------------
//
TInt CG729PayloadFormatWrite::SetSinkDataTypeCode( TFourCC aSinkFourCC, 
    TMediaId aMediaId )
    {
    #ifdef TRACE_G729_PAYLOAD_FORMAT_WRITE
        RDebug::Print (_L ("CG729PayloadFormatWrite::SetSinkDataTypeCode()"));
    #endif

    if ( KUidMediaTypeAudio != aMediaId.iMediaType ) 
        {
        return KErrNotSupported;
        }
    
    iFourCC = aSinkFourCC;
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CG729PayloadFormatWrite::SinkDataTypeCode
// Return the sink data type (four CC code) for the given media ID
// -----------------------------------------------------------------------------
//
TFourCC CG729PayloadFormatWrite::SinkDataTypeCode( TMediaId aMediaId )
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
// CG729PayloadFormatWrite::BufferEmptiedL
// Called after payload buffer is completely emptied by RtpDataSink.
// -----------------------------------------------------------------------------
//
void CG729PayloadFormatWrite::BufferEmptiedL( CMMFBuffer* /*aBuffer*/ )
    {
    
    }

// -----------------------------------------------------------------------------
// CG729PayloadFormatWrite::NumChannels
// Returns number of channels
// -----------------------------------------------------------------------------
//
TUint CG729PayloadFormatWrite::NumChannels()
    {
    return KMono;
    }

// -----------------------------------------------------------------------------
// CG729PayloadFormatWrite::SampleRate
// Returns SampleRate
// -----------------------------------------------------------------------------
//
TUint CG729PayloadFormatWrite::SampleRate()
    {
    return KG729SampleRate;
    }

// -----------------------------------------------------------------------------
// CG729PayloadFormatWrite::SetSampleRate
// Set SampleRate
// -----------------------------------------------------------------------------
//
TInt CG729PayloadFormatWrite::SetSampleRate( TUint aSampleRate )
    {
    return ( KG729SampleRate == aSampleRate ) ? KErrNone : KErrNotSupported;
    }

// -----------------------------------------------------------------------------
// CG729PayloadFormatWrite::FrameTimeInterval
// Return the frame time interval for the given media
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CG729PayloadFormatWrite::FrameTimeInterval(
                                                    TMediaId aMediaId) const
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
// CG729PayloadFormatWrite::Duration
// Return the frame time interval for the given media
// NOT SUPPORTED
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CG729PayloadFormatWrite::Duration(
                                              TMediaId /*aMediaType*/) const
    {
    return TTimeIntervalMicroSeconds( TInt64( 0 ) );
    }

// -----------------------------------------------------------------------------
// CG729PayloadFormatWrite::EmptySourceBufferL
// Empty the given sourcebuffer
// Sourcebuffer is given in iSourceBuffer.
// Called by statemachine
// -----------------------------------------------------------------------------
//
void CG729PayloadFormatWrite::EmptySourceBufferL()
    {
    #ifdef TRACE_G729_PAYLOAD_FORMAT_WRITE
        RDebug::Print (_L ("CG729PayloadFormatWrite::EmptySourceBufferL - SRC BUF SIZE: %d"),
        iSourceBuffer->Data().Size() );
    #endif

    TPtrC8 audioFrame( iSourceBuffer->Data().Mid( KG729NumOfHeaderBytes ) );
    TPtrC8 dtxHeader( iSourceBuffer->Data().Left( KG729NumOfHeaderBytes ) );

    if ( KAudioFrameHeaderByte == dtxHeader[0] )
        {
        #ifdef TRACE_G729_PAYLOAD_FORMAT_WRITE
            RDebug::Print (_L ("CG729PayloadFormatWrite::EmptySourceBufferL - AUDIO FRAME") );
        #endif

        iFrameIndex++;
        DoBitPacking( audioFrame, iSinkBuffer->Data(), EFalse );
        }
    else if ( KCNoiseFrameHeaderByte == dtxHeader[0] )
        {
        #ifdef TRACE_G729_PAYLOAD_FORMAT_WRITE
            RDebug::Print (_L ("CG729PayloadFormatWrite::EmptySourceBufferL - CNOISE FRAME") );
        #endif
        
        // This is first comfort noise received. Audio in buffer must send 
        // immediately with CN frame at the end of buffer. RFC3551 4.5.6.
        iFrameIndex = 0;
        if ( KErrNone == DoBitPacking( audioFrame, iSinkBuffer->Data(), ETrue ) )
            {
            DeliverPacketL( *iSinkBuffer );
            }
        }
    else
        {
        #ifdef TRACE_G729_PAYLOAD_FORMAT_WRITE
            RDebug::Print (_L ("CG729PayloadFormatWrite::EmptySourceBufferL - NULL BIT RATE FRAME") );
        #endif
        }
    
    if ( iFrameIndex == iFramesPerPacket )
        {
        // Packetization interval amount of audio frames is received
        iFrameIndex = 0;
        DeliverPacketL( *iSinkBuffer );
        }  
        
    iStateMachine->ChangeState( ESourceBufferEmptied );
    }

// -----------------------------------------------------------------------------
// CG729PayloadFormatWrite::DeliverPacketL
// Prepare the packet header and deliver the packet to the datasink.
// -----------------------------------------------------------------------------
//
void CG729PayloadFormatWrite::DeliverPacketL( CMMFDataBuffer& aPayload )
    {
    #ifdef TRACE_G729_PAYLOAD_FORMAT_WRITE
        RDebug::Print( _L("CG729PayloadFormatWrite::DeliverPacketL - TSTAMP: %u"),
            static_cast<TUint32>( aPayload.TimeToPlay().Int64() ) );
    #endif

    // Set the marker bit if it is very first packet.
    if ( !iFirstPacketFinished )
        {
        iRtpSendHeader.iMarker = 1;
        iFirstPacketFinished = ETrue;
        }
    else
        {
        iRtpSendHeader.iMarker = 0;
        }
    
    if ( !iClip )
        {
        User::Leave( KErrNotReady );
        }

    // Construct RTP header.                     
    iRtpSendHeader.iPayloadType = iCInfo.iPayloadType;
    iRtpSendHeader.iTimestamp 
        = static_cast<TUint32>( aPayload.TimeToPlay().Int64() );
    
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


    // Reset the payload buffer -- only if previous EmptyBufferL() is a 
    // synchronous call
    aPayload.Data().Zero();
    }

// -----------------------------------------------------------------------------
// CG729PayloadFormatWrite::SourceBufferEmptiedL
// Handle the event that sourcebuffer has been emptied.
// Sourcebuffer is given in "iSourceBuffer".
// Called by the statemachine.
// -----------------------------------------------------------------------------
//
void CG729PayloadFormatWrite::SourceBufferEmptiedL()
    {
    #ifdef TRACE_G729_PAYLOAD_FORMAT_WRITE
        RDebug::Print (_L ("CG729PayloadFormatWrite::SourceBufferEmptiedL()"));
    #endif

    iDataPath->BufferEmptiedL(iSourceBuffer);
    }

// -----------------------------------------------------------------------------
// CG729PayloadFormatWrite::SinkPrimeL
// Prime sink
// -----------------------------------------------------------------------------
//
void CG729PayloadFormatWrite::SinkPrimeL()
    {
    #ifdef TRACE_G729_PAYLOAD_FORMAT_WRITE
        RDebug::Print (_L ("CG729PayloadFormatWrite::SinkPrimeL()"));
    #endif

    iClip->SinkPrimeL();
    }

// -----------------------------------------------------------------------------
// CG729PayloadFormatWrite::SinkPlayL
// Start playing.
// -----------------------------------------------------------------------------
//
void CG729PayloadFormatWrite::SinkPlayL()
    {
    #ifdef TRACE_G729_PAYLOAD_FORMAT_WRITE
        RDebug::Print (_L ("CG729PayloadFormatWrite::SinkPlayL()"));
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
// CG729PayloadFormatWrite::SinkPauseL
// Pause sink
// -----------------------------------------------------------------------------
//
void CG729PayloadFormatWrite::SinkPauseL()
    {
    #ifdef TRACE_G729_PAYLOAD_FORMAT_WRITE
            RDebug::Print (_L ("CG729PayloadFormatWrite::SinkPauseL()"));
    #endif
    
    iStateMachine->Cancel();
    iStateMachine->ChangeState( EEncodeIdle );
    
    iClip->SinkPauseL();
    }

// -----------------------------------------------------------------------------
// CG729PayloadFormatWrite::SinkStopL
// Stop sink
// -----------------------------------------------------------------------------
//
void CG729PayloadFormatWrite::SinkStopL()
    {
    #ifdef TRACE_G729_PAYLOAD_FORMAT_WRITE
            RDebug::Print( _L("CG729PayloadFormatWrite::SinkStopL") );
    #endif
     
    // Stop state machine
    iStateMachine->Cancel();
    iStateMachine->ChangeState( EEncodeIdle );
    iClip->SinkStopL();
    }

// ---------------------------------------------------------------------------
// CG729PayloadFormatWrite::DoBitPacking
// Does bit-packing for encoded speech parameters got from encoder.
// ---------------------------------------------------------------------------
//
TInt CG729PayloadFormatWrite::DoBitPacking( const TDesC8& aSourceBuf,
    TDes8& aDestBuf, TBool aIsCNoise ) const
    {
    if ( aDestBuf.MaxLength() - aDestBuf.Length() < KG729FrameSize10ms )
        {
        #ifdef TRACE_G729_PAYLOAD_FORMAT_WRITE
            RDebug::Print ( _L ( "CG729PayloadFormatWrite::DoBitPacking - DESTINATION BUF TOO SMALL!" ) );
        #endif
        
        return KErrArgument;
        }
    else
        {
        TStreamEncoder encoder;
        encoder.Initialize( const_cast<TUint8*>( aDestBuf.Ptr() ), aDestBuf.Length(), 0 );
        
        TUint8 numOfParams( 0 );
        if ( aIsCNoise )
            {
            numOfParams = KG729NumOfCNoiseParams;
            aDestBuf.SetLength( aDestBuf.Length() + KG729CNFrameSize );
            }
        else
            {
            numOfParams = KG729NumOfAudioParams;
            aDestBuf.SetLength( aDestBuf.Length() + KG729FrameSize10ms );
            }

        TUint8 curParamBits( 0 );
        TInt byteIndex( 0 );
        TUint8 lsb( 0 );
        TUint8 msb( 0 );
        
        for ( TInt i = 0; i < numOfParams; i++ )
            {
            if ( aIsCNoise )
                {
                curParamBits = KG729CodecBufCNoiseBits[i];
                }
            else
                {
                curParamBits = KG729CodecBufAudioBits[i];
                }
            
            if ( KBitsInByte >= curParamBits )
                {
                lsb = aSourceBuf[ byteIndex ];
                encoder.Encode( TUint32( lsb ), curParamBits );
                }
            else
                {
                lsb = aSourceBuf[ byteIndex ];
                msb = aSourceBuf[ byteIndex + 1 ];
                encoder.Encode( TUint32( msb ), curParamBits - KBitsInByte );
                encoder.Encode( TUint32( lsb ), KBitsInByte );
                }
                
            byteIndex += 2;
            }
            
        return KErrNone;            
        }        
    }

// ---------------------------------------------------------------------------
// CG729PayloadFormatWrite::ConfigurePayloadFormatL
// Configure payload encoding parameters
// ---------------------------------------------------------------------------
//
void CG729PayloadFormatWrite::ConfigurePayloadFormatL( 
        const TDesC8& aConfigParams, 
        CMccRtpMediaClock& aClock  )
    {
    #ifdef TRACE_G729_PAYLOAD_FORMAT_WRITE
        RDebug::Print( _L("CG729PayloadFormatWrite::ConfigurePayloadFormatL IN") );
    #endif
    __ASSERT_ALWAYS( aConfigParams.Size() == sizeof( TMccCodecInfo ),
        User::Leave( KErrArgument ) );
    
    TMccCodecInfoBuffer infoBuffer;
    infoBuffer.Copy( aConfigParams );
    
    if ( !infoBuffer().iIsUpdate )
        {
        iCInfo = infoBuffer();
        iRtpMediaClock = &aClock; 
        iKey = iRtpMediaClock->RegisterMediaFormat( KG729SampleRate ,iCInfo.iHwFrameTime ); 
        
        iCInfo.iHwFrameTime = KG729FrameTimeInMs;   
        iFramesPerPacket = iCInfo.iPtime / iCInfo.iHwFrameTime;
        iCInfo.iFrameSize = KG729FrameSize10ms;
        iFrameTimeInterval = TInt64( iCInfo.iHwFrameTime * TInt8( iFramesPerPacket ) );

        #ifdef TRACE_G729_PAYLOAD_FORMAT_WRITE
            RDebug::Print( _L( "CG729PayloadFormatWrite::ConfigurePayloadFormatL - FramesPerPacket: %d, FrameSize: %d" ), iFramesPerPacket, iCInfo.iFrameSize );
        #endif

        if ( EGenRedUsed == iCInfo.iAlgoUsed )
            {
            #ifdef TRACE_G729_PAYLOAD_FORMAT_WRITE
            RDebug::Print( _L("CG729PayloadFormatWrite::ConfigurePayloadFormatL, RED LEVEL: %d"),
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
    else
        {
        UpdateConfigurationL( infoBuffer() ); 
        }
    
    #ifdef TRACE_G729_PAYLOAD_FORMAT_WRITE
    RDebug::Print ( _L ( "CG729PayloadFormatWrite::ConfigurePayloadFormatL OUT" ) );
    #endif
    }

// -----------------------------------------------------------------------------
// CG729PayloadFormatWrite::UpdateConfigurationL
// Update payload encoding parameters
// -----------------------------------------------------------------------------
//
void CG729PayloadFormatWrite::UpdateConfigurationL( const TMccCodecInfo& aCodecInfo )
    {
    #ifdef TRACE_G729_PAYLOAD_FORMAT_WRITE
        RDebug::Print( _L("CG729PayloadFormatWrite::UpdateConfigurationL IN") );
    #endif
    
    iCInfo.iPtime = aCodecInfo.iPtime;    
    iCInfo.iPayloadType = aCodecInfo.iPayloadType;
    iFramesPerPacket = iCInfo.iPtime / iCInfo.iHwFrameTime;
    iFrameTimeInterval = TInt64( iCInfo.iHwFrameTime * TInt8( iFramesPerPacket ) );
                                                            
    // Allocate buffer for data transfer between 
    // FormatWrite - MDataSink AND FormatWrite - redundancy payload encoder
    delete iSinkBuffer;
    iSinkBuffer = NULL;
    iSinkBuffer = CMMFDataBuffer::NewL( iCInfo.iFrameSize * iFramesPerPacket );
                                        
    // Start a new cycle of frame collecting
    iFrameIndex = 0;
    
    #ifdef TRACE_G729_PAYLOAD_FORMAT_WRITE
        RDebug::Print( _L("CG729PayloadFormatWrite::UpdateConfigurationL OUT") );
    #endif
    }    
// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File
