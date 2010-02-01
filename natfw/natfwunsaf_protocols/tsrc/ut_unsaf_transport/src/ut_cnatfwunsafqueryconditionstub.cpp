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




#include "ut_cnatfwunsafqueryconditionstub.h"
#include "ut_cnatfwunsafquerysrvstub.h"
#include "cnatfwunsafa_aaaaorigdomain.h"
#include "cnatfwunsafquerybase.h"
#include "cnatfwunsafquerysrv.h"
#include "mnatfwunsafserverquery.h"
#include "cnatfwunsafquerydata.h"
#include "mnatfwunsafhostresolver.h"

// ----------------------------------------------------------------------------
// UT_CNATFWUNSAFQueryConditionStub::NewL
// ----------------------------------------------------------------------------
//
UT_CNATFWUNSAFQueryConditionStub*
    UT_CNATFWUNSAFQueryConditionStub::NewL( MNATFWUNSAFServerQuery& aServerQuery )
    {
    UT_CNATFWUNSAFQueryConditionStub* self =
                new ( ELeave ) UT_CNATFWUNSAFQueryConditionStub( aServerQuery );
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// ----------------------------------------------------------------------------
// UT_CNATFWUNSAFQueryConditionStub::ConstructL
// ----------------------------------------------------------------------------
//
void UT_CNATFWUNSAFQueryConditionStub::ConstructL()
    {
    HBufC8* temp = CNATFWUNSAFQuerySrv::SetTargetProtocolL(
                                    iServerQuery.QueryData().OriginalTarget(),
                                    iServerQuery.QueryData().Protocol(),
                                    iServerQuery.QueryData().ServiceName());
    CleanupStack::PushL(temp);
    iServerQuery.QueryData().SetTargetL(*temp);
    iQueryBase = UT_CNATFWUNSAFQuerySrvStub::NewL(*temp);
    CleanupStack::PopAndDestroy();//temp
    }

// ----------------------------------------------------------------------------
// UT_CNATFWUNSAFQueryConditionStub::UT_CNATFWUNSAFQueryConditionStub
// ----------------------------------------------------------------------------
//
UT_CNATFWUNSAFQueryConditionStub::UT_CNATFWUNSAFQueryConditionStub(
                                            MNATFWUNSAFServerQuery& aServerQuery )
:CNATFWUNSAFQueryConditionBase(), iServerQuery (aServerQuery)
    {
    }

// ----------------------------------------------------------------------------
// UT_CNATFWUNSAFQueryConditionStub::~UT_CNATFWUNSAFQueryConditionStub
// ----------------------------------------------------------------------------
//
UT_CNATFWUNSAFQueryConditionStub::~UT_CNATFWUNSAFQueryConditionStub ()
    {
    delete iQueryBase;
    }

// ----------------------------------------------------------------------------
// UT_CNATFWUNSAFQueryConditionStub::QueryBase
// ----------------------------------------------------------------------------
//
CNATFWUNSAFQueryBase& UT_CNATFWUNSAFQueryConditionStub::QueryBase ()
    {
    return *iQueryBase;
    }

// ----------------------------------------------------------------------------
// UT_CNATFWUNSAFQueryConditionStub::HandleQueryResultL
// ----------------------------------------------------------------------------
//
CNATFWUNSAFQueryConditionBase*
                UT_CNATFWUNSAFQueryConditionStub::HandleQueryResultL(TInt aStatus)
    {
    UT_CNATFWUNSAFQueryConditionStub* condition = NULL;
    if(aStatus == KErrArgument)
        {
        iServerQuery.QueryData().SetPort(KSIPDefaultPort);
        iServerQuery.QueryData().SetTargetL(
                                    iServerQuery.QueryData().OriginalTarget());
        return (condition);
        }
    else if(aStatus == KErrCancel)
            {
            return this;
            }
        else
            {
            iQueryBase->HandleQueryResultL(iServerQuery.QueryData().Resolver());
            iServerQuery.QueryData().SetPort(iQueryBase->ResultPortL());
            iServerQuery.QueryData().SetTargetL(iQueryBase->ResultTargetL());
            return ( UT_CNATFWUNSAFQueryConditionStub::NewL( iServerQuery));
            }
    }



