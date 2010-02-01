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
*                ILBC audio.
*
*/




// INCLUDE FILES
#include    "ilbcpayloadformatread.h"
#include    "mccrtpdatasource.h"
#include    "ilbcpayloadformatutil.h"
#include    "mccinternaldef.h"
#include    "mccdef.h"
#include    "mccredpayloadread.h"

// MACROS
#ifdef _DEBUG
#define TRACE_ILBC_PAYLOAD_FORMAT_READ
#endif


// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CIlbcPayloadFormatRead::CIlbcPayloadFormatRead
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CIlbcPayloadFormatRead::CIlbcPayloadFormatRead () : 
        iMediaId( KUidMediaTypeAudio )
    {
    
    };

// -----------------------------------------------------------------------------
// CIlbcPayloadFormatRead::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CIlbcPayloadFormatRead::ConstructL( MDataSource* aSource )
    {
    __ASSERT_ALWAYS( aSource, User::Leave( KErrArgument ) );
    
    iClip = aSource;
    iBufferToReadExists = EFalse;
    
    iFourCC.Set( KMccFourCCIdILBC );
    
    // Initialize decoding state machine
    iStateMachine = CFormatDecodeStateMachine::NewL( this );
    iStateMachine->ChangeState( EDecodeIdle );
    
    iCurrentBuffer = EBufferOne;
    }
    
// -----------------------------------------------------------------------------
// CIlbcPayloadFormatRead::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CIlbcPayloadFormatRead* CIlbcPayloadFormatRead::NewL( MDataSource* aSource )
    {
    #ifdef TRACE_ILBC_PAYLOAD_FORMAT_READ
        RDebug::Print( _L("CIlbcPayloadFormatRead::NewL") );
    #endif
    
    __ASSERT_ALWAYS( aSource, User::Leave( KErrArgument ) );

    CIlbcPayloadFormatRead* self = new ( ELeave ) CIlbcPayloadFormatRead();
    CleanupStack::PushL( self );
    self->ConstructL( aSource );
    CleanupStack::Pop( self );
    return self;
    };

// -----------------------------------------------------------------------------
// CIlbcPayloadFormatRead::~CIlbcPayloadFormatRead()
// Destructor.
// -----------------------------------------------------------------------------
//
CIlbcPayloadFormatRead::~CIlbcPayloadFormatRead()
    {
    #ifdef TRACE_ILBC_PAYLOAD_FORMAT_READ
        RDebug::Print( _L("CIlbcPayloadFormatRead::~CIlbcPayloadFormatRead") );
    #endif
    
    delete iFrameBufferOne;
    delete iFrameBufferTwo;
    if ( iSourceBufOwnership )
        {
        delete iSourceBuffer;
        }
    else
        {
        iSourceBuffer = NULL;
        }
    
    delete iStateMachine;
    iClip = NULL;
	iFrameArray.Close();
    };
    

// -----------------------------------------------------------------------------
// CIlbcPayloadFormatRead::SendDataToSinkL
// Send full frame buffer to DataPath.
// -----------------------------------------------------------------------------
//
void CIlbcPayloadFormatRead::SendDataToSinkL()
    {
    if ( iCurrentBuffer == EBufferOne )
        {
        #ifdef TRACE_ILBC_PAYLOAD_FORMAT_READ
            RDebug::Print( _L( "CIlbcPayloadFormatRead::SendDataToSinkL - BufferOne") );
        #endif
        
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
        #ifdef TRACE_ILBC_PAYLOAD_FORMAT_READ
            RDebug::Print( _L( "CIlbcPayloadFormatRead::SendDataToSinkL - BufferTwo") );
        #endif

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
// CIlbcPayloadFormatRead::FillSinkBuffer
// Fill SinkBuffer.
// -----------------------------------------------------------------------------
//
void CIlbcPayloadFormatRead::FillSinkBufferL()
    {
    #ifdef TRACE_ILBC_PAYLOAD_FORMAT_READ
        RDebug::Print( _L( "CIlbcPayloadFormatRead::FillSinkBuffer" ) );
    #endif

    CMMFDataBuffer* curFrameBuffer;
    if ( EBufferOne == iCurrentBuffer )
        {
        #ifdef TRACE_ILBC_PAYLOAD_FORMAT_READ
            RDebug::Print( _L( "IlbcPayloadFormatRead: Filling EBufferOne reqsize: %d"),
                iFrameBufferOne->RequestSize() );
        #endif
            
        curFrameBuffer = iFrameBufferOne;
        }
    else
        {
        #ifdef TRACE_ILBC_PAYLOAD_FORMAT_READ
            RDebug::Print( _L("IlbcPayloadFormatRead: Filling EBufferTwo reqsize: %d"),
                iFrameBufferTwo->RequestSize() );
        #endif

        curFrameBuffer = iFrameBufferTwo;
        }
    
    TDes8& dataDes = curFrameBuffer->Data();
       
    dataDes.Zero();
    dataDes.SetLength( KiLBCNumOfHeaderBytes );

    // Put next frame decoded from RTP payload to the framebuffer
    iBufferToReadExists = GetNextFrame( dataDes );
    
    if ( iCnFrame )
        {
        #ifdef TRACE_ILBC_PAYLOAD_FORMAT_READ
            RDebug::Print( _L ("CIlbcPayloadFormatRead::FillSinkBuffer - ADD CNOISE FRAME HEADER") );
        #endif
        dataDes[0] = KCNoiseFrameHeaderByte;
        dataDes[1] = 0;
        }
    else
        {
        #ifdef TRACE_ILBC_PAYLOAD_FORMAT_READ
            RDebug::Print( _L ("CIlbcPayloadFormatRead::FillSinkBuffer - ADD AUDIO FRAME HEADER") );
        #endif        
        dataDes[0] = KAudioFrameHeaderByte;
        dataDes[1] = 0;
        }

    curFrameBuffer->SetFrameNumber( iRecvHeader.iTimestamp + ( ( iFrameIndex - 1 )
                                    * TUint( iCInfo.iHwFrameTime * KiLBCSampleRate * 0.001 ) ) );

    if ( KIlbcMode20msFrame == iCInfo.iCodecMode  )
        {
        dataDes.SetLength( KiLBCFrameSize20ms + KiLBCNumOfHeaderBytes );
        }
    else if( KIlbcMode30msFrame == iCInfo.iCodecMode )
        {
        dataDes.SetLength( KiLBCFrameSize30ms + KiLBCNumOfHeaderBytes );
        }
    else
        {
        // Make PC-LINT happy
        }        
        
    curFrameBuffer->SetStatus( EFull );
    iStateMachine->ChangeState( EEmptyDataToSink );

    #ifdef TRACE_ILBC_PAYLOAD_FORMAT_READ
        RDebug::Print( _L ("CIlbcPayloadFormatRead::FillSinkBuffer - DONE") );
    #endif
    }

// -----------------------------------------------------------------------------
// CIlbcPayloadFormatRead::FillSourceBufferL
// Send fill buffer request to RTP Data Source.
// -----------------------------------------------------------------------------
//
void CIlbcPayloadFormatRead::FillSourceBufferL()
    {
    #ifdef TRACE_ILBC_PAYLOAD_FORMAT_READ
        RDebug::Print ( _L("CIlbcPayloadFormatRead::FillSourceBufferL") );
    #endif

    // RtpSourceSink doesn't really need the Media Id.        
    iClip->FillBufferL( iSourceBuffer, this, iMediaId );
    }

// -----------------------------------------------------------------------------
// CIlbcPayloadFormatRead::SourcePrimeL
// Prime source.
// -----------------------------------------------------------------------------
//
void CIlbcPayloadFormatRead::SourcePrimeL()
    {
    #ifdef TRACE_ILBC_PAYLOAD_FORMAT_READ
        RDebug::Print (_L ("CIlbcPayloadFormatRead::SourcePrimeL()"));
    #endif
    
    iClip->SourcePrimeL();
    }

// -----------------------------------------------------------------------------
// CIlbcPayloadFormatRead::SourcePlayL
// Start playing.
// -----------------------------------------------------------------------------
//
void CIlbcPayloadFormatRead::SourcePlayL()
    {
    #ifdef TRACE_ILBC_PAYLOAD_FORMAT_READ
        RDebug::Print (_L ("CIlbcPayloadFormatRead::SourcePlayL()"));
    #endif

    iClip->SourcePlayL();
    }

// -----------------------------------------------------------------------------
// CIlbcPayloadFormatRead::SourcePauseL
// Pause source.
// -----------------------------------------------------------------------------
//
void CIlbcPayloadFormatRead::SourcePauseL()
    {
    #ifdef TRACE_ILBC_PAYLOAD_FORMAT_READ
        RDebug::Print( _L("CIlbcPayloadFormatRead::SourcePauseL") );
    #endif
    
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
// CIlbcPayloadFormatRead::SourceStopL
// Stop source.
// -----------------------------------------------------------------------------
//
void CIlbcPayloadFormatRead::SourceStopL()
    {
    #ifdef TRACE_ILBC_PAYLOAD_FORMAT_READ
        RDebug::Print( _L("CIlbcPayloadFormatRead::SourceStopL") );
    #endif
    
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
// CIlbcPayloadFormatRead::BufferFilledL
// RTP data source has filled buffer.
// NOTE: Although redundancy is negotiated, sender may deside to send audio 
// packets without redundancy (RFC2198, ch5).
// -----------------------------------------------------------------------------
//
void CIlbcPayloadFormatRead::DataBufferFilledL( CMMFBuffer* aBuffer,
                                            const TRtpRecvHeader& aRecvHeader )
    {
    #ifdef TRACE_ILBC_PAYLOAD_FORMAT_READ
        RDebug::Print (_L ("CIlbcPayloadFormatRead::DataBufferFilledL - TSTAMP: %u"),
            aRecvHeader.iTimestamp );
    #endif
    
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

    iNumOfFrames = DecodePayload( iSourceBuffer->Data() );

    // Whenever BufferFilledL is called from RtpSourceSink
    // Set the state machine to fillsinkbuffer
    if ( iNumOfFrames )
        {
        iBufferToReadExists = ETrue;
        iSourceBuffer->SetFrameNumber( iRecvHeader.iTimestamp );
        iStateMachine->ChangeState( ESourceDataReady );
        }
    else
        {
        #ifdef TRACE_ILBC_PAYLOAD_FORMAT_READ
        RDebug::Print ( _L("CIlbcPayloadFormatRead::BufferFilledL, decode failed" ) );
        #endif
        
        FillSourceBufferL();
        }
    }

// -----------------------------------------------------------------------------
// CIlbcPayloadFormatRead::SampleRate
// Returns samplerate.
// -----------------------------------------------------------------------------
//
TUint CIlbcPayloadFormatRead::SampleRate()
    {
    return KiLBCSampleRate;
    }  

// -----------------------------------------------------------------------------
// CIlbcPayloadFormatRead::SetSampleRate
// Set samplerate.
// -----------------------------------------------------------------------------
//
TInt CIlbcPayloadFormatRead::SetSampleRate( TUint aSampleRate )
    {
    return ( KiLBCSampleRate == aSampleRate ) ? KErrNone : KErrNotSupported;
    }

// -----------------------------------------------------------------------------
// CIlbcPayloadFormatRead::NumChannels
// Returns number of channels.
// -----------------------------------------------------------------------------
//
TUint CIlbcPayloadFormatRead::NumChannels()
    {
    return KMono;
    }

// -----------------------------------------------------------------------------
// CIlbcPayloadFormatRead::SourceThreadLogoff
// Logout the source thread.
// -----------------------------------------------------------------------------
//
void CIlbcPayloadFormatRead::SourceThreadLogoff()
    {
    iClip->SourceThreadLogoff();
    }
    
// -----------------------------------------------------------------------------
// CIlbcPayloadFormatRead::NegotiateSourceL
// Negotiate Source. Not used.
// -----------------------------------------------------------------------------
//
void CIlbcPayloadFormatRead::NegotiateSourceL( MDataSink& aDataSink )
    {
    iSink = &aDataSink;
    iClip->NegotiateSourceL( *this );
    }

// -----------------------------------------------------------------------------
// CIlbcPayloadFormatRead::SourceThreadLogon
// Logon the source thread.
// -----------------------------------------------------------------------------
//
TInt CIlbcPayloadFormatRead::SourceThreadLogon(
                                        MAsyncEventHandler& aEventHandler
                                        )
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
// CIlbcPayloadFormatRead::SetSourceDataTypeCode
// Sets source datatype fourCC code.
// -----------------------------------------------------------------------------
//
TInt CIlbcPayloadFormatRead::SetSourceDataTypeCode( TFourCC aSourceFourCC,
    TMediaId aMediaId )
    {
    #ifdef TRACE_ILBC_PAYLOAD_FORMAT_READ
        RDebug::Print (_L ("CIlbcPayloadFormatRead::SetSourceDataTypeCode()"));
    #endif
    
    if ( KUidMediaTypeAudio != aMediaId.iMediaType ) 
        {
        return KErrNotSupported;
        }

    iFourCC = aSourceFourCC;
    iMediaId = aMediaId;

    iClip->SetSourceDataTypeCode( iFourCC, aMediaId );
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CIlbcPayloadFormatRead::SourceDataTypeCode
// Returns the current datatype FourCC code.
// -----------------------------------------------------------------------------
//
TFourCC CIlbcPayloadFormatRead::SourceDataTypeCode( TMediaId aMediaId )
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
// CIlbcPayloadFormatRead::SinkDataTypeCode
// Returns the current datatype FourCC code.
// -----------------------------------------------------------------------------
//
TFourCC CIlbcPayloadFormatRead::SinkDataTypeCode( TMediaId aMediaId )
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
// CIlbcPayloadFormatRead::CreateSourceBufferL
// Create a source buffer for the given media and indicate in aReference 
// if buffer is created.
// -----------------------------------------------------------------------------
//
CMMFBuffer* CIlbcPayloadFormatRead::CreateSourceBufferL( TMediaId aMediaId,
    TBool &aReference )
    {
    if ( KUidMediaTypeAudio != aMediaId.iMediaType )
        {
        User::Leave( KErrNotSupported );
        }

    // the source buffers belong to IlbcPayloadFormatRead, not to datapath
    // reference should be set to ETrue and destroyed by IlbcPayloadFormatRead
    // itself.
    aReference = ETrue;
    return iFrameBufferOne; 
    }

// -----------------------------------------------------------------------------
// CIlbcPayloadFormatRead::CreateSourceBufferL
// Create a source buffer for the given media, setting frame size to match
// the given sink buffer.
// -----------------------------------------------------------------------------
//
CMMFBuffer* CIlbcPayloadFormatRead::CreateSourceBufferL(TMediaId aMediaId,
    CMMFBuffer& /*aSinkBuffer*/, TBool& aReference)
    {
    if ( KUidMediaTypeAudio != aMediaId.iMediaType )
        {
        User::Leave( KErrNotSupported );
        }
        
    return CreateSourceBufferL( aMediaId, aReference );
    }

// -----------------------------------------------------------------------------
// CIlbcPayloadFormatRead::FillBufferL
// Fill Buffer.
// -----------------------------------------------------------------------------
//
void CIlbcPayloadFormatRead::FillBufferL( CMMFBuffer* aBuffer, 
    MDataSink* aConsumer, TMediaId aMediaId )
    {
    if ( !aBuffer ) 
        {
        User::Leave( KErrGeneral );
        }

    #ifdef TRACE_ILBC_PAYLOAD_FORMAT_READ
    RDebug::Print( _L("CIlbcPayloadFormatRead::FillBufferL aBuffer 0x%x length %d bytes"),
        aBuffer, aBuffer->BufferSize() );
    #endif

    if ( KUidMediaTypeAudio != aMediaId.iMediaType )
        {
        User::Leave( KErrNotSupported );
        }

    if ( KUidMmfDataBuffer != aBuffer->Type() )
        {
        User::Leave( KErrNotSupported );
        }

    iDataPath = aConsumer;

    // aBuffer is a reference to those frame buffers that 
    // IlbcPayloadFormatRead owns
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
        // Time to ask more payload from datasource
        FillSourceBufferL();
        } 
    }

// -----------------------------------------------------------------------------
// CIlbcPayloadFormatRead::Streams
// Return number of audio streams for the given media.
// -----------------------------------------------------------------------------
//
TUint CIlbcPayloadFormatRead::Streams( TUid aMediaType ) const
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
// CIlbcPayloadFormatRead::FrameTimeInterval
// Return the frame time interval for the given media.
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CIlbcPayloadFormatRead::FrameTimeInterval(
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
// CIlbcPayloadFormatRead::Duration
// Return the frame time interval for the given media
// NOT SUPPORTED
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CIlbcPayloadFormatRead::Duration(
    TMediaId /*aMediaType*/) const
    {
    return TTimeIntervalMicroSeconds( TInt64( 0 ) );
    }
    
// -----------------------------------------------------------------------------
// CIlbcPayloadFormatRead::DecodePayload
// Decodes all audio frames from the received RTP payload buffer. Decoded
// audio frames are saved to the internal array so that audio frames can be
// requested one at a time with GetNextFrame() -method.
// No assumption about frame count in RTP packet is done.
// -----------------------------------------------------------------------------
//
TInt CIlbcPayloadFormatRead::DecodePayload( const TDesC8& aSourceBuffer )
    {
    #ifdef TRACE_ILBC_PAYLOAD_FORMAT_READ
        RDebug::Print( _L("CIlbcPayloadFormatRead::DecodePayload Src Size: %d" ),
            aSourceBuffer.Size() );
    #endif
    
    iFrameIndex = 0;
    
    const TUint8* framePtr = aSourceBuffer.Ptr();
    const TUint payloadSize( aSourceBuffer.Size() );
    const TUint8* endPtr = aSourceBuffer.Ptr() + payloadSize;
    
    // Calculate parameters for frame ripping
    TInt frames( 0 );
    TUint frameSize( 0 );
    
    // Frames of different modes MUST NOT be included within the same payload
    frameSize = payloadSize % KiLBCFrameSize20ms ? KiLBCFrameSize30ms : KiLBCFrameSize20ms;
    frames = payloadSize / frameSize;
    
    #ifdef TRACE_ILBC_PAYLOAD_FORMAT_READ
        RDebug::Print( _L("CILBCPayloadFormatRead::DecodePayload framesize: %d, frames: %d" ),
            frameSize, frames );
    #endif
    
    // Construct pointers to frames in payload
    while ( frames-- )
        {
        TPtr8 bufPtr( const_cast<TUint8*>(framePtr), frameSize, frameSize );
        iFrameArray.Append( bufPtr );

        framePtr += frameSize;
        
        if ( framePtr >= endPtr )
            {
            frames = 0;
            }
        }
        
    return iFrameArray.Count();
    }

// -----------------------------------------------------------------------------
// CIlbcPayloadFormatRead::GetNextFrameL
// Passes next audio frame decoded with DecodePayload(). Return ETrue if decoded
// frames are remaining.
// -----------------------------------------------------------------------------
//
TBool CIlbcPayloadFormatRead::GetNextFrame( TDes8& aToBuffer )
    {
    iFrameIndex++;
    
    #ifdef TRACE_ILBC_PAYLOAD_FORMAT_READ
        RDebug::Print( _L("CILBCPayloadFormatRead::GetNextFrame - FrameCount: %d"),
            iFrameArray.Count() );
        RDebug::Print( _L("CILBCPayloadFormatRead::GetNextFrame - FrameIndex: %d"),
            iFrameIndex );
    #endif    
    
    const TInt frmCount = iFrameArray.Count();
    const TInt toMax = aToBuffer.MaxLength();
    if ( iFrameIndex < frmCount &&
        ( iFrameArray[iFrameIndex - 1].MaxLength() <= toMax ) )
        {
        aToBuffer.Append( iFrameArray[iFrameIndex - 1] );
        return ETrue;
        }
    else if ( iFrameIndex == frmCount &&
        ( iFrameArray[iFrameIndex - 1].MaxLength() <= toMax ) )
        {
        #ifdef TRACE_ILBC_PAYLOAD_FORMAT_READ
        RDebug::Print( _L("CILBCPayloadFormatRead::GetNextFrame SRC_LEN: %d"),
            iFrameArray[iFrameIndex - 1].Length() );
        RDebug::Print( _L("CILBCPayloadFormatRead::GetNextFrame SRC_MAX: %d"),
            iFrameArray[iFrameIndex - 1].MaxLength() );
        RDebug::Print( _L("CILBCPayloadFormatRead::GetNextFrame DEST_LEN: %d"),
            aToBuffer.Length() );
        RDebug::Print( _L("CILBCPayloadFormatRead::GetNextFrame DEST_MAX: %d"),
            aToBuffer.MaxLength() );
        #endif

        aToBuffer.Append( iFrameArray[iFrameIndex - 1] );
        iFrameArray.Reset();
        return EFalse;
        }
    else
        {
        #ifdef TRACE_ILBC_PAYLOAD_FORMAT_READ
            RDebug::Print( _L ( "CILBCPayloadFormatRead::GetNextFrame FALSE" ));
        #endif
        
        iFrameArray.Reset(); 
        return EFalse;
        }        
    }

// -----------------------------------------------------------------------------
// CIlbcPayloadFormatRead::ConfigurePayloadFormatL
// Configure payload decoding parameters.
// -----------------------------------------------------------------------------
//
void CIlbcPayloadFormatRead::ConfigurePayloadFormatL(
    const TDesC8& aConfigParams )
    {
    #ifdef TRACE_ILBC_PAYLOAD_FORMAT_READ
        RDebug::Print( _L("CIlbcPayloadFormatRead::ConfigurePayloadFormatL IN") );
    #endif
    
    __ASSERT_ALWAYS( aConfigParams.Size() == sizeof( TMccCodecInfo ),
        User::Leave( KErrArgument ) );
        
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
    
    #ifdef TRACE_ILBC_PAYLOAD_FORMAT_READ
    RDebug::Print( _L("CIlbcPayloadFormatRead::ConfigurePayloadFormatL OUT") );
    #endif
    }

// -----------------------------------------------------------------------------
// CIlbcPayloadFormatRead::UpdateConfigurationL
// Update payload decoder parameters
// -----------------------------------------------------------------------------
//
void CIlbcPayloadFormatRead::UpdateConfigurationL(
    const TMccCodecInfo& aCodecInfo )
    {
    #ifdef TRACE_ILBC_PAYLOAD_FORMAT_READ
    RDebug::Print( _L("CIlbcPayloadFormatRead::UpdateConfigurationL IN") );
    #endif
    
    if ( iCInfo.iMaxPtime != aCodecInfo.iMaxPtime ||
         iCInfo.iPtime != aCodecInfo.iPtime ||
         iCInfo.iHwFrameTime != aCodecInfo.iHwFrameTime ||
         iCInfo.iBitrate != aCodecInfo.iBitrate )
        {
        // Reconfigure payload formatter and update the sourcebuffer fill
        // request so that multiplexer gets a update to the possibly changed
        // buffer.
        DoConfigurePayloadFormatL( aCodecInfo );
        FillSourceBufferL();
        }
    
    #ifdef TRACE_ILBC_PAYLOAD_FORMAT_READ
    RDebug::Print( _L("CIlbcPayloadFormatRead::UpdateConfigurationL OUT") );
    #endif
    }

// -----------------------------------------------------------------------------
// CIlbcPayloadFormatRead::DoConfigurePayloadFormatL
// -----------------------------------------------------------------------------
//
void CIlbcPayloadFormatRead::DoConfigurePayloadFormatL( const TMccCodecInfo& aCodecInfo )
    {
    __ASSERT_ALWAYS( aCodecInfo.iHwFrameTime != 0, User::Leave( KErrArgument ) );
    
    iCInfo = aCodecInfo;
    
    // Maximum number of frames in RTP payload
    const TUint pktCount = TUint8( iCInfo.iMaxPtime / iCInfo.iHwFrameTime );
    
    if ( KiLBCBitrateWith20ms == iCInfo.iBitrate )
        {
        iCInfo.iHwFrameTime = KiLBCFrameTime20ms;    
        iCInfo.iFrameSize = KiLBCFrameSize20ms;
        iFrameTimeInterval = (TInt64) KiLBCFrameTime20ms * pktCount;
        }
    else if ( KiLBCBitrateWith30ms == iCInfo.iBitrate )
        {
        iCInfo.iHwFrameTime = KiLBCFrameTime30ms;
        iCInfo.iFrameSize = KiLBCFrameSize30ms;
        iFrameTimeInterval = (TInt64) KiLBCFrameTime30ms * pktCount;
		}
    else
        {
        #ifdef TRACE_ILBC_PAYLOAD_FORMAT_READ
            RDebug::Print( _L("CIlbcPayloadFormatRead::DoConfigurePayloadFormatL KErrNotSupported") );
        #endif
        User::Leave( KErrNotSupported );
        }        
    
    // Create two frame buffers used in data transfer with datapath.
    // Space for two byte additional header needed by HW codec is reserved.
    // 30ms mode buffers can be used also for 20ms mode
    if ( !iFrameBufferOne && !iFrameBufferTwo )
        {
        iFrameBufferOne = 
            CMMFDataBuffer::NewL( KiLBCFrameSize30ms + KiLBCNumOfHeaderBytes );
        iFrameBufferTwo = 
            CMMFDataBuffer::NewL( KiLBCFrameSize30ms + KiLBCNumOfHeaderBytes );
        }
    
    // PayloadBuffer contains data received from network
    TInt plSize = iCInfo.iFrameSize * pktCount;
    
    #ifdef TRACE_ILBC_PAYLOAD_FORMAT_READ
        RDebug::Print( _L("CILBCPayloadFormatRead::DoConfigurePayloadFormatL FramesPerPacket: %d" ), pktCount );
        RDebug::Print( _L("CILBCPayloadFormatRead::DoConfigurePayloadFormatL FrameSize: %d" ), iCInfo.iFrameSize );
    #endif
    
    if ( EGenRedUsed == iCInfo.iAlgoUsed )
        {
        #ifdef TRACE_ILBC_PAYLOAD_FORMAT_READ
        RDebug::Print( _L("CG711PayloadFormatRead::DoConfigurePayloadFormatL, RED LEVEL: %d"),
            iCInfo.iRedundancyCount );
        #endif
        if ( iCInfo.iRedundancyCount )
            {
            plSize *= iCInfo.iRedundancyCount;
            }
        
        CPayloadFormatRead* redDecoder 
            = static_cast<CPayloadFormatRead*>( iClip );
        
        TMccRedPayloadReadConfig config;
        config.iRedBlockCount = iCInfo.iRedundancyCount;
        config.iMaxPayloadSize = iCInfo.iFrameSize * pktCount;
        config.iNumOfEncodings = 1;
        config.iRedPayloadType = iCInfo.iRedundantPayload;
        config.InitPayloadTypes();
        config.iEncPayloadTypes[0] = iCInfo.iPayloadType;
        TMccRedPayloadReadPckg pckg( config );
        redDecoder->ConfigurePayloadFormatL( pckg );
        }
    
    // PayloadBuffer contains data received from network
    if ( iSourceBufOwnership )
        {
        delete iSourceBuffer;
        }
        
    iSourceBuffer = NULL;
    iSourceBuffer = CreateClipBufferL( plSize, iSourceBufOwnership );
    }
    
// -----------------------------------------------------------------------------
// CIlbcPayloadFormatRead::CreateClipBufferL
// Creates buffer needed in data transfer with format readers clip.
// -----------------------------------------------------------------------------
//
CMMFDataBuffer* CIlbcPayloadFormatRead::CreateClipBufferL( 
        TUint aSize, TBool& aIsOwnBuffer )
    {
    #ifdef TRACE_ILBC_PAYLOAD_FORMAT_READ
        RDebug::Print( _L("CIlbcPayloadFormatRead::CreateClipBufferL") );
    #endif
    
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
