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




#ifndef CMCECOMG711CODEC_H
#define CMCECOMG711CODEC_H

//  INCLUDES
#include "mcecomaudiocodec.h"
#include "mcedefs.h"



// CLASS DECLARATION
/**
*  Container class for the codec information.
*
*  @lib 
*/
class CMceComG711Codec : public CMceComAudioCodec
    {
public:  // Constructors and destructor

    /**
    * Two-phased constructor.
    * @param aSdpName sdp name
    */
    static CMceComG711Codec* NewL( TBuf8<KMceMaxSdpNameLength> aSdpName );
    
    /**
    * Two-phased constructor.
    * @param aSdpName sdp name
    */
    static CMceComG711Codec* NewLC( TBuf8<KMceMaxSdpNameLength> aSdpName );
    
    /**
    * Destructor.
    */
    ~CMceComG711Codec();

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
    * Returns max bit rate
    * @return max bit rate
    */
    TUint GetMaxBitRate();
    
public: // from CMceAudioCodec
    
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

    
public: // serialization etc

    /**
    * Clones
    */
    CMceComAudioCodec* CloneL();
    

private:

    /**
    * C++ default constructor.
    */
    CMceComG711Codec();

    /**
    * By default Symbian 2nd phase constructor is private.
    */
    void ConstructL( TBuf8<KMceMaxSdpNameLength> aSdpName);

    /**
     * second-phase copy constructor
     */
    void ConstructL( CMceComG711Codec& aCodec );
    
            
    };


#endif      // __MCP_G711_CODEC_H__

// End of File