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




#include "mcecomavccodec.h"

#ifdef MCE_COMMON_SERVER_SIDE

#include <mmcccodecinformationfactory.h>
#include <mmcccodecinformation.h>

#include "mcecommediastream.h"

#endif


// CONSTANTS
_LIT8( KMceAvcProfileLevelIdFmtp, "profile-level-id=" );
_LIT8( KMceAvcProfileLevel1, "42800A" );
const TInt KMceAvcProfileLevelIdValLen = 6;

_LIT8( KMceAvcPacketizationModeFmtp, "packetization-mode=" );
_LIT8( KMceAvcPacketizationModeSingleNal, "0" );
const TInt KMceAvcPacketizationModeValLen = 1;

#ifdef MCE_COMMON_SERVER_SIDE

_LIT8( KMceAvcProfileLevelOnly1, "0A" );
const TInt KMceAvcProfileLevelValLen = 2;
const TInt KMceAvcProfileLevelValOffset = 4;

_LIT8( KMceAvcProfileIopDefault, "80" );
const TInt KMceAvcProfileIopValLen = 2;
const TInt KMceAvcProfileIopValOffset = 2;

_LIT8( KMceAvcSpropParameterSetsFmtp, "sprop-parameter-sets=");
const TChar KMceAvcFmtpParameterSeparator = ';';
const TChar KMceAvcFtmpParameterSpace = ' ';

#endif

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CMceComAvcCodec::NewL
// -----------------------------------------------------------------------------
//
CMceComAvcCodec* CMceComAvcCodec::NewL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComAvcCodec* self = NewLC( aSdpName );
    CleanupStack::Pop( self );
    return self;
    
    }

// -----------------------------------------------------------------------------
// CMceComAvcCodec::NewLC
// -----------------------------------------------------------------------------
//
CMceComAvcCodec* CMceComAvcCodec::NewLC( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComAvcCodec* self = new (ELeave) CMceComAvcCodec();
    CleanupStack::PushL( self );
    self->ConstructL( aSdpName );
    return self;
    }


// -----------------------------------------------------------------------------
// CMceComAvcCodec::~CMceComAvcCodec
// -----------------------------------------------------------------------------
//
CMceComAvcCodec::~CMceComAvcCodec()
    {
    }
    

// -----------------------------------------------------------------------------
// CMceComAvcCodec::CMceComAvcCodec
// -----------------------------------------------------------------------------
//
CMceComAvcCodec::CMceComAvcCodec()
    :CMceComVideoCodec()
    {
    iCodecMode = KMceAvcModeSingleNal;
    iClockRate = KMceAvcDefaultFrameRate;
    iFrameRate = KMceAvcDefaultFrameRate;
    iResolutionWidth = KMceAvcDefaultFrameWidth;
    iResolutionHeight = KMceAvcDefaultFrameHeight;
    iBitrate = KMceAvcLevel1BitrateBps;
	iMaxBitRate = KMceAvcLevel1BitrateBps;
	iAllowedBitrates = KMceAvcDefaultAllowedBitrate;
    iFrameSize = KMceAvcDefaultFrameSize;
    // Defaults should be defined in MCE API
    const TUint8 KMceDefaultAvcPayloadType = 98;
	iPayloadType = KMceDefaultAvcPayloadType;
    
    iReceiveFrameRate = KMceAvcDefaultFrameRate;
    }


// -----------------------------------------------------------------------------
// CMceComAvcCodec::ConstructL
// -----------------------------------------------------------------------------
//
CMceComVideoCodec* CMceComAvcCodec::CloneL()
    {
    CMceComAvcCodec* copy = new (ELeave) CMceComAvcCodec();
    CleanupStack::PushL( copy );
    copy->ConstructL( *this );
    CleanupStack::Pop( copy );
    return copy;
    
    }


// -----------------------------------------------------------------------------
// CMceComAvcCodec::ConstructL
// -----------------------------------------------------------------------------
//
void CMceComAvcCodec::ConstructL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComVideoCodec::ConstructL( aSdpName );
    
    }


// -----------------------------------------------------------------------------
// CMceComAvcCodec::ConstructL
// -----------------------------------------------------------------------------
//
void CMceComAvcCodec::ConstructL( CMceComAvcCodec& aCodec )
    {
    CMceComVideoCodec::ConstructL( aCodec );
    }
    
// -----------------------------------------------------------------------------
// CMceComAvcCodec::SetBitrate
// -----------------------------------------------------------------------------
//
TInt CMceComAvcCodec::SetBitrate( TUint aBitrate )
    {
	return CMceComCodec::SetBitrate( aBitrate );
    }
    
// -----------------------------------------------------------------------------
// CMceComAvcCodec::SetAllowedBitrates
// -----------------------------------------------------------------------------
//
TInt CMceComAvcCodec::SetAllowedBitrates( TUint aBitrates )
    {
    TInt error = KErrNone;
    
    //Confirm that the bitrate mask is valid
    //I.e. after all the valid bitrates are set to zero the value should be zero
    if ( !aBitrates || 
        ( aBitrates >> 16 ) > 0 ) 
        {
        error = KErrArgument;
        }
    else
        {
        const TUint KMceOtherInfoMask = 0xFF00;
        
        if ( aBitrates == KMceAllowedAvcBitrateAll )
            {
            // Start using recommended high bitrate
            aBitrates = KMceAvcAllowedBitrateRecommendedHigh;
            }
            
        CMceComCodec::SetAllowedBitrates( KMceAllowedAvcBitrateAll );
        iAllowedBitrates &= aBitrates;
        TUint maxBitrate = GetMaxBitRate();
        CMceComVideoCodec::SetMaxBitrate( maxBitrate );
        if ( iBitrate > maxBitrate )
            {
            error = CMceComCodec::SetBitrate( maxBitrate );
            } 
            
        // Update also other avc codec information stored inside
        // allowed bitrates mask
        iAllowedBitrates |= ( aBitrates & KMceOtherInfoMask );
        }
    
    return error;
    }



// -----------------------------------------------------------------------------
// CMceComAvcCodec::GetMaxBitRate
// -----------------------------------------------------------------------------
//
TUint CMceComAvcCodec::GetMaxBitRate()
    {
    TUint maxBitrate( 0 );
    if( iAllowedBitrates & KMceAvcBitrateLevel1 )
        {
        maxBitrate = KMceAvcLevel1BitrateBps;
        }
    else if( iAllowedBitrates & KMceAvcBitrateLevel1b )
        {
        maxBitrate = KMceAvcLevel1bBitrateBps;
        }
    else if( iAllowedBitrates & KMceAvcBitrateLevel1_1 )
        {
        maxBitrate = KMceAvcLevel11BitrateBps;
        }
    else if( iAllowedBitrates & KMceAvcBitrateLevel1_2 )
        {
        maxBitrate = KMceAvcLevel12BitrateBps;
        }
    else if( iAllowedBitrates & KMceAvcBitrateLevel1_3 )
        {
        maxBitrate = KMceAvcLevel13BitrateBps;
        }
    else if( iAllowedBitrates & KMceAvcBitrateLevel2 )
        {
        maxBitrate = KMceAvcLevel2BitrateBps;
        }
    else
        {
        // NOP
        }
        
    return maxBitrate;
    }

// -----------------------------------------------------------------------------
// CMceComAvcCodec::SetMaxBitrate
// -----------------------------------------------------------------------------
//
TInt CMceComAvcCodec::SetMaxBitrate( TUint aMaxBitrate )
    {
    TInt status = KErrNone;
	switch( aMaxBitrate )
	    {
	    case KMceAvcLevel1BitrateBps:
	        {
	        status = SetAllowedBitrates( KMceAvcCodecProfileIdBaseline |
	                                     KMceAvcCodecProfileIopConstraintSet | 
	                                     KMceAvcBitrateLevel1 );
	        break;
	        }
	    case KMceAvcLevel1bBitrateBps:
	        {
	        status = SetAllowedBitrates( KMceAvcCodecProfileIdBaseline |
	                                     KMceAvcCodecProfileIopConstraintSet | 
	                                     KMceAvcBitrateLevel1b );
	        break;
	        }
	    case KMceAvcLevel11BitrateBps:
	        {
	        status = SetAllowedBitrates( KMceAvcCodecProfileIdBaseline |
	                                     KMceAvcCodecProfileIopConstraintSet | 
	                                     KMceAvcBitrateLevel1_1 );
	        break;
	        }
	    case KMceAvcLevel12BitrateBps:
	        {
	        status = SetAllowedBitrates( KMceAvcCodecProfileIdBaseline |
	                                     KMceAvcCodecProfileIopConstraintSet | 
	                                     KMceAvcBitrateLevel1_2 );
	        break;
	        }
	    case KMceAvcLevel13BitrateBps:
	        {
	        status = SetAllowedBitrates( KMceAvcCodecProfileIdBaseline |
	                                     KMceAvcCodecProfileIopConstraintSet | 
	                                     KMceAvcBitrateLevel1_3 );
	        break;
	        }
	    case KMceAvcLevel2BitrateBps:
	        {
	        status = SetAllowedBitrates( KMceAvcCodecProfileIdBaseline |
	                                     KMceAvcCodecProfileIopConstraintSet | 
	                                     KMceAvcBitrateLevel2 );
	        break;
	        }
        default:
            {
            status = KErrNotSupported;
            break;
            }
	    }

    return status;
    
    }

// -----------------------------------------------------------------------------
// CMceComAvcCodec::ResolveAllowedBitrates
// -----------------------------------------------------------------------------
//
TInt CMceComAvcCodec::ResolveAllowedBitrates( 
    TUint aBitrate, TUint& aAllowedBitrates )
    {
    TInt retVal( KErrNone );
    
    if ( aBitrate <= KMceAvcLevel1BitrateBps )
        {
        aAllowedBitrates =  KMceAvcCodecProfileIdBaseline | 
	                        KMceAvcCodecProfileIopConstraintSet | 
	                        KMceAvcBitrateLevel1;
        }
    else if ( aBitrate <= KMceAvcLevel1bBitrateBps )
        {
        aAllowedBitrates =  KMceAvcCodecProfileIdBaseline | 
	                        KMceAvcCodecProfileIopConstraintSet | 
	                        KMceAvcBitrateLevel1b;
        }
    else if ( aBitrate <= KMceAvcLevel11BitrateBps )
        {
        aAllowedBitrates =  KMceAvcCodecProfileIdBaseline | 
	                        KMceAvcCodecProfileIopConstraintSet | 
	                        KMceAvcBitrateLevel1_1;
        }
    else if ( aBitrate <= KMceAvcLevel12BitrateBps )
        {
        aAllowedBitrates =  KMceAvcCodecProfileIdBaseline | 
	                        KMceAvcCodecProfileIopConstraintSet | 
	                        KMceAvcBitrateLevel1_2;
        }
    else if ( aBitrate <= KMceAvcLevel13BitrateBps )
        {
        aAllowedBitrates =  KMceAvcCodecProfileIdBaseline | 
	                        KMceAvcCodecProfileIopConstraintSet | 
	                        KMceAvcBitrateLevel1_3;
        }
    else if ( aBitrate <= KMceAvcLevel2BitrateBps )
        {
        aAllowedBitrates =  KMceAvcCodecProfileIdBaseline | 
	                        KMceAvcCodecProfileIopConstraintSet | 
	                        KMceAvcBitrateLevel2;
        }    
    else 
        {
        retVal = KErrNotSupported;
        }
        
    return retVal;
    }

// -----------------------------------------------------------------------------
// CMceComAvcCodec::Decodes
// For AVC, fmtp value contains profile level and packetization mode 
// information which need to be taken in count (same codec cannot be
// used for different levels or packetization modes). If less strict matching
// is used, profile level is ignored but packetization mode is still checked
// -----------------------------------------------------------------------------
//
TBool CMceComAvcCodec::Decodes( 
    const TDesC8& aRtpmapValue, 
    const TDesC8& aFmtpValue,
    TBool aIsStrictMatch )
    {
    TBool decodes = CMceComCodec::Decodes( aRtpmapValue, aFmtpValue, aIsStrictMatch );
    if ( decodes )
        {
        TPtrC8 existingFmtpVal = FmtpAttr();
        
        TPtrC8 packetizationMode = ParsePacketizationMode( aFmtpValue );
        TPtrC8 existingPacketizationMode = ParsePacketizationMode( existingFmtpVal );
        decodes = ( packetizationMode.CompareF( existingPacketizationMode ) == 0 );

        if ( decodes && aIsStrictMatch )
            {
            TPtrC8 profileLevelId = ParseProfileLevelId( aFmtpValue );
                
            TPtrC8 existingProfileLevelId = ParseProfileLevelId( existingFmtpVal );
            decodes = ( profileLevelId.CompareF( existingProfileLevelId ) == 0 );
            }
        }
    return decodes;
    }

// -----------------------------------------------------------------------------
// CMceComAvcCodec::ParseProfileLevelId
// -----------------------------------------------------------------------------
//
TPtrC8 CMceComAvcCodec::ParseProfileLevelId( const TDesC8& aFmtpValue )
    {
    // Non-existing profile-level-id means level 1  
    TPtrC8 profileLevelIdVal = 
        GetFmtpAttributeValuePart( aFmtpValue, 
                                   KMceAvcProfileLevelIdFmtp(), 
                                   KMceAvcProfileLevelIdValLen, 
                                   KMceAvcProfileLevel1 );
                               
    return profileLevelIdVal;
    }

// -----------------------------------------------------------------------------
// CMceComAvcCodec::ParseProfileLevelIdValPart
// -----------------------------------------------------------------------------
//
TPtrC8 CMceComAvcCodec::ParseProfileLevelIdValPart( 
    const TDesC8& aFmtpValue,
    TInt aValPartOffset,
    TInt aValPartLen,
    const TDesC8& aDefaultVal )
    {
    TPtrC8 profileLevelVal( aDefaultVal );
    
    TPtrC8 profileLevelIdVal = ParseProfileLevelId( aFmtpValue );
    if ( profileLevelIdVal.Length() >= aValPartOffset + aValPartLen )
        {
        // Take just value part
        profileLevelVal.Set( profileLevelIdVal.Mid( aValPartOffset, aValPartLen ) );
        }

    return profileLevelVal;
    }

// -----------------------------------------------------------------------------
// CMceComAvcCodec::ReplaceProfileLevelIdValuePart
// -----------------------------------------------------------------------------
//
TInt CMceComAvcCodec::ReplaceProfileLevelIdValuePart( 
    TDes8& aModifiedFmtp, 
    const TDesC8& aReplacement,
    TInt aValPartOffset )
    {
    TInt err( KErrArgument );
    
    TInt startIndex = aModifiedFmtp.FindF( KMceAvcProfileLevelIdFmtp() );
    startIndex = ( startIndex != KErrNotFound ) ? 
        ( startIndex + KMceAvcProfileLevelIdFmtp().Length() ) : startIndex;
    if ( startIndex != KErrNotFound && 
       ( startIndex + aValPartOffset + aReplacement.Length() ) <= aModifiedFmtp.Length() )
        {
        aModifiedFmtp.Replace( 
            startIndex + aValPartOffset, 
            aReplacement.Length(), 
            aReplacement );
        err = KErrNone;
        }
    return err;
    }
    
// -----------------------------------------------------------------------------
// CMceComAvcCodec::ParsePacketizationMode
// -----------------------------------------------------------------------------
//
TPtrC8 CMceComAvcCodec::ParsePacketizationMode( const TDesC8& aFmtpValue )
    {
    // Non-existing packetization mode means single nal mode
    TPtrC8 packetizationModeVal =
        GetFmtpAttributeValuePart( aFmtpValue, 
                                   KMceAvcPacketizationModeFmtp(), 
                                   KMceAvcPacketizationModeValLen, 
                                   KMceAvcPacketizationModeSingleNal );
   
    return packetizationModeVal;
    }

// -----------------------------------------------------------------------------
// CMceComAvcCodec::GetFmtpAttributeValuePart
// -----------------------------------------------------------------------------
//
TPtrC8 CMceComAvcCodec::GetFmtpAttributeValuePart( 
    const TDesC8& aFmtpValue,
    const TDesC8& aFmtpAttributeName, 
    TInt aValLen, 
    const TDesC8& aDefaultVal )
    {
    TPtrC8 outputValuePart( aDefaultVal );
    
    TInt valPos = aFmtpValue.FindF( aFmtpAttributeName );
    valPos = ( valPos != KErrNotFound ) ? 
                ( valPos + aFmtpAttributeName.Length() ) : valPos;
    if ( valPos != KErrNotFound && 
       ( valPos + aValLen ) <= aFmtpValue.Length() )
        {
        outputValuePart.Set( aFmtpValue.Mid( valPos, aValLen ) );
        }
    return outputValuePart;
    }
    
#ifdef MCE_COMMON_SERVER_SIDE

// -----------------------------------------------------------------------------
// CMceComAvcCodec::SetFmtpProposalL
// -----------------------------------------------------------------------------
//
TBool CMceComAvcCodec::SetFmtpProposalL( const TDesC8& aFmtpProposal )
    {
    HBufC8* proposal = aFmtpProposal.AllocL();
    delete iFmtpProposal;
    iFmtpProposal = proposal;
    
    CMccCodecInformationFactory* factory = CMccCodecInformationFactory::NewL();
    CleanupStack::PushL( factory );
    CMccCodecInformation* codec = 
        factory->CreateCodecInformationL( iSdpName );
    CleanupStack::PushL( codec );
    
    TRAPD( err, codec->SetFmtpAttrL( *iFmtpProposal ) );
    if ( err == KErrNoMemory )
        {
        User::Leave( err );
        }
    
    CleanupStack::PopAndDestroy( codec );
    CleanupStack::PopAndDestroy( factory );
    
    return ( err == KErrNone );
    }

// -----------------------------------------------------------------------------
// CMceComAvcCodec::SetFmtpFromIncomingSdpL
// -----------------------------------------------------------------------------
//
void CMceComAvcCodec::SetFmtpFromIncomingSdpL( 
    const TDesC8& aFmtpAttribute, 
    TBool aRoleAnswerer,
    TBool aReset )
    {
    HBufC8* fmtp = aFmtpAttribute.AllocLC();
    TPtr8 ptrFmtp( fmtp->Des() );
    
    if ( !aRoleAnswerer )
        {
        // If no strict match, we are accepting fmtp even it is against specs.
        // In that case, replace level value with existing level
        TBool decodes = Decodes( iSdpName, aFmtpAttribute );
        if ( !decodes )
            {
            TPtrC8 existingLevel = 
                ParseProfileLevelIdValPart( FmtpAttr(), 
                                            KMceAvcProfileLevelValOffset, 
                                            KMceAvcProfileLevelValLen, 
                                            KMceAvcProfileLevelOnly1 );
            ReplaceProfileLevelIdValuePart( 
                    ptrFmtp, existingLevel, KMceAvcProfileLevelValOffset );
            }
            
        // In offerer side, tolerate also incorrect iop part usage
        // by using existing iop val
        TPtrC8 existingIop = 
            ParseProfileLevelIdValPart( FmtpAttr(), 
                                        KMceAvcProfileIopValOffset, 
                                        KMceAvcProfileIopValLen, 
                                        KMceAvcProfileIopDefault );
        ReplaceProfileLevelIdValuePart( 
                    ptrFmtp, existingIop, KMceAvcProfileIopValOffset );
        }
     
    if ( iStream && iStream->BoundStream() )
        {
        // In case of two-way video streams, sprop-parameter-set (configkey)
        // from incoming sdp is put to receive stream and it will not override
        // existing key in sendstream
        if ( IS_SENDSTREAM( iStream ) && IS_RECEIVESTREAM( &iStream->BoundStreamL() ) )
            {
            CMceComVideoCodec* boundCodec = 
                static_cast<CMceComVideoCodec*>(
                    iStream->BoundStreamL().FindCodecL( *this ) );

            HBufC8* spropParameterSetsVal = 
                StripFmtpParameterL( ptrFmtp, KMceAvcSpropParameterSetsFmtp() );
            CleanupStack::PushL( spropParameterSetsVal );
            if ( boundCodec && 
                 CMceComVideoCodec::IsConfigKeyValid( spropParameterSetsVal ) )
                {
                boundCodec->SetConfigKeyL( spropParameterSetsVal );
                }
            CleanupStack::PopAndDestroy( spropParameterSetsVal );    
            }
        }
    
    SetFmtpAttributeL( ptrFmtp, aReset );
        
    CleanupStack::PopAndDestroy( fmtp );
    }

// -----------------------------------------------------------------------------
// CMceComAvcCodec::FtmpForOutgoingSdpLC
// -----------------------------------------------------------------------------
//    
HBufC8* CMceComAvcCodec::FtmpForOutgoingSdpLC()
    {
    HBufC8* fmtp = CMceComCodec::FtmpForOutgoingSdpLC();
    if ( iStream && iStream->BoundStream() )
        {
        // In case of two-way video streams, sprop-parameter-set (configkey)
        // for outgoing sdp is taken from send stream
        if ( IS_RECEIVESTREAM( iStream ) && IS_SENDSTREAM( &iStream->BoundStreamL() ) )
            {
            TPtr8 ptrFmtp( fmtp->Des() );
            
            CMceComVideoCodec* boundCodec = 
                static_cast<CMceComVideoCodec*>( 
                    iStream->BoundStreamL().FindCodecL( *this ) );
            if ( boundCodec && boundCodec->IsConfigKeyValid() )
                {
                HBufC8* spropParameterSetsVal = 
                    StripFmtpParameterL( ptrFmtp, KMceAvcSpropParameterSetsFmtp() );
                delete spropParameterSetsVal;
                
                HBufC8* newFmtp = AddFmtpParameterL( ptrFmtp,  
                                                     KMceAvcSpropParameterSetsFmtp(), 
                                                     *boundCodec->iConfigKey );
                CleanupStack::PopAndDestroy( fmtp );
                fmtp = newFmtp;
                CleanupStack::PushL( fmtp );
                }
                
            }
        }
    return fmtp;
    }

// -----------------------------------------------------------------------------
// CMceComAvcCodec::StripFmtpParameterL
// -----------------------------------------------------------------------------
//
HBufC8* CMceComAvcCodec::StripFmtpParameterL( 
    TPtr8& aInputFmtp, 
    const TDesC8& aStrippedParameter )
    {
    HBufC8* strippedParameter = NULL;
    TInt startIndex = aInputFmtp.FindF( aStrippedParameter );
    if ( startIndex != KErrNotFound )
        {
        TPtrC8 parameterVal = aInputFmtp.Mid( startIndex + aStrippedParameter.Length() );
        TInt endIndex = parameterVal.Locate( KMceAvcFmtpParameterSeparator );
        if ( endIndex == KErrNotFound )
            {
            endIndex = parameterVal.Length();
            }    
        
        if ( endIndex > 0 )
            {
            strippedParameter = parameterVal.Mid( 0, endIndex ).AllocLC();
            
            // Remove stripped part from input fmtp
            TInt deleteLen( endIndex + aStrippedParameter.Length() + 1 );
            
            aInputFmtp.Delete( startIndex, deleteLen );
            aInputFmtp.Trim();
            
            CleanupStack::Pop( strippedParameter );
            }
        }
    return strippedParameter;
    }

// -----------------------------------------------------------------------------
// CMceComAvcCodec::AddFmtpParameterL
// -----------------------------------------------------------------------------
//
HBufC8* CMceComAvcCodec::AddFmtpParameterL( 
    const TDesC8& aInputFmtp, 
    const TDesC8& aAddedParameter, 
    const TDesC8& aAddedParameterVal )
    {
    const TInt KMceAvcAdditionalLen = 2;
    HBufC8* outputFmtp = HBufC8::NewL( aInputFmtp.Length() + 
                                       aAddedParameter.Length() + 
                                       aAddedParameterVal.Length() + 
                                       KMceAvcAdditionalLen );
    TPtr8 ptrOutputFmtp( outputFmtp->Des() );
    
    // Add original param
    ptrOutputFmtp.Append( aInputFmtp );
    
    // Add separators if needed
    TInt lastSeparatorIndex = aInputFmtp.LocateReverse( KMceAvcFmtpParameterSeparator );
    if ( aInputFmtp.Length() > 0 && lastSeparatorIndex < ( aInputFmtp.Length() - 2 ) )
        {
        ptrOutputFmtp.Append( KMceAvcFmtpParameterSeparator );
        }
    TInt lastSpaceIndex = aInputFmtp.LocateReverse( KMceAvcFtmpParameterSpace );
    if ( aInputFmtp.Length() > 0 && lastSpaceIndex < ( aInputFmtp.Length() - 1 ) )
        {
        ptrOutputFmtp.Append( KMceAvcFtmpParameterSpace );
        }
        
    // Add new parameter
    ptrOutputFmtp.Append( aAddedParameter );
    ptrOutputFmtp.Append( aAddedParameterVal );
    
    return outputFmtp;
    }
    
#endif
        
// End of File

    
