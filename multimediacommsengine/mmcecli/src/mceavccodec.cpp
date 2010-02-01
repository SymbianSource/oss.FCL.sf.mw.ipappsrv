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




#include "mceavccodec.h"
#include "mcecomavccodec.h"
#include "mcevideostream.h"


#define _FLAT_DATA static_cast<CMceComAvcCodec*>( iFlatData )
#define FLAT_DATA( data ) _FLAT_DATA->data

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceAvcCodec::~CMceAvcCodec
// -----------------------------------------------------------------------------
//
EXPORT_C CMceAvcCodec::~CMceAvcCodec()
    {
    }


// -----------------------------------------------------------------------------
// CMceAvcCodec::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceAvcCodec* CMceAvcCodec::NewL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceAvcCodec* self = NewLC( aSdpName );
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CMceAvcCodec::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceAvcCodec* CMceAvcCodec::NewLC( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceAvcCodec* self = new (ELeave) CMceAvcCodec();
    CleanupStack::PushL( self );
    self->ConstructL( aSdpName );
    return self;
    }


// -----------------------------------------------------------------------------
// CMceAvcCodec::SetBitrate
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceAvcCodec::SetBitrate( TUint aBitrate )
    {
    if ( aBitrate != Bitrate() )
        {
        TInt ret = _FLAT_DATA->SetBitrate( aBitrate );
        if ( ret == KErrNone )
            {
            CMceVideoStream* stream = static_cast< CMceVideoStream* >( iStream );
            if ( stream && stream->BoundStream() )
                {
                // Find the same codec in bound stream, and set bitrate also to that.
                CMceCodec* codec = stream->Bound()->FindCodec( *this );
                if ( codec )
                    {
                    ret = codec->SetBitrate( aBitrate );
                    }
                }                
            }
        return ret;
        }
    else
        {
        // Avoid looping, go here if already set
        return KErrNone;
        }
    }


// -----------------------------------------------------------------------------
// CMceAvcCodec::SetAllowedBitrates
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceAvcCodec::SetAllowedBitrates(TUint aBitrates)
    {
    if ( aBitrates != AllowedBitrates() )
        {
        TInt ret = _FLAT_DATA->SetAllowedBitrates( aBitrates );
        if ( ret == KErrNone )
            {
            CMceVideoStream* stream = static_cast< CMceVideoStream* >( iStream );
            if ( stream && stream->BoundStream() )
                {
                // Find the same codec in bound stream, and set bitrates also to that.
                CMceCodec* codec = stream->Bound()->FindCodec( *this );
                if ( codec )
                    {
                    ret = codec->SetAllowedBitrates( aBitrates );
                    }
                }
            }
        return ret;
        }
    else
        {
        // Avoid looping, go here if already set
        return KErrNone;
        }
    }


// -----------------------------------------------------------------------------
// CMceAvcCodec::SetCodecMode
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceAvcCodec::SetCodecMode( TUint aCodecMode )
    {
    TInt ret( KErrNone );
    if ( aCodecMode != KMceAvcModeSingleNal &&
         aCodecMode != KMceAvcModeNonInterleaved )
        {
        ret = KErrNotSupported;
        }
    else if ( aCodecMode != CodecMode() )
        {
        FLAT_DATA( iCodecMode ) = aCodecMode;
        CMceVideoStream* stream = static_cast< CMceVideoStream* >( iStream );
        if ( stream && stream->BoundStream() )
            {
            // Find the same codec in bound stream, and set mode also to that.
            CMceCodec* codec = stream->Bound()->FindCodec( *this );
            if ( codec )
                {
                ret = codec->SetCodecMode( aCodecMode );
                }
            }
        }
    else
        {
        // Avoid looping, go here if already set
        }
        
    return ret;
    }


// -----------------------------------------------------------------------------
// CMceAvcCodec::SetPayloadType
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceAvcCodec::SetPayloadType(TUint8 aPayloadType)
    {
    return _FLAT_DATA->SetPayloadType( aPayloadType );
    }        

    
// -----------------------------------------------------------------------------
// CMceAvcCodec::CloneL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceVideoCodec* CMceAvcCodec::CloneL() const
    {
    CMceAvcCodec* clone = CMceAvcCodec::NewLC( FLAT_DATA( iSdpName ) );
    
    CMceComAvcCodec* cloneFlatData = 
            static_cast<CMceComAvcCodec*>(clone->iFlatData);
            
	cloneFlatData->iID = FLAT_DATA( iID );
    cloneFlatData->SetFmtpAttributeL( *(FLAT_DATA( iFmtpAttr )) );
    cloneFlatData->iPayloadType = FLAT_DATA( iPayloadType );
    
	cloneFlatData->iBitrate = FLAT_DATA( iBitrate );
	cloneFlatData->iAllowedBitrates = FLAT_DATA( iAllowedBitrates );
    cloneFlatData->iCodecMode = FLAT_DATA( iCodecMode );
	cloneFlatData->iFourCC = FLAT_DATA( iFourCC );
	cloneFlatData->iFrameSize = FLAT_DATA( iFrameSize );
	
	cloneFlatData->iFrameRate = FLAT_DATA( iFrameRate );
	cloneFlatData->iMaxBitRate = FLAT_DATA( iMaxBitRate );	
	cloneFlatData->iAllowedResolutions = FLAT_DATA( iAllowedResolutions );	
	cloneFlatData->iResolutionWidth = FLAT_DATA( iResolutionWidth );	
	cloneFlatData->iResolutionHeight = FLAT_DATA( iResolutionHeight );	
	cloneFlatData->iAllowedFrameRates = FLAT_DATA( iAllowedFrameRates );	
	cloneFlatData->iClockRate = FLAT_DATA( iClockRate );
	
	cloneFlatData->iPacketizationMode = FLAT_DATA( iPacketizationMode );
	cloneFlatData->iAllowedPacketizationModes = 
	    FLAT_DATA( iAllowedPacketizationModes );
	
	CleanupStack::Pop( clone );
    return clone;
    }


// -----------------------------------------------------------------------------
// CMceAvcCodec::
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceAvcCodec::SetMaxBitrateL( TUint aMaxBitrate )
    {
    User::LeaveIfError( _FLAT_DATA->SetMaxBitrate( aMaxBitrate ) );
    }


// -----------------------------------------------------------------------------
// CMceAvcCodec::
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceAvcCodec::SetAllowedResolutionsL( TUint aResolutions )
    {
    FLAT_DATA( iAllowedResolutions ) = aResolutions;
    }


// -----------------------------------------------------------------------------
// CMceAvcCodec::
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceAvcCodec::SetResolutionL( TSize aResolution )
    {
    FLAT_DATA( iResolutionWidth ) = aResolution.iWidth;
    FLAT_DATA( iResolutionHeight ) = aResolution.iHeight;
    }


// -----------------------------------------------------------------------------
// CMceAvcCodec::
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceAvcCodec::SetAllowedFrameRatesL( TUint aFrameRates )
    {
    FLAT_DATA( iAllowedFrameRates ) = aFrameRates;
    }


// -----------------------------------------------------------------------------
// CMceAvcCodec::
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceAvcCodec::SetFrameRateL( TReal aFrameRate )
    {
    FLAT_DATA( iFrameRate ) = aFrameRate;
    }


// -----------------------------------------------------------------------------
// CMceAvcCodec::SetPreferredEncodingDecodingDeviceL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceAvcCodec::SetPreferredEncodingDecodingDeviceL( 
    TUid aEncodingDecodingDevice )
    {
    FLAT_DATA( iEncodingDecodingDevice ) = aEncodingDecodingDevice;
    }

// -----------------------------------------------------------------------------
// CMceAvcCodec::SetConfigKeyL
// -----------------------------------------------------------------------------
//        
EXPORT_C void CMceAvcCodec::SetConfigKeyL( const TDesC8& aConfigKey )
    {
    HBufC8* configKey = aConfigKey.AllocLC();
    _FLAT_DATA->SetConfigKeyL( configKey );
    CleanupStack::PopAndDestroy( configKey );
    
    }
    

// -----------------------------------------------------------------------------
// CMceAvcCodec::SetSdpNameL
// -----------------------------------------------------------------------------
//
void CMceAvcCodec::SetSdpNameL( const TDesC8& aSdpName )
    {
    __ASSERT_ALWAYS( aSdpName.Length() <= KMceMaxSdpNameLength, 
                     User::Leave( KErrArgument ) );
    FLAT_DATA( iSdpName ).Copy( aSdpName );
    }
        
// -----------------------------------------------------------------------------
// CMceAvcCodec::CMceAvcCodec
// -----------------------------------------------------------------------------
//
CMceAvcCodec::CMceAvcCodec()
    :CMceVideoCodec()
    {
    }


// -----------------------------------------------------------------------------
// CMceAvcCodec::ConstructL
// -----------------------------------------------------------------------------
//
void CMceAvcCodec::ConstructL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComVideoCodec* codec = CMceComAvcCodec::NewLC( aSdpName );
    CMceVideoCodec::ConstructL( codec );

    CleanupStack::Pop( codec );
    }



    
