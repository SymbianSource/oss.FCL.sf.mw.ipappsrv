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




#ifndef CMCECOMMESSAGECODEC_H
#define CMCECOMMESSAGECODEC_H

//  INCLUDES
#include "mcecomcodec.h"
#include "mcecommessagestream.h"
    
    
// FUNCTION PROTOTYPES

// CLASS DECLARATION
/**
*  Container class for the codec information.
*
*  @lib 
*  @since
*/
class CMceComMessageCodec : public CMceComCodec
    {
		
public:  // Constructors and destructor

    /**
    * Two-phased constructor.
    * @param aSdpName sdp name
    */
    static CMceComMessageCodec* NewL( TBuf8<KMceMaxSdpNameLength> aSdpName );
    
    /**
    * Two-phased constructor.
    * @param aSdpName sdp name
    */
    static CMceComMessageCodec* NewLC( TBuf8<KMceMaxSdpNameLength> aSdpName );
    
    /**
    * Destructor.
    */
    ~CMceComMessageCodec();


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
    
public: //new functions 

    /**
    * Clones
    */
    virtual CMceComMessageCodec* CloneL();

    /**
    * Factory
    * @return factory
    */
    static TMceComMessageCodecFactory Factory();
     
    static TInt CompareSdpIndex( const CMceComMessageCodec& aIndex1, 
                                 const CMceComMessageCodec& aIndex2 );
                                                                      
#ifdef MCE_COMMON_SERVER_SIDE

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
    
    
    /**
    * Checks, if signalling is required - MSRP capabilities
    * @param aStream stream
    * @param aMccCurentCodec current mcc codec
    * @param aMccUpdateCodec to be used mcc codec
    */
    TInt DoMccRequireSignalling( 
                                const CMceSrvStream& aOldStream,
                                const CMceSrvStream& aCurrentStream,
                                const CMccCodecInformation& aMccCurentCodec, 
                                const CMccCodecInformation& aMccUpdateCodec ) const;
    
    /**
    * Populates mcc codec
    * @param aMccCodec mcc codec
    * @param aStream stream
    * @param aRole role
    */
    void MccPopulateL( CMccCodecInformation& aMccCodec, 
                       CMceSrvStream& aStream,
                       TMceNegotiationRole aRole );
    
    /**
    * Updates codec based on mcc codec
    * @param aMccCodec mcc codec
    */
    void UpdateL( CMccCodecInformation& aMccCodec );

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
    CMceComMessageCodec();
    
     /**
     * second-phase copy constructor
     */
	void ConstructL( CMceComMessageCodec& aCodec );
    
    
    /**
     * second-phase copy constructor
     */
	void ConstructL( TBuf8<KMceMaxSdpNameLength> aSdpName );
    
        
        
public:  // Owned serialized Data


    
    /**
    * Enables or disables VAD
    */
    TBool iEnableVAD;

	/**
    * Sampling frequency used with codec
    */
	TUint iSamplingFreq;

	/**
    * Stream's packet time
    */
	TUint iPTime;
	
	/**
    * Stream's max packet time
    */
	TUint iMaxPTime;
	
	
	TInt iRedCount;
	
	TUint iRedPayloadType;
	
    };



#endif

// End of File
