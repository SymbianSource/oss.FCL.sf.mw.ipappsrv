/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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





// INCLUDE FILES
#include "mmcccodecaac.h"
#include "mccuids.hrh"
#include "mmccinterfacelogs.h"

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccCodecAAC::CMccCodecAAC
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMccCodecAAC::CMccCodecAAC() : CMccCodecInformation()
    {
    }

// -----------------------------------------------------------------------------
// CMccCodecAAC::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMccCodecAAC::ConstructL()
    {
    iSdpName.Copy( KAACSdpName );

    iType = KUidMediaTypeAudio;
    }

// -----------------------------------------------------------------------------
// CMccCodecAAC::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMccCodecAAC* CMccCodecAAC::NewL()
    {
    CMccCodecAAC* self = new( ELeave ) CMccCodecAAC;
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// -----------------------------------------------------------------------------
// CMccCodecAAC::~CMccCodecAAC
// 
// -----------------------------------------------------------------------------
//
CMccCodecAAC::~CMccCodecAAC()
    {
    }

// -----------------------------------------------------------------------------
// CMccCodecAAC::RequireSignalling
// -----------------------------------------------------------------------------
//
TBool CMccCodecAAC::RequireSignalling(
    const CMccCodecInformation& aCandidate ) const      
    {
    return CMccCodecInformation::RequireSignalling( aCandidate );
    }
 
// -----------------------------------------------------------------------------
// CMccCodecAAC::SetBitrate
// Sets the bitrate used with AMR codec.
// -----------------------------------------------------------------------------
//
TInt CMccCodecAAC::SetBitrate( TUint aBitrate )
    {
    if ( aBitrate > 0 )
        {            
        iBitrate = aBitrate;
        return KErrNone;
        }
    else
        {
        return KErrNotSupported;
        }
    }

// -----------------------------------------------------------------------------
// CMccCodecAAC::SetSamplingFreq
// Sets the sampling frequency. 
// -----------------------------------------------------------------------------
//
TInt CMccCodecAAC::SetSamplingFreq( TUint32 aSamplingFreq )
    {
    iSamplingFreq = aSamplingFreq;
    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// CMccCodecAAC::SetSdpName
// Sets the SDP name
// -----------------------------------------------------------------------------
//
TInt CMccCodecAAC::SetSdpName( const TDesC8& aSdpName )
    {
    if ( 0 == aSdpName.CompareF( KAACSdpName ) )
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
// CMccCodecAAC::PayloadType
// Sets the payload type
// -----------------------------------------------------------------------------
//
TInt CMccCodecAAC::SetPayloadType( TUint8 aPayloadType )
    {
    if ( aPayloadType < KMinDynamicPT ||
         aPayloadType > KMaxPayloadType ) 
        {
        return KErrNotSupported;
        }
    else 
        {
        iPayloadType = aPayloadType;
        }
              
    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// CMccCodecAAC::SetCodecMode
// Sets the codec mode.
// -----------------------------------------------------------------------------
//
TInt CMccCodecAAC::SetCodecMode( TCodecMode aCodecMode )
    {
    iCodecMode = aCodecMode;
    return KErrNone;
    }
 
// -----------------------------------------------------------------------------
// CMccCodecAAC::EnableVAD
// Enable / Disable VAD
// -----------------------------------------------------------------------------
//
TInt CMccCodecAAC::EnableVAD( TBool /*aEnableVAD*/ ) 
    {
    return KErrNotSupported;
    }
    
// -----------------------------------------------------------------------------
// CMccCodecAAC::SetPTime
// Set PTime
// -----------------------------------------------------------------------------
//
TInt CMccCodecAAC::SetPTime( TUint /*aPTime*/ )
    {
    return KErrNotSupported;
    }
    
// -----------------------------------------------------------------------------
// CMccCodecAAC::SetMaxPTime
// Set Max PTime
// -----------------------------------------------------------------------------
//
TInt CMccCodecAAC::SetMaxPTime( TUint /*aMaxPTime*/ )
    {
    return KErrNotSupported;
    }
    
// -----------------------------------------------------------------------------
// CMccCodecAAC::CloneDefaultsL
// Make a default setting clone from this codec
// -----------------------------------------------------------------------------
//
CMccCodecInformation* CMccCodecAAC::CloneDefaultsL()
    {
    return CMccCodecAAC::NewL();
    }

// -----------------------------------------------------------------------------
// CMccCodecAAC::CloneDetailedL
// Make a detailed clone from this codec
// -----------------------------------------------------------------------------
//
CMccCodecInformation* CMccCodecAAC::CloneDetailedL()
    {
    CMccCodecAAC* newCodec = CMccCodecAAC::NewL();
    CleanupStack::PushL( newCodec );       
    newCodec->SetBitrate( this->Bitrate() );
    newCodec->SetCodecMode( this->CodecMode() );
    newCodec->SetMaxPTime( this->MaxPTime() );
    newCodec->SetPayloadType( this->PayloadType() );
    newCodec->SetPTime( this->PTime() );
    newCodec->SetSamplingFreq( this->SamplingFreq() );
    newCodec->SetSdpName( this->SdpName() );
    CleanupStack::Pop( newCodec );   
    return newCodec;
    }
        
// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File  
