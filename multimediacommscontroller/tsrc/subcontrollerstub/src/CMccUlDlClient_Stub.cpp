/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    UL/DL Session abstraction for Mcc Subthread clients
*
*/




// INCLUDE FILES
#include <e32math.h>

#include "mcculdlclient.h"

#include "mmccnetworksettings.h"
#include "mccsymsubthreadclient.h"
#include "mcccodecconfigurator.h"
#include "mccsubcontrollerlogs.h"
#include "mccrtpmediaclock.h"
#include "mmccsecureinterface.h"
#include "mmccEvents.h"
#include "mccinternaldef.h"
#include "mccinternalevents.h"


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
// CMccUlDlClient::CMccUlDlClient
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
CMccUlDlClient::CMccUlDlClient( MMccEventHandler* aMccEventHandler,
                                MMccResources* aMccResources,
                                TUint32 aSessionId ) :
                                iSessionId( aSessionId ),
                                iMccEventHandler( aMccEventHandler ),
                                iMccResources( aMccResources )
    {
    }

// -----------------------------------------------------------------------------
// CMccUlDlClient::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
void CMccUlDlClient::ConstructL()
    {
    //Create an event and send it directly
        {
            
        TMccEvent event( 1, 1, 1, 1, KMccEventCategoryStream, KMccInactivityEvent,
                             KErrNone, KNullDesC8 );
        iMccEventHandler->SendMccEventToClient( event );
        }
    
        {
            
        TMccRtcpEventData rtcpData;
        rtcpData.iRtcpPacketData.Copy( _L8("foofoo") );
        rtcpData.iRtcpPacketType = KRtcpAnyPacket;
        rtcpData.iSsrc = 2;
        
        TMccRtcpEventDataPackage rtcpDataPackage( rtcpData );
        
        //Create a media signal and send it 
        TMccEvent event( 1, 1, 1, 1, KMccEventCategoryRtcp, KMccRtcpReceived,
                             KErrNone, rtcpDataPackage );
        
        
        iMccEventHandler->SendMccEventToClient( event );
        
        }
        
        {  
        TMccEvent event( 1, 1, 1, 1, KMccEventCategoryStream, KMccStreamMMFEvent,
                             KErrDied, KNullDesC8 );
        iMccEventHandler->SendMccEventToClient( event );
        }
        
        {  
        TMccEvent event( 1, 1, 1, 1, KMccEventCategoryStreamControl, KMccStreamPaused,
                             KErrDied, KNullDesC8 );
        iMccEventHandler->SendMccEventToClient( event );
        
        event.iEventType = KMccStreamResumed;
        iMccEventHandler->SendMccEventToClient( event );
        
        event.iEventType = KMccStreamStopped;
        iMccEventHandler->SendMccEventToClient( event );
        }
        
    iMccEventHandler->StateChange( 0, 0 );
    iMccEventHandler->ErrorOccured( KErrNone, 0, 0, 0, 0 );    
    iMccEventHandler->ErrorOccured( KErrGeneral, 0, 0, 0, 0 );
    
    // go through DTMF event handling paths
    for ( TInt i = KMccDtmfManualStart; i <= KMccDtmfStopInDtmfString; i++ )
        {
        TMccDtmfEventData eventSignal;
        eventSignal.iDtmfEventType = TMccDtmfEventType( i );
        TMccDtmfEventDataPackage signalPkg( eventSignal );
        
        TMccEvent dtmfEvent( 1, 1, 1, 1, 
            KMccEventCategoryDtmf, KMccDtmfControl, KErrNone, signalPkg );
        iMccEventHandler->SendMccEventToClient( dtmfEvent );
        }
    };

// -----------------------------------------------------------------------------
// CMccUlDlClient::NewL
// Static constructor.
// -----------------------------------------------------------------------------
EXPORT_C CMccUlDlClient* CMccUlDlClient::NewL( MMccEventHandler* aMccEventHandler,
                                               MMccResources* aMccResources,
                                               TUint32 aSessionId )
    {
    CMccUlDlClient* self = new ( ELeave ) CMccUlDlClient( aMccEventHandler,
                                                          aMccResources,
                                                          aSessionId );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// -----------------------------------------------------------------------------
// CMccUlDlClient::~CMccUlDlClient
// Destructor
// -----------------------------------------------------------------------------  
EXPORT_C CMccUlDlClient::~CMccUlDlClient()
    {
	__SUBCONTROLLER( "CMccUlDlClient::~CMccUlDlClient" )
	__SUBCONTROLLER( "CMccUlDlClient::~CMccUlDlClient, exit" )
    }

// -----------------------------------------------------------------------------
// CMccUlDlClient::MaxGainL
// Returns the maximum gain for uplink stream
// -----------------------------------------------------------------------------
//
EXPORT_C void CMccUlDlClient::MaxGainL( const TUint32 aLinkId, 
										const TUint32 aStreamId,
                                        TInt& aMaxGain )
    {
	__SUBCONTROLLER( "CMccUlDlClient::MaxGainL" )
    }

// -----------------------------------------------------------------------------
// CMccUlDlClient::MaxVolumeL
// Returns the mareximum volume for downlink stream
// -----------------------------------------------------------------------------
//
EXPORT_C void CMccUlDlClient::MaxVolumeL( const TUint32 aLinkId,
										  const TUint32 aStreamId,
                                          TInt& aMaxVolume )
    {
	__SUBCONTROLLER( "CMccUlDlClient::MaxVolumeL" )
    }

// -----------------------------------------------------------------------------
// CMccUlDlClient::OpenL
// Opens ul/dl clients
// -----------------------------------------------------------------------------
//
EXPORT_C void CMccUlDlClient::OpenL( const TUint32 aLinkId,
                                     const TUint32 aStreamId,
                                     TInt aStreamType,
                                     TFourCC aFourCC,
                                     MDataSource* aDataSource, 
                                     MDataSink* aSink,
                                     TMMFPrioritySettings aSettings )
    {
	__SUBCONTROLLER( "CMccUlDlClient::OpenL" )
    }    

// -----------------------------------------------------------------------------
// CMccUlDlClient::PrepareL
// Prepare ul/dl client
// -----------------------------------------------------------------------------
//
EXPORT_C void CMccUlDlClient::PrepareL( const TUint32 aLinkId,
                                        const TUint32 aStreamId,
                                        const TUint32 aEndpointId )
    {
	__SUBCONTROLLER( "CMccUlDlClient::PrepareL" )
    }  

// -----------------------------------------------------------------------------
// CMccUlDlClient::PlayL
// Plays ul/dl stream
// -----------------------------------------------------------------------------
//
EXPORT_C void CMccUlDlClient::PlayL( const TUint32 aLinkId,
                                     const TUint32 aStreamId,
                                     const TUint32 aEndpointId,
                                     TBool aStreamPaused,
                                     TBool aEnableRTCP )
    {
	__SUBCONTROLLER( "CMccUlDlClient::PlayL" )
    }

// -----------------------------------------------------------------------------
// CMccUlDlClient::PauseL
// Pauses ul/dl stream
// -----------------------------------------------------------------------------
//
EXPORT_C void CMccUlDlClient::PauseL( const TUint32 aLinkId,
                                      const TUint32 aStreamId,
                                      const TUint32 aEndpointId,
                                      TBool aEnableRTCP )
    {
	__SUBCONTROLLER( "CMccUlDlClient::PauseL" )
    }

// -----------------------------------------------------------------------------
// CMccUlDlClient::StopL
// Stops ul/dl stream
// -----------------------------------------------------------------------------
//
EXPORT_C void CMccUlDlClient::StopL( const TUint32 aLinkId,
                                     const TUint32 aStreamId,
                                     const TUint32 aEndpointId )
    {
	__SUBCONTROLLER( "CMccUlDlClient::StopL" )
    }

// -----------------------------------------------------------------------------
// CMccUlDlClient::CloseL
// Closes ul/dl client
// -----------------------------------------------------------------------------
//
EXPORT_C void CMccUlDlClient::CloseL( const TUint32 aLinkId,
                                      const TUint32 aStreamId )    
    {
	__SUBCONTROLLER( "CMccUlDlClient::CloseL" )
    }

// -----------------------------------------------------------------------------
// CMccUlDlClient::ResumeL
// Resumes ul/dl stream
// -----------------------------------------------------------------------------
//
EXPORT_C void CMccUlDlClient::ResumeL( const TUint32 aLinkId,
                                       const TUint32 aStreamId,
                                       const TUint32 aEndpointId,
                                       TBool aEnableRTCP )
    {
	__SUBCONTROLLER( "CMccUlDlClient::ResumeL" )
    }    

// -----------------------------------------------------------------------------
// CMccUlDlClient::ResumeL
// Resumes ul/dl stream
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CMccUlDlClient::StreamsExistsL( const TUint32 aLinkId )
    {
	__SUBCONTROLLER( "CMccUlDlClient::StreamsExistsL" )
	return EFalse;
    }    


// -----------------------------------------------------------------------------
// CMccUlDlClient::GetGainL
// Get the current ul gain
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMccUlDlClient::GetGainL( const TUint32 aLinkId,
                                        const TUint32 aStreamId,
                                        TInt& aGain )
    {
	__SUBCONTROLLER( "CMccUlDlClient::GetGainL" )
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccUlDlClient::GetCodecL
// Get current codec
// -----------------------------------------------------------------------------
//
EXPORT_C void CMccUlDlClient::GetCodecL( const TUint32 aLinkId,
                                         const TUint32 aStreamId,
                                         TMccCodecInfo& aCodec )
    {
	__SUBCONTROLLER( "CMccUlDlClient::GetCodecL" )
    }

// -----------------------------------------------------------------------------
// CMccUlDlClient::GetFmtpAttrL
// Get current codec
// -----------------------------------------------------------------------------
//
EXPORT_C void CMccUlDlClient::GetFmtpAttrL( const TUint32 aLinkId,
                                              const TUint32 aStreamId,
                                              TDes8& aFmtp )
    {
	__SUBCONTROLLER( "CMccUlDlClient::GetFmtpAttrL" )
	aFmtp.Format( _L8( "foo42" ) );
    }

// -----------------------------------------------------------------------------
// CMccUlDlClient::GetPlayBalanceL
// Returns the play balance of dl client 
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMccUlDlClient::GetPlayBalanceL( const TUint32 aLinkId,
                                               const TUint32 aStreamId,
                                               TInt& aLeftBalance,
                                               TInt& aRightBalance )
    {
	__SUBCONTROLLER( "CMccUlDlClient::GetPlayBalanceL" )
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccUlDlClient::GetRecordBalanceL
// Returns the record balance of ul client
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMccUlDlClient::GetRecordBalanceL( const TUint32 aLinkId,
                                                 const TUint32 aStreamId,
                                                 TInt& aLeftBalance,
                                                 TInt& aRightBalance )
    {
	__SUBCONTROLLER( "CMccUlDlClient::GetRecordBalanceL" )
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccUlDlClient::SetPriorityL
// Sets the link priority
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMccUlDlClient::SetPriorityL( const TUint32 aLinkId,
                                            const TUint32 aStreamId,
                                            TMMFPrioritySettings aPrioritySettings )
    {
	__SUBCONTROLLER( "CMccUlDlClient::SetPriorityL" )
    return KErrNone;
    }   


// -----------------------------------------------------------------------------
// CMccUlDlClient::SetVolumeL
// Sets the volume for downlink stream
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMccUlDlClient::SetVolumeL( TInt aVolume )
    {
	__SUBCONTROLLER( "CMccUlDlClient::GetRecordBalanceL" )
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccUlDlClient::SetGainL
// Sets the gain for uplink stream
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMccUlDlClient::SetGainL( TInt aGain )
    {
	__SUBCONTROLLER( "CMccUlDlClient::SetGainL" )
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccUlDlClient::SetRecordBalanceL
// Set the record balance for uplink stream
// -----------------------------------------------------------------------------
//
EXPORT_C void CMccUlDlClient::SetRecordBalanceL( const TUint32 aLinkId,
                                                 const TUint32 aStreamId, 
                                                 TInt aLeftBalance, 
                                                 TInt aRightBalance )
    {
	__SUBCONTROLLER( "CMccUlDlClient::SetRecordBalanceL" )
    }

// -----------------------------------------------------------------------------
// CMccUlDlClient::SetPlayBalanceL
// Set the record balance for downlink stream
// -----------------------------------------------------------------------------
//
EXPORT_C void CMccUlDlClient::SetPlayBalanceL( const TUint32 aLinkId, 
                                               const TUint32 aStreamId, 
                                               TInt aLeftBalance, 
                                               TInt aRightBalance )
    {
	__SUBCONTROLLER( "CMccUlDlClient::SetPlayBalanceL" )
    }

// -----------------------------------------------------------------------------
// CMccUlDlClient::CreateLinkL
// Creates link, 1st step in link creation
// -----------------------------------------------------------------------------
//
EXPORT_C void CMccUlDlClient::CreateLinkL( TUint32& aLinkId,
                                           TInt aType )
    {
	__SUBCONTROLLER( "CMccUlDlClient::CreateLinkL" )
	aLinkId = 1;
    }

// -----------------------------------------------------------------------------
// CMccUlDlClient::InitializeLinkL
// Initializes ul and dl links, 2nd step in link creation
// -----------------------------------------------------------------------------
//
EXPORT_C void CMccUlDlClient::InitializeLinkL( TRequestStatus& aStatus,
                                               TUint32 aLinkId,
                                               TInt aIapId )
    {
	__SUBCONTROLLER( "CMccUlDlClient::InitializeLinkL" )
	
	TRequestStatus* status = &aStatus;
	User::RequestComplete( status, KErrNone );
    }

// -----------------------------------------------------------------------------
// CMccUlDlClient::CreateRtpSessionL
// Creates RTP session, 3rd step in link creation
// -----------------------------------------------------------------------------
//
EXPORT_C void CMccUlDlClient::CreateRtpSessionL( TUint32 aLinkId,
                                                 TUint aPort,
                                                 TInt aIpTOS,
                                                 TInt /*aMediaSignaling*/ )  
    {
    __SUBCONTROLLER( "CMccUlDlClient::CreateRtpSessionL" )
    }

// -----------------------------------------------------------------------------
// CMccUlDlClient::CloseLinkL
// Closes ul/dl client
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMccUlDlClient::CloseLinkL( TUint32 aLinkId )    
    {
	__SUBCONTROLLER( "CMccUlDlClient::CloseLinkL" )
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccUlSubThreadClient::SetRemoteAddressL
// Sets the remote address for uplink stream
// -----------------------------------------------------------------------------
EXPORT_C void CMccUlDlClient::SetRemoteAddressL( TInetAddr aRemAddr,
                                                 TUint32 aLinkId ) 
    {
	__SUBCONTROLLER( "CMccUlDlClient::SetRemoteAddressL" )
    }
    
// -----------------------------------------------------------------------------
// CMccUlSubThreadClient::SetRemoteRtcpAddrL
// Sets the remote address for uplink stream
// -----------------------------------------------------------------------------
EXPORT_C void CMccUlDlClient::SetRemoteRtcpAddrL( TInetAddr aRemAddr,
                                                 TUint32 aLinkId ) 
    {
	__SUBCONTROLLER( "CMccUlDlClient::SetRemoteRtcpAddressL" )
    }
    

// -----------------------------------------------------------------------------
// CMccUlDlClient::GetCodecDefaultsL
// Get default codec values
// -----------------------------------------------------------------------------
//
EXPORT_C void CMccUlDlClient::GetCodecDefaultsL(
	const TUid aType, TDes8& aParam, TMccCodecInfo& aCodec )
    {
	__SUBCONTROLLER( "CMccUlDlClient::GetCodecDefaultsL" )
	}

// -----------------------------------------------------------------------------
// CMccUlDlClient::GetSessionId()
// returns the session id
// -----------------------------------------------------------------------------
//
EXPORT_C TUint32 CMccUlDlClient::GetSessionId()
    {
	__SUBCONTROLLER_INT1( "CMccUlDlClient::GetSessionId, id ", iSessionId )
    return iSessionId;
    }

// -----------------------------------------------------------------------------
// CMccUlDlClient::SendMediaSignalL
// Send media signal to given stream
// -----------------------------------------------------------------------------
//
EXPORT_C void CMccUlDlClient::SendMediaSignalL( const TMccEvent& aEvent )
    {
	__SUBCONTROLLER( "CMccUlDlClient::SendMediaSignalL" )
    }

// -----------------------------------------------------------------------------
// CMccUlDlClient::SetCodecInformationL
// Set the codec information for the stream
// -----------------------------------------------------------------------------
//
EXPORT_C void CMccUlDlClient::SetCodecInformationL( const TUint32 aLinkId, 
                                                    const TUint32 aStreamId,
                                                    const TMccCodecInfo& aCodecInfo,
                                                    const TDesC8& aFmtp )
    {
	__SUBCONTROLLER( "CMccUlDlClient::SetCodecInformationL" )
    }
 
// -----------------------------------------------------------------------------
// CMccUlDlClient::GetSupportedBitratesL
// Get the supported bitrates from the codec of the specified stream
// -----------------------------------------------------------------------------
//
EXPORT_C void CMccUlDlClient::GetSupportedBitratesL( const TUint32 aLinkId, 
                                  const TUint32 /*aStreamId*/,
                                  RArray<TUint>& aBitrates )
    {
	__SUBCONTROLLER( "CMccUlDlClient::GetSupportedBitratesL" )
	User::LeaveIfError(aBitrates.Append(1000));
    }

// -----------------------------------------------------------------------------
// CMccUlDlClient::GetSSRCL
// Get syncronization source for the specified stream
// -----------------------------------------------------------------------------
//
EXPORT_C void CMccUlDlClient::GetSSRCL( const TUint32 aLinkId, 
                       					const TUint32 aStreamId, 
                        				TUint32&      aSSRCValue)
	{
	__SUBCONTROLLER( "CMccUlDlClient::GetSSRCL" )
	aSSRCValue = 1;
	}

// -----------------------------------------------------------------------------
// CMccUlDlClient::SendRTCPReceiverReportL
// Sends a RTCP receiver report
// -----------------------------------------------------------------------------
// 
EXPORT_C void CMccUlDlClient::SendRTCPReceiverReportL( const TUint32 aLinkId, 
                                                       const TUint32 aStreamId )
    {
	__SUBCONTROLLER( "CMccUlDlClient::SendRTCPReceiverReportL" )
    }

// -----------------------------------------------------------------------------
// CMccUlDlClient::SendRTCPSenderReportL
// Sends a RTCP sender report
// -----------------------------------------------------------------------------
//
EXPORT_C void CMccUlDlClient::SendRTCPSenderReportL( const TUint32 aLinkId,
                                                     const TUint32 aStreamId )
    {
	__SUBCONTROLLER( "CMccUlDlClient::SendRTCPSenderReportL" )
    }

// -----------------------------------------------------------------------------
// CMccUlDlClient::SendRTCPSenderReportL
// Sends a RTCP sender report
// -----------------------------------------------------------------------------
//
EXPORT_C void CMccUlDlClient::SendRTCPDataL( const TUint32 aLinkId,
                                             const TUint32 aStreamId,
                                             const TDesC8& aData )
    {
	__SUBCONTROLLER( "CMccUlDlClient::SendRTCPDataL" )
    }

// -----------------------------------------------------------------------------
// CMccUlDlClient::StartInactivityTimer
// Starts inactivity timer for a stream in a given link.
// -----------------------------------------------------------------------------
//
EXPORT_C void CMccUlDlClient::StartInactivityTimerL( const TUint32 aLinkId, 
                                                     const TUint32 aStreamId,
                                                     TUint32 aTimeoutTime )
    {
	__SUBCONTROLLER( "CMccUlDlClient::StartInactivityTimerL" )
    }                                           

// -----------------------------------------------------------------------------
// CMccUlDlClient::StopInactivityTimer
// Stops inactivity timer for a stream in a given link.
// -----------------------------------------------------------------------------
//
EXPORT_C void CMccUlDlClient::StopInactivityTimerL( const TUint32 aLinkId,
                                                    const TUint32 aStreamId )
    {
	__SUBCONTROLLER( "CMccUlDlClient::StopInactivityTimerL" )
    }

// -----------------------------------------------------------------------------
// CMccUlDlClient::SetAudioRouteL
// -----------------------------------------------------------------------------
//    
EXPORT_C void CMccUlDlClient::SetAudioRouteL( 
    TUint32 aLinkId, 
    TUint32 aStreamId,
    TUint32 aRoutingDestination )
    {
    __SUBCONTROLLER( "CMccUlDlClient::SetAudioRouteL" )
    }

// -----------------------------------------------------------------------------
// CMccUlDlClient::GetAudioRouteL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMccUlDlClient::GetAudioRouteL( 
    TUint32 aLinkId, 
    TUint32 aStreamId,
    TUint32& aRoutingDestination )
    {
    __SUBCONTROLLER( "CMccUlDlClient::SetAudioRouteL" )
    }
      
// -----------------------------------------------------------------------------
// CMccUlDlClient::GenerateStreamId
// Generates a new stream identifier
// -----------------------------------------------------------------------------
//
EXPORT_C TUint32 CMccUlDlClient::GenerateStreamId()
    {
	__SUBCONTROLLER( "CMccUlDlClient::GenerateStreamId" )
    TUint32 streamId = ++iErrorCode;
    return streamId;
    }   

// -----------------------------------------------------------------------------
// CMccUlDlClient::UnuseL
// Ususes stream
// -----------------------------------------------------------------------------
//   
EXPORT_C void CMccUlDlClient::UnuseL( 
    const TUint32 aLinkId,
    const TUint32 aStreamId )
    {
    __SUBCONTROLLER( "CMccUlDlClient::UnuseL" )
    }
    

// -----------------------------------------------------------------------------
// CMccUlDlClient::GetLocalIpAddressesL()
// -----------------------------------------------------------------------------
//     
EXPORT_C void CMccUlDlClient::GetLocalIpAddressesL( TMccCreateLink& /*aClientData*/ )
    {
    }
    
// -----------------------------------------------------------------------------
// CMccUlDlClient::SendMccEventToClient
// -----------------------------------------------------------------------------
//
TInt CMccUlDlClient::SendMccEventToClient( TMccEvent& aEvent )
    {
	__SUBCONTROLLER( "CMccUlDlClient::SendMccEventToClient" )
	return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccUlDlClient::StateChange
// -----------------------------------------------------------------------------
//
void CMccUlDlClient::StateChange( TInt aState, TUint32 aLinkId )
	{	
	__SUBCONTROLLER( "CMccUlDlClient::StateChange" )	
	}
	
// -----------------------------------------------------------------------------
// CMccUlDlClient::ErrorOccured
// -----------------------------------------------------------------------------
//
void CMccUlDlClient::ErrorOccured( 
    TInt aError, 
    TUint32 /*aSessionId*/, 
    TUint32 aLinkId,
	TUint32 aStreamId, 
	TUint32 aEndpointId )
	{	
	__SUBCONTROLLER( "CMccUlDlClient::ErrorOccured" )
	}
                                          
// -----------------------------------------------------------------------------
// CMccUlDlClient::FindLinkL
// finds correct link to handle
// -----------------------------------------------------------------------------
//
TInt CMccUlDlClient::FindLinkL( const TUint32 aLinkId )
    {
	__SUBCONTROLLER_INT1( "CMccUlDlClient::FindLinkL, linkid", aLinkId )

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccUlDlClient::StoreDlStream()
// stores the pointer to ul/dl stream
// -----------------------------------------------------------------------------
//
TInt CMccUlDlClient::StoreLink( CMccSubThreadClientBase* aClient )
    {
	__SUBCONTROLLER( "CMccUlDlClient::StoreLink" )
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccUlDlClient::BindContextIntoStreamL()
// -----------------------------------------------------------------------------
//        
EXPORT_C void CMccUlDlClient::BindContextIntoStreamL( TUint32 aLinkId,
                                                      TUint32 aStreamId,
                                                      TUint32 aEndpointId,
                                                      const TMccCryptoContext& aCryptoContext )
    {
    __SUBCONTROLLER( "CMccUlDlClient::BindContextIntoStreamL, IN ")
    }

// -----------------------------------------------------------------------------
// CMccUlDlClient::RemoveContextL()
// -----------------------------------------------------------------------------
//        
EXPORT_C void CMccUlDlClient::RemoveContextL( TUint32 aLinkId,
                                              TUint32 aStreamId,
                                              TUint32 aEndpointId )
    {
    __SUBCONTROLLER( "CMccUlDlClient::RemoveContextL, IN ")

    }

// -----------------------------------------------------------------------------
// CMccUlDlClient::SetParameterL()
// -----------------------------------------------------------------------------
//        
EXPORT_C void CMccUlDlClient::SetParameterL( TUint32 /*aParam*/,
                                     TUint32 /*aLinkId*/,
                                     TUint32 /*aStreamId*/,
                                     TUint32 /*aEndpointId*/,
                                     const TDesC8& /*aVal*/ )
    {
    }
         
// -----------------------------------------------------------------------------
// CMccUlDlClient::GetParameterL()
// -----------------------------------------------------------------------------
//        
EXPORT_C void CMccUlDlClient::GetParameterL( TUint32 /*aParam*/,
                                     TUint32 /*aLinkId*/,
                                     TUint32 /*aStreamId*/,
                                     TUint32 /*aEndpointId*/,
                                     TDesC8& /*aVal*/ )
    {
    }
       
// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File
