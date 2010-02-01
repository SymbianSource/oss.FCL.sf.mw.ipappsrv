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
#include "UT_CMCCCodecG729.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "mmcccodecg729.h"
#include "mccunittestmacros.h"

// CONSTRUCTION
UT_CMCCCodecG729* UT_CMCCCodecG729::NewL()
    {
    UT_CMCCCodecG729* self = UT_CMCCCodecG729::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMCCCodecG729* UT_CMCCCodecG729::NewLC()
    {
    UT_CMCCCodecG729* self = new( ELeave ) UT_CMCCCodecG729();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMCCCodecG729::~UT_CMCCCodecG729()
    {
    
    }

// Default constructor
UT_CMCCCodecG729::UT_CMCCCodecG729()
    {
    
    }

// Second phase construct
void UT_CMCCCodecG729::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS
void UT_CMCCCodecG729::SetupL()
    {
    iCodec = CMCCCodecG729::NewL();
    }

void UT_CMCCCodecG729::Teardown()
    {
    delete iCodec;
    }

void UT_CMCCCodecG729::UT_CMCCCodecG729_NewLL()
    {
    EUNIT_ASSERT( ETrue );
    }

void UT_CMCCCodecG729::UT_CMCCCodecG729_CloneDefaultsLL()
    {
    CMccCodecInformation* codec;
    codec = iCodec->CloneDefaultsL();
    delete codec;
    }

void UT_CMCCCodecG729::UT_CMCCCodecG729_CloneDetailedLL()
    {
    CMccCodecInformation* codec;
    codec = iCodec->CloneDetailedL();
    delete codec;
    }

void UT_CMCCCodecG729::UT_CMCCCodecG729_RequireSignallingL()
    {
    CMccCodecInformation* codec = CMCCCodecG729::NewL();
    EUNIT_ASSERT_EQUALS( iCodec->RequireSignalling( *codec ), EFalse );    
    codec->SetPTime( 40 );
    EUNIT_ASSERT_EQUALS( iCodec->RequireSignalling( *codec ), ETrue );
    codec->SetMaxPTime( 180 );
    EUNIT_ASSERT_EQUALS( iCodec->RequireSignalling( *codec ), ETrue );
    delete codec;
    }

void UT_CMCCCodecG729::UT_CMCCCodecG729_EnableVADL()
    {
    EUNIT_ASSERT_EQUALS( iCodec->EnableVAD( ETrue ), KErrNone );
    EUNIT_ASSERT_EQUALS( iCodec->EnableVAD( EFalse ), KErrNone );
    }

void UT_CMCCCodecG729::UT_CMCCCodecG729_SetBitrateL()
    {
    EUNIT_ASSERT_EQUALS( iCodec->SetBitrate( 32000 ), KErrNotSupported );
    EUNIT_ASSERT_EQUALS( iCodec->SetBitrate( KG729Bitrate ), KErrNone );
    }

void UT_CMCCCodecG729::UT_CMCCCodecG729_SetSamplingFreqL()
    {
    EUNIT_ASSERT_EQUALS( iCodec->SetSamplingFreq( 16000 ), KErrNotSupported );
    EUNIT_ASSERT_EQUALS( iCodec->SetSamplingFreq( 8000 ), KErrNone );
    }

void UT_CMCCCodecG729::UT_CMCCCodecG729_SetPTimeL()
    {
    EUNIT_ASSERT_EQUALS( iCodec->SetPTime( 9 ), KErrArgument );
    EUNIT_ASSERT_EQUALS( iCodec->SetPTime( 10 ), KErrNone );
    EUNIT_ASSERT_EQUALS( iCodec->SetPTime( 20 ), KErrNone );
    
    EUNIT_ASSERT_EQUALS( iCodec->SetPTime( 200 ), KErrNone );
    EUNIT_ASSERT_EQUALS( iCodec->SetPTime( 201 ), KErrArgument );
    }

void UT_CMCCCodecG729::UT_CMCCCodecG729_SetMaxPTimeL()
    {
    // Default ptime configuration (ptime=20)
    EUNIT_ASSERT_EQUALS( iCodec->SetMaxPTime( 9 ), KErrArgument );
    EUNIT_ASSERT_EQUALS( iCodec->SetMaxPTime( 10 ), KErrArgument );
    
    EUNIT_ASSERT_EQUALS( iCodec->SetMaxPTime( 20 ), KErrNone );
    EUNIT_ASSERT_EQUALS( iCodec->SetMaxPTime( 200 ), KErrNone );
    EUNIT_ASSERT_EQUALS( iCodec->SetMaxPTime( 201 ), KErrArgument );
    
    // After adjusting ptime=10, maxptime can be 10 also.
    EUNIT_ASSERT_EQUALS( iCodec->SetPTime( 10 ), KErrNone );
    EUNIT_ASSERT_EQUALS( iCodec->SetMaxPTime( 10 ), KErrNone );
    EUNIT_ASSERT_EQUALS( iCodec->SetMaxPTime( 9 ), KErrArgument );
    }

void UT_CMCCCodecG729::UT_CMCCCodecG729_SetSdpNameL()
    {
    EUNIT_ASSERT_EQUALS( iCodec->SetSdpName( KAMRSdpName ), KErrNotSupported );
    EUNIT_ASSERT_EQUALS( iCodec->SetSdpName( KG729SdpName ), KErrNone );
    }

void UT_CMCCCodecG729::UT_CMCCCodecG729_SetPayloadTypeL()
    {
    EUNIT_ASSERT_EQUALS( iCodec->SetPayloadType( KPcmaPayloadType ), KErrNotSupported );
    EUNIT_ASSERT_EQUALS( iCodec->SetPayloadType( KG729PayloadType ), KErrNone );
    }

void UT_CMCCCodecG729::UT_CMCCCodecG729_SetCodecModeL()
    {
    EUNIT_ASSERT_EQUALS( iCodec->SetCodecMode( ENothing ), KErrNone );
    EUNIT_ASSERT_EQUALS( iCodec->SetCodecMode( EPCMU ), KErrNotSupported );
    }
    
void UT_CMCCCodecG729::UT_CMCCCodecG729_CreateFmtpAttrListLL()   
    {
    iCodec->CreateFmtpAttrListL();
    iCodec->EnableVAD( EFalse );
    iCodec->CreateFmtpAttrListL();
    }

void UT_CMCCCodecG729::UT_CMCCCodecG729_ParseFmtpAttrLL()
    {
    EUNIT_ASSERT_NO_LEAVE( iCodec->ParseFmtpAttrL( KAnnexYes ) );
    EUNIT_ASSERT_NO_LEAVE( iCodec->ParseFmtpAttrL( KAnnexNo ) );
    
    _LIT8( KAnnexTest, "annextest=xx" );
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iCodec->ParseFmtpAttrL( KAnnexTest ), KErrArgument );
    }

void UT_CMCCCodecG729::UT_CMCCCodecG729_GetFmtpLL()
    {
    iCodec->GetFmtpL();
    iCodec->GetFmtpL();
    }

//  TEST TABLE
EUNIT_BEGIN_TEST_TABLE(
    UT_CMCCCodecG729,
    "CMCCCodecG729 test",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMCCCodecG729",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CMCCCodecG729_NewLL, Teardown)

EUNIT_TEST(
    "CloneDefaultsL - test ",
    "CMCCCodecG729",
    "CloneDefaultsL",
    "FUNCTIONALITY",
    SetupL, UT_CMCCCodecG729_CloneDefaultsLL, Teardown)

EUNIT_TEST(
    "CloneDetailedL - test ",
    "CMCCCodecG729",
    "CloneDetailedL",
    "FUNCTIONALITY",
    SetupL, UT_CMCCCodecG729_CloneDetailedLL, Teardown)

EUNIT_TEST(
    "RequireSignalling - test ",
    "CMCCCodecG729",
    "RequireSignalling",
    "FUNCTIONALITY",
    SetupL, UT_CMCCCodecG729_RequireSignallingL, Teardown)

EUNIT_TEST(
    "EnableVAD - test ",
    "CMCCCodecG729",
    "EnableVAD",
    "FUNCTIONALITY",
    SetupL, UT_CMCCCodecG729_EnableVADL, Teardown)

EUNIT_TEST(
    "SetBitrate - test ",
    "CMCCCodecG729",
    "SetBitrate",
    "FUNCTIONALITY",
    SetupL, UT_CMCCCodecG729_SetBitrateL, Teardown)

EUNIT_TEST(
    "SetSamplingFreq - test ",
    "CMCCCodecG729",
    "SetSamplingFreq",
    "FUNCTIONALITY",
    SetupL, UT_CMCCCodecG729_SetSamplingFreqL, Teardown)

EUNIT_TEST(
    "SetPTime - test ",
    "CMCCCodecG729",
    "SetPTime",
    "FUNCTIONALITY",
    SetupL, UT_CMCCCodecG729_SetPTimeL, Teardown)

EUNIT_TEST(
    "SetMaxPTime - test ",
    "CMCCCodecG729",
    "SetMaxPTime",
    "FUNCTIONALITY",
    SetupL, UT_CMCCCodecG729_SetMaxPTimeL, Teardown)

EUNIT_TEST(
    "SetSdpName - test ",
    "CMCCCodecG729",
    "SetSdpName",
    "FUNCTIONALITY",
    SetupL, UT_CMCCCodecG729_SetSdpNameL, Teardown)

EUNIT_TEST(
    "SetPayloadType - test ",
    "CMCCCodecG729",
    "SetPayloadType",
    "FUNCTIONALITY",
    SetupL, UT_CMCCCodecG729_SetPayloadTypeL, Teardown)

EUNIT_TEST(
    "SetCodecMode - test ",
    "CMCCCodecG729",
    "SetCodecMode",
    "FUNCTIONALITY",
    SetupL, UT_CMCCCodecG729_SetCodecModeL, Teardown)

EUNIT_TEST(
    "CreateFmtpAttrListL - test ",
    "CMCCCodecG729",
    "CreateFmtpAttrListL",
    "FUNCTIONALITY",
    SetupL, UT_CMCCCodecG729_CreateFmtpAttrListLL, Teardown)

EUNIT_TEST(
    "ParseFmtpAttrL - test ",
    "CMCCCodecG729",
    "ParseFmtpAttrL",
    "FUNCTIONALITY",
    SetupL, UT_CMCCCodecG729_ParseFmtpAttrLL, Teardown)

EUNIT_TEST(
    "GetFmtpL - test ",
    "CMCCCodecG729",
    "GetFmtpL",
    "FUNCTIONALITY",
    SetupL, UT_CMCCCodecG729_GetFmtpLL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
