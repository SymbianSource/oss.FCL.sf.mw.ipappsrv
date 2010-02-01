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





#include "mceclientserver.h"
#include "mcecsserver.h"
#include "mceservercore.h"
#include "mcecssession.h"
#include "mcecsplatsecpolicy.h"

// -----------------------------------------------------------------------------
// CMceCsServer::NewL
// -----------------------------------------------------------------------------
//
CMceCsServer* CMceCsServer::NewL (CMceServerCore& aServerCore)
	{
    CMceCsServer* self = CMceCsServer::NewLC(aServerCore);
    CleanupStack::Pop(self);
    return self;
	}

// -----------------------------------------------------------------------------
// CMceCsServer::NewLC
// -----------------------------------------------------------------------------
//
CMceCsServer* CMceCsServer::NewLC (CMceServerCore& aServerCore)
	{
	CMceCsServer* self = new(ELeave)CMceCsServer(EPriorityHigh,aServerCore);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
	}

// -----------------------------------------------------------------------------
// CMceCsServer::ConstructL
// -----------------------------------------------------------------------------
//
void CMceCsServer::ConstructL ()
	{
    StartL(KMceServerName);
	}

// -----------------------------------------------------------------------------
// CMceCsServer::CMceCsServer
// -----------------------------------------------------------------------------
//
CMceCsServer::CMceCsServer (TInt aPriority, CMceServerCore& aServerCore)
 : CPolicyServer(aPriority, TMceCSPlatSecPolicy),
   iServerCore(aServerCore)
	{
	}

// -----------------------------------------------------------------------------
// CMceCsServer::~CMceCsServer
// -----------------------------------------------------------------------------
//
CMceCsServer::~CMceCsServer ()
	{
	}

// -----------------------------------------------------------------------------
// CMceCsServer::NewSessionL
// -----------------------------------------------------------------------------
//
CSession2* CMceCsServer::NewSessionL (const TVersion &aVersion,
                                             const RMessage2& /*aMessage*/) const
	{
	// check we're the right version
	if (!User::QueryVersionSupported (TVersion (KMceServerMajorVersionNumber,
                                                KMceServerMinorVersionNumber,
                                                KMceServerBuildVersionNumber),
                                                aVersion))
		{
		User::Leave(KErrNotSupported);
		}
	// make new session
    return CMceCsSession::NewL( iServerCore );
	}

// -----------------------------------------------------------------------------
// CMceCsServer::RunError
// -----------------------------------------------------------------------------
//
TInt CMceCsServer::RunError(TInt aError)
	{
	if (aError == KErrBadDescriptor)
		{
        // A bad descriptor error implies a badly programmed client, 
        // so panic it; otherwise report the error to the client
		Message().Panic(KMceServerPanic, EBadDescriptor);
		}
	else
		{
		Message().Complete(aError);
		}
	//
	// The leave will result in an early return from CServer::RunL(), skipping
	// the call to request another message. So do that now in order to keep the
	// server running.
	ReStart();
	return KErrNone; // handled the error fully
	}
