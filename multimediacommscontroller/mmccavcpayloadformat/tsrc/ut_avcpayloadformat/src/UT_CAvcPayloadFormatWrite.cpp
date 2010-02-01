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
* Description:  
*
*/




//  CLASS HEADER
#include "UT_CAvcPayloadFormatWrite.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <digia/eunit/CEunitAllocTestCaseDecorator.h>  //for using EUNIT_ALLOC_TEST

//  INTERNAL INCLUDES
#include "avcpayloadformatwrite.h"
#include "MccInternalDef.h"
#include "Mccinternalcodecs.h"
#include "mccunittestmacros.h"
#include <f32file.h>
#include <S32FILE.H>

#define CONFIGURE_PL_FORMAT( b ) { \
TInt d( 0 ); \
CMccRtpMediaClock* dummyClock = reinterpret_cast<CMccRtpMediaClock*>( &d ); \
iAvcPayloadFormatter->ConfigurePayloadFormatL(b, *dummyClock); }
  
// CONSTRUCTION
UT_CAvcPayloadFormatWrite* UT_CAvcPayloadFormatWrite::NewL()
    {
    UT_CAvcPayloadFormatWrite* self = UT_CAvcPayloadFormatWrite::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CAvcPayloadFormatWrite* UT_CAvcPayloadFormatWrite::NewLC()
    {
    UT_CAvcPayloadFormatWrite* self = new( ELeave ) UT_CAvcPayloadFormatWrite();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CAvcPayloadFormatWrite::~UT_CAvcPayloadFormatWrite()
    {
    delete iRefData;
    }

// Default constructor
UT_CAvcPayloadFormatWrite::UT_CAvcPayloadFormatWrite()
    {
	iAvcPayloadFormatter = NULL;
	iSinkBuff = NULL;
	iSink = NULL;
	iSource = NULL;
	iRefData = NULL;
    }

// Second phase construct
void UT_CAvcPayloadFormatWrite::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    
    // load test file
    _LIT(KFileName, "C:\\data\\NALU.264");
    TBuf<50> fileName(KFileName);
    const TInt K_NALU_FILE_SIZE = 506628;
    
    RFs objFS;
    RFileReadStream naluFileStream;
    naluFileStream.PushL();
    
    User::LeaveIfError(objFS.Connect(5));
    User::LeaveIfError(naluFileStream.Open(objFS, fileName, EFileRead));
    
    iRefData = HBufC8::NewL(K_NALU_FILE_SIZE);
    TPtr8 des = iRefData->Des();
    TRAP_IGNORE( naluFileStream.ReadL(des) )
    
    CleanupStack::PopAndDestroy();	// close file stram
    objFS.Close();
    }

//  METHODS



void UT_CAvcPayloadFormatWrite::SetupL(  )
    {
    	UT_CAvcPayloadFormatWrite_NewLL();
    }

void UT_CAvcPayloadFormatWrite::EmptySetupL(  )
    {
    
    }

void UT_CAvcPayloadFormatWrite::Teardown(  )
    {
	delete iAvcPayloadFormatter;
	iAvcPayloadFormatter = NULL;
	
	delete iSink;
	iSink = NULL;
	
	delete iSource;
	iSource = NULL;
	
	iSinkBuff = NULL;	// AVC have the ownership
	
	REComSession::FinalClose();
    }

// this version is used when EmptyBufferL is not called (and hence iSinkBuff ownership) is not transferred
void UT_CAvcPayloadFormatWrite::Teardown2(  )
    {
	delete iAvcPayloadFormatter;
	iAvcPayloadFormatter = NULL;
	
	delete iSink;
	iSink = NULL;
	
	iSource = NULL;		// this is also not created in this flow
	
	iSinkBuff = NULL;	// AVC have the ownership
	
	REComSession::FinalClose();
    }


void UT_CAvcPayloadFormatWrite::UT_CAvcPayloadFormatWrite_NewLL(  )
    {
	const TUid tempUID = {KImplUidRtpDataSink};
	
	iSink = MDataSink::NewSinkL(tempUID, KNullDesC8);
	iAvcPayloadFormatter = CAvcPayloadFormatWrite::NewL(iSink);	
    }

void UT_CAvcPayloadFormatWrite::UT_CAvcPayloadFormatWrite_NewLErrL(  )
	{
	// just to generate corner cases
	TInt error;
//	const TUid tempUID = { KMccRtpSourceUid };
	const TUid tempUID = { KImplUidRtpDataSource };
	
	TRAP(error, iAvcPayloadFormatter = CAvcPayloadFormatWrite::NewL(NULL));
	
	TRAP(error, iSink = MDataSink::NewSinkL(tempUID, KNullDesC8));
	TRAP(error, iAvcPayloadFormatter = CAvcPayloadFormatWrite::NewL(iSink));	
    }

void UT_CAvcPayloadFormatWrite::UT_CAvcPayloadFormatWrite_ConfigurePayloadFormatL_0_L(  )
    {
	TFourCC fourCC('H','2','6','4');
	
	TMccCodecInfo tempCodecInfo;
	tempCodecInfo.iFourCC = fourCC;
	tempCodecInfo.iCodecMode = 0;
	tempCodecInfo.iPayloadType = 98;
	tempCodecInfo.iFramerate = 15;
	tempCodecInfo.iFrameSize = 10000;
	tempCodecInfo.iMTUSize = 1400;
	TMccCodecInfoBuffer buffer(tempCodecInfo);
	CONFIGURE_PL_FORMAT( buffer )
    }

void UT_CAvcPayloadFormatWrite::UT_CAvcPayloadFormatWrite_ConfigurePayloadFormatL_1_L(  )
    {
	TFourCC fourCC('H','2','6','4');
	
	TMccCodecInfo tempCodecInfo;
	tempCodecInfo.iFourCC = fourCC;
	tempCodecInfo.iCodecMode = 1;
	tempCodecInfo.iPayloadType = 99;
	tempCodecInfo.iFramerate = 15;
	tempCodecInfo.iFrameSize = 10000;
	tempCodecInfo.iMTUSize = 1400;
	TMccCodecInfoBuffer buffer(tempCodecInfo);
	CONFIGURE_PL_FORMAT( buffer )
    }

void UT_CAvcPayloadFormatWrite::UT_CAvcPayloadFormatWrite_ConfigurePayloadFormatL_2_L(TInt mode)
    {
	TInt error;
	TBuf8<2> buffer;
	TRAP(error, CONFIGURE_PL_FORMAT( buffer ));
	
	TMccCodecInfo tempCodecInfo;
	TFourCC fourCC('H','2','6','4');
	tempCodecInfo.iFourCC = fourCC;
	tempCodecInfo.iFramerate = 15;
	tempCodecInfo.iCodecMode = 3;
	TMccCodecInfoBuffer buffer1(tempCodecInfo);
	TRAP(error, CONFIGURE_PL_FORMAT( buffer ));
	
	tempCodecInfo.iCodecMode = mode;
	tempCodecInfo.iPayloadType = KMccPayloadTypeMax;
	TMccCodecInfoBuffer buffer2(tempCodecInfo);
	TRAP(error, CONFIGURE_PL_FORMAT( buffer2 ));
	
	tempCodecInfo.iPayloadType = 100;
	tempCodecInfo.iFrameSize = 0;
	TMccCodecInfoBuffer buffer3(tempCodecInfo);
	TRAP(error, CONFIGURE_PL_FORMAT( buffer3 ));
	
	tempCodecInfo.iFrameSize = 10000;
	tempCodecInfo.iMTUSize = 0;
	TMccCodecInfoBuffer buffer4(tempCodecInfo);
	TRAP(error, CONFIGURE_PL_FORMAT( buffer4 ));
	
	tempCodecInfo.iMTUSize = 1400;
	TMccCodecInfoBuffer buffer5(tempCodecInfo);
	TRAP(error, CONFIGURE_PL_FORMAT( buffer5 ));
    }

void UT_CAvcPayloadFormatWrite::UT_CAvcPayloadFormatWrite_EmptySourceBufferLL(  )
    {
   	MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iAvcPayloadFormatter->EmptySourceBufferL(), KErrNotReady );
   	
   	TBool bFlag;
   	iSinkBuff = iAvcPayloadFormatter->CreateSinkBufferL(KUidMediaTypeVideo, bFlag);
   	DoEmptySourceBufferL();
    }

void UT_CAvcPayloadFormatWrite::UT_CAvcPayloadFormatWrite_SourceBufferEmptiedLL(  )
    {
	MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iAvcPayloadFormatter->SourceBufferEmptiedL(), KErrNotReady );
    }

void UT_CAvcPayloadFormatWrite::UT_CAvcPayloadFormatWrite_SetPayloadTypeL( )
    {
   	iAvcPayloadFormatter->SetPayloadType(102);
    }

void UT_CAvcPayloadFormatWrite::UT_CAvcPayloadFormatWrite_CancelUlRequestL(  )
    {
	// initialization is not needed to test cancel request (as we don't call EmptyBufferL)
	UT_CAvcPayloadFormatWrite_SinkPrimeLL();
	UT_CAvcPayloadFormatWrite_SinkPlayLL();
	
	iAvcPayloadFormatter->CancelUlRequest();
    }

void UT_CAvcPayloadFormatWrite::UT_CAvcPayloadFormatWrite_FrameTimeIntervalL(  )
    {
	TMediaId mediaIdVideo( KUidMediaTypeVideo, 1 );
	iAvcPayloadFormatter->FrameTimeInterval(mediaIdVideo);
	TMediaId mediaIdAudio( KUidMediaTypeAudio, 1 );
	iAvcPayloadFormatter->FrameTimeInterval(mediaIdAudio);
    }

void UT_CAvcPayloadFormatWrite::UT_CAvcPayloadFormatWrite_DurationL(  )
    {
	TMediaId mediaIdVideo( KUidMediaTypeVideo, 1 );
	iAvcPayloadFormatter->Duration(mediaIdVideo);
    }

void UT_CAvcPayloadFormatWrite::UT_CAvcPayloadFormatWrite_CreateSinkBufferLL(  )
    {
	TBool bFlag;
	// NOTE: the ownership isn't transferred... so don't delete it
	iSinkBuff = iAvcPayloadFormatter->CreateSinkBufferL(KUidMediaTypeVideo, bFlag);
    }

void UT_CAvcPayloadFormatWrite::UT_CAvcPayloadFormatWrite_CreateSinkBufferLErrL(  )
    {
	TBool bFlag;
	TInt error;
	
	// NOTE: the ownership isn't transferred... so don't delete it
	TRAP(error, iSinkBuff = iAvcPayloadFormatter->CreateSinkBufferL(KUidMediaTypeAudio, bFlag));
    }

void UT_CAvcPayloadFormatWrite::UT_CAvcPayloadFormatWrite_SinkDataTypeCodeTestsL(  )
    {
	TFourCC fourCC('H','2','6','4');
	TFourCC temp4CC;
	TMediaId mediaId;
	TMediaId mediaIdVideo( KUidMediaTypeVideo, 1 );
	
	iAvcPayloadFormatter->SetSinkDataTypeCode(fourCC, mediaId);
	iAvcPayloadFormatter->SetSinkDataTypeCode(fourCC, mediaIdVideo);
	
	temp4CC = iAvcPayloadFormatter->SinkDataTypeCode(mediaId);
	temp4CC = iAvcPayloadFormatter->SinkDataTypeCode(mediaIdVideo);
	
	EUNIT_ASSERT(temp4CC == fourCC);
    }

void UT_CAvcPayloadFormatWrite::UT_CAvcPayloadFormatWrite_SinkThreadLogonL(  )
    {
   	iAvcPayloadFormatter->SinkThreadLogon( *this );
    }

void UT_CAvcPayloadFormatWrite::UT_CAvcPayloadFormatWrite_SinkThreadLogoffL(  )
    {
    iAvcPayloadFormatter->SinkThreadLogoff();
    }

void UT_CAvcPayloadFormatWrite::UT_CAvcPayloadFormatWrite_EmptyBufferLL(  )
    {
	TUid bId;
	bId.iUid = KImplUidRtpDataSource;
	TMediaId mediaIdVideo( KUidMediaTypeVideo, 1 );
	
	iSource = MDataSource::NewSourceL(bId, KNullDesC8);
	TBool bFlag;
	iSinkBuff = iAvcPayloadFormatter->CreateSinkBufferL(KUidMediaTypeVideo, bFlag);
	iAvcPayloadFormatter->EmptyBufferL(iSinkBuff, iSource, mediaIdVideo);
	
	// in actual flow, this is called by AO asynchronously, which is not done while using EUnit
	// so call it synchronously to complete the flow...
	DoEmptySourceBufferL();
    }

void UT_CAvcPayloadFormatWrite::UT_CAvcPayloadFormatWrite_BufferEmptiedLL(  )
    {
    iAvcPayloadFormatter->BufferEmptiedL(NULL);
    }

void UT_CAvcPayloadFormatWrite::UT_CAvcPayloadFormatWrite_SinkPrimeLL(  )
    {
    iAvcPayloadFormatter->SinkPrimeL();
    }

void UT_CAvcPayloadFormatWrite::UT_CAvcPayloadFormatWrite_SinkPlayLL(  )
    {
   	iAvcPayloadFormatter->SinkPlayL();
    }

void UT_CAvcPayloadFormatWrite::UT_CAvcPayloadFormatWrite_SinkPauseLL(  )
    {
   	iAvcPayloadFormatter->SinkPauseL();
    }

void UT_CAvcPayloadFormatWrite::UT_CAvcPayloadFormatWrite_SinkStopLL(  )
    {
	// initialization is not needed to test cancel request (as we don't call EmptyBufferL)
	UT_CAvcPayloadFormatWrite_SinkPrimeLL();
	UT_CAvcPayloadFormatWrite_SinkPlayLL();
	
	// stopL calls CancelUI internally, which requires above functiosn to be called in order to allocate resources
	iAvcPayloadFormatter->SinkStopL();
    }

void UT_CAvcPayloadFormatWrite::UT_CAvcPayloadFormatWrite_StreamVideoL()
	{
	// basic flow for video streaming

	UT_CAvcPayloadFormatWrite_SinkThreadLogonL();
	UT_CAvcPayloadFormatWrite_SinkPrimeLL();
	UT_CAvcPayloadFormatWrite_SinkPlayLL();
	
	UT_CAvcPayloadFormatWrite_EmptyBufferLL();
	
	UT_CAvcPayloadFormatWrite_SinkThreadLogoffL();
	}

void UT_CAvcPayloadFormatWrite::UT_CAvcPayloadFormatWrite_TestPayloadization_0_L(  )
    {
	// mode 0
	
	// Initialize
	UT_CAvcPayloadFormatWrite_ConfigurePayloadFormatL_0_L();
	UT_CAvcPayloadFormatWrite_SetPayloadTypeL();
	UT_CAvcPayloadFormatWrite_CreateSinkBufferLL();	// buffer ptr set into member variable
	
	TDes8& srcDes = (static_cast<CMMFDataBuffer*>(iSinkBuff))->Data();
	//srcDes.Copy(KFrameData);
	TPtrC8 strChunk = iRefData->Left(srcDes.Size());
	srcDes.Copy(strChunk);
	
	UT_CAvcPayloadFormatWrite_StreamVideoL();
    	
    }

void UT_CAvcPayloadFormatWrite::UT_CAvcPayloadFormatWrite_TestPayloadization_1_L(  )
    {
	// mode 1
	
	// Initialize
	UT_CAvcPayloadFormatWrite_ConfigurePayloadFormatL_1_L();
	UT_CAvcPayloadFormatWrite_SetPayloadTypeL();
	UT_CAvcPayloadFormatWrite_CreateSinkBufferLL();	// buffer ptr set into member variable
	
	TDes8& srcDes = (static_cast<CMMFDataBuffer*>(iSinkBuff))->Data();
	TPtrC8 strChunk = iRefData->Left(srcDes.Size());
	srcDes.Copy(strChunk);
	
	UT_CAvcPayloadFormatWrite_StreamVideoL();

    }

void UT_CAvcPayloadFormatWrite::UT_CAvcPayloadFormatWrite_TestPayloadization_2_L(  )
    {
	// this generates corner case error conditions for code coverage
	
	TInt error;
	
	_LIT8(KFrameData, "ABCDEFGHIJKLMNOPQRSTUVWXVZabcdefghijklmnopqrstuvwxvz0123456789");
	
	    	
	// Initialize
	UT_CAvcPayloadFormatWrite_ConfigurePayloadFormatL_2_L(2);	// interleaved mode
	UT_CAvcPayloadFormatWrite_SetPayloadTypeL();
	UT_CAvcPayloadFormatWrite_CreateSinkBufferLL();	// buffer ptr set into member variable
	
	TDes8& srcDes = (static_cast<CMMFDataBuffer*>(iSinkBuff))->Data();
	srcDes.Copy(KFrameData);
	
	
	UT_CAvcPayloadFormatWrite_SinkThreadLogonL();
	UT_CAvcPayloadFormatWrite_SinkPrimeLL();
	UT_CAvcPayloadFormatWrite_SinkPlayLL();
	
	TMediaId mediaId;
	TRAP(error, iAvcPayloadFormatter->EmptyBufferL(iSinkBuff, iSource, mediaId));
	
	TMediaId mediaIdVideo( KUidMediaTypeVideo, 1 );
	TRAP(error, iAvcPayloadFormatter->EmptyBufferL(iSinkBuff, NULL, mediaIdVideo));
	TRAP(error, iAvcPayloadFormatter->EmptyBufferL(NULL, iSource, mediaIdVideo));
	iAvcPayloadFormatter->EmptyBufferL(iSinkBuff, iSource, mediaIdVideo);
	// has to call this synchronously...
	TRAP(error, iAvcPayloadFormatter->EmptySourceBufferL());
	
	UT_CAvcPayloadFormatWrite_ConfigurePayloadFormatL_2_L(3);	// invalid mode
	iAvcPayloadFormatter->EmptyBufferL(iSinkBuff, iSource, mediaIdVideo);
	// has to call this synchronously...
	TRAP(error, iAvcPayloadFormatter->EmptySourceBufferL());
	
	UT_CAvcPayloadFormatWrite_SinkThreadLogoffL();

    }

void UT_CAvcPayloadFormatWrite::UT_CAvcPayloadFormatWrite_TestPayloadization_3_L(  )
    {
    	
	_LIT8(KFrameData, "ABCDEFGHIJKLMNOPQRSTUVWXVZabcdefghijklmnopqrstuvwxvz0123456789");
	
	// mode 1
	
	// Initialize
	UT_CAvcPayloadFormatWrite_ConfigurePayloadFormatL_0_L();
	UT_CAvcPayloadFormatWrite_SetPayloadTypeL();
	UT_CAvcPayloadFormatWrite_CreateSinkBufferLL();	// buffer ptr set into member variable
	
	TDes8& srcDes = (static_cast<CMMFDataBuffer*>(iSinkBuff))->Data();
	srcDes.Copy(KFrameData);
	
	UT_CAvcPayloadFormatWrite_StreamVideoL();

    }

void UT_CAvcPayloadFormatWrite::UT_CAvcPayloadFormatWrite_TestPayloadization_Of_SpropParameters_L(  )
    {
    // Formatter should ensure that sprop parameter sets will not be marked with rtp marker bit	
    //
    
	_LIT8(KFrameData, "ABCDEFGHIJKLMNOPQRSTUVWXVZabcdefghijklmnopqrstuvwxvz0123456789");
	
	// Initialize
	UT_CAvcPayloadFormatWrite_ConfigurePayloadFormatL_0_L();
	UT_CAvcPayloadFormatWrite_SetPayloadTypeL();
	UT_CAvcPayloadFormatWrite_CreateSinkBufferLL();	// buffer ptr set into member variable
	
	TDes8& srcDes = (static_cast<CMMFDataBuffer*>(iSinkBuff))->Data();
	srcDes.Copy(KFrameData);
	
	// Add nalu start and sps 
	srcDes[ 0 ] = 0x0;
	srcDes[ 1 ] = 0x0;
	srcDes[ 2 ] = 0x0;
	srcDes[ 3 ] = 0x1;
	srcDes[ 4 ] = 0x7;
	
	iAvcPayloadFormatter->iRtpSendHeader.iMarker = 1;
	
	UT_CAvcPayloadFormatWrite_StreamVideoL();
	
	EUNIT_ASSERT( iAvcPayloadFormatter->iRtpSendHeader.iMarker == 0 );

    }
    
// HELPERS

void UT_CAvcPayloadFormatWrite::DoEmptySourceBufferL()
    {
    iAvcPayloadFormatter->EmptySourceBufferL();
    }

/*****  TEST TABLE *****/


EUNIT_BEGIN_TEST_TABLE(
    UT_CAvcPayloadFormatWrite,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "TestPayloadizationL : mode 0 - test ",
    "CAvcPayloadFormatWrite",
    "AVC Paylodization flow test",
    "FUNCTIONALITY",
    SetupL, UT_CAvcPayloadFormatWrite_TestPayloadization_0_L, Teardown)

EUNIT_TEST(
    "TestPayloadizationL : mode 1 - test ",
    "CAvcPayloadFormatWrite",
    "AVC Paylodization flow test",
    "FUNCTIONALITY",
    SetupL, UT_CAvcPayloadFormatWrite_TestPayloadization_1_L, Teardown)

EUNIT_TEST(
    "TestPayloadizationL : corner cases - test ",
    "CAvcPayloadFormatWrite",
    "AVC Paylodization flow test",
    "FUNCTIONALITY",
    SetupL, UT_CAvcPayloadFormatWrite_TestPayloadization_2_L, Teardown)

EUNIT_TEST(
    "TestPayloadizationL : invalid NALU - test ",
    "CAvcPayloadFormatWrite",
    "AVC Paylodization flow test",
    "FUNCTIONALITY",
    SetupL, UT_CAvcPayloadFormatWrite_TestPayloadization_3_L, Teardown)

EUNIT_TEST(
    "TestPayloadizationL : sprop parameters - test ",
    "CAvcPayloadFormatWrite",
    "AVC Paylodization flow test",
    "FUNCTIONALITY",
    SetupL, UT_CAvcPayloadFormatWrite_TestPayloadization_Of_SpropParameters_L, Teardown)
    
EUNIT_TEST(
    "EmptySourceBufferL - test ",
    "CAvcPayloadFormatWrite",
    "EmptySourceBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CAvcPayloadFormatWrite_EmptySourceBufferLL, Teardown)

EUNIT_TEST(
    "SourceBufferEmptiedL - test ",
    "CAvcPayloadFormatWrite",
    "SourceBufferEmptiedL",
    "FUNCTIONALITY",
    SetupL, UT_CAvcPayloadFormatWrite_SourceBufferEmptiedLL, Teardown)

EUNIT_TEST(
    "SetPayloadType - test ",
    "CAvcPayloadFormatWrite",
    "SetPayloadType",
    "FUNCTIONALITY",
    SetupL, UT_CAvcPayloadFormatWrite_SetPayloadTypeL, Teardown)

EUNIT_TEST(
    "CancelUlRequest - test ",
    "CAvcPayloadFormatWrite",
    "CancelUlRequest",
    "FUNCTIONALITY",
    SetupL, UT_CAvcPayloadFormatWrite_CancelUlRequestL, Teardown)

EUNIT_TEST(
    "FrameTimeInterval - test ",
    "CAvcPayloadFormatWrite",
    "FrameTimeInterval",
    "FUNCTIONALITY",
    SetupL, UT_CAvcPayloadFormatWrite_FrameTimeIntervalL, Teardown)

EUNIT_TEST(
    "Duration - test ",
    "CAvcPayloadFormatWrite",
    "Duration",
    "FUNCTIONALITY",
    SetupL, UT_CAvcPayloadFormatWrite_DurationL, Teardown)

EUNIT_TEST(
    "CreateSinkBufferL - test ",
    "CAvcPayloadFormatWrite",
    "CreateSinkBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CAvcPayloadFormatWrite_CreateSinkBufferLL, Teardown2)

EUNIT_TEST(
    "CreateSinkBufferErrL - test ",
    "CAvcPayloadFormatWrite",
    "CreateSinkBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CAvcPayloadFormatWrite_CreateSinkBufferLErrL, Teardown2)


EUNIT_TEST(
    "SinkDataTypeCodeTests - test ",
    "CAvcPayloadFormatWrite",
    "SinkDataTypeCodeTests",
    "FUNCTIONALITY",
    SetupL, UT_CAvcPayloadFormatWrite_SinkDataTypeCodeTestsL, Teardown)

EUNIT_TEST(
    "SinkThreadLogon - test ",
    "CAvcPayloadFormatWrite",
    "SinkThreadLogon",
    "FUNCTIONALITY",
    SetupL, UT_CAvcPayloadFormatWrite_SinkThreadLogonL, Teardown)

EUNIT_TEST(
    "SinkThreadLogoff - test ",
    "CAvcPayloadFormatWrite",
    "SinkThreadLogoff",
    "FUNCTIONALITY",
    SetupL, UT_CAvcPayloadFormatWrite_SinkThreadLogoffL, Teardown)

EUNIT_TEST(
    "EmptyBufferL - test ",
    "CAvcPayloadFormatWrite",
    "EmptyBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CAvcPayloadFormatWrite_EmptyBufferLL, Teardown)

EUNIT_TEST(
    "BufferEmptiedL - test ",
    "CAvcPayloadFormatWrite",
    "BufferEmptiedL",
    "FUNCTIONALITY",
    SetupL, UT_CAvcPayloadFormatWrite_BufferEmptiedLL, Teardown)

EUNIT_TEST(
    "SinkPrimeL - test ",
    "CAvcPayloadFormatWrite",
    "SinkPrimeL",
    "FUNCTIONALITY",
    SetupL, UT_CAvcPayloadFormatWrite_SinkPrimeLL, Teardown)

EUNIT_TEST(
    "SinkPlayL - test ",
    "CAvcPayloadFormatWrite",
    "SinkPlayL",
    "FUNCTIONALITY",
    SetupL, UT_CAvcPayloadFormatWrite_SinkPlayLL, Teardown)

EUNIT_TEST(
    "SinkPauseL - test ",
    "CAvcPayloadFormatWrite",
    "SinkPauseL",
    "FUNCTIONALITY",
    SetupL, UT_CAvcPayloadFormatWrite_SinkPauseLL, Teardown)

EUNIT_TEST(
    "SinkStopL - test ",
    "CAvcPayloadFormatWrite",
    "SinkStopL",
    "FUNCTIONALITY",
    SetupL, UT_CAvcPayloadFormatWrite_SinkStopLL, Teardown)

EUNIT_TEST(
    "NewL corner cases - test ",
    "CAvcPayloadFormatWrite",
    "NewL",
    "FUNCTIONALITY",
    EmptySetupL, UT_CAvcPayloadFormatWrite_NewLErrL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
