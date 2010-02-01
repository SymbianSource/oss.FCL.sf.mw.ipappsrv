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
#include "amrpayloaddecoderoa.h"
#include "amrcommonutil.h"

// ---------------------------------------------------------------------------
// CAmrPayloadDecoderOA::CAmrPayloadDecoderOA
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CAmrPayloadDecoderOA::CAmrPayloadDecoderOA( TBool aIsNb ) : CAmrPayloadDecoder( aIsNb )
    {
    DP_AMR_DECODE( "CAmrPayloadDecoderOA::CAmrPayloadDecoderOA" );
    }


// ---------------------------------------------------------------------------
// CAmrPayloadDecoderOA::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CAmrPayloadDecoderOA::ConstructL( )
    {
    DP_AMR_DECODE( "CAmrPayloadDecoderOA::ConstructL" );
        
    // Allocate memory for iHeaderDecoder and iTocEntryDecoder and
    // set them to NULL to avoid codescanner errors
    delete iHeaderDecoder;
    iHeaderDecoder = NULL;
    delete iTocEntryDecoder;
    iTocEntryDecoder = NULL;
    
    iHeaderDecoder = CAmrPayloadHeaderOA::NewL();
    iTocEntryDecoder = CAmrTocEntryOA::NewL();   
    }


// ---------------------------------------------------------------------------
// CAmrPayloadDecoderOA::NewL
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CAmrPayloadDecoderOA* CAmrPayloadDecoderOA::NewL( TBool aIsNb )
    {
    DP_AMR_DECODE( "CAmrPayloadDecoderOA::NewL" );
        
    CAmrPayloadDecoderOA* self = new( ELeave ) CAmrPayloadDecoderOA( aIsNb );
    
    CleanupStack::PushL( self );
    self->ConstructL( );
    CleanupStack::Pop( self );

    return self;
    }


// ---------------------------------------------------------------------------
// CAmrPayloadDecoderOA::~CAmrPayloadDecoderOA
// Destructor
// ---------------------------------------------------------------------------
//
CAmrPayloadDecoderOA::~CAmrPayloadDecoderOA()
    {
    DP_AMR_DECODE( "CAmrPayloadDecoderOA::~CAmrPayloadDecoderOA" );
    }


// ---------------------------------------------------------------------------
// CAmrPayloadDecoderOA::DecodePayload
// Decode payload data received in the payload buffer.
// ---------------------------------------------------------------------------
//
TInt CAmrPayloadDecoderOA::DecodePayload( TUint32& aTimeStamp, 
        TUint32 aTimeStampInc )
    {
    DP_AMR_DECODE( "CAmrPayloadDecoderOA::DecodePayloadL" );
    
    iTimeStamp = aTimeStamp;
    iTimeStampInc = aTimeStampInc;
    CAmrPayloadDecoder::Decode( iPayload, iPayloadSize );
    aTimeStamp = iTimeStamp;
    iFrameIndex = 0;
    return iDecodedFrames;
    }


// ---------------------------------------------------------------------------
// CAmrPayloadDecoderOA::CalculateFramePosition
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
void CAmrPayloadDecoderOA::CalculateFramePosition( const TUint8* aBuffer, 
    TInt aFrameCount )
    {
    DP_AMR_DECODE( "CAmrPayloadDecoderOA::CalculateFramePosition" );
        
    TUint bitCount;

    // Number of bytes for CMR and TOC entries
    TUint frameByteIndex = ( 1 + TUint( aFrameCount ) );
    if ( aFrameCount > KMaxFrameCountPerPacket )
   		{
       	DP_AMR_DECODE3("CAmrPayloadDecoderOA::CalculateFramePosition, aFrameCount: %d > KMaxFrameCountPerPacket: %d",  \
        	aFrameCount, KMaxFrameCountPerPacket);	
        }
    for ( TInt i = 0; i < aFrameCount; i++ )
        {   
 
        if ( EAmrFrameNoData == iFrames[i].iFrameType )
            {
            iFrames[i].iBitIndex = 0;
            iFrames[i].iFrameData = NULL;
            }
        else
            {
            // Set frame byte and bit positions
            iFrames[i].iBitIndex = 0; 
            iFrames[i].iFrameData = const_cast<TUint8*>( aBuffer ) + frameByteIndex;
            }

        bitCount = TUint( SpeechBitCount( iFrames[i].iFrameType ) );
        frameByteIndex += bitCount >> KNumValue3;
        if ( bitCount & KBitIndex7 )
            {
            frameByteIndex++;
            }
        }
    }





