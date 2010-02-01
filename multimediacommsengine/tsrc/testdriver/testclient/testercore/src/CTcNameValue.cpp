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

#include "CTcNameValue.h"
#include "TesterConstants.h"

EXPORT_C CTcNameValue* CTcNameValue::NewLC()
	{
	CTcNameValue* self = new( ELeave ) CTcNameValue;
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}

EXPORT_C CTcNameValue::~CTcNameValue()
	{
	delete iName;
	delete iValue;
	}

CTcNameValue::CTcNameValue()
	{
	}

void CTcNameValue::ConstructL()
	{
	// Create emtpy descriptor for default values
	iName = KNullDesC8().AllocL();
	iValue = KNullDesC8().AllocL();
	}

MTcTyped::TParameterType CTcNameValue::ParameterType() const
	{
	return MTcTyped::ENameValue;
	}

TPtrC8 CTcNameValue::Name() const
	{
	return iName->Des();
	}

EXPORT_C void CTcNameValue::SetL( const TDesC8& aName, const TDesC8& aValue )
	{
	SetNameL( aName );
	SetValueL( aValue );
	}

EXPORT_C void CTcNameValue::SetL( const TDesC8& aName, TInt aValue )
	{
	SetNameL( aName );
	SetValueL( aValue );
	}
	
EXPORT_C void CTcNameValue::SetBooleanL( const TDesC8& aName, TBool aValue )
	{
	SetNameL( aName );
	SetBooleanValueL( aValue );
	}

EXPORT_C void CTcNameValue::SetNameL( const TDesC8& aName )
	{
	// Allocate a copy first, so that we have a valid iName even
	// if AllocL() fails.
	HBufC8* newName = aName.AllocL();
	delete iName;
	iName = newName;
	}

EXPORT_C void CTcNameValue::SetValueL( const TDesC8& aValue )
	{
	// Allocate a copy first, so that we have a valid iValue even
	// if AllocL() fails.
	HBufC8* newValue = aValue.AllocL();
	delete iValue;
	iValue = newValue;
	}

EXPORT_C void CTcNameValue::SetValueL( TInt aValue )
	{
	TBuf8< KTcMaxIntConversion > convert;
	convert.AppendNum( aValue );
	SetValueL( convert );
	}
	
EXPORT_C void CTcNameValue::SetBooleanValueL( TBool aValue )
	{
	if ( aValue )
	    {
	    SetValueL( KTcTrue );
	    }
	else
	    {
	    SetValueL( KTcFalse );
	    }
	}

EXPORT_C TPtrC8 CTcNameValue::Value() const
	{
	return iValue->Des();
	}

EXPORT_C TInt CTcNameValue::ValueAsInt() const
	{
	TInt value( 0 );
	TLex8 lexer( Value() );
	lexer.Val( value );	// ignores error

	return value;
	}

EXPORT_C TInt CTcNameValue::ValueAsIntL() const
    {
    TInt value( 0 );
    TLex8 lexer( Value() );
    User::LeaveIfError( lexer.Val( value ) );
    
    return value;
    }
    
EXPORT_C TBool CTcNameValue::ValueAsBooleanL() const
    {
    TPtrC8 value = Value();
	if( value == KTcTrue )
		{
		return ETrue;
		}	
    if ( value == KTcFalse )
        {
        return EFalse;
        }     
	User::Leave( KErrNotFound );
	return EFalse;
    }
	
EXPORT_C TInt CTcNameValue::NameAsIntL() const
    {
    TInt name( 0 );
    TLex8 lexer( Name() );
    User::LeaveIfError( lexer.Val( name ) );
    
    return name;
    }
