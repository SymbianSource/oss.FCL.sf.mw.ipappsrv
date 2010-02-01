/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    
*
*/




#ifndef MCEAVCCODEC_H
#define MCEAVCCODEC_H

// INCLUDES
#include <mcevideocodec.h>
#include <mcedefs.h>

// CONSTANTS

const TUint KMceAvcClockRate = 90000;

// Allowedbitrates bitfield combines avc profile-level-id information in
// similar manner  as defined in RFC 3984. Most significant byte stores
// profile-id value, second significant byte contains profile-iop value and
// last two bytes contain profile-level information.
//
#define MCE_AVC_ALLOWED_BITRATES( avc_profile_id, avc_profile_iop, avc_profile_bitrate_level ) \
( avc_profile_id | avc_profile_iop | avc_profile_bitrate_level )

const TUint KMceAvcBitrateLevel1 = 0x0001;
const TUint KMceAvcBitrateLevel1b = 0x0002;
const TUint KMceAvcBitrateLevel1_1 = 0x0004;
const TUint KMceAvcBitrateLevel1_2 = 0x0008;
const TUint KMceAvcBitrateLevel1_3 = 0x0010;
const TUint KMceAvcBitrateLevel2 = 0x0020;

const TUint KMceAvcCodecProfileIopConstraintSet = 0x0100;
const TUint KMceAvcCodecProfileIopConstraintSet1 = 0x0200;
const TUint KMceAvcCodecProfileIopConstraintSet2 = 0x0400;

const TUint KMceAvcCodecProfileIdBaseline = 0x1000;
const TUint KMceAvcCodecProfileIdMain = 0x2000;
const TUint KMceAvcCodecProfileIdExtended = 0x4000; 
const TUint KMceAvcCodecProfileIdHigh = 0x8000; 

const TUint KMceAllowedAvcBitrateAll = 0x00FF;

// Different framerates may be supported depending on profile level.
//
const TUint KMceAvcFrameRate7_5 = 0x0001;
const TUint KMceAvcFrameRate12_5 = 0x0002;
const TUint KMceAvcFrameRate15 = 0x0004;
const TUint KMceAvcFrameRate30 = 0x0008;
const TUint KMceAvcFrameRate36 = 0x0010;

const TUint KMceAllowedAvcFrameRateAll = 0x00FF;

// Different resolutions may be supported depending on profile level.
//
const TUint KMceAvcResolution176x144 = 0x0001;
const TUint KMceAvcResolution320x240 = 0x0002;
const TUint KMceAvcResolution352x288 = 0x0004;

const TUint KMceAllowedAvcResolutionAll = 0x000F;


// DATA TYPES
enum TAvcCodecMode
    {
    KMceAvcModeSingleNal,
    KMceAvcModeNonInterleaved,
    KMceAvcModeInterleaved
    };
    
    
// CLASS DECLARATION

/**
* Class for the H264 codec information.
*
* It checks the validity of inserted values for H264
* 
*  @lib mceclient.lib
*/
class CMceAvcCodec : public CMceVideoCodec
    {
    
	public:  // Constructors and destructor
	    
	    /**
	    * Destructor.
	    */
	    IMPORT_C ~CMceAvcCodec();

	public: 

	    /**
	    * Two-phased constructor.
	    * @param aSdpName sdp name
	    */
	    IMPORT_C static CMceAvcCodec* NewL( TBuf8<KMceMaxSdpNameLength> aSdpName);

	    /**
	    * Two-phased constructor.
	    * @param aSdpName sdp name
	    */
	    IMPORT_C static CMceAvcCodec* NewLC( TBuf8<KMceMaxSdpNameLength> aSdpName );
	        
	public: // From base classes

	    /**
	    * Sets bitrate used with codec for encoding.
	    * @param aBitrate bitrate value for encoding
	    * @return KErrNotSupported if codec doesn't support bitrate
	    *         value issued
	    */
	    virtual TInt SetBitrate( TUint aBitrate );
	    
	    /**
	    * Sets bitrates allowed with codec.
	    * @param aBitrates allowed bitrate values
	    * @return KErrNotSupported if codec doesn't support bitrate
	    *         values issued
	    */
	    virtual TInt SetAllowedBitrates( TUint aBitrates );
	    
	    /**
	    * Sets the codec specific mode.
	    * @param aCodecMode mode of the codec 
	    * @return KErrNotSupported if codec doesnt' support codec mode
	    *         value issued
	    */
	    virtual TInt SetCodecMode( TUint aCodecMode );
	    
	    /**
	    * Sets the payload type.
	    * @param aPayloadType type identifier of the payload
	    * @return KErrNotSupported if codec doesn't support payload type issued
	    */
	    virtual TInt SetPayloadType( TUint8 aPayloadType );
	    
        /**
	    * Clones the codec.
	    * @return cloned codec; ownership is transferred
	    */
	    virtual CMceVideoCodec* CloneL() const;
	    
	    /**
	    * Sets maximum bitrate of the codec.
	    * @param aMaxBitrate value of maximum bitrate used for encoding
	    * @leave KErrNotSupported if codec doesn't support frame sizes issued
	    */
	    virtual void SetMaxBitrateL( TUint aMaxBitrate );
        
        /**
	    * Sets resolutions allowed with the codec.
	    * @param aResolutions bitfield of allowed resolutions
	    * @leave KErrNotSupported if codec doesn't support resolutions issued
	    */
        virtual void SetAllowedResolutionsL( TUint aResolutions );
        
        /**
	    * Sets resolution of the video.
	    * @param aResolution resolution of the video in pixels
	    * @leave KErrNotSupported if codec doesn't support resolution issued
	    */
        virtual void SetResolutionL( TSize aResolution );
        
        /**
	    * Sets frame rates allowed with the codec.
	    * @param aFrameRates allowed frame rate values as a bitfield
	    * @leave KErrNotSupported if codec doesn't support frame rates issued
	    */
        virtual void SetAllowedFrameRatesL( TUint aFrameRates );
        
        /**
	    * Sets frame rate used with the codec for encoding.
	    * @param aFrameRate frame rate for encoding
	    * @leave KErrNotSupported if codec doesn't support frame rate value
	    *        issued
	    */
        virtual void SetFrameRateL( TReal aFrameRate );
        
        /**
        * Sets preferred encoding device for uplink stream and preferred decoding
        * device for downlink stream.
        * @param aEncodingDecodingDevice
        * @leave KErrNotSupported if setting preferred device is not possible
        */
        virtual void SetPreferredEncodingDecodingDeviceL( 
                                    TUid aEncodingDecodingDevice );
        
        /**
        * Set config key (sprop-parameter-sets)
        * @param aConfigKey, config key as Base64 encoded
        * @leave KErrNotSupported if setting config key is not possible
        */
        virtual void SetConfigKeyL( const TDesC8& aConfigKey );

    private: // Helper functions
	    
	    /**
	    * Sets the sdp name.
	    * @param aSdpName the sdp name
	    */
	    void SetSdpNameL( const TDesC8& aSdpName );
	    
	private: // Constructors

	    /**
	    * C++ default constructor.
	    */
	    CMceAvcCodec();

	    /**
	    * By default Symbian 2nd phase constructor is private.
	    */
	    void ConstructL( TBuf8<KMceMaxSdpNameLength> aSdpName); 

    //for testing

	MCE_UNIT_TEST_DEFS	    

    };


#endif      

// End of File
