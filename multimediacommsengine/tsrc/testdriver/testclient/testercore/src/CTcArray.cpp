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

#include "CTcArray.h"

EXPORT_C CTcArray* CTcArray::NewLC()
	{
	CTcArray* self = new( ELeave ) CTcArray;
	CleanupStack::PushL( self );
	return self;
	}

EXPORT_C CTcArray::~CTcArray()
	{
	}

CTcArray::CTcArray()
	: iArray( KTcGranularity )
	{
	}

MTcTyped::TParameterType CTcArray::ParameterType() const
	{
	return MTcTyped::EArray;
	}

TPtrC8 CTcArray::Name() const
	{
	return TPtrC8( iName );
	}

EXPORT_C TInt CTcArray::Count() const
	{
	return iArray.MdcaCount();
	}

EXPORT_C void CTcArray::SetName( const TDesC8& aName )
	{
	iName = aName;
	}

EXPORT_C TPtrC8 CTcArray::Item( TInt aIndex ) const
	{
	return iArray.MdcaPoint( aIndex );
	}

EXPORT_C TInt CTcArray::ItemAsInt( TInt aIndex ) const
	{
	TInt value( 0 );
	TLex8 lexer( iArray.MdcaPoint( aIndex ) );
	lexer.Val( value );	// ignores error

	return value;
	}

EXPORT_C void CTcArray::AddItemL( const TDesC8& aItem )
	{
	iArray.AppendL( aItem );
	}

EXPORT_C void CTcArray::AddItemL( TInt aItem )
	{
	TBuf8< KTcMaxIntConversion > convert;
	convert.AppendNum( aItem );
	iArray.AppendL( convert );
	}

EXPORT_C void CTcArray::AddItemsL( const MDesC8Array& aItems )
	{
	TInt count = aItems.MdcaCount();
	for( TInt i = 0; i < count; i++ )
		{
		iArray.AppendL( aItems.MdcaPoint( i ) );
		}
	}

EXPORT_C void CTcArray::RemoveItem( TInt aIndex )
	{
	iArray.Delete( aIndex );
	iArray.Compress();
	}

EXPORT_C const MDesC8Array& CTcArray::AsMDesCArray() const
	{
	return iArray;
	}
