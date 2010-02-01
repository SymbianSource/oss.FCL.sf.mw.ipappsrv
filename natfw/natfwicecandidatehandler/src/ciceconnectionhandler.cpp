/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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




#include <e32base.h>
#include "natfwcredentials.h"
#include "natfwcandidatepair.h"
#include "mnatfwpluginobserver.h"
#include "mncmconnectionmultiplexer.h"
#include "ciceconnectionhandler.h"
#include "natfwstunclient.h"
#include "natfwstunbinding.h"
#include "cicesessiondata.h"
#include "miceconncheckobserver.h"
#include "miceconnhandlerobserver.h"
#include "ticenatplugincontaineriter.h"
#include "icecandidatehandlerlogs.h"

// CONSTANTS
const TInt KRetransmitIntervalInMs = 250;

// ======== MEMBER FUNCTIONS ========


// ---------------------------------------------------------------------------
// CIceConnectionHandler::CIceConnectionHandler
// ---------------------------------------------------------------------------
//
CIceConnectionHandler::CIceConnectionHandler(
        MNcmConnectionMultiplexer& aMultiplexer,
        MIceNatPluginEventObs& aEventObserver,
        CIceSessionData& aSessionData,
        CIceNatPluginContainer& aPluginContainer,
        MIceConnHandlerObserver& aConnHandlerObs )
    :
    iMultiplexer( aMultiplexer ),
    iEventObserver( aEventObserver ),
    iSessionData( aSessionData ),
    iPluginContainer( aPluginContainer ),
    iConnHandlerObs( aConnHandlerObs )
    {
    __ICEDP( "CIceConnectionHandler::CIceConnectionHandler" )
    }


// ---------------------------------------------------------------------------
// CIceConnectionHandler::ConstructL
// ---------------------------------------------------------------------------
//
void CIceConnectionHandler::ConstructL()
    {
    __ICEDP( "CIceConnectionHandler::ConstructL" )
    
    iTimerServices = CDeltaTimer::NewL( CActive::EPriorityStandard );
    
    // RTO value given here will be overridden when sending request
    iStunClient = CSTUNClient::NewL( KRetransmitIntervalInMs,
                                     *iTimerServices,
                                     *this,
                                     &iMultiplexer );
    
    iPluginContainer.RegObserverForEventL( *this, 
        MIceNatPluginEventObs::ESendingActivated );
    iPluginContainer.RegObserverForEventL( *this, 
        MIceNatPluginEventObs::ESendingDeactivated );
        
    iPluginContainer.RegObserverForEventL( *this, 
        MIceNatPluginEventObs::EReceivingActivated );
    iPluginContainer.RegObserverForEventL( *this, 
        MIceNatPluginEventObs::EReceivingDeactivated );        
    }


// ---------------------------------------------------------------------------
// CIceConnectionHandler::NewL
// ---------------------------------------------------------------------------
//
CIceConnectionHandler* CIceConnectionHandler::NewL(
        MNcmConnectionMultiplexer& aMultiplexer,
        MIceNatPluginEventObs& aEventObserver,
        CIceSessionData& aSessionData,
        CIceNatPluginContainer& aPluginContainer,
        MIceConnHandlerObserver& aConnHandlerObs )
    {
    __ICEDP( "CIceConnectionHandler::NewL" )

    CIceConnectionHandler* self 
        = CIceConnectionHandler::NewLC( 
            aMultiplexer, aEventObserver, aSessionData, aPluginContainer,
            aConnHandlerObs );
            
    CleanupStack::Pop( self );

    return self;
    }


// ---------------------------------------------------------------------------
// CIceConnectionHandler::NewLC
// ---------------------------------------------------------------------------
//
CIceConnectionHandler* CIceConnectionHandler::NewLC(
        MNcmConnectionMultiplexer& aMultiplexer,
        MIceNatPluginEventObs& aEventObserver,
        CIceSessionData& aSessionData,
        CIceNatPluginContainer& aPluginContainer,
        MIceConnHandlerObserver& aConnHandlerObs )
    {
    __ICEDP( "CIceConnectionHandler::NewLC" )
    
    CIceConnectionHandler* self 
        = new( ELeave ) CIceConnectionHandler( 
            aMultiplexer, aEventObserver, aSessionData, 
            aPluginContainer, aConnHandlerObs );
    CleanupStack::PushL( self );
    self->ConstructL();
    
    return self;
    }


// ---------------------------------------------------------------------------
// CIceConnectionHandler::~CIceConnectionHandler
// ---------------------------------------------------------------------------
//
CIceConnectionHandler::~CIceConnectionHandler()
    {
    __ICEDP( "CIceConnectionHandler::~CIceConnectionHandler" )
    
    FreeResources();
    delete iStunClient;
    
    // timer services must be deleted AFTER STUN Client
    delete iTimerServices;
    
    iPluginContainer.UnregObserver( *this );
    }


// ---------------------------------------------------------------------------
// CIceConnectionHandler::SetReceivingStateL
// ---------------------------------------------------------------------------
//
void CIceConnectionHandler::SetReceivingStateL( 
        const CNATFWCandidate& aLocalCandidate,
        TNATFWStreamingState aState )
    {
    __ICEDP( "CIceConnectionHandler::SetReceivingStateL" )
    
    CNATFWPluginApi* item = SolvePluginL( aLocalCandidate );
    item->SetReceivingStateL( aLocalCandidate, aState );
    }


// ---------------------------------------------------------------------------
// CIceConnectionHandler::SetSendingStateL
// ---------------------------------------------------------------------------
//
void CIceConnectionHandler::SetSendingStateL( 
        const CNATFWCandidate& aLocalCandidate,
        TNATFWStreamingState aState, const TInetAddr& aDestAddr )
    {
    __ICEDP( "CIceConnectionHandler::SetSendingStateL" )
    
    CNATFWPluginApi* item = SolvePluginL( aLocalCandidate );
    item->SetSendingStateL( aLocalCandidate, aState, aDestAddr );
    }


// ---------------------------------------------------------------------------
// CIceConnectionHandler::GetConnectionIdL
// ---------------------------------------------------------------------------
//
void CIceConnectionHandler::GetConnectionIdL( 
        const CNATFWCandidate& aLocalCandidate,
        TUint& aConnectionId )
    {
    __ICEDP( "CIceConnectionHandler::GetConnectionIdL" )
    
    CNATFWPluginApi* item = SolvePluginL( aLocalCandidate );
    item->GetConnectionIdL( aLocalCandidate, aConnectionId );
    }


// ---------------------------------------------------------------------------
// CIceConnectionHandler::CreateConnectionL
// ---------------------------------------------------------------------------
//
TUint CIceConnectionHandler::CreateConnectionL( 
        const CNATFWCandidatePair& aPair )
    {
    __ICEDP( "CIceConnectionHandler::CreateConnectionL" )
    const CNATFWCandidate& localCandidate( aPair.LocalCandidate() );
    TUint streamId( localCandidate.StreamId() );
    
    iMultiplexer.RegisterConnectionObserverL( streamId, *this );
    iMultiplexer.RegisterIncomingConnectionObserverL( streamId, *this );
    
    TUint connectionId( 0 );
    if ( CNATFWCandidate::ERelay == localCandidate.Type() )
        {
        // turnconnectionid is used when utilizing TURN-plugin
        CNATFWPluginApi* plugin 
            = iPluginContainer.PluginByType( KNatPluginTypeTurn() );
        __ASSERT_ALWAYS( plugin, User::Leave( KErrNotFound ) );
        
        plugin->GetConnectionIdL( localCandidate, connectionId );
        }
    else
        {
        // create a new host connection for the candidate pair
        connectionId = iMultiplexer.CreateConnectionL( 
            streamId, localCandidate.TransportAddr() );
        }
    
    CSTUNBinding* binding = 
        CSTUNBinding::NewLC( *iStunClient, streamId, connectionId );
    
    TIceConnectionType type = 
        CNATFWCandidate::ERelay == aPair.LocalCandidate().Type() ?
            EIceConnTypeTurn : EIceConnTypeHost;
    
    TBindingData bindingData( binding, type,
        streamId, connectionId, aPair.RemoteCandidate().TransportAddr() );
    iBindings.AppendL( bindingData );
    CleanupStack::Pop( binding );
    
    return connectionId;
    }


// ---------------------------------------------------------------------------
// CIceConnectionHandler::RemoveConnection
// ---------------------------------------------------------------------------
//
void CIceConnectionHandler::RemoveConnection( 
        TUint aStreamId, TUint aConnectionId )
    {
    __ICEDP( "CIceConnectionHandler::RemoveConnection" )
    
    DoRemoveConnection( aStreamId, aConnectionId, ETrue );
    }


// ---------------------------------------------------------------------------
// CIceConnectionHandler::PerformConnCheckL
// ---------------------------------------------------------------------------
//
void CIceConnectionHandler::PerformConnCheckL( TUint aStreamId,
        TUint aConnectionId, TUint aPriority, TBool aUseCandidate,
        TUint aRtoValue, const TInetAddr& aDestAddr,
        MIceConnCheckObserver& aObserver )
    {
    __ICEDP( "CIceConnectionHandler::PerformConnCheckL" )
    
    const CNATFWCredentials* credential = 
        iSessionData.Credentials( 
            aStreamId, CNATFWCredentials::EOutbound );
    __ASSERT_ALWAYS( NULL != credential, User::Leave( KErrNotReady ) );
    __ASSERT_ALWAYS(
        credential->Username().Length(), User::Leave( KErrNotReady ) );
    __ASSERT_ALWAYS(
        credential->Password().Length(), User::Leave( KErrNotReady ) );
    
    iStunClient->SetCredentialsL( 
        credential->Username(), credential->Password() );
    
    TInt index( FindBindingIndex( aStreamId, aConnectionId, aDestAddr ) );
    __ASSERT_ALWAYS( KErrNotFound != index, User::Leave( index ) );
    iBindings[index].iObserver = &aObserver;
    
    TICEAttributes attributes( 
        aPriority, aUseCandidate, 
        ( EIceRoleControlled == iSessionData.Role() )
            ? iSessionData.TieBreaker() : TUint64( 0 ),
        ( EIceRoleControlling == iSessionData.Role() )
            ? iSessionData.TieBreaker() : TUint64( 0 ) );
    
    CSTUNBinding* stunBinding =
        static_cast<CSTUNBinding*>( iBindings[index].iBinding );
        
    stunBinding->SetICESpecificAttributes( attributes );
    iBindings[index].iCheckOngoing = ETrue;
    iBindings[index].iRtoValue = aRtoValue;
    
    if ( EStreamingStateActive != iBindings[index].iStreamingStatus )
        {
        if ( CIceConnectionHandler::EIceConnTypeTurn 
                != iBindings[index].iConnType )
            {
            iMultiplexer.SetReceivingStateL( iBindings[index].iStreamId,
                iBindings[index].iConnectionId, EStreamingStateActive );
            
            iMultiplexer.SetSendingStateL( iBindings[index].iStreamId, 
                iBindings[index].iConnectionId,
                iBindings[index].iDestAddr, EStreamingStateActive );
            // Wait for notify from multiplexer and after that call 
            // SendRequestL for this connection.
            }
        else
            {
            stunBinding->SendRequestL( 
                iBindings[index].iDestAddr, ETrue, 
                iBindings[index].iRtoValue );
            }
        }
    else
        {
        stunBinding->SendRequestL( 
            iBindings[index].iDestAddr, ETrue, iBindings[index].iRtoValue  );
        }
    }


// ---------------------------------------------------------------------------
// CIceConnectionHandler::CancelCheck
// ---------------------------------------------------------------------------
//
void CIceConnectionHandler::CancelCheck( TUint aStreamId, 
        TUint aConnectionId, const TInetAddr& aDestAddr )
    {
    __ICEDP( "CIceConnectionHandler::CancelCheck" )
    
    TInt index( FindBindingIndex( aStreamId, aConnectionId, aDestAddr ) );
    if ( KErrNotFound != index  )
        {
        CSTUNBinding* stunBinding =
            static_cast<CSTUNBinding*>( iBindings[index].iBinding );
        if ( EStreamingStateActive == iBindings[index].iStreamingStatus ||
             EIceConnTypeTurn == iBindings[index].iConnType )
            {
            // Previous transaction is fully terminated because STUN binding
            // does not currently support simultaneous transactions.
            stunBinding->CancelRequest();
            }
        else
            {
            // mux connection not yet activated
            __ICEDP( "CIceConnectionHandler::CancelCheck, NO ACTIVE CONN" )
            iBindings[index].iCancelled = ETrue;
            }
        }
    else
        {
        __ICEDP( "CIceConnectionHandler::CancelCheck, ASSERT" )
        ASSERT( EFalse );
        }
    }


// ---------------------------------------------------------------------------
// CIceConnectionHandler::ReleasePlugins
// ---------------------------------------------------------------------------
//
void CIceConnectionHandler::ReleasePlugins( TBool aRetainRelay )
    {
    __ICEDP( "CIceConnectionHandler::ReleasePlugins" )
    
    iPluginContainer.ReleasePlugins( aRetainRelay );
    }


// ---------------------------------------------------------------------------
// CIceConnectionHandler::STUNClientInitCompleted
// ---------------------------------------------------------------------------
//
void CIceConnectionHandler::STUNClientInitCompleted( 
        const CSTUNClient& /*aClient*/,
        TInt /*aCompletionCode*/ )
    {
    __ICEDP( "CIceConnectionHandler::STUNClientInitCompleted" )
    // SHOULD NOT GET CALLED IN THIS IMPLEMENTATION
    ASSERT( EFalse );
    }


// ---------------------------------------------------------------------------
// CIceConnectionHandler::STUNBindingEventOccurredL
// ---------------------------------------------------------------------------
//
void CIceConnectionHandler::STUNBindingEventOccurredL( 
        TSTUNBindingEvent aEvent, const CBinding& aBinding )
    {
    __ICEDP( "CIceConnectionHandler::STUNBindingEventOccurredL" )
    
    TInt index = FindBindingIndex( aBinding );
    User::LeaveIfError( index );
    
    // notify client about success of connectivity check with data got
    // in incoming message
    if ( EPublicAddressResolved == aEvent )
        {
        __ICEDP_ADDRLOG( "MAPPEDADDR:", aBinding.PublicAddr() )
        iBindings[index].iMappedAddr = aBinding.PublicAddr();
        ExecuteCallbackL( index, KErrNone );
        }
    else if ( ECredentialsRejected == aEvent )
        {
        iBindings[index].iMappedAddr.SetAddress( KAFUnspec );
        ExecuteCallbackL( index, KErrPermissionDenied );
        }
    else
        {
        __ICEDP( "CIceConnectionHandler::STUNBindingEventOccurredL, ELSE" )
        ASSERT( EFalse );
        }
    }


// ---------------------------------------------------------------------------
// CIceConnectionHandler::STUNBindingErrorOccurred
// ---------------------------------------------------------------------------
//
void CIceConnectionHandler::STUNBindingErrorOccurred( 
        const CBinding& aBinding, TInt aError )
    {
    __ICEDP( "CIceConnectionHandler::STUNBindingErrorOccurred" )
    
    TInt index = FindBindingIndex( aBinding );
    if ( KErrNotFound != index )
        {
        TRAP_IGNORE( ExecuteCallbackL( index, aError ) )
        }
    else
        {
        __ICEDP( "CIceConnectionHandler::STUNBindingErrorOccurred, ELSE" )
        ASSERT( EFalse );
        }
    }


// ---------------------------------------------------------------------------
// CIceConnectionHandler::ConnectionNotify
// ---------------------------------------------------------------------------
//
void CIceConnectionHandler::ConnectionNotify( 
        TUint aStreamId, TUint aConnectionId,
        MNcmConnectionObserver::TConnectionNotifyType aType, 
        TInt aError )
    {
    __ICEDP_INT1( 
        "CIceConnectionHandler::ConnectionNotify, ERR:", aError )
    
    TInt index( FindBindingIndex( aStreamId, aConnectionId ) );
    if ( KErrNotFound != index 
            && EIceConnTypeHost == iBindings[index].iConnType )
        {
        switch ( aType )
            {
            case MNcmConnectionObserver::EConnectionRemoved:
                {
                DoRemoveConnection( aStreamId, aConnectionId, EFalse );
                iConnHandlerObs.StreamClosed( aStreamId );
                }
                break;
            
            case MNcmConnectionObserver::EConnectionError:
                {
                // Handled with error handling below
                }
                break;
            
            case MNcmConnectionObserver::EReceivingActivated:
                {
                // Wait for MNcmConnectionObserver::ESendingActivated
                }
                break;
            
            case MNcmConnectionObserver::EReceivingDeactivated:
                {
                // Wait for MNcmConnectionObserver::ESendingDeactivated
                }
                break;
           
            case MNcmConnectionObserver::ESendingActivated:
                {
                iBindings[index].iStreamingStatus = EStreamingStateActive;
                
                if ( iBindings[index].iCancelled )
                    {
                    // Connectivity check is now 'cancelled' and new request
                    // can be received.
                    iBindings[index].iCancelled = EFalse;
                    }
                else
                    {
                    CSTUNBinding* stunBinding =
                        static_cast<CSTUNBinding*>( iBindings[index].iBinding );
                    TRAP_IGNORE( stunBinding->SendRequestL( 
                        iBindings[index].iDestAddr, ETrue, 
                        iBindings[index].iRtoValue ) )
                    }
                }
                break;
            
            case MNcmConnectionObserver::ESendingDeactivated:
                {
                iBindings[index].iStreamingStatus = EStreamingStatePassive;
                }
                break;
                
            case MNcmConnectionObserver::EFirstMediaPacketSent:
                {
                }
                break;    
            
            default:
                ASSERT( EFalse );
            }
        
        if ( aError )
            {
            if ( iBindings[index].iCheckOngoing )
                {
                iBindings[index].iCheckOngoing = EFalse;
                TRAP_IGNORE( ExecuteCallbackL( index, KErrUnknown ) )
                }
            
            DoRemoveConnection( aStreamId, aConnectionId, ETrue );
            }
        }
    }


// ---------------------------------------------------------------------------
// CIceConnectionHandler::IncomingMessageL
// For protocol message receiving.
// ---------------------------------------------------------------------------
//
void CIceConnectionHandler::IncomingMessageL( TUint aStreamId,
        const TDesC8& aMessage, const TInetAddr& aLocalAddr,
        const TInetAddr& aFromAddr, const TInetAddr& aPeerAddr,
        TBool& aConsumed )
    {
    __ICEDP_ADDRLOG( 
        "CIceConnectionHandler::IncomingMessageL FROMADDR", aFromAddr )
    __ICEDP_ADDRLOG( 
        "CIceConnectionHandler::IncomingMessageL PEERADDR", aPeerAddr )
    
    TInt index( iBindings.Count() - 1 );
    __ASSERT_ALWAYS( 0 <= index, User::Leave( KErrNotReady ) );
    
    TInetAddr peerAddrFromIndication( KAFUnspec );
    HBufC8* indicationData( NULL );
    TInt consumingBindingInd( KErrNotFound );
    
    // offer message for every binding in the stream until consumed
    while ( ( KErrNotFound == consumingBindingInd ) && ( 0 <= index ) )
        {
        if ( iBindings[index].iStreamId == aStreamId )
            {
            CSTUNBinding* stunBinding = NULL;
            stunBinding = 
                static_cast<CSTUNBinding*>( iBindings[index].iBinding );
            
            TBool consumed( EFalse );
            if ( indicationData )
                {
                stunBinding->HandleDataL( 
                    *indicationData, consumed, peerAddrFromIndication );
                }
            else
                {
                indicationData = stunBinding->HandleDataL( 
                    aMessage, consumed, peerAddrFromIndication );
                }
            
            if ( consumed )
                {
                consumingBindingInd = index;
                aConsumed = ETrue;
                }
            }
        
        index--;
        }
    
    if ( KErrNotFound != consumingBindingInd )
        {
        iBindings[consumingBindingInd].iLocalAddr = aLocalAddr;
        iBindings[consumingBindingInd].iFromAddr = aFromAddr;
        
        if ( aPeerAddr.IsUnspecified() )
            {
            if ( peerAddrFromIndication.IsUnspecified() )
                {
                iBindings[consumingBindingInd].iPeerAddr = aFromAddr;
                }
            else
                {
                iBindings[consumingBindingInd].iPeerAddr 
                    = peerAddrFromIndication;
                }
            }
        else
            {
            iBindings[consumingBindingInd].iPeerAddr = aPeerAddr;
            }
        
        __ICEDP_ADDRLOG( 
            "CIceConnectionHandler::IncomingMessageL BINDING PEERADDR",
            iBindings[consumingBindingInd].iPeerAddr )
        
        // mapped address is filled when binding event occurs
        }
    
    delete indicationData;
    }


// ---------------------------------------------------------------------------
// From class MIceNatPluginEventObs.
// CIceConnectionHandler::PluginEventOccured
// ---------------------------------------------------------------------------
//
void CIceConnectionHandler::PluginEventOccured( 
        const CNATFWPluginApi* /*aPlugin*/, TUint aStreamId, 
        MIceNatPluginEventObs::TNatPluginEvent aEventCode,
        TInt aErrorCode, TAny* aEventData, TUint /*aStreamConnectionId*/,
        TUint /*aComponentId*/, TBool /*aIPv6After*/ )
    {
    __ICEDP( "CIceConnectionHandler::PluginEventOccured" )
    
    switch ( aEventCode )
        {
        case MIceNatPluginEventObs::ESendingActivated:
        case MIceNatPluginEventObs::ESendingDeactivated:
        case MIceNatPluginEventObs::EReceivingActivated:
        case MIceNatPluginEventObs::EReceivingDeactivated:
            {
            iEventObserver.PluginEventOccured( NULL, aStreamId, 
                aEventCode, aErrorCode, aEventData );
            }
            break;
        
        default:
            __ICEDP( "CIceConnectionHandler::PluginEventOccured, DEFAULT" )
            ASSERT( EFalse );
        }
    }


// ---------------------------------------------------------------------------
// CIceConnectionHandler::FindBindingIndex
// Private. For finding the right binding.
// ---------------------------------------------------------------------------
//
TInt CIceConnectionHandler::FindBindingIndex( 
        TUint aStreamId, TUint aConnectionId ) const
    {
    __ICEDP( "CIceConnectionHandler::FindBindingIndex" )
    
    TInt index( iBindings.Count() - 1 );
    while ( ( index >= 0 ) 
            && !( ( iBindings[index].iStreamId == aStreamId )
            && ( iBindings[index].iConnectionId == aConnectionId ) ) )
        {
        index--;
        }
    
    return index;
    }


// ---------------------------------------------------------------------------
// CIceConnectionHandler::FindBindingIndex
// Private. For finding the right binding.
// ---------------------------------------------------------------------------
//
TInt CIceConnectionHandler::FindBindingIndex( 
        const CBinding& aBinding ) const
    {
    TInt index( iBindings.Count() - 1 );
    while ( ( index >= 0 ) && ( iBindings[index].iBinding != &aBinding ) )
        {
        index--;
        }
    
    return index;
    }


// ---------------------------------------------------------------------------
// CIceConnectionHandler::FindBindingIndex
// Private. For finding the right binding.
// ---------------------------------------------------------------------------
//
TInt CIceConnectionHandler::FindBindingIndex( 
        TUint aStreamId, TUint aConnectionId, 
        const TInetAddr& aDestAddr ) const
    {
    __ICEDP( "CIceConnectionHandler::FindBindingIndex" )
    
    TInt foundConnectionInd( KErrNotFound );
    TInt index( iBindings.Count() - 1 );
    while ( KErrNotFound == foundConnectionInd && 0 <= index )
        {
        if ( ( iBindings[index].iStreamId == aStreamId ) &&
             ( iBindings[index].iConnectionId == aConnectionId ) &&
             ( TIceUtils::MatchAddresses( 
                   iBindings[index].iDestAddr, aDestAddr ) ) )
            {
            foundConnectionInd = index;
            }
        
        index--;
        }
    
    return foundConnectionInd;
    }


// ---------------------------------------------------------------------------
// CIceConnectionHandler::SolvePluginL
// Private. Solves plugin owning given candidate.
// ---------------------------------------------------------------------------
//
CNATFWPluginApi* CIceConnectionHandler::SolvePluginL( 
        const CNATFWCandidate& aLocalCandidate )
    {
    CNATFWPluginApi* item( NULL );
    switch ( aLocalCandidate.Type() )
        {
        case CNATFWCandidate::EHost:
        case CNATFWCandidate::EPeerReflexive:
            // Host plugin is ICE instantiated object and can be resolved
            // with hardcoded id. Host plugin binds peer reflexive candidate
            // to the host connection when candidate is given to the plugin 
            // first time.
            item = iPluginContainer.PluginByIdentifier( 
                KNatPluginIdNokiaHost() );
            break;
        
        case CNATFWCandidate::EServerReflexive:
            item = iPluginContainer.PluginByType( KNatPluginTypeStun() );
            break;
        
        case CNATFWCandidate::ERelay:
            item = iPluginContainer.PluginByType( KNatPluginTypeTurn() );
            break;
       
        default:
            ASSERT( EFalse );
        }
    
    __ASSERT_ALWAYS( NULL != item, User::Leave( KErrNotFound ) );
    return item;
    }


// ---------------------------------------------------------------------------
// CIceConnectionHandler::FreeResources
// Private. Remove data from binding array.
// ---------------------------------------------------------------------------
//
void CIceConnectionHandler::FreeResources()
    {
    __ICEDP( "CIceConnectionHandler::FreeResources" )
    
    TInt count( iBindings.Count() );
    for ( TInt i( 0 ); i < count; ++i )
        {
        delete iBindings[i].iBinding;
        iBindings[i].iBinding = NULL;
        
        TRAP_IGNORE( 
            iMultiplexer.UnregisterConnectionObserverL( 
                iBindings[i].iStreamId, *this ) )
        TRAP_IGNORE(
            iMultiplexer.UnregisterIncomingConnectionObserverL( 
                iBindings[i].iStreamId, *this ) )
        
        if ( EIceConnTypeHost == iBindings[i].iConnType )
            {
            TRAP_IGNORE(
                iMultiplexer.RemoveConnectionL( 
                    iBindings[i].iStreamId, iBindings[i].iConnectionId ) )
            }
        }
    
    iBindings.Close();
    }


// ---------------------------------------------------------------------------
// CIceConnectionHandler::ExecuteCallbackL
// Private.
// ---------------------------------------------------------------------------
//
void CIceConnectionHandler::ExecuteCallbackL( TInt aIndex, TInt aError )
    {
    __ICEDP( "CIceConnectionHandler::ExecuteCallbackL" )
    
    iBindings[aIndex].iCheckOngoing = EFalse;
    __ASSERT_ALWAYS( NULL != iBindings[aIndex].iObserver, 
        User::Leave( KErrNotReady ) );
    
    iBindings[aIndex].iObserver->ConnCheckCompletedL( 
        aError,
        iBindings[aIndex].iLocalAddr,
        iBindings[aIndex].iPeerAddr,
        iBindings[aIndex].iMappedAddr );
    }


// ---------------------------------------------------------------------------
// CIceConnectionHandler::DoRemoveConnection
// ---------------------------------------------------------------------------
//
void CIceConnectionHandler::DoRemoveConnection( 
        TUint aStreamId, TUint aConnectionId, TBool aRemoveMuxConn )
    {
    __ICEDP_INT1( "CIceConnectionHandler::DoRemoveConnection, start:",
        aRemoveMuxConn )
    
    TInt index( FindBindingIndex( aStreamId, aConnectionId ) );
    if ( KErrNotFound != index )
        {
        if ( aRemoveMuxConn && 
                EIceConnTypeHost == iBindings[index].iConnType )
            {
            TRAP_IGNORE(
                iMultiplexer.RemoveConnectionL( 
                    iBindings[index].iStreamId, 
                    iBindings[index].iConnectionId ) )
            }
        
        delete iBindings[index].iBinding;
        iBindings.Remove( index );
        }
    
    __ICEDP( "CIceConnectionHandler::DoRemoveConnection, end" )
    }
