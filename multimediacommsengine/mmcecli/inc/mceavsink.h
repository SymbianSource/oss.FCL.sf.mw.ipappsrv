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




#ifndef MCEAVSINK_H
#define MCEAVSINK_H

// INCLUDES
#include <mcemediasink.h>
#include "mcecomserializable.h"
#include "mceendpointproxy.h"

#include <e32std.h>  
#include <gdi.h>

// FORWARD DECLARATIONS
class CWindowGc;
class RWindow;
class CMceManager;
class CMceItcSender;
class CMceComAvSink;

// DATA TYPES
const TMceSinkType KMceAvSink = 99;


// CLASS DECLARATION

/**
* Class representing audio/video sink in MCE.
*
* CMceAvSink can be used to show video containing audio on screen 
* 
* @lib mceclient.lib
*/
NONSHARABLE_CLASS( CMceAvSink ) : public CBase,
                                  public MMceComSerializable,
                                  public MMceEndPointProxy
                   
	{

	public: // Destructor

	    /**
	    * Two-phased constructor.
	    */
	    static CMceAvSink* NewL();

	    /**
	    * Two-phased constructor.
	    */
		static CMceAvSink* NewLC();


		~CMceAvSink();

	public: // Functions

	    /**
	    * Returns the type of the sink.
	    * @return The type of the sink.
	    */
		TMceSinkType Type() const;

	    /**
	    * Enables streaming explicitly.
	    */
		virtual void EnableL();

	    /**
	    * Disables streaming explicitly.
	    */
		virtual void DisableL();

	    /**
	    * Returns the state of the sink.
	    * @return ETrue if enabled, EFalse if disabled.
	    */
		TBool IsEnabled() const;

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
	    * Factory
	    * @return factory
	    */
	    static TMceSinkFactory Factory();
	    
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


    public: // from MMceEndPointProxy

        /**
        * Does proxy serve client
        * @param aClient a client
        * @return ETrue, if proxy serves client
        */
        TBool ServesProxyClient( const CMceMediaSink& aClient ) const;
        

        /**
        * Adds client to proxy
        * @param aClient a client
        */
        void AddProxyClientL( CMceMediaSink& aClient );

        /**
        * Removes client from proxy
        * @param aClient a client
        */
        void RemoveProxyClient( CMceMediaSink& aClient );
        

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
        void UpdateL( CMceAvSink& aUpdate );
        
	protected:

	    /**
	     * C++ default constructor.
	     */
		CMceAvSink();
		
	    /**
	     * second-phase constructor
	     */
		void ConstructL();

	    /**
	    * Enables streaming explicitly.
	    */
		void DoEnableL();

	    /**
	    * Disables streaming explicitly.
	    */
		void DoDisableL();

	private:

	    /**
	    * Handles event
	    * @param aEvent the event
	    * @return status, if event was consumed or not or object needs update
	    */
	    TInt HandleEvent( TMceEvent& aEvent );

	protected: // NOT owned data
		
	    /**
	     * stream
	     */
		CMceMediaStream* iStream;
		
		RPointerArray<CMceMediaStream> iParents;
		
		RPointerArray<CMceMediaSink> iProxyClients;
		
	protected://data
	    
	    /**
	     * flat data container
	     */
		CMceComAvSink* iFlatData;

		//for testing

	    MCE_UNIT_TEST_DEFS	
	    
		};

#endif //MCEAVSINK_H
