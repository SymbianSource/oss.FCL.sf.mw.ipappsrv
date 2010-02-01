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




#ifndef CMCECOMENDPOINT_H
#define CMCECOMENDPOINT_H

#include "mcecomserializable.h"

#ifdef MCE_COMMON_SERVER_SIDE
class TMceMccComEvent;
#endif//MCE_COMMON_SERVER_SIDE

class TMceComFactory;
class TMceComEvent;
class CMceComMediaStream;
class CMceComCodec;
class MMceComEndPointProxy;
class CMceItcSender;
class RMce;

const TBool KMceEnabledLocal = ETrue;
const TBool KMceEnabledE2e = EFalse;

typedef TUint8 TMceEndpointCategory;

/**
 * 
 *
 * @lib 
 */
class CMceComEndpoint: public CBase, 
                       public MMceComSerializable
	{

public: // Destructor

	~CMceComEndpoint();


public: // from MMceComSerializableMedia


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
    * Return category of endpoint.
    * @return category of endpoint
    */
    TMceEndpointCategory Category() const;
    
    /**
    * Returns type of endpoint. NOTE, sources and sinks have currently
    * overlapping type values so type value checking is not safe without
    * knowing endpoint category. TBD: remove overlapping from API
    * @return type of endpoint
    */
    TUint8 Type() const;

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
    * Sets and gets reference count describing to how many
    * streams sink is associated
    * @return reference count reference
    */
    TInt& ReferenceCount();

	/**
    * Parent object
    * @return Parent object
    */
	CMceComMediaStream* MediaStream() const;

    /**
    * equal operator
    * @param aEndpoint the enpoint
    * @return ETrue, if equal
    */
    TBool operator==( const CMceComEndpoint& aEndpoint ) const;

    /**
    * Stream is been deleted
    * @param aParent the parent stream
    */
    void UnInitialize( CMceComMediaStream& aParent );


public: // virtual functions


    /**
    * Returns ETrue, if enabled
    * @param aLocally, if KMceEnabledLocal means locally enabled,
    *                  KMceEnabledE2e means that local and remote 
    *                  endpoints are enabled.
    * @return ETrue, if enabled
    */
    virtual TBool IsEnabled( TBool aLocally = KMceEnabledLocal ) const;

    /**
    * Sets enabled flag
    * @param aValue enabled/disabled
    */
    void Enabled( TBool aValue );
	
    /**
    * Traversal event handler
    * @param aEvent the event
    * @return status, if event was consumed or not or object needs update
    */
    virtual TInt EventReceivedL( TMceComEvent& aEvent ) = 0;
    
    /**
    * Initializes 
    * @param aParent the parent
    */
    virtual void InitializeL( CMceComMediaStream& aParent );
    
    /**
    * Stream has been added to session
    * @param aParent the parent stream
    */
    virtual void StreamAddedL( CMceComMediaStream& aParent );

    /**
    * Returns endpoint proxy
    * @return endpoint proxy
    */
	MMceComEndPointProxy* EndpointProxy() const;

    /**
    * Called when stream bundle has beed added to prepare proxy for it
    * @param aEndpointProxy endpoint proxy setup by another endpoint or NULL
    */
    void SetupEndpointProxyL( MMceComEndPointProxy* aEndpointProxy );

    /**
    * Called endpoint proxy must be created
    * @return endpointProxy
    */
    virtual MMceComEndPointProxy* CreateEndpointProxyL() const;

    /**
    * Should this endpoint use endpoint proxy
    * @return ETrue, this endpoint uses endpoint proxy
    */
    virtual TBool UsesEndpointProxy() const;

    /**
    * Is this endpoint re-usable
    * @return ETrue, if this endpoint is re-usable
    */
    virtual TBool Reusable( const CMceComMediaStream& aParent ) const;     
    /**
    * Should use RTCP
    * return ETrue, if uses RTCP
    */
    virtual TBool UseRTCP();
    

#ifdef MCE_COMMON_SERVER_SIDE


public: //virtual functions 

    /**
    * Returns mcc type for mcc endpoint
    * @return mcc type
    */
    virtual const TUid MccType() = 0;
    
    /**
    * Returns init param for mcc endpoint
    * @return mcc type
    */
    virtual const TDesC8& InitParamL( const CMceComCodec& aCodec);
    
    /**
    * Endpoint can be informed about initialization completion
    */
    virtual void InitializedL();
    
    /**
    * Enables 'locally' this endpoint
    * @param aCodec the codec associated to this endpoint
    */
    virtual void EnableL( const CMceComCodec& aCodec );
    
    /**
    * Disables 'locally' this endpoint
    */
    virtual void DisableL();
    
    /**
    * Prepares this endpoint
    */
    virtual void PrepareL();
    
    /**
    * Called after endpoint has been prepared.
    * @return error code
    */ 
    virtual TInt DoPrepared();


public: //new functions 

    /**
    * Handles event received from mcc
    * @param aEvent the event from mcc
    */
    void EventReceived( TMceMccComEvent& aEvent );
    
    /**
    * Sets state
    * @param aEvent the event from mcc
    */
    void SetState( TMceMccComEvent& aEvent );

    void Merge( CMceComEndpoint& aMergeWith );
    
#else // MCE CLIENT SIDE

    /**
    * Gets itc sender.
    * @param aItcProvider
    */
    CMceItcSender& SenderL( RMce& aItcProvider );
#endif//MCE_COMMON_SERVER_SIDE


protected: 

    /**
    * Updates 
    * @param aEndpoint an endpoint
    */
    void BaseUpdateL( CMceComEndpoint& aEndpoint );
    
    /**
    * Zeroes instance (used in cloning)
    */
    void Zero();
    
    
protected:

    /**
     * C++ default constructor.
     * @param aType
     */
	CMceComEndpoint( TMceEndpointCategory aCategory, TUint8 aType );
		

public://serialized data

    /**
     * category of endpoint
     */
    TMceEndpointCategory iCategory;
    
    /**
     * type of endpoint
     */
    TUint8 iType;

    /**
     * ID
     */
    TMceMediaId iID;

    /**
     * reference count
     */
    TInt iReferenceCount;



protected://owned

    /**
     * proxy
     */
    MMceComEndPointProxy* iEndpointProxy;

    /**
     * Is enabled
     */
    TBool iIsEnabled;
    
    /**
    * Itc sender used by client when endpoint is not attached to session.
    */
    CMceItcSender* iItcSender;

private://NOT owned


	/**
    * Parent stream (one of)
    */
	CMceComMediaStream* iStream;
	
	/**
    * Parent streams
    */
	RPointerArray<CMceComMediaStream> iParents;
    
	//definitions for unit testing
	friend class UT_CMceSrvSource;
	};


#endif // CMCECOMENDPOINT_H
