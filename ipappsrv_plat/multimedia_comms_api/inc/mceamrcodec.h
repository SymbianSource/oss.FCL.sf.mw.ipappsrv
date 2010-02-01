/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef CMCEAMRCODEC_H
#define CMCEAMRCODEC_H

// INCLUDES
#include <mceaudiocodec.h>
#include <mcedefs.h>

// CONSTANTS
const TInt KMceAmrNbBitrate475 = 4750;
const TInt KMceAmrNbBitrate515 = 5150;
const TInt KMceAmrNbBitrate590 = 5900;
const TInt KMceAmrNbBitrate670 = 6700;
const TInt KMceAmrNbBitrate740 = 7400;
const TInt KMceAmrNbBitrate795 = 7950;
const TInt KMceAmrNbBitrate102 = 10200;
const TInt KMceAmrNbBitrate122 = 12200;

const TUint KMceAllowedAmrNbBitrate475 = 0x0001;
const TUint KMceAllowedAmrNbBitrate515 = 0x0002;
const TUint KMceAllowedAmrNbBitrate590 = 0x0004;
const TUint KMceAllowedAmrNbBitrate670 = 0x0008;
const TUint KMceAllowedAmrNbBitrate740 = 0x0010;
const TUint KMceAllowedAmrNbBitrate795 = 0x0020;
const TUint KMceAllowedAmrNbBitrate102 = 0x0040;
const TUint KMceAllowedAmrNbBitrate122 = 0x0080;
const TUint KMceAllowedAmrNbBitrateAll = 0x00FF;

// Allowed bitrates bitfield can contain additional information
const TUint KMceAllowedAmrNbAdditionalInfo = 0xF00;

// If this bit is not present, mode-change-period=1
const TUint KMceAllowedAmrNbChangePeriod2 = 0x0100;

// If this bit is not present, mode-change-neighbor=0
const TUint KMceAllowedAmrNbChangeNeighbor1 = 0x0200;


const TUint KMceAmrNbSamplingFreq = 8000;
const TUint KMceAmrWbSamplingFreq = 16000;

const TUint KMceDefaultMaxPtime = 400;
const TUint KMceDefaultPtime = 20;


// DATA TYPES
enum TAmrCodecMode
    {
    EMceBandwidthEfficient = 4,
    EMceOctetAligned
    };

// CLASS DECLARATION

/**
* Class for the AMR codec information.
*
* It checks the validity of inserted values for AMR.
* 
*  @lib mceclient.lib
*/
class CMceAmrCodec : public CMceAudioCodec
    {
	public:  // Constructors and destructor
	    
	    /**
	    * Destructor.
	    */
	    IMPORT_C ~CMceAmrCodec();
	        
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
	    * @param aBitrates, allowed bitrate values (like KMceAmrNbBitrate590|KMceAmrNbBitrate670)
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
	    * @param aCodecMode	Codec mode ( Bandwidth efficient / octet aligned )
	    * @return KErrNotSupported if codec doesnt' support codec mode value issued; otherwise KErrNone.
	    */
	    IMPORT_C TInt SetCodecMode(TUint aCodecMode);
	    
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
	    static CMceAmrCodec* NewL( TBuf8<KMceMaxSdpNameLength> aSdpName);

	    /**
	    * Two-phased constructor.
	    * @param aSdpName sdp name
	    */
	    static CMceAmrCodec* NewLC(TBuf8<KMceMaxSdpNameLength> aSdpName);

	    	    
	protected:	

	    /**
	    * C++ default constructor.
	    */
	    CMceAmrCodec();

		/**
	    * Sets the sdp name.
	    * @param aSdpName the sdp name
	    */
	    void SetSdpNameL( const TDesC8& aSdpName );

	private:

	    /**
	    * By default Symbian 2nd phase constructor is private.
	    */
	    void ConstructL( TBuf8<KMceMaxSdpNameLength> aSdpName); 



		//for testing

	    MCE_UNIT_TEST_DEFS	    
    };

#endif      

// End of File
