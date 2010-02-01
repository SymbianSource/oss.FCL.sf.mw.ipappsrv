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

#ifndef CSIPCSSUBSESSIONRECEIVERTEST_H
#define CSIPCSSUBSESSIONRECEIVERTEST_H

#include <cppunit/TestCase.h>

class CMceCsSubSessionReceiver;
class CMceCsServerITC;


class CMceCsSubSessionReceiverTest : public CTestCase
	{
public:

	CMceCsSubSessionReceiverTest () { }
    ~CMceCsSubSessionReceiverTest () { }

	// Allocate the resources for one test function
    void setUpL ();

	// Free the resources reserved in setUpL()
	void tearDown ();

	// A function to collect and return a suite of tests
    static MTest* suiteL ();

public: // From MSIPSecUser

    void RequestCredentialsL(const TDesC8& aRealm);

protected: // Test functions
	
	void TestTransportParamsL ();
	void TestReceiveL ();
	void TestRefreshReceivedL ();
	void TestRefreshErrorL ();
    void TestNextRefreshCSeqL ();
	void TestRegistrationReceivedL ();
	void TestRegistrationErrorL ();
	void TestDialogsReceivedL ();
	void TestDialogsErrorL ();

private: // Data

    CMceCsSubSessionReceiver* iReceiver;
    CMceCsServerITC* iITC;
	};

#endif // CSIPCSSUBSESSIONRECEIVERTEST_H
