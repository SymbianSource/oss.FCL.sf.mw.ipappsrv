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




#ifndef CMCEINEVENT_H
#define CMCEINEVENT_H

// INCLUDES
#include <mceevent.h>

// FORWARD DECLARATIONS
class CMceManager;
class CMceSession;
class CMceMsgSIPEvent;

// CLASS DECLARATION

/**
* Class for mobile terminated MCE events.
*
* CMceOutEvent represents incoming request to from terminal to establish event dialog.
*
* After the event has been succesfylly set up, it can be later updated and
* must be finally terminated. Notifications about the event are send using NotifyL
* function. The respond to this in not passed to user, unless there is failure.
*
* If for any reason instance of this class is deleted while in active state,
* event is automatically terminated by MCE server.
*
*  @lib mceclient.lib
*/
class CMceInEvent:public CMceEvent
    {
	public:  // Constructors and destructor
	       
	    /**
	    * Destructor.
	    */
	    IMPORT_C ~CMceInEvent();

	public: // New functions
	       
	    /**
	    * Sends accept respond to an incoming event.
	    * @pre State() == CMceEvent::EIdle
	    * @post State() == CMceEvent::EPending	    
	    */
	    IMPORT_C void AcceptL();
		
	    /**
	    * Sends reject respond to an incoming event.
	    * @pre State() == CMceEvent::EIdle
	    * @post State() == CMCEvent::ETerminated	    
	    */
	    IMPORT_C void RejectL();

	    /**
	    * Sends respond to an incoming event.
	    * @pre State() == CMceEvent::EIdle
	    * @post State() == CMCEvent::ETerminated ||	    
	    *       State() == CMCEvent::EPending	    
	    * @param aReason, reason phrase
	    * @param aCode, reason code
	    * @param aHeaders, SIP headers to be added into respond. Ownership is
		*		 transferred.
		* @param aContentType, a type of content. Ownership
		*		 is transferred.
	    * @param aContent, content to be added in body. Ownership is 
		*		 transferred.
	    */
	    IMPORT_C void RespondL(
					 const TDesC8& aReason,
	                 TUint32 aCode,
	                 CDesC8Array* aHeaders = 0,
					 HBufC8* aContentType = 0,
					 HBufC8* aContent = 0 ); 

	    /**
	    * Sends notification related to an event.
	    * @pre State() == CMceEvent::EIdle
	    * @post State() == CMCEvent::EPending	    
	    * @param aHeaders, SIP headers to be added. Ownership is
		*		 transferred.
	    * @param aContentType, content type of the body.
		*		 Ownership is transferred.
	    * @param aContent, content to be added in a body. Ownership is 
		*		 transferred.
	    */
		IMPORT_C void NotifyL(
					 CDesC8Array* aHeaders = 0, 
					 HBufC8* aContentType = 0,
					 HBufC8* aContent = 0 ); 
					 
	public: 
	
	    /**
	    * Two-phased constructor.
	    */
	    static CMceInEvent* NewL( CMceManager& aManager,
	                              TUint32 aProfileId,
	                              CMceMsgSIPEvent& aEventMsg );
	                              
		TBool IsInEvent() const;

	private: // Functions from base classes
		    
		    /**
	        * C++ default constructor.
	        * @param aManager, reference to event manager.
	        * @param aProfileId, id of the sip profile used for the event.
	        */
		CMceInEvent( CMceManager& aManager, 
		             TUint32 aProfileId,
		             TUint32 aId,
	                 TUint32 aDialogId,
	                 TUint32 aRefreshInterval );
		
	private: 

		    
	    /**
	    * second-phase constructor
	    */
	    void ConstructL( HBufC8* aEventHeader, 
	                     HBufC8* aRecipient, 
	                     HBufC8* aOriginator,
	                     TUint32 aDialogType );

		
	    MCE_UNIT_TEST_DEFS
    };

#endif    
