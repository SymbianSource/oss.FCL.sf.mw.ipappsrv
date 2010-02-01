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
#include "UT_CG711PayloadFormatWrite.h"

//  EXTERNAL INCLUDES
#include <e32base.h>
#include <digia/eunit/eunitmacros.h>

#ifdef EUNIT_ALLOC_TEST_ON
    #include <digia/eunit/CEunitAllocTestCaseDecorator.h>   //for using EUNIT_ALLOC_TEST
#endif

//  INTERNAL INCLUDES
#include "G711PayloadFormatWrite.h"
#include "MccRtpDataSink.h"
#include <mmf/server/mmfdatasink.h>
#include <mmf/server/mmfdatasource.h>
#include "Mcculdatapath.h"
#include "mccuids.hrh"
#include "mccrtpmediaclock.h"
#include "mccresourcepool.h"
#include "mccredpayloadwrite.h" 


const TUid KRtpDataSink              =  { KImplUidRtpDataSink               };
const TUid KRtpDataSource            =  { KImplUidRtpDataSource             };


// CONSTRUCTION
UT_CG711PayloadFormatWrite* UT_CG711PayloadFormatWrite::NewL()
    {
    UT_CG711PayloadFormatWrite* self = UT_CG711PayloadFormatWrite::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CG711PayloadFormatWrite* UT_CG711PayloadFormatWrite::NewLC()
    {
    UT_CG711PayloadFormatWrite* self = new( ELeave ) UT_CG711PayloadFormatWrite();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CG711PayloadFormatWrite::~UT_CG711PayloadFormatWrite()
    {
    }

// Default constructor
UT_CG711PayloadFormatWrite::UT_CG711PayloadFormatWrite()
    {
    }

// Second phase construct
void UT_CG711PayloadFormatWrite::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CG711PayloadFormatWrite::SetupAL(  )
    {
    iAlloc = ETrue;
    
    MAsyncEventHandler* eventHandler = NULL;
    CMccResourcePool* mccresource = NULL;
    iRtpMediaClock = CMccRtpMediaClock::NewL();
     
    iUlSink = CMccUlDataPath::NewL( eventHandler, mccresource, iMediaId ); //MDataSink::NewSinkL( KRtpDataSink, KNullDesC8 );
    iRtpDataSinkStub = static_cast<CMccRtpDataSink*>(MDataSink::NewSinkL( KRtpDataSink, KNullDesC8 ));  
    iWrite = CG711PayloadFormatWrite::NewL( iRtpDataSinkStub );
    
    iCodec.iAlgoUsed = 0;
    iCodec.iBitrate = 15200;
    iCodec.iCNFrameSize = 0;
    iCodec.iCodecMode = 3;
    iCodec.iEnableDTX = 1;
    iCodec.iFourCC = 0x31313747;
    iCodec.iFrameSize = 0;
    iCodec.iHwFrameTime = 20;
    iCodec.iMaxPtime = 200;
    iCodec.iPayloadType = 8;
    iCodec.iPtime = 20;
    iCodec.iRedundancyCount = 1;
    iCodec.iRedundantPayload = 99;
  
    TMccCodecInfoBuffer buffer( iCodec );
    
    iWrite->ConfigurePayloadFormatL( buffer, *iRtpMediaClock );
    
    iBuff = CMMFDescriptorBuffer::NewL( 10 );

    TMediaId mediaId( KUidMediaTypeAudio, 1 );
  
    // Datapath calls CreateSinkBufferL
    TBool bufferReference;
    iBuffer = iWrite->CreateSinkBufferL( mediaId, bufferReference );
    
    iWrite->SinkPrimeL();
    iWrite->SinkPlayL( );
    }

void UT_CG711PayloadFormatWrite::Setup2AL(  )
    {
    // not call CreateSinkBufferL
    
    iAlloc = ETrue;
    
    MAsyncEventHandler* eventHandler = NULL;
    CMccResourcePool* mccresource = NULL;
    iRtpMediaClock = CMccRtpMediaClock::NewL();
     
    iUlSink = CMccUlDataPath::NewL( eventHandler, mccresource, iMediaId ); //MDataSink::NewSinkL( KRtpDataSink, KNullDesC8 );
    iRtpDataSinkStub = static_cast<CMccRtpDataSink*>(MDataSink::NewSinkL( KRtpDataSink, KNullDesC8 ));  
    iWrite = CG711PayloadFormatWrite::NewL( iRtpDataSinkStub );
    
    iCodec.iAlgoUsed = 0;
    iCodec.iBitrate = 15200;
    iCodec.iCNFrameSize = 0;
    iCodec.iCodecMode = 3;
    iCodec.iEnableDTX = 1;
    iCodec.iFourCC = 0x31313747;
    iCodec.iFrameSize = 0;
    iCodec.iHwFrameTime = 20;
    iCodec.iMaxPtime = 200;
    iCodec.iPayloadType = 8;
    iCodec.iPtime = 20;
    iCodec.iRedundancyCount = 1;
    iCodec.iRedundantPayload = 99;
  
    TMccCodecInfoBuffer buffer( iCodec );
    
    iWrite->ConfigurePayloadFormatL( buffer, *iRtpMediaClock );
    
    iBuff = CMMFDescriptorBuffer::NewL( 10 );

    TMediaId mediaId( KUidMediaTypeAudio, 1 );
      
    iWrite->SinkPrimeL();
    iWrite->SinkPlayL( );
    }

void UT_CG711PayloadFormatWrite::SetupL(  )
    {  
    iAlloc = EFalse;
    
    MAsyncEventHandler* eventHandler = NULL;
    CMccResourcePool* mccresource = NULL;
    iRtpMediaClock = CMccRtpMediaClock::NewL();
     
    iUlSink = CMccUlDataPath::NewL( eventHandler, mccresource, iMediaId ); //MDataSink::NewSinkL( KRtpDataSink, KNullDesC8 );
    iRtpDataSinkStub = static_cast<CMccRtpDataSink*>(MDataSink::NewSinkL( KRtpDataSink, KNullDesC8 ));  
    iWrite = CG711PayloadFormatWrite::NewL( iRtpDataSinkStub );
    
    iCodec.iAlgoUsed = 0;
    iCodec.iBitrate = 15200;
    iCodec.iCNFrameSize = 0;
    iCodec.iCodecMode = 3;
    iCodec.iEnableDTX = 1;
    iCodec.iFourCC = 0x31313747;
    iCodec.iFrameSize = 0;
    iCodec.iHwFrameTime = 20;
    iCodec.iMaxPtime = 200;
    iCodec.iPayloadType = 8;
    iCodec.iPtime = 20;
    iCodec.iRedundancyCount = 1;
    iCodec.iRedundantPayload = 99;
  
    TMccCodecInfoBuffer buffer( iCodec );
    
    iWrite->ConfigurePayloadFormatL( buffer, *iRtpMediaClock );
    
    iBuff = CMMFDescriptorBuffer::NewL( 10 );

    TMediaId mediaId( KUidMediaTypeAudio, 1 );
  
    // Datapath calls CreateSinkBufferL
    TBool bufferReference;
    iBuffer = iWrite->CreateSinkBufferL( mediaId, bufferReference );

    iWrite->SinkPrimeL();
    iWrite->SinkPlayL( );
    }

void UT_CG711PayloadFormatWrite::Teardown()
    {
    delete iUlSink;
    delete iRtpDataSinkStub;
    delete iWrite;
    delete iBuff;
    delete iRtpMediaClock;
    
    REComSession::FinalClose();
    }

void UT_CG711PayloadFormatWrite::UT_CG711PayloadFormatWrite_NewLL(  )
    {
    CG711PayloadFormatWrite* write = CG711PayloadFormatWrite::NewL( iRtpDataSinkStub );
    delete write;
    EUNIT_ASSERT_SPECIFIC_LEAVE( CG711PayloadFormatWrite* write2 = CG711PayloadFormatWrite::NewL( NULL ), KErrArgument );
    }

void UT_CG711PayloadFormatWrite::UT_CG711PayloadFormatWrite_EmptySourceBufferLL(  )
    {
    EUNIT_ASSERT_NO_LEAVE( iWrite->EmptySourceBufferL() );    
    CMMFDataBuffer* buffer = iWrite->CreateSinkBufferOfSizeL(10);
    EUNIT_ASSERT_NO_LEAVE( iWrite->EmptySourceBufferL() );
    }

void UT_CG711PayloadFormatWrite::UT_CG711PayloadFormatWrite_SourceBufferEmptiedLL(  )
    {
    EUNIT_ASSERT_NO_LEAVE( iWrite->EmptyBufferL( iBuffer, iUlSink, KUidMediaTypeAudio ) );
    EUNIT_ASSERT_NO_LEAVE( iWrite->SourceBufferEmptiedL( ) );
    }

void UT_CG711PayloadFormatWrite::UT_CG711PayloadFormatWrite_SinkPrimeLL(  )
    {
    EUNIT_ASSERT_NO_LEAVE( iWrite->SinkPrimeL() );
    }

void UT_CG711PayloadFormatWrite::UT_CG711PayloadFormatWrite_SinkPlayLL(  )
    {
    iWrite->SinkPlayL();
    }

void UT_CG711PayloadFormatWrite::UT_CG711PayloadFormatWrite_SinkPauseLL(  )
    {
    if ( !iAlloc )
        {
        EUNIT_ASSERT_NO_LEAVE( iWrite->SinkPauseL() );
        
        }
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iWrite->SinkPauseL() );
        
        }
    }

void UT_CG711PayloadFormatWrite::UT_CG711PayloadFormatWrite_SinkStopLL(  )
    {
    EUNIT_ASSERT_NO_LEAVE( iWrite->SinkStopL() );
    }

void UT_CG711PayloadFormatWrite::UT_CG711PayloadFormatWrite_FrameTimeIntervalL(  )
    {
    //iWrite->iFrameTimeInterval = 20000 *  2;  // 20k * Channels
    TTimeIntervalMicroSeconds catchAfish;
    TMediaId mediaIdAudio( KUidMediaTypeAudio, 1 );
    TMediaId mediaIdVideo( KUidMediaTypeVideo, 1 );
    
    if ( !iAlloc )
        {
        catchAfish = iWrite->FrameTimeInterval ( mediaIdAudio );
        EUNIT_ASSERT_EQUALS( catchAfish.Int64(), 0 );
        
        catchAfish = iWrite->FrameTimeInterval ( mediaIdVideo );
        EUNIT_ASSERT_EQUALS( catchAfish.Int64(), 0 );
        
        
        }
        
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iWrite->FrameTimeInterval ( mediaIdAudio ) );
        EUNIT_ASSERT_NO_LEAVE( iWrite->FrameTimeInterval ( mediaIdVideo ) );
        
        }
    }

void UT_CG711PayloadFormatWrite::UT_CG711PayloadFormatWrite_DurationL(  )
    {
    TTimeIntervalMicroSeconds catchAfish;
    TMediaId mediaId( KUidMediaTypeAudio, 1 );
    
    if ( !iAlloc )
        {
        catchAfish = iWrite->Duration( mediaId );
        EUNIT_ASSERT_EQUALS( catchAfish.Int64(), 0 );
        
        }
    
    else 
        {
        EUNIT_ASSERT_NO_LEAVE( iWrite->Duration( mediaId ) );
        
        }
    }

void UT_CG711PayloadFormatWrite::UT_CG711PayloadFormatWrite_CreateSinkBufferLL(  )
    {
    TMediaId mediaId( KUidMediaTypeAudio, 1 );
    TMediaId mediaIdVideo( KUidMediaTypeVideo, 1 );
    TBool reference;
    
    if ( !iAlloc )
        {
        EUNIT_ASSERT_NO_LEAVE( iBuf = iWrite->CreateSinkBufferL( mediaId, reference ) );
        EUNIT_ASSERT_SPECIFIC_LEAVE( iBuf = iWrite->CreateSinkBufferL( mediaIdVideo, reference ), KErrNotSupported );
        
        }
    
    else
        {
        EUNIT_ASSERT_SPECIFIC_LEAVE( iWrite->CreateSinkBufferL( mediaId, reference ), KErrNoMemory );
        EUNIT_ASSERT_SPECIFIC_LEAVE( iWrite->CreateSinkBufferL( mediaIdVideo, reference ), KErrNotSupported );
        
        }
    }

void UT_CG711PayloadFormatWrite::UT_CG711PayloadFormatWrite_SinkDataTypeCodeL(  )
    {
    TMediaId mediaId( KUidMediaTypeAudio, 1 );
    TMediaId mediaIdVideo( KUidMediaTypeVideo, 1 );
    
    if ( !iAlloc )
        {
        EUNIT_ASSERT_NO_LEAVE( iWrite->SinkDataTypeCode( mediaIdVideo ) );
        EUNIT_ASSERT_NO_LEAVE( iWrite->SinkDataTypeCode( mediaId ) );
        
        
        }
    
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iWrite->SinkDataTypeCode( mediaIdVideo ) );
        EUNIT_ASSERT_NO_LEAVE( iWrite->SinkDataTypeCode( mediaId ) );
        
        
        }
    }

void UT_CG711PayloadFormatWrite::UT_CG711PayloadFormatWrite_SetSinkDataTypeCodeL(  )
    {
    TFourCC fourcc;
    TMediaId mediaId( KUidMediaTypeAudio, 1 );
    TMediaId mediaIdVideo( KUidMediaTypeVideo, 1 );
    
    if ( !iAlloc )
        {
        fourcc = iWrite->SinkDataTypeCode( mediaId );
        EUNIT_ASSERT_EQUALS( iWrite->SetSinkDataTypeCode( fourcc, mediaId ), KErrNone );
        EUNIT_ASSERT_EQUALS( iWrite->SetSinkDataTypeCode( fourcc, mediaIdVideo ), KErrNotSupported );
        fourcc = iWrite->SinkDataTypeCode( mediaIdVideo );
        EUNIT_ASSERT_EQUALS( iWrite->SetSinkDataTypeCode( fourcc, mediaId ), KErrNone );
        EUNIT_ASSERT_EQUALS( iWrite->SetSinkDataTypeCode( fourcc, mediaIdVideo ), KErrNotSupported );
        
        
        }
    
    else
        {
        fourcc = iWrite->SinkDataTypeCode( mediaIdVideo );
        EUNIT_ASSERT_EQUALS( iWrite->SetSinkDataTypeCode( fourcc, mediaIdVideo ), KErrNotSupported );
        EUNIT_ASSERT_EQUALS( iWrite->SetSinkDataTypeCode( fourcc, mediaId ), KErrNone );
        fourcc = iWrite->SinkDataTypeCode( mediaId  );
        EUNIT_ASSERT_EQUALS( iWrite->SetSinkDataTypeCode( fourcc, mediaId ), KErrNone );
        EUNIT_ASSERT_EQUALS( iWrite->SetSinkDataTypeCode( fourcc, mediaIdVideo ), KErrNotSupported );
        
        
        }
    }

void UT_CG711PayloadFormatWrite::UT_CG711PayloadFormatWrite_SinkThreadLogonL(  )
    {
    if ( !iAlloc )
        {
        EUNIT_ASSERT_EQUALS( iWrite->SinkThreadLogon( *this ), KErrNone );
        
        }
    
    else
        {
        EUNIT_ASSERT_EQUALS( iWrite->SinkThreadLogon( *this ), KErrNone );
        
        }
    }

void UT_CG711PayloadFormatWrite::UT_CG711PayloadFormatWrite_SinkThreadLogoffL(  )
    {
    if ( !iAlloc )
        {
        EUNIT_ASSERT_NO_LEAVE( iWrite->SinkThreadLogoff() );
        
        }
    
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iWrite->SinkThreadLogoff() );
        
        }
    }

void UT_CG711PayloadFormatWrite::UT_CG711PayloadFormatWrite_EmptyBufferLL(  )
    {
    TMediaId mediaId( KUidMediaTypeAudio, 1 );
    TMediaId mediaIdVideo( KUidMediaTypeVideo, 1 );
    
    if ( !iAlloc )
        {
        EUNIT_ASSERT_NO_LEAVE( iWrite->EmptyBufferL( iBuffer, iUlSink, mediaId ) );
        EUNIT_ASSERT_SPECIFIC_LEAVE( iWrite->EmptyBufferL( iBuffer, iUlSink, mediaIdVideo ), KErrNotSupported );
        
        }
    
    else 
        {
        EUNIT_ASSERT_NO_LEAVE( iWrite->EmptyBufferL( iBuffer, iUlSink, mediaId ) );
        EUNIT_ASSERT_SPECIFIC_LEAVE( iWrite->EmptyBufferL( iBuffer, iUlSink, mediaIdVideo ), KErrNotSupported );
        
        }
    }

void UT_CG711PayloadFormatWrite::UT_CG711PayloadFormatWrite_BufferEmptiedLL(  )
    {
    if ( !iAlloc )
        {
        EUNIT_ASSERT_NO_LEAVE( iWrite->BufferEmptiedL( iBuffer ) );
        }
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iWrite->BufferEmptiedL( iBuffer ) );
        }
    }

void UT_CG711PayloadFormatWrite::UT_CG711PayloadFormatWrite_NumChannelsL(  )
    {
    if ( !iAlloc )
        {
        EUNIT_ASSERT_EQUALS( iWrite->NumChannels( ), KMono );
        
        }
    
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iWrite->NumChannels( ) );
        
        }
    }

void UT_CG711PayloadFormatWrite::UT_CG711PayloadFormatWrite_SampleRateL(  )
    {
    TUint samplerate;
    
    if ( !iAlloc )
        {
        samplerate = iWrite->SampleRate( );
        EUNIT_ASSERT_EQUALS( samplerate, 8000 );
        
        }
    
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iWrite->SampleRate( ) );
        
        }
    }

void UT_CG711PayloadFormatWrite::UT_CG711PayloadFormatWrite_SetSampleRateL(  )
    {
    TInt rate( 8000 );
    if ( !iAlloc )
        {
        EUNIT_ASSERT( iWrite->SetSampleRate( rate ) == KErrNone );
        EUNIT_ASSERT( iWrite->SetSampleRate( rate + 1 ) == KErrNotSupported );
        
        }
    
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iWrite->SetSampleRate( rate ) );
        EUNIT_ASSERT_NO_LEAVE( iWrite->SetSampleRate( rate + 1 ) );
        
        }
    }

void UT_CG711PayloadFormatWrite::UT_CG711PayloadFormatWrite_ConfigurePayloadFormatLL(  )
    {
    if ( iAlloc )
        {
        EUNIT_ASSERT( ETrue );
        }
    else
        {
        TMccCodecInfo codec;
        codec.iAlgoUsed         = ENoAlgoUsed;
        codec.iBitrate          = 6400;
        codec.iCNFrameSize      = 0;
        codec.iCodecMode        = 3;
        codec.iEnableDTX        = 1;
        codec.iFourCC           = 31313747;
        codec.iFrameSize        = 0;
        codec.iHwFrameTime      = 20;
        codec.iMaxPtime         = 200;
        codec.iPayloadType      = 8;
        codec.iPtime            = 20;
        codec.iRedundancyCount  = 0;
        codec.iRedundantPayload = 0;
        codec.iComfortNoiseGenerationPt = KPayloadTypeUndefined;
        
        TMccCodecInfoBuffer buffer( codec );
        EUNIT_ASSERT_NO_LEAVE( iWrite->ConfigurePayloadFormatL( buffer, *iRtpMediaClock ) );
        
        // test updating of values
        codec.iIsUpdate         = ETrue;
        codec.iPtime            = 40;
        codec.iPayloadType      = 0;
        codec.iComfortNoiseGenerationPt = 13;
        buffer = codec;
        iWrite->ConfigurePayloadFormatL( buffer, *iRtpMediaClock );
        EUNIT_ASSERT( iWrite->iCInfo.iPtime == 40 );
        EUNIT_ASSERT( iWrite->iCInfo.iPayloadType == 0 );
        EUNIT_ASSERT( iWrite->iCInfo.iComfortNoiseGenerationPt == 13 );
        
        // test redundancy payload format usage
        codec.iIsUpdate         = EFalse;
        codec.iAlgoUsed         = EGenRedUsed;
        codec.iRedundancyCount  = 1;
        buffer = codec;         
        CMccRtpDataSink* rtpDataSinkStub = static_cast<CMccRtpDataSink*>(MDataSink::NewSinkL( KRtpDataSink, KNullDesC8 ));
        CleanupStack::PushL( rtpDataSinkStub );
        CMccRedPayloadWrite* redEncoder = CMccRedPayloadWrite::NewL( rtpDataSinkStub );
        CleanupStack::PushL( redEncoder );
        CG711PayloadFormatWrite* write = CG711PayloadFormatWrite::NewL( redEncoder );
        CleanupStack::PushL( write );
        EUNIT_ASSERT_NO_LEAVE( write->ConfigurePayloadFormatL( buffer, *iRtpMediaClock ) );
        CleanupStack::PopAndDestroy( 3 ); 
        }
    }
    
void UT_CG711PayloadFormatWrite::UT_CG711PayloadFormatWrite_UpdateConfigurationLL(  )
    {
    
    if ( !iAlloc)
        {   
        TMccCodecInfo codec;

        codec.iAlgoUsed = 0;
        codec.iBitrate = 6400;
        codec.iCNFrameSize = 0;
        codec.iCodecMode = 3;
        codec.iEnableDTX = 1;
        codec.iFourCC = 31313747;
        codec.iFrameSize = 0;
        codec.iHwFrameTime = 30;
        codec.iMaxPtime = 300;
        codec.iPayloadType = 8;
        codec.iPtime = 20;
        codec.iRedundancyCount = 0;
        codec.iRedundantPayload = 0;        
        
        CG711PayloadFormatWrite* write = CG711PayloadFormatWrite::NewL( iRtpDataSinkStub );
        CleanupStack::PushL( write );
        write->UpdateConfigurationL( codec );
        CleanupStack::PopAndDestroy( write );
        }
    else
        {
        EUNIT_ASSERT( ETrue );
        }

    }    


void UT_CG711PayloadFormatWrite::UT_CG711PayloadFormatWrite_DeliverPacketL(  )
    {
    iBuff->Data().FillZ( 10 );        
    EUNIT_ASSERT_NO_LEAVE( iWrite->DeliverPacketL( *iBuff ) );
    
    iBuff->Data().FillZ( 10 );
    iWrite->iIsRtpSink = EFalse;
    EUNIT_ASSERT_NO_LEAVE( iWrite->DeliverPacketL( *iBuff ) );
    
    iBuff->Data().FillZ( 10 );
    iWrite->iCNModeON = ETrue;
    iWrite->iCInfo.iComfortNoiseGenerationPt = KComfortNoisePT;
    EUNIT_ASSERT_NO_LEAVE( iWrite->DeliverPacketL( *iBuff ) );
    
    iBuff->Data().FillZ( 10 );
    iWrite->iIsRtpSink = ETrue;
    EUNIT_ASSERT_NO_LEAVE( iWrite->DeliverPacketL( *iBuff ) );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CG711PayloadFormatWrite,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CG711PayloadFormatWrite",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CG711PayloadFormatWrite_NewLL, Teardown)

EUNIT_TEST(
    "EmptySourceBufferL - test ", 
    "CG711PayloadFormatWrite",
    "EmptySourceBufferL",
    "FUNCTIONALITY",
   SetupL, UT_CG711PayloadFormatWrite_EmptySourceBufferLL, Teardown)



EUNIT_TEST(
    "SourceBufferEmptiedL - test ",
    "CG711PayloadFormatWrite",
    "SourceBufferEmptiedL",
    "FUNCTIONALITY",
    SetupL, UT_CG711PayloadFormatWrite_SourceBufferEmptiedLL, Teardown)

EUNIT_TEST(
    "SinkPrimeL - test ",
    "CG711PayloadFormatWrite",
    "SinkPrimeL",
    "FUNCTIONALITY",
    SetupL, UT_CG711PayloadFormatWrite_SinkPrimeLL, Teardown)

EUNIT_TEST(
    "SinkPlayL - test ",
    "CG711PayloadFormatWrite",
    "SinkPlayL",
    "FUNCTIONALITY",
    SetupL, UT_CG711PayloadFormatWrite_SinkPlayLL, Teardown)

EUNIT_TEST(
    "SinkPauseL - test ",
    "CG711PayloadFormatWrite",
    "SinkPauseL",
    "FUNCTIONALITY",
    SetupL, UT_CG711PayloadFormatWrite_SinkPauseLL, Teardown)

EUNIT_TEST(
    "SinkStopL - test ",
    "CG711PayloadFormatWrite",
    "SinkStopL",
    "FUNCTIONALITY",
    SetupL, UT_CG711PayloadFormatWrite_SinkStopLL, Teardown)

EUNIT_TEST(
    "FrameTimeInterval - test ",
    "CG711PayloadFormatWrite",
    "FrameTimeInterval",
    "FUNCTIONALITY",
    SetupL, UT_CG711PayloadFormatWrite_FrameTimeIntervalL, Teardown)

EUNIT_TEST(
    "Duration - test ",
    "CG711PayloadFormatWrite",
    "Duration",
    "FUNCTIONALITY",
    SetupL, UT_CG711PayloadFormatWrite_DurationL, Teardown)

EUNIT_TEST(
    "CreateSinkBufferL - test ",
    "CG711PayloadFormatWrite",
    "CreateSinkBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CG711PayloadFormatWrite_CreateSinkBufferLL, Teardown)

EUNIT_TEST(
    "SinkDataTypeCode - test ",
    "CG711PayloadFormatWrite",
    "SinkDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CG711PayloadFormatWrite_SinkDataTypeCodeL, Teardown)

EUNIT_TEST(
    "SetSinkDataTypeCode - test ",
    "CG711PayloadFormatWrite",
    "SetSinkDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CG711PayloadFormatWrite_SetSinkDataTypeCodeL, Teardown)

EUNIT_TEST(
    "SinkThreadLogon - test ",
    "CG711PayloadFormatWrite",
    "SinkThreadLogon",
    "FUNCTIONALITY",
    SetupL, UT_CG711PayloadFormatWrite_SinkThreadLogonL, Teardown)


EUNIT_TEST(
    "SinkThreadLogoff - test ",
    "CG711PayloadFormatWrite",
    "SinkThreadLogoff",
    "FUNCTIONALITY",
    SetupL, UT_CG711PayloadFormatWrite_SinkThreadLogoffL, Teardown)

EUNIT_TEST(
    "EmptyBufferL - test ",
    "CG711PayloadFormatWrite",
    "EmptyBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CG711PayloadFormatWrite_EmptyBufferLL, Teardown)


EUNIT_TEST(
    "NumChannels - test ",
    "CG711PayloadFormatWrite",
    "NumChannels",
    "FUNCTIONALITY",
    SetupL, UT_CG711PayloadFormatWrite_NumChannelsL, Teardown)

EUNIT_TEST(
    "SampleRate - test ",
    "CG711PayloadFormatWrite",
    "SampleRate",
    "FUNCTIONALITY",
    SetupL, UT_CG711PayloadFormatWrite_SampleRateL, Teardown)


EUNIT_TEST(
    "SetSampleRate - test ",
    "CG711PayloadFormatWrite",
    "SetSampleRate",
    "FUNCTIONALITY",
    SetupL, UT_CG711PayloadFormatWrite_SetSampleRateL, Teardown)

EUNIT_TEST(
    "ConfigurePayloadFormatL - test ",
    "CG711PayloadFormatWrite",
    "ConfigurePayloadFormatL",
    "FUNCTIONALITY",
    SetupL, UT_CG711PayloadFormatWrite_ConfigurePayloadFormatLL, Teardown)
    
EUNIT_TEST(
    "UpdateConfigurationL - test ",
    "CG711PayloadFormatWrite",
    "UpdateConfigurationL",
    "FUNCTIONALITY",
    SetupL, UT_CG711PayloadFormatWrite_UpdateConfigurationLL, Teardown)  

EUNIT_TEST(
    "DeliverPacket - test ",
    "CG711PayloadFormatWrite",
    "DeliverPacket",
    "FUNCTIONALITY",
    SetupL, UT_CG711PayloadFormatWrite_DeliverPacketL, Teardown)


// ALLOC TESTS

#ifdef EUNIT_ALLOC_TEST_ON

EUNIT_TEST(
    "NewL - test ",
    "CG711PayloadFormatWrite",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CG711PayloadFormatWrite_NewLL, Teardown)

EUNIT_ALLOC_TEST(
   "EmptySourceBufferL - alloc test ",
   "CG711PayloadFormatWrite",
   "EmptySourceBufferL",
   "ERRORHANDLING",
   SetupAL, UT_CG711PayloadFormatWrite_EmptySourceBufferLL, Teardown)

EUNIT_ALLOC_TEST(
    "SourceBufferEmptiedL - alloc test ",
    "CG711PayloadFormatWrite",
    "SourceBufferEmptiedL",
    "ERRORHANDLING",
    SetupAL, UT_CG711PayloadFormatWrite_SourceBufferEmptiedLL, Teardown)

EUNIT_ALLOC_TEST(
    "SinkPrimeL - alloc test ",
    "CG711PayloadFormatWrite",
    "SinkPrimeL",
    "ERRORHANDLING",
    SetupAL, UT_CG711PayloadFormatWrite_SinkPrimeLL, Teardown)

EUNIT_ALLOC_TEST(
    "SinkPlayL - alloc test ",
    "CG711PayloadFormatWrite",
    "SinkPlayL",
    "ERRORHANDLING",
    SetupAL, UT_CG711PayloadFormatWrite_SinkPlayLL, Teardown)

EUNIT_ALLOC_TEST(
    "SinkPauseL - alloc test ",
    "CG711PayloadFormatWrite",
    "SinkPauseL",
    "ERRORHANDLING",
    SetupAL, UT_CG711PayloadFormatWrite_SinkPauseLL, Teardown)

EUNIT_ALLOC_TEST(
    "SinkStopL - alloc test ",
    "CG711PayloadFormatWrite",
    "SinkStopL",
    "ERRORHANDLING",
    SetupAL, UT_CG711PayloadFormatWrite_SinkStopLL, Teardown)

EUNIT_ALLOC_TEST(
    "FrameTimeInterval - alloc test ",
    "CG711PayloadFormatWrite",
    "FrameTimeInterval",
    "ERRORHANDLING",
    SetupAL, UT_CG711PayloadFormatWrite_FrameTimeIntervalL, Teardown)

EUNIT_ALLOC_TEST(
    "Duration - alloc test ",
    "CG711PayloadFormatWrite",
    "Duration",
    "ERRORHANDLING",
    SetupAL, UT_CG711PayloadFormatWrite_DurationL, Teardown)

EUNIT_ALLOC_TEST(
    "CreateSinkBufferL - alloc test ",
    "CG711PayloadFormatWrite",
    "CreateSinkBufferL",
    "ERRORHANDLING",
    Setup2AL, UT_CG711PayloadFormatWrite_CreateSinkBufferLL, Teardown)

EUNIT_ALLOC_TEST(
    "SinkDataTypeCode - alloc test ",
    "CG711PayloadFormatWrite",
    "SinkDataTypeCode",
    "ERRORHANDLING",
    SetupAL, UT_CG711PayloadFormatWrite_SinkDataTypeCodeL, Teardown)

EUNIT_ALLOC_TEST(
    "SetSinkDataTypeCode - alloc test ",
    "CG711PayloadFormatWrite",
    "SetSinkDataTypeCode",
    "ERRORHANDLING",
    SetupAL, UT_CG711PayloadFormatWrite_SetSinkDataTypeCodeL, Teardown)

EUNIT_ALLOC_TEST(
    "SinkThreadLogon - alloc test ",
    "CG711PayloadFormatWrite",
    "SinkThreadLogon",
    "ERRORHANDLING",
    SetupAL, UT_CG711PayloadFormatWrite_SinkThreadLogonL, Teardown)


EUNIT_ALLOC_TEST(
    "SinkThreadLogoff - alloc test ",
    "CG711PayloadFormatWrite",
    "SinkThreadLogoff",
    "ERRORHANDLING",
    SetupAL, UT_CG711PayloadFormatWrite_SinkThreadLogoffL, Teardown)

EUNIT_ALLOC_TEST(
    "EmptyBufferL - alloc test ",
    "CG711PayloadFormatWrite",
    "EmptyBufferL",
    "ERRORHANDLING",
    SetupAL, UT_CG711PayloadFormatWrite_EmptyBufferLL, Teardown)


EUNIT_ALLOC_TEST(
    "BufferEmptiedL - Alloc test ",
    "CG711PayloadFormatWrite",
    "BufferEmptiedL",
    "ERRORHANDLING",
    SetupAL, UT_CG711PayloadFormatWrite_BufferEmptiedLL, Teardown)

EUNIT_ALLOC_TEST(
    "NumChannels - alloc test ",
    "CG711PayloadFormatWrite",
    "NumChannels",
    "ERRORHANDLING",
    SetupAL, UT_CG711PayloadFormatWrite_NumChannelsL, Teardown)

EUNIT_ALLOC_TEST(
    "SampleRate - alloc test ",
    "CG711PayloadFormatWrite",
    "SampleRate",
    "ERRORHANDLING",
    SetupAL, UT_CG711PayloadFormatWrite_SampleRateL, Teardown)


EUNIT_ALLOC_TEST(
    "SetSampleRate - alloc test ",
    "CG711PayloadFormatWrite",
    "SetSampleRate",
    "ERRORHANDLING",
    SetupAL, UT_CG711PayloadFormatWrite_SetSampleRateL, Teardown)
    
EUNIT_ALLOC_TEST(
    "ConfigurePayloadFormatL - alloc test ",
    "CG711PayloadFormatWrite",
    "ConfigurePayloadFormatL",
    "ERRORHANDLING",
    SetupAL, UT_CG711PayloadFormatWrite_ConfigurePayloadFormatLL, Teardown)
    
EUNIT_TEST(
    "UpdateConfigurationL - test ",
    "CG711PayloadFormatWrite",
    "UpdateConfigurationL",
    "ERRORHANDLING",
    SetupL, UT_CG711PayloadFormatWrite_UpdateConfigurationLL, Teardown)      


EUNIT_ALLOC_TEST(
    "DeliverPacket - alloc test ",
    "CG711PayloadFormatWrite",
    "DeliverPacket",
    "ERRORHANDLING",
    SetupL, UT_CG711PayloadFormatWrite_DeliverPacketL, Teardown)

#endif

EUNIT_END_TEST_TABLE

//  END OF FILE
