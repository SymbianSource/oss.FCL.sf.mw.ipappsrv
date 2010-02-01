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

#include "CTcGrimreaper.h"
#include "WatcherConstants.h"
#include "debuglog.h"

CTcGrimreaper* CTcGrimreaper::NewL( const RThread& aClient, TInt aTimeout )
	{
	CTcGrimreaper* self = new( ELeave ) CTcGrimreaper( aTimeout );

	CleanupStack::PushL( self );
	self->ConstructL( aClient );
	CleanupStack::Pop();

	return self;
	}

CTcGrimreaper::~CTcGrimreaper()
	{
	Cancel();
	iTimer.Close();
	iClient.Close();
	}

CTcGrimreaper::CTcGrimreaper( TInt aTimeout )
	: CActive( EPriorityHigh ),
	  iTimeout( aTimeout )
	{
	CActiveScheduler::Add( this );
	}

void CTcGrimreaper::ConstructL( const RThread& aClient )
	{
	// Initialize local client thread handle
	User::LeaveIfError( iClient.Open( aClient.Id() ) );

	// Create and start timer
	User::LeaveIfError( iTimer.CreateLocal() );
	NotYet();
	}

void CTcGrimreaper::DoCancel()
	{
	iTimer.Cancel();
	}

void CTcGrimreaper::RunL()
	{
	// Fatal error occurred, report
	User::LeaveIfError( iStatus.Int() );

	// Go for the kill, time has ran out.
	iClient.Kill( KErrTimedOut );

	// Write log entry
	LOG( _L("[GRIMREAPER] Client did not report within timeout period. Killed.") );			
	}

void CTcGrimreaper::NotYet()
	{
	Cancel();
	iTimer.After( iStatus, iTimeout );
	SetActive();
	}
