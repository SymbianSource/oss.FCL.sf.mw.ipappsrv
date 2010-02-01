/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef CSIPCSRECEIVEQUEUETEST_H
#define CSIPCSRECEIVEQUEUETEST_H

#include <cppunit/TestCase.h>

class CMceCsReceiveQueue;

class CMceCsReceiveQueueTest : public CTestCase
    {
public:

	CMceCsReceiveQueueTest () { }
    ~CMceCsReceiveQueueTest () { }

	// Allocate the resources for one test function
    void setUpL ();

	// Free the resources reserved in setUpL()
	void tearDown ();

	// A function to collect and return a suite of tests
    static MTest* suiteL ();

protected: // Test functions

    void TestIsEmptyL ();
    void TestAddAndRemoveL ();
    void TestRemoveSIPRequestsWithL ();

private: // Data

    CMceCsReceiveQueue* iReceiveQueue;
    };

#endif // CSIPCSRECEIVEQUEUETEST_H
