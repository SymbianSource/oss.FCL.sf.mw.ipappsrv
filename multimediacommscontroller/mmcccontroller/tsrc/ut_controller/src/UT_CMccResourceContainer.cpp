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
#include "UT_CMccResourceContainer.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "mccresourcecontainer.h"
#include "mccresourceitem.h"
#include "mccrtpdatasink.h"
#include "mccrtpdatasource.h"
#include "mccuids.hrh"

#include "mccunittestmacros.h"

const TUid KRtpDataSource            =  { KImplUidRtpDataSource   }; 
const TUid KRtpDataSink              =  { KImplUidRtpDataSink     };


// CONSTRUCTION
UT_CMccResourceContainer* UT_CMccResourceContainer::NewL()
    {
    UT_CMccResourceContainer* self = UT_CMccResourceContainer::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccResourceContainer* UT_CMccResourceContainer::NewLC()
    {
    UT_CMccResourceContainer* self = new( ELeave ) UT_CMccResourceContainer();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccResourceContainer::~UT_CMccResourceContainer()
    {
    }

// Default constructor
UT_CMccResourceContainer::UT_CMccResourceContainer()
    {
    }

// Second phase construct
void UT_CMccResourceContainer::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMccResourceContainer::SetupL(  )
    {
 
    iSourceStub = static_cast< CMccRtpDataSource* >( MDataSource::NewSourceL( KRtpDataSource, KNullDesC8 ) );
    iSinkStub = static_cast< CMccRtpDataSink* >(MDataSink::NewSinkL( KRtpDataSink, KNullDesC8 ));
    CMccResourceItem* item = CMccResourceItem::NewLC( 0, iSinkStub, NULL, 1, EFalse );
    User::LeaveIfError( iItems.Append(item) );
    CleanupStack::Pop(item);
    item = CMccResourceItem::NewLC( 1, NULL, iSourceStub, 2, EFalse );
    User::LeaveIfError( iItems.Append(item) );
    CleanupStack::Pop(item);
    
    /*
    EUNIT_ASSERT_SPECIFIC_LEAVE( iContainer->AddResourceItemL(param, NULL), KErrArgument );

    CMccResourceItem* item = CMccResourceItem::NewLC( 0, iSinkStub, NULL, 3, EFalse );
    EUNIT_ASSERT_NO_LEAVE( iContainer->AddResourceItemL(param, item) );

    CleanupStack::Pop(item);
    */
    TMccResourceParams param(0,0,0,0,EFalse,0);

    iOwnItem1 = CMccResourceItem::NewL( 0, iSinkStub, NULL, 3, EFalse );
    iOwnItem2 = CMccResourceItem::NewL( 1, NULL, iSourceStub, 4, EFalse );

    iContainer =  CMccResourceContainer::NewL(param, iItems);
    }

void UT_CMccResourceContainer::Teardown(  )
    {
    delete iContainer;
    delete iOwnItem2;
    delete iOwnItem1;
    iItems.ResetAndDestroy();
    delete iSinkStub;
    delete iSourceStub;
    }

void UT_CMccResourceContainer::UT_CMccResourceContainer_AddResourceItemLL(  )
    {
    TMccResourceParams param(0,0,0,0,EFalse,0);
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iContainer->AddResourceItemL(param, NULL), KErrArgument );
    MCC_EUNIT_ASSERT_NO_LEAVE( iContainer->AddResourceItemL(param, iOwnItem1) );   
    }

void UT_CMccResourceContainer::UT_CMccResourceContainer_FindResourceItemL(  )
    {
    TMccResourceParams param(0,0,0,0,EFalse,0);

    MCC_EUNIT_ASSERT_NO_LEAVE( iContainer->AddResourceItemL(param, iOwnItem1) );   
    MCC_EUNIT_ASSERT_NO_LEAVE( iContainer->AddResourceItemL(param, iOwnItem1) );   

    CMccResourceItem* item = NULL;
    item = iContainer->FindResourceItem(3);
    EUNIT_ASSERT( item != NULL );
    }

void UT_CMccResourceContainer::UT_CMccResourceContainer_StreamIdL(  )
    {
    EUNIT_ASSERT( iContainer->StreamId() == 0 );
    }

void UT_CMccResourceContainer::UT_CMccResourceContainer_SetResourceStateLL(  )
    {
    /*
    CMccResourceContainer::SetResourceStateL( 
    TUint32 aEndpointId, 
    MMccResources::TResourceState aState,
    TBool& aControlNetworkResources,
    RArray<TUint32>& aControlledEndpoints,
    RArray<TUint32>& aUncontrolledEndpoints )
    */
    TMccResourceParams param(0,0,0,0,EFalse,0);

    MCC_EUNIT_ASSERT_NO_LEAVE( iContainer->AddResourceItemL(param, iOwnItem1) );   
    MCC_EUNIT_ASSERT_NO_LEAVE( iContainer->AddResourceItemL(param, iOwnItem2) );   

    RArray<TUint32> controlledEndpoints;
    CleanupClosePushL( controlledEndpoints );
    RArray<TUint32> unControlledEndpoints;
    CleanupClosePushL( unControlledEndpoints );

    TBool controllerNR;
    iContainer->SetResourceStateL(0, MMccResources::EPrepared, 
        controllerNR, controlledEndpoints, unControlledEndpoints);
    
    EUNIT_ASSERT(controllerNR); 
    EUNIT_ASSERT( controlledEndpoints.Count() > 0 );
    EUNIT_ASSERT( unControlledEndpoints.Count() == 0 );

    CleanupStack::PopAndDestroy(&unControlledEndpoints);
    CleanupStack::PopAndDestroy(&controlledEndpoints);
    }

void UT_CMccResourceContainer::UT_CMccResourceContainer_SetResourceStateL2L(  )
    {
    /*
    CMccResourceContainer::SetResourceStateL( 
    TUint32 aEndpointId, 
    MMccResources::TResourceState aState,
    TBool& aControlNetworkResources,
    RArray<TUint32>& aControlledEndpoints,
    RArray<TUint32>& aUncontrolledEndpoints )
    */
    TMccResourceParams param(0,0,0,0,EFalse,0);

    MCC_EUNIT_ASSERT_NO_LEAVE( iContainer->AddResourceItemL(param, iOwnItem1) );   
    MCC_EUNIT_ASSERT_NO_LEAVE( iContainer->AddResourceItemL(param, iOwnItem2) );   

    RArray<TUint32> controlledEndpoints;
    CleanupClosePushL( controlledEndpoints );
    RArray<TUint32> unControlledEndpoints;
    CleanupClosePushL( unControlledEndpoints );

    TBool controllerNR;
    MCC_EUNIT_ASSERT_NO_LEAVE(iContainer->SetResourceStateL(0, MMccResources::EResourceStopped, 
        controllerNR, controlledEndpoints, unControlledEndpoints));
    
    EUNIT_ASSERT( controllerNR ); 
    EUNIT_ASSERT( controlledEndpoints.Count() > 0 );
    EUNIT_ASSERT( unControlledEndpoints.Count() == 0 );

    CleanupStack::PopAndDestroy(&unControlledEndpoints);
    CleanupStack::PopAndDestroy(&controlledEndpoints);
 
    }

void UT_CMccResourceContainer::UT_CMccResourceContainer_SetResourceStateL3L(  )
    {
    TMccResourceParams param(0,0,0,0,EFalse,0);

    MCC_EUNIT_ASSERT_NO_LEAVE( iContainer->AddResourceItemL(param, iOwnItem1) );   
    MCC_EUNIT_ASSERT_NO_LEAVE( iContainer->AddResourceItemL(param, iOwnItem2) );   

    RArray<TUint32> controlledEndpoints;
    CleanupClosePushL( controlledEndpoints );
    RArray<TUint32> unControlledEndpoints;
    CleanupClosePushL( unControlledEndpoints );

    TBool controllerNR;
    // wrong  endpointid
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(iContainer->SetResourceStateL(5, MMccResources::EStarted, 
        controllerNR, controlledEndpoints, unControlledEndpoints), KErrNotFound);
    
    MCC_EUNIT_ASSERT_NO_LEAVE(iContainer->SetResourceStateL(3, MMccResources::EPrepared, 
        controllerNR, controlledEndpoints, unControlledEndpoints));
    
    EUNIT_ASSERT( controllerNR ); 
    EUNIT_ASSERT( controlledEndpoints.Count() > 0 );
    EUNIT_ASSERT( unControlledEndpoints.Count() == 0 );

    CleanupStack::PopAndDestroy(&unControlledEndpoints);
    CleanupStack::PopAndDestroy(&controlledEndpoints);
  
    }

void UT_CMccResourceContainer::UT_CMccResourceContainer_SetResourceStateL4L(  )
    {
    TMccResourceParams param(0,0,0,0,EFalse,0);

    MCC_EUNIT_ASSERT_NO_LEAVE( iContainer->AddResourceItemL(param, iOwnItem1) );   
    MCC_EUNIT_ASSERT_NO_LEAVE( iContainer->AddResourceItemL(param, iOwnItem2) );   

    RArray<TUint32> controlledEndpoints;
    CleanupClosePushL( controlledEndpoints );
    RArray<TUint32> unControlledEndpoints;
    CleanupClosePushL( unControlledEndpoints );

    TBool controllerNR;
    MCC_EUNIT_ASSERT_NO_LEAVE(iContainer->SetResourceStateL(3, MMccResources::EResourceStopped, 
        controllerNR, controlledEndpoints, unControlledEndpoints));
    
    EUNIT_ASSERT( controllerNR ); 
    EUNIT_ASSERT( controlledEndpoints.Count() > 0 );
    EUNIT_ASSERT( unControlledEndpoints.Count() == 0 );

    CleanupStack::PopAndDestroy(&unControlledEndpoints);
    CleanupStack::PopAndDestroy(&controlledEndpoints);
  
    }
void UT_CMccResourceContainer::UT_CMccResourceContainer_IsStandByL(  )
    {
    EUNIT_ASSERT( !iContainer->IsStandBy() );
    }

void UT_CMccResourceContainer::UT_CMccResourceContainer_SetIgnoreEventL(  )
    {
    iContainer->SetStandbyControl(ETrue);
    EUNIT_ASSERT( iContainer->StandbyControl() );
    }

void UT_CMccResourceContainer::UT_CMccResourceContainer_IgnoreEventL(  )
    {
    iContainer->SetStandbyControl(EFalse);
    EUNIT_ASSERT( !iContainer->StandbyControl() );
    }

void UT_CMccResourceContainer::UT_CMccResourceContainer_FetchEndPointIdL(  )
    {
    EUNIT_ASSERT_SPECIFIC_LEAVE( iContainer->FetchStandbyEndpointL(), KErrNotFound );
    }

void UT_CMccResourceContainer::UT_CMccResourceContainer_SetEnableRtcpL(  )
    {
    iContainer->SetEnableRtcp(ETrue);
    EUNIT_ASSERT( iContainer->EnableRtcp() );
    }

void UT_CMccResourceContainer::UT_CMccResourceContainer_EnableRtcpL(  )
    {
    iContainer->SetEnableRtcp(EFalse);
    EUNIT_ASSERT( !iContainer->EnableRtcp() );
    }

void UT_CMccResourceContainer::UT_CMccResourceContainer_HandleEndpointRemovalL(  )
    {
    iContainer->HandleEndpointRemoval(0);
    EUNIT_ASSERT(iContainer->iOwnEndpoints.Count() == 0);
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccResourceContainer,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "AddResourceItemL - test ",
    "CMccResourceContainer",
    "AddResourceItemL",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourceContainer_AddResourceItemLL, Teardown)

EUNIT_TEST(
    "FindResourceItem - test ",
    "CMccResourceContainer",
    "FindResourceItem",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourceContainer_FindResourceItemL, Teardown)

EUNIT_TEST(
    "StreamId - test ",
    "CMccResourceContainer",
    "StreamId",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourceContainer_StreamIdL, Teardown)

EUNIT_TEST(
    "SetResourceStateL - test ",
    "CMccResourceContainer",
    "SetResourceStateL",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourceContainer_SetResourceStateLL, Teardown)

EUNIT_TEST(
    "SetResourceStateL2 - test ",
    "CMccResourceContainer",
    "SetResourceStateL2",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourceContainer_SetResourceStateL2L, Teardown)

EUNIT_TEST(
    "SetResourceStateL3 - test ",
    "CMccResourceContainer",
    "SetResourceStateL3",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourceContainer_SetResourceStateL3L, Teardown)

EUNIT_TEST(
    "SetResourceStateL4 - test ",
    "CMccResourceContainer",
    "SetResourceStateL4",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourceContainer_SetResourceStateL4L, Teardown)

EUNIT_TEST(
    "IsStandBy - test ",
    "CMccResourceContainer",
    "IsStandBy",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourceContainer_IsStandByL, Teardown)

EUNIT_TEST(
    "SetIgnoreEvent - test ",
    "CMccResourceContainer",
    "SetIgnoreEvent",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourceContainer_SetIgnoreEventL, Teardown)

EUNIT_TEST(
    "IgnoreEvent - test ",
    "CMccResourceContainer",
    "IgnoreEvent",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourceContainer_IgnoreEventL, Teardown)

EUNIT_TEST(
    "FetchEndPointId - test ",
    "CMccResourceContainer",
    "FetchEndPointId",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourceContainer_FetchEndPointIdL, Teardown)

EUNIT_TEST(
    "SetEnableRtcp - test ",
    "CMccResourceContainer",
    "SetEnableRtcp",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourceContainer_SetEnableRtcpL, Teardown)

EUNIT_TEST(
    "EnableRtcp - test ",
    "CMccResourceContainer",
    "EnableRtcp",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourceContainer_EnableRtcpL, Teardown)

EUNIT_TEST(
    "HandleEndpointRemoval - test ",
    "CMccResourceContainer",
    "HandleEndpointRemoval",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourceContainer_HandleEndpointRemovalL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
