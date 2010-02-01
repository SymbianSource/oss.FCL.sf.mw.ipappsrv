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
#include "ut_cncmconnectionmultiplexer.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <in_sock.h>
#include <commdbconnpref.h>

//  INTERNAL INCLUDES
#include "cncmconnectionmultiplexer.h"
#include "natfwsocketmediaconnwrapper.h"
#include "ut_testconstants.h"
#include "mncmmultiplexerconnectionobserver.h"
#include "cncmportstore.h"
#include "ncmconnectionmultiplexerlogs.h"
#include "mncmconnectionobserver.h"
#include "cncmcallbackexecuter.h"
#include "natfwunittestmacros.h"
#include "ncmconnectionmultiplexer.hrh"

//test include
#include "es_sock.h"
#include "in_sock.h"

const TUint KMediaConnection = 1;

// CONSTRUCTION
UT_CNcmConnectionMultiplexer* UT_CNcmConnectionMultiplexer::NewL()
    {
    UT_CNcmConnectionMultiplexer* self = UT_CNcmConnectionMultiplexer::NewLC();
    CleanupStack::Pop();
    return self;
    }

UT_CNcmConnectionMultiplexer* UT_CNcmConnectionMultiplexer::NewLC()
    {
    UT_CNcmConnectionMultiplexer* self = new( ELeave ) UT_CNcmConnectionMultiplexer();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// Destructor (virtual by CBase)
UT_CNcmConnectionMultiplexer::~UT_CNcmConnectionMultiplexer()
    {
    iTestSocket.Close();
    iTestConnection.Close();    
    iTestSocketSrv.Close();
    }

// Default constructor
UT_CNcmConnectionMultiplexer::UT_CNcmConnectionMultiplexer()
    {
    iTestAddress.SetAddress( INET_ADDR( 127,0,0,1 ) );
    iTestAddress.SetPort( 5002 );
    }

// Second phase construct
void UT_CNcmConnectionMultiplexer::ConstructL()
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

// METHODS

// Observer method from MNATFWConnectionMultiplexerObserver
void UT_CNcmConnectionMultiplexer::Notify(  TUint /*aSessionId*/, TUint /*aStreamId*/,
    TNotifyType /*aType*/, TInt aError )
    {
    __CONNECTIONMULTIPLEXER( "UT_CNcmConnectionMultiplexer::Notify" )    
    
    iNotifyErr = aError;
    
    if ( iActive  )
        {
        iActive = EFalse;
        CActiveScheduler::Stop();
        }
    }
     

//MNATFWConnectionObserver
void UT_CNcmConnectionMultiplexer::ConnectionNotify( TUint /*aStream*/, TUint aConnectionId,
    TConnectionNotifyType aType, TInt aError )
    {
    __CONNECTIONMULTIPLEXER( "UT_CNcmConnectionMultiplexer::ConnectionNotifyL" )  
    
    iConnId = aConnectionId;
    iConnNotifyType = aType;
    iConnNotifyErr = aError;
    
    if ( iActive )
        {
        iActive = EFalse;
        CActiveScheduler::Stop();
        }
    }
        
//MNATFWOutgoingConnectionObserver
void UT_CNcmConnectionMultiplexer::OutgoingMessageL( TUint /*aStreamId*/, TUint /*aConnectionId*/,
        const TInetAddr& /*aDestinationAddress*/, const TDesC8& /*aMessage*/, TBool& /*aConsumed*/ )
    {
    }
        
//MNATFWIncomingConnectionObserver
void UT_CNcmConnectionMultiplexer::IncomingMessageL( TUint aStreamId, const TDesC8& /*aMessage*/,
        const TInetAddr& /*aLocalAddr*/, const TInetAddr& aFromAddr,
        const TInetAddr& /*aPeerRemoteAddr*/, TBool& aConsumed )
    {
    __CONNECTIONMULTIPLEXER_INT1( "UT_CNcmConnectionMultiplexer::IncomingMessageL - aStreamId: ",aStreamId )   
    iPacketReceived = ETrue;
    
    TInetAddr fromAddr( aFromAddr );               
    TBuf<160> buffer3;
    aFromAddr.Output( buffer3 );
    RDebug::Print( _L("Local IP Address ADDRESS: %S "), &buffer3 );

    aConsumed = ETrue;
    
    if ( iActive )
        {
        iActive = EFalse;
        CActiveScheduler::Stop();
        }  
    }
        

// MNcmMessageObserver
HBufC8* UT_CNcmConnectionMultiplexer::IncomingMessageNotify( TUint /*aStreamId*/,
    const TDesC8& /*aMessage*/, const TInetAddr& /*aLocalAddr*/,
    const TInetAddr& /*aFromAddr*/, TInetAddr& /*aPeerRemoteAddr*/ )
    {
    __CONNECTIONMULTIPLEXER( "UT_CNcmConnectionMultiplexer::IncomingMessageNotify") 
    
    return NULL;
    }


HBufC8* UT_CNcmConnectionMultiplexer::OutgoingMessageNotify( TUint /*aStreamId*/,
    TUint /*aConnectionId*/, const TInetAddr& /*aDestinationAddress*/,
    const TDesC8& /*aMessage*/ )
    {
    __CONNECTIONMULTIPLEXER( "UT_CNcmConnectionMultiplexer::OutgoingMessageNotify")  
    
    return NULL;   
    }
  
        
void UT_CNcmConnectionMultiplexer::MessageSent()
    {
    __CONNECTIONMULTIPLEXER( "UT_CNcmConnectionMultiplexer::MessageSent") 
    if ( iActive )
        {
        iActive = EFalse;
        CActiveScheduler::Stop();
        }  
    }
     
        
void UT_CNcmConnectionMultiplexer::MessageSentFailure( TInt aError )
    {
    __CONNECTIONMULTIPLEXER_INT1( "UT_CNcmConnectionMultiplexer::MessageSentFailure - aError: ",aError )   
    }
    
    
TInt UT_CNcmConnectionMultiplexer::TimerFired( TAny* /*aObject*/ )
    {
    RDebug::Print( _L("UT_CNcmConnectionMultiplexer::TimerFired") );
    CActiveScheduler::Stop();
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void UT_CNcmConnectionMultiplexer::WaitForNotify( TUint /*aStreamId*/, TUint aSessionId )
    {    
    iConnectionId = aSessionId;
    iActive = ETrue;
    CActiveScheduler::Start();
    }

            
void UT_CNcmConnectionMultiplexer::SetupL(  )
    {
    iConnMux = CNcmConnectionMultiplexer::NewL( *this );
    }

    
void UT_CNcmConnectionMultiplexer::Teardown(  )
    {
    delete iConnMux;
    }
    
    
void UT_CNcmConnectionMultiplexer::Setup2L( )
    {
    iPacketReceived = EFalse;
    iTimer = CPeriodic::NewL( CActive::EPriorityStandard );
    iDefaultConnection = 1;
    
    iConnMux = CNcmConnectionMultiplexer::NewL( *this );
    
    iSessionId = iConnMux->CreateSessionL( KIapId, KStartPort,KEndPort );
    this->WaitForNotify( 1,1 );   
    iStreamId = iConnMux->CreateStreamL( iSessionId, KQos, KProtocolInetUdp );  
    }

 void UT_CNcmConnectionMultiplexer::Teardown2(  )
    {
    delete iTimer;
    delete iConnMux;
    }

    
void UT_CNcmConnectionMultiplexer::UT_CNcmConnectionMultiplexer_NewLL(  )
    {
    CNcmConnectionMultiplexer* connMux( NULL );

    NATFW_EUNIT_ASSERT_NO_LEAVE(connMux = CNcmConnectionMultiplexer::NewL( *this ));  
    delete connMux;
    }


void UT_CNcmConnectionMultiplexer::UT_CNcmConnectionMultiplexer_CreateSessionLL(  )
    {
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->CreateSessionL( KIapId, KStartPort,KEndPort ) );
    this->WaitForNotify( 1,1 );
    User::LeaveIfError( iNotifyErr );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->CreateSessionL( 99, KStartPort,KEndPort ) );
    this->WaitForNotify( 1,1 );
    if ( iNotifyErr == KErrNoMemory )
        {
        User::Leave( iNotifyErr );
        }
    EUNIT_ASSERT_EQUALS( iNotifyErr, KErrNotFound );
    }


void UT_CNcmConnectionMultiplexer::UT_CNcmConnectionMultiplexer_RemoveSessionLL(  )
    {
    TUint sessionId( 0 );
    TUint streamId( 0 );

    NATFW_EUNIT_ASSERT_NO_LEAVE( sessionId = iConnMux->CreateSessionL( KIapId, KStartPort,KEndPort ) );
    this->WaitForNotify( 1,1 );
    User::LeaveIfError( iNotifyErr );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->RemoveSessionL( sessionId ) );
    }
    
 
     
        
void UT_CNcmConnectionMultiplexer::UT_CNcmConnectionMultiplexer_CreateStreamLL(  )
    { 
    TUint sessionId( 0 );
    TUint sessionId2( 0 );
    
    TUint streamId( 0 );
    TUint streamId2( 0 );
    TUint streamId3( 0 );
    TUint streamId4( 0 );
    TUint streamId5( 0 );

    NATFW_EUNIT_ASSERT_NO_LEAVE( sessionId = iConnMux->CreateSessionL( KIapId, KStartPort,KEndPort ) );
    
    iActive = ETrue;
    CActiveScheduler::Start();
    User::LeaveIfError( iNotifyErr );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( streamId = iConnMux->CreateStreamL( sessionId, 
                                                               KQos,
                                                               KProtocolInetUdp ) );
                                                               
    NATFW_EUNIT_ASSERT_NO_LEAVE( streamId2 = iConnMux->CreateStreamL( sessionId, 
                                                               KQos,
                                                               KProtocolInetUdp ) );
                                                              
    NATFW_EUNIT_ASSERT_NO_LEAVE( streamId3 = iConnMux->CreateStreamL( sessionId, 
                                                               KQos,
                                                               KProtocolInetUdp ) );
                                                              
    NATFW_EUNIT_ASSERT_NO_LEAVE( sessionId2 = iConnMux->CreateSessionL( KIapId, KStartPort,KEndPort ) );
    
    iActive = ETrue;
    CActiveScheduler::Start();
    User::LeaveIfError( iNotifyErr );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( streamId = iConnMux->CreateStreamL( sessionId2, 
                                                               KQos,
                                                               KProtocolInetUdp ) );
                                                       
    NATFW_EUNIT_ASSERT_NO_LEAVE( streamId = iConnMux->CreateStreamL( sessionId2, 
                                                               KQos,
                                                               KProtocolInetUdp ) );                                                            
    }


void UT_CNcmConnectionMultiplexer::UT_CNcmConnectionMultiplexer_GetStreamInfoLL(  )
    {
    TUint streamId = 100;
    TUint streamId2( 0 );
    TUint32 iapId;
    TInt qos;
    TUint protocol;
    TUint sessionId( 0 );
    

    NATFW_EUNIT_ASSERT_NO_LEAVE( iSessionId = iConnMux->CreateSessionL( KIapId, KStartPort,KEndPort ) );
    this->WaitForNotify( 1,1 );
    User::LeaveIfError( iNotifyErr );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iStreamId = iConnMux->CreateStreamL( iSessionId, 
                                                             KQos,
                                                             KProtocolInetUdp ) );
                                                            
    NATFW_EUNIT_ASSERT_NO_LEAVE( streamId2 = iConnMux->CreateStreamL( iSessionId, 
                                                             45,
                                                             KProtocolInetTcp ) ); 
                                                             
                                                             
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->GetStreamInfoL( iStreamId, iapId, qos, protocol ) );
    
    EUNIT_ASSERT( iapId == KIapId );
    EUNIT_ASSERT( qos == KQos );
    EUNIT_ASSERT( protocol == KProtocolInetUdp );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->GetStreamInfoL( streamId2, iapId, qos, protocol ) );
    
    EUNIT_ASSERT( iapId == KIapId );
    EUNIT_ASSERT( qos == 45 );
    EUNIT_ASSERT( protocol == KProtocolInetTcp );
    
    //function leaves if stream is not made before
    NATFW_EUNIT_ASSERT_SPECIFIC_LEAVE(iConnMux->GetStreamInfoL(streamId, iapId, qos, protocol),KErrNotFound);
    }


void UT_CNcmConnectionMultiplexer::UT_CNcmConnectionMultiplexer_CreateConnectionLL(  )
    {    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnectionId = iConnMux->CreateConnectionL( iStreamId, KAfInet ) );
    
    EUNIT_ASSERT( 1 == iConnectionId );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnectionId = iConnMux->CreateConnectionL( iStreamId, KAfInet ) );

    EUNIT_ASSERT( 2 == iConnectionId );
    
    EUNIT_ASSERT_LEAVE( iConnectionId = iConnMux->CreateConnectionL( iStreamId, KAfInet6 ) );
    EUNIT_ASSERT_LEAVE( iConnectionId = iConnMux->CreateConnectionL( iStreamId, KAfInet6 ) );
       
    // Wrong stream ID
    EUNIT_ASSERT_LEAVE( iConnectionId = iConnMux->CreateConnectionL( 10, KAfInet ) );
    
    // Same port range -> Get new free port
    TUint sessionId2( 0 );
    NATFW_EUNIT_ASSERT_NO_LEAVE( sessionId2 = iConnMux->CreateSessionL( KIapId, KStartPort,KEndPort ) );
    this->WaitForNotify( 1,1 ); 
    User::LeaveIfError( iNotifyErr );
    
    TUint streamId2( 0 );
    NATFW_EUNIT_ASSERT_NO_LEAVE( streamId2 = iConnMux->CreateStreamL( sessionId2, KQos, KProtocolInetUdp ) );
           
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->CreateConnectionL( streamId2, KAfInet ) );
    
    EUNIT_ASSERT_PANIC( iConnMux->CreateConnectionL( iStreamId, KAFUnspec ), "", -6 );
    }


void UT_CNcmConnectionMultiplexer::UT_CNcmConnectionMultiplexer_CreateConnectionLL2(  )
    {
    TUint connectionId;
    TUint connectionId2;
    TUint connectionId3;        
    TUint connectionId4;
            
    TInetAddr localAddress;
    TInetAddr localAddress2;
    TInetAddr localAddress3;
    TInetAddr localAddress4;
            
    NATFW_EUNIT_ASSERT_NO_LEAVE( connectionId = iConnMux->CreateConnectionL( iStreamId, KAfInet ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( localAddress = iConnMux->LocalIPAddressL( iStreamId, connectionId ) );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( connectionId2 = iConnMux->CreateConnectionL( iStreamId, localAddress ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( connectionId3 = iConnMux->CreateConnectionL( iStreamId, localAddress ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( localAddress2 = iConnMux->LocalIPAddressL( iStreamId, connectionId2 ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( localAddress3 = iConnMux->LocalIPAddressL( iStreamId, connectionId3 ) );
    
    EUNIT_ASSERT( localAddress.CmpAddr( localAddress2 ) );
    EUNIT_ASSERT( localAddress.CmpAddr( localAddress3 ) );
    EUNIT_ASSERT( localAddress2.CmpAddr( localAddress3 ) );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( connectionId4 = iConnMux->CreateConnectionL( iStreamId, KAfInet ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( localAddress4 = iConnMux->LocalIPAddressL( iStreamId, connectionId4 ) );
            
    EUNIT_ASSERT( !localAddress4.CmpAddr( localAddress ) );
    EUNIT_ASSERT( !localAddress4.CmpAddr( localAddress2 ) );
    EUNIT_ASSERT( !localAddress4.CmpAddr( localAddress3 ) );
    }
    

void UT_CNcmConnectionMultiplexer::UT_CNcmConnectionMultiplexer_SetSendingStateLL(  )
    {
    iConnMux->RegisterConnectionObserverL( iStreamId, *this );

    TInetAddr address;
    address.SetAddress( INET_ADDR( 127,0,0,1 ) );

    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnectionId = iConnMux->CreateConnectionL( iStreamId, KAfInet   ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SetSendingStateL( iStreamId, iConnectionId, address, EStreamingStateActive ) );
    
    // put callback item manually to CNcmCallBackExecuter for memory allocation tests.
    // becase CNcmConnectionMultiplexer::ConnectionNotify uses TRAP_IGNORED
    TNcmCallBack callback( iConnMux->HandleCallBack, *iConnMux, iStreamId, iConnectionId,
        EMultiplexerSendingActivated, KErrNone );
    iConnMux->iAsyncCallback->AddCallBackL( callback );

    this->WaitForNotify( 1,1 );
    User::LeaveIfError( iConnNotifyErr );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SetSendingStateL( iStreamId, iConnectionId, address, EStreamingStatePassive ) ); 
    this->WaitForNotify( 1,1 );
    User::LeaveIfError( iConnNotifyErr );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnectionId = iConnMux->CreateConnectionL( iStreamId, KAfInet ) );
    EUNIT_ASSERT_PANIC( iConnMux->SetSendingStateL( iStreamId, iConnectionId, address, EStreamingStatePassive ), "", -18 );
    }


void UT_CNcmConnectionMultiplexer::UT_CNcmConnectionMultiplexer_SetReceivingStateLL(  )
    {
    iConnMux->RegisterConnectionObserverL( iStreamId, *this );

    TInetAddr address;
    address.SetAddress( INET_ADDR( 127,0,0,1 ) );

    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnectionId = iConnMux->CreateConnectionL( iStreamId, KAfInet   ) );
    iConnMux->RegisterConnectionObserverL( iStreamId, *this );  
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SetReceivingStateL( iStreamId, iConnectionId, EStreamingStateActive ) );
    
    // put callback item manually to CNcmCallBackExecuter for memory allocation tests.
    // becase CNcmConnectionMultiplexer::ConnectionNotify uses TRAP_IGNORED
    TNcmCallBack callback( iConnMux->HandleCallBack, *iConnMux, iStreamId, iConnectionId,
        EMultiplexerReceivingActivated, KErrNone );
    iConnMux->iAsyncCallback->AddCallBackL( callback );
    
    this->WaitForNotify( 1,1 );
    User::LeaveIfError( iConnNotifyErr );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SetReceivingStateL( iStreamId, iConnectionId, EStreamingStatePassive ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnectionId = iConnMux->CreateConnectionL( iStreamId, KAfInet   ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SetReceivingStateL( iStreamId, iConnectionId, EStreamingStateActive ) ); 
    this->WaitForNotify( 1,1 );
    User::LeaveIfError( iConnNotifyErr );
    }
    
    
void UT_CNcmConnectionMultiplexer::UT_CNcmConnectionMultiplexer_SetReceivingStateL_TCP(  )
    {
    _LIT8(KUDPMessage,"UDP_Message");  
    
    TUint streamId2( 0 );
    TUint connectionId2( 0 );
    
    streamId2 = iConnMux->CreateStreamL( iSessionId, KQos, KProtocolInetTcp );
    iConnMux->RegisterConnectionObserverL( streamId2, *this );
    iConnMux->RegisterConnectionObserverL( iStreamId, *this );  
             
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnectionId = iConnMux->CreateConnectionL( iStreamId, KAfInet   ) );    
    NATFW_EUNIT_ASSERT_NO_LEAVE( connectionId2 = iConnMux->CreateConnectionL( streamId2, KAfInet   ) );  
   
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->RegisterOutgoingConnectionObserverL( iStreamId, *this ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->RegisterIncomingConnectionObserverL( iStreamId, *this ) );
 
    TInetAddr temp;
    NATFW_EUNIT_ASSERT_NO_LEAVE( temp = iConnMux->LocalIPAddressL( iStreamId, iConnectionId ) );
    
    TInetAddr fromAddr( temp );               
    TBuf<160> buffer3;
    fromAddr.Output( buffer3 );
    RDebug::Print( _L("Local IP Address ADDRESS: %S "), &buffer3 );
    
    TInetAddr temp2;
    NATFW_EUNIT_ASSERT_NO_LEAVE( temp2 = iConnMux->LocalIPAddressL( streamId2, connectionId2 ) );        

    TInetAddr fromAddr2( temp2 );               
    TBuf<160> buffer2;
    fromAddr2.Output( buffer2 );
    RDebug::Print( _L("Local IP Address ADDRESS: %S "), &buffer2 );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->OpenTcpConnectionL( streamId2, connectionId2, ETcpSetupPassive, iTestAddress ) );    

    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SetReceivingStateL( iStreamId, iConnectionId, EStreamingStateActive) ); 
    }
    
        
void UT_CNcmConnectionMultiplexer::UT_CNcmConnectionMultiplexer_LocalIPAddressLL()
    {
    TInetAddr address;
    address.SetAddress( INET_ADDR( 127,0,0,1 ) );
       
    TInetAddr temp;
    
    TUint ConnectionId2( 0 );       

    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnectionId = iConnMux->CreateConnectionL( iStreamId, KAfInet ) );     
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( ConnectionId2 = iConnMux->CreateConnectionL( iStreamId, KAfInet ) );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( temp = iConnMux->LocalIPAddressL( iStreamId, iConnectionId ) );
    
    
    EUNIT_ASSERT( 5000 == temp.Port() );
    }


void UT_CNcmConnectionMultiplexer::UT_CNcmConnectionMultiplexer_RemoveStreamLL(  )
    {
    TInetAddr address;
    address.SetAddress( INET_ADDR( 127,0,0,1 ) );
    
    TUint streamId( 0 );
    NATFW_EUNIT_ASSERT_NO_LEAVE( streamId = iConnMux->CreateStreamL( iSessionId, 
                                                               KQos,
                                                               KProtocolInetUdp ) );  

    iConnMux->RegisterConnectionObserverL( streamId, *this );
    iConnMux->RegisterConnectionObserverL( iStreamId, *this );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnectionId = iConnMux->CreateConnectionL( streamId, KAfInet   ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SetSendingStateL( streamId, iConnectionId, address, EStreamingStateActive ) );   
    this->WaitForNotify( 1,1 ); 
    User::LeaveIfError( iConnNotifyErr );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnectionId = iConnMux->CreateConnectionL( iStreamId, KAfInet   ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SetSendingStateL( iStreamId, iConnectionId, address, EStreamingStateActive ) );   
    this->WaitForNotify( 1,1 );
    User::LeaveIfError( iConnNotifyErr );
    
    EUNIT_ASSERT_LEAVE( iConnMux->RemoveStreamL( 10 ) );  
    
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->RemoveStreamL( iStreamId ) ); 
    EUNIT_ASSERT_LEAVE( iConnMux->RemoveStreamL( iStreamId ) ); 
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->RemoveStreamL( streamId ) ); 
    EUNIT_ASSERT_LEAVE( iConnMux->RemoveStreamL( streamId ) );
    }


void UT_CNcmConnectionMultiplexer::UT_CNcmConnectionMultiplexer_RemoveConnectionLL(  )
    {
    TUint streamId( 0 );
    TUint ConnectionId( 0 );
    TUint ConnectionId2( 0 );
    TInetAddr address;
    address.SetAddress( INET_ADDR( 127,0,0,1 ) );

    NATFW_EUNIT_ASSERT_NO_LEAVE( streamId = iConnMux->CreateStreamL( iSessionId, 
                                                               KQos,
                                                               KProtocolInetUdp ) );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnectionId = iConnMux->CreateConnectionL( streamId, KAfInet   ) );
                                                                           
    NATFW_EUNIT_ASSERT_NO_LEAVE( ConnectionId = iConnMux->CreateConnectionL( iStreamId, KAfInet   ) );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( ConnectionId2 = iConnMux->CreateConnectionL( iStreamId, KAfInet  ) );

    NATFW_EUNIT_ASSERT_NO_LEAVE(  iConnMux->RemoveConnectionL( streamId, iConnectionId ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE(  iConnMux->RemoveConnectionL( iStreamId, ConnectionId2 ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->RemoveConnectionL( iStreamId, ConnectionId ) );
    
    EUNIT_ASSERT_LEAVE(  iConnMux->RemoveConnectionL( 10, ConnectionId ) );
    EUNIT_ASSERT_LEAVE( iConnMux->RemoveConnectionL( iStreamId, ConnectionId ) );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( ConnectionId = iConnMux->CreateConnectionL( iStreamId, KAfInet   ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SetSendingStateL( iStreamId, ConnectionId, address, EStreamingStateActive ) );  
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->RemoveConnectionL( iStreamId, ConnectionId ) );        
    }


void UT_CNcmConnectionMultiplexer::UT_CNcmConnectionMultiplexer_RegisterIncomingConnectionObserverLL()
    {
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->RegisterIncomingConnectionObserverL( iStreamId, *this ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->RegisterIncomingConnectionObserverL( iStreamId, *this ) );
    }


void UT_CNcmConnectionMultiplexer::UT_CNcmConnectionMultiplexer_RegisterOutgoingConnectionObserverLL()
    {
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->RegisterOutgoingConnectionObserverL( iStreamId, *this ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->RegisterOutgoingConnectionObserverL( iStreamId, *this ) );
    }


void UT_CNcmConnectionMultiplexer::UT_CNcmConnectionMultiplexer_UnregisterIncomingConnectionObserverLL()
    {
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->UnregisterIncomingConnectionObserverL( iStreamId, *this ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->RegisterIncomingConnectionObserverL( iStreamId, *this ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->UnregisterIncomingConnectionObserverL( iStreamId, *this ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->UnregisterIncomingConnectionObserverL( iStreamId, *this ) );
    }


void UT_CNcmConnectionMultiplexer::UT_CNcmConnectionMultiplexer_UnregisterOutgoingConnectionObserverLL()
    {
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->UnregisterOutgoingConnectionObserverL( iStreamId, *this ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->RegisterOutgoingConnectionObserverL( iStreamId, *this ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->UnregisterOutgoingConnectionObserverL( iStreamId, *this ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->UnregisterOutgoingConnectionObserverL( iStreamId, *this ) );
    }


void UT_CNcmConnectionMultiplexer::UT_CNcmConnectionMultiplexer_RegisterConnectionObserverLL()
    {
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->RegisterConnectionObserverL( iStreamId, *this ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->RegisterConnectionObserverL( iStreamId, *this ) );
    }


void UT_CNcmConnectionMultiplexer::UT_CNcmConnectionMultiplexer_UnregisterConnectionObserverLL()
    {
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->UnregisterConnectionObserverL( iStreamId, *this ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->RegisterConnectionObserverL( iStreamId, *this ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->UnregisterConnectionObserverL( iStreamId, *this ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->UnregisterConnectionObserverL( iStreamId, *this ) );
    }


void UT_CNcmConnectionMultiplexer::UT_CNcmConnectionMultiplexer_RegisterMessageObserverLL()
    {
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->RegisterMessageObserverL( iStreamId, *this ) );
    EUNIT_ASSERT_PANIC( iConnMux->RegisterMessageObserverL( iStreamId, *this), "", -18 ); 
    }


void UT_CNcmConnectionMultiplexer::UT_CNcmConnectionMultiplexer_UnregisterMessageObserverL()
    {   
    EUNIT_ASSERT_PANIC( iConnMux->UnregisterMessageObserverL( iStreamId, *this), "", -1 );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->RegisterMessageObserverL( iStreamId, *this ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->UnregisterMessageObserverL( iStreamId, *this ) );
    EUNIT_ASSERT_PANIC( iConnMux->UnregisterMessageObserverL( iStreamId, *this ),"", -1 );
    EUNIT_ASSERT_PANIC( iConnMux->UnregisterMessageObserverL( iStreamId, *this ),"", -11 );
    }
    
        
void UT_CNcmConnectionMultiplexer::UT_CNcmConnectionMultiplexer_RegisterMediaWrapperLL(  )
    {
    CNATFWSocketMediaConnWrapper* mediaWrapper = CNATFWSocketMediaConnWrapper::NewL(
        KPort, KProtocolInetUdp, iStreamId );
 
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->RegisterMediaWrapperL( mediaWrapper ) );            
    }


void UT_CNcmConnectionMultiplexer::UT_CNcmConnectionMultiplexer_GetSessionInfoLL(  )
    {
    RSocketServ* socketServer;
    TName name;

    NATFW_EUNIT_ASSERT_NO_LEAVE( socketServer = iConnMux->GetSessionInfoL( iSessionId, name ) );
    socketServer = NULL;
    }


void UT_CNcmConnectionMultiplexer::UT_CNcmConnectionMultiplexer_PortStoreL(  )
    {    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->PortStoreL( iSessionId ).Port() );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->PortStoreL( iSessionId ).Port() );    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->PortStoreL( iSessionId ).FreePort( 20 ) );    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->PortStoreL( iSessionId ).FreePort( 0 ) );
    }

void UT_CNcmConnectionMultiplexer::UT_CNcmConnectionMultiplexer_ActivateStreamReceivingLL(  )
    {
    TUint ConnectionId( 0 );

    NATFW_EUNIT_ASSERT_NO_LEAVE( iSessionId = iConnMux->CreateSessionL( KIapId, 5000, 5010 ) );
    this->WaitForNotify( 1,1 );
    User::LeaveIfError( iNotifyErr );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iStreamId = iConnMux->CreateStreamL( iSessionId, KQos, KProtocolInetUdp ) );  
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( ConnectionId = iConnMux->CreateConnectionL( iStreamId, KAfInet   ) );
        
    CNATFWSocketMediaConnWrapper* mediaWrapper = CNATFWSocketMediaConnWrapper::NewL(
        KPort, KProtocolInetUdp, iStreamId );
            
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->RegisterMediaWrapperL( mediaWrapper ) );
    mediaWrapper->SetIncomingAddrL( KInetAddrLoop );
         
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SetReceivingStateL( iStreamId, ConnectionId, EStreamingStateActive) );
    }


void UT_CNcmConnectionMultiplexer::UT_CNcmConnectionMultiplexer_SetReceivingStateForMediaLL()
    {
    TUint connectionId( 0 );

    NATFW_EUNIT_ASSERT_NO_LEAVE( connectionId = iConnMux->CreateConnectionL( iStreamId, KAfInet   ) );
    
    CNATFWSocketMediaConnWrapper* mediaWrapper = CNATFWSocketMediaConnWrapper::NewL(
        KPort, KProtocolInetUdp, iStreamId );
        
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->RegisterMediaWrapperL( mediaWrapper ) );
    mediaWrapper->SetIncomingAddrL( KInetAddrLoop );
     
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SetReceivingStateForMediaL( iStreamId, connectionId, EStreamingStateActive) );
    
    this->WaitForNotify( 1,1 );
    User::LeaveIfError( iNotifyErr );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SetReceivingStateForMediaL( iStreamId, connectionId, EStreamingStatePassive) );
    
    this->WaitForNotify( 1,1 );
    User::LeaveIfError( iNotifyErr );
    }
    
    
void UT_CNcmConnectionMultiplexer::UT_CNcmConnectionMultiplexer_ActivateStreamSendingLL(  )
    {
    
    TUint ConnectionId( 0 );
    TInetAddr address;
    address.SetAddress( INET_ADDR( 127,0,0,1 ) );
    
    iSessionId = iConnMux->CreateSessionL( KIapId, 5000, 5010 );
    this->WaitForNotify( 1,1 );
    User::LeaveIfError( iNotifyErr );
    
    iStreamId = iConnMux->CreateStreamL( iSessionId, KQos, KProtocolInetUdp );  
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( ConnectionId = iConnMux->CreateConnectionL( iStreamId, KAfInet  ) );
    
    CNATFWSocketMediaConnWrapper* mediaWrapper = CNATFWSocketMediaConnWrapper::NewL(
        KPort, KProtocolInetUdp, iStreamId );
        
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->RegisterMediaWrapperL( mediaWrapper ) );

    EUNIT_ASSERT_PANIC( iConnMux->RegisterMediaWrapperL( mediaWrapper ), "", -11 );
         
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SetSendingStateL( iStreamId, 1, address, EStreamingStateActive) );
    }


void UT_CNcmConnectionMultiplexer::UT_CNcmConnectionMultiplexer_SetSendingStateForMediaLL()
    {
    TUint connectionId( 0 );

    NATFW_EUNIT_ASSERT_NO_LEAVE( connectionId = iConnMux->CreateConnectionL( iStreamId, KAfInet   ) );
    
    CNATFWSocketMediaConnWrapper* mediaWrapper = CNATFWSocketMediaConnWrapper::NewL(
        KPort, KProtocolInetUdp, iStreamId );
        
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->RegisterMediaWrapperL( mediaWrapper ) );
    mediaWrapper->SetIncomingAddrL( KInetAddrLoop );
     
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SetSendingStateForMediaL( iStreamId, connectionId, EStreamingStateActive) );
    
    this->WaitForNotify( 1,1 );
    User::LeaveIfError( iNotifyErr );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SetSendingStateForMediaL( iStreamId, connectionId, EStreamingStatePassive) );
    
    this->WaitForNotify( 1,1 );
    User::LeaveIfError( iNotifyErr );
    }
    
void UT_CNcmConnectionMultiplexer::UT_CNcmConnectionMultiplexer_SendLL(  )
    {
     NATFW_EUNIT_ASSERT_NO_LEAVE( iConnectionId = iConnMux->CreateConnectionL( iStreamId, KAfInet  ) );
     
     NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->RegisterConnectionObserverL( iStreamId, *this ) );
     NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->RegisterOutgoingConnectionObserverL( iStreamId, *this ) );
    
    _LIT8(KUDPMessage,"UDP_Message");        
    
    TInetAddr temp;
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( temp = iConnMux->LocalIPAddressL( iStreamId, iConnectionId ) );

    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SetSendingStateL( iStreamId,iConnectionId, temp, EStreamingStateActive ) );      
    
    // put callback item manually to CNcmCallBackExecuter for memory allocation tests.
    // becase CNcmConnectionMultiplexer::ConnectionNotify uses TRAP_IGNORED
    TNcmCallBack callback( iConnMux->HandleCallBack, *iConnMux, iStreamId, iConnectionId,
        EMultiplexerSendingActivated, KErrNone );
    iConnMux->iAsyncCallback->AddCallBackL( callback );

    this->WaitForNotify( 1,1 );
    User::LeaveIfError( iConnNotifyErr );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->RegisterIncomingConnectionObserverL( iStreamId, *this ) );
    

    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SendL( iStreamId, iConnectionId, KUDPMessage, NULL ));
    User::LeaveIfError( iConnNotifyErr );
    
    EUNIT_ASSERT_LEAVE( iConnMux->SendL( iStreamId, 0, KUDPMessage, temp ) );
    User::LeaveIfError( iConnNotifyErr );
    
    EUNIT_ASSERT_LEAVE( iConnMux->SendL( iStreamId, 0, KUDPMessage, KInetAddrLoop ) );
    User::LeaveIfError( iConnNotifyErr );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SendL( iStreamId, iConnectionId, KUDPMessage, this ));
    User::LeaveIfError( iConnNotifyErr );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SendL( iStreamId, iConnectionId, KUDPMessage, this ));
    User::LeaveIfError( iConnNotifyErr );
            
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SendL( iStreamId, iConnectionId, KUDPMessage, this ));
    User::LeaveIfError( iConnNotifyErr );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SendL( iStreamId, iConnectionId, KUDPMessage, this ));
    User::LeaveIfError( iConnNotifyErr );
    }


void UT_CNcmConnectionMultiplexer::UT_CNcmConnectionMultiplexer_SendLL2(  )
    {
    _LIT8(KUDPMessage,"UDP_Message");          
    TUint streamId2( 0 );
    TUint ConnectionId2( 0 );
    
    streamId2 = iConnMux->CreateStreamL( iSessionId, KQos, KProtocolInetUdp );
    iConnMux->RegisterConnectionObserverL( streamId2, *this );
    iConnMux->RegisterConnectionObserverL( iStreamId, *this );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnectionId = iConnMux->CreateConnectionL( iStreamId, KAfInet   ) );    
    NATFW_EUNIT_ASSERT_NO_LEAVE( ConnectionId2 = iConnMux->CreateConnectionL( streamId2, KAfInet   ) );  
   
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->RegisterOutgoingConnectionObserverL( iStreamId, *this ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->RegisterIncomingConnectionObserverL( iStreamId, *this ) );
 
    TInetAddr temp;
    NATFW_EUNIT_ASSERT_NO_LEAVE( temp = iConnMux->LocalIPAddressL( iStreamId, iConnectionId ) );
    
    TInetAddr fromAddr( temp );               
    TBuf<160> buffer3;
    fromAddr.Output( buffer3 );
    RDebug::Print( _L("Local IP Address ADDRESS: %S "), &buffer3 );
    
    TInetAddr temp2;
    NATFW_EUNIT_ASSERT_NO_LEAVE( temp2 = iConnMux->LocalIPAddressL( streamId2, iConnectionId ) );        

    TInetAddr fromAddr2( temp2 );               
    TBuf<160> buffer2;
    fromAddr2.Output( buffer2 );
    RDebug::Print( _L("Local IP Address ADDRESS: %S "), &buffer2 );
    
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SetSendingStateL( iStreamId, iConnectionId, fromAddr2, EStreamingStateActive) );
    this->WaitForNotify( 1,1 );
    User::LeaveIfError( iConnNotifyErr );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SetSendingStateL( streamId2, ConnectionId2, fromAddr, EStreamingStateActive ) ); 
    
    this->WaitForNotify( iStreamId, iConnectionId );
    User::LeaveIfError( iConnNotifyErr );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SendL( iStreamId, iConnectionId, KUDPMessage, NULL ) );
    User::LeaveIfError( iConnNotifyErr );

    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SendL( iStreamId, iConnectionId, KUDPMessage, this ) );
    User::LeaveIfError( iConnNotifyErr );
    }


void UT_CNcmConnectionMultiplexer::UT_CNcmConnectionMultiplexer_SendLL3(  )
    {
    _LIT8(KUDPMessage,"UDP_111111111111111111111111111111111111111111111113456789084");       
    _LIT8(KUDPMessage2,"dfdtrretnuynyumuui67m6m5mumurd4554y654mi,ui,y88767867566577,");
    _LIT8(KUDPMessage3,"76767,56,757,e7nrt7rn64e5gytr6456n54656456n45en564n5455n6n54");
    
    TUint streamId2( 0 );
    TUint ConnectionId2( 0 );
    
    streamId2 = iConnMux->CreateStreamL( iSessionId, KQos, KProtocolInetUdp );
    iConnMux->RegisterConnectionObserverL( streamId2, *this );
    iConnMux->RegisterConnectionObserverL( iStreamId, *this );           
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnectionId = iConnMux->CreateConnectionL( iStreamId, KAfInet   ) );    
    NATFW_EUNIT_ASSERT_NO_LEAVE( ConnectionId2 = iConnMux->CreateConnectionL( streamId2, KAfInet   ) );  
   
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->RegisterOutgoingConnectionObserverL( iStreamId, *this ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->RegisterIncomingConnectionObserverL( iStreamId, *this ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->RegisterOutgoingConnectionObserverL( streamId2, *this ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->RegisterIncomingConnectionObserverL( streamId2, *this ) );
    
    TInetAddr temp;
    NATFW_EUNIT_ASSERT_NO_LEAVE( temp = iConnMux->LocalIPAddressL( iStreamId, iConnectionId ) );
    
    TInetAddr fromAddr( temp );               
    TBuf<160> buffer3;
    fromAddr.Output( buffer3 );
    RDebug::Print( _L("Local IP Address ADDRESS: %S "), &buffer3 );
    
    TInetAddr temp2;
    NATFW_EUNIT_ASSERT_NO_LEAVE( temp2 = iConnMux->LocalIPAddressL( streamId2, ConnectionId2 ) );        

    TInetAddr fromAddr2( temp2 );               
    TBuf<160> buffer2;
    fromAddr2.Output( buffer2 );
    RDebug::Print( _L("Local IP Address ADDRESS: %S "), &buffer2 );
    
    CNATFWSocketMediaConnWrapper* mediaWrapper = CNATFWSocketMediaConnWrapper::NewL(
        5030, KProtocolInetUdp, iStreamId );
        
   
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->RegisterMediaWrapperL( mediaWrapper ) );
    
    CNATFWSocketMediaConnWrapper* mediaWrapper2 = CNATFWSocketMediaConnWrapper::NewL(
        5020, KProtocolInetUdp, streamId2 );

    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->RegisterMediaWrapperL( mediaWrapper2 ) ); 
    mediaWrapper->SetIncomingAddrL( KInetAddrLoop ); 
    mediaWrapper2->SetIncomingAddrL( KInetAddrLoop ); 
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SetSendingStateL( streamId2, KMediaConnection, fromAddr, EStreamingStateActive ) ); 
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SetReceivingStateL( iStreamId, KMediaConnection, EStreamingStateActive ) );
    
    this->WaitForNotify( streamId2, KMediaConnection );
    User::LeaveIfError( iConnNotifyErr );
    this->WaitForNotify( iStreamId, KMediaConnection );
    User::LeaveIfError( iConnNotifyErr );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SendL( streamId2, KMediaConnection, KUDPMessage, NULL ) );
    this->WaitForNotify( streamId2, KMediaConnection );
    User::LeaveIfError( iConnNotifyErr );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SendL( streamId2, KMediaConnection, KUDPMessage2, NULL ) );
    this->WaitForNotify( streamId2, KMediaConnection );
    User::LeaveIfError( iConnNotifyErr );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SendL( streamId2, KMediaConnection, KUDPMessage3, NULL ) );
    this->WaitForNotify( streamId2, KMediaConnection );
    User::LeaveIfError( iConnNotifyErr );
    
    // NEXT HOP Address
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SendL( streamId2, KUDPMessage, fromAddr, KInetAddrLoop ) );
    User::LeaveIfError( iConnNotifyErr );
    }
    

void UT_CNcmConnectionMultiplexer::UT_CNcmConnectionMultiplexer_SendLL4(  )
    {
    
    _LIT8(KUDPMessage,"UDP_Message");          
    TUint streamId2( 0 );
    TUint ConnectionId2( 0 );
    
    streamId2 = iConnMux->CreateStreamL( iSessionId, KQos, KProtocolInetUdp );
    iConnMux->RegisterConnectionObserverL( streamId2, *this );
    iConnMux->RegisterConnectionObserverL( iStreamId, *this );           
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnectionId = iConnMux->CreateConnectionL( iStreamId, KAfInet   ) );    
    NATFW_EUNIT_ASSERT_NO_LEAVE( ConnectionId2 = iConnMux->CreateConnectionL( streamId2, KAfInet   ) );  
   
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->RegisterOutgoingConnectionObserverL( iStreamId, *this ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->RegisterIncomingConnectionObserverL( iStreamId, *this ) );
 
    TInetAddr temp;
    NATFW_EUNIT_ASSERT_NO_LEAVE( temp = iConnMux->LocalIPAddressL( iStreamId, iConnectionId ) );
    
    TInetAddr fromAddr( temp );               
    TBuf<160> buffer3;
    fromAddr.Output( buffer3 );
    RDebug::Print( _L("Local IP Address ADDRESS: %S "), &buffer3 );
    
    TInetAddr temp2;
    NATFW_EUNIT_ASSERT_NO_LEAVE( temp2 = iConnMux->LocalIPAddressL( streamId2, ConnectionId2 ) );        

    TInetAddr fromAddr2( temp2 );               
    TBuf<160> buffer2;
    fromAddr2.Output( buffer2 );
    RDebug::Print( _L("Local IP Address ADDRESS: %S "), &buffer2 );
    
    
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SetSendingStateL( iStreamId, iConnectionId, fromAddr2, EStreamingStateActive) );
    this->WaitForNotify( 1,1 );
    User::LeaveIfError( iConnNotifyErr );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SetReceivingStateL( streamId2, ConnectionId2, EStreamingStateActive ) );     
    this->WaitForNotify( iStreamId, iConnectionId );
    User::LeaveIfError( iConnNotifyErr );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SendL( iStreamId, iConnectionId, KUDPMessage, ETrue ) );
    User::LeaveIfError( iConnNotifyErr );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SendL( iStreamId, iConnectionId, KUDPMessage, EFalse ) );
    User::LeaveIfError( iConnNotifyErr );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SendL( iStreamId, iConnectionId, KUDPMessage ) );
    User::LeaveIfError( iConnNotifyErr );
    }


void UT_CNcmConnectionMultiplexer::UT_CNcmConnectionMultiplexer_CancelMessageSendL(  )
    {
    _LIT8(KUDPMessage,"UDP_Message");          
    TUint streamId2( 0 );
    TUint ConnectionId2( 0 );
    
    streamId2 = iConnMux->CreateStreamL( iSessionId, KQos, KProtocolInetUdp );
    iConnMux->RegisterConnectionObserverL( streamId2, *this );
    iConnMux->RegisterConnectionObserverL( iStreamId, *this );           
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnectionId = iConnMux->CreateConnectionL( iStreamId, KAfInet   ) );    
    NATFW_EUNIT_ASSERT_NO_LEAVE( ConnectionId2 = iConnMux->CreateConnectionL( streamId2, KAfInet   ) );  
   
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->RegisterOutgoingConnectionObserverL( iStreamId, *this ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->RegisterIncomingConnectionObserverL( iStreamId, *this ) );
 
    TInetAddr temp;
    NATFW_EUNIT_ASSERT_NO_LEAVE( temp = iConnMux->LocalIPAddressL( iStreamId, iConnectionId ) );
    
    TInetAddr fromAddr( temp );               
    TBuf<160> buffer3;
    fromAddr.Output( buffer3 );
    RDebug::Print( _L("Local IP Address ADDRESS: %S "), &buffer3 );
    
    TInetAddr temp2;
    NATFW_EUNIT_ASSERT_NO_LEAVE( temp2 = iConnMux->LocalIPAddressL( streamId2, ConnectionId2 ) );        

    TInetAddr fromAddr2( temp2 );               
    TBuf<160> buffer2;
    fromAddr2.Output( buffer2 );
    RDebug::Print( _L("Local IP Address ADDRESS: %S "), &buffer2 );
    
     
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SetSendingStateL( iStreamId, iConnectionId, fromAddr2, EStreamingStateActive) );
    this->WaitForNotify( 1,1 );   
    User::LeaveIfError( iConnNotifyErr );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SendL( iStreamId, iConnectionId, KUDPMessage, this ) ); 
    User::LeaveIfError( iConnNotifyErr );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->CancelMessageSend( iStreamId, iConnectionId, this ) );   
    } 
    

void UT_CNcmConnectionMultiplexer::UT_CNcmConnectionMultiplexer_StreamByIdL(  )
    {
    TUint streamId2( 0 );   
    NATFW_EUNIT_ASSERT_NO_LEAVE( streamId2 = iConnMux->CreateStreamL( iSessionId, KQos, KProtocolInetUdp ) );

    CNcmStream* stream( NULL );    
    NATFW_EUNIT_ASSERT_NO_LEAVE( stream = iConnMux->StreamById( streamId2 ) );
    EUNIT_ASSERT( stream );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( stream = iConnMux->StreamById( 45 ) );
    EUNIT_ASSERT( NULL == stream )
    } 
 
             
void UT_CNcmConnectionMultiplexer::UT_CNcmConnectionMultiplexer_MultiplexerErrorL(  )
    {
    }


void UT_CNcmConnectionMultiplexer::UT_CNcmConnectionMultiplexer_ConnectionNotifyL(  )
    {
    }


void UT_CNcmConnectionMultiplexer::UT_CNcmConnectionMultiplexer_OpenTcpConnectionLL(  )
    {    
    TUint streamId2( 0 );
    TUint ConnectionId2( 0 );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( streamId2 = iConnMux->CreateStreamL( iSessionId, KQos, KProtocolInetTcp ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->RegisterConnectionObserverL( streamId2, *this ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->RegisterConnectionObserverL( iStreamId, *this ) );  
             
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnectionId = iConnMux->CreateConnectionL( iStreamId, KAfInet   ) );    
    NATFW_EUNIT_ASSERT_NO_LEAVE( ConnectionId2 = iConnMux->CreateConnectionL( streamId2, KAfInet   ) );  
   
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->RegisterOutgoingConnectionObserverL( iStreamId, *this ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->RegisterIncomingConnectionObserverL( iStreamId, *this ) );
 
    TInetAddr temp;
    NATFW_EUNIT_ASSERT_NO_LEAVE( temp = iConnMux->LocalIPAddressL( iStreamId, iConnectionId ) );

    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->CreateConnectionL( iStreamId, temp ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->CreateConnectionL( iStreamId, temp ) );
        
    TInetAddr fromAddr( temp );               
    TBuf<160> buffer3;
    fromAddr.Output( buffer3 );
    RDebug::Print( _L("Local IP Address ADDRESS: %S "), &buffer3 );
    
    TInetAddr temp2;
    NATFW_EUNIT_ASSERT_NO_LEAVE( temp2 = iConnMux->LocalIPAddressL( streamId2, ConnectionId2 ) );        

    TInetAddr fromAddr2( temp2 );               
    TBuf<160> buffer2;
    fromAddr2.Output( buffer2 );
    RDebug::Print( _L("Local IP Address ADDRESS: %S "), &buffer2 );
          
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->OpenTcpConnectionL( streamId2, ConnectionId2, ETcpSetupPassive, iTestAddress ) );    
    }

    
void UT_CNcmConnectionMultiplexer::UT_CNcmConnectionMultiplexer_CloseTcpConnectionL(  )
    {    
    TUint streamId2( 0 );
    TUint ConnectionId2( 0 );
        
    streamId2 = iConnMux->CreateStreamL( iSessionId, KQos, KProtocolInetTcp );
    iConnMux->RegisterConnectionObserverL( streamId2, *this );
    iConnMux->RegisterConnectionObserverL( iStreamId, *this );  
             
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnectionId = iConnMux->CreateConnectionL( iStreamId, KAfInet   ) );    
    NATFW_EUNIT_ASSERT_NO_LEAVE( ConnectionId2 = iConnMux->CreateConnectionL( streamId2, KAfInet   ) );  
   
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->RegisterOutgoingConnectionObserverL( iStreamId, *this ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->RegisterIncomingConnectionObserverL( iStreamId, *this ) );
 
    TInetAddr temp;
    NATFW_EUNIT_ASSERT_NO_LEAVE( temp = iConnMux->LocalIPAddressL( iStreamId, iConnectionId ) );

    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->CreateConnectionL( iStreamId, temp ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->CreateConnectionL( iStreamId, temp ) );
        
    TInetAddr fromAddr( temp );               
    TBuf<160> buffer3;
    fromAddr.Output( buffer3 );
    RDebug::Print( _L("Local IP Address ADDRESS: %S "), &buffer3 );
    
    TInetAddr temp2;
    NATFW_EUNIT_ASSERT_NO_LEAVE( temp2 = iConnMux->LocalIPAddressL( streamId2, ConnectionId2 ) );        

    TInetAddr fromAddr2( temp2 );               
    TBuf<160> buffer2;
    fromAddr2.Output( buffer2 );
    RDebug::Print( _L("Local IP Address ADDRESS: %S "), &buffer2 );
          
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->OpenTcpConnectionL( streamId2, KMediaConnection, ETcpSetupPassive, INET_ADDR( 127,0,0,1 ) ) );                   
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->CloseTcpConnection( streamId2, KMediaConnection ) );                
    EUNIT_ASSERT_PANIC( iConnMux->CloseTcpConnection( streamId2, KMediaConnection ), "", KErrNotReady );
    }
    
    
void UT_CNcmConnectionMultiplexer::UT_CNcmConnectionMultiplexer_ResolveDestinationAddressLL(  )
    {
    TInetAddr resultAddress;
    const TUint destinationPort( 3478 );
    _LIT8( KAddress,"www.nokia.com"); 
    _LIT8( KAddress2,"irkki.com");
    
    // Wrong stream ID
    EUNIT_ASSERT_LEAVE( iConnMux->ResolveDestinationAddressL(
        3, KAddress, destinationPort, resultAddress ) );

    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->ResolveDestinationAddressL(
        iStreamId, KAddress, destinationPort, resultAddress ) );
        
    EUNIT_ASSERT_LEAVE( iConnMux->ResolveDestinationAddressL(
        iStreamId, KAddress2, destinationPort, resultAddress ) );
        
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->ResolveDestinationAddressL(
        iStreamId, KAddress, destinationPort, resultAddress ) );
    } 
 
 
 void UT_CNcmConnectionMultiplexer::UT_CNcmConnectionMultiplexer_SetAcceptedFromAddressLL(  )
    {   
    _LIT8(KUDPMessage,"UDP_111111111111111111111111111111111111111111111113456789084");       
    _LIT8(KUDPMessage2,"dfdtrretnuynyumuui67m6m5mumurd4554y654mi,ui,y88767867566577,");
    _LIT8(KUDPMessage3,"76767,56,757,e7nrt7rn64e5gytr6456n54656456n45en564n5455n6n54");

    TUint streamId2( 0 );
    TUint connectionId2( 0 );
    
    TUint connectionId3( 0 );
    TUint streamId3 = iConnMux->CreateStreamL( iSessionId, KQos, KProtocolInetUdp );
    NATFW_EUNIT_ASSERT_NO_LEAVE( connectionId3 = iConnMux->CreateConnectionL( streamId3, KAfInet ) );
    TInetAddr localAddress3;
    NATFW_EUNIT_ASSERT_NO_LEAVE( localAddress3 = iConnMux->LocalIPAddressL( streamId3, connectionId3 ) );
    
     
    streamId2 = iConnMux->CreateStreamL( iSessionId, KQos, KProtocolInetUdp );
    
    iConnMux->RegisterConnectionObserverL( streamId2, *this );
    iConnMux->RegisterConnectionObserverL( iStreamId, *this );           
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnectionId = iConnMux->CreateConnectionL( iStreamId, KAfInet ) );    
    NATFW_EUNIT_ASSERT_NO_LEAVE( connectionId2 = iConnMux->CreateConnectionL( streamId2, KAfInet ) );  
   
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->RegisterOutgoingConnectionObserverL( iStreamId, *this ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->RegisterIncomingConnectionObserverL( iStreamId, *this ) );
 
    TInetAddr temp;
    NATFW_EUNIT_ASSERT_NO_LEAVE( temp = iConnMux->LocalIPAddressL( iStreamId, iConnectionId ) );
    
    TInetAddr fromAddr( temp );               
    TBuf<160> buffer3;
    fromAddr.Output( buffer3 );
    RDebug::Print( _L("Local IP Address ADDRESS: %S "), &buffer3 );
    
    TInetAddr localAddress2;
    NATFW_EUNIT_ASSERT_NO_LEAVE( localAddress2 = iConnMux->LocalIPAddressL( streamId2, connectionId2 ) );        

    TInetAddr fromAddr2( localAddress2 );               
    TBuf<160> buffer2;
    fromAddr2.Output( buffer2 );
    RDebug::Print( _L("Local IP Address ADDRESS: %S "), &buffer2 );
    
    CNATFWSocketMediaConnWrapper* mediaWrapper = CNATFWSocketMediaConnWrapper::NewL(
        5030, KProtocolInetUdp, iStreamId );
   
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->RegisterMediaWrapperL( mediaWrapper ) );
    
    CNATFWSocketMediaConnWrapper* mediaWrapper2 = CNATFWSocketMediaConnWrapper::NewL(
        5020, KProtocolInetUdp, streamId2 );

    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->RegisterMediaWrapperL( mediaWrapper2 ) ); 
    mediaWrapper->SetIncomingAddrL( KInetAddrLoop ); 
    mediaWrapper2->SetIncomingAddrL( KInetAddrLoop ); 
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SetSendingStateL( streamId2, KMediaConnection, fromAddr, EStreamingStateActive ) ); 
    // put callback item manually to CNcmCallBackExecuter for memory allocation tests.
    // becase CNcmConnectionMultiplexer::ConnectionNotify uses TRAP_IGNORED
    TNcmCallBack ncmCallback( iConnMux->HandleCallBack, *iConnMux, iStreamId, iConnectionId,
        EMultiplexerSendingActivated, KErrNone );
    iConnMux->iAsyncCallback->AddCallBackL( ncmCallback );
    
    this->WaitForNotify( streamId2, KMediaConnection );
    User::LeaveIfError( iConnNotifyErr );

    /** Testing starts **/
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SetAcceptedFromAddressL( iStreamId, iConnectionId, localAddress2 ) );
    
    TCallBack callBack( UT_CNcmConnectionMultiplexer::TimerFired, this );
    iTimer->Start( 1000000, 100000000, callBack );
    CActiveScheduler::Start();
    iTimer->Cancel();

    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SetReceivingStateL( iStreamId, KMediaConnection, EStreamingStateActive) );
    this->WaitForNotify( iStreamId,KMediaConnection ); 
    User::LeaveIfError( iConnNotifyErr );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SendL( streamId2, KMediaConnection, KUDPMessage, NULL ) );
    User::LeaveIfError( iConnNotifyErr );
    this->WaitForNotify( streamId2, KMediaConnection );
    User::LeaveIfError( iConnNotifyErr );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SetSendingStateL( streamId3, connectionId3, fromAddr, EStreamingStateActive ) ); 
    /** packet must be filtered ***/
    iPacketReceived = EFalse;
    _LIT8(KUDPMessage4,"UDP_fgdfg9898fg98fd9898fg9gf8989f8989fg89fg8fg8fg9fgf8f8g78fg78fgufuy84");
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SendL( streamId3, connectionId3, KUDPMessage4, NULL ) );
    User::LeaveIfError( iConnNotifyErr );
    iTimer->Start( 10000000, 1000000000, callBack );
    CActiveScheduler::Start();
    iTimer->Cancel();
    User::LeaveIfError( iConnNotifyErr );
    EUNIT_ASSERT( EFalse == iPacketReceived );
    
    // Test one more..correct one
    _LIT8(KUDPMessage9,"121jjkjzxxczx..-s.aw1221211´´+xz´+z´´z<´z´z<g8fg9fgf8f8g78fg78fgufuy84");
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SendL( streamId2, KMediaConnection, KUDPMessage9, NULL ) );
    User::LeaveIfError( iConnNotifyErr );
    this->WaitForNotify( streamId2, KMediaConnection );
    User::LeaveIfError( iConnNotifyErr );
    /*****************************/

    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SetAcceptedFromAddressL( iStreamId, iConnectionId, KAFUnspec ) );
    iTimer->Start( 1000000, 100000000, callBack );
    CActiveScheduler::Start();
    iTimer->Cancel();
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SendL( streamId3, connectionId3, KUDPMessage2, NULL ) );
    User::LeaveIfError( iConnNotifyErr );
    this->WaitForNotify( streamId3, KMediaConnection );
    User::LeaveIfError( iConnNotifyErr );
    
    _LIT8(KUDPMessage55,"zxaaq--****+0+0ääapåppcjfdhuyvjsapqp+1+121312312  bt787876 ");        
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SendL( streamId2, KMediaConnection, KUDPMessage55, NULL ) );
    User::LeaveIfError( iConnNotifyErr );
    this->WaitForNotify( streamId2, KMediaConnection );
    User::LeaveIfError( iConnNotifyErr );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SetAcceptedFromAddressL( iStreamId, iConnectionId, localAddress2 ) );
    iTimer->Start( 1000000, 100000000, callBack );
    CActiveScheduler::Start();
    iTimer->Cancel();
    
     /** packet must be filtered ***/
    iPacketReceived = EFalse;
    _LIT8(KUDPMessage66,"zxz-x.-zx-.zx+´++´21´+2´+21´+´+´<xz´+zx´´z<+´z+´z+x´z+<x´+z´x+z");
    _LIT8(KUDPMessage77,"zxxzåpoas+odi098+323490+ad´0xz+xc´<x+aasddasda+0d´+0dfdfsdfs");
            
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SendL( streamId3, connectionId3, KUDPMessage66, NULL ) );
    User::LeaveIfError( iConnNotifyErr );
    iTimer->Start( 1000000, 100000000, callBack );
    CActiveScheduler::Start();
    iTimer->Cancel();
    User::LeaveIfError( iConnNotifyErr );
    EUNIT_ASSERT( EFalse == iPacketReceived );
    
    // Test one more..correct one
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SendL( streamId2, KMediaConnection, KUDPMessage66, NULL ) );
    User::LeaveIfError( iConnNotifyErr );
    this->WaitForNotify( streamId3, connectionId3 );
    User::LeaveIfError( iConnNotifyErr );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SendL( streamId2, KMediaConnection, KUDPMessage77, NULL ) );
    User::LeaveIfError( iConnNotifyErr );
    this->WaitForNotify( streamId3, connectionId3 );
    User::LeaveIfError( iConnNotifyErr );
    
    /*****************************/ 
    
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SetAcceptedFromAddressL( iStreamId, iConnectionId, fromAddr ) );

    iTimer->Start( 1000000, 100000000, callBack );
    CActiveScheduler::Start();
    iTimer->Cancel();

    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SetAcceptedFromAddressL( iStreamId, iConnectionId, localAddress2 ) );

    iTimer->Start( 1000000, 100000000, callBack );
    CActiveScheduler::Start();
    iTimer->Cancel();
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SetAcceptedFromAddressL( iStreamId, iConnectionId, localAddress3 ) );

    iTimer->Start( 1000000, 100000000, callBack );
    CActiveScheduler::Start();
    iTimer->Cancel();

    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SetAcceptedFromAddressL( iStreamId, iConnectionId, localAddress2 ) );

    iTimer->Start( 1000000, 100000000, callBack );
    CActiveScheduler::Start();
    iTimer->Cancel();        
    
     /** packet must be filtered ***/
    iPacketReceived = EFalse;
    _LIT8(KUDPMessage99,"UDP_34535635656554,mkjkjk5564j675j6754kj6kj56k54jk5hrifudsufuy84");
    _LIT8(KUDPMessage11,"UDP_3fdsdkoiweruewrunerwweri m9+0er8vw9e8rwerm8wemerwm8vw9dsufuy84");

    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SendL( streamId3, connectionId3, KUDPMessage99, NULL ) );
    User::LeaveIfError( iConnNotifyErr );                
    iTimer->Start( 1000000, 100000000, callBack );
    CActiveScheduler::Start();
    iTimer->Cancel();
    User::LeaveIfError( iConnNotifyErr );
    EUNIT_ASSERT( EFalse == iPacketReceived );
    
    // Test one more..correct one
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SendL( streamId2, KMediaConnection, KUDPMessage99, NULL ) );
    User::LeaveIfError( iConnNotifyErr );
    this->WaitForNotify( streamId3, connectionId3 );
    User::LeaveIfError( iConnNotifyErr );
    
    /*****************************/       
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SetAcceptedFromAddressL( iStreamId, iConnectionId, localAddress2 ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnMux->SetAcceptedFromAddressL( iStreamId, iConnectionId, localAddress2 ) );
    
    iTimer->Start( 1000000, 100000000, callBack );
    CActiveScheduler::Start();
    User::LeaveIfError( iConnNotifyErr );
    iTimer->Cancel();
    } 
    
    
//  TEST TABLE
EUNIT_BEGIN_TEST_TABLE(
    UT_CNcmConnectionMultiplexer,
    "CConnectionMultiplexer Test suite",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CConnectionMultiplexer",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CNcmConnectionMultiplexer_NewLL, Teardown)

EUNIT_TEST(
    "CreateSessionL - test ",
    "CConnectionMultiplexer",
    "CreateSessionL",
    "FUNCTIONALITY",
    SetupL, UT_CNcmConnectionMultiplexer_CreateSessionLL, Teardown)

EUNIT_TEST(
    "RemoveSessionL - test ",
    "CConnectionMultiplexer",
    "RemoveSessionL",
    "FUNCTIONALITY",
    SetupL, UT_CNcmConnectionMultiplexer_RemoveSessionLL, Teardown)

EUNIT_TEST(
    "CreateStreamL - test ",
    "CConnectionMultiplexer",
    "CreateStreamL",
    "FUNCTIONALITY",
    SetupL, UT_CNcmConnectionMultiplexer_CreateStreamLL, Teardown)

EUNIT_TEST(
    "GetStreamInfoL - test ",
    "CConnectionMultiplexer",
    "GetStreamInfoL",
    "FUNCTIONALITY",
    SetupL, UT_CNcmConnectionMultiplexer_GetStreamInfoLL, Teardown )

EUNIT_TEST(
    "CreateConnectionL - test ",
    "CConnectionMultiplexer",
    "CreateConnectionL",
    "FUNCTIONALITY",
    Setup2L, UT_CNcmConnectionMultiplexer_CreateConnectionLL, Teardown2 )
    
EUNIT_TEST(
    "CreateConnectionL2 - test ",
    "CConnectionMultiplexer",
    "CreateConnectionL2",
    "FUNCTIONALITY",
    Setup2L, UT_CNcmConnectionMultiplexer_CreateConnectionLL2, Teardown2 )    

EUNIT_TEST(
    "SetSendingStateL - test ",
    "CConnectionMultiplexer",
    "SetSendingStateL",
    "FUNCTIONALITY",
    Setup2L, UT_CNcmConnectionMultiplexer_SetSendingStateLL, Teardown2 )
 
EUNIT_TEST(
    "SetReceivingStateL - test ",
    "CConnectionMultiplexer",
    "SetReceivingStateL",
    "FUNCTIONALITY",
    Setup2L, UT_CNcmConnectionMultiplexer_SetReceivingStateLL, Teardown2 )

EUNIT_TEST(
    "SetReceivingStateL - TCP - test ",
    "CConnectionMultiplexer",
    "SetReceivingStateL - TCP",
    "FUNCTIONALITY",
    Setup2L, UT_CNcmConnectionMultiplexer_SetReceivingStateL_TCP, Teardown2 )     

EUNIT_TEST(
    "SetReceivingStateForMediaL - test ",
    "CConnectionMultiplexer",
    "SetReceivingStateForMediaL",
    "FUNCTIONALITY",
    Setup2L, UT_CNcmConnectionMultiplexer_SetReceivingStateForMediaLL, Teardown2 )   
    
EUNIT_TEST(
    "LocalIPAddressL - test ",
    "CConnectionMultiplexer",
    "LocalIPAddressL",
    "FUNCTIONALITY",
    Setup2L, UT_CNcmConnectionMultiplexer_LocalIPAddressLL, Teardown2 )

EUNIT_TEST(
    "RemoveStreamL - test ",
    "CConnectionMultiplexer",
    "RemoveStreamL",
    "FUNCTIONALITY",
    Setup2L, UT_CNcmConnectionMultiplexer_RemoveStreamLL, Teardown2 )
    
EUNIT_TEST(
    "RemoveConnectionL - test ",
    "CConnectionMultiplexer",
    "RemoveConnectionL",
    "FUNCTIONALITY",
    Setup2L, UT_CNcmConnectionMultiplexer_RemoveConnectionLL, Teardown2 )
    
EUNIT_TEST(
    "RegisterIncomingConnectionObserverL - test ",
    "CConnectionMultiplexer",
    "RegisterIncomingConnectionObserverL",
    "FUNCTIONALITY",
    Setup2L, UT_CNcmConnectionMultiplexer_RegisterIncomingConnectionObserverLL, Teardown2 )

EUNIT_TEST(
    "RegisterOutgoingConnectionObserverL - test ",
    "CConnectionMultiplexer",
    "RegisterOutgoingConnectionObserverL",
    "FUNCTIONALITY",
    Setup2L, UT_CNcmConnectionMultiplexer_RegisterOutgoingConnectionObserverLL, Teardown2 )

EUNIT_TEST(
    "UnregisterIncomingConnectionObserverL - test ",
    "CConnectionMultiplexer",
    "UnregisterIncomingConnectionObserverL",
    "FUNCTIONALITY",
    Setup2L, UT_CNcmConnectionMultiplexer_UnregisterIncomingConnectionObserverLL, Teardown2 )

EUNIT_TEST(
    "UnregisterOutgoingConnectionObserverL - test ",
    "CConnectionMultiplexer",
    "UnregisterOutgoingConnectionObserverL",
    "FUNCTIONALITY",
    Setup2L, UT_CNcmConnectionMultiplexer_UnregisterOutgoingConnectionObserverLL, Teardown2 )
    
EUNIT_TEST(
    "RegisterConnectionObserverL - test ",
    "CConnectionMultiplexer",
    "RegisterConnectionObserverL",
    "FUNCTIONALITY",
    Setup2L, UT_CNcmConnectionMultiplexer_RegisterConnectionObserverLL, Teardown2 )

EUNIT_TEST(
    "UnregisterConnectionObserverL - test ",
    "CConnectionMultiplexer",
    "UnregisterConnectionObserverL",
    "FUNCTIONALITY",
    Setup2L, UT_CNcmConnectionMultiplexer_UnregisterConnectionObserverLL, Teardown2 )

EUNIT_TEST(
    "RegisterMediaWrapperL - test ",
    "CConnectionMultiplexer",
    "RegisterMediaWrapperL",
    "FUNCTIONALITY",
    Setup2L, UT_CNcmConnectionMultiplexer_RegisterMediaWrapperLL, Teardown2 )

EUNIT_TEST(
    "GetSessionInfoL - test ",
    "CConnectionMultiplexer",
    "GetSessionInfoL",
    "FUNCTIONALITY",
    Setup2L, UT_CNcmConnectionMultiplexer_GetSessionInfoLL, Teardown2)

EUNIT_TEST(
    "PortStore - test ",
    "CConnectionMultiplexer",
    "PortStore",
    "FUNCTIONALITY",
    Setup2L, UT_CNcmConnectionMultiplexer_PortStoreL, Teardown2 )

EUNIT_TEST(
    "ActivateStreamReceivingL - test ",
    "CConnectionMultiplexer",
    "ActivateStreamReceivingL",
    "FUNCTIONALITY",
    SetupL, UT_CNcmConnectionMultiplexer_ActivateStreamReceivingLL, Teardown )
  
EUNIT_TEST(
    "ActivateStreamSendingL - test ",
    "CConnectionMultiplexer",
    "ActivateStreamSendingL",
    "FUNCTIONALITY",
    SetupL, UT_CNcmConnectionMultiplexer_ActivateStreamSendingLL, Teardown )

EUNIT_TEST(
    "SendL - test ",
    "CConnectionMultiplexer",
    "SendL",
    "FUNCTIONALITY",
    Setup2L, UT_CNcmConnectionMultiplexer_SendLL, Teardown2 )

EUNIT_TEST(
    "SendL2 - test ",
    "CConnectionMultiplexer",
    "SendL2",
    "FUNCTIONALITY",
    Setup2L, UT_CNcmConnectionMultiplexer_SendLL2, Teardown2 )

EUNIT_TEST(
    "SendL3 - test ",
    "CConnectionMultiplexer",
    "SendL3",
    "FUNCTIONALITY",
    Setup2L, UT_CNcmConnectionMultiplexer_SendLL3, Teardown2 )
 
 EUNIT_TEST(
    "SendL4 - test ",
    "CConnectionMultiplexer",
    "SendL4",
    "FUNCTIONALITY",
    Setup2L, UT_CNcmConnectionMultiplexer_SendLL4, Teardown2 )

 EUNIT_TEST(
    "CancelMessageSend - test ",
    "CConnectionMultiplexer",
    "CancelMessageSend",
    "FUNCTIONALITY",
    Setup2L, UT_CNcmConnectionMultiplexer_CancelMessageSendL, Teardown2 )  
    
 EUNIT_TEST(
    "StreamById - test ",
    "CConnectionMultiplexer",
    "StreamById",
    "FUNCTIONALITY",
    Setup2L, UT_CNcmConnectionMultiplexer_StreamByIdL, Teardown2 )  
    
 EUNIT_TEST(
    "RegisterMessageObserverL - test ",
    "CConnectionMultiplexer",
    "RegisterMessageObserverL",
    "FUNCTIONALITY",
    Setup2L, UT_CNcmConnectionMultiplexer_RegisterMessageObserverLL, Teardown2 )  
    
 EUNIT_TEST(
    "UnregisterMessageObserver - test ",
    "CConnectionMultiplexer",
    "UnregisterMessageObserver",
    "FUNCTIONALITY",
    Setup2L, UT_CNcmConnectionMultiplexer_UnregisterMessageObserverL, Teardown2 )  
         
 EUNIT_TEST(
    "OpenTcpConnectionL - test ",
    "CConnectionMultiplexer",
    "OpenTcpConnectionL",
    "FUNCTIONALITY",
    Setup2L, UT_CNcmConnectionMultiplexer_OpenTcpConnectionLL, Teardown2 )
    
 EUNIT_TEST(
    "CloseTcpConnectionL - test ",
    "CConnectionMultiplexer",
    "CloseTcpConnectionL",
    "FUNCTIONALITY",
    Setup2L, UT_CNcmConnectionMultiplexer_CloseTcpConnectionL, Teardown2 )

 EUNIT_TEST(
    "ResolveDestinationAddressL - test ",
    "CConnectionMultiplexer",
    "ResolveDestinationAddressL",
    "FUNCTIONALITY",
    Setup2L, UT_CNcmConnectionMultiplexer_ResolveDestinationAddressLL, Teardown2 )     

 EUNIT_TEST(
    "SetAcceptedFromAddressL - test ",
    "CConnectionMultiplexer",
    "SetAcceptedFromAddressL",
    "FUNCTIONALITY",
    Setup2L, UT_CNcmConnectionMultiplexer_SetAcceptedFromAddressLL, Teardown2 )

EUNIT_END_TEST_TABLE

//  END OF FILE
