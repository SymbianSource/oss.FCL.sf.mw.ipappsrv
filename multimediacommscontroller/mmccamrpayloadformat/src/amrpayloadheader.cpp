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
#include "amrpayloadheader.h"
#include "streamformatter.h"

// CONSTANTS


// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// ---------------------------------------------------------------------------
// CAmrPayloadHeader::CAmrPayloadHeader
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CAmrPayloadHeader::CAmrPayloadHeader() :iCmr( EAmrModeReqNone )
    {
    DP_AMR_ENCODE( "CAmrPayloadHeader::CAmrPayloadHeader" );        
    }


// ---------------------------------------------------------------------------
// CAmrPayloadHeader::NewL
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CAmrPayloadHeader* CAmrPayloadHeader::NewL()
    {
    DP_AMR_ENCODE( "CAmrPayloadHeader::NewL" );
    
    CAmrPayloadHeader* self = new( ELeave ) CAmrPayloadHeader( );
    
    return self;
    }


// Destructor
CAmrPayloadHeader::~CAmrPayloadHeader()
    {
    DP_AMR_ENCODE( "CAmrPayloadHeader::~CAmrPayloadHeader" );
    }


// ---------------------------------------------------------------------------
// CAmrPayloadHeader::Encode
// Encode the payload header fields into a given buffer at the given position.
// ---------------------------------------------------------------------------
//
void CAmrPayloadHeader::Encode( TUint8* aBuffer,
        TInt& aByteIndex, TInt& aBitIndex )
    {
    DP_AMR_ENCODE( "CAmrPayloadHeader::Encode" );
        
    TStreamEncoder encoder;
    encoder.Initialize( aBuffer, aByteIndex, aBitIndex );
    
    //In bandwidth-efficient mode, the payload header simply consists of a
    //4 bit codec mode request:
    //  0 1 2 3
    // +-+-+-+-+
    // |  CMR  |
    // +-+-+-+-+
    encoder.Encode( iCmr, KCMRFieldBits );

    // Update Byte and Bit positions
    aByteIndex = encoder.ByteIndex( ); 
    aBitIndex = encoder.BitIndex( );
    }


// ---------------------------------------------------------------------------
// CAmrPayloadHeader::Decode
// Decode the payload header from a given buffer at the given position.
// ---------------------------------------------------------------------------
//
void CAmrPayloadHeader::Decode( const TUint8* aBuffer,
        TInt& aByteIndex, TInt& aBitIndex )
    {
    DP_AMR_ENCODE( "CAmrPayloadHeader::Decode" );
    
    TStreamDecoder decoder;
    decoder.Initialize( aBuffer, aByteIndex, aBitIndex );
    iCmr = TAmrModeRequest( decoder.Decode( KCMRFieldBits ) );

    // Update Byte and Bit positions
    aByteIndex = decoder.ByteIndex( );
    aBitIndex = decoder.BitIndex( );
    }

