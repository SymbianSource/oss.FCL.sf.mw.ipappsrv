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
#include "UT_CMccCodecAMR.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "mmcccodecamr.h"

// CONSTRUCTION
UT_CMccCodecAMR* UT_CMccCodecAMR::NewL()
    {
    UT_CMccCodecAMR* self = UT_CMccCodecAMR::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccCodecAMR* UT_CMccCodecAMR::NewLC()
    {
    UT_CMccCodecAMR* self = new( ELeave ) UT_CMccCodecAMR();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccCodecAMR::~UT_CMccCodecAMR()
    {
    }

// Default constructor
UT_CMccCodecAMR::UT_CMccCodecAMR()
    {
    }

// Second phase construct
void UT_CMccCodecAMR::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMccCodecAMR::SetupL(  )
    {
    iCodecAmr = CMccCodecAMR::NewL();
    }

void UT_CMccCodecAMR::Teardown(  )
    {
    delete iCodecAmr;
    }

void UT_CMccCodecAMR::UT_CMccCodecAMR_GetFmtpL(  )
    {
    TDesC8* fmtp;
    fmtp = &(iCodecAmr->GetFmtpL());
    fmtp = &(iCodecAmr->GetFmtpL());

    }

void UT_CMccCodecAMR::UT_CMccCodecAMR_RequireSignallingL(  )
    {
    CMccCodecInformation* codec = CMccCodecAMR::NewL();
    EUNIT_ASSERT_EQUALS( iCodecAmr->RequireSignalling( *codec ), EFalse );
     
    codec->SetBitrate( KAmrNbBitrate122 );    
    codec->SetPTime( 20 );
    codec->SetMaxPTime( 200 );
    EUNIT_ASSERT_EQUALS( iCodecAmr->RequireSignalling( *codec ), ETrue );
    delete codec;
    codec = NULL;

    codec = CMccCodecAMR::NewL();
    codec->SetAllowedBitrates( KMccAllowedAmrNbBitrate122 );
    EUNIT_ASSERT_EQUALS( iCodecAmr->RequireSignalling( *codec ), ETrue );
    delete codec;
    codec = NULL; 
    
    codec = CMccCodecAMR::NewL();
    iCodecAmr->iBitrateMask = KMccAllowedModeChangeNeighbor1;
    codec->SetAllowedBitrates( KMccAllowedModeChangeNeighbor1 );
    //512 == 512
    EUNIT_ASSERT_EQUALS( iCodecAmr->RequireSignalling( *codec ), EFalse );
    codec->SetAllowedBitrates( KMccAllowedModeChangeNeighbor1 | KMccAllowedAmrNbBitrateAll );
    //512 == 767
    EUNIT_ASSERT_EQUALS( iCodecAmr->RequireSignalling( *codec ), EFalse );
    codec->SetAllowedBitrates( KMccAllowedAmrNbBitrate740 );
    //512 != 16
    EUNIT_ASSERT_EQUALS( iCodecAmr->RequireSignalling( *codec ), ETrue );
    iCodecAmr->iBitrateMask = KMccAllowedAmrNbBitrate740;
    codec->SetAllowedBitrates( KMccAllowedModeChangeNeighbor1 );
    //16 != 512
    EUNIT_ASSERT_EQUALS( iCodecAmr->RequireSignalling( *codec ), ETrue );
    delete codec;
    codec = NULL;
    
    codec = CMccCodecAMR::NewL();
    iCodecAmr->iBitrateMask = KMccAllowedModeChangeNeighbor1 | KMccAllowedAmrNbBitrateAll;
    codec->SetAllowedBitrates( KMccAllowedModeChangeNeighbor1 );
    //767 == 512
    EUNIT_ASSERT_EQUALS( iCodecAmr->RequireSignalling( *codec ), EFalse );
    codec->SetAllowedBitrates( KMccAllowedModeChangeNeighbor1 | KMccAllowedAmrNbBitrateAll );
    //767 == 767
    EUNIT_ASSERT_EQUALS( iCodecAmr->RequireSignalling( *codec ), EFalse );
    codec->SetAllowedBitrates( KMccAllowedAmrNbBitrateAll );
    //767 != 255
    EUNIT_ASSERT_EQUALS( iCodecAmr->RequireSignalling( *codec ), ETrue );
    delete codec;
    codec = NULL;
    
    }

void UT_CMccCodecAMR::UT_CMccCodecAMR_CloneDefaultsLL(  )
    {
    CMccCodecInformation* codec;
    codec = iCodecAmr->CloneDefaultsL();
    delete codec;
    }

void UT_CMccCodecAMR::UT_CMccCodecAMR_CloneDetailedLL(  )
    {
    CMccCodecInformation* codec;
    codec = iCodecAmr->CloneDetailedL();
    delete codec;
    }

void UT_CMccCodecAMR::UT_CMccCodecAMR_EnableVADL(  )
    {
    EUNIT_ASSERT_EQUALS( iCodecAmr->EnableVAD( ETrue ), KErrNone );
    EUNIT_ASSERT_EQUALS( iCodecAmr->EnableVAD( EFalse ), KErrNone );
    }

void UT_CMccCodecAMR::UT_CMccCodecAMR_SetAllowedBitratesL(  )
    {
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetAllowedBitrates( 0xFFFF ), KErrArgument );  
                                    
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetAllowedBitrates( 
                                    KMccAllowedAmrNbBitrateAll ), KErrNone );   
                                    
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetAllowedBitrates( 
        KMccAllowedAmrNbBitrateAll | KMccAllowedModeChangePeriod2 | KMccAllowedModeChangeNeighbor1 ), KErrNone );  
    }

void UT_CMccCodecAMR::UT_CMccCodecAMR_SetChannelsL(  )
    {
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetChannels( 1 ), KErrNone );
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetChannels( 0 ), KErrNotSupported );
    }

void UT_CMccCodecAMR::UT_CMccCodecAMR_SetRedCountL(  )
    {
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetRedCount( 0 ), KErrNone );
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetRedCount( 1 ), KErrNone );
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetRedCount( 3 ), KErrArgument );
    
    iCodecAmr->iMaxRed = 1;
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetRedCount( 0 ), KErrNone );
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetRedCount( 1 ), KErrNone );
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetRedCount( KMaxAmrFecRedCount + 1 ), KErrArgument );
    }

void UT_CMccCodecAMR::UT_CMccCodecAMR_SetMaxRedL(  )
    {
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetMaxRed( 40 ), KErrNone );
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetMaxRed( 3 ), KErrArgument );
    }

void UT_CMccCodecAMR::UT_CMccCodecAMR_GetChannelsL(  )
    {
    TInt channels;
    EUNIT_ASSERT_EQUALS( iCodecAmr->GetChannels( channels ), KErrNone );
    }

void UT_CMccCodecAMR::UT_CMccCodecAMR_ParseFmtpAttrLL(  )
    {
    // TC implemnted in base class.
    EUNIT_ASSERT( ETrue);
    }

void UT_CMccCodecAMR::UT_CMccCodecAMR_CreateFmtpAttrListL_1L(  )
    {
    iCodecAmr->CreateFmtpAttrListL();
    }

void UT_CMccCodecAMR::UT_CMccCodecAMR_GetBitrateModeSetL()
    {
    TInt  mode = iCodecAmr->GetBitrateModeSet( 0 );
    EUNIT_ASSERT( mode == -1 );
    
    mode = iCodecAmr->GetBitrateModeSet( KAmrNbBitrate475 );
    EUNIT_ASSERT( mode == KAMRMode0 );
    
    mode = iCodecAmr->GetBitrateModeSet( KAmrNbBitrate515 );
    EUNIT_ASSERT( mode == KAMRMode1 );
    
    mode = iCodecAmr->GetBitrateModeSet( KAmrNbBitrate590 );
    EUNIT_ASSERT( mode == KAMRMode2 );
     
    mode = iCodecAmr->GetBitrateModeSet( KAmrNbBitrate670 );
    EUNIT_ASSERT( mode == KAMRMode3 );
    
    	
    mode = iCodecAmr->GetBitrateModeSet( KAmrNbBitrate740 );
    EUNIT_ASSERT( mode == KAMRMode4 );
    
    mode = iCodecAmr->GetBitrateModeSet( KAmrNbBitrate795 );
    EUNIT_ASSERT( mode == KAMRMode5 );
    
    mode = iCodecAmr->GetBitrateModeSet( KAmrNbBitrate102 );
    EUNIT_ASSERT( mode == KAMRMode6 );
       
    mode = iCodecAmr->GetBitrateModeSet( KAmrNbBitrate122 );
    EUNIT_ASSERT( mode == KAMRMode7 );
      
    }

void UT_CMccCodecAMR::UT_CMccCodecAMR_SetBitrateModeSetL()
    {
    iCodecAmr->SetBitrateModeSet( KAMRMode0 );
    iCodecAmr->SetBitrateModeSet( KAMRMode1 );
    iCodecAmr->SetBitrateModeSet( KAMRMode2 );
    iCodecAmr->SetBitrateModeSet( KAMRMode3 );
    iCodecAmr->SetBitrateModeSet( KAMRMode4 );
    iCodecAmr->SetBitrateModeSet( KAMRMode5 );
    iCodecAmr->SetBitrateModeSet( KAMRMode6 );
    iCodecAmr->SetBitrateModeSet( KAMRMode7 );
    iCodecAmr->SetBitrateModeSet( 8 );
    }

void UT_CMccCodecAMR::UT_CMccCodecAMR_GetBitrateMaskFromModeL()
    {
    TUint bitmask;
    TInt err = iCodecAmr->GetBitrateMaskFromMode(8, bitmask);
    EUNIT_ASSERT( err == KErrNotFound );
    
    err = iCodecAmr->GetBitrateMaskFromMode(KAMRMode0, bitmask);
    EUNIT_ASSERT( err == KErrNone );
    EUNIT_ASSERT( bitmask == KMccAllowedAmrNbBitrate475 );

    err = iCodecAmr->GetBitrateMaskFromMode(KAMRMode1, bitmask);
    EUNIT_ASSERT( err == KErrNone );
    EUNIT_ASSERT( bitmask == KMccAllowedAmrNbBitrate515 );

    err = iCodecAmr->GetBitrateMaskFromMode(KAMRMode2, bitmask);
    EUNIT_ASSERT( err == KErrNone );
    EUNIT_ASSERT( bitmask == KMccAllowedAmrNbBitrate590 );

    err = iCodecAmr->GetBitrateMaskFromMode(KAMRMode3, bitmask);
    EUNIT_ASSERT( err == KErrNone );
    EUNIT_ASSERT( bitmask == KMccAllowedAmrNbBitrate670 );

    err = iCodecAmr->GetBitrateMaskFromMode(KAMRMode4, bitmask);
    EUNIT_ASSERT( err == KErrNone );
    EUNIT_ASSERT( bitmask == KMccAllowedAmrNbBitrate740 );

    err = iCodecAmr->GetBitrateMaskFromMode(KAMRMode5, bitmask);
    EUNIT_ASSERT( err == KErrNone );
    EUNIT_ASSERT( bitmask == KMccAllowedAmrNbBitrate795 );

    err = iCodecAmr->GetBitrateMaskFromMode(KAMRMode6, bitmask);
    EUNIT_ASSERT( err == KErrNone );
    EUNIT_ASSERT( bitmask == KMccAllowedAmrNbBitrate102 );

    err = iCodecAmr->GetBitrateMaskFromMode(KAMRMode7, bitmask);
    EUNIT_ASSERT( err == KErrNone );
    EUNIT_ASSERT( bitmask == KMccAllowedAmrNbBitrate122 ); 	          
    }

void UT_CMccCodecAMR::UT_CMccCodecAMR_CreateFmtpModeSetL()
    {
    RArray<TUint> bitratearray;
    CleanupClosePushL( bitratearray );
    User::LeaveIfError( bitratearray.Append( KAmrNbBitrate475 ) );
    User::LeaveIfError( bitratearray.Append( KAmrNbBitrate515 ) );
    User::LeaveIfError( bitratearray.Append( KAmrNbBitrate590 ) );
    CleanupStack::PopAndDestroy( &bitratearray );
  //  bitratearray.Reset();
  //  bitratearray.Close();
    HBufC8* buf = HBufC8::NewLC( 10 );
    buf->Des().Copy( _L8("hello") );
    TPtr8 bufPtr( buf->Des() );
    
    TBool returnValue = iCodecAmr->CreateFmtpModeSet( bufPtr, bitratearray );
    CleanupStack::PopAndDestroy( buf );
   
    }
    
void UT_CMccCodecAMR::UT_CMccCodecAMR_SetBitrateL()
    {
    EUNIT_ASSERT(iCodecAmr->SetBitrate(0) == KErrNotSupported );
    EUNIT_ASSERT(iCodecAmr->SetBitrate(KAmrNbBitrate475) == KErrNone );

    EUNIT_ASSERT(iCodecAmr->SetBitrate(KAmrNbBitrate515) == KErrNone );

    EUNIT_ASSERT(iCodecAmr->SetBitrate(KAmrNbBitrate590) == KErrNone );

    EUNIT_ASSERT(iCodecAmr->SetBitrate(KAmrNbBitrate670) == KErrNone );

    EUNIT_ASSERT(iCodecAmr->SetBitrate(KAmrNbBitrate740) == KErrNone );

    EUNIT_ASSERT(iCodecAmr->SetBitrate(KAmrNbBitrate795) == KErrNone );

    EUNIT_ASSERT(iCodecAmr->SetBitrate(KAmrNbBitrate102) == KErrNone );

    EUNIT_ASSERT(iCodecAmr->SetBitrate(KAmrNbBitrate122) == KErrNone );
    }
    
void UT_CMccCodecAMR::UT_CMccCodecAMR_SetBitrateFromBitrateMaskL()
    {
    EUNIT_ASSERT(iCodecAmr->SetBitrateFromBitrateMask(KMccAllowedAmrNbBitrate122) == KErrNone );
    
    EUNIT_ASSERT(iCodecAmr->SetBitrateFromBitrateMask(KMccAllowedAmrNbBitrate102) == KErrNone );

    EUNIT_ASSERT(iCodecAmr->SetBitrateFromBitrateMask(KMccAllowedAmrNbBitrate795) == KErrNone );

    EUNIT_ASSERT(iCodecAmr->SetBitrateFromBitrateMask(KMccAllowedAmrNbBitrate740) == KErrNone );

    EUNIT_ASSERT(iCodecAmr->SetBitrateFromBitrateMask(KMccAllowedAmrNbBitrate670) == KErrNone );

    EUNIT_ASSERT(iCodecAmr->SetBitrateFromBitrateMask(KMccAllowedAmrNbBitrate590) == KErrNone );

    EUNIT_ASSERT(iCodecAmr->SetBitrateFromBitrateMask(KMccAllowedAmrNbBitrate515) == KErrNone );

    EUNIT_ASSERT(iCodecAmr->SetBitrateFromBitrateMask(KMccAllowedAmrNbBitrate475) == KErrNone );

    EUNIT_ASSERT(iCodecAmr->SetBitrateFromBitrateMask(0) == KErrArgument );
    }

void UT_CMccCodecAMR::UT_CMccCodecAMR_SetSamplingFreqL()
    {
    EUNIT_ASSERT(iCodecAmr->SetSamplingFreq(KAmrNbSamplingFreq) == KErrNone );
    EUNIT_ASSERT(iCodecAmr->SetSamplingFreq(0) == KErrNotSupported );
    }
    
void UT_CMccCodecAMR::UT_CMccCodecAMR_SetPTimeL()
    {
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetPTime( 9 ), KErrArgument);
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetPTime( 20 ), KErrNone);
    }
    
void UT_CMccCodecAMR::UT_CMccCodecAMR_SetMaxPTimeL(  )
    {
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetMaxPTime( 9 ), KErrArgument);
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetMaxPTime( 20 ), KErrNone);
    }    

void UT_CMccCodecAMR::UT_CMccCodecAMR_SetSdpNameL()
    {
    HBufC8* name = HBufC8::NewLC(KAMRSdpName().Length());
    EUNIT_ASSERT(iCodecAmr->SetSdpName(*name) == KErrNotSupported );

    name->Des().Copy(KAMRSdpName);
    EUNIT_ASSERT(iCodecAmr->SetSdpName(*name) == KErrNone );

    CleanupStack::PopAndDestroy(name);
    }
    
void UT_CMccCodecAMR::UT_CMccCodecAMR_SetPayloadTypeL(  )
    {
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetPayloadType( KDefaultAmrNbPT ), KErrNone );
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetPayloadType( KPcmuPayloadType ), KErrArgument);
    }
    
void UT_CMccCodecAMR::UT_CMccCodecAMR_SetCodecModeL(  )
    {
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetCodecMode( ENothing ), KErrNotSupported);
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetCodecMode( EBandwidthEfficient ), KErrNone);
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetCodecMode( EOctetAligned ), KErrNone);
    } 
    
void UT_CMccCodecAMR::UT_CMccCodecAMR_CreateFmtpAttrListLL(  )
    {
    // tested in base class
    EUNIT_ASSERT( ETrue );
    }
    
void UT_CMccCodecAMR::UT_CMccCodecAMR_SetModeChangePeriodL()
    {
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetModeChangePeriod( 0 ), KErrArgument );
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetModeChangePeriod( 1 ), KErrNone );
    }

void UT_CMccCodecAMR::UT_CMccCodecAMR_ModeChangePeriodL()
    {
    EUNIT_ASSERT_EQUALS( iCodecAmr->ModeChangePeriod(), iCodecAmr->iModeChangePeriod );
    }

void UT_CMccCodecAMR::UT_CMccCodecAMR_SetModeChangeNeighborL()
    {
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetModeChangeNeighbor( EFalse ), KErrNone );
    }

void UT_CMccCodecAMR::UT_CMccCodecAMR_ModeChangeNeighborL()
    {
    EUNIT_ASSERT_EQUALS( iCodecAmr->ModeChangeNeighbor(), iCodecAmr->iNeighbor );
    }               

void UT_CMccCodecAMR::UT_CMccCodecAMR_SetCrcL()
    {
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetCrc( 0 ), KErrNone );
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetCrc( 1 ), KErrNotSupported );
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetCrc( -1 ), KErrNotSupported );
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetCrc( 2 ), KErrNotSupported );
    }

void UT_CMccCodecAMR::UT_CMccCodecAMR_GetCrcL()
    {
    EUNIT_ASSERT_EQUALS( iCodecAmr->GetCrc(), EFalse );
    }

void UT_CMccCodecAMR::UT_CMccCodecAMR_SetRobustSortingL()
    {
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetRobustSorting( 0 ), KErrNone );
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetRobustSorting( 1 ), KErrNotSupported );
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetRobustSorting( -1 ), KErrNotSupported );
    EUNIT_ASSERT_EQUALS( iCodecAmr->SetRobustSorting( 2 ), KErrNotSupported );
    }

void UT_CMccCodecAMR::UT_CMccCodecAMR_GetRobustSortingL()
    {
    EUNIT_ASSERT_EQUALS( iCodecAmr->GetRobustSorting(), EFalse );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccCodecAMR,
    "CMccCodecAMR test",
    "UNIT" )

EUNIT_TEST(
    "GetFmtpL - test ",
    "CMccCodecAMR",
    "GetFmtpL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAMR_GetFmtpL, Teardown)
    
EUNIT_TEST(
    "RequireSignalling - test ",
    "CMccCodecAMR",
    "RequireSignalling",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAMR_RequireSignallingL, Teardown)   
    
EUNIT_TEST(
    "CloneDefaultsL - test ",
    "CMccCodecAMR",
    "CloneDefaultsL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAMR_CloneDefaultsLL, Teardown) 

EUNIT_TEST(
    "CloneDetailedL - test ",
    "CMccCodecAMR",
    "CloneDetailedL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAMR_CloneDetailedLL, Teardown)     

EUNIT_TEST(
    "EnableVAD - test ",
    "CMccCodecAMR",
    "EnableVAD",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAMR_EnableVADL, Teardown)          

EUNIT_TEST(
    "SetBitrate - test ",
    "CMccCodecAMR",
    "SetBitrate",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAMR_SetBitrateL, Teardown)

EUNIT_TEST(
    "SetBitrateModeSet - test ",
    "CMccCodecAMR",
    "SetBitrateModeSet",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAMR_SetBitrateModeSetL, Teardown)

EUNIT_TEST(
    "SetSamplingFreq - test ",
    "CMccCodecAMR",
    "SetSamplingFreq",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAMR_SetSamplingFreqL, Teardown)

EUNIT_TEST(
    "SetPTime - test ",
    "CMccCodecAMR",
    "SetPTime",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAMR_SetPTimeL, Teardown)

EUNIT_TEST(
    "SetMaxPTime - test ",
    "CMccCodecAMR",
    "SetMaxPTime",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAMR_SetMaxPTimeL, Teardown)

EUNIT_TEST(
    "SetSdpName - test ",
    "CMccCodecAMR",
    "SetSdpName",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAMR_SetSdpNameL, Teardown)

EUNIT_TEST(
    "SetPayloadType - test ",
    "CMccCodecAMR",
    "SetPayloadType",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAMR_SetPayloadTypeL, Teardown)

EUNIT_TEST(
    "SetCodecMode - test ",
    "CMccCodecAMR",
    "SetCodecMode",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAMR_SetCodecModeL, Teardown)

EUNIT_TEST(
    "SetAllowedBitrates - test ",
    "CMccCodecAMR",
    "SetAllowedBitrates",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAMR_SetAllowedBitratesL, Teardown)

EUNIT_TEST(
    "SetChannels - test ",
    "CMccCodecAMR",
    "SetChannels",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAMR_SetChannelsL, Teardown)

EUNIT_TEST(
    "SetRedCount - test ",
    "CMccCodecAMR",
    "SetRedCount",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAMR_SetRedCountL, Teardown)

EUNIT_TEST(
    "SetMaxRed - test ",
    "CMccCodecAMR",
    "SetMaxRed",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAMR_SetMaxRedL, Teardown)

EUNIT_TEST(
    "GetBitrateModeSet - test ",
    "CMccCodecAMR",
    "GetBitrateModeSet",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAMR_GetBitrateModeSetL, Teardown)

EUNIT_TEST(
    "GetBitrateMaskFromMode - test ",
    "CMccCodecAMR",
    "GetBitrateMaskFromMode",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAMR_GetBitrateMaskFromModeL, Teardown)

EUNIT_TEST(
    "GetChannels - test ",
    "CMccCodecAMR",
    "GetChannels",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAMR_GetChannelsL, Teardown)

EUNIT_TEST(
    "ParseFmtpAttrL - test ",
    "CMccCodecAMR",
    "ParseFmtpAttrL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAMR_ParseFmtpAttrLL, Teardown)

EUNIT_TEST(
    "CreateFmtpAttrListL - test ",
    "CMccCodecAMR",
    "CreateFmtpAttrListL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAMR_CreateFmtpAttrListLL, Teardown)

EUNIT_TEST(
    "CreateFmtpAttrListL - test ",
    "CMccCodecAMR",
    "CreateFmtpAttrListL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAMR_CreateFmtpAttrListL_1L, Teardown)
      
EUNIT_TEST(
    "CreateFmtpModeSetL - test ",
    "CMccCodecAMR",
    "CreateFmtpModeSetL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAMR_CreateFmtpModeSetL, Teardown)
         
EUNIT_TEST(
    "SetBitrateFromBitrateMaskL - test ",
    "CMccCodecAMR",
    "SetBitrateFromBitrateMaskL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAMR_SetBitrateFromBitrateMaskL, Teardown)      
    
EUNIT_TEST(
    "SetModeChangePeriod - test ",
    "CMccCodecAMR",
    "SetModeChangePeriod",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAMR_SetModeChangePeriodL, Teardown)

EUNIT_TEST(
    "ModeChangePeriod - test ",
    "CMccCodecAMR",
    "ModeChangePeriod",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAMR_ModeChangePeriodL, Teardown)

EUNIT_TEST(
    "SetModeChangeNeighbor - test ",
    "CMccCodecAMR",
    "SetModeChangeNeighbor",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAMR_SetModeChangeNeighborL, Teardown)

EUNIT_TEST(
    "ModeChangeNeighbor - test ",
    "CMccCodecAMR",
    "ModeChangeNeighbor",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAMR_ModeChangeNeighborL, Teardown)            
    
EUNIT_TEST(
    "SetCrc - test ",
    "CMccCodecAMR",
    "SetCrc",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAMR_SetCrcL, Teardown)           

EUNIT_TEST(
    "GetCrc - test ",
    "CMccCodecAMR",
    "GetCrc",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAMR_GetCrcL, Teardown) 
    
EUNIT_TEST(
    "SetRobustSorting - test ",
    "CMccCodecAMR",
    "SetRobustSorting",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAMR_SetRobustSortingL, Teardown)           

EUNIT_TEST(
    "GetRobustSorting - test ",
    "CMccCodecAMR",
    "GetRobustSorting",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecAMR_GetRobustSortingL, Teardown) 

EUNIT_END_TEST_TABLE

//  END OF FILE
