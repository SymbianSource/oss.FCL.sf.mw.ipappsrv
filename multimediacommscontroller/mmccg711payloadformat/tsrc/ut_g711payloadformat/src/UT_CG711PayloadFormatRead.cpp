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
#include "UT_CG711PayloadFormatRead.h"
#include "MccRtpDataSink.h"
#include "mccuids.hrh"
#include "Mccdldatapath.h"
#include "MccRtpDataSource.h"
#include "mccresourcepool.h"
#include "mccredpayloadread.h"

//  EXTERNAL INCLUDES
#include <e32base.h>
#include <digia/eunit/eunitmacros.h>

#ifdef EUNIT_ALLOC_TEST_ON
    #include <digia/eunit/CEunitAllocTestCaseDecorator.h>   //for using EUNIT_ALLOC_TEST
#endif

//  INTERNAL INCLUDES
#include "G711PayloadFormatRead.h"

const TUid KRtpDataSource            =  { KImplUidRtpDataSource   }; 
const TUid KRtpDataSink              =  { KImplUidRtpDataSink     };


// CONSTRUCTION
UT_CG711PayloadFormatRead* UT_CG711PayloadFormatRead::NewL()
    {
    UT_CG711PayloadFormatRead* self = UT_CG711PayloadFormatRead::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CG711PayloadFormatRead* UT_CG711PayloadFormatRead::NewLC()
    {
    UT_CG711PayloadFormatRead* self = new( ELeave ) UT_CG711PayloadFormatRead();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CG711PayloadFormatRead::~UT_CG711PayloadFormatRead()
    {
    }

// Default constructor
UT_CG711PayloadFormatRead::UT_CG711PayloadFormatRead()
    {
    }

// Second phase construct
void UT_CG711PayloadFormatRead::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    iFourCC.Set( TFourCC( 'G','7','1','1' ) );
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS
void UT_CG711PayloadFormatRead::SetupL(  )
    {
    iAlloc = EFalse; 

    iRtpDataSourceStub = static_cast<CMccRtpDataSource*>(MDataSource::NewSourceL( KRtpDataSource, KNullDesC8 ));
    iMediaId = KUidMediaTypeAudio;
    MAsyncEventHandler* eventHandler = NULL;
    CMccResourcePool* mccresource = NULL;
    iDLDataPathStub = CMccDlDataPath::NewL( eventHandler, mccresource, iMediaId ); 

    iRead = CG711PayloadFormatRead::NewL( iRtpDataSourceStub );
    iConsumer = MDataSink::NewSinkL( KRtpDataSink, KNullDesC8 );
  
    iBuffi = CMMFDescriptorBuffer::NewL( 10 );
    iBuffi->Data().Append( _L("1234512345") ); 
    
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
    
    RDebug::Print( _L("- Algo used       = %d"), iCodec.iAlgoUsed ); // 0 jos redundanssi ei päällä, 2 jos haluat asettaa sen päälle
    RDebug::Print( _L("- Bitrate         = %d"), iCodec.iBitrate ); // 64000
    RDebug::Print( _L("- CNFrameSize     = %d"), iCodec.iCNFrameSize ); // 0
    RDebug::Print( _L("- CodecMode       = %d"), iCodec.iCodecMode ); // 3
    RDebug::Print( _L("- EnableDTX       = %d"), iCodec.iEnableDTX ); // 1
    RDebug::Print( _L("- FourCC          = %x"), iCodec.iFourCC ); // 31313747
    RDebug::Print( _L("- FrameSize       = %d"), iCodec.iFrameSize ); // 0
    RDebug::Print( _L("- HWFrameTime     = %d"), iCodec.iHwFrameTime ); // 20
    RDebug::Print( _L("- MaxPTime        = %d"), iCodec.iMaxPtime ); // 200
    RDebug::Print( _L("- PayloadType     = %d"), iCodec.iPayloadType ); // 8
    RDebug::Print( _L("- PTime           = %d"), iCodec.iPtime ); // 20
    RDebug::Print( _L("- RedundancyCount = %d"), iCodec.iRedundancyCount ); // 0 jos redundanssi ei päällä, 1 jos redundanssi päällä
    RDebug::Print( _L("- RedPayload      = %d"), iCodec.iRedundantPayload ); // 0 jos redundanssi ei päällä, 97-127 jos redundanssi päällä
  
    iRead->SetSourceDataTypeCode( iFourCC, KUidMediaTypeAudio );
    iRead->SourceThreadLogon( *this );
    
    iRead->ConfigurePayloadFormatL( buffer );

    iRead->NegotiateSourceL( *iDLDataPathStub );
    
    iRead->SourcePrimeL();
    iRead->SourcePlayL();
    } 
    
void UT_CG711PayloadFormatRead::SetupAL(  )
    {
    iAlloc = ETrue; 

    iRtpDataSourceStub = static_cast<CMccRtpDataSource*>(MDataSource::NewSourceL( KRtpDataSource, KNullDesC8 ));
    iMediaId = KUidMediaTypeAudio;
    MAsyncEventHandler* eventHandler = NULL;
    CMccResourcePool* mccresource = NULL;
    iDLDataPathStub = CMccDlDataPath::NewL( eventHandler, mccresource, iMediaId ); 

    iRead = CG711PayloadFormatRead::NewL( iRtpDataSourceStub );
    iConsumer = MDataSink::NewSinkL( KRtpDataSink, KNullDesC8 );
  
    iBuffi = CMMFDescriptorBuffer::NewL( 10 );
    iBuffi->Data().Append( _L("1234512345") ); 
    
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
    
    RDebug::Print( _L("- Algo used       = %d"), iCodec.iAlgoUsed ); // 0 jos redundanssi ei päällä, 2 jos haluat asettaa sen päälle
    RDebug::Print( _L("- Bitrate         = %d"), iCodec.iBitrate ); // 64000
    RDebug::Print( _L("- CNFrameSize     = %d"), iCodec.iCNFrameSize ); // 0
    RDebug::Print( _L("- CodecMode       = %d"), iCodec.iCodecMode ); // 3
    RDebug::Print( _L("- EnableDTX       = %d"), iCodec.iEnableDTX ); // 1
    RDebug::Print( _L("- FourCC          = %x"), iCodec.iFourCC ); // 31313747
    RDebug::Print( _L("- FrameSize       = %d"), iCodec.iFrameSize ); // 0
    RDebug::Print( _L("- HWFrameTime     = %d"), iCodec.iHwFrameTime ); // 20
    RDebug::Print( _L("- MaxPTime        = %d"), iCodec.iMaxPtime ); // 200
    RDebug::Print( _L("- PayloadType     = %d"), iCodec.iPayloadType ); // 8
    RDebug::Print( _L("- PTime           = %d"), iCodec.iPtime ); // 20
    RDebug::Print( _L("- RedundancyCount = %d"), iCodec.iRedundancyCount ); // 0 jos redundanssi ei päällä, 1 jos redundanssi päällä
    RDebug::Print( _L("- RedPayload      = %d"), iCodec.iRedundantPayload ); // 0 jos redundanssi ei päällä, 97-127 jos redundanssi päällä
  
    iRead->SetSourceDataTypeCode( iFourCC, KUidMediaTypeAudio );
    iRead->SourceThreadLogon( *this );
    
    iRead->ConfigurePayloadFormatL( buffer );

    iRead->NegotiateSourceL( *iDLDataPathStub );
    
    iRead->SourcePrimeL();
    iRead->SourcePlayL();
    } 

void UT_CG711PayloadFormatRead::Teardown(  )
    {

    delete iBuffi;
    delete iDLDataPathStub;   
    delete iRtpDataSourceStub;
    delete iRead;
    delete iConsumer;
        
    REComSession::FinalClose();
    }


void UT_CG711PayloadFormatRead::UT_CG711PayloadFormatRead_NewLL(  )
    {
    CG711PayloadFormatRead* read = CG711PayloadFormatRead::NewL( iRtpDataSourceStub );
    delete read;
    EUNIT_ASSERT_SPECIFIC_LEAVE( CG711PayloadFormatRead* read2 = CG711PayloadFormatRead::NewL( NULL ), KErrArgument );
    }


void UT_CG711PayloadFormatRead::UT_CG711PayloadFormatRead_SendDataToSinkLL(  )
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

void UT_CG711PayloadFormatRead::UT_CG711PayloadFormatRead_FillSinkBufferL(  )
    {   
    if ( !iAlloc )
        {
        EUNIT_ASSERT_NO_LEAVE( iRead->FillSinkBufferL() );
        iRead->iBufferToReadExists = 1;
        EUNIT_ASSERT_NO_LEAVE( iRead->FillSinkBufferL() );
        
        iRead->iCnFrame = 1;
        EUNIT_ASSERT_NO_LEAVE( iRead->FillSinkBufferL() );
        
        }        
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iRead->FillSinkBufferL() );
        
        }       
    }

void UT_CG711PayloadFormatRead::UT_CG711PayloadFormatRead_FillSourceBufferLL(  )
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

void UT_CG711PayloadFormatRead::UT_CG711PayloadFormatRead_StreamsL(  )
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

void UT_CG711PayloadFormatRead::UT_CG711PayloadFormatRead_FrameTimeIntervalL(  )
    {
    if ( !iAlloc )
        {
        EUNIT_ASSERT_EQUALS( iRead->FrameTimeInterval( KUidMediaTypeAudio ).Int64(), 20 );
        EUNIT_ASSERT_EQUALS( iRead->FrameTimeInterval( KUidMediaTypeVideo ).Int64(), 0 );
        
        }
        
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iRead->FrameTimeInterval( KUidMediaTypeAudio ) );
        EUNIT_ASSERT_NO_LEAVE( iRead->FrameTimeInterval( KUidMediaTypeVideo ) );
        
        }
    }

void UT_CG711PayloadFormatRead::UT_CG711PayloadFormatRead_DurationL(  )
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

void UT_CG711PayloadFormatRead::UT_CG711PayloadFormatRead_NumChannelsL(  )
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

void UT_CG711PayloadFormatRead::UT_CG711PayloadFormatRead_SampleRateL(  )
    {    
    if ( !iAlloc )
        {
        EUNIT_ASSERT_EQUALS( iRead->SampleRate(), 8000 );        
        }      
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iRead->SampleRate() );     
        }
    }

void UT_CG711PayloadFormatRead::UT_CG711PayloadFormatRead_SetSampleRateL(  )
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

void UT_CG711PayloadFormatRead::UT_CG711PayloadFormatRead_SourceDataTypeCodeL(  )
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

void UT_CG711PayloadFormatRead::UT_CG711PayloadFormatRead_SetSourceDataTypeCodeL(  )
    {
    if ( !iAlloc )
        {
        EUNIT_ASSERT_EQUALS( iRead->SetSourceDataTypeCode( iFourCC, KUidMediaTypeAudio ), KErrNone );
        EUNIT_ASSERT_EQUALS( iRead->SetSourceDataTypeCode( iFourCC, KUidMediaTypeVideo ), KErrNotSupported );
   
//        iRead->SetPayloadType( 1 );
//        EUNIT_ASSERT_NO_LEAVE( iRead->SetRedundancyL( ETrue, 1, 1 ) );
        EUNIT_ASSERT_EQUALS( iRead->SetSourceDataTypeCode( iFourCC, KUidMediaTypeAudio ), KErrNone );    
        }
    
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iRead->SetSourceDataTypeCode( iFourCC, KUidMediaTypeAudio ) );
        EUNIT_ASSERT_NO_LEAVE( iRead->SetSourceDataTypeCode( iFourCC, KUidMediaTypeVideo ) );
        
        }
    }

void UT_CG711PayloadFormatRead::UT_CG711PayloadFormatRead_CreateSourceBufferLL(  )
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

void UT_CG711PayloadFormatRead::UT_CG711PayloadFormatRead_CreateSourceBufferL_1L(  )
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

void UT_CG711PayloadFormatRead::UT_CG711PayloadFormatRead_FillBufferLL(  )
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

void UT_CG711PayloadFormatRead::UT_CG711PayloadFormatRead_SourceThreadLogonL(  )
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

void UT_CG711PayloadFormatRead::UT_CG711PayloadFormatRead_SourceThreadLogoffL(  )
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

void UT_CG711PayloadFormatRead::UT_CG711PayloadFormatRead_NegotiateSourceLL(  )
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

void UT_CG711PayloadFormatRead::UT_CG711PayloadFormatRead_SourcePrimeLL(  )
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

void UT_CG711PayloadFormatRead::UT_CG711PayloadFormatRead_SourcePlayLL(  )
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

void UT_CG711PayloadFormatRead::UT_CG711PayloadFormatRead_SourcePauseLL(  )
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

void UT_CG711PayloadFormatRead::UT_CG711PayloadFormatRead_SourceStopLL(  )
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

void UT_CG711PayloadFormatRead::UT_CG711PayloadFormatRead_BufferFilledLL(  )
    {
    TRtpRecvHeader test;
    test.iPadding = 1;
    test.iMarker = 1;
        
    if ( !iAlloc )
        {
        CMMFDataBuffer* buffi = CMMFDataBuffer::NewL( 170 );      
        CMMFBuffer* buf = buffi; 
        CleanupStack::PushL( buf );
        iRtpDataSourceStub->FillBufferL( buf, iConsumer, KUidMediaTypeAudio );
        EUNIT_ASSERT_NO_LEAVE( iRead->DataBufferFilledL( iRead->iSourceBuffer, test ) );
        CleanupStack::PopAndDestroy( buf );     
        
        buffi = CMMFDescriptorBuffer::NewL( 170 );
        buffi->Data().Append( _L("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789") );
        iRtpDataSourceStub->FillBufferL( buffi, iConsumer, KUidMediaTypeAudio );
        EUNIT_ASSERT_NO_LEAVE( iRead->DataBufferFilledL( iRead->iSourceBuffer, test ) );
        delete buffi;
        buffi = NULL;  
        }
    
    else
        {
        CMMFDataBuffer* buffi = CMMFDataBuffer::NewL( 170 );      
        CMMFBuffer* buf = buffi; 
        CleanupStack::PushL( buf );
        iRtpDataSourceStub->FillBufferL( buf, iConsumer, KUidMediaTypeAudio );
        EUNIT_ASSERT_NO_LEAVE( iRead->DataBufferFilledL( iRead->iSourceBuffer, test ) );
        CleanupStack::PopAndDestroy( buf );          
        }
    
    }
 
void UT_CG711PayloadFormatRead::UT_CG711PayloadFormatRead_SinkDataTypeCodeL( )
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
    
void UT_CG711PayloadFormatRead::UT_CG711PayloadFormatRead_GetNextFrameL( ) 
    {
    _LIT8(KLit, "Nappaako");
    TBufC8<160> jee( KLit );
    //TDes8& getBuffer;
    TBuf8<330> getBuffer;
    TPtr8 testBuf = jee.Des();
    
    if ( !iAlloc )
        {
        EUNIT_ASSERT_NO_LEAVE( iRead->GetNextFrame( getBuffer ) );
        iRead->iFrameIndex = 0;
        iRead->iFrameArray.Append( testBuf );
        EUNIT_ASSERT_NO_LEAVE( iRead->GetNextFrame( getBuffer ) );
        iRead->iFrameArray.Append( testBuf );
        iRead->iFrameArray.Append( testBuf ); 
        iRead->iFrameIndex = 0;
        EUNIT_ASSERT_NO_LEAVE( iRead->GetNextFrame( getBuffer ) );
        
        }
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iRead->GetNextFrame( getBuffer ) );
        }
    }


void UT_CG711PayloadFormatRead::UT_CG711PayloadFormatRead_ConfigurePayloadFormatLL(  )
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

        CG711PayloadFormatRead* read = CG711PayloadFormatRead::NewL( iRtpDataSourceStub );
        CleanupStack::PushL( read );
        read->ConfigurePayloadFormatL( buffer );
        CleanupStack::PopAndDestroy( read );
        
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
        codec.iAlgoUsed = 2;
        codec.iRedundancyCount = 1;
        codec.iRedundantPayload = 99;              
        buffer = codec;
        
        CMccRtpDataSource* rtpDataSourceStub = static_cast<CMccRtpDataSource*>(MDataSource::NewSourceL( KRtpDataSource, KNullDesC8 ));
        CleanupStack::PushL( rtpDataSourceStub );
        CMccRedPayloadRead* redDecoder = CMccRedPayloadRead::NewL( rtpDataSourceStub );
        CleanupStack::PushL( redDecoder );
        CG711PayloadFormatRead* read2 = CG711PayloadFormatRead::NewL( redDecoder );
        CleanupStack::PushL( read2 );
        read2->ConfigurePayloadFormatL( buffer );
        CleanupStack::PopAndDestroy( 3 ); 
        }
    else
        {
        EUNIT_ASSERT( ETrue );
        }

    }
    
void UT_CG711PayloadFormatRead::UpdateConfigurationLL()
    {
    if ( !iAlloc)
        {  
        TMccCodecInfo codec;
        codec.iHwFrameTime = 20;
        codec.iMaxPtime = 200;   
        iRead->UpdateConfigurationL( codec );

        TUint bufSize = iRead->iFrameBufferOne->Data().MaxLength();
        
        // If hwframetime is bigger, new sink buffers are created
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
        
        iRead->UpdateConfigurationL( codec );
        EUNIT_ASSERT( iRead->iFrameBufferOne->Data().MaxLength() > bufSize );
        bufSize = iRead->iFrameBufferOne->Data().MaxLength();

        // If hwframe time is smaller, new sink buffers are not created
        codec.iHwFrameTime = 10;
        iRead->UpdateConfigurationL( codec );
        EUNIT_ASSERT( iRead->iFrameBufferOne->Data().MaxLength() == bufSize );
        
        // Test with red
        CG711PayloadFormatRead* read = CG711PayloadFormatRead::NewL( iRtpDataSourceStub );
        CleanupStack::PushL( read );
        read->UpdateConfigurationL( codec );
        read->iSourceBufOwnership = ETrue;
        read->iCInfo.iRedundancyCount = 1;
        codec.iHwFrameTime = 20;
        codec.iMaxPtime = 200;
        read->UpdateConfigurationL( codec );
        
        CleanupStack::PopAndDestroy( read );
        }
    else
        {
        EUNIT_ASSERT( ETrue );
        }    
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CG711PayloadFormatRead,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CG711PayloadFormatRead",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CG711PayloadFormatRead_NewLL, Teardown)

EUNIT_TEST(
    "SendDataToSinkL - test ",
    "CG711PayloadFormatRead",
    "SendDataToSinkL",
    "FUNCTIONALITY",
    SetupL, UT_CG711PayloadFormatRead_SendDataToSinkLL, Teardown)

EUNIT_TEST(
    "FillSinkBuffer - test ",
    "CG711PayloadFormatRead",
    "FillSinkBuffer",
    "FUNCTIONALITY",
    SetupL, UT_CG711PayloadFormatRead_FillSinkBufferL, Teardown)

EUNIT_TEST(
    "FillSourceBufferL - test ",
    "CG711PayloadFormatRead",
    "FillSourceBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CG711PayloadFormatRead_FillSourceBufferLL, Teardown)

EUNIT_TEST(
    "Streams - test ",
    "CG711PayloadFormatRead",
    "Streams",
    "FUNCTIONALITY",
    SetupL, UT_CG711PayloadFormatRead_StreamsL, Teardown)

EUNIT_TEST(
    "FrameTimeInterval - test ",
    "CG711PayloadFormatRead",
    "FrameTimeInterval",
    "FUNCTIONALITY",
    SetupL, UT_CG711PayloadFormatRead_FrameTimeIntervalL, Teardown)

EUNIT_TEST(
    "Duration - test ",
    "CG711PayloadFormatRead",
    "Duration",
    "FUNCTIONALITY",
    SetupL, UT_CG711PayloadFormatRead_DurationL, Teardown)

EUNIT_TEST(
    "NumChannels - test ",
    "CG711PayloadFormatRead",
    "NumChannels",
    "FUNCTIONALITY",
    SetupL, UT_CG711PayloadFormatRead_NumChannelsL, Teardown)

EUNIT_TEST(
    "SampleRate - test ",
    "CG711PayloadFormatRead",
    "SampleRate",
    "FUNCTIONALITY",
    SetupL, UT_CG711PayloadFormatRead_SampleRateL, Teardown)

EUNIT_TEST(
    "SetSampleRate - test ",
    "CG711PayloadFormatRead",
    "SetSampleRate",
    "FUNCTIONALITY",
    SetupL, UT_CG711PayloadFormatRead_SetSampleRateL, Teardown)

EUNIT_TEST(
    "SourceDataTypeCode - test ",
    "CG711PayloadFormatRead",
    "SourceDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CG711PayloadFormatRead_SourceDataTypeCodeL, Teardown)

EUNIT_TEST(
    "SetSourceDataTypeCode - test ",
    "CG711PayloadFormatRead",
    "SetSourceDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CG711PayloadFormatRead_SetSourceDataTypeCodeL, Teardown)

EUNIT_TEST(
    "CreateSourceBufferL - test ",
    "CG711PayloadFormatRead",
    "CreateSourceBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CG711PayloadFormatRead_CreateSourceBufferLL, Teardown)

EUNIT_TEST(
    "CreateSourceBuffer1L - test ",
    "CG711PayloadFormatRead",
    "CreateSourceBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CG711PayloadFormatRead_CreateSourceBufferL_1L, Teardown)

EUNIT_TEST(
    "FillBufferL - test ",
    "CG711PayloadFormatRead",
    "FillBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CG711PayloadFormatRead_FillBufferLL, Teardown)

EUNIT_TEST(
    "SourceThreadLogon - test ",
    "CG711PayloadFormatRead",
    "SourceThreadLogon",
    "FUNCTIONALITY",
    SetupL, UT_CG711PayloadFormatRead_SourceThreadLogonL, Teardown)

EUNIT_TEST(
    "SourceThreadLogoff - test ",
    "CG711PayloadFormatRead",
    "SourceThreadLogoff",
    "FUNCTIONALITY",
    SetupL, UT_CG711PayloadFormatRead_SourceThreadLogoffL, Teardown)

EUNIT_TEST(
    "NegotiateSourceL - test ",
    "CG711PayloadFormatRead",
    "NegotiateSourceL",
    "FUNCTIONALITY",
    SetupL, UT_CG711PayloadFormatRead_NegotiateSourceLL, Teardown)

EUNIT_TEST(
    "SourcePrimeL - test ",
    "CG711PayloadFormatRead",
    "SourcePrimeL",
    "FUNCTIONALITY",
    SetupL, UT_CG711PayloadFormatRead_SourcePrimeLL, Teardown)

EUNIT_TEST(
    "SourcePlayL - test ",
    "CG711PayloadFormatRead",
    "SourcePlayL",
    "FUNCTIONALITY",
    SetupL, UT_CG711PayloadFormatRead_SourcePlayLL, Teardown)

EUNIT_TEST(
    "SourcePauseL - test ",
    "CG711PayloadFormatRead",
    "SourcePauseL",
    "FUNCTIONALITY",
    SetupL, UT_CG711PayloadFormatRead_SourcePauseLL, Teardown)

EUNIT_TEST(
    "SourceStopL - test ",
    "CG711PayloadFormatRead",
    "SourceStopL",
    "FUNCTIONALITY",
    SetupL, UT_CG711PayloadFormatRead_SourceStopLL, Teardown)

EUNIT_TEST(
    "BufferFilledL - test ",
    "CG711PayloadFormatRead",
    "BufferFilledL",
    "FUNCTIONALITY",
    SetupL, UT_CG711PayloadFormatRead_BufferFilledLL, Teardown)

EUNIT_TEST(
    "SinkDataTypeCode - test ",
    "CG711PayloadFormatRead",
    "SinkDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CG711PayloadFormatRead_SinkDataTypeCodeL, Teardown)

EUNIT_TEST(
    "GetNextFrame - test ",
    "CG711PayloadFormatRead",
    "GetNextFrame",
    "FUNCTIONALITY",
    SetupL, UT_CG711PayloadFormatRead_GetNextFrameL, Teardown)

EUNIT_TEST(
    "ConfigurePayloadFormatL - test ",
    "CG711PayloadFormatRead",
    "ConfigurePayloadFormatL",
    "FUNCTIONALITY",
    SetupL, UT_CG711PayloadFormatRead_ConfigurePayloadFormatLL, Teardown) 

EUNIT_TEST(
    "UpdateConfigurationL - test ",
    "CG711PayloadFormatRead",
    "UpdateConfigurationL",
    "FUNCTIONALITY",
    SetupL, UpdateConfigurationLL, Teardown)        

#ifdef EUNIT_ALLOC_TEST_ON  
   
EUNIT_ALLOC_TEST(
    "SendDataToSinkL - alloc test ",
    "CG711PayloadFormatRead",
    "SendDataToSinkL",
    "ERRORHANDLING",
    SetupAL, UT_CG711PayloadFormatRead_SendDataToSinkLL, Teardown)

EUNIT_ALLOC_TEST(
    "FillSinkBuffer - alloc test ",
    "CG711PayloadFormatRead",
    "FillSinkBuffer",
    "ERRORHANDLING",
    SetupAL, UT_CG711PayloadFormatRead_FillSinkBufferL, Teardown)

EUNIT_ALLOC_TEST(
    "FillSourceBufferL - alloc test ",
    "CG711PayloadFormatRead",
    "FillSourceBufferL",
    "ERRORHANDLING",
    SetupAL, UT_CG711PayloadFormatRead_FillSourceBufferLL, Teardown)

EUNIT_ALLOC_TEST(
    "Streams - alloc test ",
    "CG711PayloadFormatRead",
    "Streams",
    "ERRORHANDLING",
    SetupAL, UT_CG711PayloadFormatRead_StreamsL, Teardown)

EUNIT_ALLOC_TEST(
    "FrameTimeInterval - alloc test ",
    "CG711PayloadFormatRead",
    "FrameTimeInterval",
    "ERRORHANDLING",
    SetupAL, UT_CG711PayloadFormatRead_FrameTimeIntervalL, Teardown)

EUNIT_ALLOC_TEST(
    "Duration - alloc test ",
    "CG711PayloadFormatRead",
    "Duration",
    "ERRORHANDLING",
    SetupAL, UT_CG711PayloadFormatRead_DurationL, Teardown)

EUNIT_ALLOC_TEST(
    "NumChannels - alloc test ",
    "CG711PayloadFormatRead",
    "NumChannels",
    "ERRORHANDLING",
    SetupAL, UT_CG711PayloadFormatRead_NumChannelsL, Teardown)

EUNIT_ALLOC_TEST(
    "SampleRate - alloc test ",
    "CG711PayloadFormatRead",
    "SampleRate",
    "ERRORHANDLING",
    SetupAL, UT_CG711PayloadFormatRead_SampleRateL, Teardown)

EUNIT_ALLOC_TEST(
    "SetSampleRate - alloc test ",
    "CG711PayloadFormatRead",
    "SetSampleRate",
    "ERRORHANDLING",
    SetupAL, UT_CG711PayloadFormatRead_SetSampleRateL, Teardown)

EUNIT_ALLOC_TEST(
    "SourceDataTypeCode - alloc test ",
    "CG711PayloadFormatRead",
    "SourceDataTypeCode",
    "ERRORHANDLING",
    SetupAL, UT_CG711PayloadFormatRead_SourceDataTypeCodeL, Teardown)

EUNIT_ALLOC_TEST(
    "SetSourceDataTypeCode - alloc test ",
    "CG711PayloadFormatRead",
    "SetSourceDataTypeCode",
    "ERRORHANDLING",
    SetupAL, UT_CG711PayloadFormatRead_SetSourceDataTypeCodeL, Teardown)

EUNIT_ALLOC_TEST(
    "CreateSourceBufferL - alloc test ",
    "CG711PayloadFormatRead",
    "CreateSourceBufferL",
    "ERRORHANDLING",
    SetupAL, UT_CG711PayloadFormatRead_CreateSourceBufferLL, Teardown)

EUNIT_ALLOC_TEST(
    "CreateSourceBuffer1L - alloc test ",
    "CG711PayloadFormatRead",
    "CreateSourceBufferL",
    "ERRORHANDLING",
    SetupAL, UT_CG711PayloadFormatRead_CreateSourceBufferL_1L, Teardown)

EUNIT_ALLOC_TEST(
    "FillBufferL - alloc test ",
    "CG711PayloadFormatRead",
    "FillBufferL",
    "ERRORHANDLING",
    SetupAL, UT_CG711PayloadFormatRead_FillBufferLL, Teardown)

EUNIT_ALLOC_TEST(
    "SourceThreadLogon - alloc test ",
    "CG711PayloadFormatRead",
    "SourceThreadLogon",
    "ERRORHANDLING",
    SetupAL, UT_CG711PayloadFormatRead_SourceThreadLogonL, Teardown)

EUNIT_ALLOC_TEST(
    "SourceThreadLogoff - alloc test ",
    "CG711PayloadFormatRead",
    "SourceThreadLogoff",
    "ERRORHANDLING",
    SetupAL, UT_CG711PayloadFormatRead_SourceThreadLogoffL, Teardown)

EUNIT_ALLOC_TEST(
    "NegotiateSourceL - alloc test ",
    "CG711PayloadFormatRead",
    "NegotiateSourceL",
    "ERRORHANDLING",
    SetupAL, UT_CG711PayloadFormatRead_NegotiateSourceLL, Teardown)

EUNIT_ALLOC_TEST(
    "SourcePrimeL - alloc test ",
    "CG711PayloadFormatRead",
    "SourcePrimeL",
    "ERRORHANDLING",
    SetupAL, UT_CG711PayloadFormatRead_SourcePrimeLL, Teardown)

EUNIT_ALLOC_TEST(
    "SourcePlayL - alloc test ",
    "CG711PayloadFormatRead",
    "SourcePlayL",
    "ERRORHANDLING",
    SetupAL, UT_CG711PayloadFormatRead_SourcePlayLL, Teardown)

EUNIT_ALLOC_TEST(
    "SourcePauseL - alloc test ",
    "CG711PayloadFormatRead",
    "SourcePauseL",
    "ERRORHANDLING",
    SetupAL, UT_CG711PayloadFormatRead_SourcePauseLL, Teardown)

EUNIT_ALLOC_TEST(
    "SourceStopL - alloc test ",
    "CG711PayloadFormatRead",
    "SourceStopL",
    "ERRORHANDLING",
    SetupAL, UT_CG711PayloadFormatRead_SourceStopLL, Teardown)

EUNIT_ALLOC_TEST(
    "BufferFilledL - alloc test ",
    "CG711PayloadFormatRead",
    "BufferFilledL",
    "ERRORHANDLING",
    SetupAL, UT_CG711PayloadFormatRead_BufferFilledLL, Teardown)

EUNIT_ALLOC_TEST(
    "SinkDataTypeCode - alloc test ",
    "CG711PayloadFormatRead",
    "SinkDataTypeCode",
    "ERRORHANDLING",
    SetupAL, UT_CG711PayloadFormatRead_SinkDataTypeCodeL, Teardown)

EUNIT_ALLOC_TEST(
    "GetNextFrame - alloc test ",
    "CG711PayloadFormatRead",
    "GetNextFrame",
    "ERRORHANDLING",
    SetupL, UT_CG711PayloadFormatRead_GetNextFrameL, Teardown)
    
EUNIT_ALLOC_TEST(
    "ConfigurePayloadFormatL - alloc test ",
    "CG711PayloadFormatRead",
    "ConfigurePayloadFormatL",
    "ERRORHANDLING",
    SetupAL, UT_CG711PayloadFormatRead_ConfigurePayloadFormatLL, Teardown)
    
EUNIT_TEST(
    "UpdateConfigurationL - test ",
    "CG711PayloadFormatRead",
    "UpdateConfigurationL",
    "ERRORHANDLING",
    SetupL, UpdateConfigurationLL, Teardown)     


#endif

EUNIT_END_TEST_TABLE

//  END OF FILE
