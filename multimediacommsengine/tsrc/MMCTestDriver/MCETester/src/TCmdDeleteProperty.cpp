/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
#include "TCmdDeleteProperty.h"
#include "CTcMCEContext.h"
#include "mcetesterpskeys.h"

#include <e32property.h>

void TCmdDeleteProperty::ExecuteL()
	{	
    // ---------- Setup --------------------------------------------------------
    
    //Getting Uids and value
    TInt iUid = ExtractIntegerL( KParamPropertyUid, 0, EFalse );
    TInt iKey = ExtractIntegerL( KParamPropertyKey, 0, ETrue );
    TUid uidkey = KUidMceTesterPSKeys;
    if (iUid!=0)
        {
        uidkey = TUid::Uid( iUid );
        }

    // ---------- Execution ----------------------------------------------------
    User::LeaveIfError(RProperty::Delete( uidkey, iKey ));
	
    
    // ---------- Response creation --------------------------------------------
	}
	
TBool TCmdDeleteProperty::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("DeleteProperty") );
	}

TTcCommandBase* TCmdDeleteProperty::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdDeleteProperty( aContext );
	}
