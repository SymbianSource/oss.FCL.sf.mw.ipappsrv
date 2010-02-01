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
#include "TCmdSetProperty.h"
#include "CTcMCEContext.h"
#include "mcetesterpskeys.h"

#include <e32property.h>


void TCmdSetProperty::ExecuteL()
	{
	// ---------- Setup --------------------------------------------------------

    // Getting Uids and value
    TInt uidValue = ExtractIntegerL( KParamPropertyUid, 0, EFalse );
    TInt key = ExtractIntegerL( KParamPropertyKey, 0, ETrue );

    TInt propertyAttr = RProperty::EText;
    TInt intValue( 0 );
    TPtrC8 descrValue = ExtractTextL( KParamPropertyTextValue, EFalse );
    if ( descrValue == KNullDesC8 )
    	{
    	// As text parameter was not specified, integer parameter is now
		// mandatory, except for KMceTesterNatPluginResourceReservationStatus
		// which does not require a value.
		TBool isMandatory = ( key != KMceTesterNatPluginResourceReservationStatus );
		intValue = ExtractIntegerL( KParamPropertyValue, 0, isMandatory );

    	propertyAttr = RProperty::EInt;
    	}

    TUid uidkey = KUidMceTesterPSKeys;
    if ( uidValue != 0 )
        {
        uidkey = TUid::Uid( uidValue );
        }

    // ---------- Execution ----------------------------------------------------

    _LIT_SECURITY_POLICY_PASS( KAllowAllPolicy );
    User::LeaveIfError( RProperty::Define( uidkey, key, propertyAttr,
    									   KAllowAllPolicy, KAllowAllPolicy ) );
    if ( propertyAttr == RProperty::EInt )
    	{
    	User::LeaveIfError( RProperty::Set( uidkey, key, intValue ) );
    	}
    else
    	{
    	User::LeaveIfError( RProperty::Set( uidkey, key, descrValue ) );
    	}
    
    // ---------- Response creation --------------------------------------------

    }
	
TBool TCmdSetProperty::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("SetProperty") );
	}

TTcCommandBase* TCmdSetProperty::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdSetProperty( aContext );
	}
