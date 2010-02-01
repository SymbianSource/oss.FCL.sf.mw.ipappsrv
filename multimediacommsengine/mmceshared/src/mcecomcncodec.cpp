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




#include "mcecncodec.h"
#include "mcecomcncodec.h"

#ifdef MCE_COMMON_SERVER_SIDE

#include "mcecomaudiostream.h"

#endif//MCE_COMMON_SERVER_SIDE

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceComCnCodec::NewL
// -----------------------------------------------------------------------------
//
CMceComCnCodec* CMceComCnCodec::NewL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComCnCodec* self = NewLC( aSdpName );
    CleanupStack::Pop( self );
    return self;
    
    }

// -----------------------------------------------------------------------------
// CMceComCnCodec::NewLC
// -----------------------------------------------------------------------------
//
CMceComCnCodec* CMceComCnCodec::NewLC( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComCnCodec* self = new (ELeave) CMceComCnCodec();
    CleanupStack::PushL( self );
    self->ConstructL( aSdpName );
    return self;
    }


// -----------------------------------------------------------------------------
// CMceComCnCodec::~CMceComCnCodec
// -----------------------------------------------------------------------------
//
CMceComCnCodec::~CMceComCnCodec()
    {
    }
    

// -----------------------------------------------------------------------------
// CMceComCnCodec::CMceComCnCodec
// -----------------------------------------------------------------------------
//
CMceComCnCodec::CMceComCnCodec()
 : CMceComAudioCodec()
    {
    iState = CMceCodec::EDisabled; // NOTE: Cn is in disabled state by default
    
    iFourCC = 0;
    iEnableVAD = EFalse;
    iSamplingFreq  = KMceCnSamplingFreq;
    iPTime = KMceCnDefaultPtime;
    iMaxPTime = KMceCnDefaultMaxPtime;
    iPayloadType = KMceDefaultCnPayloadType;
    
    // valid default codec mode is needed for MCC codec interaction
    const TUint KCnCodecMode = 1;
    iCodecMode = KCnCodecMode;
    }

// -----------------------------------------------------------------------------
// CMceComCnCodec::SendSupported
// -----------------------------------------------------------------------------
//
TBool CMceComCnCodec::SendSupported()
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CMceComCnCodec::ConstructL
// -----------------------------------------------------------------------------
//
CMceComAudioCodec* CMceComCnCodec::CloneL()
    {
    CMceComCnCodec* copy = new (ELeave) CMceComCnCodec();
    CleanupStack::PushL( copy );
    copy->ConstructL( *this );
    CleanupStack::Pop( copy );
    return copy;
    
    }


// -----------------------------------------------------------------------------
// CMceComCnCodec::ConstructL
// -----------------------------------------------------------------------------
//
void CMceComCnCodec::ConstructL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComAudioCodec::ConstructL( aSdpName );
    }


// -----------------------------------------------------------------------------
// CMceComCnCodec::ConstructL
// -----------------------------------------------------------------------------
//
void CMceComCnCodec::ConstructL( CMceComCnCodec& aCodec )
    {
    CMceComAudioCodec::ConstructL( aCodec );
    }

#ifdef MCE_COMMON_SERVER_SIDE

// -----------------------------------------------------------------------------
// CMceComCnCodec::DoDecodeAudioL
// -----------------------------------------------------------------------------
//
void CMceComCnCodec::DoDecodeAudioL( 
    TInt aCodecIndex,
    RPointerArray<CMceSrvStream>& /*aStreams*/,
    CMceComAudioStream& aAudio,
    CMceMediaManager& /*aManager*/ )
    {
    // Special handling for comfort noise codecs, cn settings affect
    // to other codecs.
    //
    const TInt codecCount = aAudio.CodecCount();
    
    for ( TInt i = 0 ; i < codecCount ; ++i )
        {
        aAudio.CodecL( i )->iComfortNoiseEnabled = ETrue;
        
        // As cn cannot be used for streaming, select some other codec
        // for sending. 
        UpdateSendCodec( aCodecIndex, aAudio, *aAudio.CodecL( i ) );
        }
    }
    
#endif


// End of File

