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
#include "TCmdAddUserToClientResolverCenRep.h"
#include "SIPConstants.h"
#include "CTcSIPContext.h"
#include "sipclientresolverconfigcrkeys.h"

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	UserName, PluginUid, ApplicationUid
 *   IDs:			-
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			-
 */
void TCmdAddUserToClientResolverCenRep::ExecuteL()
	{
	HBufC8* userNameBuf = ExtractHBufLC( KParamClientUserName );
	TUid pluginUid = ExtractUidL( KParamPluginUid, ETrue );
	TUid clientUid = ExtractUidL( KParamApplicationUid, ETrue );
    
	AddUidL( iContext.Repository(),*userNameBuf,pluginUid,clientUid );
	CleanupStack::PopAndDestroy( userNameBuf );
	}
	
// -----------------------------------------------------------------------------
// TCmdAddUserToClientResolverCenRep::AddUidL
// -----------------------------------------------------------------------------
//	
void TCmdAddUserToClientResolverCenRep::AddUidL(
    CRepository& aRepository,
    const TDesC8& aUserName,
    const TUid& aPluginUid,
    const TUid& aClientUid)
    {
    TUint32 key = CreateNewKeyL( aRepository,KSIPClientResolverUserNameMask );
    
    User::LeaveIfError(
        aRepository.Create(key|KSIPClientResolverUserNameMask,aUserName));
    
    TInt pluginUidValue( aPluginUid.iUid );                 
    User::LeaveIfError(
        aRepository.Create(key|KSIPClientResolverPluginUIDMask,pluginUidValue));
        
    TInt clientUidValue( aClientUid.iUid );                 
    User::LeaveIfError(
        aRepository.Create(key|KSIPClientResolverClientUIDMask,clientUidValue));
    }

// -----------------------------------------------------------------------------
// TCmdAddUserToClientResolverCenRep::CreateNewKeyL
// -----------------------------------------------------------------------------
// 
TUint32 
	TCmdAddUserToClientResolverCenRep::CreateNewKeyL( CRepository& aRepository,
    											   	  TUint32 aKey) const
    {
    TUint32 newKey = 0;
    RArray<TUint32> keys;
    CleanupClosePushL( keys );
    TInt err = aRepository.FindL( aKey,
                                  KSIPClientResolverFieldTypeMask,
                                  keys );
    TInt keyCount = keys.Count();
    if ( err == KErrNotFound )
        {
        newKey = 1;
        }
    else
        {
        User::LeaveIfError( err );
        if ( keyCount == 0 )
            {
            newKey = 1;
            }
        else
            {
            // Find the biggest key and increment it by one
            keys.SortUnsigned();
            TUint32 maxKey = aKey^keys[ keyCount - 1 ];
            newKey = maxKey + 1;       
            }
        }
    CleanupStack::PopAndDestroy( &keys );
    return newKey;
    }
	
	
	
// -----------------------------------------------------------------------------
// TCmdAddUserToClientResolverCenRep::Match
// -----------------------------------------------------------------------------
//
TBool TCmdAddUserToClientResolverCenRep::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("AddUserToClientResolverCenRep") );
	}

// -----------------------------------------------------------------------------
// TCmdAddUserToClientResolverCenRep::CreateL
// -----------------------------------------------------------------------------
//
TTcCommandBase* 
	TCmdAddUserToClientResolverCenRep::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdAddUserToClientResolverCenRep( aContext );
	}

