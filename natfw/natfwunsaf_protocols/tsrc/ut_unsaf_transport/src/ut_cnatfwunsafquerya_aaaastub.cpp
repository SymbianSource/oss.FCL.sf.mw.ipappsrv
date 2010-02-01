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




#include <utf.h>
#include "ut_cnatfwunsafquerya_aaaastub.h"
#include "mnatfwunsafhostresolver.h"

// ----------------------------------------------------------------------------
// UT_CNATFWUNSAFQueryA_AAAAStub::NewL
// ----------------------------------------------------------------------------
//
UT_CNATFWUNSAFQueryA_AAAAStub* UT_CNATFWUNSAFQueryA_AAAAStub::
    NewL(const TDesC8& aTarget)
    {
    UT_CNATFWUNSAFQueryA_AAAAStub* self =
        new (ELeave) UT_CNATFWUNSAFQueryA_AAAAStub();
    CleanupStack::PushL(self);
    self->ConstructL(aTarget);
    CleanupStack::Pop();
    return self;
    }

// ----------------------------------------------------------------------------
// UT_CNATFWUNSAFQueryA_AAAAStub::ConstructL
// ----------------------------------------------------------------------------
//
void UT_CNATFWUNSAFQueryA_AAAAStub::ConstructL(const TDesC8& aTarget)
    {
    CNATFWUNSAFQueryA_AAAA::ConstructL(aTarget);
    }

// ----------------------------------------------------------------------------
// UT_CNATFWUNSAFQueryA_AAAAStub::UT_CNATFWUNSAFQueryA_AAAAStub
// ----------------------------------------------------------------------------
//
UT_CNATFWUNSAFQueryA_AAAAStub::UT_CNATFWUNSAFQueryA_AAAAStub()
:CNATFWUNSAFQueryA_AAAA()
    {
    }

// ----------------------------------------------------------------------------
// UT_CNATFWUNSAFQueryA_AAAAStub::~UT_CNATFWUNSAFQueryA_AAAAStub
// ----------------------------------------------------------------------------
//
UT_CNATFWUNSAFQueryA_AAAAStub::~UT_CNATFWUNSAFQueryA_AAAAStub()
    {
    }

// ----------------------------------------------------------------------------
// UT_CNATFWUNSAFQueryA_AAAAStub::Query
// ----------------------------------------------------------------------------
//
void UT_CNATFWUNSAFQueryA_AAAAStub::Query( MNATFWUNSAFHostResolver& aResolver )
    {
    aResolver.Resolver();
    aResolver.RequestStatus();
    TRefByValue <const TDesC8> ref(_L8("11.22.33.44"));
    iNResult().iAddr.Format(ref);
    iNResult().iAddr.SetPort(5076);
    }

// ----------------------------------------------------------------------------
// UT_CNATFWUNSAFQueryA_AAAAStub::QueryNext
// ----------------------------------------------------------------------------
//
TBool UT_CNATFWUNSAFQueryA_AAAAStub::QueryNext ( MNATFWUNSAFHostResolver& aResolver )
    {
    aResolver.Resolver();
    return EFalse;
    }
