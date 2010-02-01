/*
* Copyright (c) 2002 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
#include "ut_cstunclientresolvingtcp.h"


//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <digia/eunit/EUnitDecorators.h>
#include <commdbconnpref.h>

//  INTERNAL INCLUDES
#include "natfwstunclient.h"
#include "cstunclientimplementation.h"
#include "cstunclientresolvingtcp.h"
#include "cstunclientresolvingtls.h"
#include "cstunclientresolvingudp.h"
#include "natfwunittestmacros.h"

_LIT8( KServerAddress, "10.32.194.251" );

// CONSTRUCTION
ut_cstunclientresolvingtcp* ut_cstunclientresolvingtcp::NewL()
    {
    ut_cstunclientresolvingtcp* self = ut_cstunclientresolvingtcp::NewLC();
    CleanupStack::Pop();

    return self;
    }

ut_cstunclientresolvingtcp* ut_cstunclientresolvingtcp::NewLC()
    {
    ut_cstunclientresolvingtcp* self = new( ELeave ) ut_cstunclientresolvingtcp();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
ut_cstunclientresolvingtcp::~ut_cstunclientresolvingtcp()
    {
    }

// Default constructor
ut_cstunclientresolvingtcp::ut_cstunclientresolvingtcp()
    {
    }

// Second phase construct
void ut_cstunclientresolvingtcp::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS
// from mstunclientbserver
void ut_cstunclientresolvingtcp::STUNClientInitCompleted( const CSTUNClient& /*aClient*/,
                              TInt aCompletionCode )
    {
    RDebug::Print( _L("**NATFW STUNClientInitCompleted, code: %d"), aCompletionCode );
    CActiveScheduler::Stop();
    }

void ut_cstunclientresolvingtcp::STUNBindingEventOccurredL( TSTUNBindingEvent aEvent, 
                                const CBinding& /*aBinding*/ )
    {
    RDebug::Print( _L("**NATFW STUNBindingEventOccurredL, code: %d"), aEvent );
    CActiveScheduler::Stop();
    }

void ut_cstunclientresolvingtcp::STUNBindingErrorOccurred( const CBinding& /*aBinding*/, 
                               TInt aError )
    {
    RDebug::Print( _L("**NATFW STUNBindingErrorOccurred, code: %d"), aError );
    CActiveScheduler::Stop();
    }

void ut_cstunclientresolvingtcp::Notify( TUint /*aSessionId*/, TUint /*aStreamId*/, 
    TNotifyType /*aType*/, TInt /*aError*/ )
    {
    RDebug::Print( _L("**NATFW Notify") );
    }

void ut_cstunclientresolvingtcp::SetupL(  )
    {
    TUint iapID( 6 );
    TInt retransmitInterval( 10 );
    TBufC8<13> serveraddress( KServerAddress );
    TUint serverport( 2000 );
    TBufC8<4> service( KStun );
    TBool obtainsharedsecret( EFalse );
    TBool failifnoSRVrecordfound( EFalse );
    iDeltatimer = CDeltaTimer::NewL( 1 );
    User::LeaveIfError( iSocketServ.Connect() );
    iStreamId = 1;
    iSubstreamId = 1;
    TBool icmpUsed( EFalse );
    
    iMux = CNcmConnectionMultiplexer::NewL( *this );
    
    TCommDbConnPref pref;
    pref.SetDialogPreference( ECommDbDialogPrefDoNotPrompt );
    pref.SetIapId( iapID );
    
    User::LeaveIfError( iConnection.Open( iSocketServ ) );
    iConnection.Start( pref, iStatus );
    User::WaitForRequest( iStatus );
    User::LeaveIfError( iStatus.Int() );    
    
    iStunclient = CSTUNClient::NewL( retransmitInterval,
                                     serveraddress,
                                     serverport,
                                     service,
                                     iSocketServ,
                                     iConnection,
                                     *iDeltatimer,
                                     *this,
                                     obtainsharedsecret,
                                     failifnoSRVrecordfound,
                                     icmpUsed,
                                     iMux );
                                     
    CActiveScheduler::Start();
  
    iCSTUNClientImplementation = CSTUNClientImplementation::NewL( *iStunclient, 
            retransmitInterval, serveraddress, serverport, service, iSocketServ,
            iConnection, *iDeltatimer, *this, obtainsharedsecret,
            failifnoSRVrecordfound, icmpUsed, iMux, EUdpProtocol );
    
    iResolvingTLS = new ( ELeave )CSTUNClientResolvingTLS( *iResolvingUDP );
    iCSTUNClientResolvingTCP = new ( ELeave )CSTUNClientResolvingTCP( *iResolvingTLS, 
            *iResolvingUDP );
    }
    
void ut_cstunclientresolvingtcp::Teardown(  )
    {
    delete iResolvingTLS;
    delete iCSTUNClientResolvingTCP;
    delete iCSTUNClientImplementation;
    delete iStunclient;    
    iConnection.Stop();
    iConnection.Close();
    iSocketServ.Close();
    delete iDeltatimer;
    delete iMux;
    }

void ut_cstunclientresolvingtcp::T_CSTUNClientResolvingTCP__CSTUNClientResolvingTCPL(  )
    {
    iCSTUNClientResolvingTCP->~CSTUNClientResolvingTCP( );
    }
    
void ut_cstunclientresolvingtcp::T_CSTUNClientResolvingTCP_ResolvingCompletedLL(  )
    {
    NATFW_EUNIT_ASSERT_NO_LEAVE( iCSTUNClientResolvingTCP->ResolvingCompletedL(
            *iCSTUNClientImplementation, EFalse ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iCSTUNClientResolvingTCP->ResolvingCompletedL(
            *iCSTUNClientImplementation, ETrue ) );
    }
    
void ut_cstunclientresolvingtcp::T_CSTUNClientResolvingTCP_ResolvingFailedL(  )
    {
    iCSTUNClientResolvingTCP->ResolvingFailed( *iCSTUNClientImplementation, KErrArgument );
    EUNIT_ASSERT_PANIC( iCSTUNClientResolvingTCP->ResolvingFailed(
            *iCSTUNClientImplementation, KErrNone ), "", KErrArgument  );
    }
    

//  TEST TABLE
EUNIT_BEGIN_TEST_TABLE(
    ut_cstunclientresolvingtcp,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "~CSTUNClientResolvingTCP - test0",
    "CSTUNClientResolvingTCP",
    "~CSTUNClientResolvingTCP - test0",
    "FUNCTIONALITY",
    SetupL, T_CSTUNClientResolvingTCP__CSTUNClientResolvingTCPL, Teardown)
    
EUNIT_TEST(
    "ResolvingCompletedL - test1",
    "CSTUNClientResolvingTCP",
    "ResolvingCompletedL - test1",
    "FUNCTIONALITY",
    SetupL, T_CSTUNClientResolvingTCP_ResolvingCompletedLL, Teardown)
    
EUNIT_TEST(
    "ResolvingFailed - test2",
    "CSTUNClientResolvingTCP",
    "ResolvingFailed - test2",
    "FUNCTIONALITY",
    SetupL, T_CSTUNClientResolvingTCP_ResolvingFailedL, Teardown)
    

EUNIT_END_TEST_TABLE

//  END OF FILE
