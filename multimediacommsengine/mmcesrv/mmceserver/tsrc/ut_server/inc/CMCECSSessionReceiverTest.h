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

#ifndef __CSIP_CS_SESSION_RECEIVER_TEST_H__
#define __CSIP_CS_SESSION_RECEIVER_TEST_H__

#include <cppunit/TestCase.h>

class CMceCsSessionReceiver;
class CMceCsServerITC;


class CMceCsSessionReceiverTest : public CTestCase
	{
public:

	CMceCsSessionReceiverTest () { }
    ~CMceCsSessionReceiverTest () { }

	// Allocate the resources for one test function
    void setUpL ();

	// Free the resources reserved in setUpL()
	void tearDown ();

	// A function to collect and return a suite of tests
    static MTest* suiteL ();

protected: // Test functions

	void TestSIPSecUserL();
	void TestReceiveL ();
	void TestRequestCredentialsL ();

private: // Data

    CMceCsSessionReceiver* iReceiver;
    CMceCsServerITC*       iITC;
	};

#endif
