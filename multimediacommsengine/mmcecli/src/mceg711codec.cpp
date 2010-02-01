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
#include "mcecomaudiocodec.h"
#include "mceaudiostream.h"


#define _FLAT_DATA static_cast<CMceComAudioCodec*>( iFlatData )
#define FLAT_DATA( data ) _FLAT_DATA->data

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceG711Codec::~CMceG711Codec
// -----------------------------------------------------------------------------
//
EXPORT_C CMceG711Codec::~CMceG711Codec()
    {
    }
    
// -----------------------------------------------------------------------------
// CMceG711Codec::CloneL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceAudioCodec* CMceG711Codec::CloneL() const
    {
    CMceG711Codec* clone = CMceG711Codec::NewLC( FLAT_DATA( iSdpName ) );
    
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
// CMceG711Codec::EnableVAD
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceG711Codec::EnableVAD( TBool aEnableVAD )
    {
    FLAT_DATA( iEnableVAD ) = aEnableVAD;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMceG711Codec::SetBitrate
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceG711Codec::SetBitrate(TUint aBitrate)
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
// CMceG711Codec::SetAllowedBitrates
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceG711Codec::SetAllowedBitrates( TUint aBitrates )
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
// CMceG711Codec::SetSamplingFreq
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceG711Codec::SetSamplingFreq(TUint aSamplingFreq)
    {
    return _FLAT_DATA->SetSamplingFreq( aSamplingFreq );
    }


// -----------------------------------------------------------------------------
// CMceG711Codec::SetPTime
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceG711Codec::SetPTime(TUint aPTime)
    {
    return _FLAT_DATA->SetPTime( aPTime );
    }

// -----------------------------------------------------------------------------
// CMceG711Codec::SetMaxPTime
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceG711Codec::SetMaxPTime(TUint aMaxPTime)
    {
    return _FLAT_DATA->SetMaxPTime( aMaxPTime );
    }

// -----------------------------------------------------------------------------
// CMceG711Codec::SetPayloadType
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceG711Codec::SetPayloadType(TUint8 aPayloadType)
    {
    return _FLAT_DATA->SetPayloadType( aPayloadType );
    }
    
// -----------------------------------------------------------------------------
// CMceG711Codec::SetCodecMode
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceG711Codec::SetCodecMode(TUint aCodecMode)
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
// CMceG711Codec::NewL
// -----------------------------------------------------------------------------
//
CMceG711Codec* CMceG711Codec::NewL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceG711Codec* self = NewLC( aSdpName );
    CleanupStack::Pop( self );
    return self;
    
    }

// -----------------------------------------------------------------------------
// CMceG711Codec::NewL
// -----------------------------------------------------------------------------
//
CMceG711Codec* CMceG711Codec::NewLC( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceG711Codec* self = new (ELeave) CMceG711Codec();
    CleanupStack::PushL( self );
    self->ConstructL( aSdpName );
    return self;
    
    }

// -----------------------------------------------------------------------------
// CMceG711Codec::CMceG711Codec
// -----------------------------------------------------------------------------
//
CMceG711Codec::CMceG711Codec()
 : CMceAudioCodec()
    {
    }


// -----------------------------------------------------------------------------
// CMceG711Codec::ConstructL
// -----------------------------------------------------------------------------
//
void CMceG711Codec::ConstructL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComAudioCodec* codec = CMceComG711Codec::NewLC( aSdpName );
    CMceAudioCodec::ConstructL( codec );
	
    CleanupStack::Pop( codec );
    }


// -----------------------------------------------------------------------------
// CMceG711Codec::SetSdpNameL
// -----------------------------------------------------------------------------
//
void CMceG711Codec::SetSdpNameL( const TDesC8& aSdpName )
    {
    __ASSERT_ALWAYS( aSdpName.Length() <= KMceMaxSdpNameLength, 
                     User::Leave( KErrArgument ) );
    FLAT_DATA( iSdpName ).Copy( aSdpName );
    }
    
