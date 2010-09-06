/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Factory class to create Codec information instances
*
*/





// INCLUDE FILES
#include "mmcccodecmsrp.h"
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
// CMccCodecMsrp::CMccCodecMsrp
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMccCodecMsrp::CMccCodecMsrp() : CMccCodecInformation()
    {
    }

// -----------------------------------------------------------------------------
// CMccCodecMsrp::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMccCodecMsrp::ConstructL()
    {
    iSdpName.Copy( KMSRPSdpName );
	iType = KUidMediaTypeMessage;

    iHwFrameTime = 20;
    iFrameSize = 20;
    SetSamplingFreq( 1000 );
    SetBitrate( 64 );
    EnableVAD( EFalse );
    SetMaxPTime( 100 );
    SetPTime( 20 );
    iFourCC = KMccFourCCIdMSRP;
    iPayloadFormatEncoder = KImplUidMsrpPayloadFormatEncode;
    iPayloadFormatDecoder = KImplUidMsrpPayloadFormatDecode;
    }

// -----------------------------------------------------------------------------
// CMccCodecMsrp::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMccCodecMsrp* CMccCodecMsrp::NewL()
    {
    CMccCodecMsrp* self = new( ELeave ) CMccCodecMsrp;
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// -----------------------------------------------------------------------------
// CMccCodecMsrp::~CMccCodecMsrp
// 
// -----------------------------------------------------------------------------
//
CMccCodecMsrp::~CMccCodecMsrp()
    {
    }

// -----------------------------------------------------------------------------
// CMccCodecMsrp::RequireSignalling
// -----------------------------------------------------------------------------
//
TBool CMccCodecMsrp::RequireSignalling(
    const CMccCodecInformation& aCandidate ) const      
    {
    __INTERFACE( "CMccCodecMsrp::RequireSignalling" )
    TBool ret = EFalse;
              
    if ( iPTime != aCandidate.PTime() )
        {
        __INTERFACE( "CMccCodecMsrp::RequireSignalling, PTime changed" )
        ret = ETrue;
        }
        
    if ( iMaxPTime != aCandidate.MaxPTime() )
        {
        __INTERFACE( "CMccCodecMsrp::RequireSignalling, MaxPTime changed" )
        ret = ETrue;
        }
        
    if ( iPayloadType != aCandidate.PayloadType() )
        {
        __INTERFACE( "CMccCodecMsrp::RequireSignalling, PayloadType changed" )
        ret = ETrue;
        }

    if ( iBitrateMask != aCandidate.AllowedBitrates() )
        {
        __INTERFACE( "CMccCodecMsrp::RequireSignalling, BitrateMask changed" )
        ret = ETrue;
        }

    if ( iCodecMode != aCandidate.CodecMode() )
        {
        __INTERFACE( "CMccCodecMsrp::RequireSignalling, CodecMode changed" )
        ret = ETrue;
        }
        
    if ( iSamplingFreq != aCandidate.SamplingFreq() )
        {
        __INTERFACE( "CMccCodecMsrp::RequireSignalling, SamplingFreq changed" )
        ret = ETrue;
        }     
        
    if ( iMaxBitrate != aCandidate.MaxBitrate() )
        {
        __INTERFACE( "CMccCodecMsrp::RequireSignalling, MaxBitrate changed" )
        ret = ETrue;
        }
             
    __INTERFACE_INT1( "CMccCodecMsrp::RequireSignalling, exit with", ret )
    return ret;          
    }
 
// -----------------------------------------------------------------------------
// CMccCodecMsrp::SetBitrate
// Sets the bitrate used with AMR codec.
// -----------------------------------------------------------------------------
//
TInt CMccCodecMsrp::SetBitrate( TUint aBitrate )
    {
    
    // Not supported
    iBitrate = aBitrate;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccCodecMsrp::SetSamplingFreq
// Sets the sampling frequency. 
// -----------------------------------------------------------------------------
//
TInt CMccCodecMsrp::SetSamplingFreq( TUint32 aSamplingFreq )
    {
    // Not supported
    
    iSamplingFreq = aSamplingFreq;
    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// CMccCodecMsrp::SetSdpName
// Sets the SDP name
// -----------------------------------------------------------------------------
//
TInt CMccCodecMsrp::SetSdpName( const TDesC8& aSdpName )
    {
    // Not supported
    iSdpName.Copy( aSdpName );
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccCodecMsrp::PayloadType
// Sets the payload type
// -----------------------------------------------------------------------------
//
TInt CMccCodecMsrp::SetPayloadType( TUint8 /*aPayloadType*/ )
    {
    // Not supported
    TUint8 aPayloadType = KPayloadType; // dummy value for messgae codec payload type aPayloadType;
    iPayloadType = aPayloadType;
    
    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// CMccCodecMsrp::SetCodecMode
// Sets the codec mode.
// -----------------------------------------------------------------------------
//
TInt CMccCodecMsrp::SetCodecMode( TCodecMode aCodecMode )
    {
    // Not supported
    iCodecMode = aCodecMode;
    return KErrNone;
    }
 
// -----------------------------------------------------------------------------
// CMccCodecMsrp::EnableVAD
// Enable / Disable VAD
// -----------------------------------------------------------------------------
//
TInt CMccCodecMsrp::EnableVAD( TBool /*aEnableVAD*/ ) 
    {
    // Not supported
    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// CMccCodecMsrp::SetPTime
// Set PTime
// -----------------------------------------------------------------------------
//
TInt CMccCodecMsrp::SetPTime( TUint /*aPTime*/ )
    {
    // Not supported
    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// CMccCodecMsrp::SetMaxPTime
// Set Max PTime
// -----------------------------------------------------------------------------
//
TInt CMccCodecMsrp::SetMaxPTime( TUint /*aMaxPTime*/ )
    {
    // Not supported
    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// CMccCodecMsrp::CloneDefaultsL
// Make a default setting clone from this codec
// -----------------------------------------------------------------------------
//
CMccCodecInformation* CMccCodecMsrp::CloneDefaultsL()
    {
    return CMccCodecMsrp::NewL();
    }

// -----------------------------------------------------------------------------
// CMccCodecMsrp::CloneDetailedL
// Make a detailed clone from this codec
// -----------------------------------------------------------------------------
//
CMccCodecInformation* CMccCodecMsrp::CloneDetailedL()
    {
    CMccCodecMsrp* newCodec = CMccCodecMsrp::NewL();
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
