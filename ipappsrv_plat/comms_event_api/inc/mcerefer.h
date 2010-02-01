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




#ifndef CMCEREFER_H
#define CMCEREFER_H

// INCLUDES
#include <e32base.h>
#include <mcedefs.h>

// FORWARD DECLARATIONS
class CDesC8Array;
class CMceEvent;
class CMceManager;
class CMceItcSender;
class TMceIds;
class CMceMsgBase;
class TMceEvent;
class CMceReferReceiver;
class CMceSession;

// CLASS DECLARATION

/**
* Base class for MCE refers, inherited by both CMceInRefer and CMceOutRefer.
*
* CMceRefer represents peer-to-peer connection made with remote terminal.
* This connection is also known as dialog and it is set up using SIP REFER
* method.
*
* MCE refers are typically used for asking remote terminal to perform
* some action. One such action is session transfer.
*
* The refer is finally terminated by the endpoint performing the action.
* Refer can implicitically also create an event which is used to inform
* how the action proceeds. In such case refer and event and tied together
* and are finally terminated simultaneously.
*
* If for any reason instance of this class is deleted while in active state,
* event is automatically terminated by MCE server.
*
*  @lib mceclient.lib
*/
class CMceRefer : public CBase
    {
    public: 
        // Refer states
        enum TState
            {
            EIdle,
            EPending, 
            EAccepted,
            ETerminated 
            };
            
        // Refer types
        enum TType
            {
            ENoSuppression,
            ESuppressed,
            EMultipleRefer
            };
           
        
    public: // Constructors and destructors

		/**
		* Destructor.
		*/
		virtual ~CMceRefer();
		
    public: // New functions

		/**
        * Returns the state of the refer.
        * @return TMceReferState, The current state of the refer.
        */
        IMPORT_C TState State() const;

        /**
        * Returns the recipient of the refer.
        * @return the recipient of the refer.
        */
        IMPORT_C const TDesC8& Recipient() const;
        
        /**
        * Returns the originator of the refer.
        * @return the originator of the refer.
        */
        IMPORT_C const TDesC8& Originator() const;
        
        
        /**
        * Returns the ReferTo header.
        * @return the ReferTo header.
        */
        IMPORT_C const TDesC8& ReferTo() const;
        
        /**
        * Returns the refer type
        * @return the Type of the refer.
        */
        IMPORT_C TType Type() const;
        
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

        /**
        * Returns truth value, is the refer in-refer or not.
        * @return TBool, the truth value.
        */
        virtual TBool IsInRefer() const = 0;

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
        CMceItcSender& ITCSender() const;
        
        /**
        * Gets manager
        * @return manager
        */
        CMceManager& Manager() const;
        
        /**
        * Gets the event established by this refer.
        * Ownership is not transfered.
        */ 
        CMceEvent* ReferEvent() const;
        
        /**
        * Detach the refer event
        */
        void DetachReferEvent();
        
        /**
        * Transfers the owned refer event to not event.
        * Called, when the application takes the ownership of
        * refer event.
        * @return the event, ownership is transferd
        */
        CMceEvent* TransferEventOwnership();
    
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
        
        /**
        * The state of the event established by this refer has changed.
        * @param aEvent the ITC event.
        */
        void EventStateChangedL( TMceEvent& aEvent );
        
	protected:  // New functions
	    
	    /**
        * C++ default constructor.
        * @param aManager, reference to event manager.
        * @param aProfileId id of the sip profile used for the even
        * @param aDialogId id of the dialog
        * @param aType type of the refer
        */
        CMceRefer( CMceManager& aManager, 
                   TUint32 aProfileId,
                   TUint32 aDialogId,
                   TType aType );
	    
	    /**
        * Prepares ids for ITC
        * @param aId ITC ids
        */
        void PrepareForITC( TMceIds& aId );
        
        /**
        * second-phase constructor
        */
	    void ConstructL( HBufC8* aRecipient,
            	         HBufC8* aReferTo,
            	         HBufC8* aOriginator,
            	         CDesC8Array* params );
	 
	    /** 
	    * Resolves the proper refer state according to the state
	    * of refer event.
	    * @return state of the refer
	    */
	    TState ResolveStateL();
	 
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
        
        /**
        * Handles state change
        * @param aEvent the event
        */
        void HandleStateChangedL( TMceEvent& aEvent );
        
        
    protected:  // Data

        /**
        * Current event state
        */
        TState iState;
  
        /**
        * Sender, owned
        */
        CMceItcSender* iSender;
        
        /**
        * Event receiver, owned
        */
        CMceReferReceiver* iReceiver;
        
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
        HBufC8* iReferTo;

        /**
        * The recipient
        */
        HBufC8* iRecipient;
        
        /**
        * The originator
        */
        HBufC8* iOriginator;
        
        /**
        * The event established by this REFER, not owned.
        */
        CMceEvent* iEvent; 
        
        /**
        * The event established by this REFER, owned.
        */
        CMceEvent* iOwnedEvent; 
        
        /**
        * Refer type
        */
        TType iType;
        
    private: // Reserved for future use
    
        TAny* iReserved;       
        
	    MCE_UNIT_TEST_DEFS
    };

#endif
