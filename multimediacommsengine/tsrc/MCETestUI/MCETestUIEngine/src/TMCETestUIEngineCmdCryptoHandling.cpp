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
* Description:    Implementation.
*
*/




// INCLUDES

#include "TMCETestUIEngineCmdCryptoHandling.h"
#include "CMCETestUIEngineSession.h"
#include "CMCETestUIEngineOutSession.h"
#include "CMCETestUIQuestionSingleSelection.h"
#include <mcesecureoutsession.h>
#include <mcesecureinsession.h>
#include <mcesecuresession.h>



// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdCurrentCryptoContext::TMCETestUIEngineCmdCurrentCryptoContext
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdCurrentCryptoContext::TMCETestUIEngineCmdCurrentCryptoContext(
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineSession& aSession )
    : TMCETestUIEngineCmdBase( aEngine ),
      iSession( aSession )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdCurrentCryptoContext::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdCurrentCryptoContext::ExecuteL()
	{
	RArray<TMceCryptoContext> currentCryptoContexts ;
	if(iSession.iType == KMceOutSession)
		{
		CMceSecureOutSession& outSecSession = static_cast<CMceSecureOutSession&>(iSession.Session());		
		currentCryptoContexts = outSecSession.CryptoContexts();
		}
	else
		{
		CMceSecureInSession& inSecSession = static_cast<CMceSecureInSession&>(iSession.Session());
		currentCryptoContexts = inSecSession.CryptoContexts();
		}
	TBool crypto32 = EFalse;
	TBool crypto80 = EFalse;
	for(TInt i = 0; i< currentCryptoContexts.Count(); i++)
		{
		if(currentCryptoContexts[i] == EAES_CM_128_HMAC_SHA1_32 )
			{
			crypto32 = ETrue;
			}
		else if(currentCryptoContexts[i] == EAES_CM_128_HMAC_SHA1_80 )
			{
			crypto80 = ETrue;
			}
		}
	iEngine.ShowCurrentCryptoContextsL(crypto32,crypto80);
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdCurrentCryptoContext::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdCurrentCryptoContext::Caption() const
	{
	return KCommandCaptionCurrentCryptoContext;
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdSupportedCryptoContext::TMCETestUIEngineCmdSupportedCryptoContext
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdSupportedCryptoContext::TMCETestUIEngineCmdSupportedCryptoContext(
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineSession& aSession )
    : TMCETestUIEngineCmdBase( aEngine ),
      iSession( aSession )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdSupportedCryptoContext::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdSupportedCryptoContext::ExecuteL()
	{
	RArray<TMceCryptoContext> currentCryptoContexts ;
	if(iSession.iType == KMceOutSession)
		{
		CMceSecureOutSession& outSecSession = static_cast<CMceSecureOutSession&>(iSession.Session());		
		currentCryptoContexts = outSecSession.SupportedCryptoContextsL();
		}
	else
		{
		CMceSecureInSession& inSecSession = static_cast<CMceSecureInSession&>(iSession.Session());
		currentCryptoContexts = inSecSession.SupportedCryptoContextsL();
		}
	TBool crypto32 = EFalse;
	TBool crypto80 = EFalse;
	for(TInt i = 0; i< currentCryptoContexts.Count(); i++)
		{
		if(currentCryptoContexts[i] == EAES_CM_128_HMAC_SHA1_32 )
			{
			crypto32 = ETrue;
			}
		else if(currentCryptoContexts[i] == EAES_CM_128_HMAC_SHA1_80 )
			{
			crypto80 = ETrue;
			}
		}
	iEngine.ShowSupportedCryptoContextsL(crypto32,crypto80);
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdSupportedCryptoContext::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdSupportedCryptoContext::Caption() const
	{
	return KCommandCaptionSupportedCryptoContext;
	}
// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdSetCryptoContext::TMCETestUIEngineCmdSetCryptoContext
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdSetCryptoContext::TMCETestUIEngineCmdSetCryptoContext(
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineSession& aSession )
    : TMCETestUIEngineCmdBase( aEngine ),
      iSession( aSession )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdSetCryptoContext::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdSetCryptoContext::ExecuteL()
	{
	RArray<TMceCryptoContext> currentCryptoContexts ;
	TBool crypto32 = EFalse;
	TBool crypto80 = EFalse;
	iEngine.GetCryptoContextsL(crypto32,crypto80);
    if(crypto32)
    	{
    	currentCryptoContexts.Append(EAES_CM_128_HMAC_SHA1_32);
    	}
   	if(crypto80) 	
   		{
   		currentCryptoContexts.Append(EAES_CM_128_HMAC_SHA1_80);
   		}
   		
   	if(iSession.iType == KMceOutSession)
		{
		CMceSecureOutSession& outSecSession = static_cast<CMceSecureOutSession&>(iSession.Session());		
		outSecSession.SetCryptoContextsL(currentCryptoContexts);
		}
	else
		{
		CMceSecureInSession& inSecSession = static_cast<CMceSecureInSession&>(iSession.Session());
		inSecSession.SetCryptoContextsL(currentCryptoContexts);
		}
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdSetCryptoContext::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdSetCryptoContext::Caption() const
	{
	return KCommandCaptionSetCryptoContext;
	}

// End of File
