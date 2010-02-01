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
#include "amrtocentry.h"
#include "amrpayloadformatter.h"
#include "streamformatter.h"

// CONSTANTS
const TUint KFTFieldBits = 4;
const TUint KFrameFollowedBits = 1;
const TUint KQualityIndBits = 1;


// ---------------------------------------------------------------------------
// CAmrTocEntry::CAmrTocEntry
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CAmrTocEntry::CAmrTocEntry() : iFrameFollowed( 1 ),
    iFrameType( EAmrFrame5_15 ), iFrameQualityInd( 1 ) 
    {
    DP_AMR_ENCODE( "CAmrTocEntry::CAmrTocEntry" );        
    }


// ---------------------------------------------------------------------------
// CAmrTocEntry::NewL
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CAmrTocEntry* CAmrTocEntry::NewL()
    {
    DP_AMR_ENCODE( "CAmrTocEntry::NewL" );   
    CAmrTocEntry* self = new( ELeave ) CAmrTocEntry( );    
    return self;
    }


// Destructor
CAmrTocEntry::~CAmrTocEntry()
    {
    DP_AMR_ENCODE( "CAmrTocEntry::~CAmrTocEntry" );
    }


// ---------------------------------------------------------------------------
// CAmrTocEntry::Encode
// Encode the `table of contents' ( TOC ) entry fields into a given buffer.
// ---------------------------------------------------------------------------
//
void CAmrTocEntry::Encode( TUint8* aBuffer,
        TInt& aByteIndex, TInt& aBitIndex )
    {
    DP_AMR_ENCODE( "CAmrTocEntry::Encode" );
        
    TStreamEncoder encoder;
    encoder.Initialize( aBuffer, aByteIndex, aBitIndex );
    
    // In bandwidth-efficient mode, a TOC entry takes the following format
    //   0 1 2 3 4 5
    //  +-+-+-+-+-+-+
    //  |F|  FT   |Q|
    //  +-+-+-+-+-+-+
    encoder.Encode( iFrameFollowed, KFrameFollowedBits );
    encoder.Encode( iFrameType, KFTFieldBits );
    encoder.Encode( iFrameQualityInd, KQualityIndBits );

    // Update Byte and Bit positions
    aByteIndex = encoder.ByteIndex( );
    aBitIndex = encoder.BitIndex( );
    }


// ---------------------------------------------------------------------------
// CAmrTocEntry::Decode
// Decode the `table of contents' ( TOC ) entry fields from a given buffer at
// the given position.
// ---------------------------------------------------------------------------
//
void CAmrTocEntry::Decode( const TUint8* aBuffer, 
        TInt& aByteIndex, TInt& aBitIndex )
    {
    DP_AMR_ENCODE( "CAmrTocEntry::Decode" );
        
    TStreamDecoder decoder;
    decoder.Initialize( aBuffer, aByteIndex, aBitIndex );

    // In bandwidth-efficient mode, a TOC entry takes the following format
    //   0 1 2 3 4 5
    //  +-+-+-+-+-+-+
    //  |F|  FT   |Q|
    //  +-+-+-+-+-+-+
    iFrameFollowed = TUint8( decoder.Decode( KFrameFollowedBits ) );
    iFrameType = TAmrFrameType( decoder.Decode( KFTFieldBits ) );
    iFrameQualityInd = TUint8( decoder.Decode( KQualityIndBits ) );

    // Update Byte and Bit positions
    aByteIndex = decoder.ByteIndex( );
    aBitIndex = decoder.BitIndex( );
    }
