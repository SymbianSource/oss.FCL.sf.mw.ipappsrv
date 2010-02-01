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




#ifndef MCEH263CODEC_H
#define MCEH263CODEC_H

// INCLUDES
#include <mcevideocodec.h>
#include <mcedefs.h>

// CONSTANTS

const TUint KMceH263FrameHeight = 176;
const TUint KMceH263FrameWidth = 144;
const TUint KMceH263ClockRate = 90000;
const TReal KMceH263FrameRate = 15;

const TInt  KMceH263Level10Bitrate = 64000;
const TInt  KMceH263Level45Bitrate = 128000;

const TUint KMceAllowedH263Bitrate10 = 0x0001;
const TUint KMceAllowedH263Bitrate45 = 0x0002;
const TUint KMceAllowedH263BitrateAll = 0x00FF;


// CLASS DECLARATION

/**
* Class for the H263 codec information.
*
* It checks the validity of inserted values for H263
* 
*  @lib mceclient.lib
*/
class CMceH263Codec : public CMceVideoCodec
    {
    
	public:  // Constructors and destructor
	    
	    /**
	    * Destructor.
	    */
	    IMPORT_C ~CMceH263Codec();

	public: 

	    /**
	    * Two-phased constructor.
	    * @param aSdpName sdp name
	    */
	    IMPORT_C static CMceH263Codec* NewL( TBuf8<KMceMaxSdpNameLength> aSdpName);

	    /**
	    * Two-phased constructor.
	    * @param aSdpName sdp name
	    */
	    IMPORT_C static CMceH263Codec* NewLC( TBuf8<KMceMaxSdpNameLength> aSdpName );
	        
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
	    CMceH263Codec();

	    /**
	    * By default Symbian 2nd phase constructor is private.
	    */
	    void ConstructL( TBuf8<KMceMaxSdpNameLength> aSdpName); 

    //for testing

	MCE_UNIT_TEST_DEFS	    

    };


#endif      

// End of File
