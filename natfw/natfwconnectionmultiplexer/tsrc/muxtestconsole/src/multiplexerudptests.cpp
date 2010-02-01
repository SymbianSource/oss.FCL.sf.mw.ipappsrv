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
#include <in_sock.h>
#include "cncmconnectionmultiplexer.h"
#include <mmccinterface.h>
#include <mmcccodecinformation.h>

#include "multiplexerudptests.h"
#include "muxtestconsolestubs.h"
#include "testdefinitions.hrh"
#include "ncmconnectionmultiplexerlogs.h"
#include "cncmportstore.h"
#include "natfwsocketmediaconnwrapper.h"

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
// CMultiplexerUdpTestsInterface::CMultiplexerUdpTestsInterface
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CMultiplexerUdpTests::CMultiplexerUdpTests( CConsoleBase& aConsole,
    CNcmConnectionMultiplexer& aMux, CMuxTestConsoleStubs& aIFStub ) :
    iConsole( aConsole ), iMux( aMux ), iIfStub( aIFStub )
    {
    }
    
// ---------------------------------------------------------------------------
// CMultiplexerUdpTests::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CMultiplexerUdpTests::ConstructL()
    {
    RDebug::Print( _L("CMultiplexerUdpTests::ConstructL IAP: %d"), KTestIapId );
    
    // Dummy 'cause first uheap marking  
    TUint sessionId( 0 );
    TUint streamId( 0 );

    sessionId = iMux.CreateSessionL( KTestIapId, 5090, 5500 );
    CActiveScheduler::Start();
    
    streamId = iMux.CreateStreamL( sessionId, KDefaultQos, KProtocolInetUdp );

    iMux.RegisterIncomingConnectionObserverL( streamId, iIfStub );
    TUint sub = iMux.CreateConnectionL( streamId, KAfInet );
    iMux.UnregisterIncomingConnectionObserverL( streamId, iIfStub );
    iMux.RemoveSessionL( sessionId );    
    }
    
// ---------------------------------------------------------------------------
// CMultiplexerUdpTests::NewL
// Static constructor.
// ---------------------------------------------------------------------------
//   
CMultiplexerUdpTests* CMultiplexerUdpTests::NewL( CConsoleBase& aConsole,
    CNcmConnectionMultiplexer& aMux, CMuxTestConsoleStubs& aIFStub )
    {
    CMultiplexerUdpTests* self 
        = new ( ELeave ) CMultiplexerUdpTests( aConsole, aMux, aIFStub );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CMultiplexerUdpTests::~CMultiplexerUdpTests
// Destructor.
// ---------------------------------------------------------------------------
//
CMultiplexerUdpTests::~CMultiplexerUdpTests()
    {
    delete iMccInterface;
    TRAP_IGNORE( iMux.RemoveSessionL( iSessionId ) );
    }

// ---------------------------------------------------------------------------
// CMultiplexerUdpTests::DoRunUdpMuxtTests
// 
// ---------------------------------------------------------------------------
//
void CMultiplexerUdpTests::DoRunUdpMuxtTests()
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
     
    // After this test again with memory leak check
    err = KErrNone;
    RDebug::Print( _L("MUX_TEST_START: TestActivateUdpConnectionsL") );
    iConsole.Printf( _L("(De)Activate UDP connections test\n") );
    //__UHEAP_MARK;
    TRAP( err, TestActivateUdpConnectionsL() );
    //__UHEAP_MARKEND;
    RDebug::Print( _L("MUX_TEST_STOP: TestActivateUdpConnectionsL, ERR: %d"), err );

    // DO same as before... Now check memory leak
    err = KErrNone;
    RDebug::Print( _L("MUX_TEST_START: TestActivateUdpConnectionsL") );
    iConsole.Printf( _L("(De)Activate UDP connections test\n") );
    __UHEAP_MARK;
    TRAP( err, TestActivateUdpConnectionsL() );
    __UHEAP_MARKEND;
    RDebug::Print( _L("MUX_TEST_STOP: TestActivateUdpConnectionsL, ERR: %d"), err );
    
    err = KErrNone;
    RDebug::Print( _L("MUX_TEST_START: TestActivateStunUdpMediaConnectionsL") );
    iConsole.Printf( _L("(De)Activate STUN UDP media connections test\n") );
    __UHEAP_MARK;
    TRAP( err, TestActivateStunUdpMediaConnectionsL() );
    __UHEAP_MARKEND;
    RDebug::Print( _L("MUX_TEST_STOP: TestActivateStunUdpMediaConnectionsL, ERR: %d"), err );  
    }

      
// ---------------------------------------------------------------------------
// CMultiplexerUdpTests::TestCreateSessionL
// ---------------------------------------------------------------------------
//
void CMultiplexerUdpTests::TestCreateSessionL()
    {
    TUint sessionId( 0 );
    TUint sessionId2( 0 );
    
    sessionId = iMux.CreateSessionL( 0,5000, 5100 );
    CActiveScheduler::Start();
    
    iMux.RemoveSessionL( sessionId );
    
    sessionId = iMux.CreateSessionL( KTestIapId, 5000, 5100 );
    iMux.RemoveSessionL( sessionId );
    
    sessionId = iMux.CreateSessionL( KTestIapId, 5000, 5100 );
    iMux.RemoveSessionL( sessionId );
    
    sessionId = iMux.CreateSessionL( 0,5000, 5100 );
    sessionId2 = iMux.CreateSessionL( 0,5000, 5100 );
    CActiveScheduler::Start();
    
    iMux.RemoveSessionL( sessionId );
    iMux.RemoveSessionL( sessionId2 );
    
    
    }

// ---------------------------------------------------------------------------
// CMultiplexerUdpTests::TestCreateStreamL
// ---------------------------------------------------------------------------
//
void CMultiplexerUdpTests::TestCreateStreamL()
    {
    TUint sessionId( 0 );
    TUint sessionId2( 0 );
    TUint streamId( 0 );
    TUint streamId2( 0 );  
    
    sessionId = iMux.CreateSessionL( KTestIapId, 5000, 5100 ); 
    CActiveScheduler::Start();

        
    streamId = iMux.CreateStreamL( sessionId, KDefaultQos, KProtocolInetUdp );
    streamId2 = iMux.CreateStreamL( sessionId, KDefaultQos, KProtocolInetUdp );
      
    iMux.RemoveStreamL( streamId ); 
    iMux.RemoveStreamL( streamId2 );

    streamId = iMux.CreateStreamL( sessionId, KDefaultQos, KProtocolInetUdp );
    
    iMux.RegisterIncomingConnectionObserverL( streamId, iIfStub );
    iMux.RegisterIncomingConnectionObserverL( streamId, iIfStub );
    
    streamId2 = iMux.CreateStreamL( sessionId, KDefaultQos, KProtocolInetUdp );
    
    iMux.RemoveStreamL( streamId );
    iMux.RemoveSessionL( sessionId );
    iMux.RemoveSessionL( sessionId2 );
    }

// ---------------------------------------------------------------------------
// CMultiplexerUdpTests::TestCreateConnectionL
// ---------------------------------------------------------------------------
//
void CMultiplexerUdpTests::TestCreateConnectionL()
    {
    TUint sessionId( 0 );
    
    TUint streamId( 0 );
    TUint streamId2( 0 );
      
    TUint connectionId( 0 );
    TUint connectionId2( 0 );
    TUint connectionId3( 0 );
    TUint connectionId4( 0 );
    TUint connectionId5( 0 );
    
    sessionId = iMux.CreateSessionL( KTestIapId, 5000, 5100 );
    CActiveScheduler::Start();
    
    streamId = iMux.CreateStreamL( sessionId, KDefaultQos, KProtocolInetUdp );
    streamId2 = iMux.CreateStreamL( sessionId, KDefaultQos, KProtocolInetUdp );
    
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
// CMultiplexerUdpTests::TestActivateUdpConnectionsL
// ---------------------------------------------------------------------------
//
void CMultiplexerUdpTests::TestActivateUdpConnectionsL()
    {
    TUint sessionId( 0 );
    
    TUint streamId( 0 );
    TUint streamId2( 0 );
      
    TUint connectionId( 0 );
    TUint connectionId2( 0 );
    TUint connectionId3( 0 );
    
    TInetAddr connectionAddr;
    TInetAddr connectionAddr2;
      
    // Create 1 session, 1 stream, 1 connection
    sessionId = iMux.CreateSessionL( KTestIapId, 5000, 5100 );
    CActiveScheduler::Start();
          
    streamId = iMux.CreateStreamL( sessionId, KDefaultQos, KProtocolInetUdp );
    connectionId = iMux.CreateConnectionL( streamId, KAfInet );
    connectionAddr = iMux.LocalIPAddressL( streamId, connectionId );

    // Test connection
    streamId2 = iMux.CreateStreamL( sessionId, KDefaultQos, KProtocolInetUdp );
    connectionId2 = iMux.CreateConnectionL( streamId2, KAfInet );
    connectionAddr2 = iMux.LocalIPAddressL( streamId2, connectionId2 );
    
    iMux.RegisterIncomingConnectionObserverL( streamId, iIfStub );
    iMux.RegisterOutgoingConnectionObserverL( streamId, iIfStub );
    iMux.RegisterConnectionObserverL( streamId, iIfStub );
    iMux.RegisterMessageObserverL( streamId, iIfStub );
    
    // Activate receiving -> passivate receiving    
    iMux.SetReceivingStateL( streamId, connectionId, EStreamingStateActive );
    CActiveScheduler::Start();   
    iMux.SetReceivingStateL( streamId, connectionId, EStreamingStatePassive );
    CActiveScheduler::Start();
    
    // Activate sending -> passivate sending    
    iMux.SetSendingStateL( streamId, connectionId, connectionAddr2, EStreamingStateActive ); 
    CActiveScheduler::Start();    
    iMux.SetSendingStateL( streamId, connectionId, connectionAddr2, EStreamingStatePassive ); 
    CActiveScheduler::Start();    
  
     // Activate receiving/sending -> passivate receiving/sending    
    iMux.SetReceivingStateL( streamId, connectionId, EStreamingStateActive );
    iMux.SetSendingStateL( streamId, connectionId, connectionAddr2, EStreamingStateActive );    
    CActiveScheduler::Start();    
    CActiveScheduler::Start(); 
    
    iMux.SetSendingStateL( streamId, connectionId, connectionAddr2, EStreamingStatePassive );   
    iMux.SetReceivingStateL( streamId, connectionId, EStreamingStatePassive );;  
    CActiveScheduler::Start();   
    CActiveScheduler::Start();

     // Activate receiving/sending -> activate sending again with different destination address 
    iMux.SetReceivingStateL( streamId, connectionId, EStreamingStateActive );
    iMux.SetSendingStateL( streamId, connectionId, connectionAddr, EStreamingStateActive );
    CActiveScheduler::Start();    
    CActiveScheduler::Start();    
    
    // Set sending again
    iMux.SetSendingStateL( streamId, connectionId, connectionAddr2, EStreamingStateActive );
    CActiveScheduler::Start(); 
    iMux.SetSendingStateL( streamId, connectionId, connectionAddr2, EStreamingStateActive );
    CActiveScheduler::Start(); 
           
    // Set receiving again
    iMux.SetReceivingStateL( streamId, connectionId, EStreamingStateActive );
    CActiveScheduler::Start();
    iMux.SetReceivingStateL( streamId, connectionId, EStreamingStateActive );
    CActiveScheduler::Start(); 
     
    iMux.SetSendingStateL( streamId, connectionId, connectionAddr2, EStreamingStatePassive );  
    iMux.SetReceivingStateL( streamId, connectionId, EStreamingStatePassive );
    CActiveScheduler::Start();  
    CActiveScheduler::Start();
             
    iMux.RemoveSessionL( sessionId );    

    }

  
// ---------------------------------------------------------------------------
// CMultiplexerUdpTests::TestActivateStunUdpMediaConnectionsL
// ---------------------------------------------------------------------------
//
void CMultiplexerUdpTests::TestActivateStunUdpMediaConnectionsL()
    {
    TUint sessionId( 0 );
    
    TUint streamId( 0 );
    TUint streamId2( 0 );
      
    TUint connectionId( 0 );
    TUint connectionId2( 0 );
    TUint connectionId3( 0 );
    
    sessionId = iMux.CreateSessionL( KTestIapId, 5000, 5100 );
    CActiveScheduler::Start();
    
    streamId = iMux.CreateStreamL( sessionId, KDefaultQos, KProtocolInetUdp );
    streamId2 = iMux.CreateStreamL( sessionId, KDefaultQos, KProtocolInetUdp );
    
    // Simulate UDP fetch candidate    
    iMux.RegisterIncomingConnectionObserverL( streamId, iIfStub );
    iMux.RegisterConnectionObserverL( streamId, iIfStub );
    
    connectionId = iMux.CreateConnectionL( streamId, KAfInet );
    
    // Test connection
    connectionId2 = iMux.CreateConnectionL( streamId, KAfInet );  
    TInetAddr connectionAddress2( iMux.LocalIPAddressL( streamId, connectionId2 ) );

    /**** setup wrapper *****/
    CNATFWSocketMediaConnWrapper* mediaWrapper = CNATFWSocketMediaConnWrapper::NewL(
             iMux.PortStoreL( sessionId ).Port(), KProtocolInetUdp, streamId );
       
    iMux.RegisterMediaWrapperL( mediaWrapper );
            
    TInetAddr destinationAddress;    
    mediaWrapper->OutgoingAddr( destinationAddress );    
    __CONNECTIONMULTIPLEXER_ADDRLOG( "CMultiplexerUdpTests::TestUdpAudioL- Wrapper LOCAL ADDRESS: ", destinationAddress )

    
    mediaWrapper->SetIncomingAddrL( destinationAddress );

    TInetAddr stunServerAddress;
    stunServerAddress.SetAddress( INET_ADDR( 10,36,7,6 ) );
    stunServerAddress.SetPort( 3478 );
    
    iMux.SetReceivingStateL( streamId, connectionId, EStreamingStateActive );
    iMux.SetSendingStateL( streamId, connectionId, stunServerAddress, EStreamingStateActive );
    CActiveScheduler::Start();    
    CActiveScheduler::Start();
    
    // Binding request
    _LIT8(KUDPMessage,"UDP_Binding_Request");
    iMux.SendL( streamId, connectionId, KUDPMessage, NULL );
    
    // Setup media flow to peer
    // NATFW client
    iMux.SetReceivingStateForMediaL( streamId, connectionId, EStreamingStateActive );
    CActiveScheduler::Start();
   
    // STUN plugin, reuse connection.. connect to peer address
    iMux.SetSendingStateL( streamId, connectionId, connectionAddress2, EStreamingStateActive );

    // NATFW client
    iMux.SetSendingStateForMediaL( streamId, connectionId, EStreamingStateActive );
    CActiveScheduler::Start(); 
    CActiveScheduler::Start(); 

    iMux.SendL( streamId, connectionId, KUDPMessage, &iIfStub );
    iMux.SendL( streamId, connectionId, KUDPMessage, connectionAddress2, &iIfStub );
    CActiveScheduler::Start(); 
    CActiveScheduler::Start();   
    
    iMux.RemoveSessionL( sessionId );    
    }
  
  
  
// ---------------------------------------------------------------------------
// CMultiplexerUdpTests::TestUdpAudioL
// ---------------------------------------------------------------------------
//
void CMultiplexerUdpTests::TestUdpAudioStreamingL()
    {
    TUint sessionId( 0 );  
    TUint streamId( 0 );
    TUint streamId2( 0 );
    // UDP SESSIO
    RDebug::Print( _L("CMultiplexerUdpTests::TestUdpAudioL - start") );
    
    iSessionId = iMux.CreateSessionL( KTestIapId, 5000, 5100 );
    sessionId = iSessionId;
    CActiveScheduler::Start();
    
        
    // STREAM
    streamId = iMux.CreateStreamL( sessionId, KDefaultQos, KProtocolInetUdp );
    TUint connectionId2 = iMux.CreateConnectionL( streamId, KAfInet );
    
    // STREAM 2
    streamId2 = iMux.CreateStreamL( sessionId, KDefaultQos, KProtocolInetUdp );
    TUint connectionId = iMux.CreateConnectionL( streamId2, KAfInet );
    
    // Get local IP that receives media
    TInetAddr receiveMediaAddress;
    receiveMediaAddress = iMux.LocalIPAddressL( streamId, connectionId2 );
    
    __CONNECTIONMULTIPLEXER_ADDRLOG( "CMultiplexerUdpTests::TestUdpAudioL- LOCAL ADDRESS: ", receiveMediaAddress )

    // WRAPPER
    CNATFWSocketMediaConnWrapper* mediaWrapper = CNATFWSocketMediaConnWrapper::NewL(
            iMux.PortStoreL( sessionId ).Port(), KProtocolInetUdp, streamId );          
            
    iMux.RegisterMediaWrapperL( mediaWrapper );

    TInetAddr destinationAddress;
    //destinationAddress = receiveMediaAddress;
    //destinationAddress.SetPort( port);
    mediaWrapper->OutgoingAddr( destinationAddress );
        
    __CONNECTIONMULTIPLEXER_ADDRLOG( "CMultiplexerUdpTests::TestUdpAudioL- Wrapper LOCAL ADDRESS: ", destinationAddress )



    // NET settings value
    iNetsettings.iRemoteAddress.SetAddress( destinationAddress.Address() );
    iNetsettings.iRemoteAddress.SetPort( destinationAddress.Port() );
    iNetsettings.iRemoteRtcpPort = KDefaultRtcpPort;
    
    iNetsettings.iLocalAddress.SetAddress( KInetAddrAny );
    iNetsettings.iLocalAddress.SetPort( KDefaultRtpPort );
  
    iNetsettings.iLocalPublicAddress.SetAddress( KInetAddrAny );
    iNetsettings.iLocalRtcpPort = KDefaultRtcpPort;
    iNetsettings.iLocalPublicRtcpPort = KDefaultRtcpPort;
    iNetsettings.iMediaQosValue = KDefaultMediaQos;
    iNetsettings.iIapId = KTestIapId;
    
    TUint32 sessionId1;
    iMccInterface = CMccInterface::NewL( iIfStub );     
    User::LeaveIfError( iMccInterface->CreateSession( sessionId1 ) );
    

    // Create MCC LINK
    TUint32 linkId;
    User::LeaveIfError( iMccInterface->CreateLink( sessionId1, KMccLinkGeneral, linkId, iNetsettings ) ); 
      
    CActiveScheduler::Start();
    

    iNetsettings.iRemoteAddress.SetAddress( destinationAddress.Address() );
    iNetsettings.iRemoteAddress.SetPort( destinationAddress.Port() );
    
        
    __CONNECTIONMULTIPLEXER_ADDRLOG( "CMultiplexerUdpTests::TestUdpAudioL- iNetsettings.iRemoteAddress: ", iIfStub.NetSettings().iRemoteAddress )
    __CONNECTIONMULTIPLEXER_ADDRLOG( "CMultiplexerUdpTests::TestUdpAudioL- iNetsettings.iLocalAddress: ", iIfStub.NetSettings().iLocalAddress )

    User::LeaveIfError( iMccInterface->SetRemoteAddress( sessionId1, linkId, iNetsettings.iRemoteAddress ) );
    __CONNECTIONMULTIPLEXER_ADDRLOG( "CMultiplexerUdpTests::TestUdpAudioL- iNetsettings.iLocalAddress: ", iIfStub.NetSettings().iLocalAddress)


    RPointerArray<CMccCodecInformation> codecArray;
    CleanupResetAndDestroy< RPointerArray<CMccCodecInformation> >::PushL( codecArray );
    User::LeaveIfError( iMccInterface->GetCapabilities( codecArray ) );

    
    
    // Create upstream
    TUint32 upStream;
    TUint32 dataSinkId1;
    TUint32 dataSourceId1;
    User::LeaveIfError( iMccInterface->AddDataSink( KMccRtpSinkUid, KNullDesC8, dataSinkId1 ) );
    User::LeaveIfError( iMccInterface->AddDataSource( KUidMmfAudioInput, KNullDesC8, dataSourceId1 ) );
    codecArray[0]->SetPriorityPreference( KAudioPrefVoipAudioUplink );
    User::LeaveIfError( iMccInterface->CreateStream( sessionId1, linkId, upStream, 
        KMccAudioUplinkStream, *codecArray[0] ) );

    // Create downstream
    TUint32 downStream;
    TUint32 dataSinkId2;
    TUint32 dataSourceId2;
    User::LeaveIfError( iMccInterface->AddDataSink( KUidMmfAudioOutput, KNullDesC8, dataSinkId2 ) );
    User::LeaveIfError( iMccInterface->AddDataSource( KMccRtpSourceUid, KNullDesC8, dataSourceId2 ) );
    codecArray[0]->SetPriorityPreference( KAudioPrefVoipAudioDownlink );
    User::LeaveIfError( iMccInterface->CreateStream( sessionId1, linkId, downStream,
        KMccAudioDownlinkStream, *codecArray[0] ) );


    // Set destination address to wrapper
     __CONNECTIONMULTIPLEXER_ADDRLOG( "CMultiplexerUdpTests::TestUdpAudioL- iNetsettings.iLocalAddress: ", iIfStub.NetSettings().iLocalAddress )
    mediaWrapper->SetIncomingAddrL( iIfStub.NetSettings().iLocalAddress);

    mediaWrapper->SetReceivingStateL( EStreamingStateActive );
    mediaWrapper->SetSendingStateL( EStreamingStateActive );

    // Address resolving
    TInetAddr streamAddr;
    TInetAddr streamAddr2;
    
    streamAddr = iMux.LocalIPAddressL( streamId, 1 );
    streamAddr2 = iMux.LocalIPAddressL( streamId2, 1 );
    
    __CONNECTIONMULTIPLEXER_ADDRLOG( "CMultiplexerUdpTests::TestUdpAudioL - receiver - streamAddr: ", streamAddr )
    __CONNECTIONMULTIPLEXER_ADDRLOG( "CMultiplexerUdpTests::TestUdpAudioL- sender - streamAddr2: ", streamAddr2 )
    __CONNECTIONMULTIPLEXER_ADDRLOG( "CMultiplexerUdpTests::TestUdpAudioL- iNetsettings.iLocalAddress: ", iIfStub.NetSettings().iLocalAddress )
    
    // Connect internal two streams
    //iMux.SetReceivingStateL( streamId2, connectionId, EStreamingStateActive );  
    //iMux.SetSendingStateL( streamId2, connectionId, streamAddr, EStreamingStateActive );

    iMux.SetReceivingStateL( streamId, KMediaConnection, EStreamingStateActive );  
    iMux.SetSendingStateL( streamId, KMediaConnection, streamAddr/*2*/, EStreamingStateActive );

    
    iMux.RegisterIncomingConnectionObserverL( streamId/*2*/, iIfStub );
    iMux.RegisterOutgoingConnectionObserverL( streamId/*2*/, iIfStub );


    RDebug::Print( _L("CMultiplexerUdpTests::TestUdpAudioL - after prepare stream1") );
         
    User::LeaveIfError( iMccInterface->PrepareStream( sessionId1, linkId, upStream ) );
    CActiveScheduler::Start();
   CActiveScheduler::Start();    

    RDebug::Print( _L("CMultiplexerUdpTests::TestUdpAudioL - before prepare stream2") );
    // Prepare and start up and downstream  
    User::LeaveIfError( iMccInterface->PrepareStream( sessionId1, linkId, downStream ) );
    CActiveScheduler::Start();
    CActiveScheduler::Start();
        
    RDebug::Print( _L("CMultiplexerUdpTests::TestUdpAudioL - after start stream1") );
       
    User::LeaveIfError( iMccInterface->StartStream( sessionId1, linkId, upStream ) );
    CActiveScheduler::Start();
    CActiveScheduler::Start();
    

    
    RDebug::Print( _L("CMultiplexerUdpTests::TestUdpAudioL - after prepare stream2") );
   
    User::LeaveIfError( iMccInterface->StartStream( sessionId1, linkId, downStream ) );
    CActiveScheduler::Start();
    CActiveScheduler::Start();

    RDebug::Print( _L("CMultiplexerUdpTests::TestUdpAudioL - end") );   
    CleanupStack::PopAndDestroy( &codecArray );
    //iMux.RemoveStreamL( streamId );
    }
