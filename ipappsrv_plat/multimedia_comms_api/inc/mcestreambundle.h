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




#ifndef MCESTREAMBUNDLE_H
#define MCESTREAMBUNDLE_H

//  INCLUDES
#include <e32base.h>  	
#include <mcedefs.h>

// CONSTANTS

// FORWARD DECLARATIONS
class CMceMediaStream;
class CMceSession;
class RWriteStream;
class CMceComStreamBundle;
class MMceComSerializationContext;
class CMceMediaSink;
class MMceEndPointProxy;


/**
* Class for handling grouping of Mce streams (i.e. grouping of media lines).
*
* Can be used for example to synchronize audio and video streams.
*
* If streams are added to the bundle or removed from it after the bundle
* has been added to the session, the session has to be updated with
* CMceSEssion::UpdateL() function in order to complete the changes. 
*
*  @lib mceclient.lib
*/
class CMceStreamBundle: public CBase
	{
	public: // Bundle types
	
	    enum TMceStreamBundleType
	        {
	        ELS,
	        EFID
	        };
	    
	public: // Constructors and destructor

        /**
	    * Two-phased constructor.
	    */
	    IMPORT_C static CMceStreamBundle* NewL( TMceStreamBundleType aType );

	    /**
	    * Two-phased constructor.
	    */
		IMPORT_C static CMceStreamBundle* NewLC( TMceStreamBundleType aType );
             
	    /**
	    * Destructor.
	    */
		IMPORT_C ~CMceStreamBundle();

    public:
    
        /**
	    * Type of the bundle
	    * @param aParent the parent
	    */
	    IMPORT_C TMceStreamBundleType Type() const;
    
        /**
        * Adds stream to the bundle.
        * @param aStream, stream to be added
        */
        IMPORT_C void AddStreamL( CMceMediaStream& aStream );
        
        /**
        * Removes stream from the bundle.
        * @param aStream, stream to be removed
        */
        IMPORT_C void RemoveStreamL( CMceMediaStream& aStream );
        
         /**
	    * Returns the streams belonging to the bundle.
	    * @return streams of the bundle.
	    */
		IMPORT_C const RPointerArray< CMceMediaStream >& Streams();

        
    public: // from MMceComSerializable

        /**
        * Returns serialization id
        * @return serialization id
        */
        TUint64 SerializationId() const;
        
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
        * Internalizes
        * @param aSerCtx context for serialization
        */
        void InternalizeL( MMceComSerializationContext& aSerCtx );
        
        /**
        * Externalizes
        * @param aSerCtx context for serialization
        */
        void ExternalizeL( MMceComSerializationContext& aSerCtx );

	public:
		
	    /**
	    * Initializes 
	    * @param aParent the parent
	    */
	    void InitializeL( CMceSession& aParent );
	    
	    /**
	    * Called when bundle has beed added to session
	    */
	    void AddedL();
	    

	private: //methods

	    /**
	     * C++ default constructor.
	     */
		CMceStreamBundle();

	    /**
	     * second-phase constructor
	     */
		void ConstructL( TMceStreamBundleType aType );
		
        void AddedSinkL( CMceMediaStream& aStream,
                         CMceMediaSink*& aCurrentSink,
                         MMceEndPointProxy*& aProxy );


	protected: // NOT owned data
				
	    /**
	     * session
	     */
		CMceSession* iSession;

	protected://data
	    
	    /**
	     * flat data container
	     */
		CMceComStreamBundle* iFlatData;

	private: // Data
	
	    /**
	    * Bundled streams, streams are not owned
	    */
	    RPointerArray< CMceMediaStream > iStreams;
	    
	    RPointerArray< CMceMediaStream > iTempStreams;
	    
		//for testing

	    MCE_UNIT_TEST_DEFS	

	};

#endif
