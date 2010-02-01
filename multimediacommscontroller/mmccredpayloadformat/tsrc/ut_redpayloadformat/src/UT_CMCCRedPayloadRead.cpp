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
#include "UT_CMccRedPayloadRead.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <digia/eunit/CEunitAllocTestCaseDecorator.h>
#include <e32base.h>

//  INTERNAL INCLUDES
#include "MccRedPayloadRead.h"
#include "MccRtpDataSource.h"
#include "MccRtpDataSink.h"
#include "mccuids.hrh"
#include "amrpayloadformatread.h"

const TUid KRtpDataSource               =  { KImplUidRtpDataSource   }; 
const TUint KRedPayloadType = 100;

// CONSTRUCTION
UT_CMccRedPayloadRead* UT_CMccRedPayloadRead::NewL()
    {
    UT_CMccRedPayloadRead* self = UT_CMccRedPayloadRead::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMccRedPayloadRead* UT_CMccRedPayloadRead::NewLC()
    {
    UT_CMccRedPayloadRead* self = new( ELeave ) UT_CMccRedPayloadRead();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccRedPayloadRead::~UT_CMccRedPayloadRead()
    {
    }

// Default constructor
UT_CMccRedPayloadRead::UT_CMccRedPayloadRead()
    {
    }

// Second phase construct
void UT_CMccRedPayloadRead::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    iFourCC.Set( TFourCC( 'R','E','D','P' ) );
    
    CEUnitTestSuiteClass::ConstructL();
    }

void UT_CMccRedPayloadRead::SetupAL(  )
    {
    iAllocTest = ETrue;
    iDataSourceStub = CMccRtpDataSource::NewSourceL( KRtpDataSource, KNullDesC8 );
    iRead = CMccRedPayloadRead::NewL( iDataSourceStub );
    
    iDataBuf = CMMFDataBuffer::NewL( KILBCFrameSize20Ms );
    iDataBuf->Data().Copy( KILBCTestVector20Ms );
    
    iDataSinkStub = CAmrPayloadFormatRead::NewL( iRead );
    } 

//  METHODS
void UT_CMccRedPayloadRead::Setup2L(  )
    {
    iAllocTest = EFalse;
    iDataSourceStub = CMccRtpDataSource::NewSourceL( KRtpDataSource, KNullDesC8 );
    iRead = CMccRedPayloadRead::NewL( iDataSourceStub );
    
    iDataBuf = CMMFDataBuffer::NewL( KILBCFrameSize20Ms );
    iDataBuf->Data().Copy( KILBCTestVector20Ms );
    
    iDataSinkStub = CAmrPayloadFormatRead::NewL( iRead );
    
    TMccRedPayloadReadConfig config;
    config.iRedBlockCount = 1;
    config.iMaxPayloadSize = KILBCFrameSize20Ms;
    config.iNumOfEncodings = 1;
    config.iRedPayloadType = KRedPayloadType;
    config.InitPayloadTypes();
    config.iEncPayloadTypes[0] = 97;
    TMccRedPayloadReadPckg pckg( config );    
    
    iRead->ConfigurePayloadFormatL( pckg );
    iRead->iFourCC.Set( TFourCC( 'R','E','D','P' ) );
    }
    
    //  METHODS
void UT_CMccRedPayloadRead::Setup2AL(  )
    {
    iAllocTest = ETrue;
    iDataSourceStub = CMccRtpDataSource::NewSourceL( KRtpDataSource, KNullDesC8 );
    iRead = CMccRedPayloadRead::NewL( iDataSourceStub );
    
    iDataBuf = CMMFDataBuffer::NewL( KILBCFrameSize20Ms );
    iDataBuf->Data().Copy( KILBCTestVector20Ms );
    
    iDataSinkStub = CAmrPayloadFormatRead::NewL( iRead );
    
    TMccRedPayloadReadConfig config;
    config.iRedBlockCount = 1;
    config.iMaxPayloadSize = KILBCFrameSize20Ms;
    config.iNumOfEncodings = 1;
    config.iRedPayloadType = KRedPayloadType;
    config.InitPayloadTypes();
    config.iEncPayloadTypes[0] = 97;
    TMccRedPayloadReadPckg pckg( config );    
    
    iRead->ConfigurePayloadFormatL( pckg );
    
    iRead->iFourCC.Set( TFourCC( 'R','E','D','P' ) );
    }


void UT_CMccRedPayloadRead::SetupL(  )
    {
    iAllocTest = EFalse;
    iDataSourceStub = CMccRtpDataSource::NewSourceL( KRtpDataSource, KNullDesC8 );
    iRead = CMccRedPayloadRead::NewL( iDataSourceStub );
    }

void UT_CMccRedPayloadRead::Teardown(  )
    {
    delete iDataBuf;
    iDataBuf = NULL;
    
    delete iDataSourceStub;
    iDataSourceStub = NULL;
    
    delete iDataSinkStub;
    iDataSinkStub = NULL;
    
    delete iRead;
    iRead = NULL;
    
    REComSession::FinalClose();
    }


void UT_CMccRedPayloadRead::UT_CMccRedPayloadRead_SetPayloadTypesL(  )
    {
    RArray<TUint> decPayloadTypes;
    decPayloadTypes.AppendL( KRedPayloadType );
    if ( !iAllocTest )
        {
        EUNIT_ASSERT_EQUALS( iRead->SetPayloadTypes( decPayloadTypes ), KErrArgument );
        decPayloadTypes.AppendL( 2 );
        decPayloadTypes.AppendL( 3 );
        EUNIT_ASSERT_EQUALS( iRead->SetPayloadTypes( decPayloadTypes ), KErrNone );
        decPayloadTypes.Close();
        }
    else
        {
        decPayloadTypes.AppendL( 2 );
        EUNIT_ASSERT_NO_LEAVE( iRead->SetPayloadTypes( decPayloadTypes ) );
        
        decPayloadTypes.Close();
        }
    }

void UT_CMccRedPayloadRead::UT_CMccRedPayloadRead_BufferFilledLL(  )
    {
    TRtpRecvHeader rtpheader;
    rtpheader.iPayloadType = KRedPayloadType;
    
    // No valid fillbuffer request
    EUNIT_ASSERT_LEAVE( iRead->DataBufferFilledL( iDataBuf, rtpheader ) );
    
    EUNIT_ASSERT_NO_LEAVE( iRead->FillBufferL( iDataBuf, iDataSinkStub, KUidMediaTypeAudio ) );
    EUNIT_ASSERT_LEAVE( iRead->DataBufferFilledL( NULL, rtpheader ) );
    
    EUNIT_ASSERT_LEAVE( iRead->DataBufferFilledL( iDataBuf, rtpheader ));
        
    
    // Other payloads should pass through also
    TRtpRecvHeader rtpheader2;
    rtpheader2.iPayloadType = 0;
    
    EUNIT_ASSERT_NO_LEAVE( iRead->FillBufferL( iDataBuf, iDataSinkStub, KUidMediaTypeAudio ) );
    EUNIT_ASSERT_NO_LEAVE( iRead->DataBufferFilledL( iDataBuf, rtpheader2 ) );
    }

void UT_CMccRedPayloadRead::UT_CMccRedPayloadRead_ConfigurePayloadFormatLL(  )
    {
    TMccRedPayloadReadConfig config;
    config.iRedBlockCount = 1;
    config.iMaxPayloadSize = KILBCFrameSize20Ms;
    config.iNumOfEncodings = 1;
    config.iRedPayloadType = KRedPayloadType;
    config.InitPayloadTypes();
    config.iEncPayloadTypes[0] = 97;
    TMccRedPayloadReadPckg configpckg( config );   
    
    iRead->ConfigurePayloadFormatL( configpckg );
    
    }

void UT_CMccRedPayloadRead::UT_CMccRedPayloadRead_StreamsL(  )
    {
    EUNIT_ASSERT_EQUALS( iRead->Streams( KUidMediaTypeAudio ), 1 );
    EUNIT_ASSERT_EQUALS( iRead->Streams( KUidMediaTypeVideo ), 0 );
    EUNIT_ASSERT_EQUALS( iRead->Streams( KUidMediaTypeMidi ), 0 );
    EUNIT_ASSERT_EQUALS( iRead->Streams( TUid::Uid( 0x101F7D07 ) ), 0 );
    }

void UT_CMccRedPayloadRead::UT_CMccRedPayloadRead_FrameTimeIntervalL(  )
    {
    EUNIT_ASSERT_EQUALS( iRead->FrameTimeInterval( KUidMediaTypeAudio ).Int64(), 0 );
    EUNIT_ASSERT_EQUALS( iRead->FrameTimeInterval( KUidMediaTypeVideo ).Int64(), 0 );
    }

void UT_CMccRedPayloadRead::UT_CMccRedPayloadRead_DurationL(  )
    {
    EUNIT_ASSERT_EQUALS( iRead->Duration( KUidMediaTypeAudio ).Int64(), 0 );
    EUNIT_ASSERT_EQUALS( iRead->Duration( KUidMediaTypeMidi ).Int64(), 0 );
    }

void UT_CMccRedPayloadRead::UT_CMccRedPayloadRead_SourceDataTypeCodeL(  )
    {
    EUNIT_ASSERT_NO_LEAVE( iRead->SourceDataTypeCode( KUidMediaTypeAudio ) );
    EUNIT_ASSERT_NO_LEAVE( iRead->SourceDataTypeCode( KUidMediaTypeVideo ) );
    }

void UT_CMccRedPayloadRead::UT_CMccRedPayloadRead_SetSourceDataTypeCodeL(  )
    {
    EUNIT_ASSERT_EQUALS( 
        iRead->SetSourceDataTypeCode( iFourCC, KUidMediaTypeAudio ), KErrNone );
    EUNIT_ASSERT_EQUALS( 
        iRead->SetSourceDataTypeCode( iFourCC, KUidMediaTypeVideo ), KErrNotSupported );
    }

void UT_CMccRedPayloadRead::UT_CMccRedPayloadRead_CanCreateSourceBufferL(  )
    {
    EUNIT_ASSERT_EQUALS( iRead->CanCreateSourceBuffer(), ETrue );
    }

void UT_CMccRedPayloadRead::UT_CMccRedPayloadRead_CreateSourceBufferLL(  )
    {
    TBool reference( EFalse );
    
    // No suggestion on buffer size
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
        iRead->CreateSourceBufferL( KUidMediaTypeAudio, reference ), KErrNotReady );
    
    iRead->SuggestSourceBufferSize( 100 );
    
	CMMFBuffer* buffer = iRead->CreateSourceBufferL( KUidMediaTypeAudio, reference );
        
	EUNIT_ASSERT( reference == EFalse );
	delete buffer;
    }

void UT_CMccRedPayloadRead::UT_CMccRedPayloadRead_FillBufferLL(  )
    {
    CMMFDataBuffer* buffer = CMMFDataBuffer::NewL( 10 );
    CleanupStack::PushL( buffer );
    
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
        iRead->FillBufferL( NULL, iDataSinkStub, KUidMediaTypeAudio ), KErrArgument );
    
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
        iRead->FillBufferL( buffer, NULL, KUidMediaTypeAudio ), KErrArgument );
    
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
        iRead->FillBufferL( buffer, iDataSinkStub, KUidMediaTypeVideo ), KErrNotSupported );
    
    EUNIT_ASSERT_NO_LEAVE( iRead->FillBufferL( buffer, iDataSinkStub, KUidMediaTypeAudio ) );
    
    CleanupStack::PopAndDestroy( buffer );
    }

void UT_CMccRedPayloadRead::UT_CMccRedPayloadRead_SourceThreadLogonL(  )
    {
    EUNIT_ASSERT_EQUALS( iRead->SourceThreadLogon( *this ), KErrNone );
    }

void UT_CMccRedPayloadRead::UT_CMccRedPayloadRead_SourceThreadLogoffL(  )
    {
    EUNIT_ASSERT_NO_LEAVE( iRead->SourceThreadLogoff( ) );
    }

void UT_CMccRedPayloadRead::UT_CMccRedPayloadRead_SourcePrimeLL(  )
    {
    EUNIT_ASSERT_NO_LEAVE( iRead->SourcePrimeL() );
    }

void UT_CMccRedPayloadRead::UT_CMccRedPayloadRead_SourcePlayLL(  )
    {
    EUNIT_ASSERT_NO_LEAVE( iRead->SourcePlayL() );
    }

void UT_CMccRedPayloadRead::UT_CMccRedPayloadRead_SourcePauseLL(  )
    {
    EUNIT_ASSERT_NO_LEAVE( iRead->SourcePauseL() );
    }

void UT_CMccRedPayloadRead::UT_CMccRedPayloadRead_SourceStopLL(  )
    {
    iRead->SourcePlayL();
    EUNIT_ASSERT_NO_LEAVE( iRead->SourceStopL() );
    }

void UT_CMccRedPayloadRead::UT_CMccRedPayloadRead_NegotiateSourceLL(  )
    {
    EUNIT_ASSERT_NO_LEAVE( iRead->NegotiateSourceL( *iDataSinkStub ) );
    }

void UT_CMccRedPayloadRead::UT_CMccRedPayloadRead_SinkDataTypeCodeL(  )
    {
    iRead->NegotiateSourceL( *iDataSinkStub ); 
    EUNIT_ASSERT_NO_LEAVE( iRead->SinkDataTypeCode( KUidMediaTypeAudio ) );
    EUNIT_ASSERT_NO_LEAVE( iRead->SinkDataTypeCode( KUidMediaTypeVideo ) );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMccRedPayloadRead,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "SetPayloadTypes - test ",
    "CMccRedPayloadRead",
    "SetPayloadTypes",
    "FUNCTIONALITY",
    SetupL, UT_CMccRedPayloadRead_SetPayloadTypesL, Teardown)

EUNIT_TEST(
    "BufferFilledL - test ",
    "CMccRedPayloadRead",
    "BufferFilledL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccRedPayloadRead_BufferFilledLL, Teardown)

EUNIT_TEST(
    "ConfigurePayloadFormatL - test ",
    "CMccRedPayloadRead",
    "ConfigurePayloadFormatL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccRedPayloadRead_ConfigurePayloadFormatLL, Teardown)

EUNIT_TEST(
    "Streams - test ",
    "CMccRedPayloadRead",
    "Streams",
    "FUNCTIONALITY",
    SetupL, UT_CMccRedPayloadRead_StreamsL, Teardown)

EUNIT_TEST(
    "FrameTimeInterval - test ",
    "CMccRedPayloadRead",
    "FrameTimeInterval",
    "FUNCTIONALITY",
    SetupL, UT_CMccRedPayloadRead_FrameTimeIntervalL, Teardown)

EUNIT_TEST(
    "Duration - test ",
    "CMccRedPayloadRead",
    "Duration",
    "FUNCTIONALITY",
    SetupL, UT_CMccRedPayloadRead_DurationL, Teardown)

EUNIT_TEST(
    "SourceDataTypeCode - test ",
    "CMccRedPayloadRead",
    "SourceDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CMccRedPayloadRead_SourceDataTypeCodeL, Teardown)

EUNIT_TEST(
    "SetSourceDataTypeCode - test ",
    "CMccRedPayloadRead",
    "SetSourceDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CMccRedPayloadRead_SetSourceDataTypeCodeL, Teardown)

EUNIT_TEST(
    "CanCreateSourceBuffer - test ",
    "CMccRedPayloadRead",
    "CanCreateSourceBuffer",
    "FUNCTIONALITY",
    SetupL, UT_CMccRedPayloadRead_CanCreateSourceBufferL, Teardown)

EUNIT_TEST(
    "CreateSourceBufferL - test ",
    "CMccRedPayloadRead",
    "CreateSourceBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRedPayloadRead_CreateSourceBufferLL, Teardown)

EUNIT_TEST(
    "FillBufferL - test ",
    "CMccRedPayloadRead",
    "FillBufferL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccRedPayloadRead_FillBufferLL, Teardown)

EUNIT_TEST(
    "SourceThreadLogon - test ",
    "CMccRedPayloadRead",
    "SourceThreadLogon",
    "FUNCTIONALITY",
    SetupL, UT_CMccRedPayloadRead_SourceThreadLogonL, Teardown)

EUNIT_TEST(
    "SourceThreadLogoff - test ",
    "CMccRedPayloadRead",
    "SourceThreadLogoff",
    "FUNCTIONALITY",
    SetupL, UT_CMccRedPayloadRead_SourceThreadLogoffL, Teardown)

EUNIT_TEST(
    "SourcePrimeL - test ",
    "CMccRedPayloadRead",
    "SourcePrimeL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRedPayloadRead_SourcePrimeLL, Teardown)

EUNIT_TEST(
    "SourcePlayL - test ",
    "CMccRedPayloadRead",
    "SourcePlayL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRedPayloadRead_SourcePlayLL, Teardown)

EUNIT_TEST(
    "SourcePauseL - test ",
    "CMccRedPayloadRead",
    "SourcePauseL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRedPayloadRead_SourcePauseLL, Teardown)

EUNIT_TEST(
    "SourceStopL - test ",
    "CMccRedPayloadRead",
    "SourceStopL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRedPayloadRead_SourceStopLL, Teardown)

EUNIT_TEST(
    "NegotiateSourceL - test ",
    "CMccRedPayloadRead",
    "NegotiateSourceL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccRedPayloadRead_NegotiateSourceLL, Teardown)

EUNIT_TEST(
    "SinkDataTypeCode - test ",
    "CMccRedPayloadRead",
    "SinkDataTypeCode",
    "FUNCTIONALITY",
    Setup2L, UT_CMccRedPayloadRead_SinkDataTypeCodeL, Teardown)

// ALLOC-TESTS 
#ifdef ALLOC_TEST_ON

EUNIT_ALLOC_TEST(
    "SetPayloadTypes - alloc test ",
    "CMccRedPayloadRead",
    "SetPayloadTypes",
    "ERRORHANDLING",
    SetupAL, UT_CMccRedPayloadRead_SetPayloadTypesL, Teardown)

EUNIT_ALLOC_TEST(
    "BufferFilledL - alloc test ",
    "CMccRedPayloadRead",
    "BufferFilledL",
    "ERRORHANDLING",
    Setup2AL, UT_CMccRedPayloadRead_BufferFilledLL, Teardown)

EUNIT_ALLOC_TEST(
    "ConfigurePayloadFormatL - alloc test ",
    "CMccRedPayloadRead",
    "ConfigurePayloadFormatL",
    "ERRORHANDLING",
    SetupAL, UT_CMccRedPayloadRead_ConfigurePayloadFormatLL, Teardown)

EUNIT_ALLOC_TEST(
    "Streams - alloc test ",
    "CMccRedPayloadRead",
    "Streams",
    "ERRORHANDLING",
    SetupAL, UT_CMccRedPayloadRead_StreamsL, Teardown)

EUNIT_ALLOC_TEST(
    "FrameTimeInterval - alloc test ",
    "CMccRedPayloadRead",
    "FrameTimeInterval",
    "ERRORHANDLING",
    SetupAL, UT_CMccRedPayloadRead_FrameTimeIntervalL, Teardown)

EUNIT_ALLOC_TEST(
    "Duration - alloc test ",
    "CMccRedPayloadRead",
    "Duration",
    "ERRORHANDLING",
    SetupAL, UT_CMccRedPayloadRead_DurationL, Teardown)

EUNIT_ALLOC_TEST(
    "SourceDataTypeCode - alloc test ",
    "CMccRedPayloadRead",
    "SourceDataTypeCode",
    "ERRORHANDLING",
    SetupAL, UT_CMccRedPayloadRead_SourceDataTypeCodeL, Teardown)

EUNIT_ALLOC_TEST(
    "SetSourceDataTypeCode - alloc test ",
    "CMccRedPayloadRead",
    "SetSourceDataTypeCode",
    "ERRORHANDLING",
    SetupAL, UT_CMccRedPayloadRead_SetSourceDataTypeCodeL, Teardown)

EUNIT_ALLOC_TEST(
    "CanCreateSourceBuffer - alloc test ",
    "CMccRedPayloadRead",
    "CanCreateSourceBuffer",
    "ERRORHANDLING",
    SetupAL, UT_CMccRedPayloadRead_CanCreateSourceBufferL, Teardown)

EUNIT_ALLOC_TEST(
    "CreateSourceBufferL - alloc test ",
    "CMccRedPayloadRead",
    "CreateSourceBufferL",
    "ERRORHANDLING",
    SetupAL, UT_CMccRedPayloadRead_CreateSourceBufferLL, Teardown)

EUNIT_ALLOC_TEST(
    "FillBufferL - alloc test ",
    "CMccRedPayloadRead",
    "FillBufferL",
    "ERRORHANDLING",
    SetupAL, UT_CMccRedPayloadRead_FillBufferLL, Teardown)

EUNIT_ALLOC_TEST(
    "SourceThreadLogon - alloc test ",
    "CMccRedPayloadRead",
    "SourceThreadLogon",
    "ERRORHANDLING",
    SetupAL, UT_CMccRedPayloadRead_SourceThreadLogonL, Teardown)

EUNIT_ALLOC_TEST(
    "SourceThreadLogoff - alloc test ",
    "CMccRedPayloadRead",
    "SourceThreadLogoff",
    "ERRORHANDLING",
    SetupAL, UT_CMccRedPayloadRead_SourceThreadLogoffL, Teardown)

EUNIT_ALLOC_TEST(
    "SourcePrimeL - alloc test ",
    "CMccRedPayloadRead",
    "SourcePrimeL",
    "ERRORHANDLING",
    SetupAL, UT_CMccRedPayloadRead_SourcePrimeLL, Teardown)

EUNIT_ALLOC_TEST(
    "SourcePlayL - alloc test ",
    "CMccRedPayloadRead",
    "SourcePlayL",
    "ERRORHANDLING",
    SetupAL, UT_CMccRedPayloadRead_SourcePlayLL, Teardown)

EUNIT_ALLOC_TEST(
    "SourcePauseL - alloc test ",
    "CMccRedPayloadRead",
    "SourcePauseL",
    "ERRORHANDLING",
    SetupAL, UT_CMccRedPayloadRead_SourcePauseLL, Teardown)

EUNIT_ALLOC_TEST(
    "SourceStopL - alloc test ",
    "CMccRedPayloadRead",
    "SourceStopL",
    "ERRORHANDLING",
    SetupAL, UT_CMccRedPayloadRead_SourceStopLL, Teardown)

EUNIT_ALLOC_TEST(
    "NegotiateSourceL - alloc test ",
    "CMccRedPayloadRead",
    "NegotiateSourceL",
    "ERRORHANDLING",
    SetupAL, UT_CMccRedPayloadRead_NegotiateSourceLL, Teardown)

EUNIT_ALLOC_TEST(
    "SinkDataTypeCode - alloc test ",
    "CMccRedPayloadRead",
    "SinkDataTypeCode",
    "ERRORHANDLING",
    SetupAL, UT_CMccRedPayloadRead_SinkDataTypeCodeL, Teardown)

#endif

EUNIT_END_TEST_TABLE

//  END OF FILE
