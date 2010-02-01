/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __MCE_TIMERMANAGER_TEST_H__
#define __MCE_TIMERMANAGER_TEST_H__

#include <cppunit/TestCase.h>
#include "mceexpirationhandler.h"


class CMceTimerManager;

class CMceTimerManagerTest : public CTestCase,
							 public MMCEExpirationHandler
	{
public:

    inline CMceTimerManagerTest()  {}
    inline ~CMceTimerManagerTest() {}
	
    void setUpL();
    void tearDown();

    static MTest* suiteL();
	
public://tests

    void StartTestL();
    void StopTestL();
    void IsRunningTestL();
    void TimerExpiredTestL();
    
public: // From MMCEExpirationHandler	

	/**
    * Callback function to indicate about expiration of
    * session timer. RFC 4028
    * @param aTimerId timer Id
    * @param aTimerParam timer param
    */
    void TimerExpiredL( TMceTimerId aTimerId, TAny* aTimerParam );
    
private://data

	CMceTimerManager* iTimerManager;	
			
};

#endif // __MCE_TIMERMANAGER_TEST_H__
