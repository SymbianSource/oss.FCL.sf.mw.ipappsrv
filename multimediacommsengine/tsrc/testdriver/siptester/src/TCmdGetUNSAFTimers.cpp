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
#include "TCmdGetUNSAFTimers.h"
#include "TCmdSetUNSAFTimers.h"
#include "SIPConstants.h"
#include "CTcSIPContext.h"


/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	KParamIAPName, KParamDomainName
 *   IDs:			-
 *
 * OUTPUT:
 *   Parameters:	KParamUNSAFBindingRequestInterval,
 *					KParamUNSAFUDPKeepaliveInterval
 *					KParamUNSAFTCPKeepaliveInterval
 *   IDs:			-
 *
 * If some parameter is not present in the repository, its value is returned as
 * KErrNotFound.
 */
void TCmdGetUNSAFTimers::ExecuteL()
	{
	CRepository* repository = CRepository::NewLC( KCRUidUNSAFProtocols );
	TUint32 dummyKey( 0 );
	TInt bindingRequestInterval( KErrNotFound );
	TInt udpKeepAliveInterval( KErrNotFound );
	TInt tcpKeepAliveInterval( KErrNotFound );
	User::LeaveIfError( ReadTimerValuesL( *repository,
										  dummyKey,
										  bindingRequestInterval,
										  udpKeepAliveInterval,
										  tcpKeepAliveInterval ) );
	CleanupStack::PopAndDestroy( repository );

	// -- Response creation ---------------------------------------------------

	AddIntegerResponseL( KParamUNSAFBindingRequestInterval,
						 bindingRequestInterval );
	AddIntegerResponseL( KParamUNSAFUDPKeepaliveInterval,
						 udpKeepAliveInterval );
	AddIntegerResponseL( KParamUNSAFTCPKeepaliveInterval,
						 tcpKeepAliveInterval );
	}

TBool TCmdGetUNSAFTimers::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("GetUNSAFTimers") );
	}

TTcCommandBase* TCmdGetUNSAFTimers::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdGetUNSAFTimers( aContext );
	}
