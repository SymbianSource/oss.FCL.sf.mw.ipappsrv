/*
* Copyright (c) 2004-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    MCC iLBC CodecInformation
*
*/




// INCLUDE FILES
#include "mmcccodecilbc.h"
#include "mmccinterfacelogs.h"
#include "mccuids.hrh"

// LOCAL CONSTANTS AND MACROS
_LIT8( KIlbcMode20ms, "mode=20" );
_LIT8( KIlbcMode30ms, "mode=30" );

const TUint8 KiLBCKAPayloadSize20ms = 38;
const TUint8 KiLBCKeepAlivePayload20ms[KiLBCKAPayloadSize20ms] = 
    {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01
    };

const TUint8 KiLBCKAPayloadSize30ms = 50;
const TUint8 KiLBCKeepAlivePayload30ms[KiLBCKAPayloadSize30ms] = 
    {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01
    };

// Mask for 'allow all' bitrates. (defined in header)
const TUint KIlbcAllowAllBitrates = 0x00FF;

// ============================= LOCAL FUNCTIONS ===============================


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMCCCodecILBC::CMCCCodecILBC
// default constructor
// -----------------------------------------------------------------------------
//
CMCCCodecILBC::CMCCCodecILBC() : CMccCodecInformation()
    {
    iBitrateMask = KIlbcAllowAllBitrates;
    }

// -----------------------------------------------------------------------------
// CMCCCodecILBC::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMCCCodecILBC::ConstructL()
    {
    __INTERFACE( "CMCCCodecILBC::ConstructL()" )
     
    SetSdpName( KILBCSdpName );
    iKeepAliveData.Copy( KiLBCKeepAlivePayload30ms ); 
    
    SetSamplingFreq( KIlbcSampleRate );
    iHwFrameTime = KIlbcHwFrameTime; // default 30ms    
    SetBitrate( KIlbcBitrate30 ); // 30ms framesize used as default
    EnableVAD( EFalse );
    SetPayloadType( KDefaultIlbcPT );
    SetMaxPTime( KIlbcMaxPTime );
    SetPTime( KIlbcPTime );
        
    iFourCC = KMccFourCCIdILBC;
    iCodecMode = ENothing;
    
    iPayloadFormatEncoder = KImplUidiLBCPayloadFormatEncode;
    iPayloadFormatDecoder = KImplUidiLBCPayloadFormatDecode;
    }


// -----------------------------------------------------------------------------
// CMCCCodecILBC::NewL
// Static constructor.
// -----------------------------------------------------------------------------
//
CMCCCodecILBC* CMCCCodecILBC::NewL()
    {
    CMCCCodecILBC* self = new (ELeave) CMCCCodecILBC;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self ); 
    return self;
    }


// -----------------------------------------------------------------------------
// CMCCCodecILBC::~CMCCCodecILBC
// Destructor
// -----------------------------------------------------------------------------
//
CMCCCodecILBC::~CMCCCodecILBC()
    {
    __INTERFACE( "CMCCCodecILBC::~CMCCCodecILBC()" )
    }
    
// -----------------------------------------------------------------------------
// CMCCCodecILBC::EnableVAD
// Enable / Disable VAD
// -----------------------------------------------------------------------------
//
TInt CMCCCodecILBC::EnableVAD( TBool aEnableVAD )
    {
    iEnableVAD = aEnableVAD;
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CMCCCodecILBC::SetBitrate
// Set Bitrate
// -----------------------------------------------------------------------------
//
TInt CMCCCodecILBC::SetBitrate( TUint aBitrate )
    {
    __INTERFACE_INT1( "CMCCCodecILBC::SetBitrate aBitrate", aBitrate )
    __INTERFACE_INT1( "CMCCCodecILBC::SetBitrate iBitrate", iBitrate )
    
    TBool isValid( ETrue );
    
    switch ( aBitrate )
        {
        case KIlbcBitrate20:
            iHwFrameTime = KIlbcPTime20; // 20ms frame  
            iKeepAliveData.Copy( KiLBCKeepAlivePayload20ms );
            break;
        case KIlbcBitrate30:
            iHwFrameTime = KIlbcPTime30; // 30ms frame
            iKeepAliveData.Copy( KiLBCKeepAlivePayload30ms );
            break;
        default:
            isValid = EFalse;
            break;
        }
    
    if ( isValid )
        {
        __INTERFACE( "CMCCCodecILBC::SetBitrate valid, adjusting" )
        
        iBitrate = aBitrate;
        AdjustCodec();
        return KErrNone;
        }
    else
        {
        __INTERFACE( "CMCCCodecILBC::SetBitrate KErrNotSupported" )
        
        return KErrNotSupported;
        }
    }

// -----------------------------------------------------------------------------
// CMCCCodecILBC::SetSamplingFreq
// Set Sampling Frequency
// -----------------------------------------------------------------------------
//
TInt CMCCCodecILBC::SetSamplingFreq( TUint32 aSamplingFreq )
    {
    if ( KIlbcSampleRate == aSamplingFreq )
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
// CMCCCodecILBC::SetSdpName
// Set SDP Name
// -----------------------------------------------------------------------------
//
TInt CMCCCodecILBC::SetSdpName( const TDesC8& aSdpName )
    {
    if ( !aSdpName.CompareF( KILBCSdpName ) )
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
// CMCCCodecILBC::SetPayloadType
// Set PayloadType
// -----------------------------------------------------------------------------
//
TInt CMCCCodecILBC::SetPayloadType( TUint8 aPayloadType )
    {
    if ( aPayloadType > KMaxPayloadType || aPayloadType < KMinDynamicPT )
        {
        return KErrArgument;
        }
    else 
        {
        iPayloadType = aPayloadType;
        return KErrNone;
        }
    }
 
// -----------------------------------------------------------------------------
// CMCCCodecILBC::SetCodecMode
// Set Codec Mode
// -----------------------------------------------------------------------------
//   
TInt CMCCCodecILBC::SetCodecMode( TCodecMode aCodecMode )
    {
    __INTERFACE_INT1( "CMCCCodecILBC::SetCodecMode aCodecMode", aCodecMode )
    
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
// CMCCCodecILBC::CreateFmtpAttrListL
// Builds fmtp line according to current parameters.
// -----------------------------------------------------------------------------
//
void CMCCCodecILBC::CreateFmtpAttrListL()
    {
    HBufC8* buf = HBufC8::NewLC( KMaxFmtpLen ); 
    TPtr8 bufPtr = buf->Des();

    // By default there is no need to construct the 30ms fmtp line. So
    // let's do it only in 20ms mode.
    if ( KIlbcBitrate20 == iBitrate )
        {
        bufPtr.Append( KIlbcMode20ms );
        }
    else
        {
        bufPtr.Append( KNullDesC8 );
        }
     
    // Update iFmtpAttr
    SetFmtpAttrL( bufPtr, EFalse );
    
    CleanupStack::PopAndDestroy( buf );
    }
    
// -----------------------------------------------------------------------------
// CMCCCodecILBC::ParseFmtpAttrL
// Parse FMTP attribute
// -----------------------------------------------------------------------------
//
TBool CMCCCodecILBC::ParseFmtpAttrL( const TDesC8& aFmtp )
    {
    __INTERFACE( "CMCCCodecILBC::ParseFmtpAttrL, Entry" )
    
    TPtrC8 curToken;
    TLex8 fmtpLex( aFmtp );
    TBool updated( EFalse );
    
    curToken.Set( fmtpLex.NextToken() );

    // Set bitrate according to mode indicated with fmtp attribute
    if ( !curToken.Length() ||
         KErrNotFound != curToken.Find( KIlbcMode30ms ) )
        {
        SetBitrate( KIlbcBitrate30 );
        AdjustCodec();
        updated = ETrue;    
        }
    else if ( KErrNotFound != curToken.Find( KIlbcMode20ms ) )
        {
        SetBitrate( KIlbcBitrate20 );
        AdjustCodec();
        updated = ETrue;    
        }
    else
        {
        __INTERFACE( "CMCCCodecILBC::ParseFmtpAttrL, KErrArgument" )
        
        User::Leave( KErrArgument );
        }
    
    __INTERFACE_INT1( "CMCCCodecILBC::ParseFmtpAttrL, Exit updated", updated )
    
    return updated;
    }
   
// -----------------------------------------------------------------------------
// CMCCCodecILBC::GetFmtpL
// Gets the fmtp attribute
// -----------------------------------------------------------------------------
//
TDesC8& CMCCCodecILBC::GetFmtpL()
    {
    __INTERFACE( "CMCCCodecILBC::GetFmtpL" )
    
    if ( iFmtpAttr )
        {
        delete iFmtpAttr;
        iFmtpAttr = NULL;
        }
    
    CreateFmtpAttrListL();
    
    __INTERFACE( "CMCCCodecILBC::GetFmtpL, exit" )
    
    return *iFmtpAttr;
    }    

// -----------------------------------------------------------------------------
// CMCCCodecILBC::SetPTime
// Sets the packet time
// -----------------------------------------------------------------------------
//
TInt CMCCCodecILBC::SetPTime( TUint aPTime )
    {
    __INTERFACE_INT1( "CMCCCodecILBC::SetPTime, Entry aPTime", aPTime )
    __INTERFACE_INT1( "CMCCCodecILBC::SetPTime, Entry iPTime", iPTime )
        
    if ( iMaxPTime < aPTime )
        {
        __INTERFACE( "CMCCCodecILBC::SetPTime, KErrArgument 1" )
        
        return KErrArgument;
        }      
    else if( KMinPtime > aPTime )
        {
        __INTERFACE( "CMCCCodecILBC::SetPTime, KErrArgument 2" )
        
        return KErrArgument;
        }  
    else
        {
        if ( aPTime == KIlbcPTime20 || aPTime == KIlbcPTime30 )
            {
            iPTime = aPTime;
            
            // Do not allow ptime vs. bitrate mismatch
            AdjustCodec();
            
            return KErrNone;
            }
        else if( ( KIlbcBitrate20 == iBitrate ) &&
            ( aPTime % KDefaultPtime == 0 ) )
            {
            iPTime = aPTime;
            TInt mod = iMaxPTime % KDefaultPtime;
            while( mod != 0 )
                {
                iMaxPTime = iMaxPTime - KMinPtime;
                mod = iMaxPTime % KDefaultPtime;
                }
            
            if( iMaxPTime == 0 )
                {
                iMaxPTime = KMaxPtime;
                }
                
            return KErrNone;
            }
        else if( ( KIlbcBitrate30 == iBitrate ) &&
            ( aPTime % KIlbcPTime30 == 0 ) )
            {
            iPTime = aPTime;
            TInt mod = iMaxPTime % KIlbcPTime30;
            while( mod != 0 )
                {
                iMaxPTime = iMaxPTime - KMinPtime;
                mod = iMaxPTime % KIlbcPTime30;
                }
            
            if( iMaxPTime == 0 )
                {
                iMaxPTime = KIlbcMaxPTime30;
                }
                
            return KErrNone;
            }
        else
            {
            __INTERFACE( "CMCCCodecILBC::SetPTime, KErrArgument 3" )
            
            return KErrArgument;
            }
        }
    }

// -----------------------------------------------------------------------------
// CMCCCodecILBC::SetMaxPTime
// Set Max PTime
// -----------------------------------------------------------------------------
//
TInt CMCCCodecILBC::SetMaxPTime( TUint aMaxPTime )
    {
    __INTERFACE_INT1( "CMCCCodecILBC::SetMaxPTime, Entry aMaxPTime", aMaxPTime )
    __INTERFACE_INT1( "CMCCCodecILBC::SetMaxPTime, Entry iMaxPTime", iMaxPTime )
    
    if ( aMaxPTime < iPTime )
        {
        __INTERFACE( "CMCCCodecILBC::SetMaxPTime, KErrArgument 1" )
        
        return KErrArgument;
        }
    else
        {
        if ( aMaxPTime && ( KIlbcBitrate20 == iBitrate ) &&
            ( aMaxPTime % KDefaultPtime == 0 ) )
            {
            if ( KMaxPtime < aMaxPTime )
                {
                __INTERFACE( "CMCCCodecILBC::SetMaxPTime, KErrArgument 2" )
                
                return KErrArgument;
                }
            else
                {
                iMaxPTime = aMaxPTime;
                return KErrNone;
                }
            }
        else if ( aMaxPTime && ( KIlbcBitrate30 == iBitrate ) &&
            ( aMaxPTime % KIlbcPTime30 == 0 ) )
            {
            if ( KIlbcMaxPTime30 < aMaxPTime )
                {
                __INTERFACE( "CMCCCodecILBC::SetMaxPTime, KErrArgument 3" )
                
                return KErrArgument;
                }
            else
                {
                iMaxPTime = aMaxPTime;
                return KErrNone;
                }
            }
        else
            {
            __INTERFACE( "CMCCCodecILBC::SetMaxPTime, KErrArgument 4" )
            
            return KErrArgument;
            }
        }
    }

// -----------------------------------------------------------------------------
// CMCCCodecILBC::CloneDefaultsL
// Make a default setting clone from this iLBC codec
// -----------------------------------------------------------------------------
//
CMccCodecInformation* CMCCCodecILBC::CloneDefaultsL()
    {
    return CMCCCodecILBC::NewL();
    };

// -----------------------------------------------------------------------------
// CMCCCodecILBC::CloneDetailedL
// Make a detailed clone from this iLBC codec
// -----------------------------------------------------------------------------
//
CMccCodecInformation* CMCCCodecILBC::CloneDetailedL()
    {
    CMCCCodecILBC* newCodec = CMCCCodecILBC::NewL();
    CleanupStack::PushL( newCodec );            
    newCodec->SetBitrate( this->Bitrate() );
    newCodec->SetCodecMode( this->CodecMode() );
    newCodec->SetMaxPTime( this->MaxPTime() );
    newCodec->SetPayloadType( this->PayloadType() );
    newCodec->SetPTime( this->PTime() );
    newCodec->SetSamplingFreq( this->SamplingFreq() );
    newCodec->SetSdpName( this->SdpName() );
    
    TDesC8& fmtp( this->GetFmtpL() );
    if ( fmtp.Length() )
        {
        newCodec->ParseFmtpAttrL( this->GetFmtpL() );
        }

    CleanupStack::Pop( newCodec );                
    return newCodec;
    };

// -----------------------------------------------------------------------------
// CMCCCodecILBC::RequireSignalling
// -----------------------------------------------------------------------------
//
TBool CMCCCodecILBC::RequireSignalling(
    const CMccCodecInformation& aCandidate ) const      
    {
    __INTERFACE( "CMCCCodecILBC::RequireSignalling" )
    TBool ret = EFalse;
              
    if ( iPTime != aCandidate.PTime() )
        {
        __INTERFACE( "CMCCCodecILBC::RequireSignalling, PTime changed" )
        ret = ETrue;
        }
        
    if ( iMaxPTime != aCandidate.MaxPTime() )
        {
        __INTERFACE( "CMCCCodecILBC::RequireSignalling, MaxPTime changed" )
        ret = ETrue;
        }
        
    if ( iPayloadType != aCandidate.PayloadType() )
        {
        __INTERFACE( "CMCCCodecILBC::RequireSignalling, PayloadType changed" )
        ret = ETrue;
        }

    if ( iBitrate != aCandidate.Bitrate() )
        {
        __INTERFACE( "CMCCCodecILBC::RequireSignalling, Bitrate changed" )
        ret = ETrue;
        }

    if ( iCodecMode != aCandidate.CodecMode() )
        {
        __INTERFACE( "CMCCCodecILBC::RequireSignalling, CodecMode changed" )
        ret = ETrue;
        }
        
    __INTERFACE_INT1( "CMCCCodecILBC::RequireSignalling, exit with", ret )
    return ret;          
    }
    
// -----------------------------------------------------------------------------
// CMCCCodecILBC::SetComfortNoiseGeneration
// -----------------------------------------------------------------------------
// 
TInt CMCCCodecILBC::SetComfortNoiseGeneration( TUint8 aComfortNoisePT )
    {
    iComfortNoiseGenerationPt = aComfortNoisePT;
    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// CMCCCodecILBC::SetAllowedBitrates
// -----------------------------------------------------------------------------
// 

TInt CMCCCodecILBC::SetAllowedBitrates( TUint /*aBitrateMask*/ )
    {
    // iLBC does not really need the concept of 'allowed' bitrates because
    // the bitrate setting handles assertion for bitrate correctness.
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMCCCodecILBC::AdjustCodec
// -----------------------------------------------------------------------------
//     
void CMCCCodecILBC::AdjustCodec()
    {
    __INTERFACE_INT1( "CMCCCodecILBC::AdjustCodec Entry iPTime",
        iPTime )
    __INTERFACE_INT1( "CMCCCodecILBC::AdjustCodec Entry iHwFrameTime",
        iHwFrameTime )
    
    // If default ptime is in use, it is adjusted based on mode of the codec
    if ( iPTime == KIlbcPTime20 || iPTime == KIlbcPTime30 || !iPTime )
        {
        iPTime = ( KIlbcBitrate20 == iBitrate ) ? KIlbcPTime20 : KIlbcPTime30;
        }
    else if ( iPTime )
        {
        __INTERFACE( "CMCCCodecILBC::AdjustCodec ptime to closest possible" )
        
        // Default ptime is not in use, we must adjust it to closest possible
        // according to current iHwFrameTime (bitrate specific).
        TInt ptmul = (TInt) iPTime / (TInt) iHwFrameTime;
        iPTime = ptmul *
            ( ( KIlbcBitrate20 == iBitrate ) ? KIlbcPTime20 : KIlbcPTime30 );
        }
    
    __INTERFACE_INT1( "CMCCCodecILBC::AdjustCodec Exit iPTime",
        iPTime )
    }
    
// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File
