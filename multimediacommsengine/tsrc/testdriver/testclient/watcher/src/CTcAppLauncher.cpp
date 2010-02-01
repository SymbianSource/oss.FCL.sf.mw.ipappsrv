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

#include <apgcli.h>
#include <apacmdln.h>
#include <w32std.h>

#include "CTcAppLauncher.h"
#include "WatcherConstants.h"

#include "debuglog.h"

CTcAppLauncher* CTcAppLauncher::NewLC( const TDesC& aAppName,
									   TBool aDismissDialog,
									   CArrayPtr< CTcAppLauncher >& aOwner )
	{
	CTcAppLauncher* self = new( ELeave ) CTcAppLauncher( aAppName,
														 aDismissDialog,
														 aOwner );

	CleanupStack::PushL( self );
	self->ConstructL();

	return self;
	}

CTcAppLauncher::~CTcAppLauncher()
	{
	Cancel();
	iTimer.Close();
	TInt count = iOwner.Count();
	for( TInt i = 0; i < count; i++ )
		{
		if( iOwner[ i ] == this )
			{
			iOwner.Delete( i );
			break;
			}
		}
	}

CTcAppLauncher::CTcAppLauncher( const TDesC& aAppName,
								TBool aDismissDialog,
								CArrayPtr< CTcAppLauncher >& aOwner )
	: CActive( EPriorityHigh ),
	  iAppName( aAppName ),
	  iOwner( aOwner ),
	  iDismissDialog( aDismissDialog )
	{
	CActiveScheduler::Add( this );
	}

void CTcAppLauncher::ConstructL()
	{
	// Create and start timer
	User::LeaveIfError( iTimer.CreateLocal() );
	iTimer.After( iStatus, KSecondAsMicroseconds );
	SetActive();
	}

void CTcAppLauncher::DoCancel()
	{
	iTimer.Cancel();
	}

void CTcAppLauncher::RunL()
	{
	// Fatal error occurred, report
	User::LeaveIfError( iStatus.Int() );

	if( iDismissDialog )
		{
		TKeyEvent event;
		event.iCode = EKeyDevice3;
		RWsSession wsSession;
		User::LeaveIfError( wsSession.Connect() );
		wsSession.SimulateKeyEvent( event );
		wsSession.Close();
		}

	RApaLsSession apparc;
	User::LeaveIfError( apparc.Connect() );
	CleanupClosePushL( apparc );

	CApaCommandLine* cmdLine = CApaCommandLine::NewLC();
	cmdLine->SetExecutableNameL( iAppName );
	cmdLine->SetCommandL( EApaCommandRun );

	User::LeaveIfError( apparc.StartApp( *cmdLine ) );
	CleanupStack::PopAndDestroy( 2 );

	// Write log entry
	LOG( _L("[APPLAUNCHER] Client restarted.") );
	delete this;
	}

