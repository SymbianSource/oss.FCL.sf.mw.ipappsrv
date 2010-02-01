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
#include "UT_CG729PayloadFormatWrite.h"

//  EXTERNAL INCLUDES
#include <e32base.h>
#include <digia/eunit/eunitmacros.h>

#ifdef EUNIT_ALLOC_TEST_ON
    #include <digia/eunit/CEunitAllocTestCaseDecorator.h>   //for using EUNIT_ALLOC_TEST
#endif

//  INTERNAL INCLUDES
#include "G729PayloadFormatWrite.h"
#include "MccRtpDataSink.h"
#include <mmf/server/mmfdatasink.h>
#include <mmf/server/mmfdatasource.h>
#include "Mcculdatapath.h"
#include "mccuids.hrh"
#include "mccrtpmediaclock.h"
#include "mccresourcepool.h"
#include "mccredpayloadwrite.h"
#include "g729payloadformatutil.h" 


const TUid KRtpDataSink              =  { KImplUidRtpDataSink               };
const TUid KRtpDataSource            =  { KImplUidRtpDataSource             };


// CONSTRUCTION
UT_CG729PayloadFormatWrite* UT_CG729PayloadFormatWrite::NewL()
    {
    UT_CG729PayloadFormatWrite* self = UT_CG729PayloadFormatWrite::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CG729PayloadFormatWrite* UT_CG729PayloadFormatWrite::NewLC()
    {
    UT_CG729PayloadFormatWrite* self = new( ELeave ) UT_CG729PayloadFormatWrite();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CG729PayloadFormatWrite::~UT_CG729PayloadFormatWrite()
    {
    }

// Default constructor
UT_CG729PayloadFormatWrite::UT_CG729PayloadFormatWrite()
    {
    }

// Second phase construct
void UT_CG729PayloadFormatWrite::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS
    
void UT_CG729PayloadFormatWrite::SetupAL(  )
    {   
    iAlloc = ETrue;
    
    MAsyncEventHandler* eventHandler = NULL;
    CMccResourcePool* mccresource = NULL;
    iRtpMediaClock = CMccRtpMediaClock::NewL();
     
    iUlSink = CMccUlDataPath::NewL( eventHandler, mccresource, iMediaId ); //MDataSink::NewSinkL( KRtpDataSink, KNullDesC8 );
    iRtpDataSinkStub = static_cast<CMccRtpDataSink*>(MDataSink::NewSinkL( KRtpDataSink, KNullDesC8 ));  
    iWrite = CG729PayloadFormatWrite::NewL( iRtpDataSinkStub );
    
    iCodec.iAlgoUsed = 0;
    iCodec.iBitrate = 8000;
    iCodec.iCNFrameSize = 0;
    iCodec.iCodecMode = 3;
    iCodec.iEnableDTX = 1;
    iCodec.iFourCC = 0x39323747;
    iCodec.iFrameSize = 0;
    iCodec.iHwFrameTime = 20;
    iCodec.iMaxPtime = 200;
    iCodec.iPayloadType = 8;
    iCodec.iPtime = 20;
    iCodec.iRedundancyCount = 0;
    iCodec.iRedundantPayload = 0;
    
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
    
void UT_CG729PayloadFormatWrite::Setup2AL(  )
    {
     // not call CreateSinkBufferL
        
    iAlloc = ETrue;
    
    MAsyncEventHandler* eventHandler = NULL;
    CMccResourcePool* mccresource = NULL;
    iRtpMediaClock = CMccRtpMediaClock::NewL();
     
    iUlSink = CMccUlDataPath::NewL( eventHandler, mccresource, iMediaId ); //MDataSink::NewSinkL( KRtpDataSink, KNullDesC8 );
    iRtpDataSinkStub = static_cast<CMccRtpDataSink*>(MDataSink::NewSinkL( KRtpDataSink, KNullDesC8 ));  
    iWrite = CG729PayloadFormatWrite::NewL( iRtpDataSinkStub );
    
    iCodec.iAlgoUsed = 0;
    iCodec.iBitrate = 8000;
    iCodec.iCNFrameSize = 0;
    iCodec.iCodecMode = 3;
    iCodec.iEnableDTX = 1;
    iCodec.iFourCC = 0x39323747;
    iCodec.iFrameSize = 0;
    iCodec.iHwFrameTime = 20;
    iCodec.iMaxPtime = 200;
    iCodec.iPayloadType = 8;
    iCodec.iPtime = 20;
    iCodec.iRedundancyCount = 0;
    iCodec.iRedundantPayload = 0;
    
    TMccCodecInfoBuffer buffer( iCodec );
    
    iWrite->ConfigurePayloadFormatL( buffer, *iRtpMediaClock );
    
    iBuff = CMMFDescriptorBuffer::NewL( 10 );

    TMediaId mediaId( KUidMediaTypeAudio, 1 );
    
    iWrite->SinkPrimeL();
    iWrite->SinkPlayL( );
    
    }
        


void UT_CG729PayloadFormatWrite::SetupL(  )
    {
    iAlloc = EFalse;
    
    MAsyncEventHandler* eventHandler = NULL;
    CMccResourcePool* mccresource = NULL;
    iRtpMediaClock = CMccRtpMediaClock::NewL();
     
    iUlSink = CMccUlDataPath::NewL( eventHandler, mccresource, iMediaId ); //MDataSink::NewSinkL( KRtpDataSink, KNullDesC8 );
    iRtpDataSinkStub = static_cast<CMccRtpDataSink*>(MDataSink::NewSinkL( KRtpDataSink, KNullDesC8 ));  
    iWrite = CG729PayloadFormatWrite::NewL( iRtpDataSinkStub );
    
    iCodec.iAlgoUsed = 0;
    iCodec.iBitrate = 8000;
    iCodec.iCNFrameSize = 0;
    iCodec.iCodecMode = 3;
    iCodec.iEnableDTX = 1;
    iCodec.iFourCC = 0x39323747;
    iCodec.iFrameSize = 0;
    iCodec.iHwFrameTime = 20;
    iCodec.iMaxPtime = 200;
    iCodec.iPayloadType = 8;
    iCodec.iPtime = 20;
    iCodec.iRedundancyCount = 0;
    iCodec.iRedundantPayload = 0;
    
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

void UT_CG729PayloadFormatWrite::Teardown()
    {
    delete iUlSink;
    delete iRtpDataSinkStub;
    delete iWrite;
    delete iBuff;
    delete iRtpMediaClock;
    
    REComSession::FinalClose();
    }

void UT_CG729PayloadFormatWrite::UT_CG729PayloadFormatWrite_NewLL(  )
    {
    CG729PayloadFormatWrite* write = CG729PayloadFormatWrite::NewL( iRtpDataSinkStub );
    delete write;
    EUNIT_ASSERT_SPECIFIC_LEAVE( CG729PayloadFormatWrite* write2 = CG729PayloadFormatWrite::NewL( NULL ), KErrArgument );
    }


void UT_CG729PayloadFormatWrite::UT_CG729PayloadFormatWrite_EmptySourceBufferLL(  )
    {
        
    if( !iAlloc ) 
        {
        EUNIT_ASSERT_NO_LEAVE( iWrite->EmptySourceBufferL() );
        
        //Case: Testing DeliverPacketL
        //iWrite->iPayloadTypes.Insert(1,0);
        //EUNIT_ASSERT_EQUALS( iWrite->SetRedundancyL( ETrue, 2, 1 ), KErrNone );
        iWrite->SinkPlayL();
        EUNIT_ASSERT_NO_LEAVE( iWrite->EmptySourceBufferL() );
                     
        }
    
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iWrite->EmptySourceBufferL() );
        
        }
    }

void UT_CG729PayloadFormatWrite::UT_CG729PayloadFormatWrite_SourceBufferEmptiedLL(  )
    {
    if( !iAlloc )  
        {
        EUNIT_ASSERT_NO_LEAVE( iWrite->EmptyBufferL( iBuffer, iUlSink, KUidMediaTypeAudio ) );
        EUNIT_ASSERT_NO_LEAVE( iWrite->SourceBufferEmptiedL( ) );
        }
    
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iWrite->EmptyBufferL( iBuffer, iUlSink, KUidMediaTypeAudio ) );
        EUNIT_ASSERT_NO_LEAVE( iWrite->SourceBufferEmptiedL( ) );
        
        }
    }

void UT_CG729PayloadFormatWrite::UT_CG729PayloadFormatWrite_SinkPrimeLL(  )
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

void UT_CG729PayloadFormatWrite::UT_CG729PayloadFormatWrite_SinkPlayLL(  )
    {
    iWrite->SinkPlayL();     
    }

void UT_CG729PayloadFormatWrite::UT_CG729PayloadFormatWrite_SinkPauseLL(  )
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

void UT_CG729PayloadFormatWrite::UT_CG729PayloadFormatWrite_SinkStopLL(  )
    {
    if ( !iAlloc )
        {
        EUNIT_ASSERT_NO_LEAVE( iWrite->SinkStopL() );
        
        }
    
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iWrite->SinkStopL() );
        
        }
    }
    
void UT_CG729PayloadFormatWrite::UT_CG729PayloadFormatWrite_DoBitPackingL( )
    {
    if ( !iAlloc )
        {
        _LIT( KData, "dummy");
        TBool comfortNoise = EFalse;
        CMMFDataBuffer* source = CMMFDataBuffer::NewL( 170 );  
        CleanupStack::PushL(source);  
        TDes8& sourceBuf = source->Data();
        
        HBufC8* destBuf = HBufC8::NewL( 6 );        
        TPtr8 ptr = destBuf->Des();
        CleanupStack::PushL(destBuf);  
        ptr.Append( KData );        
        
        EUNIT_ASSERT_EQUALS( iWrite->DoBitPacking( sourceBuf, ptr, comfortNoise ), KErrArgument );
        
        CleanupStack::Pop(destBuf); 
        delete destBuf;
        
        source->Data().Append( _L("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789") );
        CMMFDataBuffer* destBuf1 = CMMFDataBuffer::NewL( 20 );
        TDes8& buf = destBuf1->Data();        
        EUNIT_ASSERT_EQUALS( iWrite->DoBitPacking( sourceBuf, buf, comfortNoise ), KErrNone );
        buf.Zero();
         
        CleanupStack::Pop(source);  
        delete destBuf1; 
        delete source;       
        sourceBuf.Zero();              
        }
    
    else
        {
        _LIT( KData, "dummy");
        TBool comfortNoise = EFalse;
        HBufC8* sourceBuf = NULL;
        CleanupStack::PushL(sourceBuf);
         
        HBufC8* destBuf = HBufC8::NewL( 6 );        
        CleanupStack::PushL(destBuf); 
        
        TPtr8 ptr = destBuf->Des();
        ptr.Append( KData );        
        
        EUNIT_ASSERT_EQUALS( iWrite->DoBitPacking( *sourceBuf, ptr, comfortNoise ), KErrArgument );
        CleanupStack::Pop(sourceBuf);
        CleanupStack::Pop(destBuf); 
        
        delete destBuf;       
        
        delete sourceBuf;        
        }    
    }

void UT_CG729PayloadFormatWrite::UT_CG729PayloadFormatWrite_FrameTimeIntervalL(  )
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

void UT_CG729PayloadFormatWrite::UT_CG729PayloadFormatWrite_DurationL(  )
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

void UT_CG729PayloadFormatWrite::UT_CG729PayloadFormatWrite_CreateSinkBufferLL(  )
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

void UT_CG729PayloadFormatWrite::UT_CG729PayloadFormatWrite_SinkDataTypeCodeL(  )
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

void UT_CG729PayloadFormatWrite::UT_CG729PayloadFormatWrite_SetSinkDataTypeCodeL(  )
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

void UT_CG729PayloadFormatWrite::UT_CG729PayloadFormatWrite_SinkThreadLogonL(  )
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

void UT_CG729PayloadFormatWrite::UT_CG729PayloadFormatWrite_SinkThreadLogoffL(  )
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

void UT_CG729PayloadFormatWrite::UT_CG729PayloadFormatWrite_EmptyBufferLL(  )
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

void UT_CG729PayloadFormatWrite::UT_CG729PayloadFormatWrite_BufferEmptiedLL(  )
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

void UT_CG729PayloadFormatWrite::UT_CG729PayloadFormatWrite_NumChannelsL(  )
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

void UT_CG729PayloadFormatWrite::UT_CG729PayloadFormatWrite_SampleRateL(  )
    {
    TUint samplerate;
    
    if ( !iAlloc )
        {
        EUNIT_ASSERT_NO_LEAVE( samplerate = iWrite->SampleRate( ) );
        }
    
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iWrite->SampleRate( ) );
        
        }
    }

void UT_CG729PayloadFormatWrite::UT_CG729PayloadFormatWrite_SetSampleRateL(  )
    {
    TInt rate( 8000 );
    if ( !iAlloc )
        {
        EUNIT_ASSERT( iWrite->SetSampleRate( rate ) == KErrNone );
        EUNIT_ASSERT( iWrite->SetSampleRate( rate + 1 ) == KErrNotSupported );
        iAlloc = ETrue;
        }
    
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iWrite->SetSampleRate( rate ) );
        EUNIT_ASSERT_NO_LEAVE( iWrite->SetSampleRate( rate + 1 ) );
        
        }
    }

void UT_CG729PayloadFormatWrite::UT_CG729PayloadFormatWrite_ConfigurePayloadFormatLL(  )
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
        codec.iHwFrameTime = 20;
        codec.iMaxPtime = 200;
        codec.iPayloadType = 8;
        codec.iPtime = 20;
        codec.iRedundancyCount = 0;
        codec.iRedundantPayload = 0;
        
        TMccCodecInfoBuffer buffer( codec );

        EUNIT_ASSERT_NO_LEAVE( iWrite->ConfigurePayloadFormatL( buffer, *iRtpMediaClock ));
        codec.iIsUpdate = EFalse;
        EUNIT_ASSERT_NO_LEAVE( iWrite->ConfigurePayloadFormatL( buffer, *iRtpMediaClock ));
        
        codec.iIsUpdate = ETrue;        
        codec.iAlgoUsed = 2;
        codec.iRedundancyCount = 1;
        buffer = codec;         
        CMccRtpDataSink* rtpDataSinkStub = static_cast<CMccRtpDataSink*>(MDataSink::NewSinkL( KRtpDataSink, KNullDesC8 ));
        CleanupStack::PushL( rtpDataSinkStub );
        CMccRedPayloadWrite* redEncoder = CMccRedPayloadWrite::NewL( rtpDataSinkStub );
        CleanupStack::PushL( redEncoder );
        CG729PayloadFormatWrite* write = CG729PayloadFormatWrite::NewL( redEncoder );
        CleanupStack::PushL( write );
        write->iCInfo.iHwFrameTime = 20;
        write->ConfigurePayloadFormatL( buffer, *iRtpMediaClock );
        CleanupStack::PopAndDestroy( 3 ); 
        }
    else
        {
        EUNIT_ASSERT( ETrue );
        }

    }
    
void UT_CG729PayloadFormatWrite::UT_CG729PayloadFormatWrite_UpdateConfigurationLL(  )
    {
    
    if ( !iAlloc)
        {
        CG729PayloadFormatWrite* write = CG729PayloadFormatWrite::NewL( iRtpDataSinkStub );
        CleanupStack::PushL( write );             
                
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

        write->iCInfo.iHwFrameTime = 20;
        write->UpdateConfigurationL( codec );
        CleanupStack::PopAndDestroy( write );
        }
    else
        {
        EUNIT_ASSERT( ETrue );
        }

    }    


void UT_CG729PayloadFormatWrite::UT_CG729PayloadFormatWrite_DeliverPacketL(  )
    {
    iBuff->Data().FillZ( 10 );        
    EUNIT_ASSERT_NO_LEAVE( iWrite->DeliverPacketL( *iBuff ) );
    
    iBuff->Data().FillZ( 10 );
    iWrite->iIsRtpSink = EFalse;
    EUNIT_ASSERT_NO_LEAVE( iWrite->DeliverPacketL( *iBuff ) );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CG729PayloadFormatWrite,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CG729PayloadFormatWrite",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatWrite_NewLL, Teardown)

EUNIT_TEST(
    "EmptySourceBufferL - test ",
    "CG729PayloadFormatWrite",
    "EmptySourceBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatWrite_EmptySourceBufferLL, Teardown)


EUNIT_TEST(
    "SourceBufferEmptiedL - test ",
    "CG729PayloadFormatWrite",
    "SourceBufferEmptiedL",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatWrite_SourceBufferEmptiedLL, Teardown)

EUNIT_TEST(
    "SinkPrimeL - test ",
    "CG729PayloadFormatWrite",
    "SinkPrimeL",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatWrite_SinkPrimeLL, Teardown)

EUNIT_TEST(
    "SinkPlayL - test ",
    "CG729PayloadFormatWrite",
    "SinkPlayL",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatWrite_SinkPlayLL, Teardown)


EUNIT_TEST(
    "SinkPauseL - test ",
    "CG729PayloadFormatWrite",
    "SinkPauseL",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatWrite_SinkPauseLL, Teardown)

EUNIT_TEST(
    "SinkStopL - test ",
    "CG729PayloadFormatWrite",
    "SinkStopL",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatWrite_SinkStopLL, Teardown)
    
EUNIT_TEST(
    "DoBitPacking - test ",
    "CG729PayloadFormatWrite",
    "DoBitPacking",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatWrite_DoBitPackingL, Teardown)   

EUNIT_TEST(
    "FrameTimeInterval - test ",
    "CG729PayloadFormatWrite",
    "FrameTimeInterval",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatWrite_FrameTimeIntervalL, Teardown)


EUNIT_TEST(
    "Duration - test ",
    "CG729PayloadFormatWrite",
    "Duration",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatWrite_DurationL, Teardown)


EUNIT_TEST(
    "CreateSinkBufferL - test ",
    "CG729PayloadFormatWrite",
    "CreateSinkBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatWrite_CreateSinkBufferLL, Teardown)


EUNIT_TEST(
    "SinkDataTypeCode - test ",
    "CG729PayloadFormatWrite",
    "SinkDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatWrite_SinkDataTypeCodeL, Teardown)


EUNIT_TEST(
    "SetSinkDataTypeCode - test ",
    "CG729PayloadFormatWrite",
    "SetSinkDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatWrite_SetSinkDataTypeCodeL, Teardown)


EUNIT_TEST(
    "SinkThreadLogon - test ",
    "CG729PayloadFormatWrite",
    "SinkThreadLogon",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatWrite_SinkThreadLogonL, Teardown)

EUNIT_TEST(
    "SinkThreadLogoff - test ",
    "CG729PayloadFormatWrite",
    "SinkThreadLogoff",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatWrite_SinkThreadLogoffL, Teardown)


EUNIT_TEST(
    "EmptyBufferL - test ",
    "CG729PayloadFormatWrite",
    "EmptyBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatWrite_EmptyBufferLL, Teardown)


EUNIT_TEST(
    "NumChannels - test ",
    "CG729PayloadFormatWrite",
    "NumChannels",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatWrite_NumChannelsL, Teardown)

EUNIT_TEST(
    "SampleRate - test ",
    "CG729PayloadFormatWrite",
    "SampleRate",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatWrite_SampleRateL, Teardown)

EUNIT_TEST(
    "SetSampleRate - test ",
    "CG729PayloadFormatWrite",
    "SetSampleRate",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatWrite_SetSampleRateL, Teardown)

EUNIT_TEST(
    "ConfigurePayloadFormatL - test ",
    "CG729PayloadFormatWrite",
    "ConfigurePayloadFormatL",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatWrite_ConfigurePayloadFormatLL, Teardown)
    
EUNIT_TEST(
    "UpdateConfigurationL - test ",
    "CG729PayloadFormatWrite",
    "UpdateConfigurationL",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatWrite_UpdateConfigurationLL, Teardown)  

EUNIT_TEST(
    "DeliverPacket - test ",
    "CG729PayloadFormatWrite",
    "DeliverPacket",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatWrite_DeliverPacketL, Teardown)


// ALLOC TESTS
#ifdef EUNIT_ALLOC_TEST_ON

EUNIT_TEST(
    "NewL - test ",
    "CG729PayloadFormatWrite",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatWrite_NewLL, Teardown)

EUNIT_ALLOC_TEST(
    "EmptySourceBufferL - alloc test ",
    "CG729PayloadFormatWrite",
    "EmptySourceBufferL",
    "ERRORHANDLING",
    SetupAL, UT_CG729PayloadFormatWrite_EmptySourceBufferLL, Teardown)


EUNIT_ALLOC_TEST(
    "SourceBufferEmptiedL - alloc test ",
    "CG729PayloadFormatWrite",
    "SourceBufferEmptiedL",
    "ERRORHANDLING",
    SetupAL, UT_CG729PayloadFormatWrite_SourceBufferEmptiedLL, Teardown)

EUNIT_ALLOC_TEST(
    "SinkPrimeL - alloc test ",
    "CG729PayloadFormatWrite",
    "SinkPrimeL",
    "ERRORHANDLING",
    SetupAL, UT_CG729PayloadFormatWrite_SinkPrimeLL, Teardown)

EUNIT_ALLOC_TEST(
    "SinkPlayL - alloc test ",
    "CG729PayloadFormatWrite",
    "SinkPlayL",
    "ERRORHANDLING",
    SetupAL, UT_CG729PayloadFormatWrite_SinkPlayLL, Teardown)


EUNIT_ALLOC_TEST(
    "SinkPauseL - alloc test ",
    "CG729PayloadFormatWrite",
    "SinkPauseL",
    "ERRORHANDLING",
    SetupAL, UT_CG729PayloadFormatWrite_SinkPauseLL, Teardown)

EUNIT_ALLOC_TEST(
    "SinkStopL - alloc test ",
    "CG729PayloadFormatWrite",
    "SinkStopL",
    "ERRORHANDLING",
    SetupAL, UT_CG729PayloadFormatWrite_SinkStopLL, Teardown)
    
EUNIT_TEST(
    "DoBitPacking - test ",
    "CG729PayloadFormatWrite",
    "DoBitPacking",
    "ERRORHANDLING",
    SetupL, UT_CG729PayloadFormatWrite_DoBitPackingL, Teardown)     


EUNIT_ALLOC_TEST(
    "FrameTimeInterval - alloc test ",
    "CG729PayloadFormatWrite",
    "FrameTimeInterval",
    "ERRORHANDLING",
    SetupAL, UT_CG729PayloadFormatWrite_FrameTimeIntervalL, Teardown)


EUNIT_ALLOC_TEST(
    "Duration - alloc test ",
    "CG729PayloadFormatWrite",
    "Duration",
    "ERRORHANDLING",
    SetupAL, UT_CG729PayloadFormatWrite_DurationL, Teardown)


EUNIT_ALLOC_TEST(
    "CreateSinkBufferL - alloc test ",
    "CG729PayloadFormatWrite",
    "CreateSinkBufferL",
    "ERRORHANDLING",
    Setup2AL, UT_CG729PayloadFormatWrite_CreateSinkBufferLL, Teardown)


EUNIT_ALLOC_TEST(
    "SinkDataTypeCode - alloc test ",
    "CG729PayloadFormatWrite",
    "SinkDataTypeCode",
    "ERRORHANDLING",
    SetupAL, UT_CG729PayloadFormatWrite_SinkDataTypeCodeL, Teardown)


EUNIT_ALLOC_TEST(
    "SetSinkDataTypeCode - alloc test ",
    "CG729PayloadFormatWrite",
    "SetSinkDataTypeCode",
    "ERRORHANDLING",
    SetupAL, UT_CG729PayloadFormatWrite_SetSinkDataTypeCodeL, Teardown)


EUNIT_ALLOC_TEST(
    "SinkThreadLogon - alloc test ",
    "CG729PayloadFormatWrite",
    "SinkThreadLogon",
    "ERRORHANDLING",
    SetupAL, UT_CG729PayloadFormatWrite_SinkThreadLogonL, Teardown)

EUNIT_ALLOC_TEST(
    "SinkThreadLogoff - alloc test ",
    "CG729PayloadFormatWrite",
    "SinkThreadLogoff",
    "ERRORHANDLING",
    SetupAL, UT_CG729PayloadFormatWrite_SinkThreadLogoffL, Teardown)


EUNIT_ALLOC_TEST(
    "EmptyBufferL - alloc test ",
    "CG729PayloadFormatWrite",
    "EmptyBufferL",
    "ERRORHANDLING",
    SetupL, UT_CG729PayloadFormatWrite_EmptyBufferLL, Teardown)

EUNIT_ALLOC_TEST(
    "BufferEmptiedL - Alloc test ",
    "CG729PayloadFormatWrite",
    "BufferEmptiedL",
    "ERRORHANDLING",
    SetupAL, UT_CG729PayloadFormatWrite_BufferEmptiedLL, Teardown)

EUNIT_ALLOC_TEST(
    "NumChannels - alloc test ",
    "CG729PayloadFormatWrite",
    "NumChannels",
    "ERRORHANDLING",
    SetupAL, UT_CG729PayloadFormatWrite_NumChannelsL, Teardown)

EUNIT_ALLOC_TEST(
    "SampleRate - alloc test ",
    "CG729PayloadFormatWrite",
    "SampleRate",
    "ERRORHANDLING",
    SetupAL, UT_CG729PayloadFormatWrite_SampleRateL, Teardown)

EUNIT_ALLOC_TEST(
    "SetSampleRate - alloc test ",
    "CG729PayloadFormatWrite",
    "SetSampleRate",
    "ERRORHANDLING",
    SetupAL, UT_CG729PayloadFormatWrite_SetSampleRateL, Teardown)
    
EUNIT_ALLOC_TEST(
    "ConfigurePayloadFormatL - alloc test ",
    "CG729PayloadFormatWrite",
    "ConfigurePayloadFormatL",
    "ERRORHANDLING",
    SetupAL, UT_CG729PayloadFormatWrite_ConfigurePayloadFormatLL, Teardown)
    
EUNIT_TEST(
    "UpdateConfigurationL - test ",
    "CG729PayloadFormatWrite",
    "UpdateConfigurationL",
    "ERRORHANDLING",
    SetupL, UT_CG729PayloadFormatWrite_UpdateConfigurationLL, Teardown)      


EUNIT_ALLOC_TEST(
    "DeliverPacket - alloc test ",
    "CG729PayloadFormatWrite",
    "DeliverPacket",
    "ERRORHANDLING",
    SetupL, UT_CG729PayloadFormatWrite_DeliverPacketL, Teardown)

#endif


EUNIT_END_TEST_TABLE

//  END OF FILE
