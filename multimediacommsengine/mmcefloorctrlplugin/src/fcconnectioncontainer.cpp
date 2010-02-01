/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Floor Control
*
*/




#include "fcconnectioncontainer.h"
#include "fcconnectionnotifier.h"

// -----------------------------------------------------------------------------
// CFCFCConnectionContainer::NewL
// -----------------------------------------------------------------------------
//
CFCConnectionContainer* CFCConnectionContainer::NewL( MFCConnectionNotifier& aNotifier )
	{	
	CFCConnectionContainer* self = 
		new( ELeave ) CFCConnectionContainer( aNotifier );
	return self;
	}

// -----------------------------------------------------------------------------
// CFCFCConnectionContainer::CFCFCConnectionContainer
// -----------------------------------------------------------------------------
//
CFCConnectionContainer::CFCConnectionContainer( MFCConnectionNotifier& aNotifier ) :
	CActive( CActive::EPriorityStandard ),
	iNotifier( aNotifier )
	{
	CActiveScheduler::Add( this );
	}
	
// -----------------------------------------------------------------------------
// CFCConnectionContainer::~CFCConnectionContainer
// -----------------------------------------------------------------------------
//
CFCConnectionContainer::~CFCConnectionContainer()
	{
	
	iConnection.Close();	
	Cancel();
	}

// -----------------------------------------------------------------------------
// CFCConnectionContainer::Connection
// -----------------------------------------------------------------------------
//
RConnection& CFCConnectionContainer::Connection()
	{
	return iConnection;
	}
// -----------------------------------------------------------------------------
// CFCConnectionContainer::RunL
// -----------------------------------------------------------------------------
//
void CFCConnectionContainer::RunL()
	{
	TInt status = iStatus.Int();
	if (status != KErrCancel)
		{
		iNotifier.ConnectionStarted(status );		
		}
	}

// -----------------------------------------------------------------------------
// CFCConnectionContainer::DoCancel
// -----------------------------------------------------------------------------
//
void CFCConnectionContainer::DoCancel()
	{
	iConnection.Close();
	// RConnection doesn't cancel an outstanding request at Close()
	// so we'll have to it "manually" here
	if( iStatus.Int() == KRequestPending )
		{
		TRequestStatus* status = &iStatus;
		User::RequestComplete( status, KErrCancel );
		}
	}

// -----------------------------------------------------------------------------
// CFCConnectionContainer::OpenConnection
// -----------------------------------------------------------------------------
//
TInt CFCConnectionContainer::OpenConnection( TUint32 aIapId,
										   RSocketServ& aServer )
	{
	TInt err = KErrNone;
	if( !IsActive() )
		{
		// If iConnection is already open don't reopen it.
		if( !iConnection.SubSessionHandle() )
			{
			err = iConnection.Open( aServer );
			}
		if (!err)
			{
			// Set connection preferences
			iPrefs.SetDialogPreference( ECommDbDialogPrefDoNotPrompt );
			iPrefs.SetDirection( ECommDbConnectionDirectionOutgoing );
			iPrefs.SetIapId( aIapId );
			// Start connecting
			iConnection.Start( iPrefs, iStatus );
			SetActive();
			}
		}
	return err;
	}

// -----------------------------------------------------------------------------
// CFCConnectionContainer::CloseConnection
// -----------------------------------------------------------------------------
//
void CFCConnectionContainer::CloseConnection()
	{
	iConnection.Close();	
	Cancel();
	}
