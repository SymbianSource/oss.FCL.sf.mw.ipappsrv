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
#include "UT_MNATFWSocketMediaConnWrapper.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "mnatfwsocketmediaconnwrapper.h"
#include "natfwconnectivityapidefs.h"
#include "natfwsocketmediaconnwrapper.h"

// CONSTANTS
const TUint KStreamId = 10;
const TUint KPort = 250;
const TUint KProtocol = KProtocolInetUdp;

// CONSTRUCTION
UT_MNATFWSocketMediaConnWrapper* UT_MNATFWSocketMediaConnWrapper::NewL()
    {
    UT_MNATFWSocketMediaConnWrapper* self = UT_MNATFWSocketMediaConnWrapper::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_MNATFWSocketMediaConnWrapper* UT_MNATFWSocketMediaConnWrapper::NewLC()
    {
    UT_MNATFWSocketMediaConnWrapper* self = new( ELeave ) UT_MNATFWSocketMediaConnWrapper();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_MNATFWSocketMediaConnWrapper::~UT_MNATFWSocketMediaConnWrapper()
    {
    }

// Default constructor
UT_MNATFWSocketMediaConnWrapper::UT_MNATFWSocketMediaConnWrapper()
    {
    }

// Second phase construct
void UT_MNATFWSocketMediaConnWrapper::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

// Observer Methods from MNATFWMediaWrapperObserver
void UT_MNATFWSocketMediaConnWrapper::ConnectingCompleted( const TInt /*aError*/ )
    {

    }
void UT_MNATFWSocketMediaConnWrapper::SendingCompleted( const TInt /*aError*/ )
    {

    }
        
TInt UT_MNATFWSocketMediaConnWrapper::ReceivedFrom( const TUint /*aStreamId*/, /*const TUint*/ /*aLength*/TDes8& /*aReceiveBuffer*/ )
    {
    return 0;
    }

void UT_MNATFWSocketMediaConnWrapper::GetNewFreePort( TUint& /*aPort*/ )
    {
    EUNIT_PRINT( _L8( "UT_MNATFWMediaWrapper::GetNewFreePort" ) );
    }
    
//  METHODS

void UT_MNATFWSocketMediaConnWrapper::SetupL(  )
    {
    iMediaConnWrapper = CNATFWSocketMediaConnWrapper::NewL( KPort,
        KProtocol, KStreamId );
        
    iMediaWrapperIF = iMediaConnWrapper;
    iSocketMediaConnWrapperIF = iMediaConnWrapper;    

    iSocketSrv.Connect();
    TInetAddr localAddr;
    iMediaWrapperIF->RegisterMediaWrapperObserverL( this, iSocketSrv, localAddr );
    }

void UT_MNATFWSocketMediaConnWrapper::Teardown(  )
    {    
    delete iMediaConnWrapper;
    iSocketSrv.Close();
    iMediaConnWrapper = NULL;
    iMediaWrapperIF = NULL;
    iSocketMediaConnWrapperIF = NULL;
    }

void UT_MNATFWSocketMediaConnWrapper::UT_MNATFWSocketMediaConnWrapperIF_OutgoingAddrL(  )
    {
    TSockAddr addr;
    iSocketMediaConnWrapperIF->OutgoingAddr( addr );
    }

void UT_MNATFWSocketMediaConnWrapper::UT_MNATFWSocketMediaConnWrapperIF_SetIncomingAddrLL(  )
    {
    _LIT8(KAddress, "10.10.10.10");
    TSockAddr addr(KProtocolInetUdp);
    addr.Copy(KAddress);

    iSocketMediaConnWrapperIF->SetIncomingAddrL(addr);
    }

void UT_MNATFWSocketMediaConnWrapper::UT_MNATFWSocketMediaConnWrapperIF_StreamIdL(  )
    {
    EUNIT_ASSERT( iSocketMediaConnWrapperIF->StreamId() == KStreamId );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_MNATFWSocketMediaConnWrapper,
    "MNATFWSocketMediaConnWrapper",
    "UNIT" )

EUNIT_TEST(
    "OutgoingAddr - test ",
    "MNATFWSocketMediaConnWrapper",
    "OutgoingAddr",
    "FUNCTIONALITY",
    SetupL, UT_MNATFWSocketMediaConnWrapperIF_OutgoingAddrL, Teardown)

EUNIT_TEST(
    "SetIncomingAddrL - test ",
    "MNATFWSocketMediaConnWrapper",
    "SetIncomingAddrL",
    "FUNCTIONALITY",
    SetupL, UT_MNATFWSocketMediaConnWrapperIF_SetIncomingAddrLL, Teardown)

EUNIT_TEST(
    "StreamId - test ",
    "MNATFWSocketMediaConnWrapper",
    "StreamId",
    "FUNCTIONALITY",
    SetupL, UT_MNATFWSocketMediaConnWrapperIF_StreamIdL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
