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
	__SUBCONTROLLER( "CMccUlDlClient::ConstructL" )
    User::LeaveIfNull( iMccEventHandler );
    User::LeaveIfNull( iMccResources );
    iRtpMediaClock = CMccRtpMediaClock::NewL();
    
	__SUBCONTROLLER( "CMccUlDlClient::ConstructL, exit" )
    }   

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
	
    iClientArray.ResetAndDestroy();
    iClientArray.Close();
    
    delete iRtpMediaClock;

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
    TInt stream = FindLinkL( aLinkId );
    aMaxGain = iClientArray[stream]->MaxGainL( aStreamId );
	__SUBCONTROLLER_INT1( "CMccUlDlClient::MaxGainL, exit with MaxGain",
	                      aMaxGain )
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
    TInt link = FindLinkL( aLinkId );
    aMaxVolume = iClientArray[link]->MaxVolumeL( aStreamId );
	__SUBCONTROLLER_INT1( "CMccUlDlClient::MaxVolumeL, exit with\
MaxVolume", aMaxVolume )
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
	__SUBCONTROLLER_INT1( "CMccUlDlClient linkid", aLinkId )
	__SUBCONTROLLER_INT1( "CMccUlDlClient streamid", aStreamId )
    TInt link = FindLinkL( aLinkId );
    iClientArray[link]->OpenL( aStreamType, aFourCC,
                               aDataSource, aSink,
                               aStreamId, aSettings );    
	__SUBCONTROLLER( "CMccUlDlClient::OpenL, exit with id" )
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
	__SUBCONTROLLER_INT1( "CMccUlDlClient linkid",  aLinkId )
	__SUBCONTROLLER_INT1( "CMccUlDlClient streamId",  aStreamId )
	__SUBCONTROLLER_INT1( "CMccUlDlClient endpointId",  aEndpointId )
    TInt link = FindLinkL( aLinkId );
    iClientArray[link]->PrepareL( aStreamId, aEndpointId );

	__SUBCONTROLLER( "CMccUlDlClient::PrepareL, exit" )
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
	__SUBCONTROLLER_INT1( "CMccUlDlClient linkid",  aLinkId )
	__SUBCONTROLLER_INT1( "CMccUlDlClient streamId",  aStreamId )
	__SUBCONTROLLER_INT1( "CMccUlDlClient endpointId",  aEndpointId )
    TInt link = FindLinkL( aLinkId );
    iClientArray[link]->PlayL( aStreamId, aEndpointId, aStreamPaused, aEnableRTCP );

	__SUBCONTROLLER( "CMccUlDlClient::PlayL, exit" )
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
	__SUBCONTROLLER_INT1( "CMccUlDlClient linkid",  aLinkId )
	__SUBCONTROLLER_INT1( "CMccUlDlClient streamId",  aStreamId )
	__SUBCONTROLLER_INT1( "CMccUlDlClient endpointId",  aEndpointId )
    TInt link = FindLinkL( aLinkId );
    iClientArray[link]->PauseL( aStreamId, aEndpointId, aEnableRTCP );

	__SUBCONTROLLER( "CMccUlDlClient::PauseL, exit" )
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
	__SUBCONTROLLER_INT1( "CMccUlDlClient linkid",  aLinkId )
	__SUBCONTROLLER_INT1( "CMccUlDlClient streamId",  aStreamId )
	__SUBCONTROLLER_INT1( "CMccUlDlClient endpointId",  aEndpointId )
    TInt link = FindLinkL( aLinkId );
    iClientArray[link]->StopL( aStreamId, aEndpointId );
	__SUBCONTROLLER( "CMccUlDlClient::StopL, exit" )
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
	__SUBCONTROLLER_INT1( "CMccUlDlClient linkid",  aLinkId )
	__SUBCONTROLLER_INT1( "CMccUlDlClient streamId",  aStreamId )
    TInt linkId = FindLinkL( aLinkId );
    iClientArray[linkId]->CloseL( aStreamId );
	__SUBCONTROLLER( "CMccUlDlClient::CloseL, exit" )
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
	__SUBCONTROLLER_INT1( "CMccUlDlClient linkid",  aLinkId )
	__SUBCONTROLLER_INT1( "CMccUlDlClient streamId",  aStreamId )
	__SUBCONTROLLER_INT1( "CMccUlDlClient endpointId",  aEndpointId )
    TInt link = FindLinkL( aLinkId );
    iClientArray[link]->ResumeL( aStreamId, aEndpointId, aEnableRTCP );

	__SUBCONTROLLER( "CMccUlDlClient::ResumeL, exit" )
    }    

// -----------------------------------------------------------------------------
// CMccUlDlClient::ResumeL
// Resumes ul/dl stream
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CMccUlDlClient::StreamsExistsL( const TUint32 aLinkId )
    {
	__SUBCONTROLLER( "CMccUlDlClient::StreamsExistsL" )
	__SUBCONTROLLER_INT1( "CMccUlDlClient linkid",  aLinkId )
    TInt link = FindLinkL( aLinkId );
    return iClientArray[link]->StreamsExistsL();
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
	__SUBCONTROLLER_INT1( "CMccUlDlClient linkid",  aLinkId )
	__SUBCONTROLLER_INT1( "CMccUlDlClient streamId",  aStreamId )
    TInt link = FindLinkL( aLinkId );
    aGain = iClientArray[link]->GetGainL( aStreamId );
	__SUBCONTROLLER( "CMccUlDlClient::GetGainL, exit" )
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
	__SUBCONTROLLER_INT1( "CMccUlDlClient::GetCodecL linkid",  aLinkId )
	__SUBCONTROLLER_INT1( "CMccUlDlClient::GetCodecL streamId",  aStreamId )
    
    const TInt link = FindLinkL( aLinkId );
    iClientArray[link]->CodecInformationL( aStreamId, aCodec );
    
	__SUBCONTROLLER( "CMccUlDlClient::GetCodecL exit" )
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
	__SUBCONTROLLER_INT1( "CMccUlDlClient linkid",  aLinkId )
	__SUBCONTROLLER_INT1( "CMccUlDlClient streamId",  aStreamId )
    TInt link = this->FindLinkL( aLinkId );
    iClientArray[link]->GetFmtpAttrL( aStreamId, aFmtp );
	__SUBCONTROLLER( "CMccUlDlClient::GetFmtpAttrL, exit" )
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
	__SUBCONTROLLER_INT1( "CMccUlDlClient linkid",  aLinkId )
	__SUBCONTROLLER_INT1( "CMccUlDlClient streamId",  aStreamId )
    TInt link = FindLinkL( aLinkId );
    iClientArray[link]->GetBalanceL( aStreamId, aLeftBalance, aRightBalance );
	__SUBCONTROLLER( "CMccUlDlClient::GetPlayBalanceL, exit" )
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
	__SUBCONTROLLER_INT1( "CMccUlDlClient linkid",  aLinkId )
	__SUBCONTROLLER_INT1( "CMccUlDlClient streamId",  aStreamId )
    TInt link = FindLinkL( aLinkId );
    iClientArray[link]->GetBalanceL( aStreamId, aLeftBalance, aRightBalance );
	__SUBCONTROLLER( "CMccUlDlClient::GetRecordBalanceL, exit" )
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
	__SUBCONTROLLER_INT1( "CMccUlDlClient linkid",  aLinkId )
	__SUBCONTROLLER_INT1( "CMccUlDlClient streamId",  aStreamId )
    TInt link = FindLinkL( aLinkId );
    iClientArray[link]->SetPrioritySettingsL( aStreamId, aPrioritySettings );
	__SUBCONTROLLER( "CMccUlDlClient::SetPriorityL, exit" )
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
    TInt indexCount = iClientArray.Count();      
    TInt err = KErrNone;
    TInt i = 0;
     for ( i = 0 ;  i < indexCount ; i++ )
        { 
        iClientArray[i]->SetVolumeL( aVolume );
        }
	__SUBCONTROLLER( "CMccUlDlClient::GetRecordBalanceL, exit" )
    return err;
    }

// -----------------------------------------------------------------------------
// CMccUlDlClient::SetGainL
// Sets the gain for uplink stream
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMccUlDlClient::SetGainL( TInt aGain )
    {
	__SUBCONTROLLER( "CMccUlDlClient::SetGainL" )
    TInt indexCount = iClientArray.Count();
    TInt err = KErrNone;
    TInt i = 0;
    for ( i = 0 ;  i < indexCount ; i++ )
        { 
        iClientArray[i]->SetGainL( aGain );
        }
	__SUBCONTROLLER( "CMccUlDlClient::SetGainL, exit" )
    return err;
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
	__SUBCONTROLLER_INT1( "CMccUlDlClient linkid",  aLinkId )
	__SUBCONTROLLER_INT1( "CMccUlDlClient streamId",  aStreamId )
    TInt link = FindLinkL( aLinkId );
    iClientArray[link]->SetBalanceL( aStreamId, aLeftBalance, aRightBalance );
	__SUBCONTROLLER( "CMccUlDlClient::SetRecordBalanceL, exit" )
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
	__SUBCONTROLLER_INT1( "CMccUlDlClient linkid",  aLinkId )
	__SUBCONTROLLER_INT1( "CMccUlDlClient streamId",  aStreamId )
    TInt link = FindLinkL( aLinkId );
    iClientArray[link]->SetBalanceL( aStreamId, aLeftBalance, aRightBalance );
	__SUBCONTROLLER( "CMccUlDlClient::SetPlayBalanceL, exit" )
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
	TInt err( KErrNone );
	
    switch ( aType )
        {
        case KMccLinkGeneral:
        case KMccLinkGeneralVideo:
            {
            CMccSymSubthreadClient* symClient = 
                CMccSymSubthreadClient::NewL( this, iMccResources, aType, iSessionId );

            err = StoreLink( symClient );
            if ( err == KErrNone )
                {
                aLinkId = reinterpret_cast<TUint32>( symClient );
                symClient->SetLinkId( aLinkId );
                }
            else
                {
                delete symClient;
                }
            break;
            }

        case KMccLinkLocal:
            {
            CMccSymSubthreadClient* symClient = 
                CMccSymSubthreadClient::NewL( this, iMccResources, aType, iSessionId );

            err = StoreLink( symClient );
            if ( err == KErrNone )
                {
                aLinkId = reinterpret_cast<TUint32>( symClient );
    			symClient->SetLinkId( aLinkId );
                }
            else
                {
                delete symClient;
                }
            break;
            }
            
        case KMccLinkSecure:
        case KMccLinkSecureVideo:
            {
            CMccSymSubthreadClient* symClient = 
                CMccSymSubthreadClient::NewL( this, iMccResources, aType, iSessionId );    
            
            err = StoreLink( symClient );
            if ( err == KErrNone )
                {
    			aLinkId = reinterpret_cast<TUint32>( symClient );
    			symClient->SetLinkId( aLinkId );
                }
            else
                {
                delete symClient;
                }
            break;
            }    
            
        default:
            err = KErrNotSupported;
            break;
        }

	User::LeaveIfError( err );

	__SUBCONTROLLER_INT1( "CMccUlDlClient::CreateLinkL, exit with id", aLinkId )
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

    TInt link = FindLinkL( aLinkId );
    iClientArray[link]->InitializeLinkL( aStatus, aIapId );
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
    
    TInt link = FindLinkL( aLinkId );
        
    iClientArray[link]->CreateRtpSessionL( aPort, 
                                           EFalse, 
                                           aIpTOS, 
                                           *iRtpMediaClock );
    }

// -----------------------------------------------------------------------------
// CMccUlDlClient::CloseLinkL
// Closes ul/dl client
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMccUlDlClient::CloseLinkL( TUint32 aLinkId )    
    {
	__SUBCONTROLLER( "CMccUlDlClient::CloseLinkL" )
	__SUBCONTROLLER_INT1( "CMccUlDlClient linkid",  aLinkId )
    TInt arrayId = FindLinkL( aLinkId );
    CMccSubThreadClientBase* temp = iClientArray[arrayId];    
    iClientArray[arrayId]->CloseL();
    iClientArray.Remove( arrayId );
    delete temp;

    // Notify the interface that an link has been closed
    TMccEvent event( iSessionId, aLinkId, KNullId, KNullId,
        KMccEventCategoryStream, KMccStreamClosed, KErrNone, KNullDesC8 );
    iMccEventHandler->SendMccEventToClient( event );

	__SUBCONTROLLER( "CMccUlDlClient::CloseLinkL, exit" )
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
	__SUBCONTROLLER_INT1( "CMccUlDlClient linkid",  aLinkId )
    TInt ind = FindLinkL( aLinkId );
    iClientArray[ind]->SetRemoteAddressL( aRemAddr );
	__SUBCONTROLLER( "CMccUlDlClient::SetRemoteAddressL, exit" )
    }
    
// -----------------------------------------------------------------------------
// CMccUlSubThreadClient::SetRemoteRtcpAddrL
// Sets the remote address for uplink stream
// -----------------------------------------------------------------------------
EXPORT_C void CMccUlDlClient::SetRemoteRtcpAddrL( TInetAddr aRemAddr,
                                                 TUint32 aLinkId ) 
    {
	__SUBCONTROLLER( "CMccUlDlClient::SetRemoteRtcpAddrL" )
	__SUBCONTROLLER_INT1( "CMccUlDlClient linkid",  aLinkId )
    TInt ind = FindLinkL( aLinkId );
    iClientArray[ind]->SetRemoteRtcpAddrL( aRemAddr );
	__SUBCONTROLLER( "CMccUlDlClient::SetRemoteRtcpAddrL, exit" )
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
    MccCodecConfigurator::GetCodecDefaultsL( aType, aParam, aCodec );
	__SUBCONTROLLER( "CMccUlDlClient::GetCodecDefaultsL, exit" )
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
    TInt link = FindLinkL( aEvent.iLinkId );
    iClientArray[link]->SendMediaSignalL( aEvent );
	__SUBCONTROLLER( "CMccUlDlClient::SendMediaSignalL, exit" )
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
    TInt link = this->FindLinkL( aLinkId );
    iClientArray[link]->SetCodecInformationL( aStreamId, aCodecInfo, aFmtp );
	__SUBCONTROLLER( "CMccUlDlClient::SetCodecInformationL, exit" )
    }
 
// -----------------------------------------------------------------------------
// CMccUlDlClient::GetSupportedBitratesL
// Get the supported bitrates from the codec of the specified stream
// -----------------------------------------------------------------------------
//
EXPORT_C void CMccUlDlClient::GetSupportedBitratesL( const TUint32 aLinkId, 
                                  const TUint32 /*aStreamId*/,
                                  RArray<TUint>& /*aBitrates*/ )
    {
	__SUBCONTROLLER( "CMccUlDlClient::GetSupportedBitratesL" )
    this->FindLinkL( aLinkId );
    User::Leave( KErrNotSupported );
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
	__SUBCONTROLLER_INT1( "CMccUlDlClient linkid",  aLinkId )
	__SUBCONTROLLER_INT1( "CMccUlDlClient streamId",  aStreamId )
	TInt link = this->FindLinkL( aLinkId );
	iClientArray[link]->GetSSRCL( aStreamId, aSSRCValue );
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
	__SUBCONTROLLER_INT1( "CMccUlDlClient linkid",  aLinkId )
	__SUBCONTROLLER_INT1( "CMccUlDlClient streamId",  aStreamId )
    TInt link = this->FindLinkL( aLinkId );
    iClientArray[link]->SendRTCPReceiverReportL( aStreamId );
	__SUBCONTROLLER( "CMccUlDlClient::SendRTCPReceiverReportL, exit" )
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
	__SUBCONTROLLER_INT1( "CMccUlDlClient linkid",  aLinkId )
	__SUBCONTROLLER_INT1( "CMccUlDlClient streamId",  aStreamId )
    TInt link = this->FindLinkL( aLinkId );
    iClientArray[link]->SendRTCPSenderReportL( aStreamId );
	__SUBCONTROLLER( "CMccUlDlClient::SendRTCPSenderReportL, exit" )
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
	__SUBCONTROLLER_INT1( "CMccUlDlClient linkid",  aLinkId )
	__SUBCONTROLLER_INT1( "CMccUlDlClient streamId",  aStreamId )
    TInt link = this->FindLinkL( aLinkId );
    iClientArray[link]->SendRTCPDataL( aStreamId, aData );
	__SUBCONTROLLER( "CMccUlDlClient::SendRTCPDataL, exit" )
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
	__SUBCONTROLLER_INT1( "CMccUlDlClient linkid",  aLinkId )
	__SUBCONTROLLER_INT1( "CMccUlDlClient streamId",  aStreamId )
    TInt link = this->FindLinkL( aLinkId );
    iClientArray[link]->StartInactivityTimerL( aStreamId, aTimeoutTime );
	__SUBCONTROLLER( "CMccUlDlClient::StartInactivityTimerL, exit" )
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
	__SUBCONTROLLER_INT1( "CMccUlDlClient linkid",  aLinkId )
	__SUBCONTROLLER_INT1( "CMccUlDlClient streamId",  aStreamId )
    TInt link = this->FindLinkL( aLinkId );
    iClientArray[link]->StopInactivityTimerL( aStreamId );
	__SUBCONTROLLER( "CMccUlDlClient::StopInactivityTimerL, exit" )
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
	__SUBCONTROLLER_INT1( "CMccUlDlClient linkid",  aLinkId )
	__SUBCONTROLLER_INT1( "CMccUlDlClient streamId",  aStreamId )
    TInt link = this->FindLinkL( aLinkId );
    iClientArray[ link ]->SetAudioRouteL( aStreamId, aRoutingDestination );
	__SUBCONTROLLER( "CMccUlDlClient::StopInactivityTimerL, exit" )
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
	__SUBCONTROLLER_INT1( "CMccUlDlClient linkid",  aLinkId )
	__SUBCONTROLLER_INT1( "CMccUlDlClient streamId",  aStreamId )
    TInt link = this->FindLinkL( aLinkId );
    iClientArray[ link ]->GetAudioRouteL( aStreamId, aRoutingDestination );
	__SUBCONTROLLER( "CMccUlDlClient::StopInactivityTimerL, exit" )
    }
      
// -----------------------------------------------------------------------------
// CMccUlDlClient::GenerateStreamId
// Generates a new stream identifier
// -----------------------------------------------------------------------------
//
EXPORT_C TUint32 CMccUlDlClient::GenerateStreamId()
    {
	__SUBCONTROLLER( "CMccUlDlClient::GenerateStreamId" )
    TUint32 id = 0;
    
    while( id == 0 )
        {
        id = static_cast<TUint32>( Math::Random() );
        }
    
	__SUBCONTROLLER_INT1( "CMccUlDlClient::GenerateStreamId, exit with id", id )
    return id;
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
	__SUBCONTROLLER_INT1( "CMccUlDlClient linkid",  aLinkId )
	__SUBCONTROLLER_INT1( "CMccUlDlClient streamId",  aStreamId )
    TInt linkId = FindLinkL( aLinkId );
    iClientArray[linkId]->UnuseL( aStreamId );
	__SUBCONTROLLER( "CMccUlDlClient::UnuseL, exit" )
    }
    
// -----------------------------------------------------------------------------
// CMccUlDlClient::GetLocalIpAddressesL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMccUlDlClient::GetLocalIpAddressesL( 
	TMccCreateLink& aClientData )
	{
	__SUBCONTROLLER( "CMccUlDlClient::GetLocalIpAddressesL" )
    
    TInt linkId = FindLinkL( aClientData.iLinkID );
    iClientArray[linkId]->GetLocalIpAddressesL( aClientData );

	__SUBCONTROLLER( "CMccUlDlClient::GetLocalIpAddressesL, exit" )
	}


// -----------------------------------------------------------------------------
// CMccUlDlClient::SendMccEventToClient
// -----------------------------------------------------------------------------
//
TInt CMccUlDlClient::SendMccEventToClient( TMccEvent& aEvent )
    {
	__SUBCONTROLLER( "CMccUlDlClient::SendMccEventToClient" )
	
	aEvent.iSessionId = iSessionId;
	
	return iMccEventHandler->SendMccEventToClient( aEvent );
    }

// -----------------------------------------------------------------------------
// CMccUlDlClient::StateChange
// -----------------------------------------------------------------------------
//
void CMccUlDlClient::StateChange( TInt aState, TUint32 aLinkId )
	{	
	__SUBCONTROLLER( "CMccUlDlClient::StateChange" )	
	
	iMccEventHandler->StateChange( aState, aLinkId );
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
	
	// Probably caller don't know the session id, so fill it here
	iMccEventHandler->ErrorOccured( aError, 
	                                iSessionId, 
	                                aLinkId, 
	                                aStreamId, 
	                                aEndpointId );
	}
                                          
// -----------------------------------------------------------------------------
// CMccUlDlClient::FindLinkL
// finds correct link to handle
// -----------------------------------------------------------------------------
//
TInt CMccUlDlClient::FindLinkL( const TUint32 aLinkId )
    {
	__SUBCONTROLLER_INT1( "CMccUlDlClient::FindLinkL, linkid", aLinkId )
    TInt err = KErrNotFound;
    TInt index = 0;
    TBool flagFound = EFalse;
    
    for ( TInt i = 0;  i < iClientArray.Count() && flagFound == EFalse ; i++ )
        {
        TUint32 linkId = reinterpret_cast<TUint32>(iClientArray[i]);  
       
        if( linkId == aLinkId )
        	{
        	index = i;
            err = KErrNone;
            flagFound = ETrue;	
	        }
        }

    if ( KErrNotFound == err )
        {
        __SUBCONTROLLER( "CMccUlDlClient::FindLinkL, Link not found" )
        User::Leave( err );
        }
    
	__SUBCONTROLLER( "CMccUlDlClient::FindLinkL, exit" )
    return index;
    }

// -----------------------------------------------------------------------------
// CMccUlDlClient::StoreDlStream()
// stores the pointer to ul/dl stream
// -----------------------------------------------------------------------------
//
TInt CMccUlDlClient::StoreLink( CMccSubThreadClientBase* aClient )
    {
	__SUBCONTROLLER( "CMccUlDlClient::StoreLink" )
    TInt err = KErrNone;
    err = iClientArray.Append( aClient ); 
	__SUBCONTROLLER_INT1( "CMccUlDlClient::StoreLink, exit with", err )
    return err;
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
    
    TInt link = FindLinkL( aLinkId );

    iClientArray[link]->BindContextIntoStreamL( aStreamId, aEndpointId, aCryptoContext );
    
    __SUBCONTROLLER( "CMccUlDlClient::BindContextIntoStreamL, EXIT ")
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
    
    TInt link = FindLinkL( aLinkId );

    iClientArray[link]->RemoveContextL( aStreamId, aEndpointId );
    
    __SUBCONTROLLER( "CMccUlDlClient::RemoveContextL, EXIT ")
    }

// -----------------------------------------------------------------------------
// CMccUlDlClient::SetParameterL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMccUlDlClient::SetParameterL( 
    TUint32 aParam,
    TUint32 aLinkId,
    TUint32 aStreamId,
    TUint32 aEndpointId,
    const TDesC8& aVal )
    {
    __SUBCONTROLLER( "CMccUlDlClient::SetParameterL" )
	__SUBCONTROLLER_INT1( "CMccUlDlClient linkid",  aLinkId )
	__SUBCONTROLLER_INT1( "CMccUlDlClient streamId",  aStreamId )
    TInt linkId = FindLinkL( aLinkId );
    iClientArray[linkId]->SetParameterL( aParam, aStreamId, aEndpointId, aVal );
	__SUBCONTROLLER( "CMccUlDlClient::SetParameterL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccUlDlClient::GetParameterL
// -----------------------------------------------------------------------------
//         
EXPORT_C void GetParameterL( 
    TUint32 /*aParam*/,
    TUint32 /*aLinkId*/,
    TUint32 /*aStreamId*/,
    TUint32 /*aEndpointId*/,
    TDesC8& /*aVal*/ )
    {
    __SUBCONTROLLER( "CMccUlDlClient::GetParameterL" )
    }
       
// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File
