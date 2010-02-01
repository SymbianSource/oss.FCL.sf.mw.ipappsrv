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
#include "UT_CMccVideoSink.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>

#ifdef EUNIT_ALLOC_TEST_ON
	#include <digia/eunit/CEunitAllocTestCaseDecorator.h>  //for using EUNIT_ALLOC_TEST
#endif // end of EUNIT_ALLOC_TEST_ON

//  INTERNAL INCLUDES
#include "MccVideoSink.h"
#include "MmccInterfaceDef.h"
#include "MccInternalDef.h"
#include "Mccdldatapath.h"
#include "MccInternalCodecs.h"
#include "mccresourcepool.h"
#include "mcctesteventhandler.h"
#include "mccvideosinkimpl.h"


// CONSTRUCTION
UT_CMccVideoSink* UT_CMccVideoSink::NewL()
    {
    UT_CMccVideoSink* self = UT_CMccVideoSink::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccVideoSink* UT_CMccVideoSink::NewLC()
    {
    UT_CMccVideoSink* self = new( ELeave ) UT_CMccVideoSink();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccVideoSink::~UT_CMccVideoSink()
    {
    }

// Default constructor
UT_CMccVideoSink::UT_CMccVideoSink()
    {
    }

// Second phase construct
void UT_CMccVideoSink::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMccVideoSink::SetupL(  )
    {
    iBuffer = CMMFDescriptorBuffer::NewL( 10 );
    iBuffer->Data().Copy(_L("aaaaaaaaaa"));
    TMediaId media = KUidMediaTypeAudio;
    MAsyncEventHandler* eventHandler = NULL;
    CMccResourcePool* resourcePool = NULL;
    iDLDataPathStub = 
    	CMccDlDataPath::NewL( eventHandler, resourcePool, media );
    
    iVideoSink = static_cast<CMccVideoSink*>(
    	CMccVideoSink::NewSinkL(KMccVideoSinkUid, _L8("IniPara")));
    	  
    TMccVideoSinkSettingBuf sinkSetingbuf;
	sinkSetingbuf().iAudioPayloadType = 106;
    sinkSetingbuf().iVideoPayloadType = 96;
    sinkSetingbuf().iDeviceIndex = 0;
    sinkSetingbuf().iLocation.iX = 0;
    sinkSetingbuf().iLocation.iY = 83;
    sinkSetingbuf().iSize.iWidth = 352;
    sinkSetingbuf().iSize.iHeight = 378;
    sinkSetingbuf().iIAPId = 2;
    sinkSetingbuf().iPriority.iPref = EMdaPriorityPreferenceNone;
    sinkSetingbuf().iPriority.iPriority = 0;
    sinkSetingbuf().iPriority.iState = EMMFStateIdle;
     
   
    iVideoSink->ConstructSinkL( sinkSetingbuf );
    
    /*
    TMccVideoSinkSettingBuf sinkSetingbuf;
	sinkSetingbuf().iAudioPayloadType = 106;
    sinkSetingbuf().iVideoPayloadType = 96;
    sinkSetingbuf().iDeviceIndex = 0;
    sinkSetingbuf().iLocation.iX = 0;
    sinkSetingbuf().iLocation.iY = 83;
    sinkSetingbuf().iSize.iWidth = 352;
    sinkSetingbuf().iSize.iHeight = 378;
    sinkSetingbuf().iIAPId = 2;
    sinkSetingbuf().iPriority.iPref = EMdaPriorityPreferenceNone;
    sinkSetingbuf().iPriority.iPriority = 0;
    sinkSetingbuf().iPriority.iState = EMMFStateIdle;
    iVideoSink = static_cast<CMccVideoSink*>(
    	MDataSink::NewSinkL(KMccVideoSinkUid, sinkSetingbuf));
    */	
    }

void UT_CMccVideoSink::Teardown(  )
    {
	delete iVideoSink;
	iVideoSink = NULL;
	delete iDLDataPathStub;
	delete iBuffer;
    }

void UT_CMccVideoSink::UT_CMccVideoSink_NewSinkLL(  )
    {
    //NOP
    }

void UT_CMccVideoSink::UT_CMccVideoSink_EmptyBufferLL(  )
    {
    TMediaId mediaId( KUidMediaTypeAudio );
    iVideoSink->EmptyBufferL( iBuffer, iDLDataPathStub, mediaId );
    
    TMediaId mediaId2( KUidMediaTypeVideo );
    iVideoSink->EmptyBufferL( iBuffer, iDLDataPathStub, mediaId2 );
    }

void UT_CMccVideoSink::UT_CMccVideoSink_EmptyBufferL_1L(  )
    {
    }

void UT_CMccVideoSink::UT_CMccVideoSink_SinkThreadLogonL(  )
    {
    MAsyncEventHandler* asynEventHandler = NULL;
    iVideoSink->SinkThreadLogon( *asynEventHandler );
    }

void UT_CMccVideoSink::UT_CMccVideoSink_SinkThreadLogoffL(  )
    {
    CMccTestEventHandler* handler1 = CMccTestEventHandler::NewL();
    CleanupStack::PushL( handler1 );
    User::LeaveIfError( iVideoSink->SinkThreadLogon( *handler1 ) );
    
    CMccTestEventHandler* handler2 = CMccTestEventHandler::NewL();
    CleanupStack::PushL( handler2 );
    User::LeaveIfError( iVideoSink->SinkThreadLogon( *handler2 ) );
    
    // Logoff first user, sink is not stopped as there's still other user for it
    iVideoSink->SetCurrentUser( handler1 );
    iVideoSink->SinkThreadLogoff();
    
    EUNIT_ASSERT( iVideoSink->iMediaInterface->iState != CMccVideoSinkImpl::EStopped );
    
    // Logoff second user, sink is stopped as no-one is using it anymore
    iVideoSink->SetCurrentUser( handler2 );
    iVideoSink->SinkThreadLogoff();
    
    EUNIT_ASSERT( iVideoSink->iMediaInterface->iState == CMccVideoSinkImpl::EStopped );
    
    CleanupStack::PopAndDestroy( handler2 );
    CleanupStack::PopAndDestroy( handler1 );
    }

void UT_CMccVideoSink::UT_CMccVideoSink_SinkDataTypeCodeL(  )
    {
    TMediaId mediaId;
    EUNIT_ASSERT(iVideoSink->SinkDataTypeCode( mediaId ) == TFourCC(KMMFFourCCCodeNULL) );
                        
    mediaId.iMediaType = KUidMediaTypeVideo;
    EUNIT_ASSERT(iVideoSink->SinkDataTypeCode( mediaId ) == TFourCC(KMccFourCCIdH263) );
     
    mediaId.iMediaType = KUidMediaTypeAudio;
    EUNIT_ASSERT(iVideoSink->SinkDataTypeCode( mediaId ) == TFourCC(KMccFourCCIdAMRNB) );            
    }

void UT_CMccVideoSink::UT_CMccVideoSink_SetSinkDataTypeCodeL(  )
    {
    TMediaId mediaId( KUidMediaTypeVideo );
    TFourCC codec1( KMccFourCCIdH263 );
    EUNIT_ASSERT_EQUALS(
    	iVideoSink->SetSinkDataTypeCode(codec1, mediaId),
        KErrNone );
    mediaId.iMediaType = KUidMediaTypeAudio;
    codec1.Set(KMccFourCCIdAMRNB);

    EUNIT_ASSERT_EQUALS(
    	iVideoSink->SetSinkDataTypeCode(codec1, mediaId),
        KErrNone );
    
    mediaId.iMediaType = KUidMediaTypeMidi;
    
    EUNIT_ASSERT_EQUALS(
    	iVideoSink->SetSinkDataTypeCode(codec1, mediaId),
        KErrNotSupported );
    }

void UT_CMccVideoSink::UT_CMccVideoSink_BufferFilledLL(  )
    {
    CMMFBuffer* buffer = NULL;
    EUNIT_ASSERT_SPECIFIC_LEAVE(
    	iVideoSink->BufferFilledL(buffer),KErrNotSupported );
    }

void UT_CMccVideoSink::UT_CMccVideoSink_CanCreateSinkBufferL(  )
    {
    EUNIT_ASSERT( !iVideoSink->CanCreateSinkBuffer() );
    }

void UT_CMccVideoSink::UT_CMccVideoSink_CreateSinkBufferLL(  )
    {
    TMediaId mediaId;
    TBool bufRef =EFalse;
	EUNIT_ASSERT_SPECIFIC_LEAVE(
		iVideoSink->CreateSinkBufferL(mediaId,bufRef), KErrNotSupported );
    }

void UT_CMccVideoSink::UT_CMccVideoSink_SinkPrimeLL(  )
    {
	iVideoSink->SinkPrimeL();
    }

void UT_CMccVideoSink::UT_CMccVideoSink_SinkPlayLL(  )
    {
    iVideoSink->SinkPrimeL();
    iVideoSink->SinkPlayL();
    }

void UT_CMccVideoSink::UT_CMccVideoSink_SinkPauseLL(  )
    {
    iVideoSink->SinkPrimeL();
    iVideoSink->SinkPlayL();
    iVideoSink->SinkPauseL();
    }

void UT_CMccVideoSink::UT_CMccVideoSink_SinkStopLL(  )
    {
    iVideoSink->SinkStopL();
    }

void UT_CMccVideoSink::UT_CMccVideoSink_SinkCustomCommandL(  )
    {
    TMMFMessage* message = NULL;
    iVideoSink->SinkCustomCommand( *message );
    }
    
void UT_CMccVideoSink::UT_CMccVideoSink_SetVideoCodecL()    
	{
	TMccCodecInfo codec;
	codec.iFourCC.Set(KMccFourCCIdH263);
	codec.iCodecMode = 2000;
	codec.iMaxBitrate = 35000;
	codec.iPayloadType = 96;
	iVideoSink->iAudioPayloadType = 96;

    // Impl not configured yet
	EUNIT_ASSERT_SPECIFIC_LEAVE(iVideoSink->SetVideoCodecL( codec ), KErrNotFound);

	}

void UT_CMccVideoSink::UT_CMccVideoSink_SetAudioCodecL()
    {
    TMccCodecInfo codec;
	codec.iFourCC.Set(KMccFourCCIdAMRNB);
	codec.iCodecMode = 2000;
	codec.iMaxBitrate = 35000;
	codec.iPayloadType = 96;
	iVideoSink->iVideoPayloadType = 96;

    // Impl not configured yet
	EUNIT_ASSERT_SPECIFIC_LEAVE(iVideoSink->SetAudioCodecL( codec ), KErrNotFound);
    }

void UT_CMccVideoSink::UT_CMccVideoSink_SetCurrentUserL()
    {
    MAsyncEventHandler* user = NULL;
    iVideoSink->SetCurrentUser( user );
    }
    
void UT_CMccVideoSink::UT_CMccVideoSink_GetParameterL()
    {
    TMccVideoSinkSettingBuf sinkSetingbuf;
   
    EUNIT_ASSERT_SPECIFIC_LEAVE(iVideoSink->GetParameterL(0, sinkSetingbuf), KErrArgument);
    EUNIT_ASSERT_NO_LEAVE(iVideoSink->GetParameterL(KMccDisplayLocalVideo, sinkSetingbuf));
    
    TPckgBuf<TInt> packageBuffer = 0;
    EUNIT_ASSERT_NO_LEAVE( iVideoSink->GetParameterL( KMccPrerollTime, packageBuffer ) );
    }

void UT_CMccVideoSink::UT_CMccVideoSink_SetParameterL()
    {
    TMccVideoSinkSettingBuf sinkSetingbuf;
    
	sinkSetingbuf().iAudioPayloadType = 106;
    sinkSetingbuf().iVideoPayloadType = 96;
    sinkSetingbuf().iDeviceIndex = 0;
    sinkSetingbuf().iLocation.iX = 0;
    sinkSetingbuf().iLocation.iY = 83;
    sinkSetingbuf().iSize.iWidth = 352;
    sinkSetingbuf().iSize.iHeight = 378;
    sinkSetingbuf().iIAPId = 2;
    sinkSetingbuf().iPriority.iPref = EMdaPriorityPreferenceNone;
    sinkSetingbuf().iPriority.iPriority = 0;
    sinkSetingbuf().iPriority.iState = EMMFStateIdle;
        
    EUNIT_ASSERT_SPECIFIC_LEAVE(iVideoSink->SetParameterL(0, sinkSetingbuf), KErrArgument);
    EUNIT_ASSERT_NO_LEAVE(iVideoSink->SetParameterL(KMccDisplayLocalVideo, sinkSetingbuf));
    
    EUNIT_ASSERT_NO_LEAVE(iVideoSink->SetParameterL( KMccSpeakerVolume, TPckgBuf<TInt>( 5 ) ) );

    }
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccVideoSink,
    "CMccVideoSink",
    "CMccVideoSink" )

EUNIT_TEST(
    "NewSinkL - test ",
    "CMccVideoSink",
    "NewSinkL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSink_NewSinkLL, Teardown)
    
EUNIT_TEST(
    "EmptyBufferL - test ",
    "CMccVideoSink",
    "EmptyBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSink_EmptyBufferLL, Teardown)

EUNIT_TEST(
    "EmptyBufferL - test ",
    "CMccVideoSink",
    "EmptyBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSink_EmptyBufferL_1L, Teardown)

EUNIT_TEST(
    "SinkThreadLogon - test ",
    "CMccVideoSink",
    "SinkThreadLogon",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSink_SinkThreadLogonL, Teardown)

EUNIT_TEST(
    "SinkThreadLogoff - test ",
    "CMccVideoSink",
    "SinkThreadLogoff",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSink_SinkThreadLogoffL, Teardown)

EUNIT_TEST(
    "SinkDataTypeCode - test ",
    "CMccVideoSink",
    "SinkDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSink_SinkDataTypeCodeL, Teardown)

EUNIT_TEST(
    "SetSinkDataTypeCode - test ",
    "CMccVideoSink",
    "SetSinkDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSink_SetSinkDataTypeCodeL, Teardown)

EUNIT_TEST(
    "BufferFilledL - test ",
    "CMccVideoSink",
    "BufferFilledL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSink_BufferFilledLL, Teardown)

EUNIT_TEST(
    "CanCreateSinkBuffer - test ",
    "CMccVideoSink",
    "CanCreateSinkBuffer",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSink_CanCreateSinkBufferL, Teardown)

EUNIT_TEST(
    "CreateSinkBufferL - test ",
    "CMccVideoSink",
    "CreateSinkBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSink_CreateSinkBufferLL, Teardown)

EUNIT_TEST(
    "SinkPrimeL - test ",
    "CMccVideoSink",
    "SinkPrimeL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSink_SinkPrimeLL, Teardown)

EUNIT_TEST(
    "SinkPlayL - test ",
    "CMccVideoSink",
    "SinkPlayL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSink_SinkPlayLL, Teardown)

EUNIT_TEST(
    "SinkPauseL - test ",
    "CMccVideoSink",
    "SinkPauseL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSink_SinkPauseLL, Teardown)

EUNIT_TEST(
    "SinkStopL - test ",
    "CMccVideoSink",
    "SinkStopL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSink_SinkStopLL, Teardown)

EUNIT_TEST(
    "SinkCustomCommand - test ",
    "CMccVideoSink",
    "SinkCustomCommand",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSink_SinkCustomCommandL, Teardown)

EUNIT_TEST(
    "SetVideoCodec - test ",
    "CMccVideoSink",
    "SetVideoCodecL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSink_SetVideoCodecL, Teardown)
    
EUNIT_TEST(
    "SetAudioCodecL - test ",
    "CMccVideoSink",
    "SetAudioCodecL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSink_SetAudioCodecL, Teardown)
    
EUNIT_TEST(
    "SetCurrentUserL - test ",
    "CMccVideoSink",
    "SetCurrentUserL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSink_SetCurrentUserL, Teardown)
    
EUNIT_TEST(
    "GetParameterL - test ",
    "CMccVideoSink",
    "GetParameterL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSink_GetParameterL, Teardown)
    
EUNIT_TEST(
    "SetParameterL - test ",
    "CMccVideoSink",
    "SetParameterL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSink_SetParameterL, Teardown)
    

EUNIT_END_TEST_TABLE

//  END OF FILE
