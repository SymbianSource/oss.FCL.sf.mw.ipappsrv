/*
* Copyright (c) 2006 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Implements NAT Plug-in interface, which client uses to send
*                TURN requests. Forwards client requests to Turn Connection
*                Handler.
*
*/




#include "natfwcandidate.h"
#include "natfwturnplugin.h"
#include "natfwturnconnectionhandler.h"
#include "natfwturnpluginlogs.h"

_LIT8( KNatPluginIdNokiaTurn, "nokia.turn" );

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ default constructor
// ---------------------------------------------------------------------------
//
CNATFWTurnPlugin::CNATFWTurnPlugin()
    {

    }


// ---------------------------------------------------------------------------
// Symbian constructor
// Creates NATFWTurnConnectionHandler.
// ---------------------------------------------------------------------------
//
void CNATFWTurnPlugin::ConstructL( TAny* aInitParams )
    {
    __TURNPLUGIN( "CNATFWTurnPlugin::ConstructL start" )

    __ASSERT_ALWAYS( aInitParams, User::Leave( KErrArgument ) );
    
    iPluginId = KNatPluginIdNokiaTurn().AllocL();
    
    CNATFWPluginApi::TNATFWPluginInitParams* initParams =
        static_cast<CNATFWPluginApi::TNATFWPluginInitParams*>(
            aInitParams );

    iTurnConnectionHandler = CNATFWTurnConnectionHandler::NewL(
        *this, initParams->iObserver );

    iTurnConnectionHandler->PluginInitializeL( initParams->iIapId,
                                               initParams->iDomain,
                                               initParams->iMultiplexer );

    __TURNPLUGIN( "CNATFWTurnPlugin::ConstructL end" )
    }


// ---------------------------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------------------------
//
CNATFWTurnPlugin* CNATFWTurnPlugin::NewL( TAny* aInitParams )
    {
    __TURNPLUGIN( "CNATFWTurnPlugin::NewL" )
    CNATFWTurnPlugin* self = new ( ELeave ) CNATFWTurnPlugin();
    CleanupStack::PushL( self );
    self->ConstructL( aInitParams );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CNATFWTurnPlugin::~CNATFWTurnPlugin()
    {
    delete iPluginId;
    delete iTurnConnectionHandler;
    }


// ---------------------------------------------------------------------------
// From class CNATFWPluginApi.
// CNATFWTurnPlugin::ConnectServerL
// ---------------------------------------------------------------------------
//
void CNATFWTurnPlugin::ConnectServerL( const RSocketServ& aSocketServ,
    const TName& aConnectionName )
    {
    __TURNPLUGIN( "CNATFWTurnPlugin::ConnectServerL start" )
    
    iTurnConnectionHandler->ConnectServerL( aSocketServ, aConnectionName );
    
    __TURNPLUGIN( "CNATFWTurnPlugin::ConnectServerL end" )
    }


// ---------------------------------------------------------------------------
// From class CNATFWPluginApi.
// CNATFWTurnPlugin::FetchCandidateL
// ---------------------------------------------------------------------------
//
void CNATFWTurnPlugin::FetchCandidateL( TUint aStreamId, TUint aRtoValue,
    TUint aAddrFamily )
    {
    __TURNPLUGIN( "CNATFWTurnPlugin::FetchCandidate start" )
    
    iTurnConnectionHandler->FetchCandidateL( aStreamId, aRtoValue,
        aAddrFamily, KAFUnspec );
    
    __TURNPLUGIN( "CNATFWTurnPlugin::FetchCandidate end" )
    }


// ---------------------------------------------------------------------------
// From class CNATFWPluginApi.
// CNATFWTurnPlugin::FetchCandidateL
// ---------------------------------------------------------------------------
//
void CNATFWTurnPlugin::FetchCandidateL( TUint aStreamId, TUint aRtoValue,
    const TInetAddr& aBaseAddr )
    {
    __TURNPLUGIN( "CNATFWTurnPlugin::FetchCandidateL start" )
    
    iTurnConnectionHandler->FetchCandidateL( aStreamId, aRtoValue, KAFUnspec,
        aBaseAddr );
    
    __TURNPLUGIN( "CNATFWTurnPlugin::FetchCandidateL end" )
    }


// ---------------------------------------------------------------------------
// From class CNATFWPluginApi.
// CNATFWTurnPlugin::SetReceivingStateL
// ---------------------------------------------------------------------------
//
void CNATFWTurnPlugin::SetReceivingStateL(
    const CNATFWCandidate& aLocalCandidate,
    TNATFWStreamingState aState )
    {
    __TURNPLUGIN( "CNATFWTurnPlugin::SetReceivingStateL start" )
    iTurnConnectionHandler->SetReceivingStateL( aLocalCandidate, aState );
    __TURNPLUGIN( "CNATFWTurnPlugin::SetReceivingStateL start" )
    }


// ---------------------------------------------------------------------------
// From class CNATFWPluginApi.
// CNATFWTurnPlugin::SetSendingStateL
// ---------------------------------------------------------------------------
//
void CNATFWTurnPlugin::SetSendingStateL(
    const CNATFWCandidate& aLocalCandidate,
    TNATFWStreamingState aState,
    const TInetAddr& aDestAddr )
    {
    __TURNPLUGIN( "CNATFWTurnPlugin::SetSendingStateL start" )
    
    iTurnConnectionHandler->SetSendingStateL( aLocalCandidate, aState,
        aDestAddr );
    
    __TURNPLUGIN( "CNATFWTurnPlugin::SetSendingStateL end" )
    }


// ---------------------------------------------------------------------------
// From class CNATFWPluginApi.
// CNATFWTurnPlugin::GetConnectionIdL
// ---------------------------------------------------------------------------
//
void CNATFWTurnPlugin::GetConnectionIdL(
    const CNATFWCandidate& aLocalCandidate,
    TUint& aConnectionId )
    {
    __TURNPLUGIN( "CNATFWTurnPlugin::GetConnectionIdL start" )
    iTurnConnectionHandler->GetConnectionIdL( aLocalCandidate,
        aLocalCandidate.StreamId(), aConnectionId );
    __TURNPLUGIN( "CNATFWTurnPlugin::GetConnectionIdL end" )
    }


// ---------------------------------------------------------------------------
// From class CNATFWPluginApi.
// CNATFWTurnPlugin::PluginIdentifier
// ---------------------------------------------------------------------------
//
const TDesC8& CNATFWTurnPlugin::PluginIdentifier() const
    {
    return *iPluginId;
    }

// End of file
