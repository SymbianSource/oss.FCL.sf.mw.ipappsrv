/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation
*
*/

#include "CTcWatcherSession.h"
#include "CTcWatcherServer.h"
#include "CTcCoroner.h"
#include "CTcGrimreaper.h"
#include "WatcherConstants.h"

CTcWatcherSession* CTcWatcherSession::NewL(
#ifndef __IPC_V2_PRESENT__
 											RThread& aClient,
#endif
 										    CTcWatcherServer* aServer )
	{
	CTcWatcherSession* self =
#ifdef __IPC_V2_PRESENT__
							  new( ELeave ) CTcWatcherSession;
#else
							  new( ELeave ) CTcWatcherSession( aClient );
#endif

	CleanupStack::PushL( self );
	self->ConstructL( aServer );
	CleanupStack::Pop();

	return self;
	}

CTcWatcherSession::~CTcWatcherSession()
	{
	delete iCoroner;
	delete iGrimreaper;
	iWatcherServer->SessionRemoved();
	}

CTcWatcherSession::CTcWatcherSession()
	{
	}

void CTcWatcherSession::ConstructL( CTcWatcherServer* aServer )
	{
#ifndef __IPC_V2_PRESENT__
	CreateL( *aServer );
#endif

	iWatcherServer = aServer;
	iWatcherServer->SessionAdded();
	}

#ifdef __IPC_V2_PRESENT__
void CTcWatcherSession::ServiceL( const RMessage2& aMessage )
#else
void CTcWatcherSession::ServiceL( const RMessage& aMessage )
#endif
	{
	// Any leaves from this function will be catched by the Active Scheduler
	// and forwarded to CTcWatcher::RunError().

	// Check for session related requests
	TInt status( KErrNone );
	switch( aMessage.Function() )
		{
		case ECloseSession:
			{
			// Session destruction is handled
			// by the client-server framework.
			delete iCoroner;
			iCoroner = NULL;
			delete iGrimreaper;
			iGrimreaper = NULL;
			break;
			}
		case EConfigAndStart:
			{
			RThread clientThread;
#ifdef __IPC_V2_PRESENT__
			aMessage.Client( clientThread );
			CleanupClosePushL( clientThread );

			aMessage.ReadL( 0, iAppName );
			iCoroner = CTcCoroner::NewL( clientThread, iAppName, *iWatcherServer );
			iGrimreaper = CTcGrimreaper::NewL( clientThread, aMessage.Int1() );
			CleanupStack::PopAndDestroy();
#else
			aMessage.ReadL( aMessage.Ptr0(), iAppName );
			iCoroner = CTcCoroner::NewL( Client(), iAppName, *iWatcherServer );
			iGrimreaper = CTcGrimreaper::NewL( Client(), aMessage.Int1() );
#endif
			break;
			}
		case EPing:
			{
			iGrimreaper->NotYet();
			break;
			}
		default:
			{
			// Unknown request
			status = KErrUnknown;
			}
		}

	aMessage.Complete( status );
	}
