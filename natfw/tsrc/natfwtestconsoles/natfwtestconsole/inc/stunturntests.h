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




#ifndef STUNTURNUDPTESTS_H
#define STUNTURNUDPTESTS_H

// INCLUDES
#include <e32base.h>
#include <e32cons.h>
#include <badesca.h>

#include "natfwconnectivityapidefs.h"
#include "testclient.h"
#include "testserver.h"

// FORWARD DECLARATIONS
class CNATFWConnectivityApi;
class CNATConnFWTestConsoleStubs;
class CWrongServerSettings;
class MNATFWSocketMediaConnWrapper;

// CLASS
class CStunTurnTests :
    public CBase,
    public MTestServerObserver,
    public MTestClientNotifier
    {
public:
    static CStunTurnTests* NewL(
        CConsoleBase& aConsole,      
        CNATFWConnectivityApi& aNat,
        CNATConnFWTestConsoleStubs& aIFStub,
        const TDesC8& aDomain,
        TUint aIapId,
        TUint aProtocol,
        const CDesC8Array& aPlugins );
    
    virtual ~CStunTurnTests();

public:

    void DoRunStunTurnTestsL();

// from MTestServerObserver

    void Notify( MTestServerObserver::TServerNotify aNotify, TInt aError );

// from MTestClientNotifier

    void Notify( MTestClientNotifier::TClientNotify aNotify, TInt aError );
            
private:
    
    void SetupTestL();
    
    void SetupTest2L();
    
    void SetupWrongServerAddressTestL();
    
    void TeardownWrongServerAddressTest();
    
    void TeardownTest();
    
    void TestCreateSessionL();
    
    void TestCreateStreamL();
    
    void TestCreateWrapperL();
    
    void TestSetIncomingAddrL();
    
    void TestOutgoingAddrL();
    
    void TestFetchCandidateL();

    void TestSetReceivingStatusUDPL();
    
    void TestSetReceivingStatusTCPL();
    
    void TestSetSendingStatusUDPL();
    
    void TestSetSendingStatusTCPL();

    void PrintTestResult( TInt aError );
    
private:

    CStunTurnTests( CConsoleBase& aConsole,
                       CNATFWConnectivityApi& aNat,
                       CNATConnFWTestConsoleStubs& aIFStub,
                       TUint aIapId,
                       TUint aProtocol );
    
    void ConstructL( const TDesC8& aDomain, const CDesC8Array& aPlugins );
 
private:

    CConsoleBase& iConsole;
    CNATFWConnectivityApi& iNat;
    CNATConnFWTestConsoleStubs& iIfStub;
    TUint iTestIapId;
    TUint iProtocol;
    CDesC8Array* iPlugins;
    TBool iNotificationIsOn;
    HBufC8* iTestDomain;
    TUint iSessionId;
    TUint iStreamId;
    MNATFWSocketMediaConnWrapper* iWrapper; // not own
    CWrongServerSettings* iWrongServerSettings;
    };
    
#endif // STUNTURNUDPTESTS_H

// End of File
