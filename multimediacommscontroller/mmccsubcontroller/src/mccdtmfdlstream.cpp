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
* Description:    Provides DTMF support
*
*/




#include <mmf/server/mmfformat.h>
#include "mccdtmfdlstream.h"
#include "mccsubcontrollerlogs.h"
#include "mccdatasource.h"
#include "mcccodecconfigurator.h"
#include "mccrtpmanager.h"
#include "mccrtpmediaclock.h"
#include "mccmultiplexer.h"
#include "mccresources.h"
#include "mccinternaldef.h"
#include "mccrtpdatasource.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that might leave.
// ---------------------------------------------------------------------------
//
CMccDtmfDlStream::CMccDtmfDlStream( TUint32 aMccStreamId, 
        MAsyncEventHandler* aEventhandler, MMccResources* aMccResources,
        CMccRtpManager* aManager, TFourCC aFourCC, TInt aStreamType,
        CMccRtpMediaClock& aClock )
    :
    CMccSymStreamBase( aMccStreamId, aEventhandler, aMccResources, aManager, 
        aStreamType ), iFourCC( aFourCC )
    {
    iRtpMediaClock = &aClock;
    }


// ---------------------------------------------------------------------------
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CMccDtmfDlStream::ConstructL()
    {
	__SUBCONTROLLER( "CMccDtmfDlStream::ConstructL" )
	__SUBCONTROLLER( "CMccDtmfDlStream::ConstructL, exit" )
    }


// ---------------------------------------------------------------------------
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CMccDtmfDlStream* CMccDtmfDlStream::NewL( TUint32 aMccStreamId, 
        MAsyncEventHandler* aEventHandler, MMccResources* aMccResources,
        CMccRtpManager* aManager, TFourCC aFourCC, TInt aStreamType,
        CMccRtpMediaClock& aClock )
    {
    CMccDtmfDlStream* self = CMccDtmfDlStream::NewLC( aMccStreamId, aEventHandler,
        aMccResources, aManager, aFourCC, aStreamType, aClock );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CMccDtmfDlStream* CMccDtmfDlStream::NewLC( TUint32 aMccStreamId, 
        MAsyncEventHandler* aEventHandler, MMccResources* aMccResources,
        CMccRtpManager* aManager, TFourCC aFourCC, TInt aStreamType,
        CMccRtpMediaClock& aClock )
    {
    CMccDtmfDlStream* self = new( ELeave ) CMccDtmfDlStream( aMccStreamId,
        aEventHandler, aMccResources, aManager, aFourCC, aStreamType, aClock );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// Destructor.
// ---------------------------------------------------------------------------
//
CMccDtmfDlStream::~CMccDtmfDlStream()
    {
    __SUBCONTROLLER( "CMccDtmfDlStream::~CMccDtmfDlStream" )
    
    // Must do mux cleanup before decoder (mux sink) deletion
    MultiplexerCleanup();
    
    if ( iFormatDecode )
        {
        iFormatDecode->SourceThreadLogoff();
        delete iFormatDecode;
        }

    __SUBCONTROLLER( "CMccDtmfDlStream::~CMccDtmfDlStream, exit" )
    }

// ---------------------------------------------------------------------------
// From class CMccSymStreamBase.
// Sets the MMF priority settings for this stream.
// ---------------------------------------------------------------------------
//
void CMccDtmfDlStream::SetPrioritySettingsL( 
        const TMMFPrioritySettings& aPriority )
    {
    __SUBCONTROLLER( "CMccDtmfDlStream::SetPrioritySettingsL start" );
    
    if ( iFormatDecode )
        {
        iFormatDecode->SetSourcePrioritySettings( aPriority );
        }
    
    __SUBCONTROLLER( "CMccDtmfDlStream::SetPrioritySettingsL end ");
    }

// ---------------------------------------------------------------------------
// From class CMccSymStreamBase.
// Adds the sink and source to the stream.
// ---------------------------------------------------------------------------
//
void CMccDtmfDlStream::AddSinkAndSourceL( MDataSink* aDataSink,
        MDataSource* aDataSource )
    {
	__SUBCONTROLLER( "CMccDtmfDlStream::AddSinkAndSourceL" )
    __ASSERT_ALWAYS( aDataSink && aDataSource, User::Leave( KErrArgument ) );
    
    __ASSERT_ALWAYS( aDataSink->DataSinkType() == KMccAnySinkUid,
        User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( aDataSource->DataSourceType() == KMccRtpSourceUid,
        User::Leave( KErrArgument ) );
    
	__SUBCONTROLLER( "CMccDtmfDlStream::AddSinkAndSourceL, adding" )
	iDatasink = aDataSink;
	iDatasource = aDataSource;
	
	iMultiplexer = iMccResources->MultiplexerL( iDatasource );
    
    User::LeaveIfError( iDatasink->SinkThreadLogon( *this ) );
    
    __SUBCONTROLLER( "CMccDtmfDlStream::AddSinkAndSourceL, exit" )
    }

// ---------------------------------------------------------------------------
// From class CMccSymStreamBase.
// Loads a codec to the stream.
// ---------------------------------------------------------------------------
//
void CMccDtmfDlStream::LoadCodecL( const TMccCodecInfo& aCodecInfo, 
        const TDesC8& aFmtp )
    {
    __SUBCONTROLLER_INT1( "CMccDtmfDlStream::LoadCodecL, codec state:", 
                          CurrentCodecState() )
    
    __ASSERT_ALWAYS( CurrentCodecState() == EStateCodecNone || 
                     iCodecInfo.iFourCC == aCodecInfo.iFourCC,
                     User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( iDatasink, User::Leave( KErrNotReady ) );
        
    iCodecInfo = aCodecInfo;
    
    if ( CurrentCodecState() == EStateCodecNone )
        {
        delete iFmtpAttr;
        iFmtpAttr = NULL;

    	iFmtpAttr = HBufC8::NewL( aFmtp.Length() );
        iFmtpAttr->Des().Copy( aFmtp );
        
        iCodecInfo.iFmtpLength = iFmtpAttr->Length();
        CreatePayloadFormatDecoderL();
        
        iMultiplexer->NegotiateSourceL( *iFormatDecode );
        
        SetCodecState( EStateCodecNegotiated );        	
        }
    else if ( CurrentCodecState() == EStateCodecNegotiated )
        {
        SetCodecState( EStateCodecLoaded );
        
        // Codec is not fully usable yet, do just formatter initialization
        MccCodecConfigurator::ConfigureCodecL( *iFormatDecode, 
                                                *iDatasink, 
                                                iCodecInfo,
                                                *iRtpMediaClock,
                                                CurrentCodecState() );
        }
    else if ( CurrentCodecState() == EStateCodecLoaded ||
              CurrentCodecState() == EStateCodecLoadedAndUpdating )
        {
        // Update codec info
        SetCodecState( EStateCodecLoadedAndUpdating );
    	UpdateCodecInformationL( iCodecInfo );
        }
    else
        {
        User::Leave( KErrNotReady );
        }
        
    __SUBCONTROLLER_INT1( "CMccDtmfDlStream::LoadCodecL, exit, new state:", 
                          CurrentCodecState() )
    }

// ---------------------------------------------------------------------------
// From class CMccSymStreamBase.
// Primes the stream ready.
// ---------------------------------------------------------------------------
//
void CMccDtmfDlStream::PrimeL( const TUint32 aEndpointId )
    {
	__SUBCONTROLLER( "CMccDtmfDlStream::PrimeL" )
    __ASSERT_ALWAYS( iFormatDecode, User::Leave( KErrNotReady ) );
	
    TBool controlNetworkResources = SetStateL( EStatePrepared, aEndpointId );  
    
    if ( controlNetworkResources )
        {
        iRtpmanager->CreateReceiveStreamL( *iDatasource, iCodecInfo );
        }
        
    LoadCodecL( iCodecInfo, KNullDesC8 );
    
    if ( IsControlled( MCC_ENDPOINT_ID( iDatasink ) ) )
        {
        iDatasink->SinkPrimeL();
        }
    
    if ( IsControlled( MCC_ENDPOINT_ID( iDatasource ) ) )
        {
        iFormatDecode->SourcePrimeL();
        }

    InformStreamStateChange( KMccStreamPrepared, aEndpointId );
    
	__SUBCONTROLLER( "CMccDtmfDlStream::PrimeL, exit" )
    }

// ---------------------------------------------------------------------------
// From class CMccSymStreamBase.
// Starts the stream. DTMF can be now sent and received.
// ---------------------------------------------------------------------------
//
void CMccDtmfDlStream::PlayL( 
    const TUint32 aEndpointId, 
    TBool /*aStreamPaused*/, 
    TBool /*aEnableRtcp*/ )
    {
	__SUBCONTROLLER( "CMccDtmfDlStream::PlayL" )
    __ASSERT_ALWAYS( iFormatDecode && iDatasource && iDatasink, 
                     User::Leave( KErrNotReady ) );
	
    TBool controlNetworkResources = SetStateL( EStateStreaming, aEndpointId );
    
    if ( controlNetworkResources )
        {
        iRtpmanager->StartSessionL();
        }
    
    LoadCodecL( iCodecInfo, KNullDesC8 );
    
    if ( IsControlled( MCC_ENDPOINT_ID( iDatasink ) ) )
        {
        iDatasink->SinkPlayL();
        }
    
    if ( IsControlled( MCC_ENDPOINT_ID( iDatasource ) ) )
        {
        iFormatDecode->SourcePlayL();
        }
        
    // Dtmf does not need standby, deactivate it
    CMccRtpDataSource* dSource = static_cast<CMccRtpDataSource*>( iDatasource );
    dSource->StandBy( EDeactivateStandby, iCodecInfo.iPayloadType );

    InformStreamStateChange( KMccStreamStarted, aEndpointId );
    
	__SUBCONTROLLER( "CMccDtmfDlStream::PlayL, exit" )
    }

// ---------------------------------------------------------------------------
// From class CMccSymStreamBase.
// Pauses the stream. DTMF sending and receiving are not possible in paused
// state.
// ---------------------------------------------------------------------------
//
void CMccDtmfDlStream::PauseL( const TUint32 aEndpointId, TBool /*aEnableRtcp*/ )
    {
    __SUBCONTROLLER( "CMccDtmfDlStream::PauseL" )
    __ASSERT_ALWAYS( iFormatDecode && iDatasource && iDatasink, 
                     User::Leave( KErrNotReady ) );
    
    SetStateL( EStatePaused, aEndpointId );
    
    if ( IsControlled( MCC_ENDPOINT_ID( iDatasink ) ) )
        {
        iDatasink->SinkPauseL();
        }
    
    if ( IsControlled( MCC_ENDPOINT_ID( iDatasource ) ) )
        {
        iFormatDecode->SourcePauseL();
        }
    
    InformStreamStateChange( KMccStreamPaused, aEndpointId );
    
    __SUBCONTROLLER( "CMccDtmfDlStream::PauseL, exit" )
    }

// ---------------------------------------------------------------------------
// From class CMccSymStreamBase.
// Resumes the stream.
// ---------------------------------------------------------------------------
//
void CMccDtmfDlStream::ResumeL( const TUint32 aEndpointId, TBool /*aEnableRtcp*/ )
    {
    __SUBCONTROLLER( "CMccDtmfDlStream::ResumeL" )
    __ASSERT_ALWAYS( iFormatDecode && iDatasource && iDatasink, 
                     User::Leave( KErrNotReady ) );
    
    SetStateL( EStateStreaming, aEndpointId );
    
    if ( IsControlled( MCC_ENDPOINT_ID( iDatasink ) ) )
        {
        iDatasink->SinkPlayL();
        }
    
    if ( IsControlled( MCC_ENDPOINT_ID( iDatasource ) ) )
        {
        iFormatDecode->SourcePlayL();
        }
    
    InformStreamStateChange( KMccStreamResumed, aEndpointId );

    __SUBCONTROLLER( "CMccDtmfDlStream::ResumeL, exit" )
    }

// ---------------------------------------------------------------------------
// From class CMccSymStreamBase.
// Stops the stream.
// ---------------------------------------------------------------------------
//
void CMccDtmfDlStream::StopL( const TUint32 aEndpointId )
    {
    __SUBCONTROLLER( "CMccDtmfDlStream::StopL" )
    __ASSERT_ALWAYS( iFormatDecode && iDatasource && iDatasink, 
                     User::Leave( KErrNotReady ) );
    
    SetStateL( EStateStopped, aEndpointId );
    
    if ( IsControlled( MCC_ENDPOINT_ID( iDatasink ) ) )
        {
        iDatasink->SinkStopL();
        }
    
    if ( IsControlled( MCC_ENDPOINT_ID( iDatasource ) ) )
        {
        iFormatDecode->SourceStopL();
        }
    
    InformStreamStateChange( KMccStreamStopped, aEndpointId );
    
    __SUBCONTROLLER( "CMccDtmfDlStream::StopL, exit" )
    }

// ---------------------------------------------------------------------------
// From class CMccSymStreamBase.
// Resets jitterbuffer.
// ---------------------------------------------------------------------------
//
void CMccDtmfDlStream::ResetCountersL()
    {
    __SUBCONTROLLER( "CMccDtmfDlStream::ResetCountersL - NO_OP" )
    }

// ---------------------------------------------------------------------------
// From class CMccSymStreamBase.
// Sets the current record/play balance depending on the stream type.
// ---------------------------------------------------------------------------
//
void CMccDtmfDlStream::SetBalanceL( TInt /*aLeftbal*/, TInt /*aRightbal*/ )
    {
    __SUBCONTROLLER( "CMccDtmfDlStream::SetBalanceL - NO_OP" )
    }

// ---------------------------------------------------------------------------
// From class CMccSymStreamBase.
// Gets the current record/play balance depending on the stream type
// ---------------------------------------------------------------------------
//
void CMccDtmfDlStream::GetBalanceL( TInt& /*aLeftbal*/, TInt& /*aRightbal*/ )
    {
    __SUBCONTROLLER( "CMccDtmfDlStream::GetBalanceL - NO_OP" )
    }

// ---------------------------------------------------------------------------
// From class CMccSymStreamBase.
// Starts inactivity timer for a stream in a given session.
// ---------------------------------------------------------------------------
//
void CMccDtmfDlStream::StartInactivityTimerL( TUint32 /*aTimeoutTime*/ )
    {
    __SUBCONTROLLER( "CMccDtmfDlStream::StartInactivityTimerL - NO_OP" )
    // TBD: Should be pointed once to the RTP source endpoint
    };

// ---------------------------------------------------------------------------
// From class CMccSymStreamBase.
// Stops inactivity timer for a stream in a given session.
// ---------------------------------------------------------------------------
void CMccDtmfDlStream::StopInactivityTimerL()
    {
    __SUBCONTROLLER( "CMccDtmfDlStream::StopInactivityTimerL - NO_OP" )
    // TBD: Should be pointed once to the RTP source endpoint
    }

// ---------------------------------------------------------------------------
// From class CMccSymStreamBase.
// Get synchronization source identifier used by this stream.
// ---------------------------------------------------------------------------
//
void CMccDtmfDlStream::GetSSRCL( TUint32& /*aSSRCValue*/ )
    {
    __SUBCONTROLLER( "CMccDtmfDlStream::GetSSRCL" )
    
    User::Leave( KErrNotSupported );
    }

// ---------------------------------------------------------------------------
// From class CMccSymStreamBase.
// 
// ---------------------------------------------------------------------------
//
TBool CMccDtmfDlStream::IsUplink()
    {
    __SUBCONTROLLER( "CMccDtmfDlStream::IsUplink" )
    
    return EFalse;
    }

// ---------------------------------------------------------------------------
// From class CMccSymStreamBase.
// 
// ---------------------------------------------------------------------------
//
void CMccDtmfDlStream::StandByL()
    {
    __SUBCONTROLLER( "CMccDtmfDlStream::StandByL" )
    }

// -----------------------------------------------------------------------------
// CMccDtmfDlStream::CreatePayloadFormatDecoderL
// Creates a payload format decoder for DTMF payload decoding.
// -----------------------------------------------------------------------------
//
void CMccDtmfDlStream::CreatePayloadFormatDecoderL()
    {
	__SUBCONTROLLER( "CMccDtmfDlStream::CreatePayloadFormatDecoderL" )
	__ASSERT_ALWAYS( iMultiplexer, User::Leave( KErrNotReady ) );
	__ASSERT_ALWAYS( NULL == iFormatDecode, User::Leave( KErrAlreadyExists ) );
	
    TUid formatuid;
    formatuid.iUid = static_cast<TInt32>( iCodecInfo.iPayloadFormatDecoder );
    iFormatDecode = CMMFFormatDecode::NewL( formatuid, iMultiplexer );
	iFormatDecode->SetSourceDataTypeCode( iCodecInfo.iFourCC, Type() );
    
    User::LeaveIfError( iFormatDecode->SourceThreadLogon( *this ) );
    iFormatDecode->SetSourcePrioritySettings( iPrioritySettings );
    
    User::LeaveIfError( 
        iMultiplexer->MultiplexerSourceThreadLogon( 
        *this, *iFormatDecode, ETrue, Type(), EFalse ) );
    
    RArray<TUint> payloads;
    CleanupClosePushL( payloads );
    ParsePayloadTypesL( payloads, iCodecInfo );
    iMultiplexer->RegClientForPayloadsL( *iFormatDecode, payloads );
    CleanupStack::PopAndDestroy( &payloads );
    
	__SUBCONTROLLER( "CMccDtmfDlStream::CreatePayloadFormatDecoderL, exit" )
    }
    
// -----------------------------------------------------------------------------
// CMccDtmfDlStreams::UpdateCodecInformationL
// Updates codec parameters dynamically.
// -----------------------------------------------------------------------------
//
void CMccDtmfDlStream::UpdateCodecInformationL( TMccCodecInfo& aCodecInfo )
    {
	__SUBCONTROLLER( "CMccDtmfDlStream::UpdateCodecInformationL" )
    __ASSERT_ALWAYS( iFormatDecode, User::Leave( KErrNotReady ) );
    __ASSERT_ALWAYS( iDatasink, User::Leave( KErrNotReady ) );
    
    MccCodecConfigurator::ConfigureCodecL( *iFormatDecode, 
        *iDatasink, aCodecInfo, *iRtpMediaClock, CurrentCodecState() );
    
	__SUBCONTROLLER( "CMccDtmfDlStream::UpdateCodecInformationL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccDtmfDlStream::MultiplexerSink
// -----------------------------------------------------------------------------
//
MDataSink* CMccDtmfDlStream::MultiplexerSink()
    {
    __SUBCONTROLLER( "CMccDtmfDlStream::MultiplexerSink" )
    return iFormatDecode;
    }
