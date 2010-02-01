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




#ifndef TESTCLIENT_H
#define TESTCLIENT_H

#include <e32base.h>
#include <es_sock.h>
#include <in_sock.h>

#include "timeouttimer.h"

class MTestClientNotifier;


/**
 *  ?one_line_short_description
 *
 *  ?more_complete_description
 *
 *  @lib ?library
 *  @since S60 ?S60_version *** for example, S60 v3.0
 */
class MTestClientNotifier
    {   
        
public:
    
    enum TClientNotify
        {
        EOpened = 1,
        EConnected,
        ETimerOccured
        };
        
    virtual void Notify( TClientNotify aNotify, TInt aError ) = 0;

    };
    
    
/**
 *  ?one_line_short_description
 *
 *  ?more_complete_description
 *
 *  @lib ?library
 *  @since S60 ?S60_version *** for example, S60 v3.0
 */    
class CTestClient : public CActive, public MTimeoutNotifier
    {
public:
    static CTestClient* NewL( MTestClientNotifier* aNotifier );
    
    static CTestClient* NewLC( MTestClientNotifier* aNotifier );
    
    void OpenL( TUint aIapId );
    
    void ConnectL( TInetAddr& aAddr, TUint aTimeoutTime = 0 );
    
    void ResolveLocalAddrL( TInetAddr& aAddr, TUint32 aIap );
    
    void CheckAndSetAddr(   TInetAddr& aTarget,
                            TInetAddr& aCandidate,
                            TUint32 aCandidateIap,
                            TUint32 aSpecifiedIap ) const;
                            
    void SetAddr(   TInetAddr& aTarget,
                    const TInetAddr& aSource ) const;
                    
    virtual ~CTestClient();
    
protected: // from CActive
    void RunL();
    void DoCancel();

protected: // from MTimeoutNotifier
    void TimerExpired();
        
private:
    CTestClient( MTestClientNotifier* aNotifier );
    void ConstructL();
    
private: // members

    enum TTestClientState
        {
        EOpen = 1,
        EReady,
        EConnecting,
        EConnected
        };
        
    MTestClientNotifier* iNotifier;
    TTestClientState iState;
    TUint iPort;
    RSocket iSocket;
    RSocketServ iServer;
    RConnection iConnection;
    TInetAddr iLocalAddr;
    CTimeOutTimer* iTimer;
    };

#endif // TESTCLIENT_H
