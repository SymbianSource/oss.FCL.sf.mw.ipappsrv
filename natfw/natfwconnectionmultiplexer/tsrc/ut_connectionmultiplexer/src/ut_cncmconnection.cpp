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
#include "ut_cncmconnection.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <digia/eunit/EUnitDecorators.h>
#include <commdbconnpref.h>


//  INTERNAL INCLUDES
#include "cncmconnection.h"
#include "cncmconnectionobserverhandler.h"
#include "natfwunittestmacros.h"

const TInt KIapId = 10;

const TInt KStreamId = 1;
const TInt KConnectionId = 1;
const TInt KSessionId = 1;

const TInt KQos = 46;

// OBSERVERS
    
// From MNcmMultiplexerConnectionObserver    
void UT_CNcmConnection::ConnectionError( TUint /*aSessionId*/, TUint /*aStreamId*/,
    TUint /*aConnectionId*/, TMultiplexerConnectionNotifyType /*aNotifyType*/,
    TInt /*aError*/ )
    { 
    }
        
    
void UT_CNcmConnection::ConnectionNotify( TUint /*aStreamId*/,
    TUint /*aConnectionId*/, TMultiplexerConnectionNotifyType /*aType*/,
    TInt /*aError*/ )
    { 
    if ( iActive )
        {
        iActive = EFalse;
        CActiveScheduler::Stop();
        }
    }
    
    
void UT_CNcmConnection::GetNewFreePort( TUint /*aStreamId*/,
    TUint /*aConnectionId*/, TUint& aPort )
    {   
    aPort = 5002;
    }
        
       
                    
// CONSTRUCTION
UT_CNcmConnection* UT_CNcmConnection::NewL()
    {
    UT_CNcmConnection* self = UT_CNcmConnection::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNcmConnection* UT_CNcmConnection::NewLC()
    {
    UT_CNcmConnection* self = new( ELeave ) UT_CNcmConnection();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNcmConnection::~UT_CNcmConnection()
    {
    iTestSocket.Close();
    iTestConnection.Close();    
    iTestSocketSrv.Close();
    }

// Default constructor
UT_CNcmConnection::UT_CNcmConnection()
    {      
    iAddress.SetAddress( INET_ADDR( 127,0,0,1 ) );
    iAddress.SetPort( 5000 );
    
    iTestAddress.SetAddress( INET_ADDR( 127,0,0,1 ) );
    iTestAddress.SetPort( 5001 );
    }

// Second phase construct
void UT_CNcmConnection::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    
    User::LeaveIfError( iTestSocketSrv.Connect() );

    TCommDbConnPref pref;
    pref.SetDialogPreference( ECommDbDialogPrefDoNotPrompt );
    pref.SetIapId( KIapId );  
                                                 
    User::LeaveIfError( iTestConnection.Open( iTestSocketSrv ) );
    User::LeaveIfError( iTestConnection.Start( pref ) );
    
    User::LeaveIfError( iTestSocket.Open( iTestSocketSrv, KAfInet, KSockStream, KProtocolInetTcp, iTestConnection ));
    }

//  METHODS


void UT_CNcmConnection::SetupUdpL(  )
    {
    //iAlloc = EFalse;
    iObserverHandler = CNcmConnectionObserverHandler::NewL();
        
    User::LeaveIfError( iSocketSrv.Connect() );

    TCommDbConnPref pref;
    pref.SetDialogPreference( ECommDbDialogPrefDoNotPrompt );
    pref.SetIapId( KIapId );  
                                                 
    User::LeaveIfError( iConn.Open( iSocketSrv ) );
    User::LeaveIfError( iConn.Start( pref ) );
    
    User::LeaveIfError( iSocket.Open( iSocketSrv, KAfInet, KSockDatagram, KProtocolInetUdp ));
    
    
    iConnection = CNcmConnection::NewL( KSessionId, KStreamId, KConnectionId,
        iAddress, *this, *iObserverHandler, KQos, iSocketSrv, iConn, KProtocolInetUdp,
        EFalse );                                                        
    }


void UT_CNcmConnection::SetupTcpL(  )
    {
    //iAlloc = EFalse;
    iObserverHandler = CNcmConnectionObserverHandler::NewL();        
    User::LeaveIfError( iSocketSrv.Connect() );

    TCommDbConnPref pref;
    pref.SetDialogPreference( ECommDbDialogPrefDoNotPrompt );
    pref.SetIapId( KIapId );  
                                                 
    User::LeaveIfError( iConn.Open( iSocketSrv ) );
    User::LeaveIfError( iConn.Start( pref ) );
    
    User::LeaveIfError( iSocket.Open( iSocketSrv, KAfInet, KSockDatagram, KProtocolInetUdp ));
    
    
    iConnection = CNcmConnection::NewL( KSessionId, KStreamId, KConnectionId,
        iAddress, *this, *iObserverHandler, KQos, iSocketSrv, iConn, KProtocolInetTcp,
        EFalse );                                                      
    }

    
void UT_CNcmConnection::Teardown(  )
    {
    delete iObserverHandler;
    iObserverHandler = NULL;
    delete iConnection; 
    iConnection = NULL;
    
    iConn.Close();    
    iSocketSrv.Close();
    }


void UT_CNcmConnection::UT_CNcmConnection_NewLL(  )
    {
    CNcmConnection* connection( NULL );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( connection = CNcmConnection::NewL( KSessionId, KStreamId, KConnectionId,
        iAddress, *this, *iObserverHandler, KQos, iSocketSrv, iConn, KProtocolInetUdp,
        EFalse) ); 
        
    delete connection;
    connection = NULL;
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( connection = CNcmConnection::NewL( KSessionId, KStreamId, KConnectionId,
        iAddress, *this, *iObserverHandler, KQos, iSocketSrv, iConn, KProtocolInetUdp,
        ETrue) );
        
    delete connection;
    connection = NULL;                                                           
    }
    
    
void UT_CNcmConnection::UT_CNcmConnection_TCP_NewLL(  )
    {    
    CNcmConnection* connection( NULL );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( connection = CNcmConnection::NewL( KSessionId, KStreamId, KConnectionId,
        iAddress, *this, *iObserverHandler, KQos, iSocketSrv, iConn, KProtocolInetTcp,
        EFalse) ); 
        
    delete connection;
    connection = NULL;
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( connection = CNcmConnection::NewL( KSessionId, KStreamId, KConnectionId,
        iAddress, *this, *iObserverHandler, KQos, iSocketSrv, iConn, KProtocolInetTcp,
        ETrue) );
        
    delete connection;
    connection = NULL;                                                      
    }

    
void UT_CNcmConnection::UT_CNcmConnection_SetReceivingStateLL(  )
    {     
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnection->SetReceivingStateL( EStreamingStateActive ) );         
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnection->SetReceivingStateL( EStreamingStatePassive ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnection->SetReceivingStateL( EStreamingStateActive ) ); 
     
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnection->SetReceivingStateL( EStreamingStateActive ) );       
    }
    
void UT_CNcmConnection::UT_CNcmConnection_SetSendingStateLL(  )
    {
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnection->SetSendingStateL( INET_ADDR( 127,0,0,1 ), EStreamingStateActive ) );   
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnection->SetSendingStateL( INET_ADDR( 127,0,0,1 ), EStreamingStatePassive ) );       
    
    EUNIT_ASSERT_PANIC( iConnection->SetSendingStateL( INET_ADDR( 127,0,0,1 ), EStreamingStatePassive ), "", KErrNotReady );
    }

    
void UT_CNcmConnection::UT_CNcmConnection_OpenTcpConnectionLL()
    {
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnection->OpenTcpConnectionL( ETcpSetupPassive, iAddress ) );   
          
    TRequestStatus status;
    iTestSocket.Bind( iTestAddress );
    iTestSocket.Connect( iAddress, status );
    User::WaitForRequest( status );
 
    iActive = ETrue;
    CActiveScheduler::Start();
    }

void UT_CNcmConnection::UT_CNcmConnection_CloseTcpConnectionLL()
    {
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnection->OpenTcpConnectionL( ETcpSetupPassive, iAddress ) );      
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnection->CloseTcpConnectionL() ); 
    EUNIT_ASSERT_PANIC( iConnection->CloseTcpConnectionL(), "", KErrNotReady ); 
    }


void UT_CNcmConnection::UT_CNcmConnection_TCP_SetReceivingSendingStateLL(  )
    {
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnection->OpenTcpConnectionL( ETcpSetupPassive, iAddress ) );   
          
    TRequestStatus status;
    iTestSocket.Bind( iTestAddress );
    iTestSocket.Connect( iAddress, status );
    User::WaitForRequest( status );

    iActive = ETrue;
    CActiveScheduler::Start();
     
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnection->SetReceivingStateL( EStreamingStateActive ) );         
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnection->SetReceivingStateL( EStreamingStatePassive ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnection->SetReceivingStateL( EStreamingStateActive ) ); 

    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnection->SetSendingStateL( INET_ADDR( 127,0,0,1 ), EStreamingStateActive ) );   
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnection->SetSendingStateL( INET_ADDR( 127,0,0,1 ), EStreamingStatePassive ) );
        
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnection->SetReceivingStateL( EStreamingStateActive ) );   
    
    EUNIT_ASSERT_PANIC( iConnection->SetSendingStateL( INET_ADDR( 127,0,0,1 ), EStreamingStatePassive ), "", KErrNotReady );
    }


void UT_CNcmConnection::UT_CNcmConnection_SendL(  )
    {
    _LIT8( KUDPMessage,"UDP_Message" );
      
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnection->SetSendingStateL( iAddress, EStreamingStateActive ) );      
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnection->SetReceivingStateL( EStreamingStateActive ) );
         
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnection->Send( KUDPMessage, KAFUnspec, NULL ) );
    }
    
void UT_CNcmConnection::UT_CNcmConnection_TCP_SendL(  )
    {
    _LIT8( KUDPMessage,"UDP_Message" );   
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnection->OpenTcpConnectionL( ETcpSetupPassive, iAddress ) );   
          
    TRequestStatus status;
    iTestSocket.Bind( iTestAddress );
    iTestSocket.Connect( iAddress, status );
    User::WaitForRequest( status );
     
    iActive = ETrue;
    CActiveScheduler::Start();
   
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnection->SetSendingStateL( iAddress, EStreamingStateActive ) );      
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnection->SetReceivingStateL( EStreamingStateActive ) ); 
     
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnection->Send( KUDPMessage, KAFUnspec, NULL ) );
    }
    
   
void UT_CNcmConnection::UT_CNcmConnection_CheckSenderValidityToSendingL(  )
    {
    EUNIT_ASSERT_EQUALS( iConnection->CheckSenderValidityToSending( iTestAddress ), EFalse );
    EUNIT_ASSERT_EQUALS( iConnection->CheckSenderValidityToSending( iAddress ), EFalse );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnection->SetSendingStateL( iAddress, EStreamingStateActive ) );
        
    EUNIT_ASSERT_EQUALS( iConnection->CheckSenderValidityToSending( iTestAddress ), EFalse );      
    EUNIT_ASSERT_EQUALS( iConnection->CheckSenderValidityToSending( iAddress ), ETrue );       
    }
    

//  TEST TABLE
EUNIT_BEGIN_TEST_TABLE(
    UT_CNcmConnection,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "NewL - test1",
    "CNcmConnection",
    "NewL - test1",
    "FUNCTIONALITY",
    SetupUdpL, UT_CNcmConnection_NewLL, Teardown)
    
EUNIT_TEST(
    "NewL - test2",
    "CNcmConnection",
    "NewL - test1",
    "FUNCTIONALITY",
    SetupTcpL, UT_CNcmConnection_TCP_NewLL, Teardown)
    
EUNIT_TEST(
    "Send - test3",
    "CNcmConnection",
    "Send - test1",
    "FUNCTIONALITY",
    SetupUdpL, UT_CNcmConnection_SendL, Teardown)

EUNIT_TEST(
    "SendL - test4",
    "CNcmConnection",
    "SendL - test1",
    "FUNCTIONALITY",
    SetupTcpL, UT_CNcmConnection_TCP_SendL, Teardown)    
  

EUNIT_TEST(
    "SetReceivingStateL - test6",
    "CNcmConnection",
    "SetReceivingStateL - test6",
    "FUNCTIONALITY",
    SetupUdpL, UT_CNcmConnection_SetReceivingStateLL, Teardown)
    
EUNIT_TEST(
    "SetSendingStateL - test7",
    "CNcmConnection",
    "SetSendingStateL - test7",
    "FUNCTIONALITY",
    SetupUdpL, UT_CNcmConnection_SetSendingStateLL, Teardown)
    
EUNIT_TEST(
    "OpenTcpConnectionL - test8",
    "CNcmConnection",
    "OpenTcpConnectionL - test8",
    "FUNCTIONALITY",
    SetupTcpL, UT_CNcmConnection_OpenTcpConnectionLL, Teardown)
   
EUNIT_TEST(
    "CloseTcpConnectionL - test9",
    "CNcmConnection",
    "CloseTcpConnectionL - test9",
    "FUNCTIONALITY",
    SetupTcpL, UT_CNcmConnection_CloseTcpConnectionLL, Teardown)


EUNIT_TEST(
    "SetReceivingSendingStateL - test10",
    "CNcmConnection",
    "SetReceivingSendingStateL - test10",
    "FUNCTIONALITY",
    SetupTcpL, UT_CNcmConnection_TCP_SetReceivingSendingStateLL, Teardown)
    
   
EUNIT_TEST(
    "CheckSenderValidityToSending - test12",
    "CNcmConnection",
    "CheckSenderValidityToSending - test12",
    "FUNCTIONALITY",
    SetupUdpL, UT_CNcmConnection_CheckSenderValidityToSendingL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
