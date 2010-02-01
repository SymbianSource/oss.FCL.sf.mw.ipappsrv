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
#include "TestConsoleDef.h"
#include "MmccInterface.h"
#include "MmccCodecInformation.h"
#include "MmccCodecInformationFactory.h"
#include "MccDef.h"

enum TTestConsoleMenu
    {
    EMainMenu,
    EDTMFTestMenu,
    ECodecTestMenu
    };
    
class TTimeIntervalMicroSeconds32;

_LIT(KMainTitle," Mcc Console ");

class CTestAppConsole : public CActive,public MMccCtrlObserver
    {
public:
	static CTestAppConsole* NewL();
    virtual ~CTestAppConsole();
	
	void StartTesting();
 	void DisplayMsg(const TDesC& aMsg);
	
private:	
    CTestAppConsole( );	
    void ConstructL(); 
	
    void RunL();	
    void DoCancel();
	
    void ProcessMainInput();
    
    // Command handler for DTMF test menu inputs
    void ProcessDTMFInput();

    // Command handler for codec test menu inputs
    void ProcessCodecInput();

	// create console menus
	void DisplayConsoleMenu(const TDesC& aInstructions);
	TKeyCode GetStringFromConsole(TDes& aAddr);
	TInt GetIntegerFromConsole( TInt& aVal );
    TInt GetIntegerFromConsole( TUint& aVal );

	void DoRead();

    // Network settings
    void SetNetSettings();

    //Test functions
    void RunTestCreateSession();
    void RunTestCreateStreams();
	void RunTest1c();
	void RunTestPrepareStreams();
	void RunTest2p();
	void RunTestStartStream();
	void RunTestPauseStreams();
	void RunTestResumeStreams();
	void RunTestStopStream();
	void RunTestDeleteStreams();
	void RunTestCloseSession();
	void RunTest7();
	void RunTestGetCodecAndAudioSettings();
	void RunTestDisplaySupportedCodecs();
	void RunSetCodecSettings();
    void RunSetFmtpAttr();
	void RunTestCodecFactory();
	void ToggleVAD();
	void ChangeCodec();
	void SetCodec();
	void SetRemoteAddr();
    void ToggleInactivityTimer();
    void SendNonRtcpData();

	// From MMccCtrlObserver
	
	void MccEventReceived( const TMccEvent& aEvent );
        
    void MccMediaStarted( TUint32 aSessionId,
                          TUint32 aLinkId,     
                          TUint32 aStreamId,
                          TUint32 aSinkSourceId );
                              
    void MccMediaStopped( TUint32 aSessionId,
                          TUint32 aLinkId,
                          TUint32 aStreamId,
                          TUint32 aSinkSourceId );

    void MccMediaPaused( TUint32 aSessionId,
                         TUint32 aLinkId,
                         TUint32 aStreamId,
                         TUint32 aSinkSourceId );

    void MccMediaResumed( TUint32 aSessionId,
                          TUint32 aLinkId,
                          TUint32 aStreamId,
                          TUint32 aSinkSourceId );

    void MccMediaPrepared( TUint32 aSessionId,
                           TUint32 aLinkId,
                           TUint32 aStreamId,
                           TUint32 aSinkSourceId );

    void MccMediaInactive( TUint32 aSessionId,
                           TUint32 aLinkId,
                           TUint32 aStreamId,
                           TUint32 aSinkSourceId );

    void MccMediaActive( TUint32 aSessionId,
                         TUint32 aLinkId,
                         TUint32 aStreamId,
                         TUint32 aSinkSourceId );

    void MccCtrlError( TInt aError );

    void MccCtrlError( TInt aError,
                       TUint32 aSessionId,
                       TUint32 aLinkId,
                       TUint32 aStreamId,
                       TUint32 aSinkSourceId );


private:

    CConsoleBase* iConsole; 

	HBufC* iInstruct;
	TUint32 iSessionId;
    TUint32 iDTMFSessionID;
    TUint32 iDtmfLinkID;
    TUint32 iUplinkId;
    TUint32 iDownlinkId;
	TUint32 iUpStreamId, iDownStreamId;
    TUint32 iDTMFStreamID;
    TBool iInactivityTimerActive;

    TMccNetSettings iNetSettings;
    TInt iIapId;
    TBool iNetSettingsDone;

	CMccInterface* iMccInterface;
    TInetAddr iRemoteAddr;

    RPointerArray< CMccCodecInformation > iCodecInformation;
	RPointerArray<CMccCodecInformation> iCodecArray;
    //RArray<CMccCodecInformation> iCodecArray;
	CMccCodecInformationFactory* infoFactory;

	TInt iCodecSelected;
    TTestConsoleMenu iCurMenu;
    TBool iDtmfMode;

    TBool iSdesReported;
    TBool iSrReported;
    TBool iRrReported;
    TInt iNumOfNonRtcpDataSent;
    };


#endif __TESTAPPCONSOLE_H
