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




#ifndef CMCEEVENT_H
#define CMCEEVENT_H

// INCLUDES
#include <e32base.h>
#include <mcedefs.h>

// FORWARD DECLARATIONS
class CDesC8Array;
class TMceIds;
class CMceItcSender;
class CMceManager;
class CMceEventReceiver;
class CMceMsgBase;
class TMceEvent;
class CMceSession;
class CMceRefer;

// CLASS DECLARATION

/**
* Base class for MCE events, inherited by both CMceInEvent and CMceOutEvent.
*
* CMceEvent represents peer-to-peer connection made with remote terminal.
* This connection is also known as dialog and it is set up using SIP SUBSCRIBE
* method.
*
* MCE events are typically used for queurying state of some parameter of 
* remote terminal. The queuried parameter is known as event header.
* The changes of this parameter are notified to another terminal by sending
* SIP NOTIFY messages.
*
* The event can be finally terminated by either endpoints.
*
* If for any reason instance of this class is deleted while in active state,
* event is automatically terminated by MCE server.
*
*  @lib mceclient.lib
*/
class CMceEvent : public CBase
    {
    public: // Event states
    
        enum TState
            {
            EIdle, 
            EPending,
            EActive,
            ETerminated 
            };
    
    public: // Constructors and destructors

		/**
		* Destructor.
		*/
		virtual ~CMceEvent();
	
    public: // New functions

		/**
        * Terminates the event.
	    * @pre State() == CMceEvent::EActive
	    * @post State() == CMCEvent::EPending
		* @param aHeaders, additional headers of the message. 
		*        The Ownership is transferred.
		* @param aContentType, the type of the message content. 
		*        The ownership is transferred.
	    * @param aContent, the content to be added in the message body. 
	    *        The ownership is transferred.	    	  
	    * @leave KErrNotSupported if event cannot be terminated 
	    *        (e.g. event attached to a CMceOutRefer)
        */
        IMPORT_C void TerminateL(
            CDesC8Array* aHeaders = 0,
            HBufC8* aContentType = 0,
            HBufC8* aContent = 0 );         

        /**
        * Returns the state of the event.
        * @return The current state of the event
        */
        IMPORT_C TState State() const;
        
        /**
        * Returns the address of the recipient
        * @return The the address of the recipient
        */
        IMPORT_C const TDesC8& Recipient() const;
        
        /**
        * Returns the address of the originator
        * @return the address of the Originator
        */
        IMPORT_C const TDesC8& Originator() const;
        
        /**
        * Returns the Event header.
        * @return the Event header
        */
        IMPORT_C const TDesC8& EventHeader() const;

        /**
        * Returns the associated session, if any.
        * @return CMceSession Associated session, NULL if does not exist.
        */
        IMPORT_C CMceSession* AssociatedSession() const;

        /**
        * Returns the associated events, if any.
	    * @return RPointerArray array of associated events, ownership is transferred.
	    */
		IMPORT_C RPointerArray< CMceEvent >* AssociatedEventsL() const;

        /**
        * Returns the associated refers, if any.
	    * @return RPointerArray array of associated refers, ownership is transferred.
	    */
		IMPORT_C RPointerArray< CMceRefer >* AssociatedRefersL() const;
        
	public: // New functions, internal
	    
	    /**
        * ID 
        * @return ID
        */
	    TUint32 Id() const;

	    /**
        * Dialog ID 
        * @return dialog ID
        */
	    TUint32 DialogId() const;
	    
	    /**
        * Gets SIP profile id.
        * @return profile id.
        */
		TUint32 ProfileId() const;
	    
	    /**
        * Gets sender for ITC.
        * @return ITC sender
        */
        CMceItcSender& ITCSender();
        
        /**
        * Gets manager
        * @return manager
        */
        CMceManager& Manager();
        
        /**
        * Gets the type of the event
        * @return ETrue in InEvent
        */
		virtual TBool IsInEvent() const = 0;
		
    
    public: // New functions, event handling 

        /**
        * Handles reception of event update
        * @param aId id of object whose state is needed to be updated
        */
        void EventReceivedL( TMceIds& aId );

        /**
        * Handles reception of event update
        * @param aId id of object whose state is needed to be updated
        * @param aMessage the message
        */
        void EventReceivedL( TMceIds& aId, CMceMsgBase& aMessage );

        /**
        * Handles reception of event update
        * @param aId id of object whose state is needed to be updated
        * @param aMessage the message
        * @param aContent the content
        */
        void EventReceivedL( TMceIds& aId, CMceMsgBase& aMessage, HBufC8* aContent );

        /**
        * Handling of error
        * @param aError the error
        */
        void ErrorOccuredL( TInt aError );
        
	protected:  // New functions
	    
	    /**
        * C++ default constructor.
        * @param aManager, reference to event manager.
        * @param aProfileId, id of the sip profile used for the event.
        * @param aDialogId, 
        * @param aRefreshInterval, 
        */
        CMceEvent( CMceManager& aManager, 
                   TUint32 aProfileId,
                   TUint32 aDialogId,
                   TUint32 aRefreshInterval );	
	    
	    /**
        * Prepares ids for ITC
        * @param aId ITC ids
        */
        void PrepareForITC( TMceIds& aId );
        
        /**
        * second-phase constructor
        * @param aEventHeader
        * @param aRecipient
        * @param aOriginator
        * @param aDialogType
        */
	    void ConstructL( HBufC8* aEventHeader, 
	                     HBufC8* aRecipient,
	                     HBufC8* aOriginator,
	                     TUint32 aDialogType,
	                     CDesC8Array* aParams );
	 
	 
    private: // New functions, event handling and update
    
        /**
        * Traversal event handler
        * @param aEvent the event
        */
        void EventReceivedL( TMceEvent& aEvent );
        
        /**
        * Handles event
        * @param aEvent the event
        */
        void HandleEventL( TMceEvent& aEvent );
        
    protected:  // Data

        // current event state
        TState iState;
        
        /**
        * Sender, owned
        */
        CMceItcSender* iSender;
        
        /**
        * Event receiver, owned
        */
        CMceEventReceiver* iReceiver;
        
        /**
        * Event manager,not owned
        */
        CMceManager& iManager;
        
        /**
        * SIP profile id
        */        
        TUint32 iProfileId;
        
        /**
        * ID
        */
        TUint32 iId;
        
        /**
        * dialog id
        */
	    TUint32 iDialogId;
        
        /**
        * The event header
        */
        HBufC8* iEventHeader;

        /**
        * The recipient
        */
        HBufC8* iRecipient;
        
        /**
        * The originator
        */
        HBufC8* iOriginator;
        
        /**
        * The refresh interval
        */
        TUint32 iRefreshInterval;
        
    private: // Reserved for future use
    
        TAny* iReserved;       

	    MCE_UNIT_TEST_DEFS
        
    };

#endif
