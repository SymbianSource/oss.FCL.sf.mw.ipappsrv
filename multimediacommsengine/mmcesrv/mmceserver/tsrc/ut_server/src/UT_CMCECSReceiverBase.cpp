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
#include "ut_cmcecsreceiverbase.h"
#include "MCETestHelper.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "mcecsreceiverbase.h"
#include "mcecsreceivequeue.h"

#include "mcecsserveritc.h"
#include "mcecssessionreceiver.h"
#include "mceclientserver.h"
#include "mcecsreceiveitem.h"

// CONSTRUCTION
UT_CMceCsReceiverBase* UT_CMceCsReceiverBase::NewL()
    {
    UT_CMceCsReceiverBase* self = UT_CMceCsReceiverBase::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMceCsReceiverBase* UT_CMceCsReceiverBase::NewLC()
    {
    UT_CMceCsReceiverBase* self = new( ELeave ) UT_CMceCsReceiverBase();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor(virtual by CBase)
UT_CMceCsReceiverBase::~UT_CMceCsReceiverBase()
    {
    }

// Default constructor
UT_CMceCsReceiverBase::UT_CMceCsReceiverBase()
    {
    }

// Second phase construct
void UT_CMceCsReceiverBase::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMceCsReceiverBase::SetupL()
    {
     iITC = CMceCsServerITC::NewL();
    // Using a concrete subclass to test the abstract base class
    iReceiver = CMceCsSessionReceiver::NewL( KMceCSSIPSession, *iITC );

    } 

void UT_CMceCsReceiverBase::Teardown()
    {
    delete iReceiver;
    delete iITC;
    }

void UT_CMceCsReceiverBase::UT_CMceCsReceiverBase_ClientReadyToReceiveLL()
    {
    
    EUNIT_ASSERT( iReceiver->iReceiveQueue->IsEmpty() );
    
     // Test with nothing in the receive queue
    EUNIT_ASSERT( !iReceiver->iClientReadyToReceive );
    iReceiver->ClientReadyToReceiveL( RMessage2() );
    EUNIT_ASSERT( iReceiver->iClientReadyToReceive );
    
    iReceiver->iClientReadyToReceive = ETrue;
    EUNIT_ASSERT( iReceiver->iClientReadyToReceive );
    EUNIT_ASSERT_LEAVE( iReceiver->ClientReadyToReceiveL( RMessage2() ) );
    EUNIT_ASSERT( iReceiver->iClientReadyToReceive );
    
    iReceiver->iClientReadyToReceive = EFalse;

    TMceIds ids;
	TMceCSReceiveItem item1( ids, KErrNone );

    iReceiver->iReceiveQueue->AddLast( item1 );
    iReceiver->ClientReadyToReceiveL( RMessage2() );
    
    EUNIT_ASSERT( iReceiver->iReceiveQueue->IsEmpty() );

    HBufC8* context = _L8("context").AllocLC();
    HBufC8* content = NULL;
    
	TMceCSReceiveItem item2( ids, context, content );
    iReceiver->iReceiveQueue->AddLast( item2 );
	CleanupStack::Pop( context );
    
    iReceiver->ClientReadyToReceiveL( RMessage2() );
    
    EUNIT_ASSERT( !iReceiver->iReceiveQueue->IsEmpty() );
	iReceiver->ReceiveL( RMessage2() );
    EUNIT_ASSERT( iReceiver->iReceiveQueue->IsEmpty() );

    context = _L8("context").AllocLC();
    content = _L8("content").AllocLC();
    
	TMceCSReceiveItem item3( ids, context, content );
	CleanupStack::Pop( content );
	CleanupStack::Pop( context );

    iReceiver->iReceiveQueue->AddLast( item3 );
    iReceiver->ClientReadyToReceiveL( RMessage2() );
    
    EUNIT_ASSERT( !iReceiver->iReceiveQueue->IsEmpty() );
	iReceiver->ReceiveL( RMessage2() );
    EUNIT_ASSERT( iReceiver->iReceiveQueue->IsEmpty() );

    context = NULL;
    content = _L8("content").AllocLC();
    
	TMceCSReceiveItem item4( ids, context, content );
    iReceiver->iReceiveQueue->AddLast( item4 );
	CleanupStack::Pop( content );
    
    iReceiver->ClientReadyToReceiveL( RMessage2() );
    
    EUNIT_ASSERT( !iReceiver->iReceiveQueue->IsEmpty() );
	iReceiver->ReceiveL( RMessage2() );
    EUNIT_ASSERT( iReceiver->iReceiveQueue->IsEmpty() );
    

    }
    

void UT_CMceCsReceiverBase::UT_CMceCsReceiverBase_CancelClientReceiveLL()
    {


    iReceiver->ClientReadyToReceiveL( RMessage2() );
    EUNIT_ASSERT( iReceiver->iClientReadyToReceive );
    iReceiver->CancelClientReceiveL();
    EUNIT_ASSERT( !iReceiver->iClientReadyToReceive );

    iReceiver->iClientReadyToReceive = EFalse;
    EUNIT_ASSERT_LEAVE( iReceiver->CancelClientReceiveL() );
    EUNIT_ASSERT( !iReceiver->iClientReadyToReceive );

    }

void UT_CMceCsReceiverBase::UT_CMceCsReceiverBase_ReceiveLL()
    {
    TMceIds ids;

    HBufC8* context = NULL;
    HBufC8* content = NULL;

    EUNIT_ASSERT_LEAVE( iReceiver->SendToClientL( ids, context ) );
    EUNIT_ASSERT_LEAVE( iReceiver->SendToClientL( ids, context, content ) );
    
    context = _L8("context").AllocL();    
    EUNIT_ASSERT_LEAVE( iReceiver->SendToClientL( ids, context, content ) );
	delete context;
	
    iReceiver->ClientReadyToReceiveL( RMessage2() );
    EUNIT_ASSERT( iReceiver->iClientReadyToReceive );
    
    context = _L8("context").AllocLC();
    iReceiver->SendToClientL( ids, context );
    CleanupStack::Pop( context );
    
	iReceiver->ReceiveL( RMessage2() );
    iReceiver->ClientReadyToReceiveL( RMessage2() );
    EUNIT_ASSERT( iReceiver->iClientReadyToReceive );

    context = _L8("context").AllocLC();
    content = _L8("content").AllocLC();
    iReceiver->SendToClientL( ids, context, content );
    CleanupStack::Pop( content );
    CleanupStack::Pop( context );
    
	iReceiver->ReceiveL( RMessage2() );
	
    context = _L8("context").AllocLC();
    iReceiver->SendToClientL( ids, context );
    CleanupStack::Pop( context );
	
    iReceiver->ClientReadyToReceiveL( RMessage2() );

    
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceCsReceiverBase,
    "UT_CMceCsReceiverBase",
    "UNIT" )


EUNIT_TEST(
    "ClientReadyToReceiveL - test ",
    "CMceCsReceiverBase",
    "ClientReadyToReceiveL",
    "FUNCTIONALITY",
    SetupL, UT_CMceCsReceiverBase_ClientReadyToReceiveLL, Teardown)

EUNIT_TEST(
    "CancelClientReceiveL - test ",
    "CMceCsReceiverBase",
    "CancelClientReceiveL",
    "FUNCTIONALITY",
    SetupL, UT_CMceCsReceiverBase_CancelClientReceiveLL, Teardown)


EUNIT_TEST(
    "ReceiveL - test ",
    "CMceCsReceiverBase",
    "ReceiveL",
    "FUNCTIONALITY",
    SetupL, UT_CMceCsReceiverBase_ReceiveLL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
