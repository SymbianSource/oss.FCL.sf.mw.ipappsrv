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
* Description:  Session class implementation.
*
*/

#include <sdpdocument.h>
#include "nspsessionobserver.h"
#include "nspsession.h"
#include "nspevents.h"
#include "nspcontrollerif.h"
#include "nspmediastreamcontainer.h"
#include "nspstatemachine.h"
#include "nspactionset.h"
#include "nspstatebase.h"
#include "nspcontentparser.h"
#include "nspdefs.h"

const TUint KConstProtocolTcp = 1;
const TUint KConstProtocolUdp = 2;
const TUint KDefaultGranularity = 5;

#define CONV_PROTOCOL_L( aProtocol ) \
        (TUint) User::LeaveIfError( \
            KConstProtocolTcp == aProtocol ? (TInt)KProtocolInetTcp : \
            ( KConstProtocolUdp == aProtocol ? (TInt)KProtocolInetUdp : KErrNotFound ) )

#define LOGEVENT( aEvent ) \
	switch( aEvent )\
		{\
		case MNATFWConnectivityObserver::ESessionCreated:{ NSPLOG_STR( "CNSPSession::EventOccured, event:ESessionCreated" ); break; }\
		case MNATFWConnectivityObserver::ELocalCandidateFound:{ NSPLOG_STR( "CNSPSession::EventOccured, event:ELocalCandidateFound" ); break; }\
		case MNATFWConnectivityObserver::EFetchingCompleted:{ NSPLOG_STR( "CNSPSession::EventOccured, event:EFetchingCompleted" ); break; }\
		case MNATFWConnectivityObserver::EReceivingActivated:{ NSPLOG_STR( "CNSPSession::EventOccured, event:EReceivingActivated" ); break; }\
		case MNATFWConnectivityObserver::EReceivingDeactivated:{ NSPLOG_STR( "CNSPSession::EventOccured, event:EReceivingDeactivated" ); break; }\
		case MNATFWConnectivityObserver::ESendingActivated:{ NSPLOG_STR( "CNSPSession::EventOccured, event:ESendingActivated" ); break; }\
		case MNATFWConnectivityObserver::ESendingDeactivated:{ NSPLOG_STR( "CNSPSession::EventOccured, event:ESendingDeactivated" ); break; }\
		case MNATFWConnectivityObserver::ECandidatePairFound:{ NSPLOG_STR( "CNSPSession::EventOccured, event:ECandidatePairFound" ); break; }\
		case MNATFWConnectivityObserver::EConnChecksCompleted:{ NSPLOG_STR( "CNSPSession::EventOccured, event:EConnChecksCompleted" ); break; }\
		case MNATFWConnectivityObserver::EGeneralError:{ NSPLOG_STR( "CNSPSession::EventOccured, event:EGeneralError" ); break; }\
		case MNATFWConnectivityObserver::EAllEvents:{ NSPLOG_STR( "CNSPSession::EventOccured, event:EAllEvents" ); break; }\
		default:{ NSPLOG_STR( "CNSPSession::EventOccured, event:Unknown" ); break; }\
		}

// ======== MEMBER FUNCTIONS ========
// ---------------------------------------------------------------------------
// CNSPSession::CNSPSession
// ---------------------------------------------------------------------------
// 
CNSPSession::CNSPSession( MNSPControllerIF& aController,
        MNSPSessionObserver& aSessionObserver )
    : iController( aController ),
      iSessionObserver( aSessionObserver )
    {
    }


// ---------------------------------------------------------------------------
// CNSPSession::ConstructL
// ---------------------------------------------------------------------------
//
void CNSPSession::ConstructL( TUint32 aIapId, const TDesC8& aDomain,
        TUint aProtocol )
    {
    NSPLOG_STR( "CNSPSession::ConstructL(), Entry" )
    
    iSessionId = iController.CreateSessionL( aIapId, aDomain );
    iSessionData = CNSPSessionData::NewL();
    iStreamContainer = CNSPMediaStreamContainer::NewL(
            iController, iSessionId, CONV_PROTOCOL_L( aProtocol ) );
    iStateMachine = CNSPStateMachine::NewL();
    iProtocolsArray = new (ELeave) CDesC8ArrayFlat( KDefaultGranularity );
    
    NSPLOG_STR( "CNSPSession::ConstructL(), Exit" )
    }


// ---------------------------------------------------------------------------
// CNSPSession::NewL
// ---------------------------------------------------------------------------
//
CNSPSession* CNSPSession::NewL( MNSPControllerIF& aController,
        MNSPSessionObserver& aSessionObserver, TUint32 aIapId,
        const TDesC8& aDomain, TUint aProtocol )
    {
    CNSPSession* self = CNSPSession::NewLC( aController, aSessionObserver,
            aIapId, aDomain, aProtocol );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNSPSession::NewLC
// ---------------------------------------------------------------------------
//
CNSPSession* CNSPSession::NewLC( MNSPControllerIF& aController,
        MNSPSessionObserver& aSessionObserver, TUint32 aIapId,
        const TDesC8& aDomain, TUint aProtocol )
    {
    CNSPSession* self = new ( ELeave ) CNSPSession( aController,
            aSessionObserver );
    CleanupStack::PushL( self );
    self->ConstructL( aIapId, aDomain, aProtocol );
    return self;
    }


// ---------------------------------------------------------------------------
// CNSPSession::~CNSPSession
// ---------------------------------------------------------------------------
//
CNSPSession::~CNSPSession()
    {
    NSPLOG_STR( "CNSPSession::~CNSPSession(), Entry" )
    
    delete iSessionData;
    delete iStreamContainer;
    delete iStateMachine;
    delete iProtocolsArray;
    TRAP_IGNORE( iController.CloseSessionL( iSessionId ) );
    
    NSPLOG_STR( "CNSPSession::~CNSPSession(), Exit" )
    }


// ---------------------------------------------------------------------------
// CNSPSession::CreateOfferL
// ---------------------------------------------------------------------------
//
TNatReturnStatus CNSPSession::CreateOfferL( CSdpDocument*& aOffer )
    {
    NSPLOG_STR( "CNSPSession::CreateOfferL(), Entry" )
    
    __ASSERT_ALWAYS( aOffer, User::Leave( KErrArgument ) );
    
    NSP_INPUT_OFFER( *aOffer )
    
    TNSPStateMachineEvent event( KErrNone, aOffer, NULL,
    		TNSPStateMachineEvent::ECreateOffer, *this );
    
    iStateMachine->ProcessL( event );
    
    NSP_OUTPUT_OFFER( event.Status(), *aOffer )
    
    NSPLOG_STR( "CNSPSession::CreateOfferL(), Exit" )
    
    return event.Status();
    }


// ---------------------------------------------------------------------------
// CNSPSession::ResolveL
// ---------------------------------------------------------------------------
//
TNatReturnStatus CNSPSession::ResolveL(
        CSdpDocument*& aOffer, CSdpDocument*& aAnswer )
    {
    NSPLOG_STR( "CNSPSession::ResolveL(), Entry" )
    
    __ASSERT_ALWAYS( aOffer, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( aAnswer, User::Leave( KErrArgument ) );
    
    NSP_INPUT_OFFER( *aOffer )
    NSP_INPUT_ANSWER( *aAnswer )
    
    TNSPStateMachineEvent event( KErrNone, aOffer, aAnswer,
    		TNSPStateMachineEvent::EResolve, *this );
    
    iStateMachine->ProcessL( event );
    
    NSP_OUTPUT_OFFER( event.Status(), *aOffer )
    NSP_OUTPUT_ANSWER( event.Status(), *aAnswer )
    
    NSPLOG_STR( "CNSPSession::ResolveL(), Exit" )
    
    return event.Status();
    }


// ---------------------------------------------------------------------------
// CNSPSession::DecodeAnswerL
// ---------------------------------------------------------------------------
//
TNatReturnStatus CNSPSession::DecodeAnswerL( CSdpDocument*& aAnswer )
    {
    NSPLOG_STR( "CNSPSession::DecodeAnswerL(), Entry" )
    
    __ASSERT_ALWAYS( aAnswer, User::Leave( KErrArgument ) );
    
    NSP_INPUT_ANSWER_L( *aAnswer )
    
    TNSPStateMachineEvent event( KErrNone, NULL, aAnswer,
    		TNSPStateMachineEvent::EDecodeAnswer, *this );
    
    iStateMachine->ProcessL( event );
    
    NSP_OUTPUT_ANSWER_L( event.Status(), *aAnswer )
    
    NSPLOG_STR( "CNSPSession::DecodeAnswerL(), Exit" )
    
    return event.Status();
    }


// ---------------------------------------------------------------------------
// CNSPSession::UpdateL
// ---------------------------------------------------------------------------
//
void CNSPSession::UpdateL( CSdpDocument*& aOffer )
    {    
    NSPLOG_STR( "CNSPSession::UpdateL(), Entry" )
    
    __ASSERT_ALWAYS( aOffer, User::Leave( KErrArgument ) );
    
    NSP_INPUT_OFFER( *aOffer )
    
    TNSPStateMachineEvent event( KErrNone, aOffer, NULL,
    		TNSPStateMachineEvent::EUpdate, *this );
    
    iStateMachine->ProcessL( event );
    
    NSP_OUTPUT_OFFER( event.Status(), *aOffer )
    
    NSPLOG_STR( "CNSPSession::UpdateL(), Exit" )
    }


// ---------------------------------------------------------------------------
// CNSPSession::CloseSessionL
// ---------------------------------------------------------------------------
//
TNatReturnStatus CNSPSession::CloseSessionL()
    {    
    NSPLOG_STR( "CNSPSession::CloseSessionL(), Entry" )
    
    TNSPStateMachineEvent event( KErrNone, NULL, NULL,
    		TNSPStateMachineEvent::ECloseSession, *this );
    
    iStateMachine->ProcessL( event );
    
    NSPLOG_STR( "CNSPSession::CloseSessionL(), Exit" )
    
    return event.Status();
    }


// ---------------------------------------------------------------------------
// CNSPSession::EventOccured
// ---------------------------------------------------------------------------
//
TEventReturnStatus CNSPSession::EventOccured( TUint aStreamId,
        MNATFWConnectivityObserver::TNATFWConnectivityEvent aEvent,
        TInt aError, TAny* aData )
    {
    NSPLOG_STR( "CNSPSession::EventOccured(), Entry" )
    LOGEVENT( aEvent )
    NSPLOG_INT( "CNSPSession::EventOccured, aError:", aError )
    
    __ASSERT_ALWAYS( iStateMachine, __PANIC( KErrNotFound ) );
    
    TNSPStateMachineMediaEvent event( aStreamId, aEvent, aData, aError,
    		NULL, NULL, TNSPStateMachineEvent::ENat, *this );
    
    TRAPD( error, iStateMachine->ProcessL( event ) );
    
    NSPLOG_INT( "CNSPSession::EventOccured, error:", error )
    NSPLOG_INT( "CNSPSession::EventOccured, event.Status():", event.Status() )
    
    event.Status() = ( KErrNone == error ? event.Status() : error );
    event.CallbackType() = ( NSP_ERROR( event.Status() ) ?
    			TEventReturnStatus::EError : event.CallbackType() );
    
    // If event.iOffer or event.iAnswer is not NULL, it means that ownership  
    // was transferred inside iStateMachine->ProcessL().
    // If there was an error we have to free the memory here,
    // since it won't be handled anywhere else
    if ( event.CallbackType() == TEventReturnStatus::EError )
        {
        delete event.iOffer;
        event.iOffer = NULL;
        
        delete event.iAnswer;
        event.iAnswer = NULL;
        }
    
    return static_cast<TEventReturnStatus>( event );
    }


// ---------------------------------------------------------------------------
// CNSPSession::UpdateSdpAsyncL
// ---------------------------------------------------------------------------
//
void CNSPSession::UpdateSdpAsyncL( CSdpDocument* aDocument )
    {
    NSPLOG_STR( "CNSPSession::Update(), Entry" )
    
    __ASSERT_ALWAYS( aDocument, User::Leave( KErrArgument ) );
    
    NSP_OUTPUT_UPDATESDP( KNatReady, *aDocument )
    
    iController.OrderUpdateSdpL( iSessionId, aDocument );
    
    NSPLOG_STR( "CNSPSession::Update(), Exit" )
    }


// ---------------------------------------------------------------------------
// CNSPSession::SessionId
// ---------------------------------------------------------------------------
//
TUint CNSPSession::SessionId() const
    {
    return iSessionId;
    }


// ---------------------------------------------------------------------------
// CNSPSession::Data
// ---------------------------------------------------------------------------
//
CNSPSessionData& CNSPSession::Data()
    {
    return *iSessionData;
    }


// ---------------------------------------------------------------------------
// CNSPSession::Container
// ---------------------------------------------------------------------------
//
CNSPMediaStreamContainer& CNSPSession::Container()
    {
    return *iStreamContainer;
    }


// ---------------------------------------------------------------------------
// CNSPSession::Plugins
// ---------------------------------------------------------------------------
//
CDesC8Array& CNSPSession::Plugins()
    {
    return *iProtocolsArray;
    }


// ---------------------------------------------------------------------------
// CNSPSession::RemoveIce
// ---------------------------------------------------------------------------
//
void CNSPSession::RemoveIce( CDesC8Array& aDesArray ) const
    {
    iController.RemoveIce( aDesArray );
    }


// ---------------------------------------------------------------------------
// CNSPSession::IsIce
// ---------------------------------------------------------------------------
//
TBool CNSPSession::IsIce( const TDesC8& aProtocol ) const
    {
    return iController.IsIce( aProtocol );
    }


// ---------------------------------------------------------------------------
// CNSPSession::Parser
// ---------------------------------------------------------------------------
//
const CNSPContentParser& CNSPSession::Parser() const
    {
    return iController.ContentParser();
    }


// ---------------------------------------------------------------------------
// CNSPSession::Role
// ---------------------------------------------------------------------------
//
CNSPSession::TSdpRole& CNSPSession::Role()
    {
    return iRole;
    }


// ---------------------------------------------------------------------------
// CNSPSession::SessionObserver
// ---------------------------------------------------------------------------
//
MNSPSessionObserver& CNSPSession::SessionObserver() const
    {
    return iSessionObserver;
    }


// ---------------------------------------------------------------------------
// CNSPSession::Actions
// ---------------------------------------------------------------------------
//
TNSPActionSet CNSPSession::Actions()
    {
    return TNSPActionSet( *this );
    }


// ---------------------------------------------------------------------------
// CNSPSession::SetSessionParam
// ---------------------------------------------------------------------------
//
TInt CNSPSession::SetSessionParam( CNSPPlugin::TNSPSessionParamKey aParamKey,
        TUint aParamValue )
    {
    NSPLOG_STR( "CNSPSession::SetSessionParam, Entry" )
    
    TInt status = KErrNone;
    
    switch( aParamKey )
        {
        case CNSPPlugin::ENSPResourseReservationStatusKey:
            {
            status = KErrNotSupported;
            break;
            }
        
        case CNSPPlugin::ENSPMediaTypeOfServiceKey:
            {
            status = iStreamContainer->SetMediaTos( aParamValue );
            break;
            }
        
        default:
            {
            status = KErrNotFound;
            break;
            }
        }
    
    NSPLOG_INT( "CNSPSession::SetSessionParam, Exit, status:", status )
    return status;
    }


// ---------------------------------------------------------------------------
// CNSPSession::GetSessionParam
// ---------------------------------------------------------------------------
//
TInt CNSPSession::GetSessionParam( CNSPPlugin::TNSPSessionParamKey aParamKey )
    {
    NSPLOG_STR( "CNSPSession::GetSessionParam, Entry" )
    
    TInt value = KErrNone;
    
    switch( aParamKey )
        {
        case CNSPPlugin::ENSPResourseReservationStatusKey:
            {
            if ( iSessionData->UseIce() )
                {
                Proceed( TNSPStateMachineEvent::EReservationStatus, value );
                
                value = ( KNatAsync == value ?
                          CNSPPlugin::ENSPResourcesNotReserved :
                          ( KNatReady == value ?
                            CNSPPlugin::ENSPResourcesReserved : value ) );
                }
            else
                {
                value = CNSPPlugin::ENSPResourcesReserved;
                }
            
            break;
            }
        
        case CNSPPlugin::ENSPMediaTypeOfServiceKey:
            {
            value = iStreamContainer->MediaTos();
            break;
            }
        
        default:
            {
            value = KErrNotFound;
            break;
            }
        }
    
    NSPLOG_INT( "CNSPSession::GetSessionParam, Exit, value:", value )
    return value;
    }


// ---------------------------------------------------------------------------
// CNSPSession::SetUseIce
// ---------------------------------------------------------------------------
//
void CNSPSession::SetUseIce( TBool aUseIce )
    {
    __ASSERT_ALWAYS( iSessionData, __PANIC( KErrNotFound ) );
    iSessionData->SetUseIce( aUseIce );
    }


// ---------------------------------------------------------------------------
// CNSPSession::Proceed
// ---------------------------------------------------------------------------
//
void CNSPSession::Proceed( TInt aRequest, TNatReturnStatus& aStatus )
    {
    __ASSERT_ALWAYS( iStateMachine, __PANIC( KErrNotFound ) );
    __ASSERT_ALWAYS( iSessionData, __PANIC( KErrNotFound ) );
    __ASSERT_ALWAYS( iStreamContainer, __PANIC( KErrNotFound ) );
    
    TNSPStateMachineEvent event( KErrNone, NULL, NULL,
    		(TNSPStateMachineEvent::TRequest) aRequest, *this );
    
    TRAPD( error, iStateMachine->ProcessL( event ) );
    
    aStatus = ( KErrNone == error ? event.Status() : error );
    }


// end of file
