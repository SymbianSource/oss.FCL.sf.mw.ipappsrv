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

#ifndef __CSIP_CS_SUB_SESSION_TEST_H__
#define __CSIP_CS_SUB_SESSION_TEST_H__

#include <cppunit/TestCase.h>
#include "mceClientServer.h"

class CMceServerCore;
class CMceCsSession;
class CMceCsSubSession;


class CMceCsSubSessionTest : public CTestCase
	{
public:

	CMceCsSubSessionTest () { }
    ~CMceCsSubSessionTest () { }

	// Allocate the resources for one test function
    void setUpL ();

	// Free the resources reserved in setUpL()
	void tearDown ();

	// A function to collect and return a suite of tests
    static MTest* suiteL ();

protected:

	// Own test functions that use assertions and may leave:
    void TestReceiveL ();
    void TestClientReadyToReceiveL ();
    void TestCancelClientReceiveL ();
    void TestServiceL ();

private:

    TMceIds iIds;
    CMceServerCore* iServerCore;
    CMceCsSession* iSession;
    CMceCsSubSession* iSubSession;
	};

#endif
