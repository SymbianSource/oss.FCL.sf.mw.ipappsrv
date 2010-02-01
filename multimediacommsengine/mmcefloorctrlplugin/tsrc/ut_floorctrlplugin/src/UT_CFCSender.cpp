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
#include "ut_cfcsender.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "fcsender.h"


const TInt KTimerDelay = 500;
const TUint KIAPID = 6;
const TInt KLength = 50;
const TInt KPort1 = 2000;
const TInt KPort2 = 3000;
const TInt KSendBufferLen = 10;

_LIT(KAddr, "127.0.0.1");
_LIT8(KHello, "hello...testing");


#define FC_EUNIT_ASSERT_SPECIFIC_LEAVE( func, val ) \
{\
TRAPD( fcUnitTestRetVal, func );\
if ( val != KErrNoMemory && fcUnitTestRetVal == KErrNoMemory ){\
	User::Leave( KErrNoMemory );\
	}\
else{\
	EUNIT_ASSERT_EQUALS( fcUnitTestRetVal, val );\
	}\
}

#define FC_EUNIT_ASSERT_NO_LEAVE( func ) FC_EUNIT_ASSERT_SPECIFIC_LEAVE( func, KErrNone )


#define FC_EUNIT_ASSERT_SPECIFIC_LEAVE( func, val ) \
{\
TRAPD( fcUnitTestRetVal, func );\
if ( val != KErrNoMemory && fcUnitTestRetVal == KErrNoMemory ){\
	User::Leave( KErrNoMemory );\
	}\
else{\
	EUNIT_ASSERT_EQUALS( fcUnitTestRetVal, val );\
	}\
}
// CONSTRUCTION
UT_CFCSender* UT_CFCSender::NewL()
    {
    UT_CFCSender* self = UT_CFCSender::NewLC(); 
    CleanupStack::Pop(self);
    return self;
    }

UT_CFCSender* UT_CFCSender::NewLC()
    {
    UT_CFCSender* self = new( ELeave ) UT_CFCSender();
    CleanupStack::PushL( self );
	self->ConstructL(); 
    return self;
    }

// Destructor (virtual by CBase)
UT_CFCSender::~UT_CFCSender()
    {
    }

// Default constructor
UT_CFCSender::UT_CFCSender()
    {
    }

// Second phase construct
void UT_CFCSender::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CFCSender::SetupL(  )
    {
    iSimplifiedConnect = EFalse;
    iConnection = CFCConnectionContainer::NewL(*this);
	User::LeaveIfError( iSocketServ.Connect( KESockDefaultMessageSlots) );
	iEventCount = 0;    
	iTimer = CPeriodic::NewL( EPriorityNormal );
    } 

void UT_CFCSender::Teardown(  )
    {
	delete iTimer;
	iTimer = NULL;

    if (iSender)
		{
		iSender->Cancel();
		}
	if(iConnection)
		{
		iConnection->CloseConnection();
		}
	iSocket.Close();
    delete iSender;
    iSender = NULL;
    delete iConnection;
    iConnection = NULL;
    iSocketServ.Close();
    delete iData;
    iData= NULL;
    }

void UT_CFCSender::UT_CFCSender_NewLL(  )
    {
    iConnection->OpenConnection(KIAPID, iSocketServ  );
    iSender = CFCSender::NewL(iSocket, *this);
    iConnection->Cancel(); 
    iConnection->RunL(); 
    
    EUNIT_ASSERT( iErr==0);
    }

void UT_CFCSender::UT_CFCSender_SendLL(  )
    {
  	iConnection->OpenConnection(KIAPID, iSocketServ  );
	iSender = CFCSender::NewL(iSocket, *this);
	iConnection->Cancel();
	iConnection->iStatus = KErrNone;
    iConnection->RunL();
        
    EUNIT_ASSERT( iErr==0);

	iSender->Cancel();
    iSender->iStatus = KErrNone;
    iSender->RunL();
    TInetAddr addr;
    addr.Input(KAddr);
    addr.SetPort(KPort2);
    HBufC8* data = HBufC8::NewLC(KLength);
	data->Des().Copy(KHello);
	FC_EUNIT_ASSERT_NO_LEAVE(iSender ->SendL(data, addr));
	CleanupStack::Pop( data );
    iSender->Cancel();
    }
void UT_CFCSender::UT_CFCSender_SendL2L(  )
    {
    // Test when is active.
    iConnection->OpenConnection(KIAPID, iSocketServ  );
    iSender = CFCSender::NewL(iSocket, *this);

	iConnection->Cancel(); 
	iConnection->iStatus = KErrNone;
    iConnection->RunL(); 

	iSender->Cancel();
    iSender->iStatus = KErrNone;
    iSender->RunL();
	EUNIT_ASSERT( iErr==0 );
	TInetAddr addr;
    addr.Input(KAddr);
    addr.SetPort(KPort2);
    HBufC8* data = HBufC8::NewLC(KLength);
	    data->Des().Copy(KHello);
    FC_EUNIT_ASSERT_NO_LEAVE(iSender ->SendL(data, addr));    
    CleanupStack::Pop( data );

    iSender->Cancel();
    iSender->iStatus = KErrNone;
    iSender->RunL();
    }

void UT_CFCSender::UT_CFCSender_SendL3L(  )
    {
    // Test with active and full buffer
    iConnection->OpenConnection(KIAPID, iSocketServ  );
    iSender = CFCSender::NewL(iSocket, *this);

	iConnection->Cancel(); 
 	iConnection->iStatus = KErrNone;
    iConnection->RunL(); 
    
    iSender->Cancel();
    iSender->iStatus = KErrNone;
    iSender->RunL();
	EUNIT_ASSERT( iErr==0 );
    TInetAddr addr;
    addr.Input(KAddr);
    addr.SetPort(KPort2);
    HBufC8* data = HBufC8::NewLC(KLength);
	    data->Des().Copy(KHello);
    // fill the send buffer
    while( iSender->iSendBuffer.Count() < KSendBufferLen )
        {
        HBufC8* data2 = HBufC8::NewLC(KLength);
	    data2->Des().Copy(KHello);
        iSender->iSendBuffer.AppendL( data2 );      
        CleanupStack::Pop( data2 );
        }
    iSender ->SendL(data, addr); 
       
    CleanupStack::Pop( data );
    HBufC8* data3 = HBufC8::NewLC(KLength);
	    data3->Des().Copy(KHello);
	    
	EUNIT_ASSERT_LEAVE(iSender ->SendL( data3, addr ));    	    
	CleanupStack::PopAndDestroy( data3 );
		    
    iSender->iSendBuffer.ResetAndDestroy();      
   	iSender->Cancel();
    iSender->iStatus = KErrNone;
    iSender->RunL();
    }

void UT_CFCSender::UT_CFCSender_SendL4L(  )
    {
    // Test sending when connection is not started yet, send is queued
    iSimplifiedConnect = ETrue;
    iConnection->OpenConnection(KIAPID, iSocketServ  );
    iSender = CFCSender::NewL(iSocket, *this); 
    
    TInetAddr addr;
    addr.Input(KAddr);
    addr.SetPort(KPort2);
    HBufC8* data = HBufC8::NewLC(KLength);
    data->Des().Copy(KHello);
    EUNIT_ASSERT_EQUALS( iSender->iSendBuffer.Count(), 0 );
    FC_EUNIT_ASSERT_NO_LEAVE(iSender ->SendL(data, addr));
    CleanupStack::Pop( data );
    EUNIT_ASSERT_EQUALS( iSender->iSendBuffer.Count(), 1 );
    EUNIT_ASSERT( !iSender->IsActive() );
    
    // Connection gets started and sending of queued buffer is activated
    iConnection->Cancel(); 
    iConnection->iStatus = KErrNone;
    iConnection->RunL();
    EUNIT_ASSERT_EQUALS( iSender->iSendBuffer.Count(), 0 );
    EUNIT_ASSERT( iSender->IsActive() );
    }
void UT_CFCSender::UT_CFCSender_RunErrorL(  )
    {
    iSender  = CFCSender::NewL(iSocket, *this);
	EUNIT_ASSERT( iSender->RunError(KErrNoMemory) == KErrNoMemory);
	EUNIT_ASSERT( iSender->RunError(KErrBadName) == KErrNone);
	}

void UT_CFCSender::UT_CFCSender_DoCancelL(  )
    {
    iConnection->OpenConnection(KIAPID, iSocketServ  );
    iSender = CFCSender::NewL(iSocket, *this);
     
    iConnection->Cancel(); 
    iConnection->RunL(); 
    
	iSender->DoCancel();
	EUNIT_ASSERT( iErr==0 );
	}

void UT_CFCSender::UT_CFCSender_TestRunLL(  )
	{
	// KErrNoMemory
	iSender = CFCSender::NewL(iSocket, *this);
	iConnection->Cancel(); 
	iConnection->iStatus = KErrNone;
    iConnection->RunL(); 

	iSender->Cancel();
    iSender->iStatus = KErrNoMemory;
    
    EUNIT_ASSERT_SPECIFIC_LEAVE( iSender->RunL(), KErrNoMemory );
 	iSender->Cancel();
	}

void UT_CFCSender::UT_CFCSender_TestRunL2L(  )
	{
	// data in buffer
	iConnection->OpenConnection( KIAPID, iSocketServ  );
	iSender = CFCSender::NewL(iSocket, *this);
	iConnection->Cancel(); 
	iConnection->iStatus = KErrNone;
    iConnection->RunL(); 
		
    iSender->Cancel();
    iSender->iStatus = KErrNone;
    iSender->RunL();
        
	HBufC8* data = HBufC8::NewLC(KLength);
	data->Des().Copy(KHello);
    iSender->iSendBuffer.AppendL( data );
	FC_EUNIT_ASSERT_NO_LEAVE( iSender->RunL() );
    CleanupStack::Pop( data );
 	iSender->Cancel();
   	
	}


void UT_CFCSender::ReceivedData(HBufC8* aData)
	{
	iData=aData;
	}

void UT_CFCSender::ErrorNotify(TInt aErrCode)
	{
	iErr=aErrCode;
	}
	
void UT_CFCSender::ConnectionStarted( TInt aError )
	{
	TRAPD(leaveErr ,ConnectionStartedL(aError) );
	if(leaveErr!=KErrNone)
		{
		ErrorNotify( leaveErr);	
		}
	iErr=aError;
	}	

void UT_CFCSender::ConnectionStartedL( TInt aError )
	{
	if(aError==KErrNone)
		{	
		iSender->ConnectedL();
		User::LeaveIfError(iSocket.Open(iSocketServ, KAfInet, KSockDatagram,
								KProtocolInetUdp, iConnection->Connection()) );

		TInetAddr addr;
		addr.SetAddress(KInetAddrAny);
		addr.SetPort(KPort1);
		TInt err = iSocket.Bind(addr);
		if (err == KErrInUse)
			{
			iErr = KErrInUse;
			}
		else
			{
			iErr = err;
			}
		}
	EUNIT_ASSERT(iErr==0);
	
	if ( !iSimplifiedConnect )
	    {
        TInetAddr addr2;
        addr2.Input(KAddr);
        addr2.SetPort(KPort2);
        HBufC8* data = HBufC8::NewLC(KLength);
        data->Des().Copy(KHello);
        iSender ->SendL(data, addr2);
        CleanupStack::Pop(data);
    
        iConnection->Cancel();
        iConnection->RunL();
        iConnection->iStatus = KErrNone;
        EUNIT_ASSERT(iErr==0);
	    }
	
	}

TInt UT_CFCSender::StopScheduler( TAny* aThis )
	{
	UT_CFCSender* self = reinterpret_cast< UT_CFCSender* >( aThis );
	// set test to pass and stop scheduler
	self->iCallBackCalled = ETrue;
	return 0;
	}
	

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CFCSender,
    "UT_CFCSender",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CFCSender",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CFCSender_NewLL, Teardown)

EUNIT_TEST(
    "SendL - test ",
    "CFCSender",
    "SendL",
    "FUNCTIONALITY",
    SetupL, UT_CFCSender_SendLL, Teardown)

EUNIT_TEST(
    "SendL - test 2",
    "CFCSender",
    "SendL",
    "FUNCTIONALITY",
    SetupL, UT_CFCSender_SendL2L, Teardown)

EUNIT_TEST(
    "SendL - test 3",
    "CFCSender",
    "SendL",
    "FUNCTIONALITY",
    SetupL, UT_CFCSender_SendL3L, Teardown)

EUNIT_TEST(
    "SendL - test 4",
    "CFCSender",
    "SendL",
    "FUNCTIONALITY",
    SetupL, UT_CFCSender_SendL4L, Teardown)
    
EUNIT_TEST(
    "RunError - test ",
    "CFCSender",
    "RunError",
    "FUNCTIONALITY",
    SetupL, UT_CFCSender_RunErrorL, Teardown)

EUNIT_TEST(
    "DoCancel - test ",
    "CFCSender",
    "DoCancel",
    "FUNCTIONALITY",
    SetupL, UT_CFCSender_DoCancelL, Teardown)
    
EUNIT_TEST(
    "RunL - test ",
    "CFCSender",
    "RunL",
    "FUNCTIONALITY",
    SetupL, UT_CFCSender_TestRunLL, Teardown)

EUNIT_TEST(
    "RunL - test 2",
    "CFCSender",
    "RunL",
    "FUNCTIONALITY",
    SetupL, UT_CFCSender_TestRunL2L, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
