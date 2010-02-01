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




#ifndef TESTSERVER_H
#define TESTSERVER_H

#include <e32base.h>
#include <e32std.h>
#include <es_sock.h>

#include "timeouttimer.h"

class TInetAddr;


/**
 *  ?one_line_short_description
 *
 *  ?more_complete_description
 *
 *  @lib ?library
 *  @since S60 ?S60_version *** for example, S60 v3.0
 */     
class MTestServerObserver
    {
    
public:
    
    enum TServerNotify
        {
        EConnecting = 1,
        EConnected,
        EDisconnected,
        EShutdownTimer
        };
     
    virtual void Notify( TServerNotify aNotify, TInt aError ) = 0;
    };


/**
 *  ?one_line_short_description
 *
 *  ?more_complete_description
 *
 *  @lib ?library
 *  @since S60 ?S60_version *** for example, S60 v3.0
 */
class CTestServer : public CActive, public MTimeoutNotifier
    {
public:
    static CTestServer* NewL( MTestServerObserver* aObserver );
    static CTestServer* NewLC( MTestServerObserver* aObserver );
    virtual ~CTestServer();
    
    void OpenL( TInetAddr& aAddr, TUint aTimeoutTime = 0 );
    
protected: // from CActive
    void RunL();
    void DoCancel();

protected: // from MTimeoutNotifier
    void TimerExpired();
        
private:
    CTestServer( MTestServerObserver* aObserver );
    void ConstructL();
    
private: // members

    enum TTestServerState
        {
        EServerDisconnected = 0,
        EServerConnected,
        EServerWaitingConnection
        };

    MTestServerObserver* iObserver;
    TTestServerState iState;
    RSocket iSocket;
    RSocket iListenSocket;
    RSocketServ iServer;
    CTimeOutTimer* iTimer;
    };

#endif // TESTSERVER_H
