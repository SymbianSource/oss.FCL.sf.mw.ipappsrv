/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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




#include "mccresourceiterators.h"
#include "mccresourcecontainer.h"
#include "mccresourceitem.h"
#include <mmf/server/mmfdatasink.h>
#include <mmf/server/mmfdatasource.h>

// ============================ MEMBER FUNCTIONS ===============================


// CONTAINER ITERATOR

// -----------------------------------------------------------------------------
// TMccResourceContainerIterator::TMccResourceContainerIterator
// -----------------------------------------------------------------------------
//
TMccResourceContainerIterator::TMccResourceContainerIterator( 
    const RPointerArray<CMccResourceContainer>& aContainers ) : 
    iContainers( aContainers ),
    iCurrentIndex( 0 ),
    iResourceParams( NULL ),
    iEndpointId( 0 )
    {
    }

// -----------------------------------------------------------------------------
// TMccResourceContainerIterator::TMccResourceContainerIterator
// -----------------------------------------------------------------------------
//
TMccResourceContainerIterator::TMccResourceContainerIterator( 
    const RPointerArray<CMccResourceContainer>& aContainers,
    TMccResourceParams& aResourceParams ) : 
    iContainers( aContainers ),
    iCurrentIndex( 0 ),
    iResourceParams( &aResourceParams ),
    iEndpointId( 0 )
    {
    }

// -----------------------------------------------------------------------------
// TMccResourceContainerIterator::TMccResourceContainerIterator
// -----------------------------------------------------------------------------
//
TMccResourceContainerIterator::TMccResourceContainerIterator( 
    const RPointerArray<CMccResourceContainer>& aContainers,
    TUint32 aEndpointId ) : 
    iContainers( aContainers ),
    iCurrentIndex( 0 ),
    iResourceParams( NULL ),
    iEndpointId( aEndpointId )
    {
    }
    
// -----------------------------------------------------------------------------
// TMccResourceContainerIterator::Current
// -----------------------------------------------------------------------------
//
TInt TMccResourceContainerIterator::Current()
    {
    TInt current = KErrNotFound;
    
    if ( iContainers.Count() > 0 )
        {
        current = iCurrentIndex-1;
        current = current < 0 ? 0 : current;
        }
    return current;
    }

// -----------------------------------------------------------------------------
// TMccResourceContainerIterator::IsEof
// -----------------------------------------------------------------------------
//
TBool TMccResourceContainerIterator::IsEof()
    {
    return iContainers.Count() == 0 || iCurrentIndex >= iContainers.Count();
    }

// -----------------------------------------------------------------------------
// TMccResourceContainerIterator::Next
// -----------------------------------------------------------------------------
//
TBool TMccResourceContainerIterator::Next( 
    CMccResourceContainer*& aCandidate,
    TMccIteratorMatchType aMatchType )
    {
    CMccResourceContainer* next = NULL;
    TBool negation = aMatchType == ExactMatch ? EFalse : ETrue;
    
    while( !next && !IsEof() )
        {
        CMccResourceContainer* container = iContainers[ iCurrentIndex ];
        TBool condition = EFalse;

        if ( iResourceParams )
            {
            if ( iResourceParams->iLinkId )
                {
                condition = ( container->LinkId() == iResourceParams->iLinkId );
                }
            else if ( iResourceParams->iStreamId )
                {
                condition = ( container->StreamId() == iResourceParams->iStreamId );
                }
            else if ( iEndpointId )
                {
                condition = ( container->FindResourceItem( iEndpointId ) != NULL );
                }
            else
                {
                }
            }
        else
            {
            condition = ETrue;
            }
            
        next = ( ( negation && !condition ) ||
               ( !negation && condition ) ) ? container : NULL;
               
        iCurrentIndex++;                            
        }
    
    aCandidate = next;
    return aCandidate != NULL;
    }

// -----------------------------------------------------------------------------
// TMccResourceContainerIterator::Delete
// -----------------------------------------------------------------------------
//
TInt TMccResourceContainerIterator::Delete( RPointerArray<CMccResourceContainer>& aContainers )
    {
    TInt currentIndex = Current();
    
    if ( currentIndex != KErrNotFound && currentIndex < aContainers.Count() )
        {
        delete aContainers[ currentIndex ];
        aContainers.Remove( currentIndex );
        iCurrentIndex = currentIndex;
        }
    
    return currentIndex;
    }
    
// -----------------------------------------------------------------------------
// TMccResourceContainerIterator::Reset
// -----------------------------------------------------------------------------
//
void TMccResourceContainerIterator::Reset()
    {
    iCurrentIndex = 0;
    }


// RESOURCE ITEM ITERATOR

// -----------------------------------------------------------------------------
// TMccResourceItemIterator::TMccResourceItemIterator
// -----------------------------------------------------------------------------
//
TMccResourceItemIterator::TMccResourceItemIterator( 
    const RPointerArray<CMccResourceItem>& aItems,
    TBool aOnlyInternals,
    TBool aOnlySinks,
    TBool aOnlySources ) : 
    iItems( aItems ),
    iCurrentIndex( 0 ),
    iOnlyInternals( aOnlyInternals ),
    iOnlySinks( aOnlySinks ),
    iOnlySources( aOnlySources ),
    iUid( KNullUid ),
    iResourceParams( NULL ),
    iEndpointId( 0 )
    {
    }

// -----------------------------------------------------------------------------
// TMccResourceItemIterator::TMccResourceItemIterator
// -----------------------------------------------------------------------------
//
TMccResourceItemIterator::TMccResourceItemIterator( 
    const RPointerArray<CMccResourceItem>& aItems, 
    TUid aUid ) : 
    iItems( aItems ),
    iCurrentIndex( 0 ),
    iOnlyInternals( EFalse ),
    iOnlySinks( EFalse ),
    iOnlySources( EFalse ),
    iUid( aUid ),
    iResourceParams( NULL ),
    iEndpointId( 0 )
    {
    }

// -----------------------------------------------------------------------------
// TMccResourceItemIterator::TMccResourceItemIterator
// -----------------------------------------------------------------------------
//
TMccResourceItemIterator::TMccResourceItemIterator( 
    const RPointerArray<CMccResourceItem>& aItems,
    TUid aUid,
    TMccResourceParams& aResourceParams ) :
    iItems( aItems ),
    iCurrentIndex( 0 ),
    iOnlyInternals( EFalse ),
    iOnlySinks( EFalse ),
    iOnlySources( EFalse ),
    iUid( aUid ),
    iResourceParams( &aResourceParams ),
    iEndpointId( 0 )    
    {
    }
    
// -----------------------------------------------------------------------------
// TMccResourceItemIterator::TMccResourceItemIterator
// -----------------------------------------------------------------------------
//
TMccResourceItemIterator::TMccResourceItemIterator( 
    const RPointerArray<CMccResourceItem>& aItems,
    TMccResourceParams& aResourceParams ) : 
    iItems( aItems ),
    iCurrentIndex( 0 ),
    iOnlyInternals( EFalse ),
    iOnlySinks( EFalse ),
    iOnlySources( EFalse ),
    iUid( KNullUid ),
    iResourceParams( &aResourceParams ),
    iEndpointId( 0 )
    {
    }
    
// -----------------------------------------------------------------------------
// TMccResourceItemIterator::TMccResourceItemIterator
// -----------------------------------------------------------------------------
//
TMccResourceItemIterator::TMccResourceItemIterator( 
    const RPointerArray<CMccResourceItem>& aItems,
    TUint32 aEndpointId ) : 
    iItems( aItems ),
    iCurrentIndex( 0 ),
    iOnlyInternals( EFalse ),
    iOnlySinks( EFalse ),
    iOnlySources( EFalse ),
    iUid( KNullUid ),
    iResourceParams(NULL ),
    iEndpointId( aEndpointId )
    {
    }
       
// -----------------------------------------------------------------------------
// TMccResourceItemIterator::Current
// -----------------------------------------------------------------------------
//
TInt TMccResourceItemIterator::Current()
    {
    TInt current = KErrNotFound;
    
    if ( iItems.Count() > 0 )
        {
        current = iCurrentIndex-1;
        current = current < 0 ? 0 : current;
        }
    return current;
    }

// -----------------------------------------------------------------------------
// TMccResourceItemIterator::IsEof
// -----------------------------------------------------------------------------
//
TBool TMccResourceItemIterator::IsEof()
    {
    return iItems.Count() == 0 || iCurrentIndex >= iItems.Count();
    }

// -----------------------------------------------------------------------------
// TMccResourceItemIterator::Next
// -----------------------------------------------------------------------------
//
TBool TMccResourceItemIterator::Next( 
    CMccResourceItem*& aCandidate,
    TMccIteratorMatchType aMatchType )
    {
    CMccResourceItem* next = NULL;
    TBool negation = aMatchType == ExactMatch ? EFalse : ETrue;
    
    while( !next && !IsEof() )
        {
        CMccResourceItem* item = iItems[ iCurrentIndex ];
        TBool condition = EFalse;
        
        if ( iUid != KNullUid )
            {
            condition = ( item->IsSink() && item->Sink()->DataSinkType() == iUid ) ||
                        ( item->IsSource() && item->Source()->DataSourceType() == iUid );
            if ( condition && iResourceParams )
                {
                condition = item->MatchSession( iResourceParams->iSessionId );
                }
            }
        else if ( iResourceParams )
            {
            condition = item->Match( *iResourceParams );
            }
        else if ( iEndpointId )
            {
            condition = iEndpointId == item->EndpointId();
            }
        else
            {
            condition = ( !iOnlySinks || item->IsSink() ) &&
                        ( !iOnlySources || item->IsSource() ) &&
                        ( !iOnlyInternals || item->IsInternal() );
            }
            
        next = ( ( negation && !condition ) ||
               ( !negation && condition ) ) ? item : NULL;
               
        iCurrentIndex++;                            
        }
    
    aCandidate = next;
    return aCandidate != NULL;
    }

// -----------------------------------------------------------------------------
// TMccResourceItemIterator::Reset
// -----------------------------------------------------------------------------
//
void TMccResourceItemIterator::Reset()
    {
    iCurrentIndex = 0;
    }
      
// End of file

