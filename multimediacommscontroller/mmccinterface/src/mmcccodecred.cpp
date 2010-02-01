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
* Description:    MCC Redundancy CodecInformation
*
*/




// INCLUDE FILES
#include "mmcccodecred.h"
#include "mccuids.hrh"
#include "mmccinterfacelogs.h"
#include "mccinternalcodecs.h"


// ============================= LOCAL FUNCTIONS ===============================


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccCodecRed::CMccCodecRed
// default constructor
// -----------------------------------------------------------------------------
//
CMccCodecRed::CMccCodecRed() : CMccCodecInformation()
    {
    
    }

// -----------------------------------------------------------------------------
// CMccCodecRed::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMccCodecRed::ConstructL()
    {
    iSdpName.Copy( KRedSdpName );

    iFmtpAttr = HBufC8::NewL( 1 );
    TPtr8 ptr = iFmtpAttr->Des();
    ptr.Append( KNullDesC );
    
    SetSamplingFreq( KRedSamplingFreq );
    EnableVAD( EFalse );
    SetMaxPTime( KRedMaxPTime ); // recommended "limit" 200ms
    SetPTime( KRedPTime ); // default 20ms 
    SetPayloadType( KDefaultRedPT );  
    
    iFourCC = KMccFourCCIdRed;
    iCodecMode = ENothing;
    
    iPayloadFormatEncoder = KImplUidRedPayloadFormatEncode;
    iPayloadFormatDecoder = KImplUidRedPayloadFormatDecode;
    }

// -----------------------------------------------------------------------------
// CMccCodecRed::NewLC
// Static constructor.
// -----------------------------------------------------------------------------
//
CMccCodecRed* CMccCodecRed::NewL()
    {
    CMccCodecRed* self = new (ELeave) CMccCodecRed;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self ); 
    return self;
    }

// -----------------------------------------------------------------------------
// CMccCodecRed::~CMccCodecRed
// Destructor
// -----------------------------------------------------------------------------   
//
CMccCodecRed::~CMccCodecRed()
    {
    iRedPayloads.Reset();
    iRedPayloads.Close();
    }

// -----------------------------------------------------------------------------
// CMccCodecRed::RequireSignalling
// -----------------------------------------------------------------------------
//
TBool CMccCodecRed::RequireSignalling(
    const CMccCodecInformation& aCandidate ) const      
    {
    __INTERFACE( "CMccCodecRed::RequireSignalling" )
    
    const CMccCodecRed& candidate = static_cast<const CMccCodecRed&>( aCandidate );
    
    TBool ret = EFalse;
    
    if ( iPTime != candidate.PTime() )
        {
        __INTERFACE( "CMccCodecRed::RequireSignalling, PTime changed" )
        ret = ETrue;
        }
        
    if ( iMaxPTime != candidate.MaxPTime() )
        {
        __INTERFACE( "CMccCodecRed::RequireSignalling, MaxPTime changed" )
        ret = ETrue;
        }
        
    if ( iPayloadType != candidate.PayloadType() )
        {
        __INTERFACE( "CMccCodecRed::RequireSignalling, PayloadType changed" )
        ret = ETrue;
        }

    if ( iCodecMode != candidate.CodecMode() )
        {
        __INTERFACE( "CMccCodecRed::RequireSignalling, CodecMode changed" )
        ret = ETrue;
        }     
    
    const RArray<TUint>& candidatePayloads = candidate.RedPayloads();    
    
    if ( candidatePayloads.Count() != iRedPayloads.Count() )
        {
        __INTERFACE( "CMccCodecRed::RequireSignalling, RedPayloads changed" )
        ret = ETrue;
        }
    else
        {
        for ( TInt i = 0; i < candidatePayloads.Count(); i++ )
            {
            if ( candidatePayloads[ i ] != iRedPayloads[ i ] )
                {
                __INTERFACE( "CMccCodecRed::RequireSignalling, RedPayload changed" )
                ret = ETrue;
                }
            }
        }
        
    __INTERFACE_INT1( "CMccCodecRed::RequireSignalling, exit with", ret )
    return ret;          
    }

// -----------------------------------------------------------------------------
// CMccCodecRed::SetBitrate
// Sets the bitrate used with Redundacy codec.
// -----------------------------------------------------------------------------
//
TInt CMccCodecRed::SetBitrate( TUint aBitrate )
    {
    iBitrate = aBitrate;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccCodecRed::SetSamplingFreq
// Sets the sampling frequency. 
// -----------------------------------------------------------------------------
//
TInt CMccCodecRed::SetSamplingFreq( TUint32 aSamplingFreq )
    {
    iSamplingFreq = aSamplingFreq;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccCodecRed::SetSdpName
// Sets the SDP name
// -----------------------------------------------------------------------------
TInt CMccCodecRed::SetSdpName( const TDesC8& aSdpName )
    {
    if ( !aSdpName.CompareF( KRedSdpName ) )
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
// CMccCodecRed::PayloadType
// Sets the payload type
// -----------------------------------------------------------------------------
TInt CMccCodecRed::SetPayloadType( TUint8 aPayloadType )
    {
    if ( aPayloadType < KMinDynamicPT ||
         aPayloadType > KMaxPayloadType ) 
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
// CMccCodecRed::SetCodecMode
// Sets the codec mode
// -----------------------------------------------------------------------------
TInt CMccCodecRed::SetCodecMode( TCodecMode aCodecMode )
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
// CMccCodecRed::EnableVAD
// 
// -----------------------------------------------------------------------------
TInt CMccCodecRed::EnableVAD( TBool aEnableVAD ) 
    {
    iEnableVAD = aEnableVAD;        
    return KErrNone;
    }
    

// -----------------------------------------------------------------------------
// CMccCodecRed::SetPTime
//
// -----------------------------------------------------------------------------
//
TInt CMccCodecRed::SetPTime( TUint aPTime )
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
// CMccCodecRed::SetMaxPTime
//
// -----------------------------------------------------------------------------
//
TInt CMccCodecRed::SetMaxPTime( TUint aMaxPTime )
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
// CMccCodecInformation::ParseFmtpAttr
// Parses the fmtp attribute
// -----------------------------------------------------------------------------
//
TBool CMccCodecRed::ParseFmtpAttrL( const TDesC8& aFmtp )
    {
    // Parse codecs used for primary and secondary etc. codecs from the 
    // fmtp attribute
    // example fmtp attribute:
    // a=fmtp:99 0/103 
    // 99 is the redundancy format payload type
    // 0 is the primary codecs payload type
    // 103 is the secondary codecs payload type
    
    // NOTE: currently redundancy is allowed only for single payload type
    
    TBool updated( EFalse );
    
    TInt nextIndex = 0;
    TInt prevIndex = 0;
    TUint8 prevPayloadFormat = KMccPayloadTypeMax;
    HBufC8* modifyBuf = HBufC8::NewLC( aFmtp.Length() );
    TPtr8 pFmtpValue( modifyBuf->Des() );
    pFmtpValue = aFmtp;
    
    if( aFmtp.Length() > 0 )
        {   
        // Search for all the slashes and convert number strings between them
        // to integers    
        TBool found = ETrue;
        while( EFalse != found )
            {     
            pFmtpValue = modifyBuf->Mid( nextIndex );
            nextIndex = pFmtpValue.Locate( KCharSlash );          
            if( ( KErrNotFound == nextIndex ) ) 
                {
                found = EFalse;
                if( 0 != pFmtpValue.Length() )
                    {
                    nextIndex = pFmtpValue.Length(); // Handle last payload value      
                    }
                }
            if( 0 != pFmtpValue.Length() )
                {
                TLex8 lex = pFmtpValue.Mid( prevIndex, (nextIndex - prevIndex) );
                TUint8 payloadFormat;
                TInt err = lex.Val( payloadFormat, EDecimal );
                if( KErrNone == err )
                    {
                    if ( prevPayloadFormat != KMccPayloadTypeMax &&
                         prevPayloadFormat != payloadFormat )
                         {
                         iRedPayloads.Reset();
                         User::Leave( KErrNotSupported );
                         }
                                     
                    iRedPayloads.AppendL( static_cast<TUint>( payloadFormat ) );  
                    updated = ETrue;  
                    prevPayloadFormat = payloadFormat;  
                    }
                    
                nextIndex++;         
                prevIndex = 0;
                }
            }
        }
    
    CleanupStack::PopAndDestroy( modifyBuf );
    
    return updated;
    }

// -----------------------------------------------------------------------------
// CMccCodecRed::SetAllowedBitrates
// -----------------------------------------------------------------------------
// 
TInt CMccCodecRed::SetAllowedBitrates( TUint /*aBitrateMask*/ )
    {
    return KErrNone;           
    }
        
// -----------------------------------------------------------------------------
// CMccCodecRed::SetRedCodecs
// Set the payload types used in redundancy
// -----------------------------------------------------------------------------
//
void CMccCodecRed::SetRedPayloadsL( RArray<TUint>& aRedPayloads )
    {
    iRedPayloads.Reset();
    TInt i;
    for( i = 0; i < aRedPayloads.Count(); i++ )
        {
        iRedPayloads.AppendL( aRedPayloads[ i ] );  
        }
    
    // Convert parsed payload formats back to string and set
    // the iFmtpAttr variable
    const TInt KCharsPerPayload( 4 );
    delete iFmtpAttr;
    iFmtpAttr = NULL;
    iFmtpAttr = HBufC8::NewL( iRedPayloads.Count() * KCharsPerPayload );
    
    TPtr8 descPtr = iFmtpAttr->Des();
    for( i = 0; i < iRedPayloads.Count(); i++ )
        {
        descPtr.AppendNum( static_cast<TUint64>( iRedPayloads[i] ), EDecimal );
        descPtr.Append( KCharSlash );       
        }   
    
    // Remove the last slash character
    descPtr.SetLength( descPtr.Length() - 1 );  
    }

// -----------------------------------------------------------------------------
// CMccCodecRed::RedPayloadsL
// Fills array with red payload type values
// -----------------------------------------------------------------------------
//
void CMccCodecRed::RedPayloadsL( RArray<TUint>& aRedPayloads ) const
    {
    for ( TInt i = 0; i < iRedPayloads.Count(); i++ )
        {
        aRedPayloads.AppendL( iRedPayloads[ i ] );
        }
    }
    
// -----------------------------------------------------------------------------
// CMccCodecInformation::RedPayloads
// Return the reference to the redundancy codecs array
// -----------------------------------------------------------------------------
//
const RArray<TUint>& CMccCodecRed::RedPayloads() const
    {
    return iRedPayloads;
    }

// -----------------------------------------------------------------------------
// CMccCodecRed::CloneDefaultsL
// Make a default setting clone from this Redundancy codec
// -----------------------------------------------------------------------------
//
CMccCodecInformation* CMccCodecRed::CloneDefaultsL()
    {
    return CMccCodecRed::NewL();
    }

// -----------------------------------------------------------------------------
// CMCCAMRCodec::CloneDetailedL
// Make a detailed clone from this Redundancy codec
// -----------------------------------------------------------------------------
//
CMccCodecInformation* CMccCodecRed::CloneDetailedL()
    {
    CMccCodecRed* newCodec = CMccCodecRed::NewL();
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
