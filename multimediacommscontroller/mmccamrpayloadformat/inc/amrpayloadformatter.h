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
* Description:    Formatter classes for encoding/decoding of RTP payload format
*                for AMR ( narrow band ) and AMR-WB ( wide band ) audio codecs.
*
*/




#ifndef AMRPAYLOADFORMATTER_H
#define AMRPAYLOADFORMATTER_H

// INCLUDES
#include <e32base.h>
#include "amrpayloadformatutil.h"
#include "mccinternalevents.h"
#include "amrframe.h"

#ifdef FTD_ENABLED
#include <e32msgqueue.h>
#include "MccCodecStats.h"
#include "MccJBufferStats.h"
#endif

#ifdef _DEBUG 
    #include <e32svr.h>
    #define _DEBUG_AMR_DECODE
    #define _DEBUG_AMR_ENCODE 
    #define TRACE_AMR_PAYLOAD_FORMAT_READ
    #define TRACE_AMR_PAYLOAD_FORMAT_WRITE
    #define TRACE_PAYLOAD_FORMAT
#endif

  

// CONSTANTS
const TUint KAmrNbSampleRate = 8000;
const TUint KAmrWbSampleRate = 16000;

const TUint KCMRFieldBits = 4;
// Max two frames is used with AMR FEC redundancy
const TInt KMaxAmrRedCount      =   5;

// Max number of frames to save for redundancy detection
const TInt KMaxAmrFramesSaved   =   5;

// How many octets are compared while determining is a frame already received
const TInt KMaxCompareOctets    =   6;

const TInt KMaxChannelCount = 2;
const TInt KMaxFrameBlocksPerPacket = 20;
const TInt KMaxFrameCountPerPacket
    = KMaxChannelCount * ( KMaxFrameBlocksPerPacket + KMaxAmrRedCount );

const TInt KHeaderBitsBE = 4;
const TInt KHeaderBitsOA = 8;
const TInt KTOCFieldBitsBE = 6;
const TInt KTOCFieldBitsOA = 8;

const TInt KNumberOfNbModes = 8;
// In bytes; equals to 
// [( KMaxFrameCountPerPacket * maximum-frame-size ) + header + TOC + CRC]
// in octet-aligned mode maximum-frame-size = 31 ( AMR-NB ); header = 1;
// TOC = CRC = KMaxFrameCountPerPacket

// Refer to 3GPP TS 26.101 V5.0.0 and RFC 3267
// the total number of bits in speech data for different AMR frame types
//                                    total speech
//                Index   Mode           bits
//                ----------------------------
//                  0     AMR 4.75        95
//                  1     AMR 5.15       103
//                  2     AMR 5.9        118
//                  3     AMR 6.7        134
//                  4     AMR 7.4        148
//                  5     AMR 7.95       159
//                  6     AMR 10.2       204
//                  7     AMR 12.2       244
//                  8     AMR SID         39

const TInt KNumberOfNbFrameTypes = 9;
const TInt KAmrNbTotalSpeechBits[KNumberOfNbFrameTypes] =
    {95, 103, 118, 134, 148, 159, 204, 244, 39};
const TInt KMaxBitsLength = 32; // max length in bits that TStreamFormatter 
                                // can handle once when encoding and decoding
const TInt KAmrNbModes[KNumberOfNbModes] =
    {4750, 5150, 5900, 6700, 7400, 7950, 10200, 12200};

const TInt KNumberOfWbModes = 9;
// Refer to 3GPP TS 26.201 V7.0.0 and RFC 3267
// the total number of bits in speech data for different AMR frame types
//                                    total speech
//                Index   Mode           bits
//                ----------------------------
//                  0     AMR-WB 6.60    132
//                  1     AMR-WB 8.85    177
//                  2     AMR-WB 12.65   253
//                  3     AMR-WB 14.25   285
//                  4     AMR-WB 15.85   317
//                  5     AMR-WB 18.25   365
//                  6     AMR-WB 19.85   397
//                  7     AMR-WB 23.05   461
//                  8     AMR-WB 23.85   477
//                  9     AMR-WB SID      40

const TInt KNumberOfWbFrameTypes = 10;
const TInt KAmrWbTotalSpeechBits[KNumberOfWbFrameTypes] =
    {132, 177,253, 285, 317, 365, 397, 461, 477, 40};
const TInt KAmrWbModes[KNumberOfWbModes] =
    {6600, 8850, 12650, 14250, 15850, 18250, 19850, 23050, 23850};

// MACROS
#ifdef _DEBUG_AMR_DECODE
    #define DP_AMR_DECODE(a) { _LIT( KName, a); RDebug::Print(KName); }
    #define DP_AMR_DECODE2(a, b) \
    	{ _LIT(KName, a); RDebug::Print(KName, b); }
 	#define DP_AMR_DECODE3(a, b, c) \
 		{ _LIT(KName, a);  RDebug::Print(KName, b, c); }
 	#define DP_AMR_DECODE4(a, b, c, d) \
 		{ _LIT(KName, a);  RDebug::Print(KName, b, c, d); }
 	#define DP_AMR_DECODE5(a, b, c, d, e) \
 		{ _LIT(KName, a);  RDebug::Print(KName, b, c, d, e); }
 	#define DP_AMR_DECODE6(a, b, c, d, e, f) \
 		{ _LIT(KName, a);  RDebug::Print(KName, b, c, d, e, f); }
#else
    #define DP_AMR_DECODE(a);
    #define DP_AMR_DECODE2(a, b);
    #define DP_AMR_DECODE3(a, b, c);
    #define DP_AMR_DECODE4(a, b, c, d);
    #define DP_AMR_DECODE5(a, b, c, d, e);
    #define DP_AMR_DECODE6(a, b, c, d, e, f);
#endif

#ifdef _DEBUG_AMR_ENCODE
    #define DP_AMR_ENCODE(a) { _LIT( KName, a); RDebug::Print(KName); }
  	#define DP_AMR_ENCODE2(a, b) \
  		{ _LIT(KName, a); RDebug::Print(KName, b); }
    #define DP_AMR_ENCODE3(a, b, c) \
    	{ _LIT(KName, a);  RDebug::Print(KName, b, c); }
    #define DP_AMR_ENCODE4(a, b, c, d) \
    	{ _LIT(KName, a); RDebug::Print(KName, b, c, d); }
  	#define DP_AMR_ENCODE5(a, b, c, d, e) \
  		{ _LIT(KName, a); RDebug::Print(KName, b, c, d, e); }
  	#define DP_AMR_ENCODE6(a, b, c, d, e, f) \
  		{ _LIT(KName, a);  RDebug::Print(KName, b, c, d, e, f); }  
  	#define DP_AMR_ENCODE7(a, b, c, d, e, f, g) \
  		{ _LIT(KName, a);  RDebug::Print(KName, b, c, d, e, f, g); }  
#else
    #define DP_AMR_ENCODE(a);
    #define DP_AMR_ENCODE2(a, b);
    #define DP_AMR_ENCODE3(a, b, c);
    #define DP_AMR_ENCODE4(a, b, c, d);
    #define DP_AMR_ENCODE5(a, b, c, d, e);
    #define DP_AMR_ENCODE6(a, b, c, d, e, f);
    #define DP_AMR_ENCODE7(a, b, c, d, e, f, g);
#endif

#ifdef TRACE_AMR_PAYLOAD_FORMAT_READ
    #define AMR_PAYLOAD_FORMAT_READ(a) \
    	{ _LIT( KName, a); RDebug::Print(KName); }
  	#define AMR_PAYLOAD_FORMAT_READ2(a, b) \
  		{ _LIT(KName, a);  RDebug::Print(KName, b); }
    #define AMR_PAYLOAD_FORMAT_READ3(a, b, c) \
    	{ _LIT(KName, a);  RDebug::Print(KName, b, c); }
    #define AMR_PAYLOAD_FORMAT_READ4(a, b, c, d) \
    	{ _LIT(KName, a); RDebug::Print(KName, b, c, d); }
    #define AMR_PAYLOAD_FORMAT_READ5(a, b, c, d, e) \
    	{ _LIT(KName, a); RDebug::Print(KName, b, c, d, e); }
#else
	#define AMR_PAYLOAD_FORMAT_READ(a);
	#define AMR_PAYLOAD_FORMAT_READ2(a, b);
	#define AMR_PAYLOAD_FORMAT_READ3(a, b, c);
	#define AMR_PAYLOAD_FORMAT_READ4(a, b, c, d);
	#define AMR_PAYLOAD_FORMAT_READ5(a, b, c, d, e)
#endif

#ifdef TRACE_AMR_PAYLOAD_FORMAT_WRITE
    #define AMR_PAYLOAD_FORMAT_WRITE(a) \
    	{ _LIT( KName, a); RDebug::Print(KName); }
  	#define AMR_PAYLOAD_FORMAT_WRITE2(a, b) \
  		{ _LIT(KName, a);RDebug::Print(KName, b); }
    #define AMR_PAYLOAD_FORMAT_WRITE3(a, b, c) \
    	{ _LIT(KName, a);  RDebug::Print(KName, b, c); }
    #define AMR_PAYLOAD_FORMAT_WRITE4(a, b, c, d) \
    	{ _LIT(KName, a); RDebug::Print(KName, b, c, d); }
#else
	#define AMR_PAYLOAD_FORMAT_WRITE(a)
	#define AMR_PAYLOAD_FORMAT_WRITE2(a, b)
	#define AMR_PAYLOAD_FORMAT_WRITE3(a, b, c)
	#define AMR_PAYLOAD_FORMAT_WRITE4(a, b, c, d)
#endif
    
#ifdef TRACE_PAYLOAD_FORMAT
    #define TRACE_PAYLOAD_FORMAT_PRINT(a) \
    	{ _LIT( KName, a); RDebug::Print(KName); }
#else
	#define TRACE_PAYLOAD_FORMAT_PRINT(a)
#endif
  

// DATA TYPES


// CLASS DECLARATION


/**
*  The AMR payload formatter for an RTP packet.
*
*  @lib <TBD>
*  @since Series-60 2.1
*/
class CAmrPayloadFormatter : public CBase
    {
    public:  // Constructors and destructor

        /**
        * Destructor.
        */
        virtual ~CAmrPayloadFormatter( );

    public: // New functions

        /**
        * Initializes AMR encoder/decoder. Initialization must be done when playback
        * is started.
        * @since    Series 60 3.0
        */
        void Initialize();
        
        /**
        * Set payload buffer.
        * Calling function must have allocated memory for the buffer.
        * @since Series-60 2.1
        * @param aBuffer - [input] Descriptor of payload buffer
        * @return None
        */
        virtual void SetPayloadBuffer( TDes8& aBuffer );

        /**
        * Set number of audio channels.
        * @since Series-60 2.1
        * @param aChannelCount - [input] Number of audio channels
        * @return TInt - KErrNone if successful; system wide error code otherwise
        */
        virtual TInt SetChannelCount( TInt aChannelCount );

        /**
        * Set number of AMR frame blocks included in one RTP packet.
        * Each frame block contains frames for all audio channels.
        * @since Series-60 2.1
        * @param aFrameblockCount - [input] Number of AMR frame blocks included 
        * in one RTP packet
        * @return TInt - KErrNone if successful; system wide error code otherwise
        */
        virtual TInt SetFrameBlockCount( TInt aFrameblockCount );

        /**
        * Get the payload buffer.
        * @since Series-60 2.1
        * @return TDes8& - Descriptor of payload buffer
        */
        inline TDes8& PayloadBuffer( ) { return iPayloadPtr; };
        
        /**
        * Get number of speech bits in an AMR frame
        * @since Series-60 2.1
        * @param aFrameType - [input] AMR frame type
        * @return TInt - Number of speech bits in an AMR frame
        */
        TInt SpeechBitCount( enum TAmrFrameType aFrameType ) const; 

        /**
        * Get number of audio channels.
        * @since Series-60 2.1
        * @return TInt - Number of audio channels
        */
        inline TInt ChannelCount( ) const { return iChannelCount; };

        /**
        * Get number of AMR frame blocks included in one RTP packet.
        * Each frame block contains frames for all audio channels.
        * @since Series-60 2.1
        * @return TInt - Number of AMR frame blocks
        */
        inline TInt FrameBlockCount( ) const { return iFrameBlockCount; };

        /**
        * Get number of AMR frame included in one RTP packet.
        * @since Series-60 2.1
        * @return TInt - Number of AMR frames
        */
        inline TInt FrameCount( ) const { return iFrameCount; };

    protected:  // Functions

        /**
        * Constructor.
        */
        CAmrPayloadFormatter( TBool aIsNb );
        
        virtual void DoInitialize() = 0;

    protected:  // Data

		// TRUE, if this is formater for narrow band AMR. FALSE for wide band.
		TBool iIsNb;

        // Payload parameters
        // Number of audio channels
        TInt iChannelCount;             
        
        // Number of AMR frame blocks included in one packet
        TInt iFrameBlockCount;
        
        // Number of AMR frames included in one packet
        TInt iFrameCount;
        
        // Codec mode request
        TAmrModeRequest iModeRequest;

        // Frame information
        // AMR frames for one RTP packet
        TAmrFrame iFrames[KMaxFrameCountPerPacket];

        // Current index to `iFrames' array
        TInt iFrameIndex;

        // Payload data buffer info
        // Pointer descriptor for payload buffer
        TPtr8 iPayloadPtr;
        
        // Pointer to payload buffer
        TUint8* iPayload;
        
        // Size, in bytes, of payload buffer
        TInt iPayloadSize;
        
        // Frames either for redundancy detection or redundancy sending
        RPointerArray<HBufC8> iFrameDatas;
        
        #ifdef FTD_ENABLED
		// Message queue carrying codec statistics
        RMsgQueue<TMccCodecStats> iCodecStatsQueue;

		// Message queue carrying jitter buffer statistics
        RMsgQueue<TMccJBufferStats> iJBufStatsQueue;
        #endif
        
    };

#endif      // __AMRPAYLOADFORMATTER_H

// End of File
