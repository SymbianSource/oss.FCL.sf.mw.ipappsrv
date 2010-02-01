/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies).
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
*                AMR audio.
*
*/




// INCLUDE FILES
#include "amrcommonutil.h"

// LOCAL CONSTANTS AND MACROS

// The size of AMR header, header must include bits for determining frame length
const TInt KRawAmrFrameHeaderSize = 1;

// Frame length table ( number of frame bytes )
const TInt KRawAmrNbFrameLength[16] = { 13,14,16,18,20,21,27,32,6,0,0,0,0,0,0,1 };

// Stuffing length table ( number of stuffing bits )
const TInt KRawAmrNbStuffLength[16] = { 4, 4, 5, 5, 7, 4, 7, 7, 4,0,0,0,0,0,0,3 };

// Maximum sampling rate
const TInt KRawAmrNbMaxSampleRate = 8000; 

// Maximum number of channels
const TInt KRawAmrMaxChannels = 1;

// Maximum number of PCM samples in one AMR frame
const TInt KRawAmrNbMaxSamplesPerFrame = 160;

// Frame length table ( number of frame bytes )
const TInt KRawAmrWbFrameLength[16] = { 18,24,33,37,41,47,51,59,61,6,0,0,0,0,0,1 };

// Stuffing length table ( number of stuffing bits )
const TInt KRawAmrWbStuffLength[16] = {  4, 7, 3, 3, 3, 3, 3, 3, 3,0,0,0,0,0,0,3 };

// Maximum sampling rate
const TInt KRawAmrWbMaxSampleRate = 16000; 

// Maximum number of PCM samples in one AMR-WB frame
const TInt KRawAmrWbMaxSamplesPerFrame = 320;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAmrCommonUtility::FrameInfo
//
// Calculates the frame size and returns information about the frame which
// starts from the beginning of aBuf. Returns 0, if the frame bit syntax is
// incorrect or not enough bits are available for calculating the frame size
// and the frame parameters ( aBufLen < TAmrCodecParams::FrameHeaderSize ).
// 
// The frame parameters returned via aInfo are: mode, sampling frequency,
// number of channels, frame size ( same as return value ) and number of
// samples in this frame.
// -----------------------------------------------------------------------------
//
TInt CAmrCommonUtility::FrameInfo( const TUint8* aBuf, TInt aBufLen,
        TAmrFrameInfo& aInfo, TBool aIsNb )
    {
    TInt length = 0;
    aInfo.iBitrate = 0;
    
    if( aBufLen >= KRawAmrFrameHeaderSize )
        {
        // extract mode information
        const TInt mode = ( aBuf[0] & 0x78 ) >> 3; // 1st byte 0b.MODE...
        // get length
        length =
          ( aIsNb ? KRawAmrNbFrameLength[mode] : KRawAmrWbFrameLength[mode] );

        // check start stuffing bits
        if( ( aBuf[0] & 0x83 ) != 0 )
            {
            length = 0; // syntax error
            }
            
        // check end stuffing bits
        if( length > 0  &&  aBufLen >= length )
            {
            TUint32 stuffBits = aBuf[length-1];
            TInt stuffLength =
              ( aIsNb ? KRawAmrNbStuffLength[mode] : KRawAmrWbStuffLength[mode] );
            stuffBits <<= ( KNumValue11 - stuffLength );
            if( ( stuffBits & 0x0000FF ) != 0 )
                {
                length = 0; // syntax error
                }
            }

        // update frame parameters
        aInfo.iMode = mode;
        // Frames are 20 ms long -> 50 frames/s. Frame length is in 8 bit bytes.
        // Hence the number 400. This same for both nb and wb.
        aInfo.iBitrate = length * KNumValue400;
        aInfo.iSamplingRate =
          ( aIsNb ? KRawAmrNbMaxSampleRate : KRawAmrWbMaxSampleRate );
        aInfo.iChannels = KRawAmrMaxChannels;
        aInfo.iFrameSize = length;
        aInfo.iFrameSamples =
          ( aIsNb ? KRawAmrNbMaxSamplesPerFrame : KRawAmrWbMaxSamplesPerFrame );
        aInfo.iSamplingRateOut = aInfo.iSamplingRate;
        aInfo.iChannelsOut = aInfo.iChannels;
        aInfo.iFrameSamplesOut= aInfo.iFrameSamples;
        }
        
    return length;
    }

// -----------------------------------------------------------------------------
// CAmrCommonUtility::SeekSync
//
// This routine seeks the start position of the next frame and returns
// the byte position of its header. Returns aBufLen, if no valid frame
// can not be found ( see FrameInfo ). The seek progresses from the start
// of aBuf ( 0 ) toward the end of aBuf( aBufLen - 1 ).
//    
// The level of syntax check depends on the number of bits available. At
// minimum the first frame header bits are checked only, but if more
// bits are available, they can be used to make the sync seek more robust.
// For succesfull seek the whole frame does not need to exist in aBuf.
// -----------------------------------------------------------------------------
//
TInt CAmrCommonUtility::SeekSync( const TUint8* aBuf, TInt aBufLen, TBool aIsNb )
    {
    const TInt KMaxFrames = 3;          // number of frames to check
    const TInt KNotFound = aBufLen;     // sync not found position
    TAmrFrameInfo frameInfo;            // frame parameters
    
    TInt i = 0;
    TInt syncPos = KNotFound;
    TInt maxSeek = KMaxFrames;
    const TUint8* endPtr = aBuf + aBufLen;
    
    // Seek a valid frame candidate byte by byte until a valid frame
    // is found or all bytes have been checked.
    while( aBuf < endPtr && syncPos == KNotFound )
        {
        TInt seekCount = 0;
        const TUint8* framePtr = aBuf;
        TInt frameBufLen = aBufLen;
        syncPos = i;
        // Check the validity of this frame candidate and the nearest next
        // frames. If they are not OK, syncPos will be set to KNotFound.
        while( framePtr < endPtr && syncPos != KNotFound && seekCount < maxSeek )
            {
            TInt length = FrameInfo( framePtr, frameBufLen, frameInfo, aIsNb );
            
            if( frameBufLen >= KRawAmrFrameHeaderSize && length == 0 )
                {
                syncPos = KNotFound;
                }

            framePtr += length;
            frameBufLen -= length;
            seekCount++;
            }
            
        aBuf++;
        aBufLen--;
        i++;
        }
        
    return syncPos;
    };

// -----------------------------------------------------------------------------
// CAmrCommonUtility::FrameSize
// Returns the size of frame described by given coding parameters. The only
// parameter used in calculation is the coding mode ( iMode ).
// -----------------------------------------------------------------------------
//
TInt CAmrCommonUtility::FrameSize( const TAmrFrameInfo& aInfo, TBool aIsNb )
    {
    if ( aIsNb )
        {
        return KRawAmrNbFrameLength[ aInfo.iMode ];
        }
    else        
        {
        return KRawAmrWbFrameLength[ aInfo.iMode ];
        }
    };

// -----------------------------------------------------------------------------
// CAmrCommonUtility::FrameHeaderSize
// Returns the size of one AMR frame header in bytes. The header must include
// all bits needed for determining the actual frame length.
// -----------------------------------------------------------------------------
//
TInt CAmrCommonUtility::FrameHeaderSize( )
    {
    return KRawAmrFrameHeaderSize; 
    };

// ================= OTHER EXPORTED FUNCTIONS ==============

//  End of File  

