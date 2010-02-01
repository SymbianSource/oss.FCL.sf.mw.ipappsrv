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




#ifndef CMCEMEDIASTREAM_H
#define CMCEMEDIASTREAM_H

// INCLUDES
#include <e32std.h>  	
#include <e32base.h>
#include <mcedefs.h>

// DATA TYPES
typedef TUint TMceMediaType;
 
// FORWARD DECLARATIONS
class CMceComMediaStream;
class TMceFactory;
class TMceMediaStreamFactory;
class RWriteStream;
class RReadStream;
class CDesC8Array;
class MDesC8Array;
class CMceSession;
class TMceIds;
class TMceEvent;
class TMceMediaId;
class CMceManager;
class CMceSession;
class CMceMediaSource;
class CMceMediaSink;
class MMceComSerializationContext;
class CMceFileSource;
class CMceCodec;

// CLASS DECLARATION

/**
* Base class for MCE media streams.
*
* MCE Media stream represents stream of data negotiated between two terminals.
* Streams may transfer any data.
*
* Enabling or disabling of a stream is not a local operation, but it requires
* always signalling. Thus, these operation require always to be completed by
* calling CMceSession::UpdateL() function.
*
*  @lib mceclient.lib
*/
class CMceMediaStream : public CBase
	{
	public: // Stream states

	    enum TState
	        {
	        EUninitialized,       // Stream is created
	        EInitialized,         // Stream is initialized
	        EBuffering,           // Stream is buffering
	        EIdle,                // Stream is not receiving RTP
	        EStreaming,           // Stream is streaming
	        EDisabled,            // Stream is explicitly disabled
	        ENoResources,         // Stream has no needed resources to stream
	        ETranscodingRequired, // Stream requires non-realtime transcoding
	        ETranscoding          // Stream is transcoding in non-realtime
	        };
	        
	public: // Destructor

	    /**
	    * Destructor.
	    */
		IMPORT_C ~CMceMediaStream();

	public: // Functions
	
	    /**
	    * Gets the state of the stream.
	    * @return the current state of the stream
	    */
	    IMPORT_C CMceMediaStream::TState State() const;

	    /**
	    * Returns the type of the stream.
	    * @return The type of the stream.
	    */
		IMPORT_C TMceMediaType Type() const;

	    /**
		* Configures media stream by updating SDP media lines. 
		* In order to get complete update, whole session must be 
		* updated causing sending of re-invite.
	    * @param aMediaSDPLines, application specific SDP media lines for
		*		 the stream. Ownership is transferred.
	    */
		IMPORT_C void SetMediaAttributeLinesL( CDesC8Array* aMediaSDPLines );
		
		/**
		* Gets attribute lines of the media.
		* @return array of media attribute lines, owneship is transferred.
		*/
		IMPORT_C MDesC8Array* MediaAttributeLinesL();

		/** 
	    * Sets local media port
	    * @param aLocalMediaPort, local port for the media
	    */
	    IMPORT_C void SetLocalMediaPortL(TUint aLocalMediaPort); 
	    
	    /**
	    * Gets the local media port.
	    * @return Local port used for media.
	    */
	    IMPORT_C TUint LocalMediaPort() const;
	    
	    /**
	    * Gets the session this stream belongs to, if exists. The
	    * ownership is not transfered.
	    * @return Session object
	    */
		IMPORT_C CMceSession* Session() const;
	              

	public: // Local stream control

	    /**
	    * Enables streaming explicitly.
	    */
	    virtual void EnableL(); 

	    /**
	    * Disables streaming explicitly.
	    */
		virtual void DisableL(); 
		
		/**
	    * Returns the state of the stream.
	    * @return ETrue if enabled, EFalse if disabled.
	    */
    	IMPORT_C TBool IsEnabled() const;

		/**
	    * Sets state of stream
	    * @param aState the state
	    */
	    TBool SetState( TState aState );

    public: // Source and sink functions

        /**
        * Adds source to the stream. CMceMediaStream can only
        * have one source at a time. Ownership of passed source is transferred.
        * However, the same source can be also passed to other streams resulting
        * that streams share ownership of the source. Passing NULL source will
        * remove source from stream.
        * In some cases the source might contain a codec. The codec is derived
        * to the stream when such source is added to the stream. 
        * Derived codec will get automatically highest preference.
        * @param aSource source to be added to the stream; ownership is 
        *        transferred
        */
        IMPORT_C void SetSourceL( CMceMediaSource* aSource );

        /**
        * Adds sink to the stream. CMceMediaStream can have
        * many sinks at a time. Ownership of passed sink is transferred.
        * However, the same sink can be also passed to other streams resulting
        * that streams share ownership of the sink.
        * @param aSink sink to be added to the stream; ownership is transferred
        */
        IMPORT_C void AddSinkL( CMceMediaSink* aSink );

        /**
        * Gets source of the stream. Returns NULL if source is not set.
        * @return source of the stream; ownership is not transferred
        */
        IMPORT_C CMceMediaSource* Source() const;

        /**
        * Gets array of sinks of the stream.
        * @return array of sinks
        */
        IMPORT_C const RPointerArray<CMceMediaSink>& Sinks() const;

        /**
        * Removes sink from the stream.
        * @post aSink cannot be used anymore and contents of 
        *       previously returned sink array (CMceMediaStream::Sinks())
        *       are not anymore valid
        * @param aSink sink to be removed from the stream
        */
        IMPORT_C void RemoveSinkL( CMceMediaSink& aSink );

    public: // Binding opposite way stream

        /**
        * Binds opposite direction stream to this stream.
        * @param aStream another stream to be bind to the stream; 
        *        ownership is transferred
        */
        IMPORT_C void BindL( CMceMediaStream* aStream );

        /**
        * Gets opposite direction stream of this stream.
        * @return opposite direction stream
        */
        IMPORT_C CMceMediaStream& BoundStreamL() const;

        /**
        * Checks if opposite direction stream is set.
        * @return ETrue if opposite direction stream is set; 
        *         otherwise EFalse
        */
        IMPORT_C TBool BoundStream() const;

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


public:// internal serialization
	    
        /**
        * Adds source, during serialization, to the stream.
        * @param aSerCtx serialization context
        */
        void SetSourceL( MMceComSerializationContext& aSerCtx );

        /**
        * Adds sink, during serialization, to the stream.
        * @param aSerCtx serialization context
        */
        void AddSinkL( MMceComSerializationContext& aSerCtx );
	    
	    /**
	    * Factory
	    * @return factory
	    */
	    TMceFactory BaseFactory() const;

	    /**
	    * Factory
	    * @return factory
	    */
	    static TMceMediaStreamFactory Factory();

	public: //internal

	    /**
	    * Checks if this is binder (biding owner)
	    * @return ETrue if this is binder (biding owner)
	    */
		TBool Binder() const;
		
	    /**
	    * ID 
	    * @return ID
	    */
		TMceMediaId Id() const;

	    /**
	    * Initializes 
	    * @param aParent the parent
	    * @param aDiscardUnusedCodecs
	    */
	    virtual void InitializeL( CMceSession& aParent, TBool aDiscardUnusedCodecs );
	    
	    /**
	    * Initializes 
	    * @param aManager the manager. Ownership is NOT transferred
	    * @param aSession the Session.
	    */
	    virtual void InitializeL( CMceManager* aManager, CMceSession& aSession );
	    
	    /**
	    * Called when stream is added to session
	    */
	    virtual void AddedL();
	    
	    /**
	    * Called file source wants to synchronized with stream
	    * @param aFile the file source
	    */
        virtual void SynchronizeWithFileL( CMceFileSource& aFile ) = 0;
        
        /**
        * Searches for codec matching with input codec instance or sdp name
        * @param aCodec the search term
        * @return matching codec or NULL if not found
        */
        virtual CMceCodec* FindCodec( CMceCodec& aCodec ) = 0;
        
      /**
	    * Arranges codec priority order based on codecs preference values.
	    * @param aDiscardUnusedCodecs ETrue if all disabled codecs should
	    *        be removed from the stream
	    */
        virtual void ReorderCodecsByPreferenceL( TBool aDiscardUnusedCodecs ) = 0;
        
        /**
        * Return codecs of stream.
        * @return list of codecs in the stream.
        */
        virtual const RPointerArray<CMceCodec>& BaseCodecs() = 0;
        
        /**
        * Returns pointer to the bound stream and NULL if no bound
        * stream exists.
        * @return bound stream
        */
	    CMceMediaStream* Bound() const;
	    
	    /**
	    * Gets the flat data
	    * @return flat data
	    */
	    CMceComMediaStream* FlatData();
	    
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
	    * Handles possible media errors.
	    * @return ETrue, if media error occured and was handled.
	    */
	    TBool HandleMediaError( TState aState, TBool aIsEnabled, TInt aError );
	    
	    
	protected:

	    /**
	    * C++ default constructor.
	    */
		CMceMediaStream();

	    /**
	    * Second-phase constructor
	    * @param aFlatData flat data container
	    */
		void ConstructL( CMceComMediaStream* aFlatData );
		
		/**
		* Search for codec inside given codec array
		* @param aCodecs array of codecs
		* @param aCodec search term
		* @return matching codec or NULL if not found
		*/
		CMceCodec* FindCodec( const RPointerArray<CMceCodec>& aCodecs, 
                              CMceCodec& aCodec );


	private:

	    /**
	    * Handles event
	    * @param aEvent the event
	    * @return status, if event was consumed or not or object needs update
	    */
	    TInt HandleEvent( TMceEvent& aEvent );

	    /**
	    * one-way bind
	    * @param aStream another stream to be bind to the stream.
	    */
	    void DoBindL( CMceMediaStream* aStream );
	    
	    /**
	    * Deletes sink
	    * @param aIndex index of sink in the array
	    */
        void DeleteSink( TInt aIndex );
        
	    /**
	    * Deletes source
	    */
    	void DeleteSource();
	    

	protected: // NOT owned data
				
	    /**
	     * session
	     */
		CMceSession* iSession;
		
	protected: // owned data

	    /**
	     * flat data container
	     */
		CMceComMediaStream* iFlatData;

	    /**
	     * linked stream. Owned if iLinkOwner is ETrue
	     */
	    CMceMediaStream* iLinkedStream;
	        
	    /**
	     * source
	     */
		CMceMediaSource* iSource;
		
	    /**
	     * sinks
	     */
		RPointerArray<CMceMediaSink> iSinks;
		
    private: // Reserved for future use
    
        TAny* iReserved;       

		//for testing

	    MCE_UNIT_TEST_DEFS	

	};


#endif
