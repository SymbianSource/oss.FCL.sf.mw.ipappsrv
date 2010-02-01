/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies).
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




#include <siphttpdigest.h>

// -----------------------------------------------------------------------------
// CSIPHttpDigest::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPHttpDigest*
CSIPHttpDigest::NewL(CSIP& aSIP, MSIPHttpDigestChallengeObserver& aObserver)
    {
    CSIPHttpDigest* self = CSIPHttpDigest::NewLC(aSIP, aObserver);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CSIPHttpDigest::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPHttpDigest*
CSIPHttpDigest::NewLC(CSIP& aSIP, MSIPHttpDigestChallengeObserver& aObserver)
    {
    CSIPHttpDigest* self = new (ELeave) CSIPHttpDigest(aSIP);
    CleanupStack::PushL(self);
    self->ConstructL(aObserver);
    return self;
    }

// -----------------------------------------------------------------------------
// CSIPHttpDigest::CSIPHttpDigest
// -----------------------------------------------------------------------------
//
CSIPHttpDigest::CSIPHttpDigest(CSIP& aSIP) : iSIP(&aSIP)
	{
	}

// -----------------------------------------------------------------------------
// CSIPHttpDigest::ConstructL
// -----------------------------------------------------------------------------
//	
void CSIPHttpDigest::ConstructL(MSIPHttpDigestChallengeObserver& /*aObserver*/)
	{
	}

// -----------------------------------------------------------------------------
// CSIPHttpDigest::~CSIPHttpDigest
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPHttpDigest::~CSIPHttpDigest()
	{
	}

