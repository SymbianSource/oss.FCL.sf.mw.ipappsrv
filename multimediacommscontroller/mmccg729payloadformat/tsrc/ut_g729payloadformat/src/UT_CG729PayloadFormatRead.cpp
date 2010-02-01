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
#include "UT_CG729PayloadFormatRead.h"

//  INTERNAL INCLUDES
#include "G729PayloadFormatRead.h"


#include "MccRtpDataSink.h"
#include "mccuids.hrh"
#include "Mccdldatapath.h"
#include "MccRtpDataSource.h"
#include "mccresourcepool.h"
#include "mccredpayloadread.h"
#include "g729payloadformatutil.h"

//  EXTERNAL INCLUDES
#include <e32base.h>
#include <mmf/server/mmfvideoframebuffer.h>
#include <digia/eunit/eunitmacros.h>

#ifdef EUNIT_ALLOC_TEST_ON
    #include <digia/eunit/CEunitAllocTestCaseDecorator.h>   //for using EUNIT_ALLOC_TEST
#endif



const TUid KRtpDataSource            =  { KImplUidRtpDataSource   }; 
const TUid KRtpDataSink              =  { KImplUidRtpDataSink     };


// CONSTRUCTION
UT_CG729PayloadFormatRead* UT_CG729PayloadFormatRead::NewL()
    {
    UT_CG729PayloadFormatRead* self = UT_CG729PayloadFormatRead::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CG729PayloadFormatRead* UT_CG729PayloadFormatRead::NewLC()
    {
    UT_CG729PayloadFormatRead* self = new( ELeave ) UT_CG729PayloadFormatRead();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CG729PayloadFormatRead::~UT_CG729PayloadFormatRead()
    {
    }

// Default constructor
UT_CG729PayloadFormatRead::UT_CG729PayloadFormatRead()
    {
    }

// Second phase construct
void UT_CG729PayloadFormatRead::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    iFourCC.Set( TFourCC( 'G','7','2','9' ) );
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CG729PayloadFormatRead::SetupAL(  )
    {
    iAlloc = ETrue; 

    iRtpDataSourceStub = static_cast<CMccRtpDataSource*>(MDataSource::NewSourceL( KRtpDataSource, KNullDesC8 ));
    iMediaId = KUidMediaTypeAudio;
    MAsyncEventHandler* eventHandler = NULL;
    CMccResourcePool* mccresource = NULL;
    iDLDataPathStub = CMccDlDataPath::NewL( eventHandler, mccresource, iMediaId ); 

    iRead = CG729PayloadFormatRead::NewL( iRtpDataSourceStub );
    iConsumer = MDataSink::NewSinkL( KRtpDataSink, KNullDesC8 );
  
    iBuffi = CMMFDescriptorBuffer::NewL( 10 );
    iBuffi->Data().Append( _L("1234512345") ); 
    
    iRead->SetSourceDataTypeCode( iFourCC, KUidMediaTypeAudio );
    iRead->SourceThreadLogon( *this );
   
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
    
    iRead->ConfigurePayloadFormatL( buffer );

    iRead->NegotiateSourceL( *iDLDataPathStub );
    
    iRead->SourcePrimeL();
    iRead->SourcePlayL();
    } 



void UT_CG729PayloadFormatRead::SetupL(  )
    {   
    iAlloc = EFalse; 

    iRtpDataSourceStub = static_cast<CMccRtpDataSource*>(MDataSource::NewSourceL( KRtpDataSource, KNullDesC8 ));
    iMediaId = KUidMediaTypeAudio;
    MAsyncEventHandler* eventHandler = NULL;
    CMccResourcePool* mccresource = NULL;
    iDLDataPathStub = CMccDlDataPath::NewL( eventHandler, mccresource, iMediaId ); 

    iRead = CG729PayloadFormatRead::NewL( iRtpDataSourceStub );
    iConsumer = MDataSink::NewSinkL( KRtpDataSink, KNullDesC8 );
  
    iBuffi = CMMFDescriptorBuffer::NewL( 10 );
    iBuffi->Data().Append( _L("1234512345") ); 
    
    iRead->SetSourceDataTypeCode( iFourCC, KUidMediaTypeAudio );
    iRead->SourceThreadLogon( *this );
   
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
    
    iRead->ConfigurePayloadFormatL( buffer );

    iRead->NegotiateSourceL( *iDLDataPathStub );
    
    iRead->SourcePrimeL();
    iRead->SourcePlayL();
    } 

void UT_CG729PayloadFormatRead::Teardown(  )
    {
    delete iBuffi;
    delete iDLDataPathStub;   
    delete iRtpDataSourceStub;
    delete iRead;   
    delete iConsumer;
    REComSession::FinalClose();
    }

void UT_CG729PayloadFormatRead::UT_CG729PayloadFormatRead_NewLL()
    {
    CG729PayloadFormatRead* read = CG729PayloadFormatRead::NewL( iRtpDataSourceStub );
    delete read;    
    EUNIT_ASSERT_SPECIFIC_LEAVE( CG729PayloadFormatRead::NewL( NULL), KErrArgument );   
    }

void UT_CG729PayloadFormatRead::UT_CG729PayloadFormatRead_UpdateConfigurationL()
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

    // iCInfo.iMaxPtime == codecInfo.iMaxPtime 
    iRead->iCInfo.iMaxPtime = 200;
    
    EUNIT_ASSERT_NO_LEAVE( iRead->UpdateConfigurationL( codec ) );
        
    // iCInfo.iMaxPtime != codecInfo.iMaxPtime 
    iRead->iCInfo.iMaxPtime = 100;
    iRead->UpdateConfigurationL( codec );
    }
    
void UT_CG729PayloadFormatRead::UT_CG729PayloadFormatRead_CreateClipBufferLL()
    {    
    if ( !iAlloc )
        {
        TUint pktCount = iCodec.iMaxPtime / iCodec.iHwFrameTime;
        TInt plSize = iCodec.iFrameSize * pktCount + KG729CNFrameSize;
        TBool isOwnBuffer;        
        CMMFDataBuffer* buf = iRead->CreateClipBufferL( plSize, isOwnBuffer );
        delete buf;
        }        
    else
        {
        TUint pktCount = iCodec.iMaxPtime / iCodec.iHwFrameTime;
        TInt plSize = iCodec.iFrameSize * pktCount + KG729CNFrameSize;
        TBool isOwnBuffer;        
        CMMFDataBuffer* buf = iRead->CreateClipBufferL( plSize, isOwnBuffer );
        delete buf;       
        }    
    }


void UT_CG729PayloadFormatRead::UT_CG729PayloadFormatRead_SendDataToSinkLL(  )
    {
    if ( !iAlloc )
        {       
        EUNIT_ASSERT_NO_LEAVE( iRead->FillBufferL( iBuffi, iDLDataPathStub, iMediaId ) );
        EUNIT_ASSERT_NO_LEAVE( iRead->SendDataToSinkL() );
        EUNIT_ASSERT_NO_LEAVE( iRead->FillBufferL( iBuffi, iDLDataPathStub, iMediaId ) );
        EUNIT_ASSERT_NO_LEAVE( iRead->SendDataToSinkL() );
        }        
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iRead->FillBufferL( iBuffi, iDLDataPathStub, iMediaId ) );
        EUNIT_ASSERT_NO_LEAVE( iRead->SendDataToSinkL() );       
        }
    }

void UT_CG729PayloadFormatRead::UT_CG729PayloadFormatRead_FillSinkBufferL(  )
    {   
    if ( !iAlloc )
        {
        EUNIT_ASSERT_NO_LEAVE( iRead->FillSinkBufferL() );
        iRead->iBufferToReadExists = 1;
        EUNIT_ASSERT_NO_LEAVE( iRead->FillSinkBufferL() );        
        }
        
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iRead->FillSinkBufferL() );
        }
    }

void UT_CG729PayloadFormatRead::UT_CG729PayloadFormatRead_FillSourceBufferLL(  )
    {
    if ( !iAlloc )
        {
        EUNIT_ASSERT_NO_LEAVE( iRead->FillSourceBufferL() );
        
        }
        
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iRead->FillSourceBufferL() );
        
        }
    }

void UT_CG729PayloadFormatRead::UT_CG729PayloadFormatRead_StreamsL(  )
    {
    TUid uid = {0x101F7D07};
    if ( !iAlloc )
        {
        EUNIT_ASSERT_EQUALS( iRead->Streams( KUidMediaTypeAudio ), 1 );
        EUNIT_ASSERT_EQUALS( iRead->Streams( KUidMediaTypeVideo ), 0 );
        EUNIT_ASSERT_EQUALS( iRead->Streams( KUidMediaTypeMidi ), 0 );
        EUNIT_ASSERT_EQUALS( iRead->Streams( uid ), 0 );
        
        }
        
    else
        {
        EUNIT_ASSERT_EQUALS( iRead->Streams( KUidMediaTypeAudio ), 1 );
        EUNIT_ASSERT_EQUALS( iRead->Streams( KUidMediaTypeVideo ), 0 );
        
        }
    }

void UT_CG729PayloadFormatRead::UT_CG729PayloadFormatRead_FrameTimeIntervalL(  )
    {
    if ( !iAlloc )
        {
		 
        EUNIT_ASSERT_EQUALS( iRead->FrameTimeInterval( KUidMediaTypeAudio ).Int64(), 200 );
        EUNIT_ASSERT_EQUALS( iRead->FrameTimeInterval( KUidMediaTypeVideo ).Int64(), 0 );
        
        }
        
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iRead->FrameTimeInterval( KUidMediaTypeAudio ) );
        EUNIT_ASSERT_NO_LEAVE( iRead->FrameTimeInterval( KUidMediaTypeVideo ) );
        
        }
    }

void UT_CG729PayloadFormatRead::UT_CG729PayloadFormatRead_DurationL(  )
    {
    if ( !iAlloc )
        {
        EUNIT_ASSERT_EQUALS( iRead->Duration( KUidMediaTypeAudio ).Int64(), 0 );
        EUNIT_ASSERT_EQUALS( iRead->Duration( KUidMediaTypeMidi ).Int64(), 0 );
        
        
        }
        
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iRead->Duration( KUidMediaTypeAudio ) );
        
        }
    }
    
void UT_CG729PayloadFormatRead::UT_CG729PayloadFormatRead_DecodePayloadL()
    {
    if ( !iAlloc )
        {
        TUint pktCount = iCodec.iMaxPtime / iCodec.iHwFrameTime;
        TInt plSize = iCodec.iFrameSize * pktCount + KG729CNFrameSize;
        TBool isOwnBuffer;        
        CMMFDataBuffer* buf = iRead->CreateClipBufferL( plSize, isOwnBuffer );               
        iRead->DecodePayload( buf->Data() );
        delete buf;
        
        CMMFDataBuffer* destBuf = CMMFDataBuffer::NewL( KG729CodecDecBufSize );
        TDes8& buffi = destBuf->Data();
        iRead->DecodePayload( buffi );
        delete destBuf;
        
        destBuf = CMMFDataBuffer::NewL( KG729CNFrameSize );
        buffi = destBuf->Data();
        iRead->DecodePayload( buffi );
        delete destBuf;                   
        }
        
    else
        {
        //iRead->DecodePayload
        }    
    }

void UT_CG729PayloadFormatRead::UT_CG729PayloadFormatRead_NumChannelsL(  )
    {
    if ( !iAlloc )
        {
        EUNIT_ASSERT_EQUALS( iRead->NumChannels( ), KMono );
        
        }
        
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iRead->NumChannels( ) );
        
        }
    }

void UT_CG729PayloadFormatRead::UT_CG729PayloadFormatRead_SampleRateL(  )
    {
    if ( !iAlloc )
        {
        EUNIT_ASSERT_NO_LEAVE( iRead->SampleRate() );
        
        }
        
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iRead->SampleRate() );
        
        }
    }

void UT_CG729PayloadFormatRead::UT_CG729PayloadFormatRead_SetSampleRateL()
    {
    TUint sample = 0;
    if ( !iAlloc )
        {
        sample = iRead->SampleRate(); // 8000
        EUNIT_ASSERT_EQUALS( iRead->SetSampleRate( sample ), KErrNone );
        EUNIT_ASSERT_EQUALS( iRead->SetSampleRate( sample + 1 ), KErrNotSupported );
        EUNIT_ASSERT_EQUALS( iRead->SetSampleRate( sample - 8000 ), KErrNotSupported );
        }
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iRead->SetSampleRate( sample ) );
        EUNIT_ASSERT_NO_LEAVE( iRead->SetSampleRate( sample + 1 ) );
        }
    }

void UT_CG729PayloadFormatRead::UT_CG729PayloadFormatRead_SourceDataTypeCodeL(  )
    {
    if ( !iAlloc )
        {
        EUNIT_ASSERT_EQUALS( iRead->SourceDataTypeCode( KUidMediaTypeAudio ), iFourCC );
        EUNIT_ASSERT_EQUALS( iRead->SourceDataTypeCode( KUidMediaTypeVideo ), TFourCC( ) );
        
        }
    
    else 
        {
        EUNIT_ASSERT_EQUALS( iRead->SourceDataTypeCode( KUidMediaTypeAudio ), iFourCC );
        EUNIT_ASSERT_EQUALS( iRead->SourceDataTypeCode( KUidMediaTypeVideo ), TFourCC( ) );
        
        }
    }

void UT_CG729PayloadFormatRead::UT_CG729PayloadFormatRead_SetSourceDataTypeCodeL(  )
    {
    if ( !iAlloc )
        {
        EUNIT_ASSERT_EQUALS( iRead->SetSourceDataTypeCode( iFourCC, KUidMediaTypeAudio ), KErrNone );
        EUNIT_ASSERT_EQUALS( iRead->SetSourceDataTypeCode( iFourCC, KUidMediaTypeVideo ), KErrNotSupported );
        
        //iRead->iPayloadTypes.Insert( 1,0 );
        //EUNIT_ASSERT_NO_LEAVE( iRead->SetRedundancyL( ETrue, 1, 1 ) );
        EUNIT_ASSERT_EQUALS( iRead->SetSourceDataTypeCode( iFourCC, KUidMediaTypeAudio ), KErrNone );
            
        }
    
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iRead->SetSourceDataTypeCode( iFourCC, KUidMediaTypeAudio ) );
        EUNIT_ASSERT_NO_LEAVE( iRead->SetSourceDataTypeCode( iFourCC, KUidMediaTypeVideo ) );
        
        }
    }

void UT_CG729PayloadFormatRead::UT_CG729PayloadFormatRead_CreateSourceBufferLL(  )
    {
    TBool refO( ETrue );
    TBool refV( EFalse );
    if ( !iAlloc )
        
        {
        EUNIT_ASSERT_NO_LEAVE( iRead->CreateSourceBufferL( KUidMediaTypeAudio, refO ) );
        EUNIT_ASSERT_NO_LEAVE( iRead->CreateSourceBufferL( KUidMediaTypeAudio, refV ) );
        EUNIT_ASSERT_SPECIFIC_LEAVE( iRead->CreateSourceBufferL( KUidMediaTypeVideo, refO ), KErrNotSupported );
        EUNIT_ASSERT_SPECIFIC_LEAVE( iRead->CreateSourceBufferL( KUidMediaTypeVideo, refV ), KErrNotSupported );
        EUNIT_ASSERT_SPECIFIC_LEAVE( iRead->CreateSourceBufferL( KUidMediaTypeMidi, refO ), KErrNotSupported );
        
        }
        
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iRead->CreateSourceBufferL( KUidMediaTypeAudio, refO ) );
        EUNIT_ASSERT_SPECIFIC_LEAVE( iRead->CreateSourceBufferL( KUidMediaTypeVideo, refO ), KErrNotSupported );
        
        }
    }

void UT_CG729PayloadFormatRead::UT_CG729PayloadFormatRead_CreateSourceBufferL_1L(  )
    {
    TBool refO( ETrue );
    TBool refV( EFalse );
    if ( !iAlloc )
        {
        EUNIT_ASSERT_NO_LEAVE( iRead->CreateSourceBufferL( KUidMediaTypeAudio, *iBuffi, refO ) );
        EUNIT_ASSERT_NO_LEAVE( iRead->CreateSourceBufferL( KUidMediaTypeAudio, *iBuffi, refV ) );
        EUNIT_ASSERT_SPECIFIC_LEAVE( iRead->CreateSourceBufferL( KUidMediaTypeVideo, *iBuffi, refO ), KErrNotSupported );
        
        
        }
        
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iRead->CreateSourceBufferL( KUidMediaTypeAudio, *iBuffi, refO ) );
        EUNIT_ASSERT_NO_LEAVE( iRead->CreateSourceBufferL( KUidMediaTypeAudio, *iBuffi, refV ) );
        EUNIT_ASSERT_SPECIFIC_LEAVE( iRead->CreateSourceBufferL( KUidMediaTypeVideo, *iBuffi, refO ), KErrNotSupported );
        
        }
    }

void UT_CG729PayloadFormatRead::UT_CG729PayloadFormatRead_FillBufferLL(  )
    {
    if ( !iAlloc )
        {
        iRead->iCurrentBuffer = iRead->EBufferOne;
        EUNIT_ASSERT_NO_LEAVE( iRead->FillBufferL( iBuffi, iDLDataPathStub, KUidMediaTypeAudio ) );
        
        EUNIT_ASSERT_SPECIFIC_LEAVE( iRead->FillBufferL( iBuffi, iDLDataPathStub, KUidMediaTypeVideo ), KErrNotSupported ); // KErrNotSupported
     
        iRead->iCurrentBuffer = iRead->EBufferTwo;
        EUNIT_ASSERT_NO_LEAVE( iRead->FillBufferL( iBuffi, iDLDataPathStub, KUidMediaTypeAudio ) );
       
        iRead->iBufferToReadExists = ETrue; 
        EUNIT_ASSERT_NO_LEAVE( iRead->FillBufferL( iBuffi, iDLDataPathStub, KUidMediaTypeAudio ) );

        EUNIT_ASSERT_LEAVE( iRead->FillBufferL( NULL, iDLDataPathStub, KUidMediaTypeAudio ) );
        }
        
    else
        {
        EUNIT_ASSERT_LEAVE( iRead->FillBufferL( iBuffi, iDLDataPathStub, KUidMediaTypeMidi ) );
        
        }
    }

void UT_CG729PayloadFormatRead::UT_CG729PayloadFormatRead_SourceThreadLogonL(  )
    {
    if ( !iAlloc )
        {
        EUNIT_ASSERT_EQUALS( iRead->SourceThreadLogon( *aEventH ), KErrNone );
            
        }
    
    else
        {
        EUNIT_ASSERT_EQUALS( iRead->SourceThreadLogon( *aEventH ), KErrNone);
        
        }
        
    iRead->iClip = NULL;
    EUNIT_ASSERT_EQUALS( iRead->SourceThreadLogon( *aEventH ), KErrNotReady );
    }

void UT_CG729PayloadFormatRead::UT_CG729PayloadFormatRead_SourceThreadLogoffL(  )
    {
    if ( !iAlloc )
        {
        EUNIT_ASSERT_NO_LEAVE( iRead->SourceThreadLogoff( ) );
            
        }
    
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iRead->SourceThreadLogoff( ) );
        
        }
    }

void UT_CG729PayloadFormatRead::UT_CG729PayloadFormatRead_NegotiateSourceLL(  )
    {
    if ( !iAlloc )
        {
        EUNIT_ASSERT_NO_LEAVE( iRead->NegotiateSourceL( *iDLDataPathStub ) );   
        }    
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iRead->NegotiateSourceL( *iDLDataPathStub ) );      
        }
    }

void UT_CG729PayloadFormatRead::UT_CG729PayloadFormatRead_SourcePrimeLL(  )
    {
    if ( !iAlloc )
        {
        EUNIT_ASSERT_NO_LEAVE( iRead->SourcePrimeL() );
        
        }
        
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iRead->SourcePrimeL() );
        
        }
    }

void UT_CG729PayloadFormatRead::UT_CG729PayloadFormatRead_SourcePlayLL(  )
    {
    if ( !iAlloc )
        {
        EUNIT_ASSERT_NO_LEAVE( iRead->SourcePlayL() );
        
        }
        
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iRead->SourcePlayL() );
        
        }
    }

void UT_CG729PayloadFormatRead::UT_CG729PayloadFormatRead_SourcePauseLL(  )
    {
    if ( !iAlloc )
        {
        iRead->SourcePlayL();
        EUNIT_ASSERT_NO_LEAVE( iRead->SourcePauseL() );
        
        }
        
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iRead->SourcePauseL() );
        
        }
    }

void UT_CG729PayloadFormatRead::UT_CG729PayloadFormatRead_SourceStopLL(  )
    {
    if ( !iAlloc )
        {
        iRead->SourcePlayL();
        EUNIT_ASSERT_NO_LEAVE( iRead->SourceStopL() );
        
        }
        
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iRead->SourceStopL() );
        
        }
    }

void UT_CG729PayloadFormatRead::UT_CG729PayloadFormatRead_BufferFilledLL(  )
    {
    TRtpRecvHeader test;
    test.iPayloadType = 13;
    
    if ( !iAlloc )
        {
        CMMFBuffer* buf = NULL;
        EUNIT_ASSERT_SPECIFIC_LEAVE( iRead->DataBufferFilledL( buf, test ), KErrArgument );
        CMMFDataBuffer* buffi = CMMFDataBuffer::NewL( 150 );      
        buf = buffi; 
        CleanupStack::PushL( buf );         
        iRtpDataSourceStub->FillBufferL( buf, iConsumer, KUidMediaTypeAudio );       
        EUNIT_ASSERT_NO_LEAVE( iRead->DataBufferFilledL( iRead->iSourceBuffer, test ) );

        buffi->Data().Append( _L("12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890" ));
        EUNIT_ASSERT_NO_LEAVE( iRead->DataBufferFilledL( iRead->iSourceBuffer, test ) );
        CleanupStack::PopAndDestroy( buf );   
        }
    
    else
        {
        CMMFBuffer* buf = NULL;
        EUNIT_ASSERT_SPECIFIC_LEAVE( iRead->DataBufferFilledL( buf, test ), KErrArgument );
        CMMFDataBuffer* buffi = CMMFDataBuffer::NewL( 150 );      
        buf = buffi; 
        CleanupStack::PushL( buf );         
        iRtpDataSourceStub->FillBufferL( buf, iConsumer, KUidMediaTypeAudio );       
        EUNIT_ASSERT_NO_LEAVE( iRead->DataBufferFilledL( iRead->iSourceBuffer, test ) );

        buffi->Data().Append( _L("12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890" ));
        EUNIT_ASSERT_NO_LEAVE( iRead->DataBufferFilledL( iRead->iSourceBuffer, test ) );
        CleanupStack::PopAndDestroy( buf );         
        }
    
    }


void UT_CG729PayloadFormatRead::UT_CG729PayloadFormatRead_SinkDataTypeCodeL(  )
    {
    if ( !iAlloc )
        {
        EUNIT_ASSERT_NO_LEAVE( iRead->SinkDataTypeCode( KUidMediaTypeAudio ) );
        EUNIT_ASSERT_NO_LEAVE( iRead->SinkDataTypeCode( KUidMediaTypeVideo ) );
        }
        
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iRead->SinkDataTypeCode( KUidMediaTypeAudio ) );        
        }
    }
    
void UT_CG729PayloadFormatRead::UT_CG729PayloadFormatRead_DoBitUnPackingL()
    {
    if ( !iAlloc )
        {
        _LIT( KData, "dummy");
        TBool comfortNoise = EFalse;
        
        HBufC8* sourceBuf = HBufC8::NewL( 6 );  
        CleanupStack::PushL(sourceBuf);      
        TPtr8 ptr = sourceBuf->Des();
        ptr.Append( KData );         
        
        CMMFDataBuffer* destBuf = CMMFDataBuffer::NewL( KG729CodecDecBufSize );
        TDes8& buf = destBuf->Data();         // cleanup stackiin sourceBuf ainakin
        
        EUNIT_ASSERT_EQUALS( iRead->DoBitUnPacking( *sourceBuf, buf, comfortNoise ), KErrNone );
        buf.Zero();
        
        CleanupStack::Pop(sourceBuf); 
        
        delete destBuf;
        delete sourceBuf;        
        }
        
    else
        {
        _LIT( KData, "dummy");
        TBool comfortNoise = EFalse;
        HBufC8* sourceBuf = NULL;
        
        HBufC8* destBuf = HBufC8::NewL( 6 );        
        TPtr8 ptr = destBuf->Des();
        ptr.Append( KData );        
        
        EUNIT_ASSERT_EQUALS( iRead->DoBitUnPacking( *sourceBuf, ptr, comfortNoise ), KErrArgument );
        delete destBuf;        
        }    
    }

void UT_CG729PayloadFormatRead::UT_CG729PayloadFormatRead_ConfigurePayloadFormatL2L(  )
    {
    TBuf8<20> buf;
    buf.Append( _L8("something") );
    EUNIT_ASSERT_SPECIFIC_LEAVE( iRead->ConfigurePayloadFormatL( buf ), KErrArgument );
    
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
    codec.iIsUpdate = ETrue;    
    
    TMccCodecInfoBuffer buffer( codec );

    EUNIT_ASSERT_NO_LEAVE( iRead->ConfigurePayloadFormatL( buffer ));

    }

void UT_CG729PayloadFormatRead::UT_CG729PayloadFormatRead_ConfigurePayloadFormatL3L(  )
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
    
    codec.iIsUpdate = EFalse;   
    codec.iRedundancyCount = 0;
    iRead->ConfigurePayloadFormatL( buffer );
        
    }

void UT_CG729PayloadFormatRead::UT_CG729PayloadFormatRead_ConfigurePayloadFormatLL(  )
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

        CG729PayloadFormatRead* read = CG729PayloadFormatRead::NewL( iRtpDataSourceStub );
        CleanupStack::PushL( read );
        read->ConfigurePayloadFormatL( buffer );
        CleanupStack::PopAndDestroy( read );
        
        codec.iAlgoUsed = 2;
        codec.iRedundancyCount = 1;
        codec.iRedundantPayload = 99;
       
        buffer = codec; 
        //EUNIT_ASSERT_NO_LEAVE( iRead->ConfigurePayloadFormatL( buffer ) );
        CMccRtpDataSource* rtpDataSourceStub = static_cast<CMccRtpDataSource*>(MDataSource::NewSourceL( KRtpDataSource, KNullDesC8 ));
        CleanupStack::PushL( rtpDataSourceStub );
        CMccRedPayloadRead* redDecoder = CMccRedPayloadRead::NewL( rtpDataSourceStub );
        CleanupStack::PushL( redDecoder );
        CG729PayloadFormatRead* read2 = CG729PayloadFormatRead::NewL( redDecoder );
        CleanupStack::PushL( read2 );
        read2->ConfigurePayloadFormatL( buffer );
        CleanupStack::PopAndDestroy( 3 ); 
        }
    else
        {
        EUNIT_ASSERT( ETrue );
        }

    }



//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CG729PayloadFormatRead,
    "Add test suite description here.",
    "UNIT" )
 
EUNIT_TEST(
    "NewL - test ",
    "CG729PayloadFormatRead",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatRead_NewLL, Teardown)
    
EUNIT_TEST(
    "UpdateConfigurationL - test ",
    "CG729PayloadFormatRead",
    "UpdateConfigurationL",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatRead_UpdateConfigurationL, Teardown)

EUNIT_TEST(
    "CreateClipBufferL - test ",
    "CG729PayloadFormatRead",
    "CreateClipBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatRead_CreateClipBufferLL, Teardown)


EUNIT_TEST(
    "SendDataToSinkL - test ",
    "CG729PayloadFormatRead",
    "SendDataToSinkL",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatRead_SendDataToSinkLL, Teardown)

EUNIT_TEST(
    "FillSinkBuffer - test ",
    "CG729PayloadFormatRead",
    "FillSinkBuffer",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatRead_FillSinkBufferL, Teardown)


EUNIT_TEST(
    "FillSourceBufferL - test ",
    "CG729PayloadFormatRead",
    "FillSourceBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatRead_FillSourceBufferLL, Teardown)


EUNIT_TEST(
    "Streams - test ",
    "CG729PayloadFormatRead",
    "Streams",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatRead_StreamsL, Teardown)


EUNIT_TEST(
    "FrameTimeInterval - test ",
    "CG729PayloadFormatRead",
    "FrameTimeInterval",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatRead_FrameTimeIntervalL, Teardown)

EUNIT_TEST(
    "Duration - test ",
    "CG729PayloadFormatRead",
    "Duration",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatRead_DurationL, Teardown)

EUNIT_TEST(
    "DecodePayload - test ",
    "CG729PayloadFormatRead",
    "DecodePayload",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatRead_DecodePayloadL, Teardown)

EUNIT_TEST(
    "NumChannels - test ",
    "CG729PayloadFormatRead",
    "NumChannels",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatRead_NumChannelsL, Teardown)

EUNIT_TEST(
    "SampleRate - test ",
    "CG729PayloadFormatRead",
    "SampleRate",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatRead_SampleRateL, Teardown)

EUNIT_TEST(
    "SetSampleRate - test ",
    "CG729PayloadFormatRead",
    "SetSampleRate",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatRead_SetSampleRateL, Teardown)


EUNIT_TEST(
    "SourceDataTypeCode - test ",
    "CG729PayloadFormatRead",
    "SourceDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatRead_SourceDataTypeCodeL, Teardown)


EUNIT_TEST(
    "SetSourceDataTypeCode - test ",
    "CG729PayloadFormatRead",
    "SetSourceDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatRead_SetSourceDataTypeCodeL, Teardown)

EUNIT_TEST(
    "CreateSourceBufferL - test ",
    "CG729PayloadFormatRead",
    "CreateSourceBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatRead_CreateSourceBufferLL, Teardown)


EUNIT_TEST(
    "CreateSourceBuffer1L - test ",
    "CG729PayloadFormatRead",
    "CreateSourceBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatRead_CreateSourceBufferL_1L, Teardown)


EUNIT_TEST(
    "FillBufferL - test ",
    "CG729PayloadFormatRead",
    "FillBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatRead_FillBufferLL, Teardown)


EUNIT_TEST(
    "SourceThreadLogon - test ",
    "CG729PayloadFormatRead",
    "SourceThreadLogon",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatRead_SourceThreadLogonL, Teardown)

EUNIT_TEST(
    "SourceThreadLogoff - test ",
    "CG729PayloadFormatRead",
    "SourceThreadLogoff",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatRead_SourceThreadLogoffL, Teardown)


EUNIT_TEST(
    "NegotiateSourceL - test ",
    "CG729PayloadFormatRead",
    "NegotiateSourceL",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatRead_NegotiateSourceLL, Teardown)


EUNIT_TEST(
    "SourcePrimeL - test ",
    "CG729PayloadFormatRead",
    "SourcePrimeL",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatRead_SourcePrimeLL, Teardown)


EUNIT_TEST(
    "SourcePlayL - test ",
    "CG729PayloadFormatRead",
    "SourcePlayL",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatRead_SourcePlayLL, Teardown)

EUNIT_TEST(
    "SourcePauseL - test ",
    "CG729PayloadFormatRead",
    "SourcePauseL",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatRead_SourcePauseLL, Teardown)

EUNIT_TEST(
    "SourceStopL - test ",
    "CG729PayloadFormatRead",
    "SourceStopL",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatRead_SourceStopLL, Teardown)

EUNIT_TEST(
    "BufferFilledL - test ",
    "CG729PayloadFormatRead",
    "BufferFilledL",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatRead_BufferFilledLL, Teardown)


EUNIT_TEST(
    "SinkDataTypeCodeL - test ",
    "CG729PayloadFormatRead",
    "SinkDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatRead_SinkDataTypeCodeL, Teardown)

EUNIT_TEST(
    "DoBitUnPacking - test ",
    "CG729PayloadFormatRead",
    "DoBitUnPacking",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatRead_DoBitUnPackingL, Teardown)
    
EUNIT_TEST(
    "ConfigurePayloadFormatL - test ",
    "CG729PayloadFormatRead",
    "ConfigurePayloadFormatL",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatRead_ConfigurePayloadFormatLL, Teardown)
    
    
EUNIT_TEST(
    "ConfigurePayloadFormatL2 - test ",
    "CG729PayloadFormatRead",
    "ConfigurePayloadFormatL2",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatRead_ConfigurePayloadFormatL2L, Teardown)
    
   
EUNIT_TEST(
    "ConfigurePayloadFormatL3 - test ",
    "CG729PayloadFormatRead",
    "ConfigurePayloadFormatL3",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatRead_ConfigurePayloadFormatL3L, Teardown)
    

// ALLOC TESTS

#ifdef EUNIT_ALLOC_TEST_ON

EUNIT_ALLOC_TEST(
    "UpdateConfigurationL - alloc test ",
    "CG729PayloadFormatRead",
    "UpdateConfigurationL",
    "FUNCTIONALITY",
    SetupAL, UT_CG729PayloadFormatRead_UpdateConfigurationL, Teardown)
    
EUNIT_TEST(
    "CreateClipBufferL - test ",
    "CG729PayloadFormatRead",
    "CreateClipBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatRead_CreateClipBufferLL, Teardown)    


EUNIT_ALLOC_TEST(
    "ConfigurePayloadFormatL2 - alloc test ",
    "CG729PayloadFormatRead",
    "ConfigurePayloadFormatL2",
    "FUNCTIONALITY",
    SetupAL, UT_CG729PayloadFormatRead_ConfigurePayloadFormatL2L, Teardown)
    
    
EUNIT_ALLOC_TEST(
    "ConfigurePayloadFormatL3 - alloc test ",
    "CG729PayloadFormatRead",
    "ConfigurePayloadFormatL3",
    "FUNCTIONALITY",
    SetupAL, UT_CG729PayloadFormatRead_ConfigurePayloadFormatL3L, Teardown)
    
    
EUNIT_ALLOC_TEST(
    "SendDataToSinkL - alloc test ",
    "CG729PayloadFormatRead",
    "SendDataToSinkL",
    "ERRORHANDLING",
    SetupAL, UT_CG729PayloadFormatRead_SendDataToSinkLL, Teardown)

EUNIT_ALLOC_TEST(
    "Buffer - alloc test ",
    "CG729PayloadFormatRead",
    "FillSinkBuffer",
    "ERRORHANDLING",
    SetupAL, UT_CG729PayloadFormatRead_FillSinkBufferL, Teardown)


EUNIT_ALLOC_TEST(
    "FillSourceBufferL - alloc test ",
    "CG729PayloadFormatRead",
    "FillSourceBufferL",
    "ERRORHANDLING",
    SetupAL, UT_CG729PayloadFormatRead_FillSourceBufferLL, Teardown)


EUNIT_ALLOC_TEST(
    "Streams - alloc test ",
    "CG729PayloadFormatRead",
    "Streams",
    "ERRORHANDLING",
    SetupAL, UT_CG729PayloadFormatRead_StreamsL, Teardown)


EUNIT_ALLOC_TEST(
    "FrameTimeInterval - alloc test ",
    "CG729PayloadFormatRead",
    "FrameTimeInterval",
    "ERRORHANDLING",
    SetupAL, UT_CG729PayloadFormatRead_FrameTimeIntervalL, Teardown)

EUNIT_ALLOC_TEST(
    "Duration - alloc test ",
    "CG729PayloadFormatRead",
    "Duration",
    "ERRORHANDLING",
    SetupAL, UT_CG729PayloadFormatRead_DurationL, Teardown)
    
EUNIT_TEST(
    "DecodePayload - test ",
    "CG729PayloadFormatRead",
    "DecodePayload",
    "FUNCTIONALITY",
    SetupL, UT_CG729PayloadFormatRead_DecodePayloadL, Teardown)    


EUNIT_ALLOC_TEST(
    "NumChannels - alloc test ",
    "CG729PayloadFormatRead",
    "NumChannels",
    "ERRORHANDLING",
    SetupAL, UT_CG729PayloadFormatRead_NumChannelsL, Teardown)


EUNIT_ALLOC_TEST(
    "SampleRate - alloc test ",
    "CG729PayloadFormatRead",
    "SampleRate",
    "ERRORHANDLING",
    SetupAL, UT_CG729PayloadFormatRead_SampleRateL, Teardown)

EUNIT_ALLOC_TEST(
    "SetSampleRate - alloc test ",
    "CG729PayloadFormatRead",
    "SetSampleRate",
    "ERRORHANDLING",
    SetupAL, UT_CG729PayloadFormatRead_SetSampleRateL, Teardown)


EUNIT_ALLOC_TEST(
    "SourceDataTypeCode - alloc test ",
    "CG729PayloadFormatRead",
    "SourceDataTypeCode",
    "ERRORHANDLING",
    SetupAL, UT_CG729PayloadFormatRead_SourceDataTypeCodeL, Teardown)


EUNIT_ALLOC_TEST(
    "SetSourceDataTypeCode - alloc test ",
    "CG729PayloadFormatRead",
    "SetSourceDataTypeCode",
    "ERRORHANDLING",
    SetupAL, UT_CG729PayloadFormatRead_SetSourceDataTypeCodeL, Teardown)

EUNIT_ALLOC_TEST(
    "CreateSourceBufferL - alloc test ",
    "CG729PayloadFormatRead",
    "CreateSourceBufferL",
    "ERRORHANDLING",
    SetupAL, UT_CG729PayloadFormatRead_CreateSourceBufferLL, Teardown)


EUNIT_ALLOC_TEST(
    "CreateSourceBuffer1L - alloc test ",
    "CG729PayloadFormatRead",
    "CreateSourceBufferL",
    "ERRORHANDLING",
    SetupAL, UT_CG729PayloadFormatRead_CreateSourceBufferL_1L, Teardown)


EUNIT_ALLOC_TEST(
    "FillBufferL - alloc test ",
    "CG729PayloadFormatRead",
    "FillBufferL",
    "ERRORHANDLING",
    SetupAL, UT_CG729PayloadFormatRead_FillBufferLL, Teardown)


EUNIT_ALLOC_TEST(
    "SourceThreadLogon - alloc test ",
    "CG729PayloadFormatRead",
    "SourceThreadLogon",
    "ERRORHANDLING",
    SetupAL, UT_CG729PayloadFormatRead_SourceThreadLogonL, Teardown)

EUNIT_ALLOC_TEST(
    "SourceThreadLogoff - alloc test ",
    "CG729PayloadFormatRead",
    "SourceThreadLogoff",
    "ERRORHANDLING",
    SetupAL, UT_CG729PayloadFormatRead_SourceThreadLogoffL, Teardown)


EUNIT_ALLOC_TEST(
    "NegotiateSourceL - alloc test ",
    "CG729PayloadFormatRead",
    "NegotiateSourceL",
    "ERRORHANDLING",
    SetupAL, UT_CG729PayloadFormatRead_NegotiateSourceLL, Teardown)


EUNIT_ALLOC_TEST(
    "SourcePrimeL - alloc test ",
    "CG729PayloadFormatRead",
    "SourcePrimeL",
    "ERRORHANDLING",
    SetupAL, UT_CG729PayloadFormatRead_SourcePrimeLL, Teardown)


EUNIT_ALLOC_TEST(
    "SourcePlayL - alloc test ",
    "CG729PayloadFormatRead",
    "SourcePlayL",
    "ERRORHANDLING",
    SetupAL, UT_CG729PayloadFormatRead_SourcePlayLL, Teardown)

EUNIT_ALLOC_TEST(
    "SourcePauseL - alloc test ",
    "CG729PayloadFormatRead",
    "SourcePauseL",
    "ERRORHANDLING",
    SetupAL, UT_CG729PayloadFormatRead_SourcePauseLL, Teardown)

EUNIT_ALLOC_TEST(
    "SourceStopL - alloc test ",
    "CG729PayloadFormatRead",
    "SourceStopL",
    "ERRORHANDLING",
    SetupAL, UT_CG729PayloadFormatRead_SourceStopLL, Teardown)

EUNIT_ALLOC_TEST(
    "BufferFilledL - alloc test ",
    "CG729PayloadFormatRead",
    "BufferFilledL",
    "ERRORHANDLING",
    SetupAL, UT_CG729PayloadFormatRead_BufferFilledLL, Teardown)

EUNIT_ALLOC_TEST(
    "SinkDataTypeCodeL - alloc test ",
    "CG729PayloadFormatRead",
    "SinkDataTypeCode",
    "ERRORHANDLING",
    SetupAL, UT_CG729PayloadFormatRead_SinkDataTypeCodeL, Teardown)
    
EUNIT_TEST(
    "DoBitUnPacking - test ",
    "CG729PayloadFormatRead",
    "DoBitUnPacking",
    "ERRORHANDLING",
    SetupL, UT_CG729PayloadFormatRead_DoBitUnPackingL, Teardown)    

#endif


EUNIT_END_TEST_TABLE

//  END OF FILE
