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




//  CLASS HEADER
#include "UT_CMccResourceIterators.h"
#include "mccuids.hrh"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "mccresourceiterators.h"
#include "mccresourceitem.h"
#include "mccresourcecontainer.h"
#include "mccrtpdatasink.h"
#include "mccrtpdatasource.h"

const TUid KRtpDataSource            =  { KImplUidRtpDataSource   }; 
const TUid KRtpDataSink              =  { KImplUidRtpDataSink     };
const TUid KUnknown                  =  { 0x0FF00FF0 };

// CONSTRUCTION
UT_CMccResourceIterators* UT_CMccResourceIterators::NewL()
    {
    UT_CMccResourceIterators* self = UT_CMccResourceIterators::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccResourceIterators* UT_CMccResourceIterators::NewLC()
    {
    UT_CMccResourceIterators* self = new( ELeave ) UT_CMccResourceIterators();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccResourceIterators::~UT_CMccResourceIterators()
    {
    }

// Default constructor
UT_CMccResourceIterators::UT_CMccResourceIterators()
    {
    }

// Second phase construct
void UT_CMccResourceIterators::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMccResourceIterators::SetupL(  )
    {
    iSourceStub = static_cast< CMccRtpDataSource* >( MDataSource::NewSourceL( KRtpDataSource, KNullDesC8 ) );
    iSinkStub = static_cast< CMccRtpDataSink* >(MDataSink::NewSinkL( KRtpDataSink, KNullDesC8 ) );
    
    TMccResourceParams param(1,1,1,0,EFalse,0);
    CMccResourceContainer* container =  CMccResourceContainer::NewLC( param, iItems );
    iContainers.AppendL( container );
    CleanupStack::Pop( container );
    
    CMccResourceItem* item = CMccResourceItem::NewLC( 1, iSinkStub, NULL, 1, EFalse );
    container->AddResourceItemL( param, item );
    CleanupStack::Pop( item );
    iItems.AppendL( item );

    item = CMccResourceItem::NewLC( 1, NULL, iSourceStub, 2, EFalse );
    container->AddResourceItemL( param, item );
    CleanupStack::Pop( item );
    iItems.AppendL( item );
    
    TMccResourceParams param2(2,2,2,0,EFalse,0);
    CMccResourceContainer* container2 =  CMccResourceContainer::NewLC( param2, iItems );
    iContainers.AppendL( container2 );
    CleanupStack::Pop( container2 );
    
    item = CMccResourceItem::NewLC( 2, iSinkStub, NULL, 3, EFalse );
    container2->AddResourceItemL( param2, item );
    CleanupStack::Pop( item );
    iItems.AppendL( item );

    
    item = CMccResourceItem::NewLC( 2, NULL, iSourceStub, 4, EFalse );
    container2->AddResourceItemL( param2, item );
    CleanupStack::Pop( item );
    iItems.AppendL( item );
    }

void UT_CMccResourceIterators::Teardown(  )
    {
    iContainers.ResetAndDestroy();
    iItems.Reset();
    delete iSinkStub;
    delete iSourceStub;
    }

void UT_CMccResourceIterators::UT_CMccResourceIterators_IterateContainersL(  )
    {
    TMccResourceContainerIterator iterator( iContainers );
    
    // Iterating all
    CMccResourceContainer* container = NULL;
    TInt count = 0;
    while ( iterator.Next( container ) )
        {
        count++;
        }
    EUNIT_ASSERT_EQUALS( count, 2 );
    EUNIT_ASSERT_EQUALS( iterator.Current(), 1 );
    
    // Iterating by linkid
    TMccResourceParams param(0,1,0,0,EFalse,0);
    TMccResourceContainerIterator iterator2( iContainers, param );
    container = NULL;
    EUNIT_ASSERT_EQUALS( iterator2.Next( container ), ETrue );
    EUNIT_ASSERT_EQUALS( container->LinkId(), 1 );
    
    TMccResourceParams param2(0,2,0,0,EFalse,0);
    TMccResourceContainerIterator iterator3( iContainers, param2 );
    container = NULL;
    EUNIT_ASSERT_EQUALS( iterator3.Next( container ), ETrue );
    EUNIT_ASSERT_EQUALS( container->LinkId(), 2 );
 
    // Test reset
    EUNIT_ASSERT_EQUALS( iterator3.Next( container ), EFalse );
    iterator3.Reset();
    EUNIT_ASSERT_EQUALS( iterator3.Next( container ), ETrue );
    
    // Iterating by streamid
    TMccResourceParams param3(0,0,1,0,EFalse,0);
    TMccResourceContainerIterator iterator4( iContainers, param3 );
    container = NULL;
    EUNIT_ASSERT_EQUALS( iterator4.Next( container ), ETrue );
    EUNIT_ASSERT_EQUALS( container->StreamId(), 1 );
    
    TMccResourceParams param4(0,0,2,0,EFalse,0);
    TMccResourceContainerIterator iterator5( iContainers, param4 );
    container = NULL;
    EUNIT_ASSERT_EQUALS( iterator5.Next( container ), ETrue );
    EUNIT_ASSERT_EQUALS( container->StreamId(), 2 );
    
    // Delete container by using iterator
    EUNIT_ASSERT_EQUALS( iContainers.Count(), 2 );
    iterator.Delete( iContainers );
    EUNIT_ASSERT_EQUALS( iContainers.Count(), 1 );
    iterator.Delete( iContainers );
    EUNIT_ASSERT_EQUALS( iContainers.Count(), 0 );
    EUNIT_ASSERT_EQUALS( iterator.Current(), KErrNotFound );
    }
    
void UT_CMccResourceIterators::UT_CMccResourceIterators_IterateResourceItemsL(  )
    {
    TMccResourceItemIterator iterator( iItems );
    
    // Iterating all
    CMccResourceItem* item = NULL;
    TInt count = 0;
    while ( iterator.Next( item ) )
        {
        count++;
        }
    EUNIT_ASSERT_EQUALS( count, 4 );
    EUNIT_ASSERT_EQUALS( iterator.Current(), 3 );
    
    // Iterate by uid
    
    // 2 rtp source
    TMccResourceItemIterator iterator2( iItems, KRtpDataSource );
    item = NULL;
    count = 0;
    while ( iterator2.Next( item ) )
        {
        count++;
        }
    EUNIT_ASSERT_EQUALS( count, 2 );
    
    // 2 rtp sinks
    TMccResourceItemIterator iterator3( iItems, KRtpDataSink );
    item = NULL;
    count = 0;
    while ( iterator3.Next( item ) )
        {
        count++;
        }
    EUNIT_ASSERT_EQUALS( count, 2 );
       
    // 0 "unknown"
    TMccResourceItemIterator iterator4( iItems, KUnknown );
    item = NULL;
    EUNIT_ASSERT_EQUALS( iterator4.Next( item ), EFalse );
    
    // Iterate by resource params
    
    // 2 having same streamid
    TMccResourceParams param(0,0,1,0,EFalse,0);
    TMccResourceItemIterator iterator5( iItems, param );
    item = NULL;
    count = 0;
    while ( iterator5.Next( item ) )
        {
        count++;
        }
    EUNIT_ASSERT_EQUALS( count, 2 );
    
    // 2 having same link id
    TMccResourceParams param2(0,2,0,0,EFalse,0);
    TMccResourceItemIterator iterator6( iItems, param2 );
    item = NULL;
    count = 0;
    while ( iterator6.Next( item ) )
        {
        count++;
        }
    EUNIT_ASSERT_EQUALS( count, 2 );
    
    // 2 having same session id
    TMccResourceParams paramSes(1,0,0,0,EFalse,0);
    TMccResourceItemIterator iteratorSes( iItems, KMccRtpSourceUid, paramSes );
    item = NULL;
    count = 0;
    while ( iteratorSes.Next( item ) )
        {
        count++;
        }
    EUNIT_ASSERT_EQUALS( count, 1 );
    
    // 2 having different session id
    TMccResourceParams paramSes1(3,0,0,0,EFalse,0);
    TMccResourceItemIterator iteratorSes1( iItems, KMccRtpSourceUid, paramSes1 );
    item = NULL;
    count = 0;
    while ( iteratorSes1.Next( item ) )
        {
        count++;
        }
    EUNIT_ASSERT_EQUALS( count, 0 );
    
    // 0 having stream id 3
    param.iStreamId = 3;
    TMccResourceItemIterator iterator7( iItems, param );
    item = NULL;
    EUNIT_ASSERT_EQUALS( iterator7.Next( item ), EFalse );

    // Iterate by endpoint id
    TMccResourceItemIterator iterator8( iItems, (TUint32)2 );
    item = NULL;
    EUNIT_ASSERT_EQUALS( iterator8.Next( item ), ETrue );
    EUNIT_ASSERT_EQUALS( item->EndpointId(), 2 );
    EUNIT_ASSERT_EQUALS( iterator8.Current(), 1 );
    
    TMccResourceItemIterator iterator9( iItems, (TUint32)8 );
    item = NULL;
    EUNIT_ASSERT_EQUALS( iterator9.Next( item ), EFalse );
    
    // Test empty iterator
    RPointerArray<CMccResourceItem> array;
    TMccResourceItemIterator iterator10( array );
    EUNIT_ASSERT_EQUALS( iterator10.Next( item ), EFalse );
    EUNIT_ASSERT_EQUALS( iterator10.Current(), KErrNotFound );
    
    iterator10.Reset();
    
    array.Close();
    }
    
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccResourceIterators,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "IterateContainers - test ",
    "TMccResourceIterators",
    "IterateContainers",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourceIterators_IterateContainersL, Teardown)

EUNIT_TEST(
    "IterateResourceItems - test ",
    "TMccResourceIterators",
    "IterateResourceItems",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourceIterators_IterateResourceItemsL, Teardown)
    
EUNIT_END_TEST_TABLE

//  END OF FILE
