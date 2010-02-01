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

#include "CTcRegistry.h"

EXPORT_C CTcRegistry::CTcRegistry()
	: iRegistry( KTcGranularity )
	{
	}

EXPORT_C CTcRegistry::~CTcRegistry()
	{
	// Delete all items
	DestroyAll();
	}

EXPORT_C void CTcRegistry::AddObjectL( TAny* aObject )
	{
	// Create a name for the object using its address
	TBuf8< KTcMaxIntConversion > name;
	name.AppendNum( reinterpret_cast< TInt >( aObject ) );

	CleanupStack::PushL( aObject );
	AddObjectL( name, aObject, TObjectRef::EAny );
	CleanupStack::Pop( aObject );
	}

EXPORT_C void CTcRegistry::AddObjectL( CBase* aObject )
	{
	// Create a name for the object using its address
	TBuf8< KTcMaxIntConversion > name;
	name.AppendNum( reinterpret_cast< TInt >( aObject ) );

	CleanupStack::PushL( aObject );
	AddObjectL( name, aObject, TObjectRef::ECBase );
	CleanupStack::Pop( aObject );
	}

EXPORT_C void CTcRegistry::AddObjectL( const CBase& aObject )
	{
	// Cast away the const - to be in line with the rest of the objects..
	CBase* obj = const_cast< CBase* >( &aObject );

	// Create a name for the object using its address
	TBuf8< KTcMaxIntConversion > name;
	name.AppendNum( reinterpret_cast< TInt >( obj ) );

	AddObjectL( name, obj, TObjectRef::ECBaseExternal );
	}

EXPORT_C TBool CTcRegistry::IsRegistered( const TAny* aObject )
	{
	return ( Find( aObject ) != KErrNotFound );
	}

EXPORT_C TAny* CTcRegistry::ObjectPtrL( const TDesC8& aObjectName )
	{
	return iRegistry[ FindL( aObjectName ) ].iObject;
	}

EXPORT_C TPtrC8 CTcRegistry::ObjectNameL( const TAny* aObject ) const
	{
	return TPtrC8( iRegistry[ FindL( aObject ) ].iName );
	}

EXPORT_C void CTcRegistry::DestroyObject( const TDesC8& aObjectName )
	{
	TInt objIndex( 0 );
	TRAPD( err, objIndex = FindL( aObjectName ) );
	if( !err )
		{
		Delete( iRegistry[ objIndex ] );
		iRegistry.Delete( objIndex );
		iRegistry.Compress();
		}
	}

EXPORT_C void CTcRegistry::DestroyObject( TAny* aObject )
	{
	TInt objIndex( 0 );
	TRAPD( err, objIndex = FindL( aObject ) );
	if( !err )
		{
		Delete( iRegistry[ objIndex ] );
		iRegistry.Delete( objIndex );
		iRegistry.Compress();
		}
	}

EXPORT_C void CTcRegistry::DestroyAll()
	{
	// ResetAndDestroy() cannot be used here, so loop through the array
	// and delete each item separately.
	TInt count( iRegistry.Count() );
	for( TInt i = 0; i < count; i++ )
		{
		Delete( iRegistry[ i ] );
		}

	// Remove registry entries also
	iRegistry.Reset();
	}

EXPORT_C TInt CTcRegistry::Count() const
	{
	return iRegistry.Count();
	}

void CTcRegistry::AddObjectL( const TDesC8& aObjectName,
							  TAny* aObject, TObjectRef::TType aType )
	{
	TObjectRef ref;
	ref.iName = aObjectName;
	ref.iObject = aObject;
	ref.iType = aType;

	TKeyArrayFix key( _FOFF( TObjectRef, iName ), ECmpNormal8 );
	// It's ok if the item is already in the registry.
	TRAPD( err, iRegistry.InsertIsqL( ref, key ) );
	if( err != KErrAlreadyExists )
		{
		User::LeaveIfError( err );
		}
	}

TInt CTcRegistry::FindL( const TDesC8& aObjectName ) const
	{
	// Initialize a reference object with the specified name
	TObjectRef ref;
	ref.iName = aObjectName;

	// Search the array for the reference object
	TInt objIndex( 0 );
	TKeyArrayFix key( _FOFF( TObjectRef, iName ), ECmpNormal8 );
	if( iRegistry.FindIsq( ref, key, objIndex ) )
		{
		User::Leave( KTcErrObjectNotFound );
		}

	return objIndex;
	}

TInt CTcRegistry::FindL( const TAny* aObject ) const
	{
	TInt objIndex = Find( aObject );
	if( objIndex == KErrNotFound )
		{
		User::Leave( KTcErrObjectNotFound );
		}
	return objIndex;
	}

TInt CTcRegistry::Find( const TAny* aObject ) const
	{
	// Initialize a reference object with the specified pointer
	TObjectRef ref;
	ref.iObject = const_cast< TAny* >( aObject );

	// Search the array for the reference object
	TInt objIndex( 0 );
	TKeyArrayFix key( _FOFF( TObjectRef, iObject ), ECmpTInt32 );
	if( iRegistry.Find( ref, key, objIndex ) )
		{
		return KErrNotFound;
		}

	return objIndex;
	}

void CTcRegistry::Delete( TObjectRef& aObjRef )
	{
	if( aObjRef.iType == TObjectRef::ECBase )
		{
		// ensure that destructors are executed properly
		CBase* objC = reinterpret_cast< CBase* >( aObjRef.iObject );
		delete objC;
		}
    // Any object, used for T- and M-classes
	else if( aObjRef.iType == TObjectRef::EAny )
		{
		// ensure that destructors are executed properly
		CBase* objC = reinterpret_cast< CBase* >( aObjRef.iObject );
		delete objC;
		}
	// External object are not deleted, they are just removed from the registry
	else if( aObjRef.iType != TObjectRef::ECBaseExternal )
		{
		delete aObjRef.iObject;
		}

	aObjRef.iObject = NULL;
	}
