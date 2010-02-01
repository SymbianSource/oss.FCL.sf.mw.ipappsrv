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


#include "siptransactionbase.h"
#include <sipstrings.h>
#include <sipstrconsts.h>

CSIPTransactionBase::~CSIPTransactionBase()
	{
	}

CSIPTransactionBase::CSIPTransactionBase()
	{
	}

EXPORT_C TBool CSIPTransactionBase::operator==(const CSIPTransactionBase& aTransactionBase) const
	{
	return (this == &aTransactionBase);
	}
	
void CSIPTransactionBase::Detach(const MTransactionAssociation& /*aAssociation*/)
    {
    }

// -----------------------------------------------------------------------------
// CSIPTransactionBase::Type
// -----------------------------------------------------------------------------
//
EXPORT_C RStringF CSIPTransactionBase::Type() const
    {
    return iType;
    }

EXPORT_C  CSIPTransactionBase::TState CSIPTransactionBase::StateL() const
	{
	return iState;
	}

EXPORT_C void CSIPTransactionBase::SetType(TInt aTypeNumber) 
	{
	if(aTypeNumber == 1)
		{
		iType = SIPStrings::StringF( SipStrConsts::ESubscribe);		
		}
	else if(aTypeNumber==2)
		{
		iType= 	SIPStrings::StringF( SipStrConsts::ERefer);
		}
	else if ( aTypeNumber == 3 )
	    {
	    iType= 	SIPStrings::StringF( SipStrConsts::ENotify);
	    }
		
	}
EXPORT_C TBool CSIPTransactionBase::IsSIPClientTransaction() const
	{
	return EFalse;	
	}