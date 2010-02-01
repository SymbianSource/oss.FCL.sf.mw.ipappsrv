/*
* Copyright (c) 2004-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Formatter classes for decoding of RTP payload format
*                for AMR ( narrow band ) audio codecs.
*
*/




#ifndef AMRPAYLOADDECODER_H
#define AMRPAYLOADDECODER_H

// INCLUDES
#include <e32std.h>
#include "amrpayloadformatter.h"
#include "streamformatter.h"


// FORWARD DECLARATIONS
class CAmrPayloadHeader;
class CAmrTocEntry;
class TStreamDecoder;

// CLASS DECLARATION

/**
* The AMR payload decoder for bandwidth efficient mode.
*
* @lib <TBD>
* @since Series-60 2.1
*/
class CAmrPayloadDecoder : public CAmrPayloadFormatter
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CAmrPayloadDecoder* NewL( TBool aIsNb );

        /**
        * Destructor.
        */
        virtual ~CAmrPayloadDecoder( );


    public: // New functions

        /**
        * Decode payload data received in the payload buffer.
        * If AMR FEC is used and redundant frames are discarded, time stamp is
        * increased to correspond start of remaining frames.
        *
        * @since    Series 60 3.0
        * @param    aTimeStamp      Time stamp from RTP header
        * @param    aTimeStampInc   Unit for time stamp incrementing
        * @return TInt - Number of AMR frames decoded
        */
        virtual TInt DecodePayload( TUint32& aTimeStamp,
                                    TUint32 aTimeStampInc );

        /**
        * Get AMR codec mode request ( CMR ) received in the payload.
        * @since Series-60 2.1
        * @return TAmrModeRequest - AMR codec mode request ( CMR ) received
        */
        TAmrModeRequest ModeRequest() const;

        /**
        * Get AMR frames decoded from the payload.
        * Number of frames equals the value returned by `DecodePayload( )' function.
        * @since Series-60 2.1
        * @return TAmrFrame[] - Array of AMR frames decoded
        */
        TAmrFrame* Frames( );

    protected:

        /**
        * Constructor.
        */
        CAmrPayloadDecoder( TBool aIsNb );
        
    private:
    
        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL( );

    protected: // Functions

        /**
        * Decode the AMR payload ( for one RTP packet ) from a given buffer.
        * Decoded values are returned in `iFrames' array.
        * @since Series-60 2.1
        * @param aBuffer - [input] Buffer to decode data from.
        * @param aBufferSize - [input] Buffer size in bytes.
        * @return None
        */
        void Decode( const TUint8* aBuffer,
                     TInt aBufferSize );

        /**
        * Decode TOC ( Table of Contents ) entries from a given buffer at the given
        * position. Decoded information are stored in `iFrames' array.
        * Byte and bit positions are updated after decoding is done.
        * @since Series-60 2.1
        * @param aBuffer - [input] Buffer to decode data from.
        * @param aBufferSize - [input] Buffer size in bytes.
        * @param aByteIndex - [input] Starting byte index ( starts from zero ).
        *                     [output] Byte index updated after encoding.
        * @param aBitIndex - [input] Starting bit index ( starts from zero ).
        *                    [output] Bit index updated after encoding.
        * @return Number of TOC entries decoded.
        */
        TInt DecodeTableOfContents( const TUint8* aBuffer, 
                                    TInt aBufferSize, 
                                    TInt& aByteIndex, 
                                    TInt& aBitIndex );

        /**
        * Calculate frames' starting Byte and Bit positions in a given payload buffer
        * and update `iFrames' array with the frames' positions.
        * @since Series-60 2.1
        * @param aBuffer - [input] Payload buffer.
        * @param aBufferSize - [input] Buffer size in bytes.
        * @param aFrameCount - [input] Number of frames included in the payload buffer.
        * @return None
        */
        virtual void CalculateFramePosition( const TUint8* aBuffer, 
                                             TInt aFrameCount );
                                             
    protected:  // From CAmrPayloadFormatter
    
        /**
        * Initializes decoder. Decoder should be initialized when starting
        * playing.
        *
        * @since    Series 60 3.0
        * @param    None
        * @return   void
        */
        virtual void DoInitialize();

    private:    // Functions
    
        /**
        * Compares two AMR frames.
        *
        * @since    Series 60 3.0
        * @param    aBuffer         Payload buffer containing one AMR frame
        * @param    aFrameToCmp     AMR frame to which compare
        * @param    aBitsToCmp      Number of bits to search
        * @return   ETrue if contents are same; otherwise EFalse
        */
        TBool CompareBuffers( const TDesC8& aBuffer,
                              const TAmrFrame& aFrameToCmp, 
                              TInt aBitsToCmp ) const;
                                             


    protected: // Data

        // Payload decoding
        // Decoder for payload header
        CAmrPayloadHeader* iHeaderDecoder;
        
        // Decoder for TOC entry
        CAmrTocEntry* iTocEntryDecoder;
        
        // Frames decoded from received buffer
        TInt iDecodedFrames;
        
        // Time stamp to give for next frame being decoded
        TUint32 iTimeStamp;

        // Unit for time stamp incrementing/decrementing
        TUint32 iTimeStampInc;
        private:
	
        #ifdef TEST_EUNIT
            friend class UT_CAmrPayloadDecoder;
            friend class UT_CAmrPayloadFormatRead;
        #endif   
    };



#endif  // __AMRPAYLOADDECODER_H
