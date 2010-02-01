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
* Description:    MCC AMR WB CodecInformation
*
*/




// INCLUDE FILES
#include <delimitedpathsegment8.h>

#include "mmcccodecamrwb.h"
#include "mccuids.hrh"
#include "mmccinterfacelogs.h"


// CONSTANTS
const TInt KUndefinedMode( -1 );
const TInt KMaxModeSetValue( 9 );

const TInt KNotSet = -1;

const TInt KNumValue2 = 2;

// MODULE DATA STRUCTURES

const TUint8 KAmrKAPayloadSize = 6;
const TUint8 KAmrKeepAlivePayload[KAmrKAPayloadSize] = 
    { 
    0xF4, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    
// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccCodecAmrWb::CMccCodecAmrWb
// default constructor
// -----------------------------------------------------------------------------
//
CMccCodecAmrWb::CMccCodecAmrWb() : CMccCodecAMR()
    {
    iBitrateMask = KMccAllowedAmrWbBitrateAll;
    iMaxRed = KNotSet;
    iDefaultBitrateMask = KMccAllowedAmrWbBitrateAll;
    iDefaultBitrate = KAmrWbBitrate2385;
    iAdditionalInfoMask = KMccAllowedAmrWbAdditionalInfo;
    iMaxModeSetVal = KMaxModeSetValue;
    }

// -----------------------------------------------------------------------------
// CMccCodecAmrWb::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMccCodecAmrWb::ConstructL( )
    {
    __INTERFACE( "CMccCodecAmrWb::ConstructL" )      
    iSdpName.Copy( KAMRWbSdpName );
    iKeepAliveData.Copy( KAmrKeepAlivePayload );
    iFmtpAttr = HBufC8::NewL( 1 );
    TPtr8 ptr = iFmtpAttr->Des();
    ptr.Append( KNullDesC );
    
    iFourCC = KMccFourCCIdAMRWB;
    SetSamplingFreq( KAmrWbSamplingFreq );
    EnableVAD( EFalse );
    iHwFrameTime = KAMRDefaultHwFrameTime; // default 20ms
    iFrameSize = KAmrWbDefaultFrameSize;
    SetMaxPTime( KAMRDefaultMaxPTime ); // recommended "limit" 200ms
    SetPTime( KAMRDefaultPTime ); // default 20ms 
    SetBitrate( KAmrWbBitrate2385 ); // max bitrate
    SetPayloadType( KDefaultAmrWbPT );
    SetCodecMode( EBandwidthEfficient );
    iNumOfChannels = 1;

    iPayloadFormatEncoder = KImplUidAmrPayloadFormatEncode;
    iPayloadFormatDecoder = KImplUidAmrPayloadFormatDecode;
    
    __INTERFACE( "CMccCodecAmrWb::ConstructL, exit" )      
    }

// -----------------------------------------------------------------------------
// CMccCodecAmrWb::NewL
// Static constructor.
// -----------------------------------------------------------------------------
//
CMccCodecAmrWb* CMccCodecAmrWb::NewL()
    {
    CMccCodecAmrWb* self = new (ELeave) CMccCodecAmrWb;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self ); 
    return self;
    }

// -----------------------------------------------------------------------------
// CMccCodecAmrWb::~CMccCodecAmrWb
// Destructor
// -----------------------------------------------------------------------------
//
CMccCodecAmrWb::~CMccCodecAmrWb()
    {
    __INTERFACE( "CMccCodecAmrWb::~CMccCodecAmrWb" )      
    }

    
// -----------------------------------------------------------------------------
// CMccCodecAmrWb::GetBitrateModeSet
// Get the mode from the bitrate
// -----------------------------------------------------------------------------
//
TInt CMccCodecAmrWb::GetBitrateModeSet( TUint aBitrate )
    {
    __INTERFACE( "CMccCodecAmrWb::GetBitrateModeSet" )      
    TInt mode( KUndefinedMode );
    
    switch ( aBitrate )
        {
        case KAmrWbBitrate660:
            {
            mode = KAMRMode0;
            break;
            }
        case KAmrWbBitrate885:
            {
            mode = KAMRMode1;
            break;
            }
        case KAmrWbBitrate1265:
            {
            mode = KAMRMode2;
            break;
            }
        case KAmrWbBitrate1425:
            {
            mode = KAMRMode3;
            break;
            }
        case KAmrWbBitrate1585:
            {
            mode = KAMRMode4;
            break;
            }
        case KAmrWbBitrate1825:
            {
            mode = KAMRMode5;
            break;
            }
        case KAmrWbBitrate1985:
            {
            mode = KAMRMode6;
            break;
            }
        case KAmrWbBitrate2305:
            {
            mode = KAMRMode7;
            break;
            }
        case KAmrWbBitrate2385:
            {
            mode = KAMRMode8;
            break;
            }
        default:
            {
            break;
            }                
        }
        
    __INTERFACE_INT1( "CMccCodecAmrWb::GetBitrateModeSet, exit with mode", mode )      
    return mode;
    }

// -----------------------------------------------------------------------------
// CMccCodecAmrWb::SetBitrateModeSet
// Sets the bitrate used with AMR codec from the mode .
// -----------------------------------------------------------------------------
//
void CMccCodecAmrWb::SetBitrateModeSet( TUint aModeSet )
    {
    __INTERFACE( "CMccCodecAmrWb::SetBitrateModeSet" )      
    switch ( aModeSet )
        {
        case KAMRMode0:
            {
            SetBitrate( KAmrWbBitrate660 );
            break;
            }
        case KAMRMode1:
            {
            SetBitrate( KAmrWbBitrate885 );
            break;
            }
        case KAMRMode2:
            {
            SetBitrate( KAmrWbBitrate1265 );
            break;
            }
        case KAMRMode3:
            {
            SetBitrate( KAmrWbBitrate1425 );
            break;
            }
        case KAMRMode4:
            {
            SetBitrate( KAmrWbBitrate1585 );
            break;
            }
        case KAMRMode5:
            {
            SetBitrate( KAmrWbBitrate1825 );
            break;
            }
        case KAMRMode6:
            {
            SetBitrate( KAmrWbBitrate1985 );
            break;
            }
        case KAMRMode7:
            {
            SetBitrate( KAmrWbBitrate2305 );
            break;
            }
        case KAMRMode8:
            {
            SetBitrate( KAmrWbBitrate2385 );
            break;
            }
        default:
            {
            SetBitrate( KAmrWbBitrate660 );
            break;
            }
        }
    __INTERFACE( "CMccCodecAmrWb::SetBitrateModeSet, exit" )      
    }

// -----------------------------------------------------------------------------
// CMccCodecAmrWb::GetBitrateMaskFromMode
// Get the bitrate mask from the mode
// -----------------------------------------------------------------------------
//
TInt CMccCodecAmrWb::GetBitrateMaskFromMode( TUint aMode, TUint& aBitrateMask  )
    {
    __INTERFACE( "CMccCodecAmrWb::GetBitrateMaskFromMode" )          
    switch ( aMode )
        {
        case KAMRMode0:
            {
            aBitrateMask = KMccAllowedAmrWbBitrate660;
            break;
            } 
        case KAMRMode1:
            {
            aBitrateMask = KMccAllowedAmrWbBitrate885;
            break;
            }
        case KAMRMode2:
            {
            aBitrateMask = KMccAllowedAmrWbBitrate1265;
            break;
            }
        case KAMRMode3:
            {
            aBitrateMask = KMccAllowedAmrWbBitrate1425;
            break;
            }
        case KAMRMode4:
            {
            aBitrateMask = KMccAllowedAmrWbBitrate1585;
            break;
            }
        case KAMRMode5:
            {
            aBitrateMask = KMccAllowedAmrWbBitrate1825;
            break;
            }
        case KAMRMode6:
            {
            aBitrateMask = KMccAllowedAmrWbBitrate1985;
            break;
            }
        case KAMRMode7:
            {
            aBitrateMask = KMccAllowedAmrWbBitrate2305;
            break;
            }
        case KAMRMode8:
            {
            aBitrateMask = KMccAllowedAmrWbBitrateAll;
            break;
            }
        default:
            {
            __INTERFACE( "CMccCodecAmrWb::GetBitrateMaskFromMode, exit KErrNotFound" )          
            return KErrNotFound; 
            }                
        }
    
    __INTERFACE( "CMccCodecAmrWb::GetBitrateMaskFromMode, exit" )          
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccCodecAmrWb::SetBitrate
// Sets the bitrate used with AMR codec.
// -----------------------------------------------------------------------------
//
TInt CMccCodecAmrWb::SetBitrate( TUint aBitrate )
    {
    TBool isValid( ETrue );
       
    switch ( aBitrate )
        {
        case KAmrWbBitrate660:
            break;
        case KAmrWbBitrate885:
            break;
        case KAmrWbBitrate1265:
            break;
        case KAmrWbBitrate1425:
            break;
        case KAmrWbBitrate1585:
            break;
        case KAmrWbBitrate1825:
            break;
        case KAmrWbBitrate1985:
            break;
        case KAmrWbBitrate2305:
            break;
        case KAmrWbBitrate2385:
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
// CMccCodecAmrWb::SetBitrateFromBitrateMask
// Sets the bitrate used with AMR codec from the bitrateMask.
// -----------------------------------------------------------------------------
//
TInt CMccCodecAmrWb::SetBitrateFromBitrateMask( TUint aBitrateMask )
    {
    if ( aBitrateMask & KMccAllowedAmrWbModeChangePeriod2 )
        {
        SetModeChangePeriod( KNumValue2 );
        }
    
    if ( aBitrateMask & KMccAllowedAmrWbModeChangeNeighbor1 )
        {
        SetModeChangeNeighbor( ETrue );
        }
        
    if( aBitrateMask & KMccAllowedAmrWbBitrate2385 )
        {
        SetBitrate( KAmrWbBitrate2385 );
        return KErrNone;
        }
    else if( aBitrateMask & KMccAllowedAmrWbBitrate2305 )
        {
        SetBitrate( KAmrWbBitrate2305 );
        return KErrNone;
        }
    else if( aBitrateMask & KMccAllowedAmrWbBitrate1985 )
        {
        SetBitrate( KAmrWbBitrate1985 );
        return KErrNone;
        }   
    else if( aBitrateMask & KMccAllowedAmrWbBitrate1825 )
        {
        SetBitrate( KAmrWbBitrate1825 );
        return KErrNone;
        }   
    else if( aBitrateMask & KMccAllowedAmrWbBitrate1585 )
        {
        SetBitrate( KAmrWbBitrate1585 );
        return KErrNone;
        }   
    else if( aBitrateMask & KMccAllowedAmrWbBitrate1425 )
        {
        SetBitrate( KAmrWbBitrate1425 );
        return KErrNone;
        }
    else if( aBitrateMask & KMccAllowedAmrWbBitrate1265 )
        {
        SetBitrate( KAmrWbBitrate1265 );
        return KErrNone;
        }  
    else if( aBitrateMask & KMccAllowedAmrWbBitrate885 )
        {
        SetBitrate( KAmrWbBitrate885 );
        return KErrNone;
        }
    else if( aBitrateMask & KMccAllowedAmrWbBitrate660 )
        {
        SetBitrate( KAmrWbBitrate660 );
        return KErrNone;
        }
    else
        {
        return KErrArgument;    
        }
    }

// -----------------------------------------------------------------------------
// CMccCodecAmrWb::SetSamplingFreq
// Sets the sampling frequency. 
// -----------------------------------------------------------------------------
//
TInt CMccCodecAmrWb::SetSamplingFreq( TUint32 aSamplingFreq )
    {
    if ( KAmrWbSamplingFreq == aSamplingFreq )
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
// CMccCodecAmrWb::SetSdpName
// Sets the SDP name
// -----------------------------------------------------------------------------
TInt CMccCodecAmrWb::SetSdpName( const TDesC8& aSdpName )
    {
    if ( !aSdpName.CompareF( KAMRWbSdpName ) )
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
// CMccCodecAmrWb::SetAllowedBitrates
// Set allowed bitrates
// -----------------------------------------------------------------------------
//
TInt CMccCodecAmrWb::SetAllowedBitrates( TUint aBitrateMask )
    {
    //Confirm that the bitrate mask is valid
    //I.e. after all the valid bitrates are set to zero the value should be zero
    if ( (aBitrateMask >> 16 ) > 0 )
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
        SetBitrateMask( KMccAllowedAmrWbBitrateAll & aBitrateMask );
        SetBitrateMaskAdditionalInfo( aBitrateMask );
        
        SetBitrateFromBitrateMask( iBitrateMask );
        }
    
    return KErrNone;
    }

    
// -----------------------------------------------------------------------------
// CMccCodecAmrWb::GetAllowedBitratesArrayL
// Get allowed bitrates in an array
// -----------------------------------------------------------------------------
//
TInt CMccCodecAmrWb::GetAllowedBitratesArrayL( RArray<TUint>& aBitratesArray )
    {
    if( iBitrateMask & KMccAllowedAmrWbBitrate660 )
        {
        User::LeaveIfError( aBitratesArray.Append(  KAmrWbBitrate660 ) );
        }
    if( iBitrateMask & KMccAllowedAmrWbBitrate885 )
        {
        User::LeaveIfError( aBitratesArray.Append( KAmrWbBitrate885 ) );
        }
    if( iBitrateMask & KMccAllowedAmrWbBitrate1265 )
        {
        User::LeaveIfError( aBitratesArray.Append( KAmrWbBitrate1265 ) );
        }
    if( iBitrateMask & KMccAllowedAmrWbBitrate1425 )
        {
        User::LeaveIfError( aBitratesArray.Append( KAmrWbBitrate1425 ) );
        }
    if( iBitrateMask & KMccAllowedAmrWbBitrate1585 )
        {
        User::LeaveIfError( aBitratesArray.Append( KAmrWbBitrate1585 ) );
        }
    if( iBitrateMask & KMccAllowedAmrWbBitrate1825 )
        {
        User::LeaveIfError( aBitratesArray.Append( KAmrWbBitrate1825 ) );
        }
    if( iBitrateMask & KMccAllowedAmrWbBitrate1985 )
        {
        User::LeaveIfError( aBitratesArray.Append( KAmrWbBitrate1985 ) );
        }
    if( iBitrateMask & KMccAllowedAmrWbBitrate2305 )
        {
        User::LeaveIfError( aBitratesArray.Append( KAmrWbBitrate2305 ) );
        }
    if( iBitrateMask & KMccAllowedAmrWbBitrate2385 )
        {
        User::LeaveIfError( aBitratesArray.Append( KAmrWbBitrate2385 ) );
        }
        
    return KErrNone;
    }
  
// -----------------------------------------------------------------------------
// CMccCodecAmrWb::CloneDefaultsL
// Make a default setting clone from this AMR codec
// -----------------------------------------------------------------------------
//
CMccCodecInformation* CMccCodecAmrWb::CloneDefaultsL()
    {
    __INTERFACE( "CMccCodecAmrWb::CloneDefaultsL" )          
    return CMccCodecAmrWb::NewL();
    }

// -----------------------------------------------------------------------------
// CMccCodecAmrWb::CloneDetailedL
// Make a detailed clone from this AMR codec
// -----------------------------------------------------------------------------
//
CMccCodecInformation* CMccCodecAmrWb::CloneDetailedL()
    {
    CMccCodecAmrWb* newCodec = CMccCodecAmrWb::NewL();
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

// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File
