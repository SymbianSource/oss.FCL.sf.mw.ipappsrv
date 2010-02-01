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



#include "cnatfwunsafsrvorigdomain.h"
#include "cnatfwunsafa_aaaasrvdomain.h"
#include "cnatfwunsafa_aaaaorigdomain.h"
#include "cnatfwunsafquerybase.h"
#include "cnatfwunsafquerysrv.h"
#include "mnatfwunsafserverquery.h"
#include "cnatfwunsafquerydata.h"

// ----------------------------------------------------------------------------
// CNATFWUNSAFSRVOrigDomain::CNATFWUNSAFSRVOrigDomain
// ----------------------------------------------------------------------------
//
CNATFWUNSAFSRVOrigDomain::CNATFWUNSAFSRVOrigDomain(
    MNATFWUNSAFServerQuery& aServerQuery,
    TBool aFailIfNoSRVRecordsFound ) :
    CNATFWUNSAFQueryConditionBase(),
    iServerQuery ( aServerQuery ),
    iFailIfNoSRVRecordsFound( aFailIfNoSRVRecordsFound )
    {
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFSRVOrigDomain::ConstructL
// ----------------------------------------------------------------------------
//
void CNATFWUNSAFSRVOrigDomain::ConstructL()
    {
    HBufC8* temp = CNATFWUNSAFQuerySrv::SetTargetProtocolL(
                                    iServerQuery.QueryData().OriginalTarget(),
                                    iServerQuery.QueryData().Protocol(),
                                    iServerQuery.QueryData().ServiceName() );
    CleanupStack::PushL( temp );
    iServerQuery.QueryData().SetTargetL( *temp );
    iQueryBase = CNATFWUNSAFQuerySrv::NewL( *temp );
    CleanupStack::PopAndDestroy( temp );
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFSRVOrigDomain::NewL
// ----------------------------------------------------------------------------
//
CNATFWUNSAFSRVOrigDomain*
CNATFWUNSAFSRVOrigDomain::NewL( MNATFWUNSAFServerQuery& aServerQuery,
                           TBool aFailIfNoSRVRecordsFound )
    {
    CNATFWUNSAFSRVOrigDomain* self =
        new ( ELeave ) CNATFWUNSAFSRVOrigDomain( aServerQuery,
                                            aFailIfNoSRVRecordsFound );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFSRVOrigDomain::~CNATFWUNSAFSRVOrigDomain
// ----------------------------------------------------------------------------
//
CNATFWUNSAFSRVOrigDomain::~CNATFWUNSAFSRVOrigDomain()
    {
    delete iQueryBase;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFSRVOrigDomain::QueryBase
// ----------------------------------------------------------------------------
//
CNATFWUNSAFQueryBase& CNATFWUNSAFSRVOrigDomain::QueryBase()
    {
    return *iQueryBase;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFSRVOrigDomain::HandleQueryResultL
// ----------------------------------------------------------------------------
//
CNATFWUNSAFQueryConditionBase*
                    CNATFWUNSAFSRVOrigDomain::HandleQueryResultL( TInt aStatus )
    {
    if( aStatus == KErrNone )
        {
        iQueryBase->HandleQueryResultL( iServerQuery.QueryData().Resolver() );

        if ( ( iQueryBase->ArrayCountL() == 0 ) && iFailIfNoSRVRecordsFound )
            {
            return NULL;
            }

        if ( iQueryBase->ArrayCountL() != 0    )
            {
            iServerQuery.QueryData().SetPort( iQueryBase->ResultPortL() );
            iServerQuery.QueryData().SetTargetL( iQueryBase->ResultTargetL() );
            CNATFWUNSAFQueryConditionBase* conditionbase =
                    CNATFWUNSAFA_AAAASRVDomain::NewL( iServerQuery, iQueryBase);
            iQueryBase = NULL;//ownership is transferred
            return conditionbase;
            }
        }

    if ( ( iQueryBase->ArrayCountL() == 0 ) && iFailIfNoSRVRecordsFound )
        {
        return NULL;
        }

    iServerQuery.QueryData().SetPort( iServerQuery.QueryData().DefaultPort() );
    iServerQuery.QueryData().SetTargetL(
                                iServerQuery.QueryData().OriginalTarget() );
    return ( CNATFWUNSAFA_AAAAOrigDomain::NewL( iServerQuery) );
    }

