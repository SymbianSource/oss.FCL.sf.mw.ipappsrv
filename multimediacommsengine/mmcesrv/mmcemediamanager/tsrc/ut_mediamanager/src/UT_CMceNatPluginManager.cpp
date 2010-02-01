/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
#include "UT_CMceNatPluginManager.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include "mcenatpluginmanager.h"
#include "MCETestHelper.h"

//  INTERNAL INCLUDES

// CONSTRUCTION
UT_CMceNatPluginManager* UT_CMceNatPluginManager::NewL()
    {
    UT_CMceNatPluginManager* self = UT_CMceNatPluginManager::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMceNatPluginManager* UT_CMceNatPluginManager::NewLC()
    {
    UT_CMceNatPluginManager* self = new( ELeave ) UT_CMceNatPluginManager();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceNatPluginManager::~UT_CMceNatPluginManager()
    {
    }

// Default constructor
UT_CMceNatPluginManager::UT_CMceNatPluginManager()
    {
    }

// Second phase construct
void UT_CMceNatPluginManager::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void UT_CMceNatPluginManager::SetupL()
    {
	// Pointer to CMceNatPluginManager, owned
    iNatPluginManager = CMceNatPluginManager::NewL();
    } 

void UT_CMceNatPluginManager::Teardown()
    {
    delete iNatPluginManager;
    iNatPluginManager = NULL;
    }


void UT_CMceNatPluginManager::UT_CMceNatPluginManager_GetPluginLL()
    {
	 	//TInt error = KErrNone;
    //TRAP( error, iNatPluginManager->GetPluginL() );
    //EUNIT_ASSERT(error == KErrNone);

    TRAPD( error, iNatPluginManager->GetPluginL()  );
	if ( error == KErrNoMemory )
    {
    	User::Leave( KErrNoMemory );
    }
	EUNIT_ASSERT( error == KErrNone );
    }
    

void UT_CMceNatPluginManager::UT_CMceNatPluginManager_NatEnabledL()
    {
    TBool retVal = EFalse;
    retVal = iNatPluginManager->NatEnabledL();
    EUNIT_ASSERT(retVal == ETrue);
    }

void UT_CMceNatPluginManager::UT_CMceNatPluginManager_SetSessionParamL()
    {
    TUint sessionId = 1;
    TUint paramValue = 2;
    TInt error = KErrNone;

    error = iNatPluginManager->SetSessionParam( sessionId, 
    			CNSPPlugin::ENSPResourseReservationStatusKey, paramValue );
    EUNIT_ASSERT(error == KErrNotFound);
    
    // Run NatEnabled tests -> This will set iNatPluginManager.iPlugin 
    // to correct state.
    UT_CMceNatPluginManager_NatEnabledL();
    
    error = iNatPluginManager->SetSessionParam( sessionId, 
    			CNSPPlugin::ENSPResourseReservationStatusKey, paramValue );
    EUNIT_ASSERT(error == KErrNone);
    }

     
void UT_CMceNatPluginManager::UT_CMceNatPluginManager_GetSessionParamL()
    {
    TUint sessionId = 1;
    TUint paramValue = 2;
    TInt error = KErrNone;
    
    error = iNatPluginManager->GetSessionParam(sessionId, 
    			CNSPPlugin::ENSPResourseReservationStatusKey);
    EUNIT_ASSERT(error == KErrNotFound);
    
    // Run NatEnabled tests -> This will set iNatPluginManager.iPlugin 
    // to correct state.
    UT_CMceNatPluginManager_NatEnabledL();
    
    error = iNatPluginManager->GetSessionParam(sessionId, 
    			CNSPPlugin::ENSPResourseReservationStatusKey);
    EUNIT_ASSERT(error == KErrNone);
    }



//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceNatPluginManager,
    "CMceNatPluginManager tests",
    "UNIT" )


EUNIT_TEST(
    "GetPlugin test",
    "CMceNatPluginManager",
    "<basic>",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatPluginManager_GetPluginLL, Teardown)
    
EUNIT_TEST(
    "NatEnabled test",
    "CMceNatPluginManager",
    "<basic>",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatPluginManager_NatEnabledL, Teardown)

EUNIT_TEST(
    "SetSessionParam test",
    "CMceNatPluginManager",
    "<basic>",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatPluginManager_SetSessionParamL, Teardown)

EUNIT_TEST(
    "GetSessionParam test",
    "CMceNatPluginManager",
    "<basic>",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatPluginManager_GetSessionParamL, Teardown)



EUNIT_END_TEST_TABLE

//  END OF FILE
