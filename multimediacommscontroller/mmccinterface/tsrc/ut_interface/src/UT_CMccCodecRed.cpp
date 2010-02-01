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
#include "UT_CMccCodecRed.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include "mccunittestmacros.h"


//  INTERNAL INCLUDES
#include "mmcccodecred.h"

// CONSTRUCTION
UT_CMccCodecRed* UT_CMccCodecRed::NewL()
    {
    UT_CMccCodecRed* self = UT_CMccCodecRed::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccCodecRed* UT_CMccCodecRed::NewLC()
    {
    UT_CMccCodecRed* self = new( ELeave ) UT_CMccCodecRed();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccCodecRed::~UT_CMccCodecRed()
    {
    }

// Default constructor
UT_CMccCodecRed::UT_CMccCodecRed()
    {
    }

// Second phase construct
void UT_CMccCodecRed::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMccCodecRed::SetupL(  )
    {
    iCodec = CMccCodecRed::NewL();
    }

void UT_CMccCodecRed::Teardown(  )
    {
    delete iCodec;
    }

void UT_CMccCodecRed::UT_CMccCodecRed_RequireSignallingL(  )
    {
    CMccCodecRed* codecinfo = CMccCodecRed::NewL();
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
    EUNIT_ASSERT(!iCodec->RequireSignalling(*codecinfo));
    CleanupStack::PopAndDestroy(codecinfo);       
    }

void UT_CMccCodecRed::UT_CMccCodecRed_CloneDefaultsLL(  )
    {
    CMccCodecInformation* codecinfo = iCodec->CloneDefaultsL();
    delete codecinfo; 
    }

void UT_CMccCodecRed::UT_CMccCodecRed_CloneDetailedLL(  )
    {
    CMccCodecInformation* codecinfo = iCodec->CloneDetailedL();
    delete codecinfo; 
    }

void UT_CMccCodecRed::UT_CMccCodecRed_EnableVADL(  )
    {
    EUNIT_ASSERT( iCodec->EnableVAD(ETrue) == KErrNone  );
    }


void UT_CMccCodecRed::UT_CMccCodecRed_SetBitrateL(  )
    {
    EUNIT_ASSERT( iCodec->SetBitrate(0) == KErrNone );
    }

void UT_CMccCodecRed::UT_CMccCodecRed_SetSamplingFreqL(  )
    {
    EUNIT_ASSERT( iCodec->SetSamplingFreq(64000) ==  KErrNone );
    }

void UT_CMccCodecRed::UT_CMccCodecRed_SetPTimeL(  )
    {
    EUNIT_ASSERT( iCodec->SetPTime(300) ==  KErrNotSupported );
    }

void UT_CMccCodecRed::UT_CMccCodecRed_SetMaxPTimeL(  )
    {
    EUNIT_ASSERT( iCodec->SetMaxPTime(300) ==  KErrNotSupported );
    }

void UT_CMccCodecRed::UT_CMccCodecRed_SetSdpNameL(  )
    {
    EUNIT_ASSERT( iCodec->SetSdpName( KRedSdpName ) == KErrNone);
    EUNIT_ASSERT( iCodec->SetSdpName( KAVCSdpName ) == KErrNotSupported);
    }

void UT_CMccCodecRed::UT_CMccCodecRed_SetPayloadTypeL(  )
    {
    EUNIT_ASSERT( iCodec->SetPayloadType(100) == KErrNone );
    EUNIT_ASSERT( iCodec->SetPayloadType(20) == KErrNotSupported );
    }

void UT_CMccCodecRed::UT_CMccCodecRed_SetCodecModeL(  )
    {
    EUNIT_ASSERT( iCodec->SetCodecMode(2) == KErrNotSupported );
    }

void UT_CMccCodecRed::UT_CMccCodecRed_ParseFmtpAttrLL(  )
    {
    TBuf8<20> fmtp;
    fmtp.Copy( _L8("0/1") );
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iCodec->ParseFmtpAttrL( fmtp ), KErrNotSupported );
    
    fmtp.Copy( _L8("0/0") );
    iCodec->ParseFmtpAttrL( fmtp );
    const RArray<TUint>& redpayloads = iCodec->RedPayloads();
    EUNIT_ASSERT( redpayloads.Count() == 2 );
    EUNIT_ASSERT( redpayloads[ 0 ] == 0 );
    EUNIT_ASSERT( redpayloads[ 1 ] == 0 );
    }

void UT_CMccCodecRed::UT_CMccCodecRed_SetRedPayloadsLL(  )
    {
    RArray<TUint> redpayloads; 
    CleanupClosePushL(redpayloads);
    User::LeaveIfError(redpayloads.Append(1));
    User::LeaveIfError(redpayloads.Append(2));   
    iCodec->SetRedPayloadsL(redpayloads);
    EUNIT_ASSERT( iCodec->RedPayloads().Count() == 2 );
    CleanupStack::PopAndDestroy( &redpayloads );
    }

void UT_CMccCodecRed::UT_CMccCodecRed_RedPayloadsL(  )
    {
    EUNIT_ASSERT( ETrue );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccCodecRed,
    "CMccCodecRed test",
    "UNIT" )

EUNIT_TEST(
    "RequireSignalling - test ",
    "CMccCodecRed",
    "RequireSignalling",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecRed_RequireSignallingL, Teardown)

EUNIT_TEST(
    "CloneDefaultsL - test ",
    "CMccCodecRed",
    "CloneDefaultsL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecRed_CloneDefaultsLL, Teardown)

EUNIT_TEST(
    "CloneDetailedL - test ",
    "CMccCodecRed",
    "CloneDetailedL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecRed_CloneDetailedLL, Teardown)

EUNIT_TEST(
    "EnableVAD - test ",
    "CMccCodecRed",
    "EnableVAD",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecRed_EnableVADL, Teardown)


EUNIT_TEST(
    "SetBitrate - test ",
    "CMccCodecRed",
    "SetBitrate",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecRed_SetBitrateL, Teardown)

EUNIT_TEST(
    "SetSamplingFreq - test ",
    "CMccCodecRed",
    "SetSamplingFreq",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecRed_SetSamplingFreqL, Teardown)

EUNIT_TEST(
    "SetPTime - test ",
    "CMccCodecRed",
    "SetPTime",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecRed_SetPTimeL, Teardown)

EUNIT_TEST(
    "SetMaxPTime - test ",
    "CMccCodecRed",
    "SetMaxPTime",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecRed_SetMaxPTimeL, Teardown)

EUNIT_TEST(
    "SetSdpName - test ",
    "CMccCodecRed",
    "SetSdpName",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecRed_SetSdpNameL, Teardown)

EUNIT_TEST(
    "SetPayloadType - test ",
    "CMccCodecRed",
    "SetPayloadType",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecRed_SetPayloadTypeL, Teardown)

EUNIT_TEST(
    "SetCodecMode - test ",
    "CMccCodecRed",
    "SetCodecMode",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecRed_SetCodecModeL, Teardown)

EUNIT_TEST(
    "ParseFmtpAttrL - test ",
    "CMccCodecRed",
    "ParseFmtpAttrL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecRed_ParseFmtpAttrLL, Teardown)

EUNIT_TEST(
    "SetRedPayloadsL - test ",
    "CMccCodecRed",
    "SetRedPayloadsL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecRed_SetRedPayloadsLL, Teardown)

EUNIT_TEST(
    "RedPayloads - test ",
    "CMccCodecRed",
    "RedPayloads",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecRed_RedPayloadsL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
