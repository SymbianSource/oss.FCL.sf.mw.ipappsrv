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
#include "UT_CDTMFPayloadFormatRead.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>

//  INTERNAL INCLUDES
#include "rtpheader.h"
#include "mccrtpdatasource.h"
#include "dtmfpayloadformatread.h"
#include "dtmfpayloaddecoder.h"
#include "mccrtpdatasink.h"
#include "mccuids.hrh"
#include "mccdef.h"
#include "dtmfeventpayloadinfo.h"
#include "mccmultiplexer.h"

const TUid KRtpDataSource = { KImplUidRtpDataSource }; 

// CONSTRUCTION
UT_CDTMFPayloadFormatRead* UT_CDTMFPayloadFormatRead::NewL()
    {
    UT_CDTMFPayloadFormatRead* self = UT_CDTMFPayloadFormatRead::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CDTMFPayloadFormatRead* UT_CDTMFPayloadFormatRead::NewLC()
    {
    UT_CDTMFPayloadFormatRead* self = new( ELeave ) UT_CDTMFPayloadFormatRead();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CDTMFPayloadFormatRead::~UT_CDTMFPayloadFormatRead()
    {
    }

// Default constructor
UT_CDTMFPayloadFormatRead::UT_CDTMFPayloadFormatRead()
    {
    }

// Second phase construct
void UT_CDTMFPayloadFormatRead::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    iFourCC.Set( TFourCC( 'D','T','M','F' ) );
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS
void UT_CDTMFPayloadFormatRead::SetupL()
    {
    iRtpDataSourceStub = CMccRtpDataSource::NewSourceL( KRtpDataSource, KNullDesC8 );
    iRead = CDTMFPayloadFormatRead::NewL( iRtpDataSourceStub );
    iBuffi = CMMFDescriptorBuffer::NewL( 100 );
    } 

void UT_CDTMFPayloadFormatRead::Teardown()
    {
    delete iBuffi;
    iBuffi = NULL;

    delete iRtpDataSourceStub;
    iRtpDataSourceStub = NULL;
    
    delete iRead;
    iRead = NULL;
    
    REComSession::FinalClose();
    }

void UT_CDTMFPayloadFormatRead::UT_CDTMFPayloadFormatRead_HandleError()
    {
    EUNIT_ASSERT( iRead->HandleError( KErrNotSupported, EDecodeIdle ) == KErrNotSupported )
    EUNIT_ASSERT( iRead->HandleError( KErrGeneral, EDecodeIdle ) == KErrGeneral )
    }

void UT_CDTMFPayloadFormatRead::UT_CDTMFPayloadFormatRead_PayloadFormatL()
    {
    EUNIT_ASSERT( iRead->SetPayloadFormat( EDTMFPayloadFormatEvent ) == KErrNone )
    EUNIT_ASSERT( iRead->SetPayloadFormat( EDTMFPayloadFormatTone ) == KErrNone )
    EUNIT_ASSERT( iRead->SetPayloadFormat( EDTMFPayloadFormatEvent ) == KErrNone )
    EUNIT_ASSERT( iRead->SetPayloadFormat( EDTMFPayloadFormatNotDefined ) == KErrNotSupported )
    EUNIT_ASSERT( iRead->SetPayloadFormat( (TDTMFPayloadFormat)1000 ) == KErrNotSupported )
    }

void UT_CDTMFPayloadFormatRead::UT_CDTMFPayloadFormatRead_SetPayloadFormatL()
    {
    TDTMFPayloadFormat nullData( EDTMFPayloadFormatNotDefined );
    EUNIT_ASSERT_EQUALS( iRead->SetPayloadFormat( nullData ), KErrNotSupported );
    
    TDTMFPayloadFormat validData( EDTMFPayloadFormatTone );
    EUNIT_ASSERT_EQUALS( iRead->SetPayloadFormat( validData ), KErrNone );
    }

void UT_CDTMFPayloadFormatRead::UT_CDTMFPayloadFormatRead_BufferFilledLL()
    {
    TMccCodecInfoBuffer buf( iCodec );
    iRead->ConfigurePayloadFormatL( buf );
    iRead->FillSourceBufferL( );
    
    iHeaderInfo.iPayloadType = 102;
    iRead->iCInfo.iPayloadType = 102;
        
    CMMFDataBuffer* buffer = CMMFDataBuffer::NewL( KDTMFDefaultPayloadSize );
    CleanupStack::PushL( buffer );
    buffer->Data().FillZ( KDTMFDefaultPayloadSize );
    iRead->DataBufferFilledL( buffer, iHeaderInfo );
        
    CleanupStack::PopAndDestroy( buffer );
    
    buffer = NULL;
    buffer = CMMFDataBuffer::NewL( 0 );
    CleanupStack::PushL( buffer );
    iRead->DataBufferFilledL( buffer, iHeaderInfo );
    
    CleanupStack::PopAndDestroy( buffer );
    }

void UT_CDTMFPayloadFormatRead::UT_CDTMFPayloadFormatRead_CanCreateSourceBufferL(  )
    {
    EUNIT_ASSERT_EQUALS( iRead->CanCreateSourceBuffer( ), EFalse );
    }

void UT_CDTMFPayloadFormatRead::UT_CDTMFPayloadFormatRead_StreamsL(  )
    {
    EUNIT_ASSERT_EQUALS( iRead->Streams( KUidMediaTypeAudio ), 1 );
    EUNIT_ASSERT_EQUALS( iRead->Streams( KUidMediaTypeVideo ), 0 );
    }

void UT_CDTMFPayloadFormatRead::UT_CDTMFPayloadFormatRead_FrameTimeIntervalL(  )
    {
    TInt64 interval = iRead->FrameTimeInterval( KUidMediaTypeAudio ).Int64();
    RDebug::Print( _L( "Frametime Interval value = %d" ), interval );
    
    EUNIT_ASSERT_EQUALS( iRead->FrameTimeInterval( KUidMediaTypeAudio ).Int64(), interval );
    EUNIT_ASSERT_EQUALS( iRead->FrameTimeInterval( KUidMediaTypeVideo ).Int64(), 0 );
    }

void UT_CDTMFPayloadFormatRead::UT_CDTMFPayloadFormatRead_DurationL(  )
    {
    TInt64 duration = iRead->Duration( KUidMediaTypeAudio ).Int64();
    RDebug::Print( _L( "Duration value = %d" ), duration );
    EUNIT_ASSERT_EQUALS( iRead->Duration( KUidMediaTypeAudio ).Int64(), duration );
    }

void UT_CDTMFPayloadFormatRead::UT_CDTMFPayloadFormatRead_FillBufferLL(  )
    {
    EUNIT_ASSERT_LEAVE( iRead->FillBufferL( iBuffi, NULL, KUidMediaTypeAudio ) );
    }

void UT_CDTMFPayloadFormatRead::UT_CDTMFPayloadFormatRead_CreateSourceBufferLL(  )
    {
    TBool ref( EFalse );
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
        iRead->CreateSourceBufferL( KUidMediaTypeAudio, ref ), KErrNotSupported );
    }    

void UT_CDTMFPayloadFormatRead::UT_CDTMFPayloadFormatRead_SourceDataTypeCodeL(  )
    {
    EUNIT_ASSERT_EQUALS( iRead->SourceDataTypeCode( KUidMediaTypeAudio ), TFourCC( KMccFourCCIdDTMF ) );
    EUNIT_ASSERT_EQUALS( iRead->SourceDataTypeCode( KUidMediaTypeVideo ), TFourCC() );
    }


void UT_CDTMFPayloadFormatRead::UT_CDTMFPayloadFormatRead_SourcePrimeLL(  )
    {
    EUNIT_ASSERT_NO_LEAVE( iRead->SourcePrimeL() );    
    }

void UT_CDTMFPayloadFormatRead::UT_CDTMFPayloadFormatRead_SourcePlayLL(  )
    {
    EUNIT_ASSERT_NO_LEAVE( iRead->SourcePlayL() );
    }

void UT_CDTMFPayloadFormatRead::UT_CDTMFPayloadFormatRead_SourcePauseLL(  )
    {
    EUNIT_ASSERT_NO_LEAVE( iRead->SourcePauseL() );
    }

void UT_CDTMFPayloadFormatRead::UT_CDTMFPayloadFormatRead_SourceStopLL(  )
    {
    iRead->SourcePlayL();
    EUNIT_ASSERT_NO_LEAVE( iRead->SourceStopL() );
    }

void UT_CDTMFPayloadFormatRead::UT_CDTMFPayloadFormatRead_SourceThreadLogonL(  )
    {
    EUNIT_ASSERT_EQUALS( iRead->SourceThreadLogon( *iEventH ), KErrNone );
    }

void UT_CDTMFPayloadFormatRead::UT_CDTMFPayloadFormatRead_SourceThreadLogoffL(  )
    {
    EUNIT_ASSERT_NO_LEAVE( iRead->SourceThreadLogoff( ) );
    }

void UT_CDTMFPayloadFormatRead::UT_CDTMFPayloadFormatRead_HandleDTMFEventsL()
    {
    TDTMFEventPayloadInfo payloadinfo;
 
    payloadinfo.SetEvent( '7' );
    payloadinfo.SetEndBit( 1 );
    payloadinfo.SetVolume( 4 );
    payloadinfo.SetDuration( 400 );
    
    EUNIT_ASSERT_NO_LEAVE( iRead->HandleDTMFEventsL() );
    
    User::LeaveIfError( iRead->iEventPayloadInfo.Append( payloadinfo ) );
    EUNIT_ASSERT_NO_LEAVE( iRead->HandleDTMFEventsL() );
    
    iRead->iEventPayloadInfo.Reset();
    
    payloadinfo.SetEvent( '7' );
    payloadinfo.SetEndBit( 0 );
    payloadinfo.SetVolume( 4 );
    payloadinfo.SetDuration( 400 );
    
    iRead->iFirstPacket = ETrue;
    User::LeaveIfError( iRead->iEventPayloadInfo.Append( payloadinfo ) );
    EUNIT_ASSERT_NO_LEAVE( iRead->HandleDTMFEventsL() );
    
    iRead->iEventPayloadInfo.Reset();
    
    payloadinfo.SetEvent( '7' );
    payloadinfo.SetEndBit( 0 );
    payloadinfo.SetVolume( 4 );
    payloadinfo.SetDuration( 400 );
    payloadinfo.SetTimeStamp(  TTimeIntervalMicroSeconds32 (1000) );
    iRead->iFirstPacket = EFalse;
    
    iRead->iCurrentEvent = '7';
    iRead->iTimeStampOfPrevEvent = 1000;
    User::LeaveIfError( iRead->iEventPayloadInfo.Append( payloadinfo ) );
    EUNIT_ASSERT_NO_LEAVE( iRead->HandleDTMFEventsL() );
    
    iRead->iCurrentEvent = '8';
    iRead->iTimeStampOfPrevEvent = 2000;
    iRead->iTonePlayer->CancelPrepare();
    EUNIT_ASSERT_NO_LEAVE( iRead->HandleDTMFEventsL() );

    payloadinfo.SetEvent( '8' );
    payloadinfo.SetEndBit( 0 );
    payloadinfo.SetVolume( 8 );
    payloadinfo.SetDuration( 800 );
    
    User::LeaveIfError( iRead->iEventPayloadInfo.Append( payloadinfo ) );
    iRead->iTonePlayer->CancelPrepare();
    EUNIT_ASSERT_NO_LEAVE( iRead->HandleDTMFEventsL() );
    }

void UT_CDTMFPayloadFormatRead::UT_CDTMFPayloadFormatRead_HandleDTMFTones()
    {
    TDTMFEventPayloadInfo payloadinfo;
 
    payloadinfo.SetEvent( '7' );
    payloadinfo.SetEndBit( 1 );
    payloadinfo.SetVolume( 4 );
    payloadinfo.SetDuration( 400 );
    
    iRead->HandleDTMFTones();
    
    User::LeaveIfError( iRead->iEventPayloadInfo.Append( payloadinfo ) );
    iRead->HandleDTMFTones();
    }

void UT_CDTMFPayloadFormatRead::UT_CDTMFPayloadFormatRead_SendDataToSinkLL(  )
    {
    // case #1
    iRead->iBufferToReadExists = ETrue;
    iRead->SetPayloadFormat( EDTMFPayloadFormatEvent );
    EUNIT_ASSERT_NO_LEAVE( iRead->SendDataToSinkL( ) );
    
    // case #2
    iRead->iBufferToReadExists = ETrue;
    iRead->SetPayloadFormat( EDTMFPayloadFormatTone );
    EUNIT_ASSERT_NO_LEAVE( iRead->SendDataToSinkL( ) );
    
    iRead->iBufferToReadExists = ETrue;
    iRead->SetPayloadFormat( EDTMFPayloadFormatRedEvents );
    EUNIT_ASSERT_NO_LEAVE( iRead->SendDataToSinkL( ) );
    
    // case #3
    // Expected result is same than in case #2 because of SetPayloadFormat-method
    iRead->iBufferToReadExists = ETrue;
    iRead->SetPayloadFormat( EDTMFPayloadFormatNotDefined );
    EUNIT_ASSERT_NO_LEAVE( iRead->SendDataToSinkL( ) );
    
    // case #4
    iRead->iBufferToReadExists = EFalse;
    EUNIT_ASSERT_NO_LEAVE( iRead->SendDataToSinkL( ) );
    }

void UT_CDTMFPayloadFormatRead::UT_CDTMFPayloadFormatRead_FillSinkBufferL(  )
    {
    // case #1
    iRead->iBufferToReadExists = ETrue;
    iRead->SetPayloadFormat( EDTMFPayloadFormatEvent );
    EUNIT_ASSERT_NO_LEAVE( iRead->FillSinkBufferL() );
    
    // case #2
    iRead->iBufferToReadExists = ETrue;
    iRead->SetPayloadFormat( EDTMFPayloadFormatRedEvents );
    EUNIT_ASSERT_NO_LEAVE( iRead->FillSinkBufferL() );
    
    // case #3
    iRead->iBufferToReadExists = ETrue;
    iRead->SetPayloadFormat( EDTMFPayloadFormatTone );
    EUNIT_ASSERT_NO_LEAVE( iRead->FillSinkBufferL() );
    
    // case #4
    iRead->iBufferToReadExists = ETrue;
    iRead->SetPayloadFormat( EDTMFPayloadFormatNotDefined );
    EUNIT_ASSERT_NO_LEAVE( iRead->FillSinkBufferL() );
    
    // case #5
    iRead->iBufferToReadExists = EFalse;
    EUNIT_ASSERT_NO_LEAVE( iRead->FillSinkBufferL() ); 
    }

void UT_CDTMFPayloadFormatRead::UT_CDTMFPayloadFormatRead_FillSourceBufferLL(  )
    {
    TMccCodecInfoBuffer buffer( iCodec );
    iRead->ConfigurePayloadFormatL( buffer );
    iRead->FillSourceBufferL();
    }
       
void UT_CDTMFPayloadFormatRead::UT_CDTMFPayloadFormatRead_SinkDataTypeCodeL( ) 
    {
    EUNIT_ASSERT_NO_LEAVE( iRead->SinkDataTypeCode( KUidMediaTypeAudio ) );
    EUNIT_ASSERT_NO_LEAVE( iRead->SinkDataTypeCode( KUidMediaTypeVideo ) );
    }

void UT_CDTMFPayloadFormatRead::UT_CDTMFPayloadFormatRead_SetSourcePrioritySettings(  )
    {
    TMMFPrioritySettings settings;
    EUNIT_ASSERT_NO_LEAVE( iRead->SetSourcePrioritySettings( settings ) );
    }

void UT_CDTMFPayloadFormatRead::UT_CDTMFPayloadFormatRead_MatoPrepareCompleteL( )
    {
    EUNIT_ASSERT_NO_LEAVE( iRead->MatoPrepareComplete(KErrNone) );
    EUNIT_ASSERT_NO_LEAVE( iRead->MatoPrepareComplete( KErrGeneral ) );
    }
    
void UT_CDTMFPayloadFormatRead::UT_CDTMFPayloadFormatRead_MatoPlayCompleteL( )
    {
    EUNIT_ASSERT_NO_LEAVE( iRead->MatoPlayComplete(KErrNone) );
    EUNIT_ASSERT_NO_LEAVE( iRead->MatoPlayComplete(KErrGeneral) );
    }

void UT_CDTMFPayloadFormatRead::UT_CDTMFPayloadFormatRead_ConfigurePayloadFormatLL(  )
    {
    TBuf8<10> buf;
    EUNIT_ASSERT_SPECIFIC_LEAVE( iRead->ConfigurePayloadFormatL( buf ), KErrArgument );
    
    TMccCodecInfo codec;
    codec.iFourCC.Set( TFourCC( 'D','T','M','F' ) );
    codec.iBitrate = 12200;
    codec.iCodecMode = KDtmfModeEvent;
    codec.iPtime = 20;
    codec.iMaxPtime = 200;
    codec.iPayloadType = 96;
    codec.iRedundancyCount = 0;
    codec.iEnableDTX = EFalse;
    codec.iRedundantPayload = 0;
    codec.iHwFrameTime = 0;
    codec.iAlgoUsed = 0;
    
    TMccCodecInfoBuffer buffer( codec );
    iRead->ConfigurePayloadFormatL( buffer );
    
    delete iRead->iPayloadDecoder;
    iRead->iPayloadDecoder = NULL;
    iRead->ConfigurePayloadFormatL( buffer );
    
    codec.iAlgoUsed = 2;
    codec.iRedundancyCount = 1;
    codec.iRedundantPayload = 99;
    codec.iHwFrameTime = 1;
    buffer = codec; 
    
    iRead->ConfigurePayloadFormatL( buffer );
    
    codec.iAlgoUsed = 1;
    codec.iRedundancyCount = 3;
    codec.iRedundantPayload = 99;
    codec.iHwFrameTime = 1;
    buffer = codec; 
    iRead->iCInfo.iAlgoUsed = 1;
    
    TRAPD( err, iRead->ConfigurePayloadFormatL( buffer ) )
    if ( err == KErrNoMemory )
        {
        User::Leave( err );
        }
    
    EUNIT_ASSERT_EQUALS( KErrArgument, err );
    
    codec.iAlgoUsed = 2;
    codec.iRedundancyCount = 3;
    codec.iRedundantPayload = 99;
    codec.iHwFrameTime = 1;
    iRead->iCInfo.iAlgoUsed = 2;
    buffer = codec; 
    
    iRead->ConfigurePayloadFormatL( buffer );
    
    buffer = codec; 
    CDTMFPayloadFormatRead* read = CDTMFPayloadFormatRead::NewL( iRtpDataSourceStub );
    CleanupStack::PushL( read );
    read->ConfigurePayloadFormatL( buffer );
    CleanupStack::PopAndDestroy( read );
    }

void UT_CDTMFPayloadFormatRead::UT_CDTMFPayloadFormatRead_CreateClipBufferLL()
    {
    iRead->SourceThreadLogon( *this );
    iRead->CreateClipBufferL();
    iRead->CreateClipBufferL();
    iRead->SourceThreadLogoff();
    
    CMccMultiplexer* mplx = CMccMultiplexer::NewL( iRtpDataSourceStub );
    CleanupStack::PushL( mplx );
    
    CDTMFPayloadFormatRead* read = CDTMFPayloadFormatRead::NewL( mplx );
    CleanupStack::PushL( read );
    read->SourceThreadLogon( *this );
    
    User::LeaveIfError( 
        mplx->MultiplexerSourceThreadLogon(
            *this, *read, ETrue, KUidMediaTypeAudio, EFalse ) );
    
    read->CreateClipBufferL();
    read->CreateClipBufferL();
    
    CMMFDescriptorBuffer* desBuf =
        static_cast<CMMFDescriptorBuffer*>( read->iSourceBuffer );
    desBuf->ReAllocBufferL( 90 );
    desBuf = NULL;
    
    read->CreateClipBufferL();
    read->SourceThreadLogoff();
    
    CleanupStack::PopAndDestroy( read );
    CleanupStack::PopAndDestroy( mplx );
    }

void UT_CDTMFPayloadFormatRead::UT_CDTMFPayloadFormatRead_CompareFirstPacketRtpHeaders()
    {
    TRtpRecvHeader header;
    header.iMarker = 1;
    header.iSeqNum = 2;
    header.iTimestamp = 100;
    
    iRead->iReceivedHeader.iMarker = 1;
    iRead->iReceivedHeader.iSeqNum = 1;
    iRead->iReceivedHeader.iTimestamp = 100;
    
    EUNIT_ASSERT( iRead->CompareFirstPacketRtpHeaders( header ) );
    EUNIT_ASSERT( !( iRead->CompareFirstPacketRtpHeaders( header ) ) );
    header.iSeqNum = 3;
    EUNIT_ASSERT( iRead->CompareFirstPacketRtpHeaders( header ) );
    }
    
//  TEST TABLE
EUNIT_BEGIN_TEST_TABLE( 
    UT_CDTMFPayloadFormatRead,
    "MCC DTMF payloadformat read",
    "UNIT" )

EUNIT_TEST(
    "HandleError - test ",
    "CDTMFPayloadFormatRead",
    "HandleError",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatRead_HandleError, Teardown)

EUNIT_TEST(
    "PayloadFormat - test ",
    "CDTMFPayloadFormatRead",
    "PayloadFormat",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatRead_PayloadFormatL, Teardown)

EUNIT_TEST(
    "SetPayloadFormat - test ",
    "CDTMFPayloadFormatRead",
    "SetPayloadFormat",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatRead_SetPayloadFormatL, Teardown)

EUNIT_TEST(
    "BufferFilledL - test ",
    "CDTMFPayloadFormatRead",
    "BufferFilledL",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatRead_BufferFilledLL, Teardown)

EUNIT_TEST(
    "CanCreateSourceBuffer - test ",
    "CDTMFPayloadFormatRead",
    "CanCreateSourceBuffer",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatRead_CanCreateSourceBufferL, Teardown)

EUNIT_TEST(
    "Streams - test ",
    "CDTMFPayloadFormatRead",
    "Streams",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatRead_StreamsL, Teardown)

EUNIT_TEST(
    "FrameTimeInterval - test ",
    "CDTMFPayloadFormatRead",
    "FrameTimeInterval",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatRead_FrameTimeIntervalL, Teardown)

EUNIT_TEST(
    "Duration - test ",
    "CDTMFPayloadFormatRead",
    "Duration",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatRead_DurationL, Teardown)

EUNIT_TEST(
    "FillBufferL - test ",
    "CDTMFPayloadFormatRead",
    "FillBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatRead_FillBufferLL, Teardown)

EUNIT_TEST(
    "CreateSourceBufferL - test ",
    "CDTMFPayloadFormatRead",
    "CreateSourceBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatRead_CreateSourceBufferLL, Teardown)

EUNIT_TEST(
    "SourceDataTypeCode - test ",
    "CDTMFPayloadFormatRead",
    "SourceDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatRead_SourceDataTypeCodeL, Teardown)


EUNIT_TEST(
    "SourcePrimeL - test ",
    "CDTMFPayloadFormatRead",
    "SourcePrimeL",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatRead_SourcePrimeLL, Teardown)

EUNIT_TEST(
    "SourcePlayL - test ",
    "CDTMFPayloadFormatRead",
    "SourcePlayL",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatRead_SourcePlayLL, Teardown)

EUNIT_TEST(
    "SourcePauseL - test ",
    "CDTMFPayloadFormatRead",
    "SourcePauseL",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatRead_SourcePauseLL, Teardown)

EUNIT_TEST(
    "SourceStopL - test ",
    "CDTMFPayloadFormatRead",
    "SourceStopL",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatRead_SourceStopLL, Teardown)

EUNIT_TEST(
    "SourceThreadLogon - test ",
    "CDTMFPayloadFormatRead",
    "SourceThreadLogon",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatRead_SourceThreadLogonL, Teardown)

EUNIT_TEST(
    "SourceThreadLogoff - test ",
    "CDTMFPayloadFormatRead",
    "SourceThreadLogoff",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatRead_SourceThreadLogoffL, Teardown)

EUNIT_TEST(
    "HandleDTMFEventsL - test ",
    "CDTMFPayloadFormatRead",
    "HandleDTMFEventsL",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatRead_HandleDTMFEventsL, Teardown)


EUNIT_TEST(
    "HandleDTMFTones - test ",
    "CDTMFPayloadFormatRead",
    "HandleDTMFTones",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatRead_HandleDTMFTones, Teardown)

EUNIT_TEST(
    "SendDataToSinkL - test ",
    "CDTMFPayloadFormatRead",
    "SendDataToSinkL",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatRead_SendDataToSinkLL, Teardown)

EUNIT_TEST(
    "FillSinkBuffer - test ",
    "CDTMFPayloadFormatRead",
    "FillSinkBuffer",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatRead_FillSinkBufferL, Teardown)

EUNIT_TEST(
    "FillSourceBufferL - test ",
    "CDTMFPayloadFormatRead",
    "FillSourceBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatRead_FillSourceBufferLL, Teardown)

EUNIT_TEST(
    "SinkDataTypeCode - test ",
    "CDTMFPayloadFormatRead",
    "SinkDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatRead_SinkDataTypeCodeL, Teardown)
    
EUNIT_TEST(
    "SetSourcePrioritySettings - test ",
    "CDTMFPayloadFormatRead",
    "SetSourcePrioritySettings",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatRead_SetSourcePrioritySettings, Teardown)
        
EUNIT_TEST(
    "MatoPrepareComplete - test ",
    "CDTMFPayloadFormatRead",
    "MatoPrepareComplete",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatRead_MatoPrepareCompleteL, Teardown)

EUNIT_TEST(
    "MatoPlayComplete - test ",
    "CDTMFPayloadFormatRead",
    "MatoPlayComplete",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatRead_MatoPlayCompleteL, Teardown)    

EUNIT_TEST(
    "ConfigurePayloadFormatL - test ",
    "CDTMFPayloadFormatRead",
    "ConfigurePayloadFormatL",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatRead_ConfigurePayloadFormatLL, Teardown)

EUNIT_TEST(
    "CreateClipBufferL - test ",
    "CDTMFPayloadFormatRead",
    "CreateClipBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatRead_CreateClipBufferLL, Teardown)

EUNIT_TEST(
    "CompareFirstPacketRtpHeaders - test ",
    "CDTMFPayloadFormatRead",
    "CompareFirstPacketRtpHeaders",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatRead_CompareFirstPacketRtpHeaders, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
