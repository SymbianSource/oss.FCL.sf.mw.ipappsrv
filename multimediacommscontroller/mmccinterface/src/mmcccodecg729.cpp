/*
* Copyright (c) 2004-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    MCC G.729 CodecInformation
*
*/




// INCLUDE FILES
#include "mmcccodecg729.h"
#include "mmccinterfacelogs.h"
#include "mccuids.hrh"


// CONSTANTS
const TInt KG729DefaultHwFrameTime( 10 );


// LOCAL CONSTANTS AND MACROS
const TUint8 KG729KAPayloadSize = 10;
const TUint8 KG729KeepAlivePayload[KG729KAPayloadSize] = 
    { 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    
// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CMCCCodecG729::CMCCCodecG729
// default constructor
// -----------------------------------------------------------------------------
//
CMCCCodecG729::CMCCCodecG729() : CMccCodecInformation()
    {
    }

// -----------------------------------------------------------------------------
// CMCCCodecG729::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMCCCodecG729::ConstructL()
    {
    __INTERFACE( "CMCCCodecG729::ConstructL" )
    
    SetSdpName( KG729SdpName );
    iKeepAliveData.Copy( KG729KeepAlivePayload );   
    // setting some default values 
    SetSamplingFreq( KG729SampleRate );
    SetBitrate( KG729Bitrate );
    EnableVAD( EFalse );
    iHwFrameTime = KG729DefaultHwFrameTime; // default 20ms    
    SetMaxPTime( KMaxPtime );
    SetPTime( KDefaultPtime );
    SetPayloadType( KG729PayloadType );
    iCodecMode = ENothing;
      
    iFourCC = KMccFourCCIdG729;

    iPayloadFormatEncoder = KImplUidG729PayloadFormatEncode;
    iPayloadFormatDecoder = KImplUidG729PayloadFormatDecode;
    }

// -----------------------------------------------------------------------------
// CMCCCodecG729::NewL
// Static constructor.
// -----------------------------------------------------------------------------
//
CMCCCodecG729* CMCCCodecG729::NewL()
    {
    CMCCCodecG729* self = new (ELeave) CMCCCodecG729;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self ); 
    return self;
    }

// -----------------------------------------------------------------------------
// CMCCCodecG729::~CMCCCodecG729
// Destructor
// -----------------------------------------------------------------------------
//
CMCCCodecG729::~CMCCCodecG729()
    {
    __INTERFACE( "CMCCCodecG729::~CMCCCodecG729" )
    }
    
// -----------------------------------------------------------------------------
// CMCCCodecG729::EnableVAD
// Enable / Disable VAD
// -----------------------------------------------------------------------------
//
TInt CMCCCodecG729::EnableVAD( TBool aEnableVAD )
    {
    iEnableVAD = aEnableVAD;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMCCCodecG729::SetBitrate
// Set Bitrate
// -----------------------------------------------------------------------------
//
TInt CMCCCodecG729::SetBitrate( TUint aBitrate )
    {
    if( KG729Bitrate != aBitrate )
        {
        return KErrNotSupported;
        }
    else
        {
        iBitrate = aBitrate;
        return KErrNone;
        }
    }

// -----------------------------------------------------------------------------
// CMCCCodecG729::SetSamplingFreq
// Set Sampling Frequency 
// -----------------------------------------------------------------------------
//    
TInt CMCCCodecG729::SetSamplingFreq( TUint32 aSamplingFreq )
    {
    if ( KG729SampleRate == aSamplingFreq )
        {
        iSamplingFreq = aSamplingFreq;
        return KErrNone;
        }
    else
        {
        return KErrNotSupported;
        }
    }

// -----------------------------------------------------------------------------
// CMCCCodecG729::SetSdpName
// Set SDP Name
// -----------------------------------------------------------------------------
//
TInt CMCCCodecG729::SetSdpName( const TDesC8& aSdpName )
    {
    if ( !aSdpName.CompareF( KG729SdpName ) )
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
// CMCCCodecG729::SetPayloadType
// Set PayloadType 
// -----------------------------------------------------------------------------
//
TInt CMCCCodecG729::SetPayloadType( TUint8 aPayloadType )
    {
    if ( KG729PayloadType != aPayloadType ) 
        {
        return KErrNotSupported;
        }
    else 
        {
        iPayloadType = aPayloadType;
        return KErrNone;
        }
    }
 
// -----------------------------------------------------------------------------
// CMCCCodecG729::SetPTime
// Set PacketTime
// -----------------------------------------------------------------------------
//
TInt CMCCCodecG729::SetPTime( TUint aPTime )
    {
    __INTERFACE_INT2( "CMCCCodecG729::SetPTime [aPTime, iPTime]",
        aPTime, iPTime )
    
    if ( aPTime && ( KMinPtime <= aPTime ) && 
         ( aPTime <= KMaxPtime ) && 
         ( aPTime <= iMaxPTime ) && 
         !( aPTime % KMinPtime ) )
        {
        iPTime = aPTime;
        return KErrNone;
        }
    else
        {
        return KErrArgument;
        }
    }

// -----------------------------------------------------------------------------
// CMCCCodecG729::SetMaxPTime
// Set Maximum PacketTime
// -----------------------------------------------------------------------------
//
TInt CMCCCodecG729::SetMaxPTime( TUint aMaxPTime )
    {
    __INTERFACE_INT2( "CMCCCodecG729::SetMaxPTime [aMaxPTime, iMaxPTime]",
        aMaxPTime, iMaxPTime )
    
    if ( aMaxPTime && ( iPTime <= aMaxPTime ) && 
         ( KMinPtime <= aMaxPTime ) && 
         ( aMaxPTime <= KMaxPtime ) && 
         !( aMaxPTime % KMinPtime ) )
        {
        iMaxPTime = aMaxPTime;
        return KErrNone;
        }
    else
        {
        return KErrArgument;
        }
    }

// -----------------------------------------------------------------------------
// CMCCCodecG729::SetCodecMode
// Set Codec Mode
// -----------------------------------------------------------------------------
//   
TInt CMCCCodecG729::SetCodecMode( TCodecMode aCodecMode )
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
// CMCCCodecG729::CloneDefaultsL
// Make a default setting clone from this G.729 codec
// -----------------------------------------------------------------------------
//
CMccCodecInformation* CMCCCodecG729::CloneDefaultsL()
    {
    return CMCCCodecG729::NewL();
    };

// -----------------------------------------------------------------------------
// CMCCCodecG729::CloneDetailedL
// Make a detailed clone from this G.729 codec
// -----------------------------------------------------------------------------
//
CMccCodecInformation* CMCCCodecG729::CloneDetailedL()
    {
    CMCCCodecG729* newCodec = CMCCCodecG729::NewL();
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
    };

// -----------------------------------------------------------------------------
// CMCCCodecG729::RequireSignalling
// -----------------------------------------------------------------------------
//
TBool CMCCCodecG729::RequireSignalling(
    const CMccCodecInformation& aCandidate ) const      
    {
    __INTERFACE( "CMCCCodecG729::RequireSignalling" )
    TBool ret = EFalse;
              
    if ( iPTime != aCandidate.PTime() )
        {
        __INTERFACE( "CMCCCodecG729::RequireSignalling, PTime changed" )
        ret = ETrue;
        }
        
    if ( iMaxPTime != aCandidate.MaxPTime() )
        {
        __INTERFACE( "CMCCCodecG729::RequireSignalling, MaxPTime changed" )
        ret = ETrue;
        }
        
    if ( iPayloadType != aCandidate.PayloadType() )
        {
        __INTERFACE( "CMCCCodecG729::RequireSignalling, PayloadType changed" )
        ret = ETrue;
        }
        
    __INTERFACE_INT1( "CMCCCodecG729::RequireSignalling, exit with", ret )
    return ret;          
    }
    
// -----------------------------------------------------------------------------
// CMCCCodecG729::CreateFmtpAttrListL
// Builds fmtp line according to current parameters.
// -----------------------------------------------------------------------------
//
void CMCCCodecG729::CreateFmtpAttrListL()
    {
    HBufC8* buf = HBufC8::NewLC( KMaxFmtpLen ); 
    TPtr8 bufPtr = buf->Des();
    
    if ( iEnableVAD )
        {
        bufPtr.Append( KAnnexYes );
        }
    else
        {
        bufPtr.Append( KAnnexNo );
        }
     
    // Update iFmtpAttr
    SetFmtpAttrL( bufPtr, EFalse );
    
    CleanupStack::PopAndDestroy( buf );
    }
   
// -----------------------------------------------------------------------------
// CMccCodecInformation::ParseFmtpAttrL
// Parse the fmtp string, but doesn't update the iFmtpAttr
// -----------------------------------------------------------------------------
//
TBool CMCCCodecG729::ParseFmtpAttrL( const TDesC8& aFmtp )
    {
    TBool updated( EFalse );
    
    if ( !aFmtp.CompareF( KAnnexYes ) )
        {
        iEnableVAD = ETrue;
        updated = ETrue;
        }
    else if ( !aFmtp.CompareF( KAnnexNo ) || !aFmtp.Length() )
        {
        iEnableVAD = EFalse;
        updated = ETrue;
        }
    else
        {
        User::Leave( KErrArgument );
        }
        
    return updated;
    }

// -----------------------------------------------------------------------------
// CMCCCodecG729::GetFmtpL
// Gets the fmtp attribute
// -----------------------------------------------------------------------------
//
TDesC8& CMCCCodecG729::GetFmtpL()
    {
    __INTERFACE( "CMCCCodecG729::GetFmtpL" )      
    if ( iFmtpAttr )
        {
        delete iFmtpAttr;
        iFmtpAttr = NULL;
        }
    
    CreateFmtpAttrListL();    
    
    __INTERFACE( "CMccCodecG729::GetFmtpL, exit" )      
    return *iFmtpAttr;    
    }
    
// -----------------------------------------------------------------------------
// CMCCCodecG729::SetAllowedBitrates
// -----------------------------------------------------------------------------
// 

TInt CMCCCodecG729::SetAllowedBitrates( TUint /*aBitrateMask*/ )
    {
    return KErrNone;           
    }
// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File
