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
#include "UT_CMccCnGenerator.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <mmf/server/sounddevice.h>

//  INTERNAL INCLUDES
#include "mcccngenerator.h"
#include "mccinternalcodecs.h"


// CONSTRUCTION
UT_CMccCnGenerator* UT_CMccCnGenerator::NewL()
    {
    UT_CMccCnGenerator* self = UT_CMccCnGenerator::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMccCnGenerator* UT_CMccCnGenerator::NewLC()
    {
    UT_CMccCnGenerator* self = new( ELeave ) UT_CMccCnGenerator();
    CleanupStack::PushL( self );

	self->ConstructL();
    return self;
    }

// Destructor (virtual by CBase)
UT_CMccCnGenerator::~UT_CMccCnGenerator()
    {
    }

// Default constructor
UT_CMccCnGenerator::UT_CMccCnGenerator() :
    iCnGenerator( NULL )
    {
    }

// Second phase construct
void UT_CMccCnGenerator::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void UT_CMccCnGenerator::SetupL()
    {
    iFourCC = KMccFourCCIdAMRNB;
    CMMFDevSound* devSoundDummy( NULL );
    iCnGenerator = CMccCnGenerator::NewL( iFourCC, *devSoundDummy );
    }

void UT_CMccCnGenerator::ReSetupL( TUint32 aFourCC )
    {
    if ( iCnGenerator )
        {
        delete iCnGenerator;
        iCnGenerator = NULL;
        }

    CMMFDevSound* devSoundDummy( NULL );
    iCnGenerator = CMccCnGenerator::NewL( aFourCC, *devSoundDummy );
    }

void UT_CMccCnGenerator::Teardown()
    {
    if ( iCnGenerator )
        {
        delete iCnGenerator;
        iCnGenerator = NULL;
        }
    }

void UT_CMccCnGenerator::UT_CMccCnGenerator_GenerateSidPacketL()
    {
    // A huge buffer is needed, because 7*162 bytes of sid are created
    TBuf8<1250> buf;
    // For comparison
    TBuf8<1250> buf2;
    
    iCnGenerator->GenerateSidPacket( buf, 200 );
    
    TInt result = buf.Compare( buf2 );
    
    if ( result == 0 )
        {
        EUNIT_ASSERT( EFalse );
        }
    }

void UT_CMccCnGenerator::UT_CMccCnGenerator_ConcealErrorForNextFrameL()
    {
    // Nothing to verify
    iCnGenerator->ConcealErrorForNextFrame();
    }

void UT_CMccCnGenerator::UT_CMccCnGenerator_DtxPeriodStatusL()
    {
    // Nothing to verify
    TBool dummy( iCnGenerator->DtxPeriodStatus() );
    }

void UT_CMccCnGenerator::UT_CMccCnGenerator_DoDtxDecisionL()
    {
    HBufC8* buf = HBufC8::NewL( 0 );
    iCnGenerator->DoDtxDecision( buf->Des() );
    delete buf;
    buf = NULL;
    
    _LIT8( KText, "foo42" );
    buf = KText().AllocL();

    // Reach more branches ...
    iCnGenerator->DoDtxDecision( buf->Des() );

    delete buf;
    buf = NULL;
    
    _LIT8( KText2, "%c" );
    TBuf8<3> buf2;
    
    const TUint8 KSidMaskForAmrNb = 0x80;
    
    buf2.AppendFormat( KText2, KSidMaskForAmrNb );
    buf = buf2.AllocL();

    iCnGenerator->DoDtxDecision( buf->Des() );
    delete buf;
    }

void UT_CMccCnGenerator::UT_CMccCnGenerator_IsSidBufferL()
    {
    TBuf8<3> buf;
    buf.Append( 42 );
    TBool result( EFalse );

    result = iCnGenerator->IsSidBuffer( buf );
    EUNIT_ASSERT( !result );

    const TUint8 KSidValForAmrNb = 0x40;
    buf[0] = KSidValForAmrNb;

    result = iCnGenerator->IsSidBuffer( buf );
    EUNIT_ASSERT( result );

    // Reach more branches
    this->ReSetupL( KMccFourCCIdG711 );

    _LIT8( KText, "%c%c" );
    buf.Format( KText, 0, 0 );

    result = iCnGenerator->IsSidBuffer( buf );
    EUNIT_ASSERT( !result );

    buf.Format( KText, 2, 0 );

    result = iCnGenerator->IsSidBuffer( buf );
    // Result should be true
    EUNIT_ASSERT( result );
    
    this->ReSetupL( KMccFourCCIdAMRWB );
    
    result = iCnGenerator->IsSidBuffer( buf );
    EUNIT_ASSERT( !result );

    const TUint8 KSidValForAmrWb = 0x48;
    buf[0] = KSidValForAmrWb;

    result = iCnGenerator->IsSidBuffer( buf );
    EUNIT_ASSERT( result );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMccCnGenerator,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "GenerateSidPacket - test ",
    "CMccCnGenerator",
    "GenerateSidPacket",
    "FUNCTIONALITY",
    SetupL, UT_CMccCnGenerator_GenerateSidPacketL, Teardown)

EUNIT_TEST(
    "ConcealErrorForNextFrameL - test ",
    "CMccCnGenerator",
    "ConcealErrorForNextFrameL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCnGenerator_ConcealErrorForNextFrameL, Teardown)

EUNIT_TEST(
    "DtxPeriodStatus - test ",
    "CMccCnGenerator",
    "DtxPeriodStatus",
    "FUNCTIONALITY",
    SetupL, UT_CMccCnGenerator_DtxPeriodStatusL, Teardown)

EUNIT_TEST(
    "DoDtxDecisionL - test ",
    "CMccCnGenerator",
    "DoDtxDecisionL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCnGenerator_DoDtxDecisionL, Teardown)
    

EUNIT_TEST(
    "IsSidBufferL - test ",
    "CMccCnGenerator",
    "IsSidBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCnGenerator_IsSidBufferL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
