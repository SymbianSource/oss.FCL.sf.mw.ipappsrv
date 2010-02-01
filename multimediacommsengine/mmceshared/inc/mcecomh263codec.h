/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef CMCECOMH263CODEC_H
#define CMCECOMH263CODEC_H

//  INCLUDES
#include <mceh263codec.h>
#include "mcecomvideocodec.h"
#include "mcedefs.h"

const TInt  KMceH263Level10 = 10;
const TReal KMceH263Level10Framerate = 15;
const TInt  KMceH263Level45 = 45;
const TReal KMceH263Level45Framerate = 15;
const TInt  KMceH263DefaultFrameSize = 3000;
const TInt  KMceH263DefaultAllowedBitrate = KMceAllowedH263BitrateAll;
const TInt  KMceH263DefaultMaxBitrate = KMceH263Level10Bitrate;
const TInt  KMceH263DefaultBitrate = KMceH263Level10Bitrate;


// CLASS DECLARATION
/**
*  Container class for the codec information.
*
*  @lib 
*/
class CMceComH263Codec : public CMceComVideoCodec
    {
public:  // Constructors and destructor

    /**
    * Two-phased constructor.
    * @param aSdpName sdp name
    */
    static CMceComH263Codec* NewL( TBuf8<KMceMaxSdpNameLength> aSdpName );
    
    /**
    * Two-phased constructor.
    * @param aSdpName sdp name
    */
    static CMceComH263Codec* NewLC( TBuf8<KMceMaxSdpNameLength> aSdpName );
    
    /**
    * Destructor.
    */
    ~CMceComH263Codec();

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
    * Sets maximum bitrate of the codec.
    * @param aMaxBitrate value of maximum bitrate used for encoding
    * @return KErrNotSupported if codec doesn't support bitrate
    *         values issued
    */
    TInt SetMaxBitrate( TUint aMaxBitrate );
    
    /**
    * Resolve allowed bitrates based on given bitrate value.
    * @param aBitrate
    * @param aAllowedBitrates, on return contains allowed bitrates
    * @return error code
    */
    TInt ResolveAllowedBitrates( TUint aBitrate, TUint& aAllowedBitrates );
    
public: // serialization etc

    /**
    * Clones
    * @return clone
    */
    CMceComVideoCodec* CloneL();

private:

    /**
    * C++ default constructor.
    */
    CMceComH263Codec();

    /**
    * By default Symbian 2nd phase constructor is private.
    */
    void ConstructL( TBuf8<KMceMaxSdpNameLength> aSdpName);

    /**
     * second-phase copy constructor
     */
    void ConstructL( CMceComH263Codec& aCodec );

            
    };


#endif      // CMCECOMH263CODEC_H

// End of File