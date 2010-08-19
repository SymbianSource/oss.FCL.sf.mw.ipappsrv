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
* Description:    Symmetric subthread client
*
*/



// INCLUDE FILES
#include <mmf/server/mmfdatasource.h>
#include <mmf/server/mmfdatasink.h>

#include "mccsymsubthreadclient.h"
#include "mccsymulstream.h"
#include "mccsymdlstream.h"
#include "mccsymsimpledlstream.h"
#include "mccdtmfdlstream.h"
#include "mccdtmfulstream.h"
#include "mccrtpmanager.h"
#include "amrpayloadformatutil.h"
#include "mccinternalevents.h"
#include "mccsubcontrollerlogs.h"
#include "mmccsecureinterface.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccSymSubthreadClient::CMccSymSubthreadClient
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
CMccSymSubthreadClient::CMccSymSubthreadClient(
    MMccEventHandler* aObserver,
    MMccResources* aMccResources,
    TInt aLinkType,
    TUint32 aMccSessionId ):
    CMccSubThreadClientBase( aObserver, aMccResources, aLinkType, aMccSessionId )        
    {
    }

// -----------------------------------------------------------------------------
// CMccSymSubthreadClient::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
void CMccSymSubthreadClient::ConstructL()
    {
	__SUBCONTROLLER( "CMccSymSubthreadClient::ConstructL" )
	User::LeaveIfNull( iObserver );
	User::LeaveIfNull( iMccResources );
    iRtpmanager = CMccRtpManager::NewL( *this, *iMccResources, MccSessionId() );
	__SUBCONTROLLER( "CMccSymSubthreadClient::ConstructL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccSymSubthreadClient::NewL
// Static constructor.
// -----------------------------------------------------------------------------
CMccSymSubthreadClient* CMccSymSubthreadClient::NewL( 
    MMccEventHandler* aObserver, 
    MMccResources* aMccResources,
    TInt aLinkType,
    TUint32 aMccSessionId )
    {        
    CMccSymSubthreadClient* self = 
            new ( ELeave ) CMccSymSubthreadClient( aObserver, aMccResources, aLinkType, aMccSessionId );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMccSymSubthreadClient::~CMccSymSubthreadClient
// Destructor
// -----------------------------------------------------------------------------
CMccSymSubthreadClient::~CMccSymSubthreadClient()
    {
	__SUBCONTROLLER( "CMccSymSubthreadClient::~CMccSymSubthreadClient" )
		        
    iStreams.ResetAndDestroy();
    iStreams.Close();
    
    iUnusedStreams.Reset();
    iUnusedStreams.Close();
    
    if( iRtpmanager )
        {
        iRtpmanager->CloseSession();
        delete iRtpmanager;
        }    
	__SUBCONTROLLER( "CMccSymSubthreadClient::~CMccSymSubthreadClient, exit" )
    }

// -----------------------------------------------------------------------------
// CMccSymSubthreadClient::SetVolumeL
// Creates a new RTP session in a subthread
// -----------------------------------------------------------------------------
void CMccSymSubthreadClient::SetVolumeL( TInt aVolume )
    {
	__SUBCONTROLLER_INT1( "CMccSymSubthreadClient::SetVolumeL", aVolume )
    TInt strmCount = iStreams.Count();
    if( strmCount )
        {
        for( TInt k = 0; k < strmCount; k++ )
            {
            iStreams[k]->SetVolumeL( aVolume );
            }
        }
    else
        {        
	    __SUBCONTROLLER( "CMccSymSubthreadClient::SetVolumeL, no streams" )
        }
	__SUBCONTROLLER( "CMccSymSubthreadClient::SetVolumeL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccSymSubthreadClient::SetGainL
// Creates a new RTP session in a subthread
// -----------------------------------------------------------------------------
void CMccSymSubthreadClient::SetGainL( TInt aGain )
    {
	__SUBCONTROLLER_INT1( "CMccSymSubthreadClient::SetGainL", aGain )
    TInt strmCount = iStreams.Count();
    if( strmCount )
        {
        for( TInt k = 0; k < strmCount; k++ )
            {
            iStreams[k]->SetGainL( aGain );
            }
        }
    else
        {
	    __SUBCONTROLLER( "CMccSymSubthreadClient::SetGainL, no streams" )
        }
	__SUBCONTROLLER( "CMccSymSubthreadClient::SetGainL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccSymSubthreadClient::VolumeL
// Creates a new RTP session in a subthread
// -----------------------------------------------------------------------------
TInt CMccSymSubthreadClient::GetVolumeL( const TUint32 aStreamId )
    {
	__SUBCONTROLLER_INT1( "CMccSymSubthreadClient::GetVolumeL for streamId", aStreamId )
	__ASSERT_ALWAYS( iStreams.Count(), User::Leave( KErrNotFound ) );
    TInt volume = 0;
    TInt index = FindStreamL( aStreamId );
    volume = iStreams[index]->VolumeL();
	__SUBCONTROLLER_INT1( "CMccSymSubthreadClient::GetVolumeL exit with", volume )
    return volume;
    }

// -----------------------------------------------------------------------------
// CMccSymSubthreadClient::GainL
// Creates a new RTP session in a subthread
// -----------------------------------------------------------------------------
TInt CMccSymSubthreadClient::GetGainL( const TUint32 aStreamId )
    {
	__SUBCONTROLLER_INT1( "CMccSymSubthreadClient::GetGainL for streamId", aStreamId )
	__ASSERT_ALWAYS( iStreams.Count(), User::Leave( KErrNotFound ) );
    TInt gain = 0;
    TInt index = FindStreamL( aStreamId );
    gain = iStreams[index]->GainL();
	__SUBCONTROLLER_INT1( "CMccSymSubthreadClient::GetGainL exit with", gain )
    return gain;
    }

// -----------------------------------------------------------------------------
// CMccSymSubthreadClient::MaxVolumeL
// Creates a new RTP session in a subthread
// -----------------------------------------------------------------------------
TInt CMccSymSubthreadClient::MaxVolumeL( const TUint32 aStreamId )
    {
	__SUBCONTROLLER_INT1( "CMccSymSubthreadClient::MaxVolumeL for streamId", aStreamId )
	__ASSERT_ALWAYS( iStreams.Count(), User::Leave( KErrNotFound ) );
    TInt maxvolume = 0;
    TInt index = FindStreamL( aStreamId );
    maxvolume = iStreams[index]->MaxVolumeL();
	__SUBCONTROLLER_INT1( "CMccSymSubthreadClient::MaxVolumeL exit with", maxvolume )
    return maxvolume;
    }

// -----------------------------------------------------------------------------
// CMccSymSubthreadClient::MaxGainL
// Creates a new RTP session in a subthread
// -----------------------------------------------------------------------------
TInt CMccSymSubthreadClient::MaxGainL( const TUint32 aStreamId )
    {
	__SUBCONTROLLER_INT1( "CMccSymSubthreadClient::MaxGainL for streamId", aStreamId )
	__ASSERT_ALWAYS( iStreams.Count(), User::Leave( KErrNotFound ) );
    TInt maxgain = 0;
    TInt index = FindStreamL( aStreamId );
    maxgain = iStreams[index]->MaxGainL();
	__SUBCONTROLLER_INT1( "CMccSymSubthreadClient::MaxGainL exit with", maxgain )
    return maxgain;
    }

// -----------------------------------------------------------------------------
// CMccSymSubthreadClient::SetBalanceL
// From CMccSubThreadClientBase
// -----------------------------------------------------------------------------
void CMccSymSubthreadClient::SetBalanceL( const TUint32 aStreamId, TInt aLeftBalance, 
        TInt aRightBalance )
    {
	__SUBCONTROLLER_INT1( "CMccSymSubthreadClient::SetBalanceL for streamId", aStreamId )
	__SUBCONTROLLER_INT1( "CMccSymSubthreadClient::SetBalanceL for LeftBalance", aLeftBalance )
	__SUBCONTROLLER_INT1( "CMccSymSubthreadClient::SetBalanceL for RightBalance", aRightBalance )
	__ASSERT_ALWAYS( iStreams.Count(), User::Leave( KErrNotFound ) );
    TInt index = FindStreamL( aStreamId );
    iStreams[index]->SetBalanceL( aLeftBalance, aRightBalance );
	__SUBCONTROLLER( "CMccSymSubthreadClient::SetBalanceL exit" )
    }
    
// -----------------------------------------------------------------------------
// CMccSymSubthreadClient::GetBalanceL
// From CMccSubThreadClientBase
// -----------------------------------------------------------------------------
void CMccSymSubthreadClient::GetBalanceL( const TUint32 aStreamId, TInt& aLeftBalance, 
        TInt& aRightBalance )
    {
	__SUBCONTROLLER_INT1( "CMccSymSubthreadClient::GetBalanceL for streamId", aStreamId )
	__ASSERT_ALWAYS( iStreams.Count(), User::Leave( KErrNotFound ) );
    TInt index = FindStreamL( aStreamId );
    iStreams[index]->GetBalanceL( aLeftBalance, aRightBalance );
	__SUBCONTROLLER_INT1( "CMccSymSubthreadClient::GetBalanceL for LeftBalance", aLeftBalance )
	__SUBCONTROLLER_INT1( "CMccSymSubthreadClient::GetBalanceL for RightBalance", aRightBalance )
	__SUBCONTROLLER( "CMccSymSubthreadClient::GetBalanceL exit" )
    }
    
// -----------------------------------------------------------------------------
// CMccSymSubthreadClient::OpenL
// From CMccSubThreadClientBase
// -----------------------------------------------------------------------------
void CMccSymSubthreadClient::OpenL( TInt aStreamType,
                                    TFourCC aFourCC,
                                    MDataSource* aDatasource, 
                                    MDataSink* aDatasink,
                                    const TUint32 aStreamId,
                                    TMMFPrioritySettings aSettings )
    {
	__SUBCONTROLLER_INT1( "CMccSymSubthreadClient::OpenL for streamId", aStreamId )
	__SUBCONTROLLER_INT1( "CMccSymSubthreadClient stream type", aStreamType )
    __ASSERT_ALWAYS( iSessionCreated, User::Leave( KErrNotReady ) );
    __ASSERT_ALWAYS( aDatasource, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( aDatasink, User::Leave( KErrArgument ) );
        
    TUid sourceType = aDatasource->DataSourceType();
    TUid sinkType = aDatasink->DataSinkType();

	iPrioritySettingsData = aSettings;
	__SUBCONTROLLER_INT1( "CMccSymSubthreadClient audio priority", iPrioritySettingsData.iPriority )
	__SUBCONTROLLER_INT1( "CMccSymSubthreadClient audio preference", iPrioritySettingsData.iPref )

    CMccSymStreamBase* stream( NULL );
    
    TBool localAVToDisplayStream = ( aStreamType == KMccVideoLocalStream ||
                                     aStreamType == KMccAudioLocalStream ) &&
                                     sourceType == KMccFileSourceUid &&
                                     sinkType == KMccVideoSinkUid;
    
    if ( KMccDtmfStream == aStreamType && KMccRtpSourceUid == sourceType )
        {
        __SUBCONTROLLER( "CMccSymSubthreadClient::OpenL, new CMccDtmfDlStream" )
        
        stream = CMccDtmfDlStream::NewLC( aStreamId, this, iMccResources,
            iRtpmanager, aFourCC, aStreamType, *iRtpMediaClock );
        }
    else if ( KMccDtmfStream == aStreamType && KMccRtpSinkUid == sinkType )
        {
        __SUBCONTROLLER( "CMccSymSubthreadClient::OpenL, new CMccDtmfUlStream" )
        
        stream = CMccDtmfUlStream::NewLC( aStreamId, this, iMccResources,
            iRtpmanager, aFourCC, aStreamType, *iRtpMediaClock );
        }
    // Special handling for local video if it should be displayed (treat as uplink)
    else if( sinkType == KMccRtpSinkUid ||
        localAVToDisplayStream )
        {
	    __SUBCONTROLLER( "CMccSymSubthreadClient::OpenL, new CMccSymUlStream" )
        stream = CMccSymUlStream::NewLC( aStreamId, 
                                         this, 
                                         iMccResources, 
                                         iRtpmanager, 
                                         aFourCC, 
                                         aStreamType,
                                         *iRtpMediaClock );
        }
    else if( sourceType == KMccRtpSourceUid && sinkType != KMccVideoSinkUid )
        {
	    __SUBCONTROLLER( "CMccSymSubthreadClient::OpenL, new CMccSymDlStream" )
        stream = CMccSymDlStream::NewLC( aStreamId, 
                                         this, 
                                         iMccResources,
                                         iRtpmanager, 
                                         aStreamType,
                                         *iRtpMediaClock );
        }
    else if( ( sourceType == KMccRtpSourceUid && sinkType == KMccVideoSinkUid ) ||
             ( aStreamType == KMccAudioLocalStream || aStreamType == KMccVideoLocalStream ) )
        {  
        __SUBCONTROLLER( "CMccSymSubthreadClient::OpenL, new CMccSymSimpleDlStream" )      
        stream = CMccSymSimpleDlStream::NewLC( aStreamId, 
                                               this, 
                                               iMccResources,
                                               iRtpmanager, 
                                               aFourCC, 
                                               aStreamType,
                                               *iRtpMediaClock );
                
        }
    else
        {
	    __SUBCONTROLLER( "CMccSymSubthreadClient::OpenL, KErrArgument" )
        User::Leave( KErrArgument );
        }
    
    stream->AddSinkAndSourceL( aDatasink, aDatasource );
    iStreams.AppendL( stream );
    CleanupStack::Pop( stream );
        
    TInt index = FindStreamL( aStreamId );     
    iStreams[ index ]->SetMMFPriority( aSettings );
   
	__SUBCONTROLLER( "CMccSymSubthreadClient::OpenL, exit" )
    }


// -----------------------------------------------------------------------------
// CMccSymSubthreadClient::PrepareL
// From CMccSubThreadClientBase
// -----------------------------------------------------------------------------
void CMccSymSubthreadClient::PrepareL( const TUint32 aStreamId, 
									   const TUint32 aEndpointId )
    {
	__SUBCONTROLLER_INT1( "CMccSymSubthreadClient::PrepareL, streamId", aStreamId )
	__SUBCONTROLLER_INT1( "CMccSymSubthreadClient::PrepareL, aEndpointId", aEndpointId )
	__ASSERT_ALWAYS( iStreams.Count(), User::Leave( KErrNotFound ) );
    TInt index = FindStreamL( aStreamId );
    iStreams[index]->PrimeL( aEndpointId );	
	__SUBCONTROLLER( "CMccSymSubthreadClient::PrepareL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccSymSubthreadClient::PlayL
// From CMccSubThreadClientBase
// -----------------------------------------------------------------------------
void CMccSymSubthreadClient::PlayL( const TUint32 aStreamId,
                                    const TUint32 aEndpointId,
                                    TBool aStreamPaused,
                                    TBool aEnableRTCP )
    {
	__SUBCONTROLLER_INT1( "CMccSymSubthreadClient::PlayL, streamId", aStreamId )
	__SUBCONTROLLER_INT1( "CMccSymSubthreadClient::PlayL, aEndpointId", aEndpointId )
	__ASSERT_ALWAYS( iStreams.Count(), User::Leave( KErrNotFound ) );
    TInt index = FindStreamL( aStreamId );
    iStreams[index]->PlayL( aEndpointId, aStreamPaused, aEnableRTCP ); 
	__SUBCONTROLLER( "CMccSymSubthreadClient::PlayL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccSymSubthreadClient::StopL
// From CMccSubThreadClientBase
// -----------------------------------------------------------------------------
void CMccSymSubthreadClient::StopL( const TUint32 aStreamId,
									const TUint32 aEndpointId )
    {
	__SUBCONTROLLER_INT1( "CMccSymSubthreadClient::StopL, streamId", aStreamId )
	__SUBCONTROLLER_INT1( "CMccSymSubthreadClient::StopL, aEndpointId", aEndpointId )
    TInt index = FindStream( aStreamId );
    if ( index != KErrNotFound )
        {
        iStreams[index]->StopL( aEndpointId );
        }
    else
        {
        // No need to stop if unused
        index = iUnusedStreams.Find( aStreamId );
        __ASSERT_ALWAYS( index != KErrNotFound, User::Leave( KErrNotFound ) );
        }
	__SUBCONTROLLER( "CMccSymSubthreadClient::StopL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccSymSubthreadClient::PauseL
// From CMccSubThreadClientBase
// -----------------------------------------------------------------------------
void CMccSymSubthreadClient::PauseL( const TUint32 aStreamId,
									 const TUint32 aEndpointId,
                                     TBool aEnableRTCP )
    {
	__SUBCONTROLLER_INT1( "CMccSymSubthreadClient::PauseL, streamId", aStreamId )
	__SUBCONTROLLER_INT1( "CMccSymSubthreadClient::PauseL, aEndpointId", aEndpointId )
	__ASSERT_ALWAYS( iStreams.Count(), User::Leave( KErrNotFound ) );
    TInt index = FindStreamL( aStreamId );
    iStreams[index]->PauseL( aEndpointId, aEnableRTCP );
	__SUBCONTROLLER( "CMccSymSubthreadClient::PauseL, exit" )
    }
    
// -----------------------------------------------------------------------------
// CMccSymSubthreadClient::ResumeL
// From CMccSubThreadClientBase
// -----------------------------------------------------------------------------
void CMccSymSubthreadClient::ResumeL( const TUint32 aStreamId,
									  const TUint32 aEndpointId,
                                      TBool aEnableRTCP )
    {
	__SUBCONTROLLER_INT1( "CMccSymSubthreadClient::ResumeL, streamId", aStreamId )
	__SUBCONTROLLER_INT1( "CMccSymSubthreadClient::ResumeL, aEndpointId", aEndpointId )
	__ASSERT_ALWAYS( iStreams.Count(), User::Leave( KErrNotFound ) );
    TInt index = FindStreamL( aStreamId );
    iStreams[index]->ResumeL( aEndpointId, aEnableRTCP );
    __SUBCONTROLLER( "CMccSymSubthreadClient::ResumeL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccSymSubthreadClient::CloseL
// From CMccSubThreadClientBase
// -----------------------------------------------------------------------------
void CMccSymSubthreadClient::CloseL( const TUint32 aStreamId )
    {
	__SUBCONTROLLER_INT1( "CMccSymSubthreadClient::CloseL, streamId", aStreamId )
    
    TInt index = FindStream( aStreamId );
    if ( index != KErrNotFound )
        {
        CMccSymStreamBase* tmp = iStreams[index];
        iStreams.Remove( index );
        delete tmp;
        }
    else 
        {
        index = iUnusedStreams.Find( aStreamId );
        __ASSERT_ALWAYS( index != KErrNotFound, User::Leave( KErrNotFound ) );

        // Stream was already deleted
        iUnusedStreams.Remove( index );
        }
    
    // No need to fail stream deletion if manager update fails
    TRAP_IGNORE( iRtpmanager->UpdateL() )
        
	__SUBCONTROLLER( "CMccSymSubthreadClient::CloseL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccSymSubthreadClient::CloseL 
// Closes all links
// -----------------------------------------------------------------------------
void CMccSymSubthreadClient::CloseL()
    {
	__SUBCONTROLLER( "CMccSymSubthreadClient::CloseL" )
	
    for ( TInt i = iStreams.Count() - 1; i >= 0; i-- )
    	{
        CMccSymStreamBase* tmp = iStreams[i];
        tmp->RemoveContext( 0 );
        iStreams.Remove( i );
        delete tmp;        	    	
    	}
	__SUBCONTROLLER( "CMccSymSubthreadClient::CloseL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccSymSubthreadClient::StreamsExistsL 
// -----------------------------------------------------------------------------
TBool CMccSymSubthreadClient::StreamsExistsL()
    {
    if (iStreams.Count())
        {
        return ETrue;
        }
    else
        {
        return  EFalse;
        }
    }

// -----------------------------------------------------------------------------
// CMccSymSubthreadClient::SendMediaSignalL
// Send media signalling to uplink
// -----------------------------------------------------------------------------
//  
void CMccSymSubthreadClient::SendMediaSignalL( const TMccEvent& aEvent )
    {
	__SUBCONTROLLER_INT1( "CMccSymSubthreadClient::SendMediaSignalL, streamId", aEvent.iStreamId )
	__ASSERT_ALWAYS( iStreams.Count(), User::Leave( KErrNotFound ) );
    TInt index = FindStreamL( aEvent.iStreamId );
    iStreams[index]->SendMediaSignalL( aEvent );
	__SUBCONTROLLER( "CMccSymSubthreadClient::SendMediaSignalL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccUlSubThreadClient::SendRTCPReceiverReportL
// Sends a RTCP receiver report
// -----------------------------------------------------------------------------
//  
void CMccSymSubthreadClient::SendRTCPReceiverReportL( const TUint32 aStreamId )
    {
	__SUBCONTROLLER_INT1( "CMccSymSubthreadClient::SendRTCPReceiverReportL, streamId", aStreamId )
	__ASSERT_ALWAYS( iStreams.Count(), User::Leave( KErrNotFound ) );
    TInt index = FindStreamL( aStreamId );
    iStreams[index]->SendRTCPReceiverReportL();
	__SUBCONTROLLER( "CMccSymSubthreadClient::SendRTCPReceiverReportL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccUlSubThreadClient::SendRTCPSenderReportL
// Sends a RTCP sender report
// -----------------------------------------------------------------------------
// 
void CMccSymSubthreadClient::SendRTCPSenderReportL( const TUint32 aStreamId )
    {
	__SUBCONTROLLER_INT1( "CMccSymSubthreadClient::SendRTCPSenderReportL, streamId", aStreamId )
	__ASSERT_ALWAYS( iStreams.Count(), User::Leave( KErrNotFound ) );
    TInt index = FindStreamL( aStreamId );
    iStreams[index]->SendRTCPSenderReportL();
	__SUBCONTROLLER( "CMccSymSubthreadClient::SendRTCPSenderReportL, exit" )
    }
    
// -----------------------------------------------------------------------------
// CMccSymSubthreadClient::SendRTCPDataL
// Sends Non-RTCP data
// -----------------------------------------------------------------------------
// 
void CMccSymSubthreadClient::SendRTCPDataL( const TUint32 aStreamId,
                                            const TDesC8& aData )
    {
	__SUBCONTROLLER_INT1( "CMccSymSubthreadClient::SendRTCPDataL, streamId", aStreamId )
	__ASSERT_ALWAYS( iStreams.Count(), User::Leave( KErrNotFound ) );
    TInt index = FindStreamL( aStreamId );
    iStreams[index]->SendRTCPDataL( aData );
	__SUBCONTROLLER( "CMccSymSubthreadClient::SendRTCPDataL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccUlSubThreadClient::GetSupportedBitratesL
// Gets the supported bitrates from the codec of the specified stream
// -----------------------------------------------------------------------------    
void CMccSymSubthreadClient::GetSupportedBitratesL( const TUint32 /*aStreamId*/,
                                                   RArray<TUint>& /*aBitrates*/ )
    {
    //TBI: What with the stream ID?
    //TODO //User::LeaveIfError( iSubthread.GetSupportedBitrates( aBitrates ) );
    }
    
// -----------------------------------------------------------------------------
// CMccUlSubThreadClient::GetSSRCL
// Gets the syncronization source for the specified stream
// -----------------------------------------------------------------------------    
void CMccSymSubthreadClient::GetSSRCL( const TUint32 aStreamId, TUint32& aSSRCValue )
    {
	__SUBCONTROLLER_INT1( "CMccSymSubthreadClient::GetSSRCL, streamId", aStreamId )
	__ASSERT_ALWAYS( iStreams.Count(), User::Leave( KErrNotFound ) );
    TInt index = FindStreamL( aStreamId );
    iStreams[index]->GetSSRCL( aSSRCValue );
	__SUBCONTROLLER_INT1( "CMccSymSubthreadClient::GetSSRCL, SSRC", aSSRCValue )
	__SUBCONTROLLER( "CMccSymSubthreadClient::GetSSRCL, exit" )
    }
    
// -----------------------------------------------------------------------------
// CMccDlSubThreadClient::StartInactivityTimerL
// Starts inactivity timer for a stream 
// -----------------------------------------------------------------------------    
void CMccSymSubthreadClient::StartInactivityTimerL( const TUint32 aStreamId,
                                                   TUint32 aTimeoutTime )
    {
	__SUBCONTROLLER_INT1( "CMccSymSubthreadClient::StartInactivityTimerL, streamId", aStreamId )
	__ASSERT_ALWAYS( iStreams.Count(), User::Leave( KErrNotFound ) );
    TInt index = FindStreamL( aStreamId );
    iStreams[index]->StartInactivityTimerL( aTimeoutTime );
	__SUBCONTROLLER( "CMccSymSubthreadClient::StartInactivityTimerL, exit" )
    }
       
// -----------------------------------------------------------------------------
// CMccDlSubThreadClient::StopInactivityTimerL
// Stops inactivity timer for a stream 
// -----------------------------------------------------------------------------    
void CMccSymSubthreadClient::StopInactivityTimerL( const TUint32 aStreamId )
    {
	__SUBCONTROLLER_INT1( "CMccSymSubthreadClient::StopInactivityTimerL, streamId", aStreamId )
	__ASSERT_ALWAYS( iStreams.Count(), User::Leave( KErrNotFound ) );
    TInt index = FindStreamL( aStreamId );
    iStreams[index]->StopInactivityTimerL();
	__SUBCONTROLLER( "CMccSymSubthreadClient::StopInactivityTimerL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccSymSubthreadClient::GetFmtpAttrL
// Fetches the FMTP attribute of the current codec
// -----------------------------------------------------------------------------
// 
void CMccSymSubthreadClient::GetFmtpAttrL( const TUint32 /*aStreamId*/,
                                           TDes8& /*aFmtp*/ )
    {
    
    } 
    
// -----------------------------------------------------------------------------
// CMccSymSubthreadClient::SetRemoteAddressL
// Sets the remote address of uplink stream
// -----------------------------------------------------------------------------
void CMccSymSubthreadClient::SetRemoteAddressL( TInetAddr aRemAddr )
    {    
	__SUBCONTROLLER( "CMccSymSubthreadClient::SetRemoteAddressL" )
    __ASSERT_ALWAYS( iRtpmanager, User::Leave( KErrArgument ) );

    User::LeaveIfError( iRtpmanager->SetRemoteAddress( aRemAddr ) );
    
    TInt strmCount = iStreams.Count();
    for( TInt k = 0; k < strmCount; k++ )
        {
        iStreams[k]->ResetCountersL();
        }
	__SUBCONTROLLER( "CMccSymSubthreadClient::SetRemoteAddressL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccSymSubthreadClient::SetRemoteAddressL
// Sets the remote address of uplink stream
// -----------------------------------------------------------------------------
void CMccSymSubthreadClient::SetRemoteRtcpAddrL( TInetAddr aRemAddr )
    {    
	__SUBCONTROLLER( "CMccSymSubthreadClient::SetRemoteRtcpAddrL" )
    __ASSERT_ALWAYS( iRtpmanager, User::Leave( KErrArgument ) );

    User::LeaveIfError( iRtpmanager->SetRemoteRtcpAddr( aRemAddr ) );
    
    TInt strmCount = iStreams.Count();
    for( TInt k = 0; k < strmCount; k++ )
        {
        iStreams[k]->ResetCountersL();
        }
	__SUBCONTROLLER( "CMccSymSubthreadClient::SetRemoteRtcpAddrL, exit" )
    }
    
// -----------------------------------------------------------------------------
// CMccSymSubthreadClient::InitializeLinkL
// Initializes the RTP session in the subthread
// -----------------------------------------------------------------------------
//
void CMccSymSubthreadClient::InitializeLinkL( TRequestStatus& aStatus,
                                              TInt aIapId )
    {    
	__SUBCONTROLLER( "CMccSymSubthreadClient::InitializeLinkL" )
    __ASSERT_ALWAYS( !iSessionCreated, User::Leave( KErrAlreadyExists ) );
    iRtpmanager->InitializeL( aStatus, aIapId );
	__SUBCONTROLLER( "CMccSymSubthreadClient::InitializeLinkL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccSymSubthreadClient::CreateRtpSessionL
// Creates the RTP session in the subthread
// -----------------------------------------------------------------------------
//
void CMccSymSubthreadClient::CreateRtpSessionL( TUint aPort, 
                                                TBool aEnableRTCP,
                                                TInt aIpTos, 
                                                CMccRtpMediaClock& aClock )
    {
	__SUBCONTROLLER( "CMccSymSubthreadClient::CreateRtpSessionL" )
    iRtpMediaClock = &aClock;
    
    TBool secure( EFalse );
    if ( KMccLinkSecure == iLinkType || KMccLinkSecureVideo == iLinkType )
        {
        secure = ETrue;
        }
    
    TBool increaseDefaultSocketSize( EFalse );
    if ( KMccLinkGeneralVideo == iLinkType || KMccLinkSecureVideo == iLinkType )
        {
        increaseDefaultSocketSize = ETrue;
        }
        
    iRtpmanager->CreateSessionL( aPort, aEnableRTCP, secure, increaseDefaultSocketSize );
    iRtpmanager->SetIpTOS( aIpTos );
	iSessionCreated = ETrue;
	
	__SUBCONTROLLER( "CMccSymSubthreadClient::CreateRtpSessionL, exit" )
	}
    
// -----------------------------------------------------------------------------
// CMccSymSubthreadClient::SetAudioRouteL
// -----------------------------------------------------------------------------
//    
void CMccSymSubthreadClient::SetAudioRouteL( 
    TUint32 aStreamId,
    TUint32 aRoutingDestination )
    {
    __SUBCONTROLLER( "CMccSymSubthreadClient::SetAudioRouteL" )
    TInt index = FindStreamL( aStreamId );
    iStreams[ index ]->SetAudioRouteL( aRoutingDestination );
	__SUBCONTROLLER( "CMccSymSubthreadClient::SetAudioRouteL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccSymSubthreadClient::GetAudioRouteL
// -----------------------------------------------------------------------------
//
void CMccSymSubthreadClient::GetAudioRouteL( 
    TUint32 aStreamId,
    TUint32& aRoutingDestination )
    {
    __SUBCONTROLLER( "CMccSymSubthreadClient::GetAudioRouteL" )
    TInt index = FindStreamL( aStreamId );
    iStreams[ index ]->GetAudioRouteL( aRoutingDestination );
	__SUBCONTROLLER( "CMccSymSubthreadClient::GetAudioRouteL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccSymSubthreadClient::UnuseL
// -----------------------------------------------------------------------------
//    
void CMccSymSubthreadClient::UnuseL( TUint32 aStreamId )
    {
    __SUBCONTROLLER_INT1( "CMccSymSubthreadClient::UsuseL, streamId", aStreamId )
    
    TInt index = FindStream( aStreamId );
    if ( index != KErrNotFound )
        {
        CMccSymStreamBase* tmp = iStreams[index];
        iStreams.Remove( index );
        delete tmp;
        
        // Store stream id for later use (when stream is explicitly deleted)
        iUnusedStreams.AppendL( aStreamId );
        }
    else 
        {
        index = iUnusedStreams.Find( aStreamId );
        __ASSERT_ALWAYS( index != KErrNotFound, User::Leave( KErrNotFound ) );
        }
    
	__SUBCONTROLLER( "CMccSymSubthreadClient::UsuseL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccSymSubthreadClient::GetLocalIpAddressesL
// -----------------------------------------------------------------------------
//
void CMccSymSubthreadClient::GetLocalIpAddressesL( TMccCreateLink& aClientData )
	{
	__SUBCONTROLLER( "CMccSymSubthreadClient::GetLocalIpAddressesL" )
	
	iRtpmanager->GetLocalIpAddressesL( aClientData );
	                                      
	__SUBCONTROLLER( "CMccSymSubthreadClient::GetLocalIpAddressesL, exit" )
	}

// -----------------------------------------------------------------------------
// CMccSymSubthreadClient::SetParameterL
// -----------------------------------------------------------------------------
//     
void CMccSymSubthreadClient::SetParameterL( 
    TUint32 aParam,
    TUint32 /*aStreamId*/,
    TUint32 /*aEndpointId*/,
    const TDesC8& aVal )
    {
    __SUBCONTROLLER( "CMccSymSubthreadClient::SetParameterL" )
    
    switch ( aParam )
        {
        case KMccRtpCName:
            {
            iRtpmanager->SetCNameL( aVal );
            break;
            }
        default:
            {
            User::Leave( KErrNotSupported );
            break;
            }
        }
        
    __SUBCONTROLLER( "CMccSymSubthreadClient::SetParameterL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccSymSubthreadClient::GetParameterL
// -----------------------------------------------------------------------------
//          
void CMccSymSubthreadClient::GetParameterL( 
    TUint32 /*aParam*/,
    TUint32 /*aStreamId*/,
    TUint32 /*aEndpointId*/,
    TDesC8& /*aVal*/ )
    {
    }
    

// -----------------------------------------------------------------------------
// CMccSymSubthreadClient::SendEventToClient
// -----------------------------------------------------------------------------
//    
TInt CMccSymSubthreadClient::SendEventToClient( const TMMFEvent& aEvent )
	{
	__SUBCONTROLLER( "CMccSymSubthreadClient::SendEventToClient" )
	
	// Do in a bit ackward way in order to save stack memory
	TMccEvent* mccEventPtr = 0;
	if ( IS_MCC_EVENT( aEvent ) )
	    {
	    mccEventPtr = 
	        reinterpret_cast<const TMccInternalEvent&>( aEvent ).iMccEvent;                                     
	    }
	
	if ( mccEventPtr )
	    {
	    mccEventPtr->iLinkId = iLinkId;

    	__SUBCONTROLLER_EVENT( (*mccEventPtr) )	    

        TUid eventUid = aEvent.iEventType;
        TUid amrUid = TUid::Uid( KDllUidAmrPayloadFormat );
	    
	    // If RTCP BYE received remove crypto context
	    if ( KMccRtcpReceived == mccEventPtr->iEventType )
	        {
	        const TMccRtcpEventData& eventdata = 
    		    (*reinterpret_cast<const TMccRtcpEventDataPackage*>( 
                &mccEventPtr->iEventData ))();
                
            if ( KRtcpByePacket == eventdata.iRtcpPacketType )
                {
                // Remove crypto context                               
                const TInt count = iStreams.Count();
                for( TInt k = 0; k < count; k++ )
                    {
                    // Ignore the return value as we're doing this blindly                        
                    iStreams[k]->RemoveContext( 0 );
                    }   
                }
	        }
	    
        if( amrUid == eventUid )
            {
            // Amr events not forwarded
            TInt err = HandleAmrEvent( aEvent, *mccEventPtr );
            __SUBCONTROLLER( "CMccSymSubthreadClient::SendEventToClient, exit" )
            return err;
            }
            
	    iObserver->SendMccEventToClient( *mccEventPtr );
	    }
	else
	    {
	    TMccEvent mccEvent;
	    DoMccEvent( mccEvent, aEvent );

    	__SUBCONTROLLER_EVENT( mccEvent )

	    iObserver->SendMccEventToClient( mccEvent );
	    }
    
	__SUBCONTROLLER( "CMccSymSubthreadClient::SendEventToClient, exit" )
    return KErrNone;
	}
    
// -----------------------------------------------------------------------------
// CMccSymSubthreadClient::DoMccEvent
// -----------------------------------------------------------------------------
//
void CMccSymSubthreadClient::DoMccEvent( 
    TMccEvent& aEvent, 
    const TMMFEvent& aMMFEvent )
    {
    aEvent.iEventCategory = KMccEventCategoryStream;
	aEvent.iEventType = KMccStreamMMFEvent;
    aEvent.iErrorCode = aMMFEvent.iErrorCode;
    aEvent.iLinkId = iLinkId;
    }
    
// -----------------------------------------------------------------------------
// CMccSymSubthreadClient::DataSink
// -----------------------------------------------------------------------------
//
MDataSink* CMccSymSubthreadClient::DataSink( const TUint32 aStreamId )
    {
	__SUBCONTROLLER_INT1( "CMccSymSubthreadClient::DataSink, streamId", aStreamId )
    TInt strmCount = iStreams.Count();
    if( strmCount )
        {
        TInt index = this->FindStream( aStreamId );
        if ( index != KErrNotFound )
	        {
	        return iStreams[index]->DataSink();        	
	        }
        }
    return NULL;    
    }

// -----------------------------------------------------------------------------
// CMccSymSubthreadClient::DataSource
// -----------------------------------------------------------------------------
//
MDataSource* CMccSymSubthreadClient::DataSource( const TUint32 aStreamId )
    {
	__SUBCONTROLLER_INT1( "CMccSymSubthreadClient::DataSource, streamId", aStreamId )
    TInt strmCount = iStreams.Count();
    if( strmCount )
        {
        TInt index = this->FindStream( aStreamId );
        if ( index != KErrNotFound )
	        {
	        return iStreams[index]->DataSource();        	
	        }
        }
    return NULL; 
    }

// -----------------------------------------------------------------------------
// CMccSymSubthreadClient::BindContextIntoStreamL
// -----------------------------------------------------------------------------
//
void CMccSymSubthreadClient::BindContextIntoStreamL( TUint32 aStreamId,
                                                     TUint32 aEndpointId,
                                                     const TMccCryptoContext& aContextParams )
    {
	__SUBCONTROLLER( "CMccSymSubthreadClient::BindContextIntoStreamL IN" )
	
	TInt index = this->FindStreamL( aStreamId );
	
	iStreams[ index ]->BindContextIntoStreamL( aEndpointId, aContextParams );

	__SUBCONTROLLER( "CMccSymSubthreadClient::BindContextIntoStreamL END" )
    }   

// -----------------------------------------------------------------------------
// CMccSymSubthreadClient::RemoveContextL
// -----------------------------------------------------------------------------
//
void CMccSymSubthreadClient::RemoveContextL( TUint32 aStreamId,
                                             TUint32 aEndpointId )
    {
	__SUBCONTROLLER( "CMccSymSubthreadClient::RemoveContextL IN" )
	
	TInt index = this->FindStreamL( aStreamId );
	
	iStreams[ index ]->RemoveContext( aEndpointId );

	__SUBCONTROLLER( "CMccSymSubthreadClient::RemoveContextL END" )
    }

// -----------------------------------------------------------------------------
// CMccSymSubthreadClient::HandleAmrEvent
// -----------------------------------------------------------------------------
//
TInt CMccSymSubthreadClient::HandleAmrEvent( 
    const TMMFEvent& aEvent,
    const TMccEvent& aMccEvent )
    {
    const TInt internalEvent = 
        reinterpret_cast<const TMccInternalEvent&>( aEvent ).iInternalEventType; 
    
    const TMccAmrEventData& eventdata = 
	    (*reinterpret_cast<const TMccAmrEventDataPackage*>( 
        &aMccEvent.iEventData ))();
    
    __SUBCONTROLLER_INT1( "CMccSymStreamBase::HandleAmrEvent AMR EVENT:", 
                          internalEvent );
    __SUBCONTROLLER_INT1( "CMccSymStreamBase::HandleAmrEvent AMR MODE_REQ BITRATE:", 
                          eventdata.iModeRequestBitrate );
    
    if ( EMccInternalAmrEventCmr == internalEvent )
        {
        if ( eventdata.iModeRequestBitrate > 0 )
            {
            // Mode change request, forward it to other streams
            const TInt count = iStreams.Count();
            for( TInt k = 0; k < count; k++ )
                {
                // Ignore the return value as we're doing this blindly
                iStreams[k]->ChangeBitrate( eventdata.iModeRequestBitrate );
                }
            }
        }
    return KErrNone;
    }
    
// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File
