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
* Description:  Implementation.
*
*/

#include "CTTcSIPFactory.h"
#include "CTcSIPContext.h"
#include "MTcTestContext.h"

// SIP context identifier
_LIT8( KTcSIPNameTag, "SIP:" );
_LIT8( KTcCreateSipStack, "TRUE" );

CTTcSIPFactory* CTTcSIPFactory::NewL()
    {
    return new ( ELeave ) CTTcSIPFactory;
    }

CTTcSIPFactory::CTTcSIPFactory()
	{
	}

CTTcSIPFactory::~CTTcSIPFactory()
	{
	}

MTcTestContext* CTTcSIPFactory::CreateLC( const TDesC8& aTestCaseName,
										 CTcCTRLCodec& aCodec,
										 TInt aIAPId ) const
	{
	// Does the test case name start with our tag?
	// (complete name might be e.g. "SIP:INV_01")
	if( aTestCaseName.FindF( KTcSIPNameTag ) == 0 )
		{
        // If CreateSipStack is !true then create SIP stack manually
        // otherwise create it automatically
        if ( aTestCaseName.FindF( KTcCreateSipStack ) != -1 )
            {
		    return CTcSIPContext::NewLC( aCodec, aIAPId, ETrue );
            }
		return CTcSIPContext::NewLC( aCodec, aIAPId, EFalse );
		}
	// No, the test case wasn't for us.
	else
		{
		return NULL;
		}
	}
