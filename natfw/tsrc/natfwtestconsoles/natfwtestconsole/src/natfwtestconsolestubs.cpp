/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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


//*******************************************************************************
// Method      : 
// Purpose     : Working as a stub for NAT Connectivity FW tests
// Parameters  : 
// Return Value: 
//*******************************************************************************

#include <e32base.h>
#include <e32debug.h>
#include <e32cons.h>

#include "natfwtestconsolestubs.h"
#include "mmccnetworksettings.h"
#include "natfwcandidate.h"
#include "timeouttimer.h"

//NATFWConnectivityEvents
_LIT16( KESessionCreated, "ESessionCreated" );
_LIT16( KELocalCandidateFound, "ELocalCandidateFound" );
_LIT16( KEFetchingCompleted, "EFetchingCompleted" );
_LIT16( KEReceivingActivated, "EReceivingActivated" );
_LIT16( KEReceivingDeactivated, "EReceivingDeactivated" );
_LIT16( KESendingActivated, "ESendingActivated" );
_LIT16( KESendingDeactivated, "ESendingDeactivated" );
_LIT16( KECandidatePairFound, "ECandidatePairFound" );
_LIT16( KEConnChecksCompleted, "EConnChecksCompleted" );
_LIT16( KEGeneralError, "EGeneralError" );
_LIT16( KAllEvents, "EAllEvents" );
    
CNATConnFWTestConsoleStubs* CNATConnFWTestConsoleStubs::NewL( CConsoleBase& aConsole )
    {
    CNATConnFWTestConsoleStubs* self = new (ELeave) CNATConnFWTestConsoleStubs( aConsole );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }
    
void CNATConnFWTestConsoleStubs::ConstructL()
    {
    iTimer = CTimeOutTimer::NewL( CActive::EPriorityStandard, *this );
    }

CNATConnFWTestConsoleStubs::CNATConnFWTestConsoleStubs( CConsoleBase& aConsole ) :
    iTimedExpired( EFalse ), iConsole( aConsole )
    {
    }
    
CNATConnFWTestConsoleStubs::~CNATConnFWTestConsoleStubs()
    {
    iTimer->Cancel();
    delete iTimer;
    delete iLocalCandidate;
    }


// ---------------------------------------------------------------------------
// CMediaTestStubs::LocalCandidateL
// ---------------------------------------------------------------------------
//
CNATFWCandidate& CNATConnFWTestConsoleStubs::LocalCandidateL()
    {
    RDebug::Print(
        _L( "TEST PRINT: CNATConnFWTestConsoleStubs::LocalCandidateL" ) );
    if ( NULL == iLocalCandidate )
        {
        User::Leave( KErrNotFound );
        }
    return *iLocalCandidate;
    }


// ---------------------------------------------------------------------------
// CNATConnFWTestConsoleStubs::StartActiveSchedulerL
// ---------------------------------------------------------------------------
//    
TInt CNATConnFWTestConsoleStubs::StartActiveSchedulerL( TUint aRunningTime )
    {
    RDebug::Print( _L( "TEST PRINT: CNATConnFWTestConsoleStubs::StartActiveSchedulerL" ) );
    iActive = ETrue;
    if ( aRunningTime )
        {
        iTimer->After( aRunningTime );
        }
    CActiveScheduler::Start();

    if ( iTimedExpired )
        {
        iTimedExpired = EFalse;
        return KErrTimedOut;
        }
    return KErrNone;
    }
 
 
// ---------------------------------------------------------------------------
// CNATConnFWTestConsoleStubs::StopActiveScheduler
// ---------------------------------------------------------------------------
//       
void CNATConnFWTestConsoleStubs::StopActiveScheduler()
    {
    RDebug::Print( _L(
        "TEST PRINT: CNATConnFWTestConsoleStubs::StopActiveScheduler" ) );
    iTimer->Cancel();
    CActiveScheduler::Stop();
    iActive = EFalse;
    }


// ---------------------------------------------------------------------------
// CNATConnFWTestConsoleStubs::SetFailureTest
// ---------------------------------------------------------------------------
//       
void CNATConnFWTestConsoleStubs::SetFailureTest( TBool aFailureTest )
    {
    iFailureTest = aFailureTest;
    }


// ---------------------------------------------------------------------------
// From class MNATFWConnectivityObserver
// CNATConnFWTestConsoleStubs::EventOccured
// ---------------------------------------------------------------------------
//
void CNATConnFWTestConsoleStubs::EventOccured( TUint aSessionId, TUint aStreamId,
    TNATFWConnectivityEvent aEvent, TInt aError, TAny* aEventData )
    {
    _LIT16( KNotify, "TEST PRINT: CNATConnFWTestConsoleStubs::EventOccured, SessionId: %d, StreamId: %d, Event: %S, Error: %d");
    RDebug::Print(
        KNotify, aSessionId, aStreamId, &EventDesc( aEvent ), aError );
        
    if ( KErrNone != aError && !iFailureTest )
        {
        iConsole.Printf( _L( "\n Notify, Event: %S Error: %d\n" ),
            &EventDesc( aEvent ), aError );
        }
     
   if ( iActive && ( ESessionCreated == aEvent ||
        EReceivingActivated == aEvent ||
        EReceivingDeactivated == aEvent ||
        ESendingActivated == aEvent ||
        ESendingDeactivated == aEvent ||
        EFetchingCompleted == aEvent ) ) 
        {
        StopActiveScheduler();
        }
    if ( iActive && ( ELocalCandidateFound == aEvent ) )
        {
        delete iLocalCandidate;
        iLocalCandidate = NULL;
        iLocalCandidate = static_cast<CNATFWCandidate*>( aEventData );
        RDebug::Print( _L( "TEST PRINT: LocalCandidateFound" ) );
        }
    }


// ---------------------------------------------------------------------------
// CNATConnFWTestConsoleStubs::EventOccured
// ---------------------------------------------------------------------------
//
const TDesC16& CNATConnFWTestConsoleStubs::EventDesc( TUint16 aEventVal )
    {
    TPtrC16 desc; 
    switch( aEventVal )
        {
        case ESessionCreated:
            return KESessionCreated();
        case ELocalCandidateFound:
            return KELocalCandidateFound();
        case EFetchingCompleted:
            return KEFetchingCompleted();
        case EReceivingActivated:
            return KEReceivingActivated();
        case EReceivingDeactivated:
            return KEReceivingDeactivated();
        case ESendingActivated:
            return KESendingActivated();
        case ESendingDeactivated:
            return KESendingDeactivated();
        case ECandidatePairFound:
            return KECandidatePairFound();
        case EConnChecksCompleted:
            return KEConnChecksCompleted();
        case EGeneralError:
            return KEGeneralError();
        case EAllEvents:
            return KAllEvents();
        }
    return KEGeneralError();
    }


// ---------------------------------------------------------------------------
// From class MTimeoutNotifier
// CNATConnFWTestConsoleStubs::TimerExpired
// ---------------------------------------------------------------------------
//
void CNATConnFWTestConsoleStubs::TimerExpired()
    {
    _LIT16( KTimerExpired, "TEST PRINT: CNATConnFWTestConsoleStubs::TimerExpired" );
    RDebug::Print( KTimerExpired );
        
    CActiveScheduler::Stop();
    iActive = EFalse;
    iTimedExpired = ETrue;
    }
