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




// CLASS HEADER
#include "UT_CMCCCodecILBC.h"

// EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>

// INTERNAL INCLUDES
#include "mmcccodecilbc.h"
#include "mccunittestmacros.h"

// LOCAL CONSTANTS
const TUint KIlbcAllowAllBitrates = 0x00FF;

// CONSTRUCTION
UT_CMCCCodecILBC* UT_CMCCCodecILBC::NewL()
    {
    UT_CMCCCodecILBC* self = UT_CMCCCodecILBC::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMCCCodecILBC* UT_CMCCCodecILBC::NewLC()
    {
    UT_CMCCCodecILBC* self = new( ELeave ) UT_CMCCCodecILBC();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMCCCodecILBC::~UT_CMCCCodecILBC()
    {
    }

// Default constructor
UT_CMCCCodecILBC::UT_CMCCCodecILBC()
    {
    }

// Second phase construct
void UT_CMCCCodecILBC::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS
void UT_CMCCCodecILBC::SetupL(  )
    {
    iCodec = CMCCCodecILBC::NewL();
    }

void UT_CMCCCodecILBC::Teardown(  )
    {
    delete iCodec;
    }

void UT_CMCCCodecILBC::UT_CMCCCodecILBC_NewLL(  )
    {
    EUNIT_ASSERT( ETrue );
    }

void UT_CMCCCodecILBC::UT_CMCCCodecILBC_CloneDefaultsLL(  )
    {
    CMccCodecInformation* codec;
    codec = iCodec->CloneDefaultsL();
    delete codec;
    }

void UT_CMCCCodecILBC::UT_CMCCCodecILBC_CloneDetailedLL(  )
    {
    CMccCodecInformation* codec;
    codec = iCodec->CloneDetailedL();
    delete codec;
    }

void UT_CMCCCodecILBC::UT_CMCCCodecILBC_RequireSignallingL(  )
    {
    CMccCodecInformation* codec = CMCCCodecILBC::NewL();
    EUNIT_ASSERT_EQUALS( iCodec->RequireSignalling( *codec ), EFalse );
     
    codec->SetBitrate( KIlbcBitrate20 );    
    codec->SetPTime( 20 );
    codec->SetMaxPTime( 160 );
    EUNIT_ASSERT_EQUALS( iCodec->RequireSignalling( *codec ), ETrue );
    delete codec;
    codec = NULL;
    
    codec = CMCCCodecILBC::NewL();
    codec->SetPayloadType( KPcmaPayloadType );
    EUNIT_ASSERT_EQUALS( iCodec->RequireSignalling( *codec ), EFalse );
    delete codec;
    codec = NULL; 
    }

void UT_CMCCCodecILBC::UT_CMCCCodecILBC_EnableVADL(  )
    {
    EUNIT_ASSERT_EQUALS( iCodec->EnableVAD( ETrue ), KErrNone );
    EUNIT_ASSERT_EQUALS( iCodec->EnableVAD( EFalse ), KErrNone );
    }

void UT_CMCCCodecILBC::UT_CMCCCodecILBC_SetBitrateL(  )
    {
    EUNIT_ASSERT_EQUALS( iCodec->SetBitrate( 32000 ), KErrNotSupported );
    EUNIT_ASSERT_EQUALS( iCodec->SetBitrate( KIlbcBitrate20 ), KErrNone );
    EUNIT_ASSERT_EQUALS( iCodec->SetBitrate( KIlbcBitrate30 ), KErrNone );
    EUNIT_ASSERT_EQUALS( iCodec->SetBitrate( KIlbcBitrate30 + 1 ), KErrNotSupported );
    EUNIT_ASSERT_EQUALS( iCodec->SetBitrate( KIlbcBitrate20 - 1 ), KErrNotSupported );
    }

void UT_CMCCCodecILBC::UT_CMCCCodecILBC_SetAllowedBitratesL()
    {
    // SetAllowedBitrates has 'default' implementation which returns KErrNone
    EUNIT_ASSERT_EQUALS( iCodec->SetAllowedBitrates( KIlbcBitrate20 ), KErrNone );
    EUNIT_ASSERT_EQUALS( iCodec->SetAllowedBitrates( KIlbcBitrate30 ), KErrNone );
    EUNIT_ASSERT_EQUALS( iCodec->SetAllowedBitrates( KIlbcAllowAllBitrates ), KErrNone );
    }

void UT_CMCCCodecILBC::UT_CMCCCodecILBC_SetSamplingFreqL(  )
    {
    EUNIT_ASSERT_EQUALS( iCodec->SetSamplingFreq( 16000 ), KErrNotSupported);
    EUNIT_ASSERT_EQUALS( iCodec->SetSamplingFreq( 8000 ), KErrNone);
    }

void UT_CMCCCodecILBC::UT_CMCCCodecILBC_SetSdpNameL(  )
    {
    EUNIT_ASSERT_EQUALS( iCodec->SetSdpName( KAMRSdpName ), KErrNotSupported);
    EUNIT_ASSERT_EQUALS( iCodec->SetSdpName( KILBCSdpName ), KErrNone);
    }

void UT_CMCCCodecILBC::UT_CMCCCodecILBC_SetPayloadTypeL(  )
    {
    EUNIT_ASSERT_EQUALS( iCodec->SetPayloadType( KG729PayloadType ), KErrArgument);
    EUNIT_ASSERT_EQUALS( iCodec->SetPayloadType( KDefaultIlbcPT ), KErrNone);
    }

void UT_CMCCCodecILBC::UT_CMCCCodecILBC_SetCodecModeL(  )
    {
    EUNIT_ASSERT_EQUALS( iCodec->SetCodecMode( ENothing ), KErrNone);
    EUNIT_ASSERT_EQUALS( iCodec->SetCodecMode( EPCMU ), KErrNotSupported);
    }
    
void UT_CMCCCodecILBC::UT_CMCCCodecILBC_CreateFmtpAttrListLL(  )
    {
    iCodec->CreateFmtpAttrListL();
    iCodec->SetBitrate( KIlbcBitrate30 );
    iCodec->CreateFmtpAttrListL();
    }

void UT_CMCCCodecILBC::UT_CMCCCodecILBC_ParseFmtpAttrLL(  )
    {
    _LIT8( KIlbcMode20ms, "mode=20" );
    _LIT8( KIlbcMode30ms, "mode=30" );
    EUNIT_ASSERT_NO_LEAVE( iCodec->ParseFmtpAttrL( KIlbcMode20ms ) );
    EUNIT_ASSERT_NO_LEAVE( iCodec->ParseFmtpAttrL( KIlbcMode30ms ) );
    _LIT8( KAnnexTest, "annextest=xx" );
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iCodec->ParseFmtpAttrL( KAnnexTest ), KErrArgument );
    _LIT8( KAnnexTest2, "" );
    EUNIT_ASSERT_NO_LEAVE( iCodec->ParseFmtpAttrL( KAnnexTest2 ) );
    }

void UT_CMCCCodecILBC::UT_CMCCCodecILBC_GetFmtpLL(  )
    {
    iCodec->GetFmtpL();
    iCodec->GetFmtpL();
    }

void UT_CMCCCodecILBC::UT_CMCCCodecILBC_SetMaxPTimeL(  )
    {
    EUNIT_ASSERT_EQUALS( iCodec->SetMaxPTime( 180 ), KErrNone);
    EUNIT_ASSERT_EQUALS( iCodec->SetMaxPTime( 9 ), KErrArgument);
    EUNIT_ASSERT_EQUALS( iCodec->SetMaxPTime( 160 ), KErrArgument);
    
    EUNIT_ASSERT_EQUALS( iCodec->SetMaxPTime( 210 ), KErrArgument);
    
    iCodec->SetBitrate( KIlbcBitrate20 );    
    iCodec->SetPTime( 20 );
    EUNIT_ASSERT_EQUALS( iCodec->SetMaxPTime( 160 ), KErrNone);
    EUNIT_ASSERT_EQUALS( iCodec->SetMaxPTime( 220 ), KErrArgument);
    }

void UT_CMCCCodecILBC::UT_CMCCCodecILBC_SetPTimeL(  )
    {
    EUNIT_ASSERT_EQUALS( iCodec->SetPTime( 210 ), KErrArgument);
    EUNIT_ASSERT_EQUALS( iCodec->SetPTime( 9 ), KErrArgument);
    
    // When mode=30, ptime 20 setting is ignored
    EUNIT_ASSERT_EQUALS( iCodec->SetPTime( 20 ), KErrNone);
    EUNIT_ASSERT_EQUALS( iCodec->PTime(), 30 );
    
    EUNIT_ASSERT_EQUALS( iCodec->SetPTime( 60 ), KErrNone);
    iCodec->SetBitrate( KIlbcBitrate20 );
    EUNIT_ASSERT_EQUALS( iCodec->SetPTime( 80 ), KErrNone);
    }

void UT_CMCCCodecILBC::UT_CMCCCodecILBC_SetComfortNoiseGenerationL(  )
    {
    EUNIT_ASSERT_EQUALS( iCodec->SetComfortNoiseGeneration( 13 ), KErrNone);
    EUNIT_ASSERT_EQUALS( iCodec->SetComfortNoiseGeneration( 19 ), KErrNone);
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMCCCodecILBC,
    "CMCCCodecILBC test",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMCCCodecILBC",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CMCCCodecILBC_NewLL, Teardown)

EUNIT_TEST(
    "CloneDefaultsL - test ",
    "CMCCCodecILBC",
    "CloneDefaultsL",
    "FUNCTIONALITY",
    SetupL, UT_CMCCCodecILBC_CloneDefaultsLL, Teardown)

EUNIT_TEST(
    "CloneDetailedL - test ",
    "CMCCCodecILBC",
    "CloneDetailedL",
    "FUNCTIONALITY",
    SetupL, UT_CMCCCodecILBC_CloneDetailedLL, Teardown)

EUNIT_TEST(
    "RequireSignalling - test ",
    "CMCCCodecILBC",
    "RequireSignalling",
    "FUNCTIONALITY",
    SetupL, UT_CMCCCodecILBC_RequireSignallingL, Teardown)

EUNIT_TEST(
    "EnableVAD - test ",
    "CMCCCodecILBC",
    "EnableVAD",
    "FUNCTIONALITY",
    SetupL, UT_CMCCCodecILBC_EnableVADL, Teardown)

EUNIT_TEST(
    "SetBitrate - test ",
    "CMCCCodecILBC",
    "SetBitrate",
    "FUNCTIONALITY",
    SetupL, UT_CMCCCodecILBC_SetBitrateL, Teardown)

EUNIT_TEST(
    "SetAllowedBitrates - test ",
    "CMCCCodecILBC",
    "SetAllowedBitrates",
    "FUNCTIONALITY",
    SetupL, UT_CMCCCodecILBC_SetAllowedBitratesL, Teardown)
    
EUNIT_TEST(
    "SetSamplingFreq - test ",
    "CMCCCodecILBC",
    "SetSamplingFreq",
    "FUNCTIONALITY",
    SetupL, UT_CMCCCodecILBC_SetSamplingFreqL, Teardown)

EUNIT_TEST(
    "SetSdpName - test ",
    "CMCCCodecILBC",
    "SetSdpName",
    "FUNCTIONALITY",
    SetupL, UT_CMCCCodecILBC_SetSdpNameL, Teardown)

EUNIT_TEST(
    "SetPayloadType - test ",
    "CMCCCodecILBC",
    "SetPayloadType",
    "FUNCTIONALITY",
    SetupL, UT_CMCCCodecILBC_SetPayloadTypeL, Teardown)

EUNIT_TEST(
    "SetCodecMode - test ",
    "CMCCCodecILBC",
    "SetCodecMode",
    "FUNCTIONALITY",
    SetupL, UT_CMCCCodecILBC_SetCodecModeL, Teardown)

EUNIT_TEST(
    "CreateFmtpAttrListL - test ",
    "CMCCCodecILBC",
    "CreateFmtpAttrListL",
    "FUNCTIONALITY",
    SetupL, UT_CMCCCodecILBC_CreateFmtpAttrListLL, Teardown)

EUNIT_TEST(
    "ParseFmtpAttrL - test ",
    "CMCCCodecILBC",
    "ParseFmtpAttrL",
    "FUNCTIONALITY",
    SetupL, UT_CMCCCodecILBC_ParseFmtpAttrLL, Teardown)

EUNIT_TEST(
    "GetFmtpL - test ",
    "CMCCCodecILBC",
    "GetFmtpL",
    "FUNCTIONALITY",
    SetupL, UT_CMCCCodecILBC_GetFmtpLL, Teardown)

EUNIT_TEST(
    "SetMaxPTime - test ",
    "CMCCCodecILBC",
    "SetMaxPTime",
    "FUNCTIONALITY",
    SetupL, UT_CMCCCodecILBC_SetMaxPTimeL, Teardown)

EUNIT_TEST(
    "SetPTime - test ",
    "CMCCCodecILBC",
    "SetPTime",
    "FUNCTIONALITY",
    SetupL, UT_CMCCCodecILBC_SetPTimeL, Teardown)

EUNIT_TEST(
    "SetComfortNoiseGeneration - test ",
    "CMCCCodecILBC",
    "SetComfortNoiseGeneration",
    "FUNCTIONALITY",
    SetupL, UT_CMCCCodecILBC_SetComfortNoiseGenerationL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
