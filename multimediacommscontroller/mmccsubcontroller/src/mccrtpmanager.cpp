/*
* Copyright (c) 2004-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    RTP Manager
*
*/




// INCLUDE FILES
#include <srtpsession.h>
#include "mccrtpmanager.h"
#include "mccinternalcodecs.h"
#include "mccrtpdatasink.h"
#include "mccrtpdatasource.h"
#include "mmccevents.h"
#include "mccsubcontrollerlogs.h"
#include "mccrtcpreceiver.h"
#include "mccrtpkeepalivecontainer.h"
#include "mccresources.h"
#include "mccqoshandler.h"
#include "mccinternaldef.h"

// LOCAL CONSTANTS AND MACROS

// Default IP type-of-service value.
const TInt KDefaultIpTOS = 184;

// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccRtpManager::CMccRtpManager
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMccRtpManager::CMccRtpManager( 
    MAsyncEventHandler& aHandler, 
    MMccResources& aMccResources,
    TUint32 aMccSessionId ) : 
    iRtpSessionId( KNullId ), 
    iStreamArray( RArray<TRtpId>() ), 
    iEventHandler( aHandler ),
    iMccResources( aMccResources ),
    iIpTOS( KDefaultIpTOS ), 
    iLocalPort( KDefaultRtpPort ),
    iMccSessionId( aMccSessionId ),
    iDummyRcvStreamId( KNullId )
    {

    }

// -----------------------------------------------------------------------------
// CMccRtpManager::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMccRtpManager::ConstructL()
    {
    __SUBCONTROLLER( "CMccRtpManager::ConstructL" )

    iRtpApi = CRtpAPI::NewL( *this );
    iRtcpReceiver = CMccRtcpReceiver::NewL( this, iEventHandler, iMccResources, *iRtpApi );
    iQosHandler = CMccQosHandler::NewL( *this );
    
    __SUBCONTROLLER( "CMccRtpManager::ConstructL exit" )
    }

// -----------------------------------------------------------------------------
// CMccRtpManager::NewL
// Static constructor.
// -----------------------------------------------------------------------------
//
CMccRtpManager* CMccRtpManager::NewL( 
    MAsyncEventHandler& aHandler,
    MMccResources& aMccResources,
    TUint32 aMccSessionId
    )
    {
    CMccRtpManager* self = new (ELeave) CMccRtpManager( aHandler, aMccResources, 
                                                        aMccSessionId );
    CleanupStack::PushL( self );      
    self->ConstructL();           
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMccRtpManager::~CMccRtpManager
// Destructor
// -----------------------------------------------------------------------------
//
CMccRtpManager::~CMccRtpManager()
    {
    __SUBCONTROLLER( "CMccRtpManager::~CMccRtpManager" )
    
    delete iRtcpReceiver;
    delete iRtpKeepaliveMechanism;
    
    if ( KNullId != iDummyRcvStreamId && iRtpApi )
        {
        iRtpApi->CloseStream( iDummyRcvStreamId );
        }
            
    if( KNullId != iRtpSessionId && iRtpApi )
        {
        iRtpApi->CloseSession( iRtpSessionId );
        iRtpApi->Close();
        }
    
    delete iSecSession;
    delete iRtpApi;
    delete iQosHandler;
    
    iStreamArray.Close();
    
    delete iCName;
    __SUBCONTROLLER( "CMccRtpManager::~CMccRtpManager, exit" )
    }

// -----------------------------------------------------------------------------
// CMccRtpManager::CreateSessionL
// Creates RTP session and adds session to SessionArray
// -----------------------------------------------------------------------------
//
void CMccRtpManager::CreateSessionL( 
    TUint& aPort, 
    TBool aEnableRTCP,
    TBool aSecurity, 
    TBool aIncreaseDefaultSocketSize )
    {
    __SUBCONTROLLER( "CMccRtpManager::CreateSessionL" )
    
    if( KNullId != iRtpSessionId )
        {
        User::Leave( KErrAlreadyExists );
        }

    TCreateSessionParams params;    
    params.iPriority = TCreateSessionParams::EPriorityStandard;    
    aEnableRTCP = ETrue;
    
    if ( aIncreaseDefaultSocketSize )
        {
        params.iSocketBufSize = KMccIncreasedRtpSocketSize;
        }
    __SUBCONTROLLER_INT1( "CMccRtpManager::CreateSessionL, socket size:", 
                          params.iSocketBufSize )
    
    TInetAddr localAddress = iRtpApi->GetLocalIPAddressL();
    
    if ( aSecurity )
        {
        iSecSession = CSRTPSession::NewL( localAddress );
        iRtpSessionId = iRtpApi->CreateSessionL( params, aPort, aEnableRTCP,
                                                 NULL, *iSecSession );
        }
    else
        {
        iRtpSessionId = iRtpApi->CreateSessionL( params, aPort, aEnableRTCP, NULL );
        }
        
    if( KNullId == iRtpSessionId )
        {
        User::Leave( KErrCouldNotConnect );
        }
    
    __ASSERT_ALWAYS( !iRtpKeepaliveMechanism, User::Leave( KErrAlreadyExists ) );     
    iRtpKeepaliveMechanism = 
        CMccRtpKeepaliveContainer::NewL( iEventHandler, 
                                         *iRtpApi, 
                                         iRtpSessionId );
    
    iLocalPort = aPort;
    iRtcpEnabled = aEnableRTCP;
    
    __SUBCONTROLLER( "CMccRtpManager::CreateSessionL, keepalive initialized" )
    
    iRtcpReceiver->HandleReceiving( iRtpSessionId, iRtcpEnabled );
    
    __SUBCONTROLLER( "CMccRtpManager::CreateSessionL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccRtpManager::CloseSession
// Closes RTP session
// -----------------------------------------------------------------------------
//
void CMccRtpManager::CloseSession()
    {
    __SUBCONTROLLER( "CMccRtpManager::CloseSession" )
    if ( KNullId != iRtpSessionId ) 
        {
        if ( iRtpKeepaliveMechanism )
            {
            iRtpKeepaliveMechanism->StopAll();
            }
        
        iRtpApi->CloseSession( iRtpSessionId );
        iRtpApi->Close();
        iStreamArray.Reset();
        
        iRtpSessionId = KNullId;
        
        if ( iQosHandler->QoSEnabled() )
            {
            iQosHandler->DisableQoS();
            }
        }
    __SUBCONTROLLER( "CMccRtpManager::CloseSession, exit" )
    }

// -----------------------------------------------------------------------------
// CMccRtpManager::GetRtpReference
// Returns RTPApi reference
// -----------------------------------------------------------------------------
//
CRtpAPI* CMccRtpManager::GetRtpReferenceL( )
    {
    if ( !IsOpen() ) 
        {
        User::Leave( KErrNotFound );
        }
    return iRtpApi;
    }

// -----------------------------------------------------------------------------
// CMccRtpManager::SetRemoteAddress
// Sets RTP Remote Address
// -----------------------------------------------------------------------------
//
TInt CMccRtpManager::SetRemoteAddress( const TInetAddr& aRemoteAddress )
    {
    __SUBCONTROLLER( "CMccRtpManager::SetRemoteAddress" )
    TInt err( KErrNotFound );
    if ( KNullId != iRtpSessionId ) 
        {   
        // Do only if address has changed
        err =( iRemoteAddress.Address() == aRemoteAddress.Address() && 
               iRemoteAddress.Port() == aRemoteAddress.Port() ) ? KErrNone : err;
        if ( err )
            { 
            __SUBCONTROLLER_INT1( "CMccRtpManager::SetRemoteAddress, setting addr:", 
                                  aRemoteAddress.Address() )
            __SUBCONTROLLER_INT1( "CMccRtpManager::SetRemoteAddress, setting port:", 
                                  aRemoteAddress.Port() )
            
            err = iRtpApi->SetRemoteAddress( iRtpSessionId, aRemoteAddress );
            if ( !err )
                {
                iRemoteAddress.SetAddress( aRemoteAddress.Address() );
                iRemoteAddress.SetPort( aRemoteAddress.Port() );
                if ( iRtpKeepaliveMechanism )
                    {
                    err = iRtpKeepaliveMechanism->RemoteAddressSet();
                    }
                }
            }
        }
    __SUBCONTROLLER_INT1( "CMccRtpManager::SetRemoteAddress, exit with err", err )
    return err;
    }

// -----------------------------------------------------------------------------
// CMccRtpManager::SetRemoteRtcpAddress
// Sets RTP Remote Address
// -----------------------------------------------------------------------------
//
TInt CMccRtpManager::SetRemoteRtcpAddr( const TInetAddr& aRemoteAddress )
    {
    __SUBCONTROLLER( "CMccRtpManager::SetRemoteRtcpAddress" )
    TInt err( KErrNotFound );
    if ( KNullId != iRtpSessionId ) 
        {   
        // Do only if address has changed
        err =( iRemoteRtcpAddr.Address() == aRemoteAddress.Address() && 
               iRemoteRtcpAddr.Port() == aRemoteAddress.Port() ) ? KErrNone : err;
        __SUBCONTROLLER_INT1( "Remote RtcpPort", aRemoteAddress.Port() )
        __SUBCONTROLLER_INT1( "Saved Remote RtcpPort", iRemoteAddress.Port() )
        if ( err )
            { 
            __SUBCONTROLLER_INT1( "CMccRtpManager::SetRemoteAddress, setting addr:", 
                                  aRemoteAddress.Address() )
            __SUBCONTROLLER_INT1( "CMccRtpManager::SetRemoteAddress, setting port:", 
                                  aRemoteAddress.Port() )
            
            err = iRtpApi->SetRemoteRtcpAddress( iRtpSessionId, aRemoteAddress );
            if ( !err )
                {
                iRemoteRtcpAddr.SetAddress( aRemoteAddress.Address() );
                iRemoteRtcpAddr.SetPort( aRemoteAddress.Port() );
                __SUBCONTROLLER_INT1( "Remote RtcpPort", aRemoteAddress.Port() )
                }
            }
        }
    __SUBCONTROLLER_INT1( "CMccRtpManager::SetRemoteAddress, exit with err", err )
    return err;
    }
// -----------------------------------------------------------------------------
// CMccRtpManager::SetIpTOS
// Sets TOS flag, must be called before StartSession
// -----------------------------------------------------------------------------
//
void CMccRtpManager::SetIpTOS( TInt aIpTOS ) 
    {
    iIpTOS = aIpTOS;
    }

// -----------------------------------------------------------------------------
// CMccRtpManager::StartSession
// Starts RTP session
// -----------------------------------------------------------------------------
//
void CMccRtpManager::StartSessionL()
    {
    __SUBCONTROLLER( "CMccRtpManager::StartSessionL" )
    if ( KNullId != iRtpSessionId ) 
        {
        __SUBCONTROLLER( "CMccRtpManager::StartSessionL, session exists" )
        
		RSocket* socketPtr = iRtpApi->GetRtpSocket( iRtpSessionId );
		
		if( !socketPtr )
		    {
		    __SUBCONTROLLER( "CMccRtpManager::StartSessionL, no socket!" )
            #ifdef TEST_EUNIT            
            // The stub for RTP API returns a NULL socket pointer. Just pretend
            // everything is fine.
            return;            
            #else
		    User::Leave( KErrNotReady );
            #endif
		    }
		else
		    {
            User::LeaveIfError( iRtpApi->StartSession( iRtpSessionId ) );      
            
            __SUBCONTROLLER_INT1( "CMccRtpManager::StartSessionL, iptos:", iIpTOS )
             
            User::LeaveIfError( socketPtr->SetOpt( KSoIpTOS, KSolInetIp, iIpTOS ) );
            User::LeaveIfError( socketPtr->SetOpt( KSoUdpReceiveICMPError, KSolInetUdp, 1 ) );
            
            if ( !iQosHandler->QoSEnabled() )
                {
                // These has to be set before enabling QoS
                iQosHandler->SetSubConnectionParams( iConnection,
                                             iSocketServ,
                                             *socketPtr );
                                                
                // If we cannot enable QoS for some reason, we should not
                // consider it as a fatal error. If error happens, then
                // disable qos. This will clean up some unneeded resources.
                // Also this leads to retries when streams are created.
                TRAPD( err, iQosHandler->EnableQosL() );
                if ( KErrNone != err )
                    {
                    __SUBCONTROLLER_INT1( "CMccRtpManager::StartSessionL QOS:", err )
            
                    iQosHandler->DisableQoS();
                    }
                }
		    }
        }
    else
        {
        User::Leave( KErrNotFound );
        }
    
    __SUBCONTROLLER( "CMccRtpManager::StartSessionL, exit" ) 
    }

// -----------------------------------------------------------------------------
// CMccRtpManager::Initialize
// Opens RTPApi and starts connection
// -----------------------------------------------------------------------------
//
void CMccRtpManager::InitializeL( TRequestStatus& aStatus, TInt aIapId )
    {
    TInt err = iRtpApi->OpenL( iRtpSdes, NULL, &iSocketServ, &iConnection );
    if ( KErrNone == err ) 
        {
        User::LeaveIfError( iRtpApi->StartConnection( aStatus, aIapId ) );
        }
    else
        {
        User::Leave( err );
        }
    }

// -----------------------------------------------------------------------------
// CMccRtpManager::CheckStreamExists
// Checks that stream exists
// -----------------------------------------------------------------------------
//
TBool CMccRtpManager::CheckStreamExists( const TRtpId& aStreamId ) const
    {
    TInt index = iStreamArray.Find( aStreamId );
    
    if( KErrNotFound == index )
        {
        return EFalse;
        }
    else
        {
        return ETrue;
        }
    }

// -----------------------------------------------------------------------------
// CMccRtpManager::IsOpen
// Checks that RTPApi is opened
// -----------------------------------------------------------------------------
//
TBool CMccRtpManager::IsOpen() const
    {
    if ( iRtpApi->GetRtpSocket( iRtpSessionId ) ) 
        {
        return ETrue;
        }
    else 
        {
        return EFalse;
        }
    }

// -----------------------------------------------------------------------------
// CMccRtpManager::RemoveStreamFromArray
// Removes RTP stream from stream array
// -----------------------------------------------------------------------------
//
void CMccRtpManager::RemoveStreamFromArray( const TRtpId& aStreamId )
    {
    const TInt index( iStreamArray.Find( aStreamId ) );
    if( KErrNotFound != index )
        {
        iStreamArray.Remove( index );
        }
    }

// -----------------------------------------------------------------------------
// CMccRtpManager::SessionId
// Returns the Session ID for this object
// -----------------------------------------------------------------------------
//
TRtpId CMccRtpManager::SessionId() const
    {
    return iRtpSessionId;
    }

// -----------------------------------------------------------------------------
// CMccRtpManager::ErrorNotify
// Error callback.
// -----------------------------------------------------------------------------
//
void CMccRtpManager::ErrorNotify( TInt aErrCode )
    {
    if ( KErrNone != aErrCode )
        {
        __SUBCONTROLLER_INT1( "CMccRtpManager::ErrorNotify, error", aErrCode )
        
        // Report all errors coming from here as rtp category stream errors
        // In HW when remote end drops from network, this function will be
        // called with KErrHostUnreach (-191) and in some cases this will return
        // as KErrGeneral with DTMF's. KErrCouldNotConnect error is received in
        // ICMP port unreachable situation. In these cases, handle just the 
        // KErrHostUnreach and KErrCouldNotConnect.
        if ( KErrHostUnreach == aErrCode || KErrCouldNotConnect == aErrCode )
            {
            TMccEvent event;
            event.iErrorCode = KErrHostUnreach;
            event.iEventCategory = KMccEventCategoryRtp;
            event.iEventType = KMccStreamError;
            
            // Use whatever internal uid
            TMccInternalEvent internalEvent( KMccRtpSourceUid, 
                                     EMccInternalEventNone,
                                     event );
            
            iEventHandler.SendEventToClient( internalEvent );
            }
        }
    }

// -----------------------------------------------------------------------------
// CMccRtpManager::CreateTransmitStreamL
// Creates a new transmit stream based on given parameters
// -----------------------------------------------------------------------------
//
TUint32 CMccRtpManager::CreateTransmitStreamL( MDataSink& aSink, 
    const TMccCodecInfo& aCodecInfo )
    {
    TUint32 streamId( 0 );

    if( aCodecInfo.iPayloadType < KMccPayloadTypeMax )
        {
        CMccRtpDataSink& dSink = static_cast<CMccRtpDataSink&>( aSink );
        TMccRtpSessionParams sessParams( iRtpApi, 
                                         iRtpSessionId, 
                                         iRtcpEnabled, 
                                         iRtpKeepaliveMechanism,
                                         iSecSession );
        
        dSink.SetSessionParamsL( sessParams );
        
        TMccCodecInfoBuffer infoBuffer( aCodecInfo );
        dSink.ConfigureL( infoBuffer, NULL );
        
        UpdateL( &aCodecInfo );
        }
    else
        {
        User::Leave( KErrArgument );
        }
    return streamId;
    }

// -----------------------------------------------------------------------------
// CMccRtpManager::CreateReceiveStreamL
// Creates a new receive stream based on given parameters
// -----------------------------------------------------------------------------
//
TUint32 CMccRtpManager::CreateReceiveStreamL( MDataSource& aSource, 
    const TMccCodecInfo& aCodecInfo )
    {
    TUint32 streamId( 0 );

    if( aCodecInfo.iPayloadType < KMccPayloadTypeMax )
        {
        CMccRtpDataSource& dSource = static_cast<CMccRtpDataSource&>( aSource );
        TMccRtpSessionParams sessParams( iRtpApi, 
                                         iRtpSessionId, 
                                         iRtcpEnabled,
                                         iRtpKeepaliveMechanism,
                                         iSecSession );
        
        dSource.SetSessionParamsL( sessParams );
        
        TMccCodecInfoBuffer infoBuffer( aCodecInfo );
        dSource.ConfigureL( infoBuffer, NULL );
        
        UpdateL( &aCodecInfo );
        }
    else
        {
        User::Leave( KErrArgument );
        }

    return streamId;
    }

// -----------------------------------------------------------------------------
// CMccRtpManager::SetRTCPSendReceive
// Enables/disables automatic transmission and receiving of RTCP packets
// in a session.
// -----------------------------------------------------------------------------
//
TInt CMccRtpManager::SetRTCPSendReceive( TBool aEnableRtcp )
    {
    __SUBCONTROLLER( "CMccRtpManager::SetRTCPSendReceive" )
    
    // Check that session exists
    TInt err( KErrNotFound );
    if ( iRtpSessionId != KNullId ) 
        {    
        __SUBCONTROLLER_INT1( "CMccRtpManager::SetRTCPSendReceive, enable:", aEnableRtcp )
        
        iRtcpEnabled = aEnableRtcp;
        
        err = iRtcpReceiver->HandleReceiving( iRtpSessionId, iRtcpEnabled );
        if ( !err )
            {
            __SUBCONTROLLER( "CMccRtpManager::SetRTCPSendReceive, calling rtp api" )
            err = iRtpApi->SuspendRtcpSending( iRtpSessionId, aEnableRtcp );
            }
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CMccRtpManager::SetLinkId
// -----------------------------------------------------------------------------
//
void CMccRtpManager::SetLinkId( TUint32 aLinkId )
    {
    iLinkId = aLinkId;
    }
    
// -----------------------------------------------------------------------------
// CMccRtpManager::UpdateL
// Creates dummy receive stream if rtp session does not have any receive
// stream (RTP source) or removes the dummy receive stream if RTP source
// exists.
// -----------------------------------------------------------------------------
//    
void CMccRtpManager::UpdateL( const TMccCodecInfo* aCodecInfo )
    {
    __SUBCONTROLLER( "CMccRtpManager::UpdateL" )
    
    MDataSource* source = 0;
    MDataSink* sink = 0;
    TInt err = 
        iMccResources.FindResource( iLinkId, 0, KMccRtpSourceUid, &source, &sink );
        
    if ( !err && source )
        {
        if ( iDummyRcvStreamId != KNullId )
            {
            __SUBCONTROLLER( "CMccRtpManager::UpdateL, dummy rcv stream not needed" )
            
            iRtpApi->CloseStream( iDummyRcvStreamId );
            iDummyRcvStreamId = KNullId;
            }
        }
    else
        {
        if ( iDummyRcvStreamId == KNullId && aCodecInfo )
            {
            __SUBCONTROLLER( "CMccRtpManager::UpdateL, dummy rcv stream needed" )
            
            TRcvStreamParams rcvParams;
            rcvParams.iPayloadType = aCodecInfo->iPayloadType;
            iDummyRcvStreamId = iRtpApi->CreateReceiveStreamL( iRtpSessionId, rcvParams );
            if ( KNullId == iDummyRcvStreamId )
                {
                __SUBCONTROLLER( "CMccRtpDataSource::UpdateL, could not create dummy rcv stream" )
                
                User::Leave( KErrCouldNotConnect );
                }
            }
        }
        
    __SUBCONTROLLER( "CMccRtpManager::UpdateL, exit" )
    }
    
// -----------------------------------------------------------------------------
// CMccRtpManager::GetLocalIpAddressesL
// -----------------------------------------------------------------------------
//
void CMccRtpManager::GetLocalIpAddressesL( TMccCreateLink& aClientData )
    {
    __SUBCONTROLLER( "CMccRtpManager::GetLocalIpAddressesL" )
    
    __ASSERT_ALWAYS( KNullId != iRtpSessionId && iRtpApi, User::Leave( KErrNotReady ) );
    
    RSocket* rtpSocket( iRtpApi->GetRtpSocket( iRtpSessionId ) );
    RSocket* rtcpSocket( iRtpApi->GetRtcpSocket( iRtpSessionId ) );
    TInetAddr localAddr( iRtpApi->GetLocalIPAddressL() );
    
    if ( rtpSocket )
        {
        if ( KAfInet6 == localAddr.Family() )
            {
            aClientData.iLocalAddress.SetAddress( localAddr.Ip6Address() );
            }
        else
            {
            aClientData.iLocalAddress.SetAddress( localAddr.Address() );
            }
        
        aClientData.iLocalAddress.SetPort( rtpSocket->LocalPort() );
        }
    else
        {
        User::Leave( KErrGeneral );
        }
    
    if ( iRtcpReceiver && rtcpSocket )
        {
        if ( KAfInet6 == localAddr.Family() )
            {
            aClientData.iLocalRtcpAddress.SetAddress( localAddr.Ip6Address() );
            }
        else
            {
            aClientData.iLocalRtcpAddress.SetAddress( localAddr.Address() );
            }
        
        aClientData.iLocalRtcpAddress.SetPort( rtcpSocket->LocalPort() );
        }
    
    __SUBCONTROLLER( "CMccRtpManager::GetLocalIpAddressesL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccRtpManager::SetCNameL
// -----------------------------------------------------------------------------
//
void CMccRtpManager::SetCNameL( const TDesC8& aCName )
    {
    HBufC8* cname = aCName.AllocLC();
    delete iCName;
    iCName = cname;
    CleanupStack::Pop( cname );
        
    iRtpSdes.iCName.Set( *iCName );
        
    iRtpApi->SetLocalSdes( iRtpSdes );
    }

// -----------------------------------------------------------------------------
// CMccRtpManager::MccSessionId
// -----------------------------------------------------------------------------
//
TUint32 CMccRtpManager::MccSessionId() const
    {
    return iMccSessionId;
    }

// -----------------------------------------------------------------------------
// CMccRtpManager::QoSParamEvent()
// From MMccQosEventObserver
// -----------------------------------------------------------------------------
//
void CMccRtpManager::QoSParamEvent( TInt aError, TUint32 /*aEvent*/ )
    {
    __SUBCONTROLLER_INT1( "CMccRtpManager::QoSParamEvent aError:", aError )
    
    aError = aError;
    }

// -----------------------------------------------------------------------------
// CMccRtpManager::QoSConnEvent()
// From MMccQosEventObserver
// -----------------------------------------------------------------------------
//
void CMccRtpManager::QoSConnEvent( TInt aError, TUint32 /*aEvent*/ )
    {
    __SUBCONTROLLER_INT1( "CMccRtpManager::QoSConnEvent aError:", aError )
    
    aError = aError;
    }

