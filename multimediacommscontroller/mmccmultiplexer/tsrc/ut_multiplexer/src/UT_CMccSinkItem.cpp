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
#include "UT_CMccSinkItem.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <mmf/server/mmfdatabuffer.h>



#include <ecom/ecom.h>

//  INTERNAL INCLUDES
#include "mccsinkitem.h"
#include "mccrtpdatasource.h"
#include "mccrtpdatasink.h"
#include "amrpayloadformatread.h"
#include "mccuids.hrh"
#include "Mcctesteventhandler.h"
#include "mccinternalevents.h"
#include "Mccdldatapath.h"
#include "mccresourcepool.h"
#include "mccunittestmacros.h"


const TUid KRtpDataSource =  { KImplUidRtpDataSource };
const TUid KRtpDataSink =  { KImplUidRtpDataSink };
const TUid KMediaDecoder = { KImplUidAmrPayloadFormatDecode };

// CONSTRUCTION
UT_CMccSinkItem* UT_CMccSinkItem::NewL()
    {
    UT_CMccSinkItem* self = UT_CMccSinkItem::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccSinkItem* UT_CMccSinkItem::NewLC()
    {
    UT_CMccSinkItem* self = new( ELeave ) UT_CMccSinkItem();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccSinkItem::~UT_CMccSinkItem()
    {
    }

// Default constructor
UT_CMccSinkItem::UT_CMccSinkItem()
    {
    }

// Second phase construct
void UT_CMccSinkItem::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMccSinkItem::SetupL(  )
    {
    iRtpDataSourceStub = static_cast< CMccRtpDataSource* >( MDataSource::NewSourceL( KRtpDataSource, KNullDesC8 ) );
    iRead = CAmrPayloadFormatRead::NewL( iRtpDataSourceStub );
    iEventHandler = CMccTestEventHandler::NewL();
    
    TMediaId media = KUidMediaTypeAudio;
    CMccResourcePool* mccresource = NULL;
    iDLDataPathStub = CMccDlDataPath::NewL( iEventHandler, mccresource, media ); 
    iRead->FillBufferL( NULL, iDLDataPathStub, TMediaId() );                                    
    iSinkitem = CMccSinkItem::NewLC( iRead, ETrue, KNullUid, ETrue );
    CleanupStack::Pop( iSinkitem );
    }

void UT_CMccSinkItem::Setup2L(  )
    {    
    iRtpDataSinkStub = static_cast< CMccRtpDataSink* >( MDataSink::NewSinkL( KRtpDataSink, KNullDesC8 ) );
    iEventHandler = CMccTestEventHandler::NewL();

    iSinkitem = CMccSinkItem::NewLC( static_cast< MDataSink* >( iRtpDataSinkStub ), ETrue, KMediaDecoder, ETrue );
    CleanupStack::Pop( iSinkitem );
    }
    
void UT_CMccSinkItem::Teardown(  )
    {    
    delete iSinkitem;
    delete iDLDataPathStub;
    delete iEventHandler;
    delete iRead;
    delete iRtpDataSourceStub;
   
    REComSession::FinalClose();
    }
    
void UT_CMccSinkItem::Teardown2(  )
    {
    delete iSinkitem;

    delete iEventHandler;
    delete iRtpDataSinkStub;

    REComSession::FinalClose();
    }

void UT_CMccSinkItem::UT_CMccSinkItem_Sink2L()
    {
    CMccSinkItem* sinkItem = CMccSinkItem::NewLC( static_cast< MDataSink* >( iRtpDataSinkStub ), EFalse, KNullUid, EFalse );
    MDataSink* sink = sinkItem->Sink();
    EUNIT_ASSERT( sink == iRtpDataSinkStub );
    CleanupStack::PopAndDestroy( sinkItem );  
    }

     
void UT_CMccSinkItem::UT_CMccSinkItem_SinkL()
    {
    MDataSink* sink = iSinkitem->Sink();
    EUNIT_ASSERT( sink == iRead );
    }
         
void UT_CMccSinkItem::UT_CMccSinkItem_CopyBufferL()
    {
    CMMFDescriptorBuffer* bufferOri = CMMFDescriptorBuffer::NewL( 10 ); 
    CleanupStack::PushL( bufferOri );
    CMMFDescriptorBuffer* bufferDes = CMMFDescriptorBuffer::NewL( 10 ); 
    CleanupStack::PushL( bufferDes );
    
    bufferOri->SetFrameNumber( 1 );
    bufferOri->SetLastBuffer( 2 );
    bufferOri->SetPosition( 3 );
  
    EUNIT_ASSERT_NO_LEAVE( iSinkitem->CopyBufferL( bufferDes, bufferOri ) );
    CleanupStack::PopAndDestroy( bufferDes );
    CleanupStack::PopAndDestroy( bufferOri );
    }

void UT_CMccSinkItem::UT_CMccSinkItem_IsInternalDecoderL()
    {
    EUNIT_ASSERT( iSinkitem->IsInternalDecoder( KMediaDecoder ) );
    }

void UT_CMccSinkItem::UT_CMccSinkItem_BufferFilledLL(  )
    {
    CMMFDescriptorBuffer* buffer = CMMFDescriptorBuffer::NewL( 10 );
    CleanupStack::PushL( buffer );
    TRtpRecvHeader rtpHeader;
    
    //  if ( aMediaType != KNullUid && MediaType() != aMediaType )
    MCC_EUNIT_ASSERT_NO_LEAVE( iSinkitem->BufferFilledL( buffer, &rtpHeader, EFalse, KRtpDataSource ) );
    
    iSinkitem->iBadDataCount = 20;
    EUNIT_ASSERT( iSinkitem->SourceThreadLogon( *iEventHandler ) == KErrNone );

    CMMFDescriptorBuffer* emptyBuffer = CMMFDescriptorBuffer::NewL();
    CleanupStack::PushL( emptyBuffer );
    MCC_EUNIT_ASSERT_NO_LEAVE( iSinkitem->BufferFilledL( emptyBuffer, &rtpHeader, EFalse, KNullUid ) );
    EUNIT_ASSERT( iSinkitem->iBadDataCount == 0);
    CleanupStack::PopAndDestroy( emptyBuffer );

    CMMFDescriptorBuffer* filledBuffer = CMMFDescriptorBuffer::NewL( 10 );
    CleanupStack::PushL( filledBuffer );
    
    MCC_EUNIT_ASSERT_NO_LEAVE( iSinkitem->FillBufferRequestL( filledBuffer ) );
    EUNIT_ASSERT( iSinkitem->iRequestedBuffer );
   
    iSinkitem->iPassAllBuffersSink = EFalse;
    iSinkitem->SetSourceMode( ETrue );
    MCC_EUNIT_ASSERT_NO_LEAVE( iSinkitem->FillBufferRequestL( buffer ) );
    EUNIT_ASSERT( iSinkitem->iRequestedBuffer );
    MCC_EUNIT_ASSERT_NO_LEAVE( iSinkitem->BufferFilledL( buffer, &rtpHeader, EFalse, KNullUid ) );
    EUNIT_ASSERT( !iSinkitem->iRequestedBuffer );
 
    CleanupStack::PopAndDestroy( filledBuffer );
    CleanupStack::PopAndDestroy( buffer );
    }

void UT_CMccSinkItem::UT_CMccSinkItem_BufferFilledL2L()
    {
    CMMFDescriptorBuffer* buffer = CMMFDescriptorBuffer::NewL( 10 );
    CleanupStack::PushL( buffer );
    TRtpRecvHeader rtpHeader;

    iSinkitem->iBadDataCount = 20;
    EUNIT_ASSERT( iSinkitem->SourceThreadLogon( *iEventHandler ) == KErrNone );

    CMMFDescriptorBuffer* emptyBuffer = CMMFDescriptorBuffer::NewL();
    CleanupStack::PushL( emptyBuffer );
    MCC_EUNIT_ASSERT_NO_LEAVE( iSinkitem->BufferFilledL( emptyBuffer, &rtpHeader, EFalse, KNullUid ) );
    EUNIT_ASSERT( iSinkitem->iBadDataCount == 0);
    CleanupStack::PopAndDestroy( emptyBuffer );

    CMMFDescriptorBuffer* filledBuffer = CMMFDescriptorBuffer::NewL( 10 );
    CleanupStack::PushL( filledBuffer );
    
    iSinkitem->iPassAllBuffersSink = EFalse;
    iSinkitem->SetSourceMode( EFalse );
    MCC_EUNIT_ASSERT_NO_LEAVE( iSinkitem->BufferFilledL( buffer, &rtpHeader, ETrue, KMediaDecoder ) );
    EUNIT_ASSERT( !iSinkitem->iRequestedBuffer );

    MCC_EUNIT_ASSERT_NO_LEAVE( iSinkitem->FillBufferRequestL( buffer ) );
    EUNIT_ASSERT( iSinkitem->iRequestedBuffer );
    
    MCC_EUNIT_ASSERT_NO_LEAVE( iSinkitem->BufferFilledL( buffer, &rtpHeader, ETrue, KNullUid ) );
    EUNIT_ASSERT( !iSinkitem->iRequestedBuffer );
    
    CleanupStack::PopAndDestroy( filledBuffer );
    CleanupStack::PopAndDestroy( buffer );
    }
    
void UT_CMccSinkItem::UT_CMccSinkItem_BufferFilledL3L()
    {
    CMMFDescriptorBuffer* buffer = CMMFDescriptorBuffer::NewL( 10 );
    CleanupStack::PushL( buffer );
    TRtpRecvHeader rtpHeader;
    
    iSinkitem->iPassAllBuffersSink = NULL;
    iSinkitem->iPassAllRequestsSource = NULL;
    iSinkitem->iRequestedBuffer = NULL;


    MCC_EUNIT_ASSERT_NO_LEAVE( iSinkitem->FillBufferRequestL( buffer ) );
    EUNIT_ASSERT( iSinkitem->iRequestedBuffer );
    
    MCC_EUNIT_ASSERT_NO_LEAVE( iSinkitem->BufferFilledL( buffer, &rtpHeader, EFalse, KNullUid ) );
    EUNIT_ASSERT( !iSinkitem->iRequestedBuffer );
    
    MCC_EUNIT_ASSERT_NO_LEAVE( iSinkitem->BufferFilledL( buffer, &rtpHeader, EFalse, KNullUid ) );
    EUNIT_ASSERT( iSinkitem->iStoredBuffers[0] );
    
    
    CleanupStack::PopAndDestroy( buffer );
    }
    
void UT_CMccSinkItem::UT_CMccSinkItem_FillBufferRequestLL(  )
    {
    CMMFDescriptorBuffer* buffer = CMMFDescriptorBuffer::NewL( 10 );
    CleanupStack::PushL( buffer );
    
    // if ( !iStoredBuffer )
    EUNIT_ASSERT_NO_LEAVE( iSinkitem->FillBufferRequestL( buffer ) );
    EUNIT_ASSERT( iSinkitem->iRequestedBuffer );
    
    // if ( iStoredBuffer )
    CMMFDescriptorBuffer* newBuffer = CMMFDescriptorBuffer::NewL( 10 );
    CleanupStack::PushL( newBuffer );
    MCC_EUNIT_ASSERT_EQUALS( iSinkitem->iStoredBuffers.Append( newBuffer ), KErrNone ); 
    CleanupStack::Pop( newBuffer );
    MCC_EUNIT_ASSERT_NO_LEAVE( iSinkitem->FillBufferRequestL( buffer ) );

    CleanupStack::PopAndDestroy( buffer );
    }

void UT_CMccSinkItem::UT_CMccSinkItem_Match2L(  )
    {
    EUNIT_ASSERT( iSinkitem->Match( iRtpDataSinkStub ) );
    }

void UT_CMccSinkItem::UT_CMccSinkItem_MatchL(  )
    {
    EUNIT_ASSERT( iSinkitem->Match( iRead ) );
    }
    
void UT_CMccSinkItem::UT_CMccSinkItem_StoreBufferL()
    {
    CMMFDescriptorBuffer* newBuffer = CMMFDescriptorBuffer::NewL( 10 );
    CleanupStack::PushL( newBuffer );  
    MCC_EUNIT_ASSERT_NO_LEAVE( iSinkitem->StoreBufferL( newBuffer ) );
    CleanupStack::PopAndDestroy( newBuffer );
    }

void UT_CMccSinkItem::UT_CMccSinkItem_GetStoredBufferL()
    {
    CMMFBuffer* bufferGot = NULL;
    bufferGot = iSinkitem->GetStoredBuffer();
    EUNIT_ASSERT( !bufferGot );
    CMMFDescriptorBuffer* newBuffer = CMMFDescriptorBuffer::NewL( 10 );
    CleanupStack::PushL( newBuffer );  
    MCC_EUNIT_ASSERT_NO_LEAVE( iSinkitem->StoreBufferL( newBuffer ) );
    CleanupStack::PopAndDestroy( newBuffer ); 
    
    bufferGot = iSinkitem->GetStoredBuffer();
    CleanupStack::PushL( bufferGot );
    EUNIT_ASSERT( bufferGot );
    CleanupStack::PopAndDestroy( bufferGot ); 
    }

void UT_CMccSinkItem::UT_CMccSinkItem_SourceThreadLogonL(  )
    {
    EUNIT_ASSERT( iSinkitem->SourceThreadLogon( *iEventHandler ) == KErrNone );
    }

void UT_CMccSinkItem::UT_CMccSinkItem_SendEventToClientL(  )
    {  
    TMccInternalEvent internalEvent;
    
    // ieventhandler is NULL, drop event
    EUNIT_ASSERT_EQUALS( iSinkitem->SendEventToClient( internalEvent ), KErrNotReady );
    
    EUNIT_ASSERT( iSinkitem->SourceThreadLogon(*iEventHandler) == KErrNone );
    
    // if ( iEventHandler && sendEvent )
    EUNIT_ASSERT_EQUALS( iSinkitem->SendEventToClient( internalEvent ), KErrNone );
      
    // IS_MCC_EVENT ETrue
    internalEvent.iEventType = KUidMediaTypeAudio; 
    TMccEvent mccEvent;
  
    // iMccEvent != NULL && MCC_STREAM_STATE_CHANGE_EVENT ETrue
    mccEvent.iEventCategory = KMccEventCategoryStream;
    mccEvent.iEventType = KMccStreamPrepared;
    // !iMccEvent.iErrorCode
    mccEvent.iErrorCode = KErrNone;
    mccEvent.iEventData.Copy( _L8("foofoo") );
    internalEvent.iMccEvent = &mccEvent;
    
    EUNIT_ASSERT_EQUALS( iSinkitem->SendEventToClient( internalEvent ), KErrNone );
    EUNIT_ASSERT_EQUALS( iEventHandler->iLastEvent.iEventType, KMccStreamPrepared );
    
    // Automatic stream state change event is not reported because user
    // hasn't started source yet
    mccEvent.iEventType = KMccStreamStarted;
    // !iMccEvent.iErrorCode
    mccEvent.iEventNumData = KMccAutomaticEvent;
    internalEvent.iMccEvent = &mccEvent;
    
    EUNIT_ASSERT_EQUALS( iSinkitem->SendEventToClient( internalEvent ), KErrNone );
     // Still prepared as started event was ignored
    EUNIT_ASSERT_EQUALS( iEventHandler->iLastEvent.iEventType, KMccStreamPrepared );
    }

void UT_CMccSinkItem::UT_CMccSinkItem_MediaTypeL(  )
    {
    EUNIT_ASSERT( iSinkitem->MediaType() == KMediaDecoder );
    }

void UT_CMccSinkItem::UT_CMccSinkItem_SetSourceModeL(  )
    {
    iSinkitem->SetSourceMode( ETrue );
    EUNIT_ASSERT( iSinkitem->PassAllRequestsToSource() );
    }

void UT_CMccSinkItem::UT_CMccSinkItem_PassAllRequestsToSourceL(  )
    {
    iSinkitem->SetSourceMode( ETrue );
    EUNIT_ASSERT( iSinkitem->PassAllRequestsToSource() );
    iSinkitem->SetSourceMode( EFalse );
    EUNIT_ASSERT( !iSinkitem->PassAllRequestsToSource() );
    }

void UT_CMccSinkItem::UT_CMccSinkItem_ResolveMediaTypeL(  )
    {
    CMMFDescriptorBuffer* buffer = NULL;
    // aBuffer == NULL
    EUNIT_ASSERT( iSinkitem->ResolveMediaType( buffer ) == KNullUid );
    
    buffer = CMMFDescriptorBuffer::NewL( 10 );
    CleanupStack::PushL( buffer );
    //iPassAllRequestsSource == EFalse
    EUNIT_ASSERT( iSinkitem->ResolveMediaType( buffer ) == KNullUid );
    iSinkitem->SetSourceMode( ETrue );
 
    CMMFDescriptorBuffer* secBuffer = CMMFDescriptorBuffer::NewL( 10 );
    CleanupStack::PushL( secBuffer );
    // iRequestedBuffer != aBuffer
    EUNIT_ASSERT( iSinkitem->ResolveMediaType( secBuffer ) == KNullUid );
    CleanupStack::PopAndDestroy( secBuffer );
    secBuffer = NULL;

    EUNIT_ASSERT_NO_LEAVE( iSinkitem->FillBufferRequestL( buffer ) );
    EUNIT_ASSERT( iSinkitem->ResolveMediaType( buffer ) == KMediaDecoder );

    CleanupStack::PopAndDestroy( buffer );
    buffer = NULL;   
    }

void UT_CMccSinkItem::UT_CMccSinkItem_RegisterPayloadTypesLL()
    {
    RArray< TUint > payloads;
    CleanupClosePushL( payloads );
    payloads.AppendL( 96 );
    payloads.AppendL( 97 );

    MCC_EUNIT_ASSERT_NO_LEAVE( iSinkitem->RegisterPayloadTypesL( payloads ) );
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( 
        iSinkitem->RegisterPayloadTypesL( payloads ), KErrAlreadyExists );

    CleanupStack::PopAndDestroy( &payloads );
    }

void UT_CMccSinkItem::UT_CMccSinkItem_RegisteredPayloadTypesL()
    {
    RArray< TUint > payloads;
    CleanupClosePushL( payloads );
    payloads.AppendL( 96 );
    payloads.AppendL( 97 );

    MCC_EUNIT_ASSERT_NO_LEAVE( iSinkitem->RegisterPayloadTypesL( payloads ) );
    
    const RArray< TUint >& payloadsRef = iSinkitem->RegisteredPayloadTypes();
    EUNIT_ASSERT( payloadsRef.Count() == 2 );

    CleanupStack::PopAndDestroy( &payloads );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccSinkItem,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "Sink - test ",
    "CMccSinkItem",
    "Sink",
    "FUNCTIONALITY",
    SetupL, UT_CMccSinkItem_SinkL, Teardown )
    
EUNIT_TEST(
    "Sink2 - test ",
    "CMccSinkItem",
    "Sink2",
    "FUNCTIONALITY",
    Setup2L, UT_CMccSinkItem_Sink2L, Teardown2)

   
   
EUNIT_TEST(
    "CopyBuffer - test ",
    "CMccSinkItem",
    "CopyBuffer",
    "FUNCTIONALITY",
    SetupL, UT_CMccSinkItem_CopyBufferL, Teardown)
      

EUNIT_TEST(
    "IsInternalDecoder - test ",
    "CMccSinkItem",
    "IsInternalDecoder",
    "FUNCTIONALITY",
    SetupL, UT_CMccSinkItem_IsInternalDecoderL, Teardown)

EUNIT_TEST(
    "BufferFilledL - test ",
    "CMccSinkItem",
    "BufferFilledL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSinkItem_BufferFilledLL, Teardown)

EUNIT_TEST(
    "BufferFilledL2 - test ",
    "CMccSinkItem",
    "BufferFilledL2",
    "FUNCTIONALITY",
    SetupL, UT_CMccSinkItem_BufferFilledL2L, Teardown)

EUNIT_TEST(
    "BufferFilledL3 - test ",
    "CMccSinkItem",
    "BufferFilledL3",
    "FUNCTIONALITY",
    SetupL, UT_CMccSinkItem_BufferFilledL3L, Teardown)

EUNIT_TEST(
    "FillBufferRequestL - test ",
    "CMccSinkItem",
    "FillBufferRequestL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSinkItem_FillBufferRequestLL, Teardown)

EUNIT_TEST(
    "Match - test ",
    "CMccSinkItem",
    "Match",
    "FUNCTIONALITY",
    SetupL, UT_CMccSinkItem_MatchL, Teardown)

EUNIT_TEST(
    "Match - test ",
    "CMccSinkItem",
    "Match",
    "FUNCTIONALITY",
    Setup2L, UT_CMccSinkItem_Match2L, Teardown2)

EUNIT_TEST(
    "StoreBufferL - test ",
    "CMccSinkItem",
    "StoreBufferL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccSinkItem_StoreBufferL, Teardown2)

 
EUNIT_TEST(
    "GetStoredBufferL - test ",
    "CMccSinkItem",
    "GetStoredBufferL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccSinkItem_GetStoredBufferL, Teardown2)

         
EUNIT_TEST(
    "SourceThreadLogon - test ",
    "CMccSinkItem",
    "SourceThreadLogon",
    "FUNCTIONALITY",
    SetupL, UT_CMccSinkItem_SourceThreadLogonL, Teardown)

EUNIT_TEST(
    "SendEventToClient - test ",
    "CMccSinkItem",
    "SendEventToClient",
    "FUNCTIONALITY",
    SetupL, UT_CMccSinkItem_SendEventToClientL, Teardown)

EUNIT_TEST(
    "MediaType - test ",
    "CMccSinkItem",
    "MediaType",
    "FUNCTIONALITY",
    Setup2L, UT_CMccSinkItem_MediaTypeL, Teardown2)

EUNIT_TEST(
    "SetSourceMode - test ",
    "CMccSinkItem",
    "SetSourceMode",
    "FUNCTIONALITY",
    SetupL, UT_CMccSinkItem_SetSourceModeL, Teardown)

EUNIT_TEST(
    "PassAllRequestsToSource - test ",
    "CMccSinkItem",
    "PassAllRequestsToSource",
    "FUNCTIONALITY",
    SetupL, UT_CMccSinkItem_PassAllRequestsToSourceL, Teardown)

EUNIT_TEST(
    "ResolveMediaType - test ",
    "CMccSinkItem",
    "ResolveMediaType",
    "FUNCTIONALITY",
    Setup2L, UT_CMccSinkItem_ResolveMediaTypeL, Teardown2)

EUNIT_TEST(
    "RegisterPayloadTypesL - test ",
    "CMccSinkItem",
    "RegisterPayloadTypesL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSinkItem_RegisterPayloadTypesLL, Teardown )

EUNIT_TEST(
    "RegisteredPayloadTypes - test ",
    "CMccSinkItem",
    "RegisteredPayloadTypes",
    "FUNCTIONALITY",
    SetupL, UT_CMccSinkItem_RegisteredPayloadTypesL, Teardown )

EUNIT_END_TEST_TABLE

//  END OF FILE
