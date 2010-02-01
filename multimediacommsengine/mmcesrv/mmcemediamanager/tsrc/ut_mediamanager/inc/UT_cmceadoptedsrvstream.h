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




#ifndef __UT_CMCEADOPTEDSRVSTREAM_H__
#define __UT_CMCEADOPTEDSRVSTREAM_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include "MceMediaManagerTestHelper.h"

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMccControllerStub;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMceAdoptedSrvStream : public CEUnitTestSuiteClass
    {
public:     // Constructors and destructors

    static UT_CMceAdoptedSrvStream* NewL();
    static UT_CMceAdoptedSrvStream* NewLC();
    ~UT_CMceAdoptedSrvStream();

private:    // Constructors and destructors

    UT_CMceAdoptedSrvStream();
    void ConstructL();

private:    // EUNIT methods

     void SetupL();
     void Teardown();

private:    // Test methods
    
    void UT_CMceAdoptedSrvStream_IsAdoptedL();
    void UT_CMceAdoptedSrvStream_StateL();
    void UT_CMceAdoptedSrvStream_IsEqualL();
    void UT_CMceAdoptedSrvStream_IsMccPreparedL();
    void UT_CMceAdoptedSrvStream_InvalidateL();
    void UT_CMceAdoptedSrvStream_SessionIdL();
    void UT_CMceAdoptedSrvStream_LinkIdL();
    void UT_CMceAdoptedSrvStream_SetLinkIdL();
    void UT_CMceAdoptedSrvStream_IapIdL();
    void UT_CMceAdoptedSrvStream_LocalMediaPortL();
    void UT_CMceAdoptedSrvStream_CleanupL();


private:    // Data

    CMceAdoptedSrvStream* iStream;
    
	CMceMediaManager* iManager;
	CMceComSession* iSession;
	CMceServerStub* iServer;
	CMccControllerStub* iMcc;

	EUNIT_DECLARE_TEST_TABLE; 
		
    };


#endif      //  __UT_CMCEADOPTEDSRVSTREAM_H__

// End of file
