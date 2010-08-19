/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Definitions for Mcc internal codec information
*
*/





#ifndef MCCINTERNALCODECS_H
#define MCCINTERNALCODECS_H

//  INCLUDES
#include <mmf/common/mmfutilities.h>
#include <mmf/common/mmffourcc.h>
#include <mmf/common/mmfbase.h>
#include <in_sock.h>
#include "mmcccodecinformation.h"
#include "mmccnetworksettings.h"

// CONSTANTS

// Internal value for Mcc codec information in order to catch undefined values
const TUint KMccCodecParamUndefined = 0;
// Internal value for undefined payload type as max payload type is 127 (7-bit value)
// and values 0-127 are used for different payload types. Naturally then value 128 is
// then a good value for payload type.
const TUint8 KMccPayloadTypeMax = 128;

// Default ports for RTP & RTCP
//const TUint KMDefaultRtpPort = 5000;
//const TUint KDefaultRtcpPort = 5001;

// Default jitter buffer size in milliseconds
const TUint KDefautJitterBufferSize = 200;

// Number of AMR bitrates supported
const TUint KAmrBitratesSupported = 8;

// Number of G.711 bitrates supported
const TUint KG711BitratesSupported = 1;

// Number of iLBC bitrates supported
const TUint KIlbcBitratesSupported = 2;

// Number of G.729 bitrates supported
const TUint KG729BitratesSupported = 1;

// AMR BITRATES
const TUint KAmrBitrates[KAmrBitratesSupported] = 
    { 
    4750, 5150, 5900, 6700, 7400, 7950, 10200, 12200  
    };

// G.711 BITRATES
const TUint KG711Bitrates[KG711BitratesSupported] = 
    { 
    64000 
    };

// iLBC BITRATES (Can also tell the codec mode for iLBC (20ms, 30ms)
const TUint KIlbcBitrates[KIlbcBitratesSupported] = 
    { 
    13330, 15200 
    };
    
// G.729 BITRATES
const TUint KG729Bitrates[KG729BitratesSupported] = 
    {
    8000
    };

// Generic nothing mode
const TUint KMccCodecModeNone = 1;

// G.711 Codec modes as it uses one FourCC
const TUint KG711ModeaLawEnc = 2;
const TUint KG711ModeuLawEnc = 3;

// AMR Codec modes
const TUint KAmrCodecModeBandwidthEfficient = 4;
const TUint KAmrCodecModeOctetAlign = 5;

// DTMF Modes
const TUint KDtmfModeEvent = 6;
const TUint KDtmfModeTone = 7;
const TUint KDtmfModeRedEvents = 8;

// iLBC codec modes
const TUint KIlbcMode20msFrame = 9;
const TUint KIlbcMode30msFrame = 10;

// NALU constants
const TInt KMccNaluStartCodeLen = 3;
const TInt KMccNaluEndCodeLen = 2;

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*  Mcc Codec information for Mcc internal use
*
*  @lib N/A
*  @since Series 60 3.0
*/

class TMccCodecInfo
    {
    public: // Constructors
        
        /**
        * Default constructor
        */
        inline TMccCodecInfo() : 
        				iType(TUid()),
                        iFourCC( TFourCC() ),
                        iPayloadFormatEncoder( 0 ),
                        iPayloadFormatDecoder( 0 ),                         
                        iBitrate( KMccCodecParamUndefined ),
                        iSamplingFreq( 0 ), 
                        iEnableDTX( EFalse ),
                        iCodecMode( KMccCodecParamUndefined ), 
                        iAlgoUsed( ENoAlgoUsed ),
                        iPtime( KMccCodecParamUndefined ), 
                        iMaxPtime( KMccCodecParamUndefined ),
                        iPayloadType( KMccPayloadTypeMax ), 
                        iRedundantPayload( KMccPayloadTypeMax ),
                        iRedundancyCount( static_cast<TUint8>( KMccCodecParamUndefined ) ),
                        iHwFrameTime( 0 ), 
                        iFrameSize( 0 ), 
                        iCNFrameSize( 0 ),
                        iFmtpLength( 0 ),
                        iJitterSize( 0 ),
                        iNeighbor( EFalse ),
                        iModeChangePeriod( 1 ),
                        iJitterBufInactivityTimeOut( 0 ),
                        iJitterBufPlayToneTimeout( 0 ),
                        iJitterBufPlayToneFrequency( 0 ),
                        iJitterBufPlayToneDuration( 0 ),
                        iJitterBufThreshold( 0 ),
                        iJitterBufBufferLength( 0 ),
                        iComfortNoiseGenerationPt( KPayloadTypeUndefined ),
                        iBitrateMask( 0 ),
						iAverageBitrate( 0 ),
						iMaxBitrate( 0 ),
						iFramerate( 0 ),
					    iVideoWidth( 0 ),
					    iVideoHeight( 0 ),
					    iMTUSize( 1400 ),
                        iSessionID( 0 ),
                        iLinkID( 0 ),
                        iStreamID( 0 ),
                        iKeepaliveInterval( 0 ),
                        iKeepalivePT( 0 ),
                        iKeepaliveData( KNullDesC8 ),
                        iIsUpdate(0),
                        iMaxRed( -1 ),
                        iSourceDefinedTimeStamps( EFalse ),
                        iConfigKey( KNullDesC8 ),
                        iEncodingDecodingDevice( KNullUid )
            {   };

        /**
        * Copy constructor
        */
        inline TMccCodecInfo( const TMccCodecInfo& aCodec ) : 
                    iType( aCodec.iType ),         
                    iSdpName( aCodec.iSdpName ),         
                    iFourCC( aCodec.iFourCC ), 
                    iPayloadFormatEncoder( aCodec.iPayloadFormatEncoder ),
                    iPayloadFormatDecoder( aCodec.iPayloadFormatDecoder ),                         
                    iBitrate( aCodec.iBitrate ),
                    iSamplingFreq( aCodec.iSamplingFreq ), 
                    iEnableDTX( aCodec.iEnableDTX ), 
                    iCodecMode( aCodec.iCodecMode ),
                    iAlgoUsed( aCodec.iAlgoUsed ), 
                    iPtime( aCodec.iPtime ),
                    iMaxPtime( aCodec.iMaxPtime ), 
                    iPayloadType( aCodec.iPayloadType ),
                    iRedundantPayload( aCodec.iRedundantPayload ), 
                    iRedundancyCount( aCodec.iRedundancyCount ),
                    iHwFrameTime( aCodec.iHwFrameTime ),
                    iFrameSize( aCodec.iFrameSize ), 
                    iCNFrameSize( aCodec.iCNFrameSize ),
                    iFmtpLength( aCodec.iFmtpLength ),
                    iJitterSize( aCodec.iJitterSize ),
                    iNeighbor( aCodec.iNeighbor ),
                    iModeChangePeriod( aCodec.iModeChangePeriod ),
                    iJitterBufInactivityTimeOut( aCodec.iJitterBufInactivityTimeOut ),
                    iJitterBufPlayToneTimeout( aCodec.iJitterBufPlayToneTimeout ),
                    iJitterBufPlayToneFrequency( aCodec.iJitterBufPlayToneFrequency ),
                    iJitterBufPlayToneDuration( aCodec.iJitterBufPlayToneDuration ),
                    iJitterBufThreshold( aCodec.iJitterBufThreshold ),
                    iJitterBufBufferLength( aCodec.iJitterBufBufferLength ),
                    iComfortNoiseGenerationPt( aCodec.iComfortNoiseGenerationPt ),
                    iBitrateMask( aCodec.iBitrateMask ),
					iAverageBitrate( aCodec.iAverageBitrate ),
					iMaxBitrate( aCodec.iMaxBitrate ),
					iFramerate( aCodec.iFramerate ),
					iVideoWidth( aCodec.iVideoWidth ),
					iVideoHeight( aCodec.iVideoHeight ),
					iMTUSize( aCodec.iMTUSize ),
                    iSessionID( aCodec.iSessionID ),
                    iLinkID( aCodec.iLinkID ),
                    iStreamID( aCodec.iStreamID ),
                    iPriority( aCodec.iPriority ),
                    iKeepaliveInterval( aCodec.iKeepaliveInterval ),
                    iKeepalivePT( aCodec.iKeepalivePT ),
                    iKeepaliveData( aCodec.iKeepaliveData ),
                    iIsUpdate( aCodec.iIsUpdate ),
                    iMaxRed( aCodec.iMaxRed ),
                    iSourceDefinedTimeStamps( aCodec.iSourceDefinedTimeStamps ),
                    iConfigKey( aCodec.iConfigKey ),
                    iEncodingDecodingDevice( aCodec.iEncodingDecodingDevice )
                    
            {   };

    public: // Functions
    
        /**
        * Helper method for checking the codecinfo parameters
        * @since Series 60 3.0
        * @param aCodec Codec which to check
        * @return KErrNotReady or KErrNone
        */
        static inline TInt CheckCodecInfo( const TMccCodecInfo& /*aCodec*/ )
            {
            // And different codec bitrate/mode combinations should be checked also
            /*
            if(  aCodec.iFourCC == KMMFFourCCCodeNULL)
                {
                return KErrNotReady;
                }
            else if( KMccCodecParamUndefined == aCodec.iBitrate )
                {
                return KErrNotReady-1;
                }
            else if( KMccCodecParamUndefined == aCodec.iPtime )
                {
                return KErrNotReady-2;
                }
            else if( KMccCodecParamUndefined == aCodec.iMaxPtime )
                {
                return KErrNotReady-3;
                }
            else if( KMccPayloadTypeMax <= aCodec.iPayloadType )
                {
                return KErrNotReady-4;
                }
            else
                {
                return KErrNone;
                }
            */
            // Need to reimplement this as this is not a good function
            return KErrNone;
            };
            
        /**
        * Function for comparing two codec info objects
        * @since Series 60 3.0
        * @param aCodec1 Codecinfo 1 to compare
        * @param aCodec2 Codecinfo 2 to compare
        * @return TBool ETrue if aCodec1 and aCodec2 match
        */
        static inline TBool Compare( const TMccCodecInfo& aCodec1, 
                const TMccCodecInfo& aCodec2 )
            {
            if( aCodec1.iFourCC == aCodec2.iFourCC &&
                aCodec1.iBitrate == aCodec2.iBitrate && 
                aCodec1.iSamplingFreq == aCodec2.iSamplingFreq &&
                aCodec1.iVideoHeight == aCodec2.iVideoHeight &&
                aCodec1.iVideoWidth == aCodec2.iVideoWidth &&
                aCodec1.iFramerate == aCodec2.iFramerate &&
                aCodec1.iEnableDTX == aCodec2.iEnableDTX && 
                aCodec1.iCodecMode == aCodec2.iCodecMode && 
                aCodec1.iAlgoUsed == aCodec2.iAlgoUsed && 
                aCodec1.iPtime == aCodec2.iPtime && 
                aCodec1.iMaxPtime == aCodec2.iMaxPtime && 
                aCodec1.iPayloadType == aCodec2.iPayloadType && 
                aCodec1.iRedundantPayload == aCodec2.iRedundantPayload && 
                aCodec1.iRedundancyCount == aCodec2.iRedundancyCount && 
                aCodec1.iHwFrameTime == aCodec2.iHwFrameTime && 
                aCodec1.iFrameSize == aCodec2.iFrameSize && 
                aCodec1.iCNFrameSize == aCodec2.iCNFrameSize && 
                aCodec1.iJitterSize == aCodec2.iJitterSize &&
                aCodec1.iPriority.iPriority == aCodec2.iPriority.iPriority &&
                aCodec1.iPriority.iPref == aCodec2.iPriority.iPref &&
                aCodec1.iKeepaliveInterval == aCodec2.iKeepaliveInterval &&
                aCodec1.iKeepalivePT == aCodec2.iKeepalivePT &&
                aCodec1.iKeepaliveData == aCodec2.iKeepaliveData &&
                aCodec1.iSourceDefinedTimeStamps == aCodec2.iSourceDefinedTimeStamps &&
                aCodec1.iConfigKey == aCodec2.iConfigKey &&
                aCodec1.iEncodingDecodingDevice == aCodec2.iEncodingDecodingDevice )
                {
                return ETrue;
                }
            else
                {
                return EFalse;
                }
            };
            
        static TBool IsAvcPpsOrSpsData( const TDesC8& aBuffer, TBool aIgnoreStartCode = EFalse )
            {
            TBool isPpsOrSps( EFalse );
            const TUint8 KMccAvcTypeMask = 0x1F;
            const TUint8 KMccAvcTypePps = 0x7;
            const TUint8 KMccAvcTypeSps = 0x8;
            
            TInt bufferIndex( 0 );
            TInt index( KErrNotFound );
            if ( aIgnoreStartCode )
                {
                index = aBuffer.Length() > 0 ? 0 : KErrNotFound;
                }
            else
                {
                index = FindAvcNaluStart( bufferIndex, aBuffer );
                }
            
            if ( index != KErrNotFound )
                {
                TUint8 typeInfo = aBuffer[ index ];
                isPpsOrSps = ( ( typeInfo & KMccAvcTypeMask ) == KMccAvcTypePps ||
                               ( typeInfo & KMccAvcTypeMask ) == KMccAvcTypeSps );
                }
            return isPpsOrSps;
            };
        
        /**
        * This function finds a NALU's starting byte by detecting unique start code
        * occurring in the buffer, as specified in sec B.2 of H264 specs.
        * NOTE: 4-byte start code 0x00000001 is a special case of 3-byte code, 
        * automatically catered in the logic
        *
        * @param aBufferIndex, search start index, on return contains information
        * of how for buffer was searched
        * @param aBuffer
        * @return KErrNotFound if nalu start code is not found, otherwise index
        *         to beginning of end code.
        */     
        static TInt FindAvcNaluStart( TInt& aBufferIndex, const TDesC8& aBuffer )
            {
        	TUint8 a1, a2, a3;
	
        	if ( aBufferIndex < 0 )
        	    {
        		return KErrNotFound;
        	    }
        	
        	const TUint8 *origDataPtr = aBuffer.Ptr();
        	const TUint8 *dataPtr = origDataPtr;
        	TInt size( aBuffer.Size() );
        	while ( aBufferIndex+KMccNaluStartCodeLen < size )	// at least one byte after start code
        	    {
        	    dataPtr = ( origDataPtr + aBufferIndex );
        		a1 = *dataPtr;
        		dataPtr++;
        		a2 = *dataPtr;
        		dataPtr++;
        		a3 = *dataPtr;
        		
        		if ( a1 == 0x00 && a2 == 0x00 && a3 == 0x01 )
        		    {
        			aBufferIndex += KMccNaluStartCodeLen;
        			return aBufferIndex;
        		    }
        		
        		aBufferIndex++;
        	    }
        	
        	return KErrNotFound;
            };
        
        /**
        * This function returns location of first byte at the end of  NALU
        * by detecting unique end pattern, as specified in sec B.2 of H264 specs.
        *
        * @param aBufferIndex, search start index, on return contains information
        * of how for buffer was searched
        * @param aBuffer
        * @return index to end beginning of end code or last index of aBuffer if
        *         end code is not found
        */    
        static TInt FindAvcNaluEnd( TInt& aBufferIndex, const TDesC8& aBuffer )
            {
        	TUint8 a1, a2, a3;
        	
        	if ( ( aBufferIndex < 0 ) || ( aBufferIndex >= aBuffer.Size() ) )
        	    {
        		return KErrNotFound;
        	    }
        	
        	const TUint8 *origDataPtr = aBuffer.Ptr();
        	const TUint8 *dataPtr = origDataPtr;
        	TInt size( aBuffer.Size() );	
        	while ( aBufferIndex < size )
            	{
        		if ( aBufferIndex+KMccNaluEndCodeLen < size )
        		    {
        		    dataPtr = ( origDataPtr + aBufferIndex );
        			a1 = *dataPtr;
        			dataPtr++;
            		a2 = *dataPtr;
            		dataPtr++;
            		a3 = *dataPtr;
        			
        			if( ( a1 == 0x00 && a2 == 0x00 && a3 == 0x01 ) ||
        				( a1 == 0x00 && a2 == 0x00 && a3 == 0x00 ) )
        			    {
        				return aBufferIndex;
        		    	}
        	    	}
        		aBufferIndex++;
        	    }
        	
        	return aBufferIndex;
            };
        
    public: // Data
    
    	TUid iType;
    	
        /**
        * Sdp name used in SDP messages
        */
        TBuf8<KMaxSdpNameLength> iSdpName;        
    	
        // MMF FourCC code of the codec
        TFourCC iFourCC;

        // MMF format encoder for the codec
        TUint32 iPayloadFormatEncoder;

        // MMF format decoder for the codec
        TUint32 iPayloadFormatDecoder;
        
        // Codec bitrate
        TUint iBitrate;
        
        // sampling rate
        TUint32 iSamplingFreq;
        
        // Should DTX/VAD/CNG be enabled
        TBool iEnableDTX;
        
        // Codec mode
        TUint iCodecMode;
        
        // Algorithm used
        TUint iAlgoUsed;
        
        // Minimum packet time for codec
        TUint iPtime;
        
        // Maximum packet time for codec
        TUint iMaxPtime;
        
        // RTP Payload type for the codec
        TUint8 iPayloadType;
        
        // RTP payload type for redundant audio
        TUint8 iRedundantPayload;
        
        // Number of redundant frames if enabled
        TUint8 iRedundancyCount;
        
        // Frametime
        TUint8 iHwFrameTime;
        
        // Frame size
        TUint iFrameSize;
        
        // CN Frame size
        TUint8 iCNFrameSize;
        
        //Fmtp size
        TInt iFmtpLength;
        
        // Jitter buffer size
        TUint iJitterSize;

        // Mode change neighbor
        TBool iNeighbor;
        
        // Mode change period
        TUint iModeChangePeriod;
        
        //JitterBuffer InactivityTimeOut
        TUint iJitterBufInactivityTimeOut;

        //JitterBuffer tone TimeOut
        TUint iJitterBufPlayToneTimeout; 
        
        //JitterBuffer tone frequecy
        TUint iJitterBufPlayToneFrequency; 
        
        //JitterBuffer tone duration
        TUint iJitterBufPlayToneDuration;
        
        //JitterBuffer threshold
        TUint iJitterBufThreshold;
        
        //JitterBuffer buffer length
        TUint iJitterBufBufferLength;
        
        /**
         * Comfort noise generation payload type .
         * Disabled if KPayloadTypeUndefined.
         */
        TUint8 iComfortNoiseGenerationPt;
    
        //Bitrates used in codec
        TUint iBitrateMask;
    
        //Average bitrate used in codec
		TUint iAverageBitrate;
				
        //Max bitrate used in codec
		TUint iMaxBitrate;
		
        //Frame rate for video
		TReal iFramerate;

	    // Horizontal resolution of video
	    TUint iVideoWidth;

	    // Vertical resolution of video
	    TUint iVideoHeight;
	    
	    // Network MTU size (in order to determine outgoing packet's maximum size, if needed) default: 1400
	    TUint iMTUSize;
    
        // Session ID
        TUint32 iSessionID;
        
        // Link ID
        TUint32 iLinkID;

        // Stream ID
        TUint32 iStreamID;
        
        // MMF Priority settings
        TMMFPrioritySettings iPriority;       
        
        // Rtp keepalive interval (microsecs)
        TUint32 iKeepaliveInterval;
    
        // Rtp keepalive payload type
        TUint8 iKeepalivePT;
    
        // Rtp keepalive data
        TBuf8<KMaxKeepAliveDataLen> iKeepaliveData;
        
        // Indicates if update param set
        TBool iIsUpdate;
        
        /**
         * Maximum redundancy in milliseconds
         */ 
        TInt iMaxRed;

        // Trust timestamps provided by datasource
        TBool iSourceDefinedTimeStamps;
        
        // Rtp keepalive data
        TBuf8<KMaxConfigKeyLen> iConfigKey;
        
        TUid iEncodingDecodingDevice;
    };

typedef TPckgBuf<TMccCodecInfo> TMccCodecInfoBuffer;

/**
* Mcc Stream settings for Mcc internal use
* 
* @lib N/A
* @since Series 60 3.0
*/
class TMccStreamSettings
    {
    public: // Constructors
        
        /**
        * Default constructor
        */
        inline TMccStreamSettings() : iRemoteAddress( KInetAddrAny ), 
            iLocalRtpPort( KDefaultRtpPort ), iLocalRtcpPort( KDefaultRtcpPort ),
            iPriority( TMMFPrioritySettings() ), iIapId( 0 ), iRtcpEnabled( EFalse ),
            iIpTos( 0 )
            {   };
            
    public: // Data
        
        // Remote address
        TInetAddr iRemoteAddress;
        
        // Local RTP port
        TUint iLocalRtpPort;
        
        // Local RTCP port
        TUint iLocalRtcpPort;
        
        // MMF Priority settings
        TMMFPrioritySettings iPriority;
        
        // IAP ID
        TInt iIapId;
        
        // RTCP enabled
        TBool iRtcpEnabled;
        
        // TOS setting
        TInt iIpTos;
    };
    
#endif      // MCCINTERNALCODECS_H  
            
// End of File
