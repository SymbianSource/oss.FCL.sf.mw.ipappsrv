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
#include "UT_CMccCodecAAC.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "mmcccodecaac.h"

// CONSTRUCTION
UT_CMccCodecAAC* UT_CMccCodecAAC::NewL()
    {
    UT_CMccCodecAAC* self = UT_CMccCodecAAC::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccCodecAAC* UT_CMccCodecAAC::NewLC()
    {
    UT_CMccCodecAAC* self = new( ELeave ) UT_CMccCodecAAC();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccCodecAAC::~UT_CMccCodecAAC()
    {
    }

// Default constructor
UT_CMccCodecAAC::UT_CMccCodecAAC()
    {
    }

// Second phase construct
void UT_CMccCodecAAC::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMccCodecAAC::SetupL(  )
    {
    iCodec = CMccCodecAAC::NewL();
    }

void UT_CMccCodecAAC::Teardown(  )
    {
    delete iCodec;
    }

void UT_CMccCodecAAC::UT_CMccCodecAAC_RequireSignallingL(  )
    {
    CMccCodecAAC* codecinfo = CMccCodecAAC::NewL();
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
    CleanupStack::PopAndDestroy(codecinfo);    }

void UT_CMccCodecAAC::UT_CMccCodecAAC_CloneDefaultsLL(  )
    {
    CMccCodecInformation* codecinfo = iCodec->CloneDefaultsL();
    delete codecinfo;  
    }

void UT_CMccCodecAAC::UT_CMccCodecAAC_CloneDetailedLL(  )
    {
    CMccCodecInformation* codecinfo = iCodec->CloneDetailedL();
    delete codecinfo; 
    }

void UT_CMccCodecAAC::UT_CMccCodecAAC_EnableVADL(  )
    {
    EUNIT_ASSERT( iCodec->EnableVAD(ETrue) == KErrNotSupported  );
    }

void UT_CMccCodecAAC::UT_CMccCodecAAC_SetBitrateL(  )
    {
    EUNIT_ASSERT( iCodec->SetBitrate(64000) == KErrNone );
    EUNIT_ASSERT( iCodec->SetBitrate(0) == KErrNotSupported );
    }

void UT_CMccCodecAAC::UT_CMccCodecAAC_SetSamplingFreqL(  )
    {
    EUNIT_ASSERT( iCodec->SetSamplingFreq(64000) ==  KErrNone );
    }

void UT_CMccCodecAAC::UT_CMccCodecAAC_SetPTimeL(  )
    {
    EUNIT_ASSERT( iCodec->SetPTime(300) ==  KErrNotSupported );
    }

void UT_CMccCodecAAC::UT_CMccCodecAAC_SetMaxPTimeL(  )
    {
    EUNIT_ASSERT( iCodec->SetMaxPTime(300) ==  KErrNotSupported );
    }

void UT_CMccCodecAAC::UT_CMccCodecAAC_SetSdpNameL(  )
    {
    EUNIT_ASSERT( iCodec->SetSdpName( KAACSdpName ) == KErrNone);
    EUNIT_ASSERT( iCodec->SetSdpName( KAVCSdpName ) == KErrNotSupported);
    }

void UT_CMccCodecAAC::UT_CMccCodecAAC_SetPayloadTypeL(  )
    {
    EUNIT_ASSERT( iCodec->SetPayloadType(100) == KErrNone );
    EUNIT_ASSERT( iCodec->SetPayloadType(20) == KErrNotSupported );
    }

void UT_CMccCodecAAC::UT_CMccCodecAAC_SetCodecModeL(  )
    {
    EUNIT_ASSERT( iCodec->SetCodecMode(2) == KErrNone );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccCodecAAC,
    "CMccCodecAAC test",
    "UNIT" )

EUNIT_TEST(
    "RequireSignalling - test ",
    "CMccCodecAAC",
    "RequireSignalling",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAAC_RequireSignallingL, Teardown)

EUNIT_TEST(
    "CloneDefaultsL - test ",
    "CMccCodecAAC",
    "CloneDefaultsL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAAC_CloneDefaultsLL, Teardown)

EUNIT_TEST(
    "CloneDetailedL - test ",
    "CMccCodecAAC",
    "CloneDetailedL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAAC_CloneDetailedLL, Teardown)

EUNIT_TEST(
    "EnableVAD - test ",
    "CMccCodecAAC",
    "EnableVAD",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAAC_EnableVADL, Teardown)

EUNIT_TEST(
    "SetBitrate - test ",
    "CMccCodecAAC",
    "SetBitrate",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAAC_SetBitrateL, Teardown)

EUNIT_TEST(
    "SetSamplingFreq - test ",
    "CMccCodecAAC",
    "SetSamplingFreq",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAAC_SetSamplingFreqL, Teardown)

EUNIT_TEST(
    "SetPTime - test ",
    "CMccCodecAAC",
    "SetPTime",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAAC_SetPTimeL, Teardown)

EUNIT_TEST(
    "SetMaxPTime - test ",
    "CMccCodecAAC",
    "SetMaxPTime",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAAC_SetMaxPTimeL, Teardown)

EUNIT_TEST(
    "SetSdpName - test ",
    "CMccCodecAAC",
    "SetSdpName",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAAC_SetSdpNameL, Teardown)

EUNIT_TEST(
    "SetPayloadType - test ",
    "CMccCodecAAC",
    "SetPayloadType",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAAC_SetPayloadTypeL, Teardown)

EUNIT_TEST(
    "SetCodecMode - test ",
    "CMccCodecAAC",
    "SetCodecMode",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAAC_SetCodecModeL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
