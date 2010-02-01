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
#include "UT_CFCInterface.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "FCInterface.h"

const TUid KTestPluginUid = {0x10009DB3};

// CONSTRUCTION
UT_CFCInterface* UT_CFCInterface::NewL()
    {
    UT_CFCInterface* self = UT_CFCInterface::NewLC(); 
    CleanupStack::Pop( self );

    return self;
    }

UT_CFCInterface* UT_CFCInterface::NewLC()
    {
    UT_CFCInterface* self = new( ELeave ) UT_CFCInterface();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CFCInterface::~UT_CFCInterface()
    {
    }

// Default constructor
UT_CFCInterface::UT_CFCInterface()
    {
    }

// Second phase construct
void UT_CFCInterface::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CFCInterface::SetupL(  )
    {
	
    } 

void UT_CFCInterface::Teardown(  )
    {

    }

void UT_CFCInterface::UT_CFCInterface_NewLL(  )
    {
       
    TFCInterfaceInitParams initParam(*this);
	CFCInterface* fcInterface =CFCInterface::NewL(KTestPluginUid,initParam );
	
	CleanupStack::PushL( fcInterface );
	EUNIT_ASSERT(fcInterface->Name().CompareF(_L8("TBCPTest")) == KErrNone);
	CleanupStack::PopAndDestroy( fcInterface );
   	REComSession::FinalClose();
    }

void UT_CFCInterface::ReceivedData(HBufC8* /*aData*/, MFCPlugIn* /*aPlugIn*/)
{
	
}
void UT_CFCInterface::ErrorNotify(TInt /*aErrCode*/, MFCPlugIn* /*aPlugIn*/)
{
	
}


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CFCInterface,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CFCInterface",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CFCInterface_NewLL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
