/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies).
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
#include "amrpayloaddecoder.h"
#include "amrcommonutil.h"
#include "amrpayloadheader.h"
#include "amrtocentry.h"

// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAmrPayloadDecoder::CAmrPayloadDecoder
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CAmrPayloadDecoder::CAmrPayloadDecoder( TBool aIsNb ) : CAmrPayloadFormatter( aIsNb )
    {
   	DP_AMR_DECODE ("CAmrPayloadDecoder::CAmrPayloadDecoder");
    }


// -----------------------------------------------------------------------------
// CAmrPayloadEncoder::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CAmrPayloadDecoder::ConstructL( )
    {
    DP_AMR_DECODE( "CAmrPayloadDecoder::ConstructL" );
        
    // Allocate memory for iHeaderDecoder and iTocEntryDecoder
    iHeaderDecoder = CAmrPayloadHeader::NewL();
    iTocEntryDecoder = CAmrTocEntry::NewL();
    
    #ifdef FTD_ENABLED
    
    User::LeaveIfError( iCodecStatsQueue.OpenGlobal( KMccCodecStats, 
        EOwnerProcess ) );   
    User::LeaveIfError( iJBufStatsQueue.OpenGlobal( KMccJBufferStats, 
        EOwnerProcess ) );    
    
    #endif
    }


// -----------------------------------------------------------------------------
// CAmrPayloadDecoder::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CAmrPayloadDecoder* CAmrPayloadDecoder::NewL( TBool aIsNb )
    {
    DP_AMR_DECODE( "CAmrPayloadDecoder::NewL" );
  
    CAmrPayloadDecoder* self = new( ELeave ) CAmrPayloadDecoder( aIsNb );
    
    CleanupStack::PushL( self );
    self->ConstructL( );
    CleanupStack::Pop( self );

    return self;
    }

// -----------------------------------------------------------------------------
// CAmrPayloadDecoder::~CAmrPayloadDecoder
// Destructor
// -----------------------------------------------------------------------------
//
CAmrPayloadDecoder::~CAmrPayloadDecoder( )
    {
    DP_AMR_DECODE( "CAmrPayloadDecoder::~CAmrPayloadDecoder" );

    delete iHeaderDecoder;
    delete iTocEntryDecoder;

    #ifdef FTD_ENABLED
    
    iCodecStatsQueue.Close();
    iJBufStatsQueue.Close();
    
    #endif
    }

// -----------------------------------------------------------------------------
// CAmrPayloadDecoder::DoInitialize
// Initialize decoder. Decoder should be initialized when starting playing.
// -----------------------------------------------------------------------------
//
void CAmrPayloadDecoder::DoInitialize()
    {
    DP_AMR_DECODE( "CAmrPayloadDecoder::DoInitialize" );

    }
    
// -----------------------------------------------------------------------------
// CAmrPayloadDecoder::DecodePayload
// Decode payload data received in the payload buffer.
// If AMR FEC is used and redundant frames are discarded, time stamp is
// increased to correspond start of remaining frames.
// -----------------------------------------------------------------------------
//
TInt CAmrPayloadDecoder::DecodePayload( TUint32& aTimeStamp, TUint32 aTimeStampInc )
    {
    DP_AMR_DECODE( "CAmrPayloadDecoder::DecodePayload" );
       
    iTimeStamp = aTimeStamp;
    iTimeStampInc = aTimeStampInc;
    Decode( iPayload, iPayloadSize );
    aTimeStamp = iTimeStamp;
    iFrameIndex = 0;
    return iDecodedFrames;
    }


// -----------------------------------------------------------------------------
// CAmrPayloadDecoder::ModeRequest
// Get AMR codec mode request ( CMR ) received in the payload.
// -----------------------------------------------------------------------------
//
TAmrModeRequest CAmrPayloadDecoder::ModeRequest() const
    {
    DP_AMR_DECODE( "CAmrPayloadDecoder::ModeRequest" );
       
    return iModeRequest;
    }


// -----------------------------------------------------------------------------
// CAmrPayloadDecoder::Frames
// Get AMR frames decoded from the payload.
// -----------------------------------------------------------------------------
//
TAmrFrame* CAmrPayloadDecoder::Frames( )
    {
    DP_AMR_DECODE( "CAmrPayloadDecoder::Frames" );
   
    return iFrames;
    }

// -----------------------------------------------------------------------------
// CAmrPayloadDecoder::CompareBuffers
// Compares contents of two buffers.
// -----------------------------------------------------------------------------
//
TBool CAmrPayloadDecoder::CompareBuffers( const TDesC8& aBuffer,
                                          const TAmrFrame& aFrameToCmp, 
                                          TInt aOctetsToCmp ) const
    {
    DP_AMR_DECODE( "CAmrPayloadDecoder::CompareBuffers" );

    TBool isSame( ETrue );
    if ( EAmrFrameNoData == aFrameToCmp.iFrameType )
        {
        if ( aBuffer.Size() )
            {
            return EFalse;
            }
        else
            {
            return ETrue;
            }
        }
    else
        {
        if ( 0 == aBuffer.Size() )
            {
            return EFalse;
            }
        }        

    TStreamDecoder decoder;
    decoder.Initialize( aFrameToCmp.iFrameData, 0, aFrameToCmp.iBitIndex );
    
    TInt octetsToCmp = aOctetsToCmp < aBuffer.Size() ? aOctetsToCmp : aBuffer.Size();
    for ( TInt i = 0; i < octetsToCmp && isSame; i++ )
        {
        TUint8 val = TUint8( decoder.Decode( KBitsIn1Byte ) );
        if ( aBuffer[i] != val )
            {
            isSame = EFalse;
            }
        }
       
    return isSame;
    }
    
// -----------------------------------------------------------------------------
// CAmrPayloadDecoder::Decode
// Decode the AMR payload ( for one RTP packet ) from a given buffer.
// If more frames is received than expected based on SDPs PTIme negotiation,
// we may have AMR FEC frames in a payload. Frames from previous payload are
// compared to first frame in a new payload. If match is found we have received
// that frame before and have redundant frames, which MUST be discarded.
// After processing frames them are saved for redundancy check at next round.
// -----------------------------------------------------------------------------
//
void CAmrPayloadDecoder::Decode( const TUint8* aBuffer, 
                                 TInt aBufferSize )
    {
    DP_AMR_DECODE( "CAmrPayloadDecoder::Decode" );
        
    TInt byteIndex( 0 ); 
    TInt bitIndex( 0 );
    
    // Decode Payload Header first
    iHeaderDecoder->Decode( aBuffer, byteIndex, bitIndex );
    iModeRequest = iHeaderDecoder->iCmr;
    
    // Decode TOC entries
    iDecodedFrames = DecodeTableOfContents( aBuffer, aBufferSize, byteIndex, bitIndex );
    
    // Update frame positions in the `iFrames' array.
    CalculateFramePosition( aBuffer, iDecodedFrames );
    
    // Discard redundand frames already received
    TUint framesToDel( 0 );
    TInt numOfFramesSaved( iFrameDatas.Count() );
    DP_AMR_DECODE2( "CAmrPayloadDecoder::Decode - NUM OF SAVED FRAMES %d", numOfFramesSaved );
    
    DP_AMR_DECODE2( "CAmrPayloadDecoder::Decode - DECODED FRAMES %d", iDecodedFrames );
    
    TInt i = 0;
    TInt ind = 0;

    #ifdef _DEBUG
   	if ( iDecodedFrames > KMaxFrameCountPerPacket )
   		{
       	DP_AMR_DECODE3("CAmrPayloadDecoder::Decode, iDecodedFrames: %d > KMaxFrameCountPerPacket: %d",  \
        	iDecodedFrames , KMaxFrameCountPerPacket);	
        }
    for ( i = 0; i < iDecodedFrames; i++ )
        {
        DP_AMR_DECODE2( "DECODED FRAME #%d: ", i );
        if ( EAmrFrameNoData != iFrames[i].iFrameType )
            {
            TStreamDecoder decoder;
            TUint8* refBufPtr = iFrames[i].iFrameData;
            decoder.Initialize( refBufPtr, 0, iFrames[i].iBitIndex );
            TUint32 vals[KNumValue5];
            for ( ind = 0; ind < (TInt) KNumValue5; ind ++ )
                {
                vals[ind] = decoder.Decode( KBitsIn1Byte );
                }
            DP_AMR_DECODE6( "| %d | %d | %d | %d | %d |", vals[KNumValue0], vals[KNumValue1],
            	vals[KNumValue2], vals[KNumValue3], vals[KNumValue4] );
            }
        else
            {
            DP_AMR_DECODE( "DECODED FRAME - NO DATA FRAME" );
            }            
        }

    for ( i = 0; i < numOfFramesSaved; i++ )
        {
        if ( (*iFrameDatas[i] ).Size() )
            {
            DP_AMR_DECODE3( "SAVED FRAME #%d, l: %d", i, (*iFrameDatas[i] ).Size() );
            }
        else
            {
            DP_AMR_DECODE2( "SAVED FRAME #%d - NO DATA FRAME: ", i  );
            }            
        }
    #endif

    // Last frame in previous payload is always other than NO_DATA frame,
    // so newly received frames can be compared to it.
    TInt lastFrameInd = numOfFramesSaved - 1 >= 0 ? numOfFramesSaved - 1 : 0;
    for ( i = 0; i < iDecodedFrames && numOfFramesSaved; i++ )
        {
        DP_AMR_DECODE2( "CMP LAST FRAME FROM PREV PAYLOAD TO NEW FRAME #%d", i );
        if ( CompareBuffers( *iFrameDatas[lastFrameInd],
                              iFrames[i],
                              KMaxCompareOctets ) )
            {
            // Last frame from previous payload matched with some frame in a
            // new payload. Frames from this point onwards can be accepted.
            framesToDel = i + 1;
            for ( ind = 0; ind < iDecodedFrames; ind++ )
                {
                iFrames[ ind ] = iFrames[ ind + framesToDel ];
                }

            i = iDecodedFrames;     // Stop loop
            iDecodedFrames -= framesToDel;
            if ( iDecodedFrames <= 0 )
                {
                DP_AMR_DECODE( "Amr::Decode - NO FRAMES REMAINING AFTER RED CHECK" );
                iDecodedFrames = 0;
                }
            iTimeStamp += iTimeStampInc * framesToDel;
            }
        }

    DP_AMR_DECODE2( "NUMBER OF RED FRAMES SKIPPED %d: ", framesToDel );

    // Save received frames for redundancy check purposes
    if ( iDecodedFrames )
        {
        iFrameDatas.ResetAndDestroy();
        }

	if ( iDecodedFrames > KMaxFrameCountPerPacket )
   		{
       	DP_AMR_DECODE3("CAmrPayloadDecoder::Decode, iDecodedFrames: %d > KMaxFrameCountPerPacket: %d",  \
        	iDecodedFrames , KMaxFrameCountPerPacket);	
        }
    for ( i = 0; i < iDecodedFrames; i++ )
        {
        if ( EAmrFrameNoData != iFrames[i].iFrameType )
            {
            TInt dataLen = SpeechBitCount( iFrames[i].iFrameType );
            HBufC8* frame = HBufC8::New( dataLen / KBitsIn1Byte + 1 );
            if (frame)
            	{
	            TUint8* toBuf = const_cast<TUint8*>( ( *frame ).Ptr() );
    	        TStreamEncoder encoder;
        	    encoder.Initialize( toBuf, 0, 0 );
            	encoder.Encode( iFrames[i].iFrameData, 0, iFrames[i].iBitIndex, dataLen );
            	TPtr8 dataPtr = frame->Des();
            	dataPtr.SetLength( dataLen / KBitsIn1Byte + 1 );
            	if (iFrameDatas.Append( frame ) != KErrNone)
            		{
            		delete frame;
            		}
            	}
            }
        else
            {
            HBufC8* frame = HBufC8::New( 0 );
            if (frame && iFrameDatas.Append( frame ) != KErrNone)
            	{
            	delete frame;
            	}
            }            
        }

    #ifdef FTD_ENABLED
    
    TMccCodecStats stats;
    stats.iCMRDownlink = iModeRequest;
    stats.SetFieldUpdatedFlag( ECMRDownlink );
    iCodecStatsQueue.Send( stats );
    
    TMccJBufferStats jStats;
    jStats.iRedLevel = framesToDel;
    jStats.iSampleSizeMultiplier = iDecodedFrames;
    jStats.SetFieldUpdatedFlag( ERedLevel );
    jStats.SetFieldUpdatedFlag( ESampleSizeMultiplier );
        
    iJBufStatsQueue.Send( jStats );
    
    #endif        
    }

// -----------------------------------------------------------------------------
// CAmrPayloadDecoder::DecodeTableOfContents
// Decode TOC ( Table of contents ) entries from the payload buffer.
// Decoded values are put in `iFrames[]' array.
// Return number of TOC entries decoded.
// -----------------------------------------------------------------------------
//
TInt CAmrPayloadDecoder::DecodeTableOfContents( const TUint8* aBuffer, 
                                                TInt aBufferSize,
                                                TInt& aByteIndex, 
                                                TInt& aBitIndex )
    {
    DP_AMR_DECODE( "CAmrPayloadDecoder::DecodeTableOfContents" );
        
    TInt curChannel( 0 );
    TInt i( 0 );
    
    for ( i = 0; ( i < KMaxFrameCountPerPacket ) && ( aByteIndex < aBufferSize ); i++ )
        {
        iTocEntryDecoder->Decode( aBuffer, aByteIndex, aBitIndex );
        iFrames[i].iFrameType = iTocEntryDecoder->iFrameType;
        iFrames[i].iFrameQualityInd = iTocEntryDecoder->iFrameQualityInd;
        iFrames[i].iChannel = curChannel;

        if ( !( iTocEntryDecoder->iFrameFollowed ) )
            {
            break;
            }
            
        // Update currentChannel
        curChannel++;
        if ( curChannel == iChannelCount )
            {
            curChannel = 0;
            }
        }

    return i + 1;
    }

// -----------------------------------------------------------------------------
// CAmrPayloadDecoder::CalculateFramePosition
// Calculate frames' starting Byte and Bit positions.
// -----------------------------------------------------------------------------
//
void CAmrPayloadDecoder::CalculateFramePosition( const TUint8* aBuffer, 
                                                 TInt aFrameCount )
    {
    DP_AMR_DECODE( "CAmrPayloadDecoder::CalculateFramePosition" );
        
    TUint frameByteIndex;
    TUint frameBitIndex;
    
    // Number of bits for CMR and TOC entries
    TUint bitCount = KHeaderBitsBE + KTOCFieldBitsBE * TUint( aFrameCount );    

    if ( aFrameCount > KMaxFrameCountPerPacket )
   		{
       	DP_AMR_DECODE3("CAmrPayloadDecoder::CalculateFramePosition, aFrameCount: %d > KMaxFrameCountPerPacket: %d",  \
        	aFrameCount, KMaxFrameCountPerPacket);	
        }
    for ( TInt i = 0; i < aFrameCount; i++ )
        {
        frameByteIndex = bitCount >> KNumValue3;
        frameBitIndex = bitCount & KBitIndex7;
        if ( EAmrFrameNoData == iFrames[i].iFrameType )
            {
            iFrames[i].iBitIndex = 0;
            iFrames[i].iFrameData = NULL;
            }
        else
            {
            // Set frame byte and bit positions
            iFrames[i].iBitIndex = frameBitIndex;
            iFrames[i].iFrameData = const_cast<TUint8*>( aBuffer ) + frameByteIndex;
            }

        // Update bit-count for next frame entry.
        bitCount = bitCount + TUint( SpeechBitCount( iFrames[i].iFrameType ) );
        }
    }


// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File  

