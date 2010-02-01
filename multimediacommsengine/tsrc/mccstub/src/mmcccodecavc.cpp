/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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





// INCLUDE FILES
#include "mmcccodecavc.h"
#include "mccuids.hrh"
#include "mmccinterfacelogs.h"

#include "CMccController_stub.h"
#include <delimitedpathsegment8.h>

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS
const TInt KMaxFmtpAttrLength( 200 );
const TInt KAvcDefaultFrameSize( 10000 );
const TInt KAvcLevel1Bitrate( 64000 );
const TInt KAvcLevel1bBitrate( 128000 );
const TInt KAvcLevel11Bitrate( 192000 );
const TInt KAvcLevel12Bitrate( 384000 );
const TInt KAvcLevel13Bitrate( 768000 );
const TInt KAvcLevel2Bitrate( 2000000 );
const TInt KAvcDefaultFreq( 90000 );

const TInt KAvcProfileLevelIdStrLen = 6;
const TInt KAvcProfileLevelIdValLen = 2;
const TUint KAvcProfileIopValueShift = 5;
const TUint KAvcProfileIopFlagForLevel1b = 0x10;

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccCodecAVC::CMccCodecAVC
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMccCodecAVC::CMccCodecAVC() : CMccCodecInformation()
    {
    }

// -----------------------------------------------------------------------------
// CMccCodecAVC::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMccCodecAVC::ConstructL()
    {
    iSdpName.Copy( KAVCSdpName );

    iBitrateMask = KMccAvcCodecProfileIdBaseline | 
                   KMccAvcCodecProfileIopConstraintSet | 
                   KMccAvcBitrateLevel1;
    
    iType = KUidMediaTypeVideo; 
    iFmtpAttr = HBufC8::NewL( KMaxFmtpAttrLength );
    iFourCC = KMccFourCCIdAVC;
    iSamplingFreq = KAvcDefaultFreq;

    iPayloadFormatEncoder = KImplUidAvcPayloadFormatEncode;
    iPayloadFormatDecoder = 0;

    SetCodecMode( KAvcFormatModeSingleNal );
    SetPTime( KAvcPTime );
    SetPayloadType( KAvcPayloadType );
    SetFramerate( KAvcFramerate );
    SetLevelBasedBitrate( KAvcLevel1Bitrate );
    SetMaxBitrate( KAvcLevel1Bitrate );
    SetFrameHeight( KAvcFrameHeight );
    SetFrameWidth( KAvcFrameWidth );
    iFrameSize = KAvcDefaultFrameSize;
    }

// -----------------------------------------------------------------------------
// CMccCodecAVC::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMccCodecAVC* CMccCodecAVC::NewL()
    {
    CMccCodecAVC* self = new( ELeave ) CMccCodecAVC;
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// -----------------------------------------------------------------------------
// CMccCodecAVC::~CMccCodecAVC
// 
// -----------------------------------------------------------------------------
//
CMccCodecAVC::~CMccCodecAVC()
    {
    }

// -----------------------------------------------------------------------------
// CMccCodecAVC::RequireSignalling
// -----------------------------------------------------------------------------
//
TBool CMccCodecAVC::RequireSignalling(
    const CMccCodecInformation& aCandidate ) const      
    {
    return CMccCodecInformation::RequireSignalling( aCandidate );
    }

// -----------------------------------------------------------------------------
// CMccCodecAVC::GetFmtpL
// Gets the fmtp attribute
// -----------------------------------------------------------------------------
//
TDesC8& CMccCodecAVC::GetFmtpL()
    {
    return CMccCodecInformation::GetFmtpL();
    }    

// -----------------------------------------------------------------------------
// CMccCodecAVC::CreateFmtpAttrListL
// -----------------------------------------------------------------------------
//
void CMccCodecAVC::CreateFmtpAttrListL()
    {
    HBufC8* buf = HBufC8::NewLC( KMaxFmtpAttrLength ); 
    TPtr8 bufPtr = buf->Des();
    
    TBool previousParamAdded( EFalse );
    
    if ( CreateFmtpProfileLevelId( bufPtr ) )
        {
        previousParamAdded = ETrue;
        }

    TInt semicolonPos( bufPtr.Length() );
    
    if ( CreateFmtpPacketizationMode( bufPtr ) )
        {
        // Previous param was added, so we need a semi-colon and a space
        if ( previousParamAdded )
            {
            bufPtr.Insert( semicolonPos, KSemicolonSpace );
            }
        previousParamAdded = ETrue;
        }
    
    semicolonPos = bufPtr.Length();
    
    if ( CreateFmtpSpropParameterSets( bufPtr ) )
        {
        // Previous param was added, so we need a semi-colon and a space
        if ( previousParamAdded )
            {
            bufPtr.Insert( semicolonPos, KSemicolonSpace );
            }
        previousParamAdded = ETrue;
        }
        
    // Update iFmtpAttr
    SetFmtpAttrL( bufPtr, EFalse );
    
    CleanupStack::PopAndDestroy( buf );
    }
    
// -----------------------------------------------------------------------------
// CMccCodecAVC::ParseFmtpAttrL
// -----------------------------------------------------------------------------
//
TBool CMccCodecAVC::ParseFmtpAttrL( const TDesC8& aFmtp )
    {
    // These booleans needed in FMTP parsing to handle where parameters
    // not present in FMTP

    iProfileLevelIdParamExists = EFalse;
    iPacketizationModeParamExists = EFalse;
    
    //Remove all white space from begining and end,
    //Convert the content of the descriptor to lower case
    const TUint8* pointerC = aFmtp.Ptr();
    TUint8* pointer        = const_cast<TUint8*>( pointerC );
    TPtr8 descriptor( pointer, aFmtp.Length(), aFmtp.Length() );
    descriptor.Trim();
    
    /*
    TDelimitedPathSegmentParser8 Parses path segments whose 
    components are delimited by the ';' character, 
    as defined in RFC2396.
    */
    TDelimitedPathSegmentParser8 parser;
    parser.Parse( aFmtp );
    TPtrC8 segment;
    TBool updated( EFalse );
    
    while ( parser.GetNext( segment ) != KErrNotFound )
        {
        if ( ParseFmtpSegmentL( segment ) )
            {
            updated = ETrue;
            }
        }
    
    if ( !iProfileLevelIdParamExists )
        {
        SetProfileLevelIdL( KMccAvcCodecProfileIdBaselineVal, 
                            DefaultProfileIopVal(),
                            KMccAvcCodecProfileLevel1Val );
        }
    
    if ( !iPacketizationModeParamExists )
        {
        User::LeaveIfError( SetCodecMode( KAvcFormatModeSingleNal ) );
        }
        
    return updated;
    }    

// -----------------------------------------------------------------------------    
// CMccCodecAVC::ParseFmtpSegmentL
// Parse and updates the matched param
// -----------------------------------------------------------------------------
//    
TBool CMccCodecAVC::ParseFmtpSegmentL( const TDesC8& aSeg )
    {
    TBool update( EFalse );
    _LIT8( KEqualSign, "=" );
    
    // Check for first '=' sign 
    TInt index = aSeg.Find( KEqualSign );
    
    if ( index > 0 )
        {
        // Check if match mode-set 
        if ( aSeg.Left( index ).Match( KMatchProfileLevelId ) != KErrNotFound )
            {
            update = ParseFmtpProfileLevelIdL( 
                            aSeg.Right( aSeg.Length() - ( index + 1 ) ) );
            iProfileLevelIdParamExists = ETrue;
            } 
        else if ( aSeg.Left( index ).Match( KMatchSpropParameterSets ) != KErrNotFound )
            {
            update = ParseFmtpSpropParameterSetsL( 
                            aSeg.Right( aSeg.Length() - ( index + 1 ) ) );
            }   
        else if ( aSeg.Left( index ).Match( KMatchPacketizationMode ) != KErrNotFound )
            {
            update = ParseFmtpPacketizationModeL( 
                            aSeg.Right( aSeg.Length() - ( index + 1 ) ) );
            iPacketizationModeParamExists = ETrue;
            }
        else
            {
            }
        }
  
    return update;
    }

// -----------------------------------------------------------------------------
// CMccCodecAVC::ParseFmtpProfileLevelIdL
// -----------------------------------------------------------------------------
//    
TBool CMccCodecAVC::ParseFmtpProfileLevelIdL( const TDesC8& aProfileLevelId )
    {
    // A base16 [6] (hexadecimal) representation of
    // the following three bytes in the sequence
    // parameter set NAL unit specified in [1]: 1)
    // profile_idc, 2) a byte herein referred to as
    // profile-iop, composed of the values of
    // constraint_set0_flag, constraint_set1_flag,
    // constraint_set2_flag, and reserved_zero_5bits
    // in bit-significance order, starting from the
    // most significant bit, and 3) level_idc.  Note
    // that reserved_zero_5bits is required to be
    // equal to 0 in [1], but other values for it may
    // be specified in the future by ITU-T or ISO/IEC.

    __ASSERT_ALWAYS( aProfileLevelId.Length() == KAvcProfileLevelIdStrLen, 
                     User::Leave( KErrArgument ) );
    
    TInt startPos( 0 );
    TLex8 profileIdLex( aProfileLevelId.Mid( startPos, KAvcProfileLevelIdValLen ) );
    startPos += KAvcProfileLevelIdValLen;
    TUint profileId( 0 );
    User::LeaveIfError( profileIdLex.Val( profileId, EHex ) );


    TLex8 profileIopLex( aProfileLevelId.Mid( startPos, KAvcProfileLevelIdValLen ) );
    startPos += KAvcProfileLevelIdValLen;
    TUint profileIop( 0 );
    User::LeaveIfError( profileIopLex.Val( profileIop, EHex ) );
    
    
    TLex8 profileLevelLex( aProfileLevelId.Mid( startPos, KAvcProfileLevelIdValLen ) );
    TUint profileLevel( 0 );
    User::LeaveIfError( profileLevelLex.Val( profileLevel, EHex ) );
    
    SetProfileLevelIdL( profileId, profileIop, profileLevel );
                  
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CMccCodecAVC::ParseFmtpSpropParameterSetsL
// -----------------------------------------------------------------------------
//    
TBool CMccCodecAVC::ParseFmtpSpropParameterSetsL( const TDesC8& aSpropParameterSets )
    {
    const TInt KMccMaxSpropParameterSetLen = 100;
    __ASSERT_ALWAYS( aSpropParameterSets.Length() > 0 &&
                     aSpropParameterSets.Length() <= KMccMaxSpropParameterSetLen, 
                     User::Leave( KErrArgument ) );
    
    HBufC8* sprop = aSpropParameterSets.AllocL();
    delete iConfigKey;
    iConfigKey = sprop;
                
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CMccCodecAVC::ParseFmtpPacketizationModeL
// -----------------------------------------------------------------------------
//    
TBool CMccCodecAVC::ParseFmtpPacketizationModeL( const TDesC8& aPacketizationMode )
    {
    // Convert descriptor to integer
    TLex8 myLex( aPacketizationMode );
    TInt packetizationMode;
    User::LeaveIfError( myLex.Val( packetizationMode ) );
   
    User::LeaveIfError( SetCodecMode( packetizationMode ) );
                
    return ETrue;
    }            
// -----------------------------------------------------------------------------
// CMccCodecAVC::SetBitrate
// Sets the bitrate used with AMR codec.
// -----------------------------------------------------------------------------
//
TInt CMccCodecAVC::SetBitrate( TUint aBitrate )
    {
    if ( aBitrate > 0 && aBitrate <= KAvcLevel2Bitrate )
        {            
        iBitrate = aBitrate;
        iBitrateSet = ETrue;
        return KErrNone;
        }
    else
        {
        return KErrNotSupported;
        }
    }

// -----------------------------------------------------------------------------
// CMccCodecAVC::SetSamplingFreq
// Sets the sampling frequency. 
// -----------------------------------------------------------------------------
//
TInt CMccCodecAVC::SetSamplingFreq( TUint32 /*aSamplingFreq*/ )
    {
    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// CMccCodecAVC::SetSdpName
// Sets the SDP name
// -----------------------------------------------------------------------------
//
TInt CMccCodecAVC::SetSdpName( const TDesC8& aSdpName )
    {
    if ( 0 == aSdpName.CompareF( KAVCSdpName ) )
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
// CMccCodecAVC::PayloadType
// Sets the payload type
// -----------------------------------------------------------------------------
//
TInt CMccCodecAVC::SetPayloadType( TUint8 aPayloadType )
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
// CMccCodecAVC::SetCodecMode
// Sets the codec mode
// -----------------------------------------------------------------------------
//
TInt CMccCodecAVC::SetCodecMode( TCodecMode aCodecMode )
    {
    if ( KAvcFormatModeSingleNal == aCodecMode || 
         KAvcFormatModeNonInterleaved == aCodecMode )
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
// CMccCodecAVC::SetFmtpAttrL
// -----------------------------------------------------------------------------
//
void CMccCodecAVC::SetFmtpAttrL( const TDesC8& /*aFmtp*/, TBool /*parseFmtp*/ )
    {
    if ( CMccControllerStub::Stub() && CMccControllerStub::Stub()->iSetFmtpFailure )
        {
        User::Leave( KErrNotSupported );
        }
    }
    

TInt CMccCodecAVC::SetAllowedBitrates( TUint aBitrateMask )
    {
    //Confirm that the bitrate mask is valid
    //I.e. after all the valid bitrates are set to zero the value should be zero
    if ( (aBitrateMask >> 16 ) > 0 )
        {
        return KErrArgument;
        }
    else
        {
        SetBitrateMask( KMccAllowedAvcBitrateAll & aBitrateMask );
        SetBitrateMaskAdditionalInfo( aBitrateMask );
        
        SetBitrateFromBitrateMask( iBitrateMask );
        }
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccCodecAVC::EnableVAD
// Enable / Disable VAD
// -----------------------------------------------------------------------------
//
TInt CMccCodecAVC::EnableVAD( TBool /*aEnableVAD*/ ) 
    {
    return KErrNotSupported;
    }
    
// -----------------------------------------------------------------------------
// CMccCodecAVC::SetPTime
// Set PTime
// -----------------------------------------------------------------------------
//
TInt CMccCodecAVC::SetPTime( TUint aPTime )
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
// CMccCodecAVC::SetMaxPTime
// Set Max PTime
// -----------------------------------------------------------------------------
//
TInt CMccCodecAVC::SetMaxPTime( TUint aMaxPTime )
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
// CMccCodecAVC::SetConfigKeyL
// -----------------------------------------------------------------------------
//
void CMccCodecAVC::SetConfigKeyL( const TDesC8& aConfigKey )
    {
    __ASSERT_ALWAYS( aConfigKey.Length() > 0, User::Leave( KErrArgument ) );
    
    HBufC8* key = aConfigKey.AllocL();
    delete iConfigKey;
    iConfigKey = key;
    }
    
// -----------------------------------------------------------------------------
// CMccCodecAVC::SetPreferredEncodingDecodingDevice
// -----------------------------------------------------------------------------
// 
void CMccCodecAVC::SetPreferredEncodingDecodingDevice( 
											TUid aEncodingDecodingDevice )
    {
    __INTERFACE( "CMccCodecAVC::SetPreferredEncodingDecodingDevice" )
    __INTERFACE_INT1("CMccCodecAVC::SetPreferredEncodingDecodingDevice aEncodingDecodingDevice:",aEncodingDecodingDevice.iUid)
    iEncodingDecodingDevice = aEncodingDecodingDevice;
    }
    
    
// -----------------------------------------------------------------------------
// CMccCodecAVC::CloneDefaultsL
// Make a default setting clone from this codec
// -----------------------------------------------------------------------------
//
CMccCodecInformation* CMccCodecAVC::CloneDefaultsL()
    {
    return CMccCodecAVC::NewL();
    }

// -----------------------------------------------------------------------------
// CMccCodecAVC::CloneDetailedL
// Make a detailed clone from this codec
// -----------------------------------------------------------------------------
//
CMccCodecInformation* CMccCodecAVC::CloneDetailedL()
    {
    CMccCodecAVC* newCodec = CMccCodecAVC::NewL();
    CleanupStack::PushL( newCodec );       
    newCodec->SetBitrate( this->Bitrate() );
    newCodec->SetCodecMode( this->CodecMode() );
    newCodec->SetMaxPTime( this->MaxPTime() );
    newCodec->SetPayloadType( this->PayloadType() );
    newCodec->SetPTime( this->PTime() );
    newCodec->SetSamplingFreq( this->SamplingFreq() );
    newCodec->SetSdpName( this->SdpName() );
    newCodec->ParseFmtpAttrL( this->GetFmtpL() );
    CleanupStack::Pop( newCodec );   
    return newCodec;
    }

// -----------------------------------------------------------------------------
// CMccCodecAVC:SetBitrateMask
// -----------------------------------------------------------------------------
// 
void CMccCodecAVC::SetBitrateMask( TUint aBitrateMask, TBool aKeepAdditionalInfo )
    {
    __INTERFACE_INT1( "CMccCodecAVC::SetBitrateMask, entry mask ", aBitrateMask )      
    TUint oldBitrateMask = iBitrateMask; 
    iBitrateMask = aBitrateMask;
        
    if ( aKeepAdditionalInfo )
        {
        iBitrateMask |= ( oldBitrateMask & KMccAllowedAvcAdditionalInfo );
        }
    else
        {
        SetBitrateMaskAdditionalInfo( aBitrateMask );
        }
        
    __INTERFACE_INT1( "CMccCodecAVC::SetBitrateMask, exit mask ", iBitrateMask )      
    }

// -----------------------------------------------------------------------------
// CMccCodecAVC:SetBitrateMaskAdditionalInfo
// -----------------------------------------------------------------------------
//    
void CMccCodecAVC::SetBitrateMaskAdditionalInfo( TUint aAdditionalInfo )
    {
    iBitrateMask |= ( aAdditionalInfo & KMccAllowedAvcAdditionalInfo );
    }

// -----------------------------------------------------------------------------
// CMccCodecAVC:SetProfileLevelIdL
// -----------------------------------------------------------------------------
//
void CMccCodecAVC::SetProfileLevelIdL( 
    TUint aProfileId, TUint aProfileIop, TUint aProfileLevel )
    {
    TUint allowedBitrates( 0 );
    __ASSERT_ALWAYS( aProfileId == KMccAvcCodecProfileIdBaselineVal, 
                     User::Leave( KErrNotSupported ) );                
    allowedBitrates |= KMccAvcCodecProfileIdBaseline;                
                     
    const TUint KProfileIopValueReservedBitsMask = 0xF;
    if ( aProfileIop & KProfileIopValueReservedBitsMask )
        {
        User::Leave( KErrNotSupported );
        }
    TUint profileIop = aProfileIop >> KAvcProfileIopValueShift;
    
    __ASSERT_ALWAYS( profileIop == KMccAvcCodecProfileIopConstraintSetVal, 
                     User::Leave( KErrNotSupported ) );
    allowedBitrates |= KMccAvcCodecProfileIopConstraintSet;       
    
    if ( aProfileLevel == KMccAvcCodecProfileLevel1Val )
        {
        allowedBitrates |= KMccAvcBitrateLevel1;
        }
    else if ( aProfileLevel == KMccAvcCodecProfileLevel1_1Val )
        {
        // Level 1b is differentiated from level 1.1 by iop constraint flag 3
        if( aProfileIop & KAvcProfileIopFlagForLevel1b )
            {
            allowedBitrates |= KMccAvcBitrateLevel1b;
            }
        else
            {
            allowedBitrates |= KMccAvcBitrateLevel1_1;
            }
        }
    else if ( aProfileLevel == KMccAvcCodecProfileLevel1_2Val )
        {
        allowedBitrates |= KMccAvcBitrateLevel1_2;
        }
    else if ( aProfileLevel == KMccAvcCodecProfileLevel1_3Val )
        {
        allowedBitrates |= KMccAvcBitrateLevel1_3;
        }
    else if ( aProfileLevel == KMccAvcCodecProfileLevel2Val )
        {
        allowedBitrates |= KMccAvcBitrateLevel2;
        }
    else
        {
        User::Leave( KErrNotSupported );
        }
                     
    User::LeaveIfError( SetAllowedBitrates( allowedBitrates ) );
    }

// -----------------------------------------------------------------------------
// CMccCodecAVC:ProfileLevelIdVals
// -----------------------------------------------------------------------------
//
TInt CMccCodecAVC::ProfileLevelIdVals( 
    TUint& aProfileId, TUint& aProfileIop, TUint& aProfileLevel )
    {
    TInt retVal( KErrNone );            
    if ( iBitrateMask & KMccAvcCodecProfileIdBaseline )
        {
        aProfileId = KMccAvcCodecProfileIdBaselineVal;
        }
    else
        {
        retVal = KErrNotSupported;
        }
        
    if ( iBitrateMask & KMccAvcCodecProfileIopConstraintSet )
        {
        aProfileIop = KMccAvcCodecProfileIopConstraintSetVal << KAvcProfileIopValueShift;
        }
    else
        {
        retVal = KErrNotSupported;
        }
        
    if ( iBitrateMask & KMccAvcBitrateLevel1 )
        {
        aProfileLevel = KMccAvcCodecProfileLevel1Val;
        }
    else if ( iBitrateMask & KMccAvcBitrateLevel1b )
        {
        // Level 1b information is divided to level and iop fields
        //
        
        // Level 1.1
        aProfileLevel = KMccAvcCodecProfileLevel1_1Val;
        
        // And iop constraint_flag 3
        aProfileIop |= KAvcProfileIopFlagForLevel1b;
        }
    else if ( iBitrateMask & KMccAvcBitrateLevel1_1 )
        {
        aProfileLevel = KMccAvcCodecProfileLevel1_1Val;
        }
    else if ( iBitrateMask & KMccAvcBitrateLevel1_2 )
        {
        aProfileLevel = KMccAvcCodecProfileLevel1_2Val;
        }
    else if ( iBitrateMask & KMccAvcBitrateLevel1_3 )
        {
        aProfileLevel = KMccAvcCodecProfileLevel1_3Val;
        }
    else if ( iBitrateMask & KMccAvcBitrateLevel2 )
        {
        aProfileLevel = KMccAvcCodecProfileLevel2Val;
        }
    else
        {
        retVal = KErrNotSupported;
        }
    return retVal;
    }
    
// -----------------------------------------------------------------------------
// CMccCodecAVC::SetBitrateFromBitrateMask
// -----------------------------------------------------------------------------
//
TInt CMccCodecAVC::SetBitrateFromBitrateMask( TUint aBitrateMask )
    {
    TInt ret( KErrNone );
    if ( aBitrateMask & KMccAvcBitrateLevel1 )
        {
        SetMaxBitrate( KAvcLevel1Bitrate );
        SetLevelBasedBitrate( KAvcLevel1Bitrate );
        }
    else if ( aBitrateMask & KMccAvcBitrateLevel1b )
        {
        SetMaxBitrate( KAvcLevel1bBitrate );
        SetLevelBasedBitrate( KAvcLevel1bBitrate );
        }
    else if ( aBitrateMask & KMccAvcBitrateLevel1_1 )
        {
        SetMaxBitrate( KAvcLevel11Bitrate );
        SetLevelBasedBitrate( KAvcLevel11Bitrate );
        }
    else if ( aBitrateMask & KMccAvcBitrateLevel1_2 )
        {
        SetMaxBitrate( KAvcLevel12Bitrate );
        // Don't use max bitrate as it is way too high
        SetLevelBasedBitrate( KAvcLevel11Bitrate );
        }
    else if ( aBitrateMask & KMccAvcBitrateLevel1_3 )
        {
        SetMaxBitrate( KAvcLevel13Bitrate );
        // Don't use max bitrate as it is way too high
        SetLevelBasedBitrate( KAvcLevel11Bitrate );
        }
    else if ( aBitrateMask & KMccAvcBitrateLevel2 )
        {
        SetMaxBitrate( KAvcLevel2Bitrate );
        // Don't use max bitrate as it is way too high
        SetLevelBasedBitrate( KAvcLevel11Bitrate );
        }
    else
        {
        ret = KErrArgument;
        }
        
    return ret;
    }

// -----------------------------------------------------------------------------
// CMccCodecAVC::CreateFmtpProfileLevelId
// -----------------------------------------------------------------------------
//
TBool CMccCodecAVC::CreateFmtpProfileLevelId( TDes8& aBuf )
    {
    TBool profileLevelIdAdded( EFalse );
    TUint profileId( 0 );
    TUint profileIop( 0 );
    TUint profileLevel( 0 );
    if ( ProfileLevelIdVals( profileId, profileIop, profileLevel ) == KErrNone )
        {
        aBuf.Append( KTxtProfileLevelId );
        aBuf.AppendNumFixedWidthUC( profileId, EHex, KAvcProfileLevelIdValLen ); 
        aBuf.AppendNumFixedWidthUC( profileIop, EHex, KAvcProfileLevelIdValLen ); 
        aBuf.AppendNumFixedWidthUC( profileLevel, EHex, KAvcProfileLevelIdValLen );
        profileLevelIdAdded = ETrue;
        }
    return profileLevelIdAdded;    
    }

// -----------------------------------------------------------------------------
// CMccCodecAVC::CreateFmtpPacketizationMode
// -----------------------------------------------------------------------------
// 
TBool CMccCodecAVC::CreateFmtpPacketizationMode( TDes8& aBuf )
    {
    TBool packetizationModeAdded( EFalse );
    if ( iCodecMode != KAvcFormatModeSingleNal ) 
        {
        aBuf.Append( KTxtPacketizationMode );
        aBuf.AppendNum( iCodecMode );
        packetizationModeAdded = ETrue;       
        }
        
    return packetizationModeAdded;    
    }
    
// -----------------------------------------------------------------------------
// CMccCodecAVC::CreateFmtpSpropParameterSets
// -----------------------------------------------------------------------------
// 
TBool CMccCodecAVC::CreateFmtpSpropParameterSets( TDes8& aBuf )
    {
    TBool spropParameterSetsAdded( EFalse );
    if ( iConfigKey ) 
        {
        aBuf.Append( KTxtSpropParameterSets );
        aBuf.Append( *iConfigKey );
        spropParameterSetsAdded = ETrue;       
        }
        
    return spropParameterSetsAdded;    
    }        

// -----------------------------------------------------------------------------
// CMccCodecAVC::DefaultProfileIopVal
// -----------------------------------------------------------------------------
//
TUint CMccCodecAVC::DefaultProfileIopVal()
    {
    TUint profileIop = 
        KMccAvcCodecProfileIopConstraintSetVal << KAvcProfileIopValueShift;
    return profileIop;
    }
    
// -----------------------------------------------------------------------------
// CMccCodecAVC::SetLevelBasedBitrate
// -----------------------------------------------------------------------------
//      
void CMccCodecAVC::SetLevelBasedBitrate( TUint aBitrate )
    {
    // Set level based bitrate only if bitrate value has not been set
    if ( !iBitrateSet )
        {
        iBitrate = aBitrate;
        }
    }
        
// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File  
