/*
* Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Contains functionality for DTMF digit encoding and sending.
*
*/




// INCLUDES
#include <hal.h>
#include "mmccnetworksettings.h"
#include "dtmfpayloadformatwrite.h"
#include "dtmfpayloadencoder.h"
#include "dtmfeventpayloadinfo.h"
#include "dtmftonepayloadinfo.h"
#include "mccrtpdatasink.h"
#include "mccinternaldef.h"
#include "rtpdatasink.h"
#include "mccrtpmediaclock.h"
#include "mccinternalevents.h"
#include "dtmfhighresolutiontimer.h"

// CONSTANTS
const TUint8 KValidDTMFDigits[] = { '0', '1', '2', '3', '4', '5', '6', '7', 
    '8', '9', 'a', 'b', 'c', 'd', 'A', 'B', 'C', 'D', '*', '#', 'p', 'w'  };

const TInt  KSignalInbandDtmf = 1;
const TInt  KSignalOutbandDtmf = 2;

const TInt  KDurationNotApplicable = 1000000000;

// Pause in DTMF string sending when next digit to handle is 'p' char
const TUint KDTMFPauseLengthInUs    = 2500000;
const TInt  KNumOfFinalPackets      = 3;
const TReal KUsToSecFactor          = 0.000001;
const TReal KMsToSecFactor          = 0.001;
const TUint KDtmfDefSampleRate      = 8000;
const TUint KFactor1000             = 1000;
const TInt  KDefaultTickPeriodInMs  = 1;

// ======== MEMBER FUNCTIONS ========


// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::CDTMFPayloadFormatWrite
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CDTMFPayloadFormatWrite::CDTMFPayloadFormatWrite()
    :
    iSampleRate( KDtmfDefSampleRate ), 
    iTickPeriod( KDefaultTickPeriodInMs )
    {
    DP_DTMF_WRITE( _L("CDTMFPayloadFormatWrite::CDTMFPayloadFormatWrite") );
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatWrite::ConstructL( MDataSink* aSink )
    {
    DP_DTMF_WRITE( _L("CDTMFPayloadFormatWrite::ConstructL") );
    
    iFourCC = TFourCC( KMccFourCCIdDTMF );
    
    // Set data sink
    CMccRtpDataSink* tmp = static_cast<CMccRtpDataSink*>( aSink );
    iRtpDataSink = static_cast<MMccRtpDataSink*>( tmp );
    iClip = aSink;
    
    // Allocate buffer for payload encoder
    iSinkBuffer = CMMFDataBuffer::NewL( KDTMFDefaultPayloadSize );
    iPayloadEncoder = CDTMFPayloadEncoder::NewL();
    
    // Initialize state machine
    iStateMachine = CDtmfEncStateMachine::NewL( *this );
    
    TUint validDigitCount = sizeof ( KValidDTMFDigits ) / sizeof ( TUint8 );
    for ( TUint i = 0; i < validDigitCount; i++ )
        {
        iValidDigits.Append( KValidDTMFDigits[ i ] );
        }
    
    TInt err = HAL::Get( HAL::ENanoTickPeriod, iTickPeriod );
    if ( KErrNone != err )
        {
        DP_DTMF_WRITE2( _L("CDTMFPayloadFormatWrite::ConstructL HAL ERR: %d"),
            err );
        }
    
    iTickPeriod = TInt( iTickPeriod * KMsToSecFactor );
    iSendTimer = CDtmfHighResTimer::NewL( CActive::EPriorityHigh );
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::NewL
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CDTMFPayloadFormatWrite* CDTMFPayloadFormatWrite::NewL( MDataSink* aSink )
    {
    DP_DTMF_WRITE( _L("CDTMFPayloadFormatWrite::NewL") );
    
    if ( !aSink ) 
        {
        User::Leave ( KErrArgument );
        }

    if ( KMccRtpSinkUid != aSink->DataSinkType() )
        {
        User::Leave ( KErrNotSupported );
        }
    
    CDTMFPayloadFormatWrite* self = new( ELeave ) CDTMFPayloadFormatWrite;
    
    CleanupStack::PushL( self );
    self->ConstructL( aSink );
    CleanupStack::Pop( self );

    return self;
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::~CDTMFPayloadFormatWrite
// Destructor.
// ---------------------------------------------------------------------------
//
CDTMFPayloadFormatWrite::~CDTMFPayloadFormatWrite()
    {
    DP_DTMF_WRITE( _L("CDTMFPayloadFormatWrite::~CDTMFPayloadFormatWrite") );
    
    delete iSinkBuffer;
    delete iPayloadEncoder;
    delete iSendTimer;
    delete iStateMachine;
    iEventHandler = NULL;
    iRtpDataSink = NULL;
    
    // Media clock is not owned
    if ( iRtpMediaClock )
        {
        iRtpMediaClock->UnregisterMediaFormat( iKey );
        iRtpMediaClock = NULL;
        }
    
    iSendBuffer.Close();
    iValidDigits.Close();
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::HandleTimerEventL
// STATIC METHOD. Passes timer events to the class instance interested
// about them.
// ---------------------------------------------------------------------------
//
TInt CDTMFPayloadFormatWrite::HandleTimerEventL( TAny* aObjectPtr )
    {
    if ( aObjectPtr )
        {
        return static_cast<CDTMFPayloadFormatWrite*>( aObjectPtr )->
                                                        DoHandleTimerEventL();
        }
    else
        {
        return KErrArgument;
        }
    }
    
// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::SinkThreadLogon
// Passes the logon command to the sink clip.
// ---------------------------------------------------------------------------
//
TInt CDTMFPayloadFormatWrite::SinkThreadLogon( 
    MAsyncEventHandler& aEventHandler )
    {
    DP_DTMF_WRITE( _L("CDTMFPayloadFormatWrite::SinkThreadLogon") );
    
    iEventHandler = &aEventHandler;
    return iClip->SinkThreadLogon( aEventHandler );
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::SinkThreadLogoff
// Passes the logoff command to the sink clip.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatWrite::SinkThreadLogoff()
    {
    DP_DTMF_WRITE( _L("CDTMFPayloadFormatWrite::SinkThreadLogoff") );
    
    iClip->SinkThreadLogoff();
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::CanCreateSinkBuffer
// EmptyBufferL() is not used.
// ---------------------------------------------------------------------------
//
TBool CDTMFPayloadFormatWrite::CanCreateSinkBuffer()
    {
    return EFalse;
    }
    
// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::CreateSinkBufferL
// Create a sink buffer.
// ---------------------------------------------------------------------------
//
CMMFBuffer* CDTMFPayloadFormatWrite::CreateSinkBufferL( 
    TMediaId /*aMediaId*/,
    TBool& /*aReference*/ )
    {
    DP_DTMF_WRITE( _L("CDTMFPayloadFormatWrite::CreateSinkBufferL") );
    
    User::Leave( KErrNotSupported );
    // PC_LINT #527, for compiler warning
    return NULL;
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::SetSinkDataTypeCode
// Set the sink data type to the given FourCC code for the given media.
// ---------------------------------------------------------------------------
//
TInt CDTMFPayloadFormatWrite::SetSinkDataTypeCode( TFourCC aSinkFourCC, 
                                                   TMediaId aMediaId )
    {
    DP_DTMF_WRITE( _L("CDTMFPayloadFormatWrite::SetSinkDataTypeCode") );

    if ( KUidMediaTypeAudio != aMediaId.iMediaType )
        {
        return KErrNotSupported;
        }
    
    iFourCC = aSinkFourCC;
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::SinkDataTypeCode
// Return the sink data type (four CC code) for the given media.
// ---------------------------------------------------------------------------
//
TFourCC CDTMFPayloadFormatWrite::SinkDataTypeCode( TMediaId aMediaId )
    {
    DP_DTMF_WRITE( _L("CDTMFPayloadFormatWrite::SinkDataTypeCode") );
    
    if ( KUidMediaTypeAudio == aMediaId.iMediaType )
        {
        return iFourCC;
        }
    else
        {
        // Defaults to 'NULL' fourCC
        return TFourCC();
        }
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::FrameTimeInterval
// Gets audio frame size in milliseconds.
// ---------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CDTMFPayloadFormatWrite::FrameTimeInterval( 
    TMediaId aMediaType ) const
    {
    DP_DTMF_WRITE( _L("CDTMFPayloadFormatWrite::FrameTimeInterval") );
    
    if ( KUidMediaTypeAudio == aMediaType.iMediaType )
        {
        return TTimeIntervalMicroSeconds( TInt64( 0 ) );
        }
    else
        {
        return TTimeIntervalMicroSeconds( TInt64( 0 ) );
        }
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::Duration
// Gets the duration of the sink clip for the specified media ID.
// ---------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CDTMFPayloadFormatWrite::Duration( 
    TMediaId /*aMediaType*/ ) const
    {
    DP_DTMF_WRITE( _L("CDTMFPayloadFormatWrite::Duration") );
    
    return TTimeIntervalMicroSeconds( TInt64( KDurationNotApplicable ) );
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::BufferEmptiedL
// Called by RTP data sink. Indicates that DTMF data is sent to the network.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatWrite::BufferEmptiedL( CMMFBuffer* /*aBuffer*/ )
    {
    DP_DTMF_WRITE( _L("CDTMFPayloadFormatWrite::BufferEmptiedL") );
    
    // Nothing to do
    }
    
// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::SinkPrimeL
// Passes prime transition to the RTP data sink.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatWrite::SinkPrimeL()
    {
    DP_DTMF_WRITE( _L("CDTMFPayloadFormatWrite::SinkPrimeL") );
    
    iClip->SinkPrimeL();
    
    // If the possible state machine in clip does not leave, let dtmf payload
    // formatter also allow re-priming from any state.
    CancelDTMFStringSending();
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::SinkPlayL
// Passes play transition to the RTP data sink and resets state variables.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatWrite::SinkPlayL()
    {
    DP_DTMF_WRITE( _L("CDTMFPayloadFormatWrite::SinkPlayL") );
    
    iClip->SinkPlayL();
    }
    
// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::SinkPauseL
// Passes pause transition to the RTP data sink.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatWrite::SinkPauseL()
    {
    DP_DTMF_WRITE( _L("CDTMFPayloadFormatWrite::SinkPauseL") );
    
    CancelDTMFStringSending();
    iClip->SinkPauseL();
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::SinkStopL
// Passes stop transition to the RTP data sink.
// ---------------------------------------------------------------------------
//  
void CDTMFPayloadFormatWrite::SinkStopL()
    {
    DP_DTMF_WRITE( _L("CDTMFPayloadFormatWrite::SinkStopL") );
    
    CancelDTMFStringSending();
    iClip->SinkStopL();
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::EmptyBufferL
// Not used because DTMF payload formatter generates DTMF independently.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatWrite::EmptyBufferL( CMMFBuffer* /*aBuffer*/, 
                                            MDataSource* /*aSupplier*/, 
                                            TMediaId /*aMediaId*/ )
    {
    DP_DTMF_WRITE( _L("CDTMFPayloadFormatWrite::EmptyBufferL") );
    
    User::Leave( KErrNotSupported );
    }
                    
// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::PayloadFormat
// Returns payload format used in DTMF encoding.
// ---------------------------------------------------------------------------
//
TDTMFPayloadFormat CDTMFPayloadFormatWrite::PayloadFormat() const
    {
    DP_DTMF_WRITE( _L("CDTMFPayloadFormatWrite::PayloadFormat") );
    
    return iPayloadFormat;
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::SetPayloadFormat
// Sets payload format used in DTMF encoding.
// ---------------------------------------------------------------------------
//
TInt CDTMFPayloadFormatWrite::SetPayloadFormat( 
    TDTMFPayloadFormat aPayloadFormat )
    {
    DP_DTMF_WRITE2( _L("CDTMFPayloadFormatWrite::SetPayloadFormat - Format: %d"), aPayloadFormat );
    
    TInt err = iPayloadEncoder->SetPayloadFormat( aPayloadFormat );
    if ( KErrNone == err )
        {
        iPayloadFormat = aPayloadFormat;
        }

    return err;
    }
            
// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::StartDTMFToneL
// Starts a timer which sends specified DTMF digit periodically until 
// StopDTMFTone() is called.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatWrite::StartDTMFToneL( const TChar aTone )
    {
    TUint32 tickCount( User::NTickCount() );
    
    DP_DTMF_WRITE3(  _L("CDTMFPayloadFormatWrite::StartDTMFToneL aTone = %c, tick = %u"),
        static_cast<TUint>( aTone ), tickCount );
    
    DP_DTMF_WRITE2( _L("CDTMFPayloadFormatWrite::StartDTMFToneL enc state %d"),
        iStateMachine->State() );
    
    __ASSERT_ALWAYS( IsValidDigit( aTone ), User::Leave( KErrArgument ) );
    
    iSendingMode = EModeManual;
    
    TSendBufferElement element( aTone, tickCount );
    iSendBuffer.AppendL( element );
    
    if ( EStateSendingIdle == iStateMachine->State() )
        {
        if ( KSignalOutbandDtmf == iGenerationMode )
            {
            iSinkBuffer->Data().FillZ( KDTMFDefaultPayloadSize );
            }

        iStateMachine->ChangeStateTo( EStateEncodeNextDigit );
        }
    else
        {
        DP_DTMF_WRITE( 
            _L("CDTMFPayloadFormatWrite::StartDTMFToneL - REQ BUFFERED") );
        }
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::StopDTMFToneL
// Stops the timer for DTMF digit update packet sending. Starts a timer for 
// sending Final Packet three times according to RFC 2833, section 2.5.1.4.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatWrite::StopDTMFToneL()
    {
    TUint32 tickCount( User::NTickCount() );
    DP_DTMF_WRITE2( _L("CDTMFPayloadFormatWrite::StopDTMFTone, tick = %u"),
        tickCount );
    
    // set stop time to the first unupdated element
    TBool elementFound( EFalse );
    TInt index( 0 );
    while ( !elementFound && index < iSendBuffer.Count() )
        {
        if ( iSendBuffer[index].StopTime() == 0 )
            {
            iSendBuffer[index].SetStopTime( tickCount );
            if ( iSendBuffer[index].StopTime() == 0 )
                {
                // mark stop time as updated to prevent update of the same 
                // element in future
                iSendBuffer[index].SetStopTime( 1 );
                }
            
            elementFound = ETrue;
            }
        
        index++;
        }
    
    if ( EStateToneOn == iStateMachine->State() )
        {
        if ( KSignalOutbandDtmf == iGenerationMode )
            {
            if ( EDTMFPayloadFormatEvent == iPayloadFormat
                || EDTMFPayloadFormatRedEvents == iPayloadFormat )
                {
                // Send final packet with "E" bit set. According to 
                // RFC 2833 2.5.1.4 final packet SHOULD be sent three times.
                iPayloadEncoder
                    ->UpdateEventPayload( ETrue, iToneDuration, iSinkBuffer );
                iStateMachine->ChangeStateTo( EStateToneOff );
                }
            else if ( EDTMFPayloadFormatTone == iPayloadFormat )
                {
                // Tone Payload Format doesn't have final packet redundancy
                iPayloadEncoder->UpdateTonePayload( iToneDuration, 
                                                                iSinkBuffer );
                }
            else
                {
                // Not acceptable payload format
                }
            }
        else
            {
            iStateMachine->ChangeStateTo( EStateToneOff );
            }
        }
    else
        {
        DP_DTMF_WRITE( _L("CDTMFPayloadFormatWrite::StopDTMFTone OUT-OF-SYNC") );
        }
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::SendDTMFTonesL
// Starts to send DTMF string incrementally until the string is sent.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatWrite::SendDTMFTonesL( const TDesC8& aTones )
    {
    DP_DTMF_WRITE( _L("CDTMFPayloadFormatWrite::SendDTMFTonesL") );
    
    TInt digitCount( aTones.Length() );
    __ASSERT_ALWAYS( digitCount != 0, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( IsValidDigits( aTones ), User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( !iSendTimer->IsActive(), User::Leave( KErrNotReady ) );
    
    iSendingMode = EModeAutomatic;
    
    iSendBuffer.Reset();
    for ( TInt i = 0; i < digitCount; i++ )
        {
        // in automatic mode default durations for tones are used
        TSendBufferElement element( aTones[i], 0 );
        iSendBuffer.AppendL( element );
        }
    
    iSinkBuffer->Data().FillZ( KDTMFDefaultPayloadSize );
    iStateMachine->ChangeStateTo( EStateEncodeNextDigit );
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::NotifyStopInDTMFString
// Request to notify client when stop in DTMF string is
// encountered ('w' char).
// ---------------------------------------------------------------------------
//    
void CDTMFPayloadFormatWrite::NotifyStopInDTMFString( TBool aFlag )
    {
    DP_DTMF_WRITE2( _L("CDTMFPayloadFormatWrite::NotifyStopInDTMFString - Flag: %d"), aFlag );
    
    iNotifyStopReq = aFlag;
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::ContinueDTMFStringSending
// Allows a client to continue or cancel the sending of a DTMF string when it 
// was stopped by the use of 'w' char in the string.
// ---------------------------------------------------------------------------
//
TInt CDTMFPayloadFormatWrite::ContinueDTMFStringSending( TBool aContinue )
    {
    DP_DTMF_WRITE2( _L("CDTMFPayloadFormatWrite::ContinueDTMFStringSending - aContinue: %d"), aContinue );
    
    if ( aContinue )
        {
        // Start continue send rest digits incrementally
        iStateMachine->ChangeStateTo( EStateEncodeNextDigit );
        }
    else
        {
        iSendBuffer.Reset();
        iStateMachine->ChangeStateTo( EStateSendingIdle );
        }
    
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::CancelDTMFStringSending
// 
// ---------------------------------------------------------------------------
//    
void CDTMFPayloadFormatWrite::CancelDTMFStringSending()
    {
    DP_DTMF_WRITE( _L("CDTMFPayloadFormatWrite::CancelDTMFStringSending") );
    
    iSendBuffer.Reset();
    iSendTimer->Cancel();
    iStateMachine->ResetStateMachine();
    }
           
// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::DeliverPacketL
// Prepare the RTP packet header and deliver the packet to the datasink.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatWrite::DeliverPacketL( CMMFDataBuffer& aPayload, 
                                              TBool aMarkerBit )
    {
    DP_DTMF_WRITE4( 
        _L("CDTMFPayloadFormatWrite::DeliverPacketL - TSTAMP: %u, TDUR: %d, tick = %u"),
        TUint32( aPayload.TimeToPlay().Int64() ), iToneDuration, 
        User::NTickCount() );
    
    if ( KSignalOutbandDtmf == iGenerationMode )
        {
        // Set the marker bit if it is very first packet.
        if ( aMarkerBit )
            {
            iRtpSendHeader.iMarker = 1;
            }
        else
            {
            iRtpSendHeader.iMarker = 0;
            }
            
        // Construct RTP header.
        if ( EGenRedUsed == iCInfo.iAlgoUsed )
            {
            iRtpSendHeader.iPayloadType = iCInfo.iRedundantPayload;
            }
        else
            {
            iRtpSendHeader.iPayloadType = iCInfo.iPayloadType;
            }

        // Timestamp must be updated before coming here
        iRtpSendHeader.iTimestamp = TUint32( aPayload.TimeToPlay().Int64() );

        // Deliver the packet
        iRtpDataSink->EmptyBufferL( &aPayload, this,
            TMediaId( KUidMediaTypeAudio ), iRtpSendHeader );

        // Do not reset payload buffer because update packets
        // are send based on same buffer.
        }
    else
        {
        DP_DTMF_WRITE( _L("CDTMFPayloadFormatWrite::DeliverPacketL - INBAND TONE") );
        }
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::IsValidDigit
// Check is digit valid DTMF.
// ---------------------------------------------------------------------------
//
TBool CDTMFPayloadFormatWrite::IsValidDigits( 
        const TDesC8& aDigits ) const
    {
    TBool isValid( ETrue );
    TInt index( aDigits.Length() - 1 );
    while ( isValid && 0 <= index )
        {
        if ( KErrNotFound == iValidDigits.Find( aDigits[ index ] ) )
            {
            isValid = EFalse;
            }
        
        index--;
        }
    
    return isValid;
    }
    
// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::SetSinkPrioritySettings
// Used to tell audio policy whether to mix locally played DTMFs to the record
// stream.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatWrite::SetSinkPrioritySettings( 
    const TMMFPrioritySettings& /*aPrioritySettings*/ )
    {
    DP_DTMF_WRITE( _L("CDTMFPayloadFormatWrite::SetSinkPrioritySettings") );
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::HandleControlChar
// Takes care of handling 'p' and 'w' chars in a DTMF string.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatWrite::HandleControlChar( const TChar& aChar )
    {
    if ( 'p' == aChar )
        {
        // 'p' char is interpreted as a 2.5 second pause
        DP_DTMF_WRITE( _L("CDTMFPayloadFormatWrite::HandleControlChar - PAUSE DETECTED") );
        iSendTimer->Cancel();
        iStateMachine->ChangeStateTo( EStateSendPaused );
        TCallBack callBack( HandleTimerEventL, this );
        iSendTimer->Start( 
            TTimeIntervalMicroSeconds32( KDTMFPauseLengthInUs ),
            TTimeIntervalMicroSeconds32( KDTMFPauseLengthInUs ), 
            callBack );
        }
    else if ( 'w' == aChar )
        {
        // Stop in a DTMF string
        DP_DTMF_WRITE( _L("CDTMFPayloadFormatWrite::HandleControlChar - STOP DETECTED ") );

        if ( iNotifyStopReq )
            {
            iSendTimer->Cancel();
            iStateMachine->ChangeStateTo( EStateSendStopped );

            SendEventSignalToClient( KMccDtmfStopInDtmfString );
            }
        }
    else
        {
        // not a control character
        }
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::ConfigurePayloadFormatL
// Configure payload encoding parameters.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatWrite::ConfigurePayloadFormatL( 
    const TDesC8& aConfigParams,
    CMccRtpMediaClock& aClock )
    {
    DP_DTMF_WRITE( _L("CDTMFPayloadFormatWrite::ConfigurePayloadFormatL IN") );
    
    __ASSERT_ALWAYS( aConfigParams.Size() == sizeof( TMccCodecInfo ),
        User::Leave( KErrArgument ) );
    
    TMccCodecInfoBuffer infoBuffer;
    infoBuffer.Copy( aConfigParams );
    iCInfo = infoBuffer();
    
    if ( !iPayloadEncoder )
        {
        iPayloadEncoder = CDTMFPayloadEncoder::NewL();
        }
    
    iPayloadEncoder->SetPayloadType( iCInfo.iPayloadType );
    
    // Redundancy configuration, always using generic redundacy.
    if ( iCInfo.iRedundancyCount && KMaxDtmfRedCount >=
         iCInfo.iRedundancyCount )
        {
        DP_DTMF_WRITE2( _L("CDTMFPayloadFormatWrite::ConfigurePayloadFormatL iAlgoUsed: %d"),
            iCInfo.iAlgoUsed );
        
        __ASSERT_ALWAYS( EGenRedUsed == iCInfo.iAlgoUsed,
            User::Leave( KErrArgument ) );
            
        User::LeaveIfError(
            iPayloadEncoder->SetPayloadFormat( EDTMFPayloadFormatRedEvents ) );
        }
    else
        {
        // Make sure that the EGenRedUsed is not set
        iCInfo.iAlgoUsed = ENoAlgoUsed;
        }
    
    // Inband/Outband configuration
    if ( EDTMFModeInband == iCInfo.iCodecMode )
        {
        iGenerationMode = KSignalInbandDtmf;
        }
    else
        {
        iGenerationMode = KSignalOutbandDtmf;
        
        // Check if we need to unreg from current media clock.
        if ( iRtpMediaClock )
            {
            iRtpMediaClock->UnregisterMediaFormat( iKey );
            }
        
        iKey = aClock.RegisterMediaFormat( iSampleRate, iCInfo.iPtime );
        }
    
    iRtpMediaClock = &aClock;
    
    DP_DTMF_WRITE( _L("CDTMFPayloadFormatWrite::ConfigurePayloadFormatL OUT") );
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::DoHandleTimerEvent
// Does actions depending on current state on timer event.
// ---------------------------------------------------------------------------
//
TInt CDTMFPayloadFormatWrite::DoHandleTimerEventL()
    {
    switch ( iStateMachine->State() )
        {
        case EStateToneOn:
            this->DoToneOnActionsL( EFalse );
            break;
        
        case EStateToneOff:
            this->DoToneOffActionsL( EFalse );
            break;
        
        case EStateSendPaused:
            iSendTimer->Cancel();
            iStateMachine->ChangeStateTo( EStateEncodeNextDigit );
            break;
        
        default:
            DP_DTMF_WRITE(_L("CDTMFPayloadFormatWrite::DoHandleTimerEvent DEFAULT") );
            break;
        }
    
    return ETrue;
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::EncodeNextDigitL
// Encodes next digit in sequence.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatWrite::EncodeNextDigitL()
    {
    DP_DTMF_WRITE2( _L("CDTMFPayloadFormatWrite::EncodeNextDigitL, tick = %u"),
        User::NTickCount() );

    iToneDuration = 0;
    iToneDuration += static_cast<TUint16>
        ( iCInfo.iPtime * iSampleRate * KMsToSecFactor );
    
    if ( iSendBuffer.Count() && ('p' == iSendBuffer[0].Tone() || 'w' == 
        iSendBuffer[0].Tone() ) )
        {
        HandleControlChar( iSendBuffer[0].Tone() );
        iSendBuffer.Remove( 0 );
        }
    else if ( iSendBuffer.Count() )
        {
        if ( EDTMFPayloadFormatEvent == iPayloadFormat ||
             EDTMFPayloadFormatRedEvents == iPayloadFormat )
            {
            // Encode digit to RTP packet using Payload Format
            // for Named Events
            TDTMFEventPayloadInfo eventInfo;
            eventInfo.SetEvent( iSendBuffer[0].Tone() );
            eventInfo.SetVolume( KDTMFDefaultToneVolume );
            eventInfo.SetDuration( iToneDuration );
            
            if ( KSignalOutbandDtmf == iGenerationMode )
                {
                TUint32 timeStamp( 0 );
                User::LeaveIfError( 
                    iRtpMediaClock->GetTimeStamp( iKey, timeStamp ) );
                iSinkBuffer->SetTimeToPlay( 
                    TTimeIntervalMicroSeconds( TInt64( timeStamp ) ) );
                }
            
            iPayloadEncoder->EncodeEvent( eventInfo, iSinkBuffer );
            }
        else if ( EDTMFPayloadFormatTone == iPayloadFormat )
            {
            TDTMFTonePayloadInfo toneInfo;
            toneInfo.SetModulation( KDTMFDefaultModulation );
            toneInfo.SetVolume( KDTMFDefaultToneVolume );
            toneInfo.SetDuration( iToneDuration );
            iPayloadEncoder->EncodeTone( toneInfo, iSinkBuffer );
            }
        else
            {
            // Not acceptable payload format
            DP_DTMF_WRITE2( _L("PayloadFormat not Acceptable!: %d"), 
                iPayloadFormat );
            }
        
        iStateMachine->ChangeStateTo( EStateToneOn );
        }
    else
        {
        if ( EModeAutomatic == iSendingMode )
            {
            SendEventSignalToClient( KMccDtmfSendingComplete );
            }

        iStateMachine->ChangeStateTo( EStateSendingIdle );
        }
    }
    
// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::DoToneOnActionsL
// Called by sendtimer. Does actions belonging to tone on state and exits
// state when exit conditions are met. Actions to do are: Send update packets
// at certain time interval.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatWrite::DoToneOnActionsL( TBool aEntryToState )
    {
    DP_DTMF_WRITE2( _L("CDTMFPayloadFormatWrite::DoToneOnActionsL, tick = %u"),
        User::NTickCount() );
    
    if ( aEntryToState )
        {
        iSendTimer->Cancel();
        
        if ( EModeManual == iSendingMode )
            {
            SendEventSignalToClient( KMccDtmfManualStart );
            }
        else
            {
            SendEventSignalToClient( KMccDtmfSequenceStart );
            }
        
        DeliverPacketL( *iSinkBuffer, ETrue );
        
        TCallBack callBack( HandleTimerEventL, this );
        iSendTimer->Start(
            TTimeIntervalMicroSeconds32( iCInfo.iPtime * KFactor1000 ),
            TTimeIntervalMicroSeconds32( iCInfo.iPtime * KFactor1000 ),
            callBack );
        }
    else
        {
        iToneDuration += static_cast<TUint16>
            ( iCInfo.iPtime * iSampleRate * KMsToSecFactor );
        
        if ( EModeAutomatic == iSendingMode )
            {
            TUint targetDuration = 
                TUint( KDTMFDefToneOnLengthInUs * KUsToSecFactor * 
                iSampleRate );

            if ( iToneDuration >= targetDuration )
                {
                // Exit condition
                iSendTimer->Cancel();
                iPayloadEncoder->UpdateEventPayload( ETrue, iToneDuration, 
                                                                iSinkBuffer );
                iStateMachine->ChangeStateTo( EStateToneOff );
                return;
                }
            }
        else
            {
            if ( iSendBuffer[0].StopTime() != 0 &&
                 iSendBuffer[0].Duration() * iTickPeriod 
                    * iSampleRate * KUsToSecFactor <= iToneDuration )
                {
                // Exit condition
                iSendTimer->Cancel();
                iPayloadEncoder->UpdateEventPayload( ETrue, iToneDuration, 
                                                                iSinkBuffer );
                iStateMachine->ChangeStateTo( EStateToneOff );
                return;
                }
            }

        if ( EDTMFPayloadFormatEvent == iPayloadFormat
            || EDTMFPayloadFormatRedEvents == iPayloadFormat )
            {
            // Check is this long duration event
       		// Disabling a PC-lint warning due to if-statement always being false,
       		// as it seems the possibility should be there if values are changed
       		/*lint -e685 */
            if ( KDTMFMaxDurationFieldValue < iToneDuration )
                {
                iToneDuration -= KDTMFMaxDurationFieldValue;
                iPayloadEncoder->UpdateEventPayload( EFalse, iToneDuration, 
                                                                iSinkBuffer );
                DeliverPacketL( *iSinkBuffer, EFalse );
                }
			/*lint +e685 */                
			
            else
                {
                // The update packet MUST have the same RTP timestamp value as
                // the initial packet for the event (RFC 2833, 2.5.1.2).
                iPayloadEncoder->UpdateEventPayload( EFalse, iToneDuration, 
                                                                iSinkBuffer );
                DeliverPacketL( *iSinkBuffer, EFalse );
                }    
            }
        else if ( EDTMFPayloadFormatTone == iPayloadFormat )
            {
            iPayloadEncoder->UpdateTonePayload( iToneDuration, iSinkBuffer );
            DeliverPacketL( *iSinkBuffer, EFalse );
            }
        else
            {
            // Not acceptable payload format
            }
        }
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::DoToneOffActionsL
// Called by sendtimer. Does actions belonging to tone off state and exits
// state when exit conditions are met. Actions include: sending final packets.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatWrite::DoToneOffActionsL( TBool aEntryToState )
    {
    DP_DTMF_WRITE2( _L("CDTMFPayloadFormatWrite::DoToneOffActionsL, tick = %u"), 
        User::NTickCount() );

    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x", MCC_TRACE, MCC_DTMF_PLF_WRITE_SENDFINALPKT );
    #endif
    
    if ( aEntryToState )
        {
        iSendTimer->Cancel();
        iFinalPacketCtr = 0;
        
        TCallBack callBack( HandleTimerEventL, this );
        iSendTimer->Start(
            TTimeIntervalMicroSeconds32( iCInfo.iPtime * KFactor1000 ),
            TTimeIntervalMicroSeconds32( iCInfo.iPtime * KFactor1000 ),
            callBack );
        }
    else
        {
        iFinalPacketCtr++;
        if ( KNumOfFinalPackets >= iFinalPacketCtr )
            {
            DeliverPacketL( *iSinkBuffer, EFalse );
            }
        else
            {
            TUint curOffDur = iFinalPacketCtr * iCInfo.iPtime * KFactor1000;
            if ( EModeManual == iSendingMode || 
                 KDTMFDefToneOffLengthInUs <= curOffDur )
                {
                // Prevent audio sending during final packet retransmission by
                // sending completion event only when also retransmissions are
                // done.
                if ( EModeManual == iSendingMode )
                    {
                    SendEventSignalToClient( KMccDtmfManualStop );
                    }
                else
                    {
                    SendEventSignalToClient( KMccDtmfSequenceStop );
                    }
                
                iSendTimer->Cancel();
                iFinalPacketCtr = 0;
                if ( iSendBuffer.Count() > 0 )
                	{
                	iSendBuffer.Remove( 0 );	
                	}
                iStateMachine->ChangeStateTo( EStateEncodeNextDigit );
                }
            }
        }
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::SendEventSignalToClient
// Signaling about events to the client.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatWrite::SendEventSignalToClient( 
    TMccDtmfEventType aEventType ) const
    {
    TMccDtmfEventData eventSignal;
    eventSignal.iDtmfEventType = aEventType;
    TMccDtmfEventDataPackage signalPkg( eventSignal );

    if ( iEventHandler )
        {
        TMccEvent event( 0,
                         0, 
                         0, 
                         0, 
                         KMccEventCategoryDtmf, 
                         KMccDtmfControl, 
                         KErrNone, 
                         signalPkg );

        TMccInternalEvent internalEvent( KMccDtmfFormatterUid, 
                                         EMccInternalEventNone,
                                         event );
            
        iEventHandler->SendEventToClient( internalEvent );
        }
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::IsValidDigit
// ---------------------------------------------------------------------------
//
TBool CDTMFPayloadFormatWrite::IsValidDigit( const TChar& aTone ) const
    {
    return ( KErrNotFound != iValidDigits.Find( aTone ) );
    }


//  End of File

