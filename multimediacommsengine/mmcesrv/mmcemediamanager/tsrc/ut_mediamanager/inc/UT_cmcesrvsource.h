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




#ifndef __UT_CMCESRVSOURCE_H__
#define __UT_CMCESRVSOURCE_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include "MceMediaManagerTestHelper.h"

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS

class CMccControllerStub;
class CMceSrvSource;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMceSrvSource : public CEUnitTestSuiteClass
    {
public:     // Constructors and destructors

    static UT_CMceSrvSource* NewL();
    static UT_CMceSrvSource* NewLC();
    ~UT_CMceSrvSource();

private:    // Constructors and destructors

    UT_CMceSrvSource();
    void ConstructL();

private:    // EUNIT methods

     void SetupL();
     void Teardown();

private:    // Test methods
    
    void UT_CMceSrvSource_IdL();
    void UT_CMceSrvSource_DataL();
    void UT_CMceSrvSource_EventReceivedL();
    void UT_CMceSrvSource_IsPreparedL();
    void UT_CMceSrvSource_IsStartedL();
    void UT_CMceSrvSource_TypeL();
    void UT_CMceSrvSource_MccPrepareCalledLL();
    void UT_CMceSrvSource_SetStateL();
    void UT_CMceSrvSource_StateL();
    void UT_CMceSrvSource_MergeL();
    void UT_CMceSrvSource_EnableLL();
    void UT_CMceSrvSource_DisableLL();
    

private:    // Data

    CMceSrvSource* iSource;
    
	CMceMediaManager* iManager;
	CMceComSession* iSession;
	CMceServerStub* iServer;
	CMccControllerStub* iMcc;

	EUNIT_DECLARE_TEST_TABLE; 
		
    };


#endif      //  __UT_CMCESRVSOURCE_H__

// End of file
