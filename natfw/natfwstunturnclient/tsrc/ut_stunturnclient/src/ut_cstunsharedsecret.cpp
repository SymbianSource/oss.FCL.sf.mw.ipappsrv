/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
#include "ut_cstunsharedsecret.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <digia/eunit/EUnitDecorators.h>
#include <commdbconnpref.h>

//  INTERNAL INCLUDES
#include "cstunsharedsecret.h"
#include "natfwunsafbindingrequest.h"
#include "cstunsharedsecretstate.h"
#include "natfwunsafbindingresponse.h"
#include "natfwunsafbindingerrorresponse.h"
#include "ctransactionidgenerator.h"
#include "natfwunsafsharedsecretresponse.h"
#include "natfwunsafsharedsecreterrorresponse.h"
#include "natfwunsaferrorcodeattribute.h"
#include "natfwunsafusernameattribute.h"
#include "cstunsharedsecretwaitresponse.h"
#include "cstunsharedsecretactive.h"
#include "cstunsharedsecretwaittoretry.h"
#include "natfwunsafpasswordattribute.h"
#include "natfwunittestmacros.h"

TNATFWUNSAFTransactionID transactionID;
_LIT( KServerAddress, "10.32.194.251" );
_LIT8( KUsername, "userAA" );
_LIT8( KPassword,"salasana");
_LIT8( KErr, "ERSKA" );

const TBufC8<6> username( KUsername );
const TBufC8<8> password( KPassword );


// CONSTRUCTION
ut_cstunsharedsecret* ut_cstunsharedsecret::NewL()
    {
    ut_cstunsharedsecret* self = ut_cstunsharedsecret::NewLC();
    CleanupStack::Pop();

    return self;
    }

ut_cstunsharedsecret* ut_cstunsharedsecret::NewLC()
    {
    ut_cstunsharedsecret* self = new( ELeave ) ut_cstunsharedsecret();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
ut_cstunsharedsecret::~ut_cstunsharedsecret()
    {
    }

// Default constructor
ut_cstunsharedsecret::ut_cstunsharedsecret()
    {
    }

// Second phase construct
void ut_cstunsharedsecret::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void ut_cstunsharedsecret::SetupL(  )
    {
    iServerAddr.Input( KServerAddress );
    TUint iapID( 6 );
    iDeltatimer = CDeltaTimer::NewL( 1 );
    User::LeaveIfError( iSocketServ.Connect() );
    
    TCommDbConnPref pref;
    pref.SetDialogPreference( ECommDbDialogPrefDoNotPrompt );
    pref.SetIapId( iapID );
    
    User::LeaveIfError( iConnection.Open( iSocketServ ) );
    iConnection.Start( pref, iStatus );
    User::WaitForRequest( iStatus );
    User::LeaveIfError( iStatus.Int() );
    
    iIdGenerator = CTransactionIDGenerator::NewL();
         
    iCSTUNSharedSecret = CSTUNSharedSecret::NewL( iSocketServ,
        iConnection, *iDeltatimer, iServerAddr, *iIdGenerator, *this, username, password );
    }
    

void ut_cstunsharedsecret::Teardown(  )
    {
    delete iCSTUNSharedSecret;
    delete iIdGenerator;    
    iConnection.Stop();
    iConnection.Close();
    iSocketServ.Close();  
    delete iDeltatimer;
    }
    

void ut_cstunsharedsecret::ut_cstunsharedsecret_CSTUNSharedSecretL(  )
    {
    iCSTUNSharedSecret->~CSTUNSharedSecret( );
    }
    
void ut_cstunsharedsecret::ut_cstunsharedsecret_TimerExpiredLL(  )
    {
    iCSTUNSharedSecret->TimerExpiredL( );
    
    delete iCSTUNSharedSecret->iState;
    iCSTUNSharedSecret->TimerExpiredL( );
    }
    
void ut_cstunsharedsecret::ut_cstunsharedsecret_LeaveFromTimerExpiredL(  )
    {
    iCSTUNSharedSecret->LeaveFromTimerExpired( KErrTimedOut );
    }
    
void ut_cstunsharedsecret::ut_cstunsharedsecret_TlsConnectedLL(  )
    {
    NATFW_EUNIT_ASSERT_NO_LEAVE( iCSTUNSharedSecret->TlsConnectedL( ) );
    }
    
void ut_cstunsharedsecret::ut_cstunsharedsecret_IncomingMessageLL(  )
    {
    CNATFWUNSAFBindingRequest* request = CNATFWUNSAFBindingRequest::NewLC( transactionID );
    CSTUNSharedSecretActive* active = new ( ELeave ) CSTUNSharedSecretActive;
    CleanupStack::PushL( active );
    CSTUNSharedSecretWaitToRetry* waitToRetry = new ( ELeave ) CSTUNSharedSecretWaitToRetry;
    CleanupStack::PushL( waitToRetry );
    CSTUNSharedSecretWaitResponse* waitResponse =
        new ( ELeave ) CSTUNSharedSecretWaitResponse( *active, *waitToRetry );
    CleanupStack::PushL( waitResponse );
    iCSTUNSharedSecret->ChangeState( waitResponse );
    
    CNATFWUNSAFSharedSecretResponse* response = CNATFWUNSAFSharedSecretResponse::NewLC( transactionID );
    response->AddAttributeL( CNATFWUNSAFUsernameAttribute::NewLC( username ) );
    CleanupStack::Pop(); // CNATFWUNSAFUsernameAttribute
    response->AddAttributeL( CNATFWUNSAFPasswordAttribute::NewLC( KPassword ) );
    CleanupStack::Pop(); // CNATFWUNSAFPasswordAttribute
    iCSTUNSharedSecret->ResponseReceivedL( *response );    

    CleanupStack::PopAndDestroy( response );
        
    NATFW_EUNIT_ASSERT_NO_LEAVE( iCSTUNSharedSecret->SendRequestL( ) );

    iCSTUNSharedSecret->IncomingMessageL( request );
    
    delete iCSTUNSharedSecret->iState;
    iCSTUNSharedSecret->IncomingMessageL( request );
    
    CleanupStack::PopAndDestroy( waitResponse );
    CleanupStack::PopAndDestroy( waitToRetry );
    CleanupStack::PopAndDestroy( active );

    CleanupStack::Pop( request ); // pitäsköhän poistaa?
    }
    
void ut_cstunsharedsecret::ut_cstunsharedsecret_ErrorOccuredL(  )
    {
    iCSTUNSharedSecret->ErrorOccured( KErrArgument );
    
    delete iCSTUNSharedSecret->iState;
    iCSTUNSharedSecret->ErrorOccured( KErrArgument );
    }
    
void ut_cstunsharedsecret::ut_cstunsharedsecret_UsernameL(  )
    {
    EUNIT_ASSERT_EQUALS( iCSTUNSharedSecret->Username( ), KNullDesC8 );
    
    CNATFWUNSAFSharedSecretResponse* response = CNATFWUNSAFSharedSecretResponse::NewLC( transactionID );
    response->AddAttributeL( CNATFWUNSAFUsernameAttribute::NewLC( username ) );
    CleanupStack::Pop(); // CNATFWUNSAFUsernameAttribute
    response->AddAttributeL( CNATFWUNSAFPasswordAttribute::NewLC( KPassword ) );
    CleanupStack::Pop(); // CNATFWUNSAFPasswordAttribute
    
    iCSTUNSharedSecret->ResponseReceivedL( *response );

    CleanupStack::PopAndDestroy( response );
    EUNIT_ASSERT_NOT_EQUALS( iCSTUNSharedSecret->Username( ), KNullDesC8 );    
    }
    
void ut_cstunsharedsecret::ut_cstunsharedsecret_PasswordL(  )
    {
    EUNIT_ASSERT_EQUALS( iCSTUNSharedSecret->Password( ), KNullDesC8 );
    
    CNATFWUNSAFSharedSecretResponse* response = CNATFWUNSAFSharedSecretResponse::NewLC( transactionID );
    response->AddAttributeL( CNATFWUNSAFUsernameAttribute::NewLC( username ) );
    CleanupStack::Pop(); // CNATFWUNSAFUsernameAttribute
    response->AddAttributeL( CNATFWUNSAFPasswordAttribute::NewLC( KPassword ) );
    CleanupStack::Pop(); // CNATFWUNSAFPasswordAttribute

    iCSTUNSharedSecret->ResponseReceivedL( *response );

    CleanupStack::PopAndDestroy( response );
    EUNIT_ASSERT_NOT_EQUALS( iCSTUNSharedSecret->Password( ), KNullDesC8 );    
    }
    
void ut_cstunsharedsecret::ut_cstunsharedsecret_ChangeStateL(  )
    {
    CSTUNSharedSecretState* sharedSecretState;       
    iCSTUNSharedSecret->ChangeState( sharedSecretState );
    }
    
void ut_cstunsharedsecret::ut_cstunsharedsecret_TerminateL(  )
    {
    iCSTUNSharedSecret->Terminate( KErrArgument );
    }
    
void ut_cstunsharedsecret::ut_cstunsharedsecret_SendRequestLL(  )
    {
    CNATFWUNSAFSharedSecretResponse* response = CNATFWUNSAFSharedSecretResponse::NewLC( transactionID );
    response->AddAttributeL( CNATFWUNSAFUsernameAttribute::NewLC( username ) );
    CleanupStack::Pop(); // CNATFWUNSAFUsernameAttribute
    response->AddAttributeL( CNATFWUNSAFPasswordAttribute::NewLC( KPassword ) );
    CleanupStack::Pop(); // CNATFWUNSAFPasswordAttribute
    
    iCSTUNSharedSecret->ResponseReceivedL( *response );

    CleanupStack::PopAndDestroy( response );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iCSTUNSharedSecret->SendRequestL( ) );
    
    CNATFWUNSAFSharedSecretErrorResponse* errorresponse =
        CNATFWUNSAFSharedSecretErrorResponse::NewLC( transactionID );
    errorresponse->AddAttributeL( CNATFWUNSAFErrorCodeAttribute::NewLC( 401, KErr ) );
    CleanupStack::Pop(); // CNATFWUNSAFErrorCodeAttribute
    iCSTUNSharedSecret->ErrorResponseReceivedL( *errorresponse );
 
    CleanupStack::PopAndDestroy( errorresponse );
    
    iCSTUNSharedSecret->StopTimer();
    NATFW_EUNIT_ASSERT_NO_LEAVE( iCSTUNSharedSecret->SendRequestL( ) );    
    }
    
void ut_cstunsharedsecret::ut_cstunsharedsecret_CheckMessageL(  )
    {
    CNATFWUNSAFSharedSecretResponse* response = CNATFWUNSAFSharedSecretResponse::NewLC( transactionID );
    response->AddAttributeL( CNATFWUNSAFUsernameAttribute::NewLC( username ) );
    CleanupStack::Pop(); // CNATFWUNSAFUsernameAttribute
    response->AddAttributeL( CNATFWUNSAFPasswordAttribute::NewLC( KPassword ) );
    CleanupStack::Pop(); // CNATFWUNSAFPasswordAttribute
    
    iCSTUNSharedSecret->ResponseReceivedL( *response );

    CleanupStack::PopAndDestroy( response );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iCSTUNSharedSecret->SendRequestL( ) );
    CNATFWUNSAFBindingRequest* request = CNATFWUNSAFBindingRequest::NewLC( transactionID );    
    iCSTUNSharedSecret->CheckMessage( *request );
    CleanupStack::PopAndDestroy( request );
    }
    
void ut_cstunsharedsecret::ut_cstunsharedsecret_ResponseReceivedLL(  )
    {
    CNATFWUNSAFSharedSecretResponse* response = CNATFWUNSAFSharedSecretResponse::NewLC( transactionID );
    response->AddAttributeL( CNATFWUNSAFUsernameAttribute::NewLC( username ) );
    CleanupStack::Pop(); // CNATFWUNSAFUsernameAttribute
    response->AddAttributeL( CNATFWUNSAFPasswordAttribute::NewLC( KPassword ) );
    CleanupStack::Pop(); // CNATFWUNSAFPasswordAttribute
    iCSTUNSharedSecret->ResponseReceivedL( *response );

    CleanupStack::PopAndDestroy( response );
    }
    
void ut_cstunsharedsecret::ut_cstunsharedsecret_ErrorResponseReceivedLL(  )
    {
    NATFW_EUNIT_ASSERT_NO_LEAVE( iCSTUNSharedSecret->TlsConnectedL( ) );
    
    CNATFWUNSAFSharedSecretErrorResponse* response =
        CNATFWUNSAFSharedSecretErrorResponse::NewLC( transactionID );
    response->AddAttributeL( CNATFWUNSAFErrorCodeAttribute::NewLC( 401, KErr ) );
    CleanupStack::Pop(); // CNATFWUNSAFErrorCodeAttribute
    iCSTUNSharedSecret->ErrorResponseReceivedL( *response );
 
    CleanupStack::PopAndDestroy( response );
    }
    
void ut_cstunsharedsecret::ut_cstunsharedsecret___DbgTestInvariantL(  )
    {
    iCSTUNSharedSecret->__DbgTestInvariant( );
    }
    

//  TEST TABLE
EUNIT_BEGIN_TEST_TABLE(
    ut_cstunsharedsecret,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "~CSTUNSharedSecret - test0",
    "CSTUNSharedSecret",
    "~CSTUNSharedSecret - test0",
    "FUNCTIONALITY",
    SetupL, ut_cstunsharedsecret_CSTUNSharedSecretL, Teardown)
    
EUNIT_TEST(
    "TimerExpiredL - test1",
    "CSTUNSharedSecret",
    "TimerExpiredL - test1",
    "FUNCTIONALITY",
    SetupL, ut_cstunsharedsecret_TimerExpiredLL, Teardown)
    
EUNIT_TEST(
    "LeaveFromTimerExpired - test2",
    "CSTUNSharedSecret",
    "LeaveFromTimerExpired - test2",
    "FUNCTIONALITY",
    SetupL, ut_cstunsharedsecret_LeaveFromTimerExpiredL, Teardown)
    
EUNIT_TEST(
    "TlsConnectedL - test3",
    "CSTUNSharedSecret",
    "TlsConnectedL - test3",
    "FUNCTIONALITY",
    SetupL, ut_cstunsharedsecret_TlsConnectedLL, Teardown)
    
EUNIT_TEST(
    "IncomingMessageL - test4",
    "CSTUNSharedSecret",
    "IncomingMessageL - test4",
    "FUNCTIONALITY",
    SetupL, ut_cstunsharedsecret_IncomingMessageLL, Teardown)
    
EUNIT_TEST(
    "ErrorOccured - test5",
    "CSTUNSharedSecret",
    "ErrorOccured - test5",
    "FUNCTIONALITY",
    SetupL, ut_cstunsharedsecret_ErrorOccuredL, Teardown)
    
EUNIT_TEST(
    "Username - test6",
    "CSTUNSharedSecret",
    "Username - test6",
    "FUNCTIONALITY",
    SetupL, ut_cstunsharedsecret_UsernameL, Teardown)
    
EUNIT_TEST(
    "Password - test7",
    "CSTUNSharedSecret",
    "Password - test7",
    "FUNCTIONALITY",
    SetupL, ut_cstunsharedsecret_PasswordL, Teardown)
    
EUNIT_TEST(
    "ChangeState - test8",
    "CSTUNSharedSecret",
    "ChangeState - test8",
    "FUNCTIONALITY",
    SetupL, ut_cstunsharedsecret_ChangeStateL, Teardown)
    
EUNIT_TEST(
    "Terminate - test9",
    "CSTUNSharedSecret",
    "Terminate - test9",
    "FUNCTIONALITY",
    SetupL, ut_cstunsharedsecret_TerminateL, Teardown)
    
EUNIT_TEST(
    "SendRequestL - test10",
    "CSTUNSharedSecret",
    "SendRequestL - test10",
    "FUNCTIONALITY",
    SetupL, ut_cstunsharedsecret_SendRequestLL, Teardown)
    
EUNIT_TEST(
    "CheckMessage - test11",
    "CSTUNSharedSecret",
    "CheckMessage - test11",
    "FUNCTIONALITY",
    SetupL, ut_cstunsharedsecret_CheckMessageL, Teardown)
    
EUNIT_TEST(
    "ResponseReceivedL - test12",
    "CSTUNSharedSecret",
    "ResponseReceivedL - test12",
    "FUNCTIONALITY",
    SetupL, ut_cstunsharedsecret_ResponseReceivedLL, Teardown)
    
EUNIT_TEST(
    "ErrorResponseReceivedL - test13",
    "CSTUNSharedSecret",
    "ErrorResponseReceivedL - test13",
    "FUNCTIONALITY",
    SetupL, ut_cstunsharedsecret_ErrorResponseReceivedLL, Teardown)
    
EUNIT_TEST(
    "__DbgTestInvariant - test18",
    "CSTUNSharedSecret",
    "__DbgTestInvariant - test18",
    "FUNCTIONALITY",
    SetupL, ut_cstunsharedsecret___DbgTestInvariantL, Teardown)
    

EUNIT_END_TEST_TABLE

//  END OF FILE
