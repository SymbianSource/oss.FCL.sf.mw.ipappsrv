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
#include "UT_CMccUlDataPath.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "mcculdatapath.h"
#include "MccRtpDatasource.h"
#include "MccRtpDataSink.h"
#include "mccresourcepool_stub.h"
#include "mccuids.hrh"
#include "mccinternaldef.h"
#include "AmrPayloadFormatWrite.h"

#include "mccunittestmacros.h"


const TUid KRtpDataSource            =  { KImplUidRtpDataSource   }; 
const TUid KRtpDataSink              =  { KImplUidRtpDataSink     };


// CONSTRUCTION
UT_CMccUlDataPath* UT_CMccUlDataPath::NewL()
    {
    UT_CMccUlDataPath* self = UT_CMccUlDataPath::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccUlDataPath* UT_CMccUlDataPath::NewLC()
    {
    UT_CMccUlDataPath* self = new( ELeave ) UT_CMccUlDataPath();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccUlDataPath::~UT_CMccUlDataPath()
    {
    }

// Default constructor
UT_CMccUlDataPath::UT_CMccUlDataPath()
    {
    }

// Second phase construct
void UT_CMccUlDataPath::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMccUlDataPath::SetupL(  )
    {
    iResources = CMccResourcePoolStub::NewL();
    
    TMediaId mediaId;
    iSourceStub = static_cast< CMccRtpDataSource* >( MDataSource::NewSourceL( KRtpDataSource, KNullDesC8 ) );
    iSinkStub = static_cast< CMccRtpDataSink* >(MDataSink::NewSinkL( KRtpDataSink, KNullDesC8 ));
    iWrite = CAmrPayloadFormatWrite::NewL( iSinkStub );

    iUlPath = CMccUlDataPath::NewL(this, iResources, mediaId);
    }

void UT_CMccUlDataPath::Teardown(  )
    {
    delete iUlPath;
    delete iWrite;
    delete iSinkStub;
    delete iSourceStub;
    delete iResources;
    REComSession::FinalClose();
    }

void UT_CMccUlDataPath::UT_CMccUlDataPath_NegotiateLL()
    {
    MDataSink* sink = NULL;
    // No source added, not ready
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iUlPath->NegotiateL( *sink ), KErrNotReady );
    
    iUlPath->AddDataSourceL( iSourceStub );
    // Source added, NULL ptr, argument
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iUlPath->NegotiateL( *sink ), KErrArgument );
    
    // Actual sink, no leave
    iUlPath->iState = EStopped;
    iUlPath->NegotiateL( *iSinkStub );
    }

void UT_CMccUlDataPath::UT_CMccUlDataPath_BufferEmptiedLL(  )
    {
    CMMFBuffer* buffer = NULL;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(iUlPath->BufferEmptiedL(buffer), KErrArgument);
    CMMFDescriptorBuffer* dataBuffer = CMMFDescriptorBuffer::NewL(10);
    CleanupStack::PushL(dataBuffer);
    dataBuffer->Data().Copy(_L("test"));
    iUlPath->iState = EPrimed;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(iUlPath->BufferEmptiedL(static_cast<CMMFBuffer*>(dataBuffer)), KErrNotReady);
    iUlPath->iState = EStreaming;
    iUlPath->BufferEmptiedL(static_cast<CMMFBuffer*>(dataBuffer));
    CleanupStack::PopAndDestroy(dataBuffer);
    }

void UT_CMccUlDataPath::UT_CMccUlDataPath_BufferFilledLL(  )
    {
    CMMFBuffer* buffer = NULL;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(iUlPath->BufferFilledL(buffer), KErrNotReady);
    iUlPath->iState = EStreaming;
    iUlPath->BufferFilledL(buffer);
    CMMFDescriptorBuffer* dataBuffer = CMMFDescriptorBuffer::NewL(10);
    CleanupStack::PushL(dataBuffer);
    CMMFBuffer* sinkBuffer = CMMFDescriptorBuffer::NewL(10);
    CleanupStack::PushL(sinkBuffer);
    iUlPath->iSinkBuffer = sinkBuffer;
    iUlPath->BufferFilledL(static_cast<CMMFBuffer*>(dataBuffer));
    dataBuffer->Data().Copy(_L("test"));
    iUlPath->BufferFilledL(static_cast<CMMFBuffer*>(dataBuffer));
    CleanupStack::PopAndDestroy(sinkBuffer);
    CleanupStack::PopAndDestroy(dataBuffer);
    }

void UT_CMccUlDataPath::UT_CMccUlDataPath_PrimeLL(  )
    {
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iUlPath->PrimeL( 0 ), KErrNotReady ); // state is not ready   
    
    iUlPath->iState = EStopped;
    iUlPath->AddDataSinkL( iWrite );
    iUlPath->AddDataSourceL( iSourceStub );
    
    iUlPath->iState = EPrimed;
    iUlPath->iMediaId.iMediaType = KUidMediaTypeAudio;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iUlPath->PrimeL( 0 ), KErrArgument ); 
    	
    iUlPath->iState = EStopped;
    MCC_EUNIT_ASSERT_NO_LEAVE( iUlPath->PrimeL( 0 ) );
    
    iUlPath->iState = EStopped;	
    MCC_EUNIT_ASSERT_NO_LEAVE( iUlPath->PrimeL( MCC_ENDPOINT_ID( iSourceStub ) ) );
    
    iUlPath->iDataSink = NULL;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iUlPath->PrimeL( 0 ), KErrGeneral );

    iUlPath->iDataSource = NULL;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iUlPath->PrimeL( 0 ), KErrGeneral );

    }

void UT_CMccUlDataPath::UT_CMccUlDataPath_PlayLL(  )
    {
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iUlPath->PlayL( 0 ), KErrNotReady ); // state is not ready  
  
    iUlPath->iState = EStopped;
    iUlPath->AddDataSinkL( iSinkStub );
    iUlPath->AddDataSourceL( iSourceStub );
    
    iUlPath->iState = EStreaming;  
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iUlPath->PlayL( 0 ), KErrGeneral );
    
    iUlPath->iState = EPrimed;
    CMMFBuffer* buffer = CMMFDescriptorBuffer::NewL(10);
    CleanupStack::PushL(buffer);
    iUlPath->iSinkBuffer = buffer;
    MCC_EUNIT_ASSERT_NO_LEAVE( iUlPath->PlayL( 0 ) );
    MCC_EUNIT_ASSERT_NO_LEAVE( iUlPath->PlayL( 3 ) );

    iUlPath->iState = EStreaming;  
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iUlPath->PlayL( 0 ), KErrArgument );
    
    iUlPath->iSinkBuffer = NULL;
    CleanupStack::PopAndDestroy(buffer);
    
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iUlPath->PlayL( 0 ), KErrGeneral );

    iUlPath->iDataSink = NULL;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iUlPath->PlayL( 0 ), KErrGeneral );

    iUlPath->iDataSource = NULL;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iUlPath->PlayL( 0 ), KErrGeneral );

    }

void UT_CMccUlDataPath::UT_CMccUlDataPath_PauseLL(  )
    {
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iUlPath->PauseL( 0 ), KErrNotReady ); // state is not ready 
    	
    iUlPath->iState = EStopped;
    iUlPath->AddDataSinkL( iSinkStub );
    iUlPath->AddDataSourceL( iSourceStub );
    
    iUlPath->iState = EPrimed;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iUlPath->PauseL( 0 ), KErrArgument );

    iUlPath->iState = EStreaming;
    MCC_EUNIT_ASSERT_NO_LEAVE(iUlPath->PauseL( 0 ));
    
    iUlPath->iDataSink = NULL;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iUlPath->PauseL( 0 ),  KErrGeneral );

    iUlPath->iDataSource = NULL;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iUlPath->PauseL( 0 ),  KErrGeneral );

    }

void UT_CMccUlDataPath::UT_CMccUlDataPath_StopLL(  )
    {
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(iUlPath->StopL( 0 ), KErrNotReady ); // state is not ready 
    	
    iUlPath->iState = EStopped;
    iUlPath->AddDataSinkL( iWrite );
    iUlPath->AddDataSourceL( iSourceStub );
    
    iUlPath->iState = EStopped;
    MCC_EUNIT_ASSERT_NO_LEAVE( iUlPath->StopL( 0 ) );
    
    iUlPath->iState = EStreaming;
    MCC_EUNIT_ASSERT_NO_LEAVE( iUlPath->StopL( 0 ) );
    
    MCC_EUNIT_ASSERT_NO_LEAVE( iUlPath->StopL( 3 ) );
    
    iUlPath->iDataSink = NULL;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iUlPath->StopL( 3 ),  KErrGeneral );

    iUlPath->iDataSource = NULL;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iUlPath->StopL( 3 ),  KErrGeneral );

    }

void UT_CMccUlDataPath::UT_CMccUlDataPath_ResumeLL(  )
    {
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(iUlPath->ResumeL( 0 ), KErrNotReady ); // state is not ready 
    	
    iUlPath->iState = EStopped;
    iUlPath->AddDataSinkL( iWrite );
    iUlPath->AddDataSourceL( iSourceStub );
    
    iUlPath->iState = EStreaming;
    // media type not KUidMediaTypeAudio
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iUlPath->ResumeL( 0 ), KErrArgument );

    iUlPath->iState = EPaused;

    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iUlPath->ResumeL( 0 ), KErrNotSupported );
    
    iUlPath->iState = EPaused;	    
    iUlPath->iMediaId.iMediaType = KUidMediaTypeAudio;

    MCC_EUNIT_ASSERT_NO_LEAVE( iUlPath->ResumeL( MCC_ENDPOINT_ID( iSourceStub ) ) );
    
    MCC_EUNIT_ASSERT_NO_LEAVE( iUlPath->ResumeL( 0 ) );

    iUlPath->iDataSink = NULL;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iUlPath->ResumeL( 0 ), KErrGeneral );

    iUlPath->iDataSource = NULL;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iUlPath->ResumeL( 0 ), KErrGeneral );
    }

void UT_CMccUlDataPath::UT_CMccUlDataPath_RunLL()
    {
    iUlPath->iTransferState = EWaitSink;
    iUlPath->iState = EStopped;
    iUlPath->RunL();
    
    iUlPath->iState = EPrimed;
    iUlPath->RunL();
    
    iUlPath->iState = EPaused;
    iUlPath->RunL();
    
    iUlPath->iState = EStreaming;
    iUlPath->iTransferState = EWaitSink;
    iUlPath->RunL();
    iUlPath->iTransferState = EWaitSource;
    iUlPath->RunL();
    iUlPath->iTransferState = ENeedSourceData;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iUlPath->RunL(), KErrNotReady );
    iUlPath->iTransferState = ESendDataToSink;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iUlPath->RunL(), KErrNotReady );
    
    iUlPath->iState = EStopped;
    iUlPath->AddDataSinkL( iWrite );
    iUlPath->AddDataSourceL( iSourceStub );
    iUlPath->iState = EStreaming;
    iUlPath->iTransferState = EEndOfData;
    iUlPath->RunL();
    iUlPath->iTransferState = static_cast<TTransferState>( 1000 );
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iUlPath->RunL(), KErrArgument );
    
    iUlPath->iState = static_cast<TDataPathState>( 1000 );
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iUlPath->RunL(), KErrArgument );
    }
  
void UT_CMccUlDataPath::UT_CMccUlDataPath_EmptySinkBuffer()
    {
    iUlPath->iDataSink = iWrite;
    CMMFDataBuffer* buf = CMMFDataBuffer::NewL(10);
    CleanupStack::PushL( buf );
    iUlPath->iSinkBuffer = buf;
    MCC_EUNIT_ASSERT_NO_LEAVE( iUlPath->EmptySinkBufferL() );
    CleanupStack::PopAndDestroy( buf );
    
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccUlDataPath,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "NegotiateL - test ",
    "CMccUlDataPath",
    "NegotiateL",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDataPath_NegotiateLL, Teardown)

EUNIT_TEST(
    "BufferEmptiedL - test ",
    "CMccUlDataPath",
    "BufferEmptiedL",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDataPath_BufferEmptiedLL, Teardown)

EUNIT_TEST(
    "BufferFilledL - test ",
    "CMccUlDataPath",
    "BufferFilledL",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDataPath_BufferFilledLL, Teardown)

EUNIT_TEST(
    "PrimeL - test ",
    "CMccUlDataPath",
    "PrimeL",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDataPath_PrimeLL, Teardown)

EUNIT_TEST(
    "PlayL - test ",
    "CMccUlDataPath",
    "PlayL",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDataPath_PlayLL, Teardown)

EUNIT_TEST(
    "PauseL - test ",
    "CMccUlDataPath",
    "PauseL",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDataPath_PauseLL, Teardown)

EUNIT_TEST(
    "StopL - test ",
    "CMccUlDataPath",
    "StopL",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDataPath_StopLL, Teardown)

EUNIT_TEST(
    "ResumeL - test ",
    "CMccUlDataPath",
    "ResumeL",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDataPath_ResumeLL, Teardown)

EUNIT_TEST(
    "RunL - test ",
    "CMccUlDataPath",
    "RunL",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDataPath_RunLL, Teardown)
 
EUNIT_TEST(
    "EmptySinkBuffer - test ",
    "CMccUlDataPath",
    "EmptySinkBuffer",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDataPath_EmptySinkBuffer, Teardown)
 
   
EUNIT_END_TEST_TABLE

//  END OF FILE
