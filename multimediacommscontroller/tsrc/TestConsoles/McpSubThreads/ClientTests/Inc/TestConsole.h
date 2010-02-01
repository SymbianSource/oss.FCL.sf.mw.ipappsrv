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

#include "TestConsole.hrh"
#include "TestConsoleDef.h"
#include "rtpapi.h"
#include "rtpdef.h"
#include "mccdef.h"
#include "mccinternaldef.h"

class TTimeIntervalMicroSeconds32;
class CMccSymSubthreadClient;
class MDataSource;
class MDataSink;
class CMccUlDlClient;

class CTestAppConsole : public CActive,
    public MRtpErrNotify, public MMccEventHandler
    {
public:
	static CTestAppConsole* NewL();
    virtual ~CTestAppConsole();
	
	void StartTesting();
 	void DisplayMsg(const TDesC& aMsg);

    virtual void ErrorNotify(TInt aError);

    virtual TInt SendMccEventToClient( TMccEvent& aEvent );
    virtual void ErrorOccured( TInt aError,
                               TUint32 aSessionId,
                               TUint32 aLinkId,
    	                       TUint32 aStreamId, 
    	                       TUint32 aEndpointId );
    virtual void StateChange( TInt aState, TUint32 aLink );
    
private:	
    CTestAppConsole( );	
    void ConstructL(); 
	
    void RunL();	
    void DoCancel();
	
    void ProcessMainInput();

	// create console menus
	void DisplayConsoleMenu(const TDesC& aInstructions);
	TKeyCode GetStringFromConsole(TDes& aAddr);
	
	void DoRead();

    //Test functions
    void RunTest1();
    void RunTest3();
    void RunTest4();
    void RunTest5();

    void RunTest6();

    void TestOpen(CMccSymSubthreadClient* aUlclient, CMccSymSubthreadClient* aDlclient);
    void TestPrepare(CMccSymSubthreadClient* aUlclient, CMccSymSubthreadClient* aDlclient);
    void TestControls(CMccSymSubthreadClient* aUlclient, CMccSymSubthreadClient* aDlclient);
    void TestParameters(CMccSymSubthreadClient* aUlclient, CMccSymSubthreadClient* aDlclient);

    void ConstructUlSinkSource();
    void ConstructDlSinkSource();
    void DestructSinksSources();
    void SetUpRtpStack();

private:
    CConsoleBase* iConsole; 
	HBufC* iInstruct;

    TBool iKeepgoingUL;
    TBool iKeepgoingDL;

    TInt iUlError;
    TInt iDlError;

    MDataSink* iUlsink;
    MDataSink* iDlsink;
    MDataSource* iUlsource;
    MDataSource* iDlsource;

    TRtpId iSessionID;

    TUint iIapid;
    TUint iPort;
    TInetAddr iRemoteAddr;

    CMccUlDlClient* iClient;
    
    TUint32 iLinkId;
    };


#endif __TESTAPPCONSOLE_H
