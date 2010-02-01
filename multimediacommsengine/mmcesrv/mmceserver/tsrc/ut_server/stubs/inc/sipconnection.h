/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef CSIPCONNECTION_H
#define CSIPCONNECTION_H

//  INCLUDES
#include <e32base.h>
#include <in_sock.h>

// FORWARD DECLARATIONS
class CSIP;
class MSIPConnectionObserver;
class CSIPClientTransaction;
class MSIPRegistrationContext;
class CSIPRefresh;
class CSIPRequestElements;


// CLASS DECLARATION

/**
*  Class for monitoring connection's state and sending SIP requests outside
*  the dialog associations.Class must not be used
*  -for sending requests to which received non-failure response creates a dialog
*  -for sending requests that must be sent within SIP dialog
* Note that the user can have only one instance of the class per same IAP-id
*/
class CSIPConnection :
    public CBase
	{
    public:
    
        /** Connection states */
        enum TState 
            {
            /** Connection initializing */
			EInit=1,
			/** Connection active */
			EActive,
			/** Connection suspended */
			ESuspended,
			/** Connection is inactive */
			EInactive,
			/** Connection is permanently unavailable */
			EUnavailable
            };

    public:  // Constructors and destructor

	    /**
        * Two-phased constructor.
		* @param aSip a handle to the SIP server
		* @param aIapId IAP-id
		* @param aObserver a observer for receiving asynchonous events on this
        *   connection
        * @return New object, ownership is transferred
        */

		IMPORT_C static CSIPConnection*
            NewL(CSIP& aSIP,
				 TUint32 aIapId,
				 MSIPConnectionObserver& aObserver);


	    /**
        * Two-phased constructor.
		* Constructs an object and adds the pointer to the cleanup stack;
		* @param aSip a handle to the SIP server
		* @param aIapId IAP-id
		* @param aObserver a observer for receiving asynchonous events on this
        *   connection
        * @return New object, ownership is transferred
        */

		IMPORT_C static CSIPConnection*
            NewLC(CSIP& aSIP,
				  TUint32 aIapId,
				  MSIPConnectionObserver& aObserver);


		/**
		* Destructor
		*/

		IMPORT_C ~CSIPConnection();

    public: // New functions

        /**
		* Gets current state of the connection        
        * @return current state of the connection
        */

		IMPORT_C TState State() const;

        /**
		* Creates SIP request and sends it to the given destination.
		* The function must not be used with SIP requests that creates
		* SIP dialog associations or must be sent within SIP dialog
		* association.
		* This provokes the Non-invite state machine.
		* This function must be used if the user has created the
		* registration binding.
		* @pre State() == EActive.
		* @pre aContext.IsContextActive()==ETrue
		* @param aElements contains local and remote addresses,
		*		 optional SIP message headers and body.
		*        The ownership is transferred.
		* @param aContext defines outbound proxy to be used. If the user
		*        does not provide from-address it will be constucted using
		*        user's AOR used during the registration.
        * @return SIP client transaction, ownership is transferred
        */

		IMPORT_C CSIPClientTransaction*
            SendRequestL(CSIPRequestElements* aElements,
                         const MSIPRegistrationContext& aContext);


        /**
		* Creates SIP request and sends it to the given destination.
		* The function must not be used with SIP request that creates
		* SIP dialog associations or must be sent within SIP diallog
		* association.
		* This provokes the Non-invite state machine.
		* This function must be used if the user has created the
		* registration binding.
		* @pre State() == EActive.
		* @pre aContext.IsContextActive()==ETrue
		* @param aElements contains local and remote addresses,
		*		 optional SIP message headers and body.
		*        The ownership is transferred.
		* @param aContext defines outbound proxy to be used. If the user
		*        does not provide from-address it will 
		*        be constucted using user's AOR used during the registration.
		* @param aRefresh defines the refresh interval for this transaction.
        * @return SIP client transaction, ownership is transferred
        */

		IMPORT_C CSIPClientTransaction*
            SendRequestL(CSIPRequestElements* aElements,                                        
                         const MSIPRegistrationContext& aContext,
						 CSIPRefresh& aRefresh); 


        /**
		* Creates SIP request and sends it to the given destination.
		* The function must not be used with SIP request that creates SIP
		* diallog associations or must be sent within SIP dialog association.
		* This invokes the Non-invite state machine.
		* @pre State() == EActive.
		* @pre aElements.FromHeader() != 0
		* @param aElements contains local and remote addresses,
		*		 optional SIP message headers and body.
		*        The ownership is transferred.
        * @return SIP client transaction, ownership is transferred
        */

		IMPORT_C CSIPClientTransaction*
            SendRequestL(CSIPRequestElements* aElements);
        

        /**
		* Creates SIP request and sends it to the given destination.
		* The function must not be used with SIP request that creates SIP
		* dialog associations or must be sent within SIP dialog association.
		* This invokes the Non-invite state machine.
		* @pre State() == EActive.
		* @pre aElements.FromHeader() != 0
		* @param aElements contains local and remote addresses,
		*		 optional SIP message headers and body.
		*        The ownership is transferred.
		* @param aRefresh indicates that transaction will be refreshed
        * @return SIP client transaction, ownership is transferred
        */

		IMPORT_C CSIPClientTransaction*
            SendRequestL(CSIPRequestElements* aElements,
						 CSIPRefresh& aRefresh);
 
        IMPORT_C void SetOptL(TUint aOptionName,
	                          TUint aOptionLevel,
	                          TInt aOption);    
            
    public: // Not in real SIP API:

        void SetState(TState aState);

    private: // Constructors
    
        CSIPConnection::CSIPConnection(
            CSIP& aSIP,
            TUint32 aIapId,
            MSIPConnectionObserver& aObserver);

    private: // Data

        TUint32 iIapId;
        
        // Not in real SIP API
        TState iState;
	};

#endif

