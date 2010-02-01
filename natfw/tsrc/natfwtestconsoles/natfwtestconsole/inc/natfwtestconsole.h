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
* Description:    Class definition for CTestAppConsole
**
*/



#ifndef __TESTAPPCONSOLE_H
#define __TESTAPPCONSOLE_H


#include <e32base.h>
#include <e32keys.h>
#include "natfwconnectivityapidefs.h"
#include "natcomtestobserver.h"
#include "natcomtest.h"

class CNATFWConnectivityApi;
class CNATConnFWTestConsoleStubs;
class CMediaTestStubs;
class CICETestStubs;
class CStunUdpMediaTest;
class CSettings;
class CNATSettingsTest;
class CConsoleBase;
class TIap;

class CTestAppConsole : public CActive, public MNatComTestObserver
    {
public:
	static CTestAppConsole* NewLC();
    virtual ~CTestAppConsole();
	
	void StartTesting();
 	void DisplayMsg( const TDesC& aMsg );

// From MNatComTestObserver
    void Notify( TNatComTestEvent aEvent, TInt aError );

    TInt LocalCandidates( const RPointerArray<CNATFWCandidate>& aLocalCandidates );

    TInt RemoteCandidates( RPointerArray<CNATFWCandidate>& aRemoteCandidates );
    
    void DoPause( TRequestStatus& aStatus );
    
    TInt CandidatePairs( const RPointerArray<CNATFWCandidatePair>& iCandidatePairs );
     	
private:
	
    CTestAppConsole( );
    
    void ConstructL();
	
    void RunL();
    	
    void DoCancel();
	
    void ProcessMainInput();

    void ProcessDomainSetupInputL();
    
    void ProcessMediaTestInputL();
    
    void ProcessIapInputL();

    void IapSelectionL();
    
	// create console menus
	void DisplayConsoleMenuL( const TDesC& aInstructions );
    
	TKeyCode GetStringFromConsole( TDes& aAddr );
	
	void DoRead();

    // Starting point functions for testing

    TInt RunNatSTUNUdpTests();

    TInt RunNatSTUNTcpTests();

    TInt RunTurnUdpTests();
    
    TInt RunTurnTcpTests();
    
    void NATTurnUdpPreambleL();
    
    void NATTurnTcpPreambleL();
    
    void SetupSTUNUDPTestL();
    
    void SetupSTUNTCPTestL();
    
    void TeardownSTUNTestL();
    
    TInt RunIceUdpTestsL();
    
    TInt RunIceTcpTestsL();
    
    void SetupICEUDPTestL();
    
    void SetupICETCPTestL();
    
    void TeardownICETestL();

    TInt RunStunUdpMediaTestL();
    
    void StopStunUdpMediaTestL();
    
    TInt FetchCandidates( TUint aProtocol, TBool aUseRtcp = EFalse );

    TInt StartIceComTest( TUint aProtocol,
        TNATFWIceRole aRole, TBool aUseRtcp = EFalse );
        
    TInt FetchCandidate( TUint aProtocol, TComTestType aType, TBool aUseRtcp = EFalse );
    
    TInt StartComTest( TUint aProtocol, TComTestType aType,
        TBool aUseRtcp = EFalse );
    
    void StopIceComTest();
    
    void SetupStunUdpMediaTestL();

    TInt RunNatSettingsTest();
    
    void SetupSettingsTestL();
    
    void TeardownSettingsTestL();

	TInt PrintLocalAddress();

	TInt SetLocalToDestinationAddrL();
	
	TInt PrintStoredCandidates();
	
    void CloseMediaStreamL();
    
private:

    enum EInputProcess
        {
        KMainInput,
        KDomainNameInput,
        KMediaTestInput,
        KIapInput
        };
    
    TUint iInputProcess;
    TUint32 iDestAddress;
	TUint iDestPort;
    TBool iAudioTestStun;
    TBool iAudioTestICE;
    CConsoleBase* iConsole;
	HBufC* iInstruct;
	CNATFWConnectivityApi* iNat;
	CStunUdpMediaTest* iIntTest;
	CNATConnFWTestConsoleStubs* iNatStub;
	CMediaTestStubs* iNatMediaStub;
	CICETestStubs* iNatICEStubs;
	CSettings* iSettings;
	CNatComTest* iIceComTest;
	CNATSettingsTest* iSettingsTest;
	TRequestStatus* iListenerStatus;
	TBool iPause;
	HBufC8* iDomainName;
    TUint32 iIapId;
    };

#endif __TESTAPPCONSOLE_H
