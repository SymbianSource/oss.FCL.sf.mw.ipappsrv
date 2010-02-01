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




#ifndef CMCEILBCCODEC_H
#define CMCEILBCCODEC_H

// INCLUDES
#include <mceaudiocodec.h>
#include <mcedefs.h>

// CONSTANTS

// iLBC Codec bitrate
const TUint KMceiLBCLevel20Bitrate = 15200;
const TUint KMceiLBCLevel30Bitrate = 13330;

const TUint KMceAllowediLBCBitrate20 = 0x0001;
const TUint KMceAllowediLBCBitrate30 = 0x0002;
const TUint KMceAllowediLBCBitrateAll = 0x00FF;

// iLBC Default Payload Tyep
const TUint8 KMceDefaultiLBCPayloadType = 97; 

// iLBC Codec modes
const TUint KiLBCodecMode = 1; 

// Default Sampling Rate   
const TUint KMceiLBCSamplingFreq = 8000;

// PTime and MaxPTime
const TUint KMceiLBCDefaultPtime =  30; 
const TUint KMceiLBCDefaultMaxPtime = 180;

const TUint KMceiLBCDefaultPtime20 =  20;
const TUint KMceiLBCDefaultPtime30 =  30;


// CLASS DECLARATION

/**
* Class for the iLBC codec information.
*
* It checks the validity of inserted values for iLBC.
* 
*  @l
 mceclient.lib
*/
class CMceiLBCCodec : public CMceAudioCodec
    {
	public:  // Constructors and destructor
	    
	    /**
	    * Destructor.
	    */
	    IMPORT_C ~CMceiLBCCodec();
	        
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
	    * @param aCodecMode	Codec mode (PCMU / PCMA )
	    * @return KErrNotSupported if codec doesnt' support codec mode value issued; otherwise KErrNone.
	    */
	    IMPORT_C TInt SetCodecMode(TUint aCodecMode);
	    
	    /**
	    * Enable / Disable Comfort noise codec existence.
	    * @param aComfortNoise    Boolean indicating whether to enable CN codec existence
	    * @return KErrNotSupported if codec doesnt' support codec mode value issued; otherwise KErrNone.
	    */
	    //IMPORT_C TInt EnableCN(const TBool aComfortNoise);
	    
	    /**
	    * Clones codec, ownership is transferred.
	    * @return cloned codec
	    */
	    IMPORT_C CMceAudioCodec* CloneL() const;

	public: 

	    /**
	    * Two-phased constructor.
	    * @param aSdpName sdp name
	    */
	    static CMceiLBCCodec* NewL( TBuf8<KMceMaxSdpNameLength> aSdpName);

	    /**
	    * Two-phased constructor.
	    * @param aSdpName sdp name
	    */
	    static CMceiLBCCodec* NewLC(TBuf8<KMceMaxSdpNameLength> aSdpName);

	    	    
	private:

	    /**
	    * C++ default constructor.
	    */
	    CMceiLBCCodec();

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

