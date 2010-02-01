/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Encoder implementation capable to form redundancy RTP payload.
*
*/





// INCLUDE FILES
#include    "mccredencoder.h"
#include    "streamformatter.h"
#include    "mccredpayloadformatdefs.h"
#include    "mccdef.h"

// CONSTANTS
const TUint KPrimaryEncPTInd = 0;

// ============================= LOCAL FUNCTIONS ===============================


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccRedEncoder::CMccRedEncoder
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMccRedEncoder::CMccRedEncoder()
    {
    DP_RED_ENCODE( "CMccRedEncoder::CMccRedEncoder" )
    }

// -----------------------------------------------------------------------------
// CMccRedEncoder::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMccRedEncoder::ConstructL()
    {
    DP_RED_ENCODE( "CMccRedEncoder::ConstructL" )
    
    iRedHeaders 
        = HBufC8::NewL( KDefRedCount * KRedHeaderSize + KFinalHeaderSize );
    }

// -----------------------------------------------------------------------------
// CMccRedEncoder::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMccRedEncoder* CMccRedEncoder::NewL()
    {
    DP_RED_ENCODE( "CMccRedEncoder::NewL" )
        
    CMccRedEncoder* self = new( ELeave ) CMccRedEncoder;
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

    
// Destructor
CMccRedEncoder::~CMccRedEncoder()
    {
    DP_RED_ENCODE( "CMccRedEncoder::~CMccRedEncoder" )
        
    iRedTimeStamps.Close();
    delete iRedHeaders;
    delete iRedPayload;
    }


// -----------------------------------------------------------------------------
// CMCCRedEncoder::DoInitializeL
// Initialize encoder after encoding parameters are changed or encoding is
// started again after pause.
// -----------------------------------------------------------------------------
//
void CMccRedEncoder::DoInitializeL( TInt aRedBlockCount, TInt aMaxPayloadSize,
    TInt /*aNumOfEncodings*/ )
    {
    DP_RED_ENCODE( "CMccRedEncoder::DoInitializeL" )

    if ( KMaxRedCount < aRedBlockCount || 0 > aRedBlockCount )
        {
        User::Leave( KErrArgument );
        }
        
    if ( KDefRedCount != aRedBlockCount )
        {
        delete iRedHeaders;
        iRedHeaders = NULL;
        iRedHeaders = HBufC8::NewL( 
            aRedBlockCount * KRedHeaderSize + KFinalHeaderSize );
        }

    // Memory for redundant payloads without redundancy headers
    TInt redPayloadSize( aRedBlockCount * aMaxPayloadSize );
    if ( iRedPayload )
        {
        if ( iRedPayload->Data().MaxSize() < redPayloadSize )
            {
            delete iRedPayload;
            iRedPayload = NULL;
            iRedPayload = CMMFDataBuffer::NewL( redPayloadSize );
            }
        }
    else
        {
        iRedPayload = CMMFDataBuffer::NewL( redPayloadSize );
        }
    
    iRedPackets = EFalse;
    iRedTimeStamps.Reset();
    iRedHeaders->Des().Zero();
    }


// -----------------------------------------------------------------------------
// CMccRedEncoder::EncodePayloadL
// Encode payloads set with SetEncoding() -method.
// -----------------------------------------------------------------------------
//    
void CMccRedEncoder::EncodePayloadL( TUint aCurTimeStamp )
    {
    DP_RED_ENCODE( "CMccRedEncoder::EncodePayload" )
    __ASSERT_ALWAYS( iRTPPayload, User::Leave( KErrNotReady ) );
    
    TDes8& rtpPayload = iRTPPayload->Data();
    rtpPayload.Zero();
    
    // Copy redundant payload if exist
    if ( iRedPackets )
        {
        rtpPayload.Append( iRedPayload->Data() );
        }
    
    // Append new payload to the end of redundancy RTP payload
    rtpPayload.Append( iEncodings[EMccPrimaryEncoding]->Data() );
    iRedTimeStamps.AppendL( aCurTimeStamp );
    
    ConstructRedHeaders( aCurTimeStamp );
    rtpPayload.Insert( 0, *iRedHeaders );
    
    // Save redundant data block
    iRedPayload->Data().Copy( iEncodings[EMccPrimaryEncoding]->Data() );
    iRedPackets = ETrue;
    }


// -----------------------------------------------------------------------------
// CMccRedEncoder::ConstructRedHeaders
// Construct redundancy headers.
// -----------------------------------------------------------------------------
//        
void CMccRedEncoder::ConstructRedHeaders( TUint aCurTimeStamp )
    {
    DP_RED_ENCODE( "CMccRedEncoder::ConstructRedHeaders" )
        
    // Construct redundancy headers, final header is one byte long
    iRedHeaders->Des().Zero();
    const TUint8* seekPtr = iRedHeaders->Ptr();

    TStreamEncoder streamEncoder;
    streamEncoder.Initialize( const_cast<TUint8*>( seekPtr ), 0, 0 );
    
    // Encode headers except final header
    if ( 1 < iRedTimeStamps.Count() )
        {
        iRedHeaders->Des().SetLength( 
            iRedBlockCount * KRedHeaderSize + KFinalHeaderSize );
        
        for ( TInt i = 0; i < iRedBlockCount; i++ )
            {
            // F-bit            
            streamEncoder.Encode( 1, KFBitLength );
            // PT
            streamEncoder.Encode( iPayloadTypes[KPrimaryEncPTInd], KPTFieldBits );
            // Calculate timestamp offset
            TUint offset = aCurTimeStamp - iRedTimeStamps[i];
            // Timestamp offset
            streamEncoder.Encode( offset, KTSOffsetBits );
            // Block length
            streamEncoder.Encode( iRedPayload->Data().Length(), KBlockLengthBits );
            
            DP_RED_ENCODE6( "ENC RED_HEADER NUM %d: F:%d PT:%d TIMESTAMP:%d, BL:%d", 
                i, ETrue, iPayloadTypes[KPrimaryEncPTInd], iRedTimeStamps[i], 
                iRedPayload->Data().Length() )
            }    
        }
    else
        {
        iRedHeaders->Des().SetLength( KFinalHeaderSize );            
        }            

    // Encode final header
    streamEncoder.Encode( 0, KFinalHeaderSize );
    streamEncoder.Encode( iPayloadTypes[KPrimaryEncPTInd], KPTFieldBits );
    
    DP_RED_ENCODE5( "ENC FINAL RED_HEADER %d: F:%d PT:%d #TIMESTAMP#:%d",
        iRedBlockCount, 0, iPayloadTypes[KPrimaryEncPTInd], aCurTimeStamp )
    
    // Delete oldest time stamp
    if ( iRedTimeStamps.Count() > iRedBlockCount )
        {
        iRedTimeStamps.Remove( 0 );
        }
    }
    

// ========================== OTHER EXPORTED FUNCTIONS =========================


//  End of File  
