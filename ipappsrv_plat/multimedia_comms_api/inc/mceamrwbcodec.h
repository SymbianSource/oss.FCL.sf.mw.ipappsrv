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




#ifndef CMCEAMRWBCODEC_H
#define CMCEAMRWBCODEC_H

// INCLUDES
#include <mceamrcodec.h>
#include <mcedefs.h>


// CONSTANTS

// AMR-WB Codec possible bitrates
const TInt KMceAmrWbBitrate660( 6600 );
const TInt KMceAmrWbBitrate885( 8850 );
const TInt KMceAmrWbBitrate1265( 12650 );
const TInt KMceAmrWbBitrate1425( 14250 );
const TInt KMceAmrWbBitrate1585( 15850 );
const TInt KMceAmrWbBitrate1825( 18250 );
const TInt KMceAmrWbBitrate1985( 19850 );
const TInt KMceAmrWbBitrate2305( 23050 );
const TInt KMceAmrWbBitrate2385( 23850 );

const TUint KMceAllowedAmrWbBitrate660     = 0x0001;
const TUint KMceAllowedAmrWbBitrate885     = 0x0002;
const TUint KMceAllowedAmrWbBitrate1265    = 0x0004;
const TUint KMceAllowedAmrWbBitrate1425    = 0x0008;
const TUint KMceAllowedAmrWbBitrate1585    = 0x0010;
const TUint KMceAllowedAmrWbBitrate1825    = 0x0020;
const TUint KMceAllowedAmrWbBitrate1985    = 0x0040;
const TUint KMceAllowedAmrWbBitrate2305    = 0x0080;
const TUint KMceAllowedAmrWbBitrate2385    = 0x0100;
const TUint KMceAllowedAmrWbBitrateAll     = 0x0FFF;

// Allowed bitrates bitfield can contain additional information
const TUint KMceAllowedAmrWbAdditionalInfo      = 0xF000;

// If this bit is not present, mode-change-period=1
const TUint KMceAllowedAmrWbModeChangePeriod2   = 0x1000;

// If this bit is not present, mode-change-neighbor=0
const TUint KMceAllowedAmrWbModeChangeNeighbor1 = 0x2000;


// CLASS DECLARATION

/**
* Class for the AMR WB codec information.
*
* It checks the validity of inserted values for AMR WB.
* 
*  @lib mceclient.lib
*/
class CMceAmrWbCodec : public CMceAmrCodec
    {
	public:  // Constructors and destructor
	    
	    /**
	    * Destructor.
	    */
	    IMPORT_C ~CMceAmrWbCodec();
	        
	public: // From CMceAmrCodec

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
	    static CMceAmrWbCodec* NewL( TBuf8<KMceMaxSdpNameLength> aSdpName );

	    /**
	    * Two-phased constructor.
	    * @param aSdpName sdp name
	    */
	    static CMceAmrWbCodec* NewLC(TBuf8<KMceMaxSdpNameLength> aSdpName );

	private:

	    /**
	    * C++ default constructor.
	    */
	    CMceAmrWbCodec();

	    /**
	    * By default Symbian 2nd phase constructor is private.
	    */
	    void ConstructL( TBuf8<KMceMaxSdpNameLength> aSdpName );


		// For testing
	    MCE_UNIT_TEST_DEFS
    };


#endif // CMCEAMRWBCODEC_H

// End of File
