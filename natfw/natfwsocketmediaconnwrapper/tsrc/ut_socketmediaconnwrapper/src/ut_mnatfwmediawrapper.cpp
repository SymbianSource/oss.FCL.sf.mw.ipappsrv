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
#include "UT_MNATFWMediaWrapper.h"

//  EXTERNAL INCLUDES
#include <e32debug.h>
#include <es_sock.h>
#include <in_sock.h>
#include <digia/eunit/eunitmacros.h>
#include <commdbconnpref.h>

//  INTERNAL INCLUDES
#include "natfwmediawrapper.h"
#include "natfwconnectivityapidefs.h"
#include "natfwsocketmediaconnwrapper.h"
#include "mnatfwsocketmediaconnwrapper.h"
#include "natfwsockethandler.h"
#include "socketmediaconnwrappertestdefs.h"
#include "natfwunittestmacros.h"

// CONSTANTS

const TUint KStreamId = 10;
const TUint KPort = 5000;
const TUint KTcpPort = 249;
const TInt KMaxWaitTime = 100000; // 100ms

_LIT8(KData,"qwerty");
_LIT8(KData2,"qwert2");
_LIT8(KData3,"qwert3");

// CONSTRUCTION
UT_MNATFWMediaWrapper* UT_MNATFWMediaWrapper::NewL()
    {
    UT_MNATFWMediaWrapper* self = UT_MNATFWMediaWrapper::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_MNATFWMediaWrapper* UT_MNATFWMediaWrapper::NewLC()
    {
    UT_MNATFWMediaWrapper* self = new( ELeave ) UT_MNATFWMediaWrapper();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_MNATFWMediaWrapper::~UT_MNATFWMediaWrapper()
    {
    }

// Default constructor
UT_MNATFWMediaWrapper::UT_MNATFWMediaWrapper()
    {
    }

// Second phase construct
void UT_MNATFWMediaWrapper::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }


// Observer Methods
void UT_MNATFWMediaWrapper::ConnectingCompleted( const TInt aError )
    {
    iError = aError;
    iMessage = EConnectingCompleted;
    
    if ( KErrCancel != iError )
        {
        iWaitScheduler->AsyncStop();
        }   
    }
    
void UT_MNATFWMediaWrapper::SendingCompleted( const TInt aError )
    { 
    iError = aError;
    iMessage = ESendingCompleted;
    
    iWaitScheduler->AsyncStop();
    }
        
TInt UT_MNATFWMediaWrapper::ReceivedFrom( const TUint aStreamId, 
    TDes8& /*aReceiveBuffer*/ )
    {
    iStreamId = aStreamId;
    iMessage = EReceivedFrom;
    iWaitScheduler->AsyncStop();

    return KErrNone;
    }

void UT_MNATFWMediaWrapper::GetNewFreePort( TUint& aPort )
    {
    EUNIT_PRINT( _L8( "UT_MNATFWMediaWrapper::GetNewFreePort" ) );
    
    TUint port( 250 );
    aPort = port;
    }

//  TEST METHODS
void UT_MNATFWMediaWrapper::SetupL()
    {
    iWaitScheduler = new(ELeave) CActiveSchedulerWait(); 

    iMediaConnWrapper = CNATFWSocketMediaConnWrapper::NewL( 
                                 KPort, KProtocolInetUdp, KStreamId );
    
    User::LeaveIfError( iSocketSrv.Connect() );           
    iSenderAddr.SetPort( 200 );
    
    // Open an RConnection object.  Note that you must provide an RSocketServ object
    User::LeaveIfError( iConn.Open( iSocketSrv ) );
    
    TCommDbConnPref pref;
    pref.SetDialogPreference( ECommDbDialogPrefDoNotPrompt );
    pref.SetIapId( /*6*/10 );
    
    // Start an Outgoing Connection
    iConn.Start( pref, iStatus );
    User::WaitForRequest( iStatus );
    User::LeaveIfError( iStatus.Int() );    
                                                                  
    iMediaConnWrapper->RegisterMediaWrapperObserverL( this, iSocketSrv, iWrapperLocalAddr );
    iSocketMediaConnWrapper = iMediaConnWrapper;
    iMediaWrapper = iMediaConnWrapper;
    iConn.Close();
    }
    
void UT_MNATFWMediaWrapper::Setup2L()
    {
    iWaitScheduler = new(ELeave) CActiveSchedulerWait(); 
       
    iMediaConnWrapper = CNATFWSocketMediaConnWrapper::NewL( 
                                 KTcpPort, KProtocolInetTcp, KStreamId );
                                 
    User::LeaveIfError( iSocketSrv.Connect() );  
 
    // Open an RConnection object.  Note that you must provide an RSocketServ object
    User::LeaveIfError( iConn.Open( iSocketSrv ) );
    
    TCommDbConnPref pref;
    pref.SetDialogPreference( ECommDbDialogPrefDoNotPrompt );
    pref.SetIapId( /*6*/10 );
    
    // Start an Outgoing Connection
    iConn.Start( pref, iStatus );
    User::WaitForRequest( iStatus );
    User::LeaveIfError( iStatus.Int() );
                                      
    iMediaConnWrapper->RegisterMediaWrapperObserverL( this, iSocketSrv, iWrapperLocalAddr );
    iSocketMediaConnWrapper = iMediaConnWrapper;
    iMediaWrapper = iMediaConnWrapper;
    iConn.Close();
    }    

void UT_MNATFWMediaWrapper::Teardown()
    {
    iMediaWrapper->Close();
    iSocketSrv.Close();

   // delete iScheduler;
    
    delete iWaitScheduler;    
    iWaitScheduler = NULL;
    iSocketMediaConnWrapper = NULL;
    iMediaWrapper = NULL;
    }

void UT_MNATFWMediaWrapper::UT_MNATFWMediaWrapper_SetSendingStateLL()
    {
    // UDP:
    NATFW_EUNIT_ASSERT_NO_LEAVE( iSocketMediaConnWrapper->SetIncomingAddrL( iSenderAddr ) );            
    NATFW_EUNIT_ASSERT_NO_LEAVE( iSocketMediaConnWrapper->SetSendingStateL( EStreamingStateActive ) );    
    
// Deactivate sending
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iMediaWrapper->SendL( KData ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iSocketMediaConnWrapper->SetSendingStateL( EStreamingStatePassive ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iSocketMediaConnWrapper->SetSendingStateL( EStreamingStateActive ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iMediaWrapper->SendL( KData ) );
    }

void UT_MNATFWMediaWrapper::UT_MNATFWMediaWrapper_SetSendingStateL_TcpL()
    {
    // TCP:
    NATFW_EUNIT_ASSERT_NO_LEAVE( iSocketMediaConnWrapper->SetIncomingAddrL( iSenderAddr ) );             
    // Test Hack!
    iMediaConnWrapper->iHandler->iSendState = iMediaConnWrapper->iHandler->EConnected;
    // 
    iSocketMediaConnWrapper->SetSendingStateL( EStreamingStateActive );
    }

void UT_MNATFWMediaWrapper::UT_MNATFWMediaWrapper_SetReceivingStateLL()
    {
    // UDP:
    iSocketMediaConnWrapper->OutgoingAddr( iAddress );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iSocketMediaConnWrapper->SetIncomingAddrL( iSenderAddr ) );   
    NATFW_EUNIT_ASSERT_NO_LEAVE( iSocketMediaConnWrapper->SetReceivingStateL( EStreamingStateActive ) );

    NATFW_EUNIT_ASSERT_SPECIFIC_LEAVE(
        iSocketMediaConnWrapper->SetReceivingStateL( EStreamingStateActive ), KErrInUse );
      
    NATFW_EUNIT_ASSERT_NO_LEAVE( iSocketMediaConnWrapper->SetSendingStateL( EStreamingStateActive ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iMediaWrapper->SetReceivingStateForMuxWrapper( EStreamingStateActive ) ); 
    
    iMessage = EEmpty;
    RHostResolver hr;
    iData.Append( KData );  
    
    // Open Host Resolver associated with the connection
    User::LeaveIfError( hr.Open( iSocketSrv, KAfInet, KProtocolInetUdp ) );
    TNameEntry log;
    THostName name;
    User::LeaveIfError( hr.GetByName(name,log) );    
    User::LeaveIfError( iSocket.Open( iSocketSrv, KAfInet, KSockDatagram,
                                     KProtocolInetUdp ));                                

    iSocket.SetLocalPort( iSenderAddr.Port() );                                     
    log().iAddr.SetPort( iAddress.Port() );  

    iSocket.SendTo( iData, log().iAddr, 0, iStatus ); 
    User::WaitForRequest( iStatus );
    User::LeaveIfError( iStatus.Int() );
 
    iWaitScheduler->Start(); 
    
    if ( iMessage != EReceivedFrom )
        {
        EUNIT_ASSERT( EFalse );
        }                 
    EUNIT_PRINT( iReceiveBuffer );        

    iSocket.Close();
    
// Deactivate receiving
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iMediaConnWrapper->SetReceivingStateL( EStreamingStatePassive ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iSocketMediaConnWrapper->SetReceivingStateL( EStreamingStateActive ) );    
    }

void UT_MNATFWMediaWrapper::UT_MNATFWMediaWrapper_SetReceivingStateL_TcpL()
    {
    // TCP:
    NATFW_EUNIT_ASSERT_SPECIFIC_LEAVE( iSocketMediaConnWrapper->SetReceivingStateL( EStreamingStateActive ), KErrArgument );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iSocketMediaConnWrapper->SetIncomingAddrL( iSenderAddr ) );
    // Test Hack!
    iMediaConnWrapper->iHandler->iSendState = iMediaConnWrapper->iHandler->EConnected;
    //
    NATFW_EUNIT_ASSERT_NO_LEAVE( iSocketMediaConnWrapper->SetSendingStateL( EStreamingStateActive ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iSocketMediaConnWrapper->SetReceivingStateL( EStreamingStateActive ) );
    }

void UT_MNATFWMediaWrapper::UT_MNATFWMediaWrapper_SendLL(  )
    {
    // UDP:
    NATFW_EUNIT_ASSERT_SPECIFIC_LEAVE( iMediaWrapper->SendL( KData ), KErrNotReady );         
    NATFW_EUNIT_ASSERT_NO_LEAVE( iSocketMediaConnWrapper->SetIncomingAddrL( iSenderAddr ) );

    iMessage = EEmpty;
    iSocketMediaConnWrapper->SetSendingStateL( EStreamingStateActive );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iMediaWrapper->SendL( KData ) );
    
    iWaitScheduler->Start();
    if(iMessage != ESendingCompleted)
        {
        EUNIT_ASSERT( EFalse );
        }

    NATFW_EUNIT_ASSERT_NO_LEAVE( iMediaWrapper->SendL( KData ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iMediaWrapper->SendL( KData2 ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iMediaWrapper->SendL( KData3 ) );

    iWaitScheduler->Start();
    if(iMessage != ESendingCompleted)
        {
        EUNIT_ASSERT( EFalse );
        }
    iWaitScheduler->Start();    
    if(iMessage != ESendingCompleted)
        {
        EUNIT_ASSERT( EFalse );
        }
    iWaitScheduler->Start();
    if(iMessage != ESendingCompleted)
        {
        EUNIT_ASSERT( EFalse );
        }
    }
    
void UT_MNATFWMediaWrapper::UT_MNATFWMediaWrapper_SendL_TcpL(  )
    {
    // TCP:
    NATFW_EUNIT_ASSERT_NO_LEAVE( iSocketMediaConnWrapper->SetIncomingAddrL( iSenderAddr ) );
    NATFW_EUNIT_ASSERT_SPECIFIC_LEAVE( iMediaWrapper->SendL( KData ), KErrNotReady );
    
    iMessage = EEmpty;    
    // Test Hack!
    iMediaConnWrapper->iHandler->iSendState = iMediaConnWrapper->iHandler->EConnected;    
    iSocketMediaConnWrapper->SetSendingStateL( EStreamingStateActive );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iMediaWrapper->SendL( KData ) ); 
    iWaitScheduler->Start();
    if(iMessage != ESendingCompleted)
        {
        EUNIT_ASSERT( EFalse );
        }
    }

/*void UT_MNATFWMediaWrapper::UT_MNATFWMediaWrapper_OpenTcpConnectionLL( )
    {    
    // TCP-SPESIFIC:
    iMessage = EEmpty;    
    RConnection conn;
    RSocketServ srv;
    RSocket listenSocket;     

    // Open an RConnection object.  Note that you must provide an RSocketServ object
    User::LeaveIfError( srv.Connect());
    User::LeaveIfError( conn.Open( srv ) );
    
    TCommDbConnPref pref;
    pref.SetDialogPreference( ECommDbDialogPrefDoNotPrompt );
    pref.SetIapId( 10 );
    
    // Start an Outgoing Connection
    conn.Start( pref, iStatus );
    User::WaitForRequest( iStatus );
    User::LeaveIfError( iStatus.Int() ); 
      
    // Open a Socket associated with the connection   
    User::LeaveIfError( iSocket.Open( srv, KAfInet, KSockStream, KProtocolInetTcp, conn ) );
    
    iSenderAddr.SetPort( KPort );
    User::LeaveIfError( iSocket.Bind( iSenderAddr ) );
         
    EUNIT_ASSERT_NO_LEAVE( iSocketMediaConnWrapper->SetIncomingAddrL( iSenderAddr ) );
    User::LeaveIfError( iSocket.Listen( 1 ) );
    User::LeaveIfError( listenSocket.Open( srv ) );
    iSocket.Accept( listenSocket, iStatus );
            
    EUNIT_ASSERT_NO_LEAVE( iSocketMediaConnWrapper->OpenTcpConnectionL( 
        ETcpSetupActive, *this ) );
    iWaitScheduler->Start();
    if(iMessage != EConnectingCompleted)
        {
        EUNIT_ASSERT( EFalse );
        }
        
    User::WaitForRequest( iStatus );
    User::LeaveIfError( iStatus.Int() );
    //
    iMediaWrapper->SetReceivingStateForMuxWrapper( EStreamingStateActive );
    EUNIT_ASSERT_NO_LEAVE( iSocketMediaConnWrapper->SetReceivingStateL( EStreamingStateActive ) );        
    listenSocket.Send( iData, 0, iStatus );
    User::WaitForRequest( iStatus ); 
    User::LeaveIfError( iStatus.Int() );      
    
    iWaitScheduler->Start();
    if ( iMessage != EReceivedFrom )
        {
        EUNIT_ASSERT( EFalse );
        }
    EUNIT_PRINT( iReceiveBuffer );
    
    iSocketMediaConnWrapper->SetSendingStateL( EStreamingStateActive );
    EUNIT_ASSERT_NO_LEAVE( iMediaWrapper->SendL( KData ) );
    iWaitScheduler->Start();
    if(iMessage != ESendingCompleted)
        {
        EUNIT_ASSERT( EFalse );
        }    
    //
    iSocketMediaConnWrapper->CloseTcpConnection();                
    iSocket.Close();
    Teardown();
    Setup2L();
    
    User::LeaveIfError( iSocket.Open( srv, KAfInet, KSockStream, KProtocolInetTcp, conn ) );
    User::LeaveIfError( iSocket.SetOpt( KSoReuseAddr, KProtocolInetIp, 1 ) );
    
    User::LeaveIfError( iSocket.Bind( iSenderAddr ) );
    EUNIT_ASSERT_NO_LEAVE( iSocketMediaConnWrapper->SetIncomingAddrL( iSenderAddr ) );      
    EUNIT_ASSERT_NO_LEAVE( iSocketMediaConnWrapper->OpenTcpConnectionL(
        ETcpSetupPassive, *this ) );
    iSocketMediaConnWrapper->OutgoingAddr( iAddress );
    iSocket.Connect( iAddress, iStatus );
    User::WaitForRequest( iStatus );
    User::LeaveIfError( iStatus.Int() );
    
    iWaitScheduler->Start();
    if(iMessage != EConnectingCompleted)
        {
        EUNIT_ASSERT( EFalse );
        }
        
    iMediaWrapper->SetReceivingStateForMuxWrapper( EStreamingStateActive );
    EUNIT_ASSERT_NO_LEAVE( iSocketMediaConnWrapper->SetReceivingStateL( EStreamingStateActive ) );
    iSocket.Send( iData, 0, iStatus );
    User::WaitForRequest( iStatus ); 
    User::LeaveIfError( iStatus.Int() );      
    
    iWaitScheduler->Start();
    if ( iMessage != EReceivedFrom )
        {
        EUNIT_ASSERT( EFalse );
        }
    EUNIT_PRINT( iReceiveBuffer );
    
    iSocketMediaConnWrapper->SetSendingStateL( EStreamingStateActive );
    EUNIT_ASSERT_NO_LEAVE( iMediaWrapper->SendL( KData ) );
    iWaitScheduler->Start();
    if(iMessage != ESendingCompleted)
        {
        EUNIT_ASSERT( EFalse );
        }     
    
    iSocketMediaConnWrapper->CloseTcpConnection(); 
    iSocket.Close();
    /*iWaitScheduler->Start();
    if ( iMessage != EReceivedFrom )
        {
        EUNIT_ASSERT( EFalse );
        }
    EUNIT_PRINT( iReceiveBuffer );*/    
    /*conn.Close();
    srv.Close();
    }
    
void UT_MNATFWMediaWrapper::UT_MNATFWMediaWrapper_CloseTcpConnectionSetupL()
    {
    EUNIT_ASSERT_NO_LEAVE( iSocketMediaConnWrapper->SetIncomingAddrL( iSenderAddr ) );
    EUNIT_ASSERT_NO_LEAVE( iSocketMediaConnWrapper->OpenTcpConnectionL(
        ETcpSetupPassive, *this ) );
    iSocketMediaConnWrapper->CloseTcpConnection();
    
    Teardown();
    Setup2L();    
    EUNIT_ASSERT_NO_LEAVE( iSocketMediaConnWrapper->SetIncomingAddrL( iSenderAddr ) );
    EUNIT_ASSERT_NO_LEAVE( iSocketMediaConnWrapper->OpenTcpConnectionL(
        ETcpSetupActive, *this ) );
    iSocketMediaConnWrapper->CloseTcpConnection();
    }*/

void UT_MNATFWMediaWrapper::UT_MNATFWMediaWrapper_StreamIdL(  )
    {
    EUNIT_ASSERT( iMediaWrapper->StreamId() == KStreamId );
    }

void UT_MNATFWMediaWrapper::UT_MNATFWMediaWrapper_RegisterMediaWrapperObserverL(  )
    {
    NATFW_EUNIT_ASSERT_NO_LEAVE( iMediaConnWrapper->RegisterMediaWrapperObserverL( 
        this, iSocketSrv, iWrapperLocalAddr ) );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_MNATFWMediaWrapper,
    "MNATFWMediaWrapper",
    "UNIT" )

EUNIT_TEST(
    "SetSendingStateL - UDP test ",
    "MNATFWMediaWrapper",
    "SetSendingStateL",
    "FUNCTIONALITY",
    SetupL, UT_MNATFWMediaWrapper_SetSendingStateLL, Teardown)

EUNIT_TEST(
    "SetSendingStateL - TCP test ",
    "MNATFWMediaWrapper",
    "SetSendingStateL",
    "FUNCTIONALITY",
    Setup2L, UT_MNATFWMediaWrapper_SetSendingStateL_TcpL, Teardown)

EUNIT_TEST(
    "SetReceivingStateL - UDP test ",
    "MNATFWMediaWrapper",
    "SetReceivingStateL",
    "FUNCTIONALITY",
    SetupL, UT_MNATFWMediaWrapper_SetReceivingStateLL, Teardown)    
  
EUNIT_TEST(
    "SetReceivingStateL - TCP test ",
    "MNATFWMediaWrapper",
    "SetReceivingStateL",
    "FUNCTIONALITY",
    Setup2L, UT_MNATFWMediaWrapper_SetReceivingStateL_TcpL, Teardown)        

EUNIT_TEST(
    "SendL - UDP test ",
    "MNATFWMediaWrapper",
    "SendL",
    "FUNCTIONALITY",
    SetupL, UT_MNATFWMediaWrapper_SendLL, Teardown)
   
EUNIT_TEST(
    "SendL - TCP test ",
    "MNATFWMediaWrapper",
    "SendL",
    "FUNCTIONALITY",
    Setup2L, UT_MNATFWMediaWrapper_SendL_TcpL, Teardown)

/*EUNIT_TEST(
    "OpenTcpConnectionL - test ",
    "MNATFWMediaWrapper",
    "OpenTcpConnectionL",
    "FUNCTIONALITY",
    Setup2L, UT_MNATFWMediaWrapper_OpenTcpConnectionLL, Teardown)

EUNIT_TEST(
    "CloseTcpConnectionSetup - test ",
    "MNATFWMediaWrapper",
    "CloseTcpConnectionSetup",
    "FUNCTIONALITY",
    Setup2L, UT_MNATFWMediaWrapper_CloseTcpConnectionSetupL, Teardown)*/

EUNIT_TEST(
    "StreamId - test ",
    "MNATFWMediaWrapper",
    "StreamId",
    "FUNCTIONALITY",
    SetupL, UT_MNATFWMediaWrapper_StreamIdL, Teardown)

EUNIT_TEST(
    "RegisterMediaWrapperObserver - test ",
    "MNATFWMediaWrapper",
    "RegisterMediaWrapperObserver",
    "FUNCTIONALITY",
    SetupL, UT_MNATFWMediaWrapper_RegisterMediaWrapperObserverL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
