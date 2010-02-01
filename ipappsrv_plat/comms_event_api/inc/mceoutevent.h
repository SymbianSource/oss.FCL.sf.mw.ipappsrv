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




#ifndef CMCEOUTEVENT_H
#define CMCEOUTEVENT_H

// INCLUDES
#include <mceevent.h>

// FORWARD DECLARATIONS
class CSIPProfile;
class CMceManager;
class CMceSession;
class CMceRefer;

// CLASS DECLARATION

/**
* Class for mobile originated MCE events.
*
* CMceOutEvent represents outgoing request to remote terminal to establish event dialog.
*
* After the event has been succesfylly set up, it can be later updated and
* must be finally terminated.
*
* The notification send by remote terminal are passed through
* MMceEventObserver::NotifyReceived(). the responses to these notifications are
* send by MCE server.
*
* If for any reason instance of this class is deleted while in active state,
* event is automatically terminated by MCE server.
*
*  @lib mceclient.lib
*/
class CMceOutEvent:public CMceEvent
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
		* @param aManager, reference to event manager
		* @param aProfile, reference to used SIP profile
	    * @param aRecipient, recipient of the event.
        * @param aEventHeader, Event header
        * @param aRefreshInterval, a resfresh interval
	    * @param aOriginator, originator of the event.
        */
        IMPORT_C static CMceOutEvent* NewL(
									 CMceManager& aManager,
									 CSIPProfile& aProfile,
									 const TDesC8& aRecipient,
									 const TDesC8& aEventHeader,
					                 TUint32 aRefreshInterval,
									 HBufC8* aOriginator = 0 );

        /**
	    * Two-phased constructor. Should be used when event is associated
	    * to existing session. This guarantees that event is sent
	    * to same remote terminal with the session.
		* @param aSession, session to be bound to this event
		* @param aEventHeader Event header
		* @param aRefreshInterval a resfresh interval
        */
        IMPORT_C static CMceOutEvent* NewL(
        							 CMceSession& aSession,
									 const TDesC8& aEventHeader,
					                 TUint32 aRefreshInterval );
				
        /**
	    * Two-phased constructor. Should be used when event is associated
	    * to existing event. This guarantees that events sre sent
	    * to same remote terminal.
		* @param aEvent, event to be bound to this event
		* @param aEventHeader Event header
		* @param aRefreshInterval a resfresh interval
        */
        IMPORT_C static CMceOutEvent* NewL(
        							 CMceEvent& aEvent,
									 const TDesC8& aEventHeader,
					                 TUint32 aRefreshInterval );
        
        /**
	    * Two-phased constructor. Should be used when event is associated
	    * to existing refer. This guarantees that event is sent
	    * to same remote terminal with the refer.
		* @param aRefer, refer to be bound to this event
		* @param aEventHeader Event header
		* @param aRefreshInterval a resfresh interval
        */
        IMPORT_C static CMceOutEvent* NewL( 
        							 CMceRefer& aRefer,
									 const TDesC8& aEventHeader,
					                 TUint32 aRefreshInterval );

        /**
        * Destructor.
        */
        IMPORT_C ~CMceOutEvent();
        
    public: // New functions
              
        /**
        * Subscribe to an event.
	    * @pre State() == CMceEvent::EIdle
	    * @post State() == CMceEvent::EPending	    
		* @param aHeaders, headers of received message. Ownership is
		*		 transferred.
		* @param aContentType, a type of received content. Ownership
		*		 is transferred.
	    * @param aContent, content to be added in body. Ownership is 
		*		 transferred.
	    */
        IMPORT_C void SubscribeL(
					 CDesC8Array* aHeaders = 0,
					 HBufC8* aContentType = 0,
					 HBufC8* aContent = 0 ); 
        
        /**
        * Updates the subscription.
	    * @pre State() == CMceEvent::EActive
	    * @post State() == CMceEvent::EPending	    
        * @param aRefreshInterval, new resfresh interval
		* @param aHeaders, headers of received message. Ownership is
		*		 transferred.
		* @param aContentType, a type of received content. Ownership
		*		 is transferred.
	    * @param aContent, content to be added in body. Ownership is 
		*		 transferred.
	    */
        IMPORT_C void UpdateL( 
                     TUint32 aRefreshInterval,   
					 CDesC8Array* aHeaders = 0,
					 HBufC8* aContentType = 0,
					 HBufC8* aContent = 0 ); 
		
    public: // New functions
    
		/**
        * Two-phased constructor for internal use.
		* @param aManager, reference to event manager
		* @param aProfileId, id of the used SIP profile
	    * @param aRecipient, recipient of the event.
        * @param aEventHeader, Event header
        * @param aRefreshInterval, a resfresh interval
	    * @param aOriginator, originator of the event.
	    * @param aDialogType, the dialog type
	    * @param aParams, array of parameters used for subsession creation,
	    *                 if NULL, the event specific parameters are created
        */
        static CMceOutEvent* NewL( CMceManager& aManager,
								   TUint32 aProfileId,
								   const TDesC8& aRecipient,
								   const TDesC8& aEventHeader,
					               TUint32 aRefreshInterval,
								   HBufC8* aOriginator,
								   TUint32 aDialogId,
								   TUint32 aDialogType,
								   CDesC8Array* aParams );



    public: // New functions
    
        /**
        * Gets the type of the event
        * @return ETrue in InEvent
        */
		TBool IsInEvent() const;

	private:
	
	    /**
        * C++ default constructor.
        * @param aManager, reference to event manager.
        * @param aProfileId, id of the sip profile used for the event.
        * @param aDialogId, 
        * @param aRefreshInterval, 
        */
        CMceOutEvent( 
                   CMceManager& aManager, 
                   TUint32 aProfileId,
                   TUint32 aDialogId,
                   TUint32 aRefreshInterval );	
	    
		
		/**
        * second-phase constructor
        */
	    void ConstructL( const TDesC8& aEventHeader, 
	                     const TDesC8& aRecipient, 
                         HBufC8* aOriginator,
                         TUint32 aDialogType,
                         CDesC8Array* aParams );

		/**
        * second-phase constructor
        */
	    void ConstructL( const TDesC8& aEventHeader, 
	                     const TDesC8& aRecipient, 
                         const TDesC8& aOriginator,
                         TUint32 aDialogType );
                         
	    MCE_UNIT_TEST_DEFS
    };

#endif    
