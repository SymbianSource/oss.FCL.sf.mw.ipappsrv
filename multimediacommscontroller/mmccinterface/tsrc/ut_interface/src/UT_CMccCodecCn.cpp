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
#include "UT_CMCCCodecCn.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "mmcccodeccn.h"

// CONSTRUCTION
UT_CMccCodecCn* UT_CMccCodecCn::NewL()
    {
    UT_CMccCodecCn* self = UT_CMccCodecCn::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccCodecCn* UT_CMccCodecCn::NewLC()
    {
    UT_CMccCodecCn* self = new( ELeave ) UT_CMccCodecCn();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccCodecCn::~UT_CMccCodecCn()
    {
    }

// Default constructor
UT_CMccCodecCn::UT_CMccCodecCn()
    {
    }

// Second phase construct
void UT_CMccCodecCn::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMccCodecCn::SetupL(  )
    {
    iCodec = CMccCodecCn::NewL();
    }

void UT_CMccCodecCn::Teardown(  )
    {
    delete iCodec;
    }

void UT_CMccCodecCn::UT_CMccCodecCn_NewLL(  )
    {
    EUNIT_ASSERT( ETrue );
    }

void UT_CMccCodecCn::UT_CMccCodecCn_CloneDefaultsLL(  )
    {
    iCodec->SetCodecMode( ENothing );
    CMccCodecInformation* codec;
    codec = iCodec->CloneDefaultsL();
    delete codec;
    codec =NULL;
    }

void UT_CMccCodecCn::UT_CMccCodecCn_CloneDetailedLL(  )
    {
    CMccCodecInformation* codec;
    codec = iCodec->CloneDetailedL();
    delete codec;
    }

void UT_CMccCodecCn::UT_CMccCodecCn_RequireSignallingL(  )
    {
    CMccCodecInformation* codec = CMccCodecCn::NewL();
    EUNIT_ASSERT_EQUALS( iCodec->RequireSignalling( *codec ), EFalse );    
    codec->SetPTime( 40 );
    EUNIT_ASSERT_EQUALS( iCodec->RequireSignalling( *codec ), ETrue );
    codec->SetMaxPTime( 180 );
    EUNIT_ASSERT_EQUALS( iCodec->RequireSignalling( *codec ), ETrue );
    codec->SetPayloadType( KCnPayloadTypeReserved );
    EUNIT_ASSERT_EQUALS( iCodec->RequireSignalling( *codec ), ETrue );
    delete codec;
    }

void UT_CMccCodecCn::UT_CMccCodecCn_EnableVADL(  )
    {
    EUNIT_ASSERT_EQUALS( iCodec->EnableVAD( ETrue ), KErrNone );
    EUNIT_ASSERT_EQUALS( iCodec->EnableVAD( EFalse ), KErrNone );
    }

void UT_CMccCodecCn::UT_CMccCodecCn_SetBitrateL(  )
    {
    EUNIT_ASSERT_EQUALS( iCodec->SetBitrate( 0 ), KErrNone);
    }

void UT_CMccCodecCn::UT_CMccCodecCn_SetSamplingFreqL(  )
    {
    EUNIT_ASSERT_EQUALS( iCodec->SetSamplingFreq( 8000 ), KErrNone);
    }

void UT_CMccCodecCn::UT_CMccCodecCn_SetPTimeL(  )
    {
    EUNIT_ASSERT_EQUALS( iCodec->SetPTime( 9 ), KErrNotSupported);
    EUNIT_ASSERT_EQUALS( iCodec->SetPTime( 20 ), KErrNone);
    }

void UT_CMccCodecCn::UT_CMccCodecCn_SetMaxPTimeL(  )
    {
    EUNIT_ASSERT_EQUALS( iCodec->SetMaxPTime( 9 ), KErrNotSupported);
    EUNIT_ASSERT_EQUALS( iCodec->SetMaxPTime( 20 ), KErrNone);
    }

void UT_CMccCodecCn::UT_CMccCodecCn_SetSdpNameL(  )
    {
    EUNIT_ASSERT_EQUALS( iCodec->SetSdpName( KAMRSdpName ), KErrNotSupported);
    EUNIT_ASSERT_EQUALS( iCodec->SetSdpName( KCnSdpName ), KErrNone);
    }

void UT_CMccCodecCn::UT_CMccCodecCn_SetPayloadTypeL(  )
    {
    EUNIT_ASSERT_EQUALS( iCodec->SetPayloadType( KG729PayloadType ), KErrNotSupported);
    EUNIT_ASSERT_EQUALS( iCodec->SetPayloadType( KCnPayloadType ), KErrNone);
    EUNIT_ASSERT_EQUALS( iCodec->SetPayloadType( KCnPayloadTypeReserved ), KErrNone);
    }

void UT_CMccCodecCn::UT_CMccCodecCn_SetCodecModeL(  )
    {
    EUNIT_ASSERT_EQUALS( iCodec->SetCodecMode( EPCMU ), KErrNotSupported);
    EUNIT_ASSERT_EQUALS( iCodec->SetCodecMode( ENothing ), KErrNone);
    }

void UT_CMccCodecCn::UT_CMccCodecCn_SetComfortNoiseGenerationL(  )
    {
    EUNIT_ASSERT_EQUALS( iCodec->SetComfortNoiseGeneration( 13 ), KErrNotSupported );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccCodecCn,
    "CMccCodecCn test",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMccCodecCn",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecCn_NewLL, Teardown)

EUNIT_TEST(
    "CloneDefaultsL - test ",
    "CMccCodecCn",
    "CloneDefaultsL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecCn_CloneDefaultsLL, Teardown)

EUNIT_TEST(
    "CloneDetailedL - test ",
    "CMccCodecCn",
    "CloneDetailedL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecCn_CloneDetailedLL, Teardown)

EUNIT_TEST(
    "RequireSignalling - test ",
    "CMccCodecCn",
    "RequireSignalling",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecCn_RequireSignallingL, Teardown)

EUNIT_TEST(
    "EnableVAD - test ",
    "CMccCodecCn",
    "EnableVAD",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecCn_EnableVADL, Teardown)

EUNIT_TEST(
    "SetBitrate - test ",
    "CMccCodecCn",
    "SetBitrate",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecCn_SetBitrateL, Teardown)

EUNIT_TEST(
    "SetSamplingFreq - test ",
    "CMccCodecCn",
    "SetSamplingFreq",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecCn_SetSamplingFreqL, Teardown)

EUNIT_TEST(
    "SetPTime - test ",
    "CMccCodecCn",
    "SetPTime",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecCn_SetPTimeL, Teardown)

EUNIT_TEST(
    "SetMaxPTime - test ",
    "CMccCodecCn",
    "SetMaxPTime",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecCn_SetMaxPTimeL, Teardown)

EUNIT_TEST(
    "SetSdpName - test ",
    "CMccCodecCn",
    "SetSdpName",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecCn_SetSdpNameL, Teardown)

EUNIT_TEST(
    "SetPayloadType - test ",
    "CMccCodecCn",
    "SetPayloadType",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecCn_SetPayloadTypeL, Teardown)

EUNIT_TEST(
    "SetCodecMode - test ",
    "CMccCodecCn",
    "SetCodecMode",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecCn_SetCodecModeL, Teardown)

EUNIT_TEST(
    "SetComfortNoiseGeneration - test ",
    "CMccCodecCn",
    "SetComfortNoiseGeneration",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecCn_SetComfortNoiseGenerationL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
