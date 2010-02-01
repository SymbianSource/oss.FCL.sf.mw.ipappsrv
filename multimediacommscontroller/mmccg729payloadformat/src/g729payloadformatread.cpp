/*
* Copyright (c) 2004-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    PayloadFormat plugin capable to read RTP payload containing
*                G729 audio.
*
*/




// INCLUDE FILES

#include    "g729payloadformatread.h"
#include    "g729payloadformatutil.h"
#include    "mccrtpdatasource.h"
#include    "formatstatemachine.h"
#include    "streamformatter.h"
#include    "mccinternaldef.h"
#include    "mccredpayloadread.h"

#ifdef VOIP_TRACE_ENABLED
#include <voip_trace.h>
#endif

// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CG729PayloadFormatRead::CG729PayloadFormatRead
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CG729PayloadFormatRead::CG729PayloadFormatRead()
    {
    }

// -----------------------------------------------------------------------------
// CG729PayloadFormatRead::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CG729PayloadFormatRead::ConstructL( MDataSource* aSource )
    {
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x %x", MCC_TRACE, MCC_G711_PLF_READ_CONSTRUCTL, aSource );
    #endif
    __ASSERT_ALWAYS( aSource, User::Leave( KErrArgument ) );
    
    iClip = aSource;
    iBufferToReadExists = EFalse;

    iFourCC.Set( KMccFourCCIdG729 );
    
    // Initialize decoding state machine
    iStateMachine = CFormatDecodeStateMachine::NewL( this );
    iCurrentBuffer = EBufferOne;
    }
    
// -----------------------------------------------------------------------------
// CG729PayloadFormatRead::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CG729PayloadFormatRead* CG729PayloadFormatRead::NewL( MDataSource* aSource )
    {
    #ifdef TRACE_G729_PAYLOAD_FORMAT_READ
        RDebug::Print (_L ("CG729PayloadFormatRead::NewL()"));
    #endif
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x %x", MCC_TRACE, MCC_G711_PLF_READ_NEWL, aSource );
    #endif

    __ASSERT_ALWAYS( aSource, User::Leave( KErrArgument ) );

    CG729PayloadFormatRead* self = new (ELeave) CG729PayloadFormatRead;
    CleanupStack::PushL( self );
    self->ConstructL( aSource );
    CleanupStack::Pop( self );
    return self;
    }    

// -----------------------------------------------------------------------------
// CG729PayloadFormatRead::~CG729PayloadFormatRead()
// Destructor.
// -----------------------------------------------------------------------------
//
CG729PayloadFormatRead::~CG729PayloadFormatRead()
    {
    #ifdef TRACE_G729_PAYLOAD_FORMAT_READ
        RDebug::Print (_L ("CG729PayloadFormatRead::\
            ~CG729PayloadFormatRead()"));
    #endif
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x", MCC_TRACE, MCC_G711_PLF_READ_DESTRUCTOR );
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
    
    if ( iStateMachine )
        {
        iStateMachine->Cancel();
        delete iStateMachine;
        }

    iClip = NULL;
    iDataPath = NULL;
    iClip = NULL;    
    
    iFrameArray.Close();
    }


// -----------------------------------------------------------------------------
// CG729PayloadFormatRead::SendDataToSinkL
// Send full frame buffer to the DataPath.
// -----------------------------------------------------------------------------
//
void CG729PayloadFormatRead::SendDataToSinkL()
    {
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x", MCC_TRACE, MCC_G711_PLF_READ_SENDDATATOSINKL );
    #endif
          
    if ( EBufferOne == iCurrentBuffer )
        {
        #ifdef TRACE_G729_PAYLOAD_FORMAT_READ
            RDebug::Print (_L ("CG729PayloadFormatRead::\
                SendDataToSinkL() - BufferOne"));
        #endif

        iDataPath->BufferFilledL( iFrameBufferOne);
        iCurrentBuffer = EBufferTwo;
        // More payload buffers needed
        if ( iBufferToReadExists && !iFrameBufferOne->LastBuffer() )  
            {
            iStateMachine->ChangeState( ESourceDataReady );
            }
        }
    else
        {
        #ifdef TRACE_G729_PAYLOAD_FORMAT_READ
            RDebug::Print (_L ("CG729PayloadFormatRead::\
                SendDataToSinkL() - BufferTwo"));
        #endif

        iDataPath->BufferFilledL( iFrameBufferTwo );
        iCurrentBuffer = EBufferOne;
        // More payload buffers needed
        if ( iBufferToReadExists && !iFrameBufferTwo->LastBuffer() )
            {
            iStateMachine->ChangeState( ESourceDataReady );
            }
        }
           
    #ifdef TRACE_G729_PAYLOAD_FORMAT_READ
    RDebug::Print (_L ("CG729PayloadFormatRead::SendDataToSinkL() - DONE") );
    #endif
    }

// -----------------------------------------------------------------------------
// CG729PayloadFormatRead::FillSinkBuffer
// Fill SinkBuffer.
// -----------------------------------------------------------------------------
//
void CG729PayloadFormatRead::FillSinkBufferL()
    {
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x", MCC_TRACE, MCC_G711_PLF_READ_FILLSINKBUFFER );
    #endif  

    CMMFDataBuffer* curFrameBuffer;
    if ( EBufferOne == iCurrentBuffer )
        {
        #ifdef TRACE_G729_PAYLOAD_FORMAT_READ
            RDebug::Print( _L("G729PayloadFormatRead:FillSinkBuffer() EBufferOne \
            reqsize: %d"), iFrameBufferOne->RequestSize() );
        #endif
            
        curFrameBuffer = iFrameBufferOne;
        }
    else
        {
        #ifdef TRACE_G729_PAYLOAD_FORMAT_READ
            RDebug::Print( _L("G729PayloadFormatRead:FillSinkBuffer() EBufferOne \
            reqsize: %d"), iFrameBufferTwo->RequestSize() );
        #endif

        curFrameBuffer = iFrameBufferTwo;
        }

    TDes8& curFrameData = curFrameBuffer->Data();
    curFrameData.Zero();
                        
    // Put next frame decoded from RTP payload to the framebuffer
    this->GetNextFrame( curFrameData );
    
    HBufC8* buffer = NULL;
    buffer = curFrameData.Alloc();
    curFrameData.SetLength( KG729NumOfHeaderBytes );
    TBool sidFrame = EFalse;
    
    // Check the audio frame length, if we have one
    TInt frameLen = 0;
    if( buffer )
        {
        frameLen = buffer->Length();
        }
    
    if ( KG729FrameSize10ms == frameLen )
        {
        #ifdef TRACE_G729_PAYLOAD_FORMAT_READ
            RDebug::Print( _L ("CG729PayloadFormatRead::FillSinkBuffer ADD AUDIO HEADER") );
        #endif
        
        // Add 1st G.729 header byte for audio
        curFrameData[0] = KAudioFrameHeaderByte;
        sidFrame = EFalse;
        }
    else if( KG729CNFrameSize == frameLen )
        {
        #ifdef TRACE_G729_PAYLOAD_FORMAT_READ
            RDebug::Print( _L ("CG729PayloadFormatRead::FillSinkBuffer ADD SID HEADER") );
        #endif
        
        // Add 1st G.729 header byte for SID frame
        curFrameData[0] = KCNoiseFrameHeaderByte;
        sidFrame = ETrue;
        }
    else
        {
        #ifdef TRACE_G729_PAYLOAD_FORMAT_READ
            RDebug::Print( _L ("CG729PayloadFormatRead::FillSinkBuffer UNKNOWN FRAMELEN: %d"), frameLen );
        #endif
        
        // delete the buffer as something is badly wrong, this will be handled
        // a bit later on
        delete buffer;
        buffer = NULL;
        }

    if( buffer )
        {
        // Add 2nd G.729 header byte to the frame going to the decoder,
        // always zero
        curFrameData[1] = 0;
        
        this->DoBitUnPacking( *buffer, curFrameData, sidFrame );
        delete buffer;
        buffer = NULL;
        }
    else
        {
        #ifdef TRACE_G729_PAYLOAD_FORMAT_READ
            RDebug::Print( _L ("CG729PayloadFormatRead::FillSinkBuffer NO AUDIOBUFFER") );
        #endif
        
        // May be that the allocation failed or GetNextFrame() returned a
        // unknown size frame. In this case, fill the reset of the curFrameData
        // with zeroes like it would be a normal G.729 frame.
        curFrameData.SetLength( KG729CodecDecBufSize );
        
        // This will set the header bytes to zero meaning a 0bit/s frame, which
        // is fine with the decoder.
        curFrameData.FillZ();
        }
        
    curFrameBuffer->SetFrameNumber( iRecvHeader.iTimestamp + ( ( iFrameIndex - 1 )
        * TUint( iCInfo.iHwFrameTime * KG729SampleRate * 0.001 ) ) );
        
    curFrameBuffer->SetStatus( EFull );

    iStateMachine->ChangeState( EEmptyDataToSink );

    #ifdef TRACE_G729_PAYLOAD_FORMAT_READ
        RDebug::Print( _L("G729PayloadFormatRead::FillSinkBuffer() \
        BUFSZ: %d"), curFrameBuffer->BufferSize() );
    #endif
    }

// -----------------------------------------------------------------------------
// CG729PayloadFormatRead::FillSourceBufferL
// Send fill buffer request to the RTP Data Source.
// -----------------------------------------------------------------------------
//
void CG729PayloadFormatRead::FillSourceBufferL()
    {
    #ifdef TRACE_G729_PAYLOAD_FORMAT_READ
        RDebug::Print (_L ("CG729PayloadFormatRead::FillSourceBufferL()"));
    #endif
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x", MCC_TRACE, MCC_G711_PLF_READ_FILLSOURCEBUFFERL );
    #endif 

    // RtpSourceSink doesn't really need the Media Id.        
    iClip->FillBufferL( iSourceBuffer, this, iMediaId );
    }

// -----------------------------------------------------------------------------
// CG729PayloadFormatRead::SourcePrimeL
// Prime source.
// -----------------------------------------------------------------------------
//
void CG729PayloadFormatRead::SourcePrimeL()
    {
    #ifdef TRACE_G729_PAYLOAD_FORMAT_READ
        RDebug::Print( _L("CG729PayloadFormatRead::SourcePrimeL") );
    #endif
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x", MCC_TRACE, MCC_G711_PLF_READ_SOURCEPRIMEL );
    #endif
    
    iClip->SourcePrimeL();
    }

// -----------------------------------------------------------------------------
// CG729PayloadFormatRead::SourcePlayL
// Start playing.
// -----------------------------------------------------------------------------
//
void CG729PayloadFormatRead::SourcePlayL()
    {
    #ifdef TRACE_G729_PAYLOAD_FORMAT_READ
        RDebug::Print( _L("CG729PayloadFormatRead::SourcePlayL") );
    #endif
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x", MCC_TRACE, MCC_G711_PLF_READ_SOURCEPLAYL );
    #endif

    iClip->SourcePlayL();
    }

// -----------------------------------------------------------------------------
// CG729PayloadFormatRead::SourcePauseL
// Pause source.
// -----------------------------------------------------------------------------
//
void CG729PayloadFormatRead::SourcePauseL()
    {
    #ifdef TRACE_G729_PAYLOAD_FORMAT_READ
        RDebug::Print( _L("CG729PayloadFormatRead::SourcePauseL") );
    #endif
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x", MCC_TRACE, MCC_G711_PLF_READ_SOURCEPAUSEL );
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
// CG729PayloadFormatRead::SourceStopL
// Stop source
// -----------------------------------------------------------------------------
//
void CG729PayloadFormatRead::SourceStopL()
    {
    #ifdef TRACE_G729_PAYLOAD_FORMAT_READ
        RDebug::Print( _L("CG729PayloadFormatRead::SourceStopL") );
    #endif
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x", MCC_TRACE, MCC_G711_PLF_READ_SOURCESTOPL );
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

    #ifdef DEBUG_G729_INTERVAL
        RDebug::Print( _L("Processed %d G729 frames"), iFrameCount ); 
        iFrameCount = 0;
    #endif

    iClip->SourceStopL();
    }

// -----------------------------------------------------------------------------
// CG729PayloadFormatRead::BufferFilledL
// RTP data source has filled buffer.
// NOTE: Although redundancy is negotiated, sender may deside to send audio 
// packets without redundancy (RFC2198, ch5).
// -----------------------------------------------------------------------------
//
void CG729PayloadFormatRead::DataBufferFilledL( CMMFBuffer* aBuffer, const TRtpRecvHeader &aRecvHeader)
    {
    #ifdef TRACE_G729_PAYLOAD_FORMAT_READ
        RDebug::Print( _L("CG729PayloadFormatRead::DataBufferFilledL - TSTAMP: %u"),
        aRecvHeader.iTimestamp );
    #endif
    __ASSERT_ALWAYS( aBuffer, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( KUidMmfDataBuffer == aBuffer->Type(), 
        User::Leave( KErrNotSupported ) );
    __ASSERT_ALWAYS( iSourceBuffer == aBuffer, User::Leave( KErrArgument ) );
    
    // Copy received buffer
    TDes8& destDes = iSourceBuffer->Data();
    
    iRecvHeader = aRecvHeader;
    
    // Decode data blocks
    DecodePayload( destDes );
    
    // Whenever BufferFilledL is called from RtpSourceSink
    // Set the state machine to fillsinkbuffer
    if ( iFrameArray.Count() )
        {
        iBufferToReadExists = ETrue;
        iSourceBuffer->SetFrameNumber( iRecvHeader.iTimestamp );
        iStateMachine->ChangeState( ESourceDataReady );
        }
    else
        {
        #ifdef TRACE_G729_PAYLOAD_FORMAT_READ
        RDebug::Print( _L("CG729PayloadFormatRead::BufferFilledL, decode failed" ) );
        #endif
        
        FillSourceBufferL();
        }
    }

// -----------------------------------------------------------------------------
// CG729PayloadFormatRead::SampleRate
// Returns samplerate.
// -----------------------------------------------------------------------------
//
TUint CG729PayloadFormatRead::SampleRate()
    {
    return KG729SampleRate;
    }  

// -----------------------------------------------------------------------------
// CG729PayloadFormatRead::SetSampleRate
// Set samplerate.
// -----------------------------------------------------------------------------
//
TInt CG729PayloadFormatRead::SetSampleRate( TUint aSampleRate )
    {
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x %d", MCC_TRACE, MCC_G711_PLF_READ_SETSAMPLERATE,
            aSampleRate );
    #endif

    return ( KG729SampleRate == aSampleRate ) ? KErrNone : KErrNotSupported;
    }

// -----------------------------------------------------------------------------
// CG729PayloadFormatRead::NumChannels
// Returns number of channels.
// -----------------------------------------------------------------------------
//
TUint CG729PayloadFormatRead::NumChannels()
    {
    return KMono;
    }

// -----------------------------------------------------------------------------
// CG729PayloadFormatRead::SourceThreadLogon
// Logon to the source thread.
// -----------------------------------------------------------------------------
//
TInt CG729PayloadFormatRead::SourceThreadLogon(
    MAsyncEventHandler& aEventHandler )
    {
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x", MCC_TRACE, MCC_G711_PLF_READ_SOURCETHREADLOGOFF );
    #endif

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
// CG729PayloadFormatRead::SourceThreadLogoff
// Logout the source thread.
// -----------------------------------------------------------------------------
//
void CG729PayloadFormatRead::SourceThreadLogoff()
    {
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x", MCC_TRACE, MCC_G711_PLF_READ_NEGOTIATESOURCEL );
    #endif
    iClip->SourceThreadLogoff();
    }
    
// -----------------------------------------------------------------------------
// CG729PayloadFormatRead::NegotiateSourceL
// Negotiate Source.
// -----------------------------------------------------------------------------
//
void CG729PayloadFormatRead::NegotiateSourceL(MDataSink& aDataSink)
    {
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x %d", MCC_TRACE, MCC_G711_PLF_READ_SOURCETHREADLOGON,
            iCInfo.iRedundancyCount );
    #endif

    iDataPath = &aDataSink;
    iClip->NegotiateSourceL( *this );
    }

// -----------------------------------------------------------------------------
// CG729PayloadFormatRead::SetSourceDataTypeCode
// Sets source datatype fourCC code
// -----------------------------------------------------------------------------
//
TInt CG729PayloadFormatRead::SetSourceDataTypeCode( TFourCC aSourceFourCC,
                                                    TMediaId aMediaId )
    {
    #ifdef TRACE_G729_PAYLOAD_FORMAT_READ
        RDebug::Print (_L ("CG729PayloadFormatRead::SetSourceDataTypeCode()"));
    #endif
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x %d", MCC_TRACE, MCC_G711_PLF_READ_SETSOURCEDATATYPECODE,
            aSourceFourCC.FourCC() );
    #endif

    if ( KUidMediaTypeAudio != aMediaId.iMediaType ) 
        {
        return KErrNotSupported;
        }

    iFourCC = aSourceFourCC;
    iMediaId = aMediaId;

    iClip->SetSourceDataTypeCode( iFourCC, iMediaId );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CG729PayloadFormatRead::SourceDataTypeCode
// Returns the current datatype FourCC code.
// -----------------------------------------------------------------------------
//
TFourCC CG729PayloadFormatRead::SourceDataTypeCode( TMediaId aMediaId )
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
// CG729PayloadFormatRead::SinkDataTypeCode
// Returns the current datatype FourCC code.
// -----------------------------------------------------------------------------
//
TFourCC CG729PayloadFormatRead::SinkDataTypeCode( TMediaId aMediaId )
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
// CG729PayloadFormatRead::CreateSourceBufferL
// Create a source buffer for the given media and indicate in aReference if 
// buffer is created.
// -----------------------------------------------------------------------------
//
CMMFBuffer* CG729PayloadFormatRead::CreateSourceBufferL( TMediaId aMediaId,
                                                         TBool &aReference )
    {
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x", MCC_TRACE, MCC_G711_PLF_READ_CREATESOURCEBUFFERL );
    #endif

    if ( KUidMediaTypeAudio != aMediaId.iMediaType )
        {
        User::Leave( KErrNotSupported );
        }

    // The source buffers belong to G729PayloadFormatRead, not to datapath
    // reference should be set to ETrue and destroyed by G729PayloadFormatRead
    // itself.
    aReference = ETrue;
    return iFrameBufferOne; 
    }

// -----------------------------------------------------------------------------
// CG729PayloadFormatRead::CreateSourceBufferL
// Create a source buffer for the given media, setting frame size to match
// the given sink buffer.
// -----------------------------------------------------------------------------
//
CMMFBuffer* CG729PayloadFormatRead::CreateSourceBufferL(TMediaId aMediaId,
                                                        CMMFBuffer& 
                                                            /*aSinkBuffer*/,
                                                        TBool& aReference)
    {
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x", MCC_TRACE, MCC_G711_PLF_READ_CREATESOURCEBUFFERL );
    #endif

    if ( KUidMediaTypeAudio != aMediaId.iMediaType )
        {
        User::Leave( KErrNotSupported );
        }
        
    return CreateSourceBufferL( aMediaId, aReference );
    }

// -----------------------------------------------------------------------------
// CG729PayloadFormatRead::FillBufferL
// Fill Buffer.
// -----------------------------------------------------------------------------
//
void CG729PayloadFormatRead::FillBufferL( CMMFBuffer* aBuffer, 
    MDataSink* aConsumer, TMediaId aMediaId )
    {
    if ( !aBuffer ) 
        {
        User::Leave( KErrGeneral );
        }

    #ifdef TRACE_G729_PAYLOAD_FORMAT_READ
    RDebug::Print (_L ("CG729PayloadFormatRead::FillBufferL() \
    buffer 0x%x passed in with length %d bytes \n"),
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
    iMediaId = aMediaId;

    // aBuffer is a reference to those frame buffers that 
    // G729PayloadFormatRead owns
    aBuffer->SetLastBuffer( EFalse );

    if ( EBufferOne == iCurrentBuffer )
        {
        iFrameBufferTwo->SetStatus( EAvailable );
        }
    else
        {
        iFrameBufferOne->SetStatus( EAvailable );
        }

    if ( iBufferToReadExists ) // Payload has some frames not decoded
        {
        iStateMachine->ChangeState( ESourceDataReady );
        }
    else
        {
        #ifdef TRACE_G729_PAYLOAD_FORMAT_READ
            RDebug::Print( _L("CG729PayloadFormatRead::FillBufferL -> FillSourceBufferL") );
        #endif
        
        // No payload, ask for it
        FillSourceBufferL();
        } 
    }

// -----------------------------------------------------------------------------
// CG729PayloadFormatRead::Streams
// Return number of audio streams for the given media.
// -----------------------------------------------------------------------------
//
TUint CG729PayloadFormatRead::Streams(TUid aMediaType) const
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
// CG729PayloadFormatRead::FrameTimeInterval
// Return the frame time interval for the given media
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CG729PayloadFormatRead::FrameTimeInterval(
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
// CG729PayloadFormatRead::Duration
// Return the frame time interval for the given media.
// NOT SUPPORTED
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CG729PayloadFormatRead::Duration(
                                               TMediaId /*aMediaType*/) const
    {
    return TTimeIntervalMicroSeconds( TInt64( 0 ) );
    }

// -----------------------------------------------------------------------------
// CG729PayloadFormatRead::DecodePayload
// Decodes all audio frames from the received RTP payload buffer. Decoded
// audio frames are saved to the internal array so that audio frames can be
// requested one at a time with GetNextFrame() -method.
// No assumption about frame count in RTP packet is done.
// -----------------------------------------------------------------------------
//
void CG729PayloadFormatRead::DecodePayload( const TDesC8& aSourceBuffer )
    {
    #ifdef TRACE_G729_PAYLOAD_FORMAT_READ
        RDebug::Print ( _L ( "CG729PayloadFormatRead::DecodePayload - SourceBufSize: %d" ), aSourceBuffer.Length() );
    #endif    
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x %d", MCC_TRACE, MCC_G711_PLF_READ_DECODEPAYLOAD,
            aSourceBuffer.Size() );
    #endif
    
    const TUint8* framePtr = aSourceBuffer.Ptr();
    TInt payloadSize( aSourceBuffer.Length() );
    const TUint8* endPtr = aSourceBuffer.Ptr() + payloadSize;
    
    // Calculate parameters for frame ripping
    TInt frames( 0 );
    TInt frameSize( 0 );
    TInt remainder( 0 );    
    iFrameIndex = 0;

    if ( !( payloadSize % (TInt) iCInfo.iFrameSize ) )
        {
        // Pure audio frames
        frameSize = iCInfo.iFrameSize;
        frames = payloadSize / (TInt) iCInfo.iFrameSize;
        remainder = 0;
        
        #ifdef TRACE_G729_PAYLOAD_FORMAT_READ
            RDebug::Print( _L( "CG729PayloadFormatRead::DecodePayload - PURE AUDIO FRAMES: %d" ), frames );
        #endif        
        }
    else if ( KG729CNFrameSize == payloadSize )
        {
        // Payload consists of one CN frame
        frameSize = KG729CNFrameSize;
        frames = payloadSize / KG729CNFrameSize;
        remainder = 0;
        
        #ifdef TRACE_G729_PAYLOAD_FORMAT_READ
            RDebug::Print( _L( "CG729PayloadFormatRead::DecodePayload - ONE CNOISE FRAME: %d" ), frames );
        #endif
        }
    else
        {
        // Payload contains one CNF at the end of buffer
        frameSize = iCInfo.iFrameSize;
        frames = payloadSize / (TInt) iCInfo.iFrameSize;
        remainder = payloadSize % (TInt) iCInfo.iFrameSize;
        
        #ifdef TRACE_G729_PAYLOAD_FORMAT_READ
            RDebug::Print( _L( "CG729PayloadFormatRead::DecodePayload - AUDIO FRAME(s) + ONE CNOISE FRAME, FRAMES: %d, REMAINDER: %d" ), frames, remainder );
        #endif        
        }

    #ifdef TRACE_G729_PAYLOAD_FORMAT_READ
    RDebug::Print( _L( "CG729PayloadFormatRead::DecodePayload - framesize: %d, frames: %d" ), frameSize, frames );
    #endif
    
    // Construct pointers to frames in payload
    while ( frames-- )
        {
        TPtr8 bufPtr( const_cast<TUint8*>( framePtr ), frameSize, frameSize );
        iFrameArray.Append( bufPtr );

        framePtr += frameSize;
        
        if ( framePtr >= endPtr )
            {
            frames = 0;
            }
        }

    // Add possible comfort noise frame
    if ( remainder )
        {
        TPtr8 bufPtr( const_cast<TUint8*>(framePtr), KG729CNFrameSize, KG729CNFrameSize );
        iFrameArray.Append( bufPtr );
        }
    }

// -----------------------------------------------------------------------------
// CG729PayloadFormatRead::GetNextFrame
// Passes next audio frame decoded with DecodePayload()
// -----------------------------------------------------------------------------
//
void CG729PayloadFormatRead::GetNextFrame( TDes8& aToBuffer )
    {
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x %d", MCC_TRACE, MCC_G711_PLF_READ_GETNEXTFRAME,
            iFrameArray.Count(), iFrameIndex );
    #endif

    iFrameIndex++;
    
    #ifdef TRACE_G729_PAYLOAD_FORMAT_READ
        RDebug::Print( _L( "CG729PayloadFormatRead::GetNextFrame - FrameCount: %d, FrameIndex: %d" ),
            iFrameArray.Count(), iFrameIndex );
        if ( iFrameArray.Count() )
            {
            RDebug::Print( _L("CG729PayloadFormatRead::GetNextFrame SRC_LEN: %d"),
                iFrameArray[iFrameIndex - 1].Length() );
            RDebug::Print( _L("CG729PayloadFormatRead::GetNextFrame SRC_MAX: %d"),
                iFrameArray[iFrameIndex - 1].MaxLength() );
            }

        RDebug::Print( _L("CG729PayloadFormatRead::GetNextFrame DEST_LEN: %d"), aToBuffer.Length() );
        RDebug::Print( _L("CG729PayloadFormatRead::GetNextFrame DEST_MAX: %d"), aToBuffer.MaxLength() );
    #endif
    
    const TInt frmCount = iFrameArray.Count();
    
    if ( iFrameIndex < frmCount )
        {
        aToBuffer.Append( iFrameArray[iFrameIndex - 1] );
        iBufferToReadExists = ETrue;
        }
    else if ( iFrameIndex == frmCount )
        {
        aToBuffer.Append( iFrameArray[iFrameIndex - 1] );
        iFrameArray.Reset();
        iBufferToReadExists = EFalse;
        }
    else
        {
        #ifdef TRACE_G729_PAYLOAD_FORMAT_READ
            RDebug::Print( _L ( "CG729PayloadFormatRead::GetNextFrame FALSE" ));
        #endif         
        iBufferToReadExists = EFalse;
        }        
    }
    
// -----------------------------------------------------------------------------
// CG729PayloadFormatRead::DoBitUnPacking
// Does unpacking for bit-packed G729 RTP payload.
// -----------------------------------------------------------------------------
//
TInt CG729PayloadFormatRead::DoBitUnPacking( const TDesC8& aSourceBuf,
    TDes8& aDestBuf, TBool aIsCNoise ) const
    {
    if ( KG729CodecDecBufSize < aDestBuf.MaxLength() )
        {
        #ifdef TRACE_G729_PAYLOAD_FORMAT_READ
            RDebug::Print( _L("CG729PayloadFormatRead::DoBitUnPacking - DESTINATION BUF TOO SMALL!") );
        #endif
        
        return KErrArgument;
        }
    else
        {
        TStreamDecoder decoder;
        // Initialize and take into account the header bytes
        decoder.Initialize( const_cast<TUint8*>( aSourceBuf.Ptr() ), 0, 0 );
        aDestBuf.SetLength( KG729CodecDecBufSize );
        
        // Store header to temporary variables and zero the data in the
        // destination buffer. After that, restore the header bytes.
        TUint8 header1 = aDestBuf[0];
        TUint8 header2 = aDestBuf[1];
        aDestBuf.FillZ();
        aDestBuf[0] = header1;
        aDestBuf[1] = header2;
        
        TUint8 numOfParams( 0 );
        if ( aIsCNoise )
            {
            numOfParams = KG729NumOfCNoiseParams;
            }
        else
            {
            numOfParams = KG729NumOfAudioParams;
            }
            
        // Don't overwrite the header bytes
        TUint byteIndex( KG729NumOfHeaderBytes );
        TUint8 curParamBits( 0 );
        TUint8 decodedByte( 0 );
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

            if ( curParamBits <= KBitsInByte )
                {
                decodedByte = TUint8( decoder.Decode( curParamBits ) );
                aDestBuf[ byteIndex ] = decodedByte;
                aDestBuf[ byteIndex + 1 ] = 0;
                }
            else
                {
                decodedByte = TUint8( decoder.Decode( curParamBits - KBitsInByte ) );
                aDestBuf[ byteIndex + 1 ] = decodedByte;
                
                decodedByte = TUint8( decoder.Decode( KBitsInByte ) );
                aDestBuf[ byteIndex ] = decodedByte;
                }
                
            byteIndex += 2;
            }
        
        return KErrNone;            
        }
    }

// -----------------------------------------------------------------------------
// CG729PayloadFormatRead::ConfigurePayloadFormatL
// Configure payload decoding parameters.
// -----------------------------------------------------------------------------
//
void CG729PayloadFormatRead::ConfigurePayloadFormatL( const TDesC8& aConfigParams )
    {
    #ifdef TRACE_G729_PAYLOAD_FORMAT_READ
        RDebug::Print ( _L ( "CG729PayloadFormatRead::ConfigurePayloadFormatL" ) );
    #endif
    __ASSERT_ALWAYS( aConfigParams.Size() == sizeof( TMccCodecInfo ),
        User::Leave( KErrArgument ) );
    
    TMccCodecInfoBuffer infoBuffer;
    infoBuffer.Copy( aConfigParams );
    
    if ( !infoBuffer().iIsUpdate )
        {
        iCInfo = infoBuffer();  
        // Maximum number of frames in RTP payload
        iCInfo.iHwFrameTime = KG729FrameTimeInMs; // RFC3551
        TUint pktCount = iCInfo.iMaxPtime / iCInfo.iHwFrameTime;
        iCInfo.iFrameSize = KG729FrameSize10ms;
        iFrameTimeInterval = TInt64( iCInfo.iHwFrameTime * TInt8( pktCount ) );
        
        // Create two frame buffers used in data transfer with datapath.
        // Space for two byte additional header needed by HW codec is reserved.
        if ( iFrameBufferOne )
            {
            delete iFrameBufferOne;
            iFrameBufferOne = NULL;   
            }
        iFrameBufferOne = CMMFDataBuffer::NewL( KG729CodecDecBufSize );
        
        if ( iFrameBufferTwo )
            {
            delete iFrameBufferTwo; 
            iFrameBufferTwo = NULL;  
            }
        iFrameBufferTwo = CMMFDataBuffer::NewL( KG729CodecDecBufSize );
        
        // PayloadBuffer contains data received from network
        TInt plSize = iCInfo.iFrameSize * pktCount + KG729CNFrameSize;
        
        #ifdef TRACE_G729_PAYLOAD_FORMAT_READ
            RDebug::Print( _L ( "CG729PayloadFormatRead::ConfigurePayloadFormatL FramesPerPacket: %d, FrameSize: %d" ),
                pktCount, iCInfo.iFrameSize );
        #endif
        if ( EGenRedUsed == iCInfo.iAlgoUsed )
            {
            #ifdef TRACE_G729_PAYLOAD_FORMAT_READ
            RDebug::Print( _L("CG729PayloadFormatRead::ConfigurePayloadFormatL, RED LEVEL: %d"),
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
        
        if ( iSourceBuffer && iSourceBufOwnership )
            {
            delete iSourceBuffer; 
            iSourceBuffer = NULL; 
            iSourceBufOwnership = EFalse;
            }
        iSourceBuffer = CreateClipBufferL( plSize, iSourceBufOwnership );
        }
    else
        {
        UpdateConfigurationL( infoBuffer() );
        }    
    
    #ifdef TRACE_G729_PAYLOAD_FORMAT_READ
    RDebug::Print ( _L ( "CG729PayloadFormatRead::ConfigurePayloadFormatL() OUT" ) );
    #endif
    }

// -----------------------------------------------------------------------------
// CG729PayloadFormatRead::UpdateConfigurationL
// Update payload decoder parameters
// -----------------------------------------------------------------------------
//
void CG729PayloadFormatRead::UpdateConfigurationL( TMccCodecInfo& aCodecInfo )
    {
    #ifdef TRACE_G729_PAYLOAD_FORMAT_READ
        RDebug::Print( _L("CG729PayloadFormatRead::UpdateConfigurationL IN") );
    #endif
    
    if ( iCInfo.iMaxPtime != aCodecInfo.iMaxPtime )
        {
        // Maximum number of frames in RTP payload
        const TUint pktCount = aCodecInfo.iMaxPtime / iCInfo.iHwFrameTime;
        iFrameTimeInterval = TInt64( iCInfo.iHwFrameTime * TInt8( pktCount ) );
        
        // PayloadBuffer contains data received from network
        if ( iSourceBufOwnership )
            {
            delete iSourceBuffer;
            iSourceBufOwnership = EFalse;
            }
        iSourceBuffer = NULL;

        TInt plSize = iCInfo.iFrameSize * pktCount + KG729CNFrameSize;
        
        iSourceBuffer = CreateClipBufferL( plSize, iSourceBufOwnership );
        
        iCInfo.iMaxPtime = aCodecInfo.iMaxPtime;
        }       
    
    #ifdef TRACE_G729_PAYLOAD_FORMAT_READ
    RDebug::Print( _L("CG729PayloadFormatRead::UpdateConfigurationL OUT") );
    #endif
    }

// -----------------------------------------------------------------------------
// CG729PayloadFormatRead::CreateClipBufferL
// Creates buffer needed in data transfer with format readers clip.
// -----------------------------------------------------------------------------
//
CMMFDataBuffer* CG729PayloadFormatRead::CreateClipBufferL( 
        TUint aSize, TBool& aIsOwnBuffer )
    {
    #ifdef TRACE_G729_PAYLOAD_FORMAT_READ
    RDebug::Print( _L("CG729PayloadFormatRead::CreateClipBufferL" ) );
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
