/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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



#include "cnatfwunsafquerybase.h"

// ----------------------------------------------------------------------------
// CNATFWUNSAFQueryBase::CNATFWUNSAFQueryBase
// ----------------------------------------------------------------------------
//
CNATFWUNSAFQueryBase::CNATFWUNSAFQueryBase()
    {
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFQueryBase::~CNATFWUNSAFQueryBase
// ----------------------------------------------------------------------------
//
CNATFWUNSAFQueryBase::~CNATFWUNSAFQueryBase()
    {
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFQueryBase::ResultTargetL
// ----------------------------------------------------------------------------
//
const TDesC8& CNATFWUNSAFQueryBase::ResultTargetL()
    {
    __ASSERT_ALWAYS (EFalse, User::Leave(KErrNotSupported));
    return KNullDesC8();
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFQueryBase::ResultPortL
// ----------------------------------------------------------------------------
//
TUint CNATFWUNSAFQueryBase::ResultPortL()
    {
    __ASSERT_ALWAYS (EFalse, User::Leave(KErrNotSupported));
    return 0;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFQueryBase::RemoveElementL
// ----------------------------------------------------------------------------
//
void CNATFWUNSAFQueryBase::RemoveElementL(TInt /*aIndex*/)
    {
    __ASSERT_ALWAYS (EFalse, User::Leave(KErrNotSupported));
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFQueryBase::ArrayCountL
// ----------------------------------------------------------------------------
//
TUint CNATFWUNSAFQueryBase::ArrayCountL()
    {
    __ASSERT_ALWAYS (EFalse, User::Leave(KErrNotSupported));
    return 0;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFQueryBase::SetTargetL
// ----------------------------------------------------------------------------
//
void CNATFWUNSAFQueryBase::SetTargetL(const TDesC8& /*aTarget*/)
    {
    __ASSERT_ALWAYS (EFalse, User::Leave(KErrNotSupported));
    }
