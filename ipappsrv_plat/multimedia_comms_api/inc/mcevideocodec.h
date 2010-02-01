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




#ifndef MCEVIDEOCODEC_H
#define MCEVIDEOCODEC_H

// INCLUDES
#include <e32base.h>
#include <mcecodec.h>
#include <mcedefs.h>

// FORWARD DECLARATIONS
class CMceComVideoCodec;
class TMceVideoCodecFactory;

// CONSTANTS
const TMceCodecType KMceVideoCodec = 2;


// CLASS DECLARATION

/**
* Base class for video codecs.
*
* It defines setters and getters for generic video codec attributes.
* These attributes are used in single video stream and they
* are negotiated with remote terminal.
*
* @lib mceclient.lib
*/
class CMceVideoCodec : public CMceCodec
    {
	public:  // Constructors and destructor
	        
	    /**
	    * Destructor.
	    */
	    virtual ~CMceVideoCodec();
	    	 
		
    public: // New functions
        
        /**
	    * Clones the codec.
	    * @return cloned codec; ownership is transferred
	    */
	    virtual CMceVideoCodec* CloneL() const = 0;
	    
	    /**
	    * Sets maximum bitrate of the codec.
	    * @param aMaxBitrate value of maximum bitrate used for encoding
	    * @leave KErrNotSupported if codec doesn't support frame sizes issued
	    */
	    virtual void SetMaxBitrateL( TUint aMaxBitrate ) = 0;
        
        /**
	    * Sets resolutions allowed with the codec.
	    * @param aResolutions bitfield of allowed resolutions
	    * @leave KErrNotSupported if codec doesn't support resolutions issued
	    */
        virtual void SetAllowedResolutionsL( TUint aResolutions ) = 0;
        
        /**
	    * Sets resolution of the video.
	    * @param aResolution resolution of the video in pixels
	    * @leave KErrNotSupported if codec doesn't support resolution issued
	    */
        virtual void SetResolutionL( TSize aResolution ) = 0;
        
        /**
	    * Sets frame rates allowed with the codec.
	    * @param aFrameRates allowed frame rate values as a bitfield
	    * @leave KErrNotSupported if codec doesn't support frame rates issued
	    */
        virtual void SetAllowedFrameRatesL( TUint aFrameRates ) = 0;
        
        /**
	    * Sets frame rate used with the codec for encoding.
	    * @param aFrameRate frame rate for encoding
	    * @leave KErrNotSupported if codec doesn't support frame rate value
	    *        issued
	    */
        virtual void SetFrameRateL( TReal aFrameRate ) = 0;
        
        /**
        * Sets preferred encoding device for uplink stream and preferred decoding
        * device for downlink stream.
        * @param aEncodingDecodingDevice
        * @leave KErrNotSupported if setting preferred device is not possible
        */
        virtual void SetPreferredEncodingDecodingDeviceL( 
                                    TUid aEncodingDecodingDevice ) = 0;
        
        /**
        * Set config key (sprop-parameter-sets)
        * @param aConfigKey, config key as Base64 encoded
        * @leave KErrNotSupported if setting config key is not possible
        */
        virtual void SetConfigKeyL( const TDesC8& aConfigKey ) = 0;
        		
	public: // Getters
	    
	    /**
	    * Gets maximum bit rate currently allowed
	    * @return maximum bit rate
	    */
	    IMPORT_C TUint MaxBitRate() const;

        /**
	    * Gets currently allowed resolutions as a bitfield
	    * @return currently allowed resolutions as a bitfield
	    */	
	    IMPORT_C TUint AllowedResolutions() const;

        /**
	    * Gets current resolution
	    * @return current resolution
	    */	    
	    IMPORT_C TSize Resolution() const;
	    
	    /**
	    * Gets frame rates currently allowed for encoding and decoding.
	    * @return allowed frame rate values as a bitfield
	    */
	    IMPORT_C TUint AllowedFrameRates() const;
	    
	    /**
	    * Gets frame rate currently used for encoding and decoding.
	    * @return current frame rate value
	    */
	    IMPORT_C TReal FrameRate() const;
	          
        /**
        * Get config key (sprop-parameter-sets)
        * @return config key as Base64 encoded or NULL, ownership
        *   is transferred.
        */
        IMPORT_C HBufC8* ConfigKeyL() const;

	    /**
	    * Gets video codec factory.
	    * @return factory
	    */
	    static TMceVideoCodecFactory Factory();
	    
	    static TInt ComparePreferences( const CMceVideoCodec& aIndex1, 
                                        const CMceVideoCodec& aIndex2 );

	                
	protected:  // New functions

	    /**
	    * C++ default constructor.
	    */
	    CMceVideoCodec();

	    /**
	    * Second-phase constructor
	    * @paramaFlatData flat data container
	    */
		void ConstructL( CMceComVideoCodec* aFlatData );
		
		
    private: // Reserved for future use
    
        TAny* iReserved1;       

	//for testing

    MCE_UNIT_TEST_DEFS	
    
    };


#endif

// End of File
