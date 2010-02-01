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
* Description:    MCC Comfort Noise CodecInformation
*
*/




// INCLUDE FILES
#include "mmcccodeccn.h"
#include "mccuids.hrh"
#include "mmccinterfacelogs.h"


// ============================= LOCAL FUNCTIONS ===============================


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccCodecCn::CMccCodecCn
// default constructor
// -----------------------------------------------------------------------------
//
CMccCodecCn::CMccCodecCn() : CMccCodecInformation()
    {
    
    }

// -----------------------------------------------------------------------------
// CMccCodecCn::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMccCodecCn::ConstructL()
    {
    iSdpName.Copy( KCnSdpName );

    iFmtpAttr = HBufC8::NewL( 1 );
    TPtr8 ptr = iFmtpAttr->Des();
    ptr.Append( KNullDesC );
    
    SetSamplingFreq( KCnSamplingFreq );
    EnableVAD( EFalse );
    SetMaxPTime( KCnMaxPTime ); // recommended "limit" 200ms
    SetPTime( KCnPTime ); // default 20ms 
    SetPayloadType( KCnPayloadType );  
    
    iFourCC = KMccFourCCIdCN;
    iCodecMode = ENothing;
    }

// -----------------------------------------------------------------------------
// CMccCodecCn::NewLC
// Static constructor.
// -----------------------------------------------------------------------------
//
CMccCodecCn* CMccCodecCn::NewL()
    {
    CMccCodecCn* self = new (ELeave) CMccCodecCn;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self ); 
    return self;
    }

// -----------------------------------------------------------------------------
// CMccCodecCn::~CMccCodecCn
// Destructor
// -----------------------------------------------------------------------------   
//
CMccCodecCn::~CMccCodecCn()
    {
    }

// -----------------------------------------------------------------------------
// CMccCodecCn::RequireSignalling
// -----------------------------------------------------------------------------
//
TBool CMccCodecCn::RequireSignalling(
    const CMccCodecInformation& aCandidate ) const      
    {
    return CMccCodecInformation::RequireSignalling( aCandidate );
    }

// -----------------------------------------------------------------------------
// CMccCodecCn::SetBitrate
// Sets the bitrate used with codec.
// -----------------------------------------------------------------------------
//
TInt CMccCodecCn::SetBitrate( TUint aBitrate )
    {
    iBitrate = aBitrate;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccCodecCn::SetSamplingFreq
// Sets the sampling frequency. 
// -----------------------------------------------------------------------------
//
TInt CMccCodecCn::SetSamplingFreq( TUint32 aSamplingFreq )
    {
    iSamplingFreq = aSamplingFreq;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccCodecCn::SetSdpName
// Sets the SDP name
// -----------------------------------------------------------------------------
TInt CMccCodecCn::SetSdpName( const TDesC8& aSdpName )
    {
    if ( !aSdpName.CompareF( KCnSdpName ) )
        {
        iSdpName.Copy( aSdpName );
        }
    else 
        {
        return KErrNotSupported;
        }
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccCodecCn::PayloadType
// Sets the payload type
// -----------------------------------------------------------------------------
TInt CMccCodecCn::SetPayloadType( TUint8 aPayloadType )
    {
    if ( aPayloadType == KCnPayloadType ||
         aPayloadType == KCnPayloadTypeReserved ) 
        {
        iPayloadType = aPayloadType;
        return KErrNone;
        }
    return KErrNotSupported;
    }
    
// -----------------------------------------------------------------------------
// CMccCodecCn::SetCodecMode
// Sets the codec mode
// -----------------------------------------------------------------------------
TInt CMccCodecCn::SetCodecMode( TCodecMode aCodecMode )
    {
    if ( ENothing == aCodecMode )
        {
        iCodecMode = aCodecMode;
        return KErrNone;
        }
    else
        {
        return KErrNotSupported;
        }
    }
     
// -----------------------------------------------------------------------------
// CMccCodecCn::EnableVAD
// 
// -----------------------------------------------------------------------------
TInt CMccCodecCn::EnableVAD( TBool aEnableVAD ) 
    {
    iEnableVAD = aEnableVAD;        
    return KErrNone;
    }    

// -----------------------------------------------------------------------------
// CMccCodecCn::SetPTime
//
// -----------------------------------------------------------------------------
//
TInt CMccCodecCn::SetPTime( TUint aPTime )
    {
    if ( aPTime && ( KMinPtime <= aPTime ) &&  ( aPTime <= KMaxPtime ) && 
        (( aPTime % KMinPtime )  == 0 ))
        {
        iPTime = aPTime;
        return KErrNone;    
        }
    else
        {
        return KErrNotSupported;
        }
    }
    
// -----------------------------------------------------------------------------
// CMccCodecCn::SetMaxPTime
//
// -----------------------------------------------------------------------------
//
TInt CMccCodecCn::SetMaxPTime( TUint aMaxPTime )
    {
    if ( aMaxPTime && ( iPTime <= aMaxPTime )&& ( KMinPtime <= aMaxPTime ) && 
        ( aMaxPTime <= KMaxPtime ) && (( aMaxPTime % KMinPtime )  == 0 ) )
        {
        iMaxPTime = aMaxPTime;
        return KErrNone;    
        }
    else
        {
        return KErrNotSupported;
        }
    }

// -----------------------------------------------------------------------------
// CMccCodecCn::SetAllowedBitrates
// -----------------------------------------------------------------------------
// 
TInt CMccCodecCn::SetAllowedBitrates( TUint /*aBitrateMask*/ )
    {
    return KErrNone;           
    }

// -----------------------------------------------------------------------------
// CMccCodecCn::CloneDefaultsL
// Make a default setting clone from this codec
// -----------------------------------------------------------------------------
//
CMccCodecInformation* CMccCodecCn::CloneDefaultsL()
    {
    return CMccCodecCn::NewL();
    }

// -----------------------------------------------------------------------------
// CMCCAMRCodec::CloneDetailedL
// Make a detailed clone from this codec
// -----------------------------------------------------------------------------
//
CMccCodecInformation* CMccCodecCn::CloneDetailedL()
    {
    CMccCodecCn* newCodec = CMccCodecCn::NewL();
    CleanupStack::PushL( newCodec );   
    newCodec->SetBitrate( this->Bitrate() );
    newCodec->SetCodecMode( this->CodecMode() );
    newCodec->SetMaxPTime( this->MaxPTime() );
    newCodec->SetPayloadType( this->PayloadType() );
    newCodec->SetPTime( this->PTime() );
    newCodec->SetSamplingFreq( this->SamplingFreq() );
    newCodec->SetSdpName( this->SdpName() );
    newCodec->ParseFmtpAttrL( this->GetFmtpL() );
    CleanupStack::Pop( newCodec );       
    return newCodec;
    }
    
// ========================== OTHER EXPORTED FUNCTIONS =========================


//  End of File  
