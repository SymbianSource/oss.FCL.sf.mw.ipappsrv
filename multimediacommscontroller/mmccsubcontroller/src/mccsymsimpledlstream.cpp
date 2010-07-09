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
* Description:    MCP Symmetric subthread downlink stream
*
*/



// INCLUDE FILES
#include <mmf/server/mmfformat.h>
#include <mmf/server/mmfaudiooutput.h>
#include "mccsymsimpledlstream.h"
#include "mccdldatapath.h"
#include "mccjitterbuffer.h"
#include "mccuids.hrh"
#include "mccrtpmanager.h"
#include "mcccodecconfigurator.h"
#include "mccinternaldef.h"
#include "mccvideosink.h"
#include "mccsubcontrollerlogs.h"
#include "mccmultiplexer.h"
#include "mccresources.h"

// CONSTANTS

// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccSymSimpleDlStream::CMccSymSimpleDlStream
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMccSymSimpleDlStream::CMccSymSimpleDlStream( 
    TUint32 aMccStreamId, 
    MAsyncEventHandler* aEventhandler, 
    MMccResources* aMccResources,
    CMccRtpManager* aManager, 
    TFourCC aFourCC,
    TInt aStreamType,
    CMccRtpMediaClock& aClock ) : 
    CMccSymStreamBase( aMccStreamId, 
                       aEventhandler, 
                       aMccResources, 
                       aManager, 
                       aStreamType )
    {
    iFourCC = aFourCC;
    iRtpMediaClock = &aClock;
    }

// -----------------------------------------------------------------------------
// CMccSymSimpleDlStream::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMccSymSimpleDlStream* CMccSymSimpleDlStream::NewLC( 
    TUint32 aMccStreamId, 
    MAsyncEventHandler* aEventhandler, 
    MMccResources* aMccResources,
    CMccRtpManager* aManager, 
    TFourCC aFourCC,
    TInt aStreamType,
    CMccRtpMediaClock& aClock ) 
    {
    CMccSymSimpleDlStream* s = 
            new ( ELeave ) CMccSymSimpleDlStream( aMccStreamId, 
                                                  aEventhandler,
                                                  aMccResources, 
                                                  aManager, 
                                                  aFourCC, 
                                                  aStreamType,
                                                  aClock );
    CleanupStack::PushL( s );
    s->ConstructL();

    return s;
    }

// -----------------------------------------------------------------------------
// CMccSymSimpleDlStream::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMccSymSimpleDlStream::ConstructL()
    {
    iDatapath = CMccDlDataPath::NewL( this, iMccResources, Type() );
    }

// -----------------------------------------------------------------------------
// CMccSymSimpleDlStream::~CMccSymSimpleDlStream
// Destructor
// -----------------------------------------------------------------------------
//
CMccSymSimpleDlStream::~CMccSymSimpleDlStream()
    {
    if( iSoundDevice )
        {
        iSoundDevice = NULL;
        }
    
    // Must do mux cleanup before decoder (mux sink) deletion
    MultiplexerCleanup();
        
    if( iFormatDecode )
        {
        iFormatDecode->SourceThreadLogoff();
        delete iFormatDecode;
        iFormatDecode = NULL;
        }
    }

// -----------------------------------------------------------------------------
// CMccSymSimpleDlStream::SetPrioritySettingsL
// Sets the MMF priority settings for this stream
// -----------------------------------------------------------------------------
//
void CMccSymSimpleDlStream::SetPrioritySettingsL( 
    const TMMFPrioritySettings& aPriority )
    {
    __ASSERT_ALWAYS( iDatasink, User::Leave( KErrNotReady ) ); 
   
    iDatasink->SetSinkPrioritySettings( aPriority );
    }

// -----------------------------------------------------------------------------
// CMccSymSimpleDlStream::AddSinkAndSourceL
// From CMccSymStreamBase
// -----------------------------------------------------------------------------
//
void CMccSymSimpleDlStream::AddSinkAndSourceL( 
    MDataSink* aDatasink, 
    MDataSource* aDatasource )
    {
    __ASSERT_ALWAYS( aDatasink && aDatasource, User::Leave( KErrArgument ) );

    iDatasink = aDatasink;
    iDatasource = aDatasource;    
    
    iMultiplexer = iMccResources->MultiplexerL( iDatasource );
    
    // Inform datapath about ids of associated sink and source
    iDatapath->SetAssociatedSink( iDatasink );
    iDatapath->SetAssociatedSource( iDatasource );
	        
	iDatapath->AddDataSinkL( aDatasink );

    User::LeaveIfError( iDatasink->SinkThreadLogon( *this ) );
	      
    iDatapath->AddDataSourceL( aDatasource );		    
    }

// -----------------------------------------------------------------------------
// CMccSymSimpleDlStream::LoadCodecL
// From CMccSymStreamBase
// -----------------------------------------------------------------------------
//
void CMccSymSimpleDlStream::LoadCodecL( 
    const TMccCodecInfo& aCodecInfo,
    const TDesC8& aFmtp )
    {
    __SUBCONTROLLER_INT1( "CMccSymSimpleDlStream::LoadCodecL, codec state:", 
                          CurrentCodecState() )
    
    __ASSERT_ALWAYS( CurrentCodecState() == EStateCodecNone || 
                     iCodecInfo.iFourCC == aCodecInfo.iFourCC,
                     User::Leave( KErrArgument ) );
        
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
                                                
        MccCodecConfigurator::ConfigureCodecL( *iDatasource,
                                                *iDatasink, 
                                                iCodecInfo,
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
            
            // Set state before updating as the state affects to update functionality
            SetCodecState( EStateCodecLoaded );
                                               
            UpdateCodecInformationL( iCodecInfo );
            }
        }
    else if ( CurrentCodecState() == EStateCodecLoaded ||
              CurrentCodecState() == EStateCodecLoadedAndUpdating )
        {
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
    	UpdateCodecInformationL( iCodecInfo );
        }
    else
        {
        User::Leave( KErrNotReady );
        }
        
    __SUBCONTROLLER_INT1( "CMccSymSimpleDlStream::LoadCodecL, exit, new state:", 
                          CurrentCodecState() )
    }

// -----------------------------------------------------------------------------
// CMccSymSimpleDlStream::UpdateCodecInformationL
// From CMccSymStreamBase
// -----------------------------------------------------------------------------
//
void CMccSymSimpleDlStream::UpdateCodecInformationL( TMccCodecInfo& aCodecInfo )
    {
    __ASSERT_ALWAYS( iFormatDecode, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( iDatasink, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( iDatasource, User::Leave( KErrArgument ) );

    HandleSpecialSinksAndSources();
    
    iCodecInfo.iIsUpdate = ETrue;
    
    // Configure the decoder
    MccCodecConfigurator::ConfigureCodecL( *iFormatDecode,
                                            *iDatasink, 
                                            aCodecInfo,
                                            *iRtpMediaClock,
                                            CurrentCodecState() );
    
    // Configure also the real source
    MccCodecConfigurator::ConfigureCodecL( *iDatasource,
                                            *iDatasink, 
                                            iCodecInfo,
                                            *iRtpMediaClock,
                                            CurrentCodecState() );
    }
    

// -----------------------------------------------------------------------------
// CMccSymDlStream::CreatePayloadFormatDecoderL
// Creates the payload format decoder
// -----------------------------------------------------------------------------
//
void CMccSymSimpleDlStream::CreatePayloadFormatDecoderL()
    {
    __ASSERT_ALWAYS( iMultiplexer, User::Leave( KErrArgument ) );

    TUid formatuid;
    CMMFFormatDecode* tmp = NULL;
    
    formatuid.iUid = KImplUidAnyPayloadFormatDecode;
    
    // Multiplexer is the source of the decoder
    tmp = CMMFFormatDecode::NewL( formatuid, iMultiplexer );
    
	tmp->SetSourceDataTypeCode( iCodecInfo.iFourCC, Type() );

    if( iFormatDecode )
        {
        // Remove old one from multiplexer
        iMultiplexer->MultiplexerSourceThreadLogoff( iFormatDecode );
        
        iFormatDecode->SourceThreadLogoff();
        delete iFormatDecode;
        }
    
    iFormatDecode = tmp;
    User::LeaveIfError( iFormatDecode->SourceThreadLogon( *this ) );
    
    // Add decoder as sink of the multiplexer
    User::LeaveIfError( 
        iMultiplexer->MultiplexerSourceThreadLogon( *this, *iFormatDecode, ETrue, Type() ) );

    if ( KMccRtpSourceUid == iDatasource->DataSourceType() )
        {
        RArray<TUint> payloads;
        CleanupClosePushL( payloads );
        ParsePayloadTypesL( payloads, iCodecInfo );
        iMultiplexer->RegClientForPayloadsL( *iFormatDecode, payloads );
        CleanupStack::PopAndDestroy( &payloads );
        }
    }

// -----------------------------------------------------------------------------
// CMccSymDlStream::PrimeL
// Primes the audio stream ready
// -----------------------------------------------------------------------------
//
void CMccSymSimpleDlStream::PrimeL( const TUint32 aEndpointId )
    {
    __ASSERT_ALWAYS( iRtpmanager, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( iDatapath, User::Leave( KErrArgument ) );

    TBool controlNetworkResources = SetStateL( EStatePrepared, aEndpointId );
    
     if ( controlNetworkResources && !LocalStream() )
        {
        iRtpmanager->CreateReceiveStreamL( *iDatasource, iCodecInfo );
        }
    
    LoadCodecL( iCodecInfo, KNullDesC8 );
    
    DoPrimeL();
    
    InformStreamStateChange( KMccStreamPrepared, aEndpointId );
    }

// -----------------------------------------------------------------------------
// CMccSymSimpleDlStream::PlayL
// Starts audio streaming
// -----------------------------------------------------------------------------
//
void CMccSymSimpleDlStream::PlayL( 
    const TUint32 aEndpointId, 
    TBool /*aStreamPaused*/, 
    TBool aEnableRtcp )
    {
    __ASSERT_ALWAYS( iRtpmanager, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( iDatapath, User::Leave( KErrArgument ) );
    
    TBool controlNetworkResources = SetStateL( EStateStreaming, aEndpointId );
    
    if ( !LocalStream() )
        {  
        if ( controlNetworkResources )
            {
            iRtpmanager->StartSessionL();
            }
            
        User::LeaveIfError( iRtpmanager->SetRTCPSendReceive( aEnableRtcp ) );
        }
    
    UpdateCodecLoadingPermission( ETrue );
    
    LoadCodecL( iCodecInfo, KNullDesC8 );
    
    DoPlayL();
    
    InformStreamStateChange( KMccStreamStarted, aEndpointId );
    }

// -----------------------------------------------------------------------------
// CMccSymSimpleDlStream::PauseL
// Pauses audio streaming
// -----------------------------------------------------------------------------
//
void CMccSymSimpleDlStream::PauseL( const TUint32 aEndpointId, TBool aEnableRtcp )
    {
    __ASSERT_ALWAYS( iRtpmanager, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( iDatapath, User::Leave( KErrArgument ) );
    
    SetStateL( EStatePaused, aEndpointId );
    
     if ( !LocalStream() )
        { 
        User::LeaveIfError( iRtpmanager->SetRTCPSendReceive( aEnableRtcp ) );
        }

    DoPauseL();
    
    InformStreamStateChange( KMccStreamPaused, aEndpointId );
    }

// -----------------------------------------------------------------------------
// CMccSymSimpleDlStream::ResumeL
// Resumes paused audio streaming
// -----------------------------------------------------------------------------
//
void CMccSymSimpleDlStream::ResumeL( const TUint32 aEndpointId, TBool aEnableRtcp )
    {
    __ASSERT_ALWAYS( iRtpmanager, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( iDatapath, User::Leave( KErrArgument ) );
    
    SetStateL( EStateStreaming, aEndpointId );
    
    if ( !LocalStream() )
        { 
        User::LeaveIfError( iRtpmanager->SetRTCPSendReceive( aEnableRtcp ) );
        }

    DoResumeL();
    
    InformStreamStateChange( KMccStreamResumed, aEndpointId );
    }

// -----------------------------------------------------------------------------
// CMccSymSimpleDlStream::StopL
// Stops the audio streaming
// -----------------------------------------------------------------------------
//
void CMccSymSimpleDlStream::StopL( const TUint32 aEndpointId )
    {
    __ASSERT_ALWAYS( iDatapath, User::Leave( KErrArgument ) );    
    
    SetStateL( EStateStopped, aEndpointId );
    
    DoStopL();
    
    InformStreamStateChange( KMccStreamStopped, aEndpointId );   
    }
    
// -----------------------------------------------------------------------------
// CMccSymSimpleDlStream::SetBalanceL
// Sets the current play balance
// -----------------------------------------------------------------------------
//
void CMccSymSimpleDlStream::SetBalanceL( TInt aLeftBal, TInt aRightBal )
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
// CMccSymSimpleDlStream::GetBalanceL
// Returns the current play balance
// -----------------------------------------------------------------------------
//
void CMccSymSimpleDlStream::GetBalanceL( TInt& aLeftbal, TInt& aRightbal )
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
// CMccSymSimpleDlStream::GetSSRCL
// Returns SSRC value
// -----------------------------------------------------------------------------
//
void CMccSymSimpleDlStream::GetSSRCL( TUint32& /*aSSRCValue*/ )
    {
    User::Leave ( KErrNotSupported );
    }


// -----------------------------------------------------------------------------
// CMccSymSimpleDlStream::ResetCountersL
// Resets Jitterbuffer
// -----------------------------------------------------------------------------
//
void CMccSymSimpleDlStream::ResetCountersL( )
    {
    }

// -----------------------------------------------------------------------------
// CMccSymSimpleDlStream::SendMediaSignalL
// Downlink can send RTCP media signals (only receiver reports)
// -----------------------------------------------------------------------------
//
void CMccSymSimpleDlStream::SendMediaSignalL( const TMccEvent& aEvent )
    {
    __ASSERT_ALWAYS( !LocalStream(), User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( iDatasource, User::Leave( KErrNotReady ) );
    
    CMccRtpDataSource* p = static_cast<CMccRtpDataSource*>( iDatasource );
    p->SendMediaSignallingL( aEvent );
    }
        
// -----------------------------------------------------------------------------
// CMccSymSimpleDlStream::SendRTCPReceiverReportL
// Sends a RTCP receiver report
// -----------------------------------------------------------------------------
//  
void CMccSymSimpleDlStream::SendRTCPReceiverReportL()
    {
    __ASSERT_ALWAYS( !LocalStream(), User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( iDatasource, User::Leave( KErrArgument ) );

    CMccRtpDataSource* dSource = static_cast<CMccRtpDataSource*>( iDatasource );
    User::LeaveIfError( dSource->SendRTCPReceiverReport() );
    }

// -----------------------------------------------------------------------------
// CMccSymSimpleDlStream::StartInactivityTimerL
// Starts inactivity timer for a stream
// -----------------------------------------------------------------------------
// 
void CMccSymSimpleDlStream::StartInactivityTimerL( TUint32 aTimeoutTime )
    {
    __ASSERT_ALWAYS( !LocalStream(), User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( iDatasource, User::Leave( KErrArgument ) );
    
    CMccRtpDataSource* dSource = static_cast<CMccRtpDataSource*>( iDatasource );
    dSource->StartInactivityTimerL( aTimeoutTime );
    }

      
// -----------------------------------------------------------------------------
// CMccSymSimpleDlStream::StopInactivityTimerL
// Stops inactivity timer for a stream 
// -----------------------------------------------------------------------------
// 
void CMccSymSimpleDlStream::StopInactivityTimerL()
    {
    __ASSERT_ALWAYS( !LocalStream(), User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( iDatasource, User::Leave( KErrArgument ) );
    
    CMccRtpDataSource* dSource = static_cast<CMccRtpDataSource*>( iDatasource );
    dSource->StopInactivityTimerL( );
    }

// -----------------------------------------------------------------------------
// CMccSymSimpleDlStream::IsUplink
// -----------------------------------------------------------------------------
// 
TBool CMccSymSimpleDlStream::IsUplink()
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CMccSymSimpleDlStream::StandByL
// -----------------------------------------------------------------------------
// 
void CMccSymSimpleDlStream::StandByL()
    {
    __ASSERT_ALWAYS( !LocalStream(), User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( iDatasource, User::Leave( KErrArgument ) );
    
    CMccRtpDataSource* dSource = static_cast<CMccRtpDataSource*>( iDatasource );
    dSource->StandBy( EForceStandby, iCodecInfo.iPayloadType );
    }

// -----------------------------------------------------------------------------
// CMccSymSimpleDlStream::JitterBufferError
// -----------------------------------------------------------------------------
// 
void CMccSymSimpleDlStream::JitterBufferError( TInt /*aError*/ )
	{
    }

// -----------------------------------------------------------------------------
// CMccSymDlStream::DynamicBufferChangeRequest
// -----------------------------------------------------------------------------
//
void CMccSymSimpleDlStream::DynamicBufferChangeRequest( TInt /* aBufferSize*/ )
    {
    
    }
    
// -----------------------------------------------------------------------------
// CMccSymSimpleDlStream::MultiplexerSink
// -----------------------------------------------------------------------------
//
MDataSink* CMccSymSimpleDlStream::MultiplexerSink()
    {
    return iFormatDecode;
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File  
