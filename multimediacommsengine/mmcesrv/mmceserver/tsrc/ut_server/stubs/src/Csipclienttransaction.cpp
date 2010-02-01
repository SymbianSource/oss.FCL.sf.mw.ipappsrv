/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  
*
*/



#include "sipclienttransaction.h"
#include "sipresponseelements.h"
#include "sipstrings.h"
#include "sipstrconsts.h"
#include "cmcetls.h"


EXPORT_C CSIPClientTransaction::~CSIPClientTransaction()
	{
	delete iE;
	}


CSIPClientTransaction::CSIPClientTransaction( RStringF aType )
   :CSIPTransactionBase( aType, ETrue )
	{
    iIsCancelAllowed = ETrue;
	}


EXPORT_C CSIPClientTransaction* CSIPClientTransaction::NewL( RStringF aType )
	{
	return new (ELeave) CSIPClientTransaction( aType );
	}

EXPORT_C void CSIPClientTransaction::SetResponseElements(CSIPResponseElements* aE)
	{
	if (iE)
		delete iE;
	iE = aE;

	}

EXPORT_C const CSIPResponseElements* CSIPClientTransaction::ResponseElements() const
	{
	return iE;
	}

EXPORT_C TBool CSIPClientTransaction::CancelAllowed() const
	{
	return iIsCancelAllowed;
	}


EXPORT_C CSIPClientTransaction* CSIPClientTransaction::CancelL()
    {
    if ( CMCETls::Storage() )
        {
        CSIPMessageElements* null = NULL;
        CMCETls::Storage()->SIPSends( SIPStrings::StringF( SipStrConsts::ECancel ),
                                      null );
        }
    return CSIPClientTransaction::NewL( SIPStrings::StringF( SipStrConsts::ECancel ) );
    }
