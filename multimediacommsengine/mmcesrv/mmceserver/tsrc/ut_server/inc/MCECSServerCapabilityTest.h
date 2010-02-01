/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef CMCECSSERVERCAPABILITYTEST_H
#define CMCECSSERVERCAPABILITYTEST_H

#include <cppunit/TestCase.h>
#include "mceserverstarter.h"
#include "CMceCsServerCaller.h"


// ----------------------------------------------------------------------------------------
// Test Client for PlatSec IPC tests
// ----------------------------------------------------------------------------------------

class CMceCsServerCapabilityTest : CTestCase
    {
public:

	CMceCsServerCapabilityTest () { }
	~CMceCsServerCapabilityTest () { }

   	void setUpL ();
   	void tearDown ();
   	static MTest* suiteL ();
   	
protected:

    void TestCapsPassAllL();
	void TestWriteDeviceDataCapsL();
	void TestNetworkServicesCapsL();
	void TestMultimediaCapsL();
	
private:

	CMceCsServerCaller* iServerCaller;
	};

#endif // CMCESERVERCAPABILITYTEST_H
