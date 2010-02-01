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




#ifndef __UT_CMCESRVSINK_H__
#define __UT_CMCESRVSINK_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include "MceMediaManagerTestHelper.h"

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS

class CMccControllerStub;
class CMceSrvSink;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMceSrvSink : public CEUnitTestSuiteClass
    {
public:     // Constructors and destructors

    static UT_CMceSrvSink* NewL();
    static UT_CMceSrvSink* NewLC();
    ~UT_CMceSrvSink();

private:    // Constructors and destructors

    UT_CMceSrvSink();
    void ConstructL();

private:    // EUNIT methods

     void SetupL();
     void Teardown();

private:    // Test methods
    

    void UT_CMceSrvSink_IdL();
    void UT_CMceSrvSink_DataL();
    void UT_CMceSrvSink_EventReceivedL();
    void UT_CMceSrvSink_TypeL();
    void UT_CMceSrvSink_MccPrepareCalledLL();
    void UT_CMceSrvSink_SetStateL();
    void UT_CMceSrvSink_StateL();
    void UT_CMceSrvSink_MergeL();
    void UT_CMceSrvSink_EnableLL();
    void UT_CMceSrvSink_DisableLL();
    

private:    // Data

    CMceSrvSink* iSink;
    
	CMceMediaManager* iManager;
	CMceComSession* iSession;
	CMceServerStub* iServer;
	CMccControllerStub* iMcc;

	EUNIT_DECLARE_TEST_TABLE; 
		
    };


#endif      //  __UT_CMCESRVSINK_H__

// End of file
