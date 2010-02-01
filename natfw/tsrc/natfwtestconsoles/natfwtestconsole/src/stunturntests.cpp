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
#include "natfwconnectivityapi.h"
#include <badesca.h>
#include "stunturntests.h"
#include "natfwtestconsolestubs.h"
#include "testdefinitions.hrh"
#include "mnatfwsocketmediaconnwrapper.h"
#include "wrongserversettings.h"

const TInt KDefaultQos = 46;
const TUint32 KIncomingAddr = INET_ADDR( 0,0,0,0 );
const TUint KIncomingPort = 100;
const TUint32 KTestAddress = INET_ADDR( 193,65,0,0 );
const TUint KTestServerPort = 6000;
const TUint KTestServer2Port = 6100;
const TInt KTimeoutTime = 2000000;
const TUint KRunningTime = 20000000;
const TUint KPluginLoadTime = 1000000;
const TInt KDefaultGranularity = 1;


// ======== MEMBER FUNCTIONS =================================================

// ---------------------------------------------------------------------------
// CStunTurnTestsInterface::CStunTurnTestsInterface
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CStunTurnTests::CStunTurnTests(
    CConsoleBase& aConsole,
    CNATFWConnectivityApi& aNat,
    CNATConnFWTestConsoleStubs& aIFStub,
    TUint aIapId,
    TUint aProtocol ) :
    iConsole( aConsole ), iNat( aNat ), iIfStub( aIFStub ), iTestIapId( aIapId ),
    iProtocol( aProtocol ), iNotificationIsOn( ETrue )
    {
    }
    
// ---------------------------------------------------------------------------
// CStunTurnTests::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CStunTurnTests::ConstructL( const TDesC8& aDomain,
    const CDesC8Array& aPlugins )
    {
    iTestDomain = aDomain.AllocL(); 
    
    
    iPlugins = new ( ELeave ) CDesC8ArrayFlat( KDefaultGranularity );
    
    TInt count = aPlugins.Count();
    
    for ( TInt i( 0 ); i < count; i++ )
        {
        iPlugins->AppendL( aPlugins[i] );
        }
    }
    
// ---------------------------------------------------------------------------
// CStunTurnTests::NewL
// Static constructor.
// ---------------------------------------------------------------------------
//   
CStunTurnTests* CStunTurnTests::NewL( CConsoleBase& aConsole,
    CNATFWConnectivityApi& aNat, CNATConnFWTestConsoleStubs& aIFStub,
    const TDesC8& aDomain, TUint aIapId, TUint aProtocol,
    const CDesC8Array& aPlugins )
    {
    CStunTurnTests* self 
        = new ( ELeave ) CStunTurnTests(
            aConsole, aNat, aIFStub, aIapId, aProtocol );

    CleanupStack::PushL( self );
    self->ConstructL( aDomain, aPlugins );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CStunTurnTests::~CStunTurnTests
// Destructor.
// ---------------------------------------------------------------------------
//
CStunTurnTests::~CStunTurnTests()
    {
    delete iTestDomain;
    delete iPlugins;
    }

// ---------------------------------------------------------------------------
// CStunTurnTests::DoRunInterfaceTestsL
// 
// ---------------------------------------------------------------------------
//
void CStunTurnTests::DoRunStunTurnTestsL()
    {
    TInt err( KErrNone );
    
    iConsole.Printf( _L("Create session test") );
    RDebug::Print( _L("\n\nNAT_TEST_START: Create session") );
    //__UHEAP_MARK; 
    TRAP( err, TestCreateSessionL() );
    //__UHEAP_MARKEND;
    RDebug::Print( _L("\nNAT_TEST_STOP: Create session, ERR: %d"), err );
    PrintTestResult( err );
    
    err = KErrNone;
    RDebug::Print( _L("\n\nNAT_TEST_START: Create stream") );
    iConsole.Printf( _L("Create stream test") );
    __UHEAP_MARK;
    TRAP( err, TestCreateStreamL() );
    RDebug::Print( _L("\nNAT_TEST_STOP: Create stream, ERR: %d"), err );
    PrintTestResult( err );
    __UHEAP_MARKEND;
   
    err = KErrNone;
    RDebug::Print( _L("\n\nNAT_TEST_START: Create wrapper") );
    iConsole.Printf( _L("Create wrapper test") );
    __UHEAP_MARK;
    TRAP( err, TestCreateWrapperL() ); 
    RDebug::Print( _L("\nNAT_TEST_STOP: Create wrapper, ERR: %d"), err );
    PrintTestResult( err );
    __UHEAP_MARKEND;
    
    err = KErrNone;
    RDebug::Print( _L("\n\nNAT_TEST_START: Set incoming Address to wrapper") );
    iConsole.Printf( _L("Set Incoming Address to wrapper test") );
    __UHEAP_MARK;
    TRAP( err, SetupTestL() );
    if ( !err )
        {
        __UHEAP_MARK;
        TRAP( err, TestSetIncomingAddrL() );
        __UHEAP_MARKEND;
        TeardownTest();
        }
    else
        {
        RDebug::Print( _L("\nTEST PRINT: Setup fails, ERR: %d"), err );
        }
    RDebug::Print( _L("\nNAT_TEST_STOP: Set incoming Address to wrapper, ERR: %d"), err );
    PrintTestResult( err );
    __UHEAP_MARKEND;
    
    err = KErrNone;
    RDebug::Print( _L("\n\nNAT_TEST_START: Outgoing address") );
    iConsole.Printf( _L("Get outgoing address from wrapper test") );
    __UHEAP_MARK;
    TRAP( err, SetupTestL() );
    if ( !err )
        {
        __UHEAP_MARK;
        TRAP( err, TestOutgoingAddrL() );
        __UHEAP_MARKEND;
        TeardownTest();
        }
    else
        {
        RDebug::Print( _L("\nTEST PRINT: Setup fails, ERR: %d"), err );
        }
    RDebug::Print( _L("\nNAT_TEST_STOP: Outgoing address, ERR: %d"), err );
    PrintTestResult( err );
    __UHEAP_MARKEND;
    
    err = KErrNone;
    RDebug::Print( _L("\n\nNAT_TEST_START: Fetch candidate") );
    iConsole.Printf( _L("Fetch Candidate test") );
    __UHEAP_MARK;
    TRAP( err, TestFetchCandidateL() );
    RDebug::Print( _L("\nNAT_TEST_STOP: Fetch candidate, ERR: %d"), err );
    PrintTestResult( err );
    __UHEAP_MARKEND;
    
    err = KErrNone;
    RDebug::Print( _L("\n\nNAT_TEST_START: Fetch candidate: wrong server address") );
    iConsole.Printf( _L("Fetch Candidate wrong server address test") );
    __UHEAP_MARK;
    TRAP( err, SetupWrongServerAddressTestL() );
    if ( !err )
        {
        __UHEAP_MARK;
        TRAP( err, TestFetchCandidateL() );
        __UHEAP_MARKEND;
        }
    else
        {
        RDebug::Print( _L("\nTEST PRINT: Setup fails, ERR: %d"), err );
        } 
    TeardownWrongServerAddressTest();
    RDebug::Print( _L("\nNAT_TEST_STOP: Fetch candidate wrong server address, ERR: %d"), err );
    PrintTestResult( err );
    __UHEAP_MARKEND;
    
    
    err = KErrNone;
    RDebug::Print( _L("\n\nNAT_TEST_START: Set sending status ") );
    iConsole.Printf( _L("Set Sending Status test") );
    __UHEAP_MARK;
    TRAP( err, SetupTest2L() );
    if ( !err )
        {
        __UHEAP_MARK;
         if ( KProtocolInetUdp == iProtocol )
            {
            TRAP( err, TestSetSendingStatusUDPL() );
            }
        else
            {
            TRAP( err, TestSetSendingStatusTCPL() );
            }
        __UHEAP_MARKEND;
        }
    else
        {
        RDebug::Print( _L("\nTEST PRINT: Setup fails, ERR: %d"), err );
        }
    TeardownTest();
    RDebug::Print( _L("\nNAT_TEST_STOP: Set sending status, ERR: %d"), err );
    PrintTestResult( err );
    __UHEAP_MARKEND;
  
    err = KErrNone;
    RDebug::Print( _L("\n\nNAT_TEST_START: Set receiving status") );
    iConsole.Printf( _L("Set Receiving Status test") );
    __UHEAP_MARK;
    TRAP( err, SetupTest2L() );
    if ( !err )
        {
        __UHEAP_MARK;
        if ( KProtocolInetUdp == iProtocol )
            {
            TRAP( err, TestSetReceivingStatusUDPL() );
            }
        else
            {
            TRAP( err, TestSetReceivingStatusTCPL() );
            }
        __UHEAP_MARKEND;
        TeardownTest();
        }
    else
        {
        RDebug::Print( _L("\nTEST PRINT: Setup fails, ERR: %d"), err );
        }
    
    RDebug::Print( _L("\nNAT_TEST_STOP: Set receiving status, ERR: %d"), err );
    PrintTestResult( err );
    __UHEAP_MARKEND;
    }
  

// ---------------------------------------------------------------------------
// CStunTurnTests::SetupTestL
// Create session, stream and wrapper
// ---------------------------------------------------------------------------
//
void CStunTurnTests::SetupTestL()
    {
    RDebug::Print( _L( "\nTEST PRINT: CStunTurnTests::SetupTest - Start" ) ); 
    
    TInt loadedPluginInd;
    
    iSessionId = 0;
    iStreamId = 0;
    
    iSessionId = iNat.CreateSessionL( iTestIapId, iTestDomain->Des() );
    iIfStub.StartActiveSchedulerL();
    
    iNat.LoadPluginL( iSessionId, *iPlugins, loadedPluginInd );
    iIfStub.StartActiveSchedulerL( KPluginLoadTime );
        
    iStreamId = iNat.CreateStreamL( iSessionId, iProtocol, KDefaultQos );
    
    iWrapper = &iNat.CreateWrapperL( iSessionId, iStreamId );
    	
    RDebug::Print( _L( "\nTEST PRINT: CStunTurnTests::SetupTest - End" ) );
    }


// ---------------------------------------------------------------------------
// CStunTurnTests::SetupTest2L
// FetchCandidate
// ---------------------------------------------------------------------------
//
void CStunTurnTests::SetupTest2L()
    {
    RDebug::Print( _L( "\nTEST PRINT: CStunTurnTests::SetupTest2 - Start" ) );
    TInt error;
    SetupTestL();

    iNat.FetchCandidateL( iSessionId, iStreamId, KAfInet );
    error = iIfStub.StartActiveSchedulerL( KRunningTime );
    
    __ASSERT_ALWAYS( !error, User::Leave( error ) );
    
    RDebug::Print( _L( "\nTEST PRINT: CStunTurnTests::SetupTest2 - End" ) );
    }


// ---------------------------------------------------------------------------
// CStunTurnTests::SetupWrongServerAddressTestL
// Create session, stream and wrapper
// ---------------------------------------------------------------------------
//
void CStunTurnTests::SetupWrongServerAddressTestL()
    {
    iIfStub.SetFailureTest( ETrue );
    iWrongServerSettings = CWrongServerSettings::NewL( *iTestDomain );
    }


// ---------------------------------------------------------------------------
// CStunTurnTests::TeardownWrongServerAddressTest
// Create session, stream and wrapper
// ---------------------------------------------------------------------------
//
void CStunTurnTests::TeardownWrongServerAddressTest()
    {
    delete iWrongServerSettings;
    iWrongServerSettings = 0;
    iIfStub.SetFailureTest( EFalse );
    }
    
    
// ---------------------------------------------------------------------------
// CStunTurnTests::TeardownTest
// Close Session and Stream
// ---------------------------------------------------------------------------
//
void CStunTurnTests::TeardownTest()
    {
    RDebug::Print( _L( "\nTEST CASE: CStunTurnTests::TeardownTest" ) );
    
    iNat.CloseStreamL( iSessionId, iStreamId );
    iNat.CloseSessionL( iSessionId );
    }
    
      
// ---------------------------------------------------------------------------
// CStunTurnTests::TestCreateSessionL
// ---------------------------------------------------------------------------
//
void CStunTurnTests::TestCreateSessionL()
    {
    TUint sessionId( 0 );
    TUint sessionId2( 0 );
    
    RDebug::Print(
        _L( "\nTEST CASE: Create session with invalid IapID: 18" ) );
    // Creation fails
    iIfStub.SetFailureTest( ETrue );
    
    sessionId = iNat.CreateSessionL( 18, iTestDomain->Des() );

    iIfStub.StartActiveSchedulerL();
    
    iIfStub.SetFailureTest( EFalse );
    
    iNat.CloseSessionL( sessionId );
    
    RDebug::Print( _L( "\nTEST CASE: Create session 1 with IapID: %d" ),
        iTestIapId );
    
    __UHEAP_MARK; 
    
    sessionId = iNat.CreateSessionL( iTestIapId, iTestDomain->Des() );

    iIfStub.StartActiveSchedulerL();
    
    RDebug::Print( _L( "\nTEST CASE: Create session 2 with IapID: %d" ),
        iTestIapId );
    sessionId2 = iNat.CreateSessionL( iTestIapId, iTestDomain->Des() );
    
    iIfStub.StartActiveSchedulerL();
   
    RDebug::Print( _L( "TEST PRINT: CStunTurnTests::TestCreateSessionL,\
        close session 1" ) );
    iNat.CloseSessionL( sessionId );
    
    RDebug::Print( _L( "TEST PRINT: CStunTurnTests::TestCreateSessionL, \
        close session 2" ) );
    iNat.CloseSessionL( sessionId2 );
    
    __UHEAP_MARKEND;
    
    RDebug::Print(
        _L( "\nTEST CASE: Create session without waiting call back" ) );
    
    __UHEAP_MARK; 
    
    sessionId = iNat.CreateSessionL( iTestIapId, iTestDomain->Des() );
    
    RDebug::Print( _L( "TEST PRINT: CStunTurnTests::TestCreateSessionL, \
        close session" ) );
    iNat.CloseSessionL( sessionId );
    
    __UHEAP_MARKEND;
    }
    
    
// ---------------------------------------------------------------------------
// CStunTurnTests::TestCreateStreamL
// ---------------------------------------------------------------------------
//
void CStunTurnTests::TestCreateStreamL()
    {
    TUint sessionId( 0 );
    TUint sessionId2( 0 );
    TUint streamId( 0 );
    TUint streamId2( 0 );

    sessionId = iNat.CreateSessionL( iTestIapId, iTestDomain->Des() );
    
    iIfStub.StartActiveSchedulerL();
    
    RDebug::Print( _L( "\nTEST CASE: Create stream 1 in the session 1" ) );
    streamId = iNat.CreateStreamL( sessionId, iProtocol, KDefaultQos );
    
    RDebug::Print( _L( "\nTEST CASE: Create stream 2 in the session 1" ) );
    streamId2 = iNat.CreateStreamL( sessionId, iProtocol, KDefaultQos );
    
    RDebug::Print( _L( "TEST PRINT: CStunTurnTests::TestCreateStreamL, close stream 1" ) );
    iNat.CloseStreamL( sessionId, streamId );
    
    sessionId2 = iNat.CreateSessionL( iTestIapId, iTestDomain->Des() );
    
    iIfStub.StartActiveSchedulerL();
    
    RDebug::Print( _L( "\nTEST CASE: Create stream 3 in the session 2" ) ); 
    streamId = iNat.CreateStreamL( sessionId2, iProtocol, KDefaultQos );
    
    RDebug::Print( _L( "TEST PRINT: CStunTurnTests::TestCreateSessionL, close stream 3" ) );
    iNat.CloseStreamL( sessionId2, streamId );
    
    RDebug::Print( _L( "TEST PRINT: CStunTurnTests::TestCreateSessionL, close stream 2" ) );
    iNat.CloseStreamL( sessionId, streamId2 );
    
    RDebug::Print( _L( "TEST PRINT: CStunTurnTests::TestCreateSessionL, close session 1" ) );
    iNat.CloseSessionL( sessionId );
    
    RDebug::Print( _L( "TEST PRINT: CStunTurnTests::TestCreateSessionL, close session 2" ) );
    iNat.CloseSessionL( sessionId2 );
    }


// ---------------------------------------------------------------------------
// CStunTurnTests::TestCreateWrapperL
// ---------------------------------------------------------------------------
//
void CStunTurnTests::TestCreateWrapperL()
    {
    TUint sessionId( 0 );
    TUint streamId( 0 );
    
    MNATFWSocketMediaConnWrapper* wrapperIF;
    
    RDebug::Print( _L( "TEST PRINT: Create session and stream" ) ); 
    sessionId = iNat.CreateSessionL( iTestIapId, iTestDomain->Des() );
   
    iIfStub.StartActiveSchedulerL();
    
    streamId = iNat.CreateStreamL( sessionId, iProtocol, KDefaultQos );
    
    RDebug::Print( _L( "\nTEST CASE: Create wrapper" ) );
    wrapperIF = &iNat.CreateWrapperL( sessionId, streamId );
    
    if ( wrapperIF->StreamId() != streamId )
        {
        RDebug::Print( _L( "TEST PRINT: CStunTurnTests::TestCreateWrapperL, streamId is not equal" ) );
        User::Leave( KErrCompletion );
        }
    RDebug::Print( _L( "TEST PRINT: Close stream and session" ) );    
    iNat.CloseStreamL( sessionId, streamId );
    iNat.CloseSessionL( sessionId );
    }


// ---------------------------------------------------------------------------
// CStunTurnTests::TestSetIncomingAddrL
// ---------------------------------------------------------------------------
//
void CStunTurnTests::TestSetIncomingAddrL()
    {
    RDebug::Print( _L( "\nTEST CASE: Set Incoming Address" ) );
    
    if ( KProtocolInetTcp == iProtocol )
        {
        RDebug::Print( _L( "\nTEST CASE: Set Incoming Address - TCP" ) );
        iNotificationIsOn = EFalse; 
        CTestServer* server = CTestServer::NewLC( this );
        CTestClient* client = CTestClient::NewLC( this );
        
        TInetAddr addr( KInetAddrLoop, KTestServerPort );
        server->OpenL( addr, KTimeoutTime );
        
        iWrapper->SetIncomingAddrL( addr );
        //iWrapper->SetupTcpConnectionL( ETcpSetupActive, *this );
        
        iIfStub.StartActiveSchedulerL( KRunningTime );
        
        iNotificationIsOn = ETrue;
        CleanupStack::PopAndDestroy( client );
        CleanupStack::PopAndDestroy( server );
        }
    else
        {
        RDebug::Print( _L( "\nTEST CASE: Set Incoming Address - UDP" ) );
        iWrapper->SetIncomingAddrL( TInetAddr( KIncomingAddr, KIncomingPort ) );
        }
    }


// ---------------------------------------------------------------------------
// CStunTurnTests::TestOutgoingAddrL
// ---------------------------------------------------------------------------
//    
void CStunTurnTests::TestOutgoingAddrL()
    {
    TInetAddr inetAddr;
    TBuf<40> buffer;
    
    RDebug::Print( _L( "\nTEST CASE: Get outgoing address" ) );
    iWrapper->OutgoingAddr( inetAddr );
    
    inetAddr.Output( buffer );
    
    RDebug::Print( _L("TEST PRINT: CStunTurnTests::TestOutgoingAddrL, wrapper outgoing Address: %S:%d "), &buffer, inetAddr.Port() );
    if ( buffer.Length() == 0 )
        {
        User::Leave( KErrCompletion );
        }
    }


// ---------------------------------------------------------------------------
// CStunTurnTests::TestFetchCandidateL
// ---------------------------------------------------------------------------
//
void CStunTurnTests::TestFetchCandidateL()
    {
    TUint sessionId( 0 );
    TUint sessionId2( 0 );
    
    TUint streamId( 0 );
    TUint streamId2( 0 );
    TUint streamId3( 0 );
    
    TInt error;
    TInt loadedPluginInd( 0 );
     
    RDebug::Print( _L( "TEST PRINT: Create session" ) );
    sessionId = iNat.CreateSessionL( iTestIapId, iTestDomain->Des() );
    
    iIfStub.StartActiveSchedulerL();
    
    RDebug::Print( _L( "TEST PRINT: Create stream" ) );
    streamId = iNat.CreateStreamL( sessionId, iProtocol, KDefaultQos );
    
    RDebug::Print( _L( "TEST PRINT: Load plugin" ) );   
    iNat.LoadPluginL( sessionId, *iPlugins, loadedPluginInd );
    iIfStub.StartActiveSchedulerL( KPluginLoadTime );
    
    RDebug::Print( _L( "\nTEST CASE: Fetch Candidate to the stream" ) );
    TRAPD( err, iNat.FetchCandidateL( sessionId, streamId, KAfInet ) );
    if ( err )
        {
        RDebug::Print( _L( "\nTEST PRINT: CNATFWClient::FetchCandidateL LEAVE %d" ), err );
        iNat.CloseStreamL( sessionId, streamId );
        iNat.CloseSessionL( sessionId );
        User::Leave( err );
        }
        
    error = iIfStub.StartActiveSchedulerL( /*KRunningTime*/ );
    
    RDebug::Print( _L( "TEST PRINT: Close the stream and the session" ) );
    iNat.CloseStreamL( sessionId, streamId );
    iNat.CloseSessionL( sessionId );
    
    __ASSERT_ALWAYS( !error, User::Leave( error ));
 
    RDebug::Print( _L( "TEST PRINT: Create session 1" ) );
    sessionId = iNat.CreateSessionL( iTestIapId, iTestDomain->Des() );
    
    iIfStub.StartActiveSchedulerL();
    
    RDebug::Print( _L( "TEST PRINT: Load plugin to session 1" ) );   
    iNat.LoadPluginL( sessionId, *iPlugins, loadedPluginInd );
    iIfStub.StartActiveSchedulerL( KPluginLoadTime );
    
    RDebug::Print( _L( "TEST PRINT: Create stream 1 to the session 1" ) );
    streamId = iNat.CreateStreamL( sessionId, iProtocol, KDefaultQos );

    RDebug::Print( _L( "\nTEST CASE: Fetch Candidate to the stream 1 in the session 1" ) );
    TRAP( err, iNat.FetchCandidateL( sessionId, streamId, KAfInet ) );
    if( err )
        {
        RDebug::Print( _L( "\nTEST PRINT: CNATFWClient::FetchCandidateL LEAVE %d" ), err );
        iNat.CloseStreamL( sessionId, streamId );
        iNat.CloseSessionL( sessionId );
        User::Leave( err );
        }
    
    iIfStub.StartActiveSchedulerL();
    
    RDebug::Print( _L( "TEST PRINT: Create stream 2 to the session 1" ) );
    streamId2 = iNat.CreateStreamL( sessionId, iProtocol, KDefaultQos );
    
    RDebug::Print( _L( "\nTEST CASE: Fetch Candidate to the stream 2 in the session 1" ) );
    
    TRAP( err, iNat.FetchCandidateL( sessionId, streamId2, KAfInet ) );
    if( err )
        {
        RDebug::Print( _L( "\nTEST PRINT: CNATFWClient::FetchCandidateL LEAVE %d" ), err );
        iNat.CloseStreamL( sessionId, streamId );
        iNat.CloseStreamL( sessionId, streamId2 );
        iNat.CloseSessionL( sessionId );
        User::Leave( err );
        }
    
    error = iIfStub.StartActiveSchedulerL( KRunningTime );
    
    if( !error )
        {
        RDebug::Print( _L( "TEST PRINT: Create session 2" ) );
        sessionId2 = iNat.CreateSessionL( iTestIapId, iTestDomain->Des() );
        
        iIfStub.StartActiveSchedulerL();
        
        RDebug::Print( _L( "TEST PRINT: Load plugin to session 2" ) );   
        iNat.LoadPluginL( sessionId2, *iPlugins, loadedPluginInd );
        iIfStub.StartActiveSchedulerL( KPluginLoadTime );
        
        RDebug::Print( _L( "TEST PRINT: Create stream 3 to the session 2" ) );
        streamId3 = iNat.CreateStreamL( sessionId2, iProtocol, KDefaultQos );
        
        RDebug::Print( _L( "\nTEST CASE: Fetch Candidate to the stream 3 in the session 2" ) );
        
        TRAP( err, iNat.FetchCandidateL( sessionId2, streamId3, KAfInet ) );
        if( err )
        {
        RDebug::Print( _L( "\nTEST PRINT: CNATFWClient::FetchCandidateL LEAVE %d" ), err );
        iNat.CloseStreamL( sessionId2, streamId3 );
        iNat.CloseSessionL( sessionId2 );
        iNat.CloseStreamL( sessionId, streamId );
        iNat.CloseStreamL( sessionId, streamId2 );
        iNat.CloseSessionL( sessionId );
        User::Leave( err );
        }
        
        iIfStub.StartActiveSchedulerL( KRunningTime ); 
        
        RDebug::Print( _L( "TEST PRINT: Close stream 3 and session 2" ) );
        iNat.CloseStreamL( sessionId2, streamId3 );
        iNat.CloseSessionL( sessionId2 );
        }
        
    RDebug::Print( _L( "TEST PRINT: Close streams 1 & 2 and session 1" ) );
    iNat.CloseStreamL( sessionId, streamId );
    iNat.CloseStreamL( sessionId, streamId2 );

    iNat.CloseSessionL( sessionId );
    
    __ASSERT_ALWAYS( !error, User::Leave( error ));
    }


// ---------------------------------------------------------------------------
// CStunTurnTests::TestSetSendingStatusUDPL
// ---------------------------------------------------------------------------
//
void CStunTurnTests::TestSetSendingStatusUDPL()
    {
    TInetAddr inetAddr;

	iWrapper->OutgoingAddr( inetAddr );
	iWrapper->SetIncomingAddrL( inetAddr );
	   	
	TInetAddr destination( KTestAddress, KTestServerPort );
	
    RDebug::Print( _L( "\nTEST CASE: Set Sending Status Active" ) );
    
	iNat.SetSendingStateL( iIfStub.LocalCandidateL(), EStreamingStateActive,
        destination );
    	
	iIfStub.StartActiveSchedulerL( KRunningTime );
	
	RDebug::Print( _L( "\nTEST CASE: Set Sending Status Passive" ) );
	
    iNat.SetSendingStateL( iIfStub.LocalCandidateL(), EStreamingStatePassive,
        destination );
    	
    iIfStub.StartActiveSchedulerL( KRunningTime );
    }

// ---------------------------------------------------------------------------
// CStunTurnTests::TestSetSendingStatusTCPL
// ---------------------------------------------------------------------------
//
void CStunTurnTests::TestSetSendingStatusTCPL()
    {
    TInetAddr inetAddr;
    TBuf<40> buffer;
    
    CTestClient* client = CTestClient::NewLC( this );

    // testserver to wrapper
    CTestServer* server = CTestServer::NewLC( this );
    
    TInetAddr addr( KInetAddrAny, KTestServerPort );
    
    // stop scheduler when server timer runs out
    server->OpenL( addr, KTimeoutTime );

    iNotificationIsOn = EFalse;
    
    TInetAddr testServerAddr;
    client->ResolveLocalAddrL( testServerAddr, iTestIapId );
    testServerAddr.SetPort( KTestServerPort );

    iWrapper->SetIncomingAddrL( testServerAddr );
	
	iIfStub.StartActiveSchedulerL( KRunningTime );

    iNotificationIsOn = ETrue;

	// testserver to natfw 
	CTestServer* server2 = CTestServer::NewLC( this );
 
    TInetAddr addr2( KInetAddrAny, KTestServer2Port );
    	
	server2->OpenL( addr2, KTimeoutTime );
	
	TInetAddr destination;
	client->ResolveLocalAddrL( destination, iTestIapId );
	destination.SetPort( KTestServer2Port );
    destination.Output( buffer );
    
    RDebug::Print( _L( "CStunTurnTests::TestSetSendingStatusTCPL; ADDR: %S PORT: %d" ),
        &buffer, destination.Port() );
    
    // set sending status active
    RDebug::Print( _L( "\nTEST CASE: Set Sending Status Active" ) );
    
	iNat.SetSendingStateL( iIfStub.LocalCandidateL(), EStreamingStateActive,
        destination );
    	
	iIfStub.StartActiveSchedulerL( KRunningTime );
	
	server2->Cancel();

	// set sending status passive
	RDebug::Print( _L( "\nTEST CASE: Set Sending Status Passive" ) );
	
    iNat.SetSendingStateL( iIfStub.LocalCandidateL(), EStreamingStatePassive,
        destination );
    	
    iIfStub.StartActiveSchedulerL( KRunningTime );

    CleanupStack::PopAndDestroy( server2 );
    CleanupStack::PopAndDestroy( server );
    CleanupStack::PopAndDestroy( client );
    }
        
// ---------------------------------------------------------------------------
// CStunTurnTests::TestSetReceivingStatusUDPL
// ---------------------------------------------------------------------------
//
void CStunTurnTests::TestSetReceivingStatusUDPL()
    { 
    TInetAddr addr( KTestAddress, KTestServerPort );

    iWrapper->SetIncomingAddrL( addr );
	
    RDebug::Print( _L( "\nTEST CASE: Set Receiving Status Active" ) );
    
    iNat.SetReceivingStateL( iIfStub.LocalCandidateL(),
        EStreamingStateActive );
        
    iIfStub.StartActiveSchedulerL( KRunningTime );
    
    RDebug::Print( _L( "\nTEST CASE: Set Receiving Status Passive" ) );
    	
    iNat.SetReceivingStateL( iIfStub.LocalCandidateL(),
        EStreamingStatePassive );
    
    iIfStub.StartActiveSchedulerL( KRunningTime );
    }


// ---------------------------------------------------------------------------
// CStunTurnTests::TestSetReceivingStatusTCPL
// ---------------------------------------------------------------------------
//
void CStunTurnTests::TestSetReceivingStatusTCPL()
    {
    // Connect wrapper to test server
    CTestServer* server = CTestServer::NewLC( this );

    TInetAddr addr( KInetAddrAny, KTestServerPort );
    
    // stop scheduler when clients timer runs out	
	server->OpenL( addr, KTimeoutTime );
	
	iNotificationIsOn = EFalse;
	
    CTestClient* client = CTestClient::NewLC( this );
    
    TInetAddr testServerAddr;
    client->ResolveLocalAddrL( testServerAddr, iTestIapId );
    testServerAddr.SetPort( KTestServerPort );

    iWrapper->SetIncomingAddrL( testServerAddr );

	iIfStub.StartActiveSchedulerL( KRunningTime );
    
    iNotificationIsOn = ETrue;
    
    
    // Set receiving status active
    RDebug::Print( _L( "\nTEST CASE: Set Receiving Status Active" ) );
    
    iNat.SetReceivingStateL( iIfStub.LocalCandidateL(),
        EStreamingStateActive );
    
    // connect test client to natfw
    TInetAddr incomingAddr;
    
    client->ResolveLocalAddrL( incomingAddr, iTestIapId );
    incomingAddr.SetPort( 5000 );

    TBuf<40> buffer;
    incomingAddr.Output( buffer );
    RDebug::Print(
        _L( "CStunTurnTests::TestSetReceivingStatusTCPL; ADDR: %S PORT: %d" ),
        &buffer ,incomingAddr.Port() );
    
    client->OpenL( iTestIapId );
    
    iIfStub.StartActiveSchedulerL();
    
    // stop scheduler when clients timer runs out
    client->ConnectL( incomingAddr, KTimeoutTime );
    
    iNotificationIsOn = EFalse;
    
    iIfStub.StartActiveSchedulerL( KRunningTime );
    
    iNotificationIsOn = ETrue;
    
    // Set receiving status passive
    RDebug::Print( _L( "\nTEST CASE: Set Receiving Status Passive" ) );
    	
    iNat.SetReceivingStateL( iIfStub.LocalCandidateL(),
        EStreamingStatePassive );
    
    iIfStub.StartActiveSchedulerL( KRunningTime );

    CleanupStack::PopAndDestroy( client );
    CleanupStack::PopAndDestroy( server );
    }

    
// ---------------------------------------------------------------------------
// CStunTurnTests::PrintResult
// ---------------------------------------------------------------------------
//
void CStunTurnTests::PrintTestResult( TInt aError )
    {
    if ( KErrNone == aError )
        {
        iConsole.Printf( _L("...Ready \n") );
        }
    else
        {
        iConsole.Printf( _L("...Error: %d\n"), aError );
        }
    }

// ---------------------------------------------------------------------------
// CStunTurnTests::Notify
// from MTestServerObserver
// ---------------------------------------------------------------------------
//    
void CStunTurnTests::Notify( MTestServerObserver::TServerNotify aNotify, TInt aError )
    {
    RDebug::Print( _L( "TEST PRINT: CStunTurnTests::Notify, server Notify: %d, Error: %d\n" ),
        aNotify ,aError );
    if ( KErrNone != aError || aNotify == EShutdownTimer )
        {
        if ( iNotificationIsOn )
            {
            iConsole.Printf( _L( "\nServer Notify: %d, Error: %d\n" ),aNotify ,aError );
            }
        iIfStub.StopActiveScheduler();
        }
    }


// ---------------------------------------------------------------------------
// CStunTurnTests::Notify
// from MTestClientNotifier
// ---------------------------------------------------------------------------
//    
void CStunTurnTests::Notify( MTestClientNotifier::TClientNotify aNotify, TInt aError )
    {
    RDebug::Print( _L( "TEST PRINT: CStunTurnTests::Notify, client Notify: %d, Error: %d\n" ),
        aNotify ,aError );
    if ( KErrNone != aError || aNotify == MTestClientNotifier::ETimerOccured )
        {
        if ( iNotificationIsOn )
            {
            iConsole.Printf( _L( "\nClient Notify: %d, Error: %d\n" ),aNotify ,aError );
            }
        iIfStub.StopActiveScheduler();
        }
    else if ( MTestClientNotifier::EOpened == aNotify )
        {
        iIfStub.StopActiveScheduler();
        }
    else
        {
        }
    }

