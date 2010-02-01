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

#include <e32cons.h>
#include "cncmconnectionmultiplexer.h"


class CMuxTestConsoleStubs;
class CMuxTestConsoleTcpStubs;
class CMultiplexerUdpTests;


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



    
// Test functions
private:
    
    // Starting point functions for testing

    TInt RunMuxUdpTests();

    TInt RunMuxTcpTests();

    // Test preamble function for creating the Mcc IF
    void MccInterfacePreambleUdpL();
    
    // Test preamble function for creating the Mcc IF
    void MccInterfacePreambleTcpL();
    
    // Test postamble function for destroying the Mcc IF
    void MccInterfacePostamble();
    
    

    void RunUdpAudioStreaming();
    
    void StopUdpAudioStreaming();
    
private:

    CConsoleBase* iConsole; 
	HBufC* iInstruct;
	CNcmConnectionMultiplexer* iMux;
	
	CMuxTestConsoleStubs* iMuxStub;
	
	TBool iAudioTest;
	CMultiplexerUdpTests* iIntTest;
    };


    
#endif __TESTAPPCONSOLE_H
