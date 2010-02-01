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
#include "mcecomaudiocodec.h"
#include "mceaudiostream.h"


#define _FLAT_DATA static_cast<CMceComAudioCodec*>( iFlatData )
#define FLAT_DATA( data ) _FLAT_DATA->data

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceRedCodec::~CMceRedCodec
// -----------------------------------------------------------------------------
//
EXPORT_C CMceRedCodec::~CMceRedCodec()
    {
    }
    
// -----------------------------------------------------------------------------
// CMceRedCodec::CloneL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceAudioCodec* CMceRedCodec::CloneL() const
    {
    CMceRedCodec* clone = CMceRedCodec::NewLC( FLAT_DATA( iSdpName ) );
    
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
// CMceRedCodec::EnableVAD
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceRedCodec::EnableVAD( TBool aEnableVAD )
    {
    FLAT_DATA( iEnableVAD ) = aEnableVAD;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMceRedCodec::SetBitrate
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceRedCodec::SetBitrate( TUint /*aBitrate*/ )
    {
    return KErrNotSupported;
    }

// -----------------------------------------------------------------------------
// CMceRedCodec::SetAllowedBitrates
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceRedCodec::SetAllowedBitrates( TUint /*aBitrates*/ )
    {
    return KErrNotSupported;
    }

  
// -----------------------------------------------------------------------------
// CMceRedCodec::SetSamplingFreq
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceRedCodec::SetSamplingFreq(TUint aSamplingFreq)
    {
    return _FLAT_DATA->SetSamplingFreq( aSamplingFreq );
    }


// -----------------------------------------------------------------------------
// CMceRedCodec::SetPTime
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceRedCodec::SetPTime( TUint aPTime )
    {
    return _FLAT_DATA->SetPTime( aPTime );
    }

// -----------------------------------------------------------------------------
// CMceRedCodec::SetMaxPTime
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceRedCodec::SetMaxPTime( TUint aMaxPTime )
    {
    return _FLAT_DATA->SetMaxPTime( aMaxPTime );
    }

// -----------------------------------------------------------------------------
// CMceRedCodec::SetPayloadType
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceRedCodec::SetPayloadType( TUint8 aPayloadType )
    {
    return _FLAT_DATA->SetPayloadType( aPayloadType );
    }
    
// -----------------------------------------------------------------------------
// CMceRedCodec::SetCodecMode
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceRedCodec::SetCodecMode( TUint /*aCodecMode*/ )
    {
    return KErrNotSupported;
    }

// -----------------------------------------------------------------------------
// CMceRedCodec::SetRedPayloadTypesL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceRedCodec::SetRedPayloadTypesL( 
    const RArray<TUint>& /*aRedPaylodTypes*/ )
    {
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CMceRedCodec::RedPayloadTypesL
// -----------------------------------------------------------------------------
//	    
EXPORT_C void CMceRedCodec::RedPayloadTypesL( RArray<TUint>& /*aRedPayloadTypes*/ )
    {
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CMceRedCodec::NewL
// -----------------------------------------------------------------------------
//
CMceRedCodec* CMceRedCodec::NewL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceRedCodec* self = NewLC( aSdpName );
    CleanupStack::Pop( self );
    return self;
    
    }

// -----------------------------------------------------------------------------
// CMceRedCodec::NewL
// -----------------------------------------------------------------------------
//
CMceRedCodec* CMceRedCodec::NewLC( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceRedCodec* self = new (ELeave) CMceRedCodec();
    CleanupStack::PushL( self );
    self->ConstructL( aSdpName );
    return self;
    
    }

// -----------------------------------------------------------------------------
// CMceRedCodec::CMceRedCodec
// -----------------------------------------------------------------------------
//
CMceRedCodec::CMceRedCodec()
 : CMceAudioCodec()
    {
    }


// -----------------------------------------------------------------------------
// CMceRedCodec::ConstructL
// -----------------------------------------------------------------------------
//
void CMceRedCodec::ConstructL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComAudioCodec* codec = CMceComRedCodec::NewLC( aSdpName );
    CMceAudioCodec::ConstructL( codec );
	
    CleanupStack::Pop( codec );
    }


// -----------------------------------------------------------------------------
// CMceRedCodec::SetSdpNameL
// -----------------------------------------------------------------------------
//
void CMceRedCodec::SetSdpNameL( const TDesC8& aSdpName )
    {
    __ASSERT_ALWAYS( aSdpName.Length() <= KMceMaxSdpNameLength, 
                     User::Leave( KErrArgument ) );
    FLAT_DATA( iSdpName ).Copy( aSdpName );
    }
    
