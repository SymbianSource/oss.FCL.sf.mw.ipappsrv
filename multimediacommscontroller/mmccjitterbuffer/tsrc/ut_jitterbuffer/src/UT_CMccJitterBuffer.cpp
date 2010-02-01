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




//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>

//  CLASS HEADER
#include "UT_CMccJitterBuffer.h"

//  INTERNAL INCLUDES
#include "mccjitterBuffer.h"
#include "mccjitterbufferimpl.h"
#include "mccuids.hrh"
#include "mccdatasink_stub.h"
#include "mccdatasource_stub.h"
#include <digia/eunit/CEunitAllocTestCaseDecorator.h>   // for using EUNIT_ALLOC_TEST

//Constants
const TFourCC t4cc = KMccFourCCIdAMRNB;
const TUid KRtpDataSource = { KImplUidRtpDataSource }; 
const TUid KRtpDataSink   = { KImplUidRtpDataSink   };


// CONSTRUCTION
UT_CMccJitterBuffer* UT_CMccJitterBuffer::NewL()
    {
    UT_CMccJitterBuffer* self = UT_CMccJitterBuffer::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMccJitterBuffer* UT_CMccJitterBuffer::NewLC()
    {
    UT_CMccJitterBuffer* self = new( ELeave ) UT_CMccJitterBuffer();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccJitterBuffer::~UT_CMccJitterBuffer()
    {
    }

// Default constructor
UT_CMccJitterBuffer::UT_CMccJitterBuffer() :
	iDataSource( NULL ),
	iDataSink( NULL ),
	iBuffer( NULL ),
	iBuffer2( NULL )
    {
    }

// Second phase construct
void UT_CMccJitterBuffer::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    
    i4ccjit.Set( TFourCC (' ','J','I','T') );
    }

//  METHODS

void UT_CMccJitterBuffer::SetupL()
    {
    iAlloc = EFalse;
   
    iJitter = CMccJitterBuffer::NewL();
    
    iDataSink = new( ELeave )CMccDataSinkStub();
    iDataSource  = new( ELeave )CMccDataSourceStub( KRtpDataSource );
    
    iBuffer2  = CMMFDescriptorBuffer::NewL( 10 );
    
    iCInfo.iFourCC = KMccFourCCIdAMRNB;
    iCInfo.iAlgoUsed = ENoAlgoUsed; 
    iCInfo.iBitrate = 12200;
    iCInfo.iCNFrameSize = 12;
    iCInfo.iCodecMode = KAmrCodecModeOctetAlign;
    iCInfo.iEnableDTX = EFalse;
    iCInfo.iFrameSize = 20;
    iCInfo.iHwFrameTime = 20;
    iCInfo.iPtime = 20;
    iCInfo.iMaxPtime = 200;
    iCInfo.iRedundancyCount = 0;
    iCInfo.iRedundantPayload = 0;
    iCInfo.iJitterBufBufferLength = 200;
    iCInfo.iJitterBufThreshold = 5;
    
	iJitter->AddDataSinkL( iDataSink );
	iJitter->SinkThreadLogon( *this );
	iJitter->SetupL( 200, 5, iCInfo );
    } 

void UT_CMccJitterBuffer::Setup2L()
    {
    iAlloc = EFalse;
   
    iJitter = CMccJitterBuffer::NewL( );
     
    iDataSink = new( ELeave )CMccDataSinkStub();
    iDataSource  = new( ELeave )CMccDataSourceStub( KRtpDataSource );
    
    iCInfo.iFourCC = KMccFourCCIdAMRNB;
    iCInfo.iAlgoUsed = ENoAlgoUsed; 
    iCInfo.iBitrate = 12200;
    iCInfo.iCNFrameSize = 12;
    iCInfo.iCodecMode = KAmrCodecModeOctetAlign;
    iCInfo.iEnableDTX = EFalse;
    iCInfo.iFrameSize = 20;
    iCInfo.iHwFrameTime = 20;
    iCInfo.iPtime = 20;
    iCInfo.iMaxPtime = 200;
    iCInfo.iRedundancyCount = 0;
    iCInfo.iRedundantPayload = 0;
    iCInfo.iJitterBufBufferLength = 200;
    iCInfo.iJitterBufThreshold = 5;
    
	iJitter->AddDataSinkL( iDataSink );
	iJitter->SinkThreadLogon( *this );
    iJitter->SetupL( 200, 5, iCInfo );
    }

void UT_CMccJitterBuffer::SetupAL()
    {
    iAlloc = ETrue;
    iJitter = CMccJitterBuffer::NewL( );
    
    iDataSink = new( ELeave )CMccDataSinkStub();
    iDataSource  = new( ELeave )CMccDataSourceStub( KRtpDataSource );
    
    iBuffer2  = CMMFDescriptorBuffer::NewL( 10 );

    iCInfo.iFourCC = KMccFourCCIdAMRNB;
    iCInfo.iAlgoUsed = ENoAlgoUsed; 
    iCInfo.iBitrate = 12200;
    iCInfo.iCNFrameSize = 12;
    iCInfo.iCodecMode = KAmrCodecModeOctetAlign;
    iCInfo.iEnableDTX = EFalse;
    iCInfo.iFrameSize = 20;
    iCInfo.iHwFrameTime = 20;
    iCInfo.iPtime = 20;
    iCInfo.iMaxPtime = 200;
    iCInfo.iRedundancyCount = 0;
    iCInfo.iRedundantPayload = 0;
    iCInfo.iJitterBufBufferLength = 200;
    iCInfo.iJitterBufThreshold = 5;
    
	iJitter->AddDataSinkL( iDataSink );
	iJitter->SinkThreadLogon( *this );
	iJitter->SetupL( 200, 5, iCInfo );
    } 
    
void UT_CMccJitterBuffer::SetupA2L()
    {
    iAlloc = ETrue;
    iJitter = CMccJitterBuffer::NewL( );
    
    iDataSink = new( ELeave )CMccDataSinkStub();
    iDataSource  = new( ELeave )CMccDataSourceStub( KRtpDataSource );
    
    iBuffer = CMMFDescriptorBuffer::NewL( 10 );
    
    iCInfo.iFourCC = KMccFourCCIdAMRNB;
    iCInfo.iAlgoUsed = ENoAlgoUsed; 
    iCInfo.iBitrate = 12200;
    iCInfo.iCNFrameSize = 12;
    iCInfo.iCodecMode = KAmrCodecModeOctetAlign;
    iCInfo.iEnableDTX = EFalse;
    iCInfo.iFrameSize = 20;
    iCInfo.iHwFrameTime = 20;
    iCInfo.iPtime = 20;
    iCInfo.iMaxPtime = 200;
    iCInfo.iRedundancyCount = 0;
    iCInfo.iRedundantPayload = 0;
    iCInfo.iJitterBufBufferLength = 200;
    iCInfo.iJitterBufThreshold = 5;
    
	iJitter->AddDataSinkL( iDataSink );
	iJitter->SinkThreadLogon( *this );
    iJitter->SetupL( 200, 5, iCInfo );
    }
    
void UT_CMccJitterBuffer::Teardown()
    {
    iJitter->SinkThreadLogoff();
    if ( iDataSink )
        {
        delete iDataSink;
        iDataSink = NULL;
        }

    if ( iDataSource )
        {
        delete iDataSource;
        iDataSource = NULL;
        }

    if ( iBuffer2 )
        {
        delete iBuffer2;
        iBuffer2 = NULL;
        }
    
    if ( iJitter )
        {
        delete iJitter;
        iJitter = NULL;
        }
        
    if ( iBuffer )
        {
        delete iBuffer;
        iBuffer = NULL;
        }     
    }

void UT_CMccJitterBuffer::UT_CMccJitterBuffer_SetupLL(  )
    {
    // Normal case
    iJitter->SetupL( 200, 5, iCInfo );
    
    // iBufferImpl is NULL
    delete iJitter->iBufferImpl;
    iJitter->iBufferImpl = NULL;
    iJitter->SetupL( 200, 5, iCInfo );   

    // iDataSink is NULL
    iJitter->iDataSink = NULL;
    EUNIT_ASSERT_SPECIFIC_LEAVE( iJitter->SetupL( 200, 5, iCInfo ), KErrNotReady );    
    }

void UT_CMccJitterBuffer::UT_CMccJitterBuffer_ResetBufferL()
    {
    iJitter->ResetBuffer();
    }

void UT_CMccJitterBuffer::UT_CMccJitterBuffer_DelayDownLL()
    {
    iJitter->DelayDownL();
    }

void UT_CMccJitterBuffer::UT_CMccJitterBuffer_DelayUpLL()
    {
    // Normal case
    iJitter->DelayUpL();
    
    // iBufferImpl is NULL
    delete iJitter->iBufferImpl;
    iJitter->iBufferImpl = NULL;
    EUNIT_ASSERT_LEAVE( iJitter->DelayUpL() );
    }

void UT_CMccJitterBuffer::UT_CMccJitterBuffer_SetSinkDataTypeCodeL()
    {
    EUNIT_ASSERT_EQUALS( iJitter->SetSinkDataTypeCode( i4ccjit, KUidMediaTypeAudio ), KErrNone );
    EUNIT_ASSERT_EQUALS( iJitter->SetSinkDataTypeCode( i4ccjit, KUidMediaTypeVideo ), KErrNotSupported );
    }

void UT_CMccJitterBuffer::UT_CMccJitterBuffer_SetSourceDataTypeCodeL()
    {
    iJitter->AddDataSourceL( iDataSource );
    
    EUNIT_ASSERT_EQUALS( iJitter->SetSourceDataTypeCode( i4ccjit, KUidMediaTypeAudio ), KErrNone );
    EUNIT_ASSERT_EQUALS( iJitter->SetSourceDataTypeCode( i4ccjit, KUidMediaTypeVideo ), KErrNotSupported );
    }

void UT_CMccJitterBuffer::UT_CMccJitterBuffer_SinkDataTypeCodeL()
    {
    EUNIT_ASSERT_NO_LEAVE( iJitter->SinkDataTypeCode( KUidMediaTypeAudio ) );
    EUNIT_ASSERT_NO_LEAVE( iJitter->SinkDataTypeCode( KUidMediaTypeVideo ) );
    }

void UT_CMccJitterBuffer::UT_CMccJitterBuffer_SourceDataTypeCodeL()
    {
    iJitter->AddDataSourceL( iDataSource );
    EUNIT_ASSERT_NO_LEAVE( iJitter->SourceDataTypeCode( KUidMediaTypeAudio ) );
    EUNIT_ASSERT_NO_LEAVE( iJitter->SourceDataTypeCode( KUidMediaTypeVideo ) );
    }

void UT_CMccJitterBuffer::UT_CMccJitterBuffer_ConstructSinkLL()
    {
    TBuf8<5> buf;
    EUNIT_ASSERT_SPECIFIC_LEAVE( iJitter->ConstructSinkL( buf ), KErrNotSupported );
    }

void UT_CMccJitterBuffer::UT_CMccJitterBuffer_ConstructSourceLL()
    {
    TBuf8<5> buf;
    EUNIT_ASSERT_SPECIFIC_LEAVE( iJitter->ConstructSourceL( buf ), KErrNotSupported );
    }
    
void UT_CMccJitterBuffer::UT_CMccJitterBuffer_SinkThreadLogonL()
    {
    EUNIT_ASSERT_EQUALS( iJitter->SinkThreadLogon( *this ), KErrAlreadyExists );
    }

void UT_CMccJitterBuffer::UT_CMccJitterBuffer_SourceThreadLogonL()
    {
    iJitter->AddDataSourceL( iDataSource );
    EUNIT_ASSERT_EQUALS( iJitter->SourceThreadLogon( *this ), KErrNotSupported );
    }

void UT_CMccJitterBuffer::UT_CMccJitterBuffer_SinkThreadLogoffL()
    {
    EUNIT_ASSERT_NO_LEAVE( iJitter->SinkThreadLogoff( ) );
    }

void UT_CMccJitterBuffer::UT_CMccJitterBuffer_SinkPrimeLL()
    {
    EUNIT_ASSERT_NO_LEAVE( iJitter->SinkPrimeL() );
    }

void UT_CMccJitterBuffer::UT_CMccJitterBuffer_SinkPlayLL()
    {
    iJitter->SinkPlayL();
    }

void UT_CMccJitterBuffer::UT_CMccJitterBuffer_SinkPauseLL()
    {
    EUNIT_ASSERT_NO_LEAVE( iJitter->SinkPauseL() );
    }

void UT_CMccJitterBuffer::UT_CMccJitterBuffer_SinkStopLL()
    {
    EUNIT_ASSERT_NO_LEAVE( iJitter->SinkStopL() );
    }

void UT_CMccJitterBuffer::UT_CMccJitterBuffer_CanCreateSinkBufferL()
    {
    // CMccJitterBuffer can't create buffers
    EUNIT_ASSERT_EQUALS( iJitter->CanCreateSinkBuffer(), EFalse );
    }

void UT_CMccJitterBuffer::UT_CMccJitterBuffer_CanCreateSourceBufferL()
    {
    // CMccJitterBuffer can't create buffers
    EUNIT_ASSERT_EQUALS( iJitter->CanCreateSourceBuffer(), EFalse );
    }

void UT_CMccJitterBuffer::UT_CMccJitterBuffer_CreateSinkBufferLL()
    {
    // CMccJitterBuffer can't create buffers
    TBool test = EFalse;
    EUNIT_ASSERT_LEAVE( iJitter->CreateSinkBufferL( KUidMediaTypeAudio, test ) );
    }

void UT_CMccJitterBuffer::UT_CMccJitterBuffer_CreateSourceBufferLL()
    {
    // CMccJitterBuffer can't create buffers
    TBool test = EFalse;
    EUNIT_ASSERT_LEAVE( iJitter->CreateSourceBufferL( KUidMediaTypeAudio, test ) );
    }

void UT_CMccJitterBuffer::UT_CMccJitterBuffer_FillBufferLL()
    {
    // CMccJitterBuffer can't fill buffers
    CMMFBuffer * sinkBuffer = iJitter->iDataSink->CreateSinkBufferL( KUidMediaTypeAudio, iJitter->iSnkBufRef );
    
    EUNIT_ASSERT_LEAVE( iJitter->FillBufferL( sinkBuffer, iDataSink, KUidMediaTypeAudio ) );
    }


void UT_CMccJitterBuffer::UT_CMccJitterBuffer_EmptyBufferLL()
    {    
    //testing AMR codec
    iCInfo.iFourCC = KMccFourCCIdAMRNB;
    iJitter->SetupL( 200, 5, iCInfo );
    
    delete iBuffer;
    iBuffer = NULL;    
    iBuffer = CMMFDescriptorBuffer::NewL( 10 );
    iBuffer->Data().Append( _L("1234567890") );       
    iJitter->EmptyBufferL( iBuffer, iDataSource, KUidMediaTypeAudio );
        
    iBuffer->ReAllocBufferL( 30 );
    iBuffer->Data().Append( _L("123456789012345678901234567890") );
    iJitter->EmptyBufferL( iBuffer, iDataSource, KUidMediaTypeAudio );
        
    iBuffer->ReAllocBufferL( 31 );
    iBuffer->Data().Append(_L("1234567890123456789012345678901"));
    iJitter->EmptyBufferL( iBuffer, iDataSource, KUidMediaTypeAudio );
        
    iBuffer->ReAllocBufferL( 32 );
    iBuffer->Data().Append(_L("12345678901234567890123456789012"));
    iJitter->EmptyBufferL( iBuffer, iDataSource, KUidMediaTypeAudio );
    }
 
 
 void UT_CMccJitterBuffer::UT_CMccJitterBuffer_EmptyBuffer2LL()
    {    
     //testing G711 codec
    iCInfo.iFourCC = KMccFourCCIdG711;
    iJitter->SetupL( 200, 5, iCInfo );
        
    iBuffer  = CMMFDescriptorBuffer::NewL( 10 );
    iBuffer->Data().Append( _L("1234567890") );       
    iJitter->EmptyBufferL( iBuffer, iDataSource, KUidMediaTypeAudio );
        
    iBuffer->ReAllocBufferL( 161 );
    iBuffer->Data().Append( _L("12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901") );
    iJitter->EmptyBufferL( iBuffer, iDataSource, KUidMediaTypeAudio );
        
    iBuffer->ReAllocBufferL( 162 );
    iBuffer->Data().Append(_L("123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012") );
    iJitter->EmptyBufferL( iBuffer, iDataSource, KUidMediaTypeAudio );
    }    


void UT_CMccJitterBuffer::UT_CMccJitterBuffer_EmptyBuffer3LL()
    {
    //testing iLBC codec
    iCInfo.iFourCC = KMccFourCCIdILBC;
    iJitter->SetupL( 200, 5, iCInfo );
        
    iBuffer = CMMFDescriptorBuffer::NewL( 10 );
    iBuffer->Data().Append( _L("1234567890") );       
    iJitter->EmptyBufferL( iBuffer, iDataSource, KUidMediaTypeAudio );
        
    iBuffer->ReAllocBufferL( 51 );
    iBuffer->Data().Append( _L("123456789012345678901234567890123456789012345678901") );
    iJitter->EmptyBufferL( iBuffer, iDataSource, KUidMediaTypeAudio );
        
    iBuffer->ReAllocBufferL( 52 );
    iBuffer->Data().Append( _L("1234567890123456789012345678901234567890123456789012") );
    iJitter->EmptyBufferL( iBuffer, iDataSource, KUidMediaTypeAudio );
    }


void UT_CMccJitterBuffer::UT_CMccJitterBuffer_EmptyBuffer4LL()
    {    
    //testing G729 codec
    iCInfo.iFourCC = KMccFourCCIdG729;
    iJitter->SetupL( 200, 5, iCInfo );
        
    iBuffer  = CMMFDescriptorBuffer::NewL( 10 );
    iBuffer->Data().Append( _L("1234567890") );       
    iJitter->EmptyBufferL( iBuffer, iDataSource, KUidMediaTypeAudio );
        
    iBuffer->ReAllocBufferL( 23 );
    iBuffer->Data().Append( _L("12345678901234567890123") );
    iJitter->EmptyBufferL( iBuffer, iDataSource, KUidMediaTypeAudio );
        
    iBuffer->ReAllocBufferL( 24 );
    iBuffer->Data().Append( _L("123456789012345678901234") );
    iJitter->EmptyBufferL( iBuffer, iDataSource, KUidMediaTypeAudio );
    } 

void UT_CMccJitterBuffer::UT_CMccJitterBuffer_BufferFilledLL()
    {
    EUNIT_ASSERT_LEAVE( iJitter->BufferFilledL( NULL ) );
    }

void UT_CMccJitterBuffer::UT_CMccJitterBuffer_BufferEmptiedLL()
    {
    CMMFBuffer * sinkBuffer = iJitter->iDataSink->CreateSinkBufferL( KUidMediaTypeAudio, iJitter->iSnkBufRef );
    
    iJitter->BufferEmptiedL( sinkBuffer );
    }

void UT_CMccJitterBuffer::UT_CMccJitterBuffer_AddDataSinkLL()
    {
    // leaves because KErrAlreadyExists
    EUNIT_ASSERT_LEAVE( iJitter->AddDataSinkL( iDataSink ) );
    }

void UT_CMccJitterBuffer::UT_CMccJitterBuffer_AddDataSourceLL()
    {
    MDataSource* dataSource = NULL;
    
    // dataSource is left null on purpose to test LeaveIfNull -method
    EUNIT_ASSERT_LEAVE( iJitter->AddDataSourceL( dataSource ) );
    EUNIT_ASSERT_NO_LEAVE( iJitter->AddDataSourceL( iDataSource ) );
    }

void UT_CMccJitterBuffer::UT_CMccJitterBuffer_SendJitterBufferEventToClientL()
    {
    iJitter->SendJitterBufferEventToClient();
    }

void UT_CMccJitterBuffer::UT_CMccJitterBuffer_GetDataSinkL()
    {    
    MDataSink* dataSink = NULL;
    dataSink = iJitter->GetDataSink();
    
    if ( dataSink )
        {
        EUNIT_ASSERT( ETrue );
        }
    else
        {
        EUNIT_ASSERT( EFalse );
        }
    }
    
void UT_CMccJitterBuffer::UT_CMccJitterBuffer_NegotiateLL(  )
    {
    iJitter->NegotiateL( *iDataSource );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMccJitterBuffer,
    "Add test suite description here.",
    "UNIT" )
    
EUNIT_TEST(
    "SetupL - test ",
    "CMccJitterBuffer",
    "SetupL",
    "FUNCTIONALITY",
    SetupL, UT_CMccJitterBuffer_SetupLL, Teardown )

EUNIT_TEST(
    "ResetBuffer - test ",
    "CMccJitterBuffer",
    "ResetBuffer",
    "FUNCTIONALITY",
    SetupL, UT_CMccJitterBuffer_ResetBufferL, Teardown)

EUNIT_TEST(
    "DelayDownL - test ",
    "CMccJitterBuffer",
    "DelayDownL",
    "FUNCTIONALITY",
    SetupL, UT_CMccJitterBuffer_DelayDownLL, Teardown)

EUNIT_TEST(
    "DelayUpL - test ",
    "CMccJitterBuffer",
    "DelayUpL",
    "FUNCTIONALITY",
    SetupL, UT_CMccJitterBuffer_DelayUpLL, Teardown)

EUNIT_TEST(
    "SetSinkDataTypeCode - test ",
    "CMccJitterBuffer",
    "SetSinkDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CMccJitterBuffer_SetSinkDataTypeCodeL, Teardown)

EUNIT_TEST(
    "SetSourceDataTypeCode - test ",
    "CMccJitterBuffer",
    "SetSourceDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CMccJitterBuffer_SetSourceDataTypeCodeL, Teardown)

EUNIT_TEST(
    "SinkDataTypeCode - test ",
    "CMccJitterBuffer",
    "SinkDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CMccJitterBuffer_SinkDataTypeCodeL, Teardown)

EUNIT_TEST(
    "SourceDataTypeCode - test ",
    "CMccJitterBuffer",
    "SourceDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CMccJitterBuffer_SourceDataTypeCodeL, Teardown)

EUNIT_TEST(
    "ConstructSinkL - test ",
    "CMccJitterBuffer",
    "ConstructSinkL",
    "FUNCTIONALITY",
    SetupL, UT_CMccJitterBuffer_ConstructSinkLL, Teardown)

EUNIT_TEST(
    "ConstructSourceL - test ",
    "CMccJitterBuffer",
    "ConstructSourceL",
    "FUNCTIONALITY",
    SetupL, UT_CMccJitterBuffer_ConstructSourceLL, Teardown)

EUNIT_TEST(
    "SinkThreadLogon - test ",
    "CMccJitterBuffer",
    "SinkThreadLogon",
    "FUNCTIONALITY",
    SetupL, UT_CMccJitterBuffer_SinkThreadLogonL, Teardown)

EUNIT_TEST(
    "SourceThreadLogon - test ",
    "CMccJitterBuffer",
    "SourceThreadLogon",
    "FUNCTIONALITY",
    SetupL, UT_CMccJitterBuffer_SourceThreadLogonL, Teardown)

EUNIT_TEST(
    "SinkThreadLogoff - test ",
    "CMccJitterBuffer",
    "SinkThreadLogoff",
    "FUNCTIONALITY",
    SetupL, UT_CMccJitterBuffer_SinkThreadLogoffL, Teardown)

EUNIT_TEST(
    "SinkPrimeL - test ",
    "CMccJitterBuffer",
    "SinkPrimeL",
    "FUNCTIONALITY",
    SetupL, UT_CMccJitterBuffer_SinkPrimeLL, Teardown)

EUNIT_TEST(
    "SinkPlayL - test ",
    "CMccJitterBuffer",
    "SinkPlayL",
    "FUNCTIONALITY",
    SetupL, UT_CMccJitterBuffer_SinkPlayLL, Teardown)

EUNIT_TEST(
    "SinkPauseL - test ",
    "CMccJitterBuffer",
    "SinkPauseL",
    "FUNCTIONALITY",
    SetupL, UT_CMccJitterBuffer_SinkPauseLL, Teardown)

EUNIT_TEST(
    "SinkStopL - test ",
    "CMccJitterBuffer",
    "SinkStopL",
    "FUNCTIONALITY",
    SetupL, UT_CMccJitterBuffer_SinkStopLL, Teardown)

EUNIT_TEST(
    "CanCreateSinkBuffer - test ",
    "CMccJitterBuffer",
    "CanCreateSinkBuffer",
    "FUNCTIONALITY",
    SetupL, UT_CMccJitterBuffer_CanCreateSinkBufferL, Teardown)

EUNIT_TEST(
    "CanCreateSourceBuffer - test ",
    "CMccJitterBuffer",
    "CanCreateSourceBuffer",
    "FUNCTIONALITY",
    SetupL, UT_CMccJitterBuffer_CanCreateSourceBufferL, Teardown)

EUNIT_TEST(
    "CreateSinkBufferL - test ",
    "CMccJitterBuffer",
    "CreateSinkBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CMccJitterBuffer_CreateSinkBufferLL, Teardown)

EUNIT_TEST(
    "CreateSourceBufferL - test ",
    "CMccJitterBuffer",
    "CreateSourceBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CMccJitterBuffer_CreateSourceBufferLL, Teardown)

EUNIT_TEST(
    "FillBufferL - test ",
    "CMccJitterBuffer",
    "FillBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CMccJitterBuffer_FillBufferLL, Teardown)

EUNIT_TEST(
    "EmptyBufferL - test ",
    "CMccJitterBuffer",
    "EmptyBufferL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccJitterBuffer_EmptyBufferLL, Teardown)
    
EUNIT_TEST(
    "EmptyBuffer2L - test ",
    "CMccJitterBuffer",
    "EmptyBuffer2L",
    "FUNCTIONALITY",
    Setup2L, UT_CMccJitterBuffer_EmptyBuffer2LL, Teardown)
    
EUNIT_TEST(
    "EmptyBuffer3L - test ",
    "CMccJitterBuffer",
    "EmptyBuffer3L",
    "FUNCTIONALITY",
    Setup2L, UT_CMccJitterBuffer_EmptyBuffer3LL, Teardown)
    
EUNIT_TEST(
    "EmptyBuffer4L - test ",
    "CMccJitterBuffer",
    "EmptyBuffer4L",
    "FUNCTIONALITY",
    Setup2L, UT_CMccJitterBuffer_EmptyBuffer4LL, Teardown)

EUNIT_TEST(
    "BufferFilledL - test ",
    "CMccJitterBuffer",
    "BufferFilledL",
    "FUNCTIONALITY",
    SetupL, UT_CMccJitterBuffer_BufferFilledLL, Teardown)

EUNIT_TEST(
    "BufferEmptiedL - test ",
    "CMccJitterBuffer",
    "BufferEmptiedL",
    "FUNCTIONALITY",
    SetupL, UT_CMccJitterBuffer_BufferEmptiedLL, Teardown)

EUNIT_TEST(
    "AddDataSinkL - test ",
    "CMccJitterBuffer",
    "AddDataSinkL",
    "FUNCTIONALITY",
    SetupL, UT_CMccJitterBuffer_AddDataSinkLL, Teardown)

EUNIT_TEST(
    "AddDataSourceL - test ",
    "CMccJitterBuffer",
    "AddDataSourceL",
    "FUNCTIONALITY",
    SetupL, UT_CMccJitterBuffer_AddDataSourceLL, Teardown)

EUNIT_TEST(
    "SendJitterBufferEventToClient - test ",
    "CMccJitterBuffer",
    "SendJitterBufferEventToClient",
    "FUNCTIONALITY",
    SetupL, UT_CMccJitterBuffer_SendJitterBufferEventToClientL, Teardown )

EUNIT_TEST(
    "GetDataSink - test ",
    "CMccJitterBuffer",
    "GetDataSink",
    "FUNCTIONALITY",
    SetupL, UT_CMccJitterBuffer_GetDataSinkL, Teardown)

EUNIT_TEST(
    "NegotiateL - test ",
    "CMccJitterBuffer",
    "NegotiateL",
    "FUNCTIONALITY",
    SetupL, UT_CMccJitterBuffer_NegotiateLL, Teardown)

// ALLOC TESTS

#ifdef ALLOC_TEST_ON

EUNIT_TEST(
    "SetupL - alloc test ",
    "CMccJitterBuffer",
    "SetupL",
    "ERRORHANDLING",
    SetupAL, UT_CMccJitterBuffer_SetupLL, Teardown )

EUNIT_ALLOC_TEST(
    "ResetBuffer - alloc test ",
    "CMccJitterBuffer",
    "ResetBuffer",
    "ERRORHANDLING",
    SetupAL, UT_CMccJitterBuffer_ResetBufferL, Teardown)

EUNIT_ALLOC_TEST(
    "DelayDownL - alloc test ",
    "CMccJitterBuffer",
    "DelayDownL",
    "ERRORHANDLING",
    SetupAL, UT_CMccJitterBuffer_DelayDownLL, Teardown)

EUNIT_ALLOC_TEST(
    "DelayUpL - alloc test ",
    "CMccJitterBuffer",
    "DelayUpL",
    "ERRORHANDLING",
    SetupAL, UT_CMccJitterBuffer_DelayUpLL, Teardown)

EUNIT_ALLOC_TEST(
    "SetSinkDataTypeCode - alloc test ",
    "CMccJitterBuffer",
    "SetSinkDataTypeCode",
    "ERRORHANDLING",
    SetupAL, UT_CMccJitterBuffer_SetSinkDataTypeCodeL, Teardown)

EUNIT_ALLOC_TEST(
    "SetSourceDataTypeCode - alloc test ",
    "CMccJitterBuffer",
    "SetSourceDataTypeCode",
    "ERRORHANDLING",
    SetupAL, UT_CMccJitterBuffer_SetSourceDataTypeCodeL, Teardown)

EUNIT_ALLOC_TEST(
    "SinkDataTypeCode - alloc test ",
    "CMccJitterBuffer",
    "SinkDataTypeCode",
    "ERRORHANDLING",
    SetupAL, UT_CMccJitterBuffer_SinkDataTypeCodeL, Teardown)

EUNIT_ALLOC_TEST(
    "SourceDataTypeCode - alloc test ",
    "CMccJitterBuffer",
    "SourceDataTypeCode",
    "ERRORHANDLING",
    SetupAL, UT_CMccJitterBuffer_SourceDataTypeCodeL, Teardown)

EUNIT_ALLOC_TEST(
    "ConstructSinkL - alloc test ",
    "CMccJitterBuffer",
    "ConstructSinkL",
    "ERRORHANDLING",
    SetupAL, UT_CMccJitterBuffer_ConstructSinkLL, Teardown)

EUNIT_ALLOC_TEST(
    "ConstructSourceL - alloc test ",
    "CMccJitterBuffer",
    "ConstructSourceL",
    "ERRORHANDLING",
    SetupAL, UT_CMccJitterBuffer_ConstructSourceLL, Teardown)

EUNIT_ALLOC_TEST(
    "SinkThreadLogon - alloc test ",
    "CMccJitterBuffer",
    "SinkThreadLogon",
    "ERRORHANDLING",
    SetupAL, UT_CMccJitterBuffer_SinkThreadLogonL, Teardown)

EUNIT_ALLOC_TEST(
    "SourceThreadLogon - alloc test ",
    "CMccJitterBuffer",
    "SourceThreadLogon",
    "ERRORHANDLING",
    SetupAL, UT_CMccJitterBuffer_SourceThreadLogonL, Teardown)

EUNIT_ALLOC_TEST(
    "SinkThreadLogoff - alloc test ",
    "CMccJitterBuffer",
    "SinkThreadLogoff",
    "ERRORHANDLING",
    SetupAL, UT_CMccJitterBuffer_SinkThreadLogoffL, Teardown)

EUNIT_ALLOC_TEST(
    "SinkPrimeL - alloc test ",
    "CMccJitterBuffer",
    "SinkPrimeL",
    "ERRORHANDLING",
    SetupAL, UT_CMccJitterBuffer_SinkPrimeLL, Teardown)

EUNIT_ALLOC_TEST(
    "SinkPlayL - alloc test ",
    "CMccJitterBuffer",
    "SinkPlayL",
    "ERRORHANDLING",
    SetupAL, UT_CMccJitterBuffer_SinkPlayLL, Teardown)

EUNIT_ALLOC_TEST(
    "SinkPauseL - alloc test ",
    "CMccJitterBuffer",
    "SinkPauseL",
    "ERRORHANDLING",
    SetupAL, UT_CMccJitterBuffer_SinkPauseLL, Teardown)

EUNIT_ALLOC_TEST(
    "SinkStopL - alloc test ",
    "CMccJitterBuffer",
    "SinkStopL",
    "ERRORHANDLING",
    SetupAL, UT_CMccJitterBuffer_SinkStopLL, Teardown)

EUNIT_ALLOC_TEST(
    "CanCreateSinkBuffer - alloc test ",
    "CMccJitterBuffer",
    "CanCreateSinkBuffer",
    "ERRORHANDLING",
    SetupAL, UT_CMccJitterBuffer_CanCreateSinkBufferL, Teardown)

EUNIT_ALLOC_TEST(
    "CanCreateSourceBuffer - alloc test ",
    "CMccJitterBuffer",
    "CanCreateSourceBuffer",
    "ERRORHANDLING",
    SetupAL, UT_CMccJitterBuffer_CanCreateSourceBufferL, Teardown)

EUNIT_ALLOC_TEST(
    "CreateSinkBufferL - alloc test ",
    "CMccJitterBuffer",
    "CreateSinkBufferL",
    "ERRORHANDLING",
    SetupAL, UT_CMccJitterBuffer_CreateSinkBufferLL, Teardown)

EUNIT_ALLOC_TEST(
    "CreateSourceBufferL - alloc test ",
    "CMccJitterBuffer",
    "CreateSourceBufferL",
    "ERRORHANDLING",
    SetupAL, UT_CMccJitterBuffer_CreateSourceBufferLL, Teardown)

EUNIT_ALLOC_TEST(
    "FillBufferL - alloc test ",
    "CMccJitterBuffer",
    "FillBufferL",
    "ERRORHANDLING",
    SetupAL, UT_CMccJitterBuffer_FillBufferLL, Teardown)

EUNIT_ALLOC_TEST(
    "EmptyBufferL - alloc test ",
    "CMccJitterBuffer",
    "EmptyBufferL",
    "ERRORHANDLING",
    SetupA2L, UT_CMccJitterBuffer_EmptyBufferLL, Teardown)

EUNIT_ALLOC_TEST(
    "BufferFilledL - alloc test ",
    "CMccJitterBuffer",
    "BufferFilledL",
    "ERRORHANDLING",
    SetupAL, UT_CMccJitterBuffer_BufferFilledLL, Teardown)

EUNIT_ALLOC_TEST(
    "BufferEmptiedL - alloc test ",
    "CMccJitterBuffer",
    "BufferEmptiedL",
    "ERRORHANDLING",
    SetupAL, UT_CMccJitterBuffer_BufferEmptiedLL, Teardown)

EUNIT_ALLOC_TEST(
    "AddDataSinkL - alloc test ",
    "CMccJitterBuffer",
    "AddDataSinkL",
    "ERRORHANDLING",
    SetupAL, UT_CMccJitterBuffer_AddDataSinkLL, Teardown)

EUNIT_ALLOC_TEST(
    "AddDataSourceL - alloc test ",
    "CMccJitterBuffer",
    "AddDataSourceL",
    "ERRORHANDLING",
    SetupAL, UT_CMccJitterBuffer_AddDataSourceLL, Teardown)

EUNIT_ALLOC_TEST(
    "GetDataSink - alloc test ",
    "CMccJitterBuffer",
    "GetDataSink",
    "ERRORHANDLING",
    SetupAL, UT_CMccJitterBuffer_GetDataSinkL, Teardown)

EUNIT_ALLOC_TEST(
    "NegotiateL - alloc test ",
    "CMccJitterBuffer",
    "NegotiateL",
    "ERRORHANDLING",
    SetupAL, UT_CMccJitterBuffer_NegotiateLL, Teardown)


#endif // ALLOC_TEST

EUNIT_END_TEST_TABLE

//  END OF FILE
