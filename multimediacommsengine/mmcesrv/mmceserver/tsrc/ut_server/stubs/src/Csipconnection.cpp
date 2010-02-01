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




#include "sipconnection.h"

EXPORT_C CSIPConnection* CSIPConnection::NewL(
    CSIP& aSIP,
    TUint32 aIapId,
    MSIPConnectionObserver& aSIPConnectionObserver)
	{
	return new(ELeave)CSIPConnection(aSIP, aIapId, aSIPConnectionObserver);
	}

CSIPConnection::CSIPConnection(
    CSIP& /*aSIP*/,
    TUint32 aIapId,
    MSIPConnectionObserver& /*aObserver*/)
 : iIapId(aIapId), iState(EActive)
	{
	}

EXPORT_C CSIPConnection::~CSIPConnection()
	{
	}
	
EXPORT_C CSIPConnection::TState CSIPConnection::State() const
    {
    return iState; 
    }	

EXPORT_C void CSIPConnection::SetState(TState aState)
    {
    iState = aState;
    }
    
EXPORT_C void CSIPConnection::SetOptL(
    TUint /*aOptionName*/,
    TUint /*aOptionLevel*/,
    TInt /*aOption*/)
    {
    delete HBufC::NewL(10);
    }
    