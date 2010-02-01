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




#include "mceservercore.h"
#include "mceclientserver.h"
#include "mcecsserver.h"
#include "mcecsserveritc.h"
#include "mcecssession.h"
#include "mcesipmanager.h"
#include "fcpluginengine.h"


// -----------------------------------------------------------------------------
// CMceServerCore::NewL
// -----------------------------------------------------------------------------
//
CMceServerCore* CMceServerCore::NewL ()
	{
    CMceServerCore* self = CMceServerCore::NewLC();
    CleanupStack::Pop(self);
    return self;
	}

// -----------------------------------------------------------------------------
// CMceServerCore::NewLC
// -----------------------------------------------------------------------------
//
CMceServerCore* CMceServerCore::NewLC ()
	{
	CMceServerCore* self = new(ELeave)CMceServerCore;
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
	}

// -----------------------------------------------------------------------------
// CMceServerCore::ConstructL
// -----------------------------------------------------------------------------
//
void CMceServerCore::ConstructL ()
	{
	iServer = CMceCsServer::NewL (*this);
	iManager = CMceSipManager::NewL( *this );
    iFCPlugInEngine = CFCPlugInEngine::NewL( *this ); 
	}

// -----------------------------------------------------------------------------
// CMceServerCore::CMceServerCore
// -----------------------------------------------------------------------------
//
CMceServerCore::CMceServerCore ()
	{
	}

// -----------------------------------------------------------------------------
// CMceServerCore::~CMceServerCore
// -----------------------------------------------------------------------------
//
CMceServerCore::~CMceServerCore ()
	{
	delete iServer;
	delete iManager;
    delete iFCPlugInEngine;
	  
	iFCSessions.Reset();
	iFCSessions.Close();
	}

// -----------------------------------------------------------------------------
// CMceServerCore::IncrementSessions
// -----------------------------------------------------------------------------
//
void CMceServerCore::IncrementSessions ()
	{
	}

// -----------------------------------------------------------------------------
// CMceServerCore::DecrementSessions
// -----------------------------------------------------------------------------
//
void CMceServerCore::DecrementSessions ()
	{
	}

// -----------------------------------------------------------------------------
// CMceServerCore::InactiveServerCleanup
// -----------------------------------------------------------------------------
//	
void CMceServerCore::InactiveServerCleanup()
    {
        
    }

// -----------------------------------------------------------------------------
// CMceServerCore::Server
// -----------------------------------------------------------------------------
//
CServer2& CMceServerCore::Server () const
	{
	return *iServer;
	}


     
CMceSipManager& CMceServerCore::Manager() const
	{
	return *iManager;
	}
CFCPlugInEngine& CMceServerCore::FCPluginEngine() const
	{
	return *iFCPlugInEngine;
	}


// -----------------------------------------------------------------------------
// CMceServerCore::RegisterFCSession
// -----------------------------------------------------------------------------
//
void CMceServerCore::RegisterFCSessionL( MFCObserver& aFCObserver, 
                                         const CFCSession& aFCSession )
    {
    TMceFCSessionItem item( aFCObserver, aFCSession );
    iFCSessions.AppendL( item );
    }

// -----------------------------------------------------------------------------
// CMceServerCore::DeregisterFCSession
// -----------------------------------------------------------------------------
//
void CMceServerCore::DeregisterFCSession( MFCObserver& /*aFCObserver*/, 
                                         const CFCSession& aFCSession )
    {
    TInt count = iFCSessions.Count() - 1;
    for (int i = count; i > 0; i--)
    	{
    	TMceFCSessionItem item = iFCSessions[i];
    	if (&item.iFCSession == &aFCSession)
    		{
    		iFCSessions.Remove(i);
    		}
    	}
    }


void CMceServerCore::FCReceivedData( HBufC8* /*aData*/, const CFCSession* /*aFCSession*/ )
    {
    
    }

void CMceServerCore::FCErrorNotify( TInt /*aErrCode*/, const CFCSession* /*aFCSession*/ )
    {
    
    }
