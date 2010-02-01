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




#include "mcecomh263codec.h"


const TReal KMceH263DefaultLoweredFrameRate = 12;

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CMceComH263Codec::NewL
// -----------------------------------------------------------------------------
//
CMceComH263Codec* CMceComH263Codec::NewL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComH263Codec* self = NewLC( aSdpName );
    CleanupStack::Pop( self );
    return self;
    
    }

// -----------------------------------------------------------------------------
// CMceComH263Codec::NewLC
// -----------------------------------------------------------------------------
//
CMceComH263Codec* CMceComH263Codec::NewLC( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComH263Codec* self = new (ELeave) CMceComH263Codec();
    CleanupStack::PushL( self );
    self->ConstructL( aSdpName );
    return self;
    }


// -----------------------------------------------------------------------------
// CMceComH263Codec::~CMceComH263Codec
// -----------------------------------------------------------------------------
//
CMceComH263Codec::~CMceComH263Codec()
    {
    }
    

// -----------------------------------------------------------------------------
// CMceComH263Codec::CMceComH263Codec
// -----------------------------------------------------------------------------
//
CMceComH263Codec::CMceComH263Codec()
    :CMceComVideoCodec()
    {
    iClockRate = KMceH263ClockRate;
    iFrameRate = KMceH263DefaultLoweredFrameRate;
    iResolutionWidth = KMceH263FrameWidth;
    iResolutionHeight = KMceH263FrameHeight;
    iBitrate = KMceH263DefaultBitrate;
	iMaxBitRate = KMceH263DefaultMaxBitrate;
	iAllowedBitrates = KMceH263DefaultAllowedBitrate;
    iFrameSize = KMceH263DefaultFrameSize;
    // Defaults should be defined in MCE API
    const TUint8 KMceDefaultH263PayloadType = 97;
	iPayloadType = KMceDefaultH263PayloadType;
    
    iReceiveFrameRate = KMceH263FrameRate;
    }


// -----------------------------------------------------------------------------
// CMceComH263Codec::ConstructL
// -----------------------------------------------------------------------------
//
CMceComVideoCodec* CMceComH263Codec::CloneL()
    {
    CMceComH263Codec* copy = new (ELeave) CMceComH263Codec();
    CleanupStack::PushL( copy );
    copy->ConstructL( *this );
    CleanupStack::Pop( copy );
    return copy;
    
    }


// -----------------------------------------------------------------------------
// CMceComH263Codec::ConstructL
// -----------------------------------------------------------------------------
//
void CMceComH263Codec::ConstructL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComVideoCodec::ConstructL( aSdpName );
    
    // NOTE: 1998 mode codec is disabled by default in order to prevent
    // default offer to contain both 2000 and 1998 mode versions
    if ( iSdpName.CompareF( KMceSDPNameH2631998 ) == 0 )
        {
        iState = CMceCodec::EDisabled; 
        }
    }


// -----------------------------------------------------------------------------
// CMceComH263Codec::ConstructL
// -----------------------------------------------------------------------------
//
void CMceComH263Codec::ConstructL( CMceComH263Codec& aCodec )
    {
    CMceComVideoCodec::ConstructL( aCodec );
    }


// -----------------------------------------------------------------------------
// CMceComH263Codec::SetBitrate
// -----------------------------------------------------------------------------
//
TInt CMceComH263Codec::SetBitrate( TUint aBitrate )
    {
	return CMceComCodec::SetBitrate( aBitrate );
    }
    
// -----------------------------------------------------------------------------
// CMceComH263Codec::SetAllowedBitrates
// -----------------------------------------------------------------------------
//
TInt CMceComH263Codec::SetAllowedBitrates( TUint aBitrates )
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
        CMceComCodec::SetAllowedBitrates( KMceAllowedH263BitrateAll );
        iAllowedBitrates &= aBitrates;
        CMceComVideoCodec::SetMaxBitrate( GetMaxBitRate() );
        if ( iBitrate > iMaxBitRate )
            {
            error = CMceComCodec::SetBitrate( iMaxBitRate );
            }
        }
    
    return error;
    
    }


// -----------------------------------------------------------------------------
// CMceComH263Codec::GetMaxBitRate
// -----------------------------------------------------------------------------
//
TUint CMceComH263Codec::GetMaxBitRate()
    {
    if( iAllowedBitrates & KMceAllowedH263Bitrate45 )
        {
        return KMceH263Level45Bitrate;
        }
    else if( iAllowedBitrates & KMceAllowedH263Bitrate10 )
        {
        return KMceH263Level10Bitrate;
        }
    else
        {
        return 0;    
        }
    
    }

// -----------------------------------------------------------------------------
// CMceComH263Codec::SetMaxBitrate
// -----------------------------------------------------------------------------
//
TInt CMceComH263Codec::SetMaxBitrate( TUint aMaxBitrate )
    {
    TInt status = KErrNone;
	switch( aMaxBitrate )
	    {
	    case KMceH263Level10Bitrate:
	        {
	        status = SetAllowedBitrates( KMceAllowedH263Bitrate10 );
	        break;
	        }
	    case KMceH263Level45Bitrate:
	        {
	        status = SetAllowedBitrates( KMceAllowedH263Bitrate45 );
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
// CMceComH263Codec::ResolveAllowedBitrates
// -----------------------------------------------------------------------------
//
TInt CMceComH263Codec::ResolveAllowedBitrates( 
    TUint aBitrate, TUint& aAllowedBitrates )
    {
    TInt retVal( KErrNone );
    
    if ( aBitrate <= KMceH263Level10Bitrate )
        {
        aAllowedBitrates = KMceAllowedH263Bitrate10;
        }
    else if ( aBitrate <= KMceH263Level45Bitrate )
        {
        aAllowedBitrates = KMceAllowedH263BitrateAll;
        }
    else 
        {
        retVal = KErrNotSupported;
        }
        
    return retVal;
    }
        
// End of File

    
