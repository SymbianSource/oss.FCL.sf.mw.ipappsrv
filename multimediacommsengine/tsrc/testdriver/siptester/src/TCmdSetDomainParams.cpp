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
#include "CTcSIPContext.h"
#include "TCmdSetDomainParams.h"
#include "SIPConstants.h"


/**
 *
 * If value KErrNotFound (-1) is given in KParamUNSAFSharedSecretNotSupported or 
 * KParamUNSAFBindingRequestFailed, the corresponding value is removed from CR
 *
 * INPUT:
 *   Headers:		-
 *   Parameters:	KParamDomainName, KParamUNSAFSharedSecretNotSupported*,
 *                  KParamUNSAFBindingRequestFailed*
 *   IDs:			-
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			-
 */
void TCmdSetDomainParams::ExecuteL()
	{
	TPtrC8 proxy = ExtractTextL( KParamDomainName, ETrue );
	
	TInt sharedSecretNotSupported = ExtractIntegerL( KParamUNSAFSharedSecretNotSupported, KErrGeneral, EFalse );
	
	TInt bindingRequestFailed = ExtractIntegerL( KParamUNSAFBindingRequestFailed, KErrGeneral, EFalse );

	CRepository* repository = CRepository::NewLC( KCRUidUNSAFProtocols );
	TUint32 key( 0 );
	TInt status = FindDomainKeyL( *repository, proxy, key );
	__ASSERT_ALWAYS( status == KErrNone || status == KErrNotFound,
					 User::Leave( status ) );

	User::LeaveIfError(
		repository->StartTransaction(
			CRepository::EConcurrentReadWriteTransaction ) );
    repository->CleanupCancelTransactionPushL();

	if ( status == KErrNotFound )
		{
		key = CreateNewKeyL( *repository,
                             KUNSAFProtocolsDomainMask,
                             KUNSAFProtocolsDomainTableMask );
    	WriteL( *repository, KUNSAFProtocolsDomainMask | key, proxy );
		}

	
	if ( sharedSecretNotSupported >= 0 ) 	
	    {
		WriteL( *repository, 
		        KUNSAFProtocolsDomainSharedSecretNotSupported | key, 
		        sharedSecretNotSupported );
		}
    if ( sharedSecretNotSupported == KErrNotFound )
        {
       	RemoveParameterL( *repository,
					      KUNSAFProtocolsDomainSharedSecretNotSupported | key );
        }
        
    if ( bindingRequestFailed >= 0 ) 	
	    {
		WriteL( *repository, 
		        KUNSAFProtocolsDomainBindingRequestFailed | key, 
		        bindingRequestFailed );
		}
    if ( bindingRequestFailed == KErrNotFound )
        {
       	RemoveParameterL( *repository,
					      KUNSAFProtocolsDomainBindingRequestFailed | key );
        }

	TUint32 dummy( 0 );
    User::LeaveIfError( repository->CommitTransaction( dummy ) );
    CleanupStack::Pop( 1 ); // transaction

	CleanupStack::PopAndDestroy( repository );	
	}

TBool TCmdSetDomainParams::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("SetDomainParams") );	
	}

TTcCommandBase* TCmdSetDomainParams::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdSetDomainParams( aContext );
	}
