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
#include "UT_CMccDlDataPath.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "mccdldatapath.h"
#include "MccRtpDatasource.h"
#include "MccRtpDataSink.h"
#include "mccresourcepool_stub.h"
#include "mccuids.hrh"
#include "mccinternaldef.h"
#include "AmrPayloadFormatRead.h"

#include "mccunittestmacros.h"


const TUid KRtpDataSource            =  { KImplUidRtpDataSource   }; 
const TUid KRtpDataSink              =  { KImplUidRtpDataSink     };


// CONSTRUCTION
UT_CMccDlDataPath* UT_CMccDlDataPath::NewL()
    {
    UT_CMccDlDataPath* self = UT_CMccDlDataPath::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccDlDataPath* UT_CMccDlDataPath::NewLC()
    {
    UT_CMccDlDataPath* self = new( ELeave ) UT_CMccDlDataPath();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccDlDataPath::~UT_CMccDlDataPath()
    {
    }

// Default constructor
UT_CMccDlDataPath::UT_CMccDlDataPath()
    {
    }

// Second phase construct
void UT_CMccDlDataPath::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMccDlDataPath::SetupL(  )
    {
    iResources = CMccResourcePoolStub::NewL();
    
    TMediaId mediaId;
    iSourceStub = static_cast< CMccRtpDataSource* >( MDataSource::NewSourceL( KRtpDataSource, KNullDesC8 ) );
    
    iRead = CAmrPayloadFormatRead::NewL( iSourceStub );

    iSinkStub = static_cast< CMccRtpDataSink* >(MDataSink::NewSinkL( KRtpDataSink, KNullDesC8 ));

    iDlPath = CMccDlDataPath::NewL(this, iResources, mediaId);
 
    }

void UT_CMccDlDataPath::Teardown(  )
    {
    delete iDlPath;
    delete iSinkStub;
    delete iRead;
    delete iSourceStub;
    delete iResources;
    REComSession::FinalClose();
    }

void UT_CMccDlDataPath::UT_CMccDlDataPath_NegotiateLL(  )
    {
    MDataSource* dummy( NULL );
    
    // No sink added, leave with KErrNotReady.
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iDlPath->NegotiateL( *iSourceStub ), KErrNotReady );
    
    iDlPath->iState = EStopped;
    iDlPath->AddDataSinkL( iSinkStub );
    
    // Sink added, should leave with KErrArgument (NULL ptr).
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iDlPath->NegotiateL( *dummy ), KErrArgument );
    
    // Sink added, actual source. No leave.
    iDlPath->NegotiateL( *iSourceStub );
    }

void UT_CMccDlDataPath::UT_CMccDlDataPath_BufferFilledLL(  )
    {
    CMMFDescriptorBuffer* buffer = NULL;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(
    	iDlPath->BufferFilledL(buffer), 
    	KErrNotReady ); // state is no EPlaying
    buffer = CMMFDescriptorBuffer::NewL(10);
    CleanupStack::PushL(buffer);
    iDlPath->iState = EStreaming;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(iDlPath->BufferFilledL(buffer), KErrArgument);
    buffer->Data().Copy(_L("test"));
    MCC_EUNIT_ASSERT_NO_LEAVE(iDlPath->BufferFilledL(buffer));

    CMMFDescriptorBuffer* buffer2 = CMMFDescriptorBuffer::NewL(10);
    CleanupStack::PushL(buffer2);
    iDlPath->iSourceBuffer = buffer;
    MCC_EUNIT_ASSERT_NO_LEAVE(iDlPath->BufferFilledL(buffer2));
    CleanupStack::PopAndDestroy(buffer2);
    CleanupStack::PopAndDestroy(buffer);
    }

void UT_CMccDlDataPath::UT_CMccDlDataPath_BufferEmptiedLL(  )
    {
    CMMFBuffer* buffer = CMMFDescriptorBuffer::NewL(10);
    CleanupStack::PushL(buffer);
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(
    	iDlPath->BufferEmptiedL(buffer), 
    	KErrNotReady ); // state is no EPlaying
    
    iDlPath->iState = EStreaming;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(
    	iDlPath->BufferEmptiedL(buffer), 
    	KErrArgument );
    	
    iDlPath->iSourceBuffer = buffer;
    iDlPath->BufferEmptiedL(buffer);
    iDlPath->iSourceBuffer = NULL;
    
    CleanupStack::PopAndDestroy(buffer);
    }

void UT_CMccDlDataPath::UT_CMccDlDataPath_PrimeLL(  )
    {
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iDlPath->PrimeL( 0 ),	KErrNotReady ); // state is not ready   
    
    iDlPath->iState = EStopped;
    iDlPath->AddDataSinkL( iSinkStub );
    iDlPath->AddDataSourceL( iRead );
    
    // sourcebuffer is not created
    MCC_EUNIT_ASSERT_NO_LEAVE( iDlPath->PrimeL( 0 ) ); 
 
    // call ConfigurePayloadFormatL will create source buffer 
    TMccCodecInfo codec;
    codec.iFourCC.Set(TFourCC(' ','A','M','R') );
    codec.iBitrate = 12200;
    codec.iCodecMode = KAmrCodecModeOctetAlign;
    codec.iPtime = 20;
    codec.iMaxPtime = 200;
    codec.iPayloadType = 96;
    codec.iRedundancyCount = 0;
    codec.iEnableDTX = EFalse;
    codec.iRedundantPayload = 0;
    codec.iHwFrameTime = 20;
    codec.iAlgoUsed = 0;
    codec.iPayloadFormatEncoder = KImplUidAmrPayloadFormatEncode;
    codec.iPayloadFormatDecoder = KImplUidAmrPayloadFormatDecode;
    
    TMccCodecInfoBuffer bufferi( codec );
    iRead->ConfigurePayloadFormatL( bufferi ); 
    
    iDlPath->iState = EPrimed;   
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iDlPath->PrimeL( 0 ), KErrArgument ); 
    
    iDlPath->iState = EStopped;	
    MCC_EUNIT_ASSERT_NO_LEAVE( iDlPath->PrimeL( 0 ) );
    
    iDlPath->iState = EStopped;	
    MCC_EUNIT_ASSERT_NO_LEAVE( iDlPath->PrimeL( MCC_ENDPOINT_ID( iSinkStub ) ) );
    }

void UT_CMccDlDataPath::UT_CMccDlDataPath_PlayLL(  )
    {
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iDlPath->PlayL( 0 ), KErrNotReady ); // state is not ready  
  
    iDlPath->iState = EStopped;
    iDlPath->AddDataSinkL( iSinkStub );
    iDlPath->AddDataSourceL( iSourceStub );
    
    iDlPath->iState = EStreaming;  
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iDlPath->PlayL( 0 ), KErrGeneral );

    iDlPath->iState = EPrimed;
    CMMFBuffer* buffer = CMMFDescriptorBuffer::NewL(10);
    CleanupStack::PushL(buffer);
    iDlPath->iSourceBuffer = buffer;
    MCC_EUNIT_ASSERT_NO_LEAVE( iDlPath->PlayL( 0 ) );
    
    iDlPath->iState = EStreaming;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iDlPath->PlayL( 0 ), KErrArgument );
    iDlPath->iSourceBuffer = NULL;
    CleanupStack::PopAndDestroy(buffer);
    }

void UT_CMccDlDataPath::UT_CMccDlDataPath_PauseLL(  )
    {  
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iDlPath->PauseL( 0 ), KErrNotReady ); // state is not ready 
    
    iDlPath->iState = EStopped;
    iDlPath->AddDataSinkL( iSinkStub );
    iDlPath->AddDataSourceL( iSourceStub );
    	
    iDlPath->iState = EPrimed;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iDlPath->PauseL( 0 ), KErrArgument );

    iDlPath->iState = EStreaming;
    MCC_EUNIT_ASSERT_NO_LEAVE(iDlPath->PauseL( 0 ));
    }

void UT_CMccDlDataPath::UT_CMccDlDataPath_StopLL(  )
    {
    iDlPath->iState = EStopped;
    iDlPath->AddDataSinkL( iSinkStub );
    iDlPath->AddDataSourceL( iSourceStub );

    iDlPath->iState = EStreaming;
    MCC_EUNIT_ASSERT_NO_LEAVE( iDlPath->StopL( 0 ) );
    }

void UT_CMccDlDataPath::UT_CMccDlDataPath_ResumeLL(  )
    {   
    iDlPath->iDataPathCreated = EFalse;   
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iDlPath->ResumeL( 0 ), KErrNotReady ); // state is not ready 
    	
    iDlPath->iState = EStopped;
    iDlPath->AddDataSinkL( iSinkStub );
    iDlPath->AddDataSourceL( iRead );

    iDlPath->iState = EPaused;
    MCC_EUNIT_ASSERT_NO_LEAVE( iDlPath->ResumeL( 0 ) );
    
    // call ConfigurePayloadFormatL will create source buffer 
    TMccCodecInfo codec;
    codec.iFourCC.Set(TFourCC(' ','A','M','R') );
    codec.iBitrate = 12200;
    codec.iCodecMode = KAmrCodecModeOctetAlign;
    codec.iPtime = 20;
    codec.iMaxPtime = 200;
    codec.iPayloadType = 96;
    codec.iRedundancyCount = 0;
    codec.iEnableDTX = EFalse;
    codec.iRedundantPayload = 0;
    codec.iHwFrameTime = 20;
    codec.iAlgoUsed = 0;
    codec.iPayloadFormatEncoder = KImplUidAmrPayloadFormatEncode;
    codec.iPayloadFormatDecoder = KImplUidAmrPayloadFormatDecode;
    
    TMccCodecInfoBuffer bufferi( codec );
    iRead->ConfigurePayloadFormatL( bufferi ); 
    
    iDlPath->iState = EStreaming;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iDlPath->ResumeL( 0 ), KErrArgument );
    
    iDlPath->iState = EPaused;	
    MCC_EUNIT_ASSERT_NO_LEAVE( iDlPath->ResumeL( MCC_ENDPOINT_ID( iSinkStub ) ) );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccDlDataPath,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "NegotiateL - test ",
    "CMccDlDataPath",
    "NegotiateL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDlDataPath_NegotiateLL, Teardown)

EUNIT_TEST(
    "BufferFilledL - test ",
    "CMccDlDataPath",
    "BufferFilledL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDlDataPath_BufferFilledLL, Teardown)

EUNIT_TEST(
    "BufferEmptiedL - test ",
    "CMccDlDataPath",
    "BufferEmptiedL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDlDataPath_BufferEmptiedLL, Teardown)

EUNIT_TEST(
    "PrimeL - test ",
    "CMccDlDataPath",
    "PrimeL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDlDataPath_PrimeLL, Teardown)

EUNIT_TEST(
    "PlayL - test ",
    "CMccDlDataPath",
    "PlayL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDlDataPath_PlayLL, Teardown)

EUNIT_TEST(
    "PauseL - test ",
    "CMccDlDataPath",
    "PauseL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDlDataPath_PauseLL, Teardown)

EUNIT_TEST(
    "StopL - test ",
    "CMccDlDataPath",
    "StopL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDlDataPath_StopLL, Teardown)

EUNIT_TEST(
    "ResumeL - test ",
    "CMccDlDataPath",
    "ResumeL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDlDataPath_ResumeLL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
