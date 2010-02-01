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
* Description:    PayloadFormat plugin capable to read RTP payload containing
*                Amr audio.
*
*/




// INCLUDE FILES
#include "amrpayloadencoderoa.h"
#include "amrpayloadheaderoa.h"
#include "amrtocentryoa.h"


// ============================ MEMBER FUNCTIONS =============================
  
// ---------------------------------------------------------------------------
// CAmrPayloadEncoderOA::CAmrPayloadEncoderOA
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CAmrPayloadEncoderOA::CAmrPayloadEncoderOA( TBool aIsNb )
    : CAmrPayloadEncoder( aIsNb )
    {
    DP_AMR_ENCODE( "CAmrPayloadEncoderOA::CAmrPayloadEncoderOA" );
        
    CAmrPayloadEncoderOA::InitializeFrameEncoder( );
    }


// ---------------------------------------------------------------------------
// CAmrPayloadEncoderOA::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CAmrPayloadEncoderOA::ConstructL()
    {
    DP_AMR_ENCODE( "CAmrPayloadEncoderOA::ConstructL" );

    // Allocate memory for iHeaderEncoder and iTocEntryEncoder and set them
    // to NULL to avoid codescanner errors
    delete iHeaderEncoder;
    iHeaderEncoder = NULL;
    delete iTocEntryEncoder;
    iTocEntryEncoder = NULL;
    
    iHeaderEncoder = CAmrPayloadHeaderOA::NewL();
    iTocEntryEncoder = CAmrTocEntryOA::NewL();
    }


// ---------------------------------------------------------------------------
// CAmrPayloadEncoder::NewL
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CAmrPayloadEncoderOA* CAmrPayloadEncoderOA::NewL( TBool aIsNb )
    {
    DP_AMR_ENCODE( "CAmrPayloadEncoderOA::NewL" );
        
    CAmrPayloadEncoderOA* self = new( ELeave ) CAmrPayloadEncoderOA( aIsNb );
    
    CleanupStack::PushL( self );
    self->ConstructL( );
    CleanupStack::Pop( self );

    return self;
    }


// ---------------------------------------------------------------------------
// CAmrPayloadEncoderOA::~CAmrPayloadEncoderOA
// Destructor
// ---------------------------------------------------------------------------
//
CAmrPayloadEncoderOA::~CAmrPayloadEncoderOA()
    {
    DP_AMR_ENCODE( "CAmrPayloadEncoderOA::~CAmrPayloadEncoderOA" );
    }


// ---------------------------------------------------------------------------
// CAmrPayloadEncoderOA::EncodeFrame
// Encode a received Amr frame into the payload buffer.
// ---------------------------------------------------------------------------
//
TBool CAmrPayloadEncoderOA::EncodeFrame( TInt aChannel,
                                         TAmrFrameType aFrameType,
                                         TUint8 aFrameQualityInd,
                                         const TDes8& aFrameData )
    {
    DP_AMR_ENCODE( "CAmrPayloadEncoderOA::EncodeFrame" );
        
    TBool ret = CAmrPayloadEncoder::EncodeFrame( aChannel, aFrameType, 
                                                 aFrameQualityInd, aFrameData );
    if ( EFalse == ret )
        {
        // Encoding a frame data only. Payload Header and TOCs haven't been filled
        // Padding frame data to Byte boundary.
        if ( iFrameEncoder.BitIndex( ) != 0 )
            {
            iFrameEncoder.Encode( 0, ( KBitsIn1Byte - iFrameEncoder.BitIndex( ) ) );
            }
        }
    else
        {
        InitializeFrameEncoder();
        }

    return ret;
    }


// ---------------------------------------------------------------------------
// CAmrPayloadEncoderOA::SetChannelCount
// Set number of audio channels.
// ---------------------------------------------------------------------------
//
TInt CAmrPayloadEncoderOA::SetChannelCount( TInt aChannelCount )
    {
    DP_AMR_ENCODE( "CAmrPayloadEncoderOA::SetChannelCount" );
        
    TInt ret = CAmrPayloadFormatter::SetChannelCount( aChannelCount );
    InitializeFrameEncoder( );
    return ret;
    }


// ---------------------------------------------------------------------------
// CAmrPayloadEncoderOA::SetFrameBlockCount
// Set number of Amr frame blocks included in one RTP packet.
// ---------------------------------------------------------------------------
//
TInt CAmrPayloadEncoderOA::SetFrameBlockCount( TInt aFrameblockCount )
    {
    DP_AMR_ENCODE( "CAmrPayloadEncoderOA::SetFrameBlockCount" );
        
    TInt ret = CAmrPayloadFormatter::SetFrameBlockCount( aFrameblockCount );
    InitializeFrameEncoder();
    return ret;
    }


// ---------------------------------------------------------------------------
// CAmrPayloadEncoderOA::InitializeFrameEncoder
// Initialize frame encoder ( Stream Encoder ). Starting position of first 
// AMR frame data is calculated. Refer to RFC3267 4.4.
// ---------------------------------------------------------------------------
//
void CAmrPayloadEncoderOA::InitializeFrameEncoder()
    {
    DP_AMR_ENCODE( "CAmrPayloadEncoderOA::InitializeFrameEncoder" );    

    // Total number of TOCs: iFrameCount = iChannelCount * iFrameBlockCount
    // In OA mode, Bit Index always equals to Zero.
    TInt numOfRedFrames( 0 );
    if ( iRedIntervalCollected )
        {
        numOfRedFrames = iFrameDatas.Count();
        }

    TInt frameByteIndex = ( KHeaderBitsOA 
        + KTOCFieldBitsOA * ( iFrameCount + numOfRedFrames ) ) / KBitsIn1Byte;

    iFrameEncoder.Initialize( iPayload, frameByteIndex, 0 );
    }
