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
#include "ut_cstunclientimplementation.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <digia/eunit/EUnitDecorators.h>
#include <commdbconnpref.h>

//  INTERNAL INCLUDES
#include "cstunclientimplementation.h"
#include "natfwstunclient.h"
#include "natfwstunbinding.h"
#include "tstuncallbackinfo.h"

#include "cstunclientready.h"
#include "cstunclientrenewsharedsecret.h"
#include "cstunclientgetsharedsecret.h"
#include "cstunclientresolvingudp.h"
#include "cstunbindingimplementation.h"
#include "natfwunittestmacros.h"

_LIT8( KServerAddress, "10.32.194.251" );
_LIT8( KUsername, "userAA" );
_LIT8( KPassword,"salasana");

const TBufC8<16> password( KPassword );
const TBufC8<6> usernameA( KUsername );

// CONSTRUCTION
ut_cstunclientimplementation* ut_cstunclientimplementation::NewL()
    {
    ut_cstunclientimplementation* self = ut_cstunclientimplementation::NewLC();
    CleanupStack::Pop();

    return self;
    }

ut_cstunclientimplementation* ut_cstunclientimplementation::NewLC()
    {
    ut_cstunclientimplementation* self = new( ELeave ) ut_cstunclientimplementation();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
ut_cstunclientimplementation::~ut_cstunclientimplementation()
    {
    }

// Default constructor
ut_cstunclientimplementation::ut_cstunclientimplementation()
    {
    }

// Second phase construct
void ut_cstunclientimplementation::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

// from mstunclientbserver
void ut_cstunclientimplementation::STUNClientInitCompleted( const CSTUNClient& /*aClient*/,
                              TInt aCompletionCode )
    {
    RDebug::Print( _L("**NATFW STUNClientInitCompleted, code: %d"), aCompletionCode );
    CActiveScheduler::Stop();
    }


void ut_cstunclientimplementation::STUNBindingEventOccurredL( TSTUNBindingEvent aEvent, 
                                const CBinding& /*aBinding*/ )
    {
    RDebug::Print( _L("**NATFW STUNBindingEventOccurredL, code: %d"), aEvent );
    CActiveScheduler::Stop();
    }


void ut_cstunclientimplementation::STUNBindingErrorOccurred( const CBinding& /*aBinding*/, 
                               TInt aError )
    {
    RDebug::Print( _L("**NATFW STUNBindingErrorOccurred, code: %d"), aError );
    CActiveScheduler::Stop();
    }

void ut_cstunclientimplementation::Notify( TUint /*aSessionId*/, TUint /*aStreamId*/, 
    TNotifyType /*aType*/, TInt /*aError*/ )
    {
    RDebug::Print( _L("**NATFW Notify") );
    }

void ut_cstunclientimplementation::SetupL(  )
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
 
    User::LeaveIfError( iSocket.Open( iSocketServ, KAfInet, KSockDatagram,
            KProtocolInetUdp ));    
    iSocket.SetLocalPort( 5000 );                                 
    iBinding = CSTUNBinding::NewL( *iStunclient, iSocket );
    
    iRenewSharedSecret = new ( ELeave ) CSTUNClientRenewSharedSecret;
    iReady = new ( ELeave ) CSTUNClientReady( *iRenewSharedSecret );
    iGetSharedSecret = new ( ELeave ) CSTUNClientGetSharedSecret( *iReady );
    iResolvingUDP = new ( ELeave ) CSTUNClientResolvingUDP( *iGetSharedSecret,
            *iReady );
    iCSTUNClientImplementation->ChangeState( iGetSharedSecret );
    }
    

void ut_cstunclientimplementation::Teardown(  )
    {
    delete iBinding;
    delete iStunclient;
    delete iCSTUNClientImplementation;
    iSocket.Close();
    iConnection.Stop();
    iConnection.Close();
    iSocketServ.Close();
    delete iServeraddress;
    delete iResponse;
    delete iRespDataIndication;
    delete iDeltatimer;
    delete iMux;
    delete iRenewSharedSecret;
    delete iReady;
    delete iGetSharedSecret;
    delete iResolvingUDP;
    }
    

void ut_cstunclientimplementation::T_CSTUNClientImplementation__CSTUNClientImplementationL(  )
    {
    /*TInetAddr serveraddress;
    TBufC8<4> service( KStun );
    EUNIT_ASSERT_SPECIFIC_LEAVE( CSTUNClientImplementation* impl = CSTUNClientImplementation::NewL( *iStunclient, 
            0, serveraddress, 0, service, iSocketServ,
            iConnection, *iDeltatimer, *this, EFalse,
            EFalse, EFalse, iMux, EUdpProtocol ), KErrArgument );*/
    
    iCSTUNClientImplementation->~CSTUNClientImplementation( );
    }
    
void ut_cstunclientimplementation::T_CSTUNClientImplementation_TimerExpiredLL(  )
    {
    NATFW_EUNIT_ASSERT_SPECIFIC_LEAVE( iCSTUNClientImplementation->TimerExpiredL( ), KErrNotFound );
    }
    
void ut_cstunclientimplementation::T_CSTUNClientImplementation_LeaveFromTimerExpiredL(  )
    {
	iCSTUNClientImplementation->LeaveFromTimerExpired( KErrArgument );
    }
    
void ut_cstunclientimplementation::T_CSTUNClientImplementation_CompletedLL(  )
    {
    iCSTUNClientImplementation->ChangeState( iResolvingUDP );
    iCSTUNClientImplementation->CompletedL( );
    }
    
void ut_cstunclientimplementation::T_CSTUNClientImplementation_ErrorOccuredL(  )
    {
    iCSTUNClientImplementation->ChangeState( iResolvingUDP );
	iCSTUNClientImplementation->ErrorOccured( KErrNone );
    
    iCSTUNClientImplementation->ChangeState( NULL );
    iCSTUNClientImplementation->ErrorOccured( KErrNone );
    }
    
void ut_cstunclientimplementation::T_CSTUNClientImplementation_IcmpErrorL(  )
    {
	iCSTUNClientImplementation->IcmpError( KErrNone );
    }
    
void ut_cstunclientimplementation::T_CSTUNClientImplementation_SharedSecretObtainedLL(  )
    {
    iCSTUNClientImplementation->SharedSecretObtainedL( );
    }
    
void ut_cstunclientimplementation::T_CSTUNClientImplementation_SharedSecretErrorLL(  )
    {
	iCSTUNClientImplementation->SharedSecretErrorL( KErrArgument );
    }
    
void ut_cstunclientimplementation::T_CSTUNClientImplementation_STUNClientL(  )
    {
    iCSTUNClientImplementation->STUNClient( );
    }
    
void ut_cstunclientimplementation::T_CSTUNClientImplementation_TimerProviderL(  )
    {
    iCSTUNClientImplementation->TimerProvider( );
    }
    
void ut_cstunclientimplementation::T_CSTUNClientImplementation_RetransmitIntervalL(  )
    {
    iCSTUNClientImplementation->RetransmitInterval( );
    }
    
void ut_cstunclientimplementation::T_CSTUNClientImplementation_TransportProtocolL(  )
    {
    iCSTUNClientImplementation->TransportProtocol( );
    }
    
void ut_cstunclientimplementation::T_CSTUNClientImplementation_AddressResolvedLL(  )
    {
	iCSTUNClientImplementation->AddressResolvedL( *iBinding );
    }
    
void ut_cstunclientimplementation::T_CSTUNClientImplementation_ObtainSharedSecretLL(  )
    {
    iCSTUNClientImplementation->ChangeState( iRenewSharedSecret );
	iCSTUNClientImplementation->ObtainSharedSecretL( *iBinding );
    }
    
void ut_cstunclientimplementation::T_CSTUNClientImplementation_SharedSecretRejectedLL(  )
    {
    iCSTUNClientImplementation->ChangeState( iRenewSharedSecret );
	iCSTUNClientImplementation->SharedSecretRejectedL( *iBinding, usernameA, password );
    }
    
void ut_cstunclientimplementation::T_CSTUNClientImplementation_ObtainServerAddressL(  )
    {
    TInetAddr address;
	iCSTUNClientImplementation->ObtainServerAddress( address );
    }
    
void ut_cstunclientimplementation::T_CSTUNClientImplementation_ObtainTransactionIDLL(  )
    {
    TNATFWUNSAFTransactionID transactionID;
    iCSTUNClientImplementation->ObtainTransactionIDL( transactionID );
    }
    
void ut_cstunclientimplementation::T_CSTUNClientImplementation_AttachBindingLL(  )
    {
    iCSTUNClientImplementation->AttachBindingL( *iBinding );
    iCSTUNClientImplementation->DetachBinding( *iBinding );
    }
    
void ut_cstunclientimplementation::T_CSTUNClientImplementation_DetachBindingL(  )
    {
    iCSTUNClientImplementation->DetachBinding( *iBinding );
    }
    
void ut_cstunclientimplementation::T_CSTUNClientImplementation_BindingErrorLL(  )
    {
    TBool isFatal( EFalse );
    iCSTUNClientImplementation->BindingErrorL( *iBinding, KErrNone, isFatal );
    
    iCSTUNClientImplementation->BindingErrorL( *iBinding, 401, isFatal );
    iCSTUNClientImplementation->BindingErrorL( *iBinding, 430, isFatal );
    iCSTUNClientImplementation->BindingErrorL( *iBinding, 406, isFatal );
    iCSTUNClientImplementation->BindingErrorL( *iBinding, 432, isFatal );
    iCSTUNClientImplementation->BindingErrorL( *iBinding, 433, isFatal );
    iCSTUNClientImplementation->SetCredentialsL( usernameA, password );
    iCSTUNClientImplementation->BindingErrorL( *iBinding, 433, isFatal );
    iCSTUNClientImplementation->BindingErrorL( *iBinding, 434, isFatal );
    
    iCSTUNClientImplementation->AttachBindingL( *iBinding );
    NATFW_EUNIT_ASSERT_SPECIFIC_LEAVE( 
            iCSTUNClientImplementation->BindingErrorL( *iBinding, 436, isFatal ), KErrNotFound );
    
    iCSTUNClientImplementation->DetachBinding( *iBinding );
    }
    
void ut_cstunclientimplementation::T_CSTUNClientImplementation_BindingEventOccurredL(  )
    {
    iCSTUNClientImplementation->BindingEventOccurred( *iBinding, TSTUNCallbackInfo::EEventAddressResolvedL );
     
    }
    
void ut_cstunclientimplementation::T_CSTUNClientImplementation_RenewSharedSecretLL(  )
    {
    iCSTUNClientImplementation->RenewSharedSecretL( );
     
    }
    
void ut_cstunclientimplementation::T_CSTUNClientImplementation_UsernameForIndicationL(  )
    {
    iCSTUNClientImplementation->UsernameForIndication( );
     
    }
    
void ut_cstunclientimplementation::T_CSTUNClientImplementation_PasswordForIndicationL(  )
    {
    iCSTUNClientImplementation->PasswordForIndication( );
     
    }
    
void ut_cstunclientimplementation::T_CSTUNClientImplementation_IsInitializedL(  )
    {
    iCSTUNClientImplementation->IsInitialized( );
     
    }
    
void ut_cstunclientimplementation::T_CSTUNClientImplementation_STUNServerAddrLL(  )
    {
    NATFW_EUNIT_ASSERT_SPECIFIC_LEAVE( iCSTUNClientImplementation->STUNServerAddrL( ), KErrNotReady );
    }
    
void ut_cstunclientimplementation::T_CSTUNClientImplementation_SetCredentialsLL(  )
    {
    iCSTUNClientImplementation->SetCredentialsL( usernameA, password );
     
    }
    
void ut_cstunclientimplementation::T_CSTUNClientImplementation_SharedSecretObtainedL(  )
    {
    iCSTUNClientImplementation->SharedSecretObtained( );
    }
    
void ut_cstunclientimplementation::T_CSTUNClientImplementation_HasPresetCredentialsL(  )
    {
    iCSTUNClientImplementation->HasPresetCredentials( );
     
    }
    
void ut_cstunclientimplementation::T_CSTUNClientImplementation_ChangeStateL(  )
    {
    CSTUNClientRenewSharedSecret* renewSharedSecret = 
        new ( ELeave ) CSTUNClientRenewSharedSecret;
    CleanupStack::PushL( renewSharedSecret );
    
    CSTUNClientReady* ready = new ( ELeave ) CSTUNClientReady( *renewSharedSecret );
    CleanupStack::PushL( ready );
    
    iCSTUNClientImplementation->ChangeState( ready );
    
    CleanupStack::PopAndDestroy( ready );
    CleanupStack::PopAndDestroy( renewSharedSecret );
    }
    
void ut_cstunclientimplementation::T_CSTUNClientImplementation_TerminateL(  )
    {
    iCSTUNClientImplementation->Terminate( KErrArgument );
    iCSTUNClientImplementation->AttachBindingL( *iBinding );
    iCSTUNClientImplementation->ChangeState( iReady );
    iCSTUNClientImplementation->Terminate( KErrArgument );
    iCSTUNClientImplementation->DetachBinding( *iBinding );
    }
    
void ut_cstunclientimplementation::T_CSTUNClientImplementation_TlsResolvedLL(  )
    {
    iCSTUNClientImplementation->TlsResolvedL( );
     
    }
    
void ut_cstunclientimplementation::T_CSTUNClientImplementation_TcpResolvedLL(  )
    {
    TBool tcpResolved( EFalse );
    iCSTUNClientImplementation->TcpResolvedL( tcpResolved );
     
    }
    
void ut_cstunclientimplementation::T_CSTUNClientImplementation_FreeResolverResourcesL(  )
    {
    iCSTUNClientImplementation->FreeResolverResources( );
     
    }
    
void ut_cstunclientimplementation::T_CSTUNClientImplementation_GetSharedSecretFromServerLL(  )
    {
    NATFW_EUNIT_ASSERT_SPECIFIC_LEAVE( iCSTUNClientImplementation->GetSharedSecretFromServerL( ), KErrNotFound );
    }
    
void ut_cstunclientimplementation::T_CSTUNClientImplementation_PassSharedSecretToBindingLL(  )
    {
    /*TInetAddr addr;
    iCSTUNClientImplementation->iTcpAddresses.Append( addr );
    iCSTUNClientImplementation->GetSharedSecretFromServerL( );
    iCSTUNClientImplementation->ChangeState( iGetSharedSecret );
    //iBinding->Implementation().ChangeState( iGetSharedSecret );*/
    EUNIT_ASSERT_PANIC( iCSTUNClientImplementation->PassSharedSecretToBindingL( *iBinding ), "", KErrGeneral );
    }
    
void ut_cstunclientimplementation::T_CSTUNClientImplementation_InformWaitingBindingsLL(  )
    {
    iCSTUNClientImplementation->InformWaitingBindingsL( );
    
    iCSTUNClientImplementation->AttachBindingL( *iBinding );
    iCSTUNClientImplementation->ChangeState( iGetSharedSecret );
    iCSTUNClientImplementation->InformWaitingBindingsL( );
    iCSTUNClientImplementation->DetachBinding( *iBinding );
    }
    
void ut_cstunclientimplementation::T_CSTUNClientImplementation_HandleSharedSecretErrorLL(  )
    {
    iCSTUNClientImplementation->HandleSharedSecretErrorL( KErrArgument );
     
    }
    
void ut_cstunclientimplementation::T_CSTUNClientImplementation_DoesSharedSecretMatchL(  )
    {
    iCSTUNClientImplementation->DoesSharedSecretMatch( usernameA, password );
     
    }
    
void ut_cstunclientimplementation::T_CSTUNClientImplementation_PassInitCompletedLL(  )
    {
    iCSTUNClientImplementation->PassInitCompletedL( KErrNone );
     
    }
    
void ut_cstunclientimplementation::T_CSTUNClientImplementation_PassCredentialsRejectedLL(  )
    {
    iCSTUNClientImplementation->PassCredentialsRejectedL( *iBinding );
     
    }
    
void ut_cstunclientimplementation::T_CSTUNClientImplementation_StartSharedSecretTimerL(  )
    {
    iCSTUNClientImplementation->StartSharedSecretTimer( );
     
    }
    
void ut_cstunclientimplementation::T_CSTUNClientImplementation_MultiplexerInstanceL(  )
    {
    iCSTUNClientImplementation->MultiplexerInstance( );
     
    }
    
void ut_cstunclientimplementation::T_CSTUNClientImplementation___DbgTestInvariantL(  )
    {
    iCSTUNClientImplementation->__DbgTestInvariant( );
    }

void ut_cstunclientimplementation::T_CSTUNClientImplementation_RemoveAddressL(  )
    {
    TInetAddr address;
    iCSTUNClientImplementation->ObtainServerAddress( address );
    iCSTUNClientImplementation->RemoveAddress( address );
    }

//  TEST TABLE
EUNIT_BEGIN_TEST_TABLE(
    ut_cstunclientimplementation,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "~CSTUNClientImplementation - test0",
    "CSTUNClientImplementation",
    "~CSTUNClientImplementation - test0",
    "FUNCTIONALITY",
    SetupL, T_CSTUNClientImplementation__CSTUNClientImplementationL, Teardown)
    
EUNIT_TEST(
    "TimerExpiredL - test1",
    "CSTUNClientImplementation",
    "TimerExpiredL - test1",
    "FUNCTIONALITY",
    SetupL, T_CSTUNClientImplementation_TimerExpiredLL, Teardown)
    
EUNIT_TEST(
    "LeaveFromTimerExpired - test2",
    "CSTUNClientImplementation",
    "LeaveFromTimerExpired - test2",
    "FUNCTIONALITY",
    SetupL, T_CSTUNClientImplementation_LeaveFromTimerExpiredL, Teardown)
    
EUNIT_TEST(
    "CompletedL - test3",
    "CSTUNClientImplementation",
    "CompletedL - test3",
    "FUNCTIONALITY",
    SetupL, T_CSTUNClientImplementation_CompletedLL, Teardown)
    
EUNIT_TEST(
    "ErrorOccured - test4",
    "CSTUNClientImplementation",
    "ErrorOccured - test4",
    "FUNCTIONALITY",
    SetupL, T_CSTUNClientImplementation_ErrorOccuredL, Teardown)
    
EUNIT_TEST(
    "IcmpError - test5",
    "CSTUNClientImplementation",
    "IcmpError - test5",
    "FUNCTIONALITY",
    SetupL, T_CSTUNClientImplementation_IcmpErrorL, Teardown)
    
EUNIT_TEST(
    "SharedSecretObtainedL - test6",
    "CSTUNClientImplementation",
    "SharedSecretObtainedL - test6",
    "FUNCTIONALITY",
    SetupL, T_CSTUNClientImplementation_SharedSecretObtainedLL, Teardown)
    
EUNIT_TEST(
    "SharedSecretErrorL - test7",
    "CSTUNClientImplementation",
    "SharedSecretErrorL - test7",
    "FUNCTIONALITY",
    SetupL, T_CSTUNClientImplementation_SharedSecretErrorLL, Teardown)
    
EUNIT_TEST(
    "STUNClient - test8",
    "CSTUNClientImplementation",
    "STUNClient - test8",
    "FUNCTIONALITY",
    SetupL, T_CSTUNClientImplementation_STUNClientL, Teardown)
    
EUNIT_TEST(
    "TimerProvider - test9",
    "CSTUNClientImplementation",
    "TimerProvider - test9",
    "FUNCTIONALITY",
    SetupL, T_CSTUNClientImplementation_TimerProviderL, Teardown)
    
EUNIT_TEST(
    "RetransmitInterval - test10",
    "CSTUNClientImplementation",
    "RetransmitInterval - test10",
    "FUNCTIONALITY",
    SetupL, T_CSTUNClientImplementation_RetransmitIntervalL, Teardown)
    
EUNIT_TEST(
    "TransportProtocol - test11",
    "CSTUNClientImplementation",
    "TransportProtocol - test11",
    "FUNCTIONALITY",
    SetupL, T_CSTUNClientImplementation_TransportProtocolL, Teardown)
    
EUNIT_TEST(
    "AddressResolvedL - test12",
    "CSTUNClientImplementation",
    "AddressResolvedL - test12",
    "FUNCTIONALITY",
    SetupL, T_CSTUNClientImplementation_AddressResolvedLL, Teardown)
    
EUNIT_TEST(
    "ObtainSharedSecretL - test13",
    "CSTUNClientImplementation",
    "ObtainSharedSecretL - test13",
    "FUNCTIONALITY",
    SetupL, T_CSTUNClientImplementation_ObtainSharedSecretLL, Teardown)
    
EUNIT_TEST(
    "SharedSecretRejectedL - test14",
    "CSTUNClientImplementation",
    "SharedSecretRejectedL - test14",
    "FUNCTIONALITY",
    SetupL, T_CSTUNClientImplementation_SharedSecretRejectedLL, Teardown)
  
EUNIT_TEST(
    "ObtainServerAddress - test15",
    "CSTUNClientImplementation",
    "ObtainServerAddress - test15",
    "FUNCTIONALITY",
    SetupL, T_CSTUNClientImplementation_ObtainServerAddressL, Teardown)
    
EUNIT_TEST(
    "ObtainTransactionIDL - test16",
    "CSTUNClientImplementation",
    "ObtainTransactionIDL - test16",
    "FUNCTIONALITY",
    SetupL, T_CSTUNClientImplementation_ObtainTransactionIDLL, Teardown)
    
EUNIT_TEST(
    "AttachBindingL - test17",
    "CSTUNClientImplementation",
    "AttachBindingL - test17",
    "FUNCTIONALITY",
    SetupL, T_CSTUNClientImplementation_AttachBindingLL, Teardown)
    
EUNIT_TEST(
    "DetachBinding - test18",
    "CSTUNClientImplementation",
    "DetachBinding - test18",
    "FUNCTIONALITY",
    SetupL, T_CSTUNClientImplementation_DetachBindingL, Teardown)
    
EUNIT_TEST(
    "BindingErrorL - test19",
    "CSTUNClientImplementation",
    "BindingErrorL - test19",
    "FUNCTIONALITY",
    SetupL, T_CSTUNClientImplementation_BindingErrorLL, Teardown)
    
EUNIT_TEST(
    "BindingEventOccurred - test20",
    "CSTUNClientImplementation",
    "BindingEventOccurred - test20",
    "FUNCTIONALITY",
    SetupL, T_CSTUNClientImplementation_BindingEventOccurredL, Teardown)
    
EUNIT_TEST(
    "RenewSharedSecretL - test21",
    "CSTUNClientImplementation",
    "RenewSharedSecretL - test21",
    "FUNCTIONALITY",
    SetupL, T_CSTUNClientImplementation_RenewSharedSecretLL, Teardown)
    
EUNIT_TEST(
    "UsernameForIndication - test22",
    "CSTUNClientImplementation",
    "UsernameForIndication - test22",
    "FUNCTIONALITY",
    SetupL, T_CSTUNClientImplementation_UsernameForIndicationL, Teardown)
    
EUNIT_TEST(
    "PasswordForIndication - test23",
    "CSTUNClientImplementation",
    "PasswordForIndication - test23",
    "FUNCTIONALITY",
    SetupL, T_CSTUNClientImplementation_PasswordForIndicationL, Teardown)
    
EUNIT_TEST(
    "IsInitialized - test24",
    "CSTUNClientImplementation",
    "IsInitialized - test24",
    "FUNCTIONALITY",
    SetupL, T_CSTUNClientImplementation_IsInitializedL, Teardown)
    
EUNIT_TEST(
    "STUNServerAddrL - test25",
    "CSTUNClientImplementation",
    "STUNServerAddrL - test25",
    "FUNCTIONALITY",
    SetupL, T_CSTUNClientImplementation_STUNServerAddrLL, Teardown)
    
EUNIT_TEST(
    "SetCredentialsL - test26",
    "CSTUNClientImplementation",
    "SetCredentialsL - test26",
    "FUNCTIONALITY",
    SetupL, T_CSTUNClientImplementation_SetCredentialsLL, Teardown)
    
EUNIT_TEST(
    "SharedSecretObtained - test27",
    "CSTUNClientImplementation",
    "SharedSecretObtained - test27",
    "FUNCTIONALITY",
    SetupL, T_CSTUNClientImplementation_SharedSecretObtainedL, Teardown)
    
EUNIT_TEST(
    "HasPresetCredentials - test28",
    "CSTUNClientImplementation",
    "HasPresetCredentials - test28",
    "FUNCTIONALITY",
    SetupL, T_CSTUNClientImplementation_HasPresetCredentialsL, Teardown)
    
EUNIT_TEST(
    "ChangeState - test29",
    "CSTUNClientImplementation",
    "ChangeState - test29",
    "FUNCTIONALITY",
    SetupL, T_CSTUNClientImplementation_ChangeStateL, Teardown)
    
EUNIT_TEST(
    "Terminate - test30",
    "CSTUNClientImplementation",
    "Terminate - test30",
    "FUNCTIONALITY",
    SetupL, T_CSTUNClientImplementation_TerminateL, Teardown)
    
EUNIT_TEST(
    "TlsResolvedL - test31",
    "CSTUNClientImplementation",
    "TlsResolvedL - test31",
    "FUNCTIONALITY",
    SetupL, T_CSTUNClientImplementation_TlsResolvedLL, Teardown)
    
EUNIT_TEST(
    "TcpResolvedL - test32",
    "CSTUNClientImplementation",
    "TcpResolvedL - test32",
    "FUNCTIONALITY",
    SetupL, T_CSTUNClientImplementation_TcpResolvedLL, Teardown)
    
EUNIT_TEST(
    "FreeResolverResources - test33",
    "CSTUNClientImplementation",
    "FreeResolverResources - test33",
    "FUNCTIONALITY",
    SetupL, T_CSTUNClientImplementation_FreeResolverResourcesL, Teardown)
    
EUNIT_TEST(
    "GetSharedSecretFromServerL - test34",
    "CSTUNClientImplementation",
    "GetSharedSecretFromServerL - test34",
    "FUNCTIONALITY",
    SetupL, T_CSTUNClientImplementation_GetSharedSecretFromServerLL, Teardown)
    
EUNIT_TEST(
    "PassSharedSecretToBindingL - test35",
    "CSTUNClientImplementation",
    "PassSharedSecretToBindingL - test35",
    "FUNCTIONALITY",
    SetupL, T_CSTUNClientImplementation_PassSharedSecretToBindingLL, Teardown)

EUNIT_TEST(
    "InformWaitingBindingsL - test36",
    "CSTUNClientImplementation",
    "InformWaitingBindingsL - test36",
    "FUNCTIONALITY",
    SetupL, T_CSTUNClientImplementation_InformWaitingBindingsLL, Teardown)
    
EUNIT_TEST(
    "HandleSharedSecretErrorL - test37",
    "CSTUNClientImplementation",
    "HandleSharedSecretErrorL - test37",
    "FUNCTIONALITY",
    SetupL, T_CSTUNClientImplementation_HandleSharedSecretErrorLL, Teardown)
    
EUNIT_TEST(
    "DoesSharedSecretMatch - test38",
    "CSTUNClientImplementation",
    "DoesSharedSecretMatch - test38",
    "FUNCTIONALITY",
    SetupL, T_CSTUNClientImplementation_DoesSharedSecretMatchL, Teardown)
    
EUNIT_TEST(
    "PassInitCompletedL - test39",
    "CSTUNClientImplementation",
    "PassInitCompletedL - test39",
    "FUNCTIONALITY",
    SetupL, T_CSTUNClientImplementation_PassInitCompletedLL, Teardown)
    
EUNIT_TEST(
    "PassCredentialsRejectedL - test40",
    "CSTUNClientImplementation",
    "PassCredentialsRejectedL - test40",
    "FUNCTIONALITY",
    SetupL, T_CSTUNClientImplementation_PassCredentialsRejectedLL, Teardown)
    
EUNIT_TEST(
    "StartSharedSecretTimer - test41",
    "CSTUNClientImplementation",
    "StartSharedSecretTimer - test41",
    "FUNCTIONALITY",
    SetupL, T_CSTUNClientImplementation_StartSharedSecretTimerL, Teardown)
    
EUNIT_TEST(
    "MultiplexerInstance - test42",
    "CSTUNClientImplementation",
    "MultiplexerInstance - test42",
    "FUNCTIONALITY",
    SetupL, T_CSTUNClientImplementation_MultiplexerInstanceL, Teardown)
    
EUNIT_TEST(
    "__DbgTestInvariant - test49",
    "CSTUNClientImplementation",
    "__DbgTestInvariant - test49",
    "FUNCTIONALITY",
    SetupL, T_CSTUNClientImplementation___DbgTestInvariantL, Teardown)

EUNIT_TEST(
    "RemoveAddress - test50",
    "CSTUNClientImplementation",
    "RemoveAddress - test50",
    "FUNCTIONALITY",
    SetupL, T_CSTUNClientImplementation_RemoveAddressL, Teardown)
    

EUNIT_END_TEST_TABLE

//  END OF FILE
