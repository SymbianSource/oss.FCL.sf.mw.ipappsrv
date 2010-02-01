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
#include "UT_CMccCodecH263.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "mmcccodech263.h"

// CONSTRUCTION
UT_CMccCodecH263* UT_CMccCodecH263::NewL()
    {
    UT_CMccCodecH263* self = UT_CMccCodecH263::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccCodecH263* UT_CMccCodecH263::NewLC()
    {
    UT_CMccCodecH263* self = new( ELeave ) UT_CMccCodecH263();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccCodecH263::~UT_CMccCodecH263()
    {
    }

// Default constructor
UT_CMccCodecH263::UT_CMccCodecH263()
    {
    }

// Second phase construct
void UT_CMccCodecH263::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMccCodecH263::SetupL(  )
    {
    iCodec = CMccCodecH263::NewL();

    }

void UT_CMccCodecH263::Teardown(  )
    {
    delete iCodec;
    }

void UT_CMccCodecH263::UT_CMccCodecH263_RequireSignallingL(  )
    {
    CMccCodecH263* codecinfo = CMccCodecH263::NewL();
    CleanupStack::PushL(codecinfo);
    codecinfo->SetPTime(2);
    codecinfo->SetMaxPTime(1);
    codecinfo->SetPayloadType(1);
    codecinfo->iBitrateMask = 1;
    codecinfo->SetCodecMode(1);
    codecinfo->SetSamplingFreq(1000);
    codecinfo->SetMaxBitrate(64000);
    codecinfo->SetFramerate(1.1);
    codecinfo->iVideoWidth = 1;
    codecinfo->iVideoHeight  = 1;
    EUNIT_ASSERT(iCodec->RequireSignalling(*codecinfo));
    CleanupStack::PopAndDestroy(codecinfo);        
    }

void UT_CMccCodecH263::UT_CMccCodecH263_CloneDefaultsLL(  )
    {
    // 2000 mode version cloning
    CMccCodecInformation* codecinfo = iCodec->CloneDefaultsL();
    CleanupStack::PushL( codecinfo );
    EUNIT_ASSERT( codecinfo->CodecMode() == KH263FormatModeNew );
    CleanupStack::PopAndDestroy( codecinfo ); 
    
    // 1998 mode version cloning
    iCodec->iSdpName = KH2631998SdpName;
    CMccCodecInformation* codecinfo2 = iCodec->CloneDefaultsL();
    CleanupStack::PushL( codecinfo2 );
    EUNIT_ASSERT( codecinfo2->CodecMode() == KH263FormatModeOld );
    CleanupStack::PopAndDestroy( codecinfo2 ); 
    }

void UT_CMccCodecH263::UT_CMccCodecH263_CloneDetailedLL(  )
    {
    CMccCodecInformation* codecinfo = iCodec->CloneDetailedL();
    delete codecinfo; 
    }

void UT_CMccCodecH263::UT_CMccCodecH263_EnableVADL(  )
    {
    EUNIT_ASSERT( iCodec->EnableVAD(ETrue) == KErrNotSupported  );
    }


void UT_CMccCodecH263::UT_CMccCodecH263_SetBitrateL(  )
    {
    EUNIT_ASSERT( iCodec->SetBitrate(129000) == KErrNotSupported );
    EUNIT_ASSERT( iCodec->SetBitrate(64000) == KErrNone );

    }

void UT_CMccCodecH263::UT_CMccCodecH263_SetMaxBitrateL(  )
    {
    EUNIT_ASSERT( iCodec->SetMaxBitrate(64000) == KErrNone );
    EUNIT_ASSERT( iCodec->SetMaxBitrate(128000) == KErrNone );
    EUNIT_ASSERT( iCodec->SetMaxBitrate(20) == KErrNotSupported );

    }

void UT_CMccCodecH263::UT_CMccCodecH263_SetSamplingFreqL(  )
    {    
    EUNIT_ASSERT( iCodec->SetSamplingFreq(64000) ==  KErrNone );
    }

void UT_CMccCodecH263::UT_CMccCodecH263_SetPTimeL(  )
    {
    EUNIT_ASSERT( iCodec->SetPTime(100) ==  KErrNone );
    EUNIT_ASSERT( iCodec->SetPTime(300) ==  KErrNotSupported );
    }

void UT_CMccCodecH263::UT_CMccCodecH263_SetMaxPTimeL(  )
    {
    EUNIT_ASSERT( iCodec->SetMaxPTime(100) ==  KErrNone );
    EUNIT_ASSERT( iCodec->SetMaxPTime(300) ==  KErrNotSupported );
    }

void UT_CMccCodecH263::UT_CMccCodecH263_SetSdpNameL(  )
    {
    
    EUNIT_ASSERT( iCodec->SetSdpName( KH263SdpName ) == KErrNone);
    EUNIT_ASSERT( iCodec->SetSdpName( KH2631998SdpName ) == KErrNone);

    EUNIT_ASSERT( iCodec->SetSdpName( KAACSdpName ) == KErrNotSupported);
    }

void UT_CMccCodecH263::UT_CMccCodecH263_SetPayloadTypeL(  )
    {
    EUNIT_ASSERT( iCodec->SetPayloadType(100) == KErrNone );
    EUNIT_ASSERT( iCodec->SetPayloadType(20) == KErrNotSupported );
    }

void UT_CMccCodecH263::UT_CMccCodecH263_SetCodecModeL(  )
    {
    EUNIT_ASSERT( iCodec->SetCodecMode(2) == KErrNone );
    
    // When changing to old mode, sdp name is changed automatically as well
    // Also payload type num is changed if it it default value of 2000 mode
    EUNIT_ASSERT( iCodec->SetCodecMode( KH263FormatModeOld ) == KErrNone );
    EUNIT_ASSERT( iCodec->iSdpName.CompareF( KH2631998SdpName ) == 0 );
    EUNIT_ASSERT_EQUALS( iCodec->iPayloadType, KH2631998PayloadType );
    
    // When changing back to 2000 mode, sdp name is changed
    EUNIT_ASSERT( iCodec->SetCodecMode( KH263FormatModeNew ) == KErrNone );
    EUNIT_ASSERT( iCodec->iSdpName.CompareF( KH2632000SdpName ) == 0 );

    // If paylod type is non-default, it is not modified
    iCodec->iPayloadType = KH263PayloadType - 1;
    EUNIT_ASSERT( iCodec->SetCodecMode( KH263FormatModeOld ) == KErrNone );
    EUNIT_ASSERT( iCodec->iSdpName.CompareF( KH2631998SdpName ) == 0 );
    EUNIT_ASSERT_EQUALS( iCodec->iPayloadType, KH263PayloadType - 1 );
    }

void UT_CMccCodecH263::UT_CMccCodecH263_GetFmtpLL(  )
    {
    iCodec->iMaxBitrate = 65000;
    HBufC8* fmtp = HBufC8::NewL(iCodec->GetFmtpL().Length());
    delete fmtp;
    }

void UT_CMccCodecH263::UT_CMccCodecH263_SetFmtpAttrLL(  )
    {
    iCodec->iMaxBitrate = 65000;
    HBufC8* fmtp = HBufC8::NewLC(iCodec->GetFmtpL().Length());
    fmtp->Des().Copy(iCodec->GetFmtpL());
    iCodec->SetFmtpAttrL(*fmtp, EFalse);
    
    EUNIT_ASSERT_EQUALS( iCodec->Bitrate(), 128000 ); 
    
    // When bitrate is explicitly set, pasring fmtp will not change it
    iCodec->SetBitrate( 20000 );
    iCodec->SetFmtpAttrL(*fmtp, EFalse);
    EUNIT_ASSERT_EQUALS( iCodec->Bitrate(), 20000 ); 
    
    CleanupStack::PopAndDestroy( fmtp );
    }

void UT_CMccCodecH263::UT_CMccCodecH263_SetAllowedBitratesL(  )
    {
    EUNIT_ASSERT( iCodec->SetAllowedBitrates(1) == KErrNone );
    }

void UT_CMccCodecH263::UT_CMccCodecH263_SetConfigKeyL()
    {
    // NOP
    iCodec->SetConfigKeyL( KNullDesC8 );
    }
    
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccCodecH263,
    "CMccCodecH263 test",
    "UNIT" )

EUNIT_TEST(
    "RequireSignalling - test ",
    "CMccCodecH263",
    "RequireSignalling",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecH263_RequireSignallingL, Teardown)

EUNIT_TEST(
    "CloneDefaultsL - test ",
    "CMccCodecH263",
    "CloneDefaultsL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecH263_CloneDefaultsLL, Teardown)

EUNIT_TEST(
    "CloneDetailedL - test ",
    "CMccCodecH263",
    "CloneDetailedL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecH263_CloneDetailedLL, Teardown)

EUNIT_TEST(
    "EnableVAD - test ",
    "CMccCodecH263",
    "EnableVAD",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecH263_EnableVADL, Teardown)


EUNIT_TEST(
    "SetBitrate - test ",
    "CMccCodecH263",
    "SetBitrate",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecH263_SetBitrateL, Teardown)

EUNIT_TEST(
    "SetMaxBitrate - test ",
    "CMccCodecH263",
    "SetMaxBitrate",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecH263_SetMaxBitrateL, Teardown)

EUNIT_TEST(
    "SetSamplingFreq - test ",
    "CMccCodecH263",
    "SetSamplingFreq",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecH263_SetSamplingFreqL, Teardown)

EUNIT_TEST(
    "SetPTime - test ",
    "CMccCodecH263",
    "SetPTime",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecH263_SetPTimeL, Teardown)

EUNIT_TEST(
    "SetMaxPTime - test ",
    "CMccCodecH263",
    "SetMaxPTime",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecH263_SetMaxPTimeL, Teardown)

EUNIT_TEST(
    "SetSdpName - test ",
    "CMccCodecH263",
    "SetSdpName",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecH263_SetSdpNameL, Teardown)

EUNIT_TEST(
    "SetPayloadType - test ",
    "CMccCodecH263",
    "SetPayloadType",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecH263_SetPayloadTypeL, Teardown)

EUNIT_TEST(
    "SetCodecMode - test ",
    "CMccCodecH263",
    "SetCodecMode",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecH263_SetCodecModeL, Teardown)

EUNIT_TEST(
    "GetFmtpL - test ",
    "CMccCodecH263",
    "GetFmtpL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecH263_GetFmtpLL, Teardown)

EUNIT_TEST(
    "SetFmtpAttrL - test ",
    "CMccCodecH263",
    "SetFmtpAttrL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecH263_SetFmtpAttrLL, Teardown)

EUNIT_TEST(
    "SetAllowedBitrates - test ",
    "CMccCodecH263",
    "SetAllowedBitrates",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecH263_SetAllowedBitratesL, Teardown)

EUNIT_TEST(
    "SetConfigKeyL - test ",
    "CMccCodecH263",
    "SetConfigKeyL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecH263_SetConfigKeyL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
