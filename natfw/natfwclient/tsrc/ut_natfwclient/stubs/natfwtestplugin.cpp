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
* Description:    Implements NAT Plug-in interface, which client uses to send
*                STUN requests. Forwards client requests to Stun Connection
*                Handler.
*
*/




#include "natfwtestplugin.h"
#include "natfwconnectionhandler.h"
#include "natfwpluginlogs.h"
#include "natfwcandidate.h"
#include "mnatfwpluginobserver.h"

_LIT8( KNatPluginIdNokiaStun, "nokia.stun" );


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ default constructor
// ---------------------------------------------------------------------------
//
CNATFWTestPlugin::CNATFWTestPlugin()
    {
    }


// ---------------------------------------------------------------------------
// Symbian constructor
// Creates NATFWStunConnectionHandler.
// ---------------------------------------------------------------------------
//
void CNATFWTestPlugin::ConstructL( TAny* aInitParams )
    {
    __NATPLUGIN( "CNATFWTestPlugin::ConstructL start" )

    __ASSERT_ALWAYS( aInitParams, User::Leave( KErrArgument ) );

    iPluginId = KNatPluginIdNokiaStun().AllocL();
    
    CNATFWPluginApi::TNATFWPluginInitParams* initParams = 
        static_cast<CNATFWPluginApi::TNATFWPluginInitParams*>( 
            aInitParams );

    iConnectionHandler = CNATFWConnectionHandler::NewL( 
        &( initParams->iObserver ), *this );

    iConnectionHandler->PluginInitializeL( initParams->iIapId,
                                           initParams->iDomain,
                                           initParams->iMultiplexer );
    
    __NATPLUGIN( "CNATFWTestPlugin::ConstructL end" )
    }


// ---------------------------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------------------------
//
CNATFWTestPlugin* CNATFWTestPlugin::NewL( TAny* aInitParams )
    {
    __NATPLUGIN( "CNATFWTestPlugin::NewL start" )
    CNATFWTestPlugin* self = new ( ELeave ) CNATFWTestPlugin();
    CleanupStack::PushL( self );
    self->ConstructL( aInitParams );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CNATFWTestPlugin::~CNATFWTestPlugin()
    {
    delete iPluginId;
    delete iConnectionHandler;
    }


// ---------------------------------------------------------------------------
// CNATFWTestPlugin::ConnectServerL
// ---------------------------------------------------------------------------
//
     
void CNATFWTestPlugin::ConnectServerL( const RSocketServ& /*aSocketServ*/,
        const TName& /*aConnectionName*/ )
    {
    __NATPLUGIN( "CNATFWTestPlugin::ConnectServerL" )
    }
    
    
// ---------------------------------------------------------------------------
// NAT FW Client calls to fetch candidate. Forwards request to 
// Stun Connection Handler.
// ---------------------------------------------------------------------------
//
void CNATFWTestPlugin::FetchCandidateL( TUint aStreamId, TUint /*aRtoValue*/,
        TUint /*aAddrFamily*/ )
    {
    __NATPLUGIN( "CNATFWTestPlugin::FetchCandidateL" )
    iConnectionHandler->FetchCandidateL( aStreamId );
    }


// ---------------------------------------------------------------------------
// CNATFWTestPlugin::FetchCandidatesL
// ---------------------------------------------------------------------------
//
void CNATFWTestPlugin::FetchCandidatesL( TUint aStreamCollectionId,
    TUint aStreamId, TUint /*aComponentId*/, TUint /*aAddrFamily*/ )
    {
    __NATPLUGIN( "CNATFWTestPlugin::FetchCandidatesL" )
    
    if ( 99 == aStreamCollectionId )
        {
        User::Leave( KErrGeneral );
        }
    iConnectionHandler->FetchCandidatesL( aStreamId );
    }


// ---------------------------------------------------------------------------
// Connect. Not in use.
// ---------------------------------------------------------------------------
//
void CNATFWTestPlugin::ConnectL()
    {
    __NATPLUGIN( "CNATFWTestPlugin::ConnectL" )
    User::Leave( KErrNotSupported );
    }


// ---------------------------------------------------------------------------
// Notifies the Plug-in's client of stream's current sending status.
// ---------------------------------------------------------------------------
//
void CNATFWTestPlugin::SetSendingStateL( TUint aStreamId,
                                          const TInetAddr& aDestAddr,
                                          TNATFWStreamingState aState )
    {
    __NATPLUGIN( "CNATFWTestPlugin::SetSendingStateL start" )
    iConnectionHandler->SetSendingStateL( aStreamId, aDestAddr, aState );
    __NATPLUGIN( "CNATFWTestPlugin::SetSendingStateL end" )
    }


// ---------------------------------------------------------------------------
// From class CNATFWPluginApi.
// CNATFWTestPlugin::SetReceivingStateL
// ---------------------------------------------------------------------------
//
void CNATFWTestPlugin::SetReceivingStateL( 
        const CNATFWCandidate& aLocalCandidate,
        TNATFWStreamingState aState )
    {
    __NATPLUGIN( "CNATFWTestPlugin::SetSendingStateL start" )
    iConnectionHandler->SetReceivingStateL( aLocalCandidate.StreamId(),
        aState );
    __NATPLUGIN( "CNATFWTestPlugin::SetSendingStateL end" )
    }


// ---------------------------------------------------------------------------
// From class CNATFWPluginApi.
// CNATFWTestPlugin::SetSendingStateL
// ---------------------------------------------------------------------------
//
void CNATFWTestPlugin::SetSendingStateL( 
        const CNATFWCandidate& aLocalCandidate,
        TNATFWStreamingState aState, const TInetAddr& aDestAddr )
    {
    __NATPLUGIN( "CNATFWTestPlugin::SetSendingStateL start" )
    iConnectionHandler->SetSendingStateL( aLocalCandidate.StreamId(), 
        aDestAddr, aState );
    __NATPLUGIN( "CNATFWTestPlugin::SetSendingStateL end" )
    }

// ---------------------------------------------------------------------------
// From class CNATFWPluginApi.
// CNATFWTestPlugin::PerformConnectivityChecksL
// ---------------------------------------------------------------------------
//
void CNATFWTestPlugin::PerformConnectivityChecksL( 
    RPointerArray<CNATFWCandidate>& /*aRemoteCandidates*/ )
    {
    __NATPLUGIN( "CNATFWTestPlugin::PerformConnectivityChecksL" )
    iConnectionHandler->PerformConnectivityChecksL();
    }


// ---------------------------------------------------------------------------
// From class CNATFWPluginApi.
// CNATFWTestPlugin::UpdateIceProcessingL
// ---------------------------------------------------------------------------
//
void CNATFWTestPlugin::UpdateIceProcessingL( 
        RPointerArray<CNATFWCandidatePair>& /*aPeerSelectedPairs*/ )
    {
    __NATPLUGIN( "CNATFWTestPlugin::UpdateIceProcessingL" )
    }


// ---------------------------------------------------------------------------
// From class CNATFWPluginApi.
// CNATFWTestPlugin::UpdateIceProcessingL
// ---------------------------------------------------------------------------
//
void CNATFWTestPlugin::UpdateIceProcessingL( 
        RPointerArray<CNATFWCandidate>& /*aRemoteCands*/ )
    {
    __NATPLUGIN( "CNATFWTestPlugin::UpdateIceProcessingL" )
    }

// ---------------------------------------------------------------------------
// From class CNATFWPluginApi.
// CNATFWTestPlugin::SetCredentialsL
// ---------------------------------------------------------------------------
//
void CNATFWTestPlugin::SetCredentialsL(
        const CNATFWCandidate& /*aCandidate*/,
        const CNATFWCredentials& /*aCredentials*/ )
    {
    __NATPLUGIN( "CNATFWTestPlugin::SetCredentialsL" )
    }

// ---------------------------------------------------------------------------
// From class CNATFWPluginApi.
// CNATFWTestPlugin::GetConnectionIdL
// ---------------------------------------------------------------------------
//
void CNATFWTestPlugin::GetConnectionIdL( 
        const CNATFWCandidate& /*aLocalCandidate*/,
        TUint& /*aConnectionId*/ )
    {
    __NATPLUGIN( "CNATFWTestPlugin::GetConnectionIdL" )
    }


// ---------------------------------------------------------------------------
// From class CNATFWPluginApi.
// CNATFWTestPlugin::PluginIdentifier
// ---------------------------------------------------------------------------
//
const TDesC8& CNATFWTestPlugin::PluginIdentifier() const
    {
    __NATPLUGIN( "CNATFWTestPlugin::PluginIdentifier" )
    return *iPluginId;
    }

// ---------------------------------------------------------------------------
// From class CNATFWPluginApi.
// CNATFWTestPlugin::PluginIdentifier
// ---------------------------------------------------------------------------
//
void CNATFWTestPlugin::SetRoleL( TNATFWIceRole /*aRole*/ )
    {
    __NATPLUGIN( "CNATFWTestPlugin::SetRoleL" )
    }

// End of file
