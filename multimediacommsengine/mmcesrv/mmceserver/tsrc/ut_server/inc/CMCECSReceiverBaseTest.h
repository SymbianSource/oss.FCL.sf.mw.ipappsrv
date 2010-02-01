/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef __CSIP_CS_RECEIVER_BASE_TEST_H__
#define __CSIP_CS_RECEIVER_BASE_TEST_H__

#include <cppunit/TestCase.h>

class CMceCsReceiverBase;
class CMceCsServerITC;


class CMceCsReceiverBaseTest : public CTestCase
	{
public:

	CMceCsReceiverBaseTest () { }
    ~CMceCsReceiverBaseTest () { }

	// Allocate the resources for one test function
    void setUpL ();

	// Free the resources reserved in setUpL()
	void tearDown ();

	// A function to collect and return a suite of tests
    static MTest* suiteL ();

protected:

	// Own test functions that use assertions and may leave:
    void TestClientReadyToReceiveL ();
    void TestCancelClientReceiveL ();
	void TestReceiveL ();

private:

    CMceCsReceiverBase* iReceiver;
    CMceCsServerITC*    iITC;
	};

#endif
