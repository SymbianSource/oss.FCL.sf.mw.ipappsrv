/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
#include <unsafprotocolscrkeys.h>
#include "TCmdGetDomainParams.h"
#include "TCmdSetUNSAFTimers.h"
#include "SIPConstants.h"
#include "CTcSIPContext.h"

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	KParamDomainName
 *   IDs:			-
 *
 * OUTPUT:
 *   Parameters:	KParamUNSAFSharedSecretNotSupported,
 *					KParamUNSAFBindingRequestFailed
 *   IDs:			-
 *
 * If some parameter is not present in the repository, its value is returned as
 * KErrNotFound.
 *
 */
void TCmdGetDomainParams::ExecuteL()
	{
	TPtrC8 proxy = ExtractTextL( KParamDomainName, ETrue );
	
	CRepository* repository = CRepository::NewLC( KCRUidUNSAFProtocols );
	TUint32 key( 0 );
	
	TInt sharedSecretNotSupported( KErrNotFound );
	TInt bindingRequestFailed( KErrNotFound );
	
	User::LeaveIfError( FindDomainKeyL( *repository, proxy, key ) );
	
    Read( *repository,
    	  KUNSAFProtocolsDomainSharedSecretNotSupported | key,
          sharedSecretNotSupported );
	Read( *repository,
    	  KUNSAFProtocolsDomainBindingRequestFailed | key,
          bindingRequestFailed );
	
	CleanupStack::PopAndDestroy( repository );
	
	AddIntegerResponseL( KParamUNSAFSharedSecretNotSupported,
						 sharedSecretNotSupported );
						 
	AddIntegerResponseL( KParamUNSAFBindingRequestFailed,
						 bindingRequestFailed );
	}

TBool TCmdGetDomainParams::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("GetDomainParams") );
	}

TTcCommandBase* TCmdGetDomainParams::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdGetDomainParams( aContext );
	}
