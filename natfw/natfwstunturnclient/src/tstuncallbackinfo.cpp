/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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




#include "stunassert.h"
#include "tstuncallbackinfo.h"

// -----------------------------------------------------------------------------
// TSTUNCallbackInfo::TSTUNCallbackInfo
// -----------------------------------------------------------------------------
//
TSTUNCallbackInfo::TSTUNCallbackInfo( TSTUNCallbackInfo::TFunction aFunction,
                                         const CBinding* aBinding,
                                         TInt aErrorCode,
                                         const CSTUNClient* aClient ) :
    iLink(),
    iFunction( aFunction ),
    iBinding( aBinding ),
    iErrorCode( aErrorCode ),
    iClient( aClient )
    {
    }

// -----------------------------------------------------------------------------
// TSTUNCallbackInfo::TSTUNCallbackInfo
// Dummy implementation. Default constructor is declared private and not used.
// -----------------------------------------------------------------------------
//
TSTUNCallbackInfo::TSTUNCallbackInfo() :
    iLink(),
    iFunction( EInitCompleted ),
    iBinding( NULL ),
    iErrorCode( KErrNone ),
    iClient( NULL )
    {
    }

// -----------------------------------------------------------------------------
// TSTUNCallbackInfo::TSTUNCallbackInfo
// Dummy implementation, as copy constructor is declared private and not used.
// -----------------------------------------------------------------------------
//
TSTUNCallbackInfo::TSTUNCallbackInfo(
    const TSTUNCallbackInfo& /*aCallbackInfo*/ ) :
    iLink(),
    iFunction( EInitCompleted ),
    iBinding( NULL ),
    iErrorCode( KErrNone ),
    iClient( NULL )
    {
    }

// -----------------------------------------------------------------------------
// TSTUNCallbackInfo::Validate
// -----------------------------------------------------------------------------
//
TBool TSTUNCallbackInfo::Validate() const
    {
    switch ( iFunction )
        {
        case EInitCompleted:
            return iClient && !iBinding;

        case EEventAddressResolvedL:
        case EEventCredentialsRejected:
        case EEventActiveDestinationSet:
        case EEventTCPConnectOk:
            return iBinding && ( iErrorCode == KErrNone ) && !iClient;

        case EErrorOccurred:
            return iBinding && ( iErrorCode != KErrNone ) && !iClient;            

        default:
            return EFalse;
        }    
    }

// -----------------------------------------------------------------------------
// TSTUNCallbackInfo::Execute
// -----------------------------------------------------------------------------
//
void TSTUNCallbackInfo::Execute( MSTUNClientObserver& aObserver ) const
    {
    __STUN_ASSERT_RETURN( Validate(), KErrGeneral );

    switch ( iFunction )
        {
        case EInitCompleted:
            __STUN_ASSERT_L( iClient != NULL, KErrNotFound );
            aObserver.STUNClientInitCompleted( *iClient, iErrorCode );
            break;

        case EEventAddressResolvedL:        
            PassBindingEvent( aObserver,
                              MSTUNClientObserver::EPublicAddressResolved );
            break;

        case EEventCredentialsRejected:
            PassBindingEvent( aObserver,
                              MSTUNClientObserver::ECredentialsRejected );
            break;
        
        case EEventActiveDestinationSet:
            PassBindingEvent( aObserver,
                              MSTUNClientObserver::EActiveDestinationSet );
            break;
            
        case EEventTCPConnectOk:
            PassBindingEvent( aObserver,
                              MSTUNClientObserver::ETCPConnectOk );
            break;
            
        case EErrorOccurred:
            __STUN_ASSERT_L( iBinding != NULL, KErrNotFound );
            aObserver.STUNBindingErrorOccurred( *iBinding, iErrorCode );
            break;

        default:
            __STUN_FAILURE_RETURN( KErrGeneral );
        }
    }

// -----------------------------------------------------------------------------
// TSTUNCallbackInfo::PassBindingEvent
// -----------------------------------------------------------------------------
//
void TSTUNCallbackInfo::PassBindingEvent(
    MSTUNClientObserver& aObserver,
    MSTUNClientObserver::TSTUNBindingEvent aEvent ) const
    {
    __STUN_ASSERT_L( iBinding != NULL, KErrGeneral );
    TRAP_IGNORE( aObserver.STUNBindingEventOccurredL( aEvent, *iBinding ) );
    }
