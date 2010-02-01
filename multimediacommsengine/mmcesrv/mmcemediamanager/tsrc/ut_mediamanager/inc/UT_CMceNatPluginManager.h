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




#ifndef __UT_CMceNatPluginManager_H__
#define __UT_CMceNatPluginManager_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include "MceMediaManagerTestHelper.h"

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMceNatPluginManager;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMceNatPluginManager : public CEUnitTestSuiteClass
    {
public:     // Constructors and destructors

    static UT_CMceNatPluginManager* NewL();
    static UT_CMceNatPluginManager* NewLC();
    ~UT_CMceNatPluginManager();

private:    // Constructors and destructors

    UT_CMceNatPluginManager();
    void ConstructL();

private:    // EUNIT methods

     void SetupL();
     void Teardown();

private:    // Test methods
    

    void UT_CMceNatPluginManager_GetPluginLL();
    void UT_CMceNatPluginManager_NatEnabledL();
    void UT_CMceNatPluginManager_SetSessionParamL();
    void UT_CMceNatPluginManager_GetSessionParamL();

private:    // Data

	// Pointer to CMceNatPluginManager, owned
    CMceNatPluginManager* iNatPluginManager;
	EUNIT_DECLARE_TEST_TABLE; 
		
    };


#endif      //  __UT_CMceNatPluginManager_H__

// End of file
