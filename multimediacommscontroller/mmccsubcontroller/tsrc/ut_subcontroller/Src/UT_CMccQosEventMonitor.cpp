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
#include "UT_CMccQosEventMonitor.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <CommDbConnPref.h>


//  INTERNAL INCLUDES
#include "mccqoseventmonitor.h"

#include "mccunittestmacros.h"

// CONSTRUCTION
UT_CMccQosEventMonitor* UT_CMccQosEventMonitor::NewL()
    {
    UT_CMccQosEventMonitor* self = UT_CMccQosEventMonitor::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccQosEventMonitor* UT_CMccQosEventMonitor::NewLC()
    {
    UT_CMccQosEventMonitor* self = new( ELeave ) UT_CMccQosEventMonitor();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccQosEventMonitor::~UT_CMccQosEventMonitor()
    {
    }

// Default constructor
UT_CMccQosEventMonitor::UT_CMccQosEventMonitor()
    {
    }

// Second phase construct
void UT_CMccQosEventMonitor::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMccQosEventMonitor::SetupL(  )
    {
    // RSubConnection is stubbed (rsubconnection_STUB.cpp) so no need to
    // open socket, start connection etc.
    User::LeaveIfError( iSubConnection.Open( iSocketServ,
        RSubConnection::ECreateNew, iConn ) );
        
    iMonitor = CMccQosEventMonitor::NewL( *this, iSubConnection );
    }

void UT_CMccQosEventMonitor::Teardown(  )
    {
    delete iMonitor;
    iSubConnection.Close();   
    }

void UT_CMccQosEventMonitor::UT_CMccQosEventMonitor_StartListenLL(  )
    {      
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iMonitor->StartListenL(), KErrNotReady );
    iMonitor->Cancel();
    MCC_EUNIT_ASSERT_NO_LEAVE( iMonitor->StartListenL() );  
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccQosEventMonitor,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "StartListenL - test ",
    "CMccQosEventMonitor",
    "StartListenL",
    "FUNCTIONALITY",
    SetupL, UT_CMccQosEventMonitor_StartListenLL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
