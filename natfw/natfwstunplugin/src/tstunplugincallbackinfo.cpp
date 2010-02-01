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




#include "tstunplugincallbackinfo.h"
#include "natfwstunpluginlogs.h"

#ifdef _DEBUG
_LIT16( KPanicArgument, "invalid argument" );
#endif

// ---------------------------------------------------------------------------
// TStunPluginCallbackInfo::TStunPluginCallbackInfo
// ---------------------------------------------------------------------------
//
TStunPluginCallbackInfo::TStunPluginCallbackInfo( 
    const CNATFWPluginApi& aStunPlugin,
    MNATFWPluginObserver& aObserver, 
    TStunPluginCallbackInfo::TFunction aFunction, TUint aStreamid,
    TInt aErrorCode,
    TAny* aEventData ) 
    :
    iLink(),
    iStunPlugin( aStunPlugin ),
    iObserver( aObserver ),
    iFunction( aFunction ),
    iStreamId( aStreamid ),
    iErrorCode( aErrorCode ),
    iEventData( aEventData )
    {
    }


// ---------------------------------------------------------------------------
// TStunPluginCallbackInfo::TStunPluginCallbackInfo
// Dummy implementation. Default constructor is declared private and not used.
// ---------------------------------------------------------------------------
//
TStunPluginCallbackInfo::TStunPluginCallbackInfo() :
    iLink(),
    iStunPlugin( *( CNATFWPluginApi* )0x1 ),
    iObserver( *( MNATFWPluginObserver* )0x1 ),
    iFunction( EFetchingEnd ),
    iStreamId( 0 ),
    iErrorCode( KErrNone ),
    iEventData( NULL )
    {
    }


// ---------------------------------------------------------------------------
// TStunPluginCallbackInfo::TStunPluginCallbackInfo
// Dummy implementation, as copy constructor is declared private and not used.
// ---------------------------------------------------------------------------
//
TStunPluginCallbackInfo::TStunPluginCallbackInfo(
    const TStunPluginCallbackInfo& /*aCallbackInfo*/ ) :
    iLink(),
    iStunPlugin( *( CNATFWPluginApi* )0x1 ),
    iObserver( *( MNATFWPluginObserver* )0x1 ),
    iFunction( EFetchingEnd ),
    iStreamId( 0 ),
    iErrorCode( KErrNone ),
    iEventData( NULL )
    {
    }


// ----------------------------------------------------------------------------
// TStunPluginCallbackInfo::Validate
// ----------------------------------------------------------------------------
//
TBool TStunPluginCallbackInfo::Validate() const
    {
    __STUNPLUGIN( "StunPluginCallbackInfo::Validate()" )
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
// TStunPluginCallbackInfo::Execute
// ---------------------------------------------------------------------------
//
void TStunPluginCallbackInfo::Execute() const
    {
    __STUNPLUGIN( "StunPluginCallbackInfo::Execute() in" )
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
            iObserver.NewLocalCandidateFound( iStunPlugin, candidate );
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
    
    __STUNPLUGIN( "StunPluginCallbackInfo::Execute() out" )
    }

// ---------------------------------------------------------------------------
// TStunPluginCallbackInfo::PassEvent
// ---------------------------------------------------------------------------
//
void TStunPluginCallbackInfo::PassEvent(
    MNATFWPluginObserver::TNATFWPluginEvent aEvent ) const
    {    
    iObserver.Notify( iStunPlugin, iStreamId, aEvent, iErrorCode );
    }


// ---------------------------------------------------------------------------
// TStunPluginCallbackInfo::PassError
// ---------------------------------------------------------------------------
//
void TStunPluginCallbackInfo::PassError() const
    {    
    iObserver.Error( iStunPlugin, iStreamId, iErrorCode );
    }
