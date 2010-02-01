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
* Description:  
*
*/




//  CLASS HEADER
#include "ut_cnatfwunsafserverresolverobserverstub.h"

// ----------------------------------------------------------------------------
// UT_CNATFWUNSAFServerResolverObserverStub::NewL
// ----------------------------------------------------------------------------
//
UT_CNATFWUNSAFServerResolverObserverStub*
UT_CNATFWUNSAFServerResolverObserverStub::NewL()
    {
    UT_CNATFWUNSAFServerResolverObserverStub* self = NewLC();
    CleanupStack::Pop();
    return self;
    }

// ----------------------------------------------------------------------------
// UT_CNATFWUNSAFServerResolverObserverStub::NewLC
// ----------------------------------------------------------------------------
//
UT_CNATFWUNSAFServerResolverObserverStub*
UT_CNATFWUNSAFServerResolverObserverStub::NewLC()
    {
    UT_CNATFWUNSAFServerResolverObserverStub* self =
        new ( ELeave ) UT_CNATFWUNSAFServerResolverObserverStub();
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// ----------------------------------------------------------------------------
// UT_CNATFWUNSAFServerResolverObserverStub::CompletedL
// ----------------------------------------------------------------------------
//
void UT_CNATFWUNSAFServerResolverObserverStub::CompletedL()
    {
    if(iResultArray)
        {
        iResultArray->Reset();
        delete iResultArray;
        iResultArray = NULL;
        }
    }

// ----------------------------------------------------------------------------
// UT_CNATFWUNSAFServerResolverObserverStub::ErrorOccured
// ----------------------------------------------------------------------------
//
void UT_CNATFWUNSAFServerResolverObserverStub::ErrorOccured( TInt aError )
    {
    iError = aError;
    }

// ----------------------------------------------------------------------------
// UT_CNATFWUNSAFServerResolverObserverStub::
// ~UT_CNATFWUNSAFServerResolverObserverStub
// ----------------------------------------------------------------------------
//
UT_CNATFWUNSAFServerResolverObserverStub::
~UT_CNATFWUNSAFServerResolverObserverStub()
    {
    if(iResultArray)
        {
        iResultArray->Reset();
        delete iResultArray;
        }
    }

// ----------------------------------------------------------------------------
// UT_CNATFWUNSAFServerResolverObserverStub::
// UT_CNATFWUNSAFServerResolverObserverStub
// ----------------------------------------------------------------------------
//
UT_CNATFWUNSAFServerResolverObserverStub::
UT_CNATFWUNSAFServerResolverObserverStub()
    {
    }

// ----------------------------------------------------------------------------
// UT_CNATFWUNSAFServerResolverObserverStub::ConstructL
// ----------------------------------------------------------------------------
//
void UT_CNATFWUNSAFServerResolverObserverStub::ConstructL()
    {
    iResultArray = new (ELeave) RArray<TInetAddr>;
    }
