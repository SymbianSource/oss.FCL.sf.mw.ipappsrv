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
* Description:    Class is used for configuring media codecs.
*
*/




#ifndef MMCCCODECINFORMATION_H
#define MMCCCODECINFORMATION_H

// INCLUDES
#include <e32base.h>
#include <e32des8.h>
#include <e32svr.h>
#include <mmf/common/mmfcontroller.h>

// FORWARD DECLARATIONS

class TMccCodecInfo;

// CONSTANTS

// Codec names used in SDP messages
_LIT8( KPCMUSdpName, "PCMU" );
_LIT8( KPCMASdpName, "PCMA" );
_LIT8( KAMRSdpName, "AMR" );
_LIT8( KAMRWbSdpName, "AMR-WB" );
_LIT8( KILBCSdpName, "iLBC" );
_LIT8( KG729SdpName, "G729" );
_LIT8( KTelephoneEvent, "telephone-event" );
_LIT8( KRedSdpName, "red" );
_LIT8( KCnSdpName, "CN" );
_LIT8( KH263SdpName, "H263" );
_LIT8( KH2631998SdpName, "H263-1998" );
_LIT8( KH2632000SdpName, "H263-2000" );
_LIT8( KAVCSdpName, "H264" );
_LIT8( KAACSdpName, "AAC" );
_LIT8( KIlbcFmtp, "mode=" );
_LIT8( KSemicolonSpace, "; " );
_LIT8( KCommaSign, "," );
const TInt KPTimeFactor = 1000;

// Maximum buffer length for SDP names
const TInt KMaxSdpNameLength = 20;
const TUint8 KMaxPacketrate = 10;
const TInt KMaxFmtpLen( 120 ); // Change if this gets too small
const TInt KMaxKeepAliveDataLen( 161 );

// Dynamic payload type limits, see RFC 3551
const TUint8 KMaxPayloadType = 127;
const TUint8 KMinDynamicPT = 96;
const TUint8 KPayloadTypeUndefined = 128;

// Default payload types for codecs that use dynamic payload types
const TUint8 KDefaultAmrNbPT = 96;
const TUint8 KDefaultIlbcPT = 97;
const TUint8 KDefaultDtmfPT = 98;
const TUint8 KDefaultRedPT = 99;
const TUint8 KDefaultAmrWbPT = 100;

// Static payload types, see RFC 3551
const TUint8 KPcmuPayloadType = 0;
const TUint8 KPcmaPayloadType = 8;
const TUint8 KG729PayloadType = 18;
const TUint8 KCnPayloadType = 13;
const TUint8 KCnPayloadTypeReserved = 19;

// Packet length constants, in milliseconds
const TUint KMinPtime = 10;
const TUint KDefaultPtime = 20;
const TUint KMaxPtime = 200;

// Max gen red count
const TUint KMaxGenRedCount = 1;

// Max config key length
const TInt KMaxConfigKeyLen = 100;

// FourCC values for different codecs supported by MCC

// (' ', 'A', 'M', 'R')
const TUint32 KMccFourCCIdAMRNB = 0x524d4120;

// (' ', 'A', 'W', 'B')
const TUint32 KMccFourCCIdAMRWB = 0x42574120;

// ('G', '7', '1', '1')
const TUint32 KMccFourCCIdG711 = 0x31313747;

// ('G', '7', '2', '9')
const TUint32 KMccFourCCIdG729 = 0x39323747;

// ('i', 'L', 'B', 'C')
const TUint32 KMccFourCCIdILBC = 0x43424c49;

// ( 'D', 'T', 'M', 'F' )
const TUint32 KMccFourCCIdDTMF = 0x464d5444;

// ( 'H', '2', '6', '3' )
const TUint32 KMccFourCCIdH263 = 0x461A1B1C;

// ( 'H', '2', '6', '4' )
const TUint32 KMccFourCCIdAVC = 0x461A1BDD;

// Red - not official
const TUint32 KMccFourCCIdRed = 0x4DDDDDDD;

// Comfort noise - not official
const TUint32 KMccFourCCIdCN = 0x4DDDDDDE;


// DATA TYPES
/**
* Codec mode
*/
typedef TUint TCodecMode;

const TUint ENothing = 1;
const TUint EPCMU = 2;
const TUint EPCMA = 3;
const TUint EBandwidthEfficient = 4;
const TUint EOctetAligned = 5;
const TUint EDTMFModeEvent = 6;
const TUint EDTMFModeInband = 7;
const TUint EDTMFModeTone = 8;
const TUint EDTMFModeRedEvents = 9;

/**
* Algorithm used
*/      
typedef TUint TAlgoUsed;

const TUint ENoAlgoUsed = 1;  
const TUint EGenRedUsed = 2;
const TUint EAmrFecUsed = 3; 

// FUNCTION PROTOTYPES

// CLASS DECLARATION
/**
*  Container class for the codec information.
*
*  @lib mmccinterface.dll
*/
class CMccCodecInformation : public CBase
{
public:  // Constructors and destructor

    /**
    * Destructor.
    */
    virtual ~CMccCodecInformation();

protected:  // New functions
    
    /**
    * C++ default constructor.
    */
    CMccCodecInformation();
    
public: // New functions

    /**
    * Constructs a clone from this codec object. Note that the clone falls
    * back to given codec's default settings.
    * @return CMccCodecInformation Pointer to the newly created codec clone
    */
    virtual CMccCodecInformation* CloneDefaultsL() = 0;
    
    /**
    * Constructs a clone from this codec object and copies the original 
    * object's current settings to the newly created clone.
    * @return CMccCodecInformation Pointer to the newly created codec clone
    */
    virtual CMccCodecInformation* CloneDetailedL() = 0;
            
    /**
    * Checks the codec values if signalling is needed.
    * @param aCandidate new codec to be checked
    * @return ETrue if signalling is needed
    */
    virtual TBool RequireSignalling( const CMccCodecInformation& aCandidate ) const;      
       
public: // sets
    
    virtual void SetValues( TMccCodecInfo codecInfo );
        
    virtual void GetValues( TMccCodecInfo& codecInfo ) const;

    /**
    * Type of the codec, like KUidMediaTypeAudio or KUidMediaTypeVideo.
    * @return type of the codec.
    */
    virtual TUid Type() const;
    
    /**
    * Enable / Disable Voice Activity Detection.
    * @param aEnableVAD Boolean indicating whether to enable
    *     Voice Activity Detection
    * @return KErrNotSupported if codec doesn't support VAD; otherwise KErrNone.
    */
    virtual TInt EnableVAD( TBool aEnableVAD ) = 0;
 
    /**
    * Sets bitrate used with codec.
    * @param aBitrate New bitrate value
    * @return KErrNotSupported if codec doesn't support bitrate value
    *     issued; otherwise KErrNone.
    */
    virtual TInt SetBitrate( TUint aBitrate ) = 0;
    
    /**
    * Sets the sampling frequency.
    * @param aSamplingFreq Sampling frequency to be used
    * @return KErrNotSupported if codec doesn't support sampling frequency
    *     issued; otherwise KErrNone.
    */
    virtual TInt SetSamplingFreq( TUint32 aSamplingFreq ) = 0;
    
    /**
    * Sets the sdp name of the codec.
    * @param aSdpName sdp name of the codec.
    * @return KErrNotSupported if not allowed; otherwise KErrNone.
    */
    virtual TInt SetSdpName( const TDesC8& aSdpName ) = 0;
    
    /**
    * Sets the payload type.
    * @param aPayloadType type identifier to be used for payload.
    * @return KErrNotSupported if not allowed; otherwise KErrNone.
    */
    virtual TInt SetPayloadType( TUint8 aPayloadType ) = 0;

    /**
    * Sets the codec mode.
    * @param aMode Codec mode, eg. ULAW/ALAW, bandwidth eff / octet aligned
    * @return KErrNotSupported if codec doesnt' support codec mode
    *     value issued; otherwise KErrNone.
    */
    virtual TInt SetCodecMode( TCodecMode aCodecMode ) = 0;
    
    /**
    * Sets the allowed bitrates for the codec.
    * @param aBitrateMask bitrate mask.
    * @return KErrNotSupported if codec doesnt' support the bitrate mask
    *     value issued; otherwise KErrNone.
    */
    virtual TInt SetAllowedBitrates( TUint aBitrateMask );
    
    /**
    * Sets the packet time
    * @param aPTime PacketTime.
    * @return One of the standard system-wide error codes.
    */
    virtual TInt SetPTime( TUint aPTime ) = 0;        
    
    /**
    * Sets max packet time
    * @param aMaxPTime Maximum PacketTime
    * @return One of the standard system-wide error codes.
    */
    virtual TInt SetMaxPTime( TUint aMaxPTime ) = 0; 
                     
    /**
    * Set JitterBuffer Buffer length
    * @param aJitterBufBufferLength length for buffer
    * @return One of the standard system-wide error codes.
    */
    virtual TInt SetJitterBufBufferLength( TUint aJitterBufBufferLength );      

    /**
    * Set JitterBuffer threshold
    * @param aJitterBufThreshold threshold value for buffer before playout starts
    * @return One of the standard system-wide error codes.
    */
    virtual TInt SetJitterBufThreshold( TUint aJitterBufThreshold );      
   
    /**
    * Set JitterBuffer inactivity timeout in millisecounds
    * @param aJitterBufInactivityTimeOut timeout value for RTP inactivity
    * @return One of the standard system-wide error codes.
    */
    virtual TInt SetJitterBufInactivityTimeOut( TUint aJitterBufInactivityTimeOut );     

    /**
    * Sets max bitrate
    * @param aMaxBitrate Maximum bitrate
    * @return One of the standard system-wide error codes.
    */
    virtual TInt SetMaxBitrate( TUint aMaxBitrate ); 

    /**
    * Sets average bitrate
    * @param aAverageBitrate average bitrate
    * @return One of the standard system-wide error codes.
    */
    virtual TInt SetAverageBitrate( TUint aAverageBitrate ); 

    /**
    * Sets framerate
    * @param aFramerate frame rate
    * @return One of the standard system-wide error codes.
    */
    virtual TInt SetFramerate( TReal aFramerate ); 

    /**
    * Sets height of the frame
    * @param aFrameHeight height of the frame
    * @return One of the standard system-wide error codes.
    */
    virtual TInt SetFrameHeight( TUint aFrameHeight ); 

    /**
    * Sets width of the frame
    * @param aFrameWidth width of the frame
    * @return One of the standard system-wide error codes.
    */
    virtual TInt SetFrameWidth( TUint aFrameWidth ); 
    
    /**
    * Set algorithm.
    * @param aAlgo codec specific algorithm to be used
    * @return One of the standard system-wide error codes.
    */
    virtual TInt SetAlgo( TAlgoUsed aAlgo );
    
    /**
    * Set redundancy count.
    * @param aRedCount count for redundancy
    * @return One of the standard system-wide error codes.
    */
    virtual TInt SetRedCount( TUint aRedCount );
    
    /**
    * Set redundancy payload type.
    * @param aRedPT payload type for redundancy
    * @return One of the standard system-wide error codes.
    */
    virtual TInt SetRedundancyPT( TUint8 aRedPT );
         
    /**
    * Set priority for media resource.
    * @param aPriority priority for media resource
    * @return One of the standard system-wide error codes.
    */
    virtual void SetPriority( TInt aPriority );
    
    /**
    * Set priority preference for media resource.
    * @param aPreference priority preference for media resource
    * @return One of the standard system-wide error codes.
    */
    virtual void SetPriorityPreference(TInt aPreference );

    /**
    * Set keep alive packets send timer value.
    * @param aTimerValue timer value in seconds for keep alive packets
    * @return One of the standard system-wide error codes.
    */
    virtual TInt SetKeepAliveTimer( TUint8 aTimerValue );

    /**
    * Set keep alive packets payload type.
    * @param aKeepAlivePT payload type for keep alive packets
    * @return One of the standard system-wide error codes.
    */
    virtual TInt SetKeepAlivePT( TUint8 aKeepAlivePT );

    /**
    * Set keep alive packets payload data.
    * @param aData payload data for keep alive packets
    * @return One of the standard system-wide error codes.
    */
    virtual TInt SetKeepAliveData( const TDesC8& aData );
    
    /**
    * Enable / Disable Comfort noise generation.
    * @param aComfortNoisePT Payloadtype value indicating what
    *        payload type should be used for comfort noise.
    *        KPayloadTypeUndefined can be used to turn comfor noise
    *        generation off.
    * @return One of the standard system-wide error codes.
    */
    virtual TInt SetComfortNoiseGeneration( TUint8 aComfortNoisePT );

    
    /**
    * Set number of channels. Range between 1 to 8. Refer to RFC3551 4.1
    * for interpretation of channels value. Default value is 1.
    * @return   One of the standard system-wide error codes.
    */
    virtual TInt SetChannels( TInt aNumOfChannels );
        
    /**
    * Sets the upper limit on the usage of redundancy in milliseconds.
    * @param    aMaxRed   maximum redundancy
    * @return   One of the standard system-wide error codes.
    */
    virtual TInt SetMaxRed( TInt aMaxRed );

    /**
    * Set encoder/decoder configuration key
    * @param aConfigKey, config key as Base64 encoded
    */    
    virtual void SetConfigKeyL( const TDesC8& aConfigKey );
    
    /**
    * Sets preferred encoding device for uplink stream and preferred decoding
    * device for downlink stream.
    * @param aEncodingDecodingDevice
    */
    virtual void SetPreferredEncodingDecodingDevice( 
                                    TUid aEncodingDecodingDevice );
              
public: // get

    /**
    * Get VAD status.
    * @return Boolean indicating is VAD activated
    */
    virtual TBool VAD( ) const;

    /**
    * Gets bitrate value used currently.
    * @return Bitrate value
    */
    virtual TUint Bitrate( ) const;
    
    /**
    * Gets the bitrate mask 
    * @return Bitrate mask
    */
    virtual TUint AllowedBitrates() const;

    /**
    * Gets sampling frequency used currently.
    * @return Sampling frequency
    */
    virtual TUint32 SamplingFreq( ) const;

    /**
    * Gets the sdp name.
    * @return Sdp name used
    */
    virtual const TDesC8& SdpName() const;
    
    /**
    * Gets the payload type.
    * @return Payload type used
    */
    virtual TUint8 PayloadType( ) const;

    /**
    * Gets the codec mode
    * @return Codec mode used
    */
    virtual TCodecMode CodecMode( ) const;
    
    /**
    * Gets the packet time.
    * @return Packet Time.
    */
    virtual TInt PTime( ) const;        
    
    /**
    * Gets max packet time.
    * @return Max PacketTime.
    */
    virtual TInt MaxPTime( ) const;        
    
    /**
    * Gets the codec specific fmtp value for SDP.
    * @return Fmtp.
    */
    virtual TDesC8& GetFmtpL( );        
       
    /**
    * Codec FourCC.
    * @return FourCC.
    */        
    virtual TUint32 FourCC( ) const;
            
    /**
    * Get HW FrameTime.
    * @return FrameTime in milliseconds
    */
    virtual TInt FrameTime( ) const;
    
    /**
    * Get FrameSize.
    * @return FrameSize in bytes
    */
    virtual TInt FrameSize( ) const;
            
    /**
    * Get Algorithm used.
    * @return FrameSize in bytes
    */
    virtual TAlgoUsed Algo( ) const;       
           
    /**
    * Get Redundancy Count.
    * @return Redundancy Count
    */
    virtual TUint RedCount( ) const;    
            
    /**
    * Get Redundancy PayloadType.
    * @return Redundancy PayloadType
    */
    virtual TUint8 RedundancyPT( ) const; 
    
    /**
    * Get JitterBuffer inactivity timeout
    * @return timeout time.
    */
    virtual TUint JitterBufInactivityTimeOut() const;  
    
    /**
    * Get JitterBuffer threshold
    * @return threshold.
    */
    virtual TUint JitterBufThreshold() const;      
           
    /**
    * Get JitterBuffer Buffer length
    * @return Buffer length.
    */
    virtual TUint JitterBufBufferLength() const;      
           
    /**
    * Gets max bitrate
    * @return Maximum bitrate.
    */
    virtual TUint MaxBitrate() const; 

    /**
    * Gets average bitrate
    * @return average bitrate.
    */
    virtual TUint AverageBitrate() const; 

    /**
    * Gets framerate
    * @return frame rate.
    */
    virtual TReal Framerate() const; 

    /**
    * Gets height of the frame
    * @return height of the frame.
    */
    virtual TUint FrameHeight() const; 

    /**
    * Gets width of the frame
    * @return width of the frame.
    */
    virtual TUint FrameWidth() const; 
                                         
    /**
    * Gets media priority
    * @return media priority.
    */
    virtual TInt Priority( ) const;

    /**
    * Gets media priority preference
    * @return media priority preference.
    */
    virtual TInt PriorityPreference( ) const;

    /**
    * Get keep alive packets send timer value in seconds.
    * @return keep alive packets send timer value in seconds.
    */
    virtual TUint8 KeepAliveTimer() const;

    /**
    * Get keep alive packets payload type.
    * @return keep alive packets payload type.
    */
    virtual TUint8 KeepAlivePT() const;

    /**
    * Get keep alive packets payload data.
    * @return keep alive packets payload data.
    */
    virtual const TDesC8& KeepAliveData() const;
    
    /**
    * Get Comfort noise payload type value.
    * @return Payload type value indicating which 
    *         payload type is used for comfort noise generation.
    *         If KPayloadTypeUndefined is returned, comfort
    *         noise generation is disabled.
    */
    virtual TUint8 ComfortNoiseGeneration() const;
    
    /**
    * Get number of channels.
    * @return   One of the standard system-wide error codes.
    */
    virtual TInt GetChannels( TInt& aNumOfChannels ) const;
        
    /**
    * Returns the maximum redundancy
    * @return   maximum redundancy in milliseconds
    */
    virtual TInt MaxRed() const;
          
    /**
    * Fills array with red payload types.
    * @param        aRedPayloads   
    */
    virtual void RedPayloadsL( RArray<TUint>& aRedPayloads ) const;
    
    /**
    * Get encoder/decoder configuration key
    * @return key or NULL, ownership is transferred
    */    
    virtual HBufC8* ConfigKeyL() const;
    
    /**
    * Get preferred encoding device for uplink stream and preferred decoding
    * device for downlink stream.
    */
    virtual TUid PreferredEncodingDecodingDevice() const;

public: //Fmtp, set, parse and create fmtp string from internal attributes

    /**
    * Sets the fmtp attribute, i.e. iFmtpAttr == aFmtp  
    * @param aFmtp Fmtp attribute
    * @return void
    */
    virtual void SetFmtpAttrL( const TDesC8& aFmtp, TBool parseFmtp = ETrue );
    
    /**
    * Parses the fmtp attributes, and updates the internal values
    * except the iFmtpAttr attribute
    * @param aFmtp Fmtp attribute
    * @return True if any param was updated
    */
    virtual TBool ParseFmtpAttrL( const TDesC8& aFmtp );      
   
    /**
    * Creates the fmtp string from internal values
    * @return void
    */
    virtual void CreateFmtpAttrListL();
  
    /**
    * Creates the fmtp string from internal values and the given bitrates
    * @return void
    */
    virtual void CreateFmtpAttrListL( const RArray<TUint>& aBitrates );

private:

    /**
    * Specifies the number of frame blocks,
    * that is the interval at which codec mode changes are allowed
    * @return   One of the standard system-wide error codes.
    */
    virtual TInt SetModeChangePeriod( const TInt aPeriod );
    
    /**
    * Get specified number of frame blocks
    * @return   One of the standard system-wide error codes.
    */
    virtual TInt ModeChangePeriod( ) const;
    
    /**
    * Set boolean variable, which indicates change between two modes.
    * With default value 1 changes are only allowed to the 
    * neighboring mode.
    * @return   One of the standard system-wide error codes.
    */
    virtual TInt SetModeChangeNeighbor( const TBool aNeighbor );
    
    /**
    * Get boolean variable, which indicates change between two modes.
    * @return   iNeighbor
    */
    virtual TBool ModeChangeNeighbor( ) const;
    
    /**
    * Set boolean type frame CRC value. With value 1 CRC shall be
    * included in the payload, otherwise not. 1 also implies that
    * octet-alignet operation shall be used for the session.
    * @return   One of the standard system-wide error codes.
    */
    virtual TInt SetCrc( const TBool aCrc );
    
    /**
    * Get boolean type CRC value.
    * @return   iCrc
    */
    virtual TBool GetCrc( ) const;
    
    /**
    * Set robust sorting on/off with boolean value. With value 1
    * the payload SHALL employ robust sorting. Implies automatically
    * that octet-aligned operation SHALL be used for the session.
    * @return   One of the standard system-wide error codes.
    */     
    virtual TInt SetRobustSorting( const TBool aRobust );
    
    /**
    * Get boolean type robust sorting value
    * @return   iRobust
    */
    virtual TBool GetRobustSorting( ) const;
    
    /**
    * Set interleaving as unsigned integer. Number range is 0-63.
    * Indicates that frame-block level interleaving SHALL be
    * used for the session and it's value defines the max number of
    * frame-blocks allowed in interleaving group. Implies automatically
    * that octet-aligned operation SHALL be used.
    * @return   One of the standard system-wide error codes.
    */
    virtual TInt SetInterleaving( const TInt aFrameBlockCount );
    
    /**
    * Get Interleaving value
    * @return   iFrameBlockCount
    */
    virtual TInt GetInterleaving() const;
          
public:

   /**
    * Set keep alive packets send timer value.
    * @param aTimerValueMicroSecs timer value in microseconds for keep alive packets
    * @return One of the standard system-wide error codes.
    */
	virtual TInt SetKeepAliveTimerMicroSecs( TUint32 aTimerValueMicroSecs );
	
	/**
    * Get keep alive packets send timer value in microseconds.
    * @return keep alive packets send timer value in microseconds.
    */
	virtual TUint32 KeepAliveTimerMicroSecs() const;
	   
protected:  // Data

    /**
    * Type of codec, like KUidMediaTypeAudio or KUidMediaTypeVideo
    */
    TUid iType;
    
    /**
    * Enables or disables VAD (Voice Activity Detection)
    */
    TBool iEnableVAD;

    /**
    * Sampling frequency used with codec
    */
    TUint32 iSamplingFreq;

    /**
    * Sdp name used in SDP messages
    */
    TBuf8<KMaxSdpNameLength> iSdpName;        

    /**
    * Stream's packet time
    */
    TUint iPTime;
    
    /**
    * Stream's max packet time
    */
    TUint iMaxPTime;
    
    /**
    * Fmtp attribute
    */
    HBufC8* iFmtpAttr;
    
    /**
    * Parsed fmtp attr
    */
    TUint iParsedFmtp;
    
    /**
    * Stream's bit rate
    */
    TUint iBitrate;

    /**
    * Bitrate mask, allowed modes
    */
    TUint iBitrateMask;
    
    /**
    * Payload type
    */
    TUint8 iPayloadType;

    /**
    * Codec mode ( e.g. PCMU, PCMA, Bandwidth efficient, octet aligned )
    */
    TCodecMode iCodecMode;        

    /**
    * Local port
    */
    TUint iStartMediaPort;
        
    /**
    * Codec FourCC
    */
    TUint32 iFourCC;
    
    /**
    * FrameTime
    */
    TInt iHwFrameTime;
    
    /**
    * FrameSize
    */
    TInt iFrameSize;
    
    /**
    * Algorithm used
    */
    TAlgoUsed iAlgoUsed;
    
    /**
    * Redundancy count
    */
    TUint iRedCount;
    
    /**
    * Redundancy PayloadType
    */
    TUint8 iRedundancyPT;
    
    /**
    * JitterBuffer InactivityTimeOut
    *
    * Used for talkburst
    * In millisecound when the sequence number should be zerod 
    * after a talkburst 
    * ( iJitterBufInactivityTimeOut / iHwFrameTime ) == InactivityEvent
    */
    TUint iJitterBufInactivityTimeOut;
    
    /**
    * JitterBuffer threshold
    *
    * How many packets should be in the buffer
    * before they are played
    */
    TUint iJitterBufThreshold;
    
    /**
    * JitterBuffer threshold
    *
    * How many packets the jitter buffer can hold
    */
    TUint iJitterBufBufferLength;
           
    /**
    * Mode change period
    */
    TInt iModeChangePeriod;
    
    /**
    * Mode change neighbor
    */
    TBool iNeighbor;
    
    /**
    * Crc value
    */
    TBool iCrc;
    
    /**
    * Robust sorting indicator
    */
    TBool iRobust;
    
    /**
    * Interleaving value
    */
    TInt iFrameBlockCount;
    
    /**
     * Number of channels
     */
    TInt iNumOfChannels;
        
    /**
     * Payload type for comfort noise generation
     */
    TUint8 iComfortNoiseGenerationPt;
           
    /**
    * MMF priority settings
    */
    TInt iPriority;
            
    /**
    * MMF priority settings
    */
    TInt iPriorityPreference;
    
    /**
    * Average bitrate used in codec
    */
    TUint iAverageBitrate;
            
    /**
    * Max bitrate used in codec
    */
    TUint iMaxBitrate;
    
    /**
    * Frame rate for video
    */
    TReal iFramerate;

    /**
    * Horizontal resolution of video
    */
    TUint iVideoWidth;

    /**
    * Vertical resolution of video
    */
    TUint iVideoHeight;
    
    /**
    * Timer value for keep alive data, seconds
    */
    TUint8 iKeepAliveTimer;
    
    /**
    * Payload type value for keep alive data
    */
    TUint8 iKeepAlivePT;
    
    /**
    * Data for keep alive
    */
    TBuf8<KMaxKeepAliveDataLen> iKeepAliveData;
            
    // MMF format encoder for the codec
    TUint32 iPayloadFormatEncoder;

    // MMF format decoder for the codec
    TUint32 iPayloadFormatDecoder;
    
    /**
    * Maximum redundancy in milliseconds
    */
    TInt iMaxRed;
    
    /**
    * Encoder/decoder configuration key
    */
    HBufC8* iConfigKey;
    
    /**
    * Encoder/decoder uid
    */
    TUid iEncodingDecodingDevice;
    
	/**    
	* Timer value for keep alive data, microseconds
    */
    TUint32 iKeepAliveTimerMicroSecs;
	
private:

    #ifdef TEST_EUNIT
        friend class UT_CMccCodecInformation;
    #endif
};

#endif      // MMCCCODECINFORMATION_H

// End of File
