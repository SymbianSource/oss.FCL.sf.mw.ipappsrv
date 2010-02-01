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



#include "cnatfwunsafa_aaaaorigdomain.h"
#include "cnatfwunsafquerybase.h"
#include "cnatfwunsafquerya_aaaa.h"
#include "mnatfwunsafserverquery.h"
#include "cnatfwunsafquerydata.h"
//#include "natfwunsafserverresolverobserver.h"
#include "mnatfwunsafhostresolver.h"

// ----------------------------------------------------------------------------
// CNATFWUNSAFA_AAAAOrigDomain::CNATFWUNSAFA_AAAAOrigDomain
// ----------------------------------------------------------------------------
//
CNATFWUNSAFA_AAAAOrigDomain::CNATFWUNSAFA_AAAAOrigDomain(
    MNATFWUNSAFServerQuery& aServerQuery )
    :CNATFWUNSAFQueryConditionBase(), iServerQuery ( aServerQuery )
    {
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFA_AAAAOrigDomain::ConstructL
// ----------------------------------------------------------------------------
//
void CNATFWUNSAFA_AAAAOrigDomain::ConstructL()
    {
    iQueryBase = CNATFWUNSAFQueryA_AAAA::NewL(
        iServerQuery.QueryData().Target() );
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFA_AAAAOrigDomain::NewL
// ----------------------------------------------------------------------------
//
CNATFWUNSAFA_AAAAOrigDomain* CNATFWUNSAFA_AAAAOrigDomain::NewL(
    MNATFWUNSAFServerQuery& aServerQuery )
    {
    CNATFWUNSAFA_AAAAOrigDomain* self =
        new ( ELeave ) CNATFWUNSAFA_AAAAOrigDomain( aServerQuery );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFA_AAAAOrigDomain::~CNATFWUNSAFA_AAAAOrigDomain
// ----------------------------------------------------------------------------
//
CNATFWUNSAFA_AAAAOrigDomain::~CNATFWUNSAFA_AAAAOrigDomain()
    {
    delete iQueryBase;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFA_AAAAOrigDomain::QueryBase
// ----------------------------------------------------------------------------
//
CNATFWUNSAFQueryBase& CNATFWUNSAFA_AAAAOrigDomain::QueryBase()
    {
    return *iQueryBase;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFA_AAAAOrigDomain::HandleQueryResultL
// ----------------------------------------------------------------------------
//
CNATFWUNSAFQueryConditionBase*
    CNATFWUNSAFA_AAAAOrigDomain::HandleQueryResultL( TInt aStatus )
    {
    if ( aStatus == KErrNone )
        {
        if ( !iServerQuery.QueryData().CreateResultL (
                    *static_cast<CNATFWUNSAFQueryA_AAAA*> ( iQueryBase ) ) )
            {
            iServerQuery.QueryData().Resolver().SetIPListFailed(ETrue);
            }
        }
    return NULL;
    }
