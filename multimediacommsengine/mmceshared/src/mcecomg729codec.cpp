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




#include "mceg729codec.h"
#include "mcecomg729codec.h"

#ifdef MCE_COMMON_SERVER_SIDE

#include <mmcccodecinformation.h>

#endif//MCE_COMMON_SERVER_SIDE

// Frame Size
const TUint KMceG72FrameSize = 10;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceComG729Codec::NewL
// -----------------------------------------------------------------------------
//
CMceComG729Codec* CMceComG729Codec::NewL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComG729Codec* self = NewLC( aSdpName );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMceComG729Codec::NewLC
// -----------------------------------------------------------------------------
//
CMceComG729Codec* CMceComG729Codec::NewLC( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComG729Codec* self = new (ELeave) CMceComG729Codec();
    CleanupStack::PushL( self );
    self->ConstructL( aSdpName );
    return self;
    }


// -----------------------------------------------------------------------------
// CMceComG729Codec::~CMceComG729Codec
// -----------------------------------------------------------------------------
//
CMceComG729Codec::~CMceComG729Codec()
    {
    }
    

// -----------------------------------------------------------------------------
// CMceComG729Codec::CMceComG729Codec
// -----------------------------------------------------------------------------
//
CMceComG729Codec::CMceComG729Codec()
 : CMceComAudioCodec()
    {
    iFourCC = 0;
    iEnableVAD = EFalse;
    iSamplingFreq = KMceG729SamplingFreq;
    iPTime = KMceG729DefaultPtime;          
    iMaxPTime = KMceG729DefaultMaxPtime;    
    iBitrate  = KMceG729Bitrate;   
    iAllowedBitrates = KMceAllowedG729Bitrate8K;
    iPayloadType = KMceDefaultG729PayloadType; 
    iCodecMode = KG729CodecMode; 
    iFrameSize = KMceG72FrameSize; 
    }


// -----------------------------------------------------------------------------
// CMceComG729Codec::CloneL
// -----------------------------------------------------------------------------
//
CMceComAudioCodec* CMceComG729Codec::CloneL()
    {
    CMceComG729Codec* copy = new (ELeave) CMceComG729Codec();
    CleanupStack::PushL( copy );
    
    copy->ConstructL( *this );
    CleanupStack::Pop( copy );
    return copy;
    }


// -----------------------------------------------------------------------------
// CMceComG729Codec::ConstructL
// -----------------------------------------------------------------------------
//
void CMceComG729Codec::ConstructL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComAudioCodec::ConstructL( aSdpName );
    }


// -----------------------------------------------------------------------------
// CMceComG729Codec::ConstructL
// -----------------------------------------------------------------------------
//
void CMceComG729Codec::ConstructL( CMceComG729Codec& aCodec )
    {
    CMceComAudioCodec::ConstructL( aCodec );
    }


// -----------------------------------------------------------------------------
// CMceComG729Codec::SetBitrate
// -----------------------------------------------------------------------------
//
TInt CMceComG729Codec::SetBitrate( TUint aBitrate )
    {
	TBool isValid = ETrue;
	
    switch ( aBitrate )
    	{
    	case KMceG729Bitrate:
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
// CMceComG729Codec::SetAllowedBitrates
// -----------------------------------------------------------------------------
//
TInt CMceComG729Codec::SetAllowedBitrates( TUint aBitrates )
    {
    TInt error = KErrNone;
    
    //Confirm that the bitrate mask is valid
    //I.e. after all the valid bitrates are set to zero the value should be zero
    if ( !aBitrates || 
        ( aBitrates >> 8 ) > 0 ) 
        {
        error = KErrArgument;
        }
    else
        {
        CMceComCodec::SetAllowedBitrates( KMceAllowedG729BitrateAllowAll );
        iAllowedBitrates &= aBitrates;
        TUint maxBitrate = GetMaxBitRate();
        if ( iBitrate > maxBitrate )
            {
            error = CMceComCodec::SetBitrate( maxBitrate );
            }
        }
    
    return error;
    
    }


// -----------------------------------------------------------------------------
// CMceComG729Codec::SetCodecMode
// -----------------------------------------------------------------------------
//
TInt CMceComG729Codec::SetCodecMode( TUint aCodecMode )
    {
    if ( KG729CodecMode == aCodecMode )
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
// CMceComG729Codec::GetMaxBitRate
// -----------------------------------------------------------------------------
//
TUint CMceComG729Codec::GetMaxBitRate()
	{
    if( iAllowedBitrates & KMceAllowedG729Bitrate8K )
        {
        return KMceG729Bitrate;
        }
    else
        {
        return 0;    
        }
    }   

// -----------------------------------------------------------------------------
// CMceComG729Codec::SetPTime
// -----------------------------------------------------------------------------
//
TInt CMceComG729Codec::SetPTime( TUint aPTime )
    {
    if ( ( aPTime % KMceG72FrameSize ) == 0 )
        {
        return CMceComAudioCodec::SetPTime( aPTime );
        }
    else
        {
        return KErrNotSupported;
        }
    }


// -----------------------------------------------------------------------------
// CMceComG729Codec::SetMaxPTime
// -----------------------------------------------------------------------------
//
TInt CMceComG729Codec::SetMaxPTime( TUint aMaxPTime )
    {
    if ( ( aMaxPTime % KMceG72FrameSize ) == 0 )
        {
        return CMceComAudioCodec::SetMaxPTime( aMaxPTime );
        }
    else
        {
        return KErrNotSupported;
        }
    }

// End of File
