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
#include "mcecomaudiocodec.h"
#include "mceaudiostream.h"


#define _FLAT_DATA static_cast<CMceComAudioCodec*>( iFlatData )
#define FLAT_DATA( data ) _FLAT_DATA->data

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceCnCodec::~CMceCnCodec
// -----------------------------------------------------------------------------
//
EXPORT_C CMceCnCodec::~CMceCnCodec()
    {
    }
    
// -----------------------------------------------------------------------------
// CMceCnCodec::CloneL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceAudioCodec* CMceCnCodec::CloneL() const
    {
    CMceCnCodec* clone = CMceCnCodec::NewLC( FLAT_DATA( iSdpName ) );
    
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
// CMceCnCodec::EnableVAD
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceCnCodec::EnableVAD( TBool /*aEnableVAD*/ )
    {
    return KErrNotSupported;
    }

// -----------------------------------------------------------------------------
// CMceCnCodec::SetBitrate
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceCnCodec::SetBitrate( TUint /*aBitrate*/ )
    {
    return KErrNotSupported;
    }

// -----------------------------------------------------------------------------
// CMceCnCodec::SetAllowedBitrates
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceCnCodec::SetAllowedBitrates( TUint /*aBitrates*/ )
    {
    return KErrNotSupported;
    }

  
// -----------------------------------------------------------------------------
// CMceCnCodec::SetSamplingFreq
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceCnCodec::SetSamplingFreq( TUint aSamplingFreq )
    {
    return _FLAT_DATA->SetSamplingFreq( aSamplingFreq );
    }


// -----------------------------------------------------------------------------
// CMceCnCodec::SetPTime
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceCnCodec::SetPTime( TUint aPTime )
    {
    return _FLAT_DATA->SetPTime( aPTime );
    }

// -----------------------------------------------------------------------------
// CMceCnCodec::SetMaxPTime
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceCnCodec::SetMaxPTime( TUint aMaxPTime )
    {
    return _FLAT_DATA->SetMaxPTime( aMaxPTime );
    }

// -----------------------------------------------------------------------------
// CMceCnCodec::SetPayloadType
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceCnCodec::SetPayloadType( TUint8 aPayloadType )
    {
    return _FLAT_DATA->SetPayloadType( aPayloadType );
    }
    
// -----------------------------------------------------------------------------
// CMceCnCodec::SetCodecMode
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceCnCodec::SetCodecMode( TUint /*aCodecMode*/ )
    {
    return KErrNotSupported;
    }


// -----------------------------------------------------------------------------
// CMceCnCodec::NewL
// -----------------------------------------------------------------------------
//
CMceCnCodec* CMceCnCodec::NewL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceCnCodec* self = NewLC( aSdpName );
    CleanupStack::Pop( self );
    return self;
    
    }

// -----------------------------------------------------------------------------
// CMceCnCodec::NewL
// -----------------------------------------------------------------------------
//
CMceCnCodec* CMceCnCodec::NewLC( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceCnCodec* self = new (ELeave) CMceCnCodec();
    CleanupStack::PushL( self );
    self->ConstructL( aSdpName );
    return self;
    
    }

// -----------------------------------------------------------------------------
// CMceCnCodec::CMceCnCodec
// -----------------------------------------------------------------------------
//
CMceCnCodec::CMceCnCodec()
 : CMceAudioCodec()
    {
    }


// -----------------------------------------------------------------------------
// CMceCnCodec::ConstructL
// -----------------------------------------------------------------------------
//
void CMceCnCodec::ConstructL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComAudioCodec* codec = CMceComCnCodec::NewLC( aSdpName );
    CMceAudioCodec::ConstructL( codec );
	
    CleanupStack::Pop( codec );
    }


// -----------------------------------------------------------------------------
// CMceCnCodec::SetSdpNameL
// -----------------------------------------------------------------------------
//
void CMceCnCodec::SetSdpNameL( const TDesC8& aSdpName )
    {
    __ASSERT_ALWAYS( aSdpName.Length() <= KMceMaxSdpNameLength, 
                     User::Leave( KErrArgument ) );
    FLAT_DATA( iSdpName ).Copy( aSdpName );
    }
    
