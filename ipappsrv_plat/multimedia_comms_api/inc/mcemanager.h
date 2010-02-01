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




#ifndef CMCEMANAGER_H
#define CMCEMANAGER_H

// INCLUDES
#include <e32base.h> 
#include <e32std.h>  
#include <badesca.h>  
#include <mcedefs.h>

// FORWARD DECLARATIONS
class MMceInSessionObserver;
class MMceInEventObserver;
class MMceInReferObserver;
class MMceSessionObserver;
class MMceEventObserver;
class MMceReferObserver;
class MMceTransactionObserver; 
class MMceInTransactionObserver; 
class MMceStreamObserver;
class MMceRtpObserver;
class MMceDtmfObserver;
class CMceSession;
class CMceInSession;
class CMceEvent;
class CMceRefer;
class CMceManagerReceiver;
class CMceEventManagerReceiver;
class RMce;
class MMceItc;
class TMceMediaId;
class CMceAudioCodec;
class CMceVideoCodec;
class CMceItcSender;
class TMceTransactionDataContainer;
class CMceFileSource;
class TMceIds;

// CLASS DECLARATION

/**
*  Class for connecting to MCE server.
*
*  This class provides the connection to the MCE server and
*  provides functions for setting observers for asynchronous
*  events.
* 
*  The user should create only one instance of this class.
*
* @lib mceclient.lib
*/
class CMceManager : public CBase
	{

	public: // Constructors and destructor

	    /**
	    * Two-phased constructor.
	    * @param aAppUid Identification of the application.
	    * @param aContainer If set, detailed data provided in observers.
	    *        Ownership is not transferred, instance must not be deleted
	    *        in lifetime of CMceManager.       
	    */
		IMPORT_C static CMceManager* NewL(
	                            	const TUid& aAppUid, 
	                            	TMceTransactionDataContainer* aContainer = 0);

	    /**
	    * Destructor.
	    */
		IMPORT_C ~CMceManager();

	public: // Functions

	    /**
	    * Set observer for incoming sessions.
	    * @param aInSessionObserver Observer of the sessions. Ownership is
		*	     not transferred. May be NULL to remove observer. 
	    */
		IMPORT_C void SetInSessionObserver( 
		             MMceInSessionObserver* aInSessionObserver );
		             
	    /**
	    * Set observer for incoming events.
	    * @param aInEventObserver Observer of the events. Ownership is
		*	     not transferred. May be NULL to remove observer. 
	    */
		IMPORT_C void SetInEventObserver( 
		             MMceInEventObserver* aInEventObserver );
		             
	    /**
	    * Set observer for incoming refers.
	    * @param aInReferObserver Observer of the refers. Ownership is
		*	     not transferred. May be NULL to remove observer.
	    */
		IMPORT_C void SetInReferObserver( 
		             MMceInReferObserver* aInReferObserver );
		             
	    /**
	    * Set observer for sessions.
	    * @param aSessionObserver Observer of the sessions. Ownership is
		*	     not transferred. May be NULL to remove observer.
	    */
		IMPORT_C void SetSessionObserver( 
		             MMceSessionObserver* aSessionObserver );

	    /**
	    * Set observer for medias
	    * @param aMediaObserver Observer of medias. Ownership is
		*	     not transferred. May be NULL to remove observer.
	    */
		IMPORT_C void SetMediaObserver( 
		             MMceStreamObserver* aMediaObserver );

	    /**
	    * Set observer for rtp
	    * @param aRtpObserver Observer of rtp. Ownership is
		*	     not transferred. May be NULL to remove observer.
	    */
		IMPORT_C void SetRtpObserver( 
		             MMceRtpObserver* aRtpObserver );

        /**
        * Sets observer for incoming events.
        * @param aEventObserver Observer of the event. Ownership is
		*	     not transferred. May be NULL to remove observer. 
        */
		IMPORT_C void SetEventObserver(
					 MMceEventObserver* aEventObserver );
					 
        /**
        * Sets observer for incoming REFER events.
        * @param aReferObserver Observer of the event. Ownership is
		*	     not transferred. May be NULL to remove observer.
        */
		IMPORT_C void SetReferObserver(
					 MMceReferObserver* aReferObserver );
		
        /**
        * Sets observer for DTMF events.
        * @param aDtmfObserver, observer of the event. Ownership is
		*	     not transferred. May be NULL to remove observer.
        */
		IMPORT_C void SetDtmfObserver(
					 MMceDtmfObserver* aDtmfObserver );

        /**
        * Sets observer for incoming SIP responses 
        * to outgoing in-dialog requests.
        * @param aTransactionObserver, observer of the event. Ownership is
		*	     not transferred. May be NULL to remove observer.
        */
		IMPORT_C void SetTransactionObserver(
					 MMceTransactionObserver* aTransactionObserver );
        /**
        * Sets observer for incoming SIP extension requests.
        * @param aTransactionObserver, observer of the event. Ownership is
		*	     not transferred. May be NULL to remove observer.
        */
		IMPORT_C void SetInTransactionObserver(
					 MMceInTransactionObserver* aInTransactionObserver );

	    /**
	    * Gets currently ongoing sessions
	    * @return RPointerArray array of sessions.
	    */
		IMPORT_C const RPointerArray< CMceSession >& Sessions() const;

        /**
        * Gets currently ongoing events
        * @return RPointerArray array of events.
        */
		IMPORT_C const RPointerArray< CMceEvent >& Events() const;

        /**
        * Gets currently ongoing refers
        * @return RPointerArray array of refers.
        */
		IMPORT_C const RPointerArray< CMceRefer >& Refers() const;
						
	    /**
	    * Returns supported audio codecs of the system.
	    * @return supported audio codecs of the system.
	    */
	    IMPORT_C const RPointerArray<const CMceAudioCodec>& SupportedAudioCodecs() const;

	    /**
	    * Returns supported video codecs of the system.
	    * @return supported video codecs of the system.
	    */
	    IMPORT_C const RPointerArray<const CMceVideoCodec>& SupportedVideoCodecs() const;
	    
	    /*
		* Set default headers for the method. The headers will always be added
		* to the given SIP methods unless overwritten with
		* headers given as parameter for the API method triggering the 
		* sending of request/response.
		* @param aMethods The SIP methods the headers are added to.
		* @param aHeaders The default headers, ownership is transferred.
		*/
		IMPORT_C void SetDefaultHeadersL( CDesC8Array* aMethods, CDesC8Array* aHeaders );
        
        /*
        * Get the default headers assigned for the given method.
        * If none is assigned, and empty array is returned.
        * @param aMethod The method for which the headers are queried.
        * @return the default headers, ownership is transfered.
        */	    
	    IMPORT_C CDesC8Array* DefaultHeadersL( const TDesC8& aMethod ) const;

	    
	public: // Functions

	    /**
	    * Gets observer for incoming sessions
	    * @return incoming session observer
	    */
	    MMceInSessionObserver* InSessionObserver() const;
	    
	    /**
	    * Gets observer for incoming events
	    * @return incoming event observer
	    */
	    MMceInEventObserver* InEventObserver() const;
	    
	    /**
	    * Gets observer for incoming refers
	    * @return incoming refer observer
	    */
	    MMceInReferObserver* InReferObserver() const;
	    
	    /**
	    * Gets session observer
	    * @return session observer
	    */
	    MMceSessionObserver* SessionObserver() const;

	    /**
	    * Gets media observer
	    * @return media observer
	    */
	    MMceStreamObserver* MediaObserver() const;

	    /**
	    * Gets rtp observer
	    * @return rtp observer
	    */
	    MMceRtpObserver* RtpObserver() const;
	    
        /**
        * Gets event observer
        * @return event observer
        */
        MMceEventObserver* EventObserver() const;
        
        /**
        * Gets refer observer
        * @return event observer
        */
        MMceReferObserver* ReferObserver() const;
                
        /**
        * Gets transaction observer 
        * @return event observer
        */
        MMceTransactionObserver* TransactionObserver() const;
                
                
		/**
        * Gets transaction observer 
        * @return event observer
        */
        MMceInTransactionObserver* InTransactionObserver() const;
                           
                
        
        /**
        * Gets dtmf observer
        * @return dtmf observer
        */
        MMceDtmfObserver* DtmfObserver() const;
                
	    /**
	    * Returns server sessiom
	    * @return server sessiom
	    */
	    RMce& ServerSession() const;

	    /**
	    * Returns app uid
	    * @return app uid
	    */
	    TUid AppUid() const;

	    /**
	    * Returns next id
	    * @return next id
	    */
	    TMceMediaId NextMediaId();
	    	    
        /**
        * Finds a refer instance for an event. 
        * @param aEvent The event
	    * @return NULL, if none exists.
	    */
        CMceRefer* FindRefer( const CMceEvent& aEvent );
        
        /**
        * Returs transaction container for detailed data. 
	    * @return NULL, if none exists.
        */
		TMceTransactionDataContainer* TransactionContainer() const;        
	    
        /**
        * Starts transcoding of file
        * @param aFile The file
        * @return transcoding session id
        */
	    TUint32 TranscodeL( CMceFileSource& aFile );
	    
        /**
        * Cancels transcoding
        * @param aFile The file
        * @param aSessionId the tc session id
        */
    	void CancelTranscodeL( const CMceFileSource& aFile, TUint32 aSessionId );

        /**
        * Called when trancoding is completed
        * @param aFile The file
        */
    	void TranscodeCompleted( const CMceFileSource& aFile );
    	
        /**
        * handles incoming event
        * @param aIds The ids
        */
        void HandleIncomingEventL( TMceIds& aIds );
        
        /**
        * Finds associated session
        * @param aDialogId,
        * @return Session or NULL if not found, ownership is not transferred
        */
        CMceSession* FindAssociatedSession( TUint32 aDialogId ) const;
	    
	private:

	    /**
	    * C++ default constructor.
	    * @param aAppUid, uid of the application.
	    * @param aContainer, if set, detailed data provided in observers.
	    */
	    CMceManager( const TUid& aAppUid, 
	                 TMceTransactionDataContainer* aContainer = 0);

	    /**
	     * By default Symbian 2nd phase constructor is private.
	     */
	    void ConstructL();	

	    /**
	     * Receives supported audio codecs
	     */
	    void ReceiveSupportedAudioCodecsL();

	    /**
	     * Receives supported video codecs
	     */
	    void ReceiveSupportedVideoCodecsL();
	    
	    /**
	    * Session registers itself to manager
	    * @param aSession the session
	    * @param aParams parameter list for dialog creation
	    * @param aDialogId in/out dialog id
	    * @param aConnectionState in/out connection state
	    * @return session id
	    */
	    TUint32 RegisterSessionL( CMceSession* aSession,
                                  CDesC8Array& aParams,
                                  TUint32& aDialogId,
                                  TBool& aConnectionState );

                                  
	    /**
	    * New session registers itself and deregisters its old
	    * (zombie) instance.
	    * @param aNewSession the new session
	    * @param aOldSession the old session
	    */
        void IncomingSessionUpdateL( CMceInSession& aNewSession, 
                                     CMceSession& aOldSession );
                                  
	    
	    /**
	    * Session unregisters itself to manager
	    * @param aSession the session
	    */
	    void UnregisterSession( CMceSession* aSession );

	    /**
	    * Removes session
	    * @param aSession the session
	    */
        void RemoveSession( CMceSession* aSession );
        
        /**
        * Event registers itself to  event manager
        * @param aEvent the event
	    * @param aParams parameter list for dialog creation
	    * @param aDialogId in/out dialog id
	    * @param aDialogType the dialog type
        * @return event id
        */
        TUint32 RegisterEventL( CMceEvent* aEvent,
                                CDesC8Array& aParams,
                                TUint32& aDialogId,
                                TUint32 aDialogType );
    
        /**
        * Refer registers itself to  event manager
        * @param aRefer the refer
	    * @param aParams parameter list for dialog creation
	    * @param aDialogId in/out dialog id
        * @return event id
        */
        TUint32 RegisterReferL( CMceRefer* aRefer,
                                CDesC8Array& aParams,
                                TUint32& aDialogId );
    
        /**
        * Event unregisters itself to event manager
        * @param aEvent the event
        */
        void UnregisterEvent( CMceEvent* aEvent );

        /**
        * Refer unregisters itself to event manager
        * @param aRefer the refer
        */
        void UnregisterRefer( CMceRefer* aRefer );
        
        /**
        * prepares for ITC
        * @param aRole role as sip session manager or event manager
        */
        void PrepareForITC( TMceIds& aId, TUint aRole ) const;

	    /**
	     * Adds session
	     * @param aSession the session
	     */
	    void AddSessionL( CMceSession* aSession );

	    /**
	     * Adds event
	     * @param aEvent the event
	     */
	    void AddEventL( CMceEvent* aEvent );

	    /**
	     * Adds refer
	     * @param aRefer the refer
	     */
	    void AddReferL( CMceRefer* aRefer );
	    
        
	private: // Owned data

	    /**
	     * detailed callbacks wanted
	     */
	    TBool iDetailedCallbacks;

        /**
        * Transaction data container.
        */
		TMceTransactionDataContainer* iContainer;        
		
	    /**
	     * sessions
	     */
	    RPointerArray<CMceSession> iSessions;
	    
	    /**
        * events
        */
        RPointerArray< CMceEvent > iEvents;
        
        /**
        * refers
        */
        RPointerArray< CMceRefer > iRefers;
        
	    /**
	     * supported audio codecs
	     */
	    RPointerArray<const CMceAudioCodec> iSupportedAudioCodecs;

	    /**
	     * supported video codecs
	     */
	    RPointerArray<const CMceVideoCodec> iSupportedVideoCodecs;
	    
	    /**
	     * manager receiver for sessions
	     */
	    CMceManagerReceiver* iSessionReceiver;
	    
	    /**
	     * manager receiver for events
	     */
	    CMceEventManagerReceiver* iEventReceiver;
	    
	    /**
	     * server client session
	     */
	    RMce* iServerSession;
	    
	    /**
	     * sender
	     */
	    CMceItcSender* iSender;
	    
	    /**
	     * next id
	     */
	    TUint32 iNextId;
	    
	    /**
	     * currently transcoding files
	     */
	    RPointerArray<CMceFileSource> iTranscodingFiles;

	    
	private: // NOT Owned data

	    /**
	     * application UID
	     */
	    TUid iAppUid;
	    
	    /**
	     * manager observer for incoming sessions
	     */
	    MMceInSessionObserver* iInSessionObserver;

	    /**
	     * manager observer for incoming events
	     */
	    MMceInEventObserver* iInEventObserver;
	    
	    /**
	     * manager observer for incoming refers
	     */
	    MMceInReferObserver* iInReferObserver;
	    
	    /**
	     * session observer
	     */
	    MMceSessionObserver* iSessionObserver;

	    /**
	     * media observer
	     */
	    MMceStreamObserver* iMediaObserver;

	    /**
	     * rtp observer
	     */
	    MMceRtpObserver* iRtpObserver;

        /**
        * Event observer.
        */
        MMceEventObserver* iEventObserver;
        
        /**
        * Refer observer.
        */
        MMceReferObserver* iReferObserver;
        
        
        /**
        * MCE Transaction observer
        */
        MMceTransactionObserver* iMceTransactionObserver;
        
        
        /**
        * MCE In Transaction observer
        */
        MMceInTransactionObserver* iMceInTransactionObserver;

        
        /**
        * Dtmf observer.
        */
        MMceDtmfObserver* iDtmfObserver;
                	    
    private: // Reserved for future use
    
        TAny* iReserved;       

	//for testing

	    MCE_UNIT_TEST_DEFS
    
        friend class CMceEvent;
        friend class CMceRefer;
	    friend class CMceSession;
	};

#endif 
