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
#include "mccdtmfulstream.h"
#include "mccsubcontrollerlogs.h"
#include "mccdatasink.h"
#include "mcccodecconfigurator.h"
#include "mccrtpmanager.h"
#include "mccrtpdatasink.h"
#include "dtmfpayloadformatwrite.h"
#include "mccrtpmediaclock.h"
#include "mccresources.h"
#include "mccinternaldef.h"
#include "mccrtpdatasource.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that might leave.
// ---------------------------------------------------------------------------
//
CMccDtmfUlStream::CMccDtmfUlStream( TUint32 aMccStreamId, 
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
void CMccDtmfUlStream::ConstructL()
    {
	__SUBCONTROLLER( "CMccDtmfUlStream::ConstructL" )
	__SUBCONTROLLER( "CMccDtmfUlStream::ConstructL, exit" )
    }


// ---------------------------------------------------------------------------
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CMccDtmfUlStream* CMccDtmfUlStream::NewL( TUint32 aMccStreamId, 
        MAsyncEventHandler* aEventHandler, MMccResources* aMccResources,
        CMccRtpManager* aManager, TFourCC aFourCC, TInt aStreamType,
        CMccRtpMediaClock& aClock )
    {
    CMccDtmfUlStream* self = CMccDtmfUlStream::NewLC( aMccStreamId, aEventHandler,
        aMccResources, aManager, aFourCC, aStreamType, aClock );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CMccDtmfUlStream* CMccDtmfUlStream::NewLC( TUint32 aMccStreamId, 
        MAsyncEventHandler* aEventHandler, MMccResources* aMccResources,
        CMccRtpManager* aManager, TFourCC aFourCC, TInt aStreamType,
        CMccRtpMediaClock& aClock )
    {
    CMccDtmfUlStream* self = new( ELeave ) CMccDtmfUlStream( aMccStreamId,
        aEventHandler, aMccResources, aManager, aFourCC, aStreamType, aClock );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// Destructor.
// ---------------------------------------------------------------------------
//
CMccDtmfUlStream::~CMccDtmfUlStream()
    {
    __SUBCONTROLLER( "CMccDtmfUlStream::~CMccDtmfUlStream" )
    
    // Must do mux cleanup before decoder (mux sink) deletion
    MultiplexerCleanup();
    
    if ( iFormatEncode )
        {
        iFormatEncode->SinkThreadLogoff();
        delete iFormatEncode;
        }

    __SUBCONTROLLER( "CMccDtmfUlStream::~CMccDtmfUlStream, exit" )
    }

// ---------------------------------------------------------------------------
// From class CMccSymStreamBase.
// Sets the MMF priority settings for this stream.
// ---------------------------------------------------------------------------
//
void CMccDtmfUlStream::SetPrioritySettingsL( 
        const TMMFPrioritySettings& aPriority )
    {
    __SUBCONTROLLER( "CMccDtmfUlStream::SetPrioritySettingsL start" );
    
   
    if ( iFormatEncode )
        {
        iFormatEncode->SetSinkPrioritySettings( aPriority );
        }
    
    __SUBCONTROLLER( "CMccDtmfUlStream::SetPrioritySettingsL end ");
    }

// ---------------------------------------------------------------------------
// From class CMccSymStreamBase.
// Adds the sink and source to the stream.
// ---------------------------------------------------------------------------
//
void CMccDtmfUlStream::AddSinkAndSourceL( MDataSink* aDataSink,
        MDataSource* aDataSource )
    {
	__SUBCONTROLLER( "CMccDtmfUlStream::AddSinkAndSourceL" )
    __ASSERT_ALWAYS( aDataSink && aDataSource, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( aDataSink->DataSinkType() == KMccRtpSinkUid,
        User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( aDataSource->DataSourceType() == KMccAnySourceUid,
        User::Leave( KErrArgument ) );
    
	__SUBCONTROLLER( "CMccDtmfUlStream::AddSinkAndSourceL, adding" )
	iDatasink = aDataSink;
	iDatasource = aDataSource;
    
    User::LeaveIfError( iDatasource->SourceThreadLogon( *this ) );
    
    __SUBCONTROLLER( "CMccDtmfUlStream::AddSinkAndSourceL, exit" )
    }

// ---------------------------------------------------------------------------
// From class CMccSymStreamBase.
// Loads a codec to the stream.
// ---------------------------------------------------------------------------
//
void CMccDtmfUlStream::LoadCodecL( const TMccCodecInfo& aCodecInfo, 
        const TDesC8& aFmtp )
    {
     __SUBCONTROLLER_INT1( "CMccDtmfUlStream::LoadCodecL, codec state:", 
                          CurrentCodecState() )
    
    __ASSERT_ALWAYS( CurrentCodecState() == EStateCodecNone || 
                     iCodecInfo.iFourCC == aCodecInfo.iFourCC,
                     User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( iDatasink && iDatasource, User::Leave( KErrNotReady ) );
        
    iCodecInfo = aCodecInfo;
    
    if ( CurrentCodecState() == EStateCodecNone )
        {
        delete iFmtpAttr;
        iFmtpAttr = NULL;

    	iFmtpAttr = HBufC8::NewL( aFmtp.Length() );
        iFmtpAttr->Des().Copy( aFmtp );
        
        iCodecInfo.iFmtpLength = iFmtpAttr->Length();
                
        CreatePayloadFormatEncoderL();
        
        SetCodecState( EStateCodecNegotiated );        	
        }
    else if ( CurrentCodecState() == EStateCodecNegotiated )
        {
        SetCodecState( EStateCodecLoaded );
        
        // Codec is not fully usable yet, do just formatter initialization
        MccCodecConfigurator::ConfigureCodecL( *iDatasource, 
                                                *iFormatEncode, 
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
        
    __SUBCONTROLLER_INT1( "CMccDtmfUlStream::LoadCodecL, exit, new state:", 
                          CurrentCodecState() )
    }

// ---------------------------------------------------------------------------
// From class CMccSymStreamBase.
// Primes the stream ready.
// ---------------------------------------------------------------------------
//
void CMccDtmfUlStream::PrimeL( const TUint32 aEndpointId )
    {
	__SUBCONTROLLER( "CMccDtmfUlStream::PrimeL" )
    __ASSERT_ALWAYS( iFormatEncode && iDatasink && iDatasource, 
                     User::Leave( KErrNotReady ) );
	
    TBool controlNetworkResources = SetStateL( EStatePrepared, aEndpointId );  
    
    if ( controlNetworkResources )
        {
        iRtpmanager->CreateTransmitStreamL( *iDatasink, iCodecInfo );
        }
        
    LoadCodecL( iCodecInfo, KNullDesC8 );
    
    if ( IsControlled( MCC_ENDPOINT_ID( iDatasink ) ) )
        {
        iFormatEncode->SinkPrimeL();
        }
    
    if ( IsControlled( MCC_ENDPOINT_ID( iDatasource ) ) )
        {
        iDatasource->SourcePrimeL();
        }

    InformStreamStateChange( KMccStreamPrepared, aEndpointId );
    
	__SUBCONTROLLER( "CMccDtmfUlStream::PrimeL, exit" )
    }

// ---------------------------------------------------------------------------
// From class CMccSymStreamBase.
// Starts the stream. DTMF can be now sent and received.
// ---------------------------------------------------------------------------
//
void CMccDtmfUlStream::PlayL( 
    const TUint32 aEndpointId, 
    TBool /*aStreamPaused*/, 
    TBool /*aEnableRtcp*/ )
    {
	__SUBCONTROLLER( "CMccDtmfUlStream::PlayL" )
    __ASSERT_ALWAYS( iFormatEncode && iDatasink && iDatasource, 
                     User::Leave( KErrNotReady ) );
	
    TBool controlNetworkResources = SetStateL( EStateStreaming, aEndpointId );
    
    if ( controlNetworkResources )
        {
        iRtpmanager->StartSessionL();
        }
    
    LoadCodecL( iCodecInfo, KNullDesC8 );
    
    if ( IsControlled( MCC_ENDPOINT_ID( iDatasink ) ) )
        {
        iFormatEncode->SinkPlayL();
        }
    
    if ( IsControlled( MCC_ENDPOINT_ID( iDatasource ) ) )
        {
        iDatasource->SourcePlayL();
        }

    InformStreamStateChange( KMccStreamStarted, aEndpointId );
    
	__SUBCONTROLLER( "CMccDtmfUlStream::PlayL, exit" )
    }

// ---------------------------------------------------------------------------
// From class CMccSymStreamBase.
// Pauses the stream. DTMF sending and receiving are not possible in paused
// state.
// ---------------------------------------------------------------------------
//
void CMccDtmfUlStream::PauseL( const TUint32 aEndpointId, TBool /*aEnableRtcp*/ )
    {
    __SUBCONTROLLER( "CMccDtmfUlStream::PauseL" )
    __ASSERT_ALWAYS( iFormatEncode && iDatasink && iDatasource, 
                     User::Leave( KErrNotReady ) );
    
    SetStateL( EStatePaused, aEndpointId );
    
    if ( IsControlled( MCC_ENDPOINT_ID( iDatasink ) ) )
        {
        iFormatEncode->SinkPauseL();
        }
    
    if ( IsControlled( MCC_ENDPOINT_ID( iDatasource ) ) )
        {
        iDatasource->SourcePauseL();
        }
    
    InformStreamStateChange( KMccStreamPaused, aEndpointId );
    
    __SUBCONTROLLER( "CMccDtmfUlStream::PauseL, exit" )
    }

// ---------------------------------------------------------------------------
// From class CMccSymStreamBase.
// Resumes the stream.
// ---------------------------------------------------------------------------
//
void CMccDtmfUlStream::ResumeL( const TUint32 aEndpointId, TBool /*aEnableRtcp*/ )
    {
    __SUBCONTROLLER( "CMccDtmfUlStream::ResumeL" )
    __ASSERT_ALWAYS( iFormatEncode && iDatasink && iDatasource, 
                     User::Leave( KErrNotReady ) );
    
    SetStateL( EStateStreaming, aEndpointId );
    
    if ( IsControlled( MCC_ENDPOINT_ID( iDatasink ) ) )
        {
        iFormatEncode->SinkPlayL();
        }
    
    if ( IsControlled( MCC_ENDPOINT_ID( iDatasource ) ) )
        {
        iDatasource->SourcePlayL();
        }
    
    InformStreamStateChange( KMccStreamResumed, aEndpointId );

    __SUBCONTROLLER( "CMccDtmfUlStream::ResumeL, exit" )
    }

// ---------------------------------------------------------------------------
// From class CMccSymStreamBase.
// Stops the stream.
// ---------------------------------------------------------------------------
//
void CMccDtmfUlStream::StopL( const TUint32 aEndpointId )
    {
    __SUBCONTROLLER( "CMccDtmfUlStream::StopL" )
    __ASSERT_ALWAYS( iFormatEncode && iDatasink && iDatasource, 
                     User::Leave( KErrNotReady ) );
    
    SetStateL( EStateStopped, aEndpointId );
    
    if ( IsControlled( MCC_ENDPOINT_ID( iDatasink ) ) )
        {
        iFormatEncode->SinkStopL();
        }
    
    if ( IsControlled( MCC_ENDPOINT_ID( iDatasource ) ) )
        {
        iDatasource->SourceStopL();
        }
    
    InformStreamStateChange( KMccStreamStopped, aEndpointId );
    
    __SUBCONTROLLER( "CMccDtmfUlStream::StopL, exit" )
    }

// ---------------------------------------------------------------------------
// From class CMccSymStreamBase.
// Resets jitterbuffer.
// ---------------------------------------------------------------------------
//
void CMccDtmfUlStream::ResetCountersL()
    {
    __SUBCONTROLLER( "CMccDtmfUlStream::ResetCountersL - NO_OP" )
    }

// ---------------------------------------------------------------------------
// From class CMccSymStreamBase.
// Sets the current record/play balance depending on the stream type.
// ---------------------------------------------------------------------------
//
void CMccDtmfUlStream::SetBalanceL( TInt /*aLeftbal*/, TInt /*aRightbal*/ )
    {
    __SUBCONTROLLER( "CMccDtmfUlStream::SetBalanceL - NO_OP" )
    }

// ---------------------------------------------------------------------------
// From class CMccSymStreamBase.
// Gets the current record/play balance depending on the stream type
// ---------------------------------------------------------------------------
//
void CMccDtmfUlStream::GetBalanceL( TInt& /*aLeftbal*/, TInt& /*aRightbal*/ )
    {
    __SUBCONTROLLER( "CMccDtmfUlStream::GetBalanceL - NO_OP" )
    }

// ---------------------------------------------------------------------------
// From class CMccSymStreamBase.
// Sends a media event to the uplink.
// ---------------------------------------------------------------------------
//
void CMccDtmfUlStream::SendMediaSignalL( const TMccEvent& aEvent )
    {
    __SUBCONTROLLER( "CMccDtmfUlStream::SendMediaSignalL" )
    __ASSERT_ALWAYS( iFormatEncode, User::Leave( KErrNotReady ) );
    
    if ( KMccDtmfControl == aEvent.iEventType )
        {
        const TMccDtmfEventData& dtmfEvent = 
            (*reinterpret_cast<const TMccDtmfEventDataPackage*>( 
                &aEvent.iEventData ))();

        CDTMFPayloadFormatWrite* p 
            = static_cast<CDTMFPayloadFormatWrite*>( iFormatEncode );
        
        switch ( dtmfEvent.iDtmfEventType )
            {
            case KMccDtmfSigStartTone:
                p->StartDTMFToneL( dtmfEvent.iDtmfString[0] );
                break;
            case KMccDtmfSigStopTone:
                p->StopDTMFToneL();
                break;
            case KMccDtmfSigSendString:
                p->SendDTMFTonesL( dtmfEvent.iDtmfString );
                break;
            case KMccDtmfSigContinueSending:
                p->ContinueDTMFStringSending( dtmfEvent.iContinue );
                break;
            case KMccDtmfSigCancelSending:
                p->CancelDTMFStringSending();
                break;
            default:
                __SUBCONTROLLER( "CMccDtmfUlStream::SendMediaSignalL - DEFCASE" )
                break;
            }
        }
    else
        {
        __SUBCONTROLLER( "CMccDtmfUlStream::SendMediaSignalL - NOT DTMF EVENT" )
        User::Leave( KErrNotSupported );
        }

    __SUBCONTROLLER( "CMccDtmfUlStream::SendMediaSignalL, exit" )
    }
    
// ---------------------------------------------------------------------------
// From class CMccSymStreamBase.
// Starts inactivity timer for a stream in a given session.
// ---------------------------------------------------------------------------
//
void CMccDtmfUlStream::StartInactivityTimerL( TUint32 /*aTimeoutTime*/ )
    {
    __SUBCONTROLLER( "CMccDtmfUlStream::StartInactivityTimerL - NO_OP" )
    // TBD: Should be pointed once to the RTP source endpoint
    };

// ---------------------------------------------------------------------------
// From class CMccSymStreamBase.
// Stops inactivity timer for a stream in a given session.
// ---------------------------------------------------------------------------
void CMccDtmfUlStream::StopInactivityTimerL()
    {
    __SUBCONTROLLER( "CMccDtmfUlStream::StopInactivityTimerL - NO_OP" )
    // TBD: Should be pointed once to the RTP source endpoint
    }

// ---------------------------------------------------------------------------
// From class CMccSymStreamBase.
// Get synchronization source identifier used by this stream.
// ---------------------------------------------------------------------------
//
void CMccDtmfUlStream::GetSSRCL( TUint32& aSSRCValue )
    {
    __SUBCONTROLLER( "CMccDtmfUlStream::GetSSRCL" )
    __ASSERT_ALWAYS( iDatasink, User::Leave( KErrNotReady ) );
    
    aSSRCValue = static_cast<CMccRtpDataSink*>( iDatasink )->GetSSRC();
    
    __SUBCONTROLLER( "CMccDtmfUlStream::GetSSRCL, exit" )
    }

// ---------------------------------------------------------------------------
// From class CMccSymStreamBase.
// 
// ---------------------------------------------------------------------------
//
TBool CMccDtmfUlStream::IsUplink()
    {
    __SUBCONTROLLER( "CMccDtmfUlStream::IsUplink" )
    
    return ETrue;
    }

// ---------------------------------------------------------------------------
// From class CMccSymStreamBase.
// 
// ---------------------------------------------------------------------------
//
void CMccDtmfUlStream::StandByL()
    {
    __SUBCONTROLLER( "CMccDtmfUlStream::StandByL" )
    }

// -----------------------------------------------------------------------------
// CMccDtmfUlStream::CreatePayloadFormatEncoderL
// Creates a payload format encoder for DTMF payload encoding.
// -----------------------------------------------------------------------------
//
void CMccDtmfUlStream::CreatePayloadFormatEncoderL()
    {
	__SUBCONTROLLER( "CMccDtmfUlStream::CreatePayloadFormatEncoderL" )
	__ASSERT_ALWAYS( NULL == iFormatEncode, User::Leave( KErrAlreadyExists ) );
	
    TUid formatuid;
    formatuid.iUid = static_cast<TInt32>( iCodecInfo.iPayloadFormatEncoder );
    iFormatEncode = CMMFFormatEncode::NewL( formatuid, iDatasink );
	iFormatEncode->SetSinkDataTypeCode( iCodecInfo.iFourCC, iCodecInfo.iType );
    
    User::LeaveIfError( iFormatEncode->SinkThreadLogon( *this ) );
    iFormatEncode->SetSinkPrioritySettings( iPrioritySettings );
    
	__SUBCONTROLLER( "CMccDtmfUlStream::CreatePayloadFormatEncoderL, exit" )
    }
    
// -----------------------------------------------------------------------------
// CMccDtmfUlStreams::UpdateCodecInformationL
// Updates codec parameters dynamically.
// -----------------------------------------------------------------------------
//
void CMccDtmfUlStream::UpdateCodecInformationL( TMccCodecInfo& aCodecInfo )
    {
	__SUBCONTROLLER( "CMccDtmfUlStream::UpdateCodecInformationL" )
    __ASSERT_ALWAYS( iFormatEncode && iDatasink && iDatasource, 
                     User::Leave( KErrNotReady ) );
    
    MccCodecConfigurator::ConfigureCodecL( *iDatasource, 
        *iFormatEncode, aCodecInfo, *iRtpMediaClock, CurrentCodecState() );
    
	__SUBCONTROLLER( "CMccDtmfUlStream::UpdateCodecInformationL, exit" )
    }

