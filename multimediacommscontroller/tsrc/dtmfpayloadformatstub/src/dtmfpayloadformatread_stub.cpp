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




// INCLUDES
#include "dtmfpayloadformatread.h"
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
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::NewL
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CDTMFPayloadFormatRead* CDTMFPayloadFormatRead::NewL( 
    MDataSource* /*aSource*/ )
    {
    return new( ELeave ) CDTMFPayloadFormatRead;
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::~CDTMFPayloadFormatRead
// Destructor.
// ---------------------------------------------------------------------------
//
CDTMFPayloadFormatRead::~CDTMFPayloadFormatRead()
    {
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::Streams
// Not applicable for DTMF payload formatter. Pure virtual method
// implementation needed anyway.
// ---------------------------------------------------------------------------
//
TUint CDTMFPayloadFormatRead::Streams( TUid /*aMediaType*/ ) const
    {
    return 1;
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
    return EFalse;
    }
        
// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::CreateSourceBufferL
// Should not be used.
// ---------------------------------------------------------------------------
//
CMMFBuffer* CDTMFPayloadFormatRead::CreateSourceBufferL( 
    TMediaId /*aMediaId*/,
    TBool& /*aReference*/ )
    {
    return NULL;
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::SourceDataTypeCode
// Returns source data type code.
// ---------------------------------------------------------------------------
//
TFourCC CDTMFPayloadFormatRead::SourceDataTypeCode( TMediaId /*aMediaId*/ )
    {
    return TFourCC( KMccFourCCIdDTMF );
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::SinkDataTypeCode
// Returns the current datatype FourCC code.
// ---------------------------------------------------------------------------
//
TFourCC CDTMFPayloadFormatRead::SinkDataTypeCode( TMediaId /*aMediaId*/ )
    {
    return TFourCC( KMccFourCCIdDTMF );
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::SetPayloadFormat
// Sets payload format used in DTMF decoding.
// ---------------------------------------------------------------------------
//
TInt CDTMFPayloadFormatRead::SetPayloadFormat( 
    TDTMFPayloadFormat aPayloadFormat )
    {
    iPayloadFormat = aPayloadFormat;
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::FillBufferL
// No need for data exchange with DataPath. DTMF receiving and processing
// cycle is managed by SourcePrimeL(), SourcePlayL(), SourcePauseL(),
// SourceStopL() methods.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatRead::FillBufferL( 
    CMMFBuffer* /*aBuffer*/, 
    MDataSink* /*aConsumer*/, 
    TMediaId /*aMediaId*/ )
    {
    delete HBufC::NewL( 10 );
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::FillSourceBufferL
// Called by state machine. Send fill buffer request to the RTP Data Source.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatRead::FillSourceBufferL()
    {
    delete HBufC::NewL( 10 );
    }
    
// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::FillSinkBuffer
// Called by state machine. Decode payload to DTMF string and fill
// sink buffer.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatRead::FillSinkBufferL()
    {
    delete HBufC::NewL( 10 );
    }
    
// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::SendDataToSinkL
// Called by state machine. Send decoded payload to sound device for playback.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatRead::SendDataToSinkL()
    {
    delete HBufC::NewL( 10 );
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::HandleDTMFEvents
// Handle and play DTMF events using tone player.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatRead::HandleDTMFEventsL()
    {
    delete HBufC::NewL( 10 );
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::HandleDTMFTones
// NOT SUPPORTED. Handle and play DTMF tone payload.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatRead::HandleDTMFTones()
    {
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::SourceThreadLogon
// Passes logon request to the RTP data source.
// ---------------------------------------------------------------------------
//
TInt CDTMFPayloadFormatRead::SourceThreadLogon(
    MAsyncEventHandler& /*aEventHandler*/ )
    {
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::SourceThreadLogoff
// Passes log out request to the RTP data source.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatRead::SourceThreadLogoff()
    {
    }
        
// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::SourcePrimeL
// Passes prime transition to the RTP data source.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatRead::SourcePrimeL()
    {
    delete HBufC::NewL( 10 );
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::SourcePlayL
// Passes play transition to the RTP data source.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatRead::SourcePlayL()
    {
    delete HBufC::NewL( 10 );
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::SourcePauseL
// Passes pause transition to the RTP data source.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatRead::SourcePauseL()
    {
    delete HBufC::NewL( 10 );
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::SourceStopL
// Passes stop transition to the RTP data source.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatRead::SourceStopL()
    {
    delete HBufC::NewL( 10 );
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::BufferFilledL
// Called by DataSource after it has been filled the buffer.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatRead::DataBufferFilledL( 
    CMMFBuffer* /*aBuffer*/, 
    const TRtpRecvHeader& /*aRtpHeader*/ )
    {
    delete HBufC::NewL( 10 );
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::SetSourcePrioritySettings
//
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatRead::SetSourcePrioritySettings( 
    const TMMFPrioritySettings& /*aPrioritySettings*/ )
    {
    }
    
// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::MatoPrepareComplete
// 
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatRead::MatoPrepareComplete( TInt /*aError*/ )
    {   
    }
    
// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::MatoPlayComplete
// 
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatRead::MatoPlayComplete( TInt /*aError*/ )
    {
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::ConfigurePayloadFormatL
// Configure payload decoding parameters.
// ---------------------------------------------------------------------------
//
void CDTMFPayloadFormatRead::ConfigurePayloadFormatL( 
    const TDesC8& /*aConfigParams*/ )
    {
    delete HBufC::NewL( 10 );
    }
    
// -----------------------------------------------------------------------------
// CDTMFPayloadFormatRead::CreateClipBufferL
// Creates buffer needed in data transfer with format readers clip.
// -----------------------------------------------------------------------------
//
void CDTMFPayloadFormatRead::CreateClipBufferL()
    {
    delete HBufC::NewL( 10 );
    }

// ---------------------------------------------------------------------------
// From class MPayloadFormatRead.
//
// ---------------------------------------------------------------------------
//
TInt CDTMFPayloadFormatRead::HandleError( 
    TInt aError, 
    TFormatDecodeState /*aState*/ )
    {
    return aError;
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadFormatRead::CompareFirstPacketRtpHeaders
// ---------------------------------------------------------------------------
//
TBool CDTMFPayloadFormatRead::CompareFirstPacketRtpHeaders(
    const TRtpRecvHeader& /*aRtpheader*/ )
    {
    return EFalse;
    }

//  End of File
