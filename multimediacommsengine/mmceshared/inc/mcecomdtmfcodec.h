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




#ifndef CMCECOMDTMFCODEC_H
#define CMCECOMDTMFCODEC_H

//  INCLUDES
#include "mcecomaudiocodec.h"
#include "mcedefs.h"



// CLASS DECLARATION
/**
*  Container class for the codec information.
*
*  @lib 
*/
class CMceComDtmfCodec : public CMceComAudioCodec
    {
public:  // Constructors and destructor

    /**
    * Two-phased constructor.
    * @param aSdpName sdp name
    */
    static CMceComDtmfCodec* NewL( TBuf8<KMceMaxSdpNameLength> aSdpName );
    
    /**
    * Two-phased constructor.
    * @param aSdpName sdp name
    */
    static CMceComDtmfCodec* NewLC( TBuf8<KMceMaxSdpNameLength> aSdpName );
    
    /**
    * Destructor.
    */
    ~CMceComDtmfCodec();

public: // from CMceComCodec

    /**
    * Sets bitrate used with codec for encoding.
    * @param aBitrate bitrate value for encoding
    * @return KErrNotSupported if codec doesn't support bitrate
    *         value issued
    */
    TInt SetBitrate( TUint aBitrate );
    
    /**
    * Set Allowed Bitrate
    * @return 
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
    * Sets codec enabled/disabled.
    * @param aIsEnabled
    */
    void SetEnabled( TBool aIsEnabled );
    
    /**
    * Check if codec can send.
    * @return ETrue if can send
    */
    TBool SendSupported();

#ifdef MCE_COMMON_SERVER_SIDE
    

    /**
    * Creates server stream(s) for this codec
    * @param aCodecIndex
    * @param aStreams stream is added to the array
    * @param aAudio
    * @param aManager
    */
    void DoDecodeAudioL( TInt aCodecIndex,
                         RPointerArray<CMceSrvStream>& aStreams,
                         CMceComAudioStream& aAudio,
                         CMceMediaManager& aManager );
    
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
    CMceComDtmfCodec();

    /**
    * By default Symbian 2nd phase constructor is private.
    */
    void ConstructL( TBuf8<KMceMaxSdpNameLength> aSdpName);

    /**
     * second-phase copy constructor
     */
    void ConstructL( CMceComDtmfCodec& aCodec );
    
            
    };


#endif      // CMCECOMDTMFCODEC_H

// End of File
