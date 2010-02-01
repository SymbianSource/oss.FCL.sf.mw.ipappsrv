/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Implementation
*
*/



#include "MCEConstants.h"
#include "TCmdGetProperty.h"
#include "CTcMCEContext.h"
#include "mcetesterpskeys.h"

#include <e32property.h>


void TCmdGetProperty::ExecuteL()
	{
    // ---------- Setup --------------------------------------------------------

    //Getting Uids and value
    TInt uidValue = ExtractIntegerL( KParamPropertyUid, 0, EFalse );
    TInt key = ExtractIntegerL( KParamPropertyKey, 0, ETrue );
    TUid uidkey = KUidMceTesterPSKeys;
    if ( uidValue != 0 )
        {
        uidkey = TUid::Uid( uidValue );
        }


	// ---------- Execution ----------------------------------------------------	

	TBool intParameterUsed = ETrue;
    TInt intValue( 0 );
    HBufC8* descrValue( NULL );

    // Try to get as integer value
    TInt err = RProperty::Get( uidkey, key, intValue );
    if ( err == KErrArgument )
    	{
    	// Parameter was not integer, so it is text
    	intParameterUsed = EFalse;
    	descrValue = HBufC8::NewLC( KPropertyTextValueMaxLength );

		TPtr8 ptr( descrValue->Des() );
		User::LeaveIfError( RProperty::Get( uidkey, key, ptr ) );
    	}
    else
    	{
    	User::LeaveIfError( err );
    	}


	// ---------- Response creation --------------------------------------------

	if ( intParameterUsed )
		{
		AddIntegerResponseL( KResponsePropertyValue, intValue );	
		}
	else
		{
		AddTextResponseL( KResponsePropertyTextValue, *descrValue );
		CleanupStack::PopAndDestroy( descrValue );
		}
    }
	
TBool TCmdGetProperty::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("GetProperty") );
	}

TTcCommandBase* TCmdGetProperty::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdGetProperty( aContext );
	}
