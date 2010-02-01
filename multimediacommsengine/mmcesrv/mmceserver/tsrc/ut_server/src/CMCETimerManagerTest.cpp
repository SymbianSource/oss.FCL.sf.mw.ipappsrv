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

#include <e32def.h>
#include <e32base.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestSuite.h>
#include "CMceTimerManagerTest.h"

#include "mcetimermanager.h"


// ----------------------------------------------------------------------------
// CMceTimerManagerTest::setUpL
// ----------------------------------------------------------------------------
//	
void CMceTimerManagerTest::setUpL()
    {
    iTimerManager = CMceTimerManager::NewL();				
    }

// ----------------------------------------------------------------------------
// CMceTimerManagerTest::tearDown
// ----------------------------------------------------------------------------
//
void CMceTimerManagerTest::tearDown()
    {

    delete iTimerManager;
          
    }

// ----------------------------------------------------------------------------
// CMceTimerManagerTest::suiteL
// ----------------------------------------------------------------------------
//
MTest* CMceTimerManagerTest::suiteL()
    {
    CTestSuite *suite = CTestSuite::NewL(_L8("CMceTimerManagerTest"));

	suite->addTestL(CTestCaller<CMceTimerManagerTest>::NewL(_L8("StartTestL"), &StartTestL ) );
	suite->addTestL(CTestCaller<CMceTimerManagerTest>::NewL(_L8("StopTestL"), &StopTestL ) );
	suite->addTestL(CTestCaller<CMceTimerManagerTest>::NewL(_L8("IsRunningTestL"), &IsRunningTestL ) );
	suite->addTestL(CTestCaller<CMceTimerManagerTest>::NewL(_L8("TimerExpiredTestL"), &TimerExpiredTestL ) );
		                                                          
    return suite;
    }


	
// ----------------------------------------------------------------------------
// CMceTimerManagerTest::StartTestL
// ----------------------------------------------------------------------------
//
void CMceTimerManagerTest::StartTestL()
    {
	TUint32 interval = 500;
	TMceTimerId timerId = CMceTimerManager::KNoSuchTimer;
	TRAPD (err , iTimerManager->StartL( NULL, interval ));
	assertL(err!=KErrNone);
	timerId = iTimerManager->StartL( this, interval );
	assertL(timerId!=CMceTimerManager::KNoSuchTimer);
	iTimerManager->Stop( timerId );
 
    }
    
// ----------------------------------------------------------------------------
// CMceTimerManagerTest::StopTestL
// ----------------------------------------------------------------------------
//
void CMceTimerManagerTest::StopTestL()
    {
	TUint32 interval = 500;
	TMceTimerId timerId = CMceTimerManager::KNoSuchTimer;
	
	assertL(iTimerManager->Stop( timerId )!=KErrNone);
	timerId = iTimerManager->StartL( this, interval );
	assertL(iTimerManager->Stop( timerId )==KErrNone);	  
    }
    
// ----------------------------------------------------------------------------
// CMceTimerManagerTest::IsRunningTestL
// ----------------------------------------------------------------------------
//
void CMceTimerManagerTest::IsRunningTestL()
    {
	TUint32 interval = 500;
	TMceTimerId timerId = CMceTimerManager::KNoSuchTimer;
	
	assertL(iTimerManager->IsRunning(timerId)==EFalse);	
	timerId = iTimerManager->StartL( this, interval );
	assertL(iTimerManager->IsRunning(timerId));
	iTimerManager->Stop( timerId );  
    }
    
// ----------------------------------------------------------------------------
// CMceTimerManagerTest::TimerExpiredTestL
// ----------------------------------------------------------------------------
//
void CMceTimerManagerTest::TimerExpiredTestL()
    {
    TUint32 interval = 500;
    TMceTimerId timerId = CMceTimerManager::KNoSuchTimer;

	assertL(CMceTimerManager::TimerExpiredCallBack(NULL)!=KErrNone);
	
	CMceTimerManager::TEntry *newEntry = new ( ELeave ) CMceTimerManager::TEntry();
    newEntry->iId = 1;
    newEntry->iObserver = this;
    newEntry->iObserverParam = NULL;
    newEntry->iCallBack = TCallBack( CMceTimerManager::TimerExpiredCallBack, newEntry );
    newEntry->iEntry = TDeltaTimerEntry( newEntry->iCallBack );
    newEntry->iTimerManager = iTimerManager;
    iTimerManager->iEntries.InsertL(newEntry, 0);
	assertL(CMceTimerManager::TimerExpiredCallBack(newEntry)==KErrNone);
    }

// -----------------------------------------------------------------------------
// CMceTimerManagerTest::TimerExpiredL
// -----------------------------------------------------------------------------
//    
void CMceTimerManagerTest::TimerExpiredL( TMceTimerId aTimerId, TAny* /*aTimerParam*/ )
    {
	assertL(aTimerId!=CMceTimerManager::KNoSuchTimer);
    }
