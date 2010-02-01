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
#include "ut_ctbcpfloorcontrolimpl.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "fctbcpfloorcontrolimpl.h"
const TInt KTimerDelay = 100;
const TUint KIAPID = 6;
const TInt KLength = 50;
const TInt KBufLength = 64;
const TInt KPort1 = 2000;
const TInt KPort2 = 500;
_LIT8(KTBCPT, "TBCP");
_LIT(KAddr, "127.0.0.1");
_LIT(KAddr2, "172.21.137.184");
_LIT(KAddr3, "fe80::20b:dbff:fe3c:ca82");
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


#define FC_EUNIT_ASSERT_EQUALS( func, val ) \
{\
TInt fcUnitTestRetVal = func;\
if ( fcUnitTestRetVal != KErrNoMemory ){\
	EUNIT_ASSERT_EQUALS( fcUnitTestRetVal, val );\
	}\
else{\
	User::Leave( KErrNoMemory );\
	}\
}
// CONSTRUCTION
UT_CTBCPFloorControlImpl* UT_CTBCPFloorControlImpl::NewL()
    {
    UT_CTBCPFloorControlImpl* self = UT_CTBCPFloorControlImpl::NewLC(); 
    CleanupStack::Pop(self);
    return self;
    }

UT_CTBCPFloorControlImpl* UT_CTBCPFloorControlImpl::NewLC()
    {
    UT_CTBCPFloorControlImpl* self = new( ELeave ) UT_CTBCPFloorControlImpl();
    CleanupStack::PushL( self );
	self->ConstructL(); 
    return self;
    }

// Destructor (virtual by CBase)
UT_CTBCPFloorControlImpl::~UT_CTBCPFloorControlImpl()
    {
    }

// Default constructor
UT_CTBCPFloorControlImpl::UT_CTBCPFloorControlImpl()
    {
    }

// Second phase construct
void UT_CTBCPFloorControlImpl::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CTBCPFloorControlImpl::SetupL(  )
    {
    TFCInterfaceInitParams p( *this);
	iPlugIn= CTBCPFloorControlImpl::NewL(&p);
	iError=0;
  	iEventCount = 0;    
	iTimer = CPeriodic::NewL( EPriorityNormal );
    } 

void UT_CTBCPFloorControlImpl::Teardown(  )
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
	if(iPlugIn)
		{
		delete iPlugIn;
		}
	iPlugIn= NULL;
	delete iData;
	iData= NULL;
    }

void UT_CTBCPFloorControlImpl::UT_CTBCPFloorControlImpl_NewLL(  )
    {
    TFCInterfaceInitParams p( *this);
    CTBCPFloorControlImpl* plugin=CTBCPFloorControlImpl::NewL(&p);
    EUNIT_ASSERT_EQUALS_DESC( plugin->Name(), iPlugIn->Name(), "compare name by 2 NewL instace." )
	if(plugin)
		{
		delete plugin;
		}
    }

void UT_CTBCPFloorControlImpl::UT_CTBCPFloorControlImpl_DestructorL(  )
    {
    // for coverage, delete members before calling destructor
    TFCInterfaceInitParams p( *this);
    CTBCPFloorControlImpl* plugin=CTBCPFloorControlImpl::NewL(&p);
    EUNIT_ASSERT( plugin );
    
    delete plugin->iReceiver;
    plugin->iReceiver = NULL;
    
    delete plugin->iReceiver;
    plugin->iReceiver = NULL;
    
    delete plugin->iSender; 
    plugin->iSender = NULL;
    
    delete plugin->iConnection;
    plugin->iConnection = NULL;
    
    delete plugin;
    }


void UT_CTBCPFloorControlImpl::UT_CTBCPFloorControlImpl_NameL(  )
    {
    EUNIT_ASSERT_EQUALS( iPlugIn->Name(), KTBCPT)
	}

void UT_CTBCPFloorControlImpl::UT_CTBCPFloorControlImpl_SendToNetLL(  )
    {
	iPlugIn->SetIapId(KIAPID);
    TBuf<KBufLength> addressbuffer;
    TInetAddr addr;
    addr.Input(KAddr);
    addr.Output(addressbuffer);
    iPlugIn->SetAddressL(addressbuffer, KPort1);
    iPlugIn->SetLocalPort(KPort2);
    iPlugIn->ConnectL(ETrue);
    
    iPlugIn->iConnection->Cancel();
    iPlugIn->iConnection->RunL();
	EUNIT_PRINT( _L( "DigiaEunit::Socket has open and start Listen!" ) ); 	
	HBufC8* data = HBufC8::NewLC(KLength);
	data->Des().Copy(KHello);
	EUNIT_PRINT( _L( "DigiaEunit::BeforeSendToNetL" ) ); 
	iPlugIn ->SendToNetL(data);
	CleanupStack::Pop( data );
	EUNIT_ASSERT( iError==0);
	}
void UT_CTBCPFloorControlImpl::UT_CTBCPFloorControlImpl_SendToNetL2L(  )
    {
	iPlugIn->SetIapId(KIAPID);
    TBuf<KBufLength> addressbuffer;
    TInetAddr addr;
    addr.Input(KAddr);
    addr.Output(addressbuffer);
    iPlugIn->SetAddressL(addressbuffer, KPort1);
    iPlugIn->SetLocalPort(KPort2);
    iPlugIn->ConnectL(EFalse);
    
    iPlugIn->iConnection->Cancel();
    iPlugIn->iConnection->RunL();
	EUNIT_PRINT( _L( "DigiaEunit::Socket has open and start Listen!" ) ); 	
	HBufC8* data = HBufC8::NewLC(KLength);
	data->Des().Copy(KHello);
	EUNIT_PRINT( _L( "DigiaEunit::BeforeSendToNetL" ) ); 
	iPlugIn ->SendToNetL(data);
	CleanupStack::Pop( data );
	EUNIT_ASSERT( iError==0);
	}

void UT_CTBCPFloorControlImpl::UT_CTBCPFloorControlImpl_ConnectLL(  )
    {
	iPlugIn->SetIapId(KIAPID);
    TBuf<KBufLength> addressbuffer;
    TInetAddr addr;
    addr.Input(KAddr2);
    addr.Output(addressbuffer);
    iPlugIn->SetAddressL(addressbuffer, KPort1);
    iPlugIn->SetLocalPort(KPort2);
    FC_EUNIT_ASSERT_NO_LEAVE(iPlugIn->ConnectL(ETrue) );
    
    iPlugIn->iConnection->Cancel();
    iPlugIn->iConnection->RunL();
	EUNIT_PRINT( _L( "DigiaEunit::Socket has open and start Listen!" ) ); 	
	EUNIT_ASSERT( iError==0);
	}
    
void UT_CTBCPFloorControlImpl::UT_CTBCPFloorControlImpl_SetIapIdL(  )
    {
  
    iPlugIn->SetIapId(KIAPID);
    iPlugIn->iIapId;
    EUNIT_ASSERT( iPlugIn->iIapId== KIAPID);
    }

void UT_CTBCPFloorControlImpl::UT_CTBCPFloorControlImpl_SetAddressLL(  )
    {
    TBuf<KBufLength> addressbuffer;
    TInetAddr addr;
    addr.Input(KAddr2);
    addr.Output(addressbuffer);
    iPlugIn->SetAddressL(addressbuffer, KPort1);
    EUNIT_ASSERT( iPlugIn->iRemoteAddr.Address()==addr.Address());
    }

void UT_CTBCPFloorControlImpl::UT_CTBCPFloorControlImpl_SetLocalPortL(  )
    {
    iPlugIn->SetLocalPort(KPort2);
    EUNIT_ASSERT( iPlugIn->iLocalPort== KPort2);
    }

void UT_CTBCPFloorControlImpl::UT_CTBCPFloorControlImpl_StopL(  )
    {
    iPlugIn->SetIapId(KIAPID);
    TBuf<KBufLength> addressbuffer;
    TInetAddr addr;
    addr.Input(KAddr2);
    addr.Output(addressbuffer);
    iPlugIn->SetAddressL(addressbuffer, KPort1);
    iPlugIn->SetLocalPort(KPort2);
    iPlugIn->ConnectL(ETrue);
        
    iPlugIn->iConnection->Cancel();
    iPlugIn->iConnection->RunL();
    
 	iPlugIn->Stop();
	EUNIT_PRINT( _L( "DigiaEunit::connection and sender, receiver stoped" ) ); 
	iPlugIn->ConnectL(ETrue);
	EUNIT_ASSERT( iError==0);
	}

void UT_CTBCPFloorControlImpl::UT_CTBCPFloorControlImpl_Stop2L(  )
    {
    // For coverage, with empty members
    iPlugIn->SetIapId(KIAPID);
    delete iPlugIn->iReceiver;
    iPlugIn->iReceiver = NULL;
    
    delete iPlugIn->iReceiver;
    iPlugIn->iReceiver = NULL;
    
    delete iPlugIn->iSender; 
    iPlugIn->iSender = NULL;
    
    delete iPlugIn->iConnection;
    iPlugIn->iConnection = NULL;
    
    iPlugIn->Stop();
	}


void UT_CTBCPFloorControlImpl::UT_CTBCPFloorControlImpl_StartListenL(  )
    {
    iPlugIn->SetIapId(KIAPID);
    TBuf<KBufLength> addressbuffer;
    TInetAddr addr;
    addr.Input(KAddr2);
    addr.Output(addressbuffer);
    iPlugIn->SetAddressL(addressbuffer, KPort1);
    iPlugIn->SetLocalPort(KPort2);
    iPlugIn->ConnectL(ETrue);
        
    iPlugIn->iConnection->Cancel();
    iPlugIn->iConnection->RunL();
    
	iPlugIn->StartListen();
	
	EUNIT_PRINT( _L( "DigiaEunit::Socket has open and start Listen!" ) ); 	
	EUNIT_ASSERT( iError==0);
	}

void UT_CTBCPFloorControlImpl::UT_CTBCPFloorControlImpl_ResetSocketLL(  )
    {
    iPlugIn->SetIapId(KIAPID);
    TBuf<KBufLength> addressbuffer;
    TInetAddr addr;
    addr.Input(KAddr);
    addr.Output(addressbuffer);
    iPlugIn->SetAddressL(addressbuffer, KPort1);
    iPlugIn->SetLocalPort(KPort2);
    iPlugIn->ConnectL(ETrue);
    iPlugIn->iConnection->Cancel();
    iPlugIn->iConnection->RunL();
    
    EUNIT_PRINT( _L( "DigiaEunit::Socket has open and start Listen!" ) ); 	
	iPlugIn->ResetSocketL();
	_LIT8(KOwnDesc, "iError Value = intvalue=%d" );
	EUNIT_PRINT(KOwnDesc, iError ); 
	EUNIT_ASSERT( iError==0);
	iPlugIn->Stop();
	}

void UT_CTBCPFloorControlImpl::UT_CTBCPFloorControlImpl_CancelSendL(  )
    {
    iPlugIn->SetIapId(KIAPID);
    TBuf<KBufLength> addressbuffer;
    TInetAddr addr;
    addr.Input(KAddr2);
    addr.Output(addressbuffer);
    iPlugIn->SetAddressL(addressbuffer, KPort1);
    iPlugIn->SetLocalPort(KPort2);
    iPlugIn->ConnectL(ETrue);
    
    iPlugIn->iConnection->Cancel();
    iPlugIn->iConnection->RunL();
    EUNIT_PRINT( _L( "DigiaEunit::Socket has open and start Listen!" ) ); 	
	HBufC8* data = HBufC8::NewLC(KLength);
	data->Des().Copy(KHello);

	EUNIT_PRINT( _L( "DigiaEunit::BeforeSendToNetL" ) ); 
	iPlugIn ->SendToNetL(data);
	CleanupStack::Pop( data );
	
	iPlugIn->ConnectL(ETrue); 
	iPlugIn->iConnection->Cancel();
    iPlugIn->iConnection->RunL();
    
	iPlugIn->CancelSend();
	EUNIT_PRINT( _L( "DigiaEunit::After Cancel Send" ) ); 
	EUNIT_ASSERT( iError==0);
	}

void UT_CTBCPFloorControlImpl::UT_CTBCPFloorControlImpl_ConnectionStartedLL()
    {
    iPlugIn->SetIapId(KIAPID);
    TBuf<KBufLength> addressbuffer;
    TInetAddr addr;
    addr.Input(KAddr2);
    addr.Output(addressbuffer);
    iPlugIn->SetAddressL(addressbuffer, KPort1);
    iPlugIn->SetLocalPort(KPort2);
    iPlugIn->ConnectionStarted(1);
    EUNIT_ASSERT( iError==1);
    }
    
    
void UT_CTBCPFloorControlImpl::UT_CTBCPFloorControlImpl_ConnectionStartedL()
    {
    iPlugIn->SetIapId(KIAPID);
    TBuf<KBufLength> addressbuffer;
    TInetAddr addr;
    addr.Input(KAddr3);
    addr.Output(addressbuffer);
    iPlugIn->SetAddressL(addressbuffer, KPort1);
    iPlugIn->SetLocalPort(KPort2);
    iPlugIn->ConnectL(ETrue);
    iPlugIn->iConnection->Cancel();
    iPlugIn->iConnection->RunL();
        
    EUNIT_ASSERT( iError== KErrNone );
    }

void UT_CTBCPFloorControlImpl::UT_CTBCPFloorControlImpl_ErrorNotifyL(  )
    {
    iPlugIn->SetIapId(KIAPID);
    TBuf<KBufLength> addressbuffer;
    TInetAddr addr;
    addr.Input(KAddr2);
    addr.Output(addressbuffer);
    iPlugIn->SetAddressL(addressbuffer, KPort1);
    iPlugIn->SetLocalPort(KPort2);
    iPlugIn->ConnectL(ETrue);
    iPlugIn->iConnection->Cancel();
    iPlugIn->iConnection->RunL();
    
    iPlugIn->ErrorNotify(1);
    EUNIT_ASSERT( iError==1 );
    iError=0;
    iPlugIn->ErrorNotify(0);
    EUNIT_ASSERT(iError==0);
    }


void UT_CTBCPFloorControlImpl::UT_CTBCPFloorControlImpl_ConnectionStarted2LL(  )
    {
    iPlugIn->SetIapId(KIAPID);
    TBuf<KBufLength> addressbuffer;
    TInetAddr addr;
    addr.Input(KAddr3);
    addr.Output(addressbuffer);
    iPlugIn->SetAddressL(addressbuffer, KPort1);
    iPlugIn->SetLocalPort(KPort2);
    iPlugIn->ConnectL( ETrue );
    iPlugIn->ConnectionStarted(0);
    
 	EUNIT_ASSERT(iError==0);

	TBuf<KBufLength> addressbuffer2;
	TInetAddr addr2;
    addr2.Input(KAddr2);
    addr2.Output(addressbuffer2);
    iPlugIn->SetAddressL(addressbuffer2, KPort1);
    iPlugIn->iConnection->Cancel();
    iPlugIn->iConnection->iStatus = KErrNone;
    iPlugIn->iConnection->RunL();
    
    FC_EUNIT_ASSERT_EQUALS( iError, KErrNone );
    }
    
//test iStartListen
void UT_CTBCPFloorControlImpl::UT_CTBCPFloorControlImpl_ConnectionStartedL3L(  )
    {
    iPlugIn->SetIapId(KIAPID);
  	TBuf<KBufLength> addressbuffer2;
	TInetAddr addr2;
    addr2.Input(KAddr2);
    addr2.Output(addressbuffer2);
    iPlugIn->SetAddressL(addressbuffer2, KPort1);
    iPlugIn->SetLocalPort(KPort2);
    iPlugIn->ConnectL(EFalse);
    iPlugIn->iConnection->Cancel();
    iPlugIn->iConnection->RunL();
    
    EUNIT_ASSERT(iError==0);
    }
    
void UT_CTBCPFloorControlImpl::UT_CTBCPFloorControlImpl_ReceivedDataL(  )
    {
    HBufC8* data = HBufC8::NewL(KLength);
	data->Des().Copy(KHello);
	iPlugIn->ReceivedData(data);
	if(iData)
		{
		EUNIT_ASSERT(data==iData);
    	}
    }
    
void UT_CTBCPFloorControlImpl::ReceivedData(HBufC8* aData,MFCPlugIn* /*aPlugIn*/) 
	{	
	iData=aData;
	}

void UT_CTBCPFloorControlImpl::ErrorNotify(TInt aErrCode,MFCPlugIn* /*aPlugIn*/)
	{
	iError=aErrCode;
	}

TInt UT_CTBCPFloorControlImpl::StopScheduler( TAny* aThis )
	{
	UT_CTBCPFloorControlImpl* self = reinterpret_cast< UT_CTBCPFloorControlImpl* >( aThis );
	// set test to pass and stop scheduler
	self->iCallBackCalled = ETrue;
	return 0;
	}
	

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CTBCPFloorControlImpl,
    "UT_CTBCPFloorControlImpl.",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CTBCPFloorControlImpl",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CTBCPFloorControlImpl_NewLL, Teardown)

EUNIT_TEST(
    "~CTBCPFloorControlImpl- test ",
    "CTBCPFloorControlImpl",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CTBCPFloorControlImpl_DestructorL, Teardown)

EUNIT_TEST(
    "Name - test ",
    "CTBCPFloorControlImpl",
    "Name",
    "FUNCTIONALITY",
    SetupL, UT_CTBCPFloorControlImpl_NameL, Teardown)

EUNIT_TEST(
    "SendToNetL - test ",
    "CTBCPFloorControlImpl",
    "SendToNetL",
    "FUNCTIONALITY",
    SetupL, UT_CTBCPFloorControlImpl_SendToNetLL, Teardown)

EUNIT_TEST(
    "SendToNetL - test ",
    "CTBCPFloorControlImpl",
    "SendToNetL",
    "FUNCTIONALITY",
    SetupL, UT_CTBCPFloorControlImpl_SendToNetL2L, Teardown)


EUNIT_TEST(
    "ConnectL - test ",
    "CTBCPFloorControlImpl",
    "ConnectL",
    "FUNCTIONALITY",
    SetupL, UT_CTBCPFloorControlImpl_ConnectLL, Teardown)

EUNIT_TEST(
    "SetIapId - test ",
    "CTBCPFloorControlImpl",
    "SetIapId",
    "FUNCTIONALITY",
    SetupL, UT_CTBCPFloorControlImpl_SetIapIdL, Teardown)

EUNIT_TEST(
    "SetAddressL - test ",
    "CTBCPFloorControlImpl",
    "SetAddressL",
    "FUNCTIONALITY",
    SetupL, UT_CTBCPFloorControlImpl_SetAddressLL, Teardown)

EUNIT_TEST(
    "SetLocalPort - test ",
    "CTBCPFloorControlImpl",
    "SetLocalPort",
    "FUNCTIONALITY",
    SetupL, UT_CTBCPFloorControlImpl_SetLocalPortL, Teardown)

EUNIT_TEST(
    "Stop - test ",
    "CTBCPFloorControlImpl",
    "Stop",
    "FUNCTIONALITY",
    SetupL, UT_CTBCPFloorControlImpl_StopL, Teardown)

EUNIT_TEST(
    "Stop - test 2",
    "CTBCPFloorControlImpl",
    "Stop",
    "FUNCTIONALITY",
    SetupL, UT_CTBCPFloorControlImpl_Stop2L, Teardown)


EUNIT_TEST(
    "StartListenL - test ",
    "CTBCPFloorControlImpl",
    "StartListenL",
    "FUNCTIONALITY",
    SetupL, UT_CTBCPFloorControlImpl_StartListenL, Teardown)

EUNIT_TEST(
    "ResetSocketL - test ",
    "CTBCPFloorControlImpl",
    "ResetSocketL",
    "FUNCTIONALITY",
    SetupL, UT_CTBCPFloorControlImpl_ResetSocketLL, Teardown)

EUNIT_TEST(
    "CancelSend - test ",
    "CTBCPFloorControlImpl",
    "CancelSend",
    "FUNCTIONALITY",
    SetupL, UT_CTBCPFloorControlImpl_CancelSendL, Teardown)

EUNIT_TEST(
    "ConnectionStarted - test ",
    "CTBCPFloorControlImpl",
    "ConnectionStarted1",
    "FUNCTIONALITY",
    SetupL, UT_CTBCPFloorControlImpl_ConnectionStartedLL, Teardown)

EUNIT_TEST(
    "ErrorNotify - test ",
    "CTBCPFloorControlImpl",
    "ErrorNotify",
    "FUNCTIONALITY",
    SetupL, UT_CTBCPFloorControlImpl_ErrorNotifyL, Teardown)

EUNIT_TEST(
    "ReceivedData - test ",
    "CTBCPFloorControlImpl",
    "ReceivedData",
    "FUNCTIONALITY",
    SetupL, UT_CTBCPFloorControlImpl_ReceivedDataL, Teardown)

EUNIT_TEST(
    "ConnectionStarted - test ",
    "CTBCPFloorControlImpl",
    "ConnectionStartedwIPv6",
    "FUNCTIONALITY",
    SetupL, UT_CTBCPFloorControlImpl_ConnectionStarted2LL, Teardown)
    
EUNIT_TEST(
    "ConnectionStarted - test ",
    "CTBCPFloorControlImpl",
    "ConnectionStarted2",
    "FUNCTIONALITY",
    SetupL, UT_CTBCPFloorControlImpl_ConnectionStartedL, Teardown)
    
EUNIT_TEST(
    "ConnectionStarted - test ",
    "CTBCPFloorControlImpl",
    "TestingListenBranchConnectionStartedL",
    "FUNCTIONALITY",
    SetupL, UT_CTBCPFloorControlImpl_ConnectionStartedL3L, Teardown)
 

EUNIT_END_TEST_TABLE

//  END OF FILE
