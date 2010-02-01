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
#include "UT_CMCCCodecG711.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "mmcccodecg711.h"

// CONSTRUCTION
UT_CMCCCodecG711* UT_CMCCCodecG711::NewL()
    {
    UT_CMCCCodecG711* self = UT_CMCCCodecG711::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMCCCodecG711* UT_CMCCCodecG711::NewLC()
    {
    UT_CMCCCodecG711* self = new( ELeave ) UT_CMCCCodecG711();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMCCCodecG711::~UT_CMCCCodecG711()
    {
    }

// Default constructor
UT_CMCCCodecG711::UT_CMCCCodecG711()
    {
    }

// Second phase construct
void UT_CMCCCodecG711::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMCCCodecG711::SetupL(  )
    {
    iCodec = CMCCCodecG711::NewL();
    }

void UT_CMCCCodecG711::Teardown(  )
    {
    delete iCodec;
    }

void UT_CMCCCodecG711::UT_CMCCCodecG711_NewLL(  )
    {
    EUNIT_ASSERT( ETrue );
    }

void UT_CMCCCodecG711::UT_CMCCCodecG711_CloneDefaultsLL(  )
    {
    iCodec->SetCodecMode( EPCMA );
    CMccCodecInformation* codec;
    codec = iCodec->CloneDefaultsL();
    delete codec;
    codec =NULL;
    
    iCodec->SetCodecMode( EPCMU );
    codec = iCodec->CloneDefaultsL();
    delete codec;
    }

void UT_CMCCCodecG711::UT_CMCCCodecG711_CloneDetailedLL(  )
    {
    CMccCodecInformation* codec;
    codec = iCodec->CloneDetailedL();
    delete codec;
    }

void UT_CMCCCodecG711::UT_CMCCCodecG711_RequireSignallingL(  )
    {
    CMccCodecInformation* codec = CMCCCodecG711::NewL();
    EUNIT_ASSERT_EQUALS( iCodec->RequireSignalling( *codec ), EFalse );    
    codec->SetPTime( 40 );
    EUNIT_ASSERT_EQUALS( iCodec->RequireSignalling( *codec ), ETrue );
    codec->SetMaxPTime( 180 );
    EUNIT_ASSERT_EQUALS( iCodec->RequireSignalling( *codec ), ETrue );
    codec->SetPayloadType( KPcmaPayloadType );
    EUNIT_ASSERT_EQUALS( iCodec->RequireSignalling( *codec ), ETrue );
    delete codec;
    }

void UT_CMCCCodecG711::UT_CMCCCodecG711_EnableVADL(  )
    {
    EUNIT_ASSERT_EQUALS( iCodec->EnableVAD( ETrue ), KErrNone );
    EUNIT_ASSERT_EQUALS( iCodec->EnableVAD( EFalse ), KErrNone );
    }

void UT_CMCCCodecG711::UT_CMCCCodecG711_SetBitrateL(  )
    {
    EUNIT_ASSERT_EQUALS( iCodec->SetBitrate( 32000 ), KErrNotSupported);
    EUNIT_ASSERT_EQUALS( iCodec->SetBitrate( KG711Bitrate64 ), KErrNone);
    }

void UT_CMCCCodecG711::UT_CMCCCodecG711_SetSamplingFreqL(  )
    {
    EUNIT_ASSERT_EQUALS( iCodec->SetSamplingFreq( 16000 ), KErrNotSupported);
    EUNIT_ASSERT_EQUALS( iCodec->SetSamplingFreq( 8000 ), KErrNone);
    }

void UT_CMCCCodecG711::UT_CMCCCodecG711_SetPTimeL(  )
    {
    EUNIT_ASSERT_EQUALS( iCodec->SetPTime( 9 ), KErrArgument);
    EUNIT_ASSERT_EQUALS( iCodec->SetPTime( 20 ), KErrNone);
    }

void UT_CMCCCodecG711::UT_CMCCCodecG711_SetMaxPTimeL(  )
    {
    EUNIT_ASSERT_EQUALS( iCodec->SetMaxPTime( 9 ), KErrArgument);
    EUNIT_ASSERT_EQUALS( iCodec->SetMaxPTime( 20 ), KErrNone);
    }

void UT_CMCCCodecG711::UT_CMCCCodecG711_SetSdpNameL(  )
    {
    EUNIT_ASSERT_EQUALS( iCodec->SetSdpName( KAMRSdpName ), KErrNotSupported);
    EUNIT_ASSERT_EQUALS( iCodec->SetSdpName( KPCMASdpName ), KErrNone);
    EUNIT_ASSERT_EQUALS( iCodec->SetSdpName( KPCMUSdpName ), KErrNone);
    }

void UT_CMCCCodecG711::UT_CMCCCodecG711_SetPayloadTypeL(  )
    {
    EUNIT_ASSERT_EQUALS( iCodec->SetPayloadType( KG729PayloadType ), KErrNotSupported);
    EUNIT_ASSERT_EQUALS( iCodec->SetPayloadType( KPcmuPayloadType ), KErrNone);
    EUNIT_ASSERT_EQUALS( iCodec->SetPayloadType( KPcmaPayloadType ), KErrNone);
    }

void UT_CMCCCodecG711::UT_CMCCCodecG711_SetCodecModeL(  )
    {
    EUNIT_ASSERT_EQUALS( iCodec->SetCodecMode( ENothing ), KErrNotSupported);
    EUNIT_ASSERT_EQUALS( iCodec->SetCodecMode( EPCMU ), KErrNone);
    EUNIT_ASSERT_EQUALS( iCodec->SetCodecMode( EPCMA ), KErrNone);
    }

void UT_CMCCCodecG711::UT_CMCCCodecG711_SetComfortNoiseGenerationL(  )
    {
    EUNIT_ASSERT_EQUALS( iCodec->SetComfortNoiseGeneration( 13 ), KErrNone);
    EUNIT_ASSERT_EQUALS( iCodec->SetComfortNoiseGeneration( 19 ), KErrNone);
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMCCCodecG711,
    "CMCCCodecG711 test",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMCCCodecG711",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CMCCCodecG711_NewLL, Teardown)

EUNIT_TEST(
    "CloneDefaultsL - test ",
    "CMCCCodecG711",
    "CloneDefaultsL",
    "FUNCTIONALITY",
    SetupL, UT_CMCCCodecG711_CloneDefaultsLL, Teardown)

EUNIT_TEST(
    "CloneDetailedL - test ",
    "CMCCCodecG711",
    "CloneDetailedL",
    "FUNCTIONALITY",
    SetupL, UT_CMCCCodecG711_CloneDetailedLL, Teardown)

EUNIT_TEST(
    "RequireSignalling - test ",
    "CMCCCodecG711",
    "RequireSignalling",
    "FUNCTIONALITY",
    SetupL, UT_CMCCCodecG711_RequireSignallingL, Teardown)

EUNIT_TEST(
    "EnableVAD - test ",
    "CMCCCodecG711",
    "EnableVAD",
    "FUNCTIONALITY",
    SetupL, UT_CMCCCodecG711_EnableVADL, Teardown)

EUNIT_TEST(
    "SetBitrate - test ",
    "CMCCCodecG711",
    "SetBitrate",
    "FUNCTIONALITY",
    SetupL, UT_CMCCCodecG711_SetBitrateL, Teardown)

EUNIT_TEST(
    "SetSamplingFreq - test ",
    "CMCCCodecG711",
    "SetSamplingFreq",
    "FUNCTIONALITY",
    SetupL, UT_CMCCCodecG711_SetSamplingFreqL, Teardown)

EUNIT_TEST(
    "SetPTime - test ",
    "CMCCCodecG711",
    "SetPTime",
    "FUNCTIONALITY",
    SetupL, UT_CMCCCodecG711_SetPTimeL, Teardown)

EUNIT_TEST(
    "SetMaxPTime - test ",
    "CMCCCodecG711",
    "SetMaxPTime",
    "FUNCTIONALITY",
    SetupL, UT_CMCCCodecG711_SetMaxPTimeL, Teardown)

EUNIT_TEST(
    "SetSdpName - test ",
    "CMCCCodecG711",
    "SetSdpName",
    "FUNCTIONALITY",
    SetupL, UT_CMCCCodecG711_SetSdpNameL, Teardown)

EUNIT_TEST(
    "SetPayloadType - test ",
    "CMCCCodecG711",
    "SetPayloadType",
    "FUNCTIONALITY",
    SetupL, UT_CMCCCodecG711_SetPayloadTypeL, Teardown)

EUNIT_TEST(
    "SetCodecMode - test ",
    "CMCCCodecG711",
    "SetCodecMode",
    "FUNCTIONALITY",
    SetupL, UT_CMCCCodecG711_SetCodecModeL, Teardown)

EUNIT_TEST(
    "SetComfortNoiseGeneration - test ",
    "CMCCCodecG711",
    "SetComfortNoiseGeneration",
    "FUNCTIONALITY",
    SetupL, UT_CMCCCodecG711_SetComfortNoiseGenerationL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
