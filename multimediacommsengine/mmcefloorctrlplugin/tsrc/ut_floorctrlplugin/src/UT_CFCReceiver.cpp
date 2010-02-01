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




//  CLASS HEADER
#include "ut_cfcreceiver.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "fcreceiver.h"
#include "fcsender.h"

const TInt KTimerDelay = 100;
const TUint KIAPID = 6;
const TInt KLength = 50;
const TInt KPort = 500;

_LIT8(KHello, "hello...receivingtesting");
// CONSTRUCTION
UT_CFCReceiver* UT_CFCReceiver::NewL()
    {
    UT_CFCReceiver* self = UT_CFCReceiver::NewLC(); 
    CleanupStack::Pop(self);
    return self;
    }

UT_CFCReceiver* UT_CFCReceiver::NewLC()
    {
    UT_CFCReceiver* self = new( ELeave ) UT_CFCReceiver();
    CleanupStack::PushL( self );
	self->ConstructL(); 
    return self;
    }

// Destructor (virtual by CBase)
UT_CFCReceiver::~UT_CFCReceiver()
    {
    }

// Default constructor
UT_CFCReceiver::UT_CFCReceiver()
    {
    }

// Second phase construct
void UT_CFCReceiver::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CFCReceiver::SetupL(  )
    {
    iConnection = CFCConnectionContainer::NewL(*this);
	
	User::LeaveIfError( iSocketServ.Connect( KESockDefaultMessageSlots) );
	 iEventCount = 0;    
	iTimer = CPeriodic::NewL( EPriorityNormal );
    } 

void UT_CFCReceiver::Teardown(  )
    {
	if( iEventCount > 0)
		{
		if( !iWait.IsStarted() )
			{
			iWait.Start();
			}
		}
	delete iTimer;
	iTimer = NULL;

	if (iReceiver)
		{		
		iReceiver->Cancel();
		}
	if(iSender)
		iSender->Cancel();
	if(iConnection)
		{
		iConnection->CloseConnection();
		}
	iSocket.Close();
	delete iSender;
	iSender = NULL;	
    delete iReceiver;
    iReceiver =NULL;
    delete iConnection;
    iConnection = NULL;
    iSocketServ.Close();
    delete iData;
    iData= NULL;
    }

void UT_CFCReceiver::UT_CFCReceiver_NewLL(  )
    {
   	iConnection->OpenConnection(KIAPID, iSocketServ  );
   	iReceiver = CFCReceiver::NewL(iSocket, *this);
	iConnection->Cancel();
    iConnection->RunL();
    
	EUNIT_ASSERT( iErr==0);
    }

void UT_CFCReceiver::UT_CFCReceiver_NewLCL(  )
    {
	iConnection->OpenConnection(KIAPID, iSocketServ  );
   	iReceiver = CFCReceiver::NewLC(iSocket, *this);
   	CleanupStack::Pop( iReceiver );
   	iConnection->Cancel();
    iConnection->RunL();
    
	EUNIT_ASSERT( iErr==0);
    }

void UT_CFCReceiver::UT_CFCReceiver_ListenL(  )
    {
    iConnection->OpenConnection(KIAPID, iSocketServ  );
    
   	iReceiver = CFCReceiver::NewL(iSocket, *this);
    iConnection->Cancel();
    iConnection->RunL();
    
    EUNIT_ASSERT( iErr==0);
    }

void UT_CFCReceiver::UT_CFCReceiver_ReceivedDataLL(  )
    {
    iReceiver =CFCReceiver::NewL(iSocket, *this);
    HBufC8* data = HBufC8::NewL(KLength);
	data->Des().Copy(KHello);
	iReceiver->ReceivedData(data);
	EUNIT_ASSERT( data==iData);
 	}

void UT_CFCReceiver::UT_CFCReceiver_RunErrorL(  )
    {
    iReceiver  = CFCReceiver::NewL(iSocket, *this);
    iConnection->OpenConnection(KIAPID, iSocketServ  );
    iConnection->Cancel();
    iConnection->RunL();
    
    EUNIT_ASSERT( iErr==0);
	EUNIT_ASSERT( iReceiver->RunError(KErrNoMemory) == KErrNoMemory);
	EUNIT_ASSERT( iReceiver->RunError(KErrBadName) == KErrNone);
	}

void UT_CFCReceiver::UT_CFCReceiver_TestRunLL(  )
	{
	iReceiver  = CFCReceiver::NewL(iSocket, *this);
	TRequestStatus* status = &iReceiver->iStatus;
    User::RequestComplete(status, KErrNoMemory);
    EUNIT_ASSERT_SPECIFIC_LEAVE( iReceiver->RunL(), KErrNoMemory);
   	iReceiver->Cancel();
   	User::WaitForAnyRequest();
	}
	
void UT_CFCReceiver::UT_CFCReceiver_TestRunL2L(  )
	{
	// with data
	iConnection->OpenConnection( KIAPID, iSocketServ  );
	iReceiver  = CFCReceiver::NewL(iSocket, *this);
	iConnection->Cancel();
    iConnection->RunL();
    
	HBufC8* msg = KHello().AllocL();
	delete iReceiver->iReceivedMsg;
	iReceiver->iReceivedMsg = msg;
 	TRequestStatus* status = &iReceiver->iStatus;
   	User::RequestComplete(status, KErrNone);
	EUNIT_ASSERT_NO_LEAVE( iReceiver->RunL() );
    EUNIT_ASSERT( iErr==KErrNone);
    EUNIT_ASSERT( iData && iData->Compare( *msg ) == 0 );
    iReceiver->Cancel();
    User::WaitForAnyRequest();
	}

void UT_CFCReceiver::UT_CFCReceiver_TestRunL3L(  )
	{
	iReceiver  = CFCReceiver::NewL(iSocket, *this);
	TRequestStatus* status = &iReceiver->iStatus;
    User::RequestComplete(status, KErrCancel);
    iReceiver->RunL();
    EUNIT_ASSERT( iErr==KErrNone);
   	iReceiver->Cancel();
   	User::WaitForAnyRequest();
	}

void UT_CFCReceiver::UT_CFCReceiver_TestRunL4L(  )
	{
	iReceiver  = CFCReceiver::NewL(iSocket, *this);
	TRequestStatus* status = &iReceiver->iStatus;
    User::RequestComplete(status, KErrTotalLossOfPrecision );
    iReceiver->RunL();
    EUNIT_ASSERT( iErr==KErrTotalLossOfPrecision);
   	iReceiver->Cancel();
   	User::WaitForAnyRequest();
	}

void UT_CFCReceiver::ReceivedData(HBufC8* aData)
	{
	iData= aData;
	}

void UT_CFCReceiver::ErrorNotify(TInt aErrCode)
	{
	iErr=aErrCode;
	}
//
void UT_CFCReceiver::ConnectionStarted( TInt aError )
	{
	TRAPD(leaveErr ,ConnectionStartedL(aError) );
	if(leaveErr!=KErrNone)
		{
		ErrorNotify( leaveErr);	
		}
	iErr=aError;
	}	
void UT_CFCReceiver::ConnectionStartedL( TInt aError )
	{
	iConnectErr= aError;
	User::LeaveIfError(iSocket.Open(iSocketServ, KAfInet, KSockDatagram,
								KProtocolInetUdp, iConnection->Connection()) );

	TInetAddr addr;
	addr.SetPort(KPort);
	addr.SetAddress(KInetAddrAny);
	TInt err = iSocket.Bind(addr);
	if (err == KErrInUse)
		{
		ErrorNotify(KErrInUse);
		}
	else{
		ErrorNotify(err);
		}
	EUNIT_ASSERT(iErr==0);
	iReceiver->Listen();
	}

TInt UT_CFCReceiver::StopScheduler( TAny* aThis )
	{
	UT_CFCReceiver* self = reinterpret_cast< UT_CFCReceiver* >( aThis );
	// set test to pass and stop scheduler
	self->iCallBackCalled = ETrue;
	return 0;
	}
	
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CFCReceiver,
    "UT_CFCReceiver",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CFCReceiver",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CFCReceiver_NewLL, Teardown)

EUNIT_TEST(
    "NewLC - test ",
    "CFCReceiver",
    "NewLC",
    "FUNCTIONALITY",
    SetupL, UT_CFCReceiver_NewLCL, Teardown)

EUNIT_TEST(
    "Listen - test ",
    "CFCReceiver",
    "Listen",
    "FUNCTIONALITY",
    SetupL, UT_CFCReceiver_ListenL, Teardown)

EUNIT_TEST(
    "ReceivedDataL - test ",
    "CFCReceiver",
    "ReceivedDataL",
    "FUNCTIONALITY",
    SetupL, UT_CFCReceiver_ReceivedDataLL, Teardown)

EUNIT_TEST(
    "RunErrorL - test ",
    "CFCReceiver",
    "RunError",
    "FUNCTIONALITY",
    SetupL, UT_CFCReceiver_RunErrorL, Teardown)

EUNIT_TEST(
    "RunL - test ",
    "CFCReceiver",
    "RunL",
    "FUNCTIONALITY",
    SetupL, UT_CFCReceiver_TestRunLL, Teardown)

EUNIT_TEST(
    "RunL - test 2",
    "CFCReceiver",
    "RunL",
    "FUNCTIONALITY",
    SetupL, UT_CFCReceiver_TestRunL2L, Teardown)
    
EUNIT_TEST(
    "RunL - test 3",
    "CFCReceiver",
    "RunL",
    "FUNCTIONALITY",
    SetupL, UT_CFCReceiver_TestRunL3L, Teardown)

EUNIT_TEST(
    "RunL - test 4",
    "CFCReceiver",
    "RunL",
    "FUNCTIONALITY",
    SetupL, UT_CFCReceiver_TestRunL4L, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
