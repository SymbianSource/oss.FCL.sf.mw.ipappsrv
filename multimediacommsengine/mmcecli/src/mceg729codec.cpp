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
#include "mcecomaudiocodec.h"
#include "mceaudiostream.h"


#define _FLAT_DATA static_cast<CMceComAudioCodec*>( iFlatData )
#define FLAT_DATA( data ) _FLAT_DATA->data

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceG729Codec::~CMceG729Codec
// -----------------------------------------------------------------------------
//
EXPORT_C CMceG729Codec::~CMceG729Codec()
    {
    }
    
// -----------------------------------------------------------------------------
// CMceG729Codec::CloneL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceAudioCodec* CMceG729Codec::CloneL() const
    {
    CMceG729Codec* clone = CMceG729Codec::NewLC( FLAT_DATA( iSdpName ) );
    
    CMceComAudioCodec* cloneFlatData = 
        static_cast<CMceComAudioCodec*>( clone->iFlatData );
    
    cloneFlatData->iID = FLAT_DATA( iID );
    cloneFlatData->SetFmtpAttributeL( *(FLAT_DATA( iFmtpAttr )) );
    cloneFlatData->iEnableVAD = FLAT_DATA( iEnableVAD );
	cloneFlatData->iSamplingFreq = FLAT_DATA( iSamplingFreq );
	cloneFlatData->iPTime = FLAT_DATA( iPTime );
	cloneFlatData->iMaxPTime = FLAT_DATA( iMaxPTime );
	cloneFlatData->iBitrate = FLAT_DATA( iBitrate );
	cloneFlatData->iAllowedBitrates = FLAT_DATA( iAllowedBitrates );
	cloneFlatData->iPayloadType = FLAT_DATA( iPayloadType );
	cloneFlatData->iCodecMode = FLAT_DATA( iCodecMode );
	cloneFlatData->iFourCC = FLAT_DATA( iFourCC );
	cloneFlatData->iFrameSize = FLAT_DATA( iFrameSize );
	
	CleanupStack::Pop( clone );
	
	return clone;
    }
    
    
// -----------------------------------------------------------------------------
// CMceG729Codec::SetBitrate
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceG729Codec::SetBitrate(TUint aBitrate)
    {
    if ( aBitrate != Bitrate() )
        {
        TInt ret = _FLAT_DATA->SetBitrate( aBitrate );
        if ( KErrNone == ret )
            {
            CMceAudioStream* stream = static_cast< CMceAudioStream* >( iStream );
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
// CMceG729Codec::SetAllowedBitrates
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceG729Codec::SetAllowedBitrates(TUint aBitrates)
    {
    if ( aBitrates != AllowedBitrates() )
        {
        TInt ret = _FLAT_DATA->SetAllowedBitrates( aBitrates );
        if ( ret == KErrNone )
            {
            CMceAudioStream* stream = static_cast< CMceAudioStream* >( iStream );
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
// CMceG729Codec::SetSamplingFreq
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceG729Codec::SetSamplingFreq(TUint aSamplingFreq)
    {
    return _FLAT_DATA->SetSamplingFreq( aSamplingFreq );
    }


// -----------------------------------------------------------------------------
// CMceG729Codec::SetPTime
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceG729Codec::SetPTime(TUint aPTime)
    {
    return _FLAT_DATA->SetPTime( aPTime );
    }

// -----------------------------------------------------------------------------
// CMceG729Codec::SetMaxPTime
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceG729Codec::SetMaxPTime(TUint aMaxPTime)
    {
    return _FLAT_DATA->SetMaxPTime( aMaxPTime );
    }

// -----------------------------------------------------------------------------
// CMceG729Codec::SetPayloadType
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceG729Codec::SetPayloadType(TUint8 aPayloadType)
    {
    return _FLAT_DATA->SetPayloadType( aPayloadType );
    }
    
// -----------------------------------------------------------------------------
// CMceG729Codec::SetCodecMode
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceG729Codec::SetCodecMode(TUint aCodecMode)
    {
    if ( aCodecMode != CodecMode() )
        {
        TInt ret = _FLAT_DATA->SetCodecMode( aCodecMode );
        if ( KErrNone == ret )
            {
            CMceAudioStream* stream = static_cast< CMceAudioStream* >( iStream );
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
        
        return ret;
        }
    else
        {
        // Avoid looping, go here if already set
        return KErrNone;
        }
    }


// -----------------------------------------------------------------------------
// CMceG729Codec::EnableVAD
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceG729Codec::EnableVAD( TBool aEnableVAD )
    {
    FLAT_DATA( iEnableVAD ) = aEnableVAD;
    return KErrNone;
    }


   
// -----------------------------------------------------------------------------
// CMceG729Codec::NewL
// -----------------------------------------------------------------------------
//
CMceG729Codec* CMceG729Codec::NewL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceG729Codec* self = NewLC( aSdpName );
    CleanupStack::Pop( self );
    return self;
    
    }

// -----------------------------------------------------------------------------
// CMceG729Codec::NewL
// -----------------------------------------------------------------------------
//
CMceG729Codec* CMceG729Codec::NewLC( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceG729Codec* self = new (ELeave) CMceG729Codec();
    CleanupStack::PushL( self );
    self->ConstructL( aSdpName );
    return self;
    
    }

// -----------------------------------------------------------------------------
// CMceG729Codec::CMceG729Codec
// -----------------------------------------------------------------------------
//
CMceG729Codec::CMceG729Codec()
 : CMceAudioCodec()
    {
    }


// -----------------------------------------------------------------------------
// CMceG729Codec::ConstructL
// -----------------------------------------------------------------------------
//
void CMceG729Codec::ConstructL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComAudioCodec* codec = CMceComG729Codec::NewLC( aSdpName );
    CMceAudioCodec::ConstructL( codec );
	
    CleanupStack::Pop( codec );
    }


// -----------------------------------------------------------------------------
// CMceG729Codec::SetSdpNameL
// -----------------------------------------------------------------------------
//
void CMceG729Codec::SetSdpNameL( const TDesC8& aSdpName )
    {
    __ASSERT_ALWAYS( aSdpName.Length() <= KMceMaxSdpNameLength, 
                     User::Leave( KErrArgument ) );
    FLAT_DATA( iSdpName ).Copy( aSdpName );
    }
    
