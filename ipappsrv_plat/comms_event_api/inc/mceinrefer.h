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




#ifndef CMCEINREFER_H
#define CMCEINREFER_H

// INCLUDES
#include <mcerefer.h>

// FORWARD DECLARATIONS
class CMceInEvent;
class CMceMsgSIPEvent;

// CLASS DECLARATION

/**
* Class for mobile terminated MCE refers.
*
* CMceInRefer represents incoming request to perform an action. If the remote terminal
* expects notifications about how the action is proceeding, instance of CMceInEvent
* is also passed to the user. User is then responsible of sending notifications and
* finally terminate the event and refer. If notifications are not expected by the
* remote terminal, instance of CMceInEvent is not returned and refer is terminated
* as soon as either accept or reject response is sent. 
*
* This class is never instantiated by user, but the ownership is in any case transferred
* to it. See class MMceInReferObserver.
*
* If for any reason instance of this class is deleted while in accepted state,
* it is automatically terminated by MCE server.
*
*  @lib mceclient.lib
*/
class CMceInRefer: public CMceRefer
    {
    public:  // Constructors and destructor
               
        /**
        * Destructor.
        */
        IMPORT_C ~CMceInRefer();

    public: // New functions
              
	    /**
	    * Sends accept respond to an incoming refer.
	    * @pre State() == CMceRefer::EIdle
	    * @post State() == CMCRefer::EPending ||
	    *       State() == CMCRefer::EAccepted (for suppressed refers)	    
        * @return New instance of event, if CMceRefer::Type() != CMceRefer::ESuppressed.
	    */
	    IMPORT_C CMceInEvent* AcceptL();
		
	    /**
	    * Rejects incoming refer.
	    * @pre State() == CMceRefer::EIdle
	    * @post State() == CMCRefer::ETerminated	    
	    */
	    IMPORT_C void RejectL();
	    
	    /**
	    * Sends respond to an incoming refer.
	    * @pre State() == CMceRefer::EIdle
	    * @post State() == CMCRefer::ETerminated ||
	    *       State() == CMCRefer::EPending ||	    
	    *       State() == CMCRefer::EAccepted (for suppressed refers)	    
	    * @param aReason, reason phrase
	    * @param aCode, reason code
	    * @param aHeaders, SIP headers to be added into respond. Ownership is
		*		 transferred.
		* @param aContentType, a type of content. Ownership
		*		 is transferred.
	    * @param aContent, content to be added in body. Ownership is 
		*		 transferred.
        * @return New instance of event, if refer was accepted and 
        *         CMceRefer::Type() != CMceRefer::ESuppressed.
	    */
	    IMPORT_C CMceInEvent* RespondL(
					 		 const TDesC8& aReason,
	                 		 TUint32 aCode,
	                 		 CDesC8Array* aHeaders = 0,
					 		 HBufC8* aContentType = 0,
					 		 HBufC8* aContent = 0 ); 
	    
    
    public: // Functions from base class
        
        /**
        * Returns truth value, is the refer in-refer or not.
        * @return TBool, ETrue.
        */
        TBool IsInRefer() const;

        /**
        * Two-phased constructor.
        */
        static CMceInRefer* NewL( CMceManager& aManager,
                                TUint32 aProfileId,
                                CMceMsgSIPEvent& aEventMsg,
                                CMceInEvent* aInEvent = NULL );
        
    
    private:   
        /**
        * C++ default constructor.
        * @param aManager, reference to event manager.
        * @param aProfileId, id of the sip profile used for the event.
        */
    	CMceInRefer( CMceManager& aManager, 
                      TUint32 aProfileId,
                      TUint32 aId,
                      TUint32 aDialogId,
                      TType aType );
		    
	    /**
        * second-phase constructor
        */
	    void ConstructL( HBufC8* aReferTo,
                         HBufC8* aRecipient,
                         HBufC8* aOrginator,
                         CMceInEvent* aInEvent );
    };

#endif    
