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
* Description:    ICE Communication tests
*
*/




#include "natcomtest.h"

#include "natfwconnectivityapi.h"
#include "natfwcredentials.h"
#include <e32cons.h>
#include "natfwconnectivityapidefs.h"
#include <e32debug.h>
#include "natfwcandidate.h"
#include "natfwcandidatepair.h"
#include <mmccinterfacedef.h>
#include <badesca.h>

#include "natcomtestobserver.h"
#include "testdefinitions.hrh"
#include "natfwtestconsolelogs.h"
#include "natcomteststate.h"
#include "mccmediastream.h"
#include "mccsession.h"
#include "mcclink.h"

const TInt KQoS = 45;
const TInt KStreamCollectionId = 1;

const TInt KRtpMediaComponentId = 1;
const TInt KRtcpMediaComponentId = 2;

_LIT8( KMOUsername, "icemo" );
_LIT8( KMTUsername, "icemt" );

_LIT8( KNATStun, "nokia.stun" );
_LIT8( KNATTurn, "nokia.turn" );
_LIT8( KNATIce, "nokia.ice" );

// ---------------------------------------------------------------------------
// CNatComTest::CNatComTest
// ---------------------------------------------------------------------------
//
CNatComTest::CNatComTest( TUint32 aIapId, MNatComTestObserver& aObserver ) :
    CActive( CActive::EPriorityStandard ), iIapId( aIapId ),
    iObserver( aObserver )
    {
    iOnlyCandidates = EFalse;
    iState = EReady;
    }
 
// ---------------------------------------------------------------------------
// CNatComTest::ConstructL
// ---------------------------------------------------------------------------
//   
void CNatComTest::ConstructL( const TDesC8& aDomain )
    {
    __NATFWTESTCONSOLE( "CNatComTest::ConstructL -Start" )
    const TInt KDefaultGranularity = 1;
    
    CActiveScheduler::Add( this );
    iDomain = aDomain.AllocL();
    iNat = CNATFWConnectivityApi::NewL();
    iNat->RegisterObserverForEventsL( *this,
        MNATFWConnectivityObserver::EAllEvents );
    iSession = CMccSession::NewL( iIapId );
    iPlugins = new ( ELeave ) CDesC8ArrayFlat( KDefaultGranularity );
    
    __NATFWTESTCONSOLE( "CNatComTest::ConstructL -End" )
    }

// ---------------------------------------------------------------------------
// CNatComTest::NewL
// ---------------------------------------------------------------------------
//
CNatComTest* CNatComTest::NewL( const TDesC8& aDomain, TUint32 aIapId,
    MNatComTestObserver& aObserver )
    {
    __NATFWTESTCONSOLE( "CNatComTest::NewL" )
    CNatComTest* self = CNatComTest::NewLC( aDomain, aIapId, aObserver ); 
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CNatComTest::NewLC
// ---------------------------------------------------------------------------
//
CNatComTest* CNatComTest::NewLC( const TDesC8& aDomain, TUint32 aIapId,
    MNatComTestObserver& aObserver )
    {
    __NATFWTESTCONSOLE( "CNatComTest::NewLC" )
    CNatComTest* self = new (ELeave) CNatComTest( aIapId, aObserver );
    CleanupStack::PushL( self );
    self->ConstructL( aDomain );
    return self;
    }

// ---------------------------------------------------------------------------
// CNatComTest::~CNatComTest
// ---------------------------------------------------------------------------
//
CNatComTest::~CNatComTest()
    {
    Cancel();
    delete iSession;
    delete iDomain;
    delete iNat;
    delete iPlugins;
    iLocalCandidates.ResetAndDestroy();
    iCandidatePairs.ResetAndDestroy();
    iStreamIds.Close();
    }

// ---------------------------------------------------------------------------
// CNatComTest::FetchCandidates
// ---------------------------------------------------------------------------
//  
TInt CNatComTest::OnlyFetchCandidates( TUint aProtocol )
    {
    iOnlyCandidates = ETrue;
    return StartUsingIce( aProtocol, EIceRoleControlling );
    }

// ---------------------------------------------------------------------------
// CNatComTest::StoreLocalCondidates
// ---------------------------------------------------------------------------
//  
TInt CNatComTest::StoreLocalCondidates( TUint aProtocol,
    TBool aUseRtcp )
    {
    RArray<TNatComState> sequence;
    TInt err;
    
    iPlugins->Reset();
    TRAP( err, iPlugins->AppendL( KNATIce ) );
    if ( err )
        {
        return err;
        }
    
    if ( aUseRtcp )
        {
        sequence.Append( ECreateSession );
        sequence.Append( ELoadPlugin );
        sequence.Append( ECreateRtpStream );
        sequence.Append( EFetchRtpCandidates );
        sequence.Append( ECreateRtcpStream );
        sequence.Append( EFetchRtcpCandidates );
        sequence.Append( EStoreLocalCandidates );
        sequence.Append( EEnd );
        }
    else
        {
        sequence.Append( ECreateSession );
        sequence.Append( ELoadPlugin );
        sequence.Append( ECreateRtpStream );
        sequence.Append( EFetchRtpCandidates );
        sequence.Append( EStoreLocalCandidates );
        sequence.Append( EEnd );
        }
    
    iStateSequence.SetSequence( sequence );
    iStateSequence.SetTestType( EIceComTest );
    
    SetTeardownSequence( EIceComTest, aUseRtcp );
    
    err = Start( aProtocol, EIceRoleControlling );
    sequence.Close();
    return err;
    }

// ---------------------------------------------------------------------------
// CNatComTest::StoreLocalCandidate
// ---------------------------------------------------------------------------
// 
TInt CNatComTest::StoreLocalCandidate( TUint aProtocol, TComTestType aTestType,
    TBool aUseRtcp )
    {
    RArray<TNatComState> sequence;
    TInt err = KErrNone;

    if ( aUseRtcp )
        {
        sequence.Append( ECreateSession );
        sequence.Append( ELoadPlugin );
        sequence.Append( ECreateRtpStream );
        sequence.Append( EFetchRtpCandidate );
        sequence.Append( ECreateRtcpStream );
        sequence.Append( EFetchRtcpCandidate );
        sequence.Append( EStoreLocalCandidates );
        sequence.Append( EEnd );
        }
    else
        {
        sequence.Append( ECreateSession );
        sequence.Append( ELoadPlugin );
        sequence.Append( ECreateRtpStream );
        sequence.Append( EFetchRtpCandidate );
        sequence.Append( EStoreLocalCandidates );
        sequence.Append( EEnd );
        }
    
    iStateSequence.SetSequence( sequence );
    iStateSequence.SetTestType( aTestType );
    SetTeardownSequence( aTestType, aUseRtcp );
    
    iPlugins->Reset();
    if ( EStunComTest == aTestType )
        {
        TRAP( err, iPlugins->AppendL( KNATStun ) );
        }
    if ( ETurnComTest == aTestType)
        {
        TRAP( err, iPlugins->AppendL( KNATTurn ) );
        }
    if ( err )
        {
        return err;
        }
    err = Start( aProtocol, EIceRoleControlling );
    sequence.Close();
    return err;
    }

// ---------------------------------------------------------------------------
// CNatComTest::StartComTest
// ---------------------------------------------------------------------------
// 
TInt CNatComTest::StartComTest( TUint aProtocol, TComTestType aTestType,
        TBool aUseRtcp )
    {
    RArray<TNatComState> sequence;
    TInt err = KErrNone;
    
    if ( aUseRtcp )
        {
        sequence.Append( ECreateSession );
        sequence.Append( ELoadPlugin );
        sequence.Append( ECreateRtpStream );
        sequence.Append( EFetchRtpCandidate );
        sequence.Append( ECreateRtcpStream );
        sequence.Append( EFetchRtcpCandidate );
        sequence.Append( ECreateRtpWrapper );
        sequence.Append( ECreateRtcpWrapper );
        sequence.Append( EPrepareMccLink );
        sequence.Append( ESetOutgoingAddr );
        sequence.Append( ECreateMediaUpStream );
        sequence.Append( ECreateMediaDownStream );
        sequence.Append( ESetWrapperIncomingAddr );
        sequence.Append( EPause );
        sequence.Append( ESetRtpReceivingState );
        sequence.Append( ESetRtcpReceivingState );
        sequence.Append( ESetRtpSendingState );
        sequence.Append( ESetRtcpSendingState );
        sequence.Append( EStartMediaDownStream );
        sequence.Append( EStartMediaUpStream );
        sequence.Append( ERunning );
        }
    else
        {
        sequence.Append( ECreateSession );
        sequence.Append( ELoadPlugin );
        sequence.Append( ECreateRtpStream );
        sequence.Append( EFetchRtpCandidate );
        sequence.Append( ECreateRtpWrapper );
        sequence.Append( EPrepareMccLink );
        sequence.Append( ESetOutgoingAddr );
        sequence.Append( ECreateMediaUpStream );
        sequence.Append( ECreateMediaDownStream );
        sequence.Append( ESetWrapperIncomingAddr );
        sequence.Append( EPause );
        sequence.Append( ESetRtpReceivingState );
        sequence.Append( ESetRtpSendingState );
        sequence.Append( EStartMediaDownStream );   
        sequence.Append( EStartMediaUpStream );
        sequence.Append( ERunning ); 
        }
    iStateSequence.SetSequence( sequence );
    iStateSequence.SetTestType( aTestType );
    SetTeardownSequence( aTestType, aUseRtcp );
    
    iPlugins->Reset();
    if ( EStunComTest == aTestType )
        {
        TRAP( err, iPlugins->AppendL( KNATStun ) );
        }
    if ( ETurnComTest == aTestType)
        {
        TRAP( err, iPlugins->AppendL( KNATTurn ) );
        }
    if ( err )
        {
        return err;
        }
    err = Start( aProtocol, EIceRoleControlling );
    sequence.Close();
    return err;
    }

// ---------------------------------------------------------------------------
// CNatComTest::StartUsingIce
// ---------------------------------------------------------------------------
//  
TInt CNatComTest::StartUsingIce( TUint aProtocol,
    TNATFWIceRole aRole, TBool aUseRtcp )
    {
    RArray<TNatComState> sequence;
    TInt err;
    
    iPlugins->Reset();
    TRAP( err, iPlugins->AppendL( KNATIce ) );
    if ( err )
        {
        return err;
        }
    
    if ( aUseRtcp )
        {
        sequence.Append( ECreateSession );
        sequence.Append( ELoadPlugin );
        sequence.Append( ECreateRtpStream );
        sequence.Append( EFetchRtpCandidates );
        sequence.Append( ECreateRtcpStream );
        sequence.Append( EFetchRtcpCandidates );
        sequence.Append( ESetOperationMode );
        sequence.Append( ESetIdentification );
        sequence.Append( EPause );
        sequence.Append( EConnectivityChecks );
        sequence.Append( EPause );
        sequence.Append( ECreateRtpWrapper );
        sequence.Append( ECreateRtcpWrapper );
        sequence.Append( EPrepareMccLink );
        sequence.Append( ESetOutgoingAddr );
        sequence.Append( ECreateMediaUpStream );
        sequence.Append( ECreateMediaDownStream );
        sequence.Append( ESetWrapperIncomingAddr );
        sequence.Append( EPause );
        
        if ( EIceRoleControlling == aRole )
            {
            sequence.Append( ESetRtpReceivingState );
            sequence.Append( ESetRtcpReceivingState );
            sequence.Append( ESetRtpSendingStateIce );
            sequence.Append( ESetRtcpSendingStateIce );
            sequence.Append( EStartMediaUpStream );
            sequence.Append( EStartMediaDownStream );
            }
         else
            {
            sequence.Append( ESetRtpReceivingState );
            sequence.Append( ESetRtcpReceivingState );
            sequence.Append( ESetRtpSendingStateIce );
            sequence.Append( ESetRtcpSendingStateIce );
            sequence.Append( EStartMediaDownStream );
            sequence.Append( EStartMediaUpStream );
            }
        sequence.Append( ERunning );
        }
    else
        { 
        sequence.Append( ECreateSession );
        sequence.Append( ELoadPlugin );
        sequence.Append( ECreateRtpStream );
        sequence.Append( EFetchRtpCandidates );
        sequence.Append( ESetOperationMode );
        sequence.Append( ESetIdentification );
        sequence.Append( EPause );
        sequence.Append( EConnectivityChecks );
        sequence.Append( EPause );
        sequence.Append( ECreateRtpWrapper );
        sequence.Append( EPrepareMccLink );
        sequence.Append( ESetOutgoingAddr );
        sequence.Append( ECreateMediaUpStream );
        sequence.Append( ECreateMediaDownStream );
        sequence.Append( ESetWrapperIncomingAddr );
        sequence.Append( EPause );
            
        if ( EIceRoleControlling == aRole )
            {
            sequence.Append( ESetRtpReceivingState );
            sequence.Append( ESetRtpSendingStateIce );  
            sequence.Append( EStartMediaUpStream );
            sequence.Append( EStartMediaDownStream );    
            }
        else
            {
            sequence.Append( ESetRtpReceivingState );
            sequence.Append( ESetRtpSendingStateIce );
            sequence.Append( EStartMediaDownStream );
            sequence.Append( EStartMediaUpStream );
            }
        sequence.Append( ERunning );
        }
    
    iStateSequence.SetSequence( sequence );
    iStateSequence.SetTestType( EIceComTest );
    
    SetTeardownSequence( EIceComTest, aUseRtcp );
    
    err = Start( aProtocol, aRole );
    sequence.Close();
    return err;
    }
    

// ---------------------------------------------------------------------------
// CNatComTest::SetTeardownSequence
// ---------------------------------------------------------------------------
// 
void CNatComTest::SetTeardownSequence( TInt aTestType, TBool aUseRtcp )
    {
    RArray<TNatComState> sequence;
    if ( aUseRtcp )
        {
        if ( EIceComTest == aTestType )
            {
            sequence.Append( EPassiveRtpSendingStateIce );
            sequence.Append( EPassiveRtcpSendingStateIce );
            }
        else
            {
            sequence.Append( EPassiveRtpSendingState );
            sequence.Append( EPassiveRtcpSendingState );
            }
        sequence.Append( EPassiveRtcpReceivingState );
        sequence.Append( EPassiveRtpReceivingState );
        sequence.Append( ETeardown );
        }
    else
        {
        if ( EIceComTest == aTestType )
            {
            sequence.Append( EPassiveRtpSendingStateIce );
            } 
        else
            {
            sequence.Append( EPassiveRtpSendingState );
            }
        sequence.Append( EPassiveRtpReceivingState );
        sequence.Append( ETeardown );
        }
    iTeardownSequence.SetSequence( sequence );
    iTeardownSequence.SetTestType( aTestType );
    sequence.Close();
    }


// ---------------------------------------------------------------------------
// CNatComTest::Start
// ---------------------------------------------------------------------------
//    
TInt CNatComTest::Start( TUint aProtocol,
    TNATFWIceRole aRole )
    {
    if ( EReady != iState )
        {
        return KErrInUse;
        }
    iMode = aRole;
    iProtocol = aProtocol;
    iState = iStateSequence.Current();
    ActiveRequestCompleted( KErrNone );
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CNatComTest::StopL
// ---------------------------------------------------------------------------
//    
void CNatComTest::StopL()
    {
    RDebug::Print( _L("TEST PRINT: CNatComTest::StopL" ) );
    if ( EReady != iState )
        {
        Cancel();
        iState = EEnd;
        ActiveRequestCompleted( KErrNone );
        }
    }


// ---------------------------------------------------------------------------
// CNatComTest::TeardownL
// ---------------------------------------------------------------------------
//
void CNatComTest::TeardownL()
    {
    RDebug::Print( _L("TEST PRINT: CNatComTest::TeardownL -start" ) );
    
    if ( iUpStreamId )
        {
        iSession->CloseMediaStreamL( iUpStreamId );
        iUpStreamId = 0;
        }
    if ( iDownStreamId )
        {
        iSession->CloseMediaStreamL( iDownStreamId );
        iDownStreamId = 0;
        }
    if ( iLinkId )
        {
        iSession->CloseLinkL( iLinkId );
        }
    if ( iRtpStreamId )
        {
        iNat->CloseStreamL( iSessionId, iRtpStreamId );
        }
    if ( iRtcpStreamId )
        {
        iNat->CloseStreamL( iSessionId, iRtcpStreamId );
        }
    if ( iSessionId )
        {
        iNat->CloseSessionL( iSessionId );
        }

    iLocalCandidates.ResetAndDestroy();
    iCandidatePairs.ResetAndDestroy();
    
    iState = EReady;
    ActiveRequestCompleted( KErrNone );
    
    RDebug::Print( _L("TEST PRINT: CNatComTest::TeardownL -end" ) );
    }
    
// ---------------------------------------------------------------------------
// from CActive
// CNatComTest::RunL
// ---------------------------------------------------------------------------
//
void CNatComTest::RunL()
    {
    __NATFWTESTCONSOLE_INT1( "CNatComTest::RunL", iStatus.Int() )
    
    TInt error;
    TInetAddr RtpAddr;
    TInetAddr RtcpAddr;
    
    if ( iStatus.Int() || EError == iState || EEnd == iState )
        {
        if ( EError == iState )
            {
            iObserver.Notify( MNatComTestObserver::ETestError, iStatus.Int() );
            }
        if ( iStateSequence.Current() >= ESetRtpSendingState )
            {
            iState = iTeardownSequence.Current();
            }
        else
            {
            iState = ETeardown;
            }
        }
    switch( iState )
        {
        case ECreateSession:
            RDebug::Print( _L("\nTEST PRINT: CreateSession" ) );
            CreateSession();
            break;
        case ELoadPlugin:
            RDebug::Print( _L("\nTEST PRINT: LoadPlugin" ) );
            LoadPlugin();
            break;
        case ECreateRtpStream:
            RDebug::Print( _L("\nTEST PRINT: CreateRtpStream" ) );
            CreateRtpStream();
            break;
        case ECreateRtcpStream:
            RDebug::Print( _L("\nTEST PRINT: CreateRtcpStream" ) );
            CreateRtcpStream();
            break;
        case EFetchRtpCandidate:
            RDebug::Print( _L("\nTEST PRINT: FetchRtpCandidate" ) );
            FetchRtpCandidate();
            break;
        case EFetchRtcpCandidate:
            RDebug::Print( _L("\nTEST PRINT: FetchRtcpCandidate" ) );
            FetchRtcpCandidate();
            break;
        case EFetchRtpCandidates:
            RDebug::Print( _L("\nTEST PRINT: FetchRtpCandidates" ) );
            FetchRtpCandidates();
            break;
        case EFetchRtcpCandidates:
            RDebug::Print( _L("\nTEST PRINT: FetchRtcpCandidates" ) );
            FetchRtcpCandidates();
            break;
        case EStoreLocalCandidates:
            RDebug::Print( _L("\nTEST PRINT: StoreLocalCandidates" ) );
            error = iObserver.LocalCandidates( iLocalCandidates );
            if ( error )
                {
                __NATFWTESTCONSOLE_INT1(
                    "CNatComTest::Notify; LocalCandidates error:", error )
                iState = EError;
                }
            else
                {
                iState = iStateSequence.Next();
                }
            iOnlyCandidates = EFalse;
            ActiveRequestCompleted( error );
            break;
            
        case ESetIdentification:
            RDebug::Print( _L("\nTEST PRINT: SetIdentification" ) );
            SetIdentification();
            break;
        case ESetOperationMode:
            RDebug::Print( _L("\nTEST PRINT: SetOperationMode" ) );
            SetOperationMode();
            break;
        case EPause:
            RDebug::Print( _L("\nTEST PRINT: Pause" ) );
            iState = iStateSequence.Next();
            iObserver.DoPause( iStatus );
            SetActive();
            break;
        case EConnectivityChecks:
            RDebug::Print( _L("\nTEST PRINT: ConnectivityChecks" ) );
            PerformConnectivityChecks();
            break;
        case ECreateRtpWrapper:
            RDebug::Print( _L("\nTEST PRINT: CreateRtpWrapper" ) );
            CreateRtpWrapper();
            break;
        case ECreateRtcpWrapper:
            RDebug::Print( _L("\nTEST PRINT: CreateRtcpWrapper" ) );
            CreateRtcpWrapper();
            break;
        case EPrepareMccLink:
            RDebug::Print( _L("\nTEST PRINT: PrepareMccLink" ) );
            if ( iRtpMediaWrapper && iRtcpMediaWrapper )
                {
                PrepareMccLinkWithRtcp();
                }
            else
                {
                PrepareMccLink();
                }
            break;
        case ESetOutgoingAddr:
            RDebug::Print( _L("\nTEST PRINT: SetOutgoingAddr" ) );
            iRtpMediaWrapper->OutgoingAddr( RtpAddr );
            iSession->SetRemoteAddressL( iLinkId, RtpAddr );
            if ( iRtcpMediaWrapper )
                {
                iRtcpMediaWrapper->OutgoingAddr( RtcpAddr );
                iSession->SetRemoteAddressL( iLinkId, RtpAddr,
                    RtcpAddr.Port() );
                }
            iState = iStateSequence.Next();
            ActiveRequestCompleted( KErrNone );
            break;
            
        case ECreateMediaUpStream:
            RDebug::Print( _L("\nTEST PRINT: CreateMediaUpStream" ) );
            CreateMediaUpStream();
            break;
        case ECreateMediaDownStream:
            RDebug::Print( _L("\nTEST PRINT: CreateMediaDownStream" ) );
            CreateMediaDownStream();
            break;

        case ESetWrapperIncomingAddr:
            RDebug::Print( _L("\nTEST PRINT: SetWrapperIncomingAddr" ) );
            SetWrapperIncomingAddr();
            break;
            
        case ESetRtpReceivingState:
            RDebug::Print( _L("\nTEST PRINT: SetRtpReceivingState" ) );
            SetReceivingState( *iRtpMediaWrapper, iRtpStreamId,
                EStreamingStateActive );
            break;

        case ESetRtpSendingState:
            RDebug::Print( _L("\nTEST PRINT: SetRtpSendingState" ) );
            SetSendingState( *iRtpMediaWrapper, iRtpStreamId,
                EStreamingStateActive, iStateSequence.TestType(), EFalse );
            break;
            
        case ESetRtcpReceivingState:
            RDebug::Print( _L("\nTEST PRINT: SetRtcpReceivingState" ) );
            SetReceivingState( *iRtcpMediaWrapper, iRtcpStreamId,
                EStreamingStateActive );
            break;
            
        case ESetRtcpSendingState:
            RDebug::Print( _L("\nTEST PRINT: SetRtcpSendingState" ) );
            SetSendingState( *iRtcpMediaWrapper, iRtcpStreamId,
                EStreamingStateActive, iStateSequence.TestType(), ETrue  ); 
            break;
        
        case ESetRtpSendingStateIce:
            RDebug::Print( _L("\nTEST PRINT: SetRtpSendingStateICE" ) );
            SetSendingState( *iRtpMediaWrapper, iRtpStreamId,
                EStreamingStateActive, iStateSequence.TestType(), EFalse  ); 
            break;
        
        case ESetRtcpSendingStateIce:
            RDebug::Print( _L("\nTEST PRINT: SetRtcpSendingState" ) );
            SetSendingState( *iRtcpMediaWrapper, iRtcpStreamId,
                EStreamingStateActive, iStateSequence.TestType(), ETrue  );
            break;
        
        case EStartMediaDownStream:
            RDebug::Print( _L("\nTEST PRINT: StartMediaDownStream" ) );
            StartMediaDownStream();
            break;
        case EStartMediaUpStream:
            RDebug::Print( _L("\nTEST PRINT: StartMediaUpStream" ) );
            StartMediaUpStream();
            break;
   
        case ERunning:
            RDebug::Print( _L("\nTEST PRINT: Running" ) );
            iObserver.Notify( MNatComTestObserver::ERunning, KErrNone );
            break;
        
        case EPassiveRtpReceivingState:
            RDebug::Print( _L("\nTEST PRINT: EPassiveRtpReceivingState" ) );
            SetReceivingState( *iRtpMediaWrapper, iRtpStreamId,
                EStreamingStatePassive );
            break;
            
        case EPassiveRtpSendingState:
            RDebug::Print( _L("\nTEST PRINT: EPassiveRtpSendingState" ) );
            SetSendingState( *iRtpMediaWrapper, iRtpStreamId,
                EStreamingStatePassive, iStateSequence.TestType(), EFalse  );
            break;

        case EPassiveRtcpReceivingState:
            RDebug::Print( _L("\nTEST PRINT: EPassiveRtcpReceivingState" ) );
            SetReceivingState( *iRtcpMediaWrapper, iRtcpStreamId,
                EStreamingStatePassive );
            break;

        case EPassiveRtcpSendingState:
            RDebug::Print( _L("\nTEST PRINT: EPassiveRtcpSendingState" ) );
            SetSendingState( *iRtcpMediaWrapper, iRtcpStreamId,
                EStreamingStatePassive, iStateSequence.TestType(), ETrue );
            break;
            
        case EPassiveRtpSendingStateIce:
            RDebug::Print( _L("\nTEST PRINT: EPassiveRtpSendingStateIce" ) );
            SetSendingState( *iRtpMediaWrapper, iRtpStreamId,
                EStreamingStatePassive, iStateSequence.TestType(), EFalse  );
            break;
        
        case EPassiveRtcpSendingStateIce:
            RDebug::Print( _L("\nTEST PRINT: EPassiveRtcpSendingStateIce" ) );
            SetSendingState( *iRtcpMediaWrapper, iRtcpStreamId,
                EStreamingStatePassive, iStateSequence.TestType(), ETrue  );
            break;
                
        case ETeardown:
            RDebug::Print( _L("\nTEST PRINT: Teardown" ) );
            TeardownL();
            break;
        case EReady:
            RDebug::Print( _L("\nTEST PRINT: Ready" ) );
            iObserver.Notify( MNatComTestObserver::ETestEnd, iStatus.Int() );
            break;
        default :
            break;
        }
    }
    
// ---------------------------------------------------------------------------
// from CActive
// CNatComTest::DoCancel
// ---------------------------------------------------------------------------
//
void CNatComTest::DoCancel()
    {
    }


// ---------------------------------------------------------------------------
// From class MNATFWConnectivityObserver
// CNatComTest::EventOccured
// ---------------------------------------------------------------------------
//
void CNatComTest::EventOccured( TUint aSessionId, TUint aStreamId,
        TNATFWConnectivityEvent aEvent, TInt aError, TAny* aEventData )
    {
    RDebug::Print( _L(
        "TEST PRINT: CNatComTest::EventOccured - sessionId:%d streamId:%d Event:%d Error:%d\n" ),
        aSessionId, aStreamId, aEvent, aError );
    
    TInt error = aError;
    
    if ( ESessionCreated == aEvent )
        {
        iObserver.Notify( MNatComTestObserver::ESessionCreated, error );
        iState = iStateSequence.Next();
        ActiveRequestCompleted( error );
        }
    else if ( EFetchingCompleted == aEvent )
        {
        iState = iStateSequence.Next();
        iObserver.Notify( MNatComTestObserver::EFetchingCompleted, error );
        ActiveRequestCompleted( error );
        }
    else if ( EConnChecksCompleted == aEvent )
        {
        iObserver.Notify( MNatComTestObserver::EConnChecksCompleted, error );
        if ( KErrNone == error )
            {
            if( iStreamIds.Count() == iCandidatePairs.Count() )
                {
                error = iObserver.CandidatePairs( iCandidatePairs );
                iState = iStateSequence.Next();
                }
            }
        ActiveRequestCompleted( error );
        }
    else if ( EReceivingActivated == aEvent || ESendingActivated == aEvent )
        {
        iState = iStateSequence.Next();
        ActiveRequestCompleted( aError );
        }
    else if ( EReceivingDeactivated == aEvent || ESendingDeactivated == aEvent )
        {
        iState = iState = iTeardownSequence.Next();
        ActiveRequestCompleted( aError );
        }
    else if ( ELocalCandidateFound == aEvent )
        {
        if ( EFetchRtpCandidate == iState )
            {
            static_cast<CNATFWCandidate*>( aEventData )->SetComponentId( KRtpMediaComponentId ); 
            }
        if ( EFetchRtcpCandidate == iState )
            {
            static_cast<CNATFWCandidate*>( aEventData )->SetComponentId( KRtcpMediaComponentId ); 
            }
        iLocalCandidates.Append( static_cast<CNATFWCandidate*>( aEventData ) );
        iObserver.Notify( MNatComTestObserver::ELocalCandidateFound, KErrNone );
        }
    else if ( ECandidatePairFound == aEvent )
        {
        iObserver.Notify( MNatComTestObserver::ENewCandPairFound, KErrNone );
        iCandidatePairs.Append( static_cast<CNATFWCandidatePair*>( aEventData  ) );
        }
    else if ( EGeneralError == aEvent )
        {
        iObserver.Notify( MNatComTestObserver::ETestError, aError );
        ActiveRequestCompleted( aError );
        }
    else
        {
        __NATFWTESTCONSOLE_INT2( "CNatComTest::Notify; aEvent:",
            aEvent, " aError:", error )
        }
    }



// ---------------------------------------------------------------------------
// CNatComTest::CreateSession
// ---------------------------------------------------------------------------
//
void CNatComTest::CreateSession()
    {
    TRAPD( err, iSessionId = iNat->CreateSessionL( iIapId, *iDomain ) );
    if ( err )
        {
        __NATFWTESTCONSOLE_INT1( "CNatComTest::CreateSessio Leaves:", err )
        iObserver.Notify( MNatComTestObserver::ESessionCreated, err );
        iState = EError;
        ActiveRequestCompleted( err );
        }
    else
        {
        iStatus = KRequestPending;
        SetActive();
        }
    }

// ---------------------------------------------------------------------------
// CNatComTest::LoadPlugin
// ---------------------------------------------------------------------------
//
void CNatComTest::LoadPlugin()
    {
    TInt loadedPluginInd( 0 );

    TRAPD( err, iNat->LoadPluginL( iSessionId, *iPlugins, loadedPluginInd ) );

    if ( err )
        {
        __NATFWTESTCONSOLE_INT1( "CNatComTest::LoadPlugin Leaves:", err )
        iState = EError;
        }
    else
        {
        iState = iStateSequence.Next();
        }
    ActiveRequestCompleted( err );
    }
    
// ---------------------------------------------------------------------------
// CNatComTest::CreateRtpStream
// ---------------------------------------------------------------------------
//
void CNatComTest::CreateRtpStream()
    {
    iState = iStateSequence.Next();
    TRAPD( err, iRtpStreamId = iNat->CreateStreamL( iSessionId, iProtocol, KQoS ) );
    if ( err )
        {
        iState = EError;
        __NATFWTESTCONSOLE_INT1( "CNatComTest::CreateStream Leaves:", err )
        }
    iStreamIds.Append( iRtpStreamId );
    iObserver.Notify( MNatComTestObserver::EStreamCreated, err );
    ActiveRequestCompleted( err );
    }

// ---------------------------------------------------------------------------
// CNatComTest::CreateRtcpStream
// ---------------------------------------------------------------------------
//
void CNatComTest::CreateRtcpStream()
    {
    iState = iStateSequence.Next();
    TRAPD( err, iRtcpStreamId = iNat->CreateStreamL(
        iSessionId, iProtocol, KQoS ) );
    if ( err )
        {
        iState = EError;
        __NATFWTESTCONSOLE_INT1( "CNatComTest::CreateStream Leaves:", err )
        }
    iStreamIds.Append( iRtpStreamId );
    iObserver.Notify( MNatComTestObserver::EStreamCreated, err );
    ActiveRequestCompleted( err );
    }

// ---------------------------------------------------------------------------
// CNatComTest::SetIdentification
// ---------------------------------------------------------------------------
//
void CNatComTest::SetIdentification()
    {
    TInt count;
    TInt error = KErrNotFound;
    
    iState = iStateSequence.Next();
    
    CNATFWCredentials* inboundIdentification =
        CNATFWCredentials::NewLC();
            
    CNATFWCredentials* outboundIdentification =
        CNATFWCredentials::NewLC();
    
    inboundIdentification->SetStreamCollectionId( KStreamCollectionId );
    outboundIdentification->SetStreamCollectionId( KStreamCollectionId );
      
    if ( iMode == EIceRoleControlling )
        {
        outboundIdentification->SetDirection( CNATFWCredentials::EOutbound );
        outboundIdentification->SetUsernameL( KMOUsername );
        outboundIdentification->SetPasswordL( KMOUsername );
        
        inboundIdentification->SetDirection( CNATFWCredentials::EInbound );
        inboundIdentification->SetUsernameL( KMTUsername );
        inboundIdentification->SetPasswordL( KMTUsername );
        }
    else
        {
        outboundIdentification->SetDirection( CNATFWCredentials::EOutbound );
        outboundIdentification->SetUsernameL( KMTUsername );
        outboundIdentification->SetPasswordL( KMTUsername );
        
        inboundIdentification->SetDirection( CNATFWCredentials::EInbound );
        inboundIdentification->SetUsernameL( KMOUsername );
        inboundIdentification->SetPasswordL( KMOUsername );
        }
        
    count = iLocalCandidates.Count();
    
    for ( TInt i( 0 ); i < count; i++ )
        {
        TRAP( error, iNat->SetCredentialsL(
            *iLocalCandidates[i], *outboundIdentification ) );
        if ( error )
            {
            iState = EError;
            __NATFWTESTCONSOLE_INT1( "CNatComTest::SetIdentification Leaves:", error )
            }
        else
            {
            TRAP( error, iNat->SetCredentialsL(
                *iLocalCandidates[i], *inboundIdentification ) );
            if ( error )
                {
                iState = EError;
                __NATFWTESTCONSOLE_INT1( "CNatComTest::SetIdentification Leaves:", error )
                }
            }
        }
    
    
    CleanupStack::PopAndDestroy( outboundIdentification );
    CleanupStack::PopAndDestroy( inboundIdentification );
    
    iObserver.Notify( MNatComTestObserver::ESetIdentification, error );
    ActiveRequestCompleted( error );
    }

// ---------------------------------------------------------------------------
// CNatComTest::FetchRTPCandidate
// ---------------------------------------------------------------------------
//
void CNatComTest::FetchRtpCandidate()
    {
    TRAPD( err, iNat->FetchCandidateL(
        iSessionId, iRtpStreamId, KAfInet ) );
    if ( err )
        {
        __NATFWTESTCONSOLE_INT1( "CNatComTest::FetchCandidate Leaves:", err )
        iObserver.Notify( MNatComTestObserver::EFetchingCompleted, err );
        iState = EError;
        ActiveRequestCompleted( err );
        }
    else
        {
        iStatus = KRequestPending;
        SetActive();
        }
    }

// ---------------------------------------------------------------------------
// CNatComTest::FetchRTPCandidate
// ---------------------------------------------------------------------------
//
void CNatComTest::FetchRtcpCandidate()
    {
    TRAPD( err, iNat->FetchCandidateL(
        iSessionId, iRtcpStreamId, KAfInet ) );
    if ( err )
        {
        __NATFWTESTCONSOLE_INT1( "CNatComTest::FetchCandidate Leaves:", err )
        iObserver.Notify( MNatComTestObserver::EFetchingCompleted, err );
        iState = EError;
        ActiveRequestCompleted( err );
        }
    else
        {
        iStatus = KRequestPending;
        SetActive();
        }
    }
    
// ---------------------------------------------------------------------------
// CNatComTest::FetchRTPCandidates
// ---------------------------------------------------------------------------
//
void CNatComTest::FetchRtpCandidates()
    {
    TRAPD( err, iNat->FetchCandidatesL(
        iSessionId, iRtpStreamId, KStreamCollectionId, KRtpMediaComponentId,
        KAfInet ) );
    if ( err )
        {
        __NATFWTESTCONSOLE_INT1( "CNatComTest::FetchCandidates Leaves:", err )
        iObserver.Notify( MNatComTestObserver::EFetchingCompleted, err );
        iState = EError;
        ActiveRequestCompleted( err );
        }
    else
        {
        iStatus = KRequestPending;
        SetActive();
        }
    }

// ---------------------------------------------------------------------------
// CNatComTest::FetchRTCPCandidates
// ---------------------------------------------------------------------------
//    
void CNatComTest::FetchRtcpCandidates()
    {
    TRAPD( err, iNat->FetchCandidatesL(
        iSessionId, iRtcpStreamId, KStreamCollectionId, KRtcpMediaComponentId,
        KAfInet ) );
    if ( err )
        {
        __NATFWTESTCONSOLE_INT1( "CNatComTest::FetchCandidates Leaves:", err )
        iObserver.Notify( MNatComTestObserver::EFetchingCompleted, err );
        iState = EError;
        ActiveRequestCompleted( err );
        }
    else
        {
        iStatus = KRequestPending;
        SetActive();
        }
    }
    
// ---------------------------------------------------------------------------
// CNatComTest::SetOperationMode
// ---------------------------------------------------------------------------
//    
void CNatComTest::SetOperationMode()
    {
    iState = iStateSequence.Next();
    TRAPD( err, iNat->SetRoleL( iSessionId, iMode ) );
    if ( err )
        {
        iState = EError;
        __NATFWTESTCONSOLE_INT1( "CNatComTest::SetOperationMode Leaves:", err )
        }
    iObserver.Notify( MNatComTestObserver::ESetOperationMode, err );
    ActiveRequestCompleted( err );
    }
    
// ---------------------------------------------------------------------------
// CNatComTest::PerformConnectivityChecks
// ---------------------------------------------------------------------------
//    
void CNatComTest::PerformConnectivityChecks()
    {
    RPointerArray<CNATFWCandidate> remoteCandidates;
    
    TInt error = iObserver.RemoteCandidates( remoteCandidates );
    if ( error )
        {
        __NATFWTESTCONSOLE_INT1(
            "CNatComTest::PerformConnectivityChecks error in RemoteCandidates reading:", error )
        }
    else
        {
        RPointerArray<CNATFWCandidate> targetCands;

        // RTP Stream
        GetCandidatesAssosiatedToL(
            KStreamCollectionId,
            KRtpMediaComponentId,
            remoteCandidates,
            targetCands );
        SetToCandidatesL( iSessionId, iRtpStreamId, targetCands );
        targetCands.Reset();
        
        // RTCP Stream
        GetCandidatesAssosiatedToL(
            KStreamCollectionId,
            KRtcpMediaComponentId,
            remoteCandidates,
            targetCands );
        SetToCandidatesL( iSessionId, iRtcpStreamId, targetCands );
        targetCands.Close();
        
        TRAP( error, iNat->PerformConnectivityChecksL( iSessionId, remoteCandidates ) );
        if ( error )
            {
            __NATFWTESTCONSOLE_INT1( "CNatComTest::PerformConnectivityChecks Leaves:", error )
            }
        }

    if ( error )
        {
        iObserver.Notify( MNatComTestObserver::EConnChecksCompleted, error );
        iState = EError;
        ActiveRequestCompleted( error );
        }
    else
        {
        iStatus = KRequestPending;
        SetActive();
        }
    remoteCandidates.ResetAndDestroy();
    }

// ---------------------------------------------------------------------------
// CNatComTest::CreateRTPWrapper
// ---------------------------------------------------------------------------
//  
void CNatComTest::CreateRtpWrapper()
    {
    TRAPD( error, iRtpMediaWrapper = &iNat->CreateWrapperL( iSessionId, iRtpStreamId ) );
    if ( error )
        {
        __NATFWTESTCONSOLE_INT1( "CNatComTest::CreateWrapper CreateWrapper error", error )
        }
    else
        {
        iState = iStateSequence.Next();
        }
    ActiveRequestCompleted( error );
    }

// ---------------------------------------------------------------------------
// CNatComTest::CreateRTCPWrapper
// ---------------------------------------------------------------------------
//  
void CNatComTest::CreateRtcpWrapper()
    {
    TRAPD( error, iRtcpMediaWrapper = &iNat->CreateWrapperL( iSessionId, iRtcpStreamId ) );
    if ( error )
        {
        __NATFWTESTCONSOLE_INT1( "CNatComTest::CreateWrapper CreateWrapper error", error )
        }
    else
        {
        iState = iStateSequence.Next();
        }
    ActiveRequestCompleted( error );
    }
        
// ---------------------------------------------------------------------------
// CNatComTest::PrepareMccLink
// ---------------------------------------------------------------------------
// 
void CNatComTest::PrepareMccLink()
    {
    TInetAddr addr;
    iRtpMediaWrapper->OutgoingAddr( addr );
        
    TRAPD( error, iSession->CreateLinkL( iLinkId, KMccLinkGeneral, addr, iStatus ) );
    if ( error )
        {
        __NATFWTESTCONSOLE_INT1(
            "CNatComTest::PrepareMccLink error in CreateLinkL ", error );
            
        iState = EError;
        ActiveRequestCompleted( error );    
        }
    else
        {
        iState = iStateSequence.Next();
        SetActive();
        }
    }

// ---------------------------------------------------------------------------
// CNatComTest::PrepareMccLinkWithRtcp
// ---------------------------------------------------------------------------
// 
void CNatComTest::PrepareMccLinkWithRtcp()
    {
    TInetAddr RtpAddr;
    TInetAddr RtcpAddr;
    
    iRtpMediaWrapper->OutgoingAddr( RtpAddr );
    iRtcpMediaWrapper->OutgoingAddr( RtcpAddr );
        
    TRAPD( error, iSession->CreateLinkL( iLinkId, KMccLinkGeneral, RtpAddr,
        RtcpAddr.Port(), iStatus ) );
    if ( error )
        {
        __NATFWTESTCONSOLE_INT1(
            "CNatComTest::PrepareMccLinkWithRtcp error in CreateLinkL ", error );
            
        iState = EError;
        ActiveRequestCompleted( error );
        }
    else
        {
        iState = iStateSequence.Next();
        SetActive();
        }
    }

// ---------------------------------------------------------------------------
// CNatComTest::SetWrapperIncomingAddr
// ---------------------------------------------------------------------------
// 
void CNatComTest::SetWrapperIncomingAddr()
    {
    TInetAddr addr = iSession->NetSettings().iLocalAddress;
    
    if ( iRtpMediaWrapper && iRtcpMediaWrapper )
        {
        iRtpMediaWrapper->SetIncomingAddrL( addr );
        
        addr.SetPort( iSession->NetSettings().iLocalRtcpPort ); 
        iRtcpMediaWrapper->SetIncomingAddrL( addr );
        }
    else
        {
        iRtpMediaWrapper->SetIncomingAddrL( addr );
        }
    iState = iStateSequence.Next();
    ActiveRequestCompleted( KErrNone );
    }

// ---------------------------------------------------------------------------
// CNatComTest::SetReceivingState
// ---------------------------------------------------------------------------
// 
void CNatComTest::SetReceivingState( MNATFWSocketMediaConnWrapper& aWrapper,
        TUint aStreamId, TNATFWStreamingState aStreamingState )
        {
        // natfw <- rtp
        TRAPD( error, aWrapper.SetReceivingStateL( aStreamingState ) );
        if ( error )
            {
            __NATFWTESTCONSOLE_INT1(
                "CNatComTest::SetReceivingState Wrapper Leave: ", error )
            if ( EStreamingStateActive == aStreamingState )
                {
                iState = EError;
                ActiveRequestCompleted( error );   
                return;
                }
            }
        // natfw <- peer
        error = SetReceivingStatus( aStreamId, aStreamingState );
        if( error )
            {
            if ( EStreamingStatePassive == aStreamingState )
                {
                iState = iTeardownSequence.Next();
                }
            else
                {
                iState = EError;
                }
            ActiveRequestCompleted( error );
            }
        else
            {
            iStatus = KRequestPending;
            SetActive();
            }
        }

// ---------------------------------------------------------------------------
// CNatComTest::SetSendingState
// ---------------------------------------------------------------------------
// 
void CNatComTest::SetSendingState( MNATFWSocketMediaConnWrapper& aWrapper,
    TUint aStreamId, TNATFWStreamingState aStreamingState,
    TInt aTestType, TBool aRtcp )
    {
    RPointerArray<CNATFWCandidate> remoteCandidates;
    RPointerArray<CNATFWCandidate> targetCandidates;
    TInetAddr destAddress;
    
    // natfw -> rtp
    TRAPD( error, aWrapper.SetSendingStateL( aStreamingState ) );
    if ( error )
        {
        __NATFWTESTCONSOLE_INT1(
            "CNatComTest::SetSendingState Wrapper Leave: ", error )
        if ( EStreamingStateActive == aStreamingState )
            {
            iState = EError;
            ActiveRequestCompleted( error );
            return;
            }
        }

    // natfw -> peer
    if ( EIceComTest == aTestType )
        {
        TRAP( error, destAddress = RemoteCandidateAddrByStreamIdL(
            aStreamId ) );
        if ( error )
            {
            __NATFWTESTCONSOLE_INT1(
                "RemoteCandidates reading error:", error )
            if ( EStreamingStatePassive == aStreamingState )
                {
                iState = iTeardownSequence.Next();
                }
            else
                {
                iState = EError;
                }
            ActiveRequestCompleted( error );   
            return;
            }
        }
    else
        {
        error = iObserver.RemoteCandidates( remoteCandidates );
        if ( error )
            {
            __NATFWTESTCONSOLE_INT1(
                "CNatComTest::SetSendingState -RemoteCandidates error:", error )
            if ( EStreamingStatePassive == aStreamingState )
                {
                iState = iTeardownSequence.Next();
                }
            else
                {
                iState = EError;
                }
            ActiveRequestCompleted( error );
            remoteCandidates.ResetAndDestroy();
            targetCandidates.Close();
            return;
            }
        if ( aRtcp )
            {
            GetCandidatesAssosiatedToL( KRtcpMediaComponentId,
                    remoteCandidates, targetCandidates );
            }
        else
            {
            GetCandidatesAssosiatedToL( KRtpMediaComponentId,
                    remoteCandidates, targetCandidates );
            }
        destAddress = targetCandidates[0]->TransportAddr();
        }
    
    error = SetSendingStatus( aStreamId, aStreamingState, destAddress );
    if ( error )
        {
        if ( EStreamingStatePassive == aStreamingState )
            {
            iState = iTeardownSequence.Next();
            }
        else
            {
            iState = EError;
            }
        ActiveRequestCompleted( error );
        }
    else
        {
        iStatus = KRequestPending;
        SetActive();
        }
    remoteCandidates.ResetAndDestroy();
    targetCandidates.Close();
    }

// ---------------------------------------------------------------------------
// CNatComTest::CreateMediaUpStream
// ---------------------------------------------------------------------------
// 
void CNatComTest::CreateMediaUpStream()
    {
    TRAPD( error, iSession->CreateMediaStreamL( iUpStreamId, iLinkId,
        EUpStream, iStatus ) );
    if ( error )
        {
        __NATFWTESTCONSOLE_INT1(
            "CNatComTest::CreateMediaUpStream error in Creation ",
            error );
            
        iState = EError;
        ActiveRequestCompleted( error );    
        }
    else
        {
        iState = iStateSequence.Next();
        SetActive();
        }
    }
    
// ---------------------------------------------------------------------------
// CNatComTest::CreateMediaDownStream
// ---------------------------------------------------------------------------
// 
void CNatComTest::CreateMediaDownStream()
    {
    TRAPD( error, iSession->CreateMediaStreamL( iDownStreamId, iLinkId,
        EDownStream, iStatus ) );
    if ( error )
        {
        __NATFWTESTCONSOLE_INT1(
            "CNatComTest::CreateMediaDownStream error in Creation ",
            error );
            
        iState = EError;
        ActiveRequestCompleted( error );    
        }
    else
        {
        iState = iStateSequence.Next();
        SetActive();
        }    
    }
    
// ---------------------------------------------------------------------------
// CNatComTest::StartMediaDownStream
// ---------------------------------------------------------------------------
// 
void CNatComTest::StartMediaDownStream()
    {
    TRAPD( error, iSession->StartStreamL( iDownStreamId, iStatus ) );
    if( error )
        {
        __NATFWTESTCONSOLE_INT1(
            "CNatComTest::StartMediaDownStream error in StartStreamL", error );
            
        iState = EError;
        ActiveRequestCompleted( error );
        }
    else
        {
        iState = iStateSequence.Next();
        SetActive();
        }
    }

// ---------------------------------------------------------------------------
// CNatComTest::StartMediaUpStream
// ---------------------------------------------------------------------------
// 
void CNatComTest::StartMediaUpStream()
    {
    TRAPD( error, iSession->StartStreamL( iUpStreamId, iStatus ) );
    if( error )
        {
         __NATFWTESTCONSOLE_INT1(
            "CNatComTest::StartMediaUpStream error in StartStreamL", error );
            
        iState = EError;
        ActiveRequestCompleted( error );
        }
    else
        {
        iState = iStateSequence.Next();
        SetActive();
        }
    }


// ---------------------------------------------------------------------------
// CNatComTest::SetRtpReceivingStatus
// ---------------------------------------------------------------------------
//     
TInt CNatComTest::SetReceivingStatus( TUint aStreamId,
    TNATFWStreamingState aState )
    {
    TInt error;
    switch( iStateSequence.TestType() )    
        {
        case ETurnComTest:
        case EStunComTest:
            TRAP( error, iNat->SetReceivingStateL(
                LocalCandidateByStreamIdL( aStreamId ), aState ) );
            break;
        case EIceComTest:
            TRAP( error, iNat->SetReceivingStateL(
                IceLocalCandidateByStreamIdL( aStreamId ), aState ) );
            break;
        }
    if ( error )
        {
        __NATFWTESTCONSOLE_INT1(
            "CNatComTest::SetReceivingStatus error ", error );
        }
    return error;
    }


// ---------------------------------------------------------------------------
// CNatComTest::SetRtpSendingStatus
// ---------------------------------------------------------------------------
// 
TInt CNatComTest::SetSendingStatus( TUint aStreamId,
    TNATFWStreamingState aState, const TInetAddr& aDestAddr )
    {
    TInt error;
    switch( iStateSequence.TestType() )    
        {
        case ETurnComTest:
        case EStunComTest:
            TRAP( error, iNat->SetSendingStateL(
                LocalCandidateByStreamIdL( aStreamId ), aState, aDestAddr ) );
            break;
            
        case EIceComTest:
            TRAP( error, iNat->SetSendingStateL(
                IceLocalCandidateByStreamIdL( aStreamId ), aState, aDestAddr ) );
            break;
        }

    if ( KErrNone != error )
        {
        __NATFWTESTCONSOLE_INT1(
            "CNatComTest::SetSendingStatus error ", error );
        }
    return error;
    }


// ---------------------------------------------------------------------------
// CNatComTest::RemoteCandidateAddrByStreamIdL
// ---------------------------------------------------------------------------
// 
const TInetAddr CNatComTest::RemoteCandidateAddrByStreamIdL( TUint aStreamId )
    {
    __NATFWTESTCONSOLE_INT1( "CNatComTest::RemoteCandidateAddrByStreamIdL ",
        aStreamId )
    TInt count = iCandidatePairs.Count();
    for ( TInt i( 0 ); i < count; ++i )
        {
        if ( iCandidatePairs[ i ]->RemoteCandidate().StreamId() == aStreamId )
            {
            return iCandidatePairs[ i ]->RemoteCandidate().TransportAddr();
            }
        }
     __NATFWTESTCONSOLE( "RemoteCandidate not found" )
    User::Leave( KErrNotFound );
    return NULL;
    }

// ---------------------------------------------------------------------------
// CNatComTest::GetCandidatesAssosiatedToL
// ---------------------------------------------------------------------------
// 
 void CNatComTest::GetCandidatesAssosiatedToL(
    TInt aStreamCollectionId,
    TInt aComponentId,
    const RPointerArray<CNATFWCandidate>& aSource,
    RPointerArray<CNATFWCandidate>& aTarget )
    {
    TInt count = aSource.Count();
    for ( TInt i( 0 ); i < count; ++i )
        {
        if ( aSource[i]->StreamCollectionId() == aStreamCollectionId &&
            aSource[i]->ComponentId() == aComponentId )
            {
            aTarget.AppendL( aSource[i] );
            }
        }
    }

// ---------------------------------------------------------------------------
// CNatComTest::GetCandidatesAssosiatedToL
// ---------------------------------------------------------------------------
// 
 void CNatComTest::GetCandidatesAssosiatedToL(
    TInt aMediaComponentId,
    const RPointerArray<CNATFWCandidate>& aSource,
    RPointerArray<CNATFWCandidate>& aTarget )
    {
    TInt count = aSource.Count();
    for ( TInt i( 0 ); i < count; ++i )
        {
        if ( aSource[i]->ComponentId() == aMediaComponentId )
            {
            aTarget.AppendL( aSource[i] );
            }
        }
    }

 // --------------------------------------------------------------------------
 // CNatComTest::IceLocalCandidateByStreamIdL
 // --------------------------------------------------------------------------
 //  
const CNATFWCandidate& CNatComTest::IceLocalCandidateByStreamIdL( TUint aStreamId )
    {
     __NATFWTESTCONSOLE_INT1( "CNatComTest::IceLocalCandidateByStreamIdL ",
        aStreamId )
    TInt count = iCandidatePairs.Count();
    for ( TInt i( 0 ); i < count; ++i )
        {
        if ( iCandidatePairs[ i ]->
            LocalCandidate().StreamId() == aStreamId )
            {
            return iCandidatePairs[ i ]->LocalCandidate();
            }
        }
    __NATFWTESTCONSOLE( "LocalCandidate not found" )
    User::Leave( KErrNotFound );
    }

// --------------------------------------------------------------------------
// CNatComTest::LocalCandidateByStreamIdL
// --------------------------------------------------------------------------
//  
const CNATFWCandidate& CNatComTest::LocalCandidateByStreamIdL( TUint aStreamId )
    {
    __NATFWTESTCONSOLE_INT1( "CNatComTest::LocalCandidateByStreamIdL ",
        aStreamId )
    TInt count = iLocalCandidates.Count();
    for ( TInt i( 0 ); i < count; ++i )
        {
        if ( iLocalCandidates[i]->StreamId() == aStreamId )
            {
            return *iLocalCandidates[i];
            }
        }
    __NATFWTESTCONSOLE( "LocalCandidate not found" )
    User::Leave( KErrNotFound );
    }


// ---------------------------------------------------------------------------
// CNatComTest::SetToCandidatesL
// ---------------------------------------------------------------------------
// 
void CNatComTest::SetToCandidatesL( TUint aSessionId, TInt aStreamId, 
    RPointerArray<CNATFWCandidate>& aTarget )
    {
    TInt count = aTarget.Count();
    for ( TInt i( 0 ); i < count; ++i )
        {
        aTarget[i]->SetSessionId( aSessionId );
        aTarget[i]->SetStreamId( aStreamId );
        }
    }

// ---------------------------------------------------------------------------
// CNatComTest::ActiveRequestCompleted
// ---------------------------------------------------------------------------
//    
void CNatComTest::ActiveRequestCompleted( TInt aError )
    {
    __NATFWTESTCONSOLE( "CNatComTest::ActiveRequestCompleted" )
    TRequestStatus* status = &iStatus;
    if ( !IsActive() )
        {
        iStatus = KRequestPending;
        SetActive();
        }
    User::RequestComplete( status, aError );
    }
