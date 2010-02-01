/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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




// INCLUDES
#include <e32debug.h>
#include "natfwconnectivityapidefs.h"
#include <mmccinterface.h>
#include <mmcccodecinformation.h>
#include "cncmconnectionmultiplexer.h"

#include "testdefinitions.hrh"
#include "ncmconnectionmultiplexerlogs.h"
#include "cncmportstore.h"
#include "natfwsocketmediaconnwrapper.h"
#include "muxtestconsolestubs.h"
#include "multiplexertcptests.h"


#ifndef __WINSCW__
const TInt KTestIapId = 1;
#else
const TInt KTestIapId = 6;
#endif

const TInt KAudioPrefVoipAudioUplink           = 0x05220001;
const TInt KAudioPrefVoipAudioUplinkNonSignal  = 0x05230001;
const TInt KAudioPrefVoipAudioDownlink         = 0x05210001;
const TInt KAudioDTMFString                    = 0x00150001;
const TInt KAudioPriorityDTMFString            = 45;

const TInt KDefaultQos = 46;
_LIT8(KUDPMessage,"UDP_Message"); 

const TUint KMediaConnection = 1;

// ======== MEMBER FUNCTIONS =================================================

// ---------------------------------------------------------------------------
// CMultiplexerTcpTestsInterface::CMultiplexerTcpTestsInterface
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CMultiplexerTcpTests::CMultiplexerTcpTests( CConsoleBase& aConsole,
    CNcmConnectionMultiplexer& aMux, CMuxTestConsoleStubs& aIFStub ) :
    iConsole( aConsole ), iMux( aMux ), iIfStub( aIFStub )
    {
    }
    
// ---------------------------------------------------------------------------
// CMultiplexerTcpTests::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CMultiplexerTcpTests::ConstructL()
    {
    // Dummy 'cause first uheap marking
    TUint sessionId( 0 );
    TUint streamId( 0 );

    sessionId = iMux.CreateSessionL( KTestIapId, 5500, 10000 );
    CActiveScheduler::Start();
    
    streamId = iMux.CreateStreamL( sessionId, KDefaultQos, KProtocolInetUdp );
    
    iMux.RegisterIncomingConnectionObserverL( streamId, iIfStub ); 
    
    TUint sub = iMux.CreateConnectionL( streamId, KAfInet );
    iMux.UnregisterIncomingConnectionObserverL( streamId, iIfStub );
    iMux.RemoveSessionL( sessionId );
    }
    
// ---------------------------------------------------------------------------
// CMultiplexerTcpTests::NewL
// Static constructor.
// ---------------------------------------------------------------------------
//   
CMultiplexerTcpTests* CMultiplexerTcpTests::NewL( CConsoleBase& aConsole,
    CNcmConnectionMultiplexer& aMux, CMuxTestConsoleStubs& aIFStub )
    {
    CMultiplexerTcpTests* self 
        = new ( ELeave ) CMultiplexerTcpTests( aConsole, aMux, aIFStub );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CMultiplexerTcpTests::~CMultiplexerTcpTests
// Destructor.
// ---------------------------------------------------------------------------
//
CMultiplexerTcpTests::~CMultiplexerTcpTests()
    {
    }

 void CMultiplexerTcpTests::DoRunTcpMuxTestsL()
    {
    iConsole.ClearScreen();
    iConsole.Printf( _L("\n*** MUX TESTS ***\n") );
    
    TInt err( KErrNone );

    iConsole.Printf( _L("Create session test\n") );
    RDebug::Print( _L("MUX_TEST_START: TestCreateSessionL") );
    //__UHEAP_MARK;
    TRAP( err, TestCreateSessionL() );
    //__UHEAP_MARKEND;
    RDebug::Print( _L("MUX_TEST_STOP: TestCreateSessionL, ERR: %d"), err );

    // Check again with memory leak check
    iConsole.Printf( _L("Create session test\n") );
    RDebug::Print( _L("MUX_TEST_START: TestCreateSessionL") );
    __UHEAP_MARK;
    TRAP( err, TestCreateSessionL() );
    __UHEAP_MARKEND;
    RDebug::Print( _L("MUX_TEST_STOP: TestCreateSessionL, ERR: %d"), err );
    
    err = KErrNone;
    RDebug::Print( _L("MUX_TEST_START: TestCreateStreamL") );
    iConsole.Printf( _L("Create stream test\n") );
    __UHEAP_MARK;
    TRAP( err, TestCreateStreamL() );
    __UHEAP_MARKEND;
    RDebug::Print( _L("MUX_TEST_STOP: TestCreateStreamL, ERR: %d"), err );

    err = KErrNone;
    RDebug::Print( _L("MUX_TEST_START: TestCreateConnectionL") );
    iConsole.Printf( _L("Create connection test\n") );
    __UHEAP_MARK;
    TRAP( err, TestCreateConnectionL() );
    __UHEAP_MARKEND;
    RDebug::Print( _L("MUX_TEST_STOP: TestCreateConnectionL, ERR: %d"), err );
 
    err = KErrNone;
    RDebug::Print( _L("MUX_TEST_START: TestConnectTcpConnectionsL") );
    iConsole.Printf( _L("Connect TCP connections test\n") );
    //__UHEAP_MARK;
    TRAP( err, TestConnectTcpConnectionsL() );
    //__UHEAP_MARKEND;
    RDebug::Print( _L("MUX_TEST_STOP: TestConnectTcpConnectionsL, ERR: %d"), err );

    // Check again with memory leak check
    err = KErrNone;
    RDebug::Print( _L("MUX_TEST_START: TestConnectTcpConnectionsL") );
    iConsole.Printf( _L("Connect TCP connections test\n") );
    __UHEAP_MARK;
    TRAP( err, TestConnectTcpConnectionsL() );
    __UHEAP_MARKEND;
    RDebug::Print( _L("MUX_TEST_STOP: TestConnectTcpConnectionsL, ERR: %d"), err );
    
    err = KErrNone;
    RDebug::Print( _L("MUX_TEST_START: TestActivateStunTcpMediaConnectionsL") );
    iConsole.Printf( _L("Activate STUN TCP MEDIA test\n") );
    __UHEAP_MARK;
    TRAP( err, TestActivateStunTcpMediaConnectionsL() );
    __UHEAP_MARKEND;
    RDebug::Print( _L("MUX_TEST_STOP: TestActivateStunTcpMediaConnectionsL, ERR: %d"), err );    
    }
      
// ---------------------------------------------------------------------------
// CMultiplexerTcpTests::TestCreateSessionL
// ---------------------------------------------------------------------------
//
void CMultiplexerTcpTests::TestCreateSessionL()
    {
    TUint sessionId( 0 );
    
    sessionId = iMux.CreateSessionL( KTestIapId, 5000, 8000 );
    CActiveScheduler::Start();
    
    iMux.RemoveSessionL( sessionId );
    
    sessionId = iMux.CreateSessionL( KTestIapId, 5000, 8000 );
    iMux.RemoveSessionL( sessionId );
    }

    
// ---------------------------------------------------------------------------
// CMultiplexerTcpTests::TestCreateSessionL
// ---------------------------------------------------------------------------
//
void CMultiplexerTcpTests::TestCreateStreamL()
    {
    TUint sessionId( 0 );
    TUint sessionId2( 0 );
    TUint streamId( 0 );
    TUint streamId2( 0 );  
    
    sessionId = iMux.CreateSessionL( KTestIapId, 5000, 8000 ); 
    CActiveScheduler::Start();

        
    streamId = iMux.CreateStreamL( sessionId, KDefaultQos, KProtocolInetTcp );
    streamId2 = iMux.CreateStreamL( sessionId, KDefaultQos, KProtocolInetTcp );
      
    iMux.RemoveStreamL( streamId ); 
    iMux.RemoveStreamL( streamId2 );

    streamId = iMux.CreateStreamL( sessionId, KDefaultQos, KProtocolInetTcp );
    
    iMux.RegisterIncomingConnectionObserverL( streamId, iIfStub );
    iMux.RegisterIncomingConnectionObserverL( streamId, iIfStub );
    
    streamId2 = iMux.CreateStreamL( sessionId, KDefaultQos, KProtocolInetTcp );
    
    iMux.RemoveStreamL( streamId );
    iMux.RemoveSessionL( sessionId );
    iMux.RemoveSessionL( sessionId2 );
    }

// ---------------------------------------------------------------------------
// CMultiplexerTcpTests::TestCreateConnectionL
// ---------------------------------------------------------------------------
//
void CMultiplexerTcpTests::TestCreateConnectionL()
    {
    TUint sessionId( 0 );
    
    TUint streamId( 0 );
    TUint streamId2( 0 );
      
    TUint connectionId( 0 );
    TUint connectionId2( 0 );
    TUint connectionId3( 0 );
    TUint connectionId4( 0 );
    TUint connectionId5( 0 );
    
    sessionId = iMux.CreateSessionL( KTestIapId, 5000, 8000 );
    CActiveScheduler::Start();
    
    streamId = iMux.CreateStreamL( sessionId, KDefaultQos, KProtocolInetTcp );
    streamId2 = iMux.CreateStreamL( sessionId, KDefaultQos, KProtocolInetTcp );
    
    iMux.RegisterIncomingConnectionObserverL(streamId, iIfStub );
  
    connectionId = iMux.CreateConnectionL( streamId, KAfInet );
    connectionId2 = iMux.CreateConnectionL( streamId, KAfInet );  
    connectionId3 = iMux.CreateConnectionL( streamId2, KAfInet );
    
    // New connection, Reuse local address
    TInetAddr connectionAddr = iMux.LocalIPAddressL( streamId, connectionId2 );
    connectionId4 = iMux.CreateConnectionL( streamId2, connectionAddr );  
    // Check success of address reuse
    TInetAddr connectionAddr2 = iMux.LocalIPAddressL( streamId, connectionId4 );
    ASSERT( connectionAddr.CmpAddr( connectionAddr2 ) );
    
    
    // New connection, Reuse local address
    TInetAddr connectionAddr3 = iMux.LocalIPAddressL( streamId, connectionId3 );
    connectionId5 = iMux.CreateConnectionL( streamId2, connectionAddr3 );
    // Check success of address reuse
    TInetAddr connectionAddr5 = iMux.LocalIPAddressL( streamId2, connectionId5 );    
    ASSERT( connectionAddr3.CmpAddr( connectionAddr5 ) );    
    
    // New connection, Reuse local address
    connectionAddr3 = iMux.LocalIPAddressL( streamId, connectionId3 );
    connectionId5 = iMux.CreateConnectionL( streamId2, connectionAddr3 );
    // Check success of address reuse
    connectionAddr5 = iMux.LocalIPAddressL( streamId2, connectionId5 );    
    ASSERT( connectionAddr3.CmpAddr( connectionAddr5 ) );  
        
    iMux.RemoveConnectionL( streamId2, connectionId5 );
    iMux.RemoveConnectionL( streamId, connectionId3 );
    iMux.RemoveStreamL( streamId );
    iMux.RemoveSessionL( sessionId ); 
    }

  
// ---------------------------------------------------------------------------
// CMultiplexerUdpTests::TestConnectTcpConnectionsL
// ---------------------------------------------------------------------------
//
void CMultiplexerTcpTests::TestConnectTcpConnectionsL()
    {
    TUint sessionId( 0 );
    
    TUint streamId( 0 );
    TUint streamId2( 0 );
      
    TUint connectionId( 0 );
    TUint connectionId2( 0 );
    TUint connectionId3( 0 );
    
 
    // Create 1 session, 1 stream, 1 connection
    sessionId = iMux.CreateSessionL( KTestIapId, 8000, 11000 );
    CActiveScheduler::Start();  
    
    streamId = iMux.CreateStreamL( sessionId, KDefaultQos, KProtocolInetTcp );
    streamId2 = iMux.CreateStreamL( sessionId, KDefaultQos, KProtocolInetTcp );     
    connectionId = iMux.CreateConnectionL( streamId, KAfInet ); 
    connectionId2 = iMux.CreateConnectionL( streamId2, KAfInet );
        
    // Address resolving
    TInetAddr connectionAddr( iMux.LocalIPAddressL( streamId, connectionId ) );
    TInetAddr connectionAddr2( iMux.LocalIPAddressL( streamId2, connectionId2 ) );    
    
    iMux.RegisterConnectionObserverL( streamId, iIfStub );
    iMux.RegisterConnectionObserverL( streamId2, iIfStub );    
  
    iMux.OpenTcpConnectionL( streamId, connectionId, ETcpSetupActive, connectionAddr2 ); 
    iMux.OpenTcpConnectionL( streamId2, connectionId2, ETcpSetupActive, connectionAddr );
    CActiveScheduler::Start(); 
    CActiveScheduler::Start();    

    iMux.CloseTcpConnection( streamId, connectionId );
    iMux.CloseTcpConnection( streamId2, connectionId2 );
    
    iMux.OpenTcpConnectionL( streamId, connectionId, ETcpSetupPassive, connectionAddr2 ); 
    iMux.OpenTcpConnectionL( streamId2, connectionId2, ETcpSetupActive, connectionAddr );
    CActiveScheduler::Start(); 
    CActiveScheduler::Start();
 
    iMux.CloseTcpConnection( streamId, connectionId );
    iMux.CloseTcpConnection( streamId2, connectionId2 );
    
    iMux.OpenTcpConnectionL( streamId2, connectionId2, ETcpSetupActive, connectionAddr );
    iMux.OpenTcpConnectionL( streamId, connectionId, ETcpSetupPassive, connectionAddr2 );
    CActiveScheduler::Start(); 
    CActiveScheduler::Start();
    
    iMux.CloseTcpConnection( streamId, connectionId );
    iMux.CloseTcpConnection( streamId2, connectionId2 ); 
       
    // Cancel the connection process
    iMux.OpenTcpConnectionL( streamId, connectionId, ETcpSetupPassive, connectionAddr2 );    
    iMux.CloseTcpConnection( streamId, connectionId );
    CActiveScheduler::Start();
    
    iMux.OpenTcpConnectionL( streamId, connectionId, ETcpSetupPassive, connectionAddr2 );    
    iMux.CloseTcpConnection( streamId, connectionId );
    CActiveScheduler::Start();  
    iMux.RemoveSessionL( sessionId );
    }



// ---------------------------------------------------------------------------
// CMultiplexerTcpTests::TestActivateStunTcpMediaConnectionsL
// ---------------------------------------------------------------------------
//
void CMultiplexerTcpTests::TestActivateStunTcpMediaConnectionsL()
    {
    TUint sessionId( 0 );
    
    TUint streamId( 0 );
    TUint streamId2( 0 );
      
    TUint connectionId( 0 );
    TUint connectionId2( 0 );
    TUint connectionId3( 0 );
    
    sessionId = iMux.CreateSessionL( KTestIapId, 5000, 8000 );
    CActiveScheduler::Start();
    
    streamId = iMux.CreateStreamL( sessionId, KDefaultQos, KProtocolInetTcp );
    streamId2 = iMux.CreateStreamL( sessionId, KDefaultQos, KProtocolInetTcp );
    
    // Simulate UDP fetch candidate    
    iMux.RegisterIncomingConnectionObserverL( streamId, iIfStub );
    iMux.RegisterConnectionObserverL( streamId, iIfStub );
    iMux.RegisterConnectionObserverL( streamId2, iIfStub );
    connectionId = iMux.CreateConnectionL( streamId, KAfInet );
    
    // Test connection
    connectionId2 = iMux.CreateConnectionL( streamId2, KAfInet );  
    TInetAddr connectionAddress2( iMux.LocalIPAddressL( streamId2, connectionId2 ) );

    // Test connection, SERVER
    connectionId3 = iMux.CreateConnectionL( streamId2, KAfInet );  
    TInetAddr connectionAddress3( iMux.LocalIPAddressL( streamId2, connectionId3 ) );
    
    /**** setup wrapper *****/
    CNATFWSocketMediaConnWrapper* mediaWrapper = CNATFWSocketMediaConnWrapper::NewL(
             iMux.PortStoreL( sessionId ).Port(), KProtocolInetUdp, streamId );
       
    iMux.RegisterMediaWrapperL( mediaWrapper );
            
    TInetAddr destinationAddress;    
    mediaWrapper->OutgoingAddr( destinationAddress );    
    __CONNECTIONMULTIPLEXER_ADDRLOG( "CMultiplexerUdpTests::TestUdpAudioL- Wrapper LOCAL ADDRESS: ", destinationAddress )

    
    mediaWrapper->SetIncomingAddrL( destinationAddress );
    //TODO connect connection 2 to wrapper and start wrapper receiving
    /**************************/

    // Dummy address
    TInetAddr stunServerAddress;
    stunServerAddress.SetAddress( INET_ADDR( 10,36,7,6 ) );
    stunServerAddress.SetPort( 3478 );
   
    // STUN-plugin connects connection to stun server
    iMux.OpenTcpConnectionL( streamId2, connectionId3, ETcpSetupPassive, stunServerAddress );  // Server
    iMux.OpenTcpConnectionL( streamId, connectionId, ETcpSetupActive, connectionAddress3 );
    CActiveScheduler::Start();   
    CActiveScheduler::Start(); 
    
    iMux.SetReceivingStateL( streamId, connectionId, EStreamingStateActive );
    iMux.SetSendingStateL( streamId, connectionId, stunServerAddress, EStreamingStateActive );
    iMux.SetReceivingStateL( streamId2, connectionId3, EStreamingStateActive );
    CActiveScheduler::Start();  
    CActiveScheduler::Start();    
    CActiveScheduler::Start();
    
    // Binding request
    _LIT8(KUDPMessage,"UDP_Binding_Request");
    iMux.SendL( streamId, connectionId, KUDPMessage, NULL );
    
   
    // STUN plugin, reuse connection.. connect to peer address
    iMux.CloseTcpConnection( streamId, connectionId );  // Connection to STUN server
    CActiveScheduler::Start(); 
        
    iMux.OpenTcpConnectionL( streamId2, connectionId2, ETcpSetupPassive, stunServerAddress );  // Remote end
    iMux.OpenTcpConnectionL( streamId, connectionId, ETcpSetupActive, connectionAddress2 );   
    CActiveScheduler::Start();     
    CActiveScheduler::Start();      
    
    // Remote end
         
    TInetAddr connectionAddress( iMux.LocalIPAddressL( streamId, connectionId ) );
    iMux.SetSendingStateL( streamId2, connectionId2, connectionAddress, EStreamingStateActive );
    CActiveScheduler::Start();
    
    
    // Setup media flow to peer
    // NATFW client
    iMux.SetReceivingStateL( streamId, connectionId, EStreamingStateActive );
    iMux.SetReceivingStateForMediaL( streamId, connectionId, EStreamingStateActive );
    CActiveScheduler::Start();
    CActiveScheduler::Start();
    
    // NATFW client
    iMux.SetSendingStateL( streamId, connectionId, connectionAddress2, EStreamingStateActive );;
    iMux.SetSendingStateForMediaL( streamId, connectionId, EStreamingStateActive );
    CActiveScheduler::Start(); 
    CActiveScheduler::Start(); 


    // Set sending again
    iMux.SetSendingStateL( streamId, connectionId, connectionAddress2, EStreamingStateActive );
    CActiveScheduler::Start(); 
    iMux.SetSendingStateL( streamId, connectionId, connectionAddress2, EStreamingStateActive );
    CActiveScheduler::Start(); 
           
    // Set receiving again
    iMux.SetReceivingStateL( streamId, connectionId, EStreamingStateActive );
    CActiveScheduler::Start();
    iMux.SetReceivingStateL( streamId, connectionId, EStreamingStateActive );
    CActiveScheduler::Start(); 
    
    iMux.SendL( streamId2, connectionId2, KUDPMessage, &iIfStub );
    iMux.SendL( streamId2, connectionId2, KUDPMessage, connectionAddress2, &iIfStub );
    CActiveScheduler::Start(); 
    CActiveScheduler::Start();
    
    iMux.CloseTcpConnection( streamId, connectionId );  // Connection to STUN server   
    iMux.RemoveSessionL( sessionId );  
    }
