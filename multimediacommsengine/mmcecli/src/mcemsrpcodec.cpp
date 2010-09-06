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



#include "mcemsrpcodec.h"
#include "mcecommsrpcodec.h"
#include "mcemessagestream.h"


#define _FLAT_DATA static_cast<CMceComMessageCodec*>( iFlatData )
#define FLAT_DATA( data ) _FLAT_DATA->data

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceMsrpCodec::~CMceMsrpCodec
// -----------------------------------------------------------------------------
//
EXPORT_C CMceMsrpCodec::~CMceMsrpCodec()
    {
    }
    
// -----------------------------------------------------------------------------
// CMceMsrpCodec::CloneL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceMessageCodec* CMceMsrpCodec::CloneL() const
    {
    CMceMsrpCodec* clone = CMceMsrpCodec::NewLC( FLAT_DATA( iSdpName ) );
    
    CMceComMessageCodec* cloneFlatData = 
        static_cast<CMceComMessageCodec*>( clone->iFlatData );
    
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
// CMceMsrpCodec::EnableVAD
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceMsrpCodec::EnableVAD( TBool aEnableVAD )
    {
    FLAT_DATA( iEnableVAD ) = aEnableVAD;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMceMsrpCodec::SetBitrate
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceMsrpCodec::SetBitrate(TUint /*aBitrate*/)
    {
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMceMsrpCodec::SetAllowedBitrates
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceMsrpCodec::SetAllowedBitrates(TUint /*aBitrates*/)
    {
    return KErrNone;
    }
    
    
// -----------------------------------------------------------------------------
// CMceMsrpCodec::SetSamplingFreq
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceMsrpCodec::SetSamplingFreq(TUint aSamplingFreq)
    {
    FLAT_DATA( iSamplingFreq ) = aSamplingFreq;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMceMsrpCodec::SetPTime
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceMsrpCodec::SetPTime(TUint aPTime)
    {
    FLAT_DATA( iPTime ) = aPTime;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMceMsrpCodec::SetMaxPTime
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceMsrpCodec::SetMaxPTime(TUint aMaxPTime)
    {
    FLAT_DATA( iMaxPTime ) = aMaxPTime;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMceMsrpCodec::SetPayloadType
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceMsrpCodec::SetPayloadType(TUint8 aPayloadType)
    {
    FLAT_DATA( iPayloadType ) = aPayloadType;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMceMsrpCodec::SetCodecMode
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceMsrpCodec::SetCodecMode( TUint /*aCodecMode*/ )
    {
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMceMsrpCodec::NewL
// -----------------------------------------------------------------------------
//
CMceMsrpCodec* CMceMsrpCodec::NewL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceMsrpCodec* self = NewLC( aSdpName );
    CleanupStack::Pop( self );
    return self;
    
    }

// -----------------------------------------------------------------------------
// CMceMsrpCodec::NewL
// -----------------------------------------------------------------------------
//
CMceMsrpCodec* CMceMsrpCodec::NewLC( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceMsrpCodec* self = new (ELeave) CMceMsrpCodec();
    CleanupStack::PushL( self );
    self->ConstructL( aSdpName );
    return self;
    
    }

// -----------------------------------------------------------------------------
// CMceMsrpCodec::CMceMsrpCodec
// -----------------------------------------------------------------------------
//
CMceMsrpCodec::CMceMsrpCodec()
 : CMceMessageCodec()
    {
    }


// -----------------------------------------------------------------------------
// CMceMsrpCodec::ConstructL
// -----------------------------------------------------------------------------
//
void CMceMsrpCodec::ConstructL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComMessageCodec* codec = CMceComMsrpCodec::NewLC( aSdpName );
    CMceMessageCodec::ConstructL( codec );

    CleanupStack::Pop( codec );
    }


// -----------------------------------------------------------------------------
// CMceMsrpCodec::SetSdpNameL
// -----------------------------------------------------------------------------
//
void CMceMsrpCodec::SetSdpNameL( const TDesC8& aSdpName )
    {
    __ASSERT_ALWAYS( aSdpName.Length() <= KMceMaxSdpNameLength, 
                     User::Leave( KErrArgument ) );
    FLAT_DATA( iSdpName ).Copy( aSdpName );
    }
    
