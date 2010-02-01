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
#include "ut_cstunbindinggetaddress.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <digia/eunit/EUnitDecorators.h>
#include <commdbconnpref.h>

//  INTERNAL INCLUDES
#include "natfwstunbinding.h"
#include "cstunbindingimplementation.h"
#include "mstunbindingobserver.h"
#include "cstunbindinggetaddress.h"
#include "cstunbindingactive.h"
#include "cstunbindingwaittoretry.h"
#include "cstunbindinggetsharedsecret.h"
#include "natfwstunrelaybinding.h"
#include "cstunrelaybindingimplementation.h"

_LIT8( KServerAddress, "10.32.194.251" );
_LIT8( KUsername, "userAA" );
_LIT8( KPassword,"salasana");
// CONSTRUCTION
ut_cstunbindinggetaddress* ut_cstunbindinggetaddress::NewL()
    {
    ut_cstunbindinggetaddress* self = ut_cstunbindinggetaddress::NewLC();
    CleanupStack::Pop();

    return self;
    }

ut_cstunbindinggetaddress* ut_cstunbindinggetaddress::NewLC()
    {
    ut_cstunbindinggetaddress* self = new( ELeave ) ut_cstunbindinggetaddress();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
ut_cstunbindinggetaddress::~ut_cstunbindinggetaddress()
    {
    }

// Default constructor
ut_cstunbindinggetaddress::ut_cstunbindinggetaddress()
    {
    }

// Second phase construct
void ut_cstunbindinggetaddress::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS
// from mstunclientbserver
void ut_cstunbindinggetaddress::STUNClientInitCompleted( const CSTUNClient& /*aClient*/,
                              TInt aCompletionCode )
    {
    RDebug::Print( _L("**NATFW STUNClientInitCompleted, code: %d"), aCompletionCode );
    CActiveScheduler::Stop();
    }


void ut_cstunbindinggetaddress::STUNBindingEventOccurredL( TSTUNBindingEvent aEvent, 
                                const CBinding& /*aBinding*/ )
    {
    RDebug::Print( _L("**NATFW STUNBindingEventOccurredL, code: %d"), aEvent );
    CActiveScheduler::Stop();
    }


void ut_cstunbindinggetaddress::STUNBindingErrorOccurred( const CBinding& /*aBinding*/, 
                               TInt aError )
    {
    RDebug::Print( _L("**NATFW STUNBindingErrorOccurred, code: %d"), aError );
    CActiveScheduler::Stop();
    }

void ut_cstunbindinggetaddress::Notify( TUint /*aSessionId*/, TUint /*aStreamId*/, 
    TNotifyType /*aType*/, TInt /*aError*/ )
    {
    RDebug::Print( _L("**NATFW Notify") );
    }


const CSTUNClient& ut_cstunbindinggetaddress::STUNClient() const
    {
    return *iStunclient;
    }

CDeltaTimer& ut_cstunbindinggetaddress::TimerProvider()
    {
    return *iDeltatimer;
    }
        
TInt ut_cstunbindinggetaddress::RetransmitInterval() const
    {
    TInt dummy(0);
    return dummy;
    }

TTransportProtocol ut_cstunbindinggetaddress::TransportProtocol() const
    {
    
    }

void ut_cstunbindinggetaddress::AddressResolvedL( const CBinding& /*aBinding*/ ) const
    {
    
    }

void ut_cstunbindinggetaddress::ObtainSharedSecretL( CBinding& /*aBinding*/ )
    {
    TBufC8<16> password( KPassword );
    TBufC8<6> usernameA( KUsername );    
    
    iImpl->CreateBindingRequestL( usernameA, password );
    iImpl->SendBindingRequestL();
    }
    
TBool ut_cstunbindinggetaddress::SharedSecretRejectedL( 
    const CBinding& /*aBinding*/, const TDesC8& /*aUsername*/, const TDesC8& /*aPassword*/ )
    {
    TBool dummy( EFalse );
    return dummy;    
    }

TBool ut_cstunbindinggetaddress::ObtainServerAddress( TInetAddr& aAddress )
    {
    #ifdef _DEBUG_EUNIT
        TInetAddr addr;
        addr.SetAddress( INET_ADDR( 10,32,194,251 ) );
        aAddress = addr;
        return ETrue;
    #endif

    return EFalse;
    }

void ut_cstunbindinggetaddress::ObtainTransactionIDL( TNATFWUNSAFTransactionID& /*aTransactionID*/ )
    {
    }
    
void ut_cstunbindinggetaddress::AttachBindingL( const CBinding& /*aBinding*/ )
    {
    }

void ut_cstunbindinggetaddress::DetachBinding( const CBinding& /*aBinding*/ )
    {
    }
void ut_cstunbindinggetaddress::BindingErrorL( const CBinding& /*aBinding*/, TInt aError, TBool /*aIsFatal*/ )
    {
    RDebug::Print( _L("**NATFW BindingErrorL, error: %d"), aError );
    }
void ut_cstunbindinggetaddress::BindingEventOccurred( const CBinding& /*aBinding*/, 
                           TSTUNCallbackInfo::TFunction /*aEvent*/ )
    {
    
    }

const TDesC8& ut_cstunbindinggetaddress::UsernameForIndication()
    {
    
    }

const TDesC8& ut_cstunbindinggetaddress::PasswordForIndication()
    {
    
    }


void ut_cstunbindinggetaddress::SetupL(  )
    {
    TUint iapID( 6 );
    TInt retransmitInterval( 10 );
    TBufC8<13> serveraddress( KServerAddress );
    TUint serverport( 2000 );
    TBufC8<4> protocol( KStun );
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
                                     protocol,
                                     iSocketServ,
                                     iConnection,
                                     *iDeltatimer,
                                     *this,
                                     obtainsharedsecret,
                                     failifnoSRVrecordfound,
                                     icmpUsed,
                                     iMux );
                                     
    CActiveScheduler::Start();   
    
    User::LeaveIfError( iSocket.Open( iSocketServ, KAfInet, KSockDatagram,
                                     KProtocolInetUdp ));
    iSocket.SetLocalPort( 5000 );                                 
    iBinding = CSTUNBinding::NewL( *iStunclient, iSocket );
    
    iImpl = CSTUNBindingImplementation::NewL( *iBinding, *this, iSocket );
    iCSTUNBindingGetAddress = new( ELeave )CSTUNBindingGetAddress;
    }
    

void ut_cstunbindinggetaddress::Teardown(  )
    {
    delete iImpl;
    delete iBinding;    
    delete iStunclient;    
    iSocket.Close();
    iConnection.Stop();
    iConnection.Close();
    iSocketServ.Close();
    delete iDeltatimer;
    delete iMux;   
    delete iCSTUNBindingGetAddress;  
    }
    

void ut_cstunbindinggetaddress::T_CSTUNBindingGetAddress__CSTUNBindingGetAddressL(  )
    {
    iCSTUNBindingGetAddress->~CSTUNBindingGetAddress( );
    }
    
void ut_cstunbindinggetaddress::T_CSTUNBindingGetAddress_PublicAddressReceivedLL(  )
    {
    TInetAddr publicAddr;
    iCSTUNBindingGetAddress->PublicAddressReceivedL( *iImpl, publicAddr );
    }
    
void ut_cstunbindinggetaddress::T_CSTUNBindingGetAddress_PublicAddressReceivedL2L(  )
    {
    TInetAddr reflexiveAddr;
    TInetAddr relatAddr;
    CSTUNRelayBinding* relayBinding = CSTUNRelayBinding::NewLC( *iStunclient, iStreamId, iSubstreamId );
    CSTUNRelayBindingImplementation* relayImpl = 
        CSTUNRelayBindingImplementation::NewL( *relayBinding, *this,
        iStreamId, iSubstreamId, iMux );
    CleanupStack::PushL( relayImpl );
    iCSTUNBindingGetAddress->PublicAddressReceivedL( *relayImpl, reflexiveAddr, relatAddr );
    CleanupStack::PopAndDestroy( relayImpl );
    CleanupStack::PopAndDestroy( relayBinding );
    }
    
void ut_cstunbindinggetaddress::T_CSTUNBindingGetAddress_TransactionErrorL(  )
    {
    _LIT8( KUsername, "userAA" );
    _LIT8( KPassword,"salasana");
    TBufC8<16> password( KPassword );
    TBufC8<6> usernameA( KUsername );
    
    iImpl->CreateBindingRequestL( usernameA, password );
    iCSTUNBindingGetAddress->TransactionError( *iImpl, KErrArgument );
    
    iImpl->CreateBindingRequestL( usernameA, password );
    iCSTUNBindingGetAddress->TransactionError( *iImpl, 500 );
    
    iImpl->CreateBindingRequestL( usernameA, password );
    iCSTUNBindingGetAddress->TransactionError( *iImpl, 430 );
    
    iImpl->CreateBindingRequestL( usernameA, password );
    iCSTUNBindingGetAddress->TransactionError( *iImpl, 436 );
    
    iImpl->CreateBindingRequestL( usernameA, password );
    iCSTUNBindingGetAddress->TransactionError( *iImpl, 432 );
    
    iImpl->CreateBindingRequestL( usernameA, password );
    iCSTUNBindingGetAddress->TransactionError( *iImpl, 434 );
    
    iImpl->CreateBindingRequestL( usernameA, password );
    iCSTUNBindingGetAddress->TransactionError( *iImpl, 401 );
    
    iImpl->CreateBindingRequestL( usernameA, password );
    iCSTUNBindingGetAddress->TransactionError( *iImpl, 300 );
    
    iImpl->CreateBindingRequestL( usernameA, password );
    iCSTUNBindingGetAddress->TransactionError( *iImpl, 300 );    
    }
    
void ut_cstunbindinggetaddress::T_CSTUNBindingGetAddress_SendRequestLL(  )
    {
    iCSTUNBindingGetAddress->SendRequestL( *iImpl );
    }
    
void ut_cstunbindinggetaddress::T_CSTUNBindingGetAddress_SetNeighbourStatesL(  )
    {
    CSTUNBindingGetAddress* getAddress = new ( ELeave ) CSTUNBindingGetAddress();
    CleanupStack::PushL( getAddress );
    CSTUNBindingGetSharedSecret* getSharedSecret =
        new ( ELeave ) CSTUNBindingGetSharedSecret( *getAddress );
    CleanupStack::PushL( getSharedSecret );
    CSTUNBindingActive* active = new ( ELeave ) CSTUNBindingActive( *getSharedSecret );
    CleanupStack::PushL( active );
    CSTUNBindingWaitToRetry* waitToRetry = new ( ELeave ) CSTUNBindingWaitToRetry( *getSharedSecret );
    CleanupStack::PushL( waitToRetry );
    iCSTUNBindingGetAddress->SetNeighbourStates( *getSharedSecret, 
            *waitToRetry, *active );
    
    CleanupStack::PopAndDestroy( waitToRetry );
    CleanupStack::PopAndDestroy( active );
    CleanupStack::PopAndDestroy( getSharedSecret );
    CleanupStack::PopAndDestroy( getAddress );
    }
    

//  TEST TABLE
EUNIT_BEGIN_TEST_TABLE(
    ut_cstunbindinggetaddress,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "~CSTUNBindingGetAddress - test0",
    "CSTUNBindingGetAddress",
    "~CSTUNBindingGetAddress - test0",
    "FUNCTIONALITY",
    SetupL, T_CSTUNBindingGetAddress__CSTUNBindingGetAddressL, Teardown)
    
EUNIT_TEST(
    "PublicAddressReceivedL - test1",
    "CSTUNBindingGetAddress",
    "PublicAddressReceivedL - test1",
    "FUNCTIONALITY",
    SetupL, T_CSTUNBindingGetAddress_PublicAddressReceivedLL, Teardown)
    
EUNIT_TEST(
    "PublicAddressReceivedL - test2",
    "CSTUNBindingGetAddress",
    "PublicAddressReceivedL - test2",
    "FUNCTIONALITY",
    SetupL, T_CSTUNBindingGetAddress_PublicAddressReceivedL2L, Teardown)
    
EUNIT_TEST(
    "TransactionError - test3",
    "CSTUNBindingGetAddress",
    "TransactionError - test3",
    "FUNCTIONALITY",
    SetupL, T_CSTUNBindingGetAddress_TransactionErrorL, Teardown)
    
EUNIT_TEST(
    "SendRequestL - test4",
    "CSTUNBindingGetAddress",
    "SendRequestL - test4",
    "FUNCTIONALITY",
    SetupL, T_CSTUNBindingGetAddress_SendRequestLL, Teardown)
    
EUNIT_TEST(
    "SetNeighbourStates - test5",
    "CSTUNBindingGetAddress",
    "SetNeighbourStates - test5",
    "FUNCTIONALITY",
    SetupL, T_CSTUNBindingGetAddress_SetNeighbourStatesL, Teardown)
    

EUNIT_END_TEST_TABLE

//  END OF FILE
