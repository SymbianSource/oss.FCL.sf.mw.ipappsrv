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



#include "CTcMCEFactory.h"
#include "CTcMCEContext.h"
#include "MTcTestContext.h"

// MCE context identifier
_LIT8( KTcMCENameTag, "SSC:" );

CTcMCEFactory* CTcMCEFactory::NewL()
    {
    return new ( ELeave ) CTcMCEFactory;
    }

CTcMCEFactory::CTcMCEFactory()
	{
	}

CTcMCEFactory::~CTcMCEFactory()
	{
	}

MTcTestContext* CTcMCEFactory::CreateLC( const TDesC8& aTestCaseName,
										 CTcCTRLCodec& aCodec,
										 TInt aIAPId ) const
	{
	// Check if the test case name starts with our tag.
	// (complete name might be e.g. "SSC:INV_01")
	if( aTestCaseName.FindF( KTcMCENameTag ) == 0 )
		{
		return CTcMCEContext::NewLC( aCodec, aIAPId );
		}
	// No, the test case wasn't for us.
	else
		{
		return NULL;
		}
	}
