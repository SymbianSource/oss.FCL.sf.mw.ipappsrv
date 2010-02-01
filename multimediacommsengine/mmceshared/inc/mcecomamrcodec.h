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




#ifndef CMCECOMAMRCODEC_H
#define CMCECOMAMRCODEC_H

//  INCLUDES
#include "mcecomaudiocodec.h"
#include "mcedefs.h"



// CLASS DECLARATION
/**
*  Container class for the codec information.
*
*  @lib 
*/
class CMceComAMRCodec : public CMceComAudioCodec
    {
public:  // Constructors and destructor

    /**
    * Two-phased constructor.
    * @param aSdpName sdp name
    */
    static CMceComAMRCodec* NewL( TBuf8<KMceMaxSdpNameLength> aSdpName );
    
    /**
    * Two-phased constructor.
    * @param aSdpName sdp name
    */
    static CMceComAMRCodec* NewLC( TBuf8<KMceMaxSdpNameLength> aSdpName );
    
    /**
    * Destructor.
    */
    ~CMceComAMRCodec();

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
    * Sets the codec specific mode.
    * @param aCodecMode mode of the codec
    * @return KErrNotSupported if codec doesnt' support codec mode
    *         value issued
    */
    TInt SetCodecMode( TUint aCodecMode );
    
    /**
    * Sets the payload type.
    * @param aPayloadType type identifier of the payload
    * @return KErrNotSupported if codec doesn't support payload type issued
    */
    TInt SetPayloadType( TUint8 aPayloadType );
    
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

public: // from CMceAudioCodec
    
    /**
    * Sets the sampling frequency.
    * @param aSamplingFreq, Sampling frequency to be used
    * @return KErrNotSupported if codec doesn't support sampling frequency issued;
    * otherwise KErrNone.
    */
    TInt SetSamplingFreq( TUint aSamplingFreq );

    /**
    * Sets the packetization rate. 
    * @param aPTime, Packetization rate, must be multiple of audio frame size.
    * @return KErrNotSupported if codec doesn't support packetrate value issued; otherwise KErrNone.
    */
    TInt SetPTime( TUint aPTime );
    
    /**
    * Sets the maximum packetization rate.
    * @param aMaxPTime, maximum allowed packetization rate, must be multiple of audio frame size.
    * @return KErrNotSupported if codec doesn't support packetrate value issued; otherwise KErrNone.
    */
    TInt SetMaxPTime( TUint aMaxPTime );

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

protected:

    /**
    * C++ default constructor.
    */
    CMceComAMRCodec();

    /**
    * By default Symbian 2nd phase constructor is private.
    */
    void ConstructL( TBuf8<KMceMaxSdpNameLength> aSdpName);

    /**
     * second-phase copy constructor
     */
    void ConstructL( CMceComAMRCodec& aCodec );
    
    #ifdef EUNIT_TEST
    friend class UT_CMceComAMRCodec;
    #endif
    };


#endif      // __MCP_AMR_CODEC_H__

// End of File