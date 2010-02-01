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
#include "UT_CMccRedPayloadWrite.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <e32base.h>
#include <mmf/server/mmfdatabuffer.h>
#include <digia/eunit/CEunitAllocTestCaseDecorator.h>

//  INTERNAL INCLUDES
#include "rtpheader.h"
#include "MccRedPayloadWrite.h"
#include "MccRtpDataSink.h"
#include "mccuids.hrh"
#include "mccrtpmediaclock.h"
#include "amrpayloadformatwrite.h"

const TUid KRtpDataSink              =  { KImplUidRtpDataSink               };
const TUint KRedPayloadType = 100;


// CONSTRUCTION
UT_CMccRedPayloadWrite* UT_CMccRedPayloadWrite::NewL()
    {
    UT_CMccRedPayloadWrite* self = UT_CMccRedPayloadWrite::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMccRedPayloadWrite* UT_CMccRedPayloadWrite::NewLC()
    {
    UT_CMccRedPayloadWrite* self = new( ELeave ) UT_CMccRedPayloadWrite();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccRedPayloadWrite::~UT_CMccRedPayloadWrite()
    {
    }

// Default constructor
UT_CMccRedPayloadWrite::UT_CMccRedPayloadWrite()
    {
    }

// Second phase construct
void UT_CMccRedPayloadWrite::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CMccRedPayloadWrite::SetupL(  )
    {
    iAllocTest = EFalse;
    iDataSinkStub = CMccRtpDataSink::NewSinkL( KRtpDataSink, KNullDesC8 );
    iWrite = CMccRedPayloadWrite::NewL( iDataSinkStub );
    iDataSourceStub = CAmrPayloadFormatWrite::NewL( iWrite );
    
    iBuffer = CMMFDataBuffer::NewL( 1000 );
    iRtpMediaClock = CMccRtpMediaClock::NewL();
    
    TMccRedPayloadWriteConfig config;
    config.iRedBlockCount = 1;
    config.iMaxPayloadSize = KILBCFrameSize20Ms;
    config.iNumOfEncodings = 1;
    config.iRedPayloadType = KRedPayloadType;
    config.InitPayloadTypes();
    config.iEncPayloadTypes[0] = 97;
    TMccRedPayloadWritePckg pckg( config );    
    
    iWrite->ConfigurePayloadFormatL( pckg, *iRtpMediaClock );
    } 

void UT_CMccRedPayloadWrite::SetupAL(  )
    {
    iAllocTest = ETrue;
    iDataSinkStub = CMccRtpDataSink::NewSinkL( KRtpDataSink, KNullDesC8 );
    iWrite = CMccRedPayloadWrite::NewL( iDataSinkStub );
    iDataSourceStub = CAmrPayloadFormatWrite::NewL( iWrite );
    
    iBuffer = CMMFDataBuffer::NewL( 1000 );
    iRtpMediaClock = CMccRtpMediaClock::NewL();
    TMccRedPayloadWriteConfig config;
    config.iRedBlockCount = 1;
    config.iMaxPayloadSize = KILBCFrameSize20Ms;
    config.iNumOfEncodings = 1;
    config.iRedPayloadType = KRedPayloadType;
    config.InitPayloadTypes();
    config.iEncPayloadTypes[0] = 97;
    TMccRedPayloadWritePckg pckg( config );    
    
    iWrite->ConfigurePayloadFormatL( pckg, *iRtpMediaClock );
    }
    
void UT_CMccRedPayloadWrite::Teardown(  )
    {
    delete iBuffer;
    iBuffer = NULL;

    delete iDataSourceStub;
    iDataSourceStub = NULL;
    
    delete iDataSinkStub;
    iDataSinkStub = NULL;

    delete iWrite;
    iWrite = NULL;
    
    delete iRtpMediaClock;
    iRtpMediaClock = NULL;
    
    REComSession::FinalClose();
    }

void UT_CMccRedPayloadWrite::UT_CMccRedPayloadWrite_NewLL(  )
    {
    //EUNIT_ASSERT( EFalse );
    }

void UT_CMccRedPayloadWrite::UT_CMccRedPayloadWrite_SetPayloadTypesL(  )
    {
    RArray<TUint> encPayloadTypes;
    encPayloadTypes.AppendL( KRedPayloadType );
    if ( !iAllocTest )
        {
        EUNIT_ASSERT_EQUALS( iWrite->SetPayloadTypes( encPayloadTypes ), KErrArgument );
        encPayloadTypes.AppendL( 2 );
        encPayloadTypes.AppendL( 3 );
        EUNIT_ASSERT_EQUALS( iWrite->SetPayloadTypes( encPayloadTypes ), KErrNone );
        encPayloadTypes.Close();
        }
    else
        {
        encPayloadTypes.AppendL( 2 );
        EUNIT_ASSERT_NO_LEAVE( iWrite->SetPayloadTypes( encPayloadTypes ) );
        
        encPayloadTypes.Close();
        }
    }

void UT_CMccRedPayloadWrite::UT_CMccRedPayloadWrite_ConfigurePayloadFormatLL(  )
    {
    TMccRedPayloadWriteConfig config;
    config.iRedBlockCount = 1;
    config.iMaxPayloadSize = KILBCFrameSize20Ms;
    config.iNumOfEncodings = 1;
    config.iRedPayloadType = KRedPayloadType;
    config.InitPayloadTypes();
    config.iEncPayloadTypes[0] = 96;
    TMccRedPayloadWritePckg configpckg( config ); 
    
    iWrite->ConfigurePayloadFormatL( configpckg, *iRtpMediaClock );
    }

void UT_CMccRedPayloadWrite::UT_CMccRedPayloadWrite_EmptyBufferLL(  )
    {
    TRtpSendHeader rtpHeader;
    rtpHeader.iPayloadType = KRedPayloadType;

    EUNIT_ASSERT_SPECIFIC_LEAVE( iWrite->EmptyBufferL( 
        NULL, iDataSourceStub, KUidMediaTypeAudio, rtpHeader ), KErrArgument );
    
    EUNIT_ASSERT_SPECIFIC_LEAVE( iWrite->EmptyBufferL( 
        iBuffer, NULL, KUidMediaTypeAudio, rtpHeader ), KErrArgument );
    
    EUNIT_ASSERT_SPECIFIC_LEAVE( iWrite->EmptyBufferL( 
        iBuffer, NULL, KUidMediaTypeVideo, rtpHeader ), KErrArgument );
    
 	TRAPD(err, iWrite->EmptyBufferL( 
            iBuffer, iDataSourceStub, KUidMediaTypeAudio, rtpHeader ));  
    if (err == KErrNoMemory) 
    	{
    	User::Leave(KErrNoMemory);
    	}
    if (err != KErrNone)
    	{
    	User::Leave(err);
    	}
    
    }

void UT_CMccRedPayloadWrite::UT_CMccRedPayloadWrite_FrameTimeIntervalL(  )
    {
    TMediaId mediaId( KUidMediaTypeAudio, 1 );
    TTimeIntervalMicroSeconds getMicroSecs;
    
    getMicroSecs = iWrite->FrameTimeInterval( mediaId );
    EUNIT_ASSERT_EQUALS( getMicroSecs.Int64(), 0 ); // No implementation
    }

void UT_CMccRedPayloadWrite::UT_CMccRedPayloadWrite_DurationL(  )
    {
    TTimeIntervalMicroSeconds getMicroSecs;
    TMediaId mediaId( KUidMediaTypeAudio, 1 );
    
    getMicroSecs = iWrite->Duration( mediaId );
    EUNIT_ASSERT_EQUALS( getMicroSecs.Int64(), 0 );
    }

void UT_CMccRedPayloadWrite::UT_CMccRedPayloadWrite_CreateSinkBufferLL(  )
    {
    TBool reference( ETrue );
    
    CMMFBuffer* buffer( NULL);
    CleanupStack::PushL( buffer );
    EUNIT_ASSERT_SPECIFIC_LEAVE( buffer = iWrite->CreateSinkBufferL( 
        KUidMediaTypeAudio, reference ), KErrNotSupported );
    EUNIT_ASSERT( NULL == buffer );
    EUNIT_ASSERT( ETrue == reference );
    
    EUNIT_ASSERT_SPECIFIC_LEAVE( buffer = iWrite->CreateSinkBufferL( 
        KUidMediaTypeVideo, reference ), KErrNotSupported );
    EUNIT_ASSERT( NULL == buffer );
    EUNIT_ASSERT( ETrue == reference );
    CleanupStack::PopAndDestroy( buffer );
    }

void UT_CMccRedPayloadWrite::UT_CMccRedPayloadWrite_SinkDataTypeCodeL(  )
    {
    TMediaId mediaId( KUidMediaTypeAudio, 1 );
    TMediaId mediaIdVideo( KUidMediaTypeVideo, 1 );
    
    EUNIT_ASSERT_NO_LEAVE( iWrite->SinkDataTypeCode( mediaIdVideo ) );
    EUNIT_ASSERT_NO_LEAVE( iWrite->SinkDataTypeCode( mediaId ) );
    }

void UT_CMccRedPayloadWrite::UT_CMccRedPayloadWrite_SetSinkDataTypeCodeL(  )
    {
    TFourCC fourcc;
    TMediaId mediaId( KUidMediaTypeAudio, 1 );
    TMediaId mediaIdVideo( KUidMediaTypeVideo, 1 );
    
    fourcc = iWrite->SinkDataTypeCode( mediaId );
    EUNIT_ASSERT_EQUALS( iWrite->SetSinkDataTypeCode( fourcc, mediaId ), KErrNone );
    fourcc = iWrite->SinkDataTypeCode( mediaIdVideo );
    EUNIT_ASSERT_EQUALS( iWrite->SetSinkDataTypeCode( fourcc, mediaId ), KErrNone );
    }

void UT_CMccRedPayloadWrite::UT_CMccRedPayloadWrite_SinkThreadLogonL(  )
    {
    EUNIT_ASSERT_EQUALS( iWrite->SinkThreadLogon( *this ), KErrNone );
    }

void UT_CMccRedPayloadWrite::UT_CMccRedPayloadWrite_SinkThreadLogoffL(  )
    {
    EUNIT_ASSERT_NO_LEAVE( iWrite->SinkThreadLogoff() );
    }

void UT_CMccRedPayloadWrite::UT_CMccRedPayloadWrite_SinkPrimeLL(  )
    {
    EUNIT_ASSERT_NO_LEAVE( iWrite->SinkPrimeL() );
    }

void UT_CMccRedPayloadWrite::UT_CMccRedPayloadWrite_SinkPlayLL(  )
    {
    EUNIT_ASSERT_NO_LEAVE( iWrite->SinkPlayL() );
    }

void UT_CMccRedPayloadWrite::UT_CMccRedPayloadWrite_SinkPauseLL(  )
    {
    EUNIT_ASSERT_NO_LEAVE( iWrite->SinkPauseL() );
    }

void UT_CMccRedPayloadWrite::UT_CMccRedPayloadWrite_SinkStopLL(  )
    {
    EUNIT_ASSERT_NO_LEAVE( iWrite->SinkStopL() );
    }
    
void UT_CMccRedPayloadWrite::UT_CMccRedPayloadWrite_BufferEmptiedLL(  )
    {
    TRtpSendHeader rtpheader;
    TRtpTimeStamp timestamp( 1 );
    rtpheader.iPayloadType = 97;
    rtpheader.iTimestamp = timestamp;

    EUNIT_ASSERT_SPECIFIC_LEAVE( iWrite->BufferEmptiedL( NULL ), KErrArgument );
    EUNIT_ASSERT_SPECIFIC_LEAVE( iWrite->BufferEmptiedL( iBuffer ), KErrNotReady );
    
	TRAPD(err, iWrite->EmptyBufferL( 
            iBuffer, iDataSourceStub, KUidMediaTypeAudio, rtpheader ));  
    if (err == KErrNoMemory) 
    	{
    	User::Leave(KErrNoMemory);
    	}
    	      
    EUNIT_ASSERT_NO_LEAVE( iWrite->BufferEmptiedL( iBuffer ) );
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMccRedPayloadWrite,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMccRedPayloadWrite",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRedPayloadWrite_NewLL, Teardown)

EUNIT_TEST(
    "SetPayloadTypes - test ",
    "CMccRedPayloadWrite",
    "SetPayloadTypes",
    "FUNCTIONALITY",
    SetupL, UT_CMccRedPayloadWrite_SetPayloadTypesL, Teardown)

EUNIT_TEST(
    "ConfigurePayloadFormat - test ",
    "CMccRedPayloadWrite",
    "ConfigurePayloadFormatL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRedPayloadWrite_ConfigurePayloadFormatLL, Teardown)
    
EUNIT_TEST(
    "EmptyBufferL - test ",
    "CMccRedPayloadWrite",
    "EmptyBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRedPayloadWrite_EmptyBufferLL, Teardown)


EUNIT_TEST(
    "FrameTimeInterval - test ",
    "CMccRedPayloadWrite",
    "FrameTimeInterval",
    "FUNCTIONALITY",
    SetupL, UT_CMccRedPayloadWrite_FrameTimeIntervalL, Teardown)


EUNIT_TEST(
    "Duration - test ",
    "CMccRedPayloadWrite",
    "Duration",
    "FUNCTIONALITY",
    SetupL, UT_CMccRedPayloadWrite_DurationL, Teardown)


EUNIT_TEST(
    "CreateSinkBufferL - test ",
    "CMccRedPayloadWrite",
    "CreateSinkBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRedPayloadWrite_CreateSinkBufferLL, Teardown)

EUNIT_TEST(
    "SinkDataTypeCode - test ",
    "CMccRedPayloadWrite",
    "SinkDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CMccRedPayloadWrite_SinkDataTypeCodeL, Teardown)

EUNIT_TEST(
    "SetSinkDataTypeCode - test ",
    "CMccRedPayloadWrite",
    "SetSinkDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CMccRedPayloadWrite_SetSinkDataTypeCodeL, Teardown)

EUNIT_TEST(
    "SinkThreadLogon - test ",
    "CMccRedPayloadWrite",
    "SinkThreadLogon",
    "FUNCTIONALITY",
    SetupL, UT_CMccRedPayloadWrite_SinkThreadLogonL, Teardown)

EUNIT_TEST(
    "SinkThreadLogoff - test ",
    "CMccRedPayloadWrite",
    "SinkThreadLogoff",
    "FUNCTIONALITY",
    SetupL, UT_CMccRedPayloadWrite_SinkThreadLogoffL, Teardown)

EUNIT_TEST(
    "SinkPrimeL - test ",
    "CMccRedPayloadWrite",
    "SinkPrimeL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRedPayloadWrite_SinkPrimeLL, Teardown)

EUNIT_TEST(
    "SinkPlayL - test ",
    "CMccRedPayloadWrite",
    "SinkPlayL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRedPayloadWrite_SinkPlayLL, Teardown)

EUNIT_TEST(
    "SinkPauseL - test ",
    "CMccRedPayloadWrite",
    "SinkPauseL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRedPayloadWrite_SinkPauseLL, Teardown)

EUNIT_TEST(
    "SinkStopL - test ",
    "CMccRedPayloadWrite",
    "SinkStopL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRedPayloadWrite_SinkStopLL, Teardown)

EUNIT_TEST(
    "BufferEmptiedL - test ",
    "CMccRedPayloadWrite",
    "BufferEmptiedL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRedPayloadWrite_BufferEmptiedLL, Teardown)


// ALLOC-TESTS
#ifdef ALLOC_TEST_ON

EUNIT_ALLOC_TEST(
    "SetPayloadTypes - alloc test ",
    "CMccRedPayloadWrite",
    "SetPayloadTypes",
    "ERRORHANDLING",
    SetupAL, UT_CMccRedPayloadWrite_SetPayloadTypesL, Teardown)

EUNIT_ALLOC_TEST(
    "ConfigurePayloadFormat - alloc test ",
    "CMccRedPayloadWrite",
    "ConfigurePayloadFormatL",
    "ERRORHANDLING",
    SetupAL, UT_CMccRedPayloadWrite_ConfigurePayloadFormatLL, Teardown)
    
EUNIT_ALLOC_TEST(
    "EmptyBufferL - alloc test ",
    "CMccRedPayloadWrite",
    "EmptyBufferL",
    "ERRORHANDLING",
    SetupAL, UT_CMccRedPayloadWrite_EmptyBufferLL, Teardown)

EUNIT_ALLOC_TEST(
    "FrameTimeInterval - alloc test ",
    "CMccRedPayloadWrite",
    "FrameTimeInterval",
    "ERRORHANDLING",
    SetupAL, UT_CMccRedPayloadWrite_FrameTimeIntervalL, Teardown)

EUNIT_ALLOC_TEST(
    "Duration - alloc test ",
    "CMccRedPayloadWrite",
    "Duration",
    "ERRORHANDLING",
    SetupAL, UT_CMccRedPayloadWrite_DurationL, Teardown)

EUNIT_ALLOC_TEST(
    "CreateSinkBufferL - alloc test ",
    "CMccRedPayloadWrite",
    "CreateSinkBufferL",
    "ERRORHANDLING",
    SetupAL, UT_CMccRedPayloadWrite_CreateSinkBufferLL, Teardown)

EUNIT_ALLOC_TEST(
    "SinkDataTypeCode - alloc test ",
    "CMccRedPayloadWrite",
    "SinkDataTypeCode",
    "ERRORHANDLING",
    SetupAL, UT_CMccRedPayloadWrite_SinkDataTypeCodeL, Teardown)

EUNIT_ALLOC_TEST(
    "SetSinkDataTypeCode - alloc test ",
    "CMccRedPayloadWrite",
    "SetSinkDataTypeCode",
    "ERRORHANDLING",
    SetupAL, UT_CMccRedPayloadWrite_SetSinkDataTypeCodeL, Teardown)

EUNIT_ALLOC_TEST(
    "SinkThreadLogon - alloc test ",
    "CMccRedPayloadWrite",
    "SinkThreadLogon",
    "ERRORHANDLING",
    SetupAL, UT_CMccRedPayloadWrite_SinkThreadLogonL, Teardown)

EUNIT_ALLOC_TEST(
    "SinkThreadLogoff - alloc test ",
    "CMccRedPayloadWrite",
    "SinkThreadLogoff",
    "ERRORHANDLING",
    SetupAL, UT_CMccRedPayloadWrite_SinkThreadLogoffL, Teardown)

EUNIT_ALLOC_TEST(
    "SinkPrimeL - alloc test ",
    "CMccRedPayloadWrite",
    "SinkPrimeL",
    "ERRORHANDLING",
    SetupAL, UT_CMccRedPayloadWrite_SinkPrimeLL, Teardown)

EUNIT_ALLOC_TEST(
    "SinkPlayL - alloc test ",
    "CMccRedPayloadWrite",
    "SinkPlayL",
    "ERRORHANDLING",
    SetupAL, UT_CMccRedPayloadWrite_SinkPlayLL, Teardown)

EUNIT_ALLOC_TEST(
    "SinkPauseL - alloc test ",
    "CMccRedPayloadWrite",
    "SinkPauseL",
    "ERRORHANDLING",
    SetupAL, UT_CMccRedPayloadWrite_SinkPauseLL, Teardown)

EUNIT_ALLOC_TEST(
    "SinkStopL - alloc test ",
    "CMccRedPayloadWrite",
    "SinkStopL",
    "ERRORHANDLING",
    SetupAL, UT_CMccRedPayloadWrite_SinkStopLL, Teardown)

EUNIT_ALLOC_TEST(
    "BufferEmptiedL - alloc test ",
    "CMccRedPayloadWrite",
    "BufferEmptiedL",
    "ERRORHANDLING",
    SetupAL, UT_CMccRedPayloadWrite_BufferEmptiedLL, Teardown)

#endif 

EUNIT_END_TEST_TABLE

//  END OF FILE
