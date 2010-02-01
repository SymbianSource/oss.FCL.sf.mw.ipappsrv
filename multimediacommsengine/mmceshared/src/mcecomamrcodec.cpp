/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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




#include "mceamrcodec.h"
#include "mcecomamrcodec.h"

#ifdef MCE_COMMON_SERVER_SIDE

#include <mmcccodecinformation.h>

_LIT8( KDefaultAmrFmtp, "octet-align=0; mode-set=0,1,2,3,4,5,6,7" );

#endif//MCE_COMMON_SERVER_SIDE

const TUint KMceAmrFrameTime( 20 );


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceComAMRCodec::NewL
// -----------------------------------------------------------------------------
//
CMceComAMRCodec* CMceComAMRCodec::NewL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComAMRCodec* self = NewLC( aSdpName );
    CleanupStack::Pop( self );
    return self;
    
    }

// -----------------------------------------------------------------------------
// CMceComAMRCodec::NewLC
// -----------------------------------------------------------------------------
//
CMceComAMRCodec* CMceComAMRCodec::NewLC( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComAMRCodec* self = new (ELeave) CMceComAMRCodec();
    CleanupStack::PushL( self );
    self->ConstructL( aSdpName );
    return self;
    }


// -----------------------------------------------------------------------------
// CMceComAMRCodec::~CMceComAMRCodec
// -----------------------------------------------------------------------------
//
CMceComAMRCodec::~CMceComAMRCodec()
    {
    }
    

// -----------------------------------------------------------------------------
// CMceComAMRCodec::CMceComAMRCodec
// -----------------------------------------------------------------------------
//
CMceComAMRCodec::CMceComAMRCodec()
 : CMceComAudioCodec()
    {
    iFourCC = 0;
    iEnableVAD = EFalse;
	iSamplingFreq = KMceAmrNbSamplingFreq;
	iPTime = KMceDefaultPtime;
	iMaxPTime = KMceDefaultMaxPtime;
	iBitrate = KMceAmrNbBitrate122;	
	iAllowedBitrates = KMceAllowedAmrNbBitrate475|
					   KMceAllowedAmrNbBitrate515|
					   KMceAllowedAmrNbBitrate590|
					   KMceAllowedAmrNbBitrate670|
					   KMceAllowedAmrNbBitrate740|
					   KMceAllowedAmrNbBitrate795|
					   KMceAllowedAmrNbBitrate102|
					   KMceAllowedAmrNbBitrate122;
    
    const TUint8 KMceDefaultAmrPayloadType = 96;
	iPayloadType = KMceDefaultAmrPayloadType;
	iCodecMode = EMceBandwidthEfficient;    
    iFrameSize = KMceAmrFrameTime;
    }



// -----------------------------------------------------------------------------
// CMceComAMRCodec::CloneL
// -----------------------------------------------------------------------------
//
CMceComAudioCodec* CMceComAMRCodec::CloneL()
    {
    CMceComAMRCodec* copy = new (ELeave) CMceComAMRCodec();
    CleanupStack::PushL( copy );
    copy->ConstructL( *this );
    CleanupStack::Pop( copy );
    return copy;
    
    }


// -----------------------------------------------------------------------------
// CMceComAMRCodec::ConstructL
// -----------------------------------------------------------------------------
//
void CMceComAMRCodec::ConstructL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComAudioCodec::ConstructL( aSdpName );
    
    }


// -----------------------------------------------------------------------------
// CMceComAMRCodec::ConstructL
// -----------------------------------------------------------------------------
//
void CMceComAMRCodec::ConstructL( CMceComAMRCodec& aCodec )
    {
    CMceComAudioCodec::ConstructL( aCodec );
    }


// -----------------------------------------------------------------------------
// CMceComAMRCodec::SetBitrate
// -----------------------------------------------------------------------------
//
TInt CMceComAMRCodec::SetBitrate( TUint aBitrate )
    {
	TBool isValid = ETrue;
	
    switch ( aBitrate )
    	{
    	case KMceAmrNbBitrate475:
    	case KMceAmrNbBitrate515:
    	case KMceAmrNbBitrate590:
    	case KMceAmrNbBitrate670:
    	case KMceAmrNbBitrate740:
    	case KMceAmrNbBitrate795:
    	case KMceAmrNbBitrate102:
    	case KMceAmrNbBitrate122:
    	    {
    	    if ( aBitrate > GetMaxBitRate() )
    	        {
        		isValid = EFalse;
    	        }
    	    break;
    	    }
    	default:
    		isValid = EFalse;
    		break;
    	}
    	
    if ( isValid )
    	{
        return CMceComCodec::SetBitrate( aBitrate );
    	}
    else
    	{
    	return KErrNotSupported;
    	}
    
    }
    
// -----------------------------------------------------------------------------
// CMceComAMRCodec::SetAllowedBitrates
// -----------------------------------------------------------------------------
//
TInt CMceComAMRCodec::SetAllowedBitrates( TUint aBitrates )
    {
    TInt error = KErrNone;
    
    //Confirm that the bitrate mask is valid
    //I.e. after all the valid bitrates are set to zero the value should be zero
    if ( !aBitrates || 
        ( aBitrates >> 12 ) > 0 ) 
        {
        error = KErrArgument;
        }
    else
        {
        CMceComCodec::SetAllowedBitrates( KMceAllowedAmrNbBitrateAll );
        iAllowedBitrates &= aBitrates;
        TUint maxBitrate = GetMaxBitRate();
        if ( iBitrate > maxBitrate )
            {
            error = CMceComCodec::SetBitrate( maxBitrate );
            } 
            
        // Update also other amr codec information stored inside
        // allowed bitrates mask
        const TUint KMceOtherInfoMask = 0x0F00;
        iAllowedBitrates |= ( aBitrates & KMceOtherInfoMask );
        }
    
    return error;
    
    }


// -----------------------------------------------------------------------------
// CMceComAMRCodec::SetCodecMode
// -----------------------------------------------------------------------------
//
TInt CMceComAMRCodec::SetCodecMode( TUint aCodecMode )
    {
    if ( EMceBandwidthEfficient == aCodecMode || EMceOctetAligned == aCodecMode )
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
// CMceComAMRCodec::SetPayloadType
// -----------------------------------------------------------------------------
//
TInt CMceComAMRCodec::SetPayloadType( TUint8 aPayloadType )
    {
    if ( KMinDynPayloadType <= aPayloadType )
        {
        return CMceComCodec::SetPayloadType( aPayloadType );
        }
    else
        {
        return KErrNotSupported;
        }
    }


// -----------------------------------------------------------------------------
// CMceComAMRCodec::GetMaxBitRate
// -----------------------------------------------------------------------------
//
TUint CMceComAMRCodec::GetMaxBitRate()
	{
    if( iAllowedBitrates & KMceAllowedAmrNbBitrate122 )
        {
        return KMceAmrNbBitrate122;
        }
    else if( iAllowedBitrates & KMceAllowedAmrNbBitrate102 )
        {
        return KMceAmrNbBitrate102;
        }
    else if( iAllowedBitrates & KMceAllowedAmrNbBitrate795 )
        {
        return KMceAmrNbBitrate795;
        }   
    else if( iAllowedBitrates & KMceAllowedAmrNbBitrate740 )
        {
        return KMceAmrNbBitrate740;
        }   
    else if( iAllowedBitrates & KMceAllowedAmrNbBitrate670 )
        {
        return KMceAmrNbBitrate670;
        }   
    else if( iAllowedBitrates & KMceAllowedAmrNbBitrate590 )
        {
        return KMceAmrNbBitrate590;
        }
    else if( iAllowedBitrates & KMceAllowedAmrNbBitrate515 )
        {
        return KMceAmrNbBitrate515;
        }  
    else if( iAllowedBitrates & KMceAllowedAmrNbBitrate475 )
        {
        return KMceAmrNbBitrate475;
        }
    else
        {
        return 0;    
        }
	}
	

// -----------------------------------------------------------------------------
// CMceComAMRCodec::ResolveAllowedBitrates
// -----------------------------------------------------------------------------
//
TInt CMceComAMRCodec::ResolveAllowedBitrates( 
    TUint aBitrate, TUint& aAllowedBitrates )
    {
    TInt retVal( KErrNone );
    
    if ( aBitrate <= KMceAmrNbBitrate475 )
        {
        aAllowedBitrates = KMceAllowedAmrNbBitrate475;
        }
    else if ( aBitrate <= KMceAmrNbBitrate515 )
        {
        aAllowedBitrates = KMceAllowedAmrNbBitrate515;
        }
    else if ( aBitrate <= KMceAmrNbBitrate590 )
        {
        aAllowedBitrates = KMceAllowedAmrNbBitrate590;
        }
    else if ( aBitrate <= KMceAmrNbBitrate670 )
        {
        aAllowedBitrates = KMceAllowedAmrNbBitrate670;
        }
    else if ( aBitrate <= KMceAmrNbBitrate740 )
        {
        aAllowedBitrates = KMceAllowedAmrNbBitrate740;
        }
    else if ( aBitrate <= KMceAmrNbBitrate795 )
        {
        aAllowedBitrates = KMceAllowedAmrNbBitrate795;
        }
    else if ( aBitrate <= KMceAmrNbBitrate102 )
        {
        aAllowedBitrates = KMceAllowedAmrNbBitrate102;
        }
    else if ( aBitrate <= KMceAmrNbBitrate122 )
        {
        aAllowedBitrates = KMceAllowedAmrNbBitrateAll;
        }
    else
        {
        retVal = KErrNotSupported;
        }
    
    return retVal;
    }


// -----------------------------------------------------------------------------
// CMceComAMRCodec::SetSamplingFreq
// -----------------------------------------------------------------------------
//
TInt CMceComAMRCodec::SetSamplingFreq( TUint aSamplingFreq )
    {
    if ( KMceAmrNbSamplingFreq == aSamplingFreq || 
         KMceAmrWbSamplingFreq == aSamplingFreq )
        {
        iSamplingFreq = aSamplingFreq;
        return KErrNone;
        }
    else
        {
        return KErrNotSupported;
        }
    }


// -----------------------------------------------------------------------------
// CMceComAMRCodec::SetPTime
// -----------------------------------------------------------------------------
//
TInt CMceComAMRCodec::SetPTime( TUint aPTime )
    {
    if ( ( aPTime % KMceAmrFrameTime ) == 0 )
        {
        return CMceComAudioCodec::SetPTime( aPTime );
        }
    else
        {
        return KErrNotSupported;
        }
    }


// -----------------------------------------------------------------------------
// CMceComAudioCodec::SetMaxPTime
// -----------------------------------------------------------------------------
//
TInt CMceComAMRCodec::SetMaxPTime( TUint aMaxPTime )
    {
    if ( ( aMaxPTime % KMceAmrFrameTime ) == 0 )
        {
        return CMceComAudioCodec::SetMaxPTime( aMaxPTime );
        }
    else
        {
        return KErrNotSupported;
        }
    }


#ifdef MCE_COMMON_SERVER_SIDE

// -----------------------------------------------------------------------------
// CMceComAMRCodec::DoMccValidateL
// -----------------------------------------------------------------------------
//
CMceComCodec* CMceComAMRCodec::DoMccValidateL( CMccCodecInformation& aMccCodec,
                                               CMceSrvStream& /*aStream*/,
                                               TMceNegotiationRole aRole )
    {
    //if answerer mcc codec represents the received offer and codec the answer
    //if offerer codec represents the sent offer and mcc codec the answer
    
    // Bitrates field contains also additional information, mask it away
    TUint mccBitrates = ( aMccCodec.AllowedBitrates() & KMceAllowedAmrNbBitrateAll );
    TUint mceBitrates = ( iAllowedBitrates & KMceAllowedAmrNbBitrateAll );
    
    TBool notValid = aRole == EMceRoleAnswerer ?
                  ( mceBitrates && mccBitrates == 0 ||
	                mceBitrates > mccBitrates ) :
                  ( mceBitrates && mccBitrates == 0 ||
	                mceBitrates < mccBitrates );
	
	User::LeaveIfError( notValid ? KErrNotSupported : KErrNone );
	if ( aMccCodec.CodecMode() != (TCodecMode)iCodecMode )
		{
		// codec-mode was different, fail.
		User::Leave(KErrNotSupported);
		}
	return NULL;
    }

// -----------------------------------------------------------------------------
// CMceComAMRCodec::DoSetDefaultFmtpAttributeL
// -----------------------------------------------------------------------------
//
void CMceComAMRCodec::DoSetDefaultFmtpAttributeL()
    {
    SetFmtpAttributeL( KDefaultAmrFmtp );   
    }
    
#endif

// End of File
