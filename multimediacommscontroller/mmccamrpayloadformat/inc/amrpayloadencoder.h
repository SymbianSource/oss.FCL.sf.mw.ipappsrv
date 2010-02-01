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
*                for AMR ( narrow band ) and AMR-WB ( wide band ) audio codecs.
*
*/




#ifndef AMRPAYLOADENCODER_H
#define AMRPAYLOADENCODER_H

// INCLUDES
#include <e32std.h>
#include "amrpayloadformatter.h"
#include "amrpayloadheader.h"
#include "amrtocentry.h"
#include "streamformatter.h"

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class TStreamEncoder;

// CLASS DECLARATION

/**
* The AMR payload encoder for bandwidth efficient mode.
*
* @lib <TBD>
* @since Series-60 2.1
*/
class CAmrPayloadEncoder : public CAmrPayloadFormatter
    {
    public:  // Constructors and destructor

         /**
        * Two-phased constructor.
        */
        static CAmrPayloadEncoder* NewL( TBool aIsNb );

        
        /**
        * Destructor.
        */
        virtual ~CAmrPayloadEncoder( );


    public: // Functions
        
        /**
        * Set AMR codec mode request ( CMR ).
        * @since    Series-60 2.1
        * @param    aRequest - [input] AMR codec mode request ( CMR )
        * @return   void
        */
        void SetModeRequest( TAmrModeRequest aRequest );

        /**
        * Encode a received AMR frame into the payload buffer.
        * @since Series-60 2.1
        * @param aChannel - [input] Channel number that the frame belongs to
        * @param aFrameType - [input] Frame type
        * @param aFrameQualityInd - [input] Frame quality indicator bit; 1 if frame OK; 0 if frame damaged
        * @param aFrameData - [input] Frame data; its length depends on frame type
        * @return   ETrue if payload formatting is completed; otherwise EFalse
        */
        virtual TBool EncodeFrame( TInt aChannel, 
                                   TAmrFrameType aFrameType, 
                                   TUint8 aFrameQualityInd, 
                                   const TDes8& aFrameData );

        /**
        * Reset the payload buffer.
        * @since    Series-60 2.1
        * @param    None
        * @return   void
        */
        void ResetPayloadBuffer( );

        /**
        * Do bit shifting when AmrPayloadFormatPlugin is doing buffer flushing.
        * @since    Series-60 2.1
        * @param    None
        * @return   ETrue if payload formatting is completed; otherwise EFalse
        */
        TBool ReEncodeFrameL( );

         /**
        * Do bit shifting when AmrPayloadFormatPlugin is doing buffer flushing
        * @since    Series-60 2.1
        * @param    aBuffer Buffer containing frame data
        * @return   ETrue if payload formatting is completed; otherwise EFalse
        */
        TBool ReEncodeFrameL( TUint8* aBuffer );
        
        /**
         * Returns ETrue if next frame will start new packetization period.
         * @since    Series 60 3.1
         * @return   ETrue if next frame starts new period
         */
        TBool IsStartOfPeriod();

    public: // Functions from CAmrPayloadFormatter

        /**
        * Set number of audio channels.
        * @since    Series-60 2.1
        * @param    aChannelCount - [input] Number of audio channels
        * @return   KErrNone if successful; system wide error code otherwise
        */
        virtual TInt SetChannelCount( TInt aChannelCount );
        
        /**
        * Set number of AMR frame blocks included in one RTP packet.
        * Each frame block contains frames for all audio channels.
        * @since    Series-60 2.1
        * @param    aFrameblockCount - [input] Number of AMR frame blocks included 
        * in one RTP packet
        * @return   KErrNone if successful; system wide error code otherwise
        */
        virtual TInt SetFrameBlockCount( TInt aFrameblockCount );
        
        /**
        * Set number of redundant frame blocks included in one RTP packet.
        * Each frame block contains frames for all audio channels.
        * @since    Series 60 3.0
        * @param    aRedBlockCount  Number of redundant AMR frame blocks to be
        * included in one RTP packet
        * @return   KErrNone if successful; system wide error code otherwise
        */
        virtual TInt SetRedFrameBlockCount( TInt aRedBlockCount );

        /**
        * Initialize frame encoder ( Stream Encoder ).
        * Information to initialize are obtained from iPayload, calculation 
        * using iChannelCount and iFrameBlockCount.
        * Initialization should be done whenever a payload is finished.
        * @since    Series-60 2.1
        * @param    None
        * @return   void
        */
        virtual void InitializeFrameEncoder( );

    protected: // Functions

        /**
        * Constructor.
        */
        CAmrPayloadEncoder( TBool aIsNb );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL( );

        /**
        * Encode the AMR payload ( for one RTP packet ) into a given buffer.
        * Values to encode are obtained from `iFrames' array.
        * @since    Series-60 2.1
        * @param    aBuffer - [output] Buffer to store encoded data
        * @return   Total number of bytes encoded in the buffer
        */
        TInt Encode( TUint8* aBuffer );

        /**
        * Encode 'Table of Contents' into a given buffer at the given position.
        * Information to encode are obtained from `iFrames' array.
        * Byte and bit positions are updated after encoding is done.
        * @since Series-60 2.1
        * @param aBuffer - [output] Buffer to store encoded data.
        * @param aByteIndex - [input] Starting byte index ( starts from zero ).
        *                     [output] Byte index updated after encoding.
        * @param aBitIndex - [input] Starting bit index ( starts from zero ).
        *                    [output] Bit index updated after encoding.
        * @return void
        */
        void EncodeTableOfContents( TUint8* aBuffer, 
                                    TInt& aByteIndex, 
                                    TInt& aBitIndex );
        
        /**
        * Rips off unwanted NO_DATA frames from payload according to
        * RFC3267: 4.3.2.
        *
        * @since    Series 60 3.0
        * @param    None
        * @return   Number of frames discarded from a payload
        */
        virtual TInt DiscardExtraNoDataFrames();
        
        /**
        * Saves frame so it can be sent as redundancy later.
        *
        * @since    Series 60 3.0
        * @param    aFrameData      Frame data to save
        * @return   void
        */
        virtual void SaveRedundantFrame( const TDes8& aFrameData );

    protected:  // From CAmrPayloadFormatter
    
        /**
        * Initializes encoder. Encoder should be initialized when starting
        * playing.
        *
        * @since    Series 60 3.0
        * @param    None
        * @return   void
        */
        virtual void DoInitialize();
        
    protected: // Data
        
        // Encoder for payload header
        CAmrPayloadHeader* iHeaderEncoder;

        // Encoder for TOC entry
        CAmrTocEntry* iTocEntryEncoder;
        
        // Encoder for frame data
        TStreamEncoder iFrameEncoder;
        
        // Total speech bits in current payload
        TInt iTotalSpeechBits;
        
        // Number of redundant frames to be sent
        TInt iRedCount;
        
        // Indicates is redundancy count of sent frames collected
        TBool iRedIntervalCollected;
        
        // Indicates how many NO_DATA frames is discarded at this round
        TInt iFramesDiscarded;
    private:
	
        #ifdef TEST_EUNIT
            friend class UT_CAmrPayloadEncoder;
            friend class UT_CAmrPayloadFormatWrite;
        #endif	

    };

#endif  // __AMRPAYLOADENCODER_H
