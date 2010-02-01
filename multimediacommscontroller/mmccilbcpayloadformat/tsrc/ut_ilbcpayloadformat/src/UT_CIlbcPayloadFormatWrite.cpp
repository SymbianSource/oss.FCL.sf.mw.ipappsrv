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
#include "UT_CIlbcPayloadFormatWrite.h"

//  EXTERNAL INCLUDES
#include <e32base.h>
#include <digia/eunit/eunitmacros.h>

#ifdef EUNIT_ALLOC_TEST_ON
    #include <digia/eunit/CEunitAllocTestCaseDecorator.h>   //for using EUNIT_ALLOC_TEST
#endif

//  INTERNAL INCLUDES
#include "IlbcPayloadFormatWrite.h"
#include <mmf/server/mmfdatasink.h>
#include <mmf/server/mmfdatasource.h>
#include "Mcculdatapath.h"
#include "mccuids.hrh"
#include "mccrtpmediaclock.h"
#include "mccresourcepool.h" 
#include "mccredpayloadwrite.h"
#include "mccrtpdatasink.h"

const TUid KRtpDataSink              =  { KImplUidRtpDataSink               };
const TUid KRtpDataSource            =  { KImplUidRtpDataSource             };


const TUint KiLBCSampleRate = 8000;

// CONSTRUCTION
UT_CIlbcPayloadFormatWrite* UT_CIlbcPayloadFormatWrite::NewL()
    {
    UT_CIlbcPayloadFormatWrite* self = UT_CIlbcPayloadFormatWrite::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CIlbcPayloadFormatWrite* UT_CIlbcPayloadFormatWrite::NewLC()
    {
    UT_CIlbcPayloadFormatWrite* self = new( ELeave ) UT_CIlbcPayloadFormatWrite();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CIlbcPayloadFormatWrite::~UT_CIlbcPayloadFormatWrite()
    {
    }

// Default constructor
UT_CIlbcPayloadFormatWrite::UT_CIlbcPayloadFormatWrite()
    {
    }

// Second phase construct
void UT_CIlbcPayloadFormatWrite::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS
void UT_CIlbcPayloadFormatWrite::SetupAL(  )
    {
    iAlloc = ETrue;
    
    MAsyncEventHandler* eventHandler = NULL;
    CMccResourcePool* mccresource = NULL;
    iRtpMediaClock = CMccRtpMediaClock::NewL();
     
    iUlSink = CMccUlDataPath::NewL( eventHandler, mccresource, iMediaId ); //MDataSink::NewSinkL( KRtpDataSink, KNullDesC8 );
    iRtpDataSinkStub = MDataSink::NewSinkL( KRtpDataSink, KNullDesC8 );  
    iWrite = CIlbcPayloadFormatWrite::NewL( iRtpDataSinkStub );
    
    iCodec.iAlgoUsed = ENoAlgoUsed;
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

void UT_CIlbcPayloadFormatWrite::Setup2AL(  )
    {
    // not call CreateSinkBufferL
    
    iAlloc = ETrue;
    
    MAsyncEventHandler* eventHandler = NULL;
    CMccResourcePool* mccresource = NULL;
    iRtpMediaClock = CMccRtpMediaClock::NewL();
     
    iUlSink = CMccUlDataPath::NewL( eventHandler, mccresource, iMediaId ); //MDataSink::NewSinkL( KRtpDataSink, KNullDesC8 );
    iRtpDataSinkStub = MDataSink::NewSinkL( KRtpDataSink, KNullDesC8 );  
    iWrite = CIlbcPayloadFormatWrite::NewL( iRtpDataSinkStub );
    
    iCodec.iAlgoUsed = ENoAlgoUsed;
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

void UT_CIlbcPayloadFormatWrite::SetupL(  )
    { 
    iAlloc = EFalse;
    
    MAsyncEventHandler* eventHandler = NULL;
    CMccResourcePool* mccresource = NULL;
    iRtpMediaClock = CMccRtpMediaClock::NewL();
     
    iUlSink = CMccUlDataPath::NewL( eventHandler, mccresource, iMediaId ); //MDataSink::NewSinkL( KRtpDataSink, KNullDesC8 );
    iRtpDataSinkStub = MDataSink::NewSinkL( KRtpDataSink, KNullDesC8 );  
    iWrite = CIlbcPayloadFormatWrite::NewL( iRtpDataSinkStub );
    
    iCodec.iAlgoUsed = ENoAlgoUsed;
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


void UT_CIlbcPayloadFormatWrite::Teardown()
    {
    delete iUlSink;
    delete iRtpDataSinkStub;
    delete iWrite;
    delete iBuff;
    delete iRtpMediaClock;
    
    REComSession::FinalClose();
    }

void UT_CIlbcPayloadFormatWrite::UT_CIlbcPayloadFormatWrite_NewLL(  )
    {
    CIlbcPayloadFormatWrite* write = CIlbcPayloadFormatWrite::NewL( iRtpDataSinkStub );
    delete write;
    EUNIT_ASSERT_SPECIFIC_LEAVE( CIlbcPayloadFormatWrite* write2 = CIlbcPayloadFormatWrite::NewL( NULL ), KErrArgument );
    }



void UT_CIlbcPayloadFormatWrite::UT_CIlbcPayloadFormatWrite_EmptySourceBufferLL(  )
    {
    TMediaId mediaId( KUidMediaTypeAudio, 1 );

    EUNIT_ASSERT_NO_LEAVE( iWrite->EmptyBufferL( iBuffer, iUlSink, mediaId ) );
    EUNIT_ASSERT_NO_LEAVE( iWrite->EmptySourceBufferL() );
    }

void UT_CIlbcPayloadFormatWrite::UT_CIlbcPayloadFormatWrite_SourceBufferEmptiedLL(  )
    {
    EUNIT_ASSERT_NO_LEAVE( iWrite->EmptyBufferL( iBuffer, iUlSink, KUidMediaTypeAudio ) );
    EUNIT_ASSERT_NO_LEAVE( iWrite->SourceBufferEmptiedL( ) );
    }

void UT_CIlbcPayloadFormatWrite::UT_CIlbcPayloadFormatWrite_SinkPrimeLL(  )
    {
    EUNIT_ASSERT_NO_LEAVE( iWrite->SinkPrimeL() );
    }

void UT_CIlbcPayloadFormatWrite::UT_CIlbcPayloadFormatWrite_SinkPlayLL(  )
    {
    iWrite->SinkPlayL();
    }

void UT_CIlbcPayloadFormatWrite::UT_CIlbcPayloadFormatWrite_SinkPauseLL(  )
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

void UT_CIlbcPayloadFormatWrite::UT_CIlbcPayloadFormatWrite_SinkStopLL(  )
    {
    EUNIT_ASSERT_NO_LEAVE( iWrite->SinkStopL() );
    }

void UT_CIlbcPayloadFormatWrite::UT_CIlbcPayloadFormatWrite_FrameTimeIntervalL(  )
    {
    iWrite->iFrameTimeInterval = 20000 *  2;  // 20k * Channels
    TTimeIntervalMicroSeconds catchAfish;
    TMediaId mediaIdAudio( KUidMediaTypeAudio, 1 );
    TMediaId mediaIdVideo( KUidMediaTypeVideo, 1 );
    
    if ( !iAlloc )
        {
        catchAfish = iWrite->FrameTimeInterval ( mediaIdAudio );
        EUNIT_ASSERT_EQUALS( catchAfish.Int64(), 40000 );
        
        catchAfish = iWrite->FrameTimeInterval ( mediaIdVideo );
        EUNIT_ASSERT_EQUALS( catchAfish.Int64(), 0 );
        }
        
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iWrite->FrameTimeInterval ( mediaIdAudio ) );
        EUNIT_ASSERT_NO_LEAVE( iWrite->FrameTimeInterval ( mediaIdVideo ) );
        }
    }

void UT_CIlbcPayloadFormatWrite::UT_CIlbcPayloadFormatWrite_DurationL(  )
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

void UT_CIlbcPayloadFormatWrite::UT_CIlbcPayloadFormatWrite_CreateSinkBufferLL(  )
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

void UT_CIlbcPayloadFormatWrite::UT_CIlbcPayloadFormatWrite_SinkDataTypeCodeL(  )
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

void UT_CIlbcPayloadFormatWrite::UT_CIlbcPayloadFormatWrite_SetSinkDataTypeCodeL(  )
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

void UT_CIlbcPayloadFormatWrite::UT_CIlbcPayloadFormatWrite_SinkThreadLogonL(  )
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

void UT_CIlbcPayloadFormatWrite::UT_CIlbcPayloadFormatWrite_SinkThreadLogoffL(  )
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

void UT_CIlbcPayloadFormatWrite::UT_CIlbcPayloadFormatWrite_EmptyBufferLL(  )
    {
    TMediaId mediaId( KUidMediaTypeAudio, 1 );
    TMediaId mediaIdVideo( KUidMediaTypeVideo, 1 );
    
    if ( !iAlloc )
        {
        EUNIT_ASSERT_NO_LEAVE( iWrite->EmptyBufferL( iBuffer, iUlSink, mediaId ) );
        EUNIT_ASSERT_SPECIFIC_LEAVE( iWrite->EmptyBufferL( iBuffer, iUlSink, mediaIdVideo ), KErrNotSupported );
             
        iWrite->iFrameIndex = 1;
        EUNIT_ASSERT_NO_LEAVE( iWrite->EmptyBufferL( iBuffer, iUlSink, mediaId ) );
        }
    
    else 
        {
        EUNIT_ASSERT_NO_LEAVE( iWrite->EmptyBufferL( iBuffer, iUlSink, mediaId ) );
        EUNIT_ASSERT_SPECIFIC_LEAVE( iWrite->EmptyBufferL( iBuffer, iUlSink, mediaIdVideo ), KErrNotSupported );
        
        }
    }

void UT_CIlbcPayloadFormatWrite::UT_CIlbcPayloadFormatWrite_BufferEmptiedLL(  )
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

void UT_CIlbcPayloadFormatWrite::UT_CIlbcPayloadFormatWrite_SampleRateL(  )
    {
    TUint samplerate;
    
    if ( !iAlloc )
        {
        samplerate = iWrite->SampleRate( );
        EUNIT_ASSERT_EQUALS( samplerate, KiLBCSampleRate );
        }
    
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iWrite->SampleRate( ) );
        }
    }

void UT_CIlbcPayloadFormatWrite::UT_CIlbcPayloadFormatWrite_SetSampleRateL(  )
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

void UT_CIlbcPayloadFormatWrite::UT_CIlbcPayloadFormatWrite_DeliverPacketLL( )
    {
    iBuff->Data().Append( _L("1234512345") );
     
    EUNIT_ASSERT_NO_LEAVE( iWrite->DeliverPacketL( *iBuff ) );
    
    iWrite->iCNModeON = ETrue;
    EUNIT_ASSERT_NO_LEAVE( iWrite->DeliverPacketL( *iBuff ) );    
        
    iBuff->Data().Append( _L("1234567891") );  
    EUNIT_ASSERT_NO_LEAVE( iWrite->DeliverPacketL( *iBuff ) );   
        
    iWrite->iClip = NULL;
    EUNIT_ASSERT_LEAVE( iWrite->DeliverPacketL( *iBuff ) );
    
    iWrite->iClip = iRtpDataSinkStub;
    iWrite->iRtpDataSink = NULL;
    EUNIT_ASSERT_LEAVE( iWrite->DeliverPacketL( *iBuff ) );  
    }
 
void UT_CIlbcPayloadFormatWrite::UT_CIlbcPayloadFormatWrite_ConfigurePayloadFormatLL( )
    {
    if ( iAlloc )
        {
        EUNIT_ASSERT( ETrue );
        }
    else
        { 
        TMccCodecInfo codec;
        codec.iAlgoUsed         = ENoAlgoUsed;
        codec.iBitrate          = 15200;
        codec.iCNFrameSize      = 0;
        codec.iCodecMode        = 3;
        codec.iEnableDTX        = 1;
        codec.iFourCC           = 0x31313747;
        codec.iFrameSize        = 0;
        codec.iHwFrameTime      = 20;
        codec.iMaxPtime         = 200;
        codec.iPayloadType      = 8;
        codec.iPtime            = 20;
        codec.iRedundancyCount  = 1;
        codec.iRedundantPayload = 99;
        codec.iComfortNoiseGenerationPt = KPayloadTypeUndefined;
        
        TMccCodecInfoBuffer buffer( codec );
        EUNIT_ASSERT_NO_LEAVE( iWrite->ConfigurePayloadFormatL( buffer, *iRtpMediaClock ));
        
        codec.iBitrate = 10000;
        buffer =  codec;
        EUNIT_ASSERT_LEAVE( iWrite->ConfigurePayloadFormatL( buffer, *iRtpMediaClock  ) );     
        
        codec.iBitrate = 13330;
        buffer =  codec;
        EUNIT_ASSERT_NO_LEAVE( iWrite->ConfigurePayloadFormatL( buffer, *iRtpMediaClock  ) ); 
        
        // test updating of values
        codec.iIsUpdate         = ETrue;
        codec.iBitrate          = 15200;
        codec.iPtime            = 40;
        codec.iPayloadType      = 0;
        codec.iComfortNoiseGenerationPt = 13;
        buffer =  codec;
        iWrite->ConfigurePayloadFormatL( buffer, *iRtpMediaClock );         
        TMccCodecInfoBuffer buffer2( iWrite->iCInfo );
        EUNIT_ASSERT( iWrite->iCInfo.iPtime == 40 );
        EUNIT_ASSERT( iWrite->iCInfo.iPayloadType == 0 );
        EUNIT_ASSERT( iWrite->iCInfo.iComfortNoiseGenerationPt == 13 );
        
        // test redundancy payload format usage
        codec.iIsUpdate         = EFalse;
        codec.iAlgoUsed         = EGenRedUsed;
        codec.iRedundancyCount  = 1;
        buffer = codec; 
        CMccRtpDataSink* rtpDataSinkStub = 
            static_cast<CMccRtpDataSink*>( MDataSink::NewSinkL( KRtpDataSink, KNullDesC8 ) );
        CleanupStack::PushL( rtpDataSinkStub );
        CMccRedPayloadWrite* redEncoder = CMccRedPayloadWrite::NewL( rtpDataSinkStub );
        CleanupStack::PushL( redEncoder );
        CIlbcPayloadFormatWrite* write = CIlbcPayloadFormatWrite::NewL( redEncoder );
        CleanupStack::PushL( write );
        write->iCInfo.iHwFrameTime = 20;
        EUNIT_ASSERT_NO_LEAVE( write->ConfigurePayloadFormatL( buffer, *iRtpMediaClock ) );
        CleanupStack::PopAndDestroy( 3 ); 
        }
    }


void UT_CIlbcPayloadFormatWrite::UT_CIlbcPayloadFormatWrite_UpdateConfigurationLL( )
    {          
    iCodec.iAlgoUsed = EGenRedUsed;
    iCodec.iBitrate = 13330;
    iCodec.iCNFrameSize = 0;
    iCodec.iCodecMode = 3;
    iCodec.iEnableDTX = 1;
    iCodec.iFourCC = 0x31313747;
    iCodec.iFrameSize = 0;
    iCodec.iHwFrameTime = 20;
    iCodec.iMaxPtime = 220;
    iCodec.iPayloadType = 8;
    iCodec.iPtime = 20;
    iCodec.iRedundancyCount = 1;
    iCodec.iRedundantPayload = 99;
    
    CMccRtpDataSink* rtpDataSinkStub = 
        reinterpret_cast<CMccRtpDataSink*>( MDataSink::NewSinkL( KRtpDataSink, KNullDesC8 ) );
    CleanupStack::PushL( rtpDataSinkStub );
    CMccRedPayloadWrite* redEncoder = CMccRedPayloadWrite::NewL( rtpDataSinkStub );
    CleanupStack::PushL( redEncoder );
    CIlbcPayloadFormatWrite* write2 = CIlbcPayloadFormatWrite::NewL( redEncoder );
    CleanupStack::PushL( write2 );    
    TMccCodecInfoBuffer buffer( iCodec );
    EUNIT_ASSERT_NO_LEAVE( write2->ConfigurePayloadFormatL( buffer, *iRtpMediaClock ) );

    write2->UpdateConfigurationL( iCodec );
    
    iCodec.iHwFrameTime = 0;
    iCodec.iPtime = 30;
    EUNIT_ASSERT_SPECIFIC_LEAVE( write2->UpdateConfigurationL( iCodec ), KErrArgument );
    
    CleanupStack::PopAndDestroy( 3 );
    }  
    

void UT_CIlbcPayloadFormatWrite::UT_CIlbcPayloadFormatWrite_NumChannelsL()
    {
    EUNIT_ASSERT_EQUALS( iWrite->NumChannels(), 1 );
    }  
    
     
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CIlbcPayloadFormatWrite,
    "MCC iLBC Payloadformat Unittests",
    "UNIT" )
    
EUNIT_TEST(
    "EmptySourceBufferL - test ",
    "CIlbcPayloadFormatWrite",
    "EmptySourceBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CIlbcPayloadFormatWrite_EmptySourceBufferLL, Teardown)

EUNIT_TEST(
    "SourceBufferEmptiedL - test ",
    "CIlbcPayloadFormatWrite",
    "SourceBufferEmptiedL",
    "FUNCTIONALITY",
    SetupL, UT_CIlbcPayloadFormatWrite_SourceBufferEmptiedLL, Teardown)

EUNIT_TEST(
    "SinkPrimeL - test ",
    "CIlbcPayloadFormatWrite",
    "SinkPrimeL",
    "FUNCTIONALITY",
    SetupL, UT_CIlbcPayloadFormatWrite_SinkPrimeLL, Teardown)

EUNIT_TEST(
    "SinkPlayL - test ",
    "CIlbcPayloadFormatWrite",
    "SinkPlayL",
    "FUNCTIONALITY",
    SetupL, UT_CIlbcPayloadFormatWrite_SinkPlayLL, Teardown)

EUNIT_TEST(
    "SinkPauseL - test ",
    "CIlbcPayloadFormatWrite",
    "SinkPauseL",
    "FUNCTIONALITY",
    SetupL, UT_CIlbcPayloadFormatWrite_SinkPauseLL, Teardown)

EUNIT_TEST(
    "SinkStopL - test ",
    "CIlbcPayloadFormatWrite",
    "SinkStopL",
    "FUNCTIONALITY",
    SetupL, UT_CIlbcPayloadFormatWrite_SinkStopLL, Teardown)

EUNIT_TEST(
    "FrameTimeInterval - test ",
    "CIlbcPayloadFormatWrite",
    "FrameTimeInterval",
    "FUNCTIONALITY",
    SetupL, UT_CIlbcPayloadFormatWrite_FrameTimeIntervalL, Teardown)

EUNIT_TEST(
    "Duration - test ",
    "CIlbcPayloadFormatWrite",
    "Duration",
    "FUNCTIONALITY",
    SetupL, UT_CIlbcPayloadFormatWrite_DurationL, Teardown)

EUNIT_TEST(
    "CreateSinkBufferL - test ",
    "CIlbcPayloadFormatWrite",
    "CreateSinkBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CIlbcPayloadFormatWrite_CreateSinkBufferLL, Teardown)

EUNIT_TEST(
    "SinkDataTypeCode - test ",
    "CIlbcPayloadFormatWrite",
    "SinkDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CIlbcPayloadFormatWrite_SinkDataTypeCodeL, Teardown)

EUNIT_TEST(
    "SetSinkDataTypeCode - test ",
    "CIlbcPayloadFormatWrite",
    "SetSinkDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CIlbcPayloadFormatWrite_SetSinkDataTypeCodeL, Teardown)

EUNIT_TEST(
    "SinkThreadLogon - test ",
    "CIlbcPayloadFormatWrite",
    "SinkThreadLogon",
    "FUNCTIONALITY",
    SetupL, UT_CIlbcPayloadFormatWrite_SinkThreadLogonL, Teardown)

EUNIT_TEST(
    "SinkThreadLogoff - test ",
    "CIlbcPayloadFormatWrite",
    "SinkThreadLogoff",
    "FUNCTIONALITY",
    SetupL, UT_CIlbcPayloadFormatWrite_SinkThreadLogoffL, Teardown)

EUNIT_TEST(
    "EmptyBufferL - test ",
    "CIlbcPayloadFormatWrite",
    "EmptyBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CIlbcPayloadFormatWrite_EmptyBufferLL, Teardown)

EUNIT_TEST(
    "SampleRate - test ",
    "CIlbcPayloadFormatWrite",
    "SampleRate",
    "FUNCTIONALITY",
    SetupL, UT_CIlbcPayloadFormatWrite_SampleRateL, Teardown)

EUNIT_TEST(
    "SetSampleRate - test ",
    "CIlbcPayloadFormatWrite",
    "SetSampleRate",
    "FUNCTIONALITY",
    SetupL, UT_CIlbcPayloadFormatWrite_SetSampleRateL, Teardown)

EUNIT_TEST(
    "DeliverPacketL - test ",
    "CIlbcPayloadFormatWrite",
    "DeliverPacketL",
    "FUNCTIONALITY",
    SetupL, UT_CIlbcPayloadFormatWrite_DeliverPacketLL, Teardown)  

EUNIT_TEST(
    "NumChannelsL - test ",
    "CIlbcPayloadFormatWrite",
    "NumChannelsL",
    "FUNCTIONALITY",
    SetupL, UT_CIlbcPayloadFormatWrite_NumChannelsL, Teardown)  
  
EUNIT_TEST(
    "ConfigurePayloadFormatL - test ",
    "CIlbcPayloadFormatWrite",
    "ConfigurePayloadFormatL",
    "FUNCTIONALITY",
    SetupL, UT_CIlbcPayloadFormatWrite_ConfigurePayloadFormatLL, Teardown)    
    
EUNIT_TEST(
    "UpdateConfigurationL - test ",
    "CIlbcPayloadFormatWrite",
    "UpdateConfigurationL",
    "FUNCTIONALITY",
    SetupL, UT_CIlbcPayloadFormatWrite_UpdateConfigurationLL, Teardown)    


// ALLOC TESTS
#ifdef EUNIT_ALLOC_TEST_ON

EUNIT_ALLOC_TEST(
    "EmptySourceBufferL - alloc test ",
    "CIlbcPayloadFormatWrite",
    "EmptySourceBufferL",
    "ERRORHANDLING",
    SetupAL, UT_CIlbcPayloadFormatWrite_EmptySourceBufferLL, Teardown)

EUNIT_ALLOC_TEST(
    "SourceBufferEmptiedL - alloc test ",
    "CIlbcPayloadFormatWrite",
    "SourceBufferEmptiedL",
    "ERRORHANDLING",
    SetupAL, UT_CIlbcPayloadFormatWrite_SourceBufferEmptiedLL, Teardown)

EUNIT_ALLOC_TEST(
    "SinkPrimeL - alloc test ",
    "CIlbcPayloadFormatWrite",
    "SinkPrimeL",
    "ERRORHANDLING",
    SetupAL, UT_CIlbcPayloadFormatWrite_SinkPrimeLL, Teardown)

EUNIT_ALLOC_TEST(
    "SinkPlayL - alloc test ",
    "CIlbcPayloadFormatWrite",
    "SinkPlayL",
    "ERRORHANDLING",
    SetupAL, UT_CIlbcPayloadFormatWrite_SinkPlayLL, Teardown)

EUNIT_ALLOC_TEST(
    "SinkPauseL - alloc test ",
    "CIlbcPayloadFormatWrite",
    "SinkPauseL",
    "ERRORHANDLING",
    SetupAL, UT_CIlbcPayloadFormatWrite_SinkPauseLL, Teardown)

EUNIT_ALLOC_TEST(
    "SinkStopL - alloc test ",
    "CIlbcPayloadFormatWrite",
    "SinkStopL",
    "ERRORHANDLING",
    SetupAL, UT_CIlbcPayloadFormatWrite_SinkStopLL, Teardown)

EUNIT_ALLOC_TEST(
    "FrameTimeInterval - alloc test ",
    "CIlbcPayloadFormatWrite",
    "FrameTimeInterval",
    "ERRORHANDLING",
    SetupAL, UT_CIlbcPayloadFormatWrite_FrameTimeIntervalL, Teardown)

EUNIT_ALLOC_TEST(
    "Duration - alloc test ",
    "CIlbcPayloadFormatWrite",
    "Duration",
    "ERRORHANDLING",
    SetupAL, UT_CIlbcPayloadFormatWrite_DurationL, Teardown)

EUNIT_ALLOC_TEST(
    "CreateSinkBufferL - alloc test ",
    "CIlbcPayloadFormatWrite",
    "CreateSinkBufferL",
    "ERRORHANDLING",
    Setup2AL, UT_CIlbcPayloadFormatWrite_CreateSinkBufferLL, Teardown)

EUNIT_ALLOC_TEST(
    "SinkDataTypeCode - alloc test ",
    "CIlbcPayloadFormatWrite",
    "SinkDataTypeCode",
    "ERRORHANDLING",
    SetupAL, UT_CIlbcPayloadFormatWrite_SinkDataTypeCodeL, Teardown)

EUNIT_ALLOC_TEST(
    "SetSinkDataTypeCode - alloc test ",
    "CIlbcPayloadFormatWrite",
    "SetSinkDataTypeCode",
    "ERRORHANDLING",
    SetupAL, UT_CIlbcPayloadFormatWrite_SetSinkDataTypeCodeL, Teardown)

EUNIT_ALLOC_TEST(
    "SinkThreadLogon - alloc test ",
    "CIlbcPayloadFormatWrite",
    "SinkThreadLogon",
    "ERRORHANDLING",
    SetupAL, UT_CIlbcPayloadFormatWrite_SinkThreadLogonL, Teardown)

EUNIT_ALLOC_TEST(
    "SinkThreadLogoff - alloc test ",
    "CIlbcPayloadFormatWrite",
    "SinkThreadLogoff",
    "ERRORHANDLING",
    SetupAL, UT_CIlbcPayloadFormatWrite_SinkThreadLogoffL, Teardown)

EUNIT_ALLOC_TEST(
    "EmptyBufferL - alloc test ",
    "CIlbcPayloadFormatWrite",
    "EmptyBufferL",
    "ERRORHANDLING",
    SetupAL, UT_CIlbcPayloadFormatWrite_EmptyBufferLL, Teardown)

EUNIT_ALLOC_TEST(
    "BufferEmptiedL - Alloc test ",
    "CIlbcPayloadFormatWrite",
    "BufferEmptiedL",
    "ERRORHANDLING",
    SetupAL, UT_CIlbcPayloadFormatWrite_BufferEmptiedLL, Teardown)

EUNIT_ALLOC_TEST(
    "SampleRate - alloc test ",
    "CIlbcPayloadFormatWrite",
    "SampleRate",
    "ERRORHANDLING",
    SetupAL, UT_CIlbcPayloadFormatWrite_SampleRateL, Teardown)

EUNIT_ALLOC_TEST(
    "SetSampleRate - alloc test ",
    "CIlbcPayloadFormatWrite",
    "SetSampleRate",
    "ERRORHANDLING",
    SetupAL, UT_CIlbcPayloadFormatWrite_SetSampleRateL, Teardown)


#endif

EUNIT_END_TEST_TABLE

//  END OF FILE
