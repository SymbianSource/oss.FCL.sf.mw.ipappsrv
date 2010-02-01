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
* Description:    PayloadFormat component capable to read RTP payload containing
*                G711 audio.
*
*/




// INCLUDE FILES
#include    "g711payloadformatread.h"
#include    "g711payloadformatdefs.h"
#include    "mccrtpdatasource.h"
#include    "mccinternaldef.h"
#include    "mccdef.h"
#include    "mccredpayloadread.h"


// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CG711PayloadFormatRead::CG711PayloadFormatRead
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CG711PayloadFormatRead::CG711PayloadFormatRead()
    {
    }

// -----------------------------------------------------------------------------
// CG711PayloadFormatRead::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CG711PayloadFormatRead::ConstructL( MDataSource* aSource )
    {
    __ASSERT_ALWAYS( aSource, User::Leave( KErrArgument ) );
    
    iClip = aSource;
    iBufferToReadExists = EFalse;

    iFourCC.Set( KMccFourCCIdG711 );

    iFramesPerPacket = 1;

    // Initialize decoding state machine
    iStateMachine = CFormatDecodeStateMachine::NewL( this );

    iStateMachine->ChangeState( EDecodeIdle );

    iCurrentBuffer = EBufferOne;
    }

// -----------------------------------------------------------------------------
// CG711PayloadFormatRead::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CG711PayloadFormatRead* CG711PayloadFormatRead::NewL( MDataSource* aSource )
    {
    DP_G711_READ( "CG711PayloadFormatRead::NewL()" );

    __ASSERT_ALWAYS( aSource, User::Leave( KErrArgument ) );

    CG711PayloadFormatRead* self = new (ELeave) CG711PayloadFormatRead;
    CleanupStack::PushL( self );
    self->ConstructL( aSource );
    CleanupStack::Pop( self );
    return self;
    }


// Destructor
CG711PayloadFormatRead::~CG711PayloadFormatRead()
    {
    DP_G711_READ( "CG711PayloadFormatRead::~CG711PayloadFormatRead()" );
    
    delete iFrameBufferOne;
    delete iFrameBufferTwo;
    if ( iSourceBufOwnership )
        {
        delete iSourceBuffer;
        }
    
    delete iStateMachine;
    
    iClip = NULL;
    iFrameArray.Close();
    }


// -----------------------------------------------------------------------------
// CG711PayloadFormatRead::SendDataToSinkL
// Send full frame buffer to the DataPath.
// -----------------------------------------------------------------------------
//
void CG711PayloadFormatRead::SendDataToSinkL()
    {
    if ( iCurrentBuffer == EBufferOne )
        {
        DP_G711_READ( "CG711PayloadFormatRead::SendDataToSinkL() - BufferOne" );

        iDataPath->BufferFilledL( iFrameBufferOne );
        iCurrentBuffer = EBufferTwo;
        if ( iBufferToReadExists && !iFrameBufferOne->LastBuffer() )
            {
            // More payload buffer is ready
            iStateMachine->ChangeState( ESourceDataReady );
            }
        }
    else
        {
        DP_G711_READ( "CG711PayloadFormatRead::SendDataToSinkL() - BufferTwo" );            

        iDataPath->BufferFilledL( iFrameBufferTwo );
        iCurrentBuffer = EBufferOne;
        if ( iBufferToReadExists && !iFrameBufferTwo->LastBuffer() )
            {
            // More payload buffer is ready
            iStateMachine->ChangeState( ESourceDataReady );
            }
       }
    }

// -----------------------------------------------------------------------------
// CG711PayloadFormatRead::FillSinkBuffer
// Fill SinkBuffer.
// -----------------------------------------------------------------------------
//
void CG711PayloadFormatRead::FillSinkBufferL()
    {
    DP_G711_READ( "CG711PayloadFormatRead::FillSinkBuffer()" );
    
    CMMFDataBuffer* curFrameBuffer = NULL;
    if ( EBufferOne == iCurrentBuffer )
        {
        curFrameBuffer = iFrameBufferOne;
        }
    else
        {
        curFrameBuffer = iFrameBufferTwo;
        }
        
    TDes8& curFrameData( curFrameBuffer->Data() );
    curFrameData.SetLength( KVoIPHeaderLength );

    // Put next frame decoded from RTP payload to the framebuffer
    iBufferToReadExists = GetNextFrame( curFrameData );

    // G.711 Gwc expects 2 bytes for dtx-decision
    if ( iCnFrame )
        {
	    DP_G711_READ( "CG711PayloadFormatRead::FillSinkBufferL add dtx-header" );

        curFrameData[0] = KVoIPCNFrame;
        curFrameData[1] = 0;
	    }
    else
        {
	    DP_G711_READ( "CG711PayloadFormatRead::FillSinkBufferL add voice-header" );

        curFrameData[0] = KVoIPAudioFrame;
        curFrameData[1] = 0;
        }

	curFrameBuffer->SetFrameNumber( iRecvHeader.iTimestamp + ( ( iFrameIndex - 1 )
	                           * TUint( iCInfo.iHwFrameTime * KDefaultSampleRateInkHz ) ) );

    DP_G711_READ2( "CG711PayloadFormatRead::FillSinkBuffer - FRAMENUM: %u",
        curFrameBuffer->FrameNumber() );
    
    const TInt dataLen( ( iCInfo.iHwFrameTime * KBitsPerByte )
                            + KVoIPHeaderLength );
    curFrameData.SetLength( dataLen );          
    curFrameBuffer->SetStatus( EFull );

    DP_G711_READ2( "CG711PayloadFormatRead: Filled framebuffer with buf. size: %d",
        curFrameBuffer->BufferSize() );

    iStateMachine->ChangeState( EEmptyDataToSink );

    DP_G711_READ( "CG711PayloadFormatRead::FillSinkBuffer - DONE" );
    }

// -----------------------------------------------------------------------------
// CG711PayloadFormatRead::FillSourceBufferL
// Send fill buffer request to the RTP Data Source.
// -----------------------------------------------------------------------------
//
void CG711PayloadFormatRead::FillSourceBufferL()
    {
    DP_G711_READ( "CG711PayloadFormatRead::FillSourceBufferL()");
    
    const TMediaId mediaId( KUidMediaTypeAudio );
    iClip->FillBufferL( iSourceBuffer, this, mediaId );     
    }

// -----------------------------------------------------------------------------
// CG711PayloadFormatRead::SourcePrimeL
// Prime source.
// -----------------------------------------------------------------------------
//
void CG711PayloadFormatRead::SourcePrimeL()
    {
    DP_G711_READ( "CG711PayloadFormatRead::SourcePrimeL()" );
    
    iClip->SourcePrimeL();
    }

// -----------------------------------------------------------------------------
// CG711PayloadFormatRead::SourcePlayL
// Start playing.
// -----------------------------------------------------------------------------
void CG711PayloadFormatRead::SourcePlayL()
    {
    DP_G711_READ( "CG711PayloadFormatRead::SourcePlayL()" );
    
    iClip->SourcePlayL();
    }

// -----------------------------------------------------------------------------
// CG711PayloadFormatRead::SourcePauseL
// Pause source.
// -----------------------------------------------------------------------------
//
void CG711PayloadFormatRead::SourcePauseL()
    {
    DP_G711_READ( "CG711PayloadFormatRead::SourcePauseL" );
    
    iStateMachine->Cancel();
    iStateMachine->ChangeState( EDecodeIdle );
    
    iFrameBufferOne->SetLastBuffer( EFalse );
    iFrameBufferTwo->SetLastBuffer( EFalse );

    iFrameBufferOne->SetStatus( EAvailable );
    iFrameBufferTwo->SetStatus( EAvailable );

    iBufferToReadExists = EFalse;
    iCurrentBuffer = EBufferOne;
    
    iClip->SourcePauseL();
    }

// -----------------------------------------------------------------------------
// CG711PayloadFormatRead::SourceStopL
// Stop source
// -----------------------------------------------------------------------------
//
void CG711PayloadFormatRead::SourceStopL()
    {
    DP_G711_READ( "CG711PayloadFormatRead::SourceStopL" );
    
    // DO NOT RESET PACKET COUNT BACK TO ZERO HERE
    // UPPER LAYER MAY CALL LastDlPacketCount LATER

    iStateMachine->Cancel();

    iStateMachine->ChangeState( EDecodeIdle );
    iFrameBufferOne->SetLastBuffer( EFalse );
    iFrameBufferTwo->SetLastBuffer( EFalse );

    iFrameBufferOne->SetStatus( EAvailable );
    iFrameBufferTwo->SetStatus( EAvailable );

    iBufferToReadExists = EFalse;
    iCurrentBuffer = EBufferOne;

    iClip->SourceStopL();
    }

// -----------------------------------------------------------------------------
// CG711PayloadFormatRead::BufferFilledL
// 
// -----------------------------------------------------------------------------
//
void CG711PayloadFormatRead::DataBufferFilledL( CMMFBuffer* aBuffer,
    const TRtpRecvHeader& aRecvHeader )
    {
    DP_G711_READ2( "CG711PayloadFormatRead::DataBufferFilledL iTimestamp: %u",
        aRecvHeader.iTimestamp )
    DP_G711_READ2( "CG711PayloadFormatRead::DataBufferFilledL this: 0x%x", this )
    
    __ASSERT_ALWAYS( aBuffer, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( KUidMmfDataBuffer == aBuffer->Type(), 
        User::Leave( KErrNotSupported ) );
    __ASSERT_ALWAYS( iSourceBuffer == aBuffer, User::Leave( KErrArgument ) );
    
	iRecvHeader = aRecvHeader;
    if ( KComfortNoisePT == iRecvHeader.iPayloadType
        || KOldComfortNoisePT == iRecvHeader.iPayloadType )
        {
        iCnFrame = ETrue;
        }
    else 
        {
        iCnFrame = EFalse;
        }
    
    TInt numOfFrames = DecodePayload( iSourceBuffer->Data() );
    
    // Whenever BufferFilledL is called from RtpSourceSink
    // Set the state machine to fillsinkbuffer
    if ( numOfFrames )
        {
        iBufferToReadExists = ETrue;
        iSourceBuffer->SetFrameNumber( iRecvHeader.iTimestamp );
        iStateMachine->ChangeState( ESourceDataReady );
        }
    else
        {
        DP_G711_READ( "CG711PayloadFormatRead::BufferFilledL, decode failed" )
        
        FillSourceBufferL();
        }
    }

// -----------------------------------------------------------------------------
// CG711PayloadFormatRead::SampleRate
// Returns samplerate.
// -----------------------------------------------------------------------------
//
TUint CG711PayloadFormatRead::SampleRate()
    {
    return KDefaultSampleRate;
    }  

// -----------------------------------------------------------------------------
// CG711PayloadFormatRead::SetSampleRate
// Set samplerate.
// -----------------------------------------------------------------------------
//
TInt CG711PayloadFormatRead::SetSampleRate( TUint aSampleRate )
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
// CG711PayloadFormatRead::NumChannels
// Returns number of channels.
// -----------------------------------------------------------------------------
//
TUint CG711PayloadFormatRead::NumChannels()
    {
    return KMono;
    }

// -----------------------------------------------------------------------------
// CG711PayloadFormatRead::SourceThreadLogoff
// Logout the source thread.
// -----------------------------------------------------------------------------
//
void CG711PayloadFormatRead::SourceThreadLogoff()
    {
    iClip->SourceThreadLogoff();
    }

// -----------------------------------------------------------------------------
// CG711PayloadFormatRead::NegotiateSourceL
// Negotiate Source.
// -----------------------------------------------------------------------------
//
void CG711PayloadFormatRead::NegotiateSourceL( MDataSink& aDataSink )
    {
    DP_G711_READ( "CG711PayloadFormatRead::NegotiateSourceL()" ) ;
    
    iDataPath = &aDataSink;
    iClip->NegotiateSourceL( *this );
    }

// -----------------------------------------------------------------------------
// CG711PayloadFormatRead::SourceThreadLogon
// Logon to the source thread.
// -----------------------------------------------------------------------------
//
TInt CG711PayloadFormatRead::SourceThreadLogon( MAsyncEventHandler& aEventHandler )
    {
    if ( iClip )
        {
        iClip->SourceThreadLogon( aEventHandler );
        return KErrNone;
        }
    else
        {
        return KErrNotReady;
        }
    }

// -----------------------------------------------------------------------------
// CG711PayloadFormatRead::SetSourceDataTypeCode
// Sets source datatype fourCC code
// -----------------------------------------------------------------------------
//
TInt CG711PayloadFormatRead::SetSourceDataTypeCode( TFourCC aSourceFourCC,
                                                    TMediaId aMediaId )
    {
    DP_G711_READ( "CG711PayloadFormatRead::SetSourceDataTypeCode()" );
    
    if ( KUidMediaTypeAudio != aMediaId.iMediaType ) 
        {
        return KErrNotSupported;
        }

    iFourCC = aSourceFourCC;

    iClip->SetSourceDataTypeCode( iFourCC, aMediaId );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CG711PayloadFormatRead::SourceDataTypeCode
// Returns the current datatype FourCC code.
// -----------------------------------------------------------------------------
//
TFourCC CG711PayloadFormatRead::SourceDataTypeCode( TMediaId aMediaId )
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
// CG711PayloadFormatRead::SinkDataTypeCode
// Returns the current datatype FourCC code.
// -----------------------------------------------------------------------------
//
TFourCC CG711PayloadFormatRead::SinkDataTypeCode( TMediaId aMediaId )
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
// CG711PayloadFormatRead::CreateSourceBufferL
// Create a source buffer for the given media and indicate in aReference if 
// buffer is created.
// -----------------------------------------------------------------------------
//
CMMFBuffer* CG711PayloadFormatRead::CreateSourceBufferL( TMediaId aMediaId,
                                                         TBool &aReference )
    {
    if ( KUidMediaTypeAudio != aMediaId.iMediaType )
        {
        User::Leave( KErrNotSupported );
        }

    // The source buffers belong to G711PayloadFormatRead not to datapath
    // aference should be set to ETrue and destroyed by G711PayloadFormatRead itself.
    aReference = ETrue;
    
    return iFrameBufferOne; 
    }

// -----------------------------------------------------------------------------
// CG711PayloadFormatRead::CreateSourceBufferL
// Create a source buffer for the given media, setting frame size to match
// the given sink buffer.
// -----------------------------------------------------------------------------
CMMFBuffer* CG711PayloadFormatRead::CreateSourceBufferL( TMediaId aMediaId,
                                                        CMMFBuffer& /*aSinkBuffer*/,
                                                        TBool& aReference )
    {
    if ( KUidMediaTypeAudio != aMediaId.iMediaType )
        {
        User::Leave( KErrNotSupported );
        }

    return CreateSourceBufferL( aMediaId, aReference );
    }

// -----------------------------------------------------------------------------
// CG711PayloadFormatRead::FillBufferL
// Fill Buffer.
// -----------------------------------------------------------------------------
//
void CG711PayloadFormatRead::FillBufferL( CMMFBuffer* aBuffer, 
                                          MDataSink* aConsumer, 
                                          TMediaId aMediaId )
    {
    if ( NULL == aBuffer ) 
        {
        User::Leave( KErrGeneral );
        }
        
    DP_G711_READ3( 
    "CG711PayloadFormatRead::FillBufferL() buffer 0x%x passed in with length %d bytes",
    aBuffer, aBuffer->BufferSize() );
         
    if ( KUidMediaTypeAudio != aMediaId.iMediaType )
        {
        User::Leave( KErrNotSupported );
        }

    if ( KUidMmfDataBuffer != aBuffer->Type() )
        {
        User::Leave( KErrNotSupported );
        }

    iDataPath = aConsumer;

    // aBuffer is a reference to those frame buffers that G711PayloadFormatRead owns
    aBuffer->SetLastBuffer( EFalse );

    if ( EBufferOne == iCurrentBuffer )
        {
        iFrameBufferTwo->SetStatus( EAvailable );
        }
    else
        {
        iFrameBufferOne->SetStatus( EAvailable );
        }

    if ( iBufferToReadExists )
        {
        // All decoded frame are not passed to the datapath
        iStateMachine->ChangeState( ESourceDataReady );
        }
    else
        {
        FillSourceBufferL(); // No payload ask for it
        } 
    }

// -----------------------------------------------------------------------------
// CG711PayloadFormatRead::Streams
// Return number of audio streams for the given media.
// -----------------------------------------------------------------------------
//
TUint CG711PayloadFormatRead::Streams( TUid aMediaType ) const
    {
    // Need to check aMediaType for audio
    if ( KUidMediaTypeAudio == aMediaType )
        {
        return 1;
        }
    else
        {
        return 0;
        }
    }

// -----------------------------------------------------------------------------
// CG711PayloadFormatRead::FrameTimeInterval
// Return the frame time interval for the given media
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds 
    CG711PayloadFormatRead::FrameTimeInterval( TMediaId aMediaId ) const
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
// CG711PayloadFormatRead::Duration
// Return the frame time interval for the given media.
// NOT SUPPORTED
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds 
    CG711PayloadFormatRead::Duration( TMediaId /*aMediaType*/ ) const
    {
    return TTimeIntervalMicroSeconds( TInt64( 0 ) );
    }

// -----------------------------------------------------------------------------
// CG711PayloadFormatRead::DecodePayload
// Decodes all audio frames from the received RTP payload buffer. Decoded
// audio frames are saved to the internal array so that audio frames can be
// requested one at a time with GetNextFrame() -method.
// No assumption about frame count in RTP packet is done.
// -----------------------------------------------------------------------------
//
TInt CG711PayloadFormatRead::DecodePayload( TDes8& aSourceBuffer )
    {
    DP_G711_READ2( "CG711PayloadFormatRead::DecodePayload SourceBufSize: %d",
        aSourceBuffer.Size() );
    DP_G711_READ2( "CG711PayloadFormatRead::DecodePayload SourceBufLength: %d",
        aSourceBuffer.Length() );
    
    iFrameIndex = 0;
    
    const TUint8* framePtr = aSourceBuffer.Ptr();
    const TUint8* endPtr = aSourceBuffer.Ptr() + aSourceBuffer.Size();
    
    TInt frames = aSourceBuffer.Size() / TInt( iCInfo.iFrameSize );
    
    DP_G711_READ2( "CG711PayloadFormatRead::DecodePayload FrameSize: %d",
        iCInfo.iFrameSize );
    DP_G711_READ2( "CG711PayloadFormatRead::DecodePayload Frames: %d",
        frames );
    
    // Construct pointers to frames in payload if not in CN mode
    if ( !iCnFrame )
        {
        while ( frames-- )
            {
            const TPtr8 bufPtr( const_cast<TUint8*>( framePtr ),
                iCInfo.iFrameSize, iCInfo.iFrameSize );
            iFrameArray.Append( bufPtr );
            
            framePtr += iCInfo.iFrameSize;
            if ( framePtr >= endPtr )
                {
                frames = 0;
                }
            }
        }
    else if ( aSourceBuffer.Size() && iCnFrame )
        {
        // If it is a CN frame, then we must do special handling. This is
        // because e.g Cisco kit sends 1 byte CN frames, thus we need to
        // expand the source buffer with zeroes.
        TInt targetLen = iCInfo.iFrameSize;
        targetLen = targetLen - aSourceBuffer.Size();
        
        DP_G711_READ2( "CG711PayloadFormatRead::DecodePayload CN frame size adjust: %d",
            targetLen );
        
        const TChar zero( '0' );
        aSourceBuffer.AppendFill( zero, targetLen );
        
        const TPtr8 bufPtr( const_cast<TUint8*>( framePtr ),
            iCInfo.iFrameSize, iCInfo.iFrameSize );
        
        iFrameArray.Append( bufPtr );
        }
    
    
    return iFrameArray.Count();
    }

// -----------------------------------------------------------------------------
// CG711PayloadFormatRead::GetNextFrameL
// Passes next audio frame decoded with DecodePayload(). Return ETrue if decoded
// frames are remaining.
// -----------------------------------------------------------------------------
//
TBool CG711PayloadFormatRead::GetNextFrame( TDes8& aToBuffer )
    {
    iFrameIndex++;
    
    DP_G711_READ3( "CG711PayloadFormatRead::GetNextFrame - FrameCount: %d, FrameIndex: %d",
        iFrameArray.Count(), iFrameIndex );
    
    const TInt frameCount( iFrameArray.Count() );
    
    if ( iFrameIndex == frameCount )
        {
        aToBuffer.Append( iFrameArray[iFrameIndex - 1] );
        iFrameArray.Reset();
        return EFalse;
        }
    else if ( iFrameIndex < frameCount )
        {
        aToBuffer.Append( iFrameArray[iFrameIndex - 1] );
        return ETrue;
        }
    else
        {
        return EFalse;
        }        
    }

// -----------------------------------------------------------------------------
// CG711PayloadFormatRead::ConfigurePayloadFormatL
// Configure payload decoding parameters.
// -----------------------------------------------------------------------------
//
void CG711PayloadFormatRead::ConfigurePayloadFormatL( const TDesC8& aConfigParams )
    {
    DP_G711_READ( "CG711PayloadFormatRead::ConfigurePayloadFormatL" );
    
    __ASSERT_ALWAYS( aConfigParams.Size() == sizeof( TMccCodecInfo ),
        User::Leave( KErrArgument ) );
    
    TMccCodecInfoBuffer infoBuffer;
    infoBuffer.Copy( aConfigParams );
    
    if ( !infoBuffer() .iIsUpdate )
        {
        iCInfo = infoBuffer();
        // Calculates frames/packet, frame size and frame time interval
        iFramesPerPacket = TInt8( iCInfo.iMaxPtime / iCInfo.iHwFrameTime );
        iCInfo.iFrameSize =
            static_cast<TUint>( KDefaultSampleRateInkHz * iCInfo.iHwFrameTime );
        iFrameTimeInterval = TInt64( iCInfo.iHwFrameTime );
    	
        // Create two frame buffers used in data transfer with datapath.
        // Space for two byte additional header needed by HW codec is reserved.
        iFrameBufferOne =
            CMMFDataBuffer::NewL( iCInfo.iFrameSize + KVoIPHeaderLength );
        iFrameBufferTwo =
            CMMFDataBuffer::NewL( iCInfo.iFrameSize + KVoIPHeaderLength );
        
        // PayloadBuffer contains data received from network
        TInt plSize = iCInfo.iFrameSize * iFramesPerPacket;
        DP_G711_READ3( "CG711PayloadFormatRead::ConfigurePayloadFormatL \
FramesPerPacket: %d, FrameSize: %d" , iFramesPerPacket, iCInfo.iFrameSize );
        
        if ( EGenRedUsed == iCInfo.iAlgoUsed )
            {
            DP_G711_READ2( "CG711PayloadFormatRead::ConfigurePayloadFormatL, RED LEVEL: %d",
                iCInfo.iRedundancyCount );
            
            if ( iCInfo.iRedundancyCount )
                {
                plSize *= iCInfo.iRedundancyCount;
                }
            
            CPayloadFormatRead* redDecoder 
                = static_cast<CPayloadFormatRead*>( iClip );
            
            TMccRedPayloadReadConfig config;
            config.iRedBlockCount = iCInfo.iRedundancyCount;
            config.iMaxPayloadSize = iCInfo.iFrameSize * iFramesPerPacket;
            config.iNumOfEncodings = 1;
            config.iRedPayloadType = iCInfo.iRedundantPayload;
            config.InitPayloadTypes();
            config.iEncPayloadTypes[0] = iCInfo.iPayloadType;
            TMccRedPayloadReadPckg pckg( config );
            redDecoder->ConfigurePayloadFormatL( pckg );
            }
        
        iSourceBuffer = CreateClipBufferL( plSize, iSourceBufOwnership );
        }
    else
        {
        DP_G711_READ( "CG711PayloadFormatRead::ConfigurePayloadFormatL - Update Configuration" );
        
        UpdateConfigurationL( infoBuffer() ); 
        }
    
    DP_G711_READ( "CG711PayloadFormatRead::ConfigurePayloadFormatL OUT" );
    }

// -----------------------------------------------------------------------------
// CG711PayloadFormatRead::UpdateConfigurationL
// Update payload decoder parameters
// -----------------------------------------------------------------------------
//
void CG711PayloadFormatRead::UpdateConfigurationL( TMccCodecInfo& aCodecInfo )
    {
    DP_G711_READ( "CG711PayloadFormatRead::UpdateConfigurationL" );
    
    DP_G711_READ2( "CG711PayloadFormatRead::UpdateConfigurationL maxptime: %d", 
                   aCodecInfo.iMaxPtime )
    DP_G711_READ2( "CG711PayloadFormatRead::UpdateConfigurationL hwframe: %d", 
                   aCodecInfo.iHwFrameTime )
    
    if ( iCInfo.iMaxPtime != aCodecInfo.iMaxPtime ||
        iCInfo.iHwFrameTime != aCodecInfo.iHwFrameTime )
        {
        DP_G711_READ( "CG711PayloadFormatRead::UpdateConfigurationL IS UPDATE" )
        
        iCInfo.iHwFrameTime = aCodecInfo.iHwFrameTime;
        iCInfo.iMaxPtime = aCodecInfo.iMaxPtime;
        
        iFrameTimeInterval = TInt64( iCInfo.iHwFrameTime );
        
        if ( iSourceBufOwnership )
            {
            delete iSourceBuffer;
            }

        iSourceBuffer = NULL;
        
        // Update sink buffers only if they are not big enough, extra space
        // does not matter really
        TUint updatedFrameSize = 
            static_cast<TUint>( KDefaultSampleRateInkHz * iCInfo.iHwFrameTime );
        if ( updatedFrameSize > iCInfo.iFrameSize )
            {
            DP_G711_READ( "CG711PayloadFormatRead::UpdateConfigurationL, \
creating new sink buffers" );
            
            CMMFDataBuffer* bufferOne = 
                CMMFDataBuffer::NewL( updatedFrameSize + KVoIPHeaderLength ); 
            CleanupStack::PushL( bufferOne ); 
            CMMFDataBuffer* bufferTwo =
                CMMFDataBuffer::NewL( updatedFrameSize + KVoIPHeaderLength );
            CleanupStack::PushL( bufferTwo ); 
            delete iFrameBufferOne;
            iFrameBufferOne = bufferOne;
            delete iFrameBufferTwo;
            iFrameBufferTwo = bufferTwo;
            CleanupStack::Pop( bufferTwo );
            CleanupStack::Pop( bufferOne );
            }
            
        iCInfo.iFrameSize = updatedFrameSize;
        
        // PayloadBuffer contains data received from network
        const TUint pktCount = aCodecInfo.iMaxPtime / iCInfo.iHwFrameTime;
        TInt plSize = iCInfo.iFrameSize * pktCount;
        if ( iCInfo.iRedundancyCount )
            {
            plSize *= iCInfo.iRedundancyCount;
            }
        
        DP_G711_READ3( "CG711PayloadFormatRead::UpdateConfigurationL \
pktCount: %d, iFrameSize: %d", pktCount, iCInfo.iFrameSize )
        
        iSourceBuffer = CreateClipBufferL( plSize, iSourceBufOwnership );
        iFrameArray.Reset();
        iFrameIndex = 0;
        iBufferToReadExists = EFalse;
        FillSourceBufferL();
        }
    
    DP_G711_READ( "CG711PayloadFormatRead::UpdateConfigurationL, exit" )
    }

// -----------------------------------------------------------------------------
// CG711PayloadFormatRead::CreateClipBufferL
// Creates buffer needed in data transfer with format readers clip.
// -----------------------------------------------------------------------------
//
CMMFDataBuffer* CG711PayloadFormatRead::CreateClipBufferL( 
        TUint aSize, TBool& aIsOwnBuffer )
    {
    DP_G711_READ( "CG711PayloadFormatRead::CreateClipBufferL" );
    CMMFDataBuffer* buffer( NULL );
    
    if ( iClip->CanCreateSourceBuffer() )
        {
        static_cast<CMMFFormatDecode*>( iClip )->SuggestSourceBufferSize( aSize );
        
        TBool reference( EFalse );
        CMMFBuffer* sourceBuf 
            = iClip->CreateSourceBufferL( KUidMediaTypeAudio, reference );
        TBool isSupportedBuf 
            = CMMFBuffer::IsSupportedDataBuffer( sourceBuf->Type() );
        TBool isOwnBuffer = reference ? EFalse : ETrue;
        
        if ( !isSupportedBuf )
            {
            if ( isOwnBuffer )
                {
                delete sourceBuf;
                }
            
            User::Leave( KErrNotSupported );
            }
        
        aIsOwnBuffer = isOwnBuffer;
        buffer = static_cast<CMMFDataBuffer*>( sourceBuf );
        }
    else
        {
        aIsOwnBuffer = ETrue;
        buffer = CMMFDataBuffer::NewL( aSize );
        }
    
    return buffer;
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File  
