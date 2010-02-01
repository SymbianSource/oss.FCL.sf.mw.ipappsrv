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
* Description:    Decoder capable to handle redundancy RTP payload.
*
*/




// INCLUDE FILES
#include    "mccreddecoder.h"
#include    "streamformatter.h"
#include    "mccredpayloadformatdefs.h"
#include    "mccdef.h"

const TInt KNumValue3 = 3;
const TInt KNumValue4 = 4;

// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccRedDecoder::CMccRedDecoder
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMccRedDecoder::CMccRedDecoder()
    {
    DP_RED_DECODE( "CMccRedDecoder::CMccRedDecoder" )
    }

// -----------------------------------------------------------------------------
// CMccRedDecoder::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMccRedDecoder* CMccRedDecoder::NewL()
    {
    DP_RED_DECODE( "CMccRedDecoder::NewL" )
    
    CMccRedDecoder* self = new( ELeave ) CMccRedDecoder;
    return self;
    }

// Destructor
CMccRedDecoder::~CMccRedDecoder()
    {
    DP_RED_DECODE( "CMccRedDecoder::~CMccRedDecoder" )
    
    iRedHeaderInfo.Close();
    }

// -----------------------------------------------------------------------------
// CMCCRedDecoder::DoInitializeL
// Initialize decoder after encoding parameters are changed or decoding is 
// started again after a pause.
// -----------------------------------------------------------------------------
//
void CMccRedDecoder::DoInitializeL( TInt aRedBlockCount,
                                    TInt /*aMaxPayloadSize*/,
                                    TInt /*aNumOfEncodings*/ )
    {
    DP_RED_DECODE( "CMccRedDecoder::DoInitializeL" )

    if ( KMaxRedCount >= aRedBlockCount )
        {
        iRedBlockCount = aRedBlockCount;
        iCurTimeStamp = 0;
        iLatestTimeStamp = -1;
        }
    else
        {
        User::Leave( KErrArgument );
        }
    }
    
// -----------------------------------------------------------------------------
// CMccRedDecoder::DecodePayload
// Decode encoding(s) from RTP payload given.
// -----------------------------------------------------------------------------
//
TInt CMccRedDecoder::DecodePayload( TUint aCurTimeStamp )
    {
    DP_RED_DECODE( "CMccRedDecoder::DecodePayload" )
    
    iCurTimeStamp = aCurTimeStamp;
    iRedHeaderInfo.Reset();
    iEncodings[EMccPrimaryEncoding]->Data().Zero();
    
    TInt error = GetBlockInfos( iRedHeaderInfo, iRTPPayload->Data() );
    if ( KErrNone == error )
        {
        // Ignore already received blocks
        while ( iRedHeaderInfo.Count() 
                && static_cast<TInt>( iRedHeaderInfo[0].iTimeStamp ) <= iLatestTimeStamp )
            {
            DP_RED_ENCODE2( "BLOCK with TIMESTAMP %d REMOVED",
                iRedHeaderInfo[0].iTimeStamp )
            iRedHeaderInfo.Remove( 0 );
            }
            
        // Separate encodings
        for ( TInt i = 0; i < iRedHeaderInfo.Count(); i++ )
            {
            if ( iRedHeaderInfo[i].iBlockPT 
                == iPayloadTypes[EMccPrimaryEncoding] )
                {
                TDes8& primaryEnc = iEncodings[EMccPrimaryEncoding]->Data();
                primaryEnc.Append( iRedHeaderInfo[i].iBlockIndex,
                                   iRedHeaderInfo[i].iBlockLength );
                }
            else if ( iRedHeaderInfo[i].iBlockPT 
                        == iPayloadTypes[EMccSecondaryEncoding] )
                {
                TDes8& secondaryEnc = iEncodings[EMccSecondaryEncoding]->Data();
                secondaryEnc.Append( iRedHeaderInfo[i].iBlockIndex,
                                     iRedHeaderInfo[i].iBlockLength );
                }
            else if ( iRedHeaderInfo[i].iBlockPT == KComfortNoisePT )                
                {
                DP_RED_DECODE( "CMccRedDecoder::DecodePayload() - CN" )
                TDes8& primaryEnc = iEncodings[EMccPrimaryEncoding]->Data();
                primaryEnc.Append( iRedHeaderInfo[i].iBlockIndex,
                                   iRedHeaderInfo[i].iBlockLength );
                }
            else
                {
                DP_RED_ENCODE( "NOT SUPPORTED ENCODING, SKIPPED" )
                }                
            }
            
        iLatestTimeStamp = aCurTimeStamp;
        return iRedHeaderInfo.Count();
        }
    else
        {
        DP_RED_DECODE( "CMccRedDecoder::DecodePayload() - \
            ERROR with GetBlockInfos" )
        
        iEncodings[EMccPrimaryEncoding]->Data().Zero();
        return error;
        }        
    }


// -----------------------------------------------------------------------------
// CMccRedDecoder::GetBlockInfos
// Get block indexes. Redundant blocks are ignored based on timestamp.
// Timestamp of the primary data block is saved. Data validity is verified by
// checking that decoded block indexes are inside bounds and overall length of
// decoded data blocks is not greater than RPT payload length. Time stamps
// are also checked that they are in increasing order.
// -----------------------------------------------------------------------------
//
TInt CMccRedDecoder::GetBlockInfos( RArray<TRedHeaderInfo>& aBlockInfos,
                                    const TDesC8& aRTPPayload )
    {
    DP_RED_DECODE( "CMccRedDecoder::GetBlockInfos" )
    
    if ( 0 == aRTPPayload.Size() )
        {
        return KErrArgument;
        }
    
    const TUint8* seekPtr = aRTPPayload.Ptr();
    
    TStreamDecoder streamDecoder;
    streamDecoder.Initialize( const_cast<TUint8*>( seekPtr ), 0, 0 );
    
    TBool FBitSet( ETrue );
    TInt allBlockLength(0); // For data validity check
    
    // Go through redundancy headers and count audio frame blocks
    // Determine number of redundant frame blocks to ignore based on timestamp
    TUint frameBlockCount(0);
    
    while ( FBitSet )
        {
        frameBlockCount++;
        TRedHeaderInfo curInfo;
        
        FBitSet = static_cast<TBool>( streamDecoder.Decode( KFBitLength ) );
        curInfo.iBlockPT = static_cast<TUint8>( streamDecoder.Decode( KPTFieldBits ) );

        if ( FBitSet )
            {
            TUint offset = streamDecoder.Decode( KTSOffsetBits );
            curInfo.iTimeStamp = iCurTimeStamp - offset;
            curInfo.iBlockLength = streamDecoder.Decode( KBlockLengthBits );
            
            allBlockLength += curInfo.iBlockLength;
                    
            DP_RED_DECODE6( "DEC RED_HEADER NUM %d: F:%d PT:%d \
            TIMESTAMP:%d, BL:%d", frameBlockCount, ETrue, curInfo.iBlockPT,
                curInfo.iTimeStamp, curInfo.iBlockLength )
            
            // Jump to the beginning of next redundancy header
            streamDecoder.Initialize( const_cast<TUint8*>( seekPtr ), 
                frameBlockCount * KNumValue4, 0 );
            }
        else
            {
            // Primary data block's time stamp
            curInfo.iTimeStamp = iCurTimeStamp;
            DP_RED_DECODE5( "DEC FINAL RED_HEADER %d: F:%d PT:%d \
            #TIMESTAMP#:%d", frameBlockCount, 0, curInfo.iBlockPT,
                curInfo.iTimeStamp )
            }
        
        TInt err = aBlockInfos.Append( curInfo );
        if ( err )
            {
            return err;
            }
        }
        
    // Calculate frame block indexes
    frameBlockCount = aBlockInfos.Count();
    TUint8* firstBlockIndex = ( const_cast<TUint8*>( seekPtr ) 
        + frameBlockCount * KRedHeaderSize - KNumValue3 );
    aBlockInfos[0].iBlockIndex = firstBlockIndex;

    for( TUint i = 1; i < frameBlockCount; i++ )
        {
        aBlockInfos[i].iBlockIndex 
            = firstBlockIndex + aBlockInfos[i - 1].iBlockLength;
        }

    // Length of the last block
    const TUint8* endPtr = aRTPPayload.Ptr() + aRTPPayload.Length();
    TInt lastIndex( aBlockInfos.Count() - 1 );
    aBlockInfos[lastIndex].iBlockLength = 
        endPtr - aBlockInfos[lastIndex].iBlockIndex;
    
    allBlockLength += aBlockInfos[lastIndex].iBlockLength;
    
    // Check data validity
    for ( TUint i = 0; i < frameBlockCount - 1; i++ )
        {
        // Check time stamps contiguousness
        if ( aBlockInfos[i].iTimeStamp > aBlockInfos[i+1].iTimeStamp )
            {
            return KErrCorrupt;
            }
        
        // Check index bounds
        if ( aBlockInfos[i].iBlockIndex < firstBlockIndex
            || aBlockInfos[i].iBlockIndex > endPtr )
            {
            return KErrCorrupt;
            }      
        
        TUint plLen = static_cast<TUint>( aRTPPayload.Length() );
        if ( aBlockInfos[i].iBlockLength < 1 || aBlockInfos[i+1].iBlockLength < 1
            || plLen < aBlockInfos[i].iBlockLength )  
            {
            return KErrCorrupt;
            }
        }
    
    // Check overall block lengths
    if ( aRTPPayload.Length() < allBlockLength || 0 > allBlockLength )
        {
        return KErrCorrupt;
        }
    else
        {
        return KErrNone;
        }
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File  
