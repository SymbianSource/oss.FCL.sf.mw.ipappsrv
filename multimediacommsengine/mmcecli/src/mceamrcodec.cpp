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
#include "mcecomamrcodec.h"
#include "mceaudiostream.h"


#define _FLAT_DATA static_cast<CMceComAudioCodec*>( iFlatData )
#define FLAT_DATA( data ) _FLAT_DATA->data

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceAmrCodec::~CMceAmrCodec
// -----------------------------------------------------------------------------
//
EXPORT_C CMceAmrCodec::~CMceAmrCodec()
    {
    }
    
// -----------------------------------------------------------------------------
// CMceAmrCodec::CloneL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceAudioCodec* CMceAmrCodec::CloneL() const
    {
    CMceAmrCodec* clone = CMceAmrCodec::NewLC( FLAT_DATA( iSdpName ) );
    
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
// CMceAmrCodec::EnableVAD
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceAmrCodec::EnableVAD( TBool aEnableVAD )
    {
    FLAT_DATA( iEnableVAD ) = aEnableVAD;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMceAmrCodec::SetBitrate
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceAmrCodec::SetBitrate(TUint aBitrate)
    {
    if ( aBitrate != Bitrate() )
        {
        TInt ret = _FLAT_DATA->SetBitrate( aBitrate );
        if ( ret == KErrNone )
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
// CMceAmrCodec::SetAllowedBitrates
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceAmrCodec::SetAllowedBitrates(TUint aBitrates)
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
// CMceAmrCodec::SetSamplingFreq
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceAmrCodec::SetSamplingFreq(TUint aSamplingFreq)
    {
    return _FLAT_DATA->SetSamplingFreq( aSamplingFreq );
    }

// -----------------------------------------------------------------------------
// CMceAmrCodec::SetPTime
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceAmrCodec::SetPTime(TUint aPTime)
    {
    return _FLAT_DATA->SetPTime( aPTime );
    }

// -----------------------------------------------------------------------------
// CMceAmrCodec::SetMaxPTime
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceAmrCodec::SetMaxPTime(TUint aMaxPTime)
    {
    return _FLAT_DATA->SetMaxPTime( aMaxPTime );
    }

// -----------------------------------------------------------------------------
// CMceAmrCodec::SetPayloadType
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceAmrCodec::SetPayloadType(TUint8 aPayloadType)
    {
    return _FLAT_DATA->SetPayloadType( aPayloadType );
    }

// -----------------------------------------------------------------------------
// CMceAmrCodec::SetCodecMode
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceAmrCodec::SetCodecMode( TUint aCodecMode )
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
// CMceAmrCodec::NewL
// -----------------------------------------------------------------------------
//
CMceAmrCodec* CMceAmrCodec::NewL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceAmrCodec* self = NewLC( aSdpName );
    CleanupStack::Pop( self );
    return self;
    
    }

// -----------------------------------------------------------------------------
// CMceAmrCodec::NewL
// -----------------------------------------------------------------------------
//
CMceAmrCodec* CMceAmrCodec::NewLC( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceAmrCodec* self = new (ELeave) CMceAmrCodec();
    CleanupStack::PushL( self );
    self->ConstructL( aSdpName );
    return self;
    
    }

// -----------------------------------------------------------------------------
// CMceAmrCodec::CMceAmrCodec
// -----------------------------------------------------------------------------
//
CMceAmrCodec::CMceAmrCodec()
 : CMceAudioCodec()
    {
    }


// -----------------------------------------------------------------------------
// CMceAmrCodec::ConstructL
// -----------------------------------------------------------------------------
//
void CMceAmrCodec::ConstructL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComAudioCodec* codec = CMceComAMRCodec::NewLC( aSdpName );
    CMceAudioCodec::ConstructL( codec );

    CleanupStack::Pop( codec );
    }


// -----------------------------------------------------------------------------
// CMceAmrCodec::SetSdpNameL
// -----------------------------------------------------------------------------
//
void CMceAmrCodec::SetSdpNameL( const TDesC8& aSdpName )
    {
    __ASSERT_ALWAYS( aSdpName.Length() <= KMceMaxSdpNameLength, 
                     User::Leave( KErrArgument ) );
    FLAT_DATA( iSdpName ).Copy( aSdpName );
    }
    
