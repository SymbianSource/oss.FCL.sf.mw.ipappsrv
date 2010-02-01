/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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

#include <e32cons.h>
#include "mmccinterface.h"
#include "mmccnetworksettings.h"
#include "test_async_features.h"

class CMccInterfaceStub;

class CTestAppConsole : public CActive
    {
public:
	static CTestAppConsole* NewLC();
    virtual ~CTestAppConsole();
	
	void StartTesting();
 	void DisplayMsg( const TDesC& aMsg );
	
private:
	
    CTestAppConsole( );	
    void ConstructL(); 
	
    void RunL();	
    void DoCancel();
	
    void ProcessMainInput();

	// create console menus
	void DisplayConsoleMenuL( const TDesC& aInstructions );
	TKeyCode GetStringFromConsole( TDes& aAddr );
	
	void DoRead();
	void ResetNetworkSettings();
	void QueryNetworkSettings();
	void QueryIPAddress();


// Test functions
private:
    
    // Starting point functions for testing
    TInt RunPluginTests();
    TInt RunInterfaceTests();
    TInt RunAudioControlTests();
    TInt RunDtmfTests();
    TInt RunRtcpTests();
    TInt RunAsyncFeatureTests();
    
    TInt RunSecureTests();

    // Test preamble function for creating the Mcc IF
    void MccInterfacePreambleL();
    
    // Test postamble function for destroying the Mcc IF
    void MccInterfacePostamble();
    
private:

    CConsoleBase* iConsole; 
	HBufC* iInstruct;
	CMccInterface* iMccInterface;
	CMccInterfaceStub* iMccInterfaceStub;
	TMccNetSettings iNetsettings;
    TInetAddr iRemoteAddr;
    CTestAsyncFeat* iFeatTest;
    };


    
#endif __TESTAPPCONSOLE_H
