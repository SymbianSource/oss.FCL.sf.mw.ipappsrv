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
#include <delimitedpathsegment8.h>

#include "mmcccodech263.h"
#include "mccuids.hrh"
#include "mmccinterfacelogs.h"

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS
const TInt KMaxFmtpAttrLength( 50 );
const TInt KH263DefaultFrameSize( 3000 );
const TInt KH263DefaultMaxBitrate( 64000 );
const TInt KH263DefaultBitrate( 64000 );
const TInt KH263DefaultAllowedBitrate( 1 );
const TInt KH263DefaultFreq( 90000 );

const TUint KMccAllowedH263Bitrate1 = 0x0001;
const TUint KMccAllowedH263Bitrate2 = 0x0002;
const TUint KMccAllowedH263BitrateAll = 0x0003;

const TInt  KH263Bitrate64( 64000 );
const TInt  KH263Bitrate128( 128000 );


// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccCodecH263::CMccCodecH263
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMccCodecH263::CMccCodecH263() : CMccCodecInformation()
    {
    }

// -----------------------------------------------------------------------------
// CMccCodecH263::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMccCodecH263::ConstructL()
    {
    iSdpName.Copy( KH2632000SdpName );

    iType = KUidMediaTypeVideo; 
    iFmtpAttr = HBufC8::NewL( KMaxFmtpAttrLength );
    iFourCC = KMccFourCCIdH263;
    iSamplingFreq = KH263DefaultFreq;

    iPayloadFormatEncoder = KImplUidH263PayloadFormatEncode;
    iPayloadFormatDecoder = 0;
    
    SetCodecMode( KH263FormatModeNew );
    SetPTime( KH263PTime );
    SetPayloadType( KH263PayloadType );
    SetFramerate( KH263Framerate );
	SetLevelBasedBitrate( KH263DefaultBitrate );
    SetMaxBitrate( KH263DefaultMaxBitrate );
    SetAllowedBitrates( KH263DefaultAllowedBitrate );
    SetFrameHeight( KH263FrameHeight );
    SetFrameWidth( KH263FrameWidth );
    iFrameSize = KH263DefaultFrameSize;
    }

// -----------------------------------------------------------------------------
// CMccCodecH263::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMccCodecH263* CMccCodecH263::NewL()
    {
    CMccCodecH263* self = new( ELeave ) CMccCodecH263;
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// -----------------------------------------------------------------------------
// CMccCodecH263::~CMccCodecH263
// 
// -----------------------------------------------------------------------------
//
CMccCodecH263::~CMccCodecH263()
    {
    }

// -----------------------------------------------------------------------------
// CMccCodecH263::RequireSignalling
// -----------------------------------------------------------------------------
//
TBool CMccCodecH263::RequireSignalling(
    const CMccCodecInformation& aCandidate ) const      
    {
    __INTERFACE( "CMccCodecH263::RequireSignalling" )
    TBool ret = EFalse;
    
    if ( iPTime != aCandidate.PTime() )
        {
        __INTERFACE( "CMccCodecH263::RequireSignalling, PTime changed" )
        ret = ETrue;
        }
        
    if ( iMaxPTime != aCandidate.MaxPTime() )
        {
        __INTERFACE( "CMccCodecH263::RequireSignalling, MaxPTime changed" )
        ret = ETrue;
        }
        
    if ( iPayloadType != aCandidate.PayloadType() )
        {
        __INTERFACE( "CMccCodecH263::RequireSignalling, PayloadType changed" )
        ret = ETrue;
        }

    if ( iBitrateMask != aCandidate.AllowedBitrates() )
        {
        __INTERFACE( "CMccCodecH263::RequireSignalling, BitrateMask changed" )
        ret = ETrue;
        }

    if ( iCodecMode != aCandidate.CodecMode() )
        {
        __INTERFACE( "CMccCodecH263::RequireSignalling, CodecMode changed" )
        ret = ETrue;
        }
        
    if ( iSamplingFreq != aCandidate.SamplingFreq() )
        {
        __INTERFACE( "CMccCodecH263::RequireSignalling, SamplingFreq changed" )
        ret = ETrue;
        }     
        
    if ( iMaxBitrate != aCandidate.MaxBitrate() )
        {
        __INTERFACE( "CMccCodecH263::RequireSignalling, MaxBitrate changed" )
        ret = ETrue;
        }
        
    if ( iFramerate < aCandidate.Framerate() )
        {
        __INTERFACE( "CMccCodecH263::RequireSignalling, Framerate changed" )
        ret = ETrue;
        }
        
    if ( iVideoWidth != aCandidate.FrameWidth() )
        {
        __INTERFACE( "CMccCodecH263::RequireSignalling, FrameWidth changed" )
        ret = ETrue;
        }
    
    if ( iVideoHeight != aCandidate.FrameHeight() )
        {
        __INTERFACE( "CMccCodecH263::RequireSignalling, FrameHeight changed" )
        ret = ETrue;
        }
        
    __INTERFACE_INT1( "CMccCodecH263::RequireSignalling, exit with", ret )
    return ret;          
    }

// -----------------------------------------------------------------------------
// CMccCodecH263::SetBitrate
// Sets the bitrate used with AMR codec.
// -----------------------------------------------------------------------------
//
TInt CMccCodecH263::SetBitrate( TUint aBitrate )
	{
    if ( aBitrate <= KH263Bitrate128 )
    	{
    	iBitrate = aBitrate;
    	iBitrateSet = ETrue;
    	return KErrNone;
    	}
    else
    	{
    	return KErrNotSupported;
    	}
	}

// -----------------------------------------------------------------------------
// CMccCodecH263::SetSamplingFreq
// Sets the sampling frequency. 
// -----------------------------------------------------------------------------
//
TInt CMccCodecH263::SetSamplingFreq( TUint32 /*aSamplingFreq*/ )
    {
    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// CMccCodecH263::SetSdpName
// Sets the SDP name
// -----------------------------------------------------------------------------
//
TInt CMccCodecH263::SetSdpName( const TDesC8& aSdpName )
    {
    if ( !aSdpName.CompareF( KH263SdpName ) || 
         !aSdpName.CompareF( KH2632000SdpName ))
        {
        iSdpName.Copy( KH2632000SdpName );
        if ( iCodecMode != KH263FormatModeNew )
            {
            SetCodecMode( KH263FormatModeNew );    
            }
        }
    else if( !aSdpName.CompareF( KH2631998SdpName ))
        {
        iSdpName.Copy( KH2631998SdpName );
        if ( iCodecMode != KH263FormatModeOld )
            {
            SetCodecMode( KH263FormatModeOld );    
            }
        }
    else 
        {
        return KErrNotSupported;
        }
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccCodecH263::PayloadType
// Sets the payload type
// -----------------------------------------------------------------------------
//
TInt CMccCodecH263::SetPayloadType( TUint8 aPayloadType )
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
// CMccCodecH263::SetCodecMode
// Sets the codec mode.
// -----------------------------------------------------------------------------
//
TInt CMccCodecH263::SetCodecMode( TCodecMode aCodecMode )
    {
    iCodecMode = aCodecMode;
    
    if ( iCodecMode == KH263FormatModeNew &&
         iSdpName.CompareF( KH2632000SdpName ))
        {
        SetSdpName( KH2632000SdpName );    
        }
    if ( iCodecMode == KH263FormatModeOld &&
	     iSdpName.CompareF( KH2631998SdpName ))
        {
        SetSdpName( KH2631998SdpName );  
        
        // Ensure that default dynamic pt is not the same for two different
        // codec modes
        if ( iPayloadType == KH263PayloadType )
            {
            SetPayloadType( KH2631998PayloadType );
            }
        }
    
    return KErrNone;
    }
 
// -----------------------------------------------------------------------------
// CMccCodecH263::EnableVAD
// Enable / Disable VAD
// -----------------------------------------------------------------------------
//
TInt CMccCodecH263::EnableVAD( TBool /*aEnableVAD*/ ) 
    {
    return KErrNotSupported;
    }
    
// -----------------------------------------------------------------------------
// CMccCodecH263::SetPTime
// Set PTime
// -----------------------------------------------------------------------------
//
TInt CMccCodecH263::SetPTime( TUint aPTime )
    {
    if ( aPTime && ( aPTime <= 200 ) && (( aPTime % 10 )  == 0 ))
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
// CMccCodecH263::SetMaxPTime
// Set Max PTime
// -----------------------------------------------------------------------------
//
TInt CMccCodecH263::SetMaxPTime( TUint aMaxPTime )
    {
    if ( aMaxPTime && ( aMaxPTime <= 200 ) && (( aMaxPTime % 10 )  == 0 ))
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
// CMccCodecH263:SetConfigKeyL
// -----------------------------------------------------------------------------
// 
void CMccCodecH263::SetConfigKeyL( const TDesC8& /*aConfigKey*/ )
    {
    // NOP
    }
    
// -----------------------------------------------------------------------------
// CMccCodecH263::SetAllowedBitrates
// -----------------------------------------------------------------------------
//
TInt CMccCodecH263::SetAllowedBitrates( TUint aBitrateMask )
    {
    //Confirm that the bitrate mask is valid
    //I.e. after all the valid bitrates are set to zero the value should be zero
    if ( (aBitrateMask >> 8 ) > 0 )
        {
        return KErrArgument;
        }
    else
        {
        /*
        In the future the allowed bitrates will be fetched from the codec
        I.e. the KMccAllowedH263BitrateAll will be changed to the bitrates
        supported by the codec
        */
        iBitrateMask = KMccAllowedH263BitrateAll;
        iBitrateMask &= aBitrateMask;
        
        SetMaxBitrateFromBitrateMask( iBitrateMask );
        }
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccCodecH263::CloneDefaultsL
// Make a default setting clone from this codec
// -----------------------------------------------------------------------------
//
CMccCodecInformation* CMccCodecH263::CloneDefaultsL()
    {
    CMccCodecInformation* codec = CMccCodecH263::NewL();
    CleanupStack::PushL( codec );
    
    if ( 0 == this->SdpName().CompareF( KH2632000SdpName ) )
        {
        User::LeaveIfError( codec->SetCodecMode( KH263FormatModeNew ) );
        }
    else
        {
        User::LeaveIfError( codec->SetCodecMode( KH263FormatModeOld ) );
        }
    
    CleanupStack::Pop( codec );
    return codec;
    }

// -----------------------------------------------------------------------------
// CMccCodecH263::CloneDetailedL
// Make a detailed clone from this codec
// -----------------------------------------------------------------------------
//
CMccCodecInformation* CMccCodecH263::CloneDetailedL()
    {
    CMccCodecH263* newCodec = CMccCodecH263::NewL();
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
        
// -----------------------------------------------------------------------------
// CMccCodecH263::GetFmtpL
// Gets the fmtp attribute
// -----------------------------------------------------------------------------
//
TDesC8& CMccCodecH263::GetFmtpL()
    {
    __INTERFACE( "CMccCodecH263::GetFmtpL" )      
    if ( iFmtpAttr )
        {
        delete iFmtpAttr;
        iFmtpAttr = NULL;
        }

    TInt level = KH263Level10;
    
    if ( iMaxBitrate > KH263Level10Bitrate && 
         iMaxBitrate <= KH263Level45Bitrate )  
        {
        __INTERFACE( "CMccCodecH263::GetFmtpL, level set 45" )        
        level = KH263Level45;     
        }
        
    iFmtpAttr = HBufC8::NewL( KMaxFmtpAttrLength );
    iFmtpAttr->Des().Format( KH263FmtpLine, level );                
    
    __INTERFACE( "CMccCodecH263::GetFmtpL, exit" )      
    return *iFmtpAttr;    
    }

// -----------------------------------------------------------------------------
// CMccCodecH263::SetFmtpAttrL
// Gets the fmtp attribute
// -----------------------------------------------------------------------------
//
void CMccCodecH263::SetFmtpAttrL( const TDesC8& aFmtp, TBool /*parseFmtp*/ )
    {
    __INTERFACE( "CMccCodecH263::SetFmtpAttrL" )      
    if ( iFmtpAttr )
        {
        delete iFmtpAttr;
        iFmtpAttr = NULL;
        }

    iFmtpAttr = HBufC8::NewL( aFmtp.Length() );
    iFmtpAttr->Des().Copy( aFmtp );
           
    ParseFmtpAttrL( *iFmtpAttr );          
    __INTERFACE( "CMccCodecH263::SetFmtpAttrL, exit" )      
    }        

 // -----------------------------------------------------------------------------
// CMccCodecH263::ParseFmtpAttrL
// Parse the fmtp string, but doesn't update the iFmtpAttr
// -----------------------------------------------------------------------------
//
TBool CMccCodecH263::ParseFmtpAttrL( const TDesC8& aFmtp )
    {
    __INTERFACE( "CMccCodecH263::ParseFmtpAttrL" )      
    const TUint8* pointerC = aFmtp.Ptr();
    TUint8* pointer        = const_cast<TUint8*>( pointerC );
    TPtr8 descriptor( pointer, aFmtp.Length(), aFmtp.Length() );
    descriptor.Trim();
    
    TDelimitedPathSegmentParser8 parser;
    parser.Parse( aFmtp );
    TPtrC8 segment;
    TBool updated( EFalse );
    
    while ( parser.GetNext( segment ) != KErrNotFound )
        {
        if ( ParseFmtpSegmentL( segment ) )
            {
            updated = ETrue;
            }
        }
    
    __INTERFACE( "CMccCodecH263::ParseFmtpAttrL, exit" )      
    return updated;
    }
    
// -----------------------------------------------------------------------------
// CMccCodecH263::ParseFmtpSegmentL
// Parse and updates the matched param
// -----------------------------------------------------------------------------
//    
TBool CMccCodecH263::ParseFmtpSegmentL( const TDesC8& aSeg )
    {
    __INTERFACE( "CMccCodecH263::ParseFmtpSegmentL" )      
    TBool success( ETrue );
    _LIT8( KEqualSign, "=" );
    
    // Check for first '=' sign 
    TInt index = aSeg.Find( KEqualSign );
    
    if ( index > 0 )
        {
        // Check if match profile
        if ( aSeg.Left( index).Match( KH263Profile ) != KErrNotFound )
            {
            success = ParseProfileL( aSeg.Right( aSeg.Length() - ( index + 1 ) ) );
            }
        
        // Check if match level
        else 
            {
            if ( aSeg.Left( index).Match( KH263Level ) != KErrNotFound )
                {
                success = ParseLevelL( aSeg.Right( aSeg.Length() - ( index + 1 ) ) );
                }
            }
        }  
    __INTERFACE( "CMccCodecH263::ParseFmtpSegmentL, exit" )      
    return success;
    }
    
// -----------------------------------------------------------------------------
// CMccCodecH263::ParseProfile
// -----------------------------------------------------------------------------
//    
TBool CMccCodecH263::ParseProfileL( const TDesC8& aFmtp )
    {
    __INTERFACE( "CMccCodecH263::ParseProfileL, exit" )      
    TBool success( ETrue );
    
    TLex8 myLex( aFmtp );
    TInt value;
    TInt err = myLex.Val( value );
    
    if ( err == KErrNone )
        {
        switch ( value )
            {
            case 0:
                __INTERFACE( "CMccCodecH263::ParseProfileL, profile 0" )      
                break;
            default:
                __INTERFACE( "CMccCodecH263::ParseProfileL, unsupported profile" )      
                success = EFalse;
                break;
            }
        } 
    else
        {
        success = EFalse;
        }
 
    if ( !success )
        {
        __INTERFACE( "CMccCodecH263::ParseProfileL, leaving with KErrArgument" )              
        User::Leave( KErrArgument );
        }

    __INTERFACE( "CMccCodecH263::ParseProfileL, exit" )
    
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CMccCodecH263::ParseProfile
// -----------------------------------------------------------------------------
//    
TBool CMccCodecH263::ParseLevelL( const TDesC8& aFmtp )
    {
    __INTERFACE( "CMccCodecH263::ParseLevelL" )      
    TBool success( ETrue );
 
    TLex8 myLex( aFmtp );
    TInt value;
    TInt err = myLex.Val( value );
    
    if ( err == KErrNone )
        {
        switch ( value )
            {
            case KH263Level10:
                __INTERFACE( "CMccCodecH263::ParseLevelL, level 10" )      
                SetMaxBitrate( KH263Level10Bitrate );
                SetLevelBasedBitrate( KH263Level10Bitrate );
                SetFramerate ( KH263Level10Framerate );
                break;
            case KH263Level45:
                __INTERFACE( "CMccCodecH263::ParseLevelL, level 45" )      
                SetMaxBitrate( KH263Level45Bitrate );
                SetLevelBasedBitrate( KH263Level45Bitrate );
                SetFramerate ( KH263Level45Framerate );
                break;
            default:
                __INTERFACE( "CMccCodecH263::ParseLevelL, unsupported level" )      
                success = EFalse;
                break;
            }
        } 
    else
        {
        success = EFalse;
        }
 
    if ( !success )
        {
        __INTERFACE( "CMccCodecH263::ParseLevelL, leaving with KErrArgument" )      
        User::Leave( KErrArgument );
        }

    __INTERFACE( "CMccCodecH263::ParseLevelL, exit" )      
    return success;
    }
                
// -----------------------------------------------------------------------------
// CMccCodecH263::SetMaxBitrateFromBitrateMask
// -----------------------------------------------------------------------------
//        
void CMccCodecH263::SetMaxBitrateFromBitrateMask( TUint aBitrateMask )
    {
    if( aBitrateMask & KMccAllowedH263Bitrate1 )
        {
        SetMaxBitrate( KH263Bitrate64 );
        }
    else if( aBitrateMask & KMccAllowedH263Bitrate2 )
        {
        SetMaxBitrate( KH263Bitrate128 );
        }
    else if( aBitrateMask & KMccAllowedH263BitrateAll )
        {
        SetMaxBitrate( KH263Bitrate128 );
        }
    }
    
// -----------------------------------------------------------------------------
// CMccCodecH263::SetMaxBitrate
// -----------------------------------------------------------------------------
//  
TInt CMccCodecH263::SetMaxBitrate( TUint aMaxBitrate )
    {
    TBool isValid( ETrue );
       
    switch ( aMaxBitrate )
    	{
    	case KH263Bitrate64:    	    
    		break;
    	case KH263Bitrate128:
    		break;
    	default:
    		isValid = EFalse;
    		break;
    	}
    if ( isValid )
    	{
    	iMaxBitrate = aMaxBitrate;
        SetBitrateMaskFromMaxBitrate( aMaxBitrate );
    	return KErrNone;
    	}
    else
    	{
    	return KErrNotSupported;
    	}
    }

// -----------------------------------------------------------------------------
// CMccCodecH263::SetBitrateMaskFromMaxBitrate
// -----------------------------------------------------------------------------
//      
void CMccCodecH263::SetBitrateMaskFromMaxBitrate( TUint aMaxBitrate )
     {
     switch ( aMaxBitrate )
    	{
    	case KH263Bitrate64:
    	    iBitrateMask = KMccAllowedH263Bitrate1;   	    
    		break;
    	case KH263Bitrate128:
    	    iBitrateMask = KMccAllowedH263BitrateAll;
    		break;
    	default:
    		break;
    	}
     }

// -----------------------------------------------------------------------------
// CMccCodecH263::SetLevelBasedBitrate
// -----------------------------------------------------------------------------
//      
void CMccCodecH263::SetLevelBasedBitrate( TUint aBitrate )
    {
    // Set level based bitrate only if bitrate value has not been set
    if ( !iBitrateSet )
        {
        iBitrate = aBitrate;
        }
    }
    
// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File  
