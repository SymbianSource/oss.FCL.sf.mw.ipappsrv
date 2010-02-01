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
#include "ut_cfcconnectioncontainer.h"

//  EXTERNAL INCLUDES

#include <e32math.h>
#include <e32svr.h>
#include <digia/eunit/eunitmacros.h>
#include <digia/eunit/teunitassertioninfo.h>

//  INTERNAL INCLUDES
#include "fcconnectioncontainer.h"
const TUint KIAPID = 6;
// CONSTRUCTION
UT_CFCConnectionContainer* UT_CFCConnectionContainer::NewL()
    {
    UT_CFCConnectionContainer* self = UT_CFCConnectionContainer::NewLC(); 
    CleanupStack::Pop(self);
    return self;
    }

UT_CFCConnectionContainer* UT_CFCConnectionContainer::NewLC()
    {
    UT_CFCConnectionContainer* self = new( ELeave ) UT_CFCConnectionContainer();
    CleanupStack::PushL( self );
	self->ConstructL(); 
    return self;
    }

// Destructor (virtual by CBase)
UT_CFCConnectionContainer::~UT_CFCConnectionContainer()
    {
    }

// Default constructor
UT_CFCConnectionContainer::UT_CFCConnectionContainer()
    {
    }

// Second phase construct
void UT_CFCConnectionContainer::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CFCConnectionContainer::SetupL(  )
    {
	iConnection = CFCConnectionContainer::NewL(*this);
	User::LeaveIfError( iSocketServ.Connect( KESockDefaultMessageSlots) );
    } 

void UT_CFCConnectionContainer::Teardown(  )
    {
	if(iConnection)
		{
		iConnection->CloseConnection();
		}
	delete iConnection;
	iConnection=NULL;	
	iSocketServ.Close();
    }

void UT_CFCConnectionContainer::UT_CFCConnectionContainer_NewLL(  )
    {
    iConnection->OpenConnection(KIAPID, iSocketServ  );
    CFCConnectionContainer* tmpConn=CFCConnectionContainer::NewL(*this);
    tmpConn->OpenConnection(KIAPID, iSocketServ  );
    iConnection->Cancel(); 
    iConnection->RunL(); 
    
    EUNIT_ASSERT( iErr==0);//Connection has opened
 	if(tmpConn)
		{
		tmpConn->CloseConnection();
		}
	delete tmpConn;
	tmpConn=NULL;
    }

void UT_CFCConnectionContainer::UT_CFCConnectionContainer_ConnectionL(  )
    {
    TBuf<KMaxName> aName1;
    TBuf<KMaxName> aName2;
    TInt err1;
    TInt err2;
    iConnection->OpenConnection(KIAPID, iSocketServ  );
    RConnection& tmp= iConnection->Connection();
    err1=tmp.Name(aName1);
    tmp.Close();
    iConnection->CloseConnection();
    iConnection->OpenConnection(KIAPID, iSocketServ  );
    RConnection& tmp2= iConnection->Connection();
    err2=tmp2.Name(aName2);
    
    iConnection->Cancel();
    iConnection->RunL();
        
    if (err1==KErrNone & err2==KErrNone)
    	{
    	EUNIT_ASSERT( aName1==aName2 );
    	}
	tmp2.Close();
   	}

void UT_CFCConnectionContainer::UT_CFCConnectionContainer_OpenConnectionL(  )
    {
    iConnection->OpenConnection(KIAPID, iSocketServ );
    CFCConnectionContainer* tmpConn=CFCConnectionContainer::NewL(*this);
    tmpConn->OpenConnection(KIAPID, iSocketServ );
    iConnection->Cancel(); 
    iConnection->RunL(); 
    
    EUNIT_ASSERT( iErr==0);//Connection has opened
    if(tmpConn)
		{
		tmpConn->CloseConnection();
		}
	delete tmpConn;
	tmpConn=NULL;
    }
 


void UT_CFCConnectionContainer::UT_CFCConnectionContainer_CloseConnectionL(  )
    {
    iConnection->OpenConnection(KIAPID, iSocketServ );
    iConnection->CloseConnection();
    CFCConnectionContainer* tmpConn=CFCConnectionContainer::NewL(*this);
    tmpConn->OpenConnection(KIAPID, iSocketServ );
    iConnection->Cancel(); 
    iConnection->RunL(); 
    
    EUNIT_ASSERT( iErr==0);//Connection has opened
    if(tmpConn)
		{
		tmpConn->CloseConnection();
		}
	delete tmpConn;
	tmpConn=NULL;
    }
    
void UT_CFCConnectionContainer::UT_CFCConnectionContainer_TestRunLL(  )
	{
	TRequestStatus* status = &iConnection->iStatus;
    User::RequestComplete(status, KErrCancel);
    iConnection->RunL();
    EUNIT_ASSERT( iErr==KErrNone);
   	iConnection->Cancel();
   	User::WaitForAnyRequest();
	}


void UT_CFCConnectionContainer::UT_CFCConnectionContainer_TestDoCancelL(  )
	{
	RConnection& temp = iConnection->Connection();
	User::LeaveIfError(temp.Open(iSocketServ));
	TRequestStatus* status = &iConnection->iStatus;
	User::RequestComplete(status, KRequestPending);
	iConnection->DoCancel();
	User::WaitForAnyRequest();
	iConnection->Cancel();
	User::WaitForAnyRequest();
	}

void UT_CFCConnectionContainer::ConnectionStarted( TInt aError )
	{
	iErr= aError;
	}

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CFCConnectionContainer,
    "UT_CFCConnectionContainer",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CFCConnectionContainer",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CFCConnectionContainer_NewLL, Teardown)

EUNIT_TEST(
    "Connection - test ",
    "CFCConnectionContainer",
    "Connection",
    "FUNCTIONALITY",
    SetupL, UT_CFCConnectionContainer_ConnectionL, Teardown)

EUNIT_TEST(
    "OpenConnection - test ",
    "CFCConnectionContainer",
    "OpenConnection",
    "FUNCTIONALITY",
    SetupL, UT_CFCConnectionContainer_OpenConnectionL, Teardown)
    
EUNIT_TEST(
    "CloseConnection - test ",
    "CFCConnectionContainer",
    "CloseConnection",
    "FUNCTIONALITY",
    SetupL, UT_CFCConnectionContainer_CloseConnectionL, Teardown)

EUNIT_TEST(
    "RunL - test ",
    "CFCConnectionContainer",
    "RunL",
    "FUNCTIONALITY",
    SetupL, UT_CFCConnectionContainer_TestRunLL, Teardown)

EUNIT_TEST(
    "DoCancel - test ",
    "CFCConnectionContainer",
    "DoCancel",
    "FUNCTIONALITY",
    SetupL, UT_CFCConnectionContainer_TestDoCancelL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
