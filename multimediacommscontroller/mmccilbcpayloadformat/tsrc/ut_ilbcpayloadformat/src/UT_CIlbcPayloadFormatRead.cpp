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
#include "UT_CIlbcPayloadFormatRead.h"
#include "mccuids.hrh"
#include "Mccdldatapath.h"
#include "mccresourcepool.h"
#include "mccredpayloadread.h"
#include "mccrtpdatasource.h"


//  EXTERNAL INCLUDES
#include <e32base.h>
#include <digia/eunit/eunitmacros.h>

#ifdef EUNIT_ALLOC_TEST_ON
    #include <digia/eunit/CEunitAllocTestCaseDecorator.h>   //for using EUNIT_ALLOC_TEST
#endif

//  INTERNAL INCLUDES
#include "IlbcPayloadFormatRead.h"


const TUid KRtpDataSource            =  { KImplUidRtpDataSource   };
const TUid KRtpDataSink              =  { KImplUidRtpDataSink     };
const TUint KiLBCSampleRate = 8000; //8 kHz

// CONSTRUCTION
UT_CIlbcPayloadFormatRead* UT_CIlbcPayloadFormatRead::NewL()
    {
    UT_CIlbcPayloadFormatRead* self = UT_CIlbcPayloadFormatRead::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CIlbcPayloadFormatRead* UT_CIlbcPayloadFormatRead::NewLC()
    {
    UT_CIlbcPayloadFormatRead* self = new( ELeave ) UT_CIlbcPayloadFormatRead();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CIlbcPayloadFormatRead::~UT_CIlbcPayloadFormatRead()
    {
    }

// Default constructor
UT_CIlbcPayloadFormatRead::UT_CIlbcPayloadFormatRead()
    {
    }

// Second phase construct
void UT_CIlbcPayloadFormatRead::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    iFourCC.Set( TFourCC( 'I','L','B','C' ) );
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CIlbcPayloadFormatRead::SetupAL(  )
    {
    iAlloc = ETrue;

    iRtpDataSourceStub = MDataSource::NewSourceL( KRtpDataSource, KNullDesC8 );
    iMediaId = KUidMediaTypeAudio;
    MAsyncEventHandler* eventHandler = NULL;
    CMccResourcePool* mccresource = NULL;
    iDLDataPathStub = CMccDlDataPath::NewL( eventHandler, mccresource, iMediaId ); 

    iRead = CIlbcPayloadFormatRead::NewL( iRtpDataSourceStub );
    iConsumer = MDataSink::NewSinkL( KRtpDataSink, KNullDesC8 );
  
    iBuffi = CMMFDescriptorBuffer::NewL( 10 );
    iBuffi->Data().Append( _L("1234512345") ); 
    
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
    iCodec.iRedundancyCount = 0;
    iCodec.iRedundantPayload = 99;
  
    TMccCodecInfoBuffer buffer( iCodec );
    
    iRead->SetSourceDataTypeCode( iFourCC, KUidMediaTypeAudio );
    iRead->SourceThreadLogon( *this );
    
    iRead->ConfigurePayloadFormatL( buffer );

    iRead->NegotiateSourceL( *iDLDataPathStub );
    
    iRead->SourcePrimeL();
    iRead->SourcePlayL();
    } 

void UT_CIlbcPayloadFormatRead::SetupL(  )
    {
    iAlloc = EFalse; 

    iRtpDataSourceStub = MDataSource::NewSourceL( KRtpDataSource, KNullDesC8 );
    iMediaId = KUidMediaTypeAudio;
    MAsyncEventHandler* eventHandler = NULL;
    CMccResourcePool* mccresource = NULL;
    iDLDataPathStub = CMccDlDataPath::NewL( eventHandler, mccresource, iMediaId ); 

    EUNIT_ASSERT_LEAVE( iRead = CIlbcPayloadFormatRead::NewL( NULL ));
    iRead = CIlbcPayloadFormatRead::NewL( iRtpDataSourceStub );
    iConsumer = MDataSink::NewSinkL( KRtpDataSink, KNullDesC8 );
  
    iBuffi = CMMFDescriptorBuffer::NewL( 10 );
    iBuffi->Data().Append( _L("1234512345") ); 
    
    iCodec.iAlgoUsed = ENoAlgoUsed;
    iCodec.iBitrate = 15200;
    iCodec.iCNFrameSize = 20;
    iCodec.iCodecMode = 3;
    iCodec.iEnableDTX = 1;
    iCodec.iFourCC = 0x31313747;
    iCodec.iFrameSize = 20;
    iCodec.iHwFrameTime = 20;
    iCodec.iMaxPtime = 200;
    iCodec.iPayloadType = 8;
    iCodec.iPtime = 20;
    iCodec.iRedundancyCount = 0;
    iCodec.iRedundantPayload = 99;
  
    TMccCodecInfoBuffer buffer( iCodec );
    
    iRead->SetSourceDataTypeCode( iFourCC, KUidMediaTypeAudio );
    iRead->SourceThreadLogon( *this );
    
    iRead->ConfigurePayloadFormatL( buffer );

    iRead->NegotiateSourceL( *iDLDataPathStub );
    
    iRead->SourcePrimeL();
    iRead->SourcePlayL();
    } 



void UT_CIlbcPayloadFormatRead::Teardown(  )
    {
    delete iBuffi;
    delete iDLDataPathStub;   
    delete iRtpDataSourceStub;
    delete iRead;   
    delete iConsumer;
    REComSession::FinalClose();
    }

void UT_CIlbcPayloadFormatRead::UT_CIlbcPayloadFormatRead_SendDataToSinkLL(  )
    {
    if ( !iAlloc )
        {       
        EUNIT_ASSERT_NO_LEAVE( iRead->FillBufferL( iBuffi, iDLDataPathStub, iMediaId ) );
        EUNIT_ASSERT_NO_LEAVE( iRead->SendDataToSinkL() );
        EUNIT_ASSERT_NO_LEAVE( iRead->FillBufferL( iBuffi, iDLDataPathStub, iMediaId ) );
        EUNIT_ASSERT_NO_LEAVE( iRead->SendDataToSinkL() );
        
        
        iRead->iBufferToReadExists = ETrue;
        
        EUNIT_ASSERT_NO_LEAVE( iRead->FillBufferL( iBuffi, iDLDataPathStub, iMediaId ) );
        EUNIT_ASSERT_NO_LEAVE( iRead->SendDataToSinkL() );
        }        
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iRead->FillBufferL( iBuffi, iDLDataPathStub, iMediaId ) );
        EUNIT_ASSERT_NO_LEAVE( iRead->SendDataToSinkL() );       
        }
    }

void UT_CIlbcPayloadFormatRead::UT_CIlbcPayloadFormatRead_FillSinkBufferL(  )
    {
    if ( !iAlloc )
        {
        EUNIT_ASSERT_NO_LEAVE( iRead->FillSinkBufferL() );
        iRead->iBufferToReadExists = 1;
        EUNIT_ASSERT_NO_LEAVE( iRead->FillSinkBufferL() );
        
        iRead->iCnFrame = 1;
        EUNIT_ASSERT_NO_LEAVE( iRead->FillSinkBufferL() );
        
        iRead->iCurrentBuffer = iRead->EBufferTwo;
        EUNIT_ASSERT_NO_LEAVE( iRead->FillSinkBufferL() ); 
        
        iRead->iCurrentBuffer = iRead->EBufferOne;
        iRead->iCInfo.iCodecMode =  KIlbcMode20msFrame;
        
        EUNIT_ASSERT_NO_LEAVE( iRead->FillSinkBufferL() );
        
        iRead->iFrameIndex = 5;  
        EUNIT_ASSERT_NO_LEAVE( iRead->FillSinkBufferL() );
        }        
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iRead->FillSinkBufferL() );
        
        } 
    }

void UT_CIlbcPayloadFormatRead::UT_CIlbcPayloadFormatRead_FillSourceBufferLL(  )
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

void UT_CIlbcPayloadFormatRead::UT_CIlbcPayloadFormatRead_StreamsL(  )
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

void UT_CIlbcPayloadFormatRead::UT_CIlbcPayloadFormatRead_FrameTimeIntervalL(  )
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

void UT_CIlbcPayloadFormatRead::UT_CIlbcPayloadFormatRead_DurationL(  )
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

void UT_CIlbcPayloadFormatRead::UT_CIlbcPayloadFormatRead_NumChannelsL(  )
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

void UT_CIlbcPayloadFormatRead::UT_CIlbcPayloadFormatRead_SampleRateL(  )
    {
    if ( !iAlloc )
        {
        EUNIT_ASSERT_EQUALS( iRead->SampleRate(), KiLBCSampleRate );        
        }      
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iRead->SampleRate() );     
        }
    }

void UT_CIlbcPayloadFormatRead::UT_CIlbcPayloadFormatRead_SetSampleRateL(  )
    {
    TUint sample(0);
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

void UT_CIlbcPayloadFormatRead::UT_CIlbcPayloadFormatRead_SourceDataTypeCodeL(  )
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

void UT_CIlbcPayloadFormatRead::UT_CIlbcPayloadFormatRead_SetSourceDataTypeCodeL(  )
    {
    if ( !iAlloc )
        {
        EUNIT_ASSERT_EQUALS( iRead->SetSourceDataTypeCode( iFourCC, KUidMediaTypeAudio ), KErrNone );
        EUNIT_ASSERT_EQUALS( iRead->SetSourceDataTypeCode( iFourCC, KUidMediaTypeVideo ), KErrNotSupported );
        
        }
    
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iRead->SetSourceDataTypeCode( iFourCC, KUidMediaTypeAudio ) );
        EUNIT_ASSERT_NO_LEAVE( iRead->SetSourceDataTypeCode( iFourCC, KUidMediaTypeVideo ) );
        }
    }

void UT_CIlbcPayloadFormatRead::UT_CIlbcPayloadFormatRead_CreateSourceBufferLL(  )
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

void UT_CIlbcPayloadFormatRead::UT_CIlbcPayloadFormatRead_CreateSourceBufferL_1L(  )
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

void UT_CIlbcPayloadFormatRead::UT_CIlbcPayloadFormatRead_FillBufferLL(  )
    {
    if ( !iAlloc )
        {
        iRead->iCurrentBuffer = iRead->EBufferOne;
        EUNIT_ASSERT_NO_LEAVE( iRead->FillBufferL( iBuffi, iDLDataPathStub, KUidMediaTypeAudio ) );
        
        EUNIT_ASSERT_LEAVE( iRead->FillBufferL( iBuffi, iDLDataPathStub, KUidMediaTypeVideo ) ); // KErrNotSupported
        
        iRead->iCurrentBuffer = iRead->EBufferTwo;
        EUNIT_ASSERT_NO_LEAVE( iRead->FillBufferL( iBuffi, iDLDataPathStub, KUidMediaTypeAudio ) );
        
        EUNIT_ASSERT_LEAVE( iRead->FillBufferL( NULL, iDLDataPathStub, KUidMediaTypeAudio ) );
        
        iRead->iBufferToReadExists = ETrue;
        EUNIT_ASSERT_LEAVE( iRead->FillBufferL( NULL, iDLDataPathStub, KUidMediaTypeAudio ) );
        }        
    else
        {
        EUNIT_ASSERT_LEAVE( iRead->FillBufferL( iBuffi, iDLDataPathStub, KUidMediaTypeMidi ) );    
        }
    }

void UT_CIlbcPayloadFormatRead::UT_CIlbcPayloadFormatRead_SourceThreadLogonL(  )
    {
    if ( !iAlloc )
        {
        EUNIT_ASSERT_EQUALS( iRead->SourceThreadLogon( *aEventH ), KErrNone );
        
        
        CMccRtpDataSource* rtpDataSourceStub = 
            reinterpret_cast<CMccRtpDataSource*>( MDataSource::NewSourceL( KRtpDataSource, KNullDesC8 ) );
        CleanupStack::PushL( rtpDataSourceStub );
        CMccRedPayloadRead* redDecoder = CMccRedPayloadRead::NewL( rtpDataSourceStub );
        CleanupStack::PushL( redDecoder );
                
        CIlbcPayloadFormatRead* read2 = CIlbcPayloadFormatRead::NewL( redDecoder );
        CleanupStack::PushL( read2 );
        
        read2->iClip = NULL;
        EUNIT_ASSERT_EQUALS( read2->SourceThreadLogon( *aEventH ), KErrNotReady );
        CleanupStack::PopAndDestroy( 3 ); 
        }
    
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iRead->SourceThreadLogon( *aEventH ) );
        }
    }

void UT_CIlbcPayloadFormatRead::UT_CIlbcPayloadFormatRead_SourceThreadLogoffL(  )
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

void UT_CIlbcPayloadFormatRead::UT_CIlbcPayloadFormatRead_NegotiateSourceLL(  )
    {
    EUNIT_ASSERT_NO_LEAVE( iRead->NegotiateSourceL( *iDLDataPathStub ) );
    }

void UT_CIlbcPayloadFormatRead::UT_CIlbcPayloadFormatRead_SourcePrimeLL(  )
    {
    EUNIT_ASSERT_NO_LEAVE( iRead->SourcePrimeL() );
    }

void UT_CIlbcPayloadFormatRead::UT_CIlbcPayloadFormatRead_SourcePlayLL(  )
    {
    iRead->SourceStopL();
    EUNIT_ASSERT_NO_LEAVE( iRead->SourcePlayL() );
    }

void UT_CIlbcPayloadFormatRead::UT_CIlbcPayloadFormatRead_SourcePauseLL(  )
    {
    EUNIT_ASSERT_NO_LEAVE( iRead->SourcePauseL() );
    }

void UT_CIlbcPayloadFormatRead::UT_CIlbcPayloadFormatRead_SourceStopLL(  )
    {
    EUNIT_ASSERT_NO_LEAVE( iRead->SourceStopL() );
    }

void UT_CIlbcPayloadFormatRead::UT_CIlbcPayloadFormatRead_BufferFilledLL(  )
    {
    TRtpRecvHeader test;
    test.iPayloadType = 13;
    if ( !iAlloc )
        { 
        CMMFDescriptorBuffer* buffi = CMMFDescriptorBuffer::NewL( 60 );
        CMMFBuffer* buf = buffi; 
        CleanupStack::PushL( buf );
        iRtpDataSourceStub->FillBufferL( buf, iConsumer, KUidMediaTypeAudio ); 
        buffi->Data().Append( _L("12345678901234567890123456789012345678901234567890123456789") );        
        EUNIT_ASSERT_NO_LEAVE( iRead->DataBufferFilledL( iRead->iSourceBuffer, test ) );
        CleanupStack::PopAndDestroy( buf );
        
        
        iRead->iNumOfFrames = 1;
        
        CMMFDescriptorBuffer* buffi2 = CMMFDescriptorBuffer::NewL( 38 );
        CMMFBuffer* buf2 = buffi2; 
        CleanupStack::PushL( buf2 );
        iRtpDataSourceStub->FillBufferL( buf2, iConsumer, KUidMediaTypeAudio ); 
        buffi2->Data().Append( _L("12345678901234567890123456789012345678") );        
        EUNIT_ASSERT_NO_LEAVE( iRead->DataBufferFilledL( iRead->iSourceBuffer, test ) );
        CleanupStack::PopAndDestroy( buf2 );
        }     
    else
        {
        CMMFDescriptorBuffer* buffi = CMMFDescriptorBuffer::NewL( 60 );
        CMMFBuffer* buf = buffi; 
        CleanupStack::PushL( buf );
        iRtpDataSourceStub->FillBufferL( buf, iConsumer, KUidMediaTypeAudio ); 
        EUNIT_ASSERT_NO_LEAVE( iRead->DataBufferFilledL( iRead->iSourceBuffer, test ) );
        CleanupStack::PopAndDestroy( buf );
        }
    }

void UT_CIlbcPayloadFormatRead::UT_CIlbcPayloadFormatRead_SinkDataTypeCodeL( )
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
         
   
void UT_CIlbcPayloadFormatRead::UT_CIlbcPayloadFormatRead_ConfigurePayloadFormatLL( )
    {
    iCodec.iAlgoUsed = EGenRedUsed;
    iCodec.iBitrate = 13330;
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
    
    CMccRtpDataSource* rtpDataSourceStub = 
        reinterpret_cast<CMccRtpDataSource*>( MDataSource::NewSourceL( KRtpDataSource, KNullDesC8 ) );
    CleanupStack::PushL( rtpDataSourceStub );
    CMccRedPayloadRead* redDecoder = CMccRedPayloadRead::NewL( rtpDataSourceStub );
    CleanupStack::PushL( redDecoder );
    CIlbcPayloadFormatRead* read2 = CIlbcPayloadFormatRead::NewL( redDecoder );
    CleanupStack::PushL( read2 );
    read2->ConfigurePayloadFormatL( buffer );  
   
   
    TMccCodecInfo codec;

    codec.iAlgoUsed = ENoAlgoUsed;
    codec.iBitrate = 15200;
    codec.iCNFrameSize = 0;
    codec.iCodecMode = 3;
    codec.iEnableDTX = 1;
    codec.iFourCC = 0x31313747;
    codec.iFrameSize = 0;
    codec.iHwFrameTime = 20;
    codec.iMaxPtime = 200;
    codec.iPayloadType = 8;
    codec.iPtime = 20;
    codec.iRedundancyCount = 1;
    codec.iRedundantPayload = 99;
    
    TMccCodecInfoBuffer buffer2( codec );
   
    codec.iIsUpdate = ETrue;
    codec.iBitrate = 13330;
    buffer =  codec;
    read2->ConfigurePayloadFormatL( buffer );         
    
 
    codec.iAlgoUsed = EGenRedUsed;
    codec.iRedundancyCount = 1;
    buffer2 = codec; 
    read2->ConfigurePayloadFormatL( buffer2 ); 
    
    codec.iHwFrameTime = 0;
    codec.iPtime = 30;
    buffer2 = codec; 
    EUNIT_ASSERT_SPECIFIC_LEAVE( read2->ConfigurePayloadFormatL( buffer2 ), KErrArgument ); 
   
    CleanupStack::PopAndDestroy( 3 );     
    } 
    
void UT_CIlbcPayloadFormatRead::UT_CIlbcPayloadFormatRead_UpdateConfigurationL()
    {
    TMccCodecInfo codec;

    codec.iAlgoUsed = ENoAlgoUsed;
    codec.iBitrate = 15200;
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
    
       
    
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CIlbcPayloadFormatRead,
    "MCC iLBC Payloadformat Unittests",
    "UNIT" )

EUNIT_TEST(
    "SendDataToSinkL - test ",
    "CIlbcPayloadFormatRead",
    "SendDataToSinkL",
    "FUNCTIONALITY",
    SetupL, UT_CIlbcPayloadFormatRead_SendDataToSinkLL, Teardown)

EUNIT_TEST(
    "FillSinkBuffer - test ",
    "CIlbcPayloadFormatRead",
    "FillSinkBuffer",
    "FUNCTIONALITY",
    SetupL, UT_CIlbcPayloadFormatRead_FillSinkBufferL, Teardown)

EUNIT_TEST(
    "FillSourceBufferL - test ",
    "CIlbcPayloadFormatRead",
    "FillSourceBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CIlbcPayloadFormatRead_FillSourceBufferLL, Teardown)

EUNIT_TEST(
    "Streams - test ",
    "CIlbcPayloadFormatRead",
    "Streams",
    "FUNCTIONALITY",
    SetupL, UT_CIlbcPayloadFormatRead_StreamsL, Teardown)

EUNIT_TEST(
    "FrameTimeInterval - test ",
    "CIlbcPayloadFormatRead",
    "FrameTimeInterval",
    "FUNCTIONALITY",
    SetupL, UT_CIlbcPayloadFormatRead_FrameTimeIntervalL, Teardown)

EUNIT_TEST(
    "Duration - test ",
    "CIlbcPayloadFormatRead",
    "Duration",
    "FUNCTIONALITY",
    SetupL, UT_CIlbcPayloadFormatRead_DurationL, Teardown)

EUNIT_TEST(
    "NumChannels - test ",
    "CIlbcPayloadFormatRead",
    "NumChannels",
    "FUNCTIONALITY",
    SetupL, UT_CIlbcPayloadFormatRead_NumChannelsL, Teardown)

EUNIT_TEST(
    "SampleRate - test ",
    "CIlbcPayloadFormatRead",
    "SampleRate",
    "FUNCTIONALITY",
    SetupL, UT_CIlbcPayloadFormatRead_SampleRateL, Teardown)

EUNIT_TEST(
    "SetSampleRate - test ",
    "CIlbcPayloadFormatRead",
    "SetSampleRate",
    "FUNCTIONALITY",
    SetupL, UT_CIlbcPayloadFormatRead_SetSampleRateL, Teardown)

EUNIT_TEST(
    "SourceDataTypeCode - test ",
    "CIlbcPayloadFormatRead",
    "SourceDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CIlbcPayloadFormatRead_SourceDataTypeCodeL, Teardown)

EUNIT_TEST(
    "SetSourceDataTypeCode - test ",
    "CIlbcPayloadFormatRead",
    "SetSourceDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CIlbcPayloadFormatRead_SetSourceDataTypeCodeL, Teardown)

EUNIT_TEST(
    "CreateSourceBufferL - test ",
    "CIlbcPayloadFormatRead",
    "CreateSourceBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CIlbcPayloadFormatRead_CreateSourceBufferLL, Teardown)

EUNIT_TEST(
    "CreateSourceBuffer1L - test ",
    "CIlbcPayloadFormatRead",
    "CreateSourceBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CIlbcPayloadFormatRead_CreateSourceBufferL_1L, Teardown)

EUNIT_TEST(
    "FillBufferL - test ",
    "CIlbcPayloadFormatRead",
    "FillBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CIlbcPayloadFormatRead_FillBufferLL, Teardown)

EUNIT_TEST(
    "SourceThreadLogon - test ",
    "CIlbcPayloadFormatRead",
    "SourceThreadLogon",
    "FUNCTIONALITY",
    SetupL, UT_CIlbcPayloadFormatRead_SourceThreadLogonL, Teardown)

EUNIT_TEST(
    "SourceThreadLogoff - test ",
    "CIlbcPayloadFormatRead",
    "SourceThreadLogoff",
    "FUNCTIONALITY",
    SetupL, UT_CIlbcPayloadFormatRead_SourceThreadLogoffL, Teardown)

EUNIT_TEST(
    "NegotiateSourceL - test ",
    "CIlbcPayloadFormatRead",
    "NegotiateSourceL",
    "FUNCTIONALITY",
    SetupL, UT_CIlbcPayloadFormatRead_NegotiateSourceLL, Teardown)

EUNIT_TEST(
    "SourcePrimeL - test ",
    "CIlbcPayloadFormatRead",
    "SourcePrimeL",
    "FUNCTIONALITY",
    SetupL, UT_CIlbcPayloadFormatRead_SourcePrimeLL, Teardown)

EUNIT_TEST(
    "SourcePlayL - test ",
    "CIlbcPayloadFormatRead",
    "SourcePlayL",
    "FUNCTIONALITY",
    SetupL, UT_CIlbcPayloadFormatRead_SourcePlayLL, Teardown)

EUNIT_TEST(
    "SourcePauseL - test ",
    "CIlbcPayloadFormatRead",
    "SourcePauseL",
    "FUNCTIONALITY",
    SetupL, UT_CIlbcPayloadFormatRead_SourcePauseLL, Teardown)

EUNIT_TEST(
    "SourceStopL - test ",
    "CIlbcPayloadFormatRead",
    "SourceStopL",
    "FUNCTIONALITY",
    SetupL, UT_CIlbcPayloadFormatRead_SourceStopLL, Teardown)

EUNIT_TEST(
    "BufferFilledL - test ",
    "CIlbcPayloadFormatRead",
    "BufferFilledL",
    "FUNCTIONALITY",
    SetupL, UT_CIlbcPayloadFormatRead_BufferFilledLL, Teardown)

EUNIT_TEST(
    "SinkDataTypeCode - test ",
    "CIlbcPayloadFormatRead",
    "SinkDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CIlbcPayloadFormatRead_SinkDataTypeCodeL, Teardown)
    
EUNIT_TEST(
    "ConfigurePayloadFormatL - test ",
    "CIlbcPayloadFormatRead",
    "ConfigurePayloadFormatL",
    "FUNCTIONALITY",
    SetupL, UT_CIlbcPayloadFormatRead_ConfigurePayloadFormatLL, Teardown)
 
EUNIT_TEST(
    "UpdateConfiguration - test ",
    "CIlbcPayloadFormatRead",
    "UpdateConfiguration",
    "FUNCTIONALITY",
    SetupL, UT_CIlbcPayloadFormatRead_UpdateConfigurationL, Teardown)   

// ALLOC TESTS
#ifdef EUNIT_ALLOC_TEST_ON

EUNIT_ALLOC_TEST(
    "SendDataToSinkL - alloc test ",
    "CIlbcPayloadFormatRead",
    "SendDataToSinkL",
    "ERRORHANDLING",
    SetupAL, UT_CIlbcPayloadFormatRead_SendDataToSinkLL, Teardown)

EUNIT_ALLOC_TEST(
    "FillSinkBuffer - alloc test ",
    "CIlbcPayloadFormatRead",
    "FillSinkBuffer",
    "ERRORHANDLING",
    SetupAL, UT_CIlbcPayloadFormatRead_FillSinkBufferL, Teardown)

EUNIT_ALLOC_TEST(
    "FillSourceBufferL - alloc test ",
    "CIlbcPayloadFormatRead",
    "FillSourceBufferL",
    "ERRORHANDLING",
    SetupAL, UT_CIlbcPayloadFormatRead_FillSourceBufferLL, Teardown)

EUNIT_ALLOC_TEST(
    "Streams - alloc test ",
    "CIlbcPayloadFormatRead",
    "Streams",
    "ERRORHANDLING",
    SetupAL, UT_CIlbcPayloadFormatRead_StreamsL, Teardown)

EUNIT_ALLOC_TEST(
    "FrameTimeInterval - alloc test ",
    "CIlbcPayloadFormatRead",
    "FrameTimeInterval",
    "ERRORHANDLING",
    SetupAL, UT_CIlbcPayloadFormatRead_FrameTimeIntervalL, Teardown)

EUNIT_ALLOC_TEST(
    "Duration - alloc test ",
    "CIlbcPayloadFormatRead",
    "Duration",
    "ERRORHANDLING",
    SetupAL, UT_CIlbcPayloadFormatRead_DurationL, Teardown)

EUNIT_ALLOC_TEST(
    "NumChannels - alloc test ",
    "CIlbcPayloadFormatRead",
    "NumChannels",
    "ERRORHANDLING",
    SetupAL, UT_CIlbcPayloadFormatRead_NumChannelsL, Teardown)

EUNIT_ALLOC_TEST(
    "SampleRate - alloc test ",
    "CIlbcPayloadFormatRead",
    "SampleRate",
    "ERRORHANDLING",
    SetupAL, UT_CIlbcPayloadFormatRead_SampleRateL, Teardown)

EUNIT_ALLOC_TEST(
    "SetSampleRate - alloc test ",
    "CIlbcPayloadFormatRead",
    "SetSampleRate",
    "ERRORHANDLING",
    SetupAL, UT_CIlbcPayloadFormatRead_SetSampleRateL, Teardown)

EUNIT_ALLOC_TEST(
    "SourceDataTypeCode - alloc test ",
    "CIlbcPayloadFormatRead",
    "SourceDataTypeCode",
    "ERRORHANDLING",
    SetupAL, UT_CIlbcPayloadFormatRead_SourceDataTypeCodeL, Teardown)

EUNIT_ALLOC_TEST(
    "SetSourceDataTypeCode - alloc test ",
    "CIlbcPayloadFormatRead",
    "SetSourceDataTypeCode",
    "ERRORHANDLING",
    SetupAL, UT_CIlbcPayloadFormatRead_SetSourceDataTypeCodeL, Teardown)

EUNIT_ALLOC_TEST(
    "CreateSourceBufferL - alloc test ",
    "CIlbcPayloadFormatRead",
    "CreateSourceBufferL",
    "ERRORHANDLING",
    SetupAL, UT_CIlbcPayloadFormatRead_CreateSourceBufferLL, Teardown)

EUNIT_ALLOC_TEST(
    "CreateSourceBuffer1L - alloc test ",
    "CIlbcPayloadFormatRead",
    "CreateSourceBufferL",
    "ERRORHANDLING",
    SetupAL, UT_CIlbcPayloadFormatRead_CreateSourceBufferL_1L, Teardown)

EUNIT_ALLOC_TEST(
    "FillBufferL - alloc test ",
    "CIlbcPayloadFormatRead",
    "FillBufferL",
    "ERRORHANDLING",
    SetupAL, UT_CIlbcPayloadFormatRead_FillBufferLL, Teardown)

EUNIT_ALLOC_TEST(
    "SourceThreadLogon - alloc test ",
    "CIlbcPayloadFormatRead",
    "SourceThreadLogon",
    "ERRORHANDLING",
    SetupAL, UT_CIlbcPayloadFormatRead_SourceThreadLogonL, Teardown)

EUNIT_ALLOC_TEST(
    "SourceThreadLogoff - alloc test ",
    "CIlbcPayloadFormatRead",
    "SourceThreadLogoff",
    "ERRORHANDLING",
    SetupAL, UT_CIlbcPayloadFormatRead_SourceThreadLogoffL, Teardown)


EUNIT_ALLOC_TEST(
    "NegotiateSourceL - alloc test ",
    "CIlbcPayloadFormatRead",
    "NegotiateSourceL",
    "ERRORHANDLING",
    SetupAL, UT_CIlbcPayloadFormatRead_NegotiateSourceLL, Teardown)

EUNIT_ALLOC_TEST(
    "SourcePrimeL - alloc test ",
    "CIlbcPayloadFormatRead",
    "SourcePrimeL",
    "ERRORHANDLING",
    SetupAL, UT_CIlbcPayloadFormatRead_SourcePrimeLL, Teardown)

EUNIT_ALLOC_TEST(
    "SourcePlayL - alloc test ",
    "CIlbcPayloadFormatRead",
    "SourcePlayL",
    "ERRORHANDLING",
    SetupAL, UT_CIlbcPayloadFormatRead_SourcePlayLL, Teardown)

EUNIT_ALLOC_TEST(
    "SourcePauseL - alloc test ",
    "CIlbcPayloadFormatRead",
    "SourcePauseL",
    "ERRORHANDLING",
    SetupAL, UT_CIlbcPayloadFormatRead_SourcePauseLL, Teardown)

EUNIT_ALLOC_TEST(
    "SourceStopL - alloc test ",
    "CIlbcPayloadFormatRead",
    "SourceStopL",
    "ERRORHANDLING",
    SetupAL, UT_CIlbcPayloadFormatRead_SourceStopLL, Teardown)

EUNIT_ALLOC_TEST(
    "BufferFilledL - alloc test ",
    "CIlbcPayloadFormatRead",
    "BufferFilledL",
    "ERRORHANDLING",
    SetupAL, UT_CIlbcPayloadFormatRead_BufferFilledLL, Teardown)

EUNIT_ALLOC_TEST(
    "SinkDataTypeCode - alloc test ",
    "CIlbcPayloadFormatRead",
    "SinkDataTypeCode",
    "ERRORHANDLING",
    SetupAL, UT_CIlbcPayloadFormatRead_SinkDataTypeCodeL, Teardown)

#endif

EUNIT_END_TEST_TABLE

//  END OF FILE
