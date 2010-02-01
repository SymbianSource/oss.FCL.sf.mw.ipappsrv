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




#include <ecom/ecom.h>
#include "mceservercore.h"
#include "mceclientserver.h"
#include "mcecsserver.h"
#include "mcecsserverclosetimer.h"
#include "mceactivescheduler.h"
#include "mcecssession.h"
#include "cleanupresetanddestroy.h"
#include "mcesipmanager.h"
#include "fcpluginengine.h"
#include "mcesrvlogs.h"

const TUint KServerCloseWaitTime = 500; // milliseconds

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
    MCESRV_DEBUG("CMceServerCore::ConstructL, Entry");

    MCESRV_DEBUG("MCE Server starting");
    
    iActiveScheduler = new (ELeave) CMceActiveScheduler ();
    CActiveScheduler::Install (iActiveScheduler);
	iCloseTimer = CMceCsServerCloseTimer::NewL();
	iManager = CMceSipManager::NewL( *this );
	iServer = CMceCsServer::NewL(*this);
	iFCPlugInEngine = CFCPlugInEngine::NewL( *this );  
	
    MCESRV_DEBUG("MCE Server READY");
	
    MCESRV_DEBUG("CMceServerCore::ConstructL, Exit");
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
    MCESRV_DEBUG("CMceServerCore::~CMceServerCore, Entry");
	
	// This must be deleted first to remove MCE server from system's
	// list of servers asap.
	delete iServer;
    
	iFCSessions.Reset();
	iFCSessions.Close();
	delete iFCPlugInEngine;
	delete iCloseTimer;
	delete iManager;
    delete iActiveScheduler;
	CActiveScheduler::Install (NULL);
    MCESRV_DEBUG("MCE Server stopped");
    
    MCESRV_DEBUG("CMceServerCore::~CMceServerCore, Exit");
    
	}

// -----------------------------------------------------------------------------
// CMceServerCore::IncrementSessions
// -----------------------------------------------------------------------------
//
void CMceServerCore::IncrementSessions ()
	{
	if (iCloseTimer->IsActive())
        {
        iCloseTimer->Cancel();
        }
    iSessionCount++;
	}

// -----------------------------------------------------------------------------
// CMceServerCore::DecrementSessions
// -----------------------------------------------------------------------------
//
void CMceServerCore::DecrementSessions ()
	{
	if (iSessionCount > 0)
    	{
    	iSessionCount--;    
    	}
	if (iSessionCount == 0 && iManager->OrphanSubSessionCount() == 0)
		{
		iCloseTimer->StopActiveSchedulerAfter(KServerCloseWaitTime);
		}
	}

// -----------------------------------------------------------------------------
// CMceServerCore::InactiveServerCleanup
// -----------------------------------------------------------------------------
//
void CMceServerCore::InactiveServerCleanup()
    {
    if ( iSessionCount == 0 && iManager->OrphanSubSessionCount() == 0)
		{
		iCloseTimer->StopActiveSchedulerAfter(KServerCloseWaitTime);
		} 
    }

// -----------------------------------------------------------------------------
// CMceServerCore::Server
// -----------------------------------------------------------------------------
//
CServer2& CMceServerCore::Server () const
	{
	return *iServer;
	}

// -----------------------------------------------------------------------------
// CMceServerCore::Manager
// -----------------------------------------------------------------------------
//
CMceSipManager& CMceServerCore::Manager() const
    {
    return *iManager;
    }

// -----------------------------------------------------------------------------
// CMceServerCore::FCPluginEngine
// -----------------------------------------------------------------------------
//
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
    TMceFCSessionItem item(aFCObserver, aFCSession);
    iFCSessions.AppendL(item);
    }

// -----------------------------------------------------------------------------
// CMceServerCore::DeregisterFCSession
// -----------------------------------------------------------------------------
//
void CMceServerCore::DeregisterFCSession( MFCObserver& aFCObserver, 
                                          const CFCSession& aFCSession )
    {
    TInt count = iFCSessions.Count() - 1;
    for (int i = count; i > 0; i--)
    	{
    	TMceFCSessionItem item = iFCSessions[i];
    	if (&item.iFCObserver == &aFCObserver && &item.iFCSession == &aFCSession)
    		{
    		iFCSessions.Remove(i);
    		}
    	(void)item;	
    	}
    }

// -----------------------------------------------------------------------------
// CMceServerCore::FCReceivedData
// -----------------------------------------------------------------------------
//
void CMceServerCore::FCReceivedData( HBufC8* aData, 
                                     const CFCSession* aFCSession )
    {
    TBool found = EFalse;
    TInt ind = 0;
    while ( !found && ind < iFCSessions.Count() )
        {
        TMceFCSessionItem item = iFCSessions[ ind ];
        if ( &item.iFCSession == aFCSession )
            {
            item.iFCObserver.FCReceivedData( aData, aFCSession );
            found = ETrue;
            }
        }
    }

// -----------------------------------------------------------------------------
// CMceServerCore::FCErrorNotify
// -----------------------------------------------------------------------------
//
void CMceServerCore::FCErrorNotify( TInt aErrCode, 
                                    const CFCSession* aFCSession )
    {
    TBool found = EFalse;
    TInt ind = 0;
    while ( !found && ind < iFCSessions.Count() )
        {
        TMceFCSessionItem item = iFCSessions[ ind ];
        if ( &item.iFCSession == aFCSession )
            {
            item.iFCObserver.FCErrorNotify( aErrCode, aFCSession );
            found = ETrue;
            }
        }
    }

//  End of File
