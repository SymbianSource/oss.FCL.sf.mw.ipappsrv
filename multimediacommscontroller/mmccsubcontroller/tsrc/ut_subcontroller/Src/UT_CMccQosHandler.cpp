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
#include "UT_CMccQosHandler.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <commdbconnpref.h>
#include <in_sock.h>
#include <ecom/ecom.h>

//  INTERNAL INCLUDES
#include "mccqoshandler.h"
#include "mccqossockethandler.h"
#include "mccqoseventmonitor.h"

#include "mccunittestmacros.h"

// CONSTRUCTION
UT_CMccQosHandler* UT_CMccQosHandler::NewL()
    {
    UT_CMccQosHandler* self = UT_CMccQosHandler::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccQosHandler* UT_CMccQosHandler::NewLC()
    {
    UT_CMccQosHandler* self = new( ELeave ) UT_CMccQosHandler();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccQosHandler::~UT_CMccQosHandler()
    {
    }

// Default constructor
UT_CMccQosHandler::UT_CMccQosHandler()
    {
    }

// Second phase construct
void UT_CMccQosHandler::ConstructL()
    {   
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CMccQosHandler::SetupL(  )
    {         
    __UHEAP_MARK;
    // RSubConnection is stubbed (rsubconnection_STUB.cpp) so no need to
    // open socket, start connection etc.
    iHandler = CMccQosHandler::NewL( *this );      
    }

void UT_CMccQosHandler::Teardown(  )
    {
    delete iHandler;
    REComSession::FinalClose(); 
    __UHEAP_MARKEND;
    }

void UT_CMccQosHandler::UT_CMccQosHandler_EnableQosLL(  )
    {
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iHandler->EnableQosL(), KErrNotReady );
    
    iHandler->SetSubConnectionParams( iConn, iSocketServ, iSocket );    
    MCC_EUNIT_ASSERT_NO_LEAVE( iHandler->EnableQosL() );
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iHandler->EnableQosL(), KErrInUse );
    iHandler->iQosSocketHandler->Cancel();
    iHandler->DisableQoS();
    }

void UT_CMccQosHandler::UT_CMccQosHandler_DisableQoSL(  )
    {
    iHandler->DisableQoS();
    }

void UT_CMccQosHandler::UT_CMccQosHandler_SetSubConnectionParamsL(  )
    {
    iHandler->SetSubConnectionParams( iConn, iSocketServ, iSocket );
    }

void UT_CMccQosHandler::UT_CMccQosHandler_QoSEnabledL(  )
    {
    EUNIT_ASSERT_EQUALS( iHandler->QoSEnabled(), EFalse );
    
    iHandler->SetSubConnectionParams( iConn, iSocketServ, iSocket );
    MCC_EUNIT_ASSERT_NO_LEAVE( iHandler->EnableQosL() );
    EUNIT_ASSERT_EQUALS( iHandler->QoSEnabled(), ETrue );
    iHandler->iQosSocketHandler->Cancel();    
    iHandler->DisableQoS();
        
    EUNIT_ASSERT_EQUALS( iHandler->QoSEnabled(), EFalse );    
    }
    
void UT_CMccQosHandler::UT_CMccQosHandler_HandleSocketHandlerDeletionL( )
    {
    iHandler->SetSubConnectionParams( iConn, iSocketServ, iSocket );    
    MCC_EUNIT_ASSERT_NO_LEAVE( iHandler->EnableQosL() );
    iHandler->iQosSocketHandler->Cancel();    
    iHandler->DisableQoS();          
    }
    

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccQosHandler,
    "CMccQosHandler",
    "UNIT" )

EUNIT_TEST(
    "EnableQosL - test ",
    "CMccQosHandler",
    "EnableQosL",
    "FUNCTIONALITY",
    SetupL, UT_CMccQosHandler_EnableQosLL, Teardown)

EUNIT_TEST(
    "EnableQosL - test 2",
    "CMccQosHandler",
    "EnableQosL",
    "FUNCTIONALITY",
    SetupL, UT_CMccQosHandler_EnableQosLL, Teardown)

EUNIT_TEST(
    "DisableQoS - test ",
    "CMccQosHandler",
    "DisableQoS",
    "FUNCTIONALITY",
    SetupL, UT_CMccQosHandler_DisableQoSL, Teardown)

EUNIT_TEST(
    "SetSubConnectionParams - test ",
    "CMccQosHandler",
    "SetSubConnectionParams",
    "FUNCTIONALITY",
    SetupL, UT_CMccQosHandler_SetSubConnectionParamsL, Teardown)

EUNIT_TEST(
    "QoSEnabled - test ",
    "CMccQosHandler",
    "QoSEnabled",
    "FUNCTIONALITY",
    SetupL, UT_CMccQosHandler_QoSEnabledL, Teardown)
    
EUNIT_TEST(
    "HandleSocketHandlerDeletion - test ",
    "CMccQosHandler",
    "HandleSocketHandlerDeletion",
    "FUNCTIONALITY",
    SetupL, UT_CMccQosHandler_HandleSocketHandlerDeletionL, Teardown)    


EUNIT_END_TEST_TABLE

//  END OF FILE
