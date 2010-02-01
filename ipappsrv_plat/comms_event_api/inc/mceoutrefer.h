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




#ifndef CMCEOUTREFER_H
#define CMCEOUTREFER_H

//  INCLUDES
#include <mceoutevent.h>
#include <mcerefer.h>

// FORWARD DECLARATIONS
class CSIPProfile;

// CLASS DECLARATION

/**
* Class for mobile originated MCE refers.
*
* CMceOutRefer represents outgoing request to remote terminal to perform an action.
* This request may or may not also implicitically generate an event. This is
* indicated by type while instantiating the class. If event is generated, instance
* of CMceOutEvent is returned in ReferL function call. This will lead to notifications
* send by remote terminal received through MMceEventObserver::NotifyReceived() function.
*
* The refer is finally terminated by the remote endpoint.
*
* If for any reason instance of this class is deleted while in active state,
* refer is automatically terminated by MCE server.
*
*  @lib mceclient.lib
*/
class CMceOutRefer: public CMceRefer
    {
    public:  // Constructors and destructor
        
		/**
        * Two-phased constructor.
		* @param aManager, reference to event manager
        * @param aProfile, reference to used SIP profile
	    * @param aRecipient, recipient of the refer.
		* @param aReferTo, value placed into refer-to header.
		* @param aType, type of the refer.
	    * @param aOriginator, originator of the refer.
        */
        IMPORT_C static CMceOutRefer* NewL(
									 CMceManager& aManager,
									 CSIPProfile& aProfile,
									 const TDesC8& aRecipient,
					 				 const TDesC8& aReferTo,
									 TType aType = ENoSuppression,
									 HBufC8* aOriginator = 0 );

        /**
	    * Two-phased constructor. Should be used when refer is associated
	    * to existing event. This guarantees that refer is sent
	    * to same remote terminal with the event.
		* @param aEvent,  reference to bound event.
		* @param aReferTo, value placed into refer-to header.
		* @param aType, type of the refer.
        */
        IMPORT_C static CMceOutRefer* NewL(
									 CMceEvent& aEvent,
					 				 const TDesC8& aReferTo,
									 TType aType = ENoSuppression );

        /**
	    * Two-phased constructor. Should be used when refer is associated
	    * to existing session. This guarantees that refer is sent
	    * to same remote terminal with the session.
		* @param aSession, reference to bound session
		* @param aReferTo, value placed into refer-to header.
		* @param aType, type of the refer.
        */
        IMPORT_C static CMceOutRefer* NewL(
								 	 CMceSession& aSession,
					 				 const TDesC8& aReferTo,
									 TType aType = ENoSuppression );

        /**
	    * Two-phased constructor. Should be used when refer is associated
	    * to existing refer. This guarantees that refers are sent
	    * to same remote terminal.
		* @param aRefer, reference to bound refer
		* @param aReferTo, value placed into refer-to header.
		* @param aType, type of the refer.
        */
        IMPORT_C static CMceOutRefer* NewL(
								 	 CMceRefer& aRefer,
					 				 const TDesC8& aReferTo,
									 TType aType = ENoSuppression );
									 
        /**
        * Destructor.
        */
        IMPORT_C ~CMceOutRefer();


    public: // New functions
              
		/**
		* Refers to ongoing session.
	    * @pre State() == CMceRefer::EIdle
	    * @post State() == CMCRefer::EPending	    
		* @param aHeaders, SIP headers to be added. Ownership is
		*		 transferred.
		* @param aContentType, content type of the body. Ownership is
		*		 transferred.
		* @param aContent, content to be added in a body. Ownership is transferred.
	    */
		IMPORT_C CMceOutEvent* ReferL(
					 CDesC8Array* aHeaders = 0, 
					 HBufC8* aContentType = 0,
					 HBufC8* aContent = 0 ); 
					 
    public: // Functions from base class
        
        /**
        * Returns truth value, is the refer in-refer or not.
        * @return TBool, EFalse.
        */
        TBool IsInRefer() const;
  	
    private: 
	    
	    /**
        * C++ default constructor.
        * @param aManager, reference to event manager.
        * @param aProfileId, id of the sip profile used for the event,
        * @param aDialogId, the ID of the dialog this REFER
        *                   is created into
        * @param aType
        */
	    CMceOutRefer( CMceManager& aManager, 
                      TUint32 aProfileId,
                      TUint32 aDialogId,
                      TType aType );
	   
	    /**
        * second-phase constructor
        */
	    void ConstructL(const TDesC8& aRecipient,
            	        const TDesC8& aReferTo,
            	        TType aType,
            	        HBufC8* aOriginator );
	    /**
        * second-phase constructor
        */
	    void ConstructL(const TDesC8& aRecipient,
            	        const TDesC8& aReferTo,
            	        TType aType,
            	        const TDesC8& aOriginator );

    };

#endif   
