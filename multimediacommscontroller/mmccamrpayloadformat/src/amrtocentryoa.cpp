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
#include "amrtocentryoa.h"
#include "amrpayloadformatter.h"
#include "streamformatter.h"
#include "amrcommonutil.h"

// CONSTANTS


// ---------------------------------------------------------------------------
// CAmrTocEntryOA::CAmrTocEntryOA
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CAmrTocEntryOA::CAmrTocEntryOA() : CAmrTocEntry()
    {
    DP_AMR_ENCODE( "CAmrTocEntryOA::CAmrTocEntryOA" );        
    }


// ---------------------------------------------------------------------------
// CAmrTocEntryOA::NewL
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CAmrTocEntryOA* CAmrTocEntryOA::NewL()
    {
    DP_AMR_ENCODE( "CAmrTocEntryOA::NewL" );
    
    CAmrTocEntryOA* self = new( ELeave ) CAmrTocEntryOA( );
    return self;
    }


// Destructor
CAmrTocEntryOA::~CAmrTocEntryOA()
    {
    DP_AMR_ENCODE( "CAmrTocEntryOA::~CAmrTocEntryOA" );
    }


// ---------------------------------------------------------------------------
// CAmrTocEntryOA::Encode
// Encode the `table of contents' ( TOC ) entry fields into a given buffer.
// ---------------------------------------------------------------------------
//
void CAmrTocEntryOA::Encode( TUint8* aBuffer, 
    TInt& aByteIndex, TInt& aBitIndex )
    {
    DP_AMR_ENCODE( "CAmrTocEntryOA::Encode" );
        
    //A ToC entry takes the following format in octet-aligned mode:
    //    0 1 2 3 4 5 6 7
    //   +-+-+-+-+-+-+-+-+
    //   |F|  FT   |Q|P|P|
    //   +-+-+-+-+-+-+-+-+
    //   P bits: padding bits, MUST be set to zero.
    
    // Call base-clase Encode function
    CAmrTocEntry::Encode( aBuffer, aByteIndex, aBitIndex );
    
    TStreamEncoder encoder;
    encoder.Initialize( aBuffer, aByteIndex, aBitIndex );
    encoder.Encode( 0, KNumValue2 ); // 2 Padding bits, must be ZERO

    // Update Byte and Bit positions
    aByteIndex = encoder.ByteIndex( );
    aBitIndex = encoder.BitIndex( );
    }


// ---------------------------------------------------------------------------
// CAmrTocEntryOA::Decode
// Decode the `table of contents' ( TOC ) entry fields from a given buffer.
// ---------------------------------------------------------------------------
//
void CAmrTocEntryOA::Decode( const TUint8* aBuffer, 
    TInt& aByteIndex, TInt& aBitIndex )
    {
    DP_AMR_ENCODE( "CAmrTocEntryOA::Decode" );
        
    // Call base-class Decode function
    CAmrTocEntry::Decode( aBuffer, aByteIndex, aBitIndex );

    TStreamDecoder decoder;
    decoder.Initialize( aBuffer, aByteIndex, aBitIndex );
    decoder.Decode( KNumValue2 ); // Skip 2 Padding bits

    // Update Byte and Bit positions
    aByteIndex = decoder.ByteIndex( );
    aBitIndex = decoder.BitIndex( );
    }

