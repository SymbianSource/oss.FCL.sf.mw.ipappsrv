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




#include <UriUtils.h>
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
#include "ut_cstunbinding.h"

static MNATFWUNSAFServerResolverObserver* iCNATFWUNSAFServerResolverCallbackObserver = NULL;
static RArray<TInetAddr>* iCNATFWUNSAFServerResolverResult = NULL;

static TInt CNATFWUNSAFServerResolverCallbackComplete( TAny* aAny )
    {
    if ( iCNATFWUNSAFServerResolverCallbackObserver && iCNATFWUNSAFServerResolverResult )
        {
        TInetAddr addr;
        addr.SetFamily( KAfInet );
        addr.SetAddress( INET_ADDR( 11, 11, 11, 11 ) );
        TInt err = iCNATFWUNSAFServerResolverResult->Append( addr );
        if ( !err )
            {
            TRAP(err, iCNATFWUNSAFServerResolverCallbackObserver->CompletedL() );
            }
        if ( err == KErrNoMemory && ut_cstunbinding::iActiveSchedulerWait.IsStarted() )
            {
            ut_cstunbinding::iActiveSchedulerWait.AsyncStop();
            }
        }
    return 0;
    }

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
void CNATFWUNSAFServerResolver::ConstructL( RSocketServ& /*aSocketServer*/,
    RConnection& /*aConnection*/ )
    {
    iResolver = reinterpret_cast<CNATFWUNSAFHostResolver*>( CPeriodic::NewL( CActive::EPriorityStandard ) );
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFServerResolver::~CNATFWUNSAFServerResolver
// ----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFServerResolver::~CNATFWUNSAFServerResolver()
    {
    if ( reinterpret_cast<CPeriodic*>( iResolver ) )
        {
        reinterpret_cast<CPeriodic*>( iResolver )->Cancel();
        }
    delete reinterpret_cast<CPeriodic*>( iResolver );
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFServerResolver::ResolveL
// ----------------------------------------------------------------------------
//
EXPORT_C void CNATFWUNSAFServerResolver::ResolveL( const TDesC8& aHostName,
    const TDesC8& aServiceName,
    const TDesC8& aProtocol,
    TUint /*aDefaultPort*/,
    RArray<TInetAddr>& aResult)
    {
    __ASSERT_ALWAYS(aHostName.Length() > 0, User::Leave(KErrArgument));
    __ASSERT_ALWAYS(aServiceName.Length() > 0, User::Leave(KErrArgument));
    __ASSERT_ALWAYS(aProtocol.Length() > 0, User::Leave(KErrArgument));

    iCNATFWUNSAFServerResolverCallbackObserver = &iObserver;
    
    iCNATFWUNSAFServerResolverResult = &aResult;
    reinterpret_cast<CPeriodic*>( iResolver )->Cancel();
    reinterpret_cast<CPeriodic*>( iResolver )->Start( 
            1, 100000000, TCallBack( CNATFWUNSAFServerResolverCallbackComplete, this ) );
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFServerResolver::ChooseQueryL
// ----------------------------------------------------------------------------
//
void CNATFWUNSAFServerResolver::ChooseQueryL( const TDesC8& /*aTarget*/,
    const TDesC8& /*aServiceName*/,
    const TDesC8& /*aProtocol*/,
    TUint /*aDefaultPort*/,
    RArray<TInetAddr>& /*aResult */)
    {
    
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFServerResolver::SetA_AAAAQueryL
// ----------------------------------------------------------------------------
//
void CNATFWUNSAFServerResolver::SetA_AAAAQueryL(
    CNATFWUNSAFServerQuery* /*aQuery*/ )
    {
    
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFServerResolver::SetSRVQueryL
// ----------------------------------------------------------------------------
//
void CNATFWUNSAFServerResolver::SetSRVQueryL( CNATFWUNSAFServerQuery* /*aQuery*/ )
    {
    
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFServerResolver::CancelResolving
// ----------------------------------------------------------------------------
//
EXPORT_C void CNATFWUNSAFServerResolver::CancelResolving()
    {
    reinterpret_cast<CPeriodic*>( iResolver )->Cancel();
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFServerResolver::CreateQueryLC
// ----------------------------------------------------------------------------
//
CNATFWUNSAFServerQuery* CNATFWUNSAFServerResolver::CreateQueryLC(
    const TDesC8& /*aTarget*/,
    const TDesC8& /*aServiceName*/,
    const TDesC8& /*aProtocol*/,
    TUint /*aDefaultPort*/,
    RArray<TInetAddr>& /*aResult */)
    {
    return NULL;
    }

