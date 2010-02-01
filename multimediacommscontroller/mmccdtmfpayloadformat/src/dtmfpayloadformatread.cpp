/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Contains functionality for DTMF and line event 
*                decoding and playing.
*
*/




// INCLUDES
#include "dtmfpayloadformatread.h"
#include "dtmfpayloaddecoder.h"
#include "dtmfeventpayloadinfo.h"
#include "dtmftonepayloadinfo.h"
#include "mccuids.hrh"

// ======== MEMBER FUNCTIONS ========


// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::CDTMFPayloadFormatRead
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CDTMFPayloadFormatRead::CDTMFPayloadFormatRead()
    {
    DP_DTMF_READ( _L("CDTMFPayloadFormatRead::CDTMFPayloadFormatRead") );
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatRead::ConstructL( MDataSource* aSource )
    {
    DP_DTMF_READ( _L("CDTMFPayloadFormatRead::ConstructL") );
    
    iClip = aSource;
    iBufferToReadExists = EFalse;

    // Initialize decoding state machine
    iStateMachine = CFormatDecodeStateMachine::NewL( this );
    iStateMachine->ChangeState( EDecodeIdle );

    iPayloadDecoder = CDTMFPayloadDecoder::NewL();

    iTonePlayer = CMdaAudioToneUtility::NewL( *this );
    TInt maxVol = iTonePlayer->MaxVolume();
    // Use intermediate volume value
    iTonePlayer->SetVolume( ( maxVol / 2 ) );
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::NewL
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CDTMFPayloadFormatRead* CDTMFPayloadFormatRead::NewL( MDataSource* aSource )
    {
    DP_DTMF_READ( _L("CDTMFPayloadFormatRead::NewL") );
    
    __ASSERT_ALWAYS( aSource, User::Leave( KErrArgument ) );

    CDTMFPayloadFormatRead* self = new( ELeave ) CDTMFPayloadFormatRead;

    CleanupStack::PushL( self );
    self->ConstructL( aSource );
    CleanupStack::Pop( self );

    return self;
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::~CDTMFPayloadFormatRead
// Destructor.
// ---------------------------------------------------------------------------
//
CDTMFPayloadFormatRead::~CDTMFPayloadFormatRead()
    {
    DP_DTMF_READ( _L("CDTMFPayloadFormatRead::~CDTMFPayloadFormatRead") );

    if ( iSourceBufOwnership )
        {
        delete iSourceBuffer;
        }
    
    iSourceBuffer = NULL;
        
    delete iPayloadDecoder;

    if ( iStateMachine )
        {
        iStateMachine->Cancel();
        delete iStateMachine;
        }

    delete iTonePlayer;

    // for PC_LINT #1740
    iClip = NULL;

    iEventPayloadInfo.Close();
    iTonePayloadInfo.Close();
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::Streams
// Not applicable for DTMF payload formatter. Pure virtual method
// implementation needed anyway.
// ---------------------------------------------------------------------------
//
TUint CDTMFPayloadFormatRead::Streams( TUid aMediaType ) const
    {
    DP_DTMF_READ( _L("CDTMFPayloadFormatRead::Streams") );

    if ( KUidMediaTypeAudio == aMediaType )
        {
        return 1;
        }
    else
        {
        return 0;
        }
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::FrameTimeInterval
// Not applicable for DTMF payload formatter. Pure virtual method
// implementation needed anyway.
// ---------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CDTMFPayloadFormatRead::FrameTimeInterval( 
    TMediaId /*aMediaType*/ ) const
    {
    DP_DTMF_READ( _L("CDTMFPayloadFormatRead::FrameTimeInterval") );

    return TTimeIntervalMicroSeconds( TInt64( 0 ) );
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::Duration
// Not applicable for DTMF payload formatter. Pure virtual method
// implementation needed anyway.
// ---------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CDTMFPayloadFormatRead::Duration( 
    TMediaId /*aMediaType*/ ) const
    {
    DP_DTMF_READ( _L("CDTMFPayloadFormatRead::Duration") );
    
    return TTimeIntervalMicroSeconds( TInt64( 0 ) );
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::CanCreateSourceBuffer
// SourceBuffer creation is not supported, because there is no need to
// exchange DTMF data with datapath. DTMF payload formatter handles DTMF
// playing independently through tone player.
// ---------------------------------------------------------------------------
//
TBool CDTMFPayloadFormatRead::CanCreateSourceBuffer()
    {
    DP_DTMF_READ( _L("CDTMFPayloadFormatRead::CanCreateSourceBuffer") );
    
    return EFalse;
    }
        
// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::CreateSourceBufferL
// Should not be used.
// ---------------------------------------------------------------------------
//
CMMFBuffer* CDTMFPayloadFormatRead::CreateSourceBufferL( TMediaId /*aMediaId*/,
    TBool& /*aReference*/ )
    {
    DP_DTMF_READ( _L("CDTMFPayloadFormatRead::CreateSourceBufferL") );
    
    User::Leave( KErrNotSupported );
    return NULL;
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::SourceDataTypeCode
// Returns source data type code.
// ---------------------------------------------------------------------------
//
TFourCC CDTMFPayloadFormatRead::SourceDataTypeCode( TMediaId aMediaId )
    {
    DP_DTMF_READ( _L("CDTMFPayloadFormatRead::SourceDataTypeCode() ") );
    
    if ( KUidMediaTypeAudio == aMediaId.iMediaType )
        {
        return TFourCC( KMccFourCCIdDTMF );
        }
    else
        {
         // Defaults to 'NULL' fourCC
        return TFourCC();
        }
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::SinkDataTypeCode
// Returns the current datatype FourCC code.
// ---------------------------------------------------------------------------
//
TFourCC CDTMFPayloadFormatRead::SinkDataTypeCode( TMediaId aMediaId )
    {
    DP_DTMF_READ( _L("CDTMFPayloadFormatRead::SinkDataTypeCode") );
    
    if ( KUidMediaTypeAudio == aMediaId.iMediaType )
        {
        return TFourCC( KMccFourCCIdDTMF );
        }
    else
        {
         // Defaults to 'NULL' fourCC
        return TFourCC();
        }
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::SetPayloadFormat
// Sets payload format used in DTMF decoding.
// ---------------------------------------------------------------------------
//
TInt CDTMFPayloadFormatRead::SetPayloadFormat( 
    TDTMFPayloadFormat aPayloadFormat )
    {
    DP_DTMF_READ2( _L("CDTMFPayloadFormatWrite::SetPayloadFormat - Format: %d"),
        aPayloadFormat );

    TInt err = iPayloadDecoder->SetPayloadFormat( aPayloadFormat );
    if ( KErrNone == err )
        {
        iPayloadFormat = aPayloadFormat;
        }

    return err;
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::FillBufferL
// No need for data exchange with DataPath. DTMF receiving and processing
// cycle is managed by SourcePrimeL(), SourcePlayL(), SourcePauseL(),
// SourceStopL() methods.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatRead::FillBufferL( CMMFBuffer* /*aBuffer*/, 
                                          MDataSink* /*aConsumer*/, 
                                          TMediaId /*aMediaId*/ )
    {
    DP_DTMF_READ( _L("CDTMFPayloadFormatRead::FillBufferL") );
    
    User::Leave( KErrNotSupported );
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::FillSourceBufferL
// Called by state machine. Send fill buffer request to the RTP Data Source.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatRead::FillSourceBufferL()
    {
    DP_DTMF_READ( _L("CDTMFPayloadFormatRead::FillSourceBufferL") );
    
    iClip->FillBufferL( iSourceBuffer, this, KUidMediaTypeAudio ); 
    }
    
// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::FillSinkBuffer
// Called by state machine. Decode payload to DTMF string and fill
// sink buffer.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatRead::FillSinkBufferL()
    {
    DP_DTMF_READ( _L("CDTMFPayloadFormatRead::FillSinkBufferL") );

    if ( iBufferToReadExists )
        {
        // Decode payload here and send data for continued handling in 
        // SendDataToSinkL()
        switch ( iPayloadFormat )
            {
            case EDTMFPayloadFormatEvent:
            case EDTMFPayloadFormatRedEvents:
                iPayloadDecoder
                    ->DecodeEventPayload( iSourceBuffer, iEventPayloadInfo );
                break;
            case EDTMFPayloadFormatTone:
                iPayloadDecoder
                    ->DecodeTonePayload( iSourceBuffer, iTonePayloadInfo );
                break;
            default:
                // Undefined Payload format
                DP_DTMF_READ( _L("Unsupported payload format!") );
                ASSERT( EFalse );
                break;
            }
        
        iStateMachine->ChangeState( EEmptyDataToSink );
        }
    else
        {
        DP_DTMF_READ( _L("No DTMF packets waiting at Data source.") );
        
        FillSourceBufferL();
        }
    }
    
// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::SendDataToSinkL
// Called by state machine. Send decoded payload to sound device for playback.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatRead::SendDataToSinkL()
    {
    DP_DTMF_READ( _L("CDTMFPayloadFormatRead::SendDataToSinkL") );
    
    if ( iBufferToReadExists )
        {
        iBufferToReadExists = EFalse;
        
        if ( EDTMFPayloadFormatEvent == iPayloadFormat 
             || EDTMFPayloadFormatRedEvents == iPayloadFormat )
            {
            HandleDTMFEventsL();
            }
        else if ( EDTMFPayloadFormatTone == iPayloadFormat )
            {
            HandleDTMFTones();
            }
        else
            {
            // undefined payload format
            DP_DTMF_READ( _L("Unsupported payload format!") );
            ASSERT( EFalse );
            }
        }
    
    FillSourceBufferL();
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::HandleDTMFEvents
// Handle and play DTMF events using tone player.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatRead::HandleDTMFEventsL()
    {
    DP_DTMF_READ3( _L("CDTMFPayloadFormatRead::HandleDTMFEventsL, EVENTCOUNT: %d, tick = %u"),
        iEventPayloadInfo.Count(), User::NTickCount() );
    
    const TUint eventCount( iEventPayloadInfo.Count() );
    
    if ( 1 == eventCount )
        {
        // Stop DTMF playing when end bit is set or new
        // event has been received
        if ( iEventPayloadInfo[0].EndBit() )
            {
            DP_DTMF_READ( _L("END BIT SET, CANCEL") );
            iTonePlayer->CancelPlay();
            }
        else
            {
            TBool prepareToPlay( EFalse );

            if ( iFirstPacket )
                {
                DP_DTMF_READ( _L("M BIT SET, CANCEL") );
                iFirstPacket = EFalse;
                prepareToPlay = ETrue;
                }
            else if ( iCurrentEvent != iEventPayloadInfo[0].Event()
                || iTimeStampOfPrevEvent != iEventPayloadInfo[0].TimeStamp() )
                {
                DP_DTMF_READ( _L("NEW EVENT, CANCEL") );
                prepareToPlay = ETrue;
                }
            else
                {
                // Make PC-LINT happy
                }                
            
            iTimeStampOfPrevEvent = iEventPayloadInfo[0].TimeStamp();

            if ( prepareToPlay )
                {
                DP_DTMF_READ( _L("PREPARING TO PLAY NEW DIGIT") );
                iTonePlayer->CancelPlay();
                iCurrentEvent = iEventPayloadInfo[0].Event();
                
                // Play DTMF until we have received final packet
                TBuf<1> buf;
                buf.Append( iEventPayloadInfo[0].Event() );
                iTonePlayer->SetDTMFLengths( KDTMFMaxTonePlaybackTime,
                                             KDTMFDefToneOffLengthInUs,
                                             KDTMFDefPauseLengthInUs );
                iTonePlayer->PrepareToPlayDTMFString( buf );
                }
            }            
        }
    else if ( 1 < eventCount )
        {
        // Play out contiguous (mutually exclusive) events
        TBuf<KMaxDtmfRedCount> buf;
        for ( TUint i = 0; i < KMaxDtmfRedCount; i++ )
            {
            buf.Append( iEventPayloadInfo[i].Event() );
            }
        
        // Playing with default settings
        iTonePlayer->PrepareToPlayDTMFString( buf );
        }
    else
        {
        // Some error, no events
        }
        
    // Reset for a new RTP packet and to save memory
    iEventPayloadInfo.Reset();
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::HandleDTMFTones
// NOT SUPPORTED. Handle and play DTMF tone payload.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatRead::HandleDTMFTones()
    {
    DP_DTMF_READ( _L("CDTMFPayloadFormatRead::HandleDTMFTones") );
    
    const TUint toneCount( iTonePayloadInfo.Count() );

    // Loop is needed for subscriber line event construction, which may have
    // longer cycles than one tone play out. E.g. "special information tone".
    // TBD: PlayDualToneL may stop previous tone playback
    for ( TUint i = 0; i < toneCount; i++ )
        {
        iTonePlayer->SetVolume( iTonePayloadInfo[i].Volume() );
        iTonePlayer->PrepareToPlayDualTone( 
                                        iTonePayloadInfo[i].LowFrequency(),
                                        iTonePayloadInfo[i].HighFrequency(),
                                        iTonePayloadInfo[i].ToneOnPeriod() );
        }
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::SourceThreadLogon
// Passes logon request to the RTP data source.
// ---------------------------------------------------------------------------
//
TInt CDTMFPayloadFormatRead::SourceThreadLogon(
    MAsyncEventHandler& aEventHandler )
    {
    DP_DTMF_READ( _L("CDTMFPayloadFormatRead::SourceThreadLogon") );

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

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::SourceThreadLogoff
// Passes log out request to the RTP data source.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatRead::SourceThreadLogoff( )
    {
    DP_DTMF_READ( _L("CDTMFPayloadFormatRead::SourceThreadLogoff") );
    
    iClip->SourceThreadLogoff( );
    }
        
// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::SourcePrimeL
// Passes prime transition to the RTP data source.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatRead::SourcePrimeL()
    {
    DP_DTMF_READ( _L("CDTMFPayloadFormatRead::SourcePrimeL") );
    
    iClip->SourcePrimeL();
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::SourcePlayL
// Passes play transition to the RTP data source.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatRead::SourcePlayL()
    {
    DP_DTMF_READ( _L("CDTMFPayloadFormatRead::SourcePlayL") );
    
    iClip->SourcePlayL();
    FillSourceBufferL();
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::SourcePauseL
// Passes pause transition to the RTP data source.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatRead::SourcePauseL()
    {
    DP_DTMF_READ( _L("CDTMFPayloadFormatRead::SourcePauseL") );
    
    iClip->SourcePauseL();
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::SourceStopL
// Passes stop transition to the RTP data source.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatRead::SourceStopL()
    {
    DP_DTMF_READ( _L("CDTMFPayloadFormatRead::SourceStopL") );
      
    iClip->SourceStopL();
    iStateMachine->Cancel( );
    iStateMachine->ChangeState( EDecodeIdle );
    iBufferToReadExists = EFalse;
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::BufferFilledL
// Called by DataSource after it has been filled the buffer.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatRead::DataBufferFilledL( CMMFBuffer* aBuffer, 
    const TRtpRecvHeader& aRtpHeader )
    {
    DP_DTMF_READ3( _L("CDTMFPayloadFormatRead::DataBufferFilledL, TSTAMP: %u, tick = %u"),
        aRtpHeader.iTimestamp, User::NTickCount() );
    
    if ( iCInfo.iPayloadType != aRtpHeader.iPayloadType )
        {
        // We are not interested about this RTP packet. Ask another.
        FillSourceBufferL();
        return;
        }
    
    // Do other checks, null ptr and buffer type.
    __ASSERT_ALWAYS( aBuffer, User::Leave( KErrArgument ) );
    if ( KUidMmfDataBuffer != aBuffer->Type() )
        {
        User::Leave( KErrNotSupported );
        }
    
    if ( iStateMachine->IsActive() )
        {
        DP_DTMF_READ( _L("CDTMFPayloadFormatRead::BufferFilledL - REJECTED") );
        // We are processing former events, do not mind incoming packets
        // but ask for more.
        FillSourceBufferL();
        }
    else
        {
        CMMFDataBuffer* dataBuffer = static_cast<CMMFDataBuffer*>( aBuffer );
        if ( !dataBuffer->Data().Length() )
            {
            // Do not handle packet, ask for more
            DP_DTMF_READ( _L("CDTMFPayloadFormatRead::BufferFilledL - NULL PAYLOAD") );
            FillSourceBufferL();
            }
        else
            {
            // If we have already received the first packet from this event,
            // then we should not decode and thus restart the playback because
            // this will do unnecessary stop, prepare and play sequence to
            // the tone player and causes distortions to DTMF playback.
            // Normally events with marker bit set in RTP header are sent three
            // times as specified in RFC2833 in order the implementation to be
            // robust.
            if ( !CompareFirstPacketRtpHeaders( aRtpHeader ) )
                {
                DP_DTMF_READ( _L("CDTMFPayloadFormatRead::BufferFilledL - NEW DATA") );
                
                iBufferToReadExists = ETrue;
                iFirstPacket = aRtpHeader.iMarker;
            
                // Copy received buffer
                iSourceBuffer->Data().Copy( dataBuffer->Data() );
                iPayloadDecoder->SetCurTimeStamp( aRtpHeader.iTimestamp );

                // Whenever BufferFilledL is called from RtpSourceSink
                // set the state machine to fillsinkbuffer.
                iStateMachine->ChangeState( ESourceDataReady );
                }
            else
                {
                DP_DTMF_READ( _L("CDTMFPayloadFormatRead::BufferFilledL - OLD DATA") );
                
                // Ask more data.
                FillSourceBufferL();
                }
            }
        }
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::SetSourcePrioritySettings
//
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatRead::SetSourcePrioritySettings( 
                            const TMMFPrioritySettings& aPrioritySettings )
    {
    DP_DTMF_READ( _L("CDTMFPayloadFormatRead::SetSinkPrioritySettings") );

    TMMFPrioritySettings settings = aPrioritySettings;
    settings.iState = EMMFStateTonePlaying;
    iTonePlayer->SetPriority( settings.iPriority, settings.iPref );
    }
    
// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::MatoPrepareComplete
// 
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatRead::MatoPrepareComplete( TInt aError )
    {
    if ( aError )
        {
        // Some error, do not try to play DTMF
        DP_DTMF_READ2( _L("CDTMFPayloadFormatRead::MatoPrepareComplete - ERR: %d "), aError );
        }
    else
        {
        DP_DTMF_READ2( _L("CDTMFPayloadFormatRead::MatoPrepareComplete, tick = %u"),
            User::NTickCount() );
        iTonePlayer->Play();
        }    
    }
    
// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::MatoPlayComplete
// 
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatRead::MatoPlayComplete( TInt aError )
    {
    if ( KErrNone != aError )
        {
        DP_DTMF_READ2( _L(
            "CDTMFPayloadFormatRead::MatoPlayComplete - ERR: %d"), aError );
        }
    else
        {
        DP_DTMF_READ2( _L("CDTMFPayloadFormatRead::MatoPlayComplete KErrNone, tick = %u"),
            User::NTickCount() );
        }
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::ConfigurePayloadFormatL
// Configure payload decoding parameters.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatRead::ConfigurePayloadFormatL( 
    const TDesC8& aConfigParams )
    {
    DP_DTMF_READ( _L("CDTMFPayloadFormatRead::ConfigurePayloadFormatL") );

    if ( aConfigParams.Size() == sizeof( TMccCodecInfo ) )
        {
        TMccCodecInfoBuffer infoBuffer;
        infoBuffer.Copy( aConfigParams );
        iCInfo = infoBuffer();
        
        if ( !iPayloadDecoder )
            {
            iPayloadDecoder = CDTMFPayloadDecoder::NewL();
            }

        if ( 0 < iCInfo.iRedundancyCount
            && KMaxDtmfRedCount >= TInt( iCInfo.iRedundancyCount ) )
            {
            if ( EGenRedUsed == iCInfo.iAlgoUsed )
                {
                iPayloadDecoder->SetPayloadFormat( 
                                                EDTMFPayloadFormatRedEvents );
                }
            else
                {
                User::Leave( KErrArgument );
                }
            }
        }
    else
        {
        User::Leave( KErrArgument );
        }
    
    CreateClipBufferL();
    
    DP_DTMF_READ( _L( "CDTMFPayloadFormatRead::ConfigurePayloadFormatL OUT") );
    }
    
// -----------------------------------------------------------------------------
// CDTMFPayloadFormatRead::CreateClipBufferL
// Creates buffer needed in data transfer with format readers clip.
// -----------------------------------------------------------------------------
//
void CDTMFPayloadFormatRead::CreateClipBufferL()
    {
    // If we have a source buffer already and it's size is correct, then this
    // one is a NOP
    if ( iSourceBuffer && KDTMFDefaultPayloadSize ==
         iSourceBuffer->Data().MaxLength() )
        {
        DP_DTMF_READ( _L("CDTMFPayloadFormatRead::CreateClipBufferL NOP") );
        
        return;
        }
    // We already have source buffer, own it and it can be reallocated.
    else if ( iSourceBuffer && iSourceBufOwnership &&
              KUidMmfDescriptorBuffer == iSourceBuffer->Type() )
        {
        DP_DTMF_READ( _L("CDTMFPayloadFormatRead::CreateClipBufferL ReAlloc") );
        
        CMMFDescriptorBuffer* desBuf =
            static_cast<CMMFDescriptorBuffer*>( iSourceBuffer );
        desBuf->ReAllocBufferL( KDTMFDefaultPayloadSize );
        desBuf = NULL;
        return;
        }
    // everything else...
    
    DP_DTMF_READ( _L("CDTMFPayloadFormatRead::CreateClipBufferL") );
    
    if ( iSourceBufOwnership )
        {
        delete iSourceBuffer;
        iSourceBuffer = NULL;
        }
    
    DP_DTMF_READ2( _L("CDTMFPayloadFormatRead::CreateClipBufferL 0x%x"), 
        iClip->DataSourceType().iUid );
    
    // We are normally dealing with multiplexer, so check against it.
    if ( iClip->CanCreateSourceBuffer() &&
         TUid::Uid( KDllUidMccMultiplexer ) == iClip->DataSourceType() )
        {
        static_cast<CMMFFormatDecode*>( iClip )->SuggestSourceBufferSize(
            KDTMFDefaultPayloadSize );
        
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
        
        iSourceBufOwnership = isOwnBuffer;
        iSourceBuffer = static_cast<CMMFDataBuffer*>( sourceBuf );
        }
    else
        {
        DP_DTMF_READ( _L("CDTMFPayloadFormatRead::CreateClipBufferL own buffer") );
        
        iSourceBufOwnership = ETrue;
        iSourceBuffer = CMMFDataBuffer::NewL( KDTMFDefaultPayloadSize );
        }
    
    // If we are already waiting for source data we must renew that request
    // as the pointer to our buffer goes dangling in other components, esp.
    // multiplexer.
    FillSourceBufferL();
    
    DP_DTMF_READ2( _L("CDTMFPayloadFormatRead::CreateClipBufferL ownership: %d"),
        iSourceBufOwnership );
    }

// ---------------------------------------------------------------------------
// From class MPayloadFormatRead.
//
// ---------------------------------------------------------------------------
//
TInt CDTMFPayloadFormatRead::HandleError( TInt aError, 
    TFormatDecodeState /*aState*/ )
    {
    return aError;
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::CompareFirstPacketRtpHeaders
// ---------------------------------------------------------------------------
//
TBool CDTMFPayloadFormatRead::CompareFirstPacketRtpHeaders(
    const TRtpRecvHeader& aRtpheader )
    {
    const TRtpSequence next = iReceivedHeader.iSeqNum + 1;
    TBool ret = EFalse;
    const TUint8 KMarkerSet = 1;
    
    if ( next == aRtpheader.iSeqNum && 
        iReceivedHeader.iPayloadType == aRtpheader.iPayloadType &&
        iReceivedHeader.iTimestamp == aRtpheader.iTimestamp &&
        KMarkerSet == iReceivedHeader.iMarker &&
        KMarkerSet == aRtpheader.iMarker )
        {
        DP_DTMF_READ( _L("CDTMFPayloadFormatRead::CompareFirstPacketRtpHeaders MATCH") )
        ret = ETrue;
        }
    // else we return EFalse as set when declaring ret.
    
    iReceivedHeader = aRtpheader;
    return ret;
    }

//  End of File
