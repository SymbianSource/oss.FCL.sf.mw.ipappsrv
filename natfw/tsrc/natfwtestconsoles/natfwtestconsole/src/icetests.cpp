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
#include "natfwcandidate.h"
#include "natfwcandidatepair.h"
#include <badesca.h>

#include "icetests.h"
#include "iceteststubs.h"
#include "testdefinitions.hrh"
#include "mnatfwsocketmediaconnwrapper.h"
#include "natfwcredentials.h"

const TInt KDefaultQos = 46;
const TInt KStreamCollectionId = 1;
const TInt KMediaComponentId = 1;
//_LIT8( KUsername, "username" );
//_LIT8( KPassword, "password" );
//const TUint32 KDestAddr = INET_ADDR( 0,0,0,0 );
//const TUint KDestPort = 33000;
//_LIT8( KUDPMessage, "UDP_Message" );
//_LIT8( KIncomingAddress, "10.10.10.10" );

const TInt KFetchingTime = 10000000; // 10 sec.



// ======== MEMBER FUNCTIONS =================================================

// ---------------------------------------------------------------------------
// CICETests::CICETests
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CICETests::CICETests(
    CConsoleBase& aConsole,
    CNATFWConnectivityApi& aNat,
    CICETestStubs& aIFStub,
    TUint aIapId,
    TUint32 aDestAddress, 
    TUint aDestPort,
    TUint aProtocol ) :
    iConsole( aConsole ), iNat( aNat ), iIfStub( aIFStub ), iTestIapId( aIapId ),
    iDestAddress( aDestAddress ), iDestPort( aDestPort ), iProtocol( aProtocol )
    {
    }


// ---------------------------------------------------------------------------
// CICETests::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CICETests::ConstructL( const TDesC8& aDomain )
    {
    iTestDomain = aDomain.AllocL();
    iIfStub.SetObserver( this );
    }


// ---------------------------------------------------------------------------
// CICETests::NewL
// Static constructor.
// ---------------------------------------------------------------------------
//   
CICETests* CICETests::NewL( CConsoleBase& aConsole,
    CNATFWConnectivityApi& aNat, CICETestStubs& aIFStub,
    const TDesC8& aDomain, TUint aIapId, TUint32 aDestAddress,
    TUint aDestPort, TUint aProtocol )
    {
    CICETests* self 
        = new ( ELeave ) CICETests( aConsole, aNat, aIFStub, aIapId,
        aDestAddress, aDestPort, aProtocol );

    CleanupStack::PushL( self );
    self->ConstructL( aDomain );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CICETests::~CICETests
// Destructor.
// ---------------------------------------------------------------------------
//
CICETests::~CICETests()
    {
    delete iTestDomain;
    TInt count = iCandidateArray.Count();
    for ( TInt i( 0 ); i < count; i++  )
        {
        delete iCandidateArray[i] ;
        }
    iCandidateArray.Reset();
    delete iCandidatePair;
    }


// ---------------------------------------------------------------------------
// CICETests::LocalCandidateFound
// From MTestConsoleStubsObserver
// ---------------------------------------------------------------------------
//
void CICETests::LocalCandidateFound( CNATFWCandidate& aLocalCandidate/*,
    TBool aLastCandidate*/ )
    {
    //iReceiveMediaAddress = aLocalCandidate.TransportAddr();
    //iLocalCandidate = &aLocalCandidate;
    iCandidateArray.AppendL( &aLocalCandidate );
    }


// ---------------------------------------------------------------------------
// CICETests::NewCandidatePairFound
// From MICETestStubsObserver
// ---------------------------------------------------------------------------
//
void CICETests::NewCandidatePairFound( CNATFWCandidatePair* aCandidatePair )
    {
    RDebug::Print( _L( "TEST PRINT: CICETests::NewCandidatePairFound" ) );
    TBuf<40> buffer;
    iCandidatePair = aCandidatePair;
    
    TInetAddr localAddr = iCandidatePair->LocalCandidate().TransportAddr();
    localAddr.Output( buffer );
    RDebug::Print( _L( "Local candidate: %S" ), &buffer );

    localAddr = iCandidatePair->RemoteCandidate().TransportAddr();
    buffer.Zero();
    localAddr.Output( buffer );
    RDebug::Print( _L( "Remote candidate: %S" ), &buffer );
    }


// ---------------------------------------------------------------------------
// CICETests::DoRunNatTestsL
// 
// ---------------------------------------------------------------------------
//
void CICETests::DoRunIceTestsL()
    {
    TInt err( KErrNone );

    err = KErrNone;
    RDebug::Print( _L("\n\nNAT_ICE_TEST_START: TestFetchCandidatesL") );
    iConsole.Printf( _L("Fetch Canditates test") );
    //__UHEAP_MARK;
    TRAP( err, TestFetchCandidatesL() );
    RDebug::Print( _L("\nNAT_ICE_UDP_TEST_STOP: TestFetchCandidatesL, ERR: %d"), err );
    PrintTestResult( err );
    //__UHEAP_MARKEND;
    
    err = KErrNone;
    RDebug::Print( _L( "\n\nNAT_ICE_TEST_START: Set role" ) );
    iConsole.Printf( _L( "Set role test" ) );
    __UHEAP_MARK;
    TRAP( err, TestSetOperationModeL() );
    RDebug::Print( _L( "\nNAT_ICE_UDP_TEST_STOP: Set role, ERR: %d" ), err );
    PrintTestResult( err );
    __UHEAP_MARKEND;
    /*
    err = KErrNone;
    RDebug::Print( _L( "\n\nNAT_ICE_TEST_START: Set credential" ) );
    iConsole.Printf( _L( "Set Identification test" ) );
    __UHEAP_MARK;
    TRAP( err, TestSetIdentificationL() );
    RDebug::Print( _L( "\nNAT_ICE_UDP_TEST_STOP: Set credential, ERR: %d" ), err );
    PrintTestResult( err );
    __UHEAP_MARKEND;
    
    err = KErrNone;
    RDebug::Print( _L("\n\nNAT_STUN_TEST_START: Perform Connectivity Checks") );
    iConsole.Printf( _L("Perform Candidate Checks test") );
    __UHEAP_MARK;
    TRAP( err, TestPerformConnectivityChecksL() );
    RDebug::Print( _L("\nNAT_STUN_TEST_STOP: Perform Connectivity Checks, ERR: %d"), err );
    PrintTestResult( err );
    __UHEAP_MARKEND;
    */
    }


// ---------------------------------------------------------------------------
// CICETests::TestFetchCandidatesL
// ---------------------------------------------------------------------------
//
void CICETests::TestFetchCandidatesL()
    {
    const TInt KDefaultGranularity = 1;
    
    TUint sessionId( 0 );
    TUint streamId( 0 );
    TInt loadedPluginInd( 0 );
    
    CDesC8Array* plugins =
        new ( ELeave ) CDesC8ArrayFlat( KDefaultGranularity );
    CleanupStack::PushL( plugins );
    
    plugins->AppendL( _L8( "nokia.ice" ) );
    
    sessionId = iNat.CreateSessionL( iTestIapId, *iTestDomain );
    iIfStub.StartActiveSchedulerL();
    
    iNat.LoadPluginL( sessionId, *plugins, loadedPluginInd );
    
    streamId = iNat.CreateStreamL( sessionId, iProtocol, KDefaultQos );
    
    iNat.FetchCandidatesL( sessionId, streamId, KStreamCollectionId,
    	KMediaComponentId, KAfInet );
    iIfStub.StartActiveSchedulerL( KFetchingTime );
    
    iNat.CloseStreamL( sessionId, streamId );
    iNat.CloseSessionL( sessionId );
    
    for( TInt i( 0 ); i < iCandidateArray.Count(); i++ )
        {
        RDebug::Print( _L( "\nTEST PRINT: Contents of Candidate %d" ), i + 1 );
        PrintContents( iCandidateArray[i] );
        }
    iCandidateArray.ResetAndDestroy();
    CleanupStack::PopAndDestroy( plugins );
    }


// ---------------------------------------------------------------------------
// CICETests::PrintContent  
// ---------------------------------------------------------------------------
//
void CICETests::PrintContents( CNATFWCandidate* aCandidate )
    {
    TInetAddr address;
    TBuf16<160> buffer;
    
    buffer.AppendFormat( _L( "Candidate type = %d\n"), aCandidate->Type() );

    address = aCandidate->TransportAddr();
    address.Output( buffer );
    
    buffer.AppendFormat( _L( "Transport address = %S:%d\n" ), &buffer, address.Port() );
    
    buffer.AppendFormat( _L( "Transport protocol = %d\n" ),
        aCandidate->TransportProtocol() );
        
    address = aCandidate->Base();
    address.Output( buffer );
    
    buffer.AppendFormat( _L( "Candidate base = %S:%d\n" ), &buffer, address.Port() );

    RDebug::Print( buffer );
    }
    
    
// ---------------------------------------------------------------------------
// CICETests::TestSetOperationModeL
// ---------------------------------------------------------------------------
//
void CICETests::TestSetOperationModeL()
    {
    TUint sessionId( 0 );
    
    sessionId = iNat.CreateSessionL( iTestIapId, iTestDomain->Des() );
    iIfStub.StartActiveSchedulerL();
    
    iNat.SetRoleL( sessionId, EIceRoleControlling );
    
    iNat.SetRoleL( sessionId, EIceRoleControlled );
    
    iNat.CloseSessionL( sessionId );
    iCandidateArray.ResetAndDestroy();
    }


// ---------------------------------------------------------------------------
// CICETests::TestSetIdentificationL
// ---------------------------------------------------------------------------
//
/*
void CICETests::TestSetIdentificationL()
    {
    TUint sessionId( 0 );
    TUint streamId( 0 );
    CNATFWCredentials* natIdIn = CNATFWCredentials::NewL();
    CNATFWCredentials* natIdOut = CNATFWCredentials::NewL();
    
    natIdIn->SetDirection( CNATFWCredentials::EInbound );
    natIdIn->SetUsernameL( KUsername );
    natIdIn->SetPasswordL( KPassword );
    natIdIn->SetStreamCollectionId( KStreamCollectionId );
    
    natIdOut->SetDirection( CNATFWCredentials::EOutbound );
    natIdOut->SetUsernameL( KUsername );
    natIdOut->SetPasswordL( KPassword );
    natIdOut->SetStreamCollectionId( KStreamCollectionId );
    
    sessionId = iNat.CreateSessionL( iTestIapId, iTestDomain->Des() );
    iIfStub.StartActiveSchedulerL();
    
    streamId = iNat.CreateStreamL( sessionId, iProtocol, KDefaultQos );

    iNat.SetCredentialsL( sessionId, streamId, *natIdIn );
    
    iNat.SetCredentialsL( sessionId, streamId, *natIdOut );
    
    iNat.CloseStreamL( sessionId, streamId );
    iNat.CloseSessionL( sessionId );
    delete natIdIn;
    delete natIdOut;
    }
*/

// ---------------------------------------------------------------------------
// CICETests::TestPerformConnectivityChecksL
// ---------------------------------------------------------------------------
//
/*
void CICETests::TestPerformConnectivityChecksL()
    {
    TUint sessionId( 0 );
    TUint streamId( 0 );
    CNATFWCredentials* natId = CNATFWCredentials::NewL();
    
    natId->SetDirection( CNATFWCredentials::EOutbound );
    natId->SetUsernameL( KUsername );
    natId->SetPasswordL( KPassword );
    natId->SetStreamCollectionId( KStreamCollectionId );
    
    sessionId = iNat.CreateSessionL( iTestIapId, iTestDomain->Des() );
    iIfStub.StartActiveSchedulerL();
    
    streamId = iNat.CreateStreamL( sessionId, iProtocol, KDefaultQos );
    iNat.SetCredentialsL( sessionId, streamId, *natId );
    
    iNat.FetchCandidatesL( sessionId, streamId, KStreamCollectionId,
    	KMediaComponentId );
    iIfStub.StartActiveSchedulerL( KFetchingTime );
    
    // role ICE Controlled
    iNat.SetRoleL( sessionId, EIceRoleControlled );
    iNat.PerformConnectivityChecksL( sessionId, iCandidateArray );
    
    // role ICE Controlling
    iNat.SetRoleL( sessionId, EIceRoleControlling );
    iNat.PerformConnectivityChecksL( sessionId, iCandidateArray );
    
    iNat.CloseStreamL( sessionId, streamId );
    iNat.CloseSessionL( sessionId );
    delete natId;
    iCandidateArray.ResetAndDestroy();
    }
*/

// ---------------------------------------------------------------------------
// CICETests::PrintResult
// ---------------------------------------------------------------------------
//
void CICETests::PrintTestResult( TInt aError )
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
