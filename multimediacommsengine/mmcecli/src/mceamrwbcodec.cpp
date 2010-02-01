/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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




#include "mceamrwbcodec.h"
#include "mcecomamrwbcodec.h"


#define _FLAT_DATA static_cast< CMceComAudioCodec* >( iFlatData )
#define FLAT_DATA( data ) _FLAT_DATA->data

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceAmrWbCodec::~CMceAmrWbCodec
// -----------------------------------------------------------------------------
//
EXPORT_C CMceAmrWbCodec::~CMceAmrWbCodec()
    {
    }

// -----------------------------------------------------------------------------
// CMceAmrWbCodec::CloneL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceAudioCodec* CMceAmrWbCodec::CloneL() const
    {
    CMceAmrWbCodec* clone = CMceAmrWbCodec::NewLC( FLAT_DATA( iSdpName ) );
    
    CMceComAudioCodec* cloneFlatData = 
        static_cast< CMceComAudioCodec* >( clone->iFlatData );

    cloneFlatData->iID = FLAT_DATA( iID );
    cloneFlatData->SetFmtpAttributeL( *( FLAT_DATA( iFmtpAttr ) ) );
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
// CMceAmrWbCodec::NewL
// -----------------------------------------------------------------------------
//
CMceAmrWbCodec* CMceAmrWbCodec::NewL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceAmrWbCodec* self = NewLC( aSdpName );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMceAmrWbCodec::NewL
// -----------------------------------------------------------------------------
//
CMceAmrWbCodec* CMceAmrWbCodec::NewLC( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceAmrWbCodec* self = new ( ELeave ) CMceAmrWbCodec();
    CleanupStack::PushL( self );
    self->ConstructL( aSdpName );
    return self;
    }

// -----------------------------------------------------------------------------
// CMceAmrWbCodec::CMceAmrWbCodec
// -----------------------------------------------------------------------------
//
CMceAmrWbCodec::CMceAmrWbCodec() : CMceAmrCodec()
    {
    }

// -----------------------------------------------------------------------------
// CMceAmrWbCodec::ConstructL
// -----------------------------------------------------------------------------
//
void CMceAmrWbCodec::ConstructL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComAudioCodec* codec = CMceComAMRWbCodec::NewLC( aSdpName );
    CMceAudioCodec::ConstructL( codec );
    CleanupStack::Pop( codec );
    }
