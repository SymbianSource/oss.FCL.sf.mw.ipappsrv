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




#ifndef CMCEMEDIASINK_H
#define CMCEMEDIASINK_H

// INCLUDES
#include <e32std.h>  		
#include <e32base.h> 		
#include <badesca.h>
#include <mcedefs.h>

// DATA TYPES
typedef TUint TMceSinkType;

// FORWARD DECLARATIONS
class RReadStream;
class RWriteStream;
class CMceComMediaSink;
class TMceSinkFactory;
class CMceMediaStream;
class TMceIds;
class TMceEvent;
class TMceMediaId;
class CMceManager;
class MMceComSerializationContext;
class MMceEndPointProxy;


// CLASS DECLARATION

/**
* Base class for the MCE media sinks.
*
* Media sinks represent the destination of data.
* CMceMediaSink gets it data from CMceMediaSource through
* CMceMediaStream. 
*
* CMceMediaSink has a type that defines how it can connected to other
* sources and streams.
*
* Enable and Disable operations to sinks are considered to be local,
* so they are not signalled to remote terminal.
* 
*  @lib mceclient.lib
*/
class CMceMediaSink: public CBase
	{

	public: // Destructor

		IMPORT_C ~CMceMediaSink();

	public: // Functions

	    /**
	    * Returns the type of the sink.
	    * @return The type of the sink.
	    */
		IMPORT_C TMceSinkType Type() const;

	    /**
	    * Enables streaming explicitly.
	    */
		virtual void EnableL() = 0; 

	    /**
	    * Disables streaming explicitly.
	    */
		virtual void DisableL() = 0;

	    /**
	    * Returns the state of the sink.
	    * @return ETrue if enabled, EFalse if disabled.
	    */
		IMPORT_C TBool IsEnabled() const;

	public: //internal
		
		
	    /**
	    * ID 
	    * @return ID
	    */
		TMceMediaId Id() const;
		
	    /**
	    * Initializes 
	    * @param aParent the parent
	    */
	    virtual void InitializeL( CMceMediaStream& aParent );

        /**
	    * Initializes 
	    * @param aManager the manager. Ownership is NOT transferred
	    */
	    virtual void InitializeL( CMceManager* aManager );	    
	    
	    /**
	    * Sets and gets reference count describing to how many
	    * streams sink is associated
	    * @return reference count reference
	    */
	    TInt& ReferenceCount();
	    
	    /**
	    * Stream is been deleted
	    * @param aParent the parent stream
	    */
        void UnInitialize( CMceMediaStream& aParent );
        
	    /**
	    * Stream has been added to session
	    * @param aParent the parent stream
	    */
        virtual void StreamAddedL( CMceMediaStream& aParent );

	    /**
	    * Returns endpoint proxy
	    * @return endpoint proxy
	    */
		MMceEndPointProxy* EndpointProxy() const;

	    /**
	    * Called when stream bundle has beed added to prepare proxy for it
	    * @param aEndpointProxy endpoint proxy setup by another endpoint or NULL
	    */
        void SetupEndpointProxyL( MMceEndPointProxy* aEndpointProxy );

	    /**
	    * Called endpoint proxy must be created
	    * @return endpointProxy
	    */
        virtual MMceEndPointProxy* CreateEndpointProxyL() const;

	    /**
	    * Should this endpoint use endpoint proxy
	    * @return ETrue, this endpoint uses endpoint proxy
	    */
        virtual TBool UsesEndpointProxy() const;
        
	    /**
	    * Factory
	    * @return factory
	    */
	    static TMceSinkFactory Factory();

        /**
        * Sets enabled flag
        * @param aValue enabled/disabled
        */
        void Enabled( TBool aValue );
	    
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
        virtual void InternalizeL( MMceComSerializationContext& aSerCtx );
        
        /**
        * Externalizes
        * @param aSerCtx context for serialization
        */
        virtual void ExternalizeL( MMceComSerializationContext& aSerCtx );


	public://event handling & update

	    /**
	    * Traversal event handler
	    * @param aEvent the event
	    * @return status, if event was consumed or not or object needs update
	    */
	    virtual TInt EventReceivedL( TMceEvent& aEvent );
	    
	    /**
	    * Called after update
	    */
	    virtual void Updated();

	    /**
	    * Updates
	    * @param aUpdate updated data
	    */
        void UpdateL( CMceMediaSink& aUpdate );
        
	protected:

	    /**
	     * C++ default constructor.
	     */
		CMceMediaSink();
		
	    /**
	     * second-phase constructor
	     * @paramaFlatData flat data container
	     */
		void ConstructL( CMceComMediaSink* aFlatData );

	    /**
	    * Enables streaming explicitly.
	    */
		void DoEnableL();

	    /**
	    * Disables streaming explicitly.
	    */
		void DoDisableL();

	    /**
	    * Handles event
	    * @param aEvent the event
	    * @return status, if event was consumed or not or object needs update
	    */
	    TInt HandleEvent( TMceEvent& aEvent );

    private:

	    /**
	    * Handles ITC
	    */
	    void DoITCSendL( TUint8 aITCFunction, TBool aEnabled );

	protected: // NOT owned data
		
	    /**
	     * stream
	     */
		CMceMediaStream* iStream;
		
		RPointerArray<CMceMediaStream> iParents;
		
	protected://data
	    
	    /**
	     * flat data container
	     */
		CMceComMediaSink* iFlatData;
		
		MMceEndPointProxy* iEndpointProxy;

		//for testing

	    MCE_UNIT_TEST_DEFS	
	    
		};


#endif
