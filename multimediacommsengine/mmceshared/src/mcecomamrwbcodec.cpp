/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
#include "mceamrwbcodec.h"
#include "mcecomamrwbcodec.h"

#ifdef MCE_COMMON_SERVER_SIDE

#include <mmcccodecinformation.h>

_LIT8( KDefaultAmrWbFmtp, "octet-align=0; mode-set=0,1,2,3,4,5,6,7,8" );

#endif//MCE_COMMON_SERVER_SIDE

// This should be defined in MCE API
const TUint KMceAmrWBFrameSize ( 64 );


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceComAMRWbCodec::NewL
// -----------------------------------------------------------------------------
//
CMceComAMRWbCodec* CMceComAMRWbCodec::NewL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComAMRWbCodec* self = NewLC( aSdpName );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMceComAMRWbCodec::NewLC
// -----------------------------------------------------------------------------
//
CMceComAMRWbCodec* CMceComAMRWbCodec::NewLC( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComAMRWbCodec* self = new (ELeave) CMceComAMRWbCodec();
    CleanupStack::PushL( self );
    self->ConstructL( aSdpName );
    return self;
    }

// -----------------------------------------------------------------------------
// CMceComAMRWbCodec::~CMceComAMRWbCodec
// -----------------------------------------------------------------------------
//
CMceComAMRWbCodec::~CMceComAMRWbCodec()
    {
    }

// -----------------------------------------------------------------------------
// CMceComAMRWbCodec::CMceComAMRWbCodec
// -----------------------------------------------------------------------------
//
CMceComAMRWbCodec::CMceComAMRWbCodec()
 : CMceComAMRCodec()
    {
    iCodecMode = EMceBandwidthEfficient;    
    iBitrate = KMceAmrWbBitrate2385;	
   	iAllowedBitrates = KMceAllowedAmrWbBitrate660|
					   KMceAllowedAmrWbBitrate885|
					   KMceAllowedAmrWbBitrate1265|
					   KMceAllowedAmrWbBitrate1425|
					   KMceAllowedAmrWbBitrate1585|
					   KMceAllowedAmrWbBitrate1825|
					   KMceAllowedAmrWbBitrate1985|
					   KMceAllowedAmrWbBitrate2305|
					   KMceAllowedAmrWbBitrate2385;
	//Above codec mode need to have value as 8
    iFrameSize = KMceAmrWBFrameSize;
   	const TUint8 KDefaultAmrWbPayloadType = 100;
	iPayloadType = KDefaultAmrWbPayloadType;
	iSamplingFreq = KMceAmrWbSamplingFreq;
	iPTime = KMceDefaultPtime;
	iMaxPTime = KMceDefaultMaxPtime;
    }


// -----------------------------------------------------------------------------
// CMceComAMRWbCodec::CloneL
// -----------------------------------------------------------------------------
//
CMceComAudioCodec* CMceComAMRWbCodec::CloneL()
    {
    CMceComAMRWbCodec* copy = new (ELeave) CMceComAMRWbCodec();
    CleanupStack::PushL( copy );
    copy->ConstructL( *this );
    CleanupStack::Pop( copy );
    return copy;
    
    }

// -----------------------------------------------------------------------------
// CMceComAMRWbCodec::SetBitrate
// -----------------------------------------------------------------------------
//
TInt CMceComAMRWbCodec::SetBitrate( TUint aBitrate )
    {
	TBool isValid = ETrue;
	
    switch ( aBitrate )
    	{
    	case KMceAmrWbBitrate660:
		case KMceAmrWbBitrate885:
		case KMceAmrWbBitrate1265:
		case KMceAmrWbBitrate1425:
		case KMceAmrWbBitrate1585:
		case KMceAmrWbBitrate1825:
		case KMceAmrWbBitrate1985:
		case KMceAmrWbBitrate2305:
		case KMceAmrWbBitrate2385:
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
// CMceComAMRWbCodec::SetAllowedBitrates
// -----------------------------------------------------------------------------
//
TInt CMceComAMRWbCodec::SetAllowedBitrates( TUint aBitrates )
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
        CMceComCodec::SetAllowedBitrates( KMceAllowedAmrWbBitrateAll );
        iAllowedBitrates &= aBitrates;
        TUint maxBitrate = GetMaxBitRate();
        if ( iBitrate > maxBitrate )
            {
            error = CMceComCodec::SetBitrate( maxBitrate );
            } 
            
        // Update also other amr codec information stored inside
        iAllowedBitrates |= ( aBitrates & KMceAllowedAmrWbAdditionalInfo );
        }
    
    return error;
    
    }
    

// -----------------------------------------------------------------------------
// CMceComAMRWbCodec::GetMaxBitRate
// -----------------------------------------------------------------------------
//
TUint CMceComAMRWbCodec::GetMaxBitRate()
	{
	
    if( iAllowedBitrates & KMceAllowedAmrWbBitrate2385 )
        {
        return KMceAmrWbBitrate2385;
        }
    else if( iAllowedBitrates & KMceAllowedAmrWbBitrate2305 )
        {
        return KMceAmrWbBitrate2305;
        }
    else if( iAllowedBitrates & KMceAllowedAmrWbBitrate1985 )
        {
        return KMceAmrWbBitrate1985;
        }   
    else if( iAllowedBitrates & KMceAllowedAmrWbBitrate1825 )
        {
        return KMceAmrWbBitrate1825;
        }   
    else if( iAllowedBitrates & KMceAllowedAmrWbBitrate1585 )
        {
        return KMceAmrWbBitrate1585;
        }   
    else if( iAllowedBitrates & KMceAllowedAmrWbBitrate1425 )
        {
        return KMceAmrWbBitrate1425;
        }
    else if( iAllowedBitrates & KMceAllowedAmrWbBitrate1265 )
        {
        return KMceAmrWbBitrate1265;
        }  
    else if( iAllowedBitrates & KMceAllowedAmrWbBitrate885 )
        {
        return KMceAmrWbBitrate885;
        }
    else if( iAllowedBitrates & KMceAllowedAmrWbBitrate660 )
        {
        return KMceAmrWbBitrate660;
        }
        
    else
        {
        return 0;    
        }

	}
	

// -----------------------------------------------------------------------------
// CMceComAMRWbCodec::ResolveAllowedBitrates
// -----------------------------------------------------------------------------
//
TInt CMceComAMRWbCodec::ResolveAllowedBitrates( 
    TUint aBitrate, TUint& aAllowedBitrates )
    {
    TInt retVal( KErrNone );
    
    if ( aBitrate <= KMceAmrWbBitrate660 )
        {
        aAllowedBitrates = KMceAllowedAmrWbBitrate660;
        }
    else if ( aBitrate <= KMceAmrWbBitrate885 )
        {
        aAllowedBitrates = KMceAllowedAmrWbBitrate885;
        }
    else if ( aBitrate <= KMceAmrWbBitrate1265 )
        {
        aAllowedBitrates = KMceAllowedAmrWbBitrate1265;
        }
    else if ( aBitrate <= KMceAmrWbBitrate1425 )
        {
        aAllowedBitrates = KMceAllowedAmrWbBitrate1425;
        }
    else if ( aBitrate <= KMceAmrWbBitrate1585 )
        {
        aAllowedBitrates = KMceAllowedAmrWbBitrate1585;
        }
    else if ( aBitrate <= KMceAmrWbBitrate1825 )
        {
        aAllowedBitrates = KMceAllowedAmrWbBitrate1825;
        }
    else if ( aBitrate <= KMceAmrWbBitrate1985 )
        {
        aAllowedBitrates = KMceAllowedAmrWbBitrate1985;
        }
    else if ( aBitrate <= KMceAmrWbBitrate2305 )
        {
        aAllowedBitrates = KMceAllowedAmrWbBitrate2305;
        }
    else if ( aBitrate <= KMceAmrWbBitrate2385 )
        {
        aAllowedBitrates = KMceAllowedAmrWbBitrateAll;
        }
    else
        {
        retVal = KErrNotSupported;
        }
    
    return retVal;
    }



#ifdef MCE_COMMON_SERVER_SIDE

// -----------------------------------------------------------------------------
// CMceComAMRWbCodec::DoMccValidateL
// -----------------------------------------------------------------------------
//
CMceComCodec* CMceComAMRWbCodec::DoMccValidateL( CMccCodecInformation& aMccCodec,
                                               CMceSrvStream& /*aStream*/,
                                               TMceNegotiationRole aRole )
    {
    //if answerer mcc codec represents the received offer and codec the answer
    //if offerer codec represents the sent offer and mcc codec the answer
    
    // Bitrates field contains also additional information, mask it away
	/*
    TUint mccBitrates = ( aMccCodec.AllowedBitrates() & KMceAllowedAmrWbBitrateAll );
    TUint mceBitrates = ( iAllowedBitrates & KMceAllowedAmrWbBitrateAll );
    
    TBool notValid = aRole == EMceRoleAnswerer ?
                  ( mceBitrates && mccBitrates == 0 ||
	                mceBitrates > mccBitrates ) :
                  ( mceBitrates && mccBitrates == 0 ||
	                mceBitrates < mccBitrates );
	                
	*/
//	User::LeaveIfError( notValid ? KErrNotSupported : KErrNone );
	if ( aMccCodec.CodecMode() != (TCodecMode)iCodecMode )
		{
		// codec-mode was different, fail.
		User::Leave(KErrNotSupported);
		}
	return NULL;
    }


// -----------------------------------------------------------------------------
// CMceComAMRWbCodec::DoSetDefaultFmtpAttributeL
// -----------------------------------------------------------------------------
//
void CMceComAMRWbCodec::DoSetDefaultFmtpAttributeL()
    {
    SetFmtpAttributeL( KDefaultAmrWbFmtp );   
    }
    
#endif


// End of File

