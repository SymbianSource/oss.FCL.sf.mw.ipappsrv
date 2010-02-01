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
#include "UT_CAmrPayloadFormatWrite.h"

//  EXTERNAL INCLUDES
#include <e32base.h>
#include <digia/eunit/eunitmacros.h>

#ifdef EUNIT_ALLOC_TEST_ON
    #include <digia/eunit/CEunitAllocTestCaseDecorator.h>   //for using EUNIT_ALLOC_TEST
#endif

//  INTERNAL INCLUDES
#include "AmrPayloadFormatWrite.h"
#include "MccRtpDataSink.h"
#include <mmf/server/mmfdatasink.h>
#include <mmf/server/mmfdatasource.h>
#include "Mcculdatapath.h"
#include "mccuids.hrh"
#include "mccrtpmediaclock.h"
#include "mccresourcepool.h" 
#include "mccredpayloadwrite.h"
#include "amrpayloadencoder.h"

const TUid KRtpDataSink              =  { KImplUidRtpDataSink               };

// CONSTRUCTION
UT_CAmrPayloadFormatWrite* UT_CAmrPayloadFormatWrite::NewL()
    {
    UT_CAmrPayloadFormatWrite* self = UT_CAmrPayloadFormatWrite::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CAmrPayloadFormatWrite* UT_CAmrPayloadFormatWrite::NewLC()
    {
    UT_CAmrPayloadFormatWrite* self = new( ELeave ) UT_CAmrPayloadFormatWrite();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CAmrPayloadFormatWrite::~UT_CAmrPayloadFormatWrite()
    {   
    }

// Default constructor
UT_CAmrPayloadFormatWrite::UT_CAmrPayloadFormatWrite()
    {
    
    }

// Second phase construct
void UT_CAmrPayloadFormatWrite::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CAmrPayloadFormatWrite::SetupAL(  )
    {
    iAlloc = EFalse; // ETrue;
    
    CommonSetupL( ETrue );
  
    TMediaId mediaId( KUidMediaTypeAudio, 1 );
    // Datapath calls CreateSinkBufferL
    TBool bufferReference;
    iBuffer = iWrite->CreateSinkBufferL( mediaId, bufferReference );
    
    iWrite->SinkPrimeL();
    iWrite->SinkPlayL();
    } 

void UT_CAmrPayloadFormatWrite::Setup2AL(  )
    {
    // not call CreateSinkBufferL
    
    iAlloc = ETrue;

    CommonSetupL( ETrue );
      
    iWrite->SinkPrimeL();
    iWrite->SinkPlayL( );
    }


void UT_CAmrPayloadFormatWrite::SetupL(  )
    {
    iAlloc = EFalse;
    
    CommonSetupL( ETrue );
      
    TMediaId mediaId( KUidMediaTypeAudio, 1 );
    // Datapath calls CreateSinkBufferL
    TBool bufferReference;
    iBuffer = iWrite->CreateSinkBufferL( mediaId, bufferReference );
    
    iWrite->SinkStopL();
    iWrite->SinkPrimeL();
    iWrite->SinkPlayL();
    }

void UT_CAmrPayloadFormatWrite::SetupWbAL(  )
    {
    iAlloc = ETrue;
    
    CommonSetupL( EFalse );
  
    TMediaId mediaId( KUidMediaTypeAudio, 1 );
    // Datapath calls CreateSinkBufferL
    TBool bufferReference;
    iBuffer = iWrite->CreateSinkBufferL( mediaId, bufferReference );
    
    iWrite->SinkPrimeL();
    iWrite->SinkPlayL();
    } 

void UT_CAmrPayloadFormatWrite::SetupWbL(  )
    {
    iAlloc = EFalse;
    
    CommonSetupL( EFalse );
      
    TMediaId mediaId( KUidMediaTypeAudio, 1 );
    // Datapath calls CreateSinkBufferL
    TBool bufferReference;
    iBuffer = iWrite->CreateSinkBufferL( mediaId, bufferReference );
    
    iWrite->SinkStopL();
    iWrite->SinkPrimeL();
    iWrite->SinkPlayL();
    }

void UT_CAmrPayloadFormatWrite::CommonSetupL( TBool aIsNb )
    {
    MAsyncEventHandler* eventHandler = NULL;
    CMccResourcePool* mccresource = NULL;
    iRtpMediaClock = CMccRtpMediaClock::NewL();
     
    iUlSink = CMccUlDataPath::NewL( eventHandler, mccresource, iMediaId ); //MDataSink::NewSinkL( KRtpDataSink, KNullDesC8 );
    iRtpDataSinkStub = static_cast<CMccRtpDataSink*>(MDataSink::NewSinkL( KRtpDataSink, KNullDesC8 ));  
    iWrite = CAmrPayloadFormatWrite::NewL( iRtpDataSinkStub );

    if ( aIsNb )    
        {
        iCodec.iFourCC.Set( TFourCC(' ','A','M','R') );
        iCodec.iBitrate = 12200;
        iCodec.iFrameSize = 32;
        }
    else
        {
        iCodec.iFourCC.Set( TFourCC(' ','A','W','B') );
        iCodec.iBitrate = 23850;
        iCodec.iFrameSize = 64;
        }
    iCodec.iCodecMode = KAmrCodecModeOctetAlign;
    iCodec.iPtime = 40;
    iCodec.iMaxPtime = 200;
    iCodec.iPayloadType = 96;
    iCodec.iRedundancyCount = 0;
    iCodec.iEnableDTX = EFalse;
    iCodec.iRedundantPayload = 0;
    iCodec.iHwFrameTime = 20;
    iCodec.iAlgoUsed = 0;
  
    iWrite->SetSinkDataTypeCode( iCodec.iFourCC, KUidMediaTypeAudio );

    TMccCodecInfoBuffer buffer( iCodec );
    
    iWrite->ConfigurePayloadFormatL( buffer, *iRtpMediaClock );
    
    iBuff = CMMFDescriptorBuffer::NewL( 300 );
  
    }

void UT_CAmrPayloadFormatWrite::Teardown(  )
    {
    delete iUlSink;
    delete iRtpDataSinkStub;
    delete iWrite;
    delete iBuff;
    delete iRtpMediaClock;
    
    REComSession::FinalClose();
    }
    

void UT_CAmrPayloadFormatWrite::UT_CAmrPayloadFormatWrite_CancelUlRequestL(  )
    {
    if( !iAlloc )
        {
        EUNIT_ASSERT_NO_LEAVE( iWrite->CancelUlRequest() );
        }
    else
        {
        
        EUNIT_ASSERT_NO_LEAVE( iWrite->CancelUlRequest() );
       }
    }

void UT_CAmrPayloadFormatWrite::UT_CAmrPayloadFormatWrite_EmptySourceBufferLL(  )
    {
       
    if( !iAlloc ) 
        {
        EUNIT_ASSERT_NO_LEAVE( iWrite->EmptySourceBufferL() );
        }
    
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iWrite->EmptySourceBufferL() );
        }
    
    }

void UT_CAmrPayloadFormatWrite::UT_CAmrPayloadFormatWrite_SourceBufferEmptiedLL(  )
    {
    EUNIT_ASSERT_NO_LEAVE( iWrite->EmptyBufferL( iBuffer, iUlSink, KUidMediaTypeAudio ) );
    EUNIT_ASSERT_NO_LEAVE( iWrite->SourceBufferEmptiedL( ) );     
    }


void UT_CAmrPayloadFormatWrite::UT_CAmrPayloadFormatWrite_FrameTimeIntervalL(  )
    { 
    TTimeIntervalMicroSeconds catchAfish;
    TMediaId mediaIdAudio( KUidMediaTypeAudio, 1 );
    TMediaId mediaIdVideo( KUidMediaTypeVideo, 1 );
    
    if ( !iAlloc )
        {
        EUNIT_ASSERT_NO_LEAVE( catchAfish = iWrite->FrameTimeInterval ( mediaIdAudio ) );
        EUNIT_ASSERT_NO_LEAVE( catchAfish = iWrite->FrameTimeInterval ( mediaIdVideo ) );
        }
        
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iWrite->FrameTimeInterval ( mediaIdAudio ) );
        EUNIT_ASSERT_NO_LEAVE( iWrite->FrameTimeInterval ( mediaIdVideo ) );
        }
     
    
    }

void UT_CAmrPayloadFormatWrite::UT_CAmrPayloadFormatWrite_DurationL(  )
    {
    TTimeIntervalMicroSeconds catchAfish;
    TMediaId mediaId( KUidMediaTypeAudio, 1 );
    
    if ( !iAlloc )
        {
        catchAfish = iWrite->Duration( mediaId );
        //EUNIT_ASSERT_EQUALS( catchAfish.Int64(), 1000000000 );
        EUNIT_ASSERT_EQUALS( catchAfish.Int64(), 0 );
        }
    
    else 
        {
        EUNIT_ASSERT_NO_LEAVE( iWrite->Duration( mediaId ) );
        }
    
    }

void UT_CAmrPayloadFormatWrite::UT_CAmrPayloadFormatWrite_NumChannelsL(  )
    {
   
    
    if ( !iAlloc )
        {
        EUNIT_ASSERT_EQUALS( iWrite->NumChannels( ), iWrite->iChannels );
        }
    
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iWrite->NumChannels( ) );
        }
    }

void UT_CAmrPayloadFormatWrite::UT_CAmrPayloadFormatWrite_SampleRateL(  )
    {
    TUint samplerate;
    
    if ( !iAlloc )
        {
        samplerate = iWrite->SampleRate( );
        EUNIT_ASSERT_EQUALS( samplerate, iWrite->iSamplingRate );
        }
    
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iWrite->SampleRate( ) );
        }
    

    }

void UT_CAmrPayloadFormatWrite::UT_CAmrPayloadFormatWrite_SetSampleRateL(  )
    {
    TInt rate( 8000 );
    TInt wbrate( 16000 );
    if ( !iAlloc )
        {
        EUNIT_ASSERT( iWrite->SetSampleRate( rate ) == KErrNone );
        EUNIT_ASSERT( iWrite->SetSampleRate( rate + 1 ) == KErrNotSupported );
        EUNIT_ASSERT( iWrite->SetSampleRate( wbrate ) == KErrNotSupported );
        }
    
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iWrite->SetSampleRate( rate ) );
        EUNIT_ASSERT_NO_LEAVE( iWrite->SetSampleRate( rate + 1 ) );
        EUNIT_ASSERT_NO_LEAVE( iWrite->SetSampleRate( wbrate ) );
        }
    }

void UT_CAmrPayloadFormatWrite::UT_CAmrPayloadFormatWrite_SetSampleRateWbL(  )
    {
    TInt rate( 8000 );
    TInt wbrate( 16000 );
    if ( !iAlloc )
        {
        EUNIT_ASSERT( iWrite->SetSampleRate( wbrate ) == KErrNone );
        EUNIT_ASSERT( iWrite->SetSampleRate( wbrate + 1 ) == KErrNotSupported );
        EUNIT_ASSERT( iWrite->SetSampleRate( rate ) == KErrNotSupported );
        }
    
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iWrite->SetSampleRate( wbrate ) );
        EUNIT_ASSERT_NO_LEAVE( iWrite->SetSampleRate( wbrate + 1 ) );
        EUNIT_ASSERT_NO_LEAVE( iWrite->SetSampleRate( rate ) );
        }
    }

void UT_CAmrPayloadFormatWrite::UT_CAmrPayloadFormatWrite_CreateSinkBufferLL(  )
    {
    TMediaId mediaId( KUidMediaTypeAudio, 1 );
    TMediaId mediaIdVideo( KUidMediaTypeVideo, 1 );
    TBool reference;
    
    CMMFBuffer* buffer = NULL;
    EUNIT_ASSERT_SPECIFIC_LEAVE( buffer = iWrite->CreateSinkBufferL( mediaIdVideo, reference ), KErrNotSupported );

    EUNIT_ASSERT_NO_LEAVE( buffer = iWrite->CreateSinkBufferL( mediaId, reference ) );
    EUNIT_ASSERT(reference);
    }

void UT_CAmrPayloadFormatWrite::UT_CAmrPayloadFormatWrite_SinkDataTypeCodeL(  )
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

void UT_CAmrPayloadFormatWrite::UT_CAmrPayloadFormatWrite_SetSinkDataTypeCodeL(  )
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
        EUNIT_ASSERT_EQUALS( iWrite->SetSinkDataTypeCode( fourcc, mediaId ), KErrNotSupported );
        EUNIT_ASSERT_EQUALS( iWrite->SetSinkDataTypeCode( fourcc, mediaIdVideo ), KErrNotSupported );
        }
    
    else
        {
        fourcc = iWrite->SinkDataTypeCode( mediaIdVideo );
        EUNIT_ASSERT_EQUALS( iWrite->SetSinkDataTypeCode( fourcc, mediaIdVideo ), KErrNotSupported );
        EUNIT_ASSERT_EQUALS( iWrite->SetSinkDataTypeCode( fourcc, mediaId ), KErrNotSupported );
        fourcc = iWrite->SinkDataTypeCode( mediaId  );
        EUNIT_ASSERT_EQUALS( iWrite->SetSinkDataTypeCode( fourcc, mediaId ), KErrNone );
        EUNIT_ASSERT_EQUALS( iWrite->SetSinkDataTypeCode( fourcc, mediaIdVideo ), KErrNotSupported );
        }
    
    }

void UT_CAmrPayloadFormatWrite::UT_CAmrPayloadFormatWrite_SinkThreadLogonL(  )
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

void UT_CAmrPayloadFormatWrite::UT_CAmrPayloadFormatWrite_SinkThreadLogoffL(  )
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

void UT_CAmrPayloadFormatWrite::UT_CAmrPayloadFormatWrite_EmptyBufferLL(  )
    {
    TMediaId mediaId( KUidMediaTypeAudio, 1 );
    TMediaId mediaIdVideo( KUidMediaTypeVideo, 1 );

    EUNIT_ASSERT_NO_LEAVE( iWrite->EmptyBufferL( iBuffer, iUlSink, mediaId ) );
    EUNIT_ASSERT_SPECIFIC_LEAVE( iWrite->EmptyBufferL( iBuffer, iUlSink, mediaIdVideo ), KErrNotSupported );
    }

void UT_CAmrPayloadFormatWrite::UT_CAmrPayloadFormatWrite_BufferEmptiedLL(  )
    {
    // Only alloc test   
    if ( iAlloc )
        {
        EUNIT_ASSERT_NO_LEAVE( iWrite->BufferEmptiedL( iBuffer ) );
        }
    }

void UT_CAmrPayloadFormatWrite::UT_CAmrPayloadFormatWrite_SinkPrimeLL(  )
    {
    if ( !iAlloc ) 
        {
        EUNIT_ASSERT_NO_LEAVE( iWrite->SinkPrimeL() );
        }
    
    else 
        {
        EUNIT_ASSERT_NO_LEAVE( iWrite->SinkPrimeL() );
        }
   
    }


void UT_CAmrPayloadFormatWrite::UT_CAmrPayloadFormatWrite_SinkPauseLL(  )
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

void UT_CAmrPayloadFormatWrite::UT_CAmrPayloadFormatWrite_SinkStopLL(  )
    {
    if ( !iAlloc )
        {
        //iWrite->iStateMachine = CFormatEncodeStateMachine::NewL(this);
        iWrite->iStateMachine->Cancel();
        iWrite->iStateMachine->ChangeState( EEmptySourceBuffer );
        
        EUNIT_ASSERT_NO_LEAVE( iWrite->SinkStopL() );
        iWrite->iStateMachine->Cancel();
        iWrite->iStateMachine->ChangeState( ESourceBufferEmptied );
        EUNIT_ASSERT_NO_LEAVE( iWrite->SinkStopL() );
        
        //delete iWrite->iStateMachine;
        //iWrite->iStateMachine = NULL;
        }
    
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iWrite->SinkStopL() );
        }
    }

void UT_CAmrPayloadFormatWrite::UT_CAmrPayloadFormatWrite_ConfigurePayloadFormatLL(  )
    {      
    if ( !iAlloc)
        {
        EUNIT_ASSERT_SPECIFIC_LEAVE( iWrite->ConfigurePayloadFormatL( TPckgBuf<TInt>( 4 ), *iRtpMediaClock ), 
                                     KErrArgument );
        
        TMccCodecInfo codec;

        codec.iRedundancyCount = 0;
        codec.iRedundantPayload = 0;
        codec.iHwFrameTime = 0;
        codec.iAlgoUsed = 0;
        iCodec.iFrameSize = 32;
        
        TMccCodecInfoBuffer buffer( codec );
        EUNIT_ASSERT_LEAVE( iWrite->ConfigurePayloadFormatL( buffer, *iRtpMediaClock ) );
    
        codec.iBitrateMask = 1;
        codec.iRedundancyCount = 1;
        codec.iRedundantPayload = 99;
        codec.iHwFrameTime = 1;
        codec.iAlgoUsed = 2;
        codec.iCodecMode = KAmrCodecModeOctetAlign;
        buffer = codec;
        CMccRtpDataSink* rtpDataSinkStub = static_cast<CMccRtpDataSink*>(MDataSink::NewSinkL( KRtpDataSink, KNullDesC8 ));
        CleanupStack::PushL( rtpDataSinkStub );
        CMccRedPayloadWrite* redEncoder = CMccRedPayloadWrite::NewL( rtpDataSinkStub );
        CleanupStack::PushL( redEncoder );
        CAmrPayloadFormatWrite* write = CAmrPayloadFormatWrite::NewL( redEncoder );
        CleanupStack::PushL( write );
        write->ConfigurePayloadFormatL( buffer, *iRtpMediaClock );
        CleanupStack::PopAndDestroy( 3 ); 
        
        codec.iAlgoUsed = 4;
        buffer = codec;
        iWrite->ConfigurePayloadFormatL( buffer, *iRtpMediaClock );
        
        // Unknown mode
        delete iWrite->iPayloadEncoder;
        iWrite->iPayloadEncoder = NULL;
        codec.iCodecMode = 999;
        buffer = codec;
        
        TRAPD( err, iWrite->ConfigurePayloadFormatL( buffer, *iRtpMediaClock ) )
        if ( err == KErrNoMemory )
        	{
        	User::Leave( err );
        	}
        EUNIT_ASSERT_EQUALS( KErrArgument, err );
        
        // Different mode
        delete iWrite->iPayloadEncoder;
        iWrite->iPayloadEncoder = NULL;
        codec.iCodecMode = KAmrCodecModeBandwidthEfficient;
        buffer = codec;
        iWrite->ConfigurePayloadFormatL( buffer, *iRtpMediaClock );
        
        // Update
        codec.iIsUpdate = ETrue;
        buffer = codec;
        iWrite->ConfigurePayloadFormatL( buffer, *iRtpMediaClock );
        
        // No modes defined previously
        iWrite->iCInfo.iBitrateMask = 0;
        iWrite->ConfigurePayloadFormatL( buffer, *iRtpMediaClock );
        
        // Fec
        codec.iAlgoUsed = EAmrFecUsed;
        buffer = codec;
        iWrite->ConfigurePayloadFormatL( buffer, *iRtpMediaClock );
        }
    else
        {
        EUNIT_ASSERT( ETrue );
        }   
    }

void UT_CAmrPayloadFormatWrite::UT_CAmrPayloadFormatWrite_IsBitrateChangeValidL()
    {
    // First time, no allowed bitrates defined
    iWrite->iModes.Reset();
    EUNIT_ASSERT_EQUALS( iWrite->IsBitrateChangeValid( 12200 ), EFalse );
    
    // First time, ok bitrate
    iWrite->iModes.AppendL( 5150 );
    iWrite->iModes.AppendL( 12200 );
    EUNIT_ASSERT_EQUALS( iWrite->IsBitrateChangeValid( 5150 ), ETrue );
    EUNIT_ASSERT_EQUALS( iWrite->iCInfo.iBitrate, 5150 );
    
    // Current mode set, new mode not ok
    EUNIT_ASSERT_EQUALS( iWrite->IsBitrateChangeValid( 7000 ), EFalse );
    
    // Current mode set, new mode ok
    iWrite->iFramesEncoded = 100;
    iWrite->iCInfo.iModeChangePeriod = 100;
    iWrite->iCInfo.iNeighbor = ETrue;
    EUNIT_ASSERT_EQUALS( iWrite->IsBitrateChangeValid( 12200 ), ETrue );
    }
    
// Testing privates

void UT_CAmrPayloadFormatWrite::UT_CAmrPayloadFormatWrite_ProcessFramesLL(  )
    {
    TBool isItTrue;
    
    if ( !iAlloc )
        {
        iWrite->iPayloadEncoder->SetFrameBlockCount( 2 );
        
        TDes8& srcDes = ( static_cast<CMMFDataBuffer*>( iWrite->iSourceBuffer )->Data() );
        srcDes.SetLength( 0 );
        iWrite->ProcessFramesL( );
        
        // Last buffer
        iWrite->iSourceBuffer->SetLastBuffer( ETrue );
        srcDes.SetLength( 0 );
        iWrite->ProcessFramesL( );
        
        // Try to re-encode
        iWrite->iPayloadEncoder->iFrameIndex = 0;
        iWrite->iPayloadEncoder->iRedCount = 1;
        iWrite->ProcessFramesL( );
        
        // No sink buffer
        iWrite->iPayloadEncoder->iFrameIndex = 0;
        iWrite->iPayloadEncoder->iRedCount = 1;
        CMMFDataBuffer* tmpBuffer = iWrite->iSinkBuffer;
        iWrite->iSinkBuffer = NULL;
        CleanupStack::PushL(tmpBuffer);
        iWrite->ProcessFramesL( );
        
        // Fill some dummy data 
        iWrite->iPayloadEncoder->iFrameIndex = 0;
        iWrite->iPayloadEncoder->iRedCount = 0;
        iWrite->iSinkBuffer = tmpBuffer;
        CleanupStack::Pop(tmpBuffer);
        srcDes.Append( _L("du") );
        iWrite->ProcessFramesL( );
        
        // Two "real" data frames
        iWrite->iSourceBuffer->SetLastBuffer( EFalse );
        srcDes.SetLength( 32 );
        srcDes[ 0 ] = (TUint8)( EAmrFrame12_2 << 3 );  
        iWrite->ProcessFramesL( ); 
        iWrite->ProcessFramesL( ); 
        
        // There's no sinkbuffer
        iWrite->ProcessFramesL( ); 
        tmpBuffer = iWrite->iSinkBuffer;
        iWrite->iSinkBuffer = NULL;
        iWrite->ProcessFramesL( ); 
        
        // Second frame is lastbuffer
        iWrite->iSinkBuffer = tmpBuffer;
        iWrite->ProcessFramesL( ); 
        iWrite->iSourceBuffer->SetLastBuffer( ETrue );
        iWrite->ProcessFramesL( ); 
        
        // No data frame
        iWrite->iSinkBuffer = tmpBuffer;
        srcDes.SetLength( 32 );
        srcDes[ 0 ] = (TUint8)( EAmrFrameNoData << 3 );   
        iWrite->ProcessFramesL( );
        }
    
    else 
        {
        isItTrue = iWrite->ProcessFramesL( );
        EUNIT_ASSERT( !isItTrue );
        }
    }
    
void UT_CAmrPayloadFormatWrite::UT_CAmrPayloadFormatWrite_DeliverPacketLL( )
    {
    EUNIT_ASSERT_NO_LEAVE( iWrite->DeliverPacketL( *iBuff ) );
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CAmrPayloadFormatWrite,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "CancelUlRequest - test ",
    "CAmrPayloadFormatWrite",
    "CancelUlRequest",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadFormatWrite_CancelUlRequestL, Teardown)

EUNIT_TEST(
    "EmptySourceBufferL - test ",
    "CAmrPayloadFormatWrite",
    "EmptySourceBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadFormatWrite_EmptySourceBufferLL, Teardown )

EUNIT_TEST(
    "SourceBufferEmptiedL - test ",
    "CAmrPayloadFormatWrite",
    "SourceBufferEmptiedL",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadFormatWrite_SourceBufferEmptiedLL, Teardown)
   
EUNIT_TEST(
    "FrameTimeInterval - test ",
    "CAmrPayloadFormatWrite",
    "FrameTimeInterval",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadFormatWrite_FrameTimeIntervalL, Teardown)

EUNIT_TEST(
    "Duration - test ",
    "CAmrPayloadFormatWrite",
    "Duration",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadFormatWrite_DurationL, Teardown)
    
EUNIT_TEST(
    "NumChannels - test ",
    "CAmrPayloadFormatWrite",
    "NumChannels",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadFormatWrite_NumChannelsL, Teardown)

EUNIT_TEST(
    "SampleRate - test ",
    "CAmrPayloadFormatWrite",
    "SampleRate",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadFormatWrite_SampleRateL, Teardown)

EUNIT_TEST(
    "SetSampleRate - test ",
    "CAmrPayloadFormatWrite",
    "SetSampleRate",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadFormatWrite_SetSampleRateL, Teardown)

EUNIT_TEST(
    "CreateSinkBufferL - test ",
    "CAmrPayloadFormatWrite",
    "CreateSinkBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadFormatWrite_CreateSinkBufferLL, Teardown)
   
EUNIT_TEST(
    "SinkDataTypeCode - test ",
    "CAmrPayloadFormatWrite",
    "SinkDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadFormatWrite_SinkDataTypeCodeL, Teardown)
    
EUNIT_TEST(
    "SetSinkDataTypeCode - test ",
    "CAmrPayloadFormatWrite",
    "SetSinkDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadFormatWrite_SetSinkDataTypeCodeL, Teardown)
    
EUNIT_TEST(
    "SinkThreadLogon - test ",
    "CAmrPayloadFormatWrite",
    "SinkThreadLogon",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadFormatWrite_SinkThreadLogonL, Teardown)

EUNIT_TEST(
    "SinkThreadLogoff - test ",
    "CAmrPayloadFormatWrite",
    "SinkThreadLogoff",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadFormatWrite_SinkThreadLogoffL, Teardown)

EUNIT_TEST(
    "EmptyBufferL - test ",
    "CAmrPayloadFormatWrite",
    "EmptyBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadFormatWrite_EmptyBufferLL, Teardown)

EUNIT_TEST(
    "SinkPrimeL - test ",
    "CAmrPayloadFormatWrite",
    "SinkPrimeL",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadFormatWrite_SinkPrimeLL, Teardown)

EUNIT_TEST(
    "SinkPauseL - test ",
    "CAmrPayloadFormatWrite",
    "SinkPauseL",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadFormatWrite_SinkPauseLL, Teardown)

EUNIT_TEST(
    "SinkStopL - test ",
    "CAmrPayloadFormatWrite",
    "SinkStopL",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadFormatWrite_SinkStopLL, Teardown)

EUNIT_TEST(
    "ConfigurePayloadFormatL - test ",
    "CAmrPayloadFormatWrite",
    "ConfigurePayloadFormatL",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadFormatWrite_ConfigurePayloadFormatLL, Teardown)

    
EUNIT_TEST(
    "ProcessFrames - test ",
    "CAmrPayloadFormatWrite",
    "ProcessFramesL",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadFormatWrite_ProcessFramesLL, Teardown)

EUNIT_TEST(
    "DeliverPacket - test ",
    "CAmrPayloadFormatWrite",
    "DeliverPacketL",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadFormatWrite_DeliverPacketLL, Teardown)

EUNIT_TEST(
    "IsBitrateChangeValid - test ",
    "CAmrPayloadFormatWrite",
    "IsBitrateChangeValid",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadFormatWrite_IsBitrateChangeValidL, Teardown)
     
// Wide band
EUNIT_TEST(
    "CancelUlRequest - Wb test ",
    "CAmrPayloadFormatWrite",
    "CancelUlRequest",
    "FUNCTIONALITY",
    SetupWbL, UT_CAmrPayloadFormatWrite_CancelUlRequestL, Teardown)

EUNIT_TEST(
    "EmptySourceBufferL - Wb test ",
    "CAmrPayloadFormatWrite",
    "EmptySourceBufferL",
    "FUNCTIONALITY",
    SetupWbL, UT_CAmrPayloadFormatWrite_EmptySourceBufferLL, Teardown )

EUNIT_TEST(
    "SourceBufferEmptiedL - Wb test ",
    "CAmrPayloadFormatWrite",
    "SourceBufferEmptiedL",
    "FUNCTIONALITY",
    SetupWbL, UT_CAmrPayloadFormatWrite_SourceBufferEmptiedLL, Teardown)
   
EUNIT_TEST(
    "FrameTimeInterval - Wb test ",
    "CAmrPayloadFormatWrite",
    "FrameTimeInterval",
    "FUNCTIONALITY",
    SetupWbL, UT_CAmrPayloadFormatWrite_FrameTimeIntervalL, Teardown)

EUNIT_TEST(
    "Duration - Wb test ",
    "CAmrPayloadFormatWrite",
    "Duration",
    "FUNCTIONALITY",
    SetupWbL, UT_CAmrPayloadFormatWrite_DurationL, Teardown)
    
EUNIT_TEST(
    "NumChannels - Wb test ",
    "CAmrPayloadFormatWrite",
    "NumChannels",
    "FUNCTIONALITY",
    SetupWbL, UT_CAmrPayloadFormatWrite_NumChannelsL, Teardown)

EUNIT_TEST(
    "SampleRate - Wb test ",
    "CAmrPayloadFormatWrite",
    "SampleRate",
    "FUNCTIONALITY",
    SetupWbL, UT_CAmrPayloadFormatWrite_SampleRateL, Teardown)

EUNIT_TEST(
    "SetSampleRate - Wb test ",
    "CAmrPayloadFormatWrite",
    "SetSampleRate",
    "FUNCTIONALITY",
    SetupWbL, UT_CAmrPayloadFormatWrite_SetSampleRateWbL, Teardown)

EUNIT_TEST(
    "CreateSinkBufferL - Wb test ",
    "CAmrPayloadFormatWrite",
    "CreateSinkBufferL",
    "FUNCTIONALITY",
    SetupWbL, UT_CAmrPayloadFormatWrite_CreateSinkBufferLL, Teardown)
   
EUNIT_TEST(
    "SinkDataTypeCode - Wb test ",
    "CAmrPayloadFormatWrite",
    "SinkDataTypeCode",
    "FUNCTIONALITY",
    SetupWbL, UT_CAmrPayloadFormatWrite_SinkDataTypeCodeL, Teardown)
    
EUNIT_TEST(
    "SetSinkDataTypeCode - Wb test ",
    "CAmrPayloadFormatWrite",
    "SetSinkDataTypeCode",
    "FUNCTIONALITY",
    SetupWbL, UT_CAmrPayloadFormatWrite_SetSinkDataTypeCodeL, Teardown)
    
EUNIT_TEST(
    "SinkThreadLogon - Wb test ",
    "CAmrPayloadFormatWrite",
    "SinkThreadLogon",
    "FUNCTIONALITY",
    SetupWbL, UT_CAmrPayloadFormatWrite_SinkThreadLogonL, Teardown)

EUNIT_TEST(
    "SinkThreadLogoff - Wb test ",
    "CAmrPayloadFormatWrite",
    "SinkThreadLogoff",
    "FUNCTIONALITY",
    SetupWbL, UT_CAmrPayloadFormatWrite_SinkThreadLogoffL, Teardown)

EUNIT_TEST(
    "EmptyBufferL - Wb test ",
    "CAmrPayloadFormatWrite",
    "EmptyBufferL",
    "FUNCTIONALITY",
    SetupWbL, UT_CAmrPayloadFormatWrite_EmptyBufferLL, Teardown)

EUNIT_TEST(
    "SinkPrimeL - Wb test ",
    "CAmrPayloadFormatWrite",
    "SinkPrimeL",
    "FUNCTIONALITY",
    SetupWbL, UT_CAmrPayloadFormatWrite_SinkPrimeLL, Teardown)

EUNIT_TEST(
    "SinkPauseL - Wb test ",
    "CAmrPayloadFormatWrite",
    "SinkPauseL",
    "FUNCTIONALITY",
    SetupWbL, UT_CAmrPayloadFormatWrite_SinkPauseLL, Teardown)

EUNIT_TEST(
    "SinkStopL - Wb test ",
    "CAmrPayloadFormatWrite",
    "SinkStopL",
    "FUNCTIONALITY",
    SetupWbL, UT_CAmrPayloadFormatWrite_SinkStopLL, Teardown)

EUNIT_TEST(
    "ConfigurePayloadFormatL - Wb test ",
    "CAmrPayloadFormatWrite",
    "ConfigurePayloadFormatL",
    "FUNCTIONALITY",
    SetupWbL, UT_CAmrPayloadFormatWrite_ConfigurePayloadFormatLL, Teardown)

    
EUNIT_TEST(
    "ProcessFrames - Wb test ",
    "CAmrPayloadFormatWrite",
    "ProcessFramesL",
    "FUNCTIONALITY",
    SetupWbL, UT_CAmrPayloadFormatWrite_ProcessFramesLL, Teardown)

EUNIT_TEST(
    "DeliverPacket - Wb test ",
    "CAmrPayloadFormatWrite",
    "DeliverPacketL",
    "FUNCTIONALITY",
    SetupWbL, UT_CAmrPayloadFormatWrite_DeliverPacketLL, Teardown)

EUNIT_TEST(
    "IsBitrateChangeValid - Wb test ",
    "CAmrPayloadFormatWrite",
    "IsBitrateChangeValid",
    "FUNCTIONALITY",
    SetupWbL, UT_CAmrPayloadFormatWrite_IsBitrateChangeValidL, Teardown)
     
// ALLOC TESTS

#ifdef EUNIT_ALLOC_TEST_ON


EUNIT_ALLOC_TEST(
    "CancelUlRequest - alloc test ",
    "CAmrPayloadFormatWrite",
    "CancelUlRequest",
    "ERRORHANDLING",
    SetupAL, UT_CAmrPayloadFormatWrite_CancelUlRequestL, Teardown)

EUNIT_ALLOC_TEST(
    "EmptySourceBufferL - alloc test ",
    "CAmrPayloadFormatWrite",
    "EmptySourceBufferL",
    "ERRORHANDLING",
    SetupAL, UT_CAmrPayloadFormatWrite_EmptySourceBufferLL, Teardown )

EUNIT_ALLOC_TEST(
    "SourceBufferEmptiedL - alloc test ",
    "CAmrPayloadFormatWrite",
    "SourceBufferEmptiedL",
    "ERRORHANDLING",
    SetupAL, UT_CAmrPayloadFormatWrite_SourceBufferEmptiedLL, Teardown)
    
EUNIT_ALLOC_TEST(
    "FrameTimeInterval - alloc test ",
    "CAmrPayloadFormatWrite",
    "FrameTimeInterval",
    "ERRORHANDLING",
    SetupAL, UT_CAmrPayloadFormatWrite_FrameTimeIntervalL, Teardown)

EUNIT_ALLOC_TEST(
    "Duration - alloc test ",
    "CAmrPayloadFormatWrite",
    "Duration",
    "ERRORHANDLING",
    SetupAL, UT_CAmrPayloadFormatWrite_DurationL, Teardown)
    
EUNIT_ALLOC_TEST(
    "NumChannels - alloc test ",
    "CAmrPayloadFormatWrite",
    "NumChannels",
    "ERRORHANDLING",
    SetupAL, UT_CAmrPayloadFormatWrite_NumChannelsL, Teardown)

EUNIT_ALLOC_TEST(
    "SampleRate - alloc test ",
    "CAmrPayloadFormatWrite",
    "SampleRate",
    "ERRORHANDLING",
    SetupAL, UT_CAmrPayloadFormatWrite_SampleRateL, Teardown)

EUNIT_ALLOC_TEST(
    "SetSampleRate - alloc test ",
    "CAmrPayloadFormatWrite",
    "SetSampleRate",
    "ERRORHANDLING",
    SetupAL, UT_CAmrPayloadFormatWrite_SetSampleRateL, Teardown)

EUNIT_ALLOC_TEST(
    "CreateSinkBufferL - alloc test ",
    "CAmrPayloadFormatWrite",
    "CreateSinkBufferL",
    "ERRORHANDLING",
    SetupAL, UT_CAmrPayloadFormatWrite_CreateSinkBufferLL, Teardown)
    
EUNIT_ALLOC_TEST(
    "SinkDataTypeCode - alloc test ",
    "CAmrPayloadFormatWrite",
    "SinkDataTypeCode",
    "ERRORHANDLING",
    SetupAL, UT_CAmrPayloadFormatWrite_SinkDataTypeCodeL, Teardown)
    
EUNIT_ALLOC_TEST(
    "SetSinkDataTypeCode - alloc test ",
    "CAmrPayloadFormatWrite",
    "SetSinkDataTypeCode",
    "ERRORHANDLING",
    SetupAL, UT_CAmrPayloadFormatWrite_SetSinkDataTypeCodeL, Teardown)
    
EUNIT_ALLOC_TEST(
    "SinkThreadLogon - alloc test ",
    "CAmrPayloadFormatWrite",
    "SinkThreadLogon",
    "ERRORHANDLING",
    SetupAL, UT_CAmrPayloadFormatWrite_SinkThreadLogonL, Teardown)

EUNIT_ALLOC_TEST(
    "SinkThreadLogoff - alloc test ",
    "CAmrPayloadFormatWrite",
    "SinkThreadLogoff",
    "ERRORHANDLING",
    SetupAL, UT_CAmrPayloadFormatWrite_SinkThreadLogoffL, Teardown)

EUNIT_ALLOC_TEST(
    "EmptyBufferL - alloc test ",
    "CAmrPayloadFormatWrite",
    "EmptyBufferL",
    "ERRORHANDLING",
    SetupAL, UT_CAmrPayloadFormatWrite_EmptyBufferLL, Teardown)

EUNIT_ALLOC_TEST( // Basic test missing because no content, only alloc test committed
    "BufferEmptiedL - Alloc test ",
    "CAmrPayloadFormatWrite",
    "BufferEmptiedL",
    "ERRORHANDLING",
    SetupAL, UT_CAmrPayloadFormatWrite_BufferEmptiedLL, Teardown)

EUNIT_ALLOC_TEST(
    "SinkPrimeL - alloc test ",
    "CAmrPayloadFormatWrite",
    "SinkPrimeL",
    "ERRORHANDLING",
    SetupAL, UT_CAmrPayloadFormatWrite_SinkPrimeLL, Teardown)

EUNIT_ALLOC_TEST(
    "SinkPauseL - alloc test ",
    "CAmrPayloadFormatWrite",
    "SinkPauseL",
    "ERRORHANDLING",
    SetupAL, UT_CAmrPayloadFormatWrite_SinkPauseLL, Teardown)

EUNIT_ALLOC_TEST(
    "SinkStopL - alloc test ",
    "CAmrPayloadFormatWrite",
    "SinkStopL",
    "ERRORHANDLING",
    SetupAL, UT_CAmrPayloadFormatWrite_SinkStopLL, Teardown)
    
EUNIT_ALLOC_TEST(
    "ConfigurePayloadFormatL - alloc test ",
    "CAmrPayloadFormatWrite",
    "ConfigurePayloadFormatL",
    "ERRORHANDLING",
    SetupAL, UT_CAmrPayloadFormatWrite_ConfigurePayloadFormatLL, Teardown)
    
EUNIT_ALLOC_TEST(
    "ProcessFrames - alloc test ",
    "CAmrPayloadFormatWrite",
    "ProcessFramesL",
    "ERRORHANDLING",
    SetupAL, UT_CAmrPayloadFormatWrite_ProcessFramesLL, Teardown)

EUNIT_ALLOC_TEST(
    "DeliverPacket - alloc test ",
    "CAmrPayloadFormatWrite",
    "DeliverPacketL",
    "ERRORHANDLING",
    SetupAL, UT_CAmrPayloadFormatWrite_DeliverPacketLL, Teardown)

// wide band

EUNIT_ALLOC_TEST(
    "CancelUlRequest - Wb alloc test ",
    "CAmrPayloadFormatWrite",
    "CancelUlRequest",
    "ERRORHANDLING",
    SetupWbAL, UT_CAmrPayloadFormatWrite_CancelUlRequestL, Teardown)

EUNIT_ALLOC_TEST(
    "EmptySourceBufferL - Wb alloc test ",
    "CAmrPayloadFormatWrite",
    "EmptySourceBufferL",
    "ERRORHANDLING",
    SetupWbAL, UT_CAmrPayloadFormatWrite_EmptySourceBufferLL, Teardown )

EUNIT_ALLOC_TEST(
    "SourceBufferEmptiedL - Wb alloc test ",
    "CAmrPayloadFormatWrite",
    "SourceBufferEmptiedL",
    "ERRORHANDLING",
    SetupWbAL, UT_CAmrPayloadFormatWrite_SourceBufferEmptiedLL, Teardown)
    
EUNIT_ALLOC_TEST(
    "FrameTimeInterval - Wb alloc test ",
    "CAmrPayloadFormatWrite",
    "FrameTimeInterval",
    "ERRORHANDLING",
    SetupWbAL, UT_CAmrPayloadFormatWrite_FrameTimeIntervalL, Teardown)

EUNIT_ALLOC_TEST(
    "Duration - Wb alloc test ",
    "CAmrPayloadFormatWrite",
    "Duration",
    "ERRORHANDLING",
    SetupWbAL, UT_CAmrPayloadFormatWrite_DurationL, Teardown)
    
EUNIT_ALLOC_TEST(
    "NumChannels - Wb alloc test ",
    "CAmrPayloadFormatWrite",
    "NumChannels",
    "ERRORHANDLING",
    SetupWbAL, UT_CAmrPayloadFormatWrite_NumChannelsL, Teardown)

EUNIT_ALLOC_TEST(
    "SampleRate - Wb alloc test ",
    "CAmrPayloadFormatWrite",
    "SampleRate",
    "ERRORHANDLING",
    SetupWbAL, UT_CAmrPayloadFormatWrite_SampleRateL, Teardown)

EUNIT_ALLOC_TEST(
    "SetSampleRate - Wb alloc test ",
    "CAmrPayloadFormatWrite",
    "SetSampleRate",
    "ERRORHANDLING",
    SetupWbAL, UT_CAmrPayloadFormatWrite_SetSampleRateL, Teardown)

EUNIT_ALLOC_TEST(
    "CreateSinkBufferL - Wb alloc test ",
    "CAmrPayloadFormatWrite",
    "CreateSinkBufferL",
    "ERRORHANDLING",
    SetupWbAL, UT_CAmrPayloadFormatWrite_CreateSinkBufferLL, Teardown)
    
EUNIT_ALLOC_TEST(
    "SinkDataTypeCode - Wb alloc test ",
    "CAmrPayloadFormatWrite",
    "SinkDataTypeCode",
    "ERRORHANDLING",
    SetupWbAL, UT_CAmrPayloadFormatWrite_SinkDataTypeCodeL, Teardown)
    
EUNIT_ALLOC_TEST(
    "SetSinkDataTypeCode - Wb alloc test ",
    "CAmrPayloadFormatWrite",
    "SetSinkDataTypeCode",
    "ERRORHANDLING",
    SetupWbAL, UT_CAmrPayloadFormatWrite_SetSinkDataTypeCodeL, Teardown)
    
EUNIT_ALLOC_TEST(
    "SinkThreadLogon - Wb alloc test ",
    "CAmrPayloadFormatWrite",
    "SinkThreadLogon",
    "ERRORHANDLING",
    SetupWbAL, UT_CAmrPayloadFormatWrite_SinkThreadLogonL, Teardown)

EUNIT_ALLOC_TEST(
    "SinkThreadLogoff - Wb alloc test ",
    "CAmrPayloadFormatWrite",
    "SinkThreadLogoff",
    "ERRORHANDLING",
    SetupWbAL, UT_CAmrPayloadFormatWrite_SinkThreadLogoffL, Teardown)

EUNIT_ALLOC_TEST(
    "EmptyBufferL - Wb alloc test ",
    "CAmrPayloadFormatWrite",
    "EmptyBufferL",
    "ERRORHANDLING",
    SetupWbAL, UT_CAmrPayloadFormatWrite_EmptyBufferLL, Teardown)

EUNIT_ALLOC_TEST( // Basic test missing because no content, only alloc test committed
    "BufferEmptiedL - Wb Alloc test ",
    "CAmrPayloadFormatWrite",
    "BufferEmptiedL",
    "ERRORHANDLING",
    SetupWbAL, UT_CAmrPayloadFormatWrite_BufferEmptiedLL, Teardown)

EUNIT_ALLOC_TEST(
    "SinkPrimeL - Wb alloc test ",
    "CAmrPayloadFormatWrite",
    "SinkPrimeL",
    "ERRORHANDLING",
    SetupWbAL, UT_CAmrPayloadFormatWrite_SinkPrimeLL, Teardown)

EUNIT_ALLOC_TEST(
    "SinkPauseL - Wb alloc test ",
    "CAmrPayloadFormatWrite",
    "SinkPauseL",
    "ERRORHANDLING",
    SetupWbAL, UT_CAmrPayloadFormatWrite_SinkPauseLL, Teardown)

EUNIT_ALLOC_TEST(
    "SinkStopL - Wb alloc test ",
    "CAmrPayloadFormatWrite",
    "SinkStopL",
    "ERRORHANDLING",
    SetupWbAL, UT_CAmrPayloadFormatWrite_SinkStopLL, Teardown)
    
EUNIT_ALLOC_TEST(
    "ConfigurePayloadFormatL - Wb alloc test ",
    "CAmrPayloadFormatWrite",
    "ConfigurePayloadFormatL",
    "ERRORHANDLING",
    SetupWbAL, UT_CAmrPayloadFormatWrite_ConfigurePayloadFormatLL, Teardown)
    
EUNIT_ALLOC_TEST(
    "ProcessFrames - Wb alloc test ",
    "CAmrPayloadFormatWrite",
    "ProcessFramesL",
    "ERRORHANDLING",
    SetupWbAL, UT_CAmrPayloadFormatWrite_ProcessFramesLL, Teardown)

EUNIT_ALLOC_TEST(
    "DeliverPacket - Wb alloc test ",
    "CAmrPayloadFormatWrite",
    "DeliverPacketL",
    "ERRORHANDLING",
    SetupWbAL, UT_CAmrPayloadFormatWrite_DeliverPacketLL, Teardown)

#endif

EUNIT_END_TEST_TABLE

//  END OF FILE
