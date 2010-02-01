/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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


#include <centralrepository.h>
#include "TCmdRemoveAllUserFromClientResolverCenRep.h"
#include "SIPConstants.h"
#include "CTcSIPContext.h"
#include "sipclientresolverconfigcrkeys.h"

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	ApplicationUid, ClientMatchType
 *   IDs:			-
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			-
 */
void TCmdRemoveAllUserFromClientResolverCenRep::ExecuteL()
	{
	RArray<TUint32> keys;
    CleanupClosePushL(keys);
	TInt err = iContext.Repository().FindL(KSIPClientResolverUserNameMask,
                                KSIPClientResolverFieldTypeMask,
                                keys);

	for (TInt i=0; !err && i < keys.Count(); i++)
        {
        TUint32 key = (KSIPClientResolverUserNameMask^(keys[i]));       
        // Read the related plug-in UID
        TUint32 uidKey = KSIPClientResolverPluginUIDMask|key;
        TInt uidValue(0);
        User::LeaveIfError(iContext.Repository().Get(uidKey,uidValue));
        // Read the related client UID
        TUint32 clientUidKey = KSIPClientResolverClientUIDMask|key;
        TInt clientUidValue(0);
        User::LeaveIfError(iContext.Repository().Get(clientUidKey,clientUidValue));
        
        iContext.Repository().Delete( clientUidKey );
        iContext.Repository().Delete( uidKey );
        iContext.Repository().Delete( KSIPClientResolverUserNameMask|key );
        }
    CleanupStack::PopAndDestroy(&keys);
	}

// -----------------------------------------------------------------------------
// TCmdRemoveAllUserFromClientResolverCenRep::Match
// -----------------------------------------------------------------------------
//	
TBool TCmdRemoveAllUserFromClientResolverCenRep::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("RemoveAllUserFromClientResolverCenRep") );
	}

// -----------------------------------------------------------------------------
// TCmdRemoveAllUserFromClientResolverCenRep::CreateL
// -----------------------------------------------------------------------------
//	
TTcCommandBase* TCmdRemoveAllUserFromClientResolverCenRep::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdRemoveAllUserFromClientResolverCenRep( aContext );
	}

