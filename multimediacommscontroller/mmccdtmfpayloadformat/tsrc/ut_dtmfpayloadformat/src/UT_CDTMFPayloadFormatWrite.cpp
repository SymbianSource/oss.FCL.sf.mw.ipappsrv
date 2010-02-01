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
#include "UT_CDTMFPayloadFormatWrite.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <e32base.h>
#include <e32math.h>
#include <mmf/server/mmfdatabuffer.h>



#include <mmf/server/mmfaudiooutput.h>
#include "mmccnetworksettings.h"
#include "mccrtpmediaclock.h"

//  INTERNAL INCLUDES
#include "dtmfpayloadformatwrite.h"
#include "mccrtpdatasink.h"
#include <mmf/server/mmfdatasourcesink.hrh>
#include <mmf/server/mmfdatasink.h>
#include <mmf/server/mmfdatasource.h>
#include "mccuids.hrh"
#include "dtmfpayloadformatdefs.h"
#include "dtmfpayloadencoder.h"
#include "dtmfeventpayloadinfo.h"
#include "dtmftonepayloadinfo.h"
#include "mccinternaldef.h"
#include <digia/eunit/CEunitAllocTestCaseDecorator.h> // Alloc tests enabled
#include <ecom/ecom.h>

const TUid KRtpDataSink   = { KImplUidRtpDataSink };
const TUid KRtpDataSource = { KImplUidRtpDataSource };

// CONSTRUCTION
UT_CDTMFPayloadFormatWrite* UT_CDTMFPayloadFormatWrite::NewL()
    {
    UT_CDTMFPayloadFormatWrite* self = UT_CDTMFPayloadFormatWrite::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CDTMFPayloadFormatWrite* UT_CDTMFPayloadFormatWrite::NewLC()
    {
    UT_CDTMFPayloadFormatWrite* self = new( ELeave ) UT_CDTMFPayloadFormatWrite();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CDTMFPayloadFormatWrite::~UT_CDTMFPayloadFormatWrite()
    {
    }

// Default constructor
UT_CDTMFPayloadFormatWrite::UT_CDTMFPayloadFormatWrite()
    {
    }

// Second phase construct
void UT_CDTMFPayloadFormatWrite::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS
void UT_CDTMFPayloadFormatWrite::SetupL()
    {
    iTones.Zero();
    iTones.Append('*');
    iTones.Append('4');
    iTones.Append('w');
    iTones.Append('4');
    
    iRtpDataSinkStub = CMccRtpDataSink::NewSinkL( KRtpDataSink, KNullDesC8 );
    iWrite = CDTMFPayloadFormatWrite::NewL( iRtpDataSinkStub );
    
    iBuffer = CMMFDescriptorBuffer::NewL( 10 );
    iDataSource = MDataSource::NewSourceL( KRtpDataSource, KNullDesC8 );
    iRtpMediaClock = CMccRtpMediaClock::NewL();
    iEventData = TMccDtmfEventData();
    } 

void UT_CDTMFPayloadFormatWrite::Teardown()
    {
    delete iBuffer;
    iBuffer = NULL;

    delete iDataSource;
    iDataSource = NULL;

    delete iRtpDataSinkStub;
    iRtpDataSinkStub = NULL;

    delete iWrite;
    iWrite = NULL;
    
    delete iRtpMediaClock;
    
    REComSession::FinalClose();
    }

void UT_CDTMFPayloadFormatWrite::UT_CDTMFPayloadFormatWrite_NewLL(  )
    {
    EUNIT_ASSERT( ETrue );
    }

void UT_CDTMFPayloadFormatWrite::UT_CDTMFPayloadFormatWrite_HandleTimerEventL()
    {
    TAny* object = NULL;
    EUNIT_ASSERT( iWrite->HandleTimerEventL( object ) == KErrArgument );
    EUNIT_ASSERT( iWrite->HandleTimerEventL( static_cast<TAny*>( iWrite ) ) != KErrNone );
    }

void UT_CDTMFPayloadFormatWrite::UT_CDTMFPayloadFormatWrite_StartDTMFToneLL(  )
    {
    TChar ch;
    ch = 'k';
    // case #1
    iWrite->SetPayloadFormat( EDTMFPayloadFormatEvent );
    EUNIT_ASSERT_SPECIFIC_LEAVE( iWrite->StartDTMFToneL( ch ), KErrArgument );
        
    // case #2
    ch = '2';
    iWrite->SetPayloadFormat( EDTMFPayloadFormatTone );
    iWrite->StartDTMFToneL( ch );
    
    // case #3
    ch = '3';
    iWrite->SetPayloadFormat( EDTMFPayloadFormatNotDefined );
    iWrite->StartDTMFToneL( ch );
        
    // case #'*'
    ch = '*';
    iWrite->iGenerationMode = 2;
    iWrite->StartDTMFToneL( ch );
    }

void UT_CDTMFPayloadFormatWrite::UT_CDTMFPayloadFormatWrite_StopDTMFToneLL(  )
    {
    TChar ch;
    iWrite->iGenerationMode = 2;
        
    EUNIT_ASSERT_NO_LEAVE( iWrite->StopDTMFToneL() );
        
    // case #1
    ch = '1';
    iWrite->SetPayloadFormat( EDTMFPayloadFormatEvent );

    iWrite->StartDTMFToneL( ch );
    EUNIT_ASSERT_NO_LEAVE( iWrite->StopDTMFToneL() );
    
    // case #2
    ch = '2';
    iWrite->SetPayloadFormat( EDTMFPayloadFormatRedEvents );
    EUNIT_ASSERT_NO_LEAVE( iWrite->StartDTMFToneL( ch ) );
    EUNIT_ASSERT_NO_LEAVE( iWrite->StopDTMFToneL() );
    
    // case #3
    iWrite->SetPayloadFormat( EDTMFPayloadFormatTone );
    EUNIT_ASSERT_NO_LEAVE( iWrite->StartDTMFToneL( ch ) );
    EUNIT_ASSERT_NO_LEAVE( iWrite->StopDTMFToneL() );
        
    // not acceptable payload
    iWrite->SetPayloadFormat( EDTMFPayloadFormatNotDefined );
    EUNIT_ASSERT_NO_LEAVE( iWrite->StopDTMFToneL() );
    }

void UT_CDTMFPayloadFormatWrite::UT_CDTMFPayloadFormatWrite_HandleControlChar()
    {
    User::LeaveIfError(iWrite->SinkThreadLogon( *this ));
    
    TChar ch('9');
    iWrite->HandleControlChar( ch );
    
    ch = 'w';    
    iWrite->HandleControlChar( ch );
    
    ch = 'p';
    iWrite->HandleControlChar( ch );
    }

void UT_CDTMFPayloadFormatWrite::UT_CDTMFPayloadFormatWrite_DeliverPacketL()
    {
    CMMFDataBuffer* buffer = CMMFDataBuffer::NewL( KDTMFDefaultPayloadSize );
    CleanupStack::PushL( buffer );
    buffer->Data().FillZ( KDTMFDefaultPayloadSize );
    
    EUNIT_ASSERT_NO_LEAVE( iWrite->DeliverPacketL(*buffer, EFalse) );
    
    iWrite->iGenerationMode = 2;
    iWrite->iCInfo.iAlgoUsed = 2;
    EUNIT_ASSERT_NO_LEAVE( iWrite->DeliverPacketL(*buffer, EFalse) );
    iWrite->iCInfo.iAlgoUsed = 3;
    EUNIT_ASSERT_NO_LEAVE( iWrite->DeliverPacketL(*buffer, ETrue) );


    CleanupStack::PopAndDestroy( buffer );

    }

void UT_CDTMFPayloadFormatWrite::UT_CDTMFPayloadFormatWrite_SendDTMFTonesLL(  )
    {
    TBuf8<9> tones;
    tones.Zero();
    tones.Append('*');
    tones.Append('4');
    tones.Append('w');
    tones.Append('4');
    
    // case #1
    User::LeaveIfError( iWrite->SinkThreadLogon( *this ) );
        
    iWrite->SendDTMFTonesL( tones );
    EUNIT_ASSERT( iWrite->iSendBuffer.Count() == 4 )
    
    // case #2
    iWrite->SetPayloadFormat( EDTMFPayloadFormatEvent );
    iWrite->SendDTMFTonesL( tones );
    EUNIT_ASSERT( iWrite->iSendBuffer.Count() == 4 )

    TBuf8<9> invalidTones;
    invalidTones.Zero();
    invalidTones.Append('&');
    invalidTones.Append('(');
    invalidTones.Append('Ö');
    EUNIT_ASSERT_SPECIFIC_LEAVE( iWrite->SendDTMFTonesL( invalidTones ), KErrArgument );
    }
    
void UT_CDTMFPayloadFormatWrite::UT_CDTMFPayloadFormatWrite_CancelDTMFStringSending()
    {
    iWrite->CancelDTMFStringSending();
    
    User::LeaveIfError( iWrite->SinkThreadLogon( *this ) );
    
    iWrite->SendDTMFTonesL( iTones );
    iWrite->CancelDTMFStringSending();
    }

void UT_CDTMFPayloadFormatWrite::UT_CDTMFPayloadFormatWrite_NotifyStopInDTMFStringL(  )
    {
    EUNIT_ASSERT_NO_LEAVE( iWrite->NotifyStopInDTMFString( EFalse ) );
    }

void UT_CDTMFPayloadFormatWrite::UT_CDTMFPayloadFormatWrite_ContinueDTMFStringSendingL(  )
    {
    EUNIT_ASSERT_NO_LEAVE( iWrite->ContinueDTMFStringSending( ETrue ) );
    EUNIT_ASSERT_NO_LEAVE( iWrite->ContinueDTMFStringSending( EFalse ) );
    }

void UT_CDTMFPayloadFormatWrite::UT_CDTMFPayloadFormatWrite_PayloadFormatL(  )
    {
    iWrite->SetPayloadFormat( EDTMFPayloadFormatEvent );
    EUNIT_ASSERT_EQUALS( iWrite->PayloadFormat( ), EDTMFPayloadFormatEvent );
    }

void UT_CDTMFPayloadFormatWrite::UT_CDTMFPayloadFormatWrite_SetPayloadFormatL(  )
    {
    EUNIT_ASSERT_NO_LEAVE( iWrite->SetPayloadFormat( EDTMFPayloadFormatEvent ) );
    }

void UT_CDTMFPayloadFormatWrite::UT_CDTMFPayloadFormatWrite_FrameTimeIntervalL(  )
    {
    EUNIT_ASSERT_EQUALS( iWrite->FrameTimeInterval(KUidMediaTypeAudio).Int64(), 0 );
    EUNIT_ASSERT_EQUALS( iWrite->FrameTimeInterval(KUidMediaTypeVideo).Int64(), 0 );
    }

void UT_CDTMFPayloadFormatWrite::UT_CDTMFPayloadFormatWrite_DurationL(  )
    {
    EUNIT_ASSERT_EQUALS( iWrite->Duration( KUidMediaTypeAudio ).Int64(), 1000000000 );
    }

void UT_CDTMFPayloadFormatWrite::UT_CDTMFPayloadFormatWrite_EmptyBufferLL(  )
    {
    EUNIT_ASSERT_SPECIFIC_LEAVE( iWrite->EmptyBufferL( iBuffer, iDataSource, KUidMediaTypeAudio ), KErrNotSupported );
    }

void UT_CDTMFPayloadFormatWrite::UT_CDTMFPayloadFormatWrite_BufferEmptiedLL(  )
    {
    EUNIT_ASSERT_NO_LEAVE( iWrite->BufferEmptiedL( iBuffer ) );
    }

void UT_CDTMFPayloadFormatWrite::UT_CDTMFPayloadFormatWrite_CanCreateSinkBufferL(  )
    {
    EUNIT_ASSERT( iWrite->CanCreateSinkBuffer() == EFalse );
    }

void UT_CDTMFPayloadFormatWrite::UT_CDTMFPayloadFormatWrite_CreateSinkBufferLL()
    {
    TBool ref;
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
        iWrite->CreateSinkBufferL( KUidMediaTypeAudio, ref ), KErrNotSupported );
    }

void UT_CDTMFPayloadFormatWrite::UT_CDTMFPayloadFormatWrite_SinkPrimeLL()
    {
    EUNIT_ASSERT_NO_LEAVE( iWrite->SinkPrimeL() );
    }

void UT_CDTMFPayloadFormatWrite::UT_CDTMFPayloadFormatWrite_SinkPlayLL()
    {
    EUNIT_ASSERT_NO_LEAVE( iWrite->SinkPlayL() );
    }

void UT_CDTMFPayloadFormatWrite::UT_CDTMFPayloadFormatWrite_SinkPauseLL()
    {
    EUNIT_ASSERT_NO_LEAVE( iWrite->SinkPauseL() );
    }

void UT_CDTMFPayloadFormatWrite::UT_CDTMFPayloadFormatWrite_SinkStopLL()
    {
    User::LeaveIfError( iWrite->SinkThreadLogon( *this ) );
    
    EUNIT_ASSERT_NO_LEAVE( iWrite->SinkStopL() );
    }

void UT_CDTMFPayloadFormatWrite::UT_CDTMFPayloadFormatWrite_SinkThreadLogonL(  )
    {
  
  User::LeaveIfError(iWrite->SinkThreadLogon( *this ));
    }

void UT_CDTMFPayloadFormatWrite::UT_CDTMFPayloadFormatWrite_SinkThreadLogoffL(  )
    {
    iWrite->SinkThreadLogoff();
    }

void UT_CDTMFPayloadFormatWrite::UT_CDTMFPayloadFormatWrite_SinkDataTypeCodeL(  )
    {
    EUNIT_ASSERT_NO_LEAVE( iWrite->SinkDataTypeCode( KUidMediaTypeAudio ) );
    EUNIT_ASSERT_NO_LEAVE( iWrite->SinkDataTypeCode( KUidMediaTypeVideo ) );
    }

void UT_CDTMFPayloadFormatWrite::UT_CDTMFPayloadFormatWrite_SetSinkDataTypeCodeL(  )
    {
    TFourCC fourcc;
 
    EUNIT_ASSERT_EQUALS( iWrite->SetSinkDataTypeCode( fourcc, KUidMediaTypeAudio ), KErrNone );
    EUNIT_ASSERT_EQUALS( iWrite->SetSinkDataTypeCode( fourcc, KUidMediaTypeVideo ), KErrNotSupported );
    }

void UT_CDTMFPayloadFormatWrite::UT_CDTMFPayloadFormatWrite_SetSinkPrioritySettingsL( )
    {
    TMMFPrioritySettings priori;
    priori.iState = EMMFStateIdle;
    priori.iPref = TMdaPriorityPreference( 0x05220001 );
    priori.iPriority = 1;
    
    EUNIT_ASSERT_NO_LEAVE( iWrite->SetSinkPrioritySettings( priori) );
    }

    
void UT_CDTMFPayloadFormatWrite::UT_CDTMFPayloadFormatWrite_ConfigurePayloadFormatLL(  )
    {
    TMccCodecInfo codec;
    TBuf8<10> buf;
    codec.iRedundancyCount = 0;
    codec.iRedundantPayload = 0;
    codec.iHwFrameTime = 0;
    codec.iAlgoUsed = 2;
        
    TMccCodecInfoBuffer buffer( codec );
        
    // if ( aConfigParams.Size() != sizeof( TMccCodecInfo ) )
    EUNIT_ASSERT_SPECIFIC_LEAVE( iWrite->ConfigurePayloadFormatL( buf, *iRtpMediaClock ), KErrArgument );

    iWrite->ConfigurePayloadFormatL( buffer, *iRtpMediaClock );
    
    codec.iRedundancyCount = 1;
    codec.iRedundantPayload = 99;
    codec.iHwFrameTime = 1;
    codec.iAlgoUsed = 2;
        
    buffer = codec;
    iWrite->ConfigurePayloadFormatL( buffer, *iRtpMediaClock );
        
    codec.iRedundancyCount = 0;
    codec.iRedundantPayload = 0;
    codec.iHwFrameTime = 0;
    codec.iAlgoUsed = 0;
        
    buffer = codec;
    iWrite->ConfigurePayloadFormatL( buffer, *iRtpMediaClock );
          
    delete iWrite->iPayloadEncoder;
    iWrite->iPayloadEncoder = NULL;
    codec.iRedundancyCount = 6;
    codec.iRedundantPayload = 0;
    codec.iHwFrameTime = 0;
    codec.iAlgoUsed = 2;
        
    buffer = codec;
    iWrite->ConfigurePayloadFormatL( buffer, *iRtpMediaClock );

    codec.iRedundancyCount = 3;
    codec.iRedundantPayload = 0;
    codec.iHwFrameTime = 0;
    codec.iAlgoUsed = 3; // leave
    buffer = codec;
    EUNIT_ASSERT_SPECIFIC_LEAVE( iWrite->ConfigurePayloadFormatL( buffer, *iRtpMediaClock ), KErrArgument );

    codec.iRedundancyCount = 3;
    codec.iRedundantPayload = 0;
    codec.iHwFrameTime = 0;
    codec.iAlgoUsed = 2; // leave
    buffer = codec;
    iWrite->iCInfo.iCodecMode = 1;
    iWrite->ConfigurePayloadFormatL( buffer, *iRtpMediaClock );
    }
    
void UT_CDTMFPayloadFormatWrite::UT_CDTMFPayloadFormatWrite_EncodeNextDigitLL(  )
    {
    TMccCodecInfo codec;
    codec.iRedundancyCount = 0;
    codec.iRedundantPayload = 0;
    codec.iHwFrameTime = 0;
    codec.iAlgoUsed = 2;
        
    TMccCodecInfoBuffer buffer( codec );
        
    iWrite->ConfigurePayloadFormatL( buffer, *iRtpMediaClock );
        
    TBuf8<9> tones;
    tones.Zero();
    tones.Append('p');
    tones.Append('4');
    tones.Append('w');
    tones.Append('4');
  
    User::LeaveIfError(iWrite->SinkThreadLogon( *this ));    
    
    iWrite->SendDTMFTonesL( tones );
    iWrite->iSendingMode = CDTMFPayloadFormatWrite::EModeManual;
    iWrite->EncodeNextDigitL();
    iWrite->iSendingMode = CDTMFPayloadFormatWrite::EModeAutomatic;
    iWrite->EncodeNextDigitL();
    iWrite->CancelDTMFStringSending();
        
    iWrite->SendDTMFTonesL( tones );
    iWrite->EncodeNextDigitL();
    iWrite->CancelDTMFStringSending();
    
    tones.Zero();
    tones.Append('3');
    tones.Append('4');
    tones.Append('w');
    tones.Append('5');
    
    EUNIT_ASSERT( iWrite->SetPayloadFormat( EDTMFPayloadFormatRedEvents ) == KErrNone );
    iWrite->SendDTMFTonesL( tones );
    iWrite->iGenerationMode = 2; // KSignalOutbandDtmf
    iWrite->EncodeNextDigitL(); 
    iWrite->CancelDTMFStringSending();
    
    iWrite->iGenerationMode = 1; // KSignalInbandDtmf
    EUNIT_ASSERT( iWrite->SetPayloadFormat( EDTMFPayloadFormatEvent ) == KErrNone );
    iWrite->SendDTMFTonesL( tones );
    iWrite->EncodeNextDigitL();
    iWrite->CancelDTMFStringSending();
        
    EUNIT_ASSERT( iWrite->SetPayloadFormat( EDTMFPayloadFormatTone ) == KErrNone );
    iWrite->SendDTMFTonesL( tones );
    iWrite->EncodeNextDigitL( );
    iWrite->CancelDTMFStringSending();
    
    EUNIT_ASSERT( iWrite->SetPayloadFormat( EDTMFPayloadFormatNotDefined ) == KErrNotSupported  );
    iWrite->SendDTMFTonesL( tones );
    iWrite->EncodeNextDigitL();
    iWrite->EncodeNextDigitL();
    iWrite->EncodeNextDigitL();
    iWrite->EncodeNextDigitL();
    iWrite->EncodeNextDigitL();
    iWrite->EncodeNextDigitL();
    iWrite->SinkThreadLogoff();
    }
  
void UT_CDTMFPayloadFormatWrite::UT_CDTMFPayloadFormatWrite_DoToneOnActionsLL(  )
    {
    CDTMFPayloadFormatWrite::TSendBufferElement element( '1', 0 );
    iWrite->iSendBuffer.AppendL( element );
    
    User::LeaveIfError(iWrite->SinkThreadLogon( *this ));
    
    iWrite->iSendingMode = CDTMFPayloadFormatWrite::EModeManual; 
    EUNIT_ASSERT_NO_LEAVE( iWrite->DoToneOnActionsL( ETrue ) );
    iWrite->iSendingMode = CDTMFPayloadFormatWrite::EModeAutomatic;
    EUNIT_ASSERT_NO_LEAVE( iWrite->DoToneOnActionsL( ETrue ) );
    
    iWrite->iSampleRate = 20;
    EUNIT_ASSERT_NO_LEAVE( iWrite->DoToneOnActionsL( EFalse ) ); 
    
    iWrite->iSampleRate = 200;
    EUNIT_ASSERT_NO_LEAVE( iWrite->DoToneOnActionsL( EFalse ) ); 
    
    iWrite->iSendingMode = CDTMFPayloadFormatWrite::EModeManual; 
    EUNIT_ASSERT_NO_LEAVE( iWrite->DoToneOnActionsL( EFalse ) ); 
    
    iWrite->iToneDuration = 65500;
    EUNIT_ASSERT( iWrite->SetPayloadFormat( EDTMFPayloadFormatEvent ) == KErrNone );
    EUNIT_ASSERT_NO_LEAVE( iWrite->DoToneOnActionsL( EFalse ) ); 
    iWrite->iToneDuration = 66000;
    EUNIT_ASSERT( iWrite->SetPayloadFormat( EDTMFPayloadFormatRedEvents ) == KErrNone );
    EUNIT_ASSERT_NO_LEAVE( iWrite->DoToneOnActionsL( EFalse ) ); 
    EUNIT_ASSERT( iWrite->SetPayloadFormat( EDTMFPayloadFormatTone ) == KErrNone );
    EUNIT_ASSERT_NO_LEAVE( iWrite->DoToneOnActionsL( EFalse ) ); 
    EUNIT_ASSERT( iWrite->SetPayloadFormat( EDTMFPayloadFormatNotDefined ) == KErrNotSupported );
    EUNIT_ASSERT_NO_LEAVE( iWrite->DoToneOnActionsL( EFalse ) ); 
    }
    
void UT_CDTMFPayloadFormatWrite::UT_CDTMFPayloadFormatWrite_DoToneOffActionsLL(  )
    {
    User::LeaveIfError( iWrite->SinkThreadLogon( *this ) );
    
    CDTMFPayloadFormatWrite::TSendBufferElement element( '1', 100 );
    iWrite->iSendBuffer.AppendL( element );
    
    iWrite->iSendingMode = CDTMFPayloadFormatWrite::EModeManual;
    EUNIT_ASSERT_NO_LEAVE( iWrite->DoToneOffActionsL( ETrue ) );
    EUNIT_ASSERT( KMccDtmfUnknown == iEventData.iDtmfEventType );
    
    iWrite->iSendingMode = CDTMFPayloadFormatWrite::EModeAutomatic;
    EUNIT_ASSERT_NO_LEAVE( iWrite->DoToneOffActionsL( ETrue ) );
    EUNIT_ASSERT( KMccDtmfUnknown == iEventData.iDtmfEventType );
    
    iWrite->iFinalPacketCtr = 1;
    EUNIT_ASSERT_NO_LEAVE( iWrite->DoToneOffActionsL( EFalse ) );
    EUNIT_ASSERT( KMccDtmfUnknown == iEventData.iDtmfEventType );
        
    TMccCodecInfo codec;
    TBuf8<10> buf;
    codec.iRedundancyCount = 0;
    codec.iRedundantPayload = 0;
    codec.iHwFrameTime = 0;
    codec.iAlgoUsed = 2;
    codec.iPtime = 50;
    TMccCodecInfoBuffer buffer( codec );
    iWrite->ConfigurePayloadFormatL( buffer, *iRtpMediaClock );
    iWrite->iFinalPacketCtr = 10;
    
    EUNIT_ASSERT_NO_LEAVE( iWrite->DoToneOffActionsL( EFalse ) );
    EUNIT_ASSERT( KMccDtmfSequenceStop == iEventData.iDtmfEventType );
    
    iWrite->iSendingMode = CDTMFPayloadFormatWrite::EModeManual;
    iWrite->iFinalPacketCtr = 10;
    EUNIT_ASSERT_NO_LEAVE( iWrite->DoToneOffActionsL( EFalse ) );
    EUNIT_ASSERT( KMccDtmfManualStop == iEventData.iDtmfEventType );
    }

TInt UT_CDTMFPayloadFormatWrite::SendEventToClient( const TMMFEvent& aEvent )
    {
    TMccEvent* mccEventPtr
        = reinterpret_cast<const TMccInternalEvent&>( aEvent ).iMccEvent;

    iEventData = (reinterpret_cast<const TMccDtmfEventDataPackage&>( 
        mccEventPtr->iEventData ) ) ();
    
    return KErrNone;
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CDTMFPayloadFormatWrite,
    "MCC DTMF payloadformat write",
    "UNIT" )

EUNIT_TEST(
    "StartDTMFToneL - test ",
    "CDTMFPayloadFormatWrite",
    "StartDTMFToneL",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatWrite_StartDTMFToneLL, Teardown)

EUNIT_TEST(
    "HandleTimerEventL - test ",
    "CDTMFPayloadFormatWrite",
    "HandleTimerEventL",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatWrite_HandleTimerEventL, Teardown)

EUNIT_TEST(
    "HandleControlChar - test ",
    "CDTMFPayloadFormatWrite",
    "HandleControlChar",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatWrite_HandleControlChar, Teardown)

EUNIT_TEST(
    "DeliverPacketL - test ",
    "CDTMFPayloadFormatWrite",
    "DeliverPacketL",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatWrite_DeliverPacketL, Teardown)

EUNIT_TEST(
    "StopDTMFToneL - test ",
    "CDTMFPayloadFormatWrite",
    "StopDTMFToneL",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatWrite_StopDTMFToneLL, Teardown)

EUNIT_TEST(
    "SendDTMFTonesL - test ",
    "CDTMFPayloadFormatWrite",
    "SendDTMFTonesL",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatWrite_SendDTMFTonesLL, Teardown)

EUNIT_TEST(
    "CancelDTMFStringSending - test ",
    "CDTMFPayloadFormatWrite",
    "CancelDTMFStringSending",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatWrite_CancelDTMFStringSending, Teardown)

EUNIT_TEST(
    "NotifyStopInDTMFString - test ",
    "CDTMFPayloadFormatWrite",
    "NotifyStopInDTMFString",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatWrite_NotifyStopInDTMFStringL, Teardown)

EUNIT_TEST(
    "ContinueDTMFStringSending - test ",
    "CDTMFPayloadFormatWrite",
    "ContinueDTMFStringSending",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatWrite_ContinueDTMFStringSendingL, Teardown)

EUNIT_TEST(
    "PayloadFormat - test ",
    "CDTMFPayloadFormatWrite",
    "PayloadFormat",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatWrite_PayloadFormatL, Teardown)

EUNIT_TEST(
    "SetPayloadFormat - test ",
    "CDTMFPayloadFormatWrite",
    "SetPayloadFormat",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatWrite_SetPayloadFormatL, Teardown)

EUNIT_TEST(
    "FrameTimeInterval - test ",
    "CDTMFPayloadFormatWrite",
    "FrameTimeInterval",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatWrite_FrameTimeIntervalL, Teardown)

EUNIT_TEST(
    "Duration - test ",
    "CDTMFPayloadFormatWrite",
    "Duration",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatWrite_DurationL, Teardown)

EUNIT_TEST(
    "EmptyBufferL - test ",
    "CDTMFPayloadFormatWrite",
    "EmptyBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatWrite_EmptyBufferLL, Teardown)

EUNIT_TEST(
    "BufferEmptiedL - test ",
    "CDTMFPayloadFormatWrite",
    "BufferEmptiedL",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatWrite_BufferEmptiedLL, Teardown)

EUNIT_TEST(
    "CanCreateSinkBuffer - test ",
    "CDTMFPayloadFormatWrite",
    "CanCreateSinkBuffer",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatWrite_CanCreateSinkBufferL, Teardown)

EUNIT_TEST(
    "CreateSinkBufferL - test ",
    "CDTMFPayloadFormatWrite",
    "CreateSinkBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatWrite_CreateSinkBufferLL, Teardown)

EUNIT_TEST(
    "SinkPrimeL - test ",
    "CDTMFPayloadFormatWrite",
    "SinkPrimeL",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatWrite_SinkPrimeLL, Teardown)

EUNIT_TEST(
    "SinkPlayL - test ",
    "CDTMFPayloadFormatWrite",
    "SinkPlayL",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatWrite_SinkPlayLL, Teardown)

EUNIT_TEST(
    "SinkPauseL - test ",
    "CDTMFPayloadFormatWrite",
    "SinkPauseL",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatWrite_SinkPauseLL, Teardown)

EUNIT_TEST(
    "SinkStopL - test ",
    "CDTMFPayloadFormatWrite",
    "SinkStopL",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatWrite_SinkStopLL, Teardown)

EUNIT_TEST(
    "SinkThreadLogon - test ",
    "CDTMFPayloadFormatWrite",
    "SinkThreadLogon",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatWrite_SinkThreadLogonL, Teardown)

EUNIT_TEST(
    "SinkThreadLogoff - test ",
    "CDTMFPayloadFormatWrite",
    "SinkThreadLogoff",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatWrite_SinkThreadLogoffL, Teardown)

EUNIT_TEST(
    "SinkDataTypeCode - test ",
    "CDTMFPayloadFormatWrite",
    "SinkDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatWrite_SinkDataTypeCodeL, Teardown)

EUNIT_TEST(
    "SetSinkDataTypeCode - test ",
    "CDTMFPayloadFormatWrite",
    "SetSinkDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatWrite_SetSinkDataTypeCodeL, Teardown)

EUNIT_TEST(
    "SetSinkPrioritySettings - test ",
    "CDTMFPayloadFormatWrite",
    "SetSinkPrioritySettings",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatWrite_SetSinkPrioritySettingsL, Teardown)    
     
EUNIT_TEST(
    "ConfigurePayloadFormatL - test ",
    "CDTMFPayloadFormatWrite",
    "ConfigurePayloadFormatL",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatWrite_ConfigurePayloadFormatLL, Teardown) 
    
EUNIT_TEST(
    "EncodeNextDigitL - test ",
    "CDTMFPayloadFormatWrite",
    "EncodeNextDigitL",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatWrite_EncodeNextDigitLL, Teardown) 
    
EUNIT_TEST(
    "DoToneOnActionsL - test ",
    "CDTMFPayloadFormatWrite",
    "DoToneOnActionsL",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatWrite_DoToneOnActionsLL, Teardown) 

EUNIT_TEST(
    "DoToneOffActionsL - test ",
    "CDTMFPayloadFormatWrite",
    "DoToneOffActionsL",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadFormatWrite_DoToneOffActionsLL, Teardown) 

EUNIT_END_TEST_TABLE

//  END OF FILE
