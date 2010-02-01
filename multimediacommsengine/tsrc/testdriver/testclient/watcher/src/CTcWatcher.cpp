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

#include "CTcWatcher.h"
#include "WatcherConstants.h"

EXPORT_C CTcWatcher* CTcWatcher::NewL( const TDesC& aAppName, TInt aTimeout )
	{
	CTcWatcher* self = new( ELeave ) CTcWatcher;

	CleanupStack::PushL( self );
	self->ConstructL( aAppName, aTimeout );
	CleanupStack::Pop();

	return self;
	}

EXPORT_C CTcWatcher::~CTcWatcher()
	{
	Cancel();
	iTimer.Close();
	iWatcher.Close();
	}

CTcWatcher::CTcWatcher()
	: CActive( EPriorityHigh )
	{
	CActiveScheduler::Add( this );
	}

void CTcWatcher::ConstructL( const TDesC& aAppName, TInt aTimeout )
	{
	User::LeaveIfError( iTimer.CreateLocal() );
	User::LeaveIfError( iWatcher.Connect( aAppName, aTimeout * KSecondAsMicroseconds ) );

	iTimeout = aTimeout * KSecondAsMicroseconds / 2;
	iTimer.After( iStatus, iTimeout );
	SetActive();
	}

void CTcWatcher::DoCancel()
	{
	iTimer.Cancel();
	}

void CTcWatcher::RunL()
	{
	iWatcher.Ping();

	iTimer.After( iStatus, iTimeout );
	SetActive();
	}
