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




#ifndef CMCEAUDIOCODEC_H
#define CMCEAUDIOCODEC_H

// INCLUDES
#include <e32base.h>
#include <mcedefs.h>
#include <mcecodec.h>

// FORWARD DECLARATIONS
class RReadStream;
class RWriteStream;
class CMceComAudioCodec;
class CMceMediaStream;
class TMceIds;
class TMceEvent;
class TMceMediaId;
class TMceFactory;
class TMceAudioCodecFactory;

// CLASS DECLARATION

/**
* Container class for audio codecs.
*
* It defines attributes that can be set and get from audio codec.
* These attributes are used in single audio stream and they
* are negotiated with remote terminal.
*
*  @lib mceclient.lib
*/
class CMceAudioCodec : public CMceCodec
    {
	public:  // Constructors and destructor
	        
	    /**
	    * Destructor.
	    */
	    virtual ~CMceAudioCodec();

	public: // New functions
	        
	    /**
	    * Enable / Disable Voice Activity Detection.
	    * @param aEnableVAD, Boolean indicating whether to enable Voice Activity Detection
	    * @return KErrNotSupported if codec doesn't support VAD; otherwise KErrNone.
	    */
	    virtual TInt EnableVAD(TBool aEnableVAD) = 0;
	    
	    /**
	    * Sets the sampling frequency.
	    * @param aSamplingFreq, Sampling frequency to be used
	    * @return KErrNotSupported if codec doesn't support sampling frequency issued; otherwise KErrNone.
	    */
	    virtual TInt SetSamplingFreq(TUint aSamplingFreq) = 0;
	    
	    /**
	    * Sets the packetization rate. 
	    * @param aPTime, Packetization rate, must be multiple of audio frame size.
	    * @return KErrNotSupported if codec doesn't support packetrate value issued; otherwise KErrNone.
	    */
	    virtual TInt SetPTime(TUint aPTime) = 0;        
	    
	    /**
	    * Sets the maximum packetization rate.
	    * @param aMaxPTime, maximum allowed packetization rate, must be multiple of audio frame size.
	    * @return KErrNotSupported if codec doesn't support packetrate value issued; otherwise KErrNone.
	    */
	    virtual TInt SetMaxPTime(TUint aMaxPTime) = 0; 
	          
	    /**
	    * Clones codec, ownership is transferred.
	    * @return cloned codec
	    */
	    virtual CMceAudioCodec* CloneL() const = 0;
	    
	    /**
	    * Gets sampling frequency used currently.
	    * @return Sampling frequency
	    */
	    virtual TUint SamplingFreq() const;

	    /**
	    * Get VAD status.
	    * @return Boolean indicating is VAD activated
	    */
	    virtual TBool VAD() const;
	    
	    /**
	    * Gets the packetization rate.
	    * @return packetization rate.
	    */
	    virtual TUint PTime() const;        
	    
	    /**
	    * Gets max packetization rate.
	    * @return Max packetization rate.
	    */
	    virtual TUint MaxPTime() const;

	public: 

	    /**
	    * Initializes 
	    * @param aParent the parent
	    */
	    virtual void InitializeL( CMceMediaStream& aParent );    
	                
	    /**
	    * Factory
	    * @return factory
	    */
	    static TMceAudioCodecFactory Factory();
	    
	    static TInt ComparePreferences( const CMceAudioCodec& aIndex1, 
                                        const CMceAudioCodec& aIndex2 );
	                
	protected:  // New functions

	    /**
	    * C++ default constructor.
	    */
	    CMceAudioCodec();

	    /**
	     * second-phase constructor
	     * @paramaFlatData flat data container
	     * @param aType type of sink
	     */
		void ConstructL( CMceComAudioCodec* aFlatData );


		//for testing

	    MCE_UNIT_TEST_DEFS	
    };


#endif

// End of File
