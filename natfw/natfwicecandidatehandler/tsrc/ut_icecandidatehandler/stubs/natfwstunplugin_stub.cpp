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
*                STUN requests. Forwards client requests to Stun Connection
*                Handler.
*
*/




#include "natfwcandidate.h"
#include "natfwstunplugin.h"
#include "natfwstunconnectionhandler_stub.h"
#include "natfwstunpluginlogs.h"

_LIT8( KNatPluginIdNokiaStun, "nokia.stun" );

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ default constructor
// ---------------------------------------------------------------------------
//
CNATFWStunPlugin::CNATFWStunPlugin()
    {

    }


// ---------------------------------------------------------------------------
// Symbian constructor
// Creates NATFWStunConnectionHandler.
// ---------------------------------------------------------------------------
//
void CNATFWStunPlugin::ConstructL( TAny* aInitParams )
    {
    __STUNPLUGIN( "CNATFWStunPlugin::ConstructL start" )

    __ASSERT_ALWAYS( aInitParams, User::Leave( KErrArgument ) );
    
    iPluginId = KNatPluginIdNokiaStun().AllocL();
    
    CNATFWPluginApi::TNATFWPluginInitParams* initParams = 
        static_cast<CNATFWPluginApi::TNATFWPluginInitParams*>( 
            aInitParams );

    iStunConnectionHandler = CNATFWStunConnectionHandler::NewL( 
        *this, initParams->iObserver );

    iStunConnectionHandler->PluginInitializeL( initParams->iIapId,
                                               initParams->iDomain,
                                               initParams->iMultiplexer );

    __STUNPLUGIN( "CNATFWStunPlugin::ConstructL end" )
    }


// ---------------------------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------------------------
//
CNATFWStunPlugin* CNATFWStunPlugin::NewL( TAny* aInitParams )
    {
    __STUNPLUGIN( "CNATFWStunPlugin::NewL" )
    CNATFWStunPlugin* self = new ( ELeave ) CNATFWStunPlugin();
    CleanupStack::PushL( self );
    self->ConstructL( aInitParams );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CNATFWStunPlugin::~CNATFWStunPlugin()
    {
    delete iPluginId;
    delete iStunConnectionHandler;
    }


// ---------------------------------------------------------------------------
// From class CNATFWPluginApi.
// CNATFWStunPlugin::ConnectServerL
// ---------------------------------------------------------------------------
//
void CNATFWStunPlugin::ConnectServerL( const RSocketServ& aSocketServ,
                                       const TName& aConnectionName )
    {
    __STUNPLUGIN( "CNATFWStunPlugin::ConnectServerL start" )
    
    iStunConnectionHandler->ConnectServerL( aSocketServ, aConnectionName );
    
    __STUNPLUGIN( "CNATFWStunPlugin::ConnectServerL end" )
    }


// ---------------------------------------------------------------------------
// From class CNATFWPluginApi.
// CNATFWStunPlugin::FetchCandidateL
// ---------------------------------------------------------------------------
//
void CNATFWStunPlugin::FetchCandidateL( TUint aStreamId, TUint aRtoValue,
    TUint aAddrFamily )
    {
    __STUNPLUGIN( "CNATFWStunPlugin::FetchCandidateL start" )
    
    iStunConnectionHandler->FetchCandidateL( aStreamId, aRtoValue,
        aAddrFamily, KAFUnspec );
    
    __STUNPLUGIN( "CNATFWStunPlugin::FetchCandidateL end" )
    }


// ---------------------------------------------------------------------------
// From class CNATFWPluginApi.
// CNATFWStunPlugin::FetchCandidateL
// ---------------------------------------------------------------------------
//
void CNATFWStunPlugin::FetchCandidateL( TUint aStreamId, TUint aRtoValue,
    const TInetAddr& aBaseAddr )
    {
    __STUNPLUGIN( "CNATFWStunPlugin::FetchCandidateL start" )
    
    iStunConnectionHandler->FetchCandidateL( aStreamId, aRtoValue, KAFUnspec,
        aBaseAddr );
    
    __STUNPLUGIN( "CNATFWStunPlugin::FetchCandidateL end" )
    }


// ---------------------------------------------------------------------------
// From class CNATFWPluginApi.
// CNATFWStunPlugin::SetReceivingStateL
// ---------------------------------------------------------------------------
//
void CNATFWStunPlugin::SetReceivingStateL(
    const CNATFWCandidate& aLocalCandidate,
    TNATFWStreamingState aState )
    {
    __STUNPLUGIN( "CNATFWStunPlugin::SetReceivingStateL start" )
    iStunConnectionHandler->SetReceivingStateL( aLocalCandidate, aState );
    __STUNPLUGIN( "CNATFWStunPlugin::SetReceivingStateL start" )
    }


// ---------------------------------------------------------------------------
// From class CNATFWPluginApi.
// CNATFWStunPlugin::SetSendingStateL
// ---------------------------------------------------------------------------
//
void CNATFWStunPlugin::SetSendingStateL(
    const CNATFWCandidate& aLocalCandidate,
    TNATFWStreamingState aState,
    const TInetAddr& aDestAddr )
    {
    __STUNPLUGIN( "CNATFWStunPlugin::SetSendingStateL start" )
    iStunConnectionHandler->SetSendingStateL( aLocalCandidate, aState,
        aDestAddr );
    __STUNPLUGIN( "CNATFWStunPlugin::SetSendingStateL end" )
    }


// ---------------------------------------------------------------------------
// From class CNATFWPluginApi.
// CNATFWStunPlugin::GetConnectionIdL
// ---------------------------------------------------------------------------
//
void CNATFWStunPlugin::GetConnectionIdL(
    const CNATFWCandidate& aLocalCandidate,
    TUint& aConnectionId )
    {
    __STUNPLUGIN( "CNATFWStunPlugin::GetConnectionIdL start" )
    iStunConnectionHandler->GetConnectionIdL( aLocalCandidate, 
        aLocalCandidate.StreamId(), aConnectionId );
    __STUNPLUGIN( "CNATFWStunPlugin::GetConnectionIdL end" )
    }


// ---------------------------------------------------------------------------
// From class CNATFWPluginApi.
// CNATFWStunPlugin::PluginIdentifier
// ---------------------------------------------------------------------------
//
const TDesC8& CNATFWStunPlugin::PluginIdentifier() const
    {
    return *iPluginId;
    }

// End of file
