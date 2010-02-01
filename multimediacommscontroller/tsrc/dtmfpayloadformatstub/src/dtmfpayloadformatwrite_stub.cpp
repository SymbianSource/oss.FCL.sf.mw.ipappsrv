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
* Description:    
*
*/




#include "dtmfpayloadformatwrite.h"

const TUint KDtmfDefSampleRate      = 8000;
const TInt  KDefaultTickPeriodInMs  = 1;

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
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::NewL
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CDTMFPayloadFormatWrite* CDTMFPayloadFormatWrite::NewL( MDataSink* /*aSink*/ )
    {
    return new( ELeave ) CDTMFPayloadFormatWrite;
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::~CDTMFPayloadFormatWrite
// Destructor.
// ---------------------------------------------------------------------------
//
CDTMFPayloadFormatWrite::~CDTMFPayloadFormatWrite()
    {
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::SinkThreadLogon
// Passes the logon command to the sink clip.
// ---------------------------------------------------------------------------
//
TInt CDTMFPayloadFormatWrite::SinkThreadLogon( 
    MAsyncEventHandler& /*aEventHandler*/ )
    {
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::SinkThreadLogoff
// Passes the logoff command to the sink clip.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatWrite::SinkThreadLogoff()
    {
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::SetSinkDataTypeCode
// Set the sink data type to the given FourCC code for the given media.
// ---------------------------------------------------------------------------
//
TInt CDTMFPayloadFormatWrite::SetSinkDataTypeCode( 
    TFourCC aSinkFourCC, 
    TMediaId /*aMediaId*/ )
    {
    iFourCC = aSinkFourCC;
    return KErrNone;
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
    return NULL;
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::SinkPrimeL
// Passes prime transition to the RTP data sink.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatWrite::SinkPrimeL()
    {
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::SinkPlayL
// Passes play transition to the RTP data sink and resets state variables.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatWrite::SinkPlayL()
    {
    }
    
// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::SinkPauseL
// Passes pause transition to the RTP data sink.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatWrite::SinkPauseL()
    {
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::SinkStopL
// Passes stop transition to the RTP data sink.
// ---------------------------------------------------------------------------
//  
void CDTMFPayloadFormatWrite::SinkStopL()
    {
    }
    
// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::ConfigurePayloadFormatL
// Configure payload encoding parameters.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatWrite::ConfigurePayloadFormatL( 
    const TDesC8& /*aConfigParams*/,
    CMccRtpMediaClock& /*aClock*/ )
    {
    delete HBufC::NewL( 10 );
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
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::StartDTMFToneL
// Starts a timer which sends specified DTMF digit periodically until 
// StopDTMFTone() is called.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatWrite::StartDTMFToneL( const TChar /*aTone*/ )
    {
    delete HBufC::NewL( 10 );
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::StopDTMFToneL
// Stops the timer for DTMF digit update packet sending. Starts a timer for 
// sending Final Packet three times according to RFC 2833, section 2.5.1.4.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatWrite::StopDTMFToneL()
    {
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::SendDTMFTonesL
// Starts to send DTMF string incrementally until the string is sent.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatWrite::SendDTMFTonesL( const TDesC8& /*aTones*/ )
    {
    delete HBufC::NewL( 10 );
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::NotifyStopInDTMFString
// Request to notify client when stop in DTMF string is
// encountered ('w' char).
// ---------------------------------------------------------------------------
//    
void CDTMFPayloadFormatWrite::NotifyStopInDTMFString( TBool /*aFlag*/ )
    {
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::ContinueDTMFStringSending
// Allows a client to continue or cancel the sending of a DTMF string when it 
// was stopped by the use of 'w' char in the string.
// ---------------------------------------------------------------------------
//
TInt CDTMFPayloadFormatWrite::ContinueDTMFStringSending( TBool /*aContinue*/ )
    {
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::CancelDTMFStringSending
// 
// ---------------------------------------------------------------------------
//    
void CDTMFPayloadFormatWrite::CancelDTMFStringSending()
    {
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::HandleTimerEventL
// STATIC METHOD. Passes timer events to the class instance interested
// about them.
// ---------------------------------------------------------------------------
//
TInt CDTMFPayloadFormatWrite::HandleTimerEventL( TAny* /*aObjectPtr*/ )
    {
    delete HBufC::NewL( 10 );
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::SinkDataTypeCode
// Return the sink data type (four CC code) for the given media.
// ---------------------------------------------------------------------------
//
TFourCC CDTMFPayloadFormatWrite::SinkDataTypeCode( TMediaId /*aMediaId*/ )
    {
    return iFourCC;    
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::FrameTimeInterval
// Gets audio frame size in milliseconds.
// ---------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CDTMFPayloadFormatWrite::FrameTimeInterval( 
    TMediaId /*aMediaType*/ ) const
    {
    return TTimeIntervalMicroSeconds( TInt64( 0 ) );
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::Duration
// Gets the duration of the sink clip for the specified media ID.
// ---------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CDTMFPayloadFormatWrite::Duration( 
    TMediaId /*aMediaType*/ ) const
    {
    return TTimeIntervalMicroSeconds( TInt64( 0 ) );
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::BufferEmptiedL
// Called by RTP data sink. Indicates that DTMF data is sent to the network.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatWrite::BufferEmptiedL( CMMFBuffer* /*aBuffer*/ )
    {
    delete HBufC::NewL( 10 );
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
    delete HBufC::NewL( 10 );
    }
                    
// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::PayloadFormat
// Returns payload format used in DTMF encoding.
// ---------------------------------------------------------------------------
//
TDTMFPayloadFormat CDTMFPayloadFormatWrite::PayloadFormat() const
    {    
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
    iPayloadFormat = aPayloadFormat;
    return KErrNone;
    }
           
// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::DeliverPacketL
// Prepare the RTP packet header and deliver the packet to the datasink.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatWrite::DeliverPacketL( 
    CMMFDataBuffer& /*aPayload*/, 
    TBool /*aMarkerBit*/ )
    {
    delete HBufC::NewL( 10 );
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::IsValidDigit
// Check is digit valid DTMF.
// ---------------------------------------------------------------------------
//
TBool CDTMFPayloadFormatWrite::IsValidDigits( 
    const TDesC8& /*aDigits*/ ) const
    {
    return ETrue;
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::HandleControlChar
// Takes care of handling 'p' and 'w' chars in a DTMF string.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatWrite::HandleControlChar( const TChar& /*aChar*/ )
    {
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::DoHandleTimerEvent
// Does actions depending on current state on timer event.
// ---------------------------------------------------------------------------
//
TInt CDTMFPayloadFormatWrite::DoHandleTimerEventL()
    {    
    delete HBufC::NewL( 10 );
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::EncodeNextDigitL
// Encodes next digit in sequence.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatWrite::EncodeNextDigitL()
    {
    delete HBufC::NewL( 10 );
    }
    
// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::DoToneOnActionsL
// Called by sendtimer. Does actions belonging to tone on state and exits
// state when exit conditions are met. Actions to do are: Send update packets
// at certain time interval.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatWrite::DoToneOnActionsL( TBool /*aEntryToState*/ )
    {
    delete HBufC::NewL( 10 );
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::DoToneOffActionsL
// Called by sendtimer. Does actions belonging to tone off state and exits
// state when exit conditions are met. Actions include: sending final packets.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatWrite::DoToneOffActionsL( TBool /*aEntryToState*/ )
    {
    delete HBufC::NewL( 10 );
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatWrite::SendEventSignalToClient
// Signaling about events to the client.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatWrite::SendEventSignalToClient( 
    TMccDtmfEventType /*aEventType*/ ) const
    {
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

