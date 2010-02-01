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

#include "CTcParameterList.h"

EXPORT_C CTcParameterList::CTcParameterList()
	: iParameters( KTcGranularity )
	{
	}

EXPORT_C CTcParameterList::~CTcParameterList()
	{
	Reset();
	}

EXPORT_C TInt CTcParameterList::Count() const
	{
	return iParameters.Count();
	}

EXPORT_C MTcTyped::TParameterType CTcParameterList::ParameterType(
															TInt aIndex ) const
	{
	// This function is used a safeguard in many places:
	// report unknown type if index is invalid to avoid array panics.
	if( ( aIndex < 0 ) || ( aIndex >= Count() ) )
		{
		return MTcTyped::EUnknown;
		}
	return iParameters[ aIndex ]->ParameterType();
	}

EXPORT_C CTcArray& CTcParameterList::AsArrayL( TInt aIndex ) const
	{
	__ASSERT_ALWAYS( ParameterType( aIndex ) == MTcTyped::EArray,
					 User::Leave( KTcErrRequestCorrupt ) );
	return AsArray( aIndex );
	}

EXPORT_C CTcArray& CTcParameterList::AsArray( TInt aIndex ) const
	{
	return static_cast< CTcArray& >( *iParameters[ aIndex ] );
	}

EXPORT_C CTcStructure& CTcParameterList::AsStructureL( TInt aIndex ) const
	{
	__ASSERT_ALWAYS( ParameterType( aIndex ) == MTcTyped::EStructure,
					 User::Leave( KTcErrRequestCorrupt ) );
	return AsStructure( aIndex );
	}

EXPORT_C CTcStructure& CTcParameterList::AsStructure( TInt aIndex ) const
	{
	return static_cast< CTcStructure& >( *iParameters[ aIndex ] );
	}

EXPORT_C CTcNameValue& CTcParameterList::AsNameValueL( TInt aIndex ) const
	{
	__ASSERT_ALWAYS( ParameterType( aIndex ) == MTcTyped::ENameValue,
					 User::Leave( KTcErrRequestCorrupt ) );
	return AsNameValue( aIndex );
	}

EXPORT_C CTcNameValue& CTcParameterList::AsNameValue( TInt aIndex ) const
	{
	return static_cast< CTcNameValue& >( *iParameters[ aIndex ] );
	}

EXPORT_C void CTcParameterList::AddParameterL( MTcTyped* aItem )
	{
	iParameters.AppendL( aItem );
	}

EXPORT_C void CTcParameterList::InsertParameterL( MTcTyped* aItem,
												  TInt aIndex )
	{
	iParameters.InsertL( aIndex, aItem );
	}

EXPORT_C void CTcParameterList::Reset()
	{
	iParameters.ResetAndDestroy();
	}

EXPORT_C TInt CTcParameterList::FindParameter(
										const TDesC8& aName,
										MTcTyped::TParameterType aType ) const
	{
	TInt count = iParameters.Count();
	for( TInt i = 0; i < count; i++ )
		{
		MTcTyped* param = iParameters[ i ];
		if( ( aType == MTcTyped::EUnknown ) ||
			( aType == param->ParameterType() ) )
			{
			if( param->Name().CompareF( aName ) == 0 )
				{
				return i;
				}
			}
		}

	return KErrNotFound;
	}

EXPORT_C void CTcParameterList::RemoveParameter(
										const TDesC8& aName,
										MTcTyped::TParameterType aType )
	{
	TInt pos = FindParameter( aName, aType );
	if( pos != KErrNotFound )
		{
		MTcTyped* item = iParameters.At( pos );
		iParameters.Delete( pos );
		delete item;
		}
	}
