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
* Description:    Mcc Symmetric subthread uplink stream
*
*/



// INCLUDE FILES
#include <mmf/server/mmfdatasource.h>
#include <mmf/server/mmfdatasink.h>
#include <mmf/server/mmfformat.h>
#include <mmf/server/mmfaudioinput.h>
#include "mccsymulstream.h"
#include "mcculdatapath.h"
#include "mcccodecconfigurator.h"
#include "mccrtpmanager.h"
#include "mccrtpdatasink.h"
#include "mmccinterface.h"
#include "mccsubcontrollerlogs.h"
#include "mccmultiplexer.h"
#include "mccresources.h"
#include "amrpayloadformatwrite.h" 
#include "mccinternaldef.h"

#include "mccredpayloadwrite.h"
#include "mccuids.hrh"

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccSymUlStream::CMccSymUlStream
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMccSymUlStream::CMccSymUlStream( 
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
                       aStreamType ), 
    iFormatEncode( NULL ), 
    iFourCC( aFourCC )
    {
    iRtpMediaClock = &aClock;
    }

// -----------------------------------------------------------------------------
// CMccSymUlStream::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMccSymUlStream* CMccSymUlStream::NewLC( 
    TUint32 aMccStreamId, 
    MAsyncEventHandler* aEventhandler,
    MMccResources* aMccResources,
    CMccRtpManager* aManager, 
    TFourCC aFourCC,
    TInt aStreamType,
    CMccRtpMediaClock& aClock ) 
    {
    CMccSymUlStream* s = 
            new ( ELeave ) CMccSymUlStream( aMccStreamId, 
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
// CMccSymUlStream::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMccSymUlStream::ConstructL()
    {
	__SUBCONTROLLER( "CMccSymUlStream::ConstructL" )
	__SUBCONTROLLER( "CMccSymUlStream::ConstructL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccSymUlStream::~CMccSymUlStream
// Destructor
// -----------------------------------------------------------------------------
//
CMccSymUlStream::~CMccSymUlStream()
    {
	__SUBCONTROLLER( "CMccSymUlStream::~CMccSymUlStream" )
    
    if ( iSoundDevice )
        {
        iSoundDevice = NULL;
        }
    
    MultiplexerCleanup();
    
    // Sink may need update since encoder logoff is routed to it
    HandleSpecialSinksAndSources();
    
    if ( iFormatEncode )
        {
        iFormatEncode->SinkThreadLogoff();
        delete iFormatEncode;
        }
    
    delete iRedFormatEncode;
    
	__SUBCONTROLLER( "CMccSymUlStream::~CMccSymUlStream, exit"	)

    }

// -----------------------------------------------------------------------------
// CMccSymUlStream::SetPrioritySettingsL
// Sets the MMF priority settings for this stream
// -----------------------------------------------------------------------------
//
void CMccSymUlStream::SetPrioritySettingsL( 
        const TMMFPrioritySettings& aPriority )
    {
    __SUBCONTROLLER( "CMccSymUlStream::SetPrioritySettingsL start" );
    
    __ASSERT_ALWAYS( iDatasource, User::Leave( KErrNotReady ) );  
    __ASSERT_ALWAYS( iDatasink, User::Leave( KErrNotReady ) );  

    if ( iDatasink->DataSinkType() == KUidMccVideoSink )
        {
        iDatasink->SetSinkPrioritySettings( aPriority );
        }
        
    iDatasource->SetSourcePrioritySettings( aPriority );
    
    __SUBCONTROLLER( "CMccSymUlStream::SetPrioritySettingsL end" );
    }

// -----------------------------------------------------------------------------
// CMccSymUlStream::AddSinkAndSourceL
// From CMccSymStreamBase
// -----------------------------------------------------------------------------
//
void CMccSymUlStream::AddSinkAndSourceL( MDataSink* aDatasink, 
        MDataSource* aDatasource )
    {
	__SUBCONTROLLER( "CMccSymUlStream::AddSinkAndSourceL" )
    __ASSERT_ALWAYS( aDatasink, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( aDatasource, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( !iDatapath, User::Leave( KErrAlreadyExists ) );
    
	__SUBCONTROLLER( "CMccSymUlStream::AddSinkAndSourceL, adding" )
    iDatasink = aDatasink;
    iDatasource = aDatasource;
    
    iMultiplexer = iMccResources->MultiplexerL( iDatasource );

    iDatapath = CMccUlDataPath::NewL( this, 
                                      iMccResources, 
                                      Type() );
                                      
    iDatapath->AddDataSourceL( iMultiplexer );   
    
    // Add datapath as the sink of the multiplexer, logon is routed to the
    // real datasource
    User::LeaveIfError( 
        iMultiplexer->MultiplexerSourceThreadLogon( *this, *iDatapath, EFalse, Type() ) );
    
    // Source logon must have been completed before fetching sounddevice
    if ( iDatasource->DataSourceType() == KUidMmfAudioInput )
    	{
		__SUBCONTROLLER( "CMccSymUlStream::AddSinkAndSourceL, KUidMmfAudioInput" )
		
        iSoundDevice = &( static_cast<MMMFAudioInput*>( iDatasource )->SoundDevice() );   	
    	}
    		
    // Inform datapath about ids of associated sink and source
    iDatapath->SetAssociatedSink( iDatasink );
    iDatapath->SetAssociatedSource( iDatasource );
    
	__SUBCONTROLLER( "CMccSymUlStream::AddSinkAndSourceL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccSymUlStream::LoadCodecL
// From CMccSymStreamBase
// -----------------------------------------------------------------------------
//
void CMccSymUlStream::LoadCodecL( const TMccCodecInfo& aCodecInfo,
                                  const TDesC8& aFmtp )
    {
	__SUBCONTROLLER_INT1( "CMccSymUlStream::LoadCodecL, codec state:", 
                          CurrentCodecState() )
    
    __ASSERT_ALWAYS( CurrentCodecState() == EStateCodecNone || 
                     iCodecInfo.iFourCC == aCodecInfo.iFourCC,
                     User::Leave( KErrArgument ) );
        
    iCodecInfo = aCodecInfo;
    
    if ( iDatasource && iDatasource->DataSourceType() == KUidMccFileSource )
        {
        iCodecInfo.iSourceDefinedTimeStamps = ETrue;
        }
    
    if ( CurrentCodecState() == EStateCodecNone )
        {
        delete iFmtpAttr;
        iFmtpAttr = NULL;

    	iFmtpAttr = HBufC8::NewL( aFmtp.Length() );
        iFmtpAttr->Des().Copy( aFmtp );
        
        iCodecInfo.iFmtpLength = iFmtpAttr->Length();
        
        CreatePayloadFormatEncoderL();
        
        // Just negotiate the codec, load it later
        static_cast<CMccUlDataPath*>( iDatapath )->NegotiateL( *iFormatEncode );
        
        SetCodecState( EStateCodecNegotiated );        	
        }
    else if ( CurrentCodecState() == EStateCodecNegotiated )
        {
        // Codec is not fully usable yet, do just formatter initialization
        MccCodecConfigurator::ConfigureCodecL( *iDatasource, 
                                                *iFormatEncode, 
                                                iCodecInfo,
                                                *iRtpMediaClock,
                                                CurrentCodecState(),
                                                iDatasink );  
                                                
        SetCodecState( EStateCodecPrepared );
        }
    else if ( CurrentCodecState() == EStateCodecPrepared )
        {
        if ( iDatasink->DataSinkType() == KMccRtpSinkUid )
       	    {
	        CMccRtpDataSink* dataSink = 
	                static_cast<CMccRtpDataSink*>( iDatasink );
	            
	        // For updating keep alive parameters
	        TMccCodecInfoBuffer infoBuffer( iCodecInfo ); 
	    	dataSink->ConfigureL( infoBuffer, iRtpMediaClock );
       	    }
        if ( CodecLoadingAllowed() )
            {
            // Load the codec
            static_cast<CMccUlDataPath*>( iDatapath )->LoadL( *iFormatEncode );
            
            // Set state before updating as the state affects to update functionality
            SetCodecState( EStateCodecLoaded );
                                               
            UpdateCodecInformationL( iCodecInfo );
            }
        }
    else if ( CurrentCodecState() == EStateCodecLoaded ||
              CurrentCodecState() == EStateCodecLoadedAndUpdating )
        {
        if ( iDatasink->DataSinkType() == KMccRtpSinkUid )
       	    {
            CMccRtpDataSink* dataSink = 
                static_cast<CMccRtpDataSink*>( iDatasink );
            
            // For updating keep alive parameters
            TMccCodecInfoBuffer infoBuffer( iCodecInfo );     
            dataSink->ConfigureL( infoBuffer, iRtpMediaClock );
       	    }
        // Update codec info
        SetCodecState( EStateCodecLoadedAndUpdating );
    	UpdateCodecInformationL( iCodecInfo );
        }
    else
        {
        User::Leave( KErrNotReady );
        }
        
    __SUBCONTROLLER_INT1( "CMccSymUlStream::LoadCodecL, exit, new state:", 
                          CurrentCodecState() )
    }

// -----------------------------------------------------------------------------
// CMccSymUlStream::UpdateCodecInformationL
// From CMccSymStreamBase
// -----------------------------------------------------------------------------
//
void CMccSymUlStream::UpdateCodecInformationL( TMccCodecInfo& /*aCodecInfo*/ )
    {
	__SUBCONTROLLER( "CMccSymUlStream::UpdateCodecInformationL" )
    User::LeaveIfNull( iDatasource );
    User::LeaveIfNull( iFormatEncode );
    
    HandleSpecialSinksAndSources();
                                            
    iCodecInfo.iIsUpdate = ETrue; 
    
    MccCodecConfigurator::ConfigureCodecL( *iDatasource, 
                                            *iFormatEncode, 
                                            iCodecInfo,
                                            *iRtpMediaClock,
                                            CurrentCodecState(),
                                            iDatasink );                                             
                                                  
	__SUBCONTROLLER( "CMccSymUlStream::UpdateCodecInformationL, exit" )
    }
    
// -----------------------------------------------------------------------------
// CMccSymUlStream::CreatePayloadFormatEncoderL
// Creates a payload format encoder for UL stream
// -----------------------------------------------------------------------------
//
void CMccSymUlStream::CreatePayloadFormatEncoderL()
    {
	__SUBCONTROLLER( "CMccSymUlStream::CreatePayloadFormatEncoderL" )
	__ASSERT_ALWAYS( NULL == iFormatEncode, User::Leave( KErrAlreadyExists ) );
	__ASSERT_ALWAYS( iDatasink, User::Leave( KErrNotReady ) );
    
    TUid formatuid;
    formatuid.iUid = iCodecInfo.iPayloadFormatEncoder;
    
    if ( EGenRedUsed == iCodecInfo.iAlgoUsed )
        {
        __ASSERT_ALWAYS( KMccPayloadTypeMax != iCodecInfo.iRedundantPayload,
            User::Leave( KErrArgument ) );
        
        iRedFormatEncode = CMMFFormatEncode::NewL( 
            TUid::Uid( KImplUidRedPayloadFormatEncode ), iDatasink );
        
        iFormatEncode = CMMFFormatEncode::NewL( formatuid, iRedFormatEncode );
        }
    else
        {
        iFormatEncode = CMMFFormatEncode::NewL( formatuid, iDatasink );
        }
    
    User::LeaveIfError( iFormatEncode->SetSinkDataTypeCode( 
        iCodecInfo.iFourCC, iCodecInfo.iType ) );
    User::LeaveIfError( iFormatEncode->SinkThreadLogon( *this ) );
	__SUBCONTROLLER( "CMccSymUlStream::CreatePayloadFormatEncoderL, exit" )
    }
    
// -----------------------------------------------------------------------------
// CMccSymUlStream::PrimeL
// Primes the audio stream ready
// -----------------------------------------------------------------------------
//
void CMccSymUlStream::PrimeL( const TUint32 aEndpointId )
    {
	__SUBCONTROLLER( "CMccSymUlStream::PrimeL" )
	
    TBool controlNetworkResources = SetStateL( EStatePrepared, aEndpointId );  
    
    if ( controlNetworkResources && !LocalStream() )
        {
        iRtpmanager->CreateTransmitStreamL( *iDatasink, iCodecInfo );
        }

    LoadCodecL( iCodecInfo, KNullDesC8 );
        
    DoPrimeL();
    
    InformStreamStateChange( KMccStreamPrepared, aEndpointId );

	__SUBCONTROLLER( "CMccSymUlStream::PrimeL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccSymUlStream::PlayL
// Starts audio streaming
// -----------------------------------------------------------------------------
//
void CMccSymUlStream::PlayL( TUint32 aEndpointId,
							 TBool /*aStreamPaused*/, 
                             TBool aEnableRtcp )
    {
	__SUBCONTROLLER( "CMccSymUlStream::PlayL" )
	
    TBool controlNetworkResources = SetStateL( EStateStreaming, aEndpointId );
    
    if ( !LocalStream() )
        {
        if ( controlNetworkResources )
            {
            iRtpmanager->StartSessionL();
            }
            
        User::LeaveIfError( iRtpmanager->SetRTCPSendReceive( aEnableRtcp ) );
        }
    
    UpdateCodecLoadingPermission( IsCodecOperationAllowed() );
    
    LoadCodecL( iCodecInfo, KNullDesC8 );
    
    DoPlayL();
    
    InformStreamStateChange( KMccStreamStarted, aEndpointId );
    
	__SUBCONTROLLER( "CMccSymUlStream::PlayL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccSymUlStream::PauseL
// Pauses audio streaming
// -----------------------------------------------------------------------------
//
void CMccSymUlStream::PauseL( const TUint32 aEndpointId, 
							  TBool aEnableRtcp )
    {
    __SUBCONTROLLER( "CMccSymUlStream::PauseL" )
    
    SetStateL( EStatePaused, aEndpointId );
    
    if ( !LocalStream() )
        { 
        User::LeaveIfError( iRtpmanager->SetRTCPSendReceive( aEnableRtcp ) );
        }
    
    DoPauseL();

    InformStreamStateChange( KMccStreamPaused, aEndpointId );
    }

// -----------------------------------------------------------------------------
// CMccSymUlStream::ResumeL
// Resumes paused audio streaming
// -----------------------------------------------------------------------------
//
void CMccSymUlStream::ResumeL( const TUint32 aEndpointId,
							   TBool aEnableRtcp )
    {
    __SUBCONTROLLER( "CMccSymUlStream::ResumeL" )
    
    SetStateL( EStateStreaming, aEndpointId );
    
    if ( !LocalStream() )
        { 
        User::LeaveIfError( iRtpmanager->SetRTCPSendReceive( aEnableRtcp ) );
        }
    
    DoResumeL();

    InformStreamStateChange( KMccStreamResumed, aEndpointId );
    }

// -----------------------------------------------------------------------------
// CMccSymUlStream::StopL
// Stops the audio streaming
// -----------------------------------------------------------------------------
//
void CMccSymUlStream::StopL( const TUint32 aEndpointId )
    {
    __SUBCONTROLLER( "CMccSymUlStream::StopL" )
    
    SetStateL( EStateStopped, aEndpointId );
    
    DoStopL();
    
    InformStreamStateChange( KMccStreamStopped, aEndpointId );
	}

// -----------------------------------------------------------------------------
// CMccSymUlStream::SetBalanceL
// Sets the current record balance
// -----------------------------------------------------------------------------
//
void CMccSymUlStream::SetBalanceL( TInt aLeftBal, TInt aRightBal )
    {
    if( !iSoundDevice )
        {
        User::Leave( KErrNotReady );
        }
    else
        {
        iSoundDevice->SetRecordBalanceL( aLeftBal, aRightBal );
        }
    }

// -----------------------------------------------------------------------------
// CMccSymUlStream::GetBalanceL
// Returns the current record balance
// -----------------------------------------------------------------------------
//
void CMccSymUlStream::GetBalanceL( TInt& aLeftbal, TInt& aRightbal )
    {
    if( !iSoundDevice )
        {
        User::Leave( KErrNotReady );
        }
    else
        {
        iSoundDevice->GetRecordBalanceL( aLeftbal, aRightbal );
        }
    }
    
// -----------------------------------------------------------------------------
// CMccSymUlStream::GetSSRCL
// Returns SSRC value
// -----------------------------------------------------------------------------
//
void CMccSymUlStream::GetSSRCL( TUint32& aSSRCValue )
    {
    __ASSERT_ALWAYS( !LocalStream(), User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( iDatasink, User::Leave( KErrNotReady ) );

    CMccRtpDataSink* sink = static_cast<CMccRtpDataSink*>( iDatasink );
    aSSRCValue = sink->GetSSRC();
    }

// -----------------------------------------------------------------------------
// CMccSymUlStream::ResetCountersL
// 
// -----------------------------------------------------------------------------
//
void CMccSymUlStream::ResetCountersL( )
    {
    }

// -----------------------------------------------------------------------------
// CMccSymUlStream::SendMediaSignalL
// Uplink can send RTCP media signals
// -----------------------------------------------------------------------------
//
void CMccSymUlStream::SendMediaSignalL( const TMccEvent& aEvent )
    {
    __ASSERT_ALWAYS( !LocalStream(), User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( iDatasink, User::Leave( KErrNotReady ) );
    
    CMccRtpDataSink* p = static_cast<CMccRtpDataSink*>( iDatasink );
    p->SendMediaSignallingL( aEvent );
    }
        
// -----------------------------------------------------------------------------
// CMccSymUlStream::DoChangeBitrateL
// Uplink can change bitrate during AMR streaming
// -----------------------------------------------------------------------------
//
void CMccSymUlStream::DoChangeBitrateL( TInt aBitrate )
    {    
    // This function is trapped inside ChangeBitrate() function
    __SUBCONTROLLER_INT1( "CMccSymUlStream::DoChangeBitrateL CURRENT:", iCodecInfo.iBitrate );
    __SUBCONTROLLER_INT1( "CMccSymUlStream::DoChangeBitrateL NEW:", aBitrate );
    
    // If using AMR codec and bitrate has changed for what it was set
    // then change it.
    const TUint32 fourCC = iCodecInfo.iFourCC.FourCC();
    
    if ( ( KMccFourCCIdAMRNB == fourCC || KMccFourCCIdAMRWB == fourCC ) &&
           iSoundDevice && iFormatEncode )
        {
        // Change the bitrate only if it needs to be changed
        if( iCodecInfo.iBitrate != aBitrate )
            {
            CAmrPayloadFormatWrite* write =
                static_cast<CAmrPayloadFormatWrite*>( iFormatEncode );
                
            if ( write->IsBitrateChangeValid( aBitrate ) )
                {
                MccCodecConfigurator::ChangeBitrateL( 
                    iCodecInfo, aBitrate, *iSoundDevice );
                iCodecInfo.iBitrate = aBitrate;    
                }
            }
        }
    else
        {
        // Other codec than AMR is in use and we cannot change the bitrate
        __SUBCONTROLLER( "CMccSymUlStream::DoChangeBitrateL KErrNotSupported" );
        User::Leave( KErrNotSupported );
        }
    }
    
// -----------------------------------------------------------------------------    
// CMccSymUlStream::SendRTCPSenderReportL
// Sends a RTCP sender report
// -----------------------------------------------------------------------------
// 
void CMccSymUlStream::SendRTCPSenderReportL()
    {
    __ASSERT_ALWAYS( !LocalStream(), User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( iDatasink, User::Leave( KErrNotReady ) );
    
    CMccRtpDataSink* dSink = static_cast<CMccRtpDataSink*>( iDatasink );
    User::LeaveIfError( dSink->SendRTCPSenderReport() );
    }

// -----------------------------------------------------------------------------
// CMccSymUlStream::SendRTCPDataL
// Sends Non-RTCP data
// -----------------------------------------------------------------------------
// 
void CMccSymUlStream::SendRTCPDataL( const TDesC8& aData )
    {
    __ASSERT_ALWAYS( !LocalStream(), User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( iDatasink, User::Leave( KErrNotReady ) );
    
    CMccRtpDataSink* dSink = static_cast<CMccRtpDataSink*>( iDatasink );
    dSink->SendRTCPDataL( aData );
    }

// -----------------------------------------------------------------------------
// CMccSymUlStream::IsUplink
// -----------------------------------------------------------------------------
// 
TBool CMccSymUlStream::IsUplink()
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CMccSymUlStream::StandByL
// -----------------------------------------------------------------------------
// 
void CMccSymUlStream::StandByL()
    {
    }

// -----------------------------------------------------------------------------
// CMccSymUlStream::MultiplexerSink
// -----------------------------------------------------------------------------
//
MDataSink* CMccSymUlStream::MultiplexerSink()
    {
    return iDatapath;
    }

// -----------------------------------------------------------------------------
// CMccSymDlStream::BindContextIntoStreamL
// -----------------------------------------------------------------------------
//
void CMccSymUlStream::BindContextIntoStreamL( TUint32 aEndpointId,
                                              const TMccCryptoContext& aContextParams )
    {
     __ASSERT_ALWAYS( iDatasink, User::Leave( KErrNotReady ) );
     
	if ( iDatasink->DataSinkType() == KMccRtpSinkUid &&
	     aEndpointId == MCC_ENDPOINT_ID( iDatasink ) )
	    {
        CMccRtpDataSink* dataSink = static_cast<CMccRtpDataSink*>( iDatasink );
        dataSink->ApplyCryptoContextL( aContextParams );
	    }
    else
	    {
	    User::Leave( KErrNotFound );
	    }
    }
        
// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File  
