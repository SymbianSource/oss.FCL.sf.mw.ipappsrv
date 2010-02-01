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



#include "cnatfwunsafhostresolver.h"
#include "natfwunsafserverresolverobserver.h"
#include "cnatfwunsafserverquery.h"
#include "cnatfwunsafquerydata.h"
#include "cnatfwunsafrequestqueue.h"
#include "natfwunsaflog.h"

// ----------------------------------------------------------------------------
// CNATFWUNSAFHostResolver::CNATFWUNSAFHostResolver
// ----------------------------------------------------------------------------
//
CNATFWUNSAFHostResolver::CNATFWUNSAFHostResolver()
 : CActive( EPriorityStandard )
    {
    CActiveScheduler::Add( this );
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFHostResolver::ConstructL
// ----------------------------------------------------------------------------
//
void CNATFWUNSAFHostResolver::ConstructL ( RSocketServ& aServer,
                                    RConnection& aConnection )
    {
    iObserverCancel = EFalse;
    iFailed = EFalse;
    User::LeaveIfError( iResolver.Open( aServer, KAfInet, KProtocolInetUdp,
                                        aConnection ) );
    iRequestQueue = CNATFWUNSAFRequestQueue::NewL();

    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFHostResolver::NewL
// ----------------------------------------------------------------------------
//
CNATFWUNSAFHostResolver* CNATFWUNSAFHostResolver::NewL( RSocketServ& aServer,
                                          RConnection& aConnection )
    {
    CNATFWUNSAFHostResolver* self = NewLC( aServer, aConnection );
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFHostResolver::NewLC
// ----------------------------------------------------------------------------
//
CNATFWUNSAFHostResolver* CNATFWUNSAFHostResolver::NewLC( RSocketServ& aServer,
                                           RConnection& aConnection )
    {
    CNATFWUNSAFHostResolver* self = new ( ELeave ) CNATFWUNSAFHostResolver();
    CleanupStack::PushL( self );
    self->ConstructL( aServer, aConnection );
    return self;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFHostResolver::~CNATFWUNSAFHostResolver
// ----------------------------------------------------------------------------
//
CNATFWUNSAFHostResolver::~CNATFWUNSAFHostResolver()
    {
    Cancel();
    iResolver.Close();
    delete iRequestQueue;
    delete iQuery;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFHostResolver::GetByQueryL
// ----------------------------------------------------------------------------
//
void CNATFWUNSAFHostResolver::GetByQueryL ( CNATFWUNSAFServerQuery* aQuery )
    {
    __ASSERT_ALWAYS( NULL != aQuery, User::Leave( KErrArgument ) );
    if( !IsActive() && !iQuery )
        {
        iQuery = aQuery;
        iQuery->Query( *this );
        SetActive();
        }
    else
        {
        iRequestQueue->Add( *aQuery );
        }
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFHostResolver::CancelResolving
// ----------------------------------------------------------------------------
//
void CNATFWUNSAFHostResolver::CancelResolving(
    const MNATFWUNSAFServerResolverObserver* aObserver )
    {
    if ( iQuery != 0 && iQuery->ServerResolverObserver() == aObserver )
        {
        Cancel();
        ResolveNext();
        }
    else
        {
        iRequestQueue->CancelQuery( aObserver );
        }
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFHostResolver::NextQuery
// ----------------------------------------------------------------------------
//
void CNATFWUNSAFHostResolver::NextQuery()
    {
    delete iQuery;
    iQuery = NULL;
    ResolveNext();
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFHostResolver::RunL
// ----------------------------------------------------------------------------
//
void CNATFWUNSAFHostResolver::RunL()
    {
    NATFWUNSAF_INTLOG("CNATFWUNSAFHostResolver::RunL: status", iStatus.Int())
    NATFWUNSAF_INTLOG("CNATFWUNSAFHostResolver::RunL: observerCancel",
        iObserverCancel)

    iFailed = EFalse;
    TBool subquery = EFalse;
    TInt status = iStatus.Int();
    MNATFWUNSAFServerResolverObserver* observer =
        iQuery->ServerResolverObserver();
    if ( !iObserverCancel )
        {
        subquery = iQuery->HandleQueryResultL( iStatus.Int() );
        }

    NATFWUNSAF_INTLOG("CNATFWUNSAFHostResolver::RunL: subquery", subquery);

    if ( subquery )
        {
        iQuery->Query( *this );
        SetActive();
        }
    else
        {
        if ( iObserverCancel )
            {
            iObserverCancel = EFalse;
            NextQuery();
            }
        else
            {
            if ( status == KErrNone )
                {
                NextQuery();
                observer->CompletedL();
                }
            else
                {//status _ne_ KErrNone || iFailed
                NextQuery();
                observer->ErrorOccured( status );
                }
            }
        }
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFHostResolver::RunError
// ----------------------------------------------------------------------------
//
TInt CNATFWUNSAFHostResolver::RunError ( TInt aError )
    {
    TInt err = KErrNone;
    // Complete the query, as RunL has not completed it.
    if ( iQuery )
        {
        MNATFWUNSAFServerResolverObserver* observer =
                                            iQuery->ServerResolverObserver();
        
        delete iQuery;
        iQuery = NULL;
        observer->ErrorOccured( aError );
        }

    if ( aError == KErrNoMemory )
        {
        err = aError;
        }

    return err;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFHostResolver::ResolveNext
// ----------------------------------------------------------------------------
//
void CNATFWUNSAFHostResolver::ResolveNext()
    {
    if ( !IsActive() )
        {
        iQuery = iRequestQueue->NextQuery();
        if ( iQuery )
            {
            iQuery->Query( *this );
            SetActive();
            }
        }
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFHostResolver::DoCancel
// ----------------------------------------------------------------------------
//
void CNATFWUNSAFHostResolver::DoCancel ()
    {
    iResolver.Cancel();
    if ( iQuery )
        {
        iObserverCancel = ETrue;
        delete iQuery;
        iQuery = NULL;
        }
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFHostResolver::Resolver
// ----------------------------------------------------------------------------
//
RHostResolver& CNATFWUNSAFHostResolver::Resolver ()
    {
    return iResolver;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFHostResolver::RequestStatus
// ----------------------------------------------------------------------------
//
TRequestStatus& CNATFWUNSAFHostResolver::RequestStatus ()
    {
    return iStatus;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFHostResolver::SetIPListFailed
// ----------------------------------------------------------------------------
//
void CNATFWUNSAFHostResolver::SetIPListFailed( TBool aFailed )
    {
    iFailed = aFailed;
    }
