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




#ifndef CMCECOMAUDIOCODEC_H
#define CMCECOMAUDIOCODEC_H

//  INCLUDES
#include "mcecomcodec.h"
    
#ifdef MCE_COMMON_SERVER_SIDE

class CMceMediaManager;
class CMceComAudioStream;

#endif//MCE_COMMON_SERVER_SIDE

// FUNCTION PROTOTYPES

// CLASS DECLARATION
/**
*  Container class for the codec information.
*
*  @lib 
*  @since
*/
class CMceComAudioCodec : public CMceComCodec
    {
		
public:  // Constructors and destructor

    /**
    * Two-phased constructor.
    * @param aSdpName sdp name
    */
    static CMceComAudioCodec* NewL( TBuf8<KMceMaxSdpNameLength> aSdpName );
    
    /**
    * Two-phased constructor.
    * @param aSdpName sdp name
    */
    static CMceComAudioCodec* NewLC( TBuf8<KMceMaxSdpNameLength> aSdpName );
    
    /**
    * Destructor.
    */
    ~CMceComAudioCodec();


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
    virtual CMceComAudioCodec* CloneL();

    /**
    * Factory
    * @return factory
    */
    static TMceComAudioCodecFactory Factory();
     
    static TInt CompareSdpIndex( const CMceComAudioCodec& aIndex1, 
                                 const CMceComAudioCodec& aIndex2 );
    
    /**
    * Sets the sampling frequency.
    * @param aSamplingFreq, Sampling frequency to be used
    * @return KErrNotSupported if codec doesn't support sampling frequency issued; otherwise KErrNone.
    */
    virtual TInt SetSamplingFreq( TUint aSamplingFreq );
    
    /**
    * Sets the packetization rate. 
    * @param aPTime, Packetization rate, must be multiple of audio frame size.
    * @return KErrNotSupported if codec doesn't support packetrate value issued; otherwise KErrNone.
    */
    virtual TInt SetPTime( TUint aPTime );
    
    /**
    * Sets the maximum packetization rate.
    * @param aMaxPTime, maximum allowed packetization rate, must be multiple of audio frame size.
    * @return KErrNotSupported if codec doesn't support packetrate value issued; otherwise KErrNone.
    */
    virtual TInt SetMaxPTime( TUint aMaxPTime );

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
    * Creates server stream(s) for this codec
    * @param aCodecIndex
    * @param aStreams stream is added to the array
    * @param aAudio
    * @param aManager
    */
    virtual void DoDecodeAudioL( TInt aCodecIndex,
                                 RPointerArray<CMceSrvStream>& aStreams,
                                 CMceComAudioStream& aAudio,
                                 CMceMediaManager& aManager );
    
protected: // from CMceComCodec
    
    /**
    * Set default fmtp line
    */                                    
    void DoSetDefaultFmtpAttributeL();    

protected:
    
    void DoMccEncodeComfortNoiseL( CMccCodecInformation& aMccCodec,
                                   CMceSrvStream& aStream );
    
    void DoMccEncodeRedundancyL( CMccCodecInformation& aMccCodec,
                                 CMceSrvStream& aStream );
                        
    void UpdateSendCodec( TInt& aCodecIndex, 
                          CMceComAudioStream& aAudio, 
                          CMceComAudioCodec& aCodec );
                                                         
#endif//MCE_COMMON_SERVER_SIDE
    
protected:

    /**
    * C++ default constructor.
    */
    CMceComAudioCodec();
    
     /**
     * second-phase copy constructor
     */
	void ConstructL( CMceComAudioCodec& aCodec );
    
    
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
	
	/**
	* Payload type of associated red codec
	*/ 
	TUint8 iRedPayloadType;
	
	/**
	* Redundancy level
	*/
	TUint iRedCount;
	
	/**
	* Comfort noise
	*/
	TBool iComfortNoiseEnabled;


private:    // Friend classes

    #ifdef TEST_EUNIT
        friend class UT_CMceComAudioCodec;
    #endif
    };



#endif

// End of File