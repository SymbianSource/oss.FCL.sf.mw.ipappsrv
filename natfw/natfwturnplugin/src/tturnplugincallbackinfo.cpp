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




#include "tturnplugincallbackinfo.h"
#include "natfwturnpluginlogs.h"

#ifdef _DEBUG
_LIT16( KPanicArgument, "invalid argument" );
#endif

// ---------------------------------------------------------------------------
// TTurnPluginCallbackInfo::TTurnPluginCallbackInfo
// ---------------------------------------------------------------------------
//
TTurnPluginCallbackInfo::TTurnPluginCallbackInfo(
    const CNATFWPluginApi& aTurnPlugin,
    MNATFWPluginObserver& aObserver,
    TTurnPluginCallbackInfo::TFunction aFunction, TUint aStreamId,
    TInt aErrorCode,
    TAny* aEventData )
    :
    iLink(),
    iTurnPlugin( aTurnPlugin ),
    iObserver( aObserver ),
    iFunction( aFunction ),
    iStreamId( aStreamId ),
    iErrorCode( aErrorCode ),
    iEventData( aEventData )
    {
    }

// ---------------------------------------------------------------------------
// TTurnPluginCallbackInfo::TTurnPluginCallbackInfo
// Dummy implementation. Default constructor is declared private and not used.
// ---------------------------------------------------------------------------
//
TTurnPluginCallbackInfo::TTurnPluginCallbackInfo() :
    iLink(),
    iTurnPlugin( *( CNATFWPluginApi* )0x1 ),
    iObserver( *( MNATFWPluginObserver* )0x1 ),
    iFunction( EFetchingEnd ),
    iStreamId( 0 ),
    iErrorCode( KErrNone ),
    iEventData( NULL )
    {
    }

// ---------------------------------------------------------------------------
// TTurnPluginCallbackInfo::TTurnPluginCallbackInfo
// Dummy implementation, as copy constructor is declared private and not used.
// ---------------------------------------------------------------------------
//
TTurnPluginCallbackInfo::TTurnPluginCallbackInfo(
    const TTurnPluginCallbackInfo& /*aCallbackInfo*/ ) :
    iLink(),
    iTurnPlugin( *( CNATFWPluginApi* )0x1 ),
    iObserver( *( MNATFWPluginObserver* )0x1 ),
    iFunction( EFetchingEnd ),
    iStreamId( 0 ),
    iErrorCode( KErrNone ),
    iEventData( NULL )
    {
    }

// ----------------------------------------------------------------------------
// TTurnPluginCallbackInfo::Validate
// ----------------------------------------------------------------------------
//
TBool TTurnPluginCallbackInfo::Validate() const
    {
    __TURNPLUGIN( "TurnPluginCallbackInfo::Validate()" )
    switch ( iFunction )
        {
        case EActiveSending:
        case EDeactiveSending:
        case EActiveReceiving:
        case EDeactiveReceiving:
        case ELocalCandidateFound:
        case EFetchingEnd:
            return iStreamId;

        case EConnectServer:
            return ETrue;
            
        case EError:
            return iStreamId && ( iErrorCode != KErrNone );

        default:
            return EFalse;
        }
    }

// ---------------------------------------------------------------------------
// TTurnPluginCallbackInfo::Execute
// ---------------------------------------------------------------------------
//
void TTurnPluginCallbackInfo::Execute() const
    {
    __TURNPLUGIN( "TurnPluginCallbackInfo::Execute() in" )
    __ASSERT_DEBUG( Validate(),
        User::Panic( KPanicArgument, KErrArgument ) );

    switch ( iFunction )
        {
        case EConnectServer:        
            PassEvent( MNATFWPluginObserver::EServerConnected );
            break;
                    
        case EActiveSending:
            PassEvent( MNATFWPluginObserver::ESendingActivated );
            break;

        case EDeactiveSending:
            PassEvent( MNATFWPluginObserver::ESendingDeactivated );
            break;
        
        case EActiveReceiving:
            PassEvent( MNATFWPluginObserver::EReceivingActivated );
            break;
        
        case EDeactiveReceiving:
            PassEvent( MNATFWPluginObserver::EReceivingDeactivated );
            break;
        
        case ELocalCandidateFound:
            {
            CNATFWCandidate* candidate =
                reinterpret_cast<CNATFWCandidate*>( iEventData );
            
            ASSERT( candidate );
            iObserver.NewLocalCandidateFound( iTurnPlugin, candidate );
            }
            break;
        
        case EFetchingEnd:
            PassEvent( MNATFWPluginObserver::EFetchingCompleted );
            break;
        
        case EError:
            PassError();
            break;
        
        default:
            ASSERT( EFalse );
        }
    
    __TURNPLUGIN( "TurnPluginCallbackInfo::Execute() out" )
    }

// ---------------------------------------------------------------------------
// TTurnPluginCallbackInfo::PassEvent
// ---------------------------------------------------------------------------
//
void TTurnPluginCallbackInfo::PassEvent(
    MNATFWPluginObserver::TNATFWPluginEvent aEvent ) const
    {
    iObserver.Notify( iTurnPlugin, iStreamId, aEvent, iErrorCode );
    }

// ---------------------------------------------------------------------------
// TTurnPluginCallbackInfo::PassError
// ---------------------------------------------------------------------------
//
void TTurnPluginCallbackInfo::PassError() const
    {
    iObserver.Error( iTurnPlugin, iStreamId, iErrorCode );
    }

// End of file
