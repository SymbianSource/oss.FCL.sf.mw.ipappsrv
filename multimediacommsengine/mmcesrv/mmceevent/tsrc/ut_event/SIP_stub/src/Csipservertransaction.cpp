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


EXPORT_C CSIPServerTransaction::~CSIPServerTransaction()
	{
	delete iRequestElements;
	delete iElements;
	}


CSIPServerTransaction::CSIPServerTransaction()
	{
	}


EXPORT_C CSIPServerTransaction* CSIPServerTransaction::NewL()
	{
	return new(ELeave)CSIPServerTransaction;
	}


EXPORT_C void CSIPServerTransaction::SetRequestElements(
    CSIPRequestElements* aElements)
    {
    delete iRequestElements;
    iRequestElements = aElements;
    }


EXPORT_C void CSIPServerTransaction::SendResponseL(
    CSIPResponseElements *aElements)
    {
    delete HBufC8::NewL(100); // To simulate a leave in low memory conditions
    iElements = aElements; 
    }


EXPORT_C const CSIPRequestElements* CSIPServerTransaction::RequestElements() const
    {
    return iRequestElements;
    }