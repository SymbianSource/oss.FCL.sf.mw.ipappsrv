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




#ifndef NATFW_STUBS_H
#define NATFW_STUBS_H


#include "natfwconnectivityapidefs.h"

#include "mnatfwconnectivityobserver.h"
#include "timeouttimer.h"


class CNATFWCandidatePair;
class CNATFWCandidate;
class CConsoleBase;

class CNATConnFWTestConsoleStubs :
    public CBase,
    public MNATFWConnectivityObserver,
    public MTimeoutNotifier
    {
    
public:

    static CNATConnFWTestConsoleStubs* NewL( CConsoleBase& aConsole );
    virtual ~CNATConnFWTestConsoleStubs();
    
// from base class MNATFWConnectivityObserver

    void EventOccured( TUint aSessionId, TUint aStreamId,
        TNATFWConnectivityEvent aEvent, TInt aError, TAny* aEventData );

// from base class MTimeoutNotifier
    
    void TimerExpired();

public: // new methods
    
    TInt StartActiveSchedulerL( TUint aRunningTime = 0 );
    
    void StopActiveScheduler();
    
    void SetFailureTest( TBool aFailureTest );
    
    CNATFWCandidate& LocalCandidateL();
    
protected:
    
    CNATConnFWTestConsoleStubs( CConsoleBase& aConsole );
    
    void ConstructL();
    
    TBool EventsAreEqual( /*const TMccEvent& aEvent1, 
        const TMccEvent& aEvent2*/ ) const;
    
    void ClearEvent( /*TMccEvent& aEvent*/ );
    
    void HandleEventReceival( TUint aStreamId );
    
    const TDesC16& EventDesc( TUint16 aEventVal );
        
protected: // data
    TBool iActive;

private: // data
    TBool iTimedExpired;
    TUint iWaitStreamId;
    TUint iWaitSubStreamId;
    //RPointerArray<CNATConnEvent> iConnectivityEventArray;
    CConsoleBase& iConsole;
    TBool iFailureTest;
    CTimeOutTimer* iTimer;
    CNATFWCandidate* iLocalCandidate;
    };
    
#endif // NATFW_STUBS_H