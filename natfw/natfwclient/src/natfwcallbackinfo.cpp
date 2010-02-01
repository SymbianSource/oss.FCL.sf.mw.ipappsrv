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




#include "natfwcallbackinfo.h"
#include "natfwclientlogs.h"
#include "mnatfwregistrationcontroller.h"

#ifdef _DEBUG
_LIT16( KPanicArgument, "invalid argument" );
#endif

// ---------------------------------------------------------------------------
// CNatFwCallbackInfo::CNatFwCallbackInfo
// ---------------------------------------------------------------------------
//
CNatFwCallbackInfo::CNatFwCallbackInfo(
    MNATFWRegistrationController& aController,
    CNatFwCallbackInfo::TFunction aFunction, TUint aSessionId,
    TUint aStreamId, TInt aErrorCode, TAny* aEventData ) :
    iLink(),
    iController( aController ),
    iFunction( aFunction ),
    iSessionId( aSessionId ),
    iStreamId( aStreamId ),
    iErrorCode( aErrorCode ),
    iEventData( aEventData )
    {

    }


// ----------------------------------------------------------------------------
// CNatFwCallbackInfo::Validate
// ----------------------------------------------------------------------------
//
TBool CNatFwCallbackInfo::Validate() const
    {
    __NATFWCLIENT( "CNatFwCallbackInfo::Validate()" )

    switch ( iFunction )
        {
        case ECreatedSession:
        case ELocalCandidateFound:
        case ECandidatePairFound:
            return iSessionId;

        case EFetchingEnd:
        case EActiveReceiving:
        case EDeactiveReceiving:
        case EActiveSending:
        case EDeactiveSending:
        case ECompletedConnChecks:
            return iSessionId && iStreamId;

        case EError:
            return iSessionId && iStreamId && ( iErrorCode != KErrNone );

        default:
            return EFalse;
        }
    }


// ---------------------------------------------------------------------------
// CNatFwCallbackInfo::Execute
// ---------------------------------------------------------------------------
//
void CNatFwCallbackInfo::Execute() const
    {
    __NATFWCLIENT_INT1( "CNatFwCallbackInfo::Execute() in - Function:",
        iFunction )
    __ASSERT_DEBUG( Validate(),
        User::Panic( KPanicArgument, KErrArgument ) );

    switch ( iFunction )
        {
        case ECreatedSession:
            PassEvent( MNATFWConnectivityObserver::ESessionCreated );
            break;

        case EFetchingEnd:
            PassEvent( MNATFWConnectivityObserver::EFetchingCompleted );
            break;

        case EActiveReceiving:
            PassEvent( MNATFWConnectivityObserver::EReceivingActivated );
            break;

        case EDeactiveReceiving:
            PassEvent( MNATFWConnectivityObserver::EReceivingDeactivated );
            break;

        case EActiveSending:
            PassEvent( MNATFWConnectivityObserver::ESendingActivated );
            break;

        case EDeactiveSending:
            PassEvent( MNATFWConnectivityObserver::ESendingDeactivated );
            break;

        case ECompletedConnChecks:
            PassEvent( MNATFWConnectivityObserver::EConnChecksCompleted );
            break;

        case ELocalCandidateFound:
            PassEvent( MNATFWConnectivityObserver::ELocalCandidateFound );
            break;

        case ECandidatePairFound:
            PassEvent( MNATFWConnectivityObserver::ECandidatePairFound );
            break;

        case EError:
            PassEvent( MNATFWConnectivityObserver::EGeneralError );
            break;

        default:
            ASSERT( EFalse );
        }

    __NATFWCLIENT( "CNatFwCallbackInfo::Execute() out" )
    }


// ---------------------------------------------------------------------------
// CNatFwCallbackInfo::PassEvent
// ---------------------------------------------------------------------------
//
void CNatFwCallbackInfo::PassEvent(
    MNATFWConnectivityObserver::TNATFWConnectivityEvent aEvent ) const
    {
    RArray<TNATFWEventRegistration> registry = iController.Registry();
    for ( TInt i(0); i < registry.Count(); ++i )
        {
        if ( registry[i].Events() & aEvent )
            {
            // Observer registered for this event
            registry[i].Observer().EventOccured( iSessionId, iStreamId,
                aEvent, iErrorCode, iEventData );
            }
        }
    }
