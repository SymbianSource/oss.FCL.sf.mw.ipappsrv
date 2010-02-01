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
#include "UT_CMccVideoSource.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#ifdef EUNIT_ALLOC_TEST_ON
	#include <digia/eunit/CEunitAllocTestCaseDecorator.h>  //for using EUNIT_ALLOC_TEST
#endif // end of EUNIT_ALLOC_TEST_ON

//  INTERNAL INCLUDES
#include "MccVideoSource.h"
#include "MmccInterfaceDef.h"
#include "MccInternalCodecs.h"
#include "Mcculdatapath.h"
#include "mccresourcepool.h"
#include "mcccamerahandler_STUB.h"
#include "mccvideosourceimpl.h"
#include "mcctesteventhandler.h"
#include "mccunittestmacros.h"
#include "mcctesteventhandler.h"


// CONSTRUCTION
UT_CMccVideoSource* UT_CMccVideoSource::NewL()
    {
    UT_CMccVideoSource* self = UT_CMccVideoSource::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccVideoSource* UT_CMccVideoSource::NewLC()
    {
    UT_CMccVideoSource* self = new( ELeave ) UT_CMccVideoSource();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccVideoSource::~UT_CMccVideoSource()
    {
    }

// Default constructor
UT_CMccVideoSource::UT_CMccVideoSource()
    {
    }

// Second phase construct
void UT_CMccVideoSource::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMccVideoSource::SetupL(  )
    {   
    iBuffer = CMMFDescriptorBuffer::NewL( 10 );
    iBuffer->Data().Copy(_L("aaaaaaaaaa"));
    TMediaId media = KUidMediaTypeAudio;
    iEventHandler = CMccTestEventHandler::NewL();
    MAsyncEventHandler* eventHandler = NULL;
    CMccResourcePool* resourcePool = NULL;
    iULDataPathStub = CMccUlDataPath::NewL( eventHandler, resourcePool, media );
    iVideoSource = static_cast<CMccVideoSource*>(CMccVideoSource::NewSourceL(KMccVideoSourceUid, _L8("Inidata")));
    TBuf8<5> data;
    data.Format( _L8( "foo42" ) );
    iVideoSource->ConstructSourceL( data );
    
    iVideoSource->SourceThreadLogon( *iEventHandler );
    
    iCameraHandler = CMccCameraHandlerStub::NewL();
    
    iVideoSource->SetCameraHandler( *iCameraHandler );
    }

void UT_CMccVideoSource::Teardown(  )
    {
   	delete iVideoSource;
   	delete iULDataPathStub;
    delete iBuffer;
    delete iCameraHandler;
    delete iEventHandler;
    }

void UT_CMccVideoSource::UT_CMccVideoSource_NewSourceLL(  )
    {
    }

void UT_CMccVideoSource::UT_CMccVideoSource_ConstructSourceLL(  )
    {
    iVideoSource->ConstructSourceL( _L8("Inidata") );
    }

void UT_CMccVideoSource::UT_CMccVideoSource_CMccVideoSourceL(  )
    {
    }

void UT_CMccVideoSource::UT_CMccVideoSource_SetCameraHandleL(  )
    {
    iVideoSource->SetCameraHandler( *iCameraHandler );
    }

void UT_CMccVideoSource::UT_CMccVideoSource_SetVideoCodecLL(  )
    {
    TMccCodecInfo codecInfo;
    iVideoSource->SetVideoCodecL( codecInfo );
    }

void UT_CMccVideoSource::UT_CMccVideoSource_GetVideoCodecLL(  )
    {
    TMccCodecInfo codecInfo1, codecInfo2;
    iVideoSource->SetVideoCodecL( codecInfo1 );
    iVideoSource->GetVideoCodecL( codecInfo2 );
    EUNIT_ASSERT( TMccCodecInfo::Compare( codecInfo1, codecInfo2 ) );
    }

void UT_CMccVideoSource::UT_CMccVideoSource_GetSupportedVideoCodecsLL(  )
    {
    RArray<TFourCC> fourCCArray;
    CleanupClosePushL(fourCCArray);
	iVideoSource->GetSupportedVideoCodecsL( fourCCArray );
	EUNIT_ASSERT_EQUALS( fourCCArray.Count(), 0 );
	CleanupStack::PopAndDestroy( &fourCCArray );
    }

void UT_CMccVideoSource::UT_CMccVideoSource_SetAudioCodecLL(  )
    {
    TMccCodecInfo codecInfo;
    iVideoSource->SetAudioCodecL( codecInfo );
    }

void UT_CMccVideoSource::UT_CMccVideoSource_GetAudioCodecLL(  )
    {
    TMccCodecInfo codecInfo1, codecInfo2;

    iVideoSource->GetAudioCodecL( codecInfo1 );
    EUNIT_ASSERT( TMccCodecInfo::Compare( codecInfo1, codecInfo2 ) );
    }

void UT_CMccVideoSource::UT_CMccVideoSource_GetSupportedAudioCodecsLL(  )
    {
    RArray<TFourCC> fourCCArray;
    CleanupClosePushL(fourCCArray);

	iVideoSource->GetSupportedAudioCodecsL( fourCCArray );
	EUNIT_ASSERT_EQUALS( fourCCArray.Count(), 0 );
	CleanupStack::PopAndDestroy( &fourCCArray );	
    }

void UT_CMccVideoSource::UT_CMccVideoSource_SetAudioEnabledLL(  )
    {
	iVideoSource->SetAudioEnabledL( ETrue );
	EUNIT_ASSERT( iVideoSource->AudioEnabledL());    }

void UT_CMccVideoSource::UT_CMccVideoSource_AudioEnabledLL(  )
    {
   	iVideoSource->SetAudioEnabledL( ETrue );
	EUNIT_ASSERT( iVideoSource->AudioEnabledL());
    }

void UT_CMccVideoSource::UT_CMccVideoSource_SetGainLL(  )
    {
 	iVideoSource->SetGainL( 0 );
    EUNIT_ASSERT_EQUALS( iVideoSource->GainL(), 0 );

    }

void UT_CMccVideoSource::UT_CMccVideoSource_GainLL(  )
    {
 	iVideoSource->SetGainL( 5 );
    EUNIT_ASSERT_EQUALS( iVideoSource->GainL(), 5 ); 
    }

void UT_CMccVideoSource::UT_CMccVideoSource_MaxGainLL(  )
    {
	iVideoSource->SetGainL( 5 );
    EUNIT_ASSERT_EQUALS( iVideoSource->MaxGainL(), 5 );    
    }

void UT_CMccVideoSource::UT_CMccVideoSource_SourceDataTypeCodeL(  )
    {
    TMediaId mediaId;
    EUNIT_ASSERT(iVideoSource->SourceDataTypeCode( mediaId ) == TFourCC(KMMFFourCCCodeNULL) );
                        
    mediaId.iMediaType = KUidMediaTypeVideo;
    EUNIT_ASSERT(iVideoSource->SourceDataTypeCode( mediaId ) == TFourCC() );
     
    mediaId.iMediaType = KUidMediaTypeAudio;
    EUNIT_ASSERT(iVideoSource->SourceDataTypeCode( mediaId ) == TFourCC() );                
  /*
    delete iVideoSource;
    
    iVideoSource  = static_cast<CMccVideoSource*>(CMccVideoSource::NewSourceL(KMccVideoSourceUID, _L8("Inidata")));

    EUNIT_ASSERT(iVideoSource->SourceDataTypeCode( mediaId ) == TFourCC(KMMFFourCCCodeNULL) );
    */
    }

void UT_CMccVideoSource::UT_CMccVideoSource_SetSourceDataTypeCodeL(  )
    {
    TMediaId mediaId( KUidMediaTypeVideo );
    TFourCC codec1( KMccFourCCIdH263 );
    EUNIT_ASSERT_EQUALS(
    	iVideoSource->SetSourceDataTypeCode(codec1, mediaId),
        KErrNotSupported );
    TMccCodecInfo codecInfo;
    codecInfo.iFourCC.Set(KMccFourCCIdH263);
    iVideoSource->SetAudioCodecL( codecInfo );
    iVideoSource->SetVideoCodecL( codecInfo );

    EUNIT_ASSERT_EQUALS(
    	iVideoSource->SetSourceDataTypeCode(codec1, mediaId),
        KErrNone );
    
    mediaId.iMediaType = KUidMediaTypeAudio;
    
    EUNIT_ASSERT_EQUALS(
    	iVideoSource->SetSourceDataTypeCode(codec1, mediaId),
        KErrNone ); 
    /*
    delete iVideoSource;
    
    iVideoSource  = static_cast<CMccVideoSource*>(CMccVideoSource::NewSourceL(KMccVideoSourceUID, _L8("Inidata")));
    EUNIT_ASSERT_EQUALS(iVideoSource->SetSourceDataTypeCode(codec1, mediaId), KErrArgument );   
    */
    }

void UT_CMccVideoSource::UT_CMccVideoSource_BufferEmptiedLL(  )
    {
    CMMFBuffer* buffer = NULL;
    EUNIT_ASSERT_SPECIFIC_LEAVE(
    	iVideoSource->BufferEmptiedL(buffer) , KErrNotSupported );    }

void UT_CMccVideoSource::UT_CMccVideoSource_CanCreateSourceBufferL(  )
    {
    EUNIT_ASSERT( !iVideoSource->CanCreateSourceBuffer() );
    }

void UT_CMccVideoSource::UT_CMccVideoSource_CreateSourceBufferLL(  )
    {
    TMediaId mediaId;
    TBool bufRef = EFalse;
    EUNIT_ASSERT_SPECIFIC_LEAVE(
    	iVideoSource->CreateSourceBufferL(mediaId, bufRef), KErrNotSupported );
    }

void UT_CMccVideoSource::UT_CMccVideoSource_SourceThreadLogonL(  )
    {
    MAsyncEventHandler* eventHandler = NULL;
    MCC_EUNIT_ASSERT_EQUALS(
    	iVideoSource->SourceThreadLogon(*eventHandler), KErrNone );
    }

void UT_CMccVideoSource::UT_CMccVideoSource_SourceThreadLogoffL(  )
    {
	iVideoSource->SourceThreadLogoff();
    }

void UT_CMccVideoSource::UT_CMccVideoSource_SourcePrimeLL(  )
    {
    TMccCodecInfo codecInfo;
    codecInfo.iSdpName = KH263SdpName;
    codecInfo.iFourCC.Set(KMccFourCCIdH263);
    codecInfo.iVideoHeight = 378;
    codecInfo.iVideoWidth = 352;
    codecInfo.iAverageBitrate = 30;
    codecInfo.iMaxBitrate = 45;
    codecInfo.iCodecMode = 2000;
  
    iVideoSource->SetVideoCodecL( codecInfo );
    iVideoSource->SourcePrimeL();
    
    EUNIT_ASSERT( iVideoSource->iSourceImpl->iMultipleCodecs == EFalse );
    }

void UT_CMccVideoSource::UT_CMccVideoSource_SourcePrimeL2L(  )
    {
    CMccTestEventHandler* eventHandler1 = CMccTestEventHandler::NewL();
    CleanupStack::PushL( eventHandler1 );
    TMccCodecInfo codecInfo;
    codecInfo.iSdpName = KH263SdpName;
    codecInfo.iFourCC.Set(KMccFourCCIdH263);
    codecInfo.iVideoHeight = 378;
    codecInfo.iVideoWidth = 352;
    codecInfo.iAverageBitrate = 30;
    codecInfo.iMaxBitrate = 45;
    codecInfo.iCodecMode = 2000;
    TInt err = iVideoSource->AddUser( eventHandler1 );
    MCC_EUNIT_ASSERT_EQUALS( err, KErrNone );
    EUNIT_ASSERT_EQUALS( iVideoSource->iUsers.Count(), 1 );
    // Try to add twice
    err = iVideoSource->AddUser( eventHandler1 );
    MCC_EUNIT_ASSERT_EQUALS( err, KErrNone );
    EUNIT_ASSERT_EQUALS( iVideoSource->iUsers.Count(), 1 );
    MCC_EUNIT_ASSERT_EQUALS( err, KErrNone );
    iVideoSource->SetCurrentUser( eventHandler1 );
    iVideoSource->SetVideoCodecL( codecInfo );
    
    CMccTestEventHandler* eventHandler2 = CMccTestEventHandler::NewL();
    CleanupStack::PushL( eventHandler2 );
    TMccCodecInfo codecInfo2;
    codecInfo2.iSdpName = KAVCSdpName;
    codecInfo2.iFourCC.Set(KMccFourCCIdAVC);
    codecInfo2.iVideoHeight = 378;
    codecInfo2.iVideoWidth = 352;
    codecInfo2.iAverageBitrate = 30;
    codecInfo2.iMaxBitrate = 45;
    err = iVideoSource->AddUser( eventHandler2 );
    MCC_EUNIT_ASSERT_EQUALS( err, KErrNone );
    EUNIT_ASSERT_EQUALS( iVideoSource->iUsers.Count(), 2 );
    iVideoSource->SetCurrentUser( eventHandler2 );
    iVideoSource->SetVideoCodecL( codecInfo2 );
    
    iVideoSource->SourcePrimeL();
    
    EUNIT_ASSERT( iVideoSource->iSourceImpl->iMultipleCodecs == ETrue );
    iVideoSource->RemoveUser( eventHandler2 );
    CleanupStack::PopAndDestroy( eventHandler2 );
    iVideoSource->RemoveUser( eventHandler1 );
    CleanupStack::PopAndDestroy( eventHandler1 );
    }
    
void UT_CMccVideoSource::UT_CMccVideoSource_SourcePlayLL(  )
    {
    // Codec not set
    EUNIT_ASSERT_LEAVE( iVideoSource->SourcePlayL() );
    
    UT_CMccVideoSource_SourcePrimeLL();
    
    iVideoSource->SourcePlayL();
    }

void UT_CMccVideoSource::UT_CMccVideoSource_SourcePauseLL(  )
    {
    iVideoSource->SourcePauseL();
    }

void UT_CMccVideoSource::UT_CMccVideoSource_SourceStopLL(  )
    {
    iVideoSource->SourceStopL();
    }

void UT_CMccVideoSource::UT_CMccVideoSource_FillBufferLL(  )
    {
    TMediaId mediaId( KUidMediaTypeAudio );
    EUNIT_ASSERT_SPECIFIC_LEAVE (
    	iVideoSource->FillBufferL( NULL, NULL, mediaId), KErrNotSupported );
    EUNIT_ASSERT_SPECIFIC_LEAVE (
    	iVideoSource->FillBufferL( iBuffer, NULL, mediaId), KErrArgument );	
    EUNIT_ASSERT_SPECIFIC_LEAVE (
    	iVideoSource->FillBufferL( iBuffer, iULDataPathStub, mediaId), KErrNotSupported );	
    mediaId.iMediaType = KUidMediaTypeVideo;
    iVideoSource->FillBufferL( iBuffer, iULDataPathStub, mediaId );	   
    }

void UT_CMccVideoSource::UT_CMccVideoSource_NegotiateSourceLL(  )
    {
    MDataSink* sink = NULL;
    iVideoSource->NegotiateSourceL( *sink );
    }
void UT_CMccVideoSource::UT_CMccVideoSource_UpdateL()
    {
    TMccVideoSourceSetting videosettings;
    videosettings.iDeviceIndex = 1;
    TMccVideoSourceSettingBuf settingsBuf(videosettings);
    iVideoSource->iCameraIndex = 0;

    EUNIT_ASSERT_NO_LEAVE(iVideoSource->UpdateL(settingsBuf));
    EUNIT_ASSERT( iVideoSource->iCameraIndex == 1 );
    }

void UT_CMccVideoSource::UT_CMccVideoSource_CameraIndexL()
    {
    iVideoSource->iCameraIndex = 1;
    EUNIT_ASSERT( iVideoSource->CameraIndex() == 1 );  
    }
    
void UT_CMccVideoSource::UT_CMccVideoSource_GetParameterL()
    {
    TBuf8<KMaxConfigKeyLen> confKey( KNullDesC8 );
    
    // No users
    EUNIT_ASSERT_SPECIFIC_LEAVE( iVideoSource->GetParameterL( KMccConfigKey, confKey ), KErrNotFound );
    
    // Avc and h263 users (impl behavior is quite complex, ignore return value as impl is tested separately)
    CMccTestEventHandler* eventHandler1 = CMccTestEventHandler::NewL();
    CleanupStack::PushL( eventHandler1 );
    TMccCodecInfo codecInfo;
    codecInfo.iSdpName = KH263SdpName;
    codecInfo.iFourCC.Set(KMccFourCCIdH263);
    codecInfo.iVideoHeight = 378;
    codecInfo.iVideoWidth = 352;
    codecInfo.iAverageBitrate = 30;
    codecInfo.iMaxBitrate = 45;
    codecInfo.iCodecMode = 2000;
    TInt err = iVideoSource->AddUser( eventHandler1 );
    MCC_EUNIT_ASSERT_EQUALS( err, KErrNone );
    EUNIT_ASSERT_EQUALS( iVideoSource->iUsers.Count(), 1 );
    iVideoSource->SetCurrentUser( eventHandler1 );
    iVideoSource->SetVideoCodecL( codecInfo );
    
    CMccTestEventHandler* eventHandler2 = CMccTestEventHandler::NewL();
    CleanupStack::PushL( eventHandler2 );
    TMccCodecInfo codecInfo2;
    codecInfo2.iSdpName = KAVCSdpName;
    codecInfo2.iFourCC.Set(KMccFourCCIdAVC);
    codecInfo2.iVideoHeight = 378;
    codecInfo2.iVideoWidth = 352;
    codecInfo2.iAverageBitrate = 30;
    codecInfo2.iMaxBitrate = 45;
    err = iVideoSource->AddUser( eventHandler2 );
    MCC_EUNIT_ASSERT_EQUALS( err, KErrNone );
    EUNIT_ASSERT_EQUALS( iVideoSource->iUsers.Count(), 2 );
    iVideoSource->SetCurrentUser( eventHandler2 );
    iVideoSource->SetVideoCodecL( codecInfo2 );
    
    TRAPD( err2, iVideoSource->GetParameterL( KMccConfigKey, confKey ) );
    if ( err2 == KErrNoMemory )
        {
        User::Leave( err2 );
        }

    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iVideoSource->GetParameterL( 1000, confKey ), KErrArgument ); 
    
    iVideoSource->RemoveUser( eventHandler2 );
    CleanupStack::PopAndDestroy( eventHandler2 );
    iVideoSource->RemoveUser( eventHandler1 );
    CleanupStack::PopAndDestroy( eventHandler1 );
    }

void UT_CMccVideoSource::UT_CMccVideoSource_SetParameterL()
    {
    TBuf8<KMaxConfigKeyLen> confKey( _L8("abc") );
    iVideoSource->SetParameterL( KMccConfigKey, confKey );
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iVideoSource->SetParameterL( 1000, confKey ), KErrArgument ); 
    }
        
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccVideoSource,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "NewSourceL - test ",
    "CMccVideoSource",
    "NewSourceL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSource_NewSourceLL, Teardown)

EUNIT_TEST(
    "ConstructSourceL - test ",
    "CMccVideoSource",
    "ConstructSourceL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSource_ConstructSourceLL, Teardown)

EUNIT_TEST(
    "CMccVideoSource - test ",
    "CMccVideoSource",
    "CMccVideoSource",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSource_CMccVideoSourceL, Teardown)

EUNIT_TEST(
    "SetCameraHandle - test ",
    "CMccVideoSource",
    "SetCameraHandle",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSource_SetCameraHandleL, Teardown)

EUNIT_TEST(
    "SetVideoCodecL - test ",
    "CMccVideoSource",
    "SetVideoCodecL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSource_SetVideoCodecLL, Teardown)

EUNIT_TEST(
    "GetVideoCodecL - test ",
    "CMccVideoSource",
    "GetVideoCodecL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSource_GetVideoCodecLL, Teardown)

EUNIT_TEST(
    "GetSupportedVideoCodecsL - test ",
    "CMccVideoSource",
    "GetSupportedVideoCodecsL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSource_GetSupportedVideoCodecsLL, Teardown)

EUNIT_TEST(
    "SetAudioCodecL - test ",
    "CMccVideoSource",
    "SetAudioCodecL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSource_SetAudioCodecLL, Teardown)

EUNIT_TEST(
    "GetAudioCodecL - test ",
    "CMccVideoSource",
    "GetAudioCodecL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSource_GetAudioCodecLL, Teardown)

EUNIT_TEST(
    "GetSupportedAudioCodecsL - test ",
    "CMccVideoSource",
    "GetSupportedAudioCodecsL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSource_GetSupportedAudioCodecsLL, Teardown)

EUNIT_TEST(
    "SetAudioEnabledL - test ",
    "CMccVideoSource",
    "SetAudioEnabledL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSource_SetAudioEnabledLL, Teardown)

EUNIT_TEST(
    "AudioEnabledL - test ",
    "CMccVideoSource",
    "AudioEnabledL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSource_AudioEnabledLL, Teardown)

EUNIT_TEST(
    "SetGainL - test ",
    "CMccVideoSource",
    "SetGainL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSource_SetGainLL, Teardown)

EUNIT_TEST(
    "GainL - test ",
    "CMccVideoSource",
    "GainL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSource_GainLL, Teardown)

EUNIT_TEST(
    "MaxGainL - test ",
    "CMccVideoSource",
    "MaxGainL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSource_MaxGainLL, Teardown)

EUNIT_TEST(
    "SourceDataTypeCode - test ",
    "CMccVideoSource",
    "SourceDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSource_SourceDataTypeCodeL, Teardown)

EUNIT_TEST(
    "SetSourceDataTypeCode - test ",
    "CMccVideoSource",
    "SetSourceDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSource_SetSourceDataTypeCodeL, Teardown)

EUNIT_TEST(
    "BufferEmptiedL - test ",
    "CMccVideoSource",
    "BufferEmptiedL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSource_BufferEmptiedLL, Teardown)

EUNIT_TEST(
    "CanCreateSourceBuffer - test ",
    "CMccVideoSource",
    "CanCreateSourceBuffer",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSource_CanCreateSourceBufferL, Teardown)

EUNIT_TEST(
    "CreateSourceBufferL - test ",
    "CMccVideoSource",
    "CreateSourceBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSource_CreateSourceBufferLL, Teardown)

EUNIT_TEST(
    "SourceThreadLogon - test ",
    "CMccVideoSource",
    "SourceThreadLogon",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSource_SourceThreadLogonL, Teardown)

EUNIT_TEST(
    "SourceThreadLogoff - test ",
    "CMccVideoSource",
    "SourceThreadLogoff",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSource_SourceThreadLogoffL, Teardown)

EUNIT_TEST(
    "SourcePrimeL - test ",
    "CMccVideoSource",
    "SourcePrimeL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSource_SourcePrimeLL, Teardown)

EUNIT_TEST(
    "SourcePrimeL 2 - test ",
    "CMccVideoSource",
    "SourcePrimeL 2",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSource_SourcePrimeL2L, Teardown)
    
EUNIT_TEST(
    "SourcePlayL - test ",
    "CMccVideoSource",
    "SourcePlayL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSource_SourcePlayLL, Teardown)

EUNIT_TEST(
    "SourcePauseL - test ",
    "CMccVideoSource",
    "SourcePauseL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSource_SourcePauseLL, Teardown)

EUNIT_TEST(
    "SourceStopL - test ",
    "CMccVideoSource",
    "SourceStopL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSource_SourceStopLL, Teardown)

EUNIT_TEST(
    "FillBufferL - test ",
    "CMccVideoSource",
    "FillBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSource_FillBufferLL, Teardown)

EUNIT_TEST(
    "NegotiateSourceL - test ",
    "CMccVideoSource",
    "NegotiateSourceL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSource_NegotiateSourceLL, Teardown)

EUNIT_TEST(
    "UpdateL - test ",
    "CMccVideoSource",
    "UpdateL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSource_UpdateL, Teardown)

EUNIT_TEST(
    "CameraIndexL - test ",
    "CMccVideoSource",
    "CameraIndexL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSource_CameraIndexL, Teardown)

EUNIT_TEST(
    "GetParameterL - test ",
    "CMccVideoSource",
    "GetParameterL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSource_GetParameterL, Teardown)

EUNIT_TEST(
    "SetParameterL - test ",
    "CMccVideoSource",
    "SetParameterL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSource_SetParameterL, Teardown)
    
EUNIT_END_TEST_TABLE

//  END OF FILE
