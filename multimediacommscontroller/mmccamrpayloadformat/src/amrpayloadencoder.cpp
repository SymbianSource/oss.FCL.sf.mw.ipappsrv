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
#include "amrpayloadencoder.h"
#include "amrcommonutil.h"


// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAmrPayloadEncoder::CAmrPayloadEncoder
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CAmrPayloadEncoder::CAmrPayloadEncoder( TBool aIsNb ) : CAmrPayloadFormatter( aIsNb )
    {
    DP_AMR_ENCODE( "CAmrPayloadEncoder::CAmrPayloadEncoder" );       
        
    CAmrPayloadEncoder::InitializeFrameEncoder( );
    }


// -----------------------------------------------------------------------------
// CAmrPayloadEncoder::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CAmrPayloadEncoder::ConstructL( )
    {
    DP_AMR_ENCODE( "CAmrPayloadEncoder::ConstructL" );
        
    // Allocate memory for iHeaderEncoder and iTocEntryEncoder
    iHeaderEncoder = CAmrPayloadHeader::NewL();
    iTocEntryEncoder = CAmrTocEntry::NewL();
    
    #ifdef FTD_ENABLED    
    User::LeaveIfError( iCodecStatsQueue.OpenGlobal( KMccCodecStats, EOwnerProcess ) ); 
    #endif    
    }


// -----------------------------------------------------------------------------
// CAmrPayloadEncoder::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CAmrPayloadEncoder* CAmrPayloadEncoder::NewL( TBool aIsNb )
    {
    DP_AMR_ENCODE( "CAmrPayloadEncoder::NewL" );
    
    CAmrPayloadEncoder* self = new( ELeave ) CAmrPayloadEncoder( aIsNb );
    
    CleanupStack::PushL( self );
    self->ConstructL( );
    CleanupStack::Pop( self );

    return self;
    }


// Destructor
CAmrPayloadEncoder::~CAmrPayloadEncoder( )
    {
    DP_AMR_ENCODE( "CAmrPayloadEncoder::~CAmrPayloadEncoder" );
        
    delete iHeaderEncoder;
    delete iTocEntryEncoder;
    
    #ifdef FTD_ENABLED
    
    iCodecStatsQueue.Close();
    
    #endif
    }

// -----------------------------------------------------------------------------
// CAmrPayloadEncoder::DoInitialize
// Initialize encoder. Encoder MUST BE initialized when starting playing.
// -----------------------------------------------------------------------------
//
void CAmrPayloadEncoder::DoInitialize()
    {
    DP_AMR_ENCODE( "CAmrPayloadEncoder::DoInitialize" );
        
    iRedIntervalCollected = EFalse;
    iFrameIndex = iRedCount;
    }
    
// -----------------------------------------------------------------------------
// CAmrPayloadEncoder::SetModeRequest
// Set ModeRequest.
// -----------------------------------------------------------------------------
//
void CAmrPayloadEncoder::SetModeRequest( TAmrModeRequest aRequest )
    {
    DP_AMR_ENCODE2( "CAmrPayloadEncoder::SetModeRequest - REQUEST: %d", aRequest );
        
    iModeRequest = aRequest;
    
    #ifdef FTD_ENABLED
    
    TMccCodecStats stats;
    stats.iCMRUplink = iModeRequest;
    stats.SetFieldUpdatedFlag( ECMRUplink );
    iCodecStatsQueue.Send( stats );
    
    #endif
    }


// -----------------------------------------------------------------------------
// CAmrPayloadEncoder::EncodeFrame
// Encode a received AMR frame into the payload buffer to be sent.
// If AMR FEC redundancy is used and we have collected needed count of redundant
// frames, them are encoded first to the payload buffer. Last iRedCount frames
// are saved to be used with next RTP packet. iRedCount first frames in the 
// 'iFrames'-array are reserved for redundant frames info saving. Thus new 
// frames starts from index iFrames[iRedCount].
// -----------------------------------------------------------------------------
//
TBool CAmrPayloadEncoder::EncodeFrame( TInt aChannel,
                                       TAmrFrameType aFrameType,
                                       TUint8 aFrameQualityInd,
                                       const TDes8& aFrameData )
    {
    DP_AMR_ENCODE( "CAmrPayloadEncoder::EncodeFrame" );
        
    TBool payloadComplete( EFalse );
    
    // Encode redundant frames if there is some
    // Frame info relating to redundant frames is saved at the beginning of iFrames
    TInt count( iFrameDatas.Count() );

    if ( iRedCount == iFrameIndex && count && iRedIntervalCollected )
        {
        for ( TInt i = iRedCount - count; i < iRedCount; i++ )
            {
            const TUint8* redFrameData = iFrames[i].iFrameData;
            TInt speechBits = SpeechBitCount( iFrames[i].iFrameType );
            iFrameEncoder.Encode( redFrameData, 0, 0, speechBits );
            iTotalSpeechBits += speechBits;
            
            #ifdef _DEBUG
            if ( speechBits )
                {
                DP_AMR_ENCODE2( "ENCODING RED FRAME #%d: ", i );
                DP_AMR_ENCODE6( "| %d | %d | %d | %d | %d |", redFrameData[KNumValue0],
                    redFrameData[KNumValue1], redFrameData[KNumValue2],
                    redFrameData[KNumValue3], redFrameData[KNumValue4] );    
                }
            else
                {
                DP_AMR_ENCODE2( "ENCODING RED FRAME #%d: - NO DATA FRAME", i );
                }
            #endif
            }
        }

    // Update iFrames array
    iFrames[iFrameIndex].iFrameType = aFrameType;
    iFrames[iFrameIndex].iFrameQualityInd = aFrameQualityInd;
    iFrames[iFrameIndex].iChannel = aChannel;
    if ( EAmrFrameNoData == aFrameType  )
        {
        DP_AMR_ENCODE( "FRAME TO ENCODE - NO DATA FRAME" );

        iFrames[iFrameIndex].iFrameData = NULL;
        iFrames[iFrameIndex].iBitIndex = 0;
        }
    else
        {
        iFrames[iFrameIndex].iFrameData = iPayload + iFrameEncoder.ByteIndex( );
        iFrames[iFrameIndex].iBitIndex = TUint( iFrameEncoder.BitIndex( ) );
        const TUint8* frameData = aFrameData.Ptr( );

        // Get the speech bit count of current frame
        TInt speechBits = SpeechBitCount( aFrameType ); 
        iFrameEncoder.Encode( frameData, 0, 0, speechBits );

        // Update TotalSpeechBits 
        iTotalSpeechBits += speechBits;
        
        #ifdef _DEBUG
        DP_AMR_ENCODE2( "ENCODED FRAME #%d: ", iFrameIndex );
        if ( aFrameData.Size() )
            {
            TStreamDecoder decoder;
            TUint8* refBufPtr = iFrames[iFrameIndex].iFrameData;
            decoder.Initialize( refBufPtr, 0, TInt( iFrames[iFrameIndex].iBitIndex ) );
            TUint32 vals[KNumValue5];
            for ( TInt v = 0; v < (TInt) KNumValue5; v++ )
                {
                vals[v] = decoder.Decode( KBitsIn1Byte );
                }
            DP_AMR_ENCODE6( "| %d | %d | %d | %d | %d |", vals[KNumValue0],
            vals[KNumValue1], vals[KNumValue2], vals[KNumValue3], vals[KNumValue4] );
            }
        #endif            
        }

    iFrameIndex++;
    if ( iFrameIndex == iFrameCount + iRedCount )
        {
        DP_AMR_ENCODE( "CAmrPayloadEncoder::EncodeFrame - NEEDED FRAMES COLLECTED" );

        // Add header information to the RTP packet
        TInt payloadLen = Encode( iPayload );
        DP_AMR_ENCODE3( "CAmrPayloadEncoder::EncodeFrame - PAYLOAD SIZE: %d, TARGET MAX: %d",
            payloadLen, iPayloadPtr.MaxLength() );
        iPayloadPtr.SetLength( payloadLen );

        // Finish a payload
        iFrameIndex = iRedCount;
        iTotalSpeechBits = 0;
        SaveRedundantFrame( aFrameData );
        if ( iRedCount <= iFrameDatas.Count() )
            {
            iRedIntervalCollected = ETrue;
            }
        
        // Reset iFrameEncoder
        InitializeFrameEncoder( );
        payloadComplete = ETrue;
        }
    else if ( iRedCount && iFrameIndex > iFrameCount )
        {
        // Save iRedCount latest frames to be sent with next RTP packet as redundancy
        SaveRedundantFrame( aFrameData );
        }
    else
        {
        // Make PC-LINT happy.
        }        

    return payloadComplete;
    }

// -----------------------------------------------------------------------------
// CAmrPayloadEncoder::SaveRedundantFrame
// Saves frame to the array reserved for redundancy saving.
// -----------------------------------------------------------------------------
//
void CAmrPayloadEncoder::SaveRedundantFrame( const TDes8& aFrameData )
    {
    DP_AMR_ENCODE( "CAmrPayloadEncoder::SaveRedundantFrame" );
        
    HBufC8* hFrame = aFrameData.Alloc();
    if ( hFrame )
        {
        if ( iFrameDatas.Append( hFrame ) != KErrNone )
            {
            delete hFrame;
            }
        else if ( iRedCount < iFrameDatas.Count() )
            {
            delete iFrameDatas[0];
            iFrameDatas.Remove( 0 );
            }
        }
    }
    
// -----------------------------------------------------------------------------
// CAmrPayloadEncoder::ResetPayloadBuffer
// Resets iFrames array. iRedCount newest frames from payload just sent are
// copied to the beginning of the iFrames array, where is area reserved for
// redundant frames keeping. That way frames can be encoded to the next payload
// as redundat frames.
// -----------------------------------------------------------------------------
//
void CAmrPayloadEncoder::ResetPayloadBuffer( )
    {
    DP_AMR_ENCODE( "CAmrPayloadEncoder::ResetPayloadBuffer" );
        
    iFrameIndex = iRedCount;
    iPayloadPtr.SetLength( 0 );
    
    // Save redCount newest frames to the beginning of iFrames array
    TInt numOfRedFrames = iFrameDatas.Count();
    if ( iRedCount - numOfRedFrames >= 0 )
        {
        for ( TInt i = iRedCount - numOfRedFrames; i < iRedCount; i++ )
            {
           	if ( i > KMaxFrameCountPerPacket )
   				{
       			DP_AMR_ENCODE3("CAmrPayloadEncoder::ResetPayloadBuffer,\
       				 i: %d > KMaxFrameCountPerPacket: %d",  \
       				 i , KMaxFrameCountPerPacket);	
        		}
        	if ( ( i + iFrameCount ) > KMaxFrameCountPerPacket )
   				{
       			DP_AMR_ENCODE3("CAmrPayloadEncoder::ResetPayloadBuffer, i + iFrameCount: %d > KMaxFrameCountPerPacket: %d",  \
        		(i + iFrameCount) , KMaxFrameCountPerPacket);	
        		}
        	
        	// Checking index limit to appease PC-lint
        	if ( ( i + iFrameCount ) < KMaxFrameCountPerPacket )	
        		{
        		iFrames[ i ] = iFrames[ i + iFrameCount ];
        		
            	}
            }

        for ( TInt j = 0; j < numOfRedFrames; j++ )
            {
            
            if ( ( j + ( iRedCount - numOfRedFrames )  ) > KMaxFrameCountPerPacket )
   				{
       			DP_AMR_ENCODE3("CAmrPayloadEncoder::ResetPayloadBuffer,\
       				j + ( iRedCount - numOfRedFrames ): %d > KMaxFrameCountPerPacket: %d", \
       				( j + ( iRedCount - numOfRedFrames ) ), KMaxFrameCountPerPacket);	
        		}
        		
            // Checking index limit to appease PC-lint
        	if ( ( j + ( iRedCount - numOfRedFrames )  ) < KMaxFrameCountPerPacket )
        		{
        		
            	// Move frame data ptr to point redundant frames
            	iFrames[ j + ( iRedCount - numOfRedFrames ) ].iFrameData
	                = const_cast<TUint8*>( ( *iFrameDatas[ j ] ).Ptr() );
                
            	}
            }
        }
    else
        {
        DP_AMR_ENCODE( "CAmrPayloadEncoder::ResetPayloadBuffer - TOO MANY RED FRAMES" );
        }        
        
    // NULL rest of frame datas
    TInt index( KMaxFrameCountPerPacket );
    while ( --index >= iRedCount )
        {
        iFrames[index].iFrameData = NULL;
        }
    }

// -----------------------------------------------------------------------------
// CAmrPayloadEncoder::SetChannelCount
// Set number of audio channels.
// -----------------------------------------------------------------------------
//
TInt CAmrPayloadEncoder::SetChannelCount( TInt aChannelCount )
    {
    DP_AMR_ENCODE2( "CAmrPayloadEncoder::SetChannelCount - COUNT: %d", aChannelCount );
        
    TInt ret = CAmrPayloadFormatter::SetChannelCount( aChannelCount );
    InitializeFrameEncoder( );
    return ret;
    }

// -----------------------------------------------------------------------------
// CAmrPayloadEncoder::SetFrameBlockCount
// Set number of AMR frame blocks included in one RTP packet.
// -----------------------------------------------------------------------------
//
TInt CAmrPayloadEncoder::SetFrameBlockCount( TInt aFrameblockCount )
    {
    DP_AMR_ENCODE2( "CAmrPayloadEncoder::SetFrameBlockCount - COUNT: %d", aFrameblockCount );
        
    TInt ret = CAmrPayloadFormatter::SetFrameBlockCount( aFrameblockCount );
    InitializeFrameEncoder( );
    return ret;
    }

// -----------------------------------------------------------------------------
// CAmrPayloadEncoder::SetRedFrameBlockCount
// Set number of redundant AMR frame blocks to be included in one RTP packet.
// -----------------------------------------------------------------------------
//
TInt CAmrPayloadEncoder::SetRedFrameBlockCount( TInt aRedBlockCount )
    {
    DP_AMR_ENCODE2( "CAmrPayloadEncoder::SetFrameBlockCount - COUNT: %d", aRedBlockCount );
        
    if ( KMaxAmrRedCount >= aRedBlockCount && 0 <= aRedBlockCount )
        {
        iRedCount = aRedBlockCount;
        return KErrNone;
        }
    else
        {
        return KErrArgument;
        }        
    }
    
// -----------------------------------------------------------------------------
// CAmrPayloadEncoder::Encode
// Encode the AMR payload ( for one RTP packet ) into a given buffer.
// If payload contains only NO_DATA frames, packet is not encoded at all.
// NO_DATA frames at the end of payload are ripped off.
// -----------------------------------------------------------------------------
//
TInt CAmrPayloadEncoder::Encode( TUint8* aBuffer )
    {
    DP_AMR_ENCODE( "CAmrPayloadEncoder::Encode" );
    
    TInt byteIndex( 0 );
    TInt bitIndex( 0 );

    // Rip off extra NO_DATA frames
    TInt framesDiscarded = DiscardExtraNoDataFrames( );
    if ( iRedIntervalCollected )
        {
        if ( framesDiscarded == iFrameCount + iRedCount )
            {
            DP_AMR_ENCODE( "PACKET WILL NOT BE SENT - ONLY NO DATA FRAMES" );
            return 0;
            }
        }
    else
        {
        if ( framesDiscarded == iFrameCount )
            {
            DP_AMR_ENCODE( "PACKET WILL NOT BE SENT - NEW FRAMES WERE NO DATA FRAMES" );
            DP_AMR_ENCODE2( "CAmrPayloadEncoder::Encode framesDiscarded: %d", framesDiscarded );
            return 0;
            }
        }        

    // Encode payload header
    iHeaderEncoder->iCmr = iModeRequest;
    iHeaderEncoder->Encode( aBuffer, byteIndex, bitIndex );

    // Encode TOC entries
    EncodeTableOfContents( aBuffer, byteIndex, bitIndex );
    
    // Pad last several bits if needed
    TInt totalBytes = iFrameEncoder.ByteIndex();
    
    if ( iFrameEncoder.BitIndex() != 0 )
        {
        // Padding Zeros
        iFrameEncoder.Encode( 0, KBitsIn1Byte - iFrameEncoder.BitIndex() );
        totalBytes++;
        }
    
    return totalBytes;
    }

// -----------------------------------------------------------------------------
// CAmrPayloadEncoder::DiscardExtraNoDataFrames
// Unwanted NO_DATA frames are ripped off from payload according to RFC3267: 4.3.2.
// TOC-fields indicating NO_DATA frame are deleted and speech bits following 
// TOCs are moved to start earlier correspondingly.
// -----------------------------------------------------------------------------
//
TInt CAmrPayloadEncoder::DiscardExtraNoDataFrames( )
    {
    DP_AMR_ENCODE( "CAmrPayloadEncoder::DiscardExtraNoDataFrames" );
        
    TInt endIndex( iFrameCount + iRedCount );
    iFramesDiscarded = 0;
    
    // write debug info if the endIndex is greater than the iFrames[] range
    if ( endIndex > KMaxFrameCountPerPacket )
   		{
       	DP_AMR_ENCODE3("CAmrPayloadEncoder::DiscardExtraNoDataFrames, endIndex: %d > KMaxFrameCountPerPacket: %d",  \
        	endIndex, KMaxFrameCountPerPacket);	
        }
    // Count NO_DATA frames to reject
    for ( TInt i = 0; i < endIndex; i++ )
        {
        if ( EAmrFrameNoData == iFrames[i].iFrameType )
            {
            iFramesDiscarded++;
            }
        else
            {
            iFramesDiscarded = 0;
            }                
        }
    
    // Move speech bits to start earlier if TOCs are removed
    if ( iFramesDiscarded )
        {
        DP_AMR_ENCODE2( "CAmrPayloadEncoder::DiscardExtraNoDataFrames - REJECT %d FRAMES", iFramesDiscarded );
        
        TStreamDecoder decoder;
        TStreamEncoder encoder;

        TInt numOfRedFrames( 0 );
        if ( iRedIntervalCollected )
            {
            numOfRedFrames = iFrameDatas.Count();
            }
        
        TInt writeByteIndex = ( KHeaderBitsBE 
            + KTOCFieldBitsBE * ( iFrameCount + numOfRedFrames - iFramesDiscarded ) )
            / KBitsIn1Byte;

        TInt writeBitIndex = ( KHeaderBitsBE
            + KTOCFieldBitsBE * ( iFrameCount + numOfRedFrames - iFramesDiscarded ) )
            % KBitsIn1Byte;

        encoder.Initialize( iPayload, writeByteIndex, writeBitIndex );
        
        DP_AMR_ENCODE3( "AMR ENCODER - START BYTE IND: %d, BIT IND: %d",
            encoder.ByteIndex(), encoder.BitIndex() );
        
        TInt readByteIndex = ( KHeaderBitsBE
            + KTOCFieldBitsBE * ( iFrameCount + numOfRedFrames ) )
            / KBitsIn1Byte;
        TInt readBitIndex = ( KHeaderBitsBE 
            + KTOCFieldBitsBE * ( iFrameCount + numOfRedFrames ) )
            % KBitsIn1Byte;

        decoder.Initialize( iPayload, readByteIndex, readBitIndex );
        
        // Move speech bits
        encoder.Encode( iPayload, readByteIndex, readBitIndex, iTotalSpeechBits );
        
        DP_AMR_ENCODE3( "AMR ENCODER - END BYTE IND: %d, BIT IND: %d",
            encoder.ByteIndex(), encoder.BitIndex() );
            
        // Update frame encoder position so that payload length is set correctly
        iFrameEncoder.Initialize( iPayload, encoder.ByteIndex(), encoder.BitIndex() );
        }
    
    return iFramesDiscarded;        
    }
    
// -----------------------------------------------------------------------------
// CAmrPayloadEncoder::ReEncodeFrame
// Do bit shifting when AmrPayloadFormatPlugin is doing buffer flushing.
// -----------------------------------------------------------------------------
//
TBool CAmrPayloadEncoder::ReEncodeFrameL( )
    {
    return ReEncodeFrameL( iPayload );
    }

// -----------------------------------------------------------------------------
// CAmrPayloadEncoder::ReEncodeFrame
// Do bit shifting when AmrPayloadFormatPlugin is doing buffer flushing.
// -----------------------------------------------------------------------------
//
TBool CAmrPayloadEncoder::ReEncodeFrameL( TUint8* aBuffer )
    {
    DP_AMR_ENCODE( "CAmrPayloadEncoder::ReEncodeFrameL" );
        
    // No outstanding buffer at all
    if ( iFrameIndex == iRedCount )
        {
        return EFalse;
        }

    // Only called when AmrPayloadFormatPlugin is flushing buffer out
    TInt byteIndex( 0 );
    TInt bitIndex( 0 );

    // Encode payload header
    iHeaderEncoder->iCmr = iModeRequest;
    iHeaderEncoder->Encode( aBuffer, byteIndex, bitIndex );
    
    // Encode TOC entries
    EncodeTableOfContents( aBuffer, byteIndex, bitIndex );
    
    // Do bit shifting for the speech bits
    // orginal starting position
    TInt frameByteIndex = ( KHeaderBitsBE
        + KTOCFieldBitsBE * ( iFrameCount + iRedCount ) ) / KBitsIn1Byte;

    TInt frameBitIndex = ( KHeaderBitsBE
        + KTOCFieldBitsBE * ( iFrameCount + iRedCount ) ) % KBitsIn1Byte;
    
    // Current speech data length
    TInt speechBytes
        = ( iTotalSpeechBits % KBitsIn1Byte == 0 )
        ? iTotalSpeechBits / KBitsIn1Byte : iTotalSpeechBits / KBitsIn1Byte + 1;
    
    // Read out speech data to temp buffer
    TUint8* tempBuf = static_cast<TUint8*>
        ( User::AllocL( sizeof( TUint8 ) * TUint8( speechBytes ) ) );

    CleanupStack::PushL( tempBuf );
    TStreamEncoder streamEnc;
    TStreamDecoder streamDec;
    streamEnc.Initialize( aBuffer, byteIndex, bitIndex );
    streamDec.Initialize( aBuffer, frameByteIndex, frameBitIndex );
    streamDec.Decode( tempBuf, 0, 0, iTotalSpeechBits );
    streamEnc.Encode( tempBuf, 0, 0, iTotalSpeechBits );

    if ( streamEnc.BitIndex( ) != 0 )
        {
        // Padding zeros
        streamEnc.Encode( 0, KBitsIn1Byte - streamEnc.BitIndex( ) );
        }
    
    TInt payloadLen = streamEnc.ByteIndex( );
    iPayloadPtr.SetLength( payloadLen );

    // Finish current payload
    iFrameIndex = iRedCount;
    // Reset iFrameEncoder
    InitializeFrameEncoder( );
    // Reset TotalSpeechBits counter
    iTotalSpeechBits = 0; 

    CleanupStack::PopAndDestroy( tempBuf );
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CAmrPayloadEncoder::IsStartOfPeriod
// Returns ETrue if next frame to be encoded starts a new packetization period.
// -----------------------------------------------------------------------------
//
TBool CAmrPayloadEncoder::IsStartOfPeriod()
    {
    return ( iFrameIndex == iRedCount );
    }

// -----------------------------------------------------------------------------
// CAmrPayloadEncoder::EncodeTableOfContents
// Encode 'Table of Contents' into a given buffer at the given position.
// -----------------------------------------------------------------------------
//
void CAmrPayloadEncoder::EncodeTableOfContents( TUint8* aBuffer,
                                                TInt& aByteIndex, 
                                                TInt& aBitIndex )
    {
    DP_AMR_ENCODE( "CAmrPayloadEncoder::EncodeTableOfContents" );
    
    // Iterate iFrames array and encode each TOC entry
    // Encode TOCs for redundant frames if exist
    TInt ind( iRedCount );
    if ( iRedIntervalCollected )
        {
        ind = iRedCount - iFrameDatas.Count();
        }

    TInt endIndex = iFramesDiscarded ? iFrameIndex - iFramesDiscarded : iFrameIndex;
    // write debug info if the endIndex is greater than the iFrames[] range
    if ( endIndex > KMaxFrameCountPerPacket )
   		{
       	DP_AMR_ENCODE3("CAmrPayloadEncoder::EncodeTableOfContents, endIndex: %d > KMaxFrameCountPerPacket: %d",  \
        	endIndex, KMaxFrameCountPerPacket);	
        }
    for ( ; ind < endIndex; ind++ )
        {
        if ( ind == endIndex - 1 )
            {
            iTocEntryEncoder->iFrameFollowed = 0; // last frame in a payload
            }
        else
            {
            iTocEntryEncoder->iFrameFollowed = 1;
            }

        // Checking index limit to appease PC-lint
        if ( (ind) < KMaxFrameCountPerPacket )
        	{
        iTocEntryEncoder->iFrameQualityInd = iFrames[ind].iFrameQualityInd;
        iTocEntryEncoder->iFrameType = iFrames[ind].iFrameType;
        	}
        iTocEntryEncoder->Encode( aBuffer, aByteIndex, aBitIndex );
        }
    }


// -----------------------------------------------------------------------------
// CAmrPayloadEncoder::InitializeFrameEncoder
// Initialize frame encoder ( Stream Encoder ). Starting position of first AMR
// frame data is calculated. Refer to RFC3267 4.3.
// -----------------------------------------------------------------------------
//
void CAmrPayloadEncoder::InitializeFrameEncoder( )
    {
    DP_AMR_ENCODE( "CAmrPayloadEncoder::InitializeFrameEncoder" );
        
    // Payload header: 4 bits; TOC: 6 bits
    // Total number of TOCs: iChannelCount * iFrameBlockCount
    TInt numOfRedFrames( 0 );
    if ( iRedIntervalCollected )
        {
        numOfRedFrames = iFrameDatas.Count();
        }

    // Save space for frame TOCs
    TInt frameByteIndex = ( KHeaderBitsBE
        + KTOCFieldBitsBE * ( iFrameCount + numOfRedFrames ) ) / KBitsIn1Byte;
    
    TInt frameBitIndex = ( KHeaderBitsBE 
        + KTOCFieldBitsBE * ( iFrameCount + numOfRedFrames ) ) % KBitsIn1Byte;

    iFrameEncoder.Initialize( iPayload, frameByteIndex, frameBitIndex );
    }
    

// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File
