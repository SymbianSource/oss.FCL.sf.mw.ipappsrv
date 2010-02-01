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




#ifndef CMCECOMAMRWBCODEC_H
#define CMCECOMAMRWBCODEC_H

//  INCLUDES
#include "mcecomaudiocodec.h"
#include "mcecomamrcodec.h"
#include "mcedefs.h"


// CLASS DECLARATION
/**
*  Container class for the codec information.
*
*  @lib 
*/
class CMceComAMRWbCodec : public CMceComAMRCodec

    {
public:  // Constructors and destructor

    /**
    * Two-phased constructor.
    * @param aSdpName sdp name
    */
    static CMceComAMRWbCodec* NewL( TBuf8<KMceMaxSdpNameLength> aSdpName );
    
    /**
    * Two-phased constructor.
    * @param aSdpName sdp name
    */
    static CMceComAMRWbCodec* NewLC( TBuf8<KMceMaxSdpNameLength> aSdpName );
    
    /**
    * Destructor.
    */
    ~CMceComAMRWbCodec();

public: // from CMceComCodec

   /**
    * Sets bitrate used with codec for encoding.
    * @param aBitrate bitrate value for encoding
    * @return KErrNotSupported if codec doesn't support bitrate
    *         value issued
    */
    TInt SetBitrate( TUint aBitrate );
    
    /**
    * Sets bitrates allowed with codec.
    * @param aBitrates allowed bitrate values
    * @return KErrNotSupported if codec doesn't support bitrate
    *         values issued
    */
    TInt SetAllowedBitrates( TUint aBitrates );
   
    /**
    * Returns max bit rate
    * @return max bit rate
    */
    TUint GetMaxBitRate();
    
    /**
    * Resolve allowed bitrates based on given bitrate value.
    * @param aBitrate
    * @param aAllowedBitrates, on return contains allowed bitrates
    * @return error code
    */
    TInt ResolveAllowedBitrates( TUint aBitrate, TUint& aAllowedBitrates );


#ifdef MCE_COMMON_SERVER_SIDE

protected: // from CMceComCodec

    /**
    * Validates codec values based on mcc codec
    * @param aMccCodec mcc codec
    * @param aStream stream
    * @param aRole role
    */
    CMceComCodec* DoMccValidateL( CMccCodecInformation& aMccCodec,
                                  CMceSrvStream& aStream,
                                  TMceNegotiationRole aRole );

    /**
    * Set default fmtp line
    */                                    
    void DoSetDefaultFmtpAttributeL();

#endif

public: // serialization etc

    /**
    * Clones
    */
    CMceComAudioCodec* CloneL();

private:

    /**
    * C++ default constructor.
    */
    CMceComAMRWbCodec();
    
    #ifdef EUNIT_TEST
    friend class UT_CMceComAMRWbCodec;
    #endif
    
    };
    


#endif      // CMCECOMAMRWBCODEC_H

// End of File
