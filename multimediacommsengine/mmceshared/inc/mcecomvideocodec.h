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




#ifndef CMCECOMVIDEOCODEC_H
#define CMCECOMVIDEOCODEC_H

//  INCLUDES
#include "mcecomcodec.h"
    
    
// FUNCTION PROTOTYPES

// CLASS DECLARATION
/**
*  Container class for the codec information.
*
*  @lib 
*  @since
*/
class CMceComVideoCodec : public CMceComCodec
    {
		
public:  // Constructors and destructor

    /**
    * Two-phased constructor.
    * @param aSdpName sdp name
    */
    static CMceComVideoCodec* NewL( TBuf8<KMceMaxSdpNameLength> aSdpName );
    
    /**
    * Two-phased constructor.
    * @param aSdpName sdp name
    */
    static CMceComVideoCodec* NewLC( TBuf8<KMceMaxSdpNameLength> aSdpName );
    
    /**
    * Destructor.
    */
    ~CMceComVideoCodec();


public: // from CMceComCodec

    /**
    * Internalizes flat data
    * @param aReadStream read stream
    */
    void InternalizeFlatL( RReadStream& aReadStream );

    /**
    * Externalizes flat data
    * @param aWriteStream write stream
    */
    void ExternalizeFlatL( RWriteStream& aWriteStream );

    /**
    * Updates this object
    * @param aUpdate object updates this
    */
    void UpdateL( CMceComCodec& aCodec );

    /**
    * Returns max bit rate
    * @return max bit rate
    */
    TUint GetMaxBitRate();

    static TInt CompareSdpIndex( const CMceComVideoCodec& aIndex1, 
                                 const CMceComVideoCodec& aIndex2 );
                                 
public: //new functions 
    
    /**
    * Clones
    */
    virtual CMceComVideoCodec* CloneL();

    /**
    * Sets maximum bitrate of the codec.
    * @param aMaxBitrate value of maximum bitrate used for encoding
    * @return KErrNotSupported if codec doesn't support bitrate
    *         values issued
    */
    virtual TInt SetMaxBitrate( TUint aMaxBitrate );

    /**
    * Factory
    * @return factory
    */
    static TMceComVideoCodecFactory Factory();
    
    /**
    * Set codec config key
    * @param aConfigKey can be also NULL, ownership is not transferred
    * @return ETrue if key was set, otherwise EFalse (input key not valid or
    *   same key was set already)
    */
    TBool SetConfigKeyL( HBufC8* aConfigKey );
    
    TBool IsConfigKeyValid() const;
    
    static TBool IsConfigKeyValid( HBufC8* aConfigKey );

    
#ifdef MCE_COMMON_SERVER_SIDE

public:     

    /**
    * Assigns mcc codec
    * @param aMccCodec mcc codec
    * @return assigned codecs
    */
    CMceComCodec& operator=( CMccCodecInformation& aMccCodec );

    /**
    * Decodes to mcc codec
    * @param aMccCodec mcc codec
    */
    void DoMccDecodeL( CMccCodecInformation& aMccCodec );

    /**
    * Encodes from mcc codec
    * @param aMccCodec mcc codec
    * @param aStream stream
    */
    void DoMccEncodeL( CMccCodecInformation& aMccCodec,
                       CMceSrvStream& aStream );
    
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
    * Adjust codec values based on mcc codec
    * @param aMccCodec mcc codec
    * @param aStream stream
    */
    void DoMccAdjustL( CMccCodecInformation& aMccCodec, 
                               CMceSrvStream& aStream );
                             
    /**
    * Checks, if signalling is required
    * @param aStream stream
    * @param aMccCurentCodec current mcc codec
    * @param aMccUpdateCodec to be used mcc codec
    */
    TInt DoMccRequireSignalling( 
                                const CMceSrvStream& aStream,
                                const CMccCodecInformation& aMccCurentCodec, 
                                const CMccCodecInformation& aMccUpdateCodec ) const;
    
protected: // from CMceComCodec
    
    /**
    * Set default fmtp line
    */                                    
    void DoSetDefaultFmtpAttributeL(); 
    
#endif//MCE_COMMON_SERVER_SIDE
  
protected:

    /**
    * C++ default constructor.
    */
    CMceComVideoCodec();
    
     /**
     * second-phase copy constructor
     */
	void ConstructL( CMceComVideoCodec& aCodec );
    
    
    /**
     * second-phase copy constructor
     */
	void ConstructL( TBuf8<KMceMaxSdpNameLength> aSdpName );
    
        
        
public:  // Owned serialized Data

    TReal iFrameRate;
    TUint iMaxBitRate;
    TUint iAllowedResolutions;
    TInt iResolutionWidth;
    TInt iResolutionHeight;
    TUint iAllowedFrameRates;
    TUint iClockRate;
    TReal iReceiveFrameRate;
    HBufC8* iConfigKey;
	TUid iEncodingDecodingDevice;
	
    };



#endif // CMCECOMVIDEOCODEC_H

// End of File
