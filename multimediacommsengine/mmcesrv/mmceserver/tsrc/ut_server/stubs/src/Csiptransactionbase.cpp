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
#include "sipstrings.h"
#include "sipstrconsts.h"


CSIPTransactionBase::~CSIPTransactionBase()
	{
	}

CSIPTransactionBase::CSIPTransactionBase( RStringF aType, TBool aIsCliTrx )
  : iType(aType),
    iIsClientTransaction(aIsCliTrx),
    iState(ETrying)
	{
	if ( !iIsClientTransaction )
	    {
	    iState = ETrying;
	    }
	    
	else if ( iIsClientTransaction && iType == SIPStrings::StringF( SipStrConsts::EInvite ) )
	    {
	    iState = ECalling;
	    }
	else
	    {
	    // NOP
	    }
	}

EXPORT_C TBool CSIPTransactionBase::operator==(const CSIPTransactionBase& aTransactionBase) const
	{
	return (this == &aTransactionBase);
	}

EXPORT_C  CSIPTransactionBase::TState CSIPTransactionBase::StateL() const
	{
	return iState;
	}
	    
void CSIPTransactionBase::Detach(const MTransactionAssociation& /*aAssociation*/)
    {
    }

void CSIPTransactionBase::SetState( TState aState )
    {
    iState = aState;
    }

RStringF CSIPTransactionBase::Type() const
    {
    return iType;
    }    

    
TUint32 CSIPTransactionBase::RequestId() const
    {
    return 1;
    }
    