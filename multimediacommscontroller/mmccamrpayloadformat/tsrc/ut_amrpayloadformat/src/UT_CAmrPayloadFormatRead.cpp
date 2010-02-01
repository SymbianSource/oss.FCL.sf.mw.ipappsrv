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



// ADD sentence "friend class UT_CAmrPayloadFormatRead" to AmrPayloadFormatRead.h
// 


//  CLASS HEADER
#include "UT_CAmrPayloadFormatRead.h"

#include "MccRtpDataSink.h"
#include "mccuids.hrh"
#include "Mccdldatapath.h"
#include "MccRtpDataSource.h"
#include "mccresourcepool.h"
#include "mccredpayloadread.h"
#include "amrpayloaddecoder.h"

//  EXTERNAL INCLUDES
#include <e32base.h>
#include <mmf/server/mmfvideoframebuffer.h>
#include <digia/eunit/eunitmacros.h>


#ifdef EUNIT_ALLOC_TEST_ON
    #include <digia/eunit/CEunitAllocTestCaseDecorator.h>   //for using EUNIT_ALLOC_TEST
#endif

//  INTERNAL INCLUDES
#include "amrpayloadformatread.h"

const TUid KRtpDataSource            =  { KImplUidRtpDataSource   }; 
const TUid KRtpDataSink              =  { KImplUidRtpDataSink     };

 

// CONSTRUCTION
UT_CAmrPayloadFormatRead* UT_CAmrPayloadFormatRead::NewL( )
    {
    UT_CAmrPayloadFormatRead* self = UT_CAmrPayloadFormatRead::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CAmrPayloadFormatRead* UT_CAmrPayloadFormatRead::NewLC()
    {
    UT_CAmrPayloadFormatRead* self = new( ELeave ) UT_CAmrPayloadFormatRead();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CAmrPayloadFormatRead::~UT_CAmrPayloadFormatRead()
    {
    }

// Default constructor
UT_CAmrPayloadFormatRead::UT_CAmrPayloadFormatRead()
    {
    }

// Second phase construct
void UT_CAmrPayloadFormatRead::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS
void UT_CAmrPayloadFormatRead::SetupAL(  )
    { 
    SetupL();
    iAlloc = ETrue; 
    }


void UT_CAmrPayloadFormatRead::SetupL(  )
    {
    iFourCC.Set( TFourCC( ' ','A','M','R' ) );

    iAlloc = EFalse; 

    iRtpDataSourceStub = MDataSource::NewSourceL( KRtpDataSource, KNullDesC8 );
    iMediaId = KUidMediaTypeAudio;
    MAsyncEventHandler* eventHandler = NULL;
    CMccResourcePool* mccresource = NULL;
    iDLDataPathStub = CMccDlDataPath::NewL( eventHandler, mccresource, iMediaId ); 

    iRead = CAmrPayloadFormatRead::NewL( iRtpDataSourceStub );
    iBuf = CMMFDescriptorBuffer::NewL( 300 );
    iBuffi = CMMFDescriptorBuffer::NewL( 300 );
    iBuffi->Data().Append( _L("1234512345") ); 
    iConsumer = MDataSink::NewSinkL( KRtpDataSink, KNullDesC8 );
    
    iCodec.iFourCC.Set(TFourCC(' ','A','M','R') );
    iCodec.iBitrate = 12200;
    iCodec.iCodecMode = KAmrCodecModeOctetAlign;
    iCodec.iPtime = 160;
    iCodec.iMaxPtime = 200;
    iCodec.iPayloadType = 106;
    iCodec.iRedundancyCount = 0;
    iCodec.iEnableDTX = EFalse;
    //iCodec.iFwStream = ETrue;
    iCodec.iRedundantPayload = 0;
    iCodec.iHwFrameTime = 20;
    iCodec.iFrameSize = 32;
    iCodec.iAlgoUsed = 0;
    iCodec.iPayloadFormatEncoder = KImplUidAmrPayloadFormatEncode;
    iCodec.iPayloadFormatDecoder = KImplUidAmrPayloadFormatDecode;
    
  
    TMccCodecInfoBuffer buffer( iCodec );
    
    RDebug::Print( _L("- Algo used       = %d"), iCodec.iAlgoUsed ); // 0 jos redundanssi ei päällä, 2 jos haluat asettaa sen päälle
    RDebug::Print( _L("- Bitrate         = %d"), iCodec.iBitrate ); // 64000
    RDebug::Print( _L("- CNFrameSize     = %d"), iCodec.iCNFrameSize ); // 0
    RDebug::Print( _L("- CodecMode       = %d"), iCodec.iCodecMode ); // 3
    RDebug::Print( _L("- EnableDTX       = %d"), iCodec.iEnableDTX ); // 1
    RDebug::Print( _L("- FourCC          = %x"), iCodec.iFourCC ); // 31313747
    RDebug::Print( _L("- FrameSize       = %d"), iCodec.iFrameSize ); // 0
    RDebug::Print( _L("- HWFrameTime     = %d"), iCodec.iHwFrameTime ); // 20
    RDebug::Print( _L("- MaxPTime        = %d"), iCodec.iMaxPtime ); // 200
    RDebug::Print( _L("- PayloadType     = %d"), iCodec.iPayloadType ); // 106
    RDebug::Print( _L("- PTime           = %d"), iCodec.iPtime ); // 160
    RDebug::Print( _L("- RedundancyCount = %d"), iCodec.iRedundancyCount ); // 0 jos redundanssi ei päällä, 1 jos redundanssi päällä
    RDebug::Print( _L("- RedPayload      = %d"), iCodec.iRedundantPayload ); // 0 jos redundanssi ei päällä, 97-127 jos redundanssi päällä
  
    iRead->SetSourceDataTypeCode( iFourCC, KUidMediaTypeAudio );
    iRead->SourceThreadLogon( *this );
    
    iRead->ConfigurePayloadFormatL( buffer );

    iRead->NegotiateSourceL( *iDLDataPathStub );
    
    iRead->SourcePrimeL();
    iRead->SourcePlayL();
    } 


void UT_CAmrPayloadFormatRead::SetupWbAL(  )
    { 
    SetupWbL();
    iAlloc = ETrue; 
    }


void UT_CAmrPayloadFormatRead::SetupWbL(  )
    {
    iFourCC.Set( TFourCC( ' ','A','W','B' ) );

    iAlloc = EFalse; 

    iRtpDataSourceStub = MDataSource::NewSourceL( KRtpDataSource, KNullDesC8 );
    iMediaId = KUidMediaTypeAudio;
    MAsyncEventHandler* eventHandler = NULL;
    CMccResourcePool* mccresource = NULL;
    iDLDataPathStub = CMccDlDataPath::NewL( eventHandler, mccresource, iMediaId ); 

    iRead = CAmrPayloadFormatRead::NewL( iRtpDataSourceStub );
    iBuf = CMMFDescriptorBuffer::NewL( 300 );
    iBuffi = CMMFDescriptorBuffer::NewL( 300 );
    iBuffi->Data().Append( _L("1234512345") ); 
    iConsumer = MDataSink::NewSinkL( KRtpDataSink, KNullDesC8 );
    
    iCodec.iFourCC.Set(TFourCC(' ','A','W','B') );
    iCodec.iBitrate = 23850;
    iCodec.iCodecMode = KAmrCodecModeOctetAlign;
    iCodec.iPtime = 160;
    iCodec.iMaxPtime = 200;
    iCodec.iPayloadType = 106;
    iCodec.iRedundancyCount = 0;
    iCodec.iEnableDTX = EFalse;
    //iCodec.iFwStream = ETrue;
    iCodec.iRedundantPayload = 0;
    iCodec.iHwFrameTime = 20;
    iCodec.iFrameSize = 64;
    iCodec.iAlgoUsed = 0;
    iCodec.iPayloadFormatEncoder = KImplUidAmrPayloadFormatEncode;
    iCodec.iPayloadFormatDecoder = KImplUidAmrPayloadFormatDecode;
    
  
    TMccCodecInfoBuffer buffer( iCodec );
    
    RDebug::Print( _L("- Algo used       = %d"), iCodec.iAlgoUsed ); // 0 jos redundanssi ei päällä, 2 jos haluat asettaa sen päälle
    RDebug::Print( _L("- Bitrate         = %d"), iCodec.iBitrate ); // 64000
    RDebug::Print( _L("- CNFrameSize     = %d"), iCodec.iCNFrameSize ); // 0
    RDebug::Print( _L("- CodecMode       = %d"), iCodec.iCodecMode ); // 3
    RDebug::Print( _L("- EnableDTX       = %d"), iCodec.iEnableDTX ); // 1
    RDebug::Print( _L("- FourCC          = %x"), iCodec.iFourCC ); // 31313747
    RDebug::Print( _L("- FrameSize       = %d"), iCodec.iFrameSize ); // 0
    RDebug::Print( _L("- HWFrameTime     = %d"), iCodec.iHwFrameTime ); // 20
    RDebug::Print( _L("- MaxPTime        = %d"), iCodec.iMaxPtime ); // 200
    RDebug::Print( _L("- PayloadType     = %d"), iCodec.iPayloadType ); // 106
    RDebug::Print( _L("- PTime           = %d"), iCodec.iPtime ); // 160
    RDebug::Print( _L("- RedundancyCount = %d"), iCodec.iRedundancyCount ); // 0 jos redundanssi ei päällä, 1 jos redundanssi päällä
    RDebug::Print( _L("- RedPayload      = %d"), iCodec.iRedundantPayload ); // 0 jos redundanssi ei päällä, 97-127 jos redundanssi päällä
  
    iRead->SetSourceDataTypeCode( iFourCC, KUidMediaTypeAudio );
    iRead->SourceThreadLogon( *this );
    
    iRead->ConfigurePayloadFormatL( buffer );

    iRead->NegotiateSourceL( *iDLDataPathStub );
    
    iRead->SourcePrimeL();
    iRead->SourcePlayL();
    } 


void UT_CAmrPayloadFormatRead::Teardown(  )
    {

    delete iBuffi;
    delete iDLDataPathStub;   
    delete iRtpDataSourceStub;
    delete iRead; 
    delete iBuf;  
    delete iConsumer;
    delete iFrameData;
    iFrameData = NULL;
    REComSession::FinalClose();
    }

// HELPERS

void UT_CAmrPayloadFormatRead::CreateFramesL( TInt aNumFrames, TBool aOkData )
    {
    iRead->iNumOfFrames = aNumFrames;
    iRead->iCurrentFrame = 0;
    __ASSERT_ALWAYS( iRead->iPayloadDecoder, User::Leave( KErrNotReady ) );
    for ( TInt i = 0; i < aNumFrames; i++ )
        {
        // Add some damaged frames
        TAmrFrame frame;
        if ( aOkData )
            {
            if ( !iFrameData )
                {
                iFrameData = HBufC8::NewL( 300 );
                iFrameData->Des().Append( _L8("ð¼¼¼¼¼¼¼<‘(˜Èž\x01À‰å/áPv\x14€") );
                iFrameData->Des().SetLength( iFrameData->Des().MaxLength() );
                }
            frame.iFrameType = EAmrFrame12_2;
            frame.iFrameQualityInd = 1;
            frame.iFrameData = const_cast<TUint8*>( iFrameData->Des().Ptr() );
            }
        else
            {
            frame.iFrameType = EAmrFrameNoData;
            frame.iFrameQualityInd = 0;
            }
        iRead->iPayloadDecoder->iFrames[ i ] = frame;
        }
    }

// TEST FUNCTIONS

void UT_CAmrPayloadFormatRead::UT_CAmrPayloadFormatRead_CancelDlRequestL(  )
    {
    EUNIT_ASSERT_NO_LEAVE( iRead->CancelDlRequest() );
    delete iRead->iFrameBufferOne;
    iRead->iFrameBufferOne = NULL;
    EUNIT_ASSERT_NO_LEAVE( iRead->CancelDlRequest() );
    }

void UT_CAmrPayloadFormatRead::UT_CAmrPayloadFormatRead_SendDataToSinkLL(  )
    {
    if ( !iAlloc )
        {       
        EUNIT_ASSERT_NO_LEAVE( iRead->FillBufferL( iBuffi, iDLDataPathStub, iMediaId ) );
        EUNIT_ASSERT_NO_LEAVE( iRead->SendDataToSinkL() );
        EUNIT_ASSERT_NO_LEAVE( iRead->FillBufferL( iBuffi, iDLDataPathStub, iMediaId ) );
        EUNIT_ASSERT_NO_LEAVE( iRead->SendDataToSinkL() );
        
        iRead->iBufferToReadExists = ETrue;
        iRead->iFrameBufferOne->SetLastBuffer( ETrue );
        EUNIT_ASSERT_NO_LEAVE( iRead->SendDataToSinkL() );
        iRead->iBufferToReadExists = ETrue;
        iRead->iFrameBufferOne->SetLastBuffer( ETrue );
        EUNIT_ASSERT_NO_LEAVE( iRead->SendDataToSinkL() );
        }        
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iRead->FillBufferL( iBuffi, iDLDataPathStub, iMediaId ) );
        EUNIT_ASSERT_NO_LEAVE( iRead->SendDataToSinkL() );       
        } 
    }

void UT_CAmrPayloadFormatRead::UT_CAmrPayloadFormatRead_FillSinkBufferL(  )
    {
    if ( !iAlloc )
        {
        EUNIT_ASSERT_NO_LEAVE( iRead->FillSinkBufferL() );
        iRead->iBufferToReadExists = 1;
        EUNIT_ASSERT_NO_LEAVE( iRead->FillSinkBufferL() );  
        iRead->iCurrentBuffer = CAmrPayloadFormatRead::EBufferTwo;
        
        // No data
        iRead->iFrameBufferTwo->Data().SetLength( 0 );  
        EUNIT_ASSERT_NO_LEAVE( iRead->FillSinkBufferL() );  
        EUNIT_ASSERT( iRead->iStateMachine->CurrentState() == EDecodeIdle );
        
        // Some data to forward
        iRead->iBufferToRead = iBuf;
        iRead->iFrameBufferTwo->Data().SetLength( 1 );  
        CreateFramesL( 1, ETrue );
        EUNIT_ASSERT_NO_LEAVE( iRead->FillSinkBufferL() );
        }        
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iRead->FillSinkBufferL() );
        
        } 
    }

void UT_CAmrPayloadFormatRead::UT_CAmrPayloadFormatRead_FillSourceBufferLL(  )
    {
    EUNIT_ASSERT_NO_LEAVE( iRead->FillSourceBufferL() );
    }

void UT_CAmrPayloadFormatRead::UT_CAmrPayloadFormatRead_StreamsL(  )
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

void UT_CAmrPayloadFormatRead::UT_CAmrPayloadFormatRead_FrameTimeIntervalL(  )
    {
    
    if ( !iAlloc )
        {
        RDebug::Print( _L("**FrameInterval = %d"), iRead->FrameTimeInterval( KUidMediaTypeAudio ).Int64() );
        EUNIT_ASSERT_EQUALS( iRead->FrameTimeInterval( KUidMediaTypeAudio ).Int64(), 20 );
        EUNIT_ASSERT_EQUALS( iRead->FrameTimeInterval( KUidMediaTypeVideo ).Int64(), 0 );
        
        }
        
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iRead->FrameTimeInterval( KUidMediaTypeAudio ) );
        EUNIT_ASSERT_NO_LEAVE( iRead->FrameTimeInterval( KUidMediaTypeVideo ) );
        
        }

    }

void UT_CAmrPayloadFormatRead::UT_CAmrPayloadFormatRead_DurationL(  )
    {
    if ( !iAlloc )
        {
        /*
        EUNIT_ASSERT_EQUALS( iRead->Duration( KUidMediaTypeAudio ).Int64(), 1000000000 );
        EUNIT_ASSERT_EQUALS( iRead->Duration( KUidMediaTypeMidi ).Int64(), 1000000000 );
        */
        EUNIT_ASSERT_EQUALS( iRead->Duration( KUidMediaTypeMidi ).Int64(), 0 );
        
        }
        
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iRead->Duration( KUidMediaTypeAudio ) );
        
        }

    }

void UT_CAmrPayloadFormatRead::UT_CAmrPayloadFormatRead_NumChannelsL(  )
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

void UT_CAmrPayloadFormatRead::UT_CAmrPayloadFormatRead_SampleRateL(  )
    {
    
    if ( !iAlloc )
        {
        EUNIT_ASSERT_EQUALS( iRead->SampleRate(), iRead->iSamplingRate );
        
        }
        
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iRead->SampleRate() );
        
        }
    }

void UT_CAmrPayloadFormatRead::UT_CAmrPayloadFormatRead_SetSampleRateL(  )
    {
    TUint sample(0);
    if ( !iAlloc )
        {
        sample = iRead->SampleRate(); // 8000
        EUNIT_ASSERT_EQUALS( iRead->SetSampleRate( sample ), KErrNone );
        EUNIT_ASSERT_EQUALS( iRead->SetSampleRate( sample + 1 ), KErrNotSupported );
        EUNIT_ASSERT_EQUALS( iRead->SetSampleRate( sample - 8000 ), KErrNotSupported );
        EUNIT_ASSERT_EQUALS( iRead->SetSampleRate( 8000 ), KErrNone );
        EUNIT_ASSERT_EQUALS( iRead->SetSampleRate( 16000 ), KErrNotSupported );
        
        }
        
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iRead->SetSampleRate( sample ) );
        EUNIT_ASSERT_NO_LEAVE( iRead->SetSampleRate( sample + 1 ) );
        
        }
    }

void UT_CAmrPayloadFormatRead::UT_CAmrPayloadFormatRead_SetSampleRateWbL(  )
    {
    TUint sample(0);
    if ( !iAlloc )
        {
        sample = iRead->SampleRate(); // 16000
        EUNIT_ASSERT_EQUALS( iRead->SetSampleRate( sample ), KErrNone );
        EUNIT_ASSERT_EQUALS( iRead->SetSampleRate( sample + 1 ), KErrNotSupported );
        EUNIT_ASSERT_EQUALS( iRead->SetSampleRate( sample - 8000 ), KErrNotSupported );
        EUNIT_ASSERT_EQUALS( iRead->SetSampleRate( 8000 ), KErrNotSupported );
        EUNIT_ASSERT_EQUALS( iRead->SetSampleRate( 16000 ), KErrNone );
        
        }
        
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iRead->SetSampleRate( sample ) );
        EUNIT_ASSERT_NO_LEAVE( iRead->SetSampleRate( sample + 1 ) );
        
        }
    }

void UT_CAmrPayloadFormatRead::UT_CAmrPayloadFormatRead_FillBufferLL(  )
    {
    if ( !iAlloc )
        {
        iRead->iCurrentBuffer = iRead->EBufferOne;
        EUNIT_ASSERT_NO_LEAVE( iRead->FillBufferL( iBuffi, iDLDataPathStub, KUidMediaTypeAudio ) );
        
        EUNIT_ASSERT_LEAVE( iRead->FillBufferL( iBuffi, iDLDataPathStub, KUidMediaTypeVideo ) ); // KErrNotSupported
        
        iRead->iCurrentBuffer = iRead->EBufferTwo;
        EUNIT_ASSERT_NO_LEAVE( iRead->FillBufferL( iBuffi, iDLDataPathStub, KUidMediaTypeAudio ) );
        
        EUNIT_ASSERT_LEAVE( iRead->FillBufferL( NULL, iDLDataPathStub, KUidMediaTypeAudio ) );
        }        
    else
        {
        EUNIT_ASSERT_LEAVE( iRead->FillBufferL( iBuffi, iDLDataPathStub, KUidMediaTypeMidi ) );    
        }
    }

void UT_CAmrPayloadFormatRead::UT_CAmrPayloadFormatRead_SourcePrimeLL(  ) 
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

void UT_CAmrPayloadFormatRead::UT_CAmrPayloadFormatRead_SourcePlayLL(  )
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

void UT_CAmrPayloadFormatRead::UT_CAmrPayloadFormatRead_SourcePauseLL(  )
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

void UT_CAmrPayloadFormatRead::UT_CAmrPayloadFormatRead_SourceStopLL(  )
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


void UT_CAmrPayloadFormatRead::UT_CAmrPayloadFormatRead_CreateSourceBufferLL(  )
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

void UT_CAmrPayloadFormatRead::UT_CAmrPayloadFormatRead_CreateSourceBufferL_1L(  )
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

void UT_CAmrPayloadFormatRead::UT_CAmrPayloadFormatRead_SourceDataTypeCodeL(  )
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

void UT_CAmrPayloadFormatRead::UT_CAmrPayloadFormatRead_SetSourceDataTypeCodeL(  )
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

void UT_CAmrPayloadFormatRead::UT_CAmrPayloadFormatRead_SourceThreadLogonL(  )
    {
    if ( !iAlloc )
        {
        EUNIT_ASSERT_EQUALS( iRead->SourceThreadLogon( *aEventH ), KErrNone );
            
        }
    
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iRead->SourceThreadLogon( *aEventH ) );
        
        }
    }

void UT_CAmrPayloadFormatRead::UT_CAmrPayloadFormatRead_SourceThreadLogoffL(  )
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


void UT_CAmrPayloadFormatRead::UT_CAmrPayloadFormatRead_NegotiateSourceLL(  )
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


void UT_CAmrPayloadFormatRead::UT_CAmrPayloadFormatRead_BufferFilledLL(  )
    {
    TRtpRecvHeader test;
    test.iMarker = 1;
    test.iPadding = 1;
    
    if ( !iAlloc )
        {
        CMMFDataBuffer* buffi = CMMFDataBuffer::NewL( 200 );      
        CMMFBuffer* buf = buffi; 
        CleanupStack::PushL( buf );
        iRtpDataSourceStub->FillBufferL( buf, iConsumer, KUidMediaTypeAudio );
        iRead->DataBufferFilledL( iRead->iSourceBuffer, test );
        CleanupStack::PopAndDestroy( buf );
        
        // NULL buffer
        EUNIT_ASSERT_SPECIFIC_LEAVE( iRead->DataBufferFilledL( NULL, test ), KErrArgument );
        
        // Wrong buffer type
        CMMFYUVBuffer* yuvBuf = CMMFYUVBuffer::NewL();  
        CleanupStack::PushL( yuvBuf ); 
        EUNIT_ASSERT_SPECIFIC_LEAVE( iRead->DataBufferFilledL( yuvBuf, test ), KErrNotSupported );
        CleanupStack::PopAndDestroy( yuvBuf );
        
        // Wrong buffer   
        CMMFDataBuffer* buffi2 = CMMFDataBuffer::NewL( 200 );     
        CleanupStack::PushL( buffi2 ); 
        EUNIT_ASSERT_SPECIFIC_LEAVE( iRead->DataBufferFilledL( buffi2, test ), KErrArgument );
        CleanupStack::PopAndDestroy( buffi2 );
        
        // Some data
        iRead->iSourceBuffer->Data().Append( _L8("ð¼¼¼¼¼¼¼<‘(˜Èž\x01À‰å/áPv\x14€") );
        iRead->DataBufferFilledL( iRead->iSourceBuffer, test );
        }
    
    else
        {
        CMMFDataBuffer* buffi = CMMFDataBuffer::NewL( 200 );      
        CMMFBuffer* buf = buffi; 
        CleanupStack::PushL( buf );
        iRtpDataSourceStub->FillBufferL( buf, iConsumer, KUidMediaTypeAudio );
        iRead->DataBufferFilledL( iRead->iSourceBuffer, test );
        CleanupStack::PopAndDestroy( buf );
        
        }
    }
    
void UT_CAmrPayloadFormatRead::UT_CAmrPayloadFormatRead_ConfigurePayloadFormatLL(  )
    {   
    if ( !iAlloc )
        {
                
        TMccCodecInfo codec;
        codec.iFourCC.Set(TFourCC(' ','A','M','R') );
        codec.iBitrate = 12200;
        codec.iCodecMode = KAmrCodecModeOctetAlign;
        codec.iPtime = 160;
        codec.iMaxPtime = 200;
        codec.iPayloadType = 106;
        codec.iRedundancyCount = 0;
        codec.iEnableDTX = EFalse;
        codec.iRedundantPayload = 0;
        codec.iHwFrameTime = 20;
        codec.iFrameSize = 32;
        codec.iAlgoUsed = 0;
        codec.iPayloadFormatEncoder = KImplUidAmrPayloadFormatEncode;
        codec.iPayloadFormatDecoder = KImplUidAmrPayloadFormatDecode;
        
        TMccCodecInfoBuffer buffer( codec );
        iRead->ConfigurePayloadFormatL( buffer );

        codec.iAlgoUsed = EGenRedUsed;
        codec.iRedundancyCount = 1;
        codec.iRedundantPayload = 99;
        
        buffer = codec; 
        CMccRtpDataSource* rtpDataSourceStub = static_cast<CMccRtpDataSource*>(MDataSource::NewSourceL( KRtpDataSource, KNullDesC8 ));
        CleanupStack::PushL( rtpDataSourceStub );
        CMccRedPayloadRead* redDecoder = CMccRedPayloadRead::NewL( rtpDataSourceStub );
        CleanupStack::PushL( redDecoder );
        CAmrPayloadFormatRead* read = CAmrPayloadFormatRead::NewL( redDecoder );
        CleanupStack::PushL( read );
        read->ConfigurePayloadFormatL( buffer );
        CleanupStack::PopAndDestroy( 3 ); 
        
        codec.iAlgoUsed = EAmrFecUsed;
        buffer = codec; 
        iRead->ConfigurePayloadFormatL( buffer );      
        
        // Wrong mode
        delete iRead->iPayloadDecoder;
        iRead->iPayloadDecoder = NULL;
        codec.iCodecMode = 999;
        buffer = codec; 
        EUNIT_ASSERT_SPECIFIC_LEAVE( iRead->ConfigurePayloadFormatL( buffer ), KErrArgument );
        
         // Test different mode and no red
        delete iRead->iPayloadDecoder;
        iRead->iPayloadDecoder = NULL;
        codec.iCodecMode = KAmrCodecModeBandwidthEfficient;
        codec.iMaxRed = 2;
        codec.iRedundancyCount = 0;
        buffer = codec; 
        iRead->ConfigurePayloadFormatL( buffer );
        
        // Test source buffer created by source, incorrect type
        static_cast<CMccRtpDataSource*>( iRtpDataSourceStub )->iCanCreateSourceBuffer = ETrue;
        static_cast<CMccRtpDataSource*>( iRtpDataSourceStub )->iWrongBufferType = ETrue;

        TRAPD( err, iRead->ConfigurePayloadFormatL( buffer ) )
        if ( err == KErrNoMemory )
        	{
        	User::Leave( err );
        	}
        EUNIT_ASSERT_EQUALS( KErrNotSupported, err );

        // Correct type
        static_cast<CMccRtpDataSource*>( iRtpDataSourceStub )->iWrongBufferType = EFalse;
        iRead->ConfigurePayloadFormatL( buffer );
        
        // Ownership stays in source
        iRead->iSourceBuffer = NULL; // Source will delete the buffer
        static_cast<CMccRtpDataSource*>( iRtpDataSourceStub )->iReference = ETrue;
        iRead->ConfigurePayloadFormatL( buffer );
        
        // Incorrect type and ownership in source
        iRead->iSourceBuffer = NULL; // Source will delete the buffer
        static_cast<CMccRtpDataSource*>( iRtpDataSourceStub )->iWrongBufferType = ETrue;

        TRAPD( err2, iRead->ConfigurePayloadFormatL( buffer ) )
        if ( err2 == KErrNoMemory )
        	{
        	User::Leave( err2 );
        	}
        EUNIT_ASSERT_EQUALS( KErrNotSupported, err2 );
        
        // No hw frametime
        static_cast<CMccRtpDataSource*>( iRtpDataSourceStub )->iCanCreateSourceBuffer = EFalse;
        codec.iHwFrameTime = 0;
        buffer = codec; 

        TRAPD( err3, iRead->ConfigurePayloadFormatL( buffer ) )
        if ( err3 == KErrNoMemory )
        	{
        	User::Leave( err3 );
        	}
        EUNIT_ASSERT_EQUALS( KErrArgument, err3 );


        
        // Update
        codec.iIsUpdate = ETrue;
        buffer = codec; 
        iRead->ConfigurePayloadFormatL( buffer );
        
        // Maxptime changed update
        codec.iMaxPtime = 400;
        buffer = codec;
        iRead->ConfigurePayloadFormatL( buffer );
        
        // Fec
        codec.iMaxPtime = 200;
        codec.iAlgoUsed = EAmrFecUsed;
        buffer = codec;
        iRead->ConfigurePayloadFormatL( buffer );
        }
    
    else
        {
        EUNIT_ASSERT( ETrue );
        }          
    }
 
void UT_CAmrPayloadFormatRead::UT_CAmrPayloadFormatRead_ConfigurePayloadFormatLWbL(  )
    {   
    if ( !iAlloc )
        {
                
        TMccCodecInfo codec;
        codec.iFourCC.Set(TFourCC(' ','A','W','B') );
        codec.iBitrate = 23850;
        codec.iCodecMode = KAmrCodecModeOctetAlign;
        codec.iPtime = 160;
        codec.iMaxPtime = 200;
        codec.iPayloadType = 106;
        codec.iRedundancyCount = 0;
        codec.iEnableDTX = EFalse;
        codec.iRedundantPayload = 0;
        codec.iHwFrameTime = 20;
        codec.iFrameSize = 64;
        codec.iAlgoUsed = 0;
        codec.iPayloadFormatEncoder = KImplUidAmrPayloadFormatEncode;
        codec.iPayloadFormatDecoder = KImplUidAmrPayloadFormatDecode;
        
        TMccCodecInfoBuffer buffer( codec );
        iRead->ConfigurePayloadFormatL( buffer );

        codec.iAlgoUsed = EGenRedUsed;
        codec.iRedundancyCount = 1;
        codec.iRedundantPayload = 99;
        
        buffer = codec; 
        CMccRtpDataSource* rtpDataSourceStub = static_cast<CMccRtpDataSource*>(MDataSource::NewSourceL( KRtpDataSource, KNullDesC8 ));
        CleanupStack::PushL( rtpDataSourceStub );
        CMccRedPayloadRead* redDecoder = CMccRedPayloadRead::NewL( rtpDataSourceStub );
        CleanupStack::PushL( redDecoder );
        CAmrPayloadFormatRead* read = CAmrPayloadFormatRead::NewL( redDecoder );
        CleanupStack::PushL( read );
        read->ConfigurePayloadFormatL( buffer );
        CleanupStack::PopAndDestroy( 3 ); 
        
        codec.iAlgoUsed = EAmrFecUsed;
        buffer = codec; 
        iRead->ConfigurePayloadFormatL( buffer );      
        
        // Wrong mode
        delete iRead->iPayloadDecoder;
        iRead->iPayloadDecoder = NULL;
        codec.iCodecMode = 999;
        buffer = codec; 
        EUNIT_ASSERT_SPECIFIC_LEAVE( iRead->ConfigurePayloadFormatL( buffer ), KErrArgument );
        
         // Test different mode and no red
        delete iRead->iPayloadDecoder;
        iRead->iPayloadDecoder = NULL;
        codec.iCodecMode = KAmrCodecModeBandwidthEfficient;
        codec.iMaxRed = 2;
        codec.iRedundancyCount = 0;
        buffer = codec; 
        iRead->ConfigurePayloadFormatL( buffer );
        
        // Test source buffer created by source, incorrect type
        static_cast<CMccRtpDataSource*>( iRtpDataSourceStub )->iCanCreateSourceBuffer = ETrue;
        static_cast<CMccRtpDataSource*>( iRtpDataSourceStub )->iWrongBufferType = ETrue;
        
        TRAPD( err,iRead->ConfigurePayloadFormatL( buffer ) )
        if ( err == KErrNoMemory )
        	{
        	User::Leave( err );
        	}
        EUNIT_ASSERT_EQUALS( KErrNotSupported, err );

        // Correct type
        static_cast<CMccRtpDataSource*>( iRtpDataSourceStub )->iWrongBufferType = EFalse;
        iRead->ConfigurePayloadFormatL( buffer );
        
        // Ownership stays in source
        iRead->iSourceBuffer = NULL; // Source will delete the buffer
        static_cast<CMccRtpDataSource*>( iRtpDataSourceStub )->iReference = ETrue;
        iRead->ConfigurePayloadFormatL( buffer );
        
        // Incorrect type and ownership in source
        iRead->iSourceBuffer = NULL; // Source will delete the buffer
        static_cast<CMccRtpDataSource*>( iRtpDataSourceStub )->iWrongBufferType = ETrue;
        
        TRAPD(err2, iRead->ConfigurePayloadFormatL( buffer ))
        if ( err2 == KErrNoMemory )
        	{
        	User::Leave( err2 );
        	}
        EUNIT_ASSERT_EQUALS( KErrNotSupported,err2 );
        
        // No hw frametime
        static_cast<CMccRtpDataSource*>( iRtpDataSourceStub )->iCanCreateSourceBuffer = EFalse;
        codec.iHwFrameTime = 0;
        buffer = codec; 
        
        TRAPD(err3, iRead->ConfigurePayloadFormatL( buffer ))
        if ( err3 == KErrNoMemory )
        	{
        	User::Leave( err3 );
        	}
        EUNIT_ASSERT_EQUALS( KErrArgument,err3 );
        
        
        // Update
        codec.iIsUpdate = ETrue;
        buffer = codec; 
        iRead->ConfigurePayloadFormatL( buffer );
        
        // Maxptime changed update
        codec.iMaxPtime = 400;
        buffer = codec;
        iRead->ConfigurePayloadFormatL( buffer );
        
        // Fec
        codec.iMaxPtime = 200;
        codec.iAlgoUsed = EAmrFecUsed;
        buffer = codec;
        iRead->ConfigurePayloadFormatL( buffer );
        }
    
    else
        {
        EUNIT_ASSERT( ETrue );
        }          
    }
 

void UT_CAmrPayloadFormatRead::UT_CAmrPayloadFormatRead_ConvertModeToBitrate()
{
    EUNIT_ASSERT_EQUALS( iRead->ConvertModeToBitrate( EAmrModeReqNone ), 0 );
    EUNIT_ASSERT_EQUALS( iRead->ConvertModeToBitrate( EAmrModeReq8 ), 0 );
    EUNIT_ASSERT_EQUALS( iRead->ConvertModeToBitrate( EAmrModeReq5 ), 7950 );
}

void UT_CAmrPayloadFormatRead::UT_CAmrPayloadFormatRead_ConvertModeToBitrateWb()
{
    EUNIT_ASSERT_EQUALS( iRead->ConvertModeToBitrate( EAmrModeReqNone ), 0 );
    EUNIT_ASSERT_EQUALS( iRead->ConvertModeToBitrate( EAmrModeReq8 ), 23850 );
    EUNIT_ASSERT_EQUALS( iRead->ConvertModeToBitrate( EAmrModeReq5 ), 18250 );
}

void UT_CAmrPayloadFormatRead::UT_CAmrPayloadFormatRead_SendAmrEventToClient()
{
    iRead->SendAmrEventToClient( EMccInternalRtpSinkError );
}

// PRIVAT   

void UT_CAmrPayloadFormatRead::UT_CAmrPayloadFormatRead_DoReadL( )
    {
    TRtpRecvHeader test;
    test.iPadding = 1;
    test.iMarker = 1;
     
    iRead->DataBufferFilledL( iRead->iSourceBuffer/*iBuffi*/, test );
    if ( !iAlloc )
        {
        // Null buffer
        CMMFDescriptorBuffer* buf = iBuf;
        iBuf = NULL;
        EUNIT_ASSERT_EQUALS( iRead->DoRead( iBuf ), EFalse );
        
        // No frames
        iBuf = buf;    
        EUNIT_ASSERT_EQUALS( iRead->DoRead( iBuf ), EFalse );
        
        // Full buffer        
        iBuf->SetStatus( EFull );   
        EUNIT_ASSERT_EQUALS( iRead->DoRead( iBuf ), EFalse );
        
        // One frame, no real data
        iBuf->SetStatus( EAvailable );
        iRead->iBufferToRead = iBuf;
        CreateFramesL( 1 );
        EUNIT_ASSERT_EQUALS( iRead->DoRead( iBuf ), ETrue );
        
        // Two frames, first buffer
        iBuf->SetStatus( EAvailable );
        iRead->iBufferToRead = iBuf;
        CreateFramesL( 2 );
        EUNIT_ASSERT_EQUALS( iRead->DoRead( iBuf ), EFalse );
        EUNIT_ASSERT_EQUALS( iRead->iCurrentFrame, 1 );
        
        // Second frame
        iBuf->SetStatus( EAvailable );
        iBuf->SetLastBuffer( ETrue );
        EUNIT_ASSERT_EQUALS( iRead->DoRead( iBuf ), ETrue );
        EUNIT_ASSERT( iBuf->Status() == EFull );
        EUNIT_ASSERT_EQUALS( iRead->iCurrentFrame, 2 );
        }
    
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iRead->DoRead( iBuf ) );
        }
        
    }


void UT_CAmrPayloadFormatRead::UT_CAmrPayloadFormatRead_SinkDataTypeCodeL( )
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


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CAmrPayloadFormatRead,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "CancelDlRequest - test ",
    "CAmrPayloadFormatRead",
    "CancelDlRequest",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadFormatRead_CancelDlRequestL, Teardown)

EUNIT_TEST(
    "SendDataToSinkL - test ",
    "CAmrPayloadFormatRead",
    "SendDataToSinkL",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadFormatRead_SendDataToSinkLL, Teardown)

EUNIT_TEST(
    "FillSinkBuffer - test ",
    "CAmrPayloadFormatRead",
    "FillSinkBuffer",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadFormatRead_FillSinkBufferL, Teardown)

EUNIT_TEST(
    "FillSourceBufferL - test ",
    "CAmrPayloadFormatRead",
    "FillSourceBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadFormatRead_FillSourceBufferLL, Teardown)

EUNIT_TEST(
    "Streams - test ",
    "CAmrPayloadFormatRead",
    "Streams",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadFormatRead_StreamsL, Teardown)

EUNIT_TEST(
    "FrameTimeInterval - test ",
    "CAmrPayloadFormatRead",
    "FrameTimeInterval",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadFormatRead_FrameTimeIntervalL, Teardown)

EUNIT_TEST(
    "Duration - test ",
    "CAmrPayloadFormatRead",
    "Duration",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadFormatRead_DurationL, Teardown)

EUNIT_TEST(
    "NumChannels - test ",
    "CAmrPayloadFormatRead",
    "NumChannels",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadFormatRead_NumChannelsL, Teardown)

EUNIT_TEST(
    "SampleRate - test ",
    "CAmrPayloadFormatRead",
    "SampleRate",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadFormatRead_SampleRateL, Teardown)

EUNIT_TEST(
    "SetSampleRate - test ",
    "CAmrPayloadFormatRead",
    "SetSampleRate",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadFormatRead_SetSampleRateL, Teardown)

EUNIT_TEST(
    "FillBufferL - test ",
    "CAmrPayloadFormatRead",
    "FillBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadFormatRead_FillBufferLL, Teardown)

EUNIT_TEST(
    "SourcePrimeL - test ",
    "CAmrPayloadFormatRead",
    "SourcePrimeL",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadFormatRead_SourcePrimeLL, Teardown)

EUNIT_TEST(
    "SourcePlayL - test ",
    "CAmrPayloadFormatRead",
    "SourcePlayL",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadFormatRead_SourcePlayLL, Teardown)

EUNIT_TEST(
    "SourcePauseL - test ",
    "CAmrPayloadFormatRead",
    "SourcePauseL",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadFormatRead_SourcePauseLL, Teardown)

EUNIT_TEST(
    "SourceStopL - test ",
    "CAmrPayloadFormatRead",
    "SourceStopL",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadFormatRead_SourceStopLL, Teardown)

EUNIT_TEST(
    "CreateSourceBufferL - test ",
    "CAmrPayloadFormatRead",
    "CreateSourceBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadFormatRead_CreateSourceBufferLL, Teardown)

EUNIT_TEST(
    "CreateSourceBufferL - test ",
    "CAmrPayloadFormatRead",
    "CreateSourceBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadFormatRead_CreateSourceBufferL_1L, Teardown)

EUNIT_TEST(
    "SourceDataTypeCode - test ",
    "CAmrPayloadFormatRead",
    "SourceDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadFormatRead_SourceDataTypeCodeL, Teardown)

EUNIT_TEST(
    "SetSourceDataTypeCode - test ",
    "CAmrPayloadFormatRead",
    "SetSourceDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadFormatRead_SetSourceDataTypeCodeL, Teardown)

EUNIT_TEST(
    "SourceThreadLogon - test ",
    "CAmrPayloadFormatRead",
    "SourceThreadLogon",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadFormatRead_SourceThreadLogonL, Teardown)

EUNIT_TEST(
    "SourceThreadLogoff - test ",
    "CAmrPayloadFormatRead",
    "SourceThreadLogoff",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadFormatRead_SourceThreadLogoffL, Teardown)

EUNIT_TEST(
    "NegotiateSourceL - test ",
    "CAmrPayloadFormatRead",
    "NegotiateSourceL",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadFormatRead_NegotiateSourceLL, Teardown)

EUNIT_TEST(
    "BufferFilledL - test ",
    "CAmrPayloadFormatRead",
    "BufferFilledL",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadFormatRead_BufferFilledLL, Teardown)

EUNIT_TEST(
    "ConfigurePayloadFormatL - test ",
    "CAmrPayloadFormatRead",
    "ConfigurePayloadFormatL",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadFormatRead_ConfigurePayloadFormatLL, Teardown)


EUNIT_TEST(
    "DoRead - test ",
    "CAmrPayloadFormatRead",
    "DoRead",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadFormatRead_DoReadL, Teardown)

EUNIT_TEST(
    "SinkDataTypeCode - test ",
    "CAmrPayloadFormatRead",
    "SinkDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadFormatRead_SinkDataTypeCodeL, Teardown)    

EUNIT_TEST(
    "ConvertModeToBitrate - test ",
    "CAmrPayloadFormatRead",
    "ConvertModeToBitrate",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadFormatRead_ConvertModeToBitrate, Teardown)    
  
EUNIT_TEST(
    "SendAmrEventToClient - test ",
    "CAmrPayloadFormatRead",
    "SendAmrEventToClient",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadFormatRead_SendAmrEventToClient, Teardown)    

// Wide band tests

EUNIT_TEST(
    "CancelDlRequest - Wb test ",
    "CAmrPayloadFormatRead",
    "CancelDlRequest",
    "FUNCTIONALITY",
    SetupWbL, UT_CAmrPayloadFormatRead_CancelDlRequestL, Teardown)

EUNIT_TEST(
    "SendDataToSinkL - Wb test ",
    "CAmrPayloadFormatRead",
    "SendDataToSinkL",
    "FUNCTIONALITY",
    SetupWbL, UT_CAmrPayloadFormatRead_SendDataToSinkLL, Teardown)

EUNIT_TEST(
    "FillSinkBuffer - Wb test ",
    "CAmrPayloadFormatRead",
    "FillSinkBuffer",
    "FUNCTIONALITY",
    SetupWbL, UT_CAmrPayloadFormatRead_FillSinkBufferL, Teardown)

EUNIT_TEST(
    "FillSourceBufferL - Wb test ",
    "CAmrPayloadFormatRead",
    "FillSourceBufferL",
    "FUNCTIONALITY",
    SetupWbL, UT_CAmrPayloadFormatRead_FillSourceBufferLL, Teardown)

EUNIT_TEST(
    "Streams - Wb test ",
    "CAmrPayloadFormatRead",
    "Streams",
    "FUNCTIONALITY",
    SetupWbL, UT_CAmrPayloadFormatRead_StreamsL, Teardown)

EUNIT_TEST(
    "FrameTimeInterval - Wb test ",
    "CAmrPayloadFormatRead",
    "FrameTimeInterval",
    "FUNCTIONALITY",
    SetupWbL, UT_CAmrPayloadFormatRead_FrameTimeIntervalL, Teardown)

EUNIT_TEST(
    "Duration - Wb test ",
    "CAmrPayloadFormatRead",
    "Duration",
    "FUNCTIONALITY",
    SetupWbL, UT_CAmrPayloadFormatRead_DurationL, Teardown)

EUNIT_TEST(
    "NumChannels - Wb test ",
    "CAmrPayloadFormatRead",
    "NumChannels",
    "FUNCTIONALITY",
    SetupWbL, UT_CAmrPayloadFormatRead_NumChannelsL, Teardown)

EUNIT_TEST(
    "SampleRate - Wb test ",
    "CAmrPayloadFormatRead",
    "SampleRate",
    "FUNCTIONALITY",
    SetupWbL, UT_CAmrPayloadFormatRead_SampleRateL, Teardown)

EUNIT_TEST(
    "SetSampleRate - Wb test ",
    "CAmrPayloadFormatRead",
    "SetSampleRate",
    "FUNCTIONALITY",
    SetupWbL, UT_CAmrPayloadFormatRead_SetSampleRateWbL, Teardown)

EUNIT_TEST(
    "FillBufferL - Wb test ",
    "CAmrPayloadFormatRead",
    "FillBufferL",
    "FUNCTIONALITY",
    SetupWbL, UT_CAmrPayloadFormatRead_FillBufferLL, Teardown)

EUNIT_TEST(
    "SourcePrimeL - Wb test ",
    "CAmrPayloadFormatRead",
    "SourcePrimeL",
    "FUNCTIONALITY",
    SetupWbL, UT_CAmrPayloadFormatRead_SourcePrimeLL, Teardown)

EUNIT_TEST(
    "SourcePlayL - Wb test ",
    "CAmrPayloadFormatRead",
    "SourcePlayL",
    "FUNCTIONALITY",
    SetupWbL, UT_CAmrPayloadFormatRead_SourcePlayLL, Teardown)

EUNIT_TEST(
    "SourcePauseL - Wb test ",
    "CAmrPayloadFormatRead",
    "SourcePauseL",
    "FUNCTIONALITY",
    SetupWbL, UT_CAmrPayloadFormatRead_SourcePauseLL, Teardown)

EUNIT_TEST(
    "SourceStopL - Wb test ",
    "CAmrPayloadFormatRead",
    "SourceStopL",
    "FUNCTIONALITY",
    SetupWbL, UT_CAmrPayloadFormatRead_SourceStopLL, Teardown)

EUNIT_TEST(
    "CreateSourceBufferL - Wb test ",
    "CAmrPayloadFormatRead",
    "CreateSourceBufferL",
    "FUNCTIONALITY",
    SetupWbL, UT_CAmrPayloadFormatRead_CreateSourceBufferLL, Teardown)

EUNIT_TEST(
    "CreateSourceBufferL - Wb test ",
    "CAmrPayloadFormatRead",
    "CreateSourceBufferL",
    "FUNCTIONALITY",
    SetupWbL, UT_CAmrPayloadFormatRead_CreateSourceBufferL_1L, Teardown)

EUNIT_TEST(
    "SourceDataTypeCode - Wb test ",
    "CAmrPayloadFormatRead",
    "SourceDataTypeCode",
    "FUNCTIONALITY",
    SetupWbL, UT_CAmrPayloadFormatRead_SourceDataTypeCodeL, Teardown)

EUNIT_TEST(
    "SetSourceDataTypeCode - Wb test ",
    "CAmrPayloadFormatRead",
    "SetSourceDataTypeCode",
    "FUNCTIONALITY",
    SetupWbL, UT_CAmrPayloadFormatRead_SetSourceDataTypeCodeL, Teardown)

EUNIT_TEST(
    "SourceThreadLogon - Wb test ",
    "CAmrPayloadFormatRead",
    "SourceThreadLogon",
    "FUNCTIONALITY",
    SetupWbL, UT_CAmrPayloadFormatRead_SourceThreadLogonL, Teardown)

EUNIT_TEST(
    "SourceThreadLogoff - Wb test ",
    "CAmrPayloadFormatRead",
    "SourceThreadLogoff",
    "FUNCTIONALITY",
    SetupWbL, UT_CAmrPayloadFormatRead_SourceThreadLogoffL, Teardown)

EUNIT_TEST(
    "NegotiateSourceL - Wb test ",
    "CAmrPayloadFormatRead",
    "NegotiateSourceL",
    "FUNCTIONALITY",
    SetupWbL, UT_CAmrPayloadFormatRead_NegotiateSourceLL, Teardown)

EUNIT_TEST(
    "BufferFilledL - Wb test ",
    "CAmrPayloadFormatRead",
    "BufferFilledL",
    "FUNCTIONALITY",
    SetupWbL, UT_CAmrPayloadFormatRead_BufferFilledLL, Teardown)

EUNIT_TEST(
    "ConfigurePayloadFormatL - Wb test ",
    "CAmrPayloadFormatRead",
    "ConfigurePayloadFormatL",
    "FUNCTIONALITY",
    SetupWbL, UT_CAmrPayloadFormatRead_ConfigurePayloadFormatLWbL, Teardown)


EUNIT_TEST(
    "DoRead - Wb test ",
    "CAmrPayloadFormatRead",
    "DoRead",
    "FUNCTIONALITY",
    SetupWbL, UT_CAmrPayloadFormatRead_DoReadL, Teardown)

EUNIT_TEST(
    "SinkDataTypeCode - Wb test ",
    "CAmrPayloadFormatRead",
    "SinkDataTypeCode",
    "FUNCTIONALITY",
    SetupWbL, UT_CAmrPayloadFormatRead_SinkDataTypeCodeL, Teardown)    
  
EUNIT_TEST(
    "ConvertModeToBitrate - Wb test ",
    "CAmrPayloadFormatRead",
    "ConvertModeToBitrate",
    "FUNCTIONALITY",
    SetupWbL, UT_CAmrPayloadFormatRead_ConvertModeToBitrateWb, Teardown)    
  
EUNIT_TEST(
    "SendAmrEventToClient - Wb test ",
    "CAmrPayloadFormatRead",
    "SendAmrEventToClient",
    "FUNCTIONALITY",
    SetupWbL, UT_CAmrPayloadFormatRead_SendAmrEventToClient, Teardown)    

// ALLOC TESTS
#ifdef EUNIT_ALLOC_TEST_ON

EUNIT_ALLOC_TEST(
    "CancelDlRequest - alloc test ",
    "CAmrPayloadFormatRead",
    "CancelDlRequest",
    "ERRORHANDLING",
    SetupAL, UT_CAmrPayloadFormatRead_CancelDlRequestL, Teardown)

EUNIT_ALLOC_TEST(
    "SendDataToSinkL - alloc test ",
    "CAmrPayloadFormatRead",
    "SendDataToSinkL",
    "ERRORHANDLING",
    SetupAL, UT_CAmrPayloadFormatRead_SendDataToSinkLL, Teardown)

EUNIT_ALLOC_TEST(
    "FillSinkBuffer - alloc test ",
    "CAmrPayloadFormatRead",
    "FillSinkBuffer",
    "ERRORHANDLING",
    SetupAL, UT_CAmrPayloadFormatRead_FillSinkBufferL, Teardown)

EUNIT_ALLOC_TEST(
    "FillSourceBufferL - alloc test ",
    "CAmrPayloadFormatRead",
    "FillSourceBufferL",
    "ERRORHANDLING",
    SetupAL, UT_CAmrPayloadFormatRead_FillSourceBufferLL, Teardown)

EUNIT_ALLOC_TEST(
    "Streams - alloc test ",
    "CAmrPayloadFormatRead",
    "Streams",
    "ERRORHANDLING",
    SetupAL, UT_CAmrPayloadFormatRead_StreamsL, Teardown)

EUNIT_ALLOC_TEST(
    "FrameTimeInterval - alloc test ",
    "CAmrPayloadFormatRead",
    "FrameTimeInterval",
    "ERRORHANDLING",
    SetupAL, UT_CAmrPayloadFormatRead_FrameTimeIntervalL, Teardown)

EUNIT_ALLOC_TEST(
    "Duration - alloc test ",
    "CAmrPayloadFormatRead",
    "Duration",
    "ERRORHANDLING",
    SetupAL, UT_CAmrPayloadFormatRead_DurationL, Teardown)

EUNIT_ALLOC_TEST(
    "NumChannels - alloc test ",
    "CAmrPayloadFormatRead",
    "NumChannels",
    "ERRORHANDLING",
    SetupAL, UT_CAmrPayloadFormatRead_NumChannelsL, Teardown)

EUNIT_ALLOC_TEST(
    "SampleRate - alloc test ",
    "CAmrPayloadFormatRead",
    "SampleRate",
    "ERRORHANDLING",
    SetupAL, UT_CAmrPayloadFormatRead_SampleRateL, Teardown)

EUNIT_ALLOC_TEST(
    "SetSampleRate - alloc test ",
    "CAmrPayloadFormatRead",
    "SetSampleRate",
    "ERRORHANDLING",
    SetupAL, UT_CAmrPayloadFormatRead_SetSampleRateL, Teardown)

EUNIT_ALLOC_TEST(
    "FillBufferL - alloc test ",
    "CAmrPayloadFormatRead",
    "FillBufferL",
    "ERRORHANDLING",
    SetupAL, UT_CAmrPayloadFormatRead_FillBufferLL, Teardown)

EUNIT_ALLOC_TEST(
    "SourcePrimeL - alloc test ",
    "CAmrPayloadFormatRead",
    "SourcePrimeL",
    "ERRORHANDLING",
    SetupAL, UT_CAmrPayloadFormatRead_SourcePrimeLL, Teardown)

EUNIT_ALLOC_TEST(
    "SourcePlayL - alloc test ",
    "CAmrPayloadFormatRead",
    "SourcePlayL",
    "ERRORHANDLING",
    SetupAL, UT_CAmrPayloadFormatRead_SourcePlayLL, Teardown)

EUNIT_ALLOC_TEST(
    "SourcePauseL - alloc test ",
    "CAmrPayloadFormatRead",
    "SourcePauseL",
    "ERRORHANDLING",
    SetupAL, UT_CAmrPayloadFormatRead_SourcePauseLL, Teardown)

EUNIT_ALLOC_TEST(
    "SourceStopL - alloc test ",
    "CAmrPayloadFormatRead",
    "SourceStopL",
    "ERRORHANDLING",
    SetupAL, UT_CAmrPayloadFormatRead_SourceStopLL, Teardown)

EUNIT_ALLOC_TEST(
    "CreateSourceBufferL - alloc test ",
    "CAmrPayloadFormatRead",
    "CreateSourceBufferL",
    "ERRORHANDLING",
    SetupAL, UT_CAmrPayloadFormatRead_CreateSourceBufferLL, Teardown)

EUNIT_ALLOC_TEST(
    "CreateSourceBufferL - alloc test ",
    "CAmrPayloadFormatRead",
    "CreateSourceBufferL",
    "ERRORHANDLING",
    SetupAL, UT_CAmrPayloadFormatRead_CreateSourceBufferL_1L, Teardown)

EUNIT_ALLOC_TEST(
    "SourceDataTypeCode - alloc test ",
    "CAmrPayloadFormatRead",
    "SourceDataTypeCode",
    "ERRORHANDLING",
    SetupAL, UT_CAmrPayloadFormatRead_SourceDataTypeCodeL, Teardown)

EUNIT_ALLOC_TEST(
    "SetSourceDataTypeCode - alloc test ",
    "CAmrPayloadFormatRead",
    "SetSourceDataTypeCode",
    "ERRORHANDLING",
    SetupAL, UT_CAmrPayloadFormatRead_SetSourceDataTypeCodeL, Teardown)

EUNIT_ALLOC_TEST(
    "SourceThreadLogon - alloc test ",
    "CAmrPayloadFormatRead",
    "SourceThreadLogon",
    "ERRORHANDLING",
    SetupAL, UT_CAmrPayloadFormatRead_SourceThreadLogonL, Teardown)

EUNIT_ALLOC_TEST(
    "SourceThreadLogoff - alloc test ",
    "CAmrPayloadFormatRead",
    "SourceThreadLogoff",
    "ERRORHANDLING",
    SetupAL, UT_CAmrPayloadFormatRead_SourceThreadLogoffL, Teardown)

EUNIT_ALLOC_TEST(
    "NegotiateSourceL - alloc test ",
    "CAmrPayloadFormatRead",
    "NegotiateSourceL",
    "ERRORHANDLING",
    SetupAL, UT_CAmrPayloadFormatRead_NegotiateSourceLL, Teardown)

EUNIT_ALLOC_TEST(
    "BufferFilledL - alloc test ",
    "CAmrPayloadFormatRead",
    "BufferFilledL",
    "ERRORHANDLING",
    SetupAL, UT_CAmrPayloadFormatRead_BufferFilledLL, Teardown)
    
EUNIT_ALLOC_TEST(
    "ConfigurePayloadFormatL - alloc test ",
    "CAmrPayloadFormatRead",
    "ConfigurePayloadFormatL",
    "ERRORHANDLING",
    SetupAL, UT_CAmrPayloadFormatRead_ConfigurePayloadFormatLL, Teardown)
    
EUNIT_ALLOC_TEST(
    "DoRead - alloc test ",
    "CAmrPayloadFormatRead",
    "DoRead",
    "ERRORHANDLING",
    SetupAL, UT_CAmrPayloadFormatRead_DoReadL, Teardown)

EUNIT_ALLOC_TEST(
    "SinkDataTypeCode - alloc test ",
    "CAmrPayloadFormatRead",
    "SinkDataTypeCode",
    "ERRORHANDLING",
    SetupAL, UT_CAmrPayloadFormatRead_SinkDataTypeCodeL, Teardown)    

EUNIT_TEST(
    "ConvertModeToBitrate - alloc test ",
    "CAmrPayloadFormatRead",
    "ConvertModeToBitrate",
    "FUNCTIONALITY",
    SetupAL, UT_CAmrPayloadFormatRead_ConvertModeToBitrate, Teardown)    
  
EUNIT_TEST(
    "SendAmrEventToClient - alloc test ",
    "CAmrPayloadFormatRead",
    "SendAmrEventToClient",
    "FUNCTIONALITY",
    SetupAL, UT_CAmrPayloadFormatRead_SendAmrEventToClient, Teardown)    

// Wide band tests

EUNIT_ALLOC_TEST(
    "CancelDlRequest - Wb alloc test ",
    "CAmrPayloadFormatRead",
    "CancelDlRequest",
    "ERRORHANDLING",
    SetupWbAL, UT_CAmrPayloadFormatRead_CancelDlRequestL, Teardown)

EUNIT_ALLOC_TEST(
    "SendDataToSinkL - Wb alloc test ",
    "CAmrPayloadFormatRead",
    "SendDataToSinkL",
    "ERRORHANDLING",
    SetupWbAL, UT_CAmrPayloadFormatRead_SendDataToSinkLL, Teardown)

EUNIT_ALLOC_TEST(
    "FillSinkBuffer - Wb alloc test ",
    "CAmrPayloadFormatRead",
    "FillSinkBuffer",
    "ERRORHANDLING",
    SetupWbAL, UT_CAmrPayloadFormatRead_FillSinkBufferL, Teardown)

EUNIT_ALLOC_TEST(
    "FillSourceBufferL - Wb alloc test ",
    "CAmrPayloadFormatRead",
    "FillSourceBufferL",
    "ERRORHANDLING",
    SetupWbAL, UT_CAmrPayloadFormatRead_FillSourceBufferLL, Teardown)

EUNIT_ALLOC_TEST(
    "Streams - Wb alloc test ",
    "CAmrPayloadFormatRead",
    "Streams",
    "ERRORHANDLING",
    SetupWbAL, UT_CAmrPayloadFormatRead_StreamsL, Teardown)

EUNIT_ALLOC_TEST(
    "FrameTimeInterval - Wb alloc test ",
    "CAmrPayloadFormatRead",
    "FrameTimeInterval",
    "ERRORHANDLING",
    SetupWbAL, UT_CAmrPayloadFormatRead_FrameTimeIntervalL, Teardown)

EUNIT_ALLOC_TEST(
    "Duration - Wb alloc test ",
    "CAmrPayloadFormatRead",
    "Duration",
    "ERRORHANDLING",
    SetupWbAL, UT_CAmrPayloadFormatRead_DurationL, Teardown)

EUNIT_ALLOC_TEST(
    "NumChannels - Wb alloc test ",
    "CAmrPayloadFormatRead",
    "NumChannels",
    "ERRORHANDLING",
    SetupWbAL, UT_CAmrPayloadFormatRead_NumChannelsL, Teardown)

EUNIT_ALLOC_TEST(
    "SampleRate - Wb alloc test ",
    "CAmrPayloadFormatRead",
    "SampleRate",
    "ERRORHANDLING",
    SetupWbAL, UT_CAmrPayloadFormatRead_SampleRateL, Teardown)

EUNIT_ALLOC_TEST(
    "SetSampleRate - Wb alloc test ",
    "CAmrPayloadFormatRead",
    "SetSampleRate",
    "ERRORHANDLING",
    SetupWbAL, UT_CAmrPayloadFormatRead_SetSampleRateWbL, Teardown)

EUNIT_ALLOC_TEST(
    "FillBufferL - Wb alloc test ",
    "CAmrPayloadFormatRead",
    "FillBufferL",
    "ERRORHANDLING",
    SetupWbAL, UT_CAmrPayloadFormatRead_FillBufferLL, Teardown)

EUNIT_ALLOC_TEST(
    "SourcePrimeL - Wb alloc test ",
    "CAmrPayloadFormatRead",
    "SourcePrimeL",
    "ERRORHANDLING",
    SetupWbAL, UT_CAmrPayloadFormatRead_SourcePrimeLL, Teardown)

EUNIT_ALLOC_TEST(
    "SourcePlayL - Wb alloc test ",
    "CAmrPayloadFormatRead",
    "SourcePlayL",
    "ERRORHANDLING",
    SetupWbAL, UT_CAmrPayloadFormatRead_SourcePlayLL, Teardown)

EUNIT_ALLOC_TEST(
    "SourcePauseL - Wb alloc test ",
    "CAmrPayloadFormatRead",
    "SourcePauseL",
    "ERRORHANDLING",
    SetupWbAL, UT_CAmrPayloadFormatRead_SourcePauseLL, Teardown)

EUNIT_ALLOC_TEST(
    "SourceStopL - Wb alloc test ",
    "CAmrPayloadFormatRead",
    "SourceStopL",
    "ERRORHANDLING",
    SetupWbAL, UT_CAmrPayloadFormatRead_SourceStopLL, Teardown)

EUNIT_ALLOC_TEST(
    "CreateSourceBufferL - Wb alloc test ",
    "CAmrPayloadFormatRead",
    "CreateSourceBufferL",
    "ERRORHANDLING",
    SetupWbAL, UT_CAmrPayloadFormatRead_CreateSourceBufferLL, Teardown)

EUNIT_ALLOC_TEST(
    "CreateSourceBufferL - Wb alloc test ",
    "CAmrPayloadFormatRead",
    "CreateSourceBufferL",
    "ERRORHANDLING",
    SetupWbAL, UT_CAmrPayloadFormatRead_CreateSourceBufferL_1L, Teardown)

EUNIT_ALLOC_TEST(
    "SourceDataTypeCode - Wb alloc test ",
    "CAmrPayloadFormatRead",
    "SourceDataTypeCode",
    "ERRORHANDLING",
    SetupWbAL, UT_CAmrPayloadFormatRead_SourceDataTypeCodeL, Teardown)

EUNIT_ALLOC_TEST(
    "SetSourceDataTypeCode - Wb alloc test ",
    "CAmrPayloadFormatRead",
    "SetSourceDataTypeCode",
    "ERRORHANDLING",
    SetupWbAL, UT_CAmrPayloadFormatRead_SetSourceDataTypeCodeL, Teardown)

EUNIT_ALLOC_TEST(
    "SourceThreadLogon - Wb alloc test ",
    "CAmrPayloadFormatRead",
    "SourceThreadLogon",
    "ERRORHANDLING",
    SetupWbAL, UT_CAmrPayloadFormatRead_SourceThreadLogonL, Teardown)

EUNIT_ALLOC_TEST(
    "SourceThreadLogoff - Wb alloc test ",
    "CAmrPayloadFormatRead",
    "SourceThreadLogoff",
    "ERRORHANDLING",
    SetupWbAL, UT_CAmrPayloadFormatRead_SourceThreadLogoffL, Teardown)

EUNIT_ALLOC_TEST(
    "NegotiateSourceL - Wb alloc test ",
    "CAmrPayloadFormatRead",
    "NegotiateSourceL",
    "ERRORHANDLING",
    SetupWbAL, UT_CAmrPayloadFormatRead_NegotiateSourceLL, Teardown)

EUNIT_ALLOC_TEST(
    "BufferFilledL - Wb alloc test ",
    "CAmrPayloadFormatRead",
    "BufferFilledL",
    "ERRORHANDLING",
    SetupWbAL, UT_CAmrPayloadFormatRead_BufferFilledLL, Teardown)
    
EUNIT_ALLOC_TEST(
    "ConfigurePayloadFormatL - Wb alloc test ",
    "CAmrPayloadFormatRead",
    "ConfigurePayloadFormatL",
    "ERRORHANDLING",
    SetupWbAL, UT_CAmrPayloadFormatRead_ConfigurePayloadFormatLWbL, Teardown)
    
EUNIT_ALLOC_TEST(
    "DoRead - Wb alloc test ",
    "CAmrPayloadFormatRead",
    "DoRead",
    "ERRORHANDLING",
    SetupWbAL, UT_CAmrPayloadFormatRead_DoReadL, Teardown)

EUNIT_ALLOC_TEST(
    "SinkDataTypeCode - Wb alloc test ",
    "CAmrPayloadFormatRead",
    "SinkDataTypeCode",
    "ERRORHANDLING",
    SetupWbAL, UT_CAmrPayloadFormatRead_SinkDataTypeCodeL, Teardown)    

EUNIT_TEST(
    "ConvertModeToBitrate - Wb alloc test ",
    "CAmrPayloadFormatRead",
    "ConvertModeToBitrate",
    "FUNCTIONALITY",
    SetupWbAL, UT_CAmrPayloadFormatRead_ConvertModeToBitrateWb, Teardown)    
  
EUNIT_TEST(
    "SendAmrEventToClient - Wb alloc test ",
    "CAmrPayloadFormatRead",
    "SendAmrEventToClient",
    "FUNCTIONALITY",
    SetupWbAL, UT_CAmrPayloadFormatRead_SendAmrEventToClient, Teardown)    

#endif  

EUNIT_END_TEST_TABLE

//  END OF FILE
