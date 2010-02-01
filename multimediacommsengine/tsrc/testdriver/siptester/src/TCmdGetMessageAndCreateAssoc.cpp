/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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

#include "CTcSIPConnectionContainer.h"
#include "CTcSIPContext.h"
#include "SIPConstants.h"
#include "TCmdGetMessageAndCreateAssoc.h"
#include "TTcSIPReceived.h"

#include <sipclienttransaction.h>
#include <sipcseqheader.h>
#include <sipdialog.h>
#include <sipfromheader.h>
#include <sipinvitedialogassoc.h>
#include <sipmessageelements.h>
#include <siprefresh.h>
#include <sipregistrationbinding.h>
#include <sipresponseelements.h>
#include <siprequestelements.h>
#include <sipservertransaction.h>
#include <sipsubscribedialogassoc.h>
#include <siptoheader.h>

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	Timeout*
 *   IDs:			ConnectionId*
 *
 * OUTPUT:
 *   Parameters:	StackErrorMsg*, IAPName*, EventType*
 *   IDs:			RequestId*, DialogId*, RegistrationId*, RefreshId*
 *   Message:		ReturnCode, Headers
 */
void TCmdGetMessageAndCreateAssoc::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// Select connection; either default or user specified (and existing)
	CTcSIPConnectionContainer& conn = SelectConnectionL();

	// -- Execution -----------------------------------------------------------

	// Get an item off the receive queue (waits until timeout if none is present)
	TInt timeout = ExtractIntegerL( KParamTimeout, KDefaultReceiveTimeout, EFalse );
	TTcSIPReceived item = conn.ReceivedItemL( timeout );

	// -- Response creation ---------------------------------------------------

	// Get client transaction, if any
	const CSIPClientTransaction* transaction = item.iClientTransaction;
	const CSIPServerTransaction* serverTransaction = item.iServerTransaction;
	if( transaction )
		{
		// Get SIP elements from transaction
		const CSIPResponseElements* resElements = transaction->ResponseElements();
		// Check if SIP elements really exists
        if( resElements && (
              resElements->FromHeader() != NULL ||
              resElements->ToHeader() != NULL ||
			  resElements->CSeqHeader() != NULL ) )
			{
			CTcNameValue* respCode = CTcNameValue::NewLC();
			respCode->SetL( KResponseCode, resElements->StatusCode() );

			// Create a new array for the response
			CTcArray* headers = CTcArray::NewLC();
			headers->SetName( KResponseHeaders );

			// Add From header to response array if it's present
			if( resElements->FromHeader() )
				{
				headers->AddItemL( resElements->FromHeader()->ToTextLC()->Des() );
				CleanupStack::PopAndDestroy();
				}
			// Add To header to response array if it's present
			if( resElements->ToHeader() )
				{
				headers->AddItemL( resElements->ToHeader()->ToTextLC()->Des() );
				CleanupStack::PopAndDestroy();
				}
			// Add CSeq header to response array if it's present
			if( resElements->CSeqHeader() )
				{
				headers->AddItemL( resElements->CSeqHeader()->ToTextLC()->Des() );
				CleanupStack::PopAndDestroy();
				}
			// Add any user header from message elements to response array
			const CSIPMessageElements& elements = resElements->MessageElements();
			const RPointerArray< CSIPHeaderBase >& userHeaders = elements.UserHeaders();
			TInt count = userHeaders.Count();
			for( TInt i = 0; i < count; i++ )
				{
				headers->AddItemL( userHeaders[ i ]->ToTextLC()->Des() );
				CleanupStack::PopAndDestroy();
				}

			// Add parameters to return list
			// ReceivedMsg needs to be the first in the parameter list
			iContext.ReturnList().InsertParameterL( headers, KSIPMessageAtIndex );
			iContext.ReturnList().InsertParameterL( respCode, KSIPMessageAtIndex );

			CleanupStack::Pop( 2 );	// headers, respCode
			}

		AddIdResponseL( KTransactionId, transaction );
		}
	else if( serverTransaction )
		{
		// Get SIP elements from transaction
		const CSIPRequestElements* reqElements = serverTransaction->RequestElements();
		// Check if SIP elements really exists
        if( reqElements && (
              reqElements->FromHeader() != NULL ||
              reqElements->ToHeader() != NULL ) )
			{
			// not defined in requests, of course
			CTcNameValue* respCode = CTcNameValue::NewLC();
			respCode->SetL( KResponseCode, 0 );

			CTcNameValue* method = CTcNameValue::NewLC();
			method->SetL( KParamMethod, reqElements->Method().DesC() );

			// Create a new array for the response
			CTcArray* headers = CTcArray::NewLC();
			headers->SetName( KResponseHeaders );

			// Add From header to response array if it's present
			if( reqElements->FromHeader() )
				{
				headers->AddItemL( reqElements->FromHeader()->ToTextLC()->Des() );
				CleanupStack::PopAndDestroy();
				}
			// Add To header to response array if it's present
			if( reqElements->ToHeader() )
				{
				headers->AddItemL( reqElements->ToHeader()->ToTextLC()->Des() );
				CleanupStack::PopAndDestroy();
				}
				// Add any user header from message elements to response array
			const CSIPMessageElements& elements = reqElements->MessageElements();
			const RPointerArray< CSIPHeaderBase >& userHeaders = elements.UserHeaders();
			TInt count = userHeaders.Count();
			for( TInt i = 0; i < count; i++ )
				{
				headers->AddItemL( userHeaders[ i ]->ToTextLC()->Des() );
				CleanupStack::PopAndDestroy();
				}

			// Add parameters to return list
			// ReceivedMsg needs to be the first in the parameter list
			iContext.ReturnList().InsertParameterL( headers, KSIPMessageAtIndex );
			iContext.ReturnList().InsertParameterL( method, KSIPMessageAtIndex );
			iContext.ReturnList().InsertParameterL( respCode, KSIPMessageAtIndex );

			CleanupStack::Pop( 3 );
			}

		AddIdResponseL( KServerTransactionId, serverTransaction );
		}
	if( item.iDialog )
		{
		AddIdResponseL( KDialogId, item.iDialog );
		}

	if( item.iInviteDialogAssoc )
		{
        CSIPServerTransaction* newTransaction = 
                    const_cast< CSIPServerTransaction* >( item.iServerTransaction );
        CSIPInviteDialogAssoc* inviteDialogAssocNew = 
                    CSIPInviteDialogAssoc::NewL( *(newTransaction) );
                    
        // Add invite dialog id to response
        item.iInviteDialogAssoc = inviteDialogAssocNew;
		AddIdResponseL( KInviteDialogId, item.iInviteDialogAssoc );
		}
	else if( item.iSubscribeDialogAssoc )
		{
		AddIdResponseL( KSubscribeDialogId, item.iSubscribeDialogAssoc );
		}

	if( item.iRegistration )
		{
		AddIdResponseL( KRegistrationId, item.iRegistration );
		}
	else if( item.iRefresh )
		{
		AddIdResponseL( KRefreshId, item.iRefresh );
		}

	if( item.iIapId )
		{
		AddTextResponseL( KParamIAPName, iContext.IAPNameL( item.iIapId ) );
		}
		
	if ( item.iEvent )
        {
        AddIntegerResponseL( KParamEventType, item.iEvent );
        }

	if( item.iError != KErrNone )
		{
		AddTextResponseL( KParamStackErrorMsg, iContext.SIPErrorToText( item.iError ) );
		User::Leave( item.iError );
		}
	}

TBool TCmdGetMessageAndCreateAssoc::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("GetMessageAndCreateAssoc") );
	}

TTcCommandBase* TCmdGetMessageAndCreateAssoc::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdGetMessageAndCreateAssoc( aContext );
	}

