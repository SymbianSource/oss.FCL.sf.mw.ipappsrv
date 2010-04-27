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
#include "UT_CMccCodecDTMF.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "mmcccodecdtmf.h"
#include "mccunittestmacros.h"

// CONSTRUCTION
UT_CMccCodecDTMF* UT_CMccCodecDTMF::NewL()
    {
    UT_CMccCodecDTMF* self = UT_CMccCodecDTMF::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccCodecDTMF* UT_CMccCodecDTMF::NewLC()
    {
    UT_CMccCodecDTMF* self = new( ELeave ) UT_CMccCodecDTMF();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccCodecDTMF::~UT_CMccCodecDTMF()
    {
    }

// Default constructor
UT_CMccCodecDTMF::UT_CMccCodecDTMF()
    {
    }

// Second phase construct
void UT_CMccCodecDTMF::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMccCodecDTMF::SetupL(  )
    {
    iCodec = CMccCodecDTMF::NewL();
    }

void UT_CMccCodecDTMF::Teardown(  )
    {
    delete iCodec;
    }

void UT_CMccCodecDTMF::UT_CMccCodecDTMF_RequireSignallingL(  )
    {
    CMccCodecDTMF* codecinfo = CMccCodecDTMF::NewL();
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

void UT_CMccCodecDTMF::UT_CMccCodecDTMF_CloneDefaultsLL(  )
    {
    CMccCodecInformation* codecinfo = iCodec->CloneDefaultsL();
    delete codecinfo;   
    }

void UT_CMccCodecDTMF::UT_CMccCodecDTMF_CloneDetailedLL(  )
    {
    CMccCodecInformation* codecinfo = iCodec->CloneDetailedL();
    delete codecinfo; 
    }

void UT_CMccCodecDTMF::UT_CMccCodecDTMF_EnableVADL(  )
    {
    EUNIT_ASSERT( iCodec->EnableVAD(ETrue) == KErrNotSupported  );
    }

void UT_CMccCodecDTMF::UT_CMccCodecDTMF_SetBitrateL(  )
    {
    EUNIT_ASSERT( iCodec->SetBitrate(0) == KErrNone );
    }

void UT_CMccCodecDTMF::UT_CMccCodecDTMF_SetSamplingFreqL(  )
    {
    EUNIT_ASSERT( iCodec->SetSamplingFreq(64000) ==  KErrNotSupported );
    }

void UT_CMccCodecDTMF::UT_CMccCodecDTMF_SetPTimeL(  )
    {
    EUNIT_ASSERT( iCodec->SetPTime(100) ==  KErrNone );
    EUNIT_ASSERT( iCodec->SetPTime(300) ==  KErrNotSupported );

    }

void UT_CMccCodecDTMF::UT_CMccCodecDTMF_SetMaxPTimeL(  )
    {
    EUNIT_ASSERT( iCodec->SetMaxPTime(100) ==  KErrNone );
    EUNIT_ASSERT( iCodec->SetMaxPTime(300) ==  KErrNotSupported );
    
    }

void UT_CMccCodecDTMF::UT_CMccCodecDTMF_SetSdpNameL(  )
    {
    EUNIT_ASSERT( iCodec->SetSdpName( KTelephoneEvent ) == KErrNone);
    EUNIT_ASSERT( iCodec->SetSdpName( KAACSdpName ) == KErrNotSupported);
    }

void UT_CMccCodecDTMF::UT_CMccCodecDTMF_SetPayloadTypeL(  )
    {
    EUNIT_ASSERT( iCodec->SetPayloadType(100) == KErrNone );
    EUNIT_ASSERT( iCodec->SetPayloadType(20) == KErrNotSupported );

    }
void UT_CMccCodecDTMF::UT_CMccCodecDTMF_SetCodecModeL(  )
    {
    EUNIT_ASSERT( iCodec->SetCodecMode(EDTMFModeEvent) == KErrNone );
    EUNIT_ASSERT( iCodec->SetCodecMode(EDTMFModeTone) == KErrNotSupported );

    }

void UT_CMccCodecDTMF::UT_CMccCodecDTMF_AddAllowedBit()
    {
    EUNIT_ASSERT( iCodec->AddAllowedBit( KDtmfNumber0 ) );
    EUNIT_ASSERT( iCodec->AddAllowedBit( KDtmfNumber2 ) );
    EUNIT_ASSERT( iCodec->AddAllowedBit( KDtmfNumber3 ) );
    EUNIT_ASSERT( iCodec->AddAllowedBit( KDtmfNumber4 ) );
    EUNIT_ASSERT( iCodec->AddAllowedBit( KDtmfNumber5 ) );
    EUNIT_ASSERT( iCodec->AddAllowedBit( KDtmfNumber6 ) );
    EUNIT_ASSERT( iCodec->AddAllowedBit( KDtmfNumber7 ) );
    EUNIT_ASSERT( iCodec->AddAllowedBit( KDtmfNumber8 ) );
    EUNIT_ASSERT( iCodec->AddAllowedBit( KDtmfNumber9 ) );
    EUNIT_ASSERT( iCodec->AddAllowedBit( KDtmfNumber10 ) );
    EUNIT_ASSERT( iCodec->AddAllowedBit( KDtmfNumber11 ) );
    EUNIT_ASSERT( iCodec->AddAllowedBit( KDtmfNumber12 ) );
    EUNIT_ASSERT( iCodec->AddAllowedBit( KDtmfNumber13 ) );
    EUNIT_ASSERT( iCodec->AddAllowedBit( KDtmfNumber14 ) );
    EUNIT_ASSERT( iCodec->AddAllowedBit( KDtmfNumber15 ) );
    EUNIT_ASSERT( !iCodec->AddAllowedBit( 16 ) );

    }

void UT_CMccCodecDTMF::UT_CMccCodecDTMF_GetAllowedToneNumbersArray()
    {
    RArray<TUint> toneNumbers;
    CleanupClosePushL( toneNumbers );
    iCodec->iBitrateMask = 0;
    toneNumbers.Reset();
    iCodec->GetAllowedToneNumbersArrayL( toneNumbers );
    EUNIT_ASSERT( toneNumbers.Count() == 0 );

    iCodec->iBitrateMask = 0x003E;
    iCodec->GetAllowedToneNumbersArrayL( toneNumbers );

    EUNIT_ASSERT( toneNumbers.Count() == 5 );
    toneNumbers.Reset();
    
    iCodec->iBitrateMask = 0xFFFF;
    iCodec->GetAllowedToneNumbersArrayL( toneNumbers );

    EUNIT_ASSERT( toneNumbers.Count() == 16 );
    toneNumbers.Reset();
    
    
    iCodec->iBitrateMask = 0x9CE7;
    iCodec->GetAllowedToneNumbersArrayL( toneNumbers );

    EUNIT_ASSERT( toneNumbers.Count() == 10 );
    toneNumbers.Reset();
    
    CleanupStack::PopAndDestroy();
  
    }

void UT_CMccCodecDTMF::UT_CMccCodecDTMF_FindContinuusIntegers()
    {
    RArray<TUint> toneNumbers;
    CleanupClosePushL( toneNumbers );
    TUint pos = 0;
    
    iCodec->FindContinuusIntegers( toneNumbers, pos );
    EUNIT_ASSERT( pos == 0 );
    EUNIT_ASSERT( toneNumbers.Count() == 0 );
    
    toneNumbers.InsertInOrderL( 0 );
    
    // 0
    iCodec->FindContinuusIntegers( toneNumbers, pos );
    EUNIT_ASSERT( pos == 0 );
    EUNIT_ASSERT( toneNumbers.Count() == 1 );

    // 15  
    toneNumbers.Reset();
    toneNumbers.InsertInOrderL( 15 );
    iCodec->FindContinuusIntegers( toneNumbers, pos );
    EUNIT_ASSERT( pos == 0 );
    EUNIT_ASSERT( toneNumbers.Count() == 1 );

    // 0-2,4,7-9,11,13-15
    toneNumbers.Reset();
    toneNumbers.InsertInOrderL( 0 );
    toneNumbers.InsertInOrderL( 1 );
    toneNumbers.InsertInOrderL( 2 );
    toneNumbers.InsertInOrderL( 4 );
    toneNumbers.InsertInOrderL( 7 );
    toneNumbers.InsertInOrderL( 8 );
    toneNumbers.InsertInOrderL( 9 );
    toneNumbers.InsertInOrderL( 11 );
    toneNumbers.InsertInOrderL( 13 );
    toneNumbers.InsertInOrderL( 14 );
    toneNumbers.InsertInOrderL( 15 );
    EUNIT_ASSERT( toneNumbers.Count() == 11 );

    iCodec->FindContinuusIntegers( toneNumbers, pos );
    EUNIT_ASSERT( pos == 2 );
    
    iCodec->FindContinuusIntegers( toneNumbers, ++pos );
    EUNIT_ASSERT( pos == 3 );
    
    iCodec->FindContinuusIntegers( toneNumbers, ++pos );
    EUNIT_ASSERT( pos == 6 );
    
    iCodec->FindContinuusIntegers( toneNumbers, ++pos );
    EUNIT_ASSERT( pos == 7 );
    
    iCodec->FindContinuusIntegers( toneNumbers, ++pos );
    EUNIT_ASSERT( pos == 10 );

    CleanupStack::PopAndDestroy();
    }

void UT_CMccCodecDTMF::UT_CMccCodecDTMF_CreateFmtpAttrListL()
    {
    RArray<TUint> toneNumbers;
    CleanupClosePushL( toneNumbers );
    
    toneNumbers.InsertInOrder( 2);
    toneNumbers.InsertInOrder( 3);
    toneNumbers.InsertInOrder( 4);
    toneNumbers.InsertInOrder( 5);
    
    toneNumbers.InsertInOrder( 7);
    toneNumbers.InsertInOrder( 9);
    toneNumbers.InsertInOrder( 10);
    
    toneNumbers.InsertInOrder( 12);
    toneNumbers.InsertInOrder( 13);
    toneNumbers.InsertInOrder( 14);
    toneNumbers.InsertInOrder( 15);
 
    iCodec->CreateFmtpAttrListL( toneNumbers );
    
    CleanupStack::PopAndDestroy();    
    }

void UT_CMccCodecDTMF::UT_CMccCodecDTMF_AddToneNumbersL()
    {
    
    // left side value is greater then right side value
    TBool ret = EFalse;
    iCodec->iBitrateMask = 0;
    EUNIT_ASSERT_SPECIFIC_LEAVE( ret = iCodec->AddToneNumbersL( 15, 0 ), KErrArgument );
    EUNIT_ASSERT( !ret );
    EUNIT_ASSERT(iCodec->iBitrateMask == 0 ); 
    

    EUNIT_ASSERT( iCodec->AddToneNumbersL( 0, 15 ) ); 
    EUNIT_ASSERT( iCodec->iBitrateMask & KMccAllowedDtmfNumberALL );
 
    iCodec->iBitrateMask = 0;
    EUNIT_ASSERT( iCodec->AddToneNumbersL( 0, 20 ) ); 
    EUNIT_ASSERT( iCodec->iBitrateMask & KMccAllowedDtmfNumberALL );
    
    iCodec->iBitrateMask = 0;
    EUNIT_ASSERT( iCodec->AddToneNumbersL( 1, 14 ) ); 
    EUNIT_ASSERT( iCodec->iBitrateMask == 0x7FFE );
    
    iCodec->iBitrateMask = 0;
    EUNIT_ASSERT( iCodec->AddToneNumbersL( 2, 20 ) ); 
    EUNIT_ASSERT( iCodec->iBitrateMask == 0xFFFC );
    
    }
    
void UT_CMccCodecDTMF::UT_CMccCodecDTMF_ParserToneNumberL()
    {
       
    // length is zero
    TBool ret = EFalse;
    EUNIT_ASSERT_SPECIFIC_LEAVE( ret = iCodec->ParseToneNumberL( _L8("") ), KErrArgument );
    
    EUNIT_ASSERT_NO_LEAVE( ret = iCodec->ParseToneNumberL( _L8("10")));
    EUNIT_ASSERT( ret );
    EUNIT_ASSERT( iCodec->iBitrateMask &  KMccAllowedDtmfNumber10 );
    
    iCodec->iBitrateMask = 0;
    ret = EFalse;    
    EUNIT_ASSERT_SPECIFIC_LEAVE( ret = iCodec->ParseToneNumberL( _L8("-3")), KErrGeneral );
    EUNIT_ASSERT( !ret );
    EUNIT_ASSERT( !iCodec->iBitrateMask );

    iCodec->iBitrateMask = 0;
    ret = EFalse;    
    EUNIT_ASSERT_NO_LEAVE( ret = iCodec->ParseToneNumberL( _L8("11")));
    EUNIT_ASSERT( ret );
    EUNIT_ASSERT( iCodec->iBitrateMask &  KMccAllowedDtmfNumber11 );

    }

void UT_CMccCodecDTMF::UT_CMccCodecDTMF_ParserToneNumbersL()
    {
    // length is zero
    TBool ret = EFalse;
    EUNIT_ASSERT_SPECIFIC_LEAVE( ret = iCodec->ParseToneNumbersL( _L8(""), _L8("10") ), KErrArgument );
    EUNIT_ASSERT( !ret );
    
    iCodec->iBitrateMask = 0;
    ret = EFalse;
    EUNIT_ASSERT_SPECIFIC_LEAVE( ret = iCodec->ParseToneNumbersL( _L8("10"), _L8("") ), KErrArgument );
    EUNIT_ASSERT( !ret );
    
    iCodec->iBitrateMask = 0;
    ret = EFalse;
    EUNIT_ASSERT_NO_LEAVE( ret = iCodec->ParseToneNumbersL( _L8("0"), _L8("20")));
    EUNIT_ASSERT( ret );
    EUNIT_ASSERT( iCodec->iBitrateMask &  KMccAllowedDtmfNumberALL );

    iCodec->iBitrateMask = 0;
    ret = EFalse;
    EUNIT_ASSERT_NO_LEAVE( ret = iCodec->ParseToneNumbersL( _L8("0"), _L8("10")));
    EUNIT_ASSERT( ret );
    EUNIT_ASSERT( iCodec->iBitrateMask & 0x7FE );
    
    }

void UT_CMccCodecDTMF::UT_CMccCodecDTMF_ParseTokenL()
    {
    // length is zero
    TBool ret = EFalse;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( ret = iCodec->ParseTokenL( _L8("") ), KErrArgument );
    EUNIT_ASSERT( !ret );
    
    // contains non digit
    ret = EFalse;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( ret = iCodec->ParseTokenL( _L8("2.3") ), KErrArgument );
    EUNIT_ASSERT( !ret );
    
    // contains -60, 70-
    ret = EFalse;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( ret = iCodec->ParseTokenL( _L8("-60") ), KErrArgument );
    EUNIT_ASSERT( !ret );
    
    // contains -60, 70-
    ret = EFalse;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( ret = iCodec->ParseTokenL( _L8("70-") ), KErrArgument );
    EUNIT_ASSERT( !ret );
    
    // contains events=0-15
    ret = EFalse;
    EUNIT_ASSERT_NO_LEAVE( ret = iCodec->ParseTokenL( _L8("events=0-15") ) );
    EUNIT_ASSERT( ret );
    
    iCodec->iBitrateMask = 0;
    ret = EFalse;
    EUNIT_ASSERT_NO_LEAVE( ret = iCodec->ParseTokenL( _L8("11-15") ));
    EUNIT_ASSERT( ret );
    EUNIT_ASSERT( iCodec->iBitrateMask & 0x7C00 );
    
    iCodec->iBitrateMask = 0;
    ret = EFalse;
    EUNIT_ASSERT_NO_LEAVE( ret = iCodec->ParseTokenL( _L8("9") ));
    EUNIT_ASSERT( ret );
    EUNIT_ASSERT( iCodec->iBitrateMask & KMccAllowedDtmfNumber9 );

    }

void UT_CMccCodecDTMF::UT_CMccCodecDTMF_AddTokenL()
    {
    
    CPtrC8Array* tokens = new (ELeave) CPtrC8Array(1);  
    CleanupStack::PushL( tokens ); 
    
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iCodec->AddTokenL( *tokens, _L8("0-15,60,70"), -1, 3 ), KErrArgument );
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iCodec->AddTokenL( *tokens, _L8("0-15,60,70"), 0, -1 ), KErrArgument );
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iCodec->AddTokenL( *tokens, _L8("0-15,60,70"), 3, 1 ), KErrArgument );
    EUNIT_ASSERT( tokens->MdcaCount() == 0);
    
    iCodec->AddTokenL( *tokens, _L8("0-15,60,3,70"), 0, 3 );
    EUNIT_ASSERT( tokens->MdcaCount() == 1);
    EUNIT_ASSERT( tokens->MdcaPoint(0) == _L8("0-15"));


    EUNIT_ASSERT_NO_LEAVE( iCodec->AddTokenL( *tokens, _L8("0-15,60,3,70"), 5, 6 ));
    EUNIT_ASSERT( tokens->MdcaCount() == 2);
    EUNIT_ASSERT( tokens->MdcaPoint(1) == _L8("60"));

    
    iCodec->AddTokenL( *tokens, _L8("0-15,60,3,70"), 8, 8 );
    EUNIT_ASSERT( tokens->MdcaCount() == 3);
    EUNIT_ASSERT( tokens->MdcaPoint(2) == _L8("3"));

    
    EUNIT_ASSERT_NO_LEAVE( iCodec->AddTokenL( *tokens, _L8("0-15,60,3,70"), 10, 11 ));
    EUNIT_ASSERT( tokens->MdcaCount() == 4);
    EUNIT_ASSERT( tokens->MdcaPoint(3) == _L8("70"));

    CleanupStack::PopAndDestroy( tokens );
    }

void UT_CMccCodecDTMF::UT_CMccCodecDTMF_TokenizeL()
    {
    CPtrC8Array* tokens = NULL ;
    
    // value lenth is 0
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( tokens = iCodec->TokenizeL( _L8(""), ','), KErrArgument );
    EUNIT_ASSERT( !tokens );
   
    // seperator is not ','
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( tokens = iCodec->TokenizeL( _L8("0-15,60,3,70"), ';'), KErrArgument );
    EUNIT_ASSERT( !tokens );

    tokens = iCodec->TokenizeL( _L8("0-15,60,3,70"), ',');
    CleanupStack::PushL( tokens );
    EUNIT_ASSERT( tokens );
    EUNIT_ASSERT( tokens->MdcaCount() == 4 );
    CleanupStack::PopAndDestroy( tokens );
    tokens = NULL;

    // TokenizeLC does not care about the parsing
    tokens = iCodec->TokenizeL( _L8("-15,60,-3,4-,70"), ',');
    CleanupStack::PushL( tokens );
    EUNIT_ASSERT( tokens );
    EUNIT_ASSERT( tokens->MdcaCount() == 5 );

    CleanupStack::PopAndDestroy( tokens );
    
    }

void UT_CMccCodecDTMF::UT_CMccCodecDTMF_EncodeTokensL()
    {
    CPtrC8Array* firsttokens = NULL;
    TBool ret = EFalse;
    iCodec->iBitrateMask = 0;
    firsttokens = iCodec->TokenizeL( _L8("0-15,60,3,70"), ',');
    CleanupStack::PushL( firsttokens );
    EUNIT_ASSERT( firsttokens );
    EUNIT_ASSERT( firsttokens->MdcaCount() == 4 );
    EUNIT_ASSERT( iCodec->iBitrateMask == 0 );
    EUNIT_ASSERT_NO_LEAVE( ret = iCodec->EncodeTokensL(*firsttokens) );
    EUNIT_ASSERT( ret );
    EUNIT_ASSERT( iCodec->iBitrateMask & KMccAllowedDtmfNumberALL );
    iCodec->iBitrateMask = 0;

    CleanupStack::PopAndDestroy( firsttokens );
    firsttokens = NULL;

    // TokenizeLC does not care about the parsing
    ret = EFalse;
    CPtrC8Array* sectokens = NULL ;
    sectokens = iCodec->TokenizeL( _L8("-15,60,-3,4-,70"), ',');
    CleanupStack::PushL( sectokens );

    EUNIT_ASSERT( sectokens );
    EUNIT_ASSERT( sectokens->MdcaCount() == 5 );
    EUNIT_ASSERT( iCodec->iBitrateMask == 0 );
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( ret = iCodec->EncodeTokensL(*sectokens), KErrArgument );
    EUNIT_ASSERT( iCodec->iBitrateMask == 0 );
    EUNIT_ASSERT( !ret );
    
    CleanupStack::PopAndDestroy( sectokens );
    }

void UT_CMccCodecDTMF::UT_CMccCodecDTMF_ParseFmtpAttrL()
    {
    TBool retValue = EFalse;
	TBool err( KErrNone );
	
    retValue = iCodec->ParseFmtpAttrL(_L8("0-15,60,70")); 
    EUNIT_ASSERT( retValue );
    EUNIT_ASSERT( iCodec->iBitrateMask & KMccAllowedDtmfNumberALL );
    iCodec->iBitrateMask = 0;
    
    retValue = EFalse;
    retValue = iCodec->ParseFmtpAttrL(_L8("1-14,60,70")); 
    EUNIT_ASSERT( retValue );
    EUNIT_ASSERT( iCodec->iBitrateMask & 0x7FFE ); 
    iCodec->iBitrateMask = 0;

    retValue = EFalse;
    retValue = iCodec->ParseFmtpAttrL(_L8("70,60,0-15,80")); 
    EUNIT_ASSERT( retValue );
    EUNIT_ASSERT( iCodec->iBitrateMask & KMccAllowedDtmfNumberALL );
    iCodec->iBitrateMask = 0;
    
    retValue = EFalse;
    retValue = iCodec->ParseFmtpAttrL(_L8("70,60,80")); 
    EUNIT_ASSERT( !retValue );
    EUNIT_ASSERT( iCodec->iBitrateMask == 0 ); 
    iCodec->iBitrateMask = 0;

    retValue = EFalse;
    retValue = iCodec->ParseFmtpAttrL(_L8("70,60,1,2,3,4,80")); 
    EUNIT_ASSERT( retValue );
    EUNIT_ASSERT(iCodec->iBitrateMask == 0x001E ); 
    iCodec->iBitrateMask = 0;

    // space in the beginning and end is not allowed
    retValue = EFalse;
	MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( retValue = iCodec->ParseFmtpAttrL(_L8(" 70,60,80,0-15 ")), KErrArgument); 
    EUNIT_ASSERT( !retValue );
    EUNIT_ASSERT( iCodec->iBitrateMask == 0 ); 
    iCodec->iBitrateMask = 0;
    
 
    // -60 could not be parse
    retValue = EFalse;
    err = KErrNone;
    TRAP( err, retValue = iCodec->ParseFmtpAttrL(_L8("70,-60,1,2,3,4,80")) );
    EUNIT_ASSERT( err );
    EUNIT_ASSERT( !retValue );
    EUNIT_ASSERT( iCodec->iBitrateMask == 0 ); 
    iCodec->iBitrateMask = 0;
   
    // 1- can not be parsed
    retValue = EFalse;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( retValue = iCodec->ParseFmtpAttrL(_L8("70,60,1-,2,3,4,80")), KErrArgument); 
    EUNIT_ASSERT( !retValue );
    EUNIT_ASSERT( iCodec->iBitrateMask == 0 ); 
    iCodec->iBitrateMask = 0;
    
    // space between numbers can not be parsed
    retValue = EFalse;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( retValue = iCodec->ParseFmtpAttrL(_L8("70,60,1-4 ,80")), KErrArgument); 
    EUNIT_ASSERT( !retValue );
    EUNIT_ASSERT( iCodec->iBitrateMask == 0 ); 
    iCodec->iBitrateMask = 0;
    
    
    // two comma together can not be parsed ",,"
    retValue = EFalse;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( retValue = iCodec->ParseFmtpAttrL(_L8("70,60,,80")), KErrArgument); 
    EUNIT_ASSERT( !retValue );
    EUNIT_ASSERT( iCodec->iBitrateMask == 0 ); 
    iCodec->iBitrateMask = 0;
    
    // comma in the begining can not be parsed
    retValue = EFalse;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( retValue = iCodec->ParseFmtpAttrL(_L8(",70")), KErrArgument); 
    EUNIT_ASSERT( !retValue );
    EUNIT_ASSERT( iCodec->iBitrateMask == 0 ); 
    iCodec->iBitrateMask = 0;
    
    
    // comma in the begining can not be parsed
    retValue = EFalse;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( retValue = iCodec->ParseFmtpAttrL(_L8(",0-15")), KErrArgument); 
    EUNIT_ASSERT( !retValue );
    EUNIT_ASSERT( iCodec->iBitrateMask == 0 ); 
    iCodec->iBitrateMask = 0;
    
    // comma in the end can not be parsed
    retValue = EFalse;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( retValue = iCodec->ParseFmtpAttrL(_L8("70,")), KErrArgument); 
    EUNIT_ASSERT( !retValue );
    EUNIT_ASSERT( iCodec->iBitrateMask == 0 ); 
    iCodec->iBitrateMask = 0;
    
    // comma in the end can not be parsed
    retValue = EFalse;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( retValue = iCodec->ParseFmtpAttrL(_L8("0-15,")), KErrArgument); 
    EUNIT_ASSERT( !retValue );
    EUNIT_ASSERT( iCodec->iBitrateMask == 0 ); 
    iCodec->iBitrateMask = 0;
   
    // multiple fmtp
    retValue = EFalse;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( retValue = iCodec->ParseFmtpAttrL(_L8("0-15;0-15")), KErrArgument); 
    EUNIT_ASSERT( !retValue );
    EUNIT_ASSERT( iCodec->iBitrateMask == 0 ); 
    iCodec->iBitrateMask = 0;
    }

void UT_CMccCodecDTMF::UT_CMccCodecDTMF_GetFmtpL()
    {
    HBufC8* fmtp = HBufC8::NewLC( 50 );
    TPtr8 fmtpPtr = fmtp->Des();
    
    // default case
    fmtpPtr = iCodec->GetFmtpL(); 
    TLex8 fmtpPtrLex( fmtpPtr.PtrZ() );
    EUNIT_ASSERT( fmtpPtrLex.Get() == '0' );
    EUNIT_ASSERT( fmtpPtrLex.Get() == '-' );
    EUNIT_ASSERT( fmtpPtrLex.Get() == '1' );
    EUNIT_ASSERT( fmtpPtrLex.Get() == '5' );
    EUNIT_ASSERT( !fmtpPtrLex.Get() );  
    
    iCodec->iBitrateMask = 0;
    iCodec->iBitrateMask |= KMccAllowedDtmfNumber0;
    iCodec->iBitrateMask |= KMccAllowedDtmfNumber1;
    iCodec->iBitrateMask |= KMccAllowedDtmfNumber2;
    iCodec->iBitrateMask |= KMccAllowedDtmfNumber3;
    iCodec->iBitrateMask |= KMccAllowedDtmfNumber4;
    iCodec->iBitrateMask |= KMccAllowedDtmfNumber5;
    fmtpPtr = iCodec->GetFmtpL(); 
    fmtpPtrLex = fmtpPtr.PtrZ();
    EUNIT_ASSERT( fmtpPtrLex.Get() == '0' );
    EUNIT_ASSERT( fmtpPtrLex.Get() == '-' );
    EUNIT_ASSERT( fmtpPtrLex.Get() == '5' );
    EUNIT_ASSERT( !fmtpPtrLex.Get() );
    
    iCodec->iBitrateMask = 0;
    iCodec->iBitrateMask |= KMccAllowedDtmfNumber10;
    fmtpPtr = iCodec->GetFmtpL(); 
    fmtpPtrLex = fmtpPtr.PtrZ();
    EUNIT_ASSERT( fmtpPtrLex.Get() == '1' );
    EUNIT_ASSERT( fmtpPtrLex.Get() == '0' );
    EUNIT_ASSERT( !fmtpPtrLex.Get() );

    iCodec->iBitrateMask = 0;
    iCodec->iBitrateMask |= KMccAllowedDtmfNumber11;
    iCodec->iBitrateMask |= KMccAllowedDtmfNumber12;
    iCodec->iBitrateMask |= KMccAllowedDtmfNumber13;
    iCodec->iBitrateMask |= KMccAllowedDtmfNumber14;
    iCodec->iBitrateMask |= KMccAllowedDtmfNumber15;
    fmtpPtr = iCodec->GetFmtpL(); 
    fmtpPtrLex = fmtpPtr.PtrZ();
    EUNIT_ASSERT( fmtpPtrLex.Get() == '1' );
    EUNIT_ASSERT( fmtpPtrLex.Get() == '1' );
    EUNIT_ASSERT( fmtpPtrLex.Get() == '-' );
    EUNIT_ASSERT( fmtpPtrLex.Get() == '1' );
    EUNIT_ASSERT( fmtpPtrLex.Get() == '5' );
    EUNIT_ASSERT( !fmtpPtrLex.Get() );
    
    CleanupStack::PopAndDestroy( fmtp );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccCodecDTMF,
    "CMccCodecDTMF test",
    "UNIT" )

EUNIT_TEST(
    "RequireSignalling - test ",
    "CMccCodecDTMF",
    "RequireSignalling",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecDTMF_RequireSignallingL, Teardown)

EUNIT_TEST(
    "CloneDefaultsL - test ",
    "CMccCodecDTMF",
    "CloneDefaultsL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecDTMF_CloneDefaultsLL, Teardown)

EUNIT_TEST(
    "CloneDetailedL - test ",
    "CMccCodecDTMF",
    "CloneDetailedL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecDTMF_CloneDetailedLL, Teardown)

EUNIT_TEST(
    "EnableVAD - test ",
    "CMccCodecDTMF",
    "EnableVAD",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecDTMF_EnableVADL, Teardown)


EUNIT_TEST(
    "SetBitrate - test ",
    "CMccCodecDTMF",
    "SetBitrate",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecDTMF_SetBitrateL, Teardown)

EUNIT_TEST(
    "SetSamplingFreq - test ",
    "CMccCodecDTMF",
    "SetSamplingFreq",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecDTMF_SetSamplingFreqL, Teardown)

EUNIT_TEST(
    "SetPTime - test ",
    "CMccCodecDTMF",
    "SetPTime",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecDTMF_SetPTimeL, Teardown)

EUNIT_TEST(
    "SetMaxPTime - test ",
    "CMccCodecDTMF",
    "SetMaxPTime",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecDTMF_SetMaxPTimeL, Teardown)

EUNIT_TEST(
    "SetSdpName - test ",
    "CMccCodecDTMF",
    "SetSdpName",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecDTMF_SetSdpNameL, Teardown)

EUNIT_TEST(
    "SetPayloadType - test ",
    "CMccCodecDTMF",
    "SetPayloadType",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecDTMF_SetPayloadTypeL, Teardown)

EUNIT_TEST(
    "SetCodecMode - test ",
    "CMccCodecDTMF",
    "SetCodecMode",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecDTMF_SetCodecModeL, Teardown)

EUNIT_TEST(
    "AddAllowedBit - test ",
    "CMccCodecDTMF",
    "AddAllowedBit",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecDTMF_AddAllowedBit, Teardown)
    
EUNIT_TEST(
    "GetAllowedToneNumbersArray - test ",
    "CMccCodecDTMF",
    "GetAllowedToneNumbersArray",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecDTMF_GetAllowedToneNumbersArray, Teardown)
    
EUNIT_TEST(
    "FindContinuusIntegers - test ",
    "CMccCodecDTMF",
    "FindContinuusIntegers",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecDTMF_FindContinuusIntegers, Teardown)

EUNIT_TEST(
    "CreateFmtpAttrListL - test ",
    "CMccCodecDTMF",
    "CreateFmtpAttrListL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecDTMF_CreateFmtpAttrListL, Teardown)

EUNIT_TEST(
    "AddToneNumbersL - test ",
    "CMccCodecDTMF",
    "AddToneNumbersL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecDTMF_AddToneNumbersL, Teardown)

  
EUNIT_TEST(
    "ParserToneNumberL - test ",
    "CMccCodecDTMF",
    "ParserToneNumberL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecDTMF_ParserToneNumberL, Teardown)

         
EUNIT_TEST(
    "ParserToneNumbersL - test ",
    "CMccCodecDTMF",
    "ParserToneNumbersL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecDTMF_ParserToneNumbersL, Teardown)

         
EUNIT_TEST(
    "ParseTokenL - test ",
    "CMccCodecDTMF",
    "ParseTokenL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecDTMF_ParseTokenL, Teardown)

         
EUNIT_TEST(
    "AddTokenL - test ",
    "CMccCodecDTMF",
    "AddTokenL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecDTMF_AddTokenL, Teardown)

         
EUNIT_TEST(
    "TokenizeL - test ",
    "CMccCodecDTMF",
    "TokenizeL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecDTMF_TokenizeL, Teardown)

  
EUNIT_TEST(
    "EncodeTokensL - test ",
    "CMccCodecDTMF",
    "EncodeTokensL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecDTMF_EncodeTokensL, Teardown)

       
EUNIT_TEST(
    "ParseFmtpAttrL - test ",
    "CMccCodecDTMF",
    "ParseFmtpAttrL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecDTMF_ParseFmtpAttrL, Teardown)

EUNIT_TEST(
    "GetFmtpL - test ",
    "CMccCodecDTMF",
    "GetFmtpL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecDTMF_GetFmtpL, Teardown)



      
EUNIT_END_TEST_TABLE

//  END OF FILE
