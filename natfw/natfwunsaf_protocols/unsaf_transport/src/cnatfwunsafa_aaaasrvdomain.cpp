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



#include "cnatfwunsafa_aaaasrvdomain.h"
#include "cnatfwunsafa_aaaaorigdomain.h"
#include "cnatfwunsafquerysrv.h"
#include "cnatfwunsafquerya_aaaa.h"
#include "cnatfwunsafquerybase.h"
#include "mnatfwunsafserverquery.h"
#include "cnatfwunsafquerydata.h"

// ----------------------------------------------------------------------------
// CNATFWUNSAFA_AAAASRVDomain::CNATFWUNSAFA_AAAASRVDomain
// ----------------------------------------------------------------------------
//
CNATFWUNSAFA_AAAASRVDomain::CNATFWUNSAFA_AAAASRVDomain(
    MNATFWUNSAFServerQuery& aServerQuery )
    :CNATFWUNSAFQueryConditionBase(), iServerQuery ( aServerQuery )
    {
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFA_AAAASRVDomain::ConstructL
// ----------------------------------------------------------------------------
//
void CNATFWUNSAFA_AAAASRVDomain::ConstructL( CNATFWUNSAFQueryBase* aSrvQuery )
    {
    iQueryBase = CNATFWUNSAFQueryA_AAAA::NewL(
        iServerQuery.QueryData().Target() );
    iSrvQuery = aSrvQuery;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFA_AAAASRVDomain::NewL
// ----------------------------------------------------------------------------
//
CNATFWUNSAFA_AAAASRVDomain* CNATFWUNSAFA_AAAASRVDomain::NewL(
    MNATFWUNSAFServerQuery& aServerQuery,
    CNATFWUNSAFQueryBase* aSrvQuery )
    {
    CNATFWUNSAFA_AAAASRVDomain* self =
        new ( ELeave ) CNATFWUNSAFA_AAAASRVDomain( aServerQuery );
    CleanupStack::PushL( self );
    self->ConstructL( aSrvQuery );
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFA_AAAASRVDomain::~CNATFWUNSAFA_AAAASRVDomain
// ----------------------------------------------------------------------------
//
CNATFWUNSAFA_AAAASRVDomain::~CNATFWUNSAFA_AAAASRVDomain()
    {
    delete iQueryBase;
    delete iSrvQuery;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFA_AAAASRVDomain::QueryBase
// ----------------------------------------------------------------------------
//
CNATFWUNSAFQueryBase& CNATFWUNSAFA_AAAASRVDomain::QueryBase()
    {
    return *iQueryBase;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFA_AAAASRVDomain::HandleQueryResultL
// ----------------------------------------------------------------------------
//
CNATFWUNSAFQueryConditionBase*
                CNATFWUNSAFA_AAAASRVDomain::HandleQueryResultL( TInt aStatus )
    {
    if ( aStatus == KErrNone )
        {
        iServerQuery.QueryData().CreateResultL(
            *static_cast<CNATFWUNSAFQueryA_AAAA*> ( iQueryBase ) );
        }

    //more srvrecords
    iSrvQuery->RemoveElementL( 0 );
    if ( iSrvQuery->ArrayCountL() == 0 )
        {
        if ( iServerQuery.QueryData().ResultArray().Count() == 0 )
            {
            iServerQuery.QueryData().SetPort(
                                    iServerQuery.QueryData().DefaultPort() );
            iServerQuery.QueryData().SetTargetL(
                                iServerQuery.QueryData().OriginalTarget() );
            return ( CNATFWUNSAFA_AAAAOrigDomain::NewL( iServerQuery ) );
            }
        else
            {
            return NULL;
            }
        }
    iServerQuery.QueryData().SetPort( iSrvQuery->ResultPortL() );
    iServerQuery.QueryData().SetTargetL( iSrvQuery->ResultTargetL() );
    iQueryBase->SetTargetL( iServerQuery.QueryData().Target() );
    return this;
    }

