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

#ifndef __CSIP_CS_SESSION_TEST_H__
#define __CSIP_CS_SESSION_TEST_H__

#include <cppunit/TestCase.h>

class CMceServerCore;
class CMceCsSession;


class CMceCsSessionTest : public CTestCase
	{
public:

	CMceCsSessionTest () { }
    ~CMceCsSessionTest () { }

	// Allocate the resources for one test function
    void setUpL ();

	// Free the resources reserved in setUpL()
	void tearDown ();

	// A function to collect and return a suite of tests
    static MTest* suiteL ();

protected:

	// Own test functions that use assertions and may leave:
    void TestOpenSubSessionL ();
    void TestSetAppUidL ();
	void TestSupportedCodecsL ();
    void TestClientReadyToReceiveL ();
    void TestCancelClientReceiveL ();
    void TestCallSubSessionL ();
    void TestServiceL ();

private:

    CMceServerCore* iServerCore;
    CMceCsSession* iSession;
	};

#endif
