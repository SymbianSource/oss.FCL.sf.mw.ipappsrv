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
#include "TCmdEnableCRLFRefresh.h"
#include "SIPConstants.h"


/**
 * If KParamUNSAFEnableCRLFRefresh is not given, then the configured value is
 * removed from the repository.
 *
 * INPUT:
 *   Headers:		-
 *   Parameters:	KParamDomainName, KParamUNSAFEnableCRLFRefresh
 *   IDs:			-
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			-
 */
void TCmdEnableCRLFRefresh::ExecuteL()
	{
	TPtrC8 proxy = ExtractTextL( KParamDomainName, ETrue );
	TBool disable( EFalse );
	TRAPD( err, disable = ExtractBooleanL( KParamUNSAFEnableCRLFRefresh,
										   ETrue ) );
	__ASSERT_ALWAYS( err == KErrNone || err == KTcErrMandatoryParameterNotFound,
					 User::Leave( err ) );	

	CRepository* repository = CRepository::NewLC( KCRUidUNSAFProtocols );
	TUint32 key( 0 );
	TInt status = FindDomainKeyL( *repository, proxy, key );
	__ASSERT_ALWAYS( status == KErrNone || status == KErrNotFound,
					 User::Leave( status ) );

	User::LeaveIfError(
		repository->StartTransaction(
			CRepository::EConcurrentReadWriteTransaction ) );
    repository->CleanupCancelTransactionPushL();

	if ( err == KErrNone )
		{
		if ( status == KErrNotFound )
			{
			key = CreateNewKeyL( *repository,
	                             KUNSAFProtocolsDomainMask,
	                             KUNSAFProtocolsDomainTableMask );
	    	WriteL( *repository, KUNSAFProtocolsDomainMask | key, proxy );
			}

		const TInt KDisable = 1;
		const TInt KEnable = 0;
	    WriteL( *repository,
	            KUNSAFProtocolsDomainEnableCRLFRefresh | key,
	            disable ? KDisable : KEnable );
		}
	else
		{
		if ( status == KErrNone )
			{
			RemoveParameterL( *repository,
							  KUNSAFProtocolsDomainEnableCRLFRefresh | key );
			}
		}

	TUint32 dummy( 0 );
    User::LeaveIfError( repository->CommitTransaction( dummy ) );
    CleanupStack::Pop( 1 ); // transaction

	CleanupStack::PopAndDestroy( repository );	
	}

TBool TCmdEnableCRLFRefresh::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("EnableCRLFRefresh") );	
	}

TTcCommandBase* TCmdEnableCRLFRefresh::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdEnableCRLFRefresh( aContext );
	}
