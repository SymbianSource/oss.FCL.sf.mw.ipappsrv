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




#include "cnatfwunsafrequestqueue.h"
#include "cnatfwunsafserverquery.h"
#include "natfwunsafserverresolverobserver.h"

// ----------------------------------------------------------------------------
// CNATFWUNSAFRequestQueue::CNATFWUNSAFRequestQueue
// ----------------------------------------------------------------------------
//
CNATFWUNSAFRequestQueue::CNATFWUNSAFRequestQueue():
    iList( CNATFWUNSAFServerQuery::iOffset ), iListIter( iList )
    {}

// ----------------------------------------------------------------------------
// CNATFWUNSAFRequestQueue::NewL
// ----------------------------------------------------------------------------
//
CNATFWUNSAFRequestQueue* CNATFWUNSAFRequestQueue::NewL()
    {
    CNATFWUNSAFRequestQueue* self = NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFRequestQueue::NewLC
// ----------------------------------------------------------------------------
//
CNATFWUNSAFRequestQueue* CNATFWUNSAFRequestQueue::NewLC()
    {
    CNATFWUNSAFRequestQueue* self = new ( ELeave ) CNATFWUNSAFRequestQueue();
    CleanupStack::PushL( self );
    return self;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFRequestQueue::~CNATFWUNSAFRequestQueue
// ----------------------------------------------------------------------------
//
CNATFWUNSAFRequestQueue::~CNATFWUNSAFRequestQueue()
    {
    CNATFWUNSAFServerQuery* query;
    iListIter.SetToFirst();
    query = iListIter++;
    while ( query )
        {
        iList.Remove( *query );
        delete query;
        query = iListIter++;
        }
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFRequestQueue::CancelQuery
// ----------------------------------------------------------------------------
//
void CNATFWUNSAFRequestQueue::CancelQuery(
    const MNATFWUNSAFServerResolverObserver* aObserver )
    {
    if ( !iList.IsEmpty() )
        {
        CNATFWUNSAFServerQuery* query;
        iListIter.SetToFirst();
        query = iListIter++;
        while( query )
            {
            if( query->ServerResolverObserver() == aObserver )
                {
                iList.Remove( *query );
                delete query;
                query = iListIter++;
                }
            }
        }
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFRequestQueue::Add
// ----------------------------------------------------------------------------
//
void CNATFWUNSAFRequestQueue::Add( CNATFWUNSAFServerQuery& aQuery )
    {
    if ( !iList.IsEmpty() )
        {
        CNATFWUNSAFServerQuery* query;
        iListIter.SetToFirst();
        query = iListIter++;
        while ( query )
            {
            query = iListIter++;
            }
        }
    iList.AddLast( aQuery );
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFRequestQueue::NextQuery
// ----------------------------------------------------------------------------
//
CNATFWUNSAFServerQuery* CNATFWUNSAFRequestQueue::NextQuery()
    {
    if ( !iList.IsEmpty() )
        {
        CNATFWUNSAFServerQuery* query = iList.First();
        iList.Remove( *query );
        return query;
        }
    return 0;
    }

