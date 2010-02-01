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
*                Amr audio.
*
*/




// INCLUDE FILES
#include "amrpayloadheaderoa.h"
#include "streamformatter.h"
#include "amrcommonutil.h"

// CONSTANTS


// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CAmrPayloadHeader::CAmrPayloadHeader
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CAmrPayloadHeaderOA::CAmrPayloadHeaderOA() : CAmrPayloadHeader()
    {
    DP_AMR_ENCODE( "CAmrPayloadHeaderOA::CAmrPayloadHeaderOA" );        
    }


// ---------------------------------------------------------------------------
// CAmrPayloadHeaderOA::NewL
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CAmrPayloadHeaderOA* CAmrPayloadHeaderOA::NewL()
    {
    DP_AMR_ENCODE( "CAmrPayloadHeaderOA::NewL" );
    
    CAmrPayloadHeaderOA* self = new( ELeave ) CAmrPayloadHeaderOA( );

    return self;
    }


// Destructor
CAmrPayloadHeaderOA::~CAmrPayloadHeaderOA()
    {
    DP_AMR_ENCODE( "CAmrPayloadHeaderOA::~CAmrPayloadHeaderOA" );
    }
    
    
// ---------------------------------------------------------------------------
// CAmrPayloadHeaderOA::Encode
// Encode the payload header fields into a given buffer at the given position.
// ---------------------------------------------------------------------------
//
void CAmrPayloadHeaderOA::Encode( TUint8* aBuffer, 
        TInt& aByteIndex, TInt& aBitIndex )
    {
    DP_AMR_ENCODE( "CAmrPayloadHeaderOA::Encode" );
        
    //In octet-aligned mode, the payload header consists of a 4 bit CMR, 4
    //reserved bits, and optionally, an 8 bit interleaving header, as shown
    //below:
    //    0                   1
    //    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
    //   +-+-+-+-+-+-+-+-+- - - - - - - -
    //   |  CMR  |R|R|R|R|  ILL  |  ILP  |
    //   +-+-+-+-+-+-+-+-+- - - - - - - -
    // ILL and ILP are optional, PoC is not supporting these two fields.

    // Call base-class Encoder function
    CAmrPayloadHeader::Encode( aBuffer, aByteIndex, aBitIndex ); 

    TStreamEncoder encoder;
    encoder.Initialize( aBuffer, aByteIndex, aBitIndex );
    encoder.Encode( 0, KNumValue4 ); // 4 Reserved bits

    // Update Byte and Bit positions
    aByteIndex = encoder.ByteIndex( );
    aBitIndex = encoder.BitIndex( );
    }


// ---------------------------------------------------------------------------
// CAmrPayloadHeaderOA::Decode
// Decode the payload header from a given buffer at the given position.
// ---------------------------------------------------------------------------
//
void CAmrPayloadHeaderOA::Decode( const TUint8* aBuffer, 
    TInt& aByteIndex, TInt& aBitIndex )
    {
    DP_AMR_ENCODE( "CAmrPayloadHeaderOA::Decode" );
        
    // Call base-class Decoder function
    CAmrPayloadHeader::Decode( aBuffer, aByteIndex, aBitIndex );

    TStreamDecoder decoder;
    decoder.Initialize( aBuffer, aByteIndex, aBitIndex );
    decoder.Decode( KNumValue4 );    // skip 4 Reserved bits

    // Update Byte and Bit positions
    aByteIndex = decoder.ByteIndex( );
    aBitIndex = decoder.BitIndex( );
    }
