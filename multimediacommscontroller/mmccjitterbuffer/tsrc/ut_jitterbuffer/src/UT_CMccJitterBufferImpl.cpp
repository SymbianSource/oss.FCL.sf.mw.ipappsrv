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
#include "UT_CMccJitterBufferImpl.h"

//  INTERNAL INCLUDES
#include "mccjitterbufferimpl.h"
#include "mmccevents.h"
#include "mccinternalevents.h"

// CONSTRUCTION
UT_CMccJitterBufferImpl* UT_CMccJitterBufferImpl::NewL()
    {
    UT_CMccJitterBufferImpl* self = UT_CMccJitterBufferImpl::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMccJitterBufferImpl* UT_CMccJitterBufferImpl::NewLC()
    {
    UT_CMccJitterBufferImpl* self = new( ELeave ) UT_CMccJitterBufferImpl();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccJitterBufferImpl::~UT_CMccJitterBufferImpl()
    {
    }

// Default constructor
UT_CMccJitterBufferImpl::UT_CMccJitterBufferImpl() :
    iJBufImpl( NULL ),
    iDevSoundDummy( NULL )
    {
    }

// Second phase construct
void UT_CMccJitterBufferImpl::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void UT_CMccJitterBufferImpl::CreateJitterBufferEvent( TMccEvent& aEvent )
    {
    TMccJitterBufferEventData eventData;
    eventData.iFramesPlayed = 2;
    eventData.iFramesReceived = 4;
    aEvent.iEventData = TMccJitterBufferEventDataPackage( eventData );
    }

void UT_CMccJitterBufferImpl::SetupL()
    {
    iJBufImpl = CMccJitterBufferImpl::NewL();

    iCInfo.iFourCC = KMccFourCCIdAMRNB;
    iCInfo.iPtime = 20;
    iCInfo.iHwFrameTime = 20;
    iCInfo.iBitrate = 12200;
    iCInfo.iJitterBufBufferLength = 200;
    iCInfo.iJitterBufInactivityTimeOut = 35;

    iJBufImpl->SetupL( 5, iCInfo, *iDevSoundDummy, this, 0 );
    } 

void UT_CMccJitterBufferImpl::Setup2L()
    {
    iJBufImpl = CMccJitterBufferImpl::NewL();
    } 

void UT_CMccJitterBufferImpl::Teardown()
    {
    if ( iJBufImpl )
        {
        delete iJBufImpl;
        iJBufImpl = NULL;
        }
    }

void UT_CMccJitterBufferImpl::UT_CMccJitterBufferImpl_SetupLL()
    {
    CMMFDevSound* devSoundDummy( NULL );
    TMccCodecInfo cInfo;
    // aCInfo.iPtime = 0
    EUNIT_ASSERT_SPECIFIC_LEAVE( iJBufImpl->SetupL( 0, 
                                 cInfo, *devSoundDummy, this, 0 ),
                                 KErrArgument );
    
    cInfo.iPtime = 20;
    // aCInfo.iJitterBufBufferLength = 0
    EUNIT_ASSERT_SPECIFIC_LEAVE( iJBufImpl->SetupL( 0, 
                                 cInfo, *devSoundDummy, this, 0 ),
                                 KErrArgument );
                                 
    cInfo.iJitterBufBufferLength = 20;
    // aCInfo.iHwFrameTime = 0
    EUNIT_ASSERT_SPECIFIC_LEAVE( iJBufImpl->SetupL( 0, 
                                 cInfo, *devSoundDummy, this, 0 ),
                                 KErrArgument );
    
    cInfo.iHwFrameTime = 20;
    cInfo.iJitterBufInactivityTimeOut = 30;
    cInfo.iJitterBufPlayToneFrequency = 1000;
    cInfo.iJitterBufPlayToneDuration = 100;
    cInfo.iJitterBufBufferLength = 0;
    // aBufferSize < aCInfo.iPtime
    EUNIT_ASSERT_SPECIFIC_LEAVE( iJBufImpl->SetupL( 0, 
                                 cInfo, *devSoundDummy, this, 0 ),
                                 KErrArgument );
    
    cInfo.iJitterBufBufferLength = 20;
    cInfo.iFourCC = KMccFourCCIdDTMF;
    // wrong codec
    EUNIT_ASSERT_SPECIFIC_LEAVE( iJBufImpl->SetupL( 0, 
                                 cInfo, *devSoundDummy, this, 0 ),
                                 KErrNotSupported );

    cInfo.iFourCC = KMccFourCCIdG711;
    cInfo.iJitterBufBufferLength = 10;

    // aPlayoutThreshold > iBufferLength
    EUNIT_ASSERT_SPECIFIC_LEAVE( iJBufImpl->SetupL( 20,
                                 cInfo, *devSoundDummy, this, 0 ),
                                 KErrTooBig );
    
    // Run the same test for the rest of the codecs just to reach the branches
    cInfo.iFourCC = KMccFourCCIdILBC;
    EUNIT_ASSERT_SPECIFIC_LEAVE( iJBufImpl->SetupL( 20,
                                 cInfo, *devSoundDummy, this, 0 ),
                                 KErrTooBig );
    
    cInfo.iFourCC = KMccFourCCIdG729;
    EUNIT_ASSERT_SPECIFIC_LEAVE( iJBufImpl->SetupL( 20,
                                 cInfo, *devSoundDummy, this, 0 ),
                                 KErrTooBig );
    }
    
void UT_CMccJitterBufferImpl::UT_CMccJitterBufferImpl_SetupL2L()
    {
    // AMR-NB
    CMMFDevSound* devSoundDummy( NULL );
    TMccCodecInfo cInfo;

    cInfo.iFourCC = KMccFourCCIdAMRNB;
    cInfo.iPtime = 20;
    cInfo.iHwFrameTime = 20;
    cInfo.iBitrate = 12200;
    cInfo.iJitterBufBufferLength = 200;

    iJBufImpl->SetupL( 5, cInfo, *devSoundDummy, this, 0 );
    iJBufImpl->SetupL( 33, cInfo, *devSoundDummy, this, 0 );
    }
    
void UT_CMccJitterBufferImpl::UT_CMccJitterBufferImpl_SetupL3L()
    {
    // codec G711
    CMMFDevSound* devSoundDummy( NULL );
    TMccCodecInfo cInfo;
    cInfo.iPtime = 20;
    cInfo.iHwFrameTime = 20;
    cInfo.iBitrate = 12200;
    cInfo.iJitterBufBufferLength = 200;

    cInfo.iFourCC = KMccFourCCIdG711;
    iJBufImpl->SetupL( 5, cInfo, *devSoundDummy, this, 0 );
    // Test G.711 dynamic configuration.
    cInfo.iHwFrameTime = 10;
    iJBufImpl->SetupL( 5, cInfo, *devSoundDummy, this, 0 );
    iJBufImpl->SetupL( 33, cInfo, *devSoundDummy, this, 0 );
    }
    
void UT_CMccJitterBufferImpl::UT_CMccJitterBufferImpl_SetupL4L()
    {
    // codec KMccFourCCIdILBC
    CMMFDevSound* devSoundDummy( NULL );
    TMccCodecInfo cInfo;

    cInfo.iPtime = 20;
    cInfo.iHwFrameTime = 20;
    cInfo.iBitrate = 12200;
    cInfo.iJitterBufBufferLength = 200;

    cInfo.iFourCC = KMccFourCCIdILBC;
    iJBufImpl->SetupL( 5, cInfo, *devSoundDummy, this, 0 );
    iJBufImpl->SetupL( 33, cInfo, *devSoundDummy, this, 0 );
    }  
    
void UT_CMccJitterBufferImpl::UT_CMccJitterBufferImpl_SetupL5L()
    {
    // KMccFourCCIdG729
    CMMFDevSound* devSoundDummy( NULL );
    TMccCodecInfo cInfo;

    cInfo.iPtime = 20;
    cInfo.iHwFrameTime = 20;
    cInfo.iBitrate = 12200;
    cInfo.iJitterBufBufferLength = 200;
    
    cInfo.iFourCC = KMccFourCCIdG729;
    iJBufImpl->SetupL( 5, cInfo, *devSoundDummy, this, 0 );
    iJBufImpl->SetupL( 33, cInfo, *devSoundDummy, this, 0 );
    }   
    
void UT_CMccJitterBufferImpl::UT_CMccJitterBufferImpl_SetupL6L()
    {
    // KMccFourCCIdAMRWB
    CMMFDevSound* devSoundDummy( NULL );
    TMccCodecInfo cInfo;

    cInfo.iPtime = 20;
    cInfo.iHwFrameTime = 20;
    cInfo.iBitrate = 12200;
    cInfo.iJitterBufBufferLength = 200;
    
    cInfo.iFourCC = KMccFourCCIdAMRWB;
    iJBufImpl->SetupL( 5, cInfo, *devSoundDummy, this, 0 );
    iJBufImpl->SetupL( 33, cInfo, *devSoundDummy, this, 0 );
    EUNIT_ASSERT( iJBufImpl->iSampleRate == 16 );
    
    
    // Check that values are not changed: 
    
    // Save original values:
	TMccCodecInfo originalCInfo( cInfo );
    
    // Set dummy values:
    cInfo.iPayloadFormatEncoder = 111;
    cInfo.iPayloadFormatDecoder = 222;
    cInfo.iBitrate = 333;
    cInfo.iSamplingFreq = 444;
    cInfo.iEnableDTX = ETrue;
    cInfo.iCodecMode = 555;
    cInfo.iAlgoUsed = 666;
    cInfo.iPtime = 777;
    cInfo.iMaxPtime = 888;
    cInfo.iPayloadType = 999;
    cInfo.iRedundantPayload = 1111;
    cInfo.iRedundancyCount = 2222;
    cInfo.iFrameSize = 3333;
    cInfo.iCNFrameSize = 4444;
    cInfo.iFmtpLength = 5555;
    cInfo.iJitterSize = 6666;
    cInfo.iNeighbor = ETrue;
    cInfo.iModeChangePeriod = 7777;
    cInfo.iJitterBufPlayToneTimeout = 8888; 
    cInfo.iJitterBufPlayToneFrequency = 9999; 
    cInfo.iJitterBufPlayToneDuration = 1;
    cInfo.iComfortNoiseGenerationPt = 2;
    cInfo.iBitrateMask = 3;
	cInfo.iAverageBitrate = 4;
	cInfo.iMaxBitrate = 5;
	cInfo.iFramerate = 6;
    cInfo.iVideoWidth = 7;
    cInfo.iVideoHeight = 8;   
    cInfo.iMTUSize = 9;
    cInfo.iSessionID = 1;
    cInfo.iLinkID = 2;
    cInfo.iStreamID = 3;
    cInfo.iKeepaliveInterval = 1234;
    cInfo.iKeepalivePT = 2;
    cInfo.iIsUpdate = ETrue;
    cInfo.iMaxRed = 1000;
    cInfo.iSourceDefinedTimeStamps = ETrue;

	// Put the same parameter values than in previous function call, 
    iJBufImpl->SetupL( 33, cInfo, *devSoundDummy, this, 0 );
    // and check that TMccCodecInfo values were not changed:
    TBool retval = originalCInfo.Compare( originalCInfo, iJBufImpl->iCInfo );
    EUNIT_ASSERT( retval );  
    }  

void UT_CMccJitterBufferImpl::UT_CMccJitterBufferImpl_ResetBufferL()
    {
    iJBufImpl->DelayUpL();
    iJBufImpl->ResetBuffer();
    }
    
void UT_CMccJitterBufferImpl::UT_CMccJitterBufferImpl_CurrentCodecL()
    {
    TFourCC codec = iJBufImpl->CurrentCodec();
    EUNIT_ASSERT_EQUALS( codec.FourCC(), KMccFourCCIdAMRNB );
    }

void UT_CMccJitterBufferImpl::UT_CMccJitterBufferImpl_DelayUpLL()
    {
    CMMFDevSound* devSoundDummy( NULL );
    TMccCodecInfo cInfo;

    cInfo.iFourCC = KMccFourCCIdAMRNB;
    cInfo.iPtime = 20;
    cInfo.iHwFrameTime = 20;
    cInfo.iBitrate = 12200;
    cInfo.iJitterBufBufferLength = 200;

    // Use a playout treshold of 0
    iJBufImpl->SetupL( 0, cInfo, *devSoundDummy, this, 0 );

    iJBufImpl->DelayUpL();

    // Reach more branches using the default playout treshold
    this->Teardown();
    this->SetupL();

    iJBufImpl->DelayUpL();
    }

void UT_CMccJitterBufferImpl::UT_CMccJitterBufferImpl_DelayDownLL()
    {
    iJBufImpl->DelayDownL();

    // Put a packet in the buffer
    CMMFDescriptorBuffer* buf = CMMFDescriptorBuffer::NewL( 10 );
    CleanupStack::PushL( buf );
    
    TDes8& bufData = buf->Data();
    _LIT( KText, "foo42" );
    bufData.Copy( KText );
    iJBufImpl->AddDataFrameL( buf );

    iJBufImpl->DelayDownL();
    
    iJBufImpl->AddDataFrameL( buf );
    iJBufImpl->AddDataFrameL( buf );
    iJBufImpl->AddDataFrameL( buf );
    iJBufImpl->AddDataFrameL( buf );
    iJBufImpl->AddDataFrameL( buf );
    iJBufImpl->AddDataFrameL( buf );
    iJBufImpl->AddDataFrameL( buf );
    iJBufImpl->AddDataFrameL( buf );

    iJBufImpl->DelayDownL();

    // Create a new Jbuf with parameters that let us reach more branches
    this->Teardown();
    this->Setup2L();

    iCInfo.iFourCC = KMccFourCCIdAMRNB;
    iCInfo.iPtime = 20;
    iCInfo.iHwFrameTime = 20;
    iCInfo.iBitrate = 12200;
    iCInfo.iJitterBufBufferLength = 2;
    iCInfo.iJitterBufInactivityTimeOut = 35;

    iJBufImpl->SetupL( 1, iCInfo, *iDevSoundDummy, this, 0 );

    iJBufImpl->DelayDownL();
    iJBufImpl->DelayDownL();

    iJBufImpl->ResetBuffer();

    // Make the buffer full (it has space for only two elements)
    iJBufImpl->AddDataFrameL( buf );
    iJBufImpl->AddDataFrameL( buf );

    iJBufImpl->DelayDownL();
    iJBufImpl->DelayDownL();
    iJBufImpl->DelayDownL();

    CleanupStack::PopAndDestroy( buf );
    }

void UT_CMccJitterBufferImpl::UT_CMccJitterBufferImpl_CalculateDelayL()
    {
    // Put a packet in the buffer
    CMMFDescriptorBuffer* buf = CMMFDescriptorBuffer::NewL( 10 );
    CleanupStack::PushL( buf );
    
    TDes8& bufData = buf->Data();
    _LIT( KText, "foo42" );
    bufData.Copy( KText );
    iJBufImpl->AddDataFrameL( buf );

    TTimeIntervalMicroSeconds32 time( iJBufImpl->CalculateDelay() );
    EUNIT_ASSERT_EQUALS( time.Int(), 20 );

    CleanupStack::PopAndDestroy( buf ); 
    }

void UT_CMccJitterBufferImpl::UT_CMccJitterBufferImpl_AddDataFrameLL()
    {
    EUNIT_ASSERT_SPECIFIC_LEAVE( iJBufImpl->AddDataFrameL( NULL ),
                                 KErrNoMemory );

    CMMFDescriptorBuffer* buf = CMMFDescriptorBuffer::NewL( 10 );
    CleanupStack::PushL( buf );
    
    TDes8& bufData = buf->Data();
    _LIT( KText, "foo42" );
    bufData.Copy( KText );

    iJBufImpl->AddDataFrameL( buf );
    iJBufImpl->AddDataFrameL( buf );
    iJBufImpl->AddDataFrameL( buf );

    CleanupStack::PopAndDestroy( buf );
    }

void UT_CMccJitterBufferImpl::UT_CMccJitterBufferImpl_GetDataFrameLL()
    {
    EUNIT_ASSERT_SPECIFIC_LEAVE( iJBufImpl->GetDataFrameL( NULL ),
                                 KErrNoMemory );

    // Need a big buffer for the SID packet in WINSCW
    CMMFDescriptorBuffer* buf = CMMFDescriptorBuffer::NewL( 1250 );
    CleanupStack::PushL( buf );
    
    iJBufImpl->GetDataFrameL( buf );

    buf->ReAllocBufferL( 1250 );

    // This time, the inactivity timeout should be "passed"
    iJBufImpl->GetDataFrameL( buf );
    buf->ReAllocBufferL( 1250 );

    // Now put some real stuff in the buffer
    CMMFDescriptorBuffer* buf2 = CMMFDescriptorBuffer::NewL( 10 );
    CleanupStack::PushL( buf2 );
    
    TDes8& bufData = buf2->Data();
    _LIT( KText, "foo42" );
    bufData.Copy( KText );
    iJBufImpl->AddDataFrameL( buf2 );

    // Reach the iPacketsInBuffer < iCurrentPlayThreshold branch
    iJBufImpl->GetDataFrameL( buf );
    buf->ReAllocBufferL( 1250 );

    // Add enough data to be played
    iJBufImpl->AddDataFrameL( buf2 );
    iJBufImpl->AddDataFrameL( buf2 );
    iJBufImpl->AddDataFrameL( buf2 );
    iJBufImpl->AddDataFrameL( buf2 );
    iJBufImpl->AddDataFrameL( buf2 );
    iJBufImpl->AddDataFrameL( buf2 );

    iJBufImpl->GetDataFrameL( buf );

    CleanupStack::PopAndDestroy( buf2 );
    CleanupStack::PopAndDestroy( buf );
    }

void UT_CMccJitterBufferImpl::UT_CMccJitterBufferImpl_BufferLengthL()
    {
    EUNIT_ASSERT_EQUALS( iJBufImpl->BufferLength(), 200 );
    }

void UT_CMccJitterBufferImpl::UT_CMccJitterBufferImpl_GenerateStatisticsL()
    {
    // Put a packet in the buffer
    CMMFDescriptorBuffer* buf = CMMFDescriptorBuffer::NewL( 10 );
    CleanupStack::PushL( buf );
    
    TDes8& bufData = buf->Data();
    _LIT( KText, "foo42" );
    bufData.Copy( KText );
    iJBufImpl->AddDataFrameL( buf );
    
    TMccJitterBufferEventData eventData;

    iJBufImpl->GenerateStatistics( eventData );
    EUNIT_ASSERT_EQUALS( eventData.iBufferLength, 200 );
    EUNIT_ASSERT_EQUALS( eventData.iFramesInBuffer, 1 );

    CleanupStack::PopAndDestroy( buf ); 
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMccJitterBufferImpl,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "SetupL - 1 test failure",
    "CMccJitterBufferImpl",
    "SetupL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccJitterBufferImpl_SetupLL, Teardown )

EUNIT_TEST(
    "SetupL - 2 test amrnb",
    "CMccJitterBufferImpl",
    "SetupL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccJitterBufferImpl_SetupL2L, Teardown )

EUNIT_TEST(
    "SetupL - 3 test g711",
    "CMccJitterBufferImpl",
    "SetupL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccJitterBufferImpl_SetupL3L, Teardown )
    
EUNIT_TEST(
    "SetupL - 4 test ilbc",
    "CMccJitterBufferImpl",
    "SetupL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccJitterBufferImpl_SetupL4L, Teardown )
    
EUNIT_TEST(
    "SetupL - 5 test g729",
    "CMccJitterBufferImpl",
    "SetupL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccJitterBufferImpl_SetupL5L, Teardown )
    
 EUNIT_TEST(
    "SetupL - 6 test amrwb",
    "CMccJitterBufferImpl",
    "SetupL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccJitterBufferImpl_SetupL6L, Teardown )
    
EUNIT_TEST(
    "ResetBuffer - test ",
    "CMccJitterBufferImpl",
    "ResetBuffer",
    "FUNCTIONALITY",
    SetupL, UT_CMccJitterBufferImpl_ResetBufferL, Teardown )
    
EUNIT_TEST(
    "CurrentCodec - test ",
    "CMccJitterBufferImpl",
    "CurrentCodec",
    "FUNCTIONALITY",
    SetupL, UT_CMccJitterBufferImpl_CurrentCodecL, Teardown )

EUNIT_TEST(
    "DelayUpL - test ",
    "CMccJitterBufferImpl",
    "DelayUpL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccJitterBufferImpl_DelayUpLL, Teardown )

EUNIT_TEST(
    "DelayDownL - test ",
    "CMccJitterBufferImpl",
    "DelayDownL",
    "FUNCTIONALITY",
    SetupL, UT_CMccJitterBufferImpl_DelayDownLL, Teardown )

EUNIT_TEST(
    "CalculateDelay - test ",
    "CMccJitterBufferImpl",
    "CalculateDelay",
    "FUNCTIONALITY",
    SetupL, UT_CMccJitterBufferImpl_CalculateDelayL, Teardown )

EUNIT_TEST(
    "AddDataFrameL - test ",
    "CMccJitterBufferImpl",
    "AddDataFrameL",
    "FUNCTIONALITY",
    SetupL, UT_CMccJitterBufferImpl_AddDataFrameLL, Teardown )

EUNIT_TEST(
    "GetDataFrameL - test ",
    "CMccJitterBufferImpl",
    "GetDataFrameL",
    "FUNCTIONALITY",
    SetupL, UT_CMccJitterBufferImpl_GetDataFrameLL, Teardown )

EUNIT_TEST(
    "BufferLength - test ",
    "CMccJitterBufferImpl",
    "BufferLength",
    "FUNCTIONALITY",
    SetupL, UT_CMccJitterBufferImpl_BufferLengthL, Teardown )

EUNIT_TEST(
    "GenerateStatistics - test ",
    "CMccJitterBufferImpl",
    "GenerateStatistics",
    "FUNCTIONALITY",
    SetupL, UT_CMccJitterBufferImpl_GenerateStatisticsL, Teardown )

EUNIT_END_TEST_TABLE

//  END OF FILE
