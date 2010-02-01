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

#include "CTcStructure.h"

EXPORT_C CTcStructure* CTcStructure::NewLC()
	{
	CTcStructure* self = new( ELeave ) CTcStructure;
	CleanupStack::PushL( self );
	return self;
	}

EXPORT_C CTcStructure::~CTcStructure()
	{
	iArray.ResetAndDestroy();
	}

CTcStructure::CTcStructure()
	: iArray( KTcGranularity )
	{
	}

MTcTyped::TParameterType CTcStructure::ParameterType() const
	{
	return MTcTyped::EStructure;
	}

TPtrC8 CTcStructure::Name() const
	{
	return TPtrC8( iName );
	}

EXPORT_C TInt CTcStructure::Count() const
	{
	return iArray.Count();
	}

EXPORT_C void CTcStructure::SetName( const TDesC8& aName )
	{
	iName = aName;
	}

EXPORT_C const CTcNameValue& CTcStructure::Item( TInt aIndex ) const
	{
	return *iArray[ aIndex ];
	}

EXPORT_C CTcNameValue* CTcStructure::ItemL( TInt aIndex, 
											TBool aMandatory )
	{
	if( ( aIndex < 0 ) || ( aIndex >= iArray.Count() ) )
		{
		if( aMandatory )
			{
			User::Leave( KTcErrMandatoryStructureItemNotFound );
			}
		return NULL;
		}

	return iArray[ aIndex ];
	}

EXPORT_C CTcNameValue* CTcStructure::Item( const TDesC8& aName )
	{
	TInt count = iArray.Count();
	for( TInt i = 0; i < count; i++ )
		{
		if( iArray[ i ]->Name().CompareF( aName ) == 0 )
			{
			return iArray[ i ];
			}
		}
	return NULL;
	}

EXPORT_C CTcNameValue* CTcStructure::ItemL( const TDesC8& aName,
											TBool aMandatory )
	{
	CTcNameValue* item = Item( aName );
	if( aMandatory && !item )
		{
		User::Leave( KTcErrMandatoryStructureItemNotFound );
		}
	return item;
	}


EXPORT_C void CTcStructure::AddItemL( CTcNameValue* aItem )
	{
	__ASSERT_ALWAYS( aItem != NULL, User::Leave( KErrArgument ) );
	iArray.AppendL( aItem );
	}

EXPORT_C void CTcStructure::AddItemL( const TDesC8& aName, const TDesC8& aValue )
	{
	CTcNameValue* item = CTcNameValue::NewLC();
	item->SetL( aName, aValue );
	iArray.AppendL( item );
	CleanupStack::Pop( item );
	}

EXPORT_C void CTcStructure::AddItemL( const TDesC8& aName, TInt aValue )
	{
	CTcNameValue* item = CTcNameValue::NewLC();
	item->SetL( aName, aValue );
	iArray.AppendL( item );
	CleanupStack::Pop( item );
	}
	
EXPORT_C void CTcStructure::AddBooleanItemL( const TDesC8& aName, TBool aValue )
	{
	CTcNameValue* item = CTcNameValue::NewLC();
	item->SetBooleanL( aName, aValue );
	iArray.AppendL( item );
	CleanupStack::Pop( item );
	}
