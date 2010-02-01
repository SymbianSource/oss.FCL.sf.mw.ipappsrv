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


#ifndef UT_CNATTRAVERSALCONFIG_H
#define UT_CNATTRAVERSALCONFIG_H

#include <digia/eunit/ceunittestsuiteclass.h>
#include "MNATBindingRefresherObserver.h"

#include <e32def.h>
#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif

class CRepository;
class CNATTraversalConfig;


NONSHARABLE_CLASS( UT_CNATTraversalConfig ) :
    public CEUnitTestSuiteClass
	{
public:

  	static UT_CNATTraversalConfig* NewL();
    static UT_CNATTraversalConfig* NewLC();

	~UT_CNATTraversalConfig();

private:    // Constructors and destructors

    UT_CNATTraversalConfig();
    void ConstructL();

private: // New Methods

    void SetupL();
	void Teardown();

protected: // Test functions

	void TestCRLFRefreshEnabledToProxyL();
	void TestCRLFRefreshEnabledToProxyValueL();
    void TestReadCRLFRefreshEnabledL();

private: // Data

	EUNIT_DECLARE_TEST_TABLE;

    CNATTraversalConfig* iConfig;
    CRepository* iCR;
    TUint32 iDomainKey;
	};

#endif // UT_CNATTRAVERSALCONFIG_H

// End of File
