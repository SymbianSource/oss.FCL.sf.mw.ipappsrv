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
#include "UT_CMccCodecAVC.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "mmcccodecavc.h"
#include "mccunittestmacros.h"

// CONSTRUCTION
UT_CMccCodecAVC* UT_CMccCodecAVC::NewL()
    {
    UT_CMccCodecAVC* self = UT_CMccCodecAVC::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccCodecAVC* UT_CMccCodecAVC::NewLC()
    {
    UT_CMccCodecAVC* self = new( ELeave ) UT_CMccCodecAVC();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccCodecAVC::~UT_CMccCodecAVC()
    {
    }

// Default constructor
UT_CMccCodecAVC::UT_CMccCodecAVC()
    {
    }

// Second phase construct
void UT_CMccCodecAVC::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMccCodecAVC::SetupL(  )
    {
    iCodec = CMccCodecAVC::NewL();
    }

void UT_CMccCodecAVC::Teardown(  )
    {
    delete iCodec;
    }

void UT_CMccCodecAVC::UT_CMccCodecAVC_RequireSignallingL(  )
    {
    CMccCodecAVC* codecinfo = CMccCodecAVC::NewL();
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

void UT_CMccCodecAVC::UT_CMccCodecAVC_CloneDefaultsLL(  )
    {
    CMccCodecInformation* codecinfo = iCodec->CloneDefaultsL();
    delete codecinfo;
    }

void UT_CMccCodecAVC::UT_CMccCodecAVC_CloneDetailedLL(  )
    {
    CMccCodecInformation* codecinfo = iCodec->CloneDetailedL();
    delete codecinfo;   
    }

void UT_CMccCodecAVC::UT_CMccCodecAVC_EnableVADL(  )
    {
    EUNIT_ASSERT( iCodec->EnableVAD(ETrue) == KErrNotSupported  );
    }

void UT_CMccCodecAVC::UT_CMccCodecAVC_SetBitrateL(  )
    {
    EUNIT_ASSERT( iCodec->SetBitrate(64000) == KErrNone );
    EUNIT_ASSERT( iCodec->SetBitrate(0) == KErrNotSupported );
    EUNIT_ASSERT( iCodec->SetBitrate(2000001) == KErrNotSupported );
    
    // No effect as bitrate has been set explicitly
    iCodec->SetLevelBasedBitrate( 80000 );
    EUNIT_ASSERT( iCodec->iBitrate == 64000 );
    }

void UT_CMccCodecAVC::UT_CMccCodecAVC_SetSamplingFreqL(  )
    {
    EUNIT_ASSERT( iCodec->SetSamplingFreq(64000) == KErrNone );
    }

void UT_CMccCodecAVC::UT_CMccCodecAVC_SetPTimeL(  )
    {
    EUNIT_ASSERT( iCodec->SetPTime(100) ==  KErrNone );
    EUNIT_ASSERT( iCodec->SetPTime(300) ==  KErrNotSupported );

    }

void UT_CMccCodecAVC::UT_CMccCodecAVC_SetMaxPTimeL(  )
    {
    EUNIT_ASSERT( iCodec->SetMaxPTime(100) ==  KErrNone );
    EUNIT_ASSERT( iCodec->SetMaxPTime(300) ==  KErrNotSupported );
    }

void UT_CMccCodecAVC::UT_CMccCodecAVC_SetSdpNameL(  )
    {
    EUNIT_ASSERT( iCodec->SetSdpName( KAVCSdpName ) == KErrNone);
    EUNIT_ASSERT( iCodec->SetSdpName( KAACSdpName ) == KErrNotSupported);

    }

void UT_CMccCodecAVC::UT_CMccCodecAVC_SetPayloadTypeL(  )
    {
    EUNIT_ASSERT( iCodec->SetPayloadType(100) == KErrNone );
    EUNIT_ASSERT( iCodec->SetPayloadType(20) == KErrNotSupported );

    }

void UT_CMccCodecAVC::UT_CMccCodecAVC_SetCodecModeL(  )
    {
    EUNIT_ASSERT( iCodec->SetCodecMode( KAvcFormatModeSingleNal ) == KErrNone );
    EUNIT_ASSERT( iCodec->SetCodecMode( KAvcFormatModeNonInterleaved ) == KErrNone );
    EUNIT_ASSERT( iCodec->SetCodecMode( KAvcFormatModeInterleaved ) == KErrNotSupported );
    }

void UT_CMccCodecAVC::UT_CMccCodecAVC_SetAllowedBitratesL()
    {
    EUNIT_ASSERT( iCodec->SetAllowedBitrates( 0x0FFFFF ) == KErrArgument );
    
    EUNIT_ASSERT( iCodec->SetAllowedBitrates( KMccAvcBitrateLevel1 ) == KErrNone );
    EUNIT_ASSERT_EQUALS( iCodec->Bitrate(), 64000 );
    
    // Bitrate for new level is not taken in use as it is not supported
    EUNIT_ASSERT( iCodec->SetAllowedBitrates( 0x40 ) == KErrNone );
    EUNIT_ASSERT_EQUALS( iCodec->Bitrate(), 64000 );
    
    }
    
void UT_CMccCodecAVC::UT_CMccCodecAVC_SetGetConfigKeyL()
    {
    _LIT8( KTestConfKey, "abcd" );
    iCodec->SetConfigKeyL( KTestConfKey() );
    HBufC8* confKey = iCodec->ConfigKeyL();
    CleanupStack::PushL( confKey );
    EUNIT_ASSERT( *confKey == KTestConfKey() );
    CleanupStack::PopAndDestroy( confKey );
    }

void UT_CMccCodecAVC::UT_CMccCodecAVC_GetFmtpL()
    {
    _LIT8( KTestDefaultProfileLevel, "profile-level-id=42800A" );
    _LIT8( KTestPacketizationMode, "packetization-mode=1" );
    _LIT8( KTestSpropParameterSets, "sprop-parameter-sets=abcd" );
    _LIT8( KTestConfigKey, "abcd" );
     
    // Default
    //
    TDesC8& fmtp = iCodec->GetFmtpL();
   
    EUNIT_ASSERT( fmtp.FindF( KTestDefaultProfileLevel ) != KErrNotFound );
    EUNIT_ASSERT( fmtp.FindF( KTxtPacketizationMode ) == KErrNotFound );
    
    // With packetization-mode and sprop-parametersets (incorrect profile id)
    //
    iCodec->iBitrateMask = 
        KMccAvcCodecProfileIdExtended | KMccAvcCodecProfileIopConstraintSet | KMccAvcBitrateLevel1;
    iCodec->iCodecMode = KAvcFormatModeNonInterleaved;
    iCodec->SetConfigKeyL( KTestConfigKey() );
    TDesC8& fmtp2 = iCodec->GetFmtpL();
    EUNIT_ASSERT( fmtp2.FindF( KTxtProfileLevelId ) == KErrNotFound );
    EUNIT_ASSERT( fmtp2.FindF( KTestPacketizationMode ) != KErrNotFound );
    EUNIT_ASSERT( fmtp2.FindF( KTestSpropParameterSets ) != KErrNotFound );
    
    // With only packetization mode
    //
    delete iCodec->iConfigKey;
    iCodec->iConfigKey = NULL;
    iCodec->iBitrateMask = 
        KMccAvcCodecProfileIdExtended | KMccAvcCodecProfileIopConstraintSet | KMccAvcBitrateLevel1;
    iCodec->iCodecMode = KAvcFormatModeNonInterleaved;
    TDesC8& fmtp3 = iCodec->GetFmtpL();
    EUNIT_ASSERT( fmtp3.CompareF( KTestPacketizationMode ) == 0 );
    
    // With only sprop-parametersets
    //
    iCodec->iBitrateMask = 
        KMccAvcCodecProfileIdExtended | KMccAvcCodecProfileIopConstraintSet | KMccAvcBitrateLevel1;
    iCodec->iCodecMode = KAvcFormatModeSingleNal;
    iCodec->SetConfigKeyL( KTestConfigKey() );
    TDesC8& fmtp4 = iCodec->GetFmtpL();
    EUNIT_ASSERT( fmtp4.CompareF( KTestSpropParameterSets ) == 0 );
    
    // With incorrect profile iop
    //
    iCodec->iBitrateMask = 
        KMccAvcCodecProfileIdBaseline | KMccAvcCodecProfileIopConstraintSet2 | KMccAvcBitrateLevel1;
    TDesC8& fmtp5 = iCodec->GetFmtpL();
    EUNIT_ASSERT( fmtp5.FindF( KTxtProfileLevelId ) == KErrNotFound );
    
    // With incorrect profile level
    //
    const TUint KIncorrectBitrateLevel = 0x40;
    iCodec->iBitrateMask = 
        KMccAvcCodecProfileIdBaseline | KMccAvcCodecProfileIopConstraintSet2 | KIncorrectBitrateLevel;
    TDesC8& fmtp6 = iCodec->GetFmtpL();
    EUNIT_ASSERT( fmtp6.FindF( KTxtProfileLevelId ) == KErrNotFound );
    
    // Profile level 1b
    _LIT8( KTestDefaultProfileLevel1b, "profile-level-id=42900B" );
    iCodec->iBitrateMask = 
        KMccAvcCodecProfileIdBaseline | KMccAvcCodecProfileIopConstraintSet | KMccAvcBitrateLevel1b;
    TDesC8& fmtp7 = iCodec->GetFmtpL();
    EUNIT_ASSERT( fmtp7.FindF( KTestDefaultProfileLevel1b ) != KErrNotFound );
    
    // Profile level 1.1
    _LIT8( KTestDefaultProfileLevel11, "profile-level-id=42800B" );
    iCodec->iBitrateMask = 
        KMccAvcCodecProfileIdBaseline | KMccAvcCodecProfileIopConstraintSet | KMccAvcBitrateLevel1_1;
    TDesC8& fmtp8 = iCodec->GetFmtpL();
    EUNIT_ASSERT( fmtp8.FindF( KTestDefaultProfileLevel11 ) != KErrNotFound );
    
    // Profile level 1.2
    _LIT8( KTestDefaultProfileLevel12, "profile-level-id=42800C" );
    iCodec->iBitrateMask = 
        KMccAvcCodecProfileIdBaseline | KMccAvcCodecProfileIopConstraintSet | KMccAvcBitrateLevel1_2;
    TDesC8& fmtp9 = iCodec->GetFmtpL();
    EUNIT_ASSERT( fmtp9.FindF( KTestDefaultProfileLevel12 ) != KErrNotFound );
    
    // Profile level 1.3
    _LIT8( KTestDefaultProfileLevel13, "profile-level-id=42800D" );
    iCodec->iBitrateMask = 
        KMccAvcCodecProfileIdBaseline | KMccAvcCodecProfileIopConstraintSet | KMccAvcBitrateLevel1_3;
    TDesC8& fmtp10 = iCodec->GetFmtpL();
    EUNIT_ASSERT( fmtp10.FindF( KTestDefaultProfileLevel13 ) != KErrNotFound );
    
    // Profile level 2
    _LIT8( KTestDefaultProfileLevel2, "profile-level-id=428014" );
    iCodec->iBitrateMask = 
        KMccAvcCodecProfileIdBaseline | KMccAvcCodecProfileIopConstraintSet | KMccAvcBitrateLevel2;
    TDesC8& fmtp11 = iCodec->GetFmtpL();
    EUNIT_ASSERT( fmtp11.FindF( KTestDefaultProfileLevel2 ) != KErrNotFound );
    }

void UT_CMccCodecAVC::UT_CMccCodecAVC_ParseFmtpL()
    {
    // Default
    //
    HBufC8* defaultFmtp = iCodec->GetFmtpL().AllocLC();
    EUNIT_ASSERT( iCodec->ParseFmtpAttrL( *defaultFmtp ) == ETrue );
    EUNIT_ASSERT( iCodec->iBitrateMask == ( KMccAvcCodecProfileIdBaseline | 
                                            KMccAvcCodecProfileIopConstraintSet | 
                                            KMccAvcBitrateLevel1 ) );
    
    // Empty fmtp
    //
    EUNIT_ASSERT( iCodec->ParseFmtpAttrL( KNullDesC8 ) == EFalse );
    
    // Profile level, totally wrong
    //
    _LIT8( KTestTotallyIncorrectProfileLevelId, "profile-level-id=G5" );
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( 
        iCodec->ParseFmtpAttrL( KTestTotallyIncorrectProfileLevelId ), KErrArgument );
        
    // Profile level, incorrect profile id
    //
    _LIT8( KTestIncorrectProfileId, "profile-level-id=45800A" );
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( 
        iCodec->ParseFmtpAttrL( KTestIncorrectProfileId ), KErrNotSupported );
 
    // Profile level, incorrect profile iop
    //
    _LIT8( KTestIncorrectProfileIop, "profile-level-id=42FF0A" );
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( 
        iCodec->ParseFmtpAttrL( KTestIncorrectProfileIop ), KErrNotSupported );
    
    //Profile level, For support Sony's terminal
    _LIT8( KTestcorrectProfileIop, "profile-level-id=42A00A" );
    EUNIT_ASSERT( iCodec->ParseFmtpAttrL( KTestcorrectProfileIop ) == ETrue );
    
    // Profile level, incorrect profile level
    //
    _LIT8( KTestIncorrectProfileLvel, "profile-level-id=42800F" );
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( 
        iCodec->ParseFmtpAttrL( KTestIncorrectProfileLvel ), KErrNotSupported );
        
    // Sprop-parameter-sets, too long
    //
    _LIT8( KTestSpropParameterSetsTooLong, 
"sprop-parameter-sets=0123456789012345678901234567890123\
4567890123456789012345678901234567890123456789012345678901234567890123456789" );
     MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( 
        iCodec->ParseFmtpAttrL( KTestSpropParameterSetsTooLong ), KErrArgument );
    
    // Packetization-mode non supported mode
    //
    _LIT8( KTestIncorrectPacketizationMode, "packetization-mode=2" );
     MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( 
        iCodec->ParseFmtpAttrL( KTestIncorrectPacketizationMode ), KErrNotSupported );
        
    // Ok sprop-parameter-set and packetization-mode
    //
    _LIT8( KTestConfigKey, "012345678901234567890" );
    _LIT8( KTestOkPacketizationModeAndSpropParameterSets, 
"packetization-mode=1; sprop-parameter-sets=012345678901234567890" );
    EUNIT_ASSERT( iCodec->ParseFmtpAttrL( KTestOkPacketizationModeAndSpropParameterSets ) == ETrue );
    EUNIT_ASSERT( iCodec->iConfigKey && iCodec->iConfigKey->CompareF( KTestConfigKey ) == 0 );
    EUNIT_ASSERT( iCodec->iCodecMode == KAvcFormatModeNonInterleaved );

    
    // Level 1.1
    //
    _LIT8( KTestProfileLevel11, "profile-level-id=42800B" );
    iCodec->ParseFmtpAttrL( KTestProfileLevel11 );
    EUNIT_ASSERT_EQUALS( iCodec->iMaxBitrate, 192000 );
        
    // Level 1b (1.1 with iop bit)
    //
    _LIT8( KTestProfileLevel1b, "profile-level-id=42900B" );
    iCodec->ParseFmtpAttrL( KTestProfileLevel1b );
    EUNIT_ASSERT_EQUALS( iCodec->iMaxBitrate, 128000 );
    
    // Level 1.2
    //
    _LIT8( KTestProfileLevel12, "profile-level-id=42800C" );
    iCodec->ParseFmtpAttrL( KTestProfileLevel12 );
    EUNIT_ASSERT_EQUALS( iCodec->iMaxBitrate, 384000 );
    
    // Level 1.3
    //
    _LIT8( KTestProfileLevel13, "profile-level-id=42800D" );
    iCodec->ParseFmtpAttrL( KTestProfileLevel13 );
    EUNIT_ASSERT_EQUALS( iCodec->iMaxBitrate, 768000 );
    
    // Level 2
    //
    _LIT8( KTestProfileLevel2, "profile-level-id=428014" );
    iCodec->ParseFmtpAttrL( KTestProfileLevel2 );
    EUNIT_ASSERT_EQUALS( iCodec->iMaxBitrate, 2000000 );
        
    CleanupStack::PopAndDestroy( defaultFmtp );
    }
    
void UT_CMccCodecAVC::UT_CMccCodecAVC_SetPreferredEncodingDecodingDeviceL()
    {
    iCodec->SetPreferredEncodingDecodingDevice( TUid::Uid( 0x20001c13 ));
    EUNIT_ASSERT(iCodec->PreferredEncodingDecodingDevice() == TUid::Uid( 0x20001c13 ));
    }
    
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccCodecAVC,
    "CMccCodecAVC test",
    "UNIT" )

EUNIT_TEST(
    "RequireSignalling - test ",
    "CMccCodecAVC",
    "RequireSignalling",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAVC_RequireSignallingL, Teardown)

EUNIT_TEST(
    "CloneDefaultsL - test ",
    "CMccCodecAVC",
    "CloneDefaultsL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAVC_CloneDefaultsLL, Teardown)

EUNIT_TEST(
    "CloneDetailedL - test ",
    "CMccCodecAVC",
    "CloneDetailedL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAVC_CloneDetailedLL, Teardown)

EUNIT_TEST(
    "EnableVAD - test ",
    "CMccCodecAVC",
    "EnableVAD",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAVC_EnableVADL, Teardown)


EUNIT_TEST(
    "SetBitrate - test ",
    "CMccCodecAVC",
    "SetBitrate",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAVC_SetBitrateL, Teardown)

EUNIT_TEST(
    "SetSamplingFreq - test ",
    "CMccCodecAVC",
    "SetSamplingFreq",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAVC_SetSamplingFreqL, Teardown)

EUNIT_TEST(
    "SetPTime - test ",
    "CMccCodecAVC",
    "SetPTime",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAVC_SetPTimeL, Teardown)

EUNIT_TEST(
    "SetMaxPTime - test ",
    "CMccCodecAVC",
    "SetMaxPTime",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAVC_SetMaxPTimeL, Teardown)

EUNIT_TEST(
    "SetSdpName - test ",
    "CMccCodecAVC",
    "SetSdpName",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAVC_SetSdpNameL, Teardown)

EUNIT_TEST(
    "SetPayloadType - test ",
    "CMccCodecAVC",
    "SetPayloadType",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAVC_SetPayloadTypeL, Teardown)

EUNIT_TEST(
    "SetCodecMode - test ",
    "CMccCodecAVC",
    "SetCodecMode",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAVC_SetCodecModeL, Teardown)

EUNIT_TEST(
    "SetAllowedBitrates - test ",
    "CMccCodecAVC",
    "SetAllowedBitrates",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAVC_SetAllowedBitratesL, Teardown)
    
EUNIT_TEST(
    "SetGetConfigKeyL - test ",
    "CMccCodecAVC",
    "SetGetConfigKeyL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAVC_SetGetConfigKeyL, Teardown)

EUNIT_TEST(
    "GetFmtpL - test ",
    "CMccCodecAVC",
    "GetFmtpL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAVC_GetFmtpL, Teardown)

EUNIT_TEST(
    "ParseFmtpL - test ",
    "CMccCodecAVC",
    "ParseFmtpL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAVC_ParseFmtpL, Teardown)
    
EUNIT_TEST(
    "SetPreferredEncodingDecodingDeviceL - test ",
    "CMccCodecAVC",
    "SetPreferredEncodingDecodingDeviceL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAVC_SetPreferredEncodingDecodingDeviceL, Teardown)    
    
    
EUNIT_END_TEST_TABLE

//  END OF FILE
