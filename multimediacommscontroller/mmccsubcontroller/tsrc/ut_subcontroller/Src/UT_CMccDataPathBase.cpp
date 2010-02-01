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
#include "UT_CMccDataPathBase.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <mmf/server/mmfvideoframebuffer.h>


//  INTERNAL INCLUDES
#include "mcculdatapath.h"
#include "Mccdatapathbase.h"
#include "MccRtpDatasource.h"
#include "MccRtpDataSink.h"
#include "mccresourcepool_stub.h"
#include "mccuids.hrh"

#include "mccunittestmacros.h"

const TUid KRtpDataSource            =  { KImplUidRtpDataSource   }; 
const TUid KRtpDataSink              =  { KImplUidRtpDataSink     };

// CONSTRUCTION
UT_CMccDataPathBase* UT_CMccDataPathBase::NewL()
    {
    UT_CMccDataPathBase* self = UT_CMccDataPathBase::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccDataPathBase* UT_CMccDataPathBase::NewLC()
    {
    UT_CMccDataPathBase* self = new( ELeave ) UT_CMccDataPathBase();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccDataPathBase::~UT_CMccDataPathBase()
    {
    }

// Default constructor
UT_CMccDataPathBase::UT_CMccDataPathBase()
    {
    }

// Second phase construct
void UT_CMccDataPathBase::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMccDataPathBase::SetupL(  )
    {
    iResources = CMccResourcePoolStub::NewL();
    
  	TMediaId mediaId;
    iUlPath = CMccUlDataPath::NewL(this, iResources, mediaId);
    iSourceStub = static_cast< CMccRtpDataSource* >( MDataSource::NewSourceL( KRtpDataSource, KNullDesC8 ) );
    iSinkStub = static_cast< CMccRtpDataSink* >(MDataSink::NewSinkL( KRtpDataSink, KNullDesC8 ));
    
    }

void UT_CMccDataPathBase::Teardown(  )
    {
    delete iSinkStub;
    delete iSourceStub;
    delete iUlPath;
    delete iResources;
    REComSession::FinalClose();
    }

void UT_CMccDataPathBase::UT_CMccDataPathBase_ChangeDataPathTransferState()
    {
  
    TTransferState state = ENeedSourceData;
    iUlPath->iTransferState = EWaitSource;
    EUNIT_ASSERT(! iUlPath->ChangeDataPathTransferState( state ) );
    
    // Change is allowed if handling failure situation
    EUNIT_ASSERT( iUlPath->ChangeDataPathTransferState( state, ETrue ) );
    
    
    iUlPath->iState = EStopped;
    iUlPath->iTransferState = ESendDataToSink;
    EUNIT_ASSERT( iUlPath->ChangeDataPathTransferState( state ) );

    }
    
void UT_CMccDataPathBase::UT_CMccDataPathBase_ConstructSinkSource()
    {
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iUlPath->CMccDataPathBase::ConstructSinkL(_L8("sink")), KErrNotSupported );    
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iUlPath->CMccDataPathBase::ConstructSourceL(_L8("source")), KErrNotSupported );    
    }

void UT_CMccDataPathBase::UT_CMccDataPathBase_IsBufferSupportedL()
    {
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iUlPath->IsBufferSupportedL( NULL ), KErrArgument ); 
    
    CMMFDescriptorBuffer* buffer = CMMFDescriptorBuffer::NewL(10);
    CleanupStack::PushL( buffer );
    MCC_EUNIT_ASSERT_NO_LEAVE( iUlPath->IsBufferSupportedL( buffer ) );
    CleanupStack::PopAndDestroy( buffer );
    
    CMMFYUVBuffer* yuvBuf = CMMFYUVBuffer::NewL();  
    CleanupStack::PushL( yuvBuf ); 
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iUlPath->IsBufferSupportedL( yuvBuf ), KErrNotSupported ); 
    CleanupStack::PopAndDestroy( yuvBuf );
    
    CMMFDataBuffer* databuf = CMMFDataBuffer::NewL( 10 );         
    CleanupStack::PushL( databuf );
    MCC_EUNIT_ASSERT_NO_LEAVE( iUlPath->IsBufferSupportedL( databuf ) );
    CleanupStack::PopAndDestroy();
    }

void UT_CMccDataPathBase::UT_CMccDataPathBase_CreateCompleteCallBackL()
    {
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iUlPath->CreateCompleteCallBackL(), KErrArgument );
    MCC_EUNIT_ASSERT_NO_LEAVE( iUlPath->CMccDataPathBase::AddDataSinkL(iSinkStub) );
    MCC_EUNIT_ASSERT_NO_LEAVE( iUlPath->CreateCompleteCallBackL() );
    }

void UT_CMccDataPathBase::UT_CMccDataPathBase_CopyBuffer()
    {
    CMMFDataBuffer* desbuf = CMMFDataBuffer::NewL( 10 );         
    CleanupStack::PushL( desbuf );
    
    CMMFDataBuffer* oribuf = CMMFDataBuffer::NewL( 10 );         
    CleanupStack::PushL( oribuf );
    
    iUlPath->CopyBuffer( NULL, NULL );
    iUlPath->CopyBuffer( desbuf, NULL );
    iUlPath->CopyBuffer( NULL, oribuf );
    iUlPath->CopyBuffer( desbuf, oribuf );
    iUlPath->CopyBuffer( desbuf, desbuf );

    CleanupStack::PopAndDestroy( oribuf );

    CleanupStack::PopAndDestroy( desbuf );
    }

void UT_CMccDataPathBase::UT_CMccDataPathBase_AddDataSinkLL(  )
    {
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iUlPath->CMccDataPathBase::AddDataSinkL(NULL), KErrArgument);
    MCC_EUNIT_ASSERT_NO_LEAVE( iUlPath->CMccDataPathBase::AddDataSinkL(iSinkStub) );
    MCC_EUNIT_ASSERT_NO_LEAVE( iUlPath->CMccDataPathBase::AddDataSourceL(iSourceStub) );
    MCC_EUNIT_ASSERT_NO_LEAVE( iUlPath->CMccDataPathBase::AddDataSinkL(iSinkStub) );
    
    iUlPath->iState = EPaused;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iUlPath->CMccDataPathBase::AddDataSinkL(iSinkStub), KErrNotReady );

    }

void UT_CMccDataPathBase::UT_CMccDataPathBase_AddDataSourceLL(  )
    {    
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iUlPath->CMccDataPathBase::AddDataSourceL(NULL), KErrArgument);

    MCC_EUNIT_ASSERT_NO_LEAVE( iUlPath->CMccDataPathBase::AddDataSourceL(iSourceStub) );
    MCC_EUNIT_ASSERT_NO_LEAVE( iUlPath->CMccDataPathBase::AddDataSinkL(iSinkStub) );
    MCC_EUNIT_ASSERT_NO_LEAVE( iUlPath->CMccDataPathBase::AddDataSourceL(iSourceStub) );
    
    iUlPath->iState = EPaused;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iUlPath->CMccDataPathBase::AddDataSourceL(iSourceStub), KErrNotReady );

    }

void UT_CMccDataPathBase::UT_CMccDataPathBase_EndOfDataL(  )
    {
    iUlPath->CMccDataPathBase::AddDataSinkL(iSinkStub);
    iUlPath->CMccDataPathBase::CreateCompleteCallBackL();
    
    iUlPath->CMccDataPathBase::EndOfData();
    }

void UT_CMccDataPathBase::UT_CMccDataPathBase_PrimeLL(  )
    {
    EUNIT_ASSERT( ETrue );
    }

void UT_CMccDataPathBase::UT_CMccDataPathBase_PlayLL(  )
    {
    EUNIT_ASSERT( ETrue );
    }

void UT_CMccDataPathBase::UT_CMccDataPathBase_PauseLL(  )
    {
    EUNIT_ASSERT( ETrue );
    }

void UT_CMccDataPathBase::UT_CMccDataPathBase_StopLL(  )
    {
    EUNIT_ASSERT( ETrue );
    }

void UT_CMccDataPathBase::UT_CMccDataPathBase_ResumeLL(  )
    {
    EUNIT_ASSERT( ETrue );
    }

void UT_CMccDataPathBase::UT_CMccDataPathBase_CreateSinkBufferLL(  )
    {
    TMediaId mediaId;
    TBool bufRef;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(iUlPath->CreateSinkBufferL(mediaId,bufRef) ,KErrNotSupported);
    }

void UT_CMccDataPathBase::UT_CMccDataPathBase_CreateSourceBufferLL(  )
    {
    TMediaId mediaId;
    TBool bufRef;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(iUlPath->CreateSourceBufferL(mediaId,bufRef) ,KErrNotSupported);
    }

void UT_CMccDataPathBase::UT_CMccDataPathBase_CanCreateSinkBufferL(  )
    {
    EUNIT_ASSERT( !iUlPath->CMccDataPathBase::CanCreateSinkBuffer() );
    }

void UT_CMccDataPathBase::UT_CMccDataPathBase_CanCreateSourceBufferL(  )
    {
    EUNIT_ASSERT( !iUlPath->CMccDataPathBase::CanCreateSourceBuffer() );
    }

void UT_CMccDataPathBase::UT_CMccDataPathBase_EmptyBufferLL(  )
    {
    CMMFBuffer* buffer = NULL;
    MDataSource* source = NULL;
    TMediaId mediaId;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(
    	iUlPath->CMccDataPathBase::EmptyBufferL(buffer,source,mediaId), 
    	KErrNotSupported);
    }

void UT_CMccDataPathBase::UT_CMccDataPathBase_FillBufferLL(  )
    {
    CMMFBuffer* buffer = NULL;
    MDataSink* sink = NULL;
    TMediaId mediaId;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(
    	iUlPath->CMccDataPathBase::FillBufferL(buffer,sink,mediaId), 
    	KErrNotSupported);
    }

void UT_CMccDataPathBase::UT_CMccDataPathBase_SinkDataTypeCodeL(  )
    {
    TMediaId mediaId;
    
    // Supported media types
    mediaId.iMediaType = KUidMediaTypeVideo;
    const KTestFourCCVal1 = 0x100000;
    iUlPath->iSinkFourCC = TFourCC( KTestFourCCVal1 );
    EUNIT_ASSERT( iUlPath->SinkDataTypeCode( mediaId ) == TFourCC( KTestFourCCVal1 ) );
    const KTestFourCCVal2 = 0x200000;
    iUlPath->iSinkFourCC = TFourCC( KTestFourCCVal2 );
    mediaId.iMediaType = KUidMediaTypeAudio;
    EUNIT_ASSERT( iUlPath->SinkDataTypeCode( mediaId ) == TFourCC( KTestFourCCVal2 ) );
    
    // Unsupported media type
    mediaId.iMediaType = KUidMediaTypeMidi;
    EUNIT_ASSERT( iUlPath->SinkDataTypeCode( mediaId ) == TFourCC( KFourCCNULL ) );
    }

void UT_CMccDataPathBase::UT_CMccDataPathBase_SourceDataTypeCodeL(  )
    {
    TMediaId mediaId;
    
    // Supported media types
    mediaId.iMediaType = KUidMediaTypeVideo;
    const KTestFourCCVal1 = 0x100000;
    iUlPath->iSourceFourCC = TFourCC( KTestFourCCVal1 );
    EUNIT_ASSERT( iUlPath->SourceDataTypeCode( mediaId ) == TFourCC( KTestFourCCVal1 ) );
    const KTestFourCCVal2 = 0x200000;
    iUlPath->iSourceFourCC = TFourCC( KTestFourCCVal2 );
    mediaId.iMediaType = KUidMediaTypeAudio;
    EUNIT_ASSERT( iUlPath->SourceDataTypeCode( mediaId ) == TFourCC( KTestFourCCVal2 ) );
    
    // Unsupported media type
    mediaId.iMediaType = KUidMediaTypeMidi;
    EUNIT_ASSERT( iUlPath->SourceDataTypeCode( mediaId ) == TFourCC( KFourCCNULL ) );
    
    }

void UT_CMccDataPathBase::UT_CMccDataPathBase_SetSinkDataTypeCodeL(  )
    {
    TMediaId mediaId;
    mediaId.iMediaType = KRtpDataSource;
    EUNIT_ASSERT_EQUALS(
    	iUlPath->CMccDataPathBase::SetSinkDataTypeCode(TFourCC(),mediaId), 
    	KErrNotSupported);
    iUlPath->iMediaId.iMediaType = KRtpDataSource;
    EUNIT_ASSERT_EQUALS(
    	iUlPath->CMccDataPathBase::SetSinkDataTypeCode(TFourCC(),mediaId), 
    	KErrNone);
    }

void UT_CMccDataPathBase::UT_CMccDataPathBase_SetSourceDataTypeCodeL(  )
    {
    TMediaId mediaId;
    mediaId.iMediaType = KRtpDataSource;
    EUNIT_ASSERT_EQUALS(
    	iUlPath->CMccDataPathBase::SetSourceDataTypeCode(TFourCC(),mediaId), 
    	KErrNotSupported);
    iUlPath->iMediaId.iMediaType = KRtpDataSource;
    EUNIT_ASSERT_EQUALS(
    	iUlPath->CMccDataPathBase::SetSourceDataTypeCode(TFourCC(),mediaId), 
    	KErrNone);
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccDataPathBase,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "ChangeDataPathTransferState - test ",
    "CMccDataPathBase",
    "ChangeDataPathTransferState",
    "FUNCTIONALITY",
    SetupL, UT_CMccDataPathBase_ChangeDataPathTransferState, Teardown)

EUNIT_TEST(
    "ConstructSinkSource - test ",
    "CMccDataPathBase",
    "ConstructSinkSource",
    "FUNCTIONALITY",
    SetupL, UT_CMccDataPathBase_ConstructSinkSource, Teardown)

EUNIT_TEST(
    "IsBufferSupportedL - test ",
    "CMccDataPathBase",
    "IsBufferSupportedL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDataPathBase_IsBufferSupportedL, Teardown)

EUNIT_TEST(
    "CreateCompleteCallBackL - test ",
    "CMccDataPathBase",
    "CreateCompleteCallBackL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDataPathBase_CreateCompleteCallBackL, Teardown)



EUNIT_TEST(
    "CopyBuffer - test ",
    "CMccDataPathBase",
    "CopyBuffer",
    "FUNCTIONALITY",
    SetupL, UT_CMccDataPathBase_CopyBuffer, Teardown)

EUNIT_TEST(
    "AddDataSinkL - test ",
    "CMccDataPathBase",
    "AddDataSinkL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDataPathBase_AddDataSinkLL, Teardown)

EUNIT_TEST(
    "AddDataSourceL - test ",
    "CMccDataPathBase",
    "AddDataSourceL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDataPathBase_AddDataSourceLL, Teardown)

EUNIT_TEST(
    "EndOfData - test ",
    "CMccDataPathBase",
    "EndOfData",
    "FUNCTIONALITY",
    SetupL, UT_CMccDataPathBase_EndOfDataL, Teardown)

EUNIT_TEST(
    "PrimeL - test ",
    "CMccDataPathBase",
    "PrimeL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDataPathBase_PrimeLL, Teardown)

EUNIT_TEST(
    "PlayL - test ",
    "CMccDataPathBase",
    "PlayL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDataPathBase_PlayLL, Teardown)

EUNIT_TEST(
    "PauseL - test ",
    "CMccDataPathBase",
    "PauseL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDataPathBase_PauseLL, Teardown)

EUNIT_TEST(
    "StopL - test ",
    "CMccDataPathBase",
    "StopL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDataPathBase_StopLL, Teardown)

EUNIT_TEST(
    "ResumeL - test ",
    "CMccDataPathBase",
    "ResumeL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDataPathBase_ResumeLL, Teardown)

EUNIT_TEST(
    "CreateSinkBufferL - test ",
    "CMccDataPathBase",
    "CreateSinkBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDataPathBase_CreateSinkBufferLL, Teardown)

EUNIT_TEST(
    "CreateSourceBufferL - test ",
    "CMccDataPathBase",
    "CreateSourceBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDataPathBase_CreateSourceBufferLL, Teardown)

EUNIT_TEST(
    "CanCreateSinkBuffer - test ",
    "CMccDataPathBase",
    "CanCreateSinkBuffer",
    "FUNCTIONALITY",
    SetupL, UT_CMccDataPathBase_CanCreateSinkBufferL, Teardown)

EUNIT_TEST(
    "CanCreateSourceBuffer - test ",
    "CMccDataPathBase",
    "CanCreateSourceBuffer",
    "FUNCTIONALITY",
    SetupL, UT_CMccDataPathBase_CanCreateSourceBufferL, Teardown)

EUNIT_TEST(
    "EmptyBufferL - test ",
    "CMccDataPathBase",
    "EmptyBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDataPathBase_EmptyBufferLL, Teardown)

EUNIT_TEST(
    "FillBufferL - test ",
    "CMccDataPathBase",
    "FillBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDataPathBase_FillBufferLL, Teardown)

EUNIT_TEST(
    "SinkDataTypeCode - test ",
    "CMccDataPathBase",
    "SinkDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CMccDataPathBase_SinkDataTypeCodeL, Teardown)

EUNIT_TEST(
    "SourceDataTypeCode - test ",
    "CMccDataPathBase",
    "SourceDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CMccDataPathBase_SourceDataTypeCodeL, Teardown)

EUNIT_TEST(
    "SetSinkDataTypeCode - test ",
    "CMccDataPathBase",
    "SetSinkDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CMccDataPathBase_SetSinkDataTypeCodeL, Teardown)

EUNIT_TEST(
    "SetSourceDataTypeCode - test ",
    "CMccDataPathBase",
    "SetSourceDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CMccDataPathBase_SetSourceDataTypeCodeL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
