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

#include "TCmdChannel.h"
#include "CTcSIPContext.h"
#include "SIPConstants.h"
#include "CTcSIPClientDiscoveryContainer.h"
#include "CleanupResetAndDestroy.h"

#include <sipclientdiscovery.h>
#include <sipheaderbase.h>
#include <siptoheader.h>
#include <sipcontactheader.h>
#include <sipfromheader.h>
#include <sipcontenttypeheader.h>
#include <sipstrings.h>
#include <stringpool.h>
#include <uri8.h>

/**
 * INPUT:
 *   Headers: From*, To*, Contact*, Content-Type*
 *   Parameters: Method, RemoteURI, ApplicationUid*, Content*
 *   IDs: -
 *
 * OUTPUT:
 *   Parameters: ClientDiscoveryRequestId
 *   IDs: -
 */
void TCmdChannel::ExecuteL()
	{
	// Extract optional uid
	TUid paramUid = ExtractUidL( KParamApplicationUid, EFalse );
	
	// Extract method
	TPtrC8 method = ExtractTextL( KParamMethod );
	RStringF methodStr = SIPStrings::Pool().OpenFStringL( method );
	CleanupClosePushL( methodStr );
	
	// Extract remote URI
	CUri8* uri = ExtractRemoteURILC( ETrue );	
	
	// Extract headers and add them to array if they existed
	RPointerArray< CSIPHeaderBase > headers;
	CleanupResetAndDestroyPushL( headers );
	
	CSIPToHeader* toHeader = ExtractToHeaderLC( EFalse );
	if ( toHeader )
	    {
	    headers.AppendL( toHeader );
	    CleanupStack::Pop( toHeader );
	    }
	
	CSIPContactHeader* contactHeader = ExtractContactHeaderLC( EFalse );
	if ( contactHeader )
	    {
	    headers.AppendL( contactHeader );
	    CleanupStack::Pop( contactHeader );
	    }

	CSIPFromHeader* fromHeader = ExtractFromHeaderLC( EFalse );
	if ( fromHeader )
	    {
	    headers.AppendL( fromHeader );
	    CleanupStack::Pop( fromHeader );
	    }
	    
	// Extract content
	HBufC8* content = ExtractHBufLC( KParamContent );
	if ( !content )
	    {
	    // Create dummy zero length content
	    content = HBufC8::NewLC( 0 );
	    }
	    
	// Extract content-type header
	CSIPContentTypeHeader* contentType = ExtractContentTypeHeaderLC( EFalse );
	
	// Execute
	TUint32 requestId( 0 );
	if ( paramUid.iUid > 0 )
	    {
	    // ApplicationUid param was present, use also it in ChannelL call
	    requestId = iContext.ClientDiscoveryL().ClientDiscovery().ChannelL( 
        	            paramUid, 
        	            methodStr, 
        	            uri->Uri().UriDes(),
        	            headers, 
        	            *content, 
        	            contentType );
	    }
	else
	    {
	    requestId = iContext.ClientDiscoveryL().ClientDiscovery().ChannelL(
        	            methodStr, 
        	            uri->Uri().UriDes(),
        	            headers, 
        	            *content, 
        	            contentType );
	    }

    // Cleanup
	if ( contentType )
	    {
	    CleanupStack::PopAndDestroy( contentType );
	    }
	CleanupStack::PopAndDestroy( 4 ); // content, headers, uri, methodStr
	
	AddIntegerResponseL( KParamClientDiscoveryRequestId, requestId );
	}

TBool TCmdChannel::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("Channel") );
	}

TTcCommandBase* TCmdChannel::CreateL( MTcTestContext& aContext )
	{
	return new ( ELeave ) TCmdChannel( aContext );
	}

