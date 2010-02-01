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




#ifndef MCECOMSTREAMBUNDLE_H
#define MCECOMSTREAMBUNDLE_H

//  INCLUDES
#include <e32base.h>  	
#include <mcedefs.h>

// CONSTANTS

#ifdef MCE_COMMON_SERVER_SIDE

#include "mcesrv.h"

#endif//MCE_COMMON_SERVER_SIDE

#include "mcestreambundle.h"
#include "mcecomserializable.h"

class CMceComMediaStream;
class CMceComSession;
class CMceComEndpoint;
class TMceComFactory;
class MMceComEndPointProxy;

// FORWARD DECLARATIONS

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
class CMceComStreamBundle : public CBase, 
                            public MMceComSerializable
	{
	
public: // Constructors and destructor

    /**
    * Two-phased constructor.
    */
    static CMceComStreamBundle* NewL( CMceStreamBundle::TMceStreamBundleType aType );

    /**
    * Two-phased constructor.
    */
	static CMceComStreamBundle* NewLC( CMceStreamBundle::TMceStreamBundleType aType );
         
    /**
    * Destructor.
    */
	~CMceComStreamBundle();

public: //static API


    static TBool InBundle( CMceComEndpoint& aEndpoint, 
                           CMceComSession& aSession );

public:

    /**
    * Type of the bundle
    * @param aParent the parent
    */
    CMceStreamBundle::TMceStreamBundleType Type() const;

    /**
    * Adds stream to the bundle.
    * @param aStream, stream to be added
    */
    void AddStreamL( CMceComMediaStream& aStream );
    
    /**
    * Removes stream from the bundle.
    * @param aStream, stream to be removed
    */
    void RemoveStreamL( CMceComMediaStream& aStream );
    
     /**
    * Returns the streams belonging to the bundle.
    * @return streams of the bundle.
    */
	const RPointerArray< CMceComMediaStream >& Streams() const;
	
    /**
    * Called when bundle has beed added to session
    */
    void AddedL();
    
    
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


public: // new functions

    /**
    * ID 
    * @return ID
    */
	TMceMediaId Id() const;

    /**
    * Factory
    * @return factory
    */
    TMceComFactory BaseFactory();

    /**
    * Initializes 
    * @param aParent the parent
    */
    void InitializeL( CMceComSession& aParent );

    /**
    * Updates this object
    * @param aUpdate object updates this
    */
    void UpdateL( CMceComStreamBundle& aUpdate );
    
    /**
    * Clones this object
    * @return the cloned object
    */
    CMceComStreamBundle* CloneL();

    /**
    * Clones this object
    * @param aStreams array containing streams
    * @return the cloned object
    */
    CMceComStreamBundle* CloneL( const RPointerArray<CMceComMediaStream>& aStreams );


private: //methods

    /**
     * C++ default constructor.
     */
	CMceComStreamBundle( CMceStreamBundle::TMceStreamBundleType aType );

    /**
     * second-phase constructor
     */
	void ConstructL();

    /**
    * Zeroes instance (used in cloning)
    */
    void Zero();


    void AddedSinkL( CMceComMediaStream& aStream,
                     CMceComEndpoint*& aCurrentSink,
                     MMceComEndPointProxy*& aProxy );

    void AddedSourceL( CMceComMediaStream& aStream,
                       CMceComEndpoint*& aCurrentSource,
                       MMceComEndPointProxy*& aProxy );

    
private: // Data


    /**
     * type
     */
    CMceStreamBundle::TMceStreamBundleType iBundleType;
    
    /**
     * ID
     */
    TMceMediaId iID;

    /**
     * streams
     */
    RPointerArray< CMceComMediaStream > iStreams;
    
    /**
     * session
     */
    CMceComSession* iSession;
    
    
#ifdef MCE_COMMON_SERVER_SIDE

	//definitions for unit testing
	MCEMM_UT_DEFINITIONS
    
#endif//MCE_COMMON_SERVER_SIDE
        
	MCE_UNIT_TEST_DEFS
	
	};

	

#endif //MCECOMSTREAMBUNDLE_H
