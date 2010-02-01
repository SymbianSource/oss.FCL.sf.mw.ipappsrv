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




#ifndef CMCEAUDIOSTREAM_H
#define CMCEAUDIOSTREAM_H

//  INCLUDES
#include <e32std.h>  		
#include <mcemediastream.h>

// CONSTANTS
const TMceMediaType KMceAudio = 1;

// FORWARD DECLARATIONS
class CMceAudioCodec;

// CLASS DECLARATION

/**
* Class for representing unidirectional audio stream.
*
* It defines the used audio codecs for the stream.
* In order to be complete structure, at least one codec is required.
*
*  @lib mceclient.lib
*/
class CMceAudioStream: public CMceMediaStream
	{
	public: // Constructors and destructor

	    /**
	    * Two-phased constructor.
	    */
		IMPORT_C static CMceAudioStream* NewL();
	   
	    /**
	    * Two-phased constructor.
	    */
		IMPORT_C static CMceAudioStream* NewLC();
		
	    /**
	    * Destructor.
	    */
		IMPORT_C ~CMceAudioStream();

	public: // Functions

	    /**
	    * Returns the available codecs to the session.
	    * For CMEOutSession, array contains all codecs supported by
	    * terminal in default. For CMEInSession, array contains codecs
	    * that were offered by remote terminal, and supported by local terminal.
	    * Codecs are in preference order in the array. 
	    * Codec in the first element of the array has highest preference.
	    * @return codecs available for the session.
	    */
		IMPORT_C const RPointerArray<CMceAudioCodec>& Codecs();

	    /**
	    * Adds codec to the stream.
	    * @param aCodec codec added to the stream; ownership is transferred
	    */
		IMPORT_C void AddCodecL( CMceAudioCodec* aCodec );

	    /**
	    * Removes codec from the stream
	    * @param aCodec codec removed from stream
	    */
		IMPORT_C void RemoveCodecL( CMceAudioCodec& aCodec );
		
		/**
	    * Replaces codecs of the stream. Codecs should be in preference order in 
	    * the passed array.
	    * @param aCodecs array of codecs which will replace existing codecs; 
	    *        ownership is transferred
	    */
		IMPORT_C void ReplaceCodecsL( RPointerArray<CMceAudioCodec>* aCodecs );

	public: // serialization

	    /**
	    * Internalizes
	    * @param aReadStream read stream
	    * @param aTarget to which read to
	    */
	    void InternalizeL( MMceComSerializationContext& aSerCtx );
	    
	    /**
	    * Externalizes
	    * @param aWriteStream write stream
	    */
	    void ExternalizeL( MMceComSerializationContext& aSerCtx );

	    
	public: //from CMceMediaStream
		
	    /**
	    * Initializes 
	    * @param aParent the parent
	    * @param aDiscardUnusedCodecs
	    */
	    void InitializeL( CMceSession& aParent, TBool aDiscardUnusedCodecs );

	    /**
	    * Initializes 
	    * @param aManager the manager. Ownership is NOT transferred
	    * @param aSession the Session.
	    */
        void InitializeL( CMceManager* aManager, CMceSession& aSession );

	    /**
	    * Traversal event handler
	    * @param aEvent the event
	    * @return status, if event was consumed or not or object needs update
	    */
	    TInt EventReceivedL( TMceEvent& aEvent );

	    /**
	    * Called file source wants to synchronized with stream
	    * @param aFile the file source
	    */
        void SynchronizeWithFileL( CMceFileSource& aFile );
        
        /**
        * Searches for codec matching with input codec instance or sdp name
        * @param aCodec the search term
        * @return matching codec or NULL if not found
        */
        CMceCodec* FindCodec( CMceCodec& aCodec );
        
        /**
        * Arrange audio codecs according codec preferences.
        */
        void ReorderCodecsByPreferenceL( TBool aDiscardUnusedCodecs );
        
        /**
        * Return codecs of stream.
        * @return list of codecs in the stream.
        */
        const RPointerArray<CMceCodec>& BaseCodecs();
             
    public: //internal
        
        /**
        * Serches for existence of components needed for dtmf usage.
        * @param aRtpSourceExists
        * @param aRtpSinkExists
        * @param aDtmfCodecExists
        * @param aBinderOriginated
        */	    
	    void FindDtmfRelatedComponents( TBool& aRtpSourceExists, 
	                                    TBool& aRtpSinkExists, 
	                                    TBool& aDtmfCodecExists,
	                                    TBool aBinderOriginated ); 
        /**
        * Removes codec
        * @param aCodec
        * @param aBinderOriginated
        */	   
        void RemoveCodecL( CMceAudioCodec& aCodec, TBool aBinderOriginated );

	private: //methods

	    /**
	     * C++ default constructor.
	     */
		CMceAudioStream();

	    /**
	    * Second-phase constructor.
	    */
		void ConstructL();
	    		

	private: // Owned data

	    /**
	     * codecs
	     */
		RPointerArray<CMceAudioCodec> iCodecs;

		//for testing

	    MCE_UNIT_TEST_DEFS	
	};

#endif
