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


#define DEBUG_INFO

/*
#ifdef DEBUG_USE_RTP
    #include <RtpHeader.h>
#else
    #include <RtpStpHeader.h>
#endif // DEBUG_USE_RTP
*/

#include <e32cons.h>

#include "TestConsole.hrh"
#include "TestConsoleDef.h"
#include "MccRtpDataSink.h"
#include <mmf/server/mmfdatasource.h>

class TTimeIntervalMicroSeconds32;
class MDataSource;

class CTestAppConsole : public CActive
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

	// create console menus
	void DisplayConsoleMenu(const TDesC& aInstructions);
	TKeyCode GetStringFromConsole(TDes& aAddr);
	
	void DoRead();

    //Test functions
    void RunTest1();
    void RunTest2();

	//End funktio
	void Stop();

private:
    CConsoleBase* iConsole; 
	HBufC* iInstruct;
	CMccRtpDataSink* Sink;
	TInt iError;
//	CMccPayloadFormatEncoder* this2;
    };




//****MDataSource pure virtual funktions****************************************
//TFourCC SourceDataTypeCode(TMediaId /*aMediaId*/) {return 0;};
//void FillBufferL(CMMFBuffer* aBuffer, MDataSink* aConsumer,TMediaId /*aMediaId*/){}; 
//void BufferEmptiedL(CMMFBuffer* aBuffer){};  
//TBool CanCreateSourceBuffer(){return false;};
//CMMFBuffer* CreateSourceBufferL(TMediaId /*aMediaId*/, TBool &aReference){return NULL;};
//void ConstructSourceL(  const TDesC8& aInitData ){};
//***********************************************



#endif __TESTAPPCONSOLE_H
