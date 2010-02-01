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
#include "TCmdSetSTUNServer.h"
#include "SIPConstants.h"


/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	KParamDomainName, KParamSTUNServerDomain,
 *					KParamSTUNServerPort
 *   IDs:			-
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			-
 *
 * If at least one of the parameters KParamSTUNServerDomain and
 * KParamSTUNServerPort is specified, they are written to repository.
 * If neither is specified, the configured values are removed from the
 * repository.
 */
void TCmdSetSTUNServer::ExecuteL()
	{
	TPtrC8 sipAOR = ExtractTextL( KParamDomainName, ETrue );
	TPtrC8 server = ExtractTextL( KParamSTUNServerDomain, EFalse );
	//UDP and TLS both use the same port
	TInt port = ExtractIntegerL( KParamSTUNServerPort, KErrNotFound, EFalse );
	TBool writeToRepository = ( server != KNullDesC8 || port > 0 );

	CRepository* repository = CRepository::NewLC( KCRUidUNSAFProtocols );
	TUint32 key( 0 );
	TInt status = FindDomainKeyL( *repository, sipAOR, key );
	__ASSERT_ALWAYS( status == KErrNone || status == KErrNotFound,
					 User::Leave( status ) );

	User::LeaveIfError(
		repository->StartTransaction(
			CRepository::EConcurrentReadWriteTransaction ) );
    repository->CleanupCancelTransactionPushL();

	if ( writeToRepository )
		{
		if ( status == KErrNotFound )
			{
			key = CreateNewKeyL( *repository,
	                             KUNSAFProtocolsDomainMask,
	                             KUNSAFProtocolsDomainTableMask );
	    	WriteL( *repository, KUNSAFProtocolsDomainMask | key, sipAOR );
			}
		WriteL( *repository, KUNSAFProtocolsSTUNServerMask | key, server );
	    WriteL( *repository, KUNSAFProtocolsSTUNServerPortMask | key, port );
		}
	else
		{
		if ( status == KErrNone )
			{
			RemoveParameterL( *repository,
							  KUNSAFProtocolsSTUNServerMask | key );
			RemoveParameterL( *repository,
							  KUNSAFProtocolsSTUNServerPortMask | key );
			}
		}

	TUint32 dummy( 0 );
    User::LeaveIfError( repository->CommitTransaction( dummy ) );
    CleanupStack::Pop( 1 ); // transaction

	CleanupStack::PopAndDestroy( repository );
	}

TBool TCmdSetSTUNServer::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("SetSTUNServer") );	
	}

TTcCommandBase* TCmdSetSTUNServer::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdSetSTUNServer( aContext );
	}
