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




#include "mceg711codec.h"
#include "mcecomg711codec.h"

#ifdef MCE_COMMON_SERVER_SIDE

#include <mmcccodecinformation.h>

#endif //MCE_COMMON_SERVER_SIDE

//G.711 Default Frame Size    
const TUint KMceG711FrameSize = 10; 

// ============================ MEMBER FUNCTIONS ===============================



// -----------------------------------------------------------------------------
// CMceComG711Codec::NewL
// -----------------------------------------------------------------------------
//
CMceComG711Codec* CMceComG711Codec::NewL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComG711Codec* self = NewLC( aSdpName );
    CleanupStack::Pop( self );
    return self;
    
    }

// -----------------------------------------------------------------------------
// CMceComG711Codec::NewLC
// -----------------------------------------------------------------------------
//
CMceComG711Codec* CMceComG711Codec::NewLC( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComG711Codec* self = new (ELeave) CMceComG711Codec();
    CleanupStack::PushL( self );
    self->ConstructL( aSdpName );
    return self;
    }


// -----------------------------------------------------------------------------
// CMceComG711Codec::~CMceComG711Codec
// -----------------------------------------------------------------------------
//
CMceComG711Codec::~CMceComG711Codec()
    {
    }
    

// -----------------------------------------------------------------------------
// CMceComG711Codec::CMceComG711Codec
// -----------------------------------------------------------------------------
//
CMceComG711Codec::CMceComG711Codec()
 : CMceComAudioCodec()
    {
    iFourCC = 0;
    iEnableVAD = EFalse;
	iSamplingFreq  = KMceG711SamplingFreq;
	iBitrate  = KMceG711Bitrate64;
	iPTime = KMceG711DefaultPtime;
	iMaxPTime = KMceG711DefaultMaxPtime;
	iAllowedBitrates = KMceAllowedG711Bitrate64K;
	iPayloadType = KMcePCMAPayloadType;
	iCodecMode = KMceG711PCMA;    
    iFrameSize = KMceG711FrameSize;
    }


// -----------------------------------------------------------------------------
// CMceComG711Codec::CloneL
// -----------------------------------------------------------------------------
//
CMceComAudioCodec* CMceComG711Codec::CloneL()
    {
    CMceComG711Codec* copy = new (ELeave) CMceComG711Codec();
    CleanupStack::PushL( copy );
    copy->ConstructL( *this );
    CleanupStack::Pop( copy );
    return copy;
    
    }


// -----------------------------------------------------------------------------
// CMceComG711Codec::ConstructL
// -----------------------------------------------------------------------------
//
void CMceComG711Codec::ConstructL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    
    CMceComAudioCodec::ConstructL( aSdpName );
    
    if( !iSdpName.CompareF(KMceSDPNamePCMU) )
        {
        iCodecMode = KMceG711PCMU; 
        iPayloadType = KMcePCMUPayloadType;
        }
    else
        {
        iCodecMode = KMceG711PCMA;
        iPayloadType = KMcePCMAPayloadType;
        }
    
    }

// -----------------------------------------------------------------------------
// CMceComG711Codec::ConstructL
// -----------------------------------------------------------------------------
//
void CMceComG711Codec::ConstructL( CMceComG711Codec& aCodec )
    {
    CMceComAudioCodec::ConstructL( aCodec );
    }



// -----------------------------------------------------------------------------
// CMceComG711Codec::SetBitrate
// -----------------------------------------------------------------------------
//
TInt CMceComG711Codec::SetBitrate( TUint aBitrate )
    {
	
    TBool isValid = ETrue;
	
    switch ( aBitrate )
    	{
    	case KMceG711Bitrate64:
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
// CMceComG711Codec::SetAllowedBitrates
// -----------------------------------------------------------------------------
//
TInt CMceComG711Codec::SetAllowedBitrates( TUint aBitrates )
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
        CMceComCodec::SetAllowedBitrates( KMceAllowedG711BitrateAllowAll );
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
// CMceComG711Codec::SetCodecMode
// -----------------------------------------------------------------------------
//
TInt CMceComG711Codec::SetCodecMode( TUint aCodecMode )
    {
    if ( KMceG711PCMU == aCodecMode || KMceG711PCMA == aCodecMode )
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
// CMceComG711Codec::GetMaxBitRate
// -----------------------------------------------------------------------------
//
TUint CMceComG711Codec::GetMaxBitRate()
	{
    if( iAllowedBitrates & KMceAllowedG711Bitrate64K )
        {
        return KMceG711Bitrate64;
        }
    else
        {
        return 0;    
        }
    }   
    
// -----------------------------------------------------------------------------
// CMceComG711Codec::SetPTime
// -----------------------------------------------------------------------------
//
TInt CMceComG711Codec::SetPTime( TUint aPTime )
    {
    if ( ( aPTime % KMceG711FrameSize ) == 0 )
        {
        return CMceComAudioCodec::SetPTime( aPTime );
        }
    else
        {
        return KErrNotSupported;
        }
    }


// -----------------------------------------------------------------------------
// CMceComG711Codec::SetMaxPTime
// -----------------------------------------------------------------------------
//
TInt CMceComG711Codec::SetMaxPTime( TUint aMaxPTime )
    {
    if ( ( aMaxPTime % KMceG711FrameSize ) == 0 )
        {
        return CMceComAudioCodec::SetMaxPTime( aMaxPTime );
        }
    else
        {
        return KErrNotSupported;
        }
    }

// End of File
