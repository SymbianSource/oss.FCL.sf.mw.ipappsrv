/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    MCC DTMF CodecInformation
*
*/





// INCLUDE FILES
#include "mmcccodecdtmf.h"
#include "mccuids.hrh"
#include "mmccinterfacelogs.h"
#include "mccinternalcodecs.h"

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS
const TUint KDefaultSamplingFreq = 8000;
const TInt KMaxFmtpAttrLength( 50 );

const TText8 KCharComma = ',';
const TText8 KCharHyphen = '-';
const TText8 KCharSpace = ' ';




// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccCodecDTMF::CMccCodecDTMF
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMccCodecDTMF::CMccCodecDTMF() : CMccCodecInformation()
    {
    }

// -----------------------------------------------------------------------------
// CMccCodecDTMF::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMccCodecDTMF::ConstructL()
    {
    iSdpName.Copy( KTelephoneEvent );
    
    iFmtpAttr = HBufC8::NewL( KMaxFmtpAttrLength );
    SetPTime( KDTMFPTime );
    SetPayloadType( KDefaultDtmfPT );
    SetCodecMode( EDTMFModeEvent );
    SetSamplingFreq( KDefaultSamplingFreq );
    SetAllowedBitrates( KMccAllowedDtmfNumberALL );
    iFourCC = KMccFourCCIdDTMF;
    
    iPayloadFormatEncoder = KImplUidDTMFPayloadFormatEncode;
    iPayloadFormatDecoder = KImplUidDTMFPayloadFormatDecode;
    }

// -----------------------------------------------------------------------------
// CMccCodecDTMF::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMccCodecDTMF* CMccCodecDTMF::NewL()
    {
    CMccCodecDTMF* self = new( ELeave ) CMccCodecDTMF;
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

    
// -----------------------------------------------------------------------------
// CMccCodecDTMF::~CMccCodecDTMF
// -----------------------------------------------------------------------------
//
CMccCodecDTMF::~CMccCodecDTMF()
    {
    }
    
// -----------------------------------------------------------------------------
// CMccCodecDTMF::RequireSignalling
// -----------------------------------------------------------------------------
//
TBool CMccCodecDTMF::RequireSignalling(
    const CMccCodecInformation& aCandidate ) const      
    {
    return CMccCodecInformation::RequireSignalling( aCandidate );
    }
       
// -----------------------------------------------------------------------------
// CMccCodecDTMF::SetBitrate
// -----------------------------------------------------------------------------
//
TInt CMccCodecDTMF::SetBitrate( TUint aBitrate )
    {
    return ( aBitrate > 0 ) ? KErrNotSupported : KErrNone;
    }
    
// -----------------------------------------------------------------------------
// CMccCodecDTMF::SetSamplingFreq
// -----------------------------------------------------------------------------
//
TInt CMccCodecDTMF::SetSamplingFreq( TUint32 aSamplingFreq )
    {
    return ( KDefaultSamplingFreq != aSamplingFreq ) ? KErrNotSupported : KErrNone;
    }
    
// -----------------------------------------------------------------------------
// CMccCodecDTMF::SetSdpName
// Sets the SDP name
// -----------------------------------------------------------------------------
//
TInt CMccCodecDTMF::SetSdpName( const TDesC8& aSdpName )
    {
    if ( 0 == aSdpName.CompareF( KTelephoneEvent ) )
        {
        iSdpName.Copy( aSdpName );
        }
    else 
        {
        return KErrNotSupported;
        }
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccCodecDTMF::PayloadType
// Sets the payload type
// -----------------------------------------------------------------------------
//
TInt CMccCodecDTMF::SetPayloadType( TUint8 aPayloadType )
    {
    if ( aPayloadType < KMinDynamicPT ||
         aPayloadType > KMaxPayloadType ) 
        {
        return KErrNotSupported;
        }
    else 
        {
        iPayloadType = aPayloadType;
        }
              
    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// CMccCodecDTMF::SetCodecMode
// Sets the codec mode.
// -----------------------------------------------------------------------------
//
TInt CMccCodecDTMF::SetCodecMode( TCodecMode aCodecMode )
    {
	if ( EDTMFModeEvent == aCodecMode )
	    {
	    iCodecMode = aCodecMode;
	    return KErrNone;
	    }
    else if( EDTMFModeInband == aCodecMode )
        {
        iCodecMode = aCodecMode;
        return KErrNone;
        }
    else
        {
        return KErrNotSupported;
        }
    }

// -----------------------------------------------------------------------------
// CMccCodecDTMF::SetAllowedBitrates
// -----------------------------------------------------------------------------
//    
TInt CMccCodecDTMF::SetAllowedBitrates( TUint aBitrateMask )
    {
    if ( (aBitrateMask >> 16 ) > 0 )
        {
        return KErrArgument;
        }
    else
        {
        iBitrateMask = KMccAllowedDtmfNumberALL;
        iBitrateMask &= aBitrateMask;
        }
    
    return KErrNone;
    
    }
 
// -----------------------------------------------------------------------------
// CMccCodecDTMF::EnableVAD
// Enable / Disable VAD
// -----------------------------------------------------------------------------
//
TInt CMccCodecDTMF::EnableVAD( TBool aEnableVAD ) 
    {
    return aEnableVAD ? KErrNotSupported : KErrNone;
    }
    
// -----------------------------------------------------------------------------
// CMccCodecDTMF::SetPTime
// Set PTime
// -----------------------------------------------------------------------------
//
TInt CMccCodecDTMF::SetPTime( TUint aPTime )
    {
    if ( ( aPTime <= 200 ) && (( aPTime % 10 )  == 0 ))
        {
        iPTime = aPTime;
        return KErrNone;    
        }
    else
        {
        return KErrNotSupported;
        }
    }
    
// -----------------------------------------------------------------------------
// CMccCodecDTMF::SetMaxPTime
// Set Max PTime
// -----------------------------------------------------------------------------
//
TInt CMccCodecDTMF::SetMaxPTime( TUint aMaxPTime )
    {
    if ( ( aMaxPTime <= 200 ) && (( aMaxPTime % 10 )  == 0 ))
        {
        iMaxPTime = aMaxPTime;
        return KErrNone;    
        }
    else
        {
        return KErrNotSupported;
        }
    }
    
// -----------------------------------------------------------------------------
// CMccCodecDTMF::CloneDefaultsL
// Make a default setting clone from this DTMF codec
// -----------------------------------------------------------------------------
//
CMccCodecInformation* CMccCodecDTMF::CloneDefaultsL()
    {
    return CMccCodecDTMF::NewL();
    }

// -----------------------------------------------------------------------------
// CMccCodecDTMF::CloneDetailedL
// -----------------------------------------------------------------------------
//
CMccCodecInformation* CMccCodecDTMF::CloneDetailedL()
    {
    CMccCodecDTMF* newCodec = CMccCodecDTMF::NewL();
    CleanupStack::PushL( newCodec );
    newCodec->SetBitrate( this->Bitrate() );
    newCodec->SetAllowedBitrates( this->AllowedBitrates() );
    newCodec->SetCodecMode( this->CodecMode() );
    newCodec->SetMaxPTime( this->MaxPTime() );
    newCodec->SetPayloadType( this->PayloadType() );
    newCodec->SetPTime( this->PTime() );
    newCodec->SetSamplingFreq( this->SamplingFreq() );
    newCodec->SetSdpName( this->SdpName() );  
    CleanupStack::Pop( newCodec );
    return newCodec;
    }
    
// -----------------------------------------------------------------------------
// CMccCodecDTMF::GetFmtpL
// -----------------------------------------------------------------------------
// 
TDesC8& CMccCodecDTMF::GetFmtpL()
    {
    __INTERFACE( "CMccCodecDTMF::GetFmtpL" )      
 
    delete iFmtpAttr;
    iFmtpAttr = NULL;

    CreateFmtpAttrListL();
    
    __INTERFACE( "CMccCodecDTMF::GetFmtpL, exit" ) 
         
    return *iFmtpAttr;    
    }
   
// -----------------------------------------------------------------------------
// CMccCodecDTMF::CreateFmtpAttrListL
// -----------------------------------------------------------------------------
//  
void CMccCodecDTMF::CreateFmtpAttrListL()
    {
    __INTERFACE( "CMccCodecDTMF::CreateFmtpAttrListL" )

    RArray<TUint> toneNumbers;
    CleanupClosePushL( toneNumbers );
    
    GetAllowedToneNumbersArrayL( toneNumbers );
    CreateFmtpAttrListL( toneNumbers );
    
    CleanupStack::PopAndDestroy(); // toneNumbers
    
    __INTERFACE( "CMccCodecDTMF::CreateFmtpAttrListL, exit" )

    }
    
// -----------------------------------------------------------------------------
// CMccCodecDTMF::ParseFmtpAttrL
// -----------------------------------------------------------------------------
//       
TBool CMccCodecDTMF::ParseFmtpAttrL( const TDesC8& aFmtp )
    {
    __INTERFACE( "CMccCodecDTMF::ParseFmtpAttrL" )      
    __INTERFACE_STR( "CMccCodecDTMF::ParseFmtpAttrL, fmtpValue=", aFmtp )
    
    /*
    a=fmtp:<format> <list of values>
    The list of values consists of comma-separated elements, which can be
    either a single decimal number or two decimal numbers separated by a
    hyphen (dash), where the second number is larger than the first. No
    whitespace is allowed between numbers or hyphens. The list does not
    have to be sorted.
    */
    
    TBool updated = EFalse;
    
    if ( aFmtp.Length() > 0 )
        {
        // white space is not allowed 
        if ( aFmtp.Locate( KCharSpace ) >= 0 )
            {
            User::Leave( KErrArgument );  
            }
           
        CPtrC8Array* tokens = TokenizeL( aFmtp, KCharComma );
        CleanupStack::PushL( tokens );
        
        // Mask will be updated while parsing
        iBitrateMask = 0;
        
        updated = EncodeTokensL( *tokens );
        
        CleanupStack::PopAndDestroy( tokens );
        }
    
    __INTERFACE( "CMccCodecDTMF::ParseFmtpAttrL, exit" )   
    
    return updated;
    }
 
// -----------------------------------------------------------------------------
// CMccCodecDTMF::TokenizeL
// -----------------------------------------------------------------------------
//  
CPtrC8Array* CMccCodecDTMF::TokenizeL( const TDesC8& aValue,
                                       const TChar& aSeparator )
    {
    __INTERFACE( "CMccCodecDTMF::TokenizeLC" )      

	__ASSERT_ALWAYS ( aValue.Length() > 0, User::Leave(KErrArgument) ); 
	__ASSERT_ALWAYS ( aSeparator == KCharComma, User::Leave(KErrArgument) ); 

    CPtrC8Array* tokens = new (ELeave) CPtrC8Array(1);  
    CleanupStack::PushL( tokens );
    
	TLex8 lex(aValue);
	TChar chr = lex.Get();
    TInt previuosCommaPos = -1;
    TInt currentCommaPos = 0;
    
	while ( chr )
		{
		// seperator (comma ',') found
		if (chr == aSeparator)
			{
		    AddTokenL( *tokens, aValue, previuosCommaPos+1, currentCommaPos-1 );
		    
    		previuosCommaPos = currentCommaPos; 
			}
    	chr = lex.Get();
    
        currentCommaPos++;
		}
    
    // the token is last one or first one without comma found
    if ( ( currentCommaPos - previuosCommaPos ) > 1 )
        {
        AddTokenL( *tokens, aValue, previuosCommaPos+1, currentCommaPos-1 );
        }
    else // token error
        {
        User::Leave( KErrArgument );   
        }
    
    CleanupStack::Pop( tokens );
    
    __INTERFACE( "CMccCodecDTMF::TokenizeLC, exit" )      

    return tokens;
    }
    
// -----------------------------------------------------------------------------
// CMccCodecDTMF::AddTokenL
// -----------------------------------------------------------------------------
//  
void CMccCodecDTMF::AddTokenL ( CPtrC8Array& aTokenArray, 
                                const TDesC8& aValue, 
                                TInt aTokenStartPos,
                                TInt aTokenEndPos )
    {
    __INTERFACE( "CMccCodecDTMF::AddTokenL" )      

	__ASSERT_ALWAYS ( aTokenStartPos >= 0, User::Leave(KErrArgument) ); 
    __ASSERT_ALWAYS ( aTokenEndPos >= 0, User::Leave(KErrArgument) ); 
    __ASSERT_ALWAYS ( aTokenStartPos <= aTokenEndPos, User::Leave(KErrArgument) ); 
    
    TUint8 tokenlength = aTokenEndPos - aTokenStartPos + 1;
	TPtrC8 token ( aValue.Mid(  aTokenStartPos , tokenlength ) );

	aTokenArray.AppendL(token);
	
	__INTERFACE( "CMccCodecDTMF::AddTokenL, exit" )      

    }
           
// -----------------------------------------------------------------------------
// CMccCodecDTMF::EncodeTokensL
// -----------------------------------------------------------------------------
//  
TBool CMccCodecDTMF::EncodeTokensL( MDesC8Array& aTokenArray )
    {
    __INTERFACE( "CMccCodecDTMF::EncodeTokensL" )      

    __ASSERT_ALWAYS ( aTokenArray.MdcaCount() > 0, User::Leave(KErrArgument) ); 

    TBool updated = EFalse;
    
    for ( TInt i=0; i < aTokenArray.MdcaCount(); i++ )
        {
        if ( ParseTokenL(aTokenArray.MdcaPoint( i )) ) 
            {
            updated = ETrue;   
            }
        }
     
    __INTERFACE( "CMccCodecDTMF::EncodeTokensL, exit" )      

    return updated;
    }
 
// -----------------------------------------------------------------------------
// CMccCodecDTMF::ParseTokenL
// -----------------------------------------------------------------------------
//       
TBool CMccCodecDTMF::ParseTokenL( const TDesC8& aValue )
    {
    __INTERFACE( "CMccCodecDTMF::ParseTokenL" )      
    
    __ASSERT_ALWAYS ( aValue.Length() > 0, User::Leave(KErrArgument) ); 
    
    TBool updated = EFalse;
    
    TLex8 lex(aValue);
	TChar chr = lex.Get();
	TUint8 numberofHyphens = 0;
	while (chr)
		{
		// token must contain digit numbers, 
		// which may seperated by hyphens '-', 
		// e.g 0-15 or e.g 60 or e.g 7
		if ( chr == KCharHyphen )
    		{
    		++numberofHyphens;
    		}
    	// token must contains position digit number or one hyphen
		if ( ( chr != KCharHyphen && !chr.IsDigit() ) || numberofHyphens > 1 )
			{
		    User::Leave( KErrArgument );
			}
	    chr = lex.Get();
		} 
    
    // try to find the hyphen '-', aValue might contains
    // e.g 1-15,60,70 or 60,70
    TInt hyphenPosition = aValue.Locate( KCharHyphen );
    
    // the aValue does not contain hyphen '-', e.g 60
	if ( hyphenPosition < 0 )
    	{
        updated	= ParseToneNumberL( aValue );
    	}
    // the aValue contains hyphen '-', e.g 0-15
    else if ( hyphenPosition > 0 && hyphenPosition < aValue.Length() )
        {
        TPtrC8 leftside ( aValue.Left(hyphenPosition) );
        TPtrC8 rightside ( aValue.Right( aValue.Length() - hyphenPosition - 1) );
        updated = ParseToneNumbersL( leftside, rightside );   
        }
    // the aValue is wrong, e.g -70 or 60-
    else 
        {
        User::Leave( KErrArgument );   
        }
	
	__INTERFACE( "CMccCodecDTMF::ParseTokenL, exit" )      

	return updated;
    }

// -----------------------------------------------------------------------------
// CMccCodecDTMF::ParseToneNumberL
// -----------------------------------------------------------------------------
//                          
TBool CMccCodecDTMF::ParseToneNumberL( const TDesC8& aValue )
    {
    __INTERFACE( "CMccCodecDTMF::ParserToneNumberL" )      

    __ASSERT_ALWAYS ( aValue.Length() > 0, User::Leave(KErrArgument) ); 
    
    TBool updated = EFalse;
    
    TLex8 myLex ( aValue );
    TUint myNumber;
    User::LeaveIfError( myLex.Val( myNumber, EDecimal ) );
    
    if ( myNumber <= 15 )
        {
        updated = AddAllowedBit( myNumber );
        }
        
    return updated;
    }
    
// -----------------------------------------------------------------------------
// CMccCodecDTMF::ParseToneNumbersL
// -----------------------------------------------------------------------------
//     
TBool CMccCodecDTMF::ParseToneNumbersL( const TDesC8& aValueLeft, 
                                        const TDesC8& aValueRitght )
    {
    __INTERFACE( "CMccCodecDTMF::ParserToneNumbersL" )      

    __ASSERT_ALWAYS ( aValueLeft.Length() > 0, User::Leave(KErrArgument) ); 
    __ASSERT_ALWAYS ( aValueRitght.Length() > 0, User::Leave(KErrArgument) ); 

    TBool updated = EFalse;
    
    TLex8 myLeftLex ( aValueLeft );
    TUint myLeftNumber;
    User::LeaveIfError( myLeftLex.Val( myLeftNumber, EDecimal ) );
    
    TLex8 myRightLex ( aValueRitght );
    TUint myRightNumber;
    User::LeaveIfError( myRightLex.Val( myRightNumber, EDecimal ) );
    
    updated = AddToneNumbersL( myLeftNumber, myRightNumber );
    
    return updated;
    }
        
// -----------------------------------------------------------------------------
// CMccCodecDTMF::AddToneNumbersL
// -----------------------------------------------------------------------------
//   
TBool CMccCodecDTMF::AddToneNumbersL( TUint aLeftNumber, TUint aRightNumber )
    {
    __INTERFACE( "CMccCodecDTMF::AddToneNumbersL" )      

    __ASSERT_ALWAYS ( aLeftNumber < aRightNumber, User::Leave(KErrArgument) ); 

    TBool updated = EFalse;
    
    TInt i = aLeftNumber > 0 ? aLeftNumber : 0;
    
    // if is DTMF tone, save DTMF tone numbers(0-15) to iBitrateMask
    for ( ;i <= (TInt) aRightNumber && i <= 15; i++ )
        {
        updated = AddAllowedBit( i );
        }
        
    return updated;
    }
    
// -----------------------------------------------------------------------------
// CMccCodecDTMF::GetAllowedToneNumbersArrayL
// -----------------------------------------------------------------------------
// 
void CMccCodecDTMF::GetAllowedToneNumbersArrayL( RArray<TUint>& aToneNumbers )
    {
    __INTERFACE( "CMccCodecDTMF::GetAllowedToneNumbersArray" )      

    if ( iBitrateMask & KMccAllowedDtmfNumber0 )
        { 
        aToneNumbers.InsertInOrderL( KDtmfNumber0 );          
        }
    if ( iBitrateMask & KMccAllowedDtmfNumber1 )
        {
        aToneNumbers.InsertInOrderL( KDtmfNumber1 );          
        }
    if ( iBitrateMask & KMccAllowedDtmfNumber2 )
        {
        aToneNumbers.InsertInOrderL( KDtmfNumber2 );          
        }
    if ( iBitrateMask & KMccAllowedDtmfNumber3 )
        {
        aToneNumbers.InsertInOrderL( KDtmfNumber3 );
        }
    if ( iBitrateMask & KMccAllowedDtmfNumber4 )
        {
        aToneNumbers.InsertInOrderL( KDtmfNumber4 );
        }
    if ( iBitrateMask & KMccAllowedDtmfNumber5 )
        {
        aToneNumbers.InsertInOrderL( KDtmfNumber5 );
        }
    if ( iBitrateMask & KMccAllowedDtmfNumber6 )
        {
        aToneNumbers.InsertInOrderL( KDtmfNumber6 );
        }
    if ( iBitrateMask & KMccAllowedDtmfNumber7 )
        {
        aToneNumbers.InsertInOrderL( KDtmfNumber7 );
        }
    if ( iBitrateMask & KMccAllowedDtmfNumber8 )
        {
        aToneNumbers.InsertInOrderL( KDtmfNumber8 );
        }
    if ( iBitrateMask & KMccAllowedDtmfNumber9 )
        {
        aToneNumbers.InsertInOrderL( KDtmfNumber9 );
        }
    if ( iBitrateMask & KMccAllowedDtmfNumber10 )
        {
        aToneNumbers.InsertInOrderL( KDtmfNumber10 );
        }
    if ( iBitrateMask & KMccAllowedDtmfNumber11 )
        {
        aToneNumbers.InsertInOrderL( KDtmfNumber11 );
        }
    if ( iBitrateMask & KMccAllowedDtmfNumber12 )
        {
        aToneNumbers.InsertInOrderL( KDtmfNumber12 );
        }
    if ( iBitrateMask & KMccAllowedDtmfNumber13 )
        {
        aToneNumbers.InsertInOrderL( KDtmfNumber13 );
        }
    if ( iBitrateMask & KMccAllowedDtmfNumber14 )
        {
        aToneNumbers.InsertInOrderL( KDtmfNumber14 );
        }
    if ( iBitrateMask & KMccAllowedDtmfNumber15 )
        {
        aToneNumbers.InsertInOrderL( KDtmfNumber15 );
        }  
    }
  
// -----------------------------------------------------------------------------
// CMccCodecDTMF::CreateFmtpAttrListL
// -----------------------------------------------------------------------------
//       
void CMccCodecDTMF::CreateFmtpAttrListL( const RArray<TUint>& aToneNumbers )
    {
    __INTERFACE( "CMccCodecDTMF::CreateFmtpAttrListL" )      

    delete iFmtpAttr;
    iFmtpAttr = NULL;
    iFmtpAttr = HBufC8::NewL( KMaxFmtpAttrLength ); 
   
    // if aToneNumbers contain e.g 2,3,4,5,7,9,10,11,12,13
    // it will append to iFmtpAttr like "1-5,7,9-13"
    TUint firstPos = 0;
    TUint nextpos = 0;
 
    while ( (TInt) nextpos < aToneNumbers.Count() )
        {
        FindContinuusIntegers( aToneNumbers, nextpos );
        if ( firstPos == nextpos ) 
            {
            iFmtpAttr->Des().AppendNum( aToneNumbers[ nextpos ] );
            }
        else if ( firstPos < nextpos )
            {
            iFmtpAttr->Des().AppendNum( aToneNumbers[ firstPos ] );
            iFmtpAttr->Des().Append( KCharHyphen );
            iFmtpAttr->Des().AppendNum( aToneNumbers[ nextpos ] );
            }
        if ( nextpos != aToneNumbers.Count() - 1 )
            {
            iFmtpAttr->Des().Append( KCharComma );
            }
        ++nextpos; 
        firstPos = nextpos;
        }
 
    }
 
// -----------------------------------------------------------------------------
// CMccCodecDTMF::AddAllowedBit
// -----------------------------------------------------------------------------
//       
TBool CMccCodecDTMF::AddAllowedBit( TUint aBit )
    { 
    __INTERFACE( "CMccCodecDTMF::AddAllowedBit" )      

    TBool updated = EFalse;
 
    switch ( aBit )
        {
        case KDtmfNumber0:
            {
            iBitrateMask |= KMccAllowedDtmfNumber0;
            updated = ETrue;
            break;
            }
        case KDtmfNumber1:
            {
            iBitrateMask |= KMccAllowedDtmfNumber1;
            updated = ETrue;
            break;
            }
        case KDtmfNumber2:
            {
            iBitrateMask |= KMccAllowedDtmfNumber2;
            updated = ETrue;
            break;
            }
        case KDtmfNumber3:
            {
            iBitrateMask |= KMccAllowedDtmfNumber3;
            updated = ETrue;
            break;
            }
        case KDtmfNumber4:
            {
            iBitrateMask |= KMccAllowedDtmfNumber4;
            updated = ETrue;
            break;
            }
        case KDtmfNumber5:
            {
            iBitrateMask |= KMccAllowedDtmfNumber5;
            updated = ETrue;
            break;
            }
        case KDtmfNumber6:
            {
            iBitrateMask |= KMccAllowedDtmfNumber6;
            updated = ETrue;
            break;
            }
        case KDtmfNumber7:
            {
            iBitrateMask |= KMccAllowedDtmfNumber7;
            updated = ETrue;
            break;
            }
        case KDtmfNumber8:
            {
            iBitrateMask |= KMccAllowedDtmfNumber8;
            updated = ETrue;
            break;
            }
        case KDtmfNumber9:
            {
            iBitrateMask |= KMccAllowedDtmfNumber9;
            updated = ETrue;
            break;
            }
        case KDtmfNumber10:
            {
            iBitrateMask |= KMccAllowedDtmfNumber10;
            updated = ETrue;
            break;
            }
        case KDtmfNumber11:
            {
            iBitrateMask |= KMccAllowedDtmfNumber11;
            updated = ETrue;
            break;
            }
        case KDtmfNumber12:
            {
            iBitrateMask |= KMccAllowedDtmfNumber12;
            updated = ETrue;
            break;
            }
        case KDtmfNumber13:
            {
            iBitrateMask |= KMccAllowedDtmfNumber13;
            updated = ETrue;
            break;
            }
        case KDtmfNumber14:
            {
            iBitrateMask |= KMccAllowedDtmfNumber14;
            updated = ETrue;
            break;
            }
        case KDtmfNumber15:
            {
            iBitrateMask |= KMccAllowedDtmfNumber15;
            updated = ETrue;
            break;
            }
        default:
            {
            updated = EFalse;
            break;
            }
        }
   
    return updated;    
    }

// -----------------------------------------------------------------------------
// CMccCodecDTMF::FindContinuusIntegers
// -----------------------------------------------------------------------------
// 
void CMccCodecDTMF::FindContinuusIntegers( const RArray<TUint>& aToneNumbers, 
                                            TUint& aPosition  )
    {
    TBool found = EFalse;
    for ( TInt i=aPosition+1; i < aToneNumbers.Count() && !found; i++ )
        {
        if ( aToneNumbers[i-1] + 1 != aToneNumbers[i] )
            {
            aPosition = i-1;
            found = ETrue;    
            }
        if ( !found && i == ( aToneNumbers.Count() - 1 ) )
            {
            aPosition = i;   
            }
        }
    }
                        
// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File  
