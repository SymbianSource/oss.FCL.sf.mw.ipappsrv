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
#include "UT_CMccQosSocketHandler.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <commdbconnpref.h>
#include <in_sock.h>

//  INTERNAL INCLUDES
#include "mccqossockethandler.h"

// CONSTRUCTION
UT_CMccQosSocketHandler* UT_CMccQosSocketHandler::NewL()
    {
    UT_CMccQosSocketHandler* self = UT_CMccQosSocketHandler::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccQosSocketHandler* UT_CMccQosSocketHandler::NewLC()
    {
    UT_CMccQosSocketHandler* self = new( ELeave ) UT_CMccQosSocketHandler();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccQosSocketHandler::~UT_CMccQosSocketHandler()
    {
    }

// Default constructor
UT_CMccQosSocketHandler::UT_CMccQosSocketHandler()
    {
    }

// Second phase construct
void UT_CMccQosSocketHandler::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CMccQosSocketHandler::SetupL(  )
    {   
    // RSubConnection is stubbed (rsubconnection_STUB.cpp) so no need to
    // open socket, start connection etc.
    User::LeaveIfError( iSubConnection.Open( iSocketServ,
        RSubConnection::ECreateNew, iConn ) );                                      
                                              
    iSocketHandler = CMccQosSocketHandler::NewL( *this, iSubConnection, iSocket );     
    }

void UT_CMccQosSocketHandler::Teardown(  )
    {
    delete iSocketHandler;
    iSubConnection.Close();  
    }

void UT_CMccQosSocketHandler::UT_CMccQosSocketHandler_AsyncDeleteNeededL(  )
    {
    iSocketHandler->Cancel();
    iSocketHandler->AsyncDeleteNeeded( EFalse );
    }

void UT_CMccQosSocketHandler::UT_CMccQosSocketHandler_RemoveSocketFromSubConnL(  )
    {
    iSocketHandler->Cancel();
    iSocketHandler->RemoveSocketFromSubConn();
    }    
    

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccQosSocketHandler,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "AsyncDeleteNeeded - test ",
    "CMccQosSocketHandler",
    "AsyncDeleteNeeded",
    "FUNCTIONALITY",
    SetupL, UT_CMccQosSocketHandler_AsyncDeleteNeededL, Teardown)
    
EUNIT_TEST(
    "RemoveSocketFromSubConn - test ",
    "CMccQosSocketHandler",
    "RemoveSocketFromSubConn",
    "FUNCTIONALITY",
    SetupL, UT_CMccQosSocketHandler_RemoveSocketFromSubConnL, Teardown)    


EUNIT_END_TEST_TABLE

//  END OF FILE
