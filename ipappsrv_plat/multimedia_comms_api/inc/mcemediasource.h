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




#ifndef CMCEMEDIASOURCE_H
#define CMCEMEDIASOURCE_H

// INCLUDES
#include <e32std.h>  		
#include <e32base.h> 	
#include <mcedefs.h>

// DATA TYPES
typedef TUint TMceSourceType;

// FORWARD DECLARATIONS
class RReadStream;
class RWriteStream;
class CMceComMediaSource;
class TMceSourceFactory;
class CMceMediaStream;
class TMceEvent;
class TMceMediaId;
class CMceManager;
class MMceComSerializationContext;

// CLASS DECLARATION

/**
* Base class for the MCE media sources.
*
* Media sources represent the origin of data.
* CMceMediaSource puts it data through CMceMediaStream into
* CMceMediaSink.
*
* CMceMediaSource has a type that defines how it can connected to other
* sinks and streams.
*
* Enable and Disable operations to sinks are considered to be local,
* so they are not signalled to remote terminal.
*
* DTMF tones can be send from media sources that are connected to steams of TMceMediaType KMceAudio.
* Also, stream must have negotiated codec of type Telephony-Event.
*
*  @lib mceclient.lib
*/
class CMceMediaSource: public CBase
	{
	public: // Destructor

		IMPORT_C ~CMceMediaSource();

	public: // Functions

	    /**
	    * Returns the type of the source.
	    * @return The type of the source.
	    */
		IMPORT_C TMceSourceType Type() const;

	    /**
	    * Enables streaming explicitly.
	    */
		virtual void EnableL() = 0; 

	    /**
	    * Disables streaming explicitly.
	    */
		virtual void DisableL() = 0;

	    /**
	    * Returns the state of the source.
	    * @return ETrue if enabled, EFalse if disabled.
	    */
		IMPORT_C TBool IsEnabled() const;
		
        /**
	    * Checks if DTMF is available for using. DTMF is available
	    * if source is connected to steam of type TMceMediaType KMceAudio,
        * and it has negotiated codec of type Telephony-Event.
	    * @return ETrue if DTMF can be used, EFalse otherwise.
        */
        virtual TBool DtmfAvailable();

        /**
	    * Checks if DTMF is currently being played.
	    * @return ETrue if DTMF is currently played, EFalse otherwise.
        */
        virtual TBool DtmfActive();

        /**
	    * Starts playing of a tone. Tone is played until stopped
	    * by the client.
        * @pre DtmfAvailable() == ETrue && DtmfActive() == EFalse.
        * @post DtmfActive() == ETrue.
        * @param aTone DTMF tone to be played.
        */
        virtual void StartDtmfToneL( const TChar& aTone );

        /**
	    * Stops playing of a tone.
        * @pre DtmfAvailable() == ETrue && DtmfActive() == ETrue.
        */
        virtual void StopDtmfToneL();            	

        /**
	    * Sends a single tone.
        * @pre DtmfAvailable() == ETrue && DtmfActive() == EFalse.
        * @post DtmfActive() == ETrue.
        * @param aTone DTMF tone to be played.
        * @param aDuration Duration of DTMF tone to be played.
        */
        virtual void SendDtmfToneL( const TChar& aTone, TTimeIntervalMicroSeconds32 aDuration );

        /**
	    * Sends a sequence of tones.
        * @pre DtmfAvailable() == ETrue && DtmfActive() == EFalse.
        * @post DtmfActive() == ETrue.
        * @param aSequence Sequence of DTMF tones to be played.
        */
        virtual void SendDtmfToneSequenceL( const TDesC& aSequence );
	    
	    /**
	    * Cancels the sending of tone sequence.
        * @pre DtmfAvailable() == ETrue && DtmfActive() == ETrue.
        */
	    virtual void CancelDtmfToneSequenceL();
	    
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
	    * streams source is associated
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
	    static TMceSourceFactory Factory();

        /**
        * Sets enabled flag
        * @param aValue enabled/disabled
        */
        void Enabled( TBool aValue );

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
        void UpdateL( CMceMediaSource& aUpdate );
	    	    
	protected:

	    /**
	     * C++ default constructor.
	     */
		CMceMediaSource();

	    /**
	     * second-phase constructor
	     * @paramaFlatData flat data container
	     */
		void ConstructL( CMceComMediaSource* aFlatData );
		
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
	    void DoITCSendL( TUint8 aITCFunction, 
	                     TBool aEnabled, 
	                     TUint32 aData1 = 0,
                         TUint32 aData2 = 0 );

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
		CMceComMediaSource* iFlatData;

		//for testing

	    MCE_UNIT_TEST_DEFS	

		};


#endif 
