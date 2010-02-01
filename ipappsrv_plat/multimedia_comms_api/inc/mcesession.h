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




#ifndef CMCESESSION_H
#define CMCESESSION_H

//  INCLUDES
#include <e32base.h>
#include <mcedefs.h>

// DATA TYPES
typedef TUint TMceSessionType;

// CONSTANTS
const TMceSessionType KMceSessionNormal = 1;

// FORWARD DECLARATIONS
class TMceTransactionDataContainer;
class CDesC8Array;
class MDesC8Array;
class TMceIds;
class CMceMediaStream;
class CMceManager;
class TMceFactory;
class RReadStream;
class RWriteStream;
class CMceItcSender;
class CMceSessionReceiver;
class MMceFcMsgObserver;
class CMceComSession;
class MMceComSerializationContext;
class CMceMsgBase;
class TMceEvent;
class CMceInSession;
class CMceMsgSIPReply;
class CMceMsgSIPData;
class TMceFactory;
class CMceEvent;
class CMceRefer;
class CMceStreamBundle;
template <class T> class CMceMsgArray;


// CLASS DECLARATION

/**
* Base class for MCE sessions, inherited by both CMceInSession and CMceOutSession.
*
* CMceSession represents peer-to-peer connection made with remote terminal.
* This connection is also known as dialog and it is set up using SIP INVITE
* method.
*
* The characteristics of the session are defined by user of this class by attaching
* media streams into the session.
*
* After the session has been succesfylly established, it can be later updated and
* must be finally terminated.
*
* If for any reason instance of this class is deleted while in established state,
* session is automatically terminated by MCE server.
*
*  @lib mceclient.lib
*/
class CMceSession : public CBase
	{	
	public: // Session states

	    enum TState
	        {
	        EIdle, 			// Idle
	        EOffering,      // Outgoing session, or session update establishment in progress, 
	        EIncoming,		// Session received incoming call
	        EReserving,     // UpdateL called to incoming session during session establishment
	        EAnswering, 	// Answering an incoming call
	        EProceeding,	// Session media negotiations proceeding
	        EEstablished,   // Session established
	        ECancelling,    // Cancelling outgoing session establishment
	        ETerminating,   // Terminating session
	        ETerminated,    // Session terminated
	        };

	public: // Session termination reasons

	    enum TReason
	        {
	        EDeclined,
	        EBusy,
	        ECancelled,
	        EDisconnected
	        };

	public :// Control (signalling) path security levels

		enum TControlPathSecurityLevel 
	        {
	        EControlPathSecurityUnknown,
	        EControlPathUnsecure,
	        EControlPathSecure
	        };
        
	public: 

	    /**
	    * Destructor.
	    */
		virtual ~CMceSession();

	public: // Functions

	    /**
	    * Returns the state of the session.
	    * @return The current state of the session
	    */
	    IMPORT_C TState State() const;

	    /**
	    * Returns the state of network connection of the session.
	    * @return ETrue if connection active, EFalse if connection inactive.
	    */
	    IMPORT_C TBool ConnectionActive() const;
	    
	    /**
	    * Removes media stream from session.
		* @pre State() == CMceSession::EIdle ||
		*      State() == CMceSession::EIncoming ||
		*      State() == CMceSession::EEstablished
		* @post aMediaStream cannot be used anymore and contents of 
		*       previously returned stream array (CMceSession::Streams())
		*       are not anymore valid
	    * @param aMediaStream media stream to be removed
	    */
	    IMPORT_C void RemoveStreamL( CMceMediaStream& aMediaStream );

	    /**
		* Updates offered session parameters during session
		* establishement phase for first answer.
	    * Updates established session using re-invite.
	    * In state EIncoming the timeout value must be smaller or equal to
	    * offered value obtained by SessionTimer().
		* @pre State() == CMceSession::EIncoming ||
		*      State() == CMceSession::EEstablished
		* @post State() == CMceSession::EReserving for incoming session ||
		*       State() == CMceSession::EOffering for established session
	    * @param aTimeout, the session timer value in seconds
	    * @param aHeaders, SIP headers to be added. Ownership is
		*		 transferred.
	    * @param aContentType, content type of the body.
		*		 Ownership is transferred.
	    * @param aContent, content to be added in body. Ownership is 
		*		 transferred.
	    * @param aContentHeaders, content headers to be added. Ownership is
		*		 transferred.
	    */
		IMPORT_C void UpdateL( TUint32 aTimeout = 0,
                               CDesC8Array* aHeaders = 0, 
                               HBufC8* aContentType = 0,
                               HBufC8* aContent = 0,
                               CDesC8Array* aContentHeaders = 0 ); 

        /**
	    * Terminates established session.
		* @pre State() == CMceSession::EEstablished
		* @post State() == CMceSession::ETerminating
	    * @param aHeaders, SIP headers to be added. Ownership is
		*		 transferred.
	    * @param aContentType, content type of the body.
		*		 Ownership is transferred.
	    * @param aContent, content to be added in body. Ownership is 
		*		 transferred.
	    */
		IMPORT_C void TerminateL( CDesC8Array* aHeaders = 0,
					              HBufC8* aContentType = 0,
					              HBufC8* aContent = 0 ); 

	    /**
	    * Returns the streams belonging to the session.
	    * @return streams of the session.
	    */
		IMPORT_C const RPointerArray<CMceMediaStream>& Streams() const;

	    /**
	    * Returns the recipient of the session.
		* @pre State() == CMceSession::EIncoming ||
		*      State() == CMceSession::EEstablished
	    * @return The recipients address.
	    */
	    IMPORT_C const TDesC8& Recipient() const;

	    /**
	    * Returns the originator of the session.
		* @pre State() == CMceSession::EIdle ||
		*      State() == CMceSession::EIncoming ||
		*      State() == CMceSession::EEstablished
	    * @return the originators address.
	    */
	    IMPORT_C const TDesC8& Originator() const;

	    /**
		* Configures session by updating SDP session lines. 
		* In order to get complete update, whole session must be 
		* updated causing sending of re-invite.
		* @pre State() == CMceSession::EIdle ||
		*      State() == CMceSession::EIncoming ||
		*      State() == CMceSession::EEstablished
		* @pre aSessionSDPLines != 0
	    * @param aSessionSDPLines, application specific SDP lines for session.
		*		 Ownership is transferred.
	    */
		IMPORT_C void SetSessionSDPLinesL( CDesC8Array* aSessionSDPLines );
		
		/**
		* Gets SDP lines of the session.
		* @return array of session SDP lines, owneship is transferred.
		*/
		IMPORT_C MDesC8Array* SessionSDPLinesL();

		/**
	    * Returns the session timer value of the session.
		* @return the session timer value.
	    */
		IMPORT_C TUint32 SessionTimer() const;
				
		/**
		* Adds grouped media streams to the session. Changes SDP
		* of the session. In order to get complete update, whole
		* session must be updated causing sending of re-invite.
		* @pre State() == CMceSession::EIdle ||
		*      State() == CMceSession::EIncoming ||
		*      State() == CMceSession::EEstablished
		* @pre aBundle != 0
	    * @param aBundle group of media streams;
	    *        ownership of the bundle is transferred
	    */
		IMPORT_C void AddBundleL( CMceStreamBundle* aBundle );
		
		/**
	    * Removes grouped media stream from session. Changes SDP
		* of the session. In order to get complete update, whole
		* session must be updated causing sending of re-invite.
		* @pre State() == CMceSession::EIdle ||
		*      State() == CMceSession::EIncoming ||
		*      State() == CMceSession::EEstablished
		* @post aBundle cannot be used anymore and contents of 
		*       previously returned bundle array (CMceSession::Bundles())
		*       are not anymore valid
	    * @param aBundle bundle to be removed
	    */
	    IMPORT_C void RemoveBundleL( CMceStreamBundle& aBundle );
		
		/**
	    * Gets the grouped media streams of the session.
	    * @return bundles of the session
	    */
		IMPORT_C const RPointerArray< CMceStreamBundle >& Bundles() const;
		
        /**
        * Returns the associated events, if any.
	    * @return RPointerArray array of associated events. 
	    *         The ownership is transferred.
	    */
		IMPORT_C RPointerArray< CMceEvent >* AssociatedEventsL() const;

        /**
        * Returns the associated refers, if any.
	    * @return RPointerArray array of associated refers. 
	    *         The ownership is transferred.
	    */
		IMPORT_C RPointerArray< CMceRefer >* AssociatedRefersL() const;

        /**
        * Gets session modifier of type TUint.
        * @param aModifier is a modifier to be retrieved
        * @param aValue on return will contain modifier value
        * @leave KErrNotFound if modifier is not found
        */
        IMPORT_C void GetModifierL( TMceSessionModifier aModifier,
                                    TUint& aValue ) const;

        /**
        * Sets session modifier of type TUint.
        * @param aModifier is a modifier to be set 
        * @param aValue is a new value of a modifier
        * @leave KErrNotFound if modifier is not found
        */
        IMPORT_C void SetModifierL( TMceSessionModifier aModifier, 
                                    TUint aValue );        

        /**
        * Returns the identifier of profile used by the sesison.
	    * @return TUint32 Identifier of the used profile. 0, 
	    *         if session is setup without profile.
	    */
		IMPORT_C TUint32 Profile() const;

		 /**
		* Configures connections of the session by setting
		* type of service value for outgoing datagrams.
		* @pre State() == CMceSession::EIdle ||
		*      State() == CMceSession::EIncoming
		* @pre aServiceType >= 0 && aServiceType < 256 
		* @param aServiceType, Type of service value for outgoing datagrams.
		*/
		IMPORT_C void SetServiceTypeL( TUint32 aServiceType );       

		/**
		* Returns setting for type of service value for outgoing datagrams
		* used in session.
		* @return Type of service value for outgoing datagrams.
		*/
		IMPORT_C TUint32 ServiceType() const;       
		
	    /** 
	    * Checks the assumed end-to-end security level 
	    * on the session control/signalling path.  
	    * @return the assumed control path security level     
	    */ 
    	IMPORT_C TControlPathSecurityLevel ControlPathSecurityLevel() const;

	    /**
		* Sends an outgoing extension SIP request such as INFO 
		* within an existing session.
		* @pre State() != CMceSession::EIdle &&
		*      State() != CMceSession::EIncoming &&
		*      State() != CMceSession::ETerminated
		* @pre aMethod != "REGISTER" |"INVITE" | "BYE" | "CANCEL" | "ACK" |
        *   "SUBSCRIBE" | "NOTIFY" | "UPDATE" | "PRACK" | "REFER"
	    * @param aMethod, SIP method
	    * @param aHeaders, SIP headers to be added. 
	    *        The ownership is transferred.
	    * @param aContentType, content type of the body.
		*		 The ownership is transferred.
	    * @param aContent, content to be added in body. 
	    *        The ownership is transferred.
	    */
		IMPORT_C TMceTransactionId SendRequestL( const TDesC8& aMethod,
		                                         CDesC8Array* aHeaders = 0,
		                                         HBufC8* aContentType = 0,
		                                         HBufC8* aContent = 0 ); 
				 
		/**
		* Sends a response to an incoming extension SIP request 
		* such as INFO within an existing session.
		* @pre State() != CMceSession::EIdle &&
		*      State() != CMceSession::EIncoming &&
		*      State() != CMceSession::ETerminated
	    * @param aTransactionId Transaction Identifier
	    * @param aReason, Reason Phrase
	    * @param aCode, Response Code 
	    * @param aHeaders, SIP headers to be added. 
	    *        The ownership is transferred.
	    * @param aContentType, content type of the body.
		*		 The ownership is transferred.
	    * @param aContent, content to be added in body. 
	    *        The ownership is transferred.
	    */	
		IMPORT_C void SendResponseL( TMceTransactionId aTransactionId,
		                             const TDesC8& aReason,
		                             TUint32 aCode,	
		                             CDesC8Array* aHeaders = 0,
		                             HBufC8* aContentType = 0,
		                             HBufC8* aContent = 0 );

	    /**
	    * Adds media stream into session.
		* @pre State() == CMceSession::EIdle ||
		*      State() == CMceSession::EIncoming ||
		*      State() == CMceSession::EEstablished
		* @pre aMediaStream != 0
	    * @param aMediaStream, media stream to be added.
	    *		 ownership is transferred.
	    */
	    virtual void AddStreamL( CMceMediaStream* aMediaStream );

	    /**
	    * Returns the type of the session.
	    * @return The type of the session.
	    */
	    virtual TMceSessionType Type() const;


	public: //event handling interface

	    /**
	    * Handles reception of session update
	    * @param aId id of object whose state is needed to be updated
	    */
	    void EventReceivedL( TMceIds& aId );

	    /**
	    * Handles reception of session update
	    * @param aId id of object whose state is needed to be updated
	    * @param aMessage the message
	    */
	    void EventReceivedL( TMceIds& aId, CMceMsgBase& aMessage );

	    /**
	    * Handles reception of session update
	    * @param aId id of object whose state is needed to be updated
	    * @param aMessage the message
	    * @param aContent the content
	    */
	    void EventReceivedL( TMceIds& aId, CMceMsgBase& aMessage, HBufC8* aContent );

	    /**
	    * Handles reception of session update
	    * @param aInSession the incoming update
	    * @param aContent the content
	    */
        void IncomingUpdateL( CMceInSession* aInSession, HBufC8* aContent );

	    /**
	    * Floor control message handling
	    * @param aMessage FC message
	    */
	    void FCMessageReceived( HBufC8* aMessage );
		
		/**
	    * Floor control error handling
	    * @param aError FC error
	    */
		void FCErrorOccurred( TInt aError );
		
	    /**
	    * Handling of error
	    * @param aError the error
	    */
	    void ErrorOccuredL( TInt aError );
	    
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

    public: //internal serialization
    	    
	    /**
	    * Factory
	    * @return factory
	    */
	    TMceFactory BaseFactory();
	    
	    /**
	    * Factory
	    * @return factory
	    */
	    static TMceFactory Factory();
	    
	public: //internal

	    /**
	    * ID 
	    * @return ID
	    */
		TUint32 Id() const;
		
	    /**
	    * Dialog ID 
	    * @return Dialog ID 
	    */
		TUint32 DialogId() const;
		
	    /**
	    * Returns SIP params and gives ownership of them to caller.
	    * After call, SIP params are destroyed from session
	    * @param aHeaders SIP headers
	    * @param aContentType SIP content type
	    * @param aContentHeaders SIP content headers
	    */
		void GetSIPParams( CDesC8Array*& aHeaders, 
		                   HBufC8*& aContentType, 
		                   CDesC8Array*& aContentHeaders );

	    /**
	    * Sets SIP params 
	    * @param aHeaders SIP headers
	    * @param aContentType SIP content type
	    * @param aContentHeaders SIP content headers
	    */
	    void SetSIPParams( CDesC8Array* aHeaders, 
	                       HBufC8* aContentType,
	                       CDesC8Array* aContentHeaders );
	    
	    /**
	    * Handles SIP parameter cleanup in leave situations.
	    * @param aAny this session
	    */
	    static void SIPParamsCleanup( TAny* aSession );
	    
	    /**
	    * Gets sender for ITC.
	    * @return ITC sender
	    */
	    CMceItcSender& ITCSender() const;

	    /**
	    * Gets manager
	    * @return manager
	    */
	    CMceManager& Manager() const;

	    /**
	    * Sets observer for FC.
	    * @return ITC sender
	    */
	    void SetFCObserver(MMceFcMsgObserver* aPtr);

	    /**
	    * Prepares ids for ITC
	    * @param aId ITC ids
	    */
	    void PrepareForITC( TMceIds& aId );
	    
	    /**
	    * Sets this instance as zombie
	    * @param aInSession the incoming update
	    */
        void SetZombie( CMceInSession& aInSession );
        
	    /**
	    * Return ETrue, if session is zombie by update
	    * @return ETrue, if session is zombie by update
	    */
	    TBool IsZombie();

	protected:

	    /**
	     * C++ default constructor.
	     * @param aManager, reference to session manager.
	     * @param aProfileId, id of the sip profile used for the session.
	     */
		CMceSession( CMceManager* aManager, TUint32 aProfileId );

	    /**
	     * second-phase constructor
	     */
		void ConstructL( TBool aUpdateSession = EFalse );

	    /**
	    * Initializes 
	    */
	    void InitializeL( TBool aClientSideMedia = ETrue, 
	                      TBool aDiscardUnusedCodecs = EFalse );
	    
	    /**
	     * Sends an ITC event
	     * @param aITCFunction ITC operation	     
	     */
		void SendITCEventL( TUint8 aITCFunction );
	    
	    /**
	     * Synchronizes session data with server
	     * @param aIds ITC ids	     
	     */
        void SynchronizeL( TMceIds& aIds );
        
	    /**
	     * Adds stream to media streams list
	     * @param aMediaStream media stream
	     */
	    void DoAddStreamL( CMceMediaStream* aMediaStream );

		/**
	     * Checks if the stream should be initialized.
	     * @param aMediaStream media stream
	     */
	    virtual TBool StreamInitializeCondition(
	    	CMceMediaStream& aMediaStream ) const = 0;

	private://event handling & update

	    /**
	    * Traversal event handler
	    * @param aEvent the event
	    */
	    void EventReceivedL( TMceEvent& aEvent );
	    
	    /**
	    * Called after update
	    */
	    void Updated();
	    
	    /**
	    * Called after update has failed
	    */
	    void UpdateFailed( TMceEvent& aEvent, 
						   TMceTransactionDataContainer* aContainer );
	    
	    /**
	    * Updates whole session object witn new one
	    * @param aSession new session
	    */
	    void UpdateL( CMceSession& aSession );

	    /**
	    * Handles event
	    * @param aEvent the event
	    */
	    void HandleEvent( TMceEvent& aEvent );
	    

		/**
	    * Handles Request Event within session
	    * @param aEvent the event
	    */
	    void HandleEventRequestReceivedL( TMceEvent& aEvent );


		/**
	    * Handles Responses Event within session
	    * @param aEvent the event
	    */
	    void HandleEventResponseReceivedL( TMceEvent& aEvent );

	    
	    
	    /**
	    * Sets container based on response
	    * @param aReply the data for container
	    * @param aContent a content
	    * @return ETrue if content was deleted, otherwise EFalse
	    */
        TBool SetContainer( CMceMsgSIPReply& aReply, HBufC8* aContent );

	    /**
	    * Sets container based on request
	    * @param aRequest the data for container
	    * @param aContent a content
	    * @return ETrue if content was deleted, otherwise EFalse
	    */
        TBool SetContainer( CMceMsgSIPData& aRequest, HBufC8* aContent );

	    /**
	    * Sets container based on response & handles session update
	    * @param aMsgArray the data for container
	    * @param aContent a content
	    */
        void SetContainer( CMceMsgArray<TMceFactory>& aMsgArray, HBufC8* aContent );
        
        /**
	    * Sends the Update
	    * @param aContent
	    */
        void SendUpdateL( HBufC8* aContent );
	    


	protected: // Not owned data

	    /**
	     * manager
	     */
	     CMceManager* iManager;

	    /**
	     * profile id
	     */
	     TUint32 iProfileId;

	protected: // owned data

	    /**
	     * flat data container
	     */
		CMceComSession* iFlatData;

	    /**
	     * sender
	     */
	    CMceItcSender* iSender;
	    
	    /**
	     * receiver
	     */
		CMceSessionReceiver* iReceiver;
	    
	    /**
	     * FC observer
	     */
		MMceFcMsgObserver* iFCObserver;
		
		/**
	     * media streams
	     */
		RPointerArray<CMceMediaStream> iMediaStreams;
		 
	private: // Owned data

		
	    /**
	     * bundles
	     */
		RPointerArray<CMceStreamBundle> iBundles;

    private: // Reserved for future use
    
        TAny* iReserved;       

		//for testing

		friend class CMceManager;
		friend class CMceFcMsgExchange;
		
	    MCE_UNIT_TEST_DEFS
	        
	};

#endif // CMCESESSION_H
