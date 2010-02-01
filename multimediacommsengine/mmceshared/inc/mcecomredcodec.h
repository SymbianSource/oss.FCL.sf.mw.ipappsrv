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




#ifndef MCECOMREDCODEC_H
#define MCECOMREDCODEC_H

//  INCLUDES
#include "mcecomaudiocodec.h"
#include "mcedefs.h"



// CLASS DECLARATION
/**
*  Container class for the codec information.
*
*  @lib 
*/
class CMceComRedCodec : public CMceComAudioCodec
    {
public:  // Constructors and destructor

    /**
    * Two-phased constructor.
    * @param aSdpName sdp name
    */
    static CMceComRedCodec* NewL( TBuf8<KMceMaxSdpNameLength> aSdpName );
    
    /**
    * Two-phased constructor.
    * @param aSdpName sdp name
    */
    static CMceComRedCodec* NewLC( TBuf8<KMceMaxSdpNameLength> aSdpName );
    
    /**
    * Destructor.
    */
    ~CMceComRedCodec();
    
public: // from CMceComCodec

    /**
    * Check if codec can send.
    * @return ETrue if can send
    */
    TBool SendSupported();

    /**
    * Sets the payload type.
    * @param aPayloadType type identifier of the payload
    * @return KErrNotSupported if codec doesn't support payload type issued
    */
    TInt SetPayloadType( TUint8 aPayloadType );
    
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
    CMceComRedCodec();

    /**
    * By default Symbian 2nd phase constructor is private.
    */
    void ConstructL( TBuf8<KMceMaxSdpNameLength> aSdpName);

    /**
     * second-phase copy constructor
     */
    void ConstructL( CMceComRedCodec& aCodec );
    
            
    };


#endif      // MCECOMREDCODEC_H


// End of File
