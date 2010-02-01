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
* Description:  Implementation
*
*/

#include "CTcSIPContext.h"
#include "SIPConstants.h"
#include "TCmdGetClientDiscoveryStatus.h"
#include "CTcSIPClientDiscoveryContainer.h"

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	Timeout*
 *   IDs:			-
 *
 * OUTPUT:
 *	 Parameters:	ClientDiscoveryRequestId, ClientDiscoveryErrorCode, 
 *                  ApplicationUid, ClientDiscoveryStatus
 *
 *	 IDs:			
 */
void TCmdGetClientDiscoveryStatus::ExecuteL()
	{
	// Get an item off the receive queue (waits until timeout if none is present)
	//
	TInt timeout = ExtractIntegerL( KParamTimeout, KDefaultReceiveTimeout, EFalse );
	
	TTcClientDiscoveryEvent event = 
	    iContext.ClientDiscoveryL().ReceivedClientDiscoveryEventL( timeout );


    // Create response
    //
	AddIntegerResponseL( 
	    KParamClientDiscoveryRequestId, static_cast<TInt>( event.iRequestId ) );
	AddIntegerResponseL( KParamClientDiscoveryErrorCode, event.iError );
	
	const TInt KMaxTUidAsTextLength = 11;
	TBuf8<KMaxTUidAsTextLength> uidText;
	uidText.AppendFormat( _L8("0x%x"), event.iChannel.iUid );
	AddTextResponseL( KParamApplicationUid, uidText );
	
	TPtrC8 status;
	switch( event.iType )
		{
		case ETcClientDiscoveryChannelResolved:
			{
			status.Set( KClientDiscoveryChannelResolved );
			break;
			}
		case ETcClientDiscoveryClientNotFound:
			{
			status.Set( KClientDiscoveryClientNotFound );
			break;
			}
		case ETcClientDiscoveryError:
			{
			status.Set( KClientDiscoveryErrorOccured );
			break;
			}
		default:
			{
			status.Set( KUnknown );
			break;
			}
		}

	AddTextResponseL( KParamClientDiscoveryStatus, status );
	}


TBool TCmdGetClientDiscoveryStatus::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("GetClientDiscoveryStatus") );
	}

TTcCommandBase* TCmdGetClientDiscoveryStatus::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdGetClientDiscoveryStatus( aContext );
	}

