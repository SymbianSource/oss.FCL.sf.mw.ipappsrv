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




#include "mceredcodec.h"
#include "mcecomredcodec.h"

#ifdef MCE_COMMON_SERVER_SIDE

#include <mmcccodecinformationfactory.h>
#include <mmcccodecinformation.h>
#include "mcecomaudiostream.h"

#endif //MCE_COMMON_SERVER_SIDE

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceComRedCodec::NewL
// -----------------------------------------------------------------------------
//
CMceComRedCodec* CMceComRedCodec::NewL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComRedCodec* self = NewLC( aSdpName );
    CleanupStack::Pop( self );
    return self;
    
    }

// -----------------------------------------------------------------------------
// CMceComRedCodec::NewLC
// -----------------------------------------------------------------------------
//
CMceComRedCodec* CMceComRedCodec::NewLC( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComRedCodec* self = new (ELeave) CMceComRedCodec();
    CleanupStack::PushL( self );
    self->ConstructL( aSdpName );
    return self;
    }


// -----------------------------------------------------------------------------
// CMceComRedCodec::~CMceComRedCodec
// -----------------------------------------------------------------------------
//
CMceComRedCodec::~CMceComRedCodec()
    {
    }
    

// -----------------------------------------------------------------------------
// CMceComRedCodec::CMceComRedCodec
// -----------------------------------------------------------------------------
//
CMceComRedCodec::CMceComRedCodec()
 : CMceComAudioCodec()
    {
    iState = CMceCodec::EDisabled; // NOTE: Red is in disabled state by default
    
    iFourCC = 0;
    iEnableVAD = EFalse;
    iSamplingFreq  = KMceRedSamplingFreq;   
    iPTime = KMceRedDefaultPtime;
    iMaxPTime = KMceRedDefaultMaxPtime;
    iPayloadType = KMceDefaultRedPayloadType;
    
    // valid default codec mode is needed for MCC codec interaction
    const TUint KRedCodecMode = 1;
    iCodecMode = KRedCodecMode;
    }

// -----------------------------------------------------------------------------
// CMceComRedCodec::SendSupported
// -----------------------------------------------------------------------------
//
TBool CMceComRedCodec::SendSupported()
    {
    return EFalse;
    }
    
// -----------------------------------------------------------------------------
// CMceComRedCodec::ConstructL
// -----------------------------------------------------------------------------
//
CMceComAudioCodec* CMceComRedCodec::CloneL()
    {
    CMceComRedCodec* copy = new (ELeave) CMceComRedCodec();
    CleanupStack::PushL( copy );
    copy->ConstructL( *this );
    CleanupStack::Pop( copy );
    return copy;
    
    }


// -----------------------------------------------------------------------------
// CMceComRedCodec::ConstructL
// -----------------------------------------------------------------------------
//
void CMceComRedCodec::ConstructL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComAudioCodec::ConstructL( aSdpName );
    }


// -----------------------------------------------------------------------------
// CMceComRedCodec::ConstructL
// -----------------------------------------------------------------------------
//
void CMceComRedCodec::ConstructL( CMceComRedCodec& aCodec )
    {
    CMceComAudioCodec::ConstructL( aCodec );
    }


// -----------------------------------------------------------------------------
// CMceComRedCodec::SetPayloadType
// -----------------------------------------------------------------------------
//
TInt CMceComRedCodec::SetPayloadType( TUint8 aPayloadType )
    {
    if ( KMinDynPayloadType <= aPayloadType )
        {
        return CMceComCodec::SetPayloadType( aPayloadType );
        }
    else
        {
        return KErrNotSupported;
        }
    }


#ifdef MCE_COMMON_SERVER_SIDE

// -----------------------------------------------------------------------------
// CMceComRedCodec::DoDecodeAudioL
// -----------------------------------------------------------------------------
//
void CMceComRedCodec::DoDecodeAudioL( 
    TInt aCodecIndex,
    RPointerArray<CMceSrvStream>& /*aStreams*/,
    CMceComAudioStream& aAudio,
    CMceMediaManager& /*aManager*/ )
    {
    // Special handling for red codecs, red settings affect to other codecs.
    // If red codec is preferred send codec, associated codec of highest
    // preference is selected as send stream. If there's no associated
    // codec(s) for the red for some reason, next codec with sending capability
    // is enabled.
    //
    CMccCodecInformationFactory* factory = CMccCodecInformationFactory::NewL();
    CleanupStack::PushL( factory );
    CMccCodecInformation* mccCodec = factory->CreateCodecInformationL( iSdpName );
    CleanupStack::PushL( mccCodec );
    
    // Use dummy mcc codec for parsing the fmtp
    mccCodec->SetFmtpAttrL( *iFmtpAttr, ETrue );
    
    RArray<TUint> redPayloads;
    CleanupClosePushL( redPayloads );
    mccCodec->RedPayloadsL( redPayloads );
    
    const TInt codecCount = aAudio.CodecCount();
    
    for ( TInt i = 0 ; i < codecCount ; i++ )
        {
        // There needs to be two entries of certain payload type in red payloads
        // array if red level 1 should be used.
        //
        CMceComAudioCodec* codec = aAudio.CodecL( i );
        
        const TInt KMceRedNotPresent( -1 );
        TInt redCount( KMceRedNotPresent );
        
        for ( TInt j = 0; j < redPayloads.Count(); j++ )
            {
            if ( codec->iPayloadType == static_cast<TUint8>( redPayloads[ j ] ) )
                {
                redCount++;
                }
            }
        
        if ( redCount > 0 )
            {
            codec->iRedPayloadType = iPayloadType;
            codec->iRedCount = redCount;
            UpdateSendCodec( aCodecIndex, aAudio, *codec );
            }
        else
            {
            codec->iRedPayloadType = KMcePayloadTypeUndefined;
            codec->iRedCount = 0;
            }
        
        if ( !redPayloads.Count() )
            {
            UpdateSendCodec( aCodecIndex, aAudio, *codec );
            }
        }
    
    CleanupStack::PopAndDestroy( &redPayloads );    
    CleanupStack::PopAndDestroy( mccCodec );
    CleanupStack::PopAndDestroy( factory );
    }
    
#endif

// End of File

