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

#include <commdb.h>

#include "CTcIAPManager.h"

EXPORT_C CTcIAPManager* CTcIAPManager::NewL()
	{
	CTcIAPManager* self = new( ELeave ) CTcIAPManager();

	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop();

	return self;
	}

EXPORT_C CTcIAPManager::~CTcIAPManager()
	{
	}

CTcIAPManager::CTcIAPManager()
	: iEntries( 1 )
	{
	}

void CTcIAPManager::ConstructL()
	{
	// Open Comms DB
	CCommsDatabase* commsDb = CCommsDatabase::NewL();
	CleanupStack::PushL( commsDb );

	// Get the IAP table
	CCommsDbTableView* ispTable = commsDb->OpenTableLC( TPtrC( IAP ) );

	// Loop through IAP entries
	TInt res = ispTable->GotoFirstRecord();
	while( res == KErrNone )
		{
		// Fetch Name and Id for each entry and store then into an array
		TEntry iapEntry;
		TBuf< KCommsDbSvrMaxColumnNameLength > tmpName;
		ispTable->ReadUintL( TPtrC( COMMDB_ID ), iapEntry.iId );
		ispTable->ReadTextL( TPtrC( COMMDB_NAME ), tmpName );
		iapEntry.iName.Copy( tmpName );	// unicode to narrow conversion
		iEntries.AppendL( iapEntry );

		// Get next record in table
		res = ispTable->GotoNextRecord();
		}

	CleanupStack::PopAndDestroy( 2 );	// ispTable, commsDb
	}

TInt CTcIAPManager::MdcaCount() const
	{
	return Count();
	}

TPtrC8 CTcIAPManager::MdcaPoint( TInt aIndex ) const
	{
	return TPtrC8( Name( aIndex ) );
	}

EXPORT_C TInt CTcIAPManager::Count() const
	{
	return iEntries.Count();
	}

EXPORT_C TUint32 CTcIAPManager::Id( TInt aIndex ) const
	{
	return iEntries[ aIndex ].iId;
	}

EXPORT_C const TDesC8& CTcIAPManager::Name( TInt aIndex ) const
	{
	return iEntries[ aIndex ].iName;
	}

EXPORT_C const TDesC8& CTcIAPManager::NameForIdL( TUint32 aId ) const
	{
	TInt count = iEntries.Count();
	for( TInt i = 0; i < count; i++ )
		{
		const TEntry& entry = iEntries[ i ];
		if( entry.iId == aId )
			{
			return entry.iName;
			}
		}

	User::Leave( KErrNotFound );
	return KNullDesC8;	// keep compiler happy
	}

EXPORT_C TUint32 CTcIAPManager::IdForNameL( const TDesC8& aName ) const
	{
	TInt count = iEntries.Count();
	for( TInt i = 0; i < count; i++ )
		{
		const TEntry& entry = iEntries[ i ];
		if( entry.iName.CompareF( aName ) == 0 )
			{
			return entry.iId;
			}
		}

	User::Leave( KErrNotFound );
	return 0;	// keep compiler happy
	}
