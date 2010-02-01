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




#include "mceh263codec.h"
#include "mcecomh263codec.h"
#include "mcevideostream.h"


#define _FLAT_DATA static_cast<CMceComVideoCodec*>( iFlatData )
#define FLAT_DATA( data ) _FLAT_DATA->data

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceH263Codec::~CMceH263Codec
// -----------------------------------------------------------------------------
//
EXPORT_C CMceH263Codec::~CMceH263Codec()
    {
    }


// -----------------------------------------------------------------------------
// CMceH263Codec::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceH263Codec* CMceH263Codec::NewL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceH263Codec* self = NewLC( aSdpName );
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CMceH263Codec::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceH263Codec* CMceH263Codec::NewLC( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceH263Codec* self = new (ELeave) CMceH263Codec();
    CleanupStack::PushL( self );
    self->ConstructL( aSdpName );
    return self;
    }


// -----------------------------------------------------------------------------
// CMceH263Codec::SetBitrate
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceH263Codec::SetBitrate( TUint aBitrate )
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
// CMceH263Codec::SetAllowedBitrates
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceH263Codec::SetAllowedBitrates(TUint aBitrates)
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
// CMceH263Codec::SetCodecMode
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceH263Codec::SetCodecMode(TUint aCodecMode)
    {
    if ( aCodecMode != CodecMode() )
        {
        FLAT_DATA( iCodecMode ) = aCodecMode;
        TInt ret = KErrNone;
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
        return ret;
        }
    else
        {
        // Avoid looping, go here if already set
        return KErrNone;
        }}


// -----------------------------------------------------------------------------
// CMceH263Codec::SetPayloadType
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceH263Codec::SetPayloadType(TUint8 aPayloadType)
    {
    return _FLAT_DATA->SetPayloadType( aPayloadType );
    }        

    
// -----------------------------------------------------------------------------
// CMceH263Codec::CloneL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceVideoCodec* CMceH263Codec::CloneL() const
    {
    CMceH263Codec* clone = CMceH263Codec::NewLC( FLAT_DATA( iSdpName ) );
    
    CMceComVideoCodec* cloneFlatData = 
            static_cast<CMceComVideoCodec*>(clone->iFlatData);
            
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
	
	CleanupStack::Pop( clone );
    return clone;
    }


// -----------------------------------------------------------------------------
// CMceH263Codec::
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceH263Codec::SetMaxBitrateL( TUint aMaxBitrate )
    {
    User::LeaveIfError( _FLAT_DATA->SetMaxBitrate( aMaxBitrate ) );
    }


// -----------------------------------------------------------------------------
// CMceH263Codec::
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceH263Codec::SetAllowedResolutionsL( TUint aResolutions )
    {
    FLAT_DATA( iAllowedResolutions ) = aResolutions;
    }


// -----------------------------------------------------------------------------
// CMceH263Codec::
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceH263Codec::SetResolutionL( TSize aResolution )
    {
    FLAT_DATA( iResolutionWidth ) = aResolution.iWidth;
    FLAT_DATA( iResolutionHeight ) = aResolution.iHeight;
    }


// -----------------------------------------------------------------------------
// CMceH263Codec::
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceH263Codec::SetAllowedFrameRatesL( TUint aFrameRates )
    {
    FLAT_DATA( iAllowedFrameRates ) = aFrameRates;
    }


// -----------------------------------------------------------------------------
// CMceH263Codec::
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceH263Codec::SetFrameRateL( TReal aFrameRate )
    {
    FLAT_DATA( iFrameRate ) = aFrameRate;
    }

// -----------------------------------------------------------------------------
// CMceH263Codec::SetPreferredEncodingDecodingDeviceL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceH263Codec::SetPreferredEncodingDecodingDeviceL( 
    TUid /*aEncodingDecodingDevice*/ )
    {
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CMceH263Codec::SetConfigKeyL
// -----------------------------------------------------------------------------
//        
EXPORT_C void CMceH263Codec::SetConfigKeyL( const TDesC8& /*aConfigKey*/ )
    {
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CMceH263Codec::SetSdpNameL
// -----------------------------------------------------------------------------
//
void CMceH263Codec::SetSdpNameL( const TDesC8& aSdpName )
    {
    __ASSERT_ALWAYS( aSdpName.Length() <= KMceMaxSdpNameLength, 
                     User::Leave( KErrArgument ) );
    FLAT_DATA( iSdpName ).Copy( aSdpName );
    }


// -----------------------------------------------------------------------------
// CMceH263Codec::CMceH263Codec
// -----------------------------------------------------------------------------
//
CMceH263Codec::CMceH263Codec()
    :CMceVideoCodec()
    {
    }


// -----------------------------------------------------------------------------
// CMceH263Codec::ConstructL
// -----------------------------------------------------------------------------
//
void CMceH263Codec::ConstructL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComVideoCodec* codec = CMceComH263Codec::NewLC( aSdpName );
    CMceVideoCodec::ConstructL( codec );

    CleanupStack::Pop( codec );
    }



    
