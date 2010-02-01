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

#include "CTcCoroner.h"
#include "MTcDeathObserver.h"
#include "WatcherConstants.h"
#include "debuglog.h"

CTcCoroner* CTcCoroner::NewL( const RThread& aClient,  const TDesC& aAppName,
							  MTcDeathObserver& aObserver )
	{
	CTcCoroner* self = new( ELeave ) CTcCoroner( aObserver, aAppName );

	CleanupStack::PushL( self );
	self->ConstructL( aClient );
	CleanupStack::Pop();

	return self;
	}

CTcCoroner::~CTcCoroner()
	{
	Cancel();
	iClient.Close();
	}

CTcCoroner::CTcCoroner( MTcDeathObserver& aObserver, const TDesC& aAppName )
	: CActive( EPriorityHigh ),
	  iAppName( aAppName ),
	  iObserver( aObserver )
	{
	CActiveScheduler::Add( this );
	}

void CTcCoroner::ConstructL( const RThread& aClient )
	{
	// Initialize local client thread handle
	User::LeaveIfError( iClient.Open( aClient.Id() ) );

	// Request for client death notifications
	iClient.Logon( iStatus );
	SetActive();
	}

void CTcCoroner::DoCancel()
	{
	iClient.LogonCancel( iStatus );
	}

void CTcCoroner::RunL()
	{
	TBuf< 16 > exitType;
	TBool dialogPresent( EFalse );
	switch( iClient.ExitType() )
		{
		case EExitKill:
			{
			exitType = _L("was killed");
			break;
			}
		case EExitPanic:
			{
			dialogPresent = ETrue;
			exitType = _L("panicked");
			break;
			}
		case EExitPending:
			{
			LOG( _L("[CORONER] Thread logon failed.") );
			return;
			}
		default:
			{
			dialogPresent = ETrue;
			exitType = _L("terminated");
			break;
			}
		}

	LOG( _L("[CORONER] Client \"%s\" %s, reason %s:%d"),
						iAppName.PtrZ(), exitType.PtrZ(),
						iClient.ExitCategory().PtrZ(), iClient.ExitReason() );

	iObserver.Died( iAppName, dialogPresent );
	}
