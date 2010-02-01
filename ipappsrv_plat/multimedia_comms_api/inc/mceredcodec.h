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




#ifndef CMCEREDCODEC_H
#define CMCEREDCODEC_H

// INCLUDES
#include <mceaudiocodec.h>
#include <mcedefs.h>

// CONSTANTS

// Red Default Payload Type
const TUint8 KMceDefaultRedPayloadType = 99; 

// Default Sampling Rate   
const TUint KMceRedSamplingFreq = 8000;

// PTime and MaxPTime
const TUint KMceRedDefaultPtime =  20; 
const TUint KMceRedDefaultMaxPtime = 200;


// CLASS DECLARATION

/**
* Class for the Redundancy codec information.
*
* It checks the validity of inserted values for Red.
* 
*  @l
 mceclient.lib
*/
class CMceRedCodec : public CMceAudioCodec
    {
	public:  // Constructors and destructor
	    
	    /**
	    * Destructor.
	    */
	    IMPORT_C ~CMceRedCodec();
	        
	public: // From base class

	    /**
	    * Enable / Disable Voice Activity Detection.
	    * @param aEnableVAD, Boolean indicating whether to enable Voice Activity Detection
	    * @return KErrNotSupported if codec doesn't support VAD; otherwise KErrNone.
	    */
	    IMPORT_C TInt EnableVAD(TBool aEnableVAD);
        
        /**
	    * Sets bitrate used with codec for encoding.
	    * @param aBitrate, bitrate value for encoding
	    * @return KErrNotSupported if codec doesn't support bitrate value issued; otherwise KErrNone.
	    */
	    IMPORT_C TInt SetBitrate(TUint aBitrate);
	    
	    /**
	    * Sets bitrates allowed with codec.
	    * @param aBitrates, allowed bitrate values 
	    * @return KErrNotSupported if codec doesn't support bitrate value issued; otherwise KErrNone.
	    */
	    IMPORT_C TInt SetAllowedBitrates(TUint aBitrates);
	        
	    /**
	    * Sets the sampling frequency.
	    * @param aSamplingFreq, Sampling frequency to be used
	    * @return KErrNotSupported if codec doesn't support sampling frequency issued; otherwise KErrNone.
	    */
	    IMPORT_C TInt SetSamplingFreq(TUint aSamplingFreq);

	    /**
	    * Sets the packetization rate. 
	    * @param aPTime, Packetization rate, must be multiple of audio frame size.
	    * @return KErrNotSupported if codec doesn't support packetrate value issued; otherwise KErrNone.
	    */
	    IMPORT_C TInt SetPTime(TUint aPTime);
	    
	    /**
	    * Sets the maximum packetization rate.
	    * @param aMaxPTime, maximum allowed packetization rate, must be multiple of audio frame size.
	    * @return KErrNotSupported if codec doesn't support packetrate value issued; otherwise KErrNone.
	    */
	    IMPORT_C TInt SetMaxPTime(TUint aMaxPTime);
	    
	    /**
	    * Sets the payload type identifier.
	    * @param aPayloadType payload type identifier.
	    * @return One of the standard system-wide error codes.
	    */
	    IMPORT_C TInt SetPayloadType(TUint8 aPayloadType);

	    /**
	    * Sets the codec mode.
	    * @param aCodecMode	Codec mode
	    * @return KErrNotSupported if codec doesnt' support codec mode value issued; otherwise KErrNone.
	    */
	    IMPORT_C TInt SetCodecMode(TUint aCodecMode);

	    /**
	    * Clones codec, ownership is transferred.
	    * @return cloned codec
	    */
	    IMPORT_C CMceAudioCodec* CloneL() const;
	    
	    /**
	    * Set redundancy payload types.
	    * @param aRedPaylodTypes, array of redundancy payload types
	    */
	    IMPORT_C void SetRedPayloadTypesL( const RArray<TUint>& aRedPaylodTypes );
	    
	    /**
	    * Get redundancy payload types.
	    * @param aRedPaylodTypes, on return contains current redundancy payload types
	    */
	    IMPORT_C void RedPayloadTypesL( RArray<TUint>& aRedPayloadTypes );

	public: 

	    /**
	    * Two-phased constructor.
	    * @param aSdpName sdp name
	    */
	    static CMceRedCodec* NewL( TBuf8<KMceMaxSdpNameLength> aSdpName);

	    /**
	    * Two-phased constructor.
	    * @param aSdpName sdp name
	    */
	    static CMceRedCodec* NewLC(TBuf8<KMceMaxSdpNameLength> aSdpName);

	    	    
	private:

	    /**
	    * C++ default constructor.
	    */
	    CMceRedCodec();

	    /**
	    * By default Symbian 2nd phase constructor is private.
	    */
	    void ConstructL( TBuf8<KMceMaxSdpNameLength> aSdpName);

	    /**
	    * Sets the sdp name.
	    * @param aSdpName the sdp name
	    */
	    void SetSdpNameL( const TDesC8& aSdpName );

		//for testing

	    MCE_UNIT_TEST_DEFS	    
    };


#endif      

// End of File

