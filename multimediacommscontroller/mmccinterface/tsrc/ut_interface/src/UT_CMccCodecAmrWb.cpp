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
#include "UT_CMccCodecAmrWb.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "mmcccodecamrwb.h"

// CONSTRUCTION
UT_CMccCodecAmrWb* UT_CMccCodecAmrWb::NewL()
    {
    UT_CMccCodecAmrWb* self = UT_CMccCodecAmrWb::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccCodecAmrWb* UT_CMccCodecAmrWb::NewLC()
    {
    UT_CMccCodecAmrWb* self = new( ELeave ) UT_CMccCodecAmrWb();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccCodecAmrWb::~UT_CMccCodecAmrWb()
    {
    }

// Default constructor
UT_CMccCodecAmrWb::UT_CMccCodecAmrWb()
    {
    }

// Second phase construct
void UT_CMccCodecAmrWb::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMccCodecAmrWb::SetupL(  )
    {
    iCodecAmr = CMccCodecAmrWb::NewL();
    }

void UT_CMccCodecAmrWb::Teardown(  )
    {
    delete iCodecAmr;
    }

void UT_CMccCodecAmrWb::UT_CMccCodecAmrWb_GetFmtpL(  )
    {
    TDesC8* fmtp;
    fmtp = &(iCodecAmr->GetFmtpL());
    fmtp = &(iCodecAmr->GetFmtpL());

    }

void UT_CMccCodecAmrWb::UT_CMccCodecAmrWb_RequireSignallingL(  )
    {
    CMccCodecInformation* codec = CMccCodecAmrWb::NewL();
    EUNIT_ASSERT_EQUALS( iCodecAmr->RequireSignalling( *codec ), EFalse );
     
    codec->SetBitrate( KAmrNbBitrate122 );    
    codec->SetPTime( 20 );
    codec->SetMaxPTime( 200 );
    EUNIT_ASSERT_EQUALS( iCodecAmr->RequireSignalling( *codec ), ETrue );
    delete codec;
    codec = NULL;

    codec = CMccCodecAmrWb::NewL();
    codec->SetAllowedBitrates( KMccAllowedAmrNbBitrate122 );
    EUNIT_ASSERT_EQUALS( iCodecAmr->RequireSignalling( *codec ), ETrue );
    delete codec;
    codec = NULL; 
    }

void UT_CMccCodecAmrWb::UT_CMccCodecAmrWb_CloneDefaultsLL(  )
    {
    CMccCodecInformation* codec;
    codec = iCodecAmr->CloneDefaultsL();
    delete codec;
    }

void UT_CMccCodecAmrWb::UT_CMccCodecAmrWb_CloneDetailedLL(  )
    {
    CMccCodecInformation* codec;
    codec = iCodecAmr->CloneDetailedL();
    delete codec;
    }

void UT_CMccCodecAmrWb::UT_CMccCodecAmrWb_EnableVADL(  )
    {
    EUNIT_ASSERT_EQUALS( iCodecAmr->EnableVAD( ETrue ), KErrNone );
    EUNIT_ASSERT_EQUALS( iCodecAmr->EnableVAD( EFalse ), KErrNone );
    }

void UT_CMccCodecAmrWb::UT_CMccCodecAmrWb_SetAllowedBitratesL(  )
    {
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetAllowedBitrates( 0xFFFFF ), KErrArgument );  
                                    
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetAllowedBitrates( 
                                    KMccAllowedAmrWbBitrateAll ), KErrNone );   
                                    
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetAllowedBitrates( 
        KMccAllowedAmrWbBitrateAll | 
        KMccAllowedAmrWbModeChangePeriod2 | 
        KMccAllowedAmrWbModeChangeNeighbor1 ), KErrNone );  
    }

void UT_CMccCodecAmrWb::UT_CMccCodecAmrWb_SetChannelsL(  )
    {
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetChannels( 1 ), KErrNone );
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetChannels( 0 ), KErrNotSupported );
    }

void UT_CMccCodecAmrWb::UT_CMccCodecAmrWb_SetRedCountL(  )
    {
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetRedCount( 0 ), KErrNone );
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetRedCount( 1 ), KErrNone );
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetRedCount( 3 ), KErrArgument );
    
    iCodecAmr->iMaxRed = 1;
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetRedCount( 0 ), KErrNone );
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetRedCount( 1 ), KErrNone );
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetRedCount( KMaxAmrFecRedCount + 1 ), KErrArgument );
    }

void UT_CMccCodecAmrWb::UT_CMccCodecAmrWb_SetMaxRedL(  )
    {
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetMaxRed( 40 ), KErrNone );
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetMaxRed( 3 ), KErrArgument );
    }

void UT_CMccCodecAmrWb::UT_CMccCodecAmrWb_GetChannelsL(  )
    {
    TInt channels;
    EUNIT_ASSERT_EQUALS( iCodecAmr->GetChannels( channels ), KErrNone );
    }

void UT_CMccCodecAmrWb::UT_CMccCodecAmrWb_ParseFmtpAttrLL(  )
    {
    // TC implemnted in base class.
    EUNIT_ASSERT( ETrue);
    }

void UT_CMccCodecAmrWb::UT_CMccCodecAmrWb_CreateFmtpAttrListL_1L(  )
    {
    iCodecAmr->CreateFmtpAttrListL();
    }

void UT_CMccCodecAmrWb::UT_CMccCodecAmrWb_GetBitrateModeSetL()
    {
    TInt  mode = iCodecAmr->GetBitrateModeSet(0);
    EUNIT_ASSERT( mode == -1 );
    
    mode = iCodecAmr->GetBitrateModeSet(KAmrWbBitrate660);
    EUNIT_ASSERT( mode == KAMRMode0);
    
    mode = iCodecAmr->GetBitrateModeSet(KAmrWbBitrate885);
    EUNIT_ASSERT( mode == KAMRMode1);
    
    mode = iCodecAmr->GetBitrateModeSet(KAmrWbBitrate1265);
    EUNIT_ASSERT( mode == KAMRMode2);
     
    mode = iCodecAmr->GetBitrateModeSet(KAmrWbBitrate1425);
    EUNIT_ASSERT( mode == KAMRMode3);
    
    	
    mode = iCodecAmr->GetBitrateModeSet(KAmrWbBitrate1585);
    EUNIT_ASSERT( mode == KAMRMode4);
    
    mode = iCodecAmr->GetBitrateModeSet(KAmrWbBitrate1825);
    EUNIT_ASSERT( mode == KAMRMode5);
    
    mode = iCodecAmr->GetBitrateModeSet(KAmrWbBitrate1985);
    EUNIT_ASSERT( mode == KAMRMode6);
       
    mode = iCodecAmr->GetBitrateModeSet(KAmrWbBitrate2305);
    EUNIT_ASSERT( mode == KAMRMode7);
    
    mode = iCodecAmr->GetBitrateModeSet(KAmrWbBitrate2385);
    EUNIT_ASSERT( mode == KAMRMode8);  
    }

void UT_CMccCodecAmrWb::UT_CMccCodecAmrWb_SetBitrateModeSetL()
    {
    iCodecAmr->SetBitrateModeSet(KAMRMode0);
    iCodecAmr->SetBitrateModeSet(KAMRMode1);
    iCodecAmr->SetBitrateModeSet(KAMRMode2);
    iCodecAmr->SetBitrateModeSet(KAMRMode3);
    iCodecAmr->SetBitrateModeSet(KAMRMode4);
    iCodecAmr->SetBitrateModeSet(KAMRMode5);
    iCodecAmr->SetBitrateModeSet(KAMRMode6);
    iCodecAmr->SetBitrateModeSet(KAMRMode7);
    iCodecAmr->SetBitrateModeSet(KAMRMode8);
    iCodecAmr->SetBitrateModeSet(9);
    }

void UT_CMccCodecAmrWb::UT_CMccCodecAmrWb_GetBitrateMaskFromModeL()
    {
    TUint bitmask;
    TInt err = iCodecAmr->GetBitrateMaskFromMode(9, bitmask);
    EUNIT_ASSERT( err == KErrNotFound );
    
    err = iCodecAmr->GetBitrateMaskFromMode(KAMRMode0, bitmask);
    EUNIT_ASSERT( err == KErrNone );
    EUNIT_ASSERT( bitmask == KMccAllowedAmrWbBitrate660 );

    err = iCodecAmr->GetBitrateMaskFromMode(KAMRMode1, bitmask);
    EUNIT_ASSERT( err == KErrNone );
    EUNIT_ASSERT( bitmask == KMccAllowedAmrWbBitrate885 );

    err = iCodecAmr->GetBitrateMaskFromMode(KAMRMode2, bitmask);
    EUNIT_ASSERT( err == KErrNone );
    EUNIT_ASSERT( bitmask == KMccAllowedAmrWbBitrate1265 );

    err = iCodecAmr->GetBitrateMaskFromMode(KAMRMode3, bitmask);
    EUNIT_ASSERT( err == KErrNone );
    EUNIT_ASSERT( bitmask == KMccAllowedAmrWbBitrate1425 );

    err = iCodecAmr->GetBitrateMaskFromMode(KAMRMode4, bitmask);
    EUNIT_ASSERT( err == KErrNone );
    EUNIT_ASSERT( bitmask == KMccAllowedAmrWbBitrate1585 );

    err = iCodecAmr->GetBitrateMaskFromMode(KAMRMode5, bitmask);
    EUNIT_ASSERT( err == KErrNone );
    EUNIT_ASSERT( bitmask == KMccAllowedAmrWbBitrate1825 );

    err = iCodecAmr->GetBitrateMaskFromMode(KAMRMode6, bitmask);
    EUNIT_ASSERT( err == KErrNone );
    EUNIT_ASSERT( bitmask == KMccAllowedAmrWbBitrate1985 );

    err = iCodecAmr->GetBitrateMaskFromMode(KAMRMode7, bitmask);
    EUNIT_ASSERT( err == KErrNone );
    EUNIT_ASSERT( bitmask == KMccAllowedAmrWbBitrate2305 );
    
    err = iCodecAmr->GetBitrateMaskFromMode(KAMRMode8, bitmask);
    EUNIT_ASSERT( err == KErrNone );
    EUNIT_ASSERT( bitmask == KMccAllowedAmrWbBitrateAll );        
    }

void UT_CMccCodecAmrWb::UT_CMccCodecAmrWb_CreateFmtpModeSetL()
    {
    RArray<TUint> bitratearray;
    CleanupClosePushL(bitratearray);
    User::LeaveIfError( bitratearray.Append(KAmrWbBitrate660) );
    User::LeaveIfError( bitratearray.Append(KAmrWbBitrate885) );
    User::LeaveIfError( bitratearray.Append(KAmrWbBitrate1425) );
    CleanupStack::PopAndDestroy(&bitratearray);
  //  bitratearray.Reset();
  //  bitratearray.Close();
    HBufC8* buf = HBufC8::NewLC(10);
    buf->Des().Copy( _L8("hello") );
    TPtr8 bufPtr( buf->Des() );
    
    TBool returnValue = iCodecAmr->CreateFmtpModeSet(bufPtr, bitratearray);
    CleanupStack::PopAndDestroy(buf);
   
    }
    
void UT_CMccCodecAmrWb::UT_CMccCodecAmrWb_SetBitrateL()
    {
    EUNIT_ASSERT(iCodecAmr->SetBitrate(0) == KErrNotSupported );
    EUNIT_ASSERT(iCodecAmr->SetBitrate(KAmrWbBitrate660) == KErrNone );

    EUNIT_ASSERT(iCodecAmr->SetBitrate(KAmrWbBitrate885) == KErrNone );

    EUNIT_ASSERT(iCodecAmr->SetBitrate(KAmrWbBitrate1265) == KErrNone );

    EUNIT_ASSERT(iCodecAmr->SetBitrate(KAmrWbBitrate1425) == KErrNone );

    EUNIT_ASSERT(iCodecAmr->SetBitrate(KAmrWbBitrate1585) == KErrNone );

    EUNIT_ASSERT(iCodecAmr->SetBitrate(KAmrWbBitrate1825) == KErrNone );

    EUNIT_ASSERT(iCodecAmr->SetBitrate(KAmrWbBitrate1985) == KErrNone );

    EUNIT_ASSERT(iCodecAmr->SetBitrate(KAmrWbBitrate2305) == KErrNone );
    
    EUNIT_ASSERT(iCodecAmr->SetBitrate(KAmrWbBitrate2385) == KErrNone );
    }
    
void UT_CMccCodecAmrWb::UT_CMccCodecAmrWb_SetBitrateFromBitrateMaskL()
    {
    EUNIT_ASSERT(iCodecAmr->SetBitrateFromBitrateMask(KMccAllowedAmrWbBitrate2385) == KErrNone );
    
    EUNIT_ASSERT(iCodecAmr->SetBitrateFromBitrateMask(KMccAllowedAmrWbBitrate2305) == KErrNone );

    EUNIT_ASSERT(iCodecAmr->SetBitrateFromBitrateMask(KMccAllowedAmrWbBitrate1985) == KErrNone );

    EUNIT_ASSERT(iCodecAmr->SetBitrateFromBitrateMask(KMccAllowedAmrWbBitrate1825) == KErrNone );

    EUNIT_ASSERT(iCodecAmr->SetBitrateFromBitrateMask(KMccAllowedAmrWbBitrate1585) == KErrNone );

    EUNIT_ASSERT(iCodecAmr->SetBitrateFromBitrateMask(KMccAllowedAmrWbBitrate1425) == KErrNone );

    EUNIT_ASSERT(iCodecAmr->SetBitrateFromBitrateMask(KMccAllowedAmrWbBitrate1265) == KErrNone );

    EUNIT_ASSERT(iCodecAmr->SetBitrateFromBitrateMask(KMccAllowedAmrWbBitrate885) == KErrNone );

    EUNIT_ASSERT(iCodecAmr->SetBitrateFromBitrateMask(KMccAllowedAmrWbBitrate660) == KErrNone );
    
    EUNIT_ASSERT(iCodecAmr->SetBitrateFromBitrateMask(0) == KErrArgument );
    }

void UT_CMccCodecAmrWb::UT_CMccCodecAmrWb_SetSamplingFreqL()
    {
    EUNIT_ASSERT(iCodecAmr->SetSamplingFreq(KAmrNbSamplingFreq) == KErrNotSupported );
    EUNIT_ASSERT(iCodecAmr->SetSamplingFreq(KAmrWbSamplingFreq) == KErrNone );
    EUNIT_ASSERT(iCodecAmr->SetSamplingFreq(0) == KErrNotSupported );
    }
    
void UT_CMccCodecAmrWb::UT_CMccCodecAmrWb_SetPTimeL()
    {
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetPTime( 9 ), KErrArgument);
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetPTime( 20 ), KErrNone);
    }
    
void UT_CMccCodecAmrWb::UT_CMccCodecAmrWb_SetMaxPTimeL(  )
    {
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetMaxPTime( 9 ), KErrArgument);
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetMaxPTime( 20 ), KErrNone);
    }    

void UT_CMccCodecAmrWb::UT_CMccCodecAmrWb_SetSdpNameL()
    {
    HBufC8* name = HBufC8::NewLC(KAMRWbSdpName().Length());
    EUNIT_ASSERT(iCodecAmr->SetSdpName(*name) == KErrNotSupported );

    name->Des().Copy(KAMRSdpName);
    EUNIT_ASSERT(iCodecAmr->SetSdpName(*name) == KErrNotSupported );

    name->Des().Copy(KAMRWbSdpName);
    EUNIT_ASSERT(iCodecAmr->SetSdpName(*name) == KErrNone );
    
    CleanupStack::PopAndDestroy(name);
    }
    
void UT_CMccCodecAmrWb::UT_CMccCodecAmrWb_SetPayloadTypeL(  )
    {
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetPayloadType( KDefaultAmrNbPT ), KErrNone );
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetPayloadType( KPcmuPayloadType ), KErrArgument);
    }
    
void UT_CMccCodecAmrWb::UT_CMccCodecAmrWb_SetCodecModeL(  )
    {
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetCodecMode( ENothing ), KErrNotSupported);
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetCodecMode( EBandwidthEfficient ), KErrNone);
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetCodecMode( EOctetAligned ), KErrNone);
    } 
    
void UT_CMccCodecAmrWb::UT_CMccCodecAmrWb_CreateFmtpAttrListLL(  )
    {
    // tested in base class
    EUNIT_ASSERT( ETrue );
    }
    
void UT_CMccCodecAmrWb::UT_CMccCodecAmrWb_SetModeChangePeriodL()
    {
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetModeChangePeriod( 0 ), KErrArgument );
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetModeChangePeriod( 1 ), KErrNone );
    }

void UT_CMccCodecAmrWb::UT_CMccCodecAmrWb_ModeChangePeriodL()
    {
    EUNIT_ASSERT_EQUALS( iCodecAmr->ModeChangePeriod(), iCodecAmr->iModeChangePeriod );
    }

void UT_CMccCodecAmrWb::UT_CMccCodecAmrWb_SetModeChangeNeighborL()
    {
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetModeChangeNeighbor( EFalse ), KErrNone );
    }

void UT_CMccCodecAmrWb::UT_CMccCodecAmrWb_ModeChangeNeighborL()
    {
    EUNIT_ASSERT_EQUALS( iCodecAmr->ModeChangeNeighbor(), iCodecAmr->iNeighbor );
    }               

void UT_CMccCodecAmrWb::UT_CMccCodecAmrWb_SetCrcL()
    {
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetCrc( 0 ), KErrNone );
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetCrc( 1 ), KErrNotSupported );
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetCrc( -1 ), KErrNotSupported );
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetCrc( 2 ), KErrNotSupported );
    }

void UT_CMccCodecAmrWb::UT_CMccCodecAmrWb_GetCrcL()
    {
    EUNIT_ASSERT_EQUALS( iCodecAmr->GetCrc(), EFalse );
    }

void UT_CMccCodecAmrWb::UT_CMccCodecAmrWb_SetRobustSortingL()
    {
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetRobustSorting( 0 ), KErrNone );
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetRobustSorting( 1 ), KErrNotSupported );
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetRobustSorting( -1 ), KErrNotSupported );
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetRobustSorting( 2 ), KErrNotSupported );
    }

void UT_CMccCodecAmrWb::UT_CMccCodecAmrWb_GetRobustSortingL()
    {
    EUNIT_ASSERT_EQUALS( iCodecAmr->GetRobustSorting(), EFalse );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccCodecAmrWb,
    "CMccCodecAmrWb test",
    "UNIT" )

EUNIT_TEST(
    "GetFmtpL - test ",
    "CMccCodecAmrWb",
    "GetFmtpL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAmrWb_GetFmtpL, Teardown)
    
EUNIT_TEST(
    "RequireSignalling - test ",
    "CMccCodecAmrWb",
    "RequireSignalling",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAmrWb_RequireSignallingL, Teardown)   
    
EUNIT_TEST(
    "CloneDefaultsL - test ",
    "CMccCodecAmrWb",
    "CloneDefaultsL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAmrWb_CloneDefaultsLL, Teardown) 

EUNIT_TEST(
    "CloneDetailedL - test ",
    "CMccCodecAmrWb",
    "CloneDetailedL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAmrWb_CloneDetailedLL, Teardown)     

EUNIT_TEST(
    "EnableVAD - test ",
    "CMccCodecAmrWb",
    "EnableVAD",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAmrWb_EnableVADL, Teardown)          

EUNIT_TEST(
    "SetBitrate - test ",
    "CMccCodecAmrWb",
    "SetBitrate",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAmrWb_SetBitrateL, Teardown)

EUNIT_TEST(
    "SetBitrateModeSet - test ",
    "CMccCodecAmrWb",
    "SetBitrateModeSet",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAmrWb_SetBitrateModeSetL, Teardown)

EUNIT_TEST(
    "SetSamplingFreq - test ",
    "CMccCodecAmrWb",
    "SetSamplingFreq",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAmrWb_SetSamplingFreqL, Teardown)

EUNIT_TEST(
    "SetPTime - test ",
    "CMccCodecAmrWb",
    "SetPTime",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAmrWb_SetPTimeL, Teardown)

EUNIT_TEST(
    "SetMaxPTime - test ",
    "CMccCodecAmrWb",
    "SetMaxPTime",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAmrWb_SetMaxPTimeL, Teardown)

EUNIT_TEST(
    "SetSdpName - test ",
    "CMccCodecAmrWb",
    "SetSdpName",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAmrWb_SetSdpNameL, Teardown)

EUNIT_TEST(
    "SetPayloadType - test ",
    "CMccCodecAmrWb",
    "SetPayloadType",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAmrWb_SetPayloadTypeL, Teardown)

EUNIT_TEST(
    "SetCodecMode - test ",
    "CMccCodecAmrWb",
    "SetCodecMode",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAmrWb_SetCodecModeL, Teardown)

EUNIT_TEST(
    "SetAllowedBitrates - test ",
    "CMccCodecAmrWb",
    "SetAllowedBitrates",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAmrWb_SetAllowedBitratesL, Teardown)

EUNIT_TEST(
    "SetChannels - test ",
    "CMccCodecAmrWb",
    "SetChannels",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAmrWb_SetChannelsL, Teardown)

EUNIT_TEST(
    "SetRedCount - test ",
    "CMccCodecAmrWb",
    "SetRedCount",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAmrWb_SetRedCountL, Teardown)

EUNIT_TEST(
    "SetMaxRed - test ",
    "CMccCodecAmrWb",
    "SetMaxRed",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAmrWb_SetMaxRedL, Teardown)

EUNIT_TEST(
    "GetBitrateModeSet - test ",
    "CMccCodecAmrWb",
    "GetBitrateModeSet",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAmrWb_GetBitrateModeSetL, Teardown)

EUNIT_TEST(
    "GetBitrateMaskFromMode - test ",
    "CMccCodecAmrWb",
    "GetBitrateMaskFromMode",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAmrWb_GetBitrateMaskFromModeL, Teardown)

EUNIT_TEST(
    "GetChannels - test ",
    "CMccCodecAmrWb",
    "GetChannels",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAmrWb_GetChannelsL, Teardown)

EUNIT_TEST(
    "ParseFmtpAttrL - test ",
    "CMccCodecAmrWb",
    "ParseFmtpAttrL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAmrWb_ParseFmtpAttrLL, Teardown)

EUNIT_TEST(
    "CreateFmtpAttrListL - test ",
    "CMccCodecAmrWb",
    "CreateFmtpAttrListL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAmrWb_CreateFmtpAttrListLL, Teardown)

EUNIT_TEST(
    "CreateFmtpAttrListL - test ",
    "CMccCodecAmrWb",
    "CreateFmtpAttrListL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAmrWb_CreateFmtpAttrListL_1L, Teardown)
      
EUNIT_TEST(
    "CreateFmtpModeSetL - test ",
    "CMccCodecAmrWb",
    "CreateFmtpModeSetL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAmrWb_CreateFmtpModeSetL, Teardown)
         
EUNIT_TEST(
    "SetBitrateFromBitrateMaskL - test ",
    "CMccCodecAmrWb",
    "SetBitrateFromBitrateMaskL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAmrWb_SetBitrateFromBitrateMaskL, Teardown)      
    
EUNIT_TEST(
    "SetModeChangePeriod - test ",
    "CMccCodecAmrWb",
    "SetModeChangePeriod",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAmrWb_SetModeChangePeriodL, Teardown)

EUNIT_TEST(
    "ModeChangePeriod - test ",
    "CMccCodecAmrWb",
    "ModeChangePeriod",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAmrWb_ModeChangePeriodL, Teardown)

EUNIT_TEST(
    "SetModeChangeNeighbor - test ",
    "CMccCodecAmrWb",
    "SetModeChangeNeighbor",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAmrWb_SetModeChangeNeighborL, Teardown)

EUNIT_TEST(
    "ModeChangeNeighbor - test ",
    "CMccCodecAmrWb",
    "ModeChangeNeighbor",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAmrWb_ModeChangeNeighborL, Teardown)            

EUNIT_TEST(
    "SetCrc - test ",
    "CMccCodecAmrWb",
    "SetCrc",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAmrWb_SetCrcL, Teardown)           

EUNIT_TEST(
    "GetCrc - test ",
    "CMccCodecAmrWb",
    "GetCrc",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAmrWb_GetCrcL, Teardown) 
    
EUNIT_TEST(
    "SetRobustSorting - test ",
    "CMccCodecAmrWb",
    "SetRobustSorting",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAmrWb_SetRobustSortingL, Teardown)           

EUNIT_TEST(
    "GetRobustSorting - test ",
    "CMccCodecAmrWb",
    "GetRobustSorting",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAmrWb_GetRobustSortingL, Teardown) 

EUNIT_END_TEST_TABLE

//  END OF FILE
