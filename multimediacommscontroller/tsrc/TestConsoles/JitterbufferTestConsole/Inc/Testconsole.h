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
#include "MccJitterBuffer.h"
//#include <MmccInterface.h>
//#include <MmccCodecInformation.h>
//#include <Mccdef.h>


class TTimeIntervalMicroSeconds32;
#define KTestUid  0x0     

const TUint32 KMCCFourCCIdAMRNB = 1380794656;
const TUint32 KMCCFourCCIdG711 = 825308999;

const TUid KTestApp = { KTestUid };
_LIT(KMainTitle," MCC Console ");

class CTestAppConsole : public CActive, MDataSink, MDataSource
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

	void GenerateRandomSourceData( TDes8& aToBuffer );
	void PrintBuffer( TDes8& aPayload );
	void SetSeqNum();
    //Test functions
    void RunTestFillBuffer();
	void RunTestEmptyBuffer();
	void RunTestSetupJitterbuffer();

	// MDataSink
	TFourCC SinkDataTypeCode( TMediaId /*aMediaId*/ );
	TBool CanCreateSinkBuffer();
	CMMFBuffer* CreateSinkBufferL( TMediaId /*aMediaId*/, TBool& /*aReference*/ );
	void EmptyBufferL( CMMFBuffer* aBuffer, MDataSource* aSupplier, 
									 TMediaId /*aMediaId*/);
	void BufferFilledL( CMMFBuffer* aBuffer );
	void ConstructSinkL( const TDesC8& /*aInitData*/ );

	// MDataSource
	TInt SetSourceDataTypeCode( TFourCC /*aCodec*/, TMediaId /*aMedia*/ );
	TFourCC SourceDataTypeCode( TMediaId /*aMediaId*/ );
	void FillBufferL( CMMFBuffer* aBuffer,
                      MDataSink* aConsumer,
                      TMediaId /*aMediaId*/ );
	void BufferEmptiedL( CMMFBuffer* aBuffer );
//	CMMFBuffer* CreateSinkBufferL( TMediaId /*aMediaId*/, TBool& /*aReference*/ );
	CMMFBuffer* CreateSourceBufferL(TMediaId /*aMediaId*/, TBool &/*aReference*/);
	void ConstructSourceL( const TDesC8& /*aInitData*/ );
	TBool CanCreateSourceBuffer();


private:

    CConsoleBase* iConsole; 

	HBufC* iInstruct;
	//TRtpId iSessionId;
	//TInt streamID, streamID2;

	//CMCCInterface* iMccInterface;
	CMccJitterBuffer* iJitterBuffer;
    //TInetAddr iRemoteAddr;

    //RPointerArray<CMCCCodecInformation> iCodecArray;
	TUint32 iCodec;
	TInt iSeqNum;
    };


#endif __TESTAPPCONSOLE_H
