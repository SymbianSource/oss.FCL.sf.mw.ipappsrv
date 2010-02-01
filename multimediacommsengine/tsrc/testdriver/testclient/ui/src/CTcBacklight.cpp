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
* Description:  Implementation.
*
*/

#include "CTcBacklight.h"
#include "TestClientConstants.h"

CTcBacklight* CTcBacklight::NewL()
	{
	CTcBacklight* self = new( ELeave ) CTcBacklight;

	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop();

	return self;
	}

CTcBacklight::~CTcBacklight()
	{
	Cancel();
	iTimer.Close();
	}

CTcBacklight::CTcBacklight()
	: CActive( EPriorityStandard )
	{
	CActiveScheduler::Add( this );
	}

void CTcBacklight::ConstructL()
	{
	User::LeaveIfError( iTimer.CreateLocal() );
	Start();
	}

void CTcBacklight::DoCancel()
	{
	iTimer.Cancel();
	}

void CTcBacklight::RunL()
	{
	User::ResetInactivityTime();
	Start();
	}

void CTcBacklight::Start()
	{
	iTimer.After( iStatus, KPeriod10Sec );
	SetActive();
	}
