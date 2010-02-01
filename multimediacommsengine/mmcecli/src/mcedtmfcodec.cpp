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




#include "mcedtmfcodec.h"
#include "mcecomdtmfcodec.h"
#include "mcecomaudiocodec.h"
#include "mceaudiostream.h"


#define _FLAT_DATA static_cast<CMceComAudioCodec*>( iFlatData )
#define FLAT_DATA( data ) _FLAT_DATA->data

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceDtmfCodec::~CMceDtmfCodec
// -----------------------------------------------------------------------------
//
EXPORT_C CMceDtmfCodec::~CMceDtmfCodec()
    {
    }
    
// -----------------------------------------------------------------------------
// CMceDtmfCodec::CloneL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceAudioCodec* CMceDtmfCodec::CloneL() const
    {
    CMceDtmfCodec* clone = CMceDtmfCodec::NewLC( FLAT_DATA( iSdpName ) );
    
    CMceComAudioCodec* cloneFlatData = 
        static_cast<CMceComAudioCodec*>( clone->iFlatData );
    
    cloneFlatData->iID = FLAT_DATA( iID );
    cloneFlatData->SetFmtpAttributeL( *(FLAT_DATA( iFmtpAttr )) );
	cloneFlatData->iSamplingFreq = FLAT_DATA( iSamplingFreq );
	cloneFlatData->iPTime = FLAT_DATA( iPTime );
	cloneFlatData->iPayloadType = FLAT_DATA( iPayloadType );
	cloneFlatData->iCodecMode = FLAT_DATA( iCodecMode );
	cloneFlatData->iFourCC = FLAT_DATA( iFourCC );
	
	CleanupStack::Pop( clone );
	
	return clone;
    }
    
// -----------------------------------------------------------------------------
// CMceDtmfCodec::EnableVAD
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceDtmfCodec::EnableVAD( TBool /*aEnableVAD*/ )
    {
    return KErrNotSupported;
    }

// -----------------------------------------------------------------------------
// CMceDtmfCodec::SetBitrate
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceDtmfCodec::SetBitrate( TUint /*aBitrate*/ )
    {
    return KErrNotSupported;
    }

// -----------------------------------------------------------------------------
// CMceDtmfCodec::SetAllowedBitrates
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceDtmfCodec::SetAllowedBitrates( TUint aBitrates )
    {
    if ( aBitrates != AllowedBitrates() )
        {
        TInt ret = _FLAT_DATA->SetAllowedBitrates( aBitrates );
        if ( ret == KErrNone )
            {
            CMceAudioStream* stream = static_cast< CMceAudioStream* >( iStream );
            if ( stream && stream->BoundStream() )
                {
                // Find the same codec in bound stream, and set bitrate also to that. 
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
// CMceDtmfCodec::SetSamplingFreq
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceDtmfCodec::SetSamplingFreq( TUint aSamplingFreq )
    {
    return _FLAT_DATA->SetSamplingFreq( aSamplingFreq );
    }


// -----------------------------------------------------------------------------
// CMceDtmfCodec::SetPTime
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceDtmfCodec::SetPTime( TUint aPTime )
    {
    return _FLAT_DATA->SetPTime( aPTime );
    }

// -----------------------------------------------------------------------------
// CMceDtmfCodec::SetMaxPTime
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceDtmfCodec::SetMaxPTime( TUint /*aMaxPTime*/ )
    {
    return KErrNotSupported;
    }

// -----------------------------------------------------------------------------
// CMceDtmfCodec::SetPayloadType
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceDtmfCodec::SetPayloadType( TUint8 aPayloadType )
    {
    return _FLAT_DATA->SetPayloadType( aPayloadType );
    }
    
// -----------------------------------------------------------------------------
// CMceDtmfCodec::SetCodecMode
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceDtmfCodec::SetCodecMode( TUint aCodecMode )
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
// CMceDtmfCodec::NewL
// -----------------------------------------------------------------------------
//
CMceDtmfCodec* CMceDtmfCodec::NewL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceDtmfCodec* self = NewLC( aSdpName );
    CleanupStack::Pop( self );
    return self;
    
    }

// -----------------------------------------------------------------------------
// CMceDtmfCodec::NewL
// -----------------------------------------------------------------------------
//
CMceDtmfCodec* CMceDtmfCodec::NewLC( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceDtmfCodec* self = new (ELeave) CMceDtmfCodec();
    CleanupStack::PushL( self );
    self->ConstructL( aSdpName );
    return self;
    
    }

// -----------------------------------------------------------------------------
// CMceDtmfCodec::CMceDtmfCodec
// -----------------------------------------------------------------------------
//
CMceDtmfCodec::CMceDtmfCodec()
 : CMceAudioCodec()
    {
    }


// -----------------------------------------------------------------------------
// CMceDtmfCodec::ConstructL
// -----------------------------------------------------------------------------
//
void CMceDtmfCodec::ConstructL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComAudioCodec* codec = CMceComDtmfCodec::NewLC( aSdpName );
    CMceAudioCodec::ConstructL( codec );
	
    CleanupStack::Pop( codec );
    }


// -----------------------------------------------------------------------------
// CMceDtmfCodec::SetSdpNameL
// -----------------------------------------------------------------------------
//
void CMceDtmfCodec::SetSdpNameL( const TDesC8& aSdpName )
    {
    __ASSERT_ALWAYS( aSdpName.Length() <= KMceMaxSdpNameLength, 
                     User::Leave( KErrArgument ) );
    FLAT_DATA( iSdpName ).Copy( aSdpName );
    }
    
