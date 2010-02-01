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




#include <uriutils.h>
#include "natfwunsafserverresolverobserver.h"
#include "natfwunsafserverresolver.h"
#include "cnatfwunsafhostresolver.h"
#include "cnatfwunsafquerydata.h"
#include "cnatfwunsafserverquery.h"
#include "cnatfwunsafsrvorigdomain.h"
#include "cnatfwunsafquerysrv.h"
#include "cnatfwunsafa_aaaaorigdomain.h"
#include "cnatfwunsafquerya_aaaa.h"
#include "mnatfwunsafhostresolver.h"

// ----------------------------------------------------------------------------
// CNATFWUNSAFServerResolver::NewL
// ----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFServerResolver* CNATFWUNSAFServerResolver::NewL(
                                RSocketServ& aSocketServer,
                                RConnection& aConnection,
                                MNATFWUNSAFServerResolverObserver& aObserver,
                                TBool aFailIfNoSRVRecordsFound )
    {
    CNATFWUNSAFServerResolver* self = NewLC( aSocketServer,
                                        aConnection,
                                        aObserver,
                                        aFailIfNoSRVRecordsFound );
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFServerResolver::NewLC
// ----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFServerResolver* CNATFWUNSAFServerResolver::NewLC(
                                  RSocketServ& aSocketServer,
                                  RConnection& aConnection,
                                  MNATFWUNSAFServerResolverObserver& aObserver,
                                  TBool aFailIfNoSRVRecordsFound )
    {
    CNATFWUNSAFServerResolver* self =
        new ( ELeave ) CNATFWUNSAFServerResolver( aObserver,
                                             aFailIfNoSRVRecordsFound );
    CleanupStack::PushL( self );
    self->ConstructL( aSocketServer, aConnection );
    return self;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFServerResolver::CNATFWUNSAFServerResolver
// ----------------------------------------------------------------------------
//
CNATFWUNSAFServerResolver::CNATFWUNSAFServerResolver(
    MNATFWUNSAFServerResolverObserver& aObserver,
    TBool aFailIfNoSRVRecordsFound ) :
    iObserver( aObserver ),
    iFailIfNoSRVRecordsFound( aFailIfNoSRVRecordsFound )
    {
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFServerResolver::ConstructL
// ----------------------------------------------------------------------------
//
void CNATFWUNSAFServerResolver::ConstructL( RSocketServ& aSocketServer,
    RConnection& aConnection )
    {
    iResolver = CNATFWUNSAFHostResolver::NewL( aSocketServer, aConnection );
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFServerResolver::~CNATFWUNSAFServerResolver
// ----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFServerResolver::~CNATFWUNSAFServerResolver()
    {
    delete iResolver;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFServerResolver::ResolveL
// ----------------------------------------------------------------------------
//
EXPORT_C void CNATFWUNSAFServerResolver::ResolveL( const TDesC8& aHostName,
    const TDesC8& aServiceName,
    const TDesC8& aProtocol,
    TUint aDefaultPort,
    RArray<TInetAddr>& aResult)
    {
    __ASSERT_ALWAYS(aHostName.Length() > 0, User::Leave(KErrArgument));
    __ASSERT_ALWAYS(aServiceName.Length() > 0, User::Leave(KErrArgument));
    __ASSERT_ALWAYS(aProtocol.Length() > 0, User::Leave(KErrArgument));

    ChooseQueryL( aHostName, aServiceName, aProtocol, aDefaultPort, aResult );
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFServerResolver::ChooseQueryL
// ----------------------------------------------------------------------------
//
void CNATFWUNSAFServerResolver::ChooseQueryL( const TDesC8& aTarget,
    const TDesC8& aServiceName,
    const TDesC8& aProtocol,
    TUint aDefaultPort,
    RArray<TInetAddr>& aResult )
    {
    CNATFWUNSAFServerQuery* query = CreateQueryLC( aTarget, aServiceName,
                                              aProtocol, aDefaultPort,
                                              aResult );
    if (!( UriUtils::HostType( aTarget ) == UriUtils::ETextHost ) )
        {
        query->QueryData().SetPort( aDefaultPort );
        SetA_AAAAQueryL( query );
        }
    else
        {
        SetSRVQueryL( query );
        }

    CleanupStack::Pop( query );
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFServerResolver::SetA_AAAAQueryL
// ----------------------------------------------------------------------------
//
void CNATFWUNSAFServerResolver::SetA_AAAAQueryL(
    CNATFWUNSAFServerQuery* aQuery )
    {
    __ASSERT_ALWAYS( NULL != aQuery, User::Leave( KErrArgument ) );
    CNATFWUNSAFA_AAAAOrigDomain* querycondition =
        CNATFWUNSAFA_AAAAOrigDomain::NewL( *aQuery );
    CleanupStack::PushL( querycondition );
    aQuery->SetQueryConditionL( querycondition );
    CleanupStack::Pop( querycondition );
    iResolver->GetByQueryL( aQuery );
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFServerResolver::SetSRVQueryL
// ----------------------------------------------------------------------------
//
void CNATFWUNSAFServerResolver::SetSRVQueryL( CNATFWUNSAFServerQuery* aQuery )
    {
    __ASSERT_ALWAYS( NULL != aQuery, User::Leave( KErrArgument ) );
    CNATFWUNSAFSRVOrigDomain* querycondition =
        CNATFWUNSAFSRVOrigDomain::NewL( *aQuery, iFailIfNoSRVRecordsFound );
    CleanupStack::PushL( querycondition );
    aQuery->SetQueryConditionL( querycondition );
    CleanupStack::Pop( querycondition );
    iResolver->GetByQueryL( aQuery );
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFServerResolver::CancelResolving
// ----------------------------------------------------------------------------
//
EXPORT_C void CNATFWUNSAFServerResolver::CancelResolving()
    {
    iResolver->CancelResolving( &iObserver );
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFServerResolver::CreateQueryLC
// ----------------------------------------------------------------------------
//
CNATFWUNSAFServerQuery* CNATFWUNSAFServerResolver::CreateQueryLC(
    const TDesC8& aTarget,
    const TDesC8& aServiceName,
    const TDesC8& aProtocol,
    TUint aDefaultPort,
    RArray<TInetAddr>& aResult )
    {
    CNATFWUNSAFQueryData* querydata = CNATFWUNSAFQueryData::NewL( &iObserver,
                                                        aDefaultPort,
                                                        aProtocol,
                                                        aTarget,
                                                        aServiceName,
                                                        *iResolver,
                                                        aResult );
    CleanupStack::PushL( querydata );
    CNATFWUNSAFServerQuery* query =
        CNATFWUNSAFServerQuery::NewL( &iObserver, querydata );
    CleanupStack::Pop( querydata );
    CleanupStack::PushL( query );
    return query;
    }

