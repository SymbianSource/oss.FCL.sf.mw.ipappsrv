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
* Description:    MCC G.711 CodecInformation
*
*/




// INCLUDE FILES
#include "mmcccodecg711.h"
#include "mccuids.hrh"
#include "mmccinterfacelogs.h"

// CONSTANTS

const TUint8 KG711KAPayloadSize = 160;
const TUint8 KG711KeepAlivePayload[KG711KAPayloadSize] = 
    { 
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
    };

const TInt KG711SampleRate = 8000;
// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMCCCodecG711::CMCCCodecG711
// default constructor
// -----------------------------------------------------------------------------
//
CMCCCodecG711::CMCCCodecG711() : CMccCodecInformation()
    {   
    }

// -----------------------------------------------------------------------------
// CMCCCodecG711::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMCCCodecG711::ConstructL()
    {
    __INTERFACE( "CMCCCodecG711::ConstructL" )
    
    iKeepAliveData.Copy( KG711KeepAlivePayload,  KMaxKeepAliveDataLen);    
    SetSamplingFreq( KG711SampleRate );
    SetBitrate( KG711Bitrate64 );
    EnableVAD( EFalse );
    iHwFrameTime = KG711DefaultHwFrameTime; // default 20ms
    SetMaxPTime( KMaxPtime );
    SetPTime( KDefaultPtime );
    SetPayloadType( KPcmuPayloadType );
    SetSdpName( KPCMUSdpName );
    SetCodecMode( EPCMU );
    
    iFourCC = KMccFourCCIdG711;
    iPayloadFormatEncoder = KImplUidG711PayloadFormatEncode;
    iPayloadFormatDecoder = KImplUidG711PayloadFormatDecode;
    }

// -----------------------------------------------------------------------------
// CMCCCodecG711::NewL
// Static constructor.
// -----------------------------------------------------------------------------
//
CMCCCodecG711* CMCCCodecG711::NewL()
    {
    CMCCCodecG711* self = new (ELeave) CMCCCodecG711;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self ); 
    return self;
    }

// -----------------------------------------------------------------------------
// CMCCCodecG711::~CMCCCodecG711
// Destructor
// -----------------------------------------------------------------------------
//
CMCCCodecG711::~CMCCCodecG711()
    {
    __INTERFACE( "CMCCCodecG711::~CMCCCodecG711" )
    }
    
// -----------------------------------------------------------------------------
// CMCCCodecG711::EnableVAD
// Enable / Disable VAD
// -----------------------------------------------------------------------------
//
TInt CMCCCodecG711::EnableVAD( TBool aEnableVAD )
    {
    iEnableVAD = aEnableVAD;
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CMCCCodecG711::SetBitrate
// Set Bitrate
// -----------------------------------------------------------------------------
//
TInt CMCCCodecG711::SetBitrate( TUint aBitrate )
    {
    if ( KG711Bitrate64 != aBitrate )
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
// CMCCCodecG711::SetSamplingFreq
// Set Sampling Frequency
// -----------------------------------------------------------------------------
//
TInt CMCCCodecG711::SetSamplingFreq( TUint32 aSamplingFreq )
    {
    if ( KG711SampleRate == aSamplingFreq )
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
// CMCCCodecG711::SetSdpName
// Set SDP Name
// -----------------------------------------------------------------------------
//
TInt CMCCCodecG711::SetSdpName( const TDesC8& aSdpName )
    {
    if ( !aSdpName.CompareF( KPCMUSdpName ) )
        {
        iSdpName.Copy( aSdpName );
        SetCodecMode( EPCMU );
        }
    else if ( !aSdpName.CompareF( KPCMASdpName ) )
        {
        iSdpName.Copy( aSdpName );
        SetCodecMode( EPCMA );
        }
    else 
        {
        return KErrNotSupported;        
        }
        
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMCCCodecG711::SetPayloadType
// Set PayloadType
// -----------------------------------------------------------------------------
//
TInt CMCCCodecG711::SetPayloadType( TUint8 aType )
    {
    if ( KPcmuPayloadType != aType && KPcmaPayloadType != aType ) 
        {
        return KErrNotSupported;
        }
    else 
        {
        iPayloadType = aType;
        }
    
    return KErrNone;
    }
 
// -----------------------------------------------------------------------------
// CMCCCodecG711::SetPTime
// Set PTime
// -----------------------------------------------------------------------------
//
TInt CMCCCodecG711::SetPTime( TUint aPTime )
    {
    if ( aPTime && ( KMinPtime <= aPTime ) && ( aPTime <= iMaxPTime ) && 
        ( KMaxPtime >= aPTime ) && (( aPTime % KMinPtime ) == 0 ))
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
// CMCCCodecG711::SetMaxPTime
// Set Max PTime
// -----------------------------------------------------------------------------
//
TInt CMCCCodecG711::SetMaxPTime( TUint aMaxPTime )
    {
    if ( aMaxPTime && ( iPTime <= aMaxPTime ) && ( KMinPtime <= aMaxPTime ) &&
        ( KMaxPtime >= aMaxPTime ) && (( aMaxPTime % KMinPtime ) == 0 ))
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
// CMCCCodecG711::SetCodecMode
// Set Codec Mode
// -----------------------------------------------------------------------------
//
TInt CMCCCodecG711::SetCodecMode( TCodecMode aCodecMode )
    {
    if ( EPCMU == aCodecMode )
        {
        iCodecMode = aCodecMode;
        iSdpName.Copy( KPCMUSdpName );
        SetPayloadType( KG711PayloadTypePCMU );
        return KErrNone;
        }
    else if ( EPCMA == aCodecMode )
        {
        iCodecMode = aCodecMode;
        iSdpName.Copy( KPCMASdpName );            
        SetPayloadType( KG711PayloadTypePCMA );  
        return KErrNone;
        }
    else
        {
        return KErrNotSupported;
        }
    }
    
// -----------------------------------------------------------------------------
// CMCCCodecG711::CloneDefaultsL
// Make a default setting clone from this G.711 codec
// -----------------------------------------------------------------------------
//
CMccCodecInformation* CMCCCodecG711::CloneDefaultsL()
    {
    CMccCodecInformation* codec = CMCCCodecG711::NewL();
    CleanupStack::PushL( codec );
    
    if ( 0 == this->SdpName().CompareF( KPCMASdpName ) )
        {
        User::LeaveIfError( codec->SetCodecMode( EPCMA ) );
        }
    else
        {
        User::LeaveIfError( codec->SetCodecMode( EPCMU ) );
        }
    
    CleanupStack::Pop( codec );
    return codec;
    }

// -----------------------------------------------------------------------------
// CMCCCodecG711::CloneDetailedL
// Make a detailed clone from this G.711 codec
// -----------------------------------------------------------------------------
//
CMccCodecInformation* CMCCCodecG711::CloneDetailedL()
    {
    CMCCCodecG711* newCodec = CMCCCodecG711::NewL();
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
// CMCCCodecG711::RequireSignalling
// -----------------------------------------------------------------------------
//
TBool CMCCCodecG711::RequireSignalling(
    const CMccCodecInformation& aCandidate ) const      
    {
    return CMccCodecInformation::RequireSignalling( aCandidate );
    }
        
// -----------------------------------------------------------------------------
// CMCPG711Codec::SetComfortNoiseGeneration
// -----------------------------------------------------------------------------
// 
TInt CMCCCodecG711::SetComfortNoiseGeneration( TUint8 aComfortNoisePT )
    {
    iComfortNoiseGenerationPt = aComfortNoisePT;
    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// CMCCCodecG711::SetAllowedBitrates
// -----------------------------------------------------------------------------
// 
TInt CMCCCodecG711::SetAllowedBitrates( TUint /*aBitrateMask*/ )
    {
    return KErrNone;           
    }
        
// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File
