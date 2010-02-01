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



#include "cnatfwunsafserverquery.h"
#include "cnatfwunsafqueryconditionbase.h"
#include "cnatfwunsafquerybase.h"
#include "cnatfwunsafquerydata.h"

const TInt CNATFWUNSAFServerQuery::iOffset =
    _FOFF(CNATFWUNSAFServerQuery, iLink);

// ----------------------------------------------------------------------------
// CNATFWUNSAFServerQuery::CNATFWUNSAFServerQuery
// ----------------------------------------------------------------------------
//
CNATFWUNSAFServerQuery::CNATFWUNSAFServerQuery()
    {
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFServerQuery::ConstructL
// ----------------------------------------------------------------------------
//
void CNATFWUNSAFServerQuery::ConstructL(
    MNATFWUNSAFServerResolverObserver* aObserver,
    CNATFWUNSAFQueryData* aQueryData )
    {
    iObserver = aObserver;
    iQueryData = aQueryData;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFServerQuery::NewL
// ----------------------------------------------------------------------------
//
CNATFWUNSAFServerQuery* CNATFWUNSAFServerQuery::NewL(
    MNATFWUNSAFServerResolverObserver* aObserver,
    CNATFWUNSAFQueryData* aQueryData )
    {
    CNATFWUNSAFServerQuery* self = NewLC( aObserver, aQueryData );
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFServerQuery::NewLC
// ----------------------------------------------------------------------------
//
CNATFWUNSAFServerQuery* CNATFWUNSAFServerQuery::NewLC(
    MNATFWUNSAFServerResolverObserver* aObserver,
    CNATFWUNSAFQueryData* aQueryData )
    {
    CNATFWUNSAFServerQuery* self =
        new ( ELeave ) CNATFWUNSAFServerQuery();
    CleanupStack::PushL( self );
    self->ConstructL( aObserver, aQueryData );
    return self;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFServerQuery::~CNATFWUNSAFServerQuery
// ----------------------------------------------------------------------------
//
CNATFWUNSAFServerQuery::~CNATFWUNSAFServerQuery()
    {
    delete iQueryCondition;
    delete iQueryData;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFServerQuery::Query
// ----------------------------------------------------------------------------
//
void CNATFWUNSAFServerQuery::Query( MNATFWUNSAFHostResolver& aResolver )
    {
    iQueryCondition->QueryBase().Query( aResolver );
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFServerQuery::HandleQueryResultL
// ----------------------------------------------------------------------------
//
TBool CNATFWUNSAFServerQuery::HandleQueryResultL( TInt aStatus )
    {
    CNATFWUNSAFQueryConditionBase* querycondition =
                                iQueryCondition->HandleQueryResultL( aStatus );
    TBool result = EFalse;
    if ( !querycondition )
        {
        delete iQueryCondition;
        iQueryCondition = NULL;
        result = EFalse;
        }
    else if ( querycondition == iQueryCondition )
            {
            result = ETrue;
            }
         else
             {
            delete iQueryCondition;
            iQueryCondition = querycondition;
            result = ETrue;
            }
    return result;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFServerQuery::ServerResolverObserver
// ----------------------------------------------------------------------------
//
MNATFWUNSAFServerResolverObserver*
CNATFWUNSAFServerQuery::ServerResolverObserver()
    {
    return iObserver;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFServerQuery::QueryData
// ----------------------------------------------------------------------------
//
CNATFWUNSAFQueryData& CNATFWUNSAFServerQuery::QueryData()
    {
    return *iQueryData;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFServerQuery::SetQueryConditionL
// ----------------------------------------------------------------------------
//
void CNATFWUNSAFServerQuery::SetQueryConditionL(
    CNATFWUNSAFQueryConditionBase* aQueryCondition )
    {
    __ASSERT_ALWAYS ( aQueryCondition, User::Leave( KErrArgument ) );
    delete iQueryCondition;
    iQueryCondition = aQueryCondition;
    }

