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




#include "mncmconnectionmultiplexer.h"
#include <e32std.h>
#include <e32base.h>
#include "natfwcandidate.h"
#include "natfwconnectionhandler.h"

#include "natfwpluginlogs.h"
#include "natfwcandidate.h"
#include "natfwcandidatepair.h"
#include "mnatfwpluginobserver.h"

const TUint32 KErrorTestAddress = INET_ADDR( 255, 255, 255, 255 );
const TUint32 KLocalAddress = INET_ADDR( 100, 100, 100, 100 );
const TUint KLocalPort = 1000;
const TUint KTimerVal = 1000000; //1000ms


// ======== MEMBER FUNCTIONS ========


// ---------------------------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------------------------
//
CNATFWConnectionHandler* CNATFWConnectionHandler::NewL( 
    MNATFWPluginObserver* aPluginObserver, CNATFWPluginApi& aPlugin )
    {
    __NATPLUGIN( "CNATFWConnectionHandler::NewL" )
    CNATFWConnectionHandler* self = 
        new( ELeave ) CNATFWConnectionHandler( aPluginObserver, aPlugin );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// C++ default constructor
// ---------------------------------------------------------------------------
//
CNATFWConnectionHandler::CNATFWConnectionHandler(
    MNATFWPluginObserver* aPluginObserver,
    CNATFWPluginApi& aPlugin ) :
    iPluginObserver(aPluginObserver), iPlugin( aPlugin ),
    CActive( CActive::EPriorityStandard )
    {
    CActiveScheduler::Add( this );
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CNATFWConnectionHandler::~CNATFWConnectionHandler()
    {
    delete iDomain;
    iConnMux = NULL;
    delete iCandidate;    
    delete iCandidatePair;
    Cancel();
    iTimer.Cancel();
    iTimer.Close();
    
    }


// ---------------------------------------------------------------------------
// Symbian constructor
// Creates NATFWStunConnectionHandler.
// ---------------------------------------------------------------------------
//
void CNATFWConnectionHandler::ConstructL( )
    {
    User ::LeaveIfError(iTimer.CreateLocal()) ;
    }


// ---------------------------------------------------------------------------
// Initializes STUN Connection Handler.
// ---------------------------------------------------------------------------
//
void CNATFWConnectionHandler::PluginInitializeL(
    TUint32 aIapId,
    const TDesC8& aDomain,
    MNcmConnectionMultiplexer& aMultiplexer )
    {
    iConnMux = &aMultiplexer;
    iDomain = aDomain.AllocL();
    iIapId = aIapId;
    }
    
// ---------------------------------------------------------------------------
// NAT FW Client calls to fetch candidate. Forwards request to 
// Stun Connection Handler.
// ---------------------------------------------------------------------------
//
void CNATFWConnectionHandler::FetchCandidateL( TUint aStreamId )
    {
    iStreamId = aStreamId;
    
    iState = EFetchCandidate;
        
    TInetAddr publicAddr( KLocalAddress, KLocalPort );
    
    delete iCandidate;
    iCandidate = NULL;
    
    iCandidate = CNATFWCandidate::NewL();
    
    //Set candidate parameters
    iCandidate->SetStreamId( aStreamId );
    iCandidate->SetType( CNATFWCandidate::EServerReflexive );
    iCandidate->SetTransportAddrL( publicAddr );
    
    if ( iFetchQueue == 0 )
        {
        iTimer.After( iStatus, TTimeIntervalMicroSeconds32( KTimerVal ) );
        SetActive();
        }
    iFetchQueue++;
    }


// ---------------------------------------------------------------------------
// CNATFWConnectionHandler::FetchCandidatesL
// ---------------------------------------------------------------------------
//
void CNATFWConnectionHandler::FetchCandidatesL( TUint aStreamId )
    {
    iStreamId = aStreamId;
    
    iState = EFetchCandidates;
        
    TInetAddr publicAddr( KLocalAddress, KLocalPort );
    
    delete iCandidate;
    iCandidate = NULL;
    
    iCandidate = CNATFWCandidate::NewL();
    
    //Set candidate parameters
    iCandidate->SetStreamId( aStreamId );
    iCandidate->SetType( CNATFWCandidate::EServerReflexive );
    iCandidate->SetTransportAddrL( publicAddr );
    
    iTimer.After( iStatus, TTimeIntervalMicroSeconds32( KTimerVal ) );
    SetActive();
    }


// ---------------------------------------------------------------------------
// CNATFWConnectionHandler::PerformConnectivityChecksL
// ---------------------------------------------------------------------------
//
void CNATFWConnectionHandler::PerformConnectivityChecksL()
    {
    const TUint KStreamId = 99;
    
    iStreamId = KStreamId;
    TInetAddr publicAddr( KLocalAddress, KLocalPort );
    
    iState = EConnectivityChecks;
    
    CNATFWCandidate* remoteCand = CNATFWCandidate::NewLC();
    CNATFWCandidate* localCand = CNATFWCandidate::NewLC();
    
    //Set candidate parameters
    remoteCand->SetStreamId( KStreamId );
    remoteCand->SetType( CNATFWCandidate::EServerReflexive );
    remoteCand->SetTransportAddrL( publicAddr );
    
    localCand->SetStreamId( KStreamId );
    localCand->SetType( CNATFWCandidate::EServerReflexive );
    localCand->SetTransportAddrL( publicAddr );
    
    delete iCandidatePair;
    iCandidatePair = NULL;
    
    iCandidatePair = CNATFWCandidatePair::NewL( *localCand, *remoteCand );
    
    CleanupStack::PopAndDestroy( 2 );
    
    iTimer.After( iStatus, TTimeIntervalMicroSeconds32( KTimerVal ) );
    SetActive();
    }


// ---------------------------------------------------------------------------
// Connect. Not in use.
// ---------------------------------------------------------------------------
//
void CNATFWConnectionHandler::ConnectL()
    {
    User::Leave( KErrNotSupported );
    }


// ---------------------------------------------------------------------------
// Notifies the Plug-in's client of stream's current sending status.
// ---------------------------------------------------------------------------
//
void CNATFWConnectionHandler::SetReceivingStateL( TUint aStreamId,
        TNATFWStreamingState aState )
    {
    iStreamId = aStreamId;
    iStreamingStatus = aState;
    iState = ESetReceivingState;
    
    iTimer.After( iStatus, TTimeIntervalMicroSeconds32( KTimerVal ) );
    SetActive();
    }


// ---------------------------------------------------------------------------
// Notifies the Plug-in's client of stream's current sending status.
// ---------------------------------------------------------------------------
//
void CNATFWConnectionHandler::SetSendingStateL( TUint aStreamId,
                                          const TInetAddr& aDestAddr,
                                          TNATFWStreamingState aState )
    {
    iStreamId = aStreamId;
    iStreamingStatus = aState;
    
    if ( KErrorTestAddress == aDestAddr.Address() )
        {
        iState = EErrorTestAddress;
        }
    else
        {
        iState = ESetSendingState;
        }

    iTimer.After( iStatus, TTimeIntervalMicroSeconds32( KTimerVal ) );
    SetActive();
    }

// ---------------------------------------------------------------------------
// CNATFWConnectionHandler::RunL
// ---------------------------------------------------------------------------
//
void CNATFWConnectionHandler::RunL()
    {
    switch( iState )
        {
        case EFetchCandidate:
            iPluginObserver->NewLocalCandidateFound( iPlugin, iCandidate );
            iPluginObserver->Notify( iPlugin, iStreamId, 
                MNATFWPluginObserver::EFetchingCompleted, KErrNone );
            
            iFetchQueue--;
            
            if ( iFetchQueue > 0 )
                {
                iTimer.After( iStatus,
                    TTimeIntervalMicroSeconds32( KTimerVal ) );
                SetActive();
                iState = EFetchCandidate;
                }
            else
                {
                iState = EIdle;
                }
            break;
            
        case EFetchCandidates:
            iPluginObserver->NewLocalCandidateFound( iPlugin, iCandidate );
            iPluginObserver->Notify( iPlugin, iStreamId, 
                MNATFWPluginObserver::EFetchingCompleted, KErrNone );
            iState = EIdle;
            break;
        case EConnectivityChecks:
            iPluginObserver->NewCandidatePairFound( iPlugin, iCandidatePair );
            iPluginObserver->Notify( iPlugin, iStreamId, 
                MNATFWPluginObserver::EConnChecksCompleted, KErrNone );
            iState = EIdle;
            break;
        case ESetReceivingState:
            if ( EStreamingStateActive == iStreamingStatus )
                {
                iPluginObserver->Notify( iPlugin, iStreamId, 
                    MNATFWPluginObserver::EReceivingActivated, KErrNone );
                }
            else
                {
                iPluginObserver->Notify( iPlugin, iStreamId, 
                    MNATFWPluginObserver::EReceivingDeactivated, KErrNone );
                }
            iState = EIdle;
            break;
            
        case ESetSendingState:
            if (  EStreamingStateActive == iStreamingStatus )
                {
                iPluginObserver->Notify( iPlugin, iStreamId, 
                    MNATFWPluginObserver::ESendingActivated, KErrNone );
                }
            else
                {
                iPluginObserver->Notify( iPlugin, iStreamId, 
                    MNATFWPluginObserver::ESendingDeactivated, KErrNone );
                }
            iState = EIdle;
            break;
        case EErrorTestAddress:
            iPluginObserver->Error( iPlugin, iStreamId, KErrArgument );
            iState = EIdle;
            break;
        default:
            iState = EIdle;
            break;
        }
    }

// ---------------------------------------------------------------------------
// CNATFWConnectionHandler::DoCancel
// ---------------------------------------------------------------------------
//    
void CNATFWConnectionHandler::DoCancel()
    {
    iTimer.Cancel();
    }
    
// End of file