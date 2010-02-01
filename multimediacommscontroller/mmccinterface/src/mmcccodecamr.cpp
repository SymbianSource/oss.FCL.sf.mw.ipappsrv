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
* Description:    MCC AMR CodecInformation
*
*/




// INCLUDE FILES
#include <delimitedpathsegment8.h>

#include "mmcccodecamr.h"
#include "mccuids.hrh"
#include "mmccinterfacelogs.h"


// CONSTANTS
const TInt KUndefinedMode( -1 );
const TInt KMaxModeSetValue( 8 );

const TInt KNotSet = -1;

const TInt KNumValue1 = 1;
const TInt KNumValue2 = 2;

// MODULE DATA STRUCTURES

const TUint8 KAmrKAPayloadSize = 6;
const TUint8 KAmrKeepAlivePayload[KAmrKAPayloadSize] = 
    { 
    0xF4, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    
// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccCodecAMR::CMccCodecAMR
// default constructor
// -----------------------------------------------------------------------------
//
CMccCodecAMR::CMccCodecAMR() : CMccCodecInformation()
    {
    iBitrateMask = KMccAllowedAmrNbBitrateAll;
    iMaxRed = KNotSet;
    iDefaultBitrateMask = KMccAllowedAmrNbBitrateAll;
    iDefaultBitrate = KAmrNbBitrate122;
    iAdditionalInfoMask = KMccAllowedAmrAdditionalInfo;
    iMaxModeSetVal = KMaxModeSetValue;
    }

// -----------------------------------------------------------------------------
// CMccCodecAMR::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMccCodecAMR::ConstructL( )
    {
    __INTERFACE( "CMccCodecAMR::ConstructL" )      
    iSdpName.Copy( KAMRSdpName );
    iKeepAliveData.Copy( KAmrKeepAlivePayload );
    iFmtpAttr = HBufC8::NewL( 1 );
    TPtr8 ptr = iFmtpAttr->Des();
    ptr.Append( KNullDesC );
    
    iFourCC = KMccFourCCIdAMRNB;
    SetSamplingFreq( KAmrNbSamplingFreq );
    EnableVAD( EFalse );
    iHwFrameTime = KAMRDefaultHwFrameTime; // default 20ms
    iFrameSize = KAMRDefaultFrameSize;
    SetMaxPTime( KAMRDefaultMaxPTime ); // recommended "limit" 200ms
    SetPTime( KAMRDefaultPTime ); // default 20ms 
    SetBitrate( KAmrNbBitrate122 ); // max bitrate
    SetPayloadType( KDefaultAmrNbPT );
    SetCodecMode( EBandwidthEfficient );
    iNumOfChannels = 1;

    iPayloadFormatEncoder = KImplUidAmrPayloadFormatEncode;
    iPayloadFormatDecoder = KImplUidAmrPayloadFormatDecode;
    
    __INTERFACE( "CMccCodecAMR::ConstructL, exit" )      
    }

// -----------------------------------------------------------------------------
// CMccCodecAMR::NewL
// Static constructor.
// -----------------------------------------------------------------------------
//
CMccCodecAMR* CMccCodecAMR::NewL()
    {
    CMccCodecAMR* self = new (ELeave) CMccCodecAMR;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self ); 
    return self;
    }

// -----------------------------------------------------------------------------
// CMccCodecAMR::~CMccCodecAMR
// Destructor
// -----------------------------------------------------------------------------
//
CMccCodecAMR::~CMccCodecAMR()
    {
    __INTERFACE( "CMccCodecAMR::~CMccCodecAMR" )      
    }


// -----------------------------------------------------------------------------
// CMccCodecAMR::GetFmtpL
// Gets the fmtp attribute
// -----------------------------------------------------------------------------
//
TDesC8& CMccCodecAMR::GetFmtpL()
    {
    __INTERFACE( "CMccCodecAMR::GetFmtpL" )      
    if ( iFmtpAttr )
        {
        delete iFmtpAttr;
        iFmtpAttr = NULL;
        }
    
    if( iBitrateMask )
        {
        RArray<TUint> array;
        CleanupClosePushL( array );
        
        if( GetAllowedBitratesArrayL( array ) == KErrNone )
            {
            CreateFmtpAttrListL( array );    
            }
        else
            {
            CreateFmtpAttrListL();        
            }
                
        CleanupStack::Pop( &array );
        array.Close();
        }
    else
        {
        CreateFmtpAttrListL();    
        }

    
    __INTERFACE_STR( "CMccCodecAMR::GetFmtpL", *iFmtpAttr);     
    return *iFmtpAttr;    
    }
    
// -----------------------------------------------------------------------------
// CMccCodecAMR::GetBitrateModeSet
// Get the mode from the bitrate
// -----------------------------------------------------------------------------
//
TInt CMccCodecAMR::GetBitrateModeSet( TUint aBitrate )
    {
    __INTERFACE( "CMccCodecAMR::GetBitrateModeSet" )      
    TInt mode( KUndefinedMode );
    
    switch ( aBitrate )
        {
        case KAmrNbBitrate475:
            {
            mode = KAMRMode0;
            break;
            }
        case KAmrNbBitrate515:
            {
            mode = KAMRMode1;
            break;
            }
        case KAmrNbBitrate590:
            {
            mode = KAMRMode2;
            break;
            }
        case KAmrNbBitrate670:
            {
            mode = KAMRMode3;
            break;
            }
        case KAmrNbBitrate740:
            {
            mode = KAMRMode4;
            break;
            }
        case KAmrNbBitrate795:
            {
            mode = KAMRMode5;
            break;
            }
        case KAmrNbBitrate102:
            {
            mode = KAMRMode6;
            break;
            }
        case KAmrNbBitrate122:
            {
            mode = KAMRMode7;
            break;
            }
        default:
            {
            break;
            }                
        }
        
    __INTERFACE_INT1( "CMccCodecAMR::GetBitrateModeSet, exit with mode", mode )      
    return mode;
    }

// -----------------------------------------------------------------------------
// CMccCodecAMR::SetBitrateModeSet
// Sets the bitrate used with AMR codec from the mode .
// -----------------------------------------------------------------------------
//
void CMccCodecAMR::SetBitrateModeSet( TUint aModeSet )
    {
    __INTERFACE( "CMccCodecAMR::SetBitrateModeSet" )      
    switch ( aModeSet )
        {
        case KAMRMode0:
            {
            SetBitrate( KAmrNbBitrate475 );
            break;
            }
        case KAMRMode1:
            {
            SetBitrate( KAmrNbBitrate515 );
            break;
            }
        case KAMRMode2:
            {
            SetBitrate( KAmrNbBitrate590 );
            break;
            }
        case KAMRMode3:
            {
            SetBitrate( KAmrNbBitrate670 );
            break;
            }
        case KAMRMode4:
            {
            SetBitrate( KAmrNbBitrate740 );
            break;
            }
        case KAMRMode5:
            {
            SetBitrate( KAmrNbBitrate795 );
            break;
            }
        case KAMRMode6:
            {
            SetBitrate( KAmrNbBitrate102 );
            break;
            }
        case KAMRMode7:
            {
            SetBitrate( KAmrNbBitrate122 );
            break;
            }
        default:
            {
            SetBitrate( KAmrNbBitrate475 );
            break;
            }
        }
    __INTERFACE( "CMccCodecAMR::SetBitrateModeSet, exit" )      
    }

// -----------------------------------------------------------------------------
// CMccCodecAMR::GetBitrateMaskFromMode
// Get the bitrate mask from the mode
// -----------------------------------------------------------------------------
//
TInt CMccCodecAMR::GetBitrateMaskFromMode( TUint aMode, TUint& aBitrateMask  )
    {
    __INTERFACE( "CMccCodecAMR::GetBitrateMaskFromMode" )          
    switch ( aMode )
        {
        case KAMRMode0:
            {
            aBitrateMask = KMccAllowedAmrNbBitrate475;
            break;
            }
            
        case KAMRMode1:
            {
            aBitrateMask = KMccAllowedAmrNbBitrate515;
            break;
            }
            
        case KAMRMode2:
            {
            aBitrateMask = KMccAllowedAmrNbBitrate590;
            break;
            }
            
        case KAMRMode3:
            {
            aBitrateMask = KMccAllowedAmrNbBitrate670;
            break;
            }
        case KAMRMode4:
            {
            aBitrateMask = KMccAllowedAmrNbBitrate740;
            break;
            }
        case KAMRMode5:
            {
            aBitrateMask = KMccAllowedAmrNbBitrate795;
            break;
            }
        case KAMRMode6:
            {
            aBitrateMask = KMccAllowedAmrNbBitrate102;
            break;
            }
        case KAMRMode7:
            {
            aBitrateMask = KMccAllowedAmrNbBitrate122;
            break;
            }
        
        default:
            {
            __INTERFACE( "CMccCodecAMR::GetBitrateMaskFromMode, exit KErrNotFound" )          
            return KErrNotFound; 
            }                
        }
    
    __INTERFACE( "CMccCodecAMR::GetBitrateMaskFromMode, exit" )          
    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// CMccCodecAMR::CreateFmtpModeSet
// Creates the mode-set value of the FMTP according to the bitrates
// -----------------------------------------------------------------------------
//
TBool CMccCodecAMR::CreateFmtpModeSet( TDes8& aBuf,
                                       const RArray<TUint>& aBitrates )
    {
    __INTERFACE( "CMccCodecAMR::CreateFmtpModeSet" )          
    TInt tempMode;
    TBool modeSetStringAdded( EFalse );
    _LIT8( KTxtTempMode, "%d" );
    _LIT8( KTxtComma, "," );

    TInt count( aBitrates.Count() );
    
    if ( count > iMaxModeSetVal )
        {
        return modeSetStringAdded;
        }
        
    for ( TInt i = 0; i < count; i++ )
        {
        tempMode = this->GetBitrateModeSet( aBitrates[i] );
        // Do not modify the buffer until we have a recognized mode value
        if ( tempMode >= 0 )
            {
            if ( !modeSetStringAdded )
                {
                // Insert the mode string before the first value
                aBuf.Append( KTxtModeSet ); // "mode-set="
                modeSetStringAdded = ETrue;
                
                // Also clear the bitrate mask
                SetBitrateMask( 0, ETrue );
                }
            else
                {
                // Insert a comma before all other values
                aBuf.Append( KTxtComma );
                }
            // Update the bitrate mask
            TUint currentMask( 0 );
            GetBitrateMaskFromMode( tempMode, currentMask );
            
            SetBitrateMask( ( iBitrateMask | currentMask ), ETrue );
            
            // Update the FMTP string
            aBuf.AppendFormat( KTxtTempMode, tempMode );
            }
        }
    __INTERFACE( "CMccCodecAMR::CreateFmtpModeSet, exit" )          
    return modeSetStringAdded;
    }

// -----------------------------------------------------------------------------
// CMccCodecAMR::SetBitrate
// Sets the bitrate used with AMR codec.
// -----------------------------------------------------------------------------
//
TInt CMccCodecAMR::SetBitrate( TUint aBitrate )
    {
    TBool isValid( ETrue );
       
    switch ( aBitrate )
        {
        case KAmrNbBitrate475:
            break;
        case KAmrNbBitrate515:
            break;
        case KAmrNbBitrate590:
            break;
        case KAmrNbBitrate670:
            break;
        case KAmrNbBitrate740:
            break;
        case KAmrNbBitrate795:
            break;
        case KAmrNbBitrate102:
            break;
        case KAmrNbBitrate122:
            break;
        default:
            isValid = EFalse;
            break;
        }
    if ( isValid )
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
// CMccCodecAMR::SetBitrateFromBitrateMask
// Sets the bitrate used with AMR codec from the bitrateMask.
// -----------------------------------------------------------------------------
//
TInt CMccCodecAMR::SetBitrateFromBitrateMask( TUint aBitrateMask )
    {
    if ( aBitrateMask & KMccAllowedModeChangePeriod2 )
        {
        SetModeChangePeriod( KNumValue2 );
        }
    
    if ( aBitrateMask & KMccAllowedModeChangeNeighbor1 )
        {
        SetModeChangeNeighbor( ETrue );
        }
        
    if( aBitrateMask & KMccAllowedAmrNbBitrate122 )
        {
        SetBitrate( KAmrNbBitrate122 );
        return KErrNone;
        }
    else if( aBitrateMask & KMccAllowedAmrNbBitrate102 )
        {
        SetBitrate( KAmrNbBitrate102 );
        return KErrNone;
        }
    else if( aBitrateMask & KMccAllowedAmrNbBitrate795 )
        {
        SetBitrate( KAmrNbBitrate795 );
        return KErrNone;
        }   
    else if( aBitrateMask & KMccAllowedAmrNbBitrate740 )
        {
        SetBitrate( KAmrNbBitrate740 );
        return KErrNone;
        }   
    else if( aBitrateMask & KMccAllowedAmrNbBitrate670 )
        {
        SetBitrate( KAmrNbBitrate670 );
        return KErrNone;
        }   
    else if( aBitrateMask & KMccAllowedAmrNbBitrate590 )
        {
        SetBitrate( KAmrNbBitrate590 );
        return KErrNone;
        }
    else if( aBitrateMask & KMccAllowedAmrNbBitrate515 )
        {
        SetBitrate( KAmrNbBitrate515 );
        return KErrNone;
        }  
    else if( aBitrateMask & KMccAllowedAmrNbBitrate475 )
        {
        SetBitrate( KAmrNbBitrate475 );
        return KErrNone;
        }
    else
        {
        return KErrArgument;    
        }
    }

// -----------------------------------------------------------------------------
// CMccCodecAMR::SetSamplingFreq
// Sets the sampling frequency. 
// -----------------------------------------------------------------------------
//
TInt CMccCodecAMR::SetSamplingFreq( TUint32 aSamplingFreq )
    {
    // AMR-WB is not supported yet
    if ( KAmrNbSamplingFreq == aSamplingFreq /* || KAmrWbSamplingFreq == aSamplingFreq */ )
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
// CMccCodecAMR::SetSdpName
// Sets the SDP name
// -----------------------------------------------------------------------------
TInt CMccCodecAMR::SetSdpName( const TDesC8& aSdpName )
    {
    if ( !aSdpName.CompareF( KAMRSdpName ) )
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
// CMccCodecAMR::PayloadType
// Sets the payload type
// -----------------------------------------------------------------------------
//
TInt CMccCodecAMR::SetPayloadType( TUint8 aPayloadType )
    {
    if ( aPayloadType < KMinDynamicPT ||
         aPayloadType > KMaxPayloadType ) 
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
// CMccCodecAMR::SetCodecMode
// Sets the codec mode
// -----------------------------------------------------------------------------
//
TInt CMccCodecAMR::SetCodecMode( TCodecMode aCodecMode )
    {
    if ( EBandwidthEfficient == aCodecMode || EOctetAligned == aCodecMode )
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
// CMccCodecAMR::SetAllowedBitrates
// Set allowed bitrates
// -----------------------------------------------------------------------------
//
TInt CMccCodecAMR::SetAllowedBitrates( TUint aBitrateMask )
    {
    //Confirm that the bitrate mask is valid
    //I.e. after all the valid bitrates are set to zero the value should be zero
    if ( (aBitrateMask >> 12 ) > 0 )
        {
        return KErrArgument;
        }
    else
        {
        /*
        In the future the allowed bitrates will be fetched from the codec
        I.e. the KMccAllowedAmrNbBitrateAll will be changed to the bitrates
        supported by the codec
        */
        SetBitrateMask( KMccAllowedAmrNbBitrateAll & aBitrateMask );
        SetBitrateMaskAdditionalInfo( aBitrateMask );
        
        SetBitrateFromBitrateMask( iBitrateMask );
        }
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccCodecAMR::SetRedCount
// Sets level of redundancy.
// -----------------------------------------------------------------------------
//
TInt CMccCodecAMR::SetRedCount( TUint aRedCount )
    {
    TInt retVal( KErrArgument );
    if ( iMaxRed != KNotSet )
        {
        if ( KMaxAmrFecRedCount >= aRedCount )
            {
            TAlgoUsed algo = aRedCount > 0 ? EAmrFecUsed : ENoAlgoUsed;
            SetAlgo( algo );
            iRedCount = aRedCount;
            retVal = KErrNone;
            }
        }
    else if ( KMaxGenRedCount >= aRedCount )
        {   
        TAlgoUsed algo = aRedCount > 0 ? EGenRedUsed : ENoAlgoUsed;
        SetAlgo( algo );
        iRedCount = aRedCount;
        retVal = KErrNone;
        }
    else
        {
        // NOP
        }
    return retVal;
    }
    
// -----------------------------------------------------------------------------
// CMccCodecAMR::GetAllowedBitratesArrayL
// Get allowed bitrates in an array
// -----------------------------------------------------------------------------
//
TInt CMccCodecAMR::GetAllowedBitratesArrayL( RArray<TUint>& aBitratesArray )
    {
    if( iBitrateMask & KMccAllowedAmrNbBitrate475 )
        {
        User::LeaveIfError( aBitratesArray.Append(  KAmrNbBitrate475 ) );
        }
    if( iBitrateMask & KMccAllowedAmrNbBitrate515 )
        {
        User::LeaveIfError( aBitratesArray.Append( KAmrNbBitrate515 ) );
        }
    if( iBitrateMask & KMccAllowedAmrNbBitrate590 )
        {
        User::LeaveIfError( aBitratesArray.Append( KAmrNbBitrate590 ) );
        }
    if( iBitrateMask & KMccAllowedAmrNbBitrate670 )
        {
        User::LeaveIfError( aBitratesArray.Append( KAmrNbBitrate670 ) );
        }
    if( iBitrateMask & KMccAllowedAmrNbBitrate740 )
        {
        User::LeaveIfError( aBitratesArray.Append( KAmrNbBitrate740 ) );
        }
    if( iBitrateMask & KMccAllowedAmrNbBitrate795 )
        {
        User::LeaveIfError( aBitratesArray.Append( KAmrNbBitrate795 ) );
        }
    if( iBitrateMask & KMccAllowedAmrNbBitrate102 )
        {
        User::LeaveIfError( aBitratesArray.Append( KAmrNbBitrate102 ) );
        }
    if( iBitrateMask & KMccAllowedAmrNbBitrate122 )
        {
        User::LeaveIfError( aBitratesArray.Append( KAmrNbBitrate122 ) );
        }
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccCodecAMR::EnableVAD
// Enable / Disable VAD
// -----------------------------------------------------------------------------
//
TInt CMccCodecAMR::EnableVAD( TBool aEnableVAD ) 
    {
    iEnableVAD = aEnableVAD;        
    return KErrNone;
    }
    

// -----------------------------------------------------------------------------
// CMccCodecAMR::SetPTime
// Set PTime
// -----------------------------------------------------------------------------
//
TInt CMccCodecAMR::SetPTime( TUint aPTime )
    {
    if ( ( aPTime <= iMaxPTime ) && (( aPTime % KDefaultPtime )  == 0 ) &&
         aPTime && ( KMinPtime < aPTime ) )
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
// CMccCodecAMR::SetMaxPTime
// Set Max PTime
// -----------------------------------------------------------------------------
//
TInt CMccCodecAMR::SetMaxPTime( TUint aMaxPTime )
    {
    if (  aMaxPTime && ( aMaxPTime <= KAMRDefaultMaxPTime ) &&
        ( ( aMaxPTime % KDefaultPtime )  == 0 ) && ( KMinPtime < aMaxPTime )
        && iPTime <= aMaxPTime )
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
// CMccCodecAMR::CloneDefaultsL
// Make a default setting clone from this AMR codec
// -----------------------------------------------------------------------------
//
CMccCodecInformation* CMccCodecAMR::CloneDefaultsL()
    {
    __INTERFACE( "CMccCodecAMR::CloneDefaultsL" )          
    return CMccCodecAMR::NewL();
    }

// -----------------------------------------------------------------------------
// CMccCodecAMR::CloneDetailedL
// Make a detailed clone from this AMR codec
// -----------------------------------------------------------------------------
//
CMccCodecInformation* CMccCodecAMR::CloneDetailedL()
    {
    CMccCodecAMR* newCodec = CMccCodecAMR::NewL();
    CleanupStack::PushL( newCodec );
    newCodec->SetBitrate( this->Bitrate() );
    newCodec->SetCodecMode( this->CodecMode() );
    newCodec->SetMaxPTime( this->MaxPTime() );
    newCodec->SetPayloadType( this->PayloadType() );
    newCodec->SetPTime( this->PTime() );
    newCodec->SetSamplingFreq( this->SamplingFreq() );
    newCodec->SetSdpName( this->SdpName() );
    newCodec->SetFmtpAttrL( this->GetFmtpL(), EFalse );
    CleanupStack::Pop( newCodec );
    return newCodec;
    }
 
// -----------------------------------------------------------------------------
// CMccCodecAMR::RequireSignalling
// -----------------------------------------------------------------------------
//
TBool CMccCodecAMR::RequireSignalling(
    const CMccCodecInformation& aCandidate ) const      
    {
    __INTERFACE( "CMccCodecAMR::RequireSignalling" )
    TBool ret = EFalse;
              
    if ( iPTime != aCandidate.PTime() )
        {
        __INTERFACE( "CMccCodecAMR::RequireSignalling, PTime changed" )
        ret = ETrue;
        }
        
    if ( iMaxPTime != aCandidate.MaxPTime() )
        {
        __INTERFACE( "CMccCodecAMR::RequireSignalling, MaxPTime changed" )
        ret = ETrue;
        }
        
    if ( iPayloadType != aCandidate.PayloadType() )
        {
        __INTERFACE( "CMccCodecAMR::RequireSignalling, PayloadType changed" )
        ret = ETrue;
        }

	__INTERFACE_INT1( "CMccCodecAMR::RequireSignalling, bitrate mask 1", iBitrateMask )
	__INTERFACE_INT1( "CMccCodecAMR::RequireSignalling, bitrate mask 2", aCandidate.AllowedBitrates() )
  
	if ( iBitrateMask != aCandidate.AllowedBitrates() )
        {
        if ( iBitrateMask != ( KMccAllowedModeChangeNeighbor1 | KMccAllowedAmrNbBitrateAll ) &&
        	 iBitrateMask != KMccAllowedModeChangeNeighbor1 )
        	{
        	__INTERFACE( "CMccCodecAMR::RequireSignalling, BitrateMask changed" )
        	ret = ETrue;
        	}
        if ( aCandidate.AllowedBitrates() != ( KMccAllowedModeChangeNeighbor1 |
        									   KMccAllowedAmrNbBitrateAll ) &&
        	 aCandidate.AllowedBitrates() != KMccAllowedModeChangeNeighbor1 )
        	{
        	__INTERFACE( "CMccCodecAMR::RequireSignalling, BitrateMask changed" )
        	ret = ETrue;
        	}
        }

    if ( iCodecMode != aCandidate.CodecMode() )
        {
        __INTERFACE( "CMccCodecAMR::RequireSignalling, CodecMode changed" )
        ret = ETrue;
        }
        
    __INTERFACE_INT1( "CMccCodecAMR::RequireSignalling, exit with", ret )
    return ret;          
    }
 
// -----------------------------------------------------------------------------
// CMccCodecAMR::ParseFmtpAttrL
// Parse the fmtp string, but doesn't update the iFmtpAttr
// -----------------------------------------------------------------------------
//
TBool CMccCodecAMR::ParseFmtpAttrL( const TDesC8& aFmtp )
    {
    // These booleans needed in FMTP parsing to handle where parameters
    // not present in FMTP

    iModesExist = EFalse;
    iOctetAlignExist = EFalse;
    iModeChangePeriodExist = EFalse;
    iModeChangeNeighborExist = EFalse;
    iMaxRedExist = EFalse;
    
    //Remove all white space from begining and end,
    //Convert the content of the descriptor to lower case
    const TUint8* pointerC = aFmtp.Ptr();
    TUint8* pointer        = const_cast<TUint8*>( pointerC );
    TPtr8 descriptor( pointer, aFmtp.Length(), aFmtp.Length() );
    descriptor.Trim();
    
    /*
    TDelimitedPathSegmentParser8 Parses path segments whose 
    components are delimited by the ';' character, 
    as defined in RFC2396.
    */
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
    
    if ( !iModesExist )
        {
        // Allow all modes
        SetBitrateMask( iDefaultBitrateMask, ETrue );
        SetBitrate( iDefaultBitrate ); // max bitrate
        }
        
    if ( !iOctetAlignExist )
        {
        SetCodecMode( EBandwidthEfficient );
        }
    
    if ( !iModeChangePeriodExist )
        {
        SetModeChangePeriod( KDefaultModeChangePeriod );
        }
        
    if ( !iModeChangeNeighborExist )
        {
        SetModeChangeNeighbor( EFalse );
        }
        
    if ( !iMaxRedExist )
        {
        SetMaxRed( KNotSet );
        }   
                
    return updated;
    }

// -----------------------------------------------------------------------------
// CMccCodecAMR::ParseFmtpSegmentL
// Parse and updates the matched param
// -----------------------------------------------------------------------------
//    
TBool CMccCodecAMR::ParseFmtpSegmentL( const TDesC8& aSeg )
    {
    TBool update( EFalse );
    _LIT8( KEqualSign, "=" );
    
    // Check for first '=' sign 
    TInt index = aSeg.Find( KEqualSign );
    
    if ( index > 0 )
        {
        // Check if match mode-set 
        if ( aSeg.Left( index ).Match( KMatchModeSet  ) != KErrNotFound )
            {
            update = ParseFmtpModeSetL( aSeg.Right( aSeg.Length() - ( index + 1 ) ) );
            iModesExist = ETrue;
            }      
        // Check if match octet-align
        else if ( aSeg.Left( index ).Match( KMatchOctetAlign ) != KErrNotFound )
            {
            update = ParseFmtpOctetAlignL( aSeg.Right( aSeg.Length() - ( index + 1 ) ) );
            iOctetAlignExist = ETrue;
            }

        // Check if match mode-change-period
        else if ( aSeg.Left( index ).Match( KMatchModeChangePeriod ) != KErrNotFound )
            {
            update = ParseFmtpModeChangePeriodL( aSeg.Right( aSeg.Length() - ( index + 1 ) ) );
            iModeChangePeriodExist = ETrue;
            }
        // Check if match mode-change-neighbor
        else if ( aSeg.Left( index ).Match( KMatchModeChangeNeighbor ) != KErrNotFound )
            {
            update = ParseFmtpModeChangeNeighborL( aSeg.Right( aSeg.Length() - ( index + 1 ) ) );
            iModeChangeNeighborExist = ETrue;
            }
        // Check if match crc
        else if ( aSeg.Left( index ).Match( KCrc ) != KErrNotFound )
            {
            update = ParseFmtpCrcL( aSeg.Right( aSeg.Length() - ( index + 1 ) ) );
            }
        // Check if match crc
        else if ( aSeg.Left( index ).Match( KRobustSorting ) != KErrNotFound )
            {
            update = ParseFmtpRobustSortingL( aSeg.Right( aSeg.Length() - ( index + 1 ) ) );
            }
        // Check if match interleaving
        else if ( aSeg.Left( index ).Match( KInterleaving ) != KErrNotFound )
            {
            update = ParseFmtpInterleavingL( aSeg.Right( aSeg.Length() - ( index + 1 ) ) );
            }
        else if ( aSeg.Left( index ).Match( KMaxRed ) != KErrNotFound )
            {
            update = ParseFmtpMaxRedL( aSeg.Right( aSeg.Length() - ( index + 1 ) ) );
            }              
        else
            {
            // Make PC-LINT happy.
            }
        }
  
    return update;
    }

// -----------------------------------------------------------------------------
// CMccCodecAMR::ParseFmtpModeSet
// Parses and updates the bitrate
// -----------------------------------------------------------------------------
//      
TBool CMccCodecAMR::ParseFmtpModeSetL( const TDesC8& aFmtpModeSet )
    {
    /**
     * RFC 3267, page 39
     *
     * mode-set: Requested AMR mode set. Restricts the active codec mode 
     * set to a subset of all modes. Possible values are a comma separetad
     * list of modes from the set: 0,..,7. If such mode set is specified by 
     * the decoder, the encoder MUST abide by the request and  MUST NOT use
     * modes outside of the subset. If not present, all codec modes are 
     * allowed for the session
     **/
     
    TInt err( KErrNone );
    TInt error( KErrNone );
    
    HBufC8* hBuf = HBufC8::NewLC( aFmtpModeSet.Length() );

    hBuf->Des().Copy( aFmtpModeSet );
    TPtr8 ptr( hBuf->Des() );

    // Convert descriptor to integer using TLex8
    TLex8 lex;
    TInt index( KErrNone );
    TInt maxValue( 0 );
    TInt currentValue( 0 );
    
    TUint bitrateMaskCurrent( 0 );
    TUint bitrateMaskUpdated( 0 );
    
    TBool more( ETrue );
    
    while ( more )
        {
        ptr.TrimAll();

        // Find comma sign
        index = ptr.Find( KCommaSign );
        
        if ( index == KErrNotFound )
            {
            // No commas found, this must be the last value
            lex.Assign( ptr );
            err = lex.Val( currentValue );
            more = EFalse;
            }
        else
            {
            // Read what is to the left of the comma
            lex.Assign( ptr.Left( index ) );
            err = lex.Val( currentValue );

            // Delete the processed characters and the comma
            ptr.Delete( 0, ( index + 1 ) );    
            }
        
        // Get the value if conversion succeeded
        if ( err == KErrNone )
            {
            error = GetBitrateMaskFromMode( currentValue,  bitrateMaskCurrent );
            
            //Update bitrateMask
            if( error == KErrNone )
                {
                bitrateMaskUpdated |= bitrateMaskCurrent;
                }
            //if mode not supported, then leave
            else
                {
                User::Leave( KErrArgument );
                }
                
            //Update max mode-set value
            if ( currentValue > maxValue )
                {
                maxValue = currentValue;
                }
            }
        
        // This is for the case when the last character is a comma,
        // e.g. "mode-set=0,"
        if ( ptr.Length() == 0 )
            {
            more = EFalse;
            }
        }
    
    User::LeaveIfError( err );

    // Delete the copy
    if( hBuf )
        {
        CleanupStack::PopAndDestroy( hBuf );
        hBuf = NULL;
        }
    
    // Set bitrate to the maximum value found in the string
    SetBitrateModeSet( maxValue );
    
    //Update the bitrateMask
    SetBitrateMask( bitrateMaskUpdated, ETrue );
    
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CMccCodecAMR::ParseFmtpOctetAlign
// Updates the CodecMode to either EOctetAligned/EBandwidthEfficient
// -----------------------------------------------------------------------------
//    
TBool CMccCodecAMR::ParseFmtpOctetAlignL( const TDesC8& aFmtpOctetAlign )
    {
    /**
     * RFC 3267, page 39
     *
     * octet-align: Permissible values are 0 and 1. If 1, octet-aligned operation
     * SHALL be used. If 0 or if not present, bandwidth efficient operation is 
     * employed
     */
    
    TBool update( ETrue );
    
    // Convert descriptor to integer
    TLex8 myLex( aFmtpOctetAlign );
    TInt octetAlign;
    User::LeaveIfError( myLex.Val( octetAlign ) );
    
    switch ( octetAlign )
        {
        case 0:
            SetCodecMode( EBandwidthEfficient );                
            break;

        case 1:
            SetCodecMode( EOctetAligned );
            break;

        default:
            update = EFalse;
            break;
        } // switch

    return update;
    }

// -----------------------------------------------------------------------------
// CMccCodecAMR::ParseFmtpModeChangePeriodL
// -----------------------------------------------------------------------------
//    
TBool CMccCodecAMR::ParseFmtpModeChangePeriodL( const TDesC8& aFmtpModeChangePeriod )
    {
    /**
     * RFC 3267, page 39-40
     *
     * Specifies a number of frame-blocks, N, that is
     * the interval at which codec mode changes are allowed.
     * The initial phase of the interval is arbitrary, but
     * changes must be separated by multiples of N frame-blocks.
     * If this parameter is not present, mode changes are
     * allowed at any time during the session.
     */

    // Convert descriptor to integer
    TLex8 myLex( aFmtpModeChangePeriod );
    TInt modeChangePeriod;
    User::LeaveIfError( myLex.Val( modeChangePeriod ) );

    User::LeaveIfError( this->SetModeChangePeriod( modeChangePeriod ) );
     
    return ETrue;
    }
    
// -----------------------------------------------------------------------------
// CMccCodecAMR::ParseFmtpModeChangeNeighborL
// -----------------------------------------------------------------------------
//    
TBool CMccCodecAMR::ParseFmtpModeChangeNeighborL( const TDesC8& aFmtpModeChangeNeighbor )
    {
    // Convert descriptor to integer
    TLex8 myLex( aFmtpModeChangeNeighbor );
    TInt modeChangeNeighbor;
    User::LeaveIfError( myLex.Val( modeChangeNeighbor ) );
   
    User::LeaveIfError( this->SetModeChangeNeighbor( modeChangeNeighbor ) );
                
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CMccCodecAMR::ParseFmtpCrcL
// -----------------------------------------------------------------------------
//    
TBool CMccCodecAMR::ParseFmtpCrcL( const TDesC8& aFmtpCrc )
    {
    // Convert descriptor to integer
    TLex8 myLex( aFmtpCrc );
    TInt crc;
    User::LeaveIfError( myLex.Val( crc ) );
     
    User::LeaveIfError( this->SetCrc( crc ) );
               
    return ETrue;
    }
    
// -----------------------------------------------------------------------------
// CMccCodecAMR::ParseFmtpRobustSortingL
// -----------------------------------------------------------------------------
//    
TBool CMccCodecAMR::ParseFmtpRobustSortingL( const TDesC8& aFmtpRobustSorting )
    {
    // Convert descriptor to integer
    TLex8 myLex( aFmtpRobustSorting );
    TInt robustSorting;
    User::LeaveIfError( myLex.Val( robustSorting ) );
  
    User::LeaveIfError( this->SetRobustSorting( robustSorting ) );
                
    return ETrue;
    }
        
// -----------------------------------------------------------------------------
// CMccCodecAMR::ParseFmtpInterleaving
// -----------------------------------------------------------------------------
//    
TBool CMccCodecAMR::ParseFmtpInterleavingL( const TDesC8& aFmtpInterleaving )
    {  
    // Convert descriptor to integer
    TLex8 myLex( aFmtpInterleaving );
    TInt interleaving;
    User::LeaveIfError( myLex.Val( interleaving ) );
   
    User::LeaveIfError( this->SetInterleaving( interleaving ) );
       
    return ETrue;
    } 

// -----------------------------------------------------------------------------
// CMccCodecAMR::ParseFmtpMaxRedL
// -----------------------------------------------------------------------------
//    
TBool CMccCodecAMR::ParseFmtpMaxRedL( const TDesC8& aFmtpMaxRed )
    {  
    // Convert descriptor to integer
    TLex8 myLex( aFmtpMaxRed );
    TInt maxRed;
    User::LeaveIfError( myLex.Val( maxRed ) );   
    User::LeaveIfError( this->SetMaxRed( maxRed ) );
       
    return ETrue;
    } 
    
// -----------------------------------------------------------------------------
// CMccCodecAMR::CreateFmtpOctetAlign
// Creates the octet-alignment value of the FMTP according to the codec
// -----------------------------------------------------------------------------
//
TBool CMccCodecAMR::CreateFmtpOctetAlign( TDes8& aBuf )
    {
    TBool octetAlignAdded( EFalse );
    // Append only if octet-aligned is EOctetAligned
    if( CodecMode() == EOctetAligned ) 
        {
        TInt octetAlignValue( 1 );    
        // Append it to the buffer        
        _LIT8( KFmtpOctetAlign, "octet-align=%d" );
        aBuf.AppendFormat( KFmtpOctetAlign, octetAlignValue );
        octetAlignAdded = ETrue;
        }
    return octetAlignAdded;
    }
    
// -----------------------------------------------------------------------------
// CMccCodecAMR::CreateFmtpModeChangePeriod
// Creates the mode-change-period value of the FMTP according to the codec
// -----------------------------------------------------------------------------
//
TBool CMccCodecAMR::CreateFmtpModeChangePeriod( TDes8& aBuf )
    {
    TBool modeChangePeriodAdded( EFalse );
    // No need to add to fmtp if mode-change-period=1 (default)
    if ( iModeChangePeriod == KNumValue2 ) 
        {  
        // Append it to the buffer        
        _LIT8( KFmtpModeChangePeriod, "mode-change-period=%d" );
        aBuf.AppendFormat( KFmtpModeChangePeriod, iModeChangePeriod );
        modeChangePeriodAdded = ETrue;       
        }
        
    return modeChangePeriodAdded;    
    }
    
// -----------------------------------------------------------------------------
// CMccCodecAMR::CreateFmtpModeChangeNeighbor
// Creates the mode-change-neighbor value of the FMTP according to the codec
// -----------------------------------------------------------------------------
//
TBool CMccCodecAMR::CreateFmtpModeChangeNeighbor( TDes8& aBuf )
    {
    TBool modeChangeNeighborAdded( EFalse );
    // Append only if mode-change-neighbor is ETrue
    if ( iNeighbor ) 
        {  
        // Append it to the buffer        
        _LIT8( KFmtpModeChangeNeighbor, "mode-change-neighbor=%d" );
        aBuf.AppendFormat( KFmtpModeChangeNeighbor, iNeighbor );
        modeChangeNeighborAdded = ETrue;       
        
        }
        
    return modeChangeNeighborAdded;    
    }  

// -----------------------------------------------------------------------------
// CMccCodecAMR::CreateFmtpMaxRed
// -----------------------------------------------------------------------------
//
TBool CMccCodecAMR::CreateFmtpMaxRed( TDes8& aBuf )
    {
    TBool maxRedAdded( EFalse );
    
    if ( KNotSet < iMaxRed && EAmrFecUsed == iAlgoUsed ) 
        {  
        // Append it to the buffer        
        _LIT8( KFmtpMaxRed, "max-red=%d" );
        aBuf.AppendFormat( KFmtpMaxRed, iMaxRed );
        maxRedAdded = ETrue;       
        }
        
    return maxRedAdded;    
    }
      
// -----------------------------------------------------------------------------
// CMccCodecAMR::CreateFmtpAttrListL
// If fmtp does not exist, then create it
// -----------------------------------------------------------------------------
//   
void CMccCodecAMR::CreateFmtpAttrListL()
    {
    RArray<TUint> array;
    CleanupClosePushL( array );
    
    this->GetAllowedBitratesArrayL( array );
    
    this->CreateFmtpAttrListL( array );
    
    CleanupStack::Pop( &array );
    array.Close();
    }

// -----------------------------------------------------------------------------
// CMccCodecAMR::CreateFmtpAttrListL
// If fmtp does not exist, then create it
// -----------------------------------------------------------------------------
//        
void CMccCodecAMR::CreateFmtpAttrListL( const RArray<TUint>& aBitrates )
    {
    // Create the fmtp string: octet-align=0/1; mode-set=[0][,1][,2]...[,7]
    // mode-change-period=1/2; mode-change-neighbor=0/1
    HBufC8* buf = HBufC8::NewLC( KMaxFmtpLen ); 
    TPtr8 bufPtr = buf->Des();
    
    TBool previousParamAdded( EFalse );
    
    if ( CreateFmtpOctetAlign( bufPtr ) )
        {
        previousParamAdded = ETrue;
        }
    
    TInt semicolonPos( bufPtr.Length() );
    
    if ( CreateFmtpModeSet( bufPtr, aBitrates ) )
        {
        // Previous param was added, so we need a semi-colon and a space
        if ( previousParamAdded )
            {
            bufPtr.Insert( semicolonPos, KSemicolonSpace );
            }
        previousParamAdded = ETrue;
        }
    
    semicolonPos = bufPtr.Length();
    
    if ( CreateFmtpModeChangePeriod( bufPtr ) )
        {
        // Previous param was added, so we need a semi-colon and a space
        if ( previousParamAdded )
            {
            bufPtr.Insert( semicolonPos, KSemicolonSpace );
            }
        previousParamAdded = ETrue;
        }
    
    semicolonPos = bufPtr.Length();
        
    if ( CreateFmtpModeChangeNeighbor( bufPtr ) )
        {
        // Previous param was added, so we need a semi-colon and a space
        if ( previousParamAdded )
            {
            bufPtr.Insert( semicolonPos, KSemicolonSpace );
            }
        previousParamAdded = ETrue;
        }
        
    semicolonPos = bufPtr.Length();
        
    if ( CreateFmtpMaxRed( bufPtr ) )
        {
        // Previous param was added, so we need a semi-colon and a space
        if ( previousParamAdded )
            {
            bufPtr.Insert( semicolonPos, KSemicolonSpace );
            }
        //previousParamAdded = ETrue; not necessery if no more attributes
        }     
                
    // Remove extra space from the end
    bufPtr.TrimRight();
        
    // Update iFmtpAttr
    SetFmtpAttrL( bufPtr, EFalse );
    
    CleanupStack::PopAndDestroy( buf );
    
    __INTERFACE_STR8( "CMccCodecAMR::CreateFmtpAttrListL, *iFmtpAttr", *iFmtpAttr )
    }    

// -----------------------------------------------------------------------------
// CMccCodecAMR::SetModeChangePeriod
// Set mode change period
// -----------------------------------------------------------------------------
//
TInt CMccCodecAMR::SetModeChangePeriod( const TInt aPeriod )
    {
    // valid value 1 or 2
    if( KNumValue1 > aPeriod || KNumValue2 < aPeriod )
        {
        return KErrArgument;
        }
    
    iModeChangePeriod = aPeriod;
    
    SetBitrateMaskAdditionalInfo( 
        ( iModeChangePeriod == KNumValue2 ) ? KMccAllowedModeChangePeriod2 : 0 );
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccCodecAMR::ModeChangePeriod
// Get mode change period
// -----------------------------------------------------------------------------
//
TInt CMccCodecAMR::ModeChangePeriod( ) const
    {
    return iModeChangePeriod;
    }

// -----------------------------------------------------------------------------
// CMccCodecAMR::SetModeChangeNeighbor
// Set mode change neighbor
// -----------------------------------------------------------------------------
//
TInt CMccCodecAMR::SetModeChangeNeighbor( const TBool aNeighbor )
    {
    // Default value is ETrue
    iNeighbor = aNeighbor;
    
    SetBitrateMaskAdditionalInfo( 
        iNeighbor ? KMccAllowedModeChangeNeighbor1 : 0 );
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccCodecAMR::ModeChangeNeighbor
// Get mode change neighbor
// -----------------------------------------------------------------------------
//
TBool CMccCodecAMR::ModeChangeNeighbor( ) const
    {
    return iNeighbor;
    }

// -----------------------------------------------------------------------------
// CMccCodecAMR::SetCrc
// Set boolean type CRC value
// -----------------------------------------------------------------------------
//  
TInt CMccCodecAMR::SetCrc( const TBool aCrc )
    {
    return ( aCrc ? KErrNotSupported : KErrNone );
    }

// -----------------------------------------------------------------------------
// CMccCodecAMR::GetCrc
// Get boolean type CRC value
// -----------------------------------------------------------------------------
// 
TBool CMccCodecAMR::GetCrc( ) const
    {
    return iCrc;
    }

// -----------------------------------------------------------------------------
// CMccCodecAMR::SetRobustSorting
// Default implementation
// -----------------------------------------------------------------------------
// 
TInt CMccCodecAMR::SetRobustSorting( const TBool aRobust )
    {
    return ( aRobust ? KErrNotSupported : KErrNone );
    }

// -----------------------------------------------------------------------------
// CMccCodecAMR::GetRobustSorting
// Default implementation
// -----------------------------------------------------------------------------
// 
TBool CMccCodecAMR::GetRobustSorting( ) const
    {
    return iRobust;
    }

// -----------------------------------------------------------------------------
// CMccCodecAMR::SetInterleaving
// Default implementation
// -----------------------------------------------------------------------------
// 
TInt CMccCodecAMR::SetInterleaving( const TInt /*aFrameBlockCount*/ )
    {
    return KErrNotSupported;
    }

// -----------------------------------------------------------------------------
// CMccCodecAMR:GetInterleaving
// Default implementation
// -----------------------------------------------------------------------------
// 
TInt CMccCodecAMR::GetInterleaving( ) const
    {
    return iFrameBlockCount;
    }  

// -----------------------------------------------------------------------------
// CMCPAMRCodec::SetChannels 
// -----------------------------------------------------------------------------
// 
TInt CMccCodecAMR::SetChannels( TInt aNumOfChannels )
    {
    if ( aNumOfChannels != 1 )
        {
        return KErrNotSupported;
        }

    iNumOfChannels = aNumOfChannels;    
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMCPAMRCodec:GetChannels
// -----------------------------------------------------------------------------
// 
TInt CMccCodecAMR::GetChannels( TInt& aNumOfChannels ) const
    {   
    aNumOfChannels = iNumOfChannels;
    return KErrNone; 
    }

// -----------------------------------------------------------------------------
// CMccCodecAMR:SetMaxRed
// -----------------------------------------------------------------------------
// 
TInt CMccCodecAMR::SetMaxRed( TInt aMaxRed ) 
    {   
    if ( ( KNotSet > aMaxRed ) ||
        ( aMaxRed % TInt( KDefaultPtime ) ) != 0 )
        {
        return KErrArgument;
        }
        
    iMaxRed = aMaxRed;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccCodecAMR:SetBitrateMask
// -----------------------------------------------------------------------------
// 
void CMccCodecAMR::SetBitrateMask( TUint aBitrateMask, TBool aKeepAdditionalInfo )
    {
    __INTERFACE_INT1( "CMccCodecAMR::SetBitrateMask, entry mask ", aBitrateMask )      
    TUint oldBitrateMask = iBitrateMask; 
    iBitrateMask = aBitrateMask;
        
    if ( aKeepAdditionalInfo )
        {
        iBitrateMask |= ( oldBitrateMask & iAdditionalInfoMask );
        }
    else
        {
        SetBitrateMaskAdditionalInfo( aBitrateMask );
        }
        
    __INTERFACE_INT1( "CMccCodecAMR::SetBitrateMask, exit mask ", iBitrateMask )      
    }

// -----------------------------------------------------------------------------
// CMccCodecAMR:SetBitrateMaskAdditionalInfo
// -----------------------------------------------------------------------------
//    
void CMccCodecAMR::SetBitrateMaskAdditionalInfo( TUint aAdditionalInfo )
    {
    iBitrateMask |= ( aAdditionalInfo & iAdditionalInfoMask );
    }
    
// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File
