/*
* Copyright (c) 2005-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Mcc Symmetric subthread downlink stream
*
*/



// INCLUDE FILES
#include <mmf/server/mmfformat.h>
#include <mmf/server/mmfaudiooutput.h>
#include "mccsymdlstream.h"
#include "mccdldatapath.h"
#include "mccjitterbuffer.h"
#include "mccrtpmanager.h"
#include "mcccodecconfigurator.h"
#include "mccinternaldef.h"
#include "mccaudiorouter.h"
#include "mccsubcontrollerlogs.h"
#include "mccmultiplexer.h"
#include "mccresources.h"
#include "mccredpayloadread.h"

// CONSTANTS

// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccSymDlStream::CMccSymDlStream
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMccSymDlStream::CMccSymDlStream( 
    TUint32 aMccStreamId, 
    MAsyncEventHandler* aEventhandler, 
    MMccResources* aMccResources, 
    CMccRtpManager* aManager, 
    TInt aStreamType,
    CMccRtpMediaClock& aClock ) : 
    CMccSymStreamBase( aMccStreamId, 
                       aEventhandler, 
                       aMccResources, 
                       aManager, 
                       aStreamType ), 
    iJitterBuffer( NULL ), 
    iFormatDecode( NULL )
    {
    iRtpMediaClock = &aClock;
    }

// -----------------------------------------------------------------------------
// CMccSymDlStream::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMccSymDlStream* CMccSymDlStream::NewLC( 
    TUint32 aMccStreamId, 
    MAsyncEventHandler* aEventhandler, 
    MMccResources* aMccResources,
    CMccRtpManager* aManager, 
    TInt aStreamType,
    CMccRtpMediaClock& aClock ) 
    {
    CMccSymDlStream* s = 
            new ( ELeave ) CMccSymDlStream( aMccStreamId, 
                                            aEventhandler, 
                                            aMccResources,
                                            aManager, 
                                            aStreamType,
                                            aClock );
    CleanupStack::PushL( s );
    s->ConstructL();
    return s;
    }

// -----------------------------------------------------------------------------
// CMccSymDlStream::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMccSymDlStream::ConstructL()
    {
    iDatapath = 
        CMccDlDataPath::NewL( this, iMccResources, KUidMediaTypeAudio );
    iJitterBuffer = CMccJitterBuffer::NewL( this );
    }

// -----------------------------------------------------------------------------
// CMccSymDlStream::~CMccSymDlStream
// Destructor
// -----------------------------------------------------------------------------
//
CMccSymDlStream::~CMccSymDlStream()
    {
    delete iAudioRouter;
    
    if ( iSoundDevice )
        {
        iSoundDevice = NULL;
        }
        
    if ( iJitterBuffer )
        {
        iJitterBuffer->SinkThreadLogoff();
        delete iJitterBuffer;
        iJitterBuffer = NULL;
        }
    
    // Must do mux cleanup before decoder (mux sink) deletion
    MultiplexerCleanup();
         
    if ( iFormatDecode )
        {
        iFormatDecode->SourceThreadLogoff();
        delete iFormatDecode;
        iFormatDecode = NULL;
        }
    
    delete iRedFormatDecode;
    }

// -----------------------------------------------------------------------------
// CMccSymDlStream::SetPrioritySettingsL
// Sets the MMF priority settings for this stream
// -----------------------------------------------------------------------------
//
void CMccSymDlStream::SetPrioritySettingsL( 
        const TMMFPrioritySettings& aPriority )
    {
    __ASSERT_ALWAYS( iDatasink, User::Leave( KErrNotReady ) );    
    
    iDatasink->SetSinkPrioritySettings( aPriority );
    }

// -----------------------------------------------------------------------------
// CMccSymDlStream::AddSinkAndSourceL
// From CMccSymStreamBase
// -----------------------------------------------------------------------------
//
void CMccSymDlStream::AddSinkAndSourceL( MDataSink* aDatasink, 
        MDataSource* aDatasource )
    {
    __SUBCONTROLLER( "CMccSymDlStream::AddSinkAndSourceL" )
    
    __ASSERT_ALWAYS( aDatasink && aDatasource, User::Leave( KErrArgument ) );

    iDatasink = aDatasink;
    iDatasource = aDatasource;

    iMultiplexer = iMccResources->MultiplexerL( iDatasource );
    
 	if ( iDatasink->DataSinkType() == KUidMmfAudioOutput )
    	{
	    iJitterBuffer->AddDataSinkL( iDatasink );
	    
	    // Cause the devsound to be loaded, as this call will pass through
	    // jitterbuffer
	    User::LeaveIfError( iJitterBuffer->SinkThreadLogon( *this ) );
	    
	    __SUBCONTROLLER( "CMccSymDlStream::AddSinkAndSourceL, devsound loaded" )
	    
	    iSoundDevice = &( static_cast<MMMFAudioOutput*>( iDatasink )->SoundDevice() );
	    
	    // Create new audio router
	    delete iAudioRouter;
        iAudioRouter = 0;
        iAudioRouter = CMccAudioRouter::NewL( *iSoundDevice );
    
        iDatapath->AddDataSinkL( iJitterBuffer );
    	}
    else if ( iDatasink->DataSinkType() == KMccVideoSinkUid )
    	{
        iDatapath->AddDataSinkL( iDatasink );
                                 
        User::LeaveIfError( iDatasink->SinkThreadLogon(*this ) );
    	}
    else
        {
        }
        
    // Inform datapath about ids of associated sink and source
    iDatapath->SetAssociatedSink( iDatasink );
    iDatapath->SetAssociatedSource( iDatasource );
    
    __SUBCONTROLLER( "CMccSymDlStream::AddSinkAndSourceL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccSymDlStream::LoadCodecL
// From CMccSymStreamBase
// -----------------------------------------------------------------------------
//
void CMccSymDlStream::LoadCodecL( const TMccCodecInfo& aCodecInfo,
                                  const TDesC8& aFmtp )
    {
    __SUBCONTROLLER_INT1( "CMccSymDlStream::LoadCodecL codec state:", 
        CurrentCodecState() )
    
    __ASSERT_ALWAYS( CurrentCodecState() == EStateCodecNone || 
                     iCodecInfo.iFourCC == aCodecInfo.iFourCC,
                     User::Leave( KErrArgument ) );
    
    TMccCodecInfo orig = iCodecInfo;
    iCodecInfo = aCodecInfo;
    
    if ( CurrentCodecState() == EStateCodecNone )
        {
        delete iFmtpAttr;
        iFmtpAttr = NULL;

    	iFmtpAttr = HBufC8::NewL( aFmtp.Length() );
        iFmtpAttr->Des().Copy( aFmtp );
        
        iCodecInfo.iFmtpLength = iFmtpAttr->Length();
        CreatePayloadFormatDecoderL();
        
        // Just negotiate the codec, load it later
        static_cast<CMccDlDataPath*>( iDatapath )->NegotiateL( *iFormatDecode );
        
        SetCodecState( EStateCodecNegotiated );        	
        }
    else if ( CurrentCodecState() == EStateCodecNegotiated )
        {
        // Codec is not fully usable yet, do just formatter initialization
        MccCodecConfigurator::ConfigureCodecL( *iFormatDecode,
                                                *iDatasink,
                                                aCodecInfo,
                                                *iRtpMediaClock,
                                                CurrentCodecState() );
                                                
        SetCodecState( EStateCodecPrepared );
        }
    else if ( CurrentCodecState() == EStateCodecPrepared )
        {
        if ( CodecLoadingAllowed() )
            {
            // Load the codec
            static_cast<CMccDlDataPath*>( iDatapath )->LoadL( *iFormatDecode );
            
            TInt jbufLen = iCodecInfo.iMaxPtime;
            iJitterBuffer->SetupL( jbufLen, 
                                   iCodecInfo.iJitterBufThreshold, 
                                   iCodecInfo );
            
            // Set state before updating as the state affects to update functionality
            SetCodecState( EStateCodecLoaded );
                                               
            UpdateCodecInformationL( orig, iCodecInfo );
            }
        }
    else if ( CurrentCodecState() == EStateCodecLoaded ||
              CurrentCodecState() == EStateCodecLoadedAndUpdating )
        {
        iJitterBuffer->SetupL( iCodecInfo.iMaxPtime, 
                       		   iCodecInfo.iJitterBufThreshold, 
                       		   iCodecInfo );
        
        // Update codec info
        if ( iDatasource->DataSourceType() == KMccRtpSourceUid )
       	    {
            CMccRtpDataSource* dataSource = 
                static_cast<CMccRtpDataSource*>( iDatasource );
            
            // For updating keep alive parameters
            TMccCodecInfoBuffer infoBuffer( iCodecInfo );     
            dataSource->ConfigureL( infoBuffer, iRtpMediaClock );
       	    }
        SetCodecState( EStateCodecLoadedAndUpdating );
    	UpdateCodecInformationL( orig, iCodecInfo );
        }
    else
        {
        User::Leave( KErrNotReady );
        }
        
    __SUBCONTROLLER_INT1( "CMccSymDlStream::LoadCodecL exit, new state:", 
        CurrentCodecState() )
    }

// -----------------------------------------------------------------------------
// CMccSymDlStream::UpdateCodecInformationL
// From CMccSymStreamBase
// -----------------------------------------------------------------------------
//
void CMccSymDlStream::UpdateCodecInformationL( const TMccCodecInfo& aOriginal,
    const TMccCodecInfo& aNew )
    {
    __SUBCONTROLLER( "CMccSymDlStream::UpdateCodecInformationL Entry" )
    
    // UpdateCodecInformationL is called from LoadCodecL which checks that
    // codec itself is not changed. Here we re-configure the codec
    // parameters.
    __ASSERT_ALWAYS( iFormatDecode, User::Leave( KErrNotReady ) );
    __ASSERT_ALWAYS( iDatasink, User::Leave( KErrNotReady ) );
    
    HandleSpecialSinksAndSources();
    
    // NB; updated in LoadCodecL, update information added here
    iCodecInfo.iIsUpdate = ETrue;
    
    TBool specNeed = MccCodecConfigurator::IlbcNeedsStopResume(
            aOriginal.iBitrate,
            iCodecInfo.iBitrate,
            iCodecInfo.iFourCC );
    
    // Need to check also the codec state because CurrentCodecState may
    // return such codec state that does not allow iLBC reconfiguration.
    TCodecState codecState = CurrentCodecState();
    if ( specNeed && EStateStreaming == CurrentState() )
        {
        iJitterBuffer->SinkStopL();
        codecState = CMccSymStreamBase::EStateCodecLoaded;
        }
    
    MccCodecConfigurator::ConfigureCodecL( *iFormatDecode,
                                           *iDatasink,
                                           aNew,
                                           *iRtpMediaClock,
                                           codecState );
    
    if ( specNeed && EStateStreaming == CurrentState() )
        {
        iJitterBuffer->SinkPlayL();
        }
    
    __SUBCONTROLLER( "CMccSymDlStream::UpdateCodecInformationL Exit" )
    }
    


// -----------------------------------------------------------------------------
// CMccSymDlStream::CreatePayloadFormatDecoderL
// Creates the payload format decoder
// -----------------------------------------------------------------------------
//
void CMccSymDlStream::CreatePayloadFormatDecoderL()
    {
    __SUBCONTROLLER( "CMccSymDlStream::CreatePayloadFormatEncoderL" )
    __ASSERT_ALWAYS( NULL == iFormatDecode, User::Leave( KErrAlreadyExists ) );
    __ASSERT_ALWAYS( iMultiplexer, User::Leave( KErrNotReady ) );
    
    TUid formatuid;
    formatuid.iUid = iCodecInfo.iPayloadFormatDecoder;
    
    RArray<TUint> payloads;
    CleanupClosePushL( payloads );
    ParsePayloadTypesL( payloads, iCodecInfo );
    
    if ( EGenRedUsed == iCodecInfo.iAlgoUsed )
        {
        __ASSERT_ALWAYS( KMccPayloadTypeMax != iCodecInfo.iRedundantPayload,
            User::Leave( KErrArgument ) );
        
        iRedFormatDecode = CMMFFormatDecode::NewL(
            TUid::Uid( KImplUidRedPayloadFormatDecode ), iMultiplexer );
        
        iFormatDecode = CMMFFormatDecode::NewL( formatuid, iRedFormatDecode );
        User::LeaveIfError( iMultiplexer->MultiplexerSourceThreadLogon( *this, 
            *iRedFormatDecode, ETrue, Type(), EFalse ) );
        
        iMultiplexer->RegClientForPayloadsL( *iRedFormatDecode, payloads );
        }
    else
        {
        iFormatDecode = CMMFFormatDecode::NewL( formatuid, iMultiplexer );
        User::LeaveIfError( iMultiplexer->MultiplexerSourceThreadLogon( *this, 
            *iFormatDecode, ETrue, Type(), EFalse ) );
        
        iMultiplexer->RegClientForPayloadsL( *iFormatDecode, payloads );
        }
    
    iFormatDecode->SetSourceDataTypeCode( iCodecInfo.iFourCC, Type() );
    User::LeaveIfError( iFormatDecode->SourceThreadLogon( *this ) );

    CleanupStack::PopAndDestroy( &payloads );
    }

// -----------------------------------------------------------------------------
// CMccSymDlStream::PrimeL
// Primes the audio stream ready
// -----------------------------------------------------------------------------
//
void CMccSymDlStream::PrimeL( const TUint32 aEndpointId )
    {
    __SUBCONTROLLER( "CMccSymDlStream::PrimeL" )
    
    TBool controlNetworkResources = SetStateL( EStatePrepared, aEndpointId );    

    if ( controlNetworkResources )
        {
        iRtpmanager->CreateReceiveStreamL( *iDatasource, iCodecInfo );
        }
            
    LoadCodecL( iCodecInfo, KNullDesC8 ); 
                                                    
    DoPrimeL();
    
    InformStreamStateChange( KMccStreamPrepared, aEndpointId );
    }

// -----------------------------------------------------------------------------
// CMccSymDlStream::PlayL
// Starts audio streaming
// -----------------------------------------------------------------------------
//
void CMccSymDlStream::PlayL( const TUint32 aEndpointId,
							 TBool /*aStreamPaused*/, 
                             TBool aEnableRtcp )
    {
    __SUBCONTROLLER( "CMccSymDlStream::PlayL" )
        
    TBool controlNetworkResources = SetStateL( EStateStreaming, aEndpointId );
    
    if ( controlNetworkResources )
        {
        iRtpmanager->StartSessionL();
        }
    
    User::LeaveIfError( iRtpmanager->SetRTCPSendReceive( aEnableRtcp ) ); 
    
    UpdateCodecLoadingPermission( IsCodecOperationAllowed() );
    
    LoadCodecL( iCodecInfo, KNullDesC8 );
    
    DoPlayL();
    
    HandleStandby( aEndpointId, EFalse );

    InformStreamStateChange( KMccStreamStarted, aEndpointId );
    
    __SUBCONTROLLER( "CMccSymDlStream::PlayL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccSymDlStream::PauseL
// Pauses audio streaming
// -----------------------------------------------------------------------------
//
void CMccSymDlStream::PauseL( const TUint32 aEndpointId, TBool aEnableRtcp )
    {
    __SUBCONTROLLER( "CMccSymDlStream::PauseL" )
    
    /*TBool standbyEndpointControl( StandbyStream() && 
                                  aEndpointId == MCC_ENDPOINT_ID( iDatasink ) );*/
    
    SetStateL( EStatePaused, aEndpointId );
    User::LeaveIfError( iRtpmanager->SetRTCPSendReceive( aEnableRtcp ) );
    
    DoPauseL();
    
    HandleStandby( aEndpointId, ETrue );
    
    InformStreamStateChange( KMccStreamPaused, aEndpointId ); 
    }

// -----------------------------------------------------------------------------
// CMccSymDlStream::ResumeL
// Resumes paused audio streaming
// -----------------------------------------------------------------------------
//
void CMccSymDlStream::ResumeL( const TUint32 aEndpointId, TBool aEnableRtcp )
    { 
    __SUBCONTROLLER( "CMccSymDlStream::ResumeL" )
    
    // In first resume of the standby stream sink is treated as in normal
    // startup phase
    TBool standbyEndpointControl( StandbyStream() && 
                                  aEndpointId == MCC_ENDPOINT_ID( iDatasink ) );
    
    if ( standbyEndpointControl && !iFirstResumeDone )
        {
        __SUBCONTROLLER( "CMccSymDlStream::ResumeL, standby, first resume" )
        
        SetStateL( EStateStreaming, aEndpointId );
        
        UpdateCodecLoadingPermission( IsCodecOperationAllowed() );
    
        LoadCodecL( iCodecInfo, KNullDesC8 );
        
        DoPlayL();
        
        iFirstResumeDone = IsControllingMmfEndpoint();
        }
    else
        {
        /*TBool controlNetworkResources = */
        SetStateL( EStateStreaming, aEndpointId );

        User::LeaveIfError( iRtpmanager->SetRTCPSendReceive( aEnableRtcp ) );

        DoResumeL();
        }
        
    HandleStandby( aEndpointId, EFalse );
       
    InformStreamStateChange( KMccStreamResumed, aEndpointId );
    
    __SUBCONTROLLER( "CMccSymDlStream::ResumeL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccSymDlStream::StopL
// Stops the audio streaming
// -----------------------------------------------------------------------------
//
void CMccSymDlStream::StopL( const TUint32 aEndpointId )
    {
    __SUBCONTROLLER( "CMccSymDlStream::StopL" )
    
    SetStateL( EStateStopped, aEndpointId );    
    
    DoStopL();
    
    InformStreamStateChange( KMccStreamStopped, aEndpointId );
    }
    
// -----------------------------------------------------------------------------
// CMccSymDlStream::SetBalanceL
// Sets the current play balance
// -----------------------------------------------------------------------------
//
void CMccSymDlStream::SetBalanceL( TInt aLeftBal, TInt aRightBal )
    {
    if( !iSoundDevice )
        {
        User::Leave( KErrNotReady );
        }
    else
        {
        iSoundDevice->SetPlayBalanceL( aLeftBal, aRightBal );
        }
    }

// -----------------------------------------------------------------------------
// CMccSymDlStream::GetBalanceL
// Returns the current play balance
// -----------------------------------------------------------------------------
//
void CMccSymDlStream::GetBalanceL( TInt& aLeftbal, TInt& aRightbal )
    {
    if( !iSoundDevice )
        {
        User::Leave( KErrNotReady );
        }
    else
        {
        iSoundDevice->GetPlayBalanceL( aLeftbal, aRightbal );
        }
    }
    
// -----------------------------------------------------------------------------
// CMccSymDlStream::GetSSRCL
// Returns SSRC value
// -----------------------------------------------------------------------------
//
void CMccSymDlStream::GetSSRCL( TUint32& /*aSSRCValue*/ )
    {
    User::Leave ( KErrNotSupported );
    }


// -----------------------------------------------------------------------------
// CMccSymDlStream::ResetCountersL
// Resets Jitterbuffer
// -----------------------------------------------------------------------------
//
void CMccSymDlStream::ResetCountersL( )
    {
    }

// -----------------------------------------------------------------------------
// CMccSymDlStream::SendMediaSignalL
// Downlink can send RTCP media signals (only receiver reports)
// -----------------------------------------------------------------------------
//
void CMccSymDlStream::SendMediaSignalL( const TMccEvent& aEvent )
    {
    __ASSERT_ALWAYS( iDatasource, User::Leave( KErrNotReady ) );
    
    CMccRtpDataSource* p = static_cast<CMccRtpDataSource*>( iDatasource );
    p->SendMediaSignallingL( aEvent );
    }
        
// -----------------------------------------------------------------------------
// CMccSymDlStream::SendRTCPReceiverReportL
// Sends a RTCP receiver report
// -----------------------------------------------------------------------------
//  
void CMccSymDlStream::SendRTCPReceiverReportL()
    {
    User::LeaveIfNull( iDatasource );
    CMccRtpDataSource* dSource = static_cast<CMccRtpDataSource*>( iDatasource );
    User::LeaveIfError( dSource->SendRTCPReceiverReport() );
    }

// -----------------------------------------------------------------------------
// CMccSymDlStream::StartInactivityTimerL
// Starts inactivity timer for a stream
// -----------------------------------------------------------------------------
// 
void CMccSymDlStream::StartInactivityTimerL( TUint32 aTimeoutTime )
    {
    User::LeaveIfNull( iDatasource );
    CMccRtpDataSource* dSource = static_cast<CMccRtpDataSource*>( iDatasource );
    dSource->StartInactivityTimerL( aTimeoutTime );
    }

      
// -----------------------------------------------------------------------------
// CMccSymDlStream::StopInactivityTimerL
// Stops inactivity timer for a stream 
// -----------------------------------------------------------------------------
// 
void CMccSymDlStream::StopInactivityTimerL()
    {
    User::LeaveIfNull( iDatasource );
    CMccRtpDataSource* dSource = static_cast<CMccRtpDataSource*>( iDatasource );
    dSource->StopInactivityTimerL( );
    }

// -----------------------------------------------------------------------------
// CMccSymDlStream::IsUplink
// -----------------------------------------------------------------------------
// 
TBool CMccSymDlStream::IsUplink()
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CMccSymDlStream::StandByL
// -----------------------------------------------------------------------------
// 
void CMccSymDlStream::StandByL()
    {
    User::LeaveIfNull( iDatasource );
    CMccRtpDataSource* dSource = static_cast<CMccRtpDataSource*>( iDatasource );
    dSource->StandBy( EForceStandby, iCodecInfo.iPayloadType );
    iJitterBuffer->ResetBuffer( EFalse );
    }

// -----------------------------------------------------------------------------
// CMccSymDlStream::SetAudioRouteL
// -----------------------------------------------------------------------------
//    
void CMccSymDlStream::SetAudioRouteL( TUint32 aRoutingDestination )
    {
    __ASSERT_ALWAYS( iAudioRouter, User::Leave( KErrNotReady ) );
    iAudioRouter->SetAudioRouteL( aRoutingDestination );
    }

// -----------------------------------------------------------------------------
// CMccSymDlStream::GetAudioRouteL
// -----------------------------------------------------------------------------
//     
void CMccSymDlStream::GetAudioRouteL( TUint32& aRoutingDestination )
    {
    __ASSERT_ALWAYS( iAudioRouter, User::Leave( KErrNotReady ) );
    iAudioRouter->GetAudioRouteL( aRoutingDestination );
    }

// -----------------------------------------------------------------------------
// CMccSymDlStream::JitterBufferError
// -----------------------------------------------------------------------------
//
void CMccSymDlStream::JitterBufferError( TInt aError )
    {
    __SUBCONTROLLER_INT1("CMccSymDlStream::JitterBufferError, err=", aError)
    if ( aError == MJitterBufferObserver::EBufferOverflow && iDatasource )
        {
        CMccRtpDataSource* dSource = static_cast<CMccRtpDataSource*>( iDatasource );
        dSource->StandBy( EForceStandby, iCodecInfo.iPayloadType ); 
        }
       
    __SUBCONTROLLER( "CMccSymDlStream::JitterBufferError, exit" )

    }
    
// -----------------------------------------------------------------------------
// CMccSymDlStream::MultiplexerSink
// -----------------------------------------------------------------------------
//
MDataSink* CMccSymDlStream::MultiplexerSink()
    {
    if ( EGenRedUsed == iCodecInfo.iAlgoUsed )
        {
        return iRedFormatDecode;
        }
    else
        {
        return iFormatDecode;
        }
    }

// -----------------------------------------------------------------------------
// CMccSymDlStream::DynamicBufferChangeRequest
// -----------------------------------------------------------------------------
//
void CMccSymDlStream::DynamicBufferChangeRequest( TInt aBufferSize )
    {
    const TInt KMccBitsPerByte = 8;
    const TInt hwFrameTime( aBufferSize / KMccBitsPerByte );
    
    __SUBCONTROLLER_INT1("CMccSymDlStream::DynamicBufferChangeRequest, aBufferSize = ", aBufferSize ) 
    __SUBCONTROLLER_INT1("CMccSymDlStream::DynamicBufferChangeRequest, hwFrameTime = ", hwFrameTime )
    __SUBCONTROLLER_INT1("CMccSymDlStream::DynamicBufferChangeRequest, iHwFrameTime = ", iCodecInfo.iHwFrameTime )
    
    if ( hwFrameTime != iCodecInfo.iHwFrameTime
        && iCodecInfo.iFourCC == KMccFourCCIdG711 )
        {
        TRAPD( err, DoDynamicFrameSizeChangeL( hwFrameTime ) );
        if ( KErrNone == err )
            {
            iCodecInfo.iHwFrameTime = hwFrameTime;
            }
        }
    }

// -----------------------------------------------------------------------------
// CMccSymDlStream::DynamicBufferChangeRequest
// -----------------------------------------------------------------------------
//
void CMccSymDlStream::DoDynamicFrameSizeChangeL( TInt aFrameTime )
    {
    __SUBCONTROLLER( "CMccSymDlStream::DoDynamicFrameSizeChangeL" )
    
    __ASSERT_ALWAYS( iFormatDecode && iJitterBuffer,
                    User::Leave( KErrNotReady ) );
    
    TMccCodecInfo codecInfo( iCodecInfo );
    codecInfo.iHwFrameTime = aFrameTime;
    codecInfo.iIsUpdate = ETrue;
    const TMccCodecInfoBuffer buffer( codecInfo );
    
    CPayloadFormatRead* g711Format(
        static_cast<CPayloadFormatRead*>( iFormatDecode ) );
        
    g711Format->ConfigurePayloadFormatL( buffer );
        
    const TInt jbufLen( codecInfo.iMaxPtime );
    iJitterBuffer->SetupL( jbufLen, 
                           codecInfo.iJitterBufThreshold,
                           codecInfo );
    }

// -----------------------------------------------------------------------------
// CMccSymDlStream::BindContextIntoStreamL
// -----------------------------------------------------------------------------
//
void CMccSymDlStream::BindContextIntoStreamL( TUint32 aEndpointId,
                                      const TMccCryptoContext& aContextParams )
    {
    __ASSERT_ALWAYS( iDatasource, User::Leave( KErrNotReady ) );   
    
    if ( iDatasource->DataSourceType() == KMccRtpSourceUid &&
         aEndpointId == MCC_ENDPOINT_ID( iDatasource )  )
	    {
        CMccRtpDataSource* dataSource =
            static_cast<CMccRtpDataSource*>( iDatasource );
            
        dataSource->ApplyCryptoContextL( aContextParams );
	    }
	else
	    {
	    User::Leave( KErrNotFound );
	    }        
    }
// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File  
