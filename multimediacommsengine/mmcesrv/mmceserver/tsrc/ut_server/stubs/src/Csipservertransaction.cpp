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




#include "sipservertransaction.h"
#include "siprequestelements.h"
#include "sipresponseelements.h"
#include "cmcetls.h"
#include <sipstrings.h>
#include <sipstrconsts.h>


EXPORT_C CSIPServerTransaction::~CSIPServerTransaction()
	{
	delete iRequestElements;
	}


CSIPServerTransaction::CSIPServerTransaction( RStringF aType )
 : CSIPTransactionBase( aType, EFalse )
	{
	}


EXPORT_C CSIPServerTransaction* CSIPServerTransaction::NewL( RStringF aType )
	{
	return new(ELeave)CSIPServerTransaction( aType );
	}


EXPORT_C void CSIPServerTransaction::SetRequestElements(
    CSIPRequestElements* aElements)
    {
    if( iRequestElements )
    	{
    	delete iRequestElements;
    	}
    
    iRequestElements = aElements;
    }


EXPORT_C void CSIPServerTransaction::SendResponseL(
    CSIPResponseElements *aElements)
    {
    if ( aElements && aElements->StatusCode() >= KMceSipOK )
        {
        SetState( ETerminated );
        }
    if ( CMCETls::Storage() )
        {
        CMCETls::Storage()->SIPSends( Type(), aElements );
        }
    else
        {
        delete aElements;
        }
    
    }


EXPORT_C const CSIPRequestElements* CSIPServerTransaction::RequestElements() const
    {
    return iRequestElements;
    }
    
EXPORT_C TBool CSIPServerTransaction::ResponseAllowed() const
    {
    if ( Type() == SIPStrings::StringF( SipStrConsts::EAck ) )
        {
        return EFalse;
        }
    else
        {
        return ETrue;
        }
    }
