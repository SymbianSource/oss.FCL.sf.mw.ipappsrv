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
class CSIPTransactionBase;
class CSIPClientTransaction;
class CSIPServerTransaction;
class MSIPRegistrationContext;
class CSIPRegistrationBinding;
class CRegistrationState;
class CSIPDialog;
class CDialogState;
class CSIPRefresh;
class CSIPMessageElements;
class CSIPRequestElements;
class CSIPResponseElements;


// CLASS DECLARATION

/**
*  Class for monitoring connection's state and sending SIP requests outside
*  the dialog associations.Class must not be used
*  -for sending requests to which received non-failure response creates a dialog
*  -for sending requests that must be sent within SIP dialog
* Note that the user can have only one instance of the class per same IAP-id
*/
class CSIPConnection : public CBase
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
				 TUint32 aIapId/*,
				 MSIPConnectionObserver& aObserver*/);


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
				  TUint32 aIapId/*,
				  MSIPConnectionObserver& aObserver*/);


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


        /**
		* Creates SIP REGISTER request and sends it to the given destination.
		* User must no define any Contact or Expires-headers
		* in the provided elements.
		* @pre State() == EActive
        * @pre aElements != 0
		* @pre aElements->Method().DesC().Length()==0
		* @param aElements contains local and remote addresses, optional SIP
        *        message headers and body. User must not set any expires values
        *        to aElements. The ownership is transferred.
        * @return SIP REGISTER client transaction, ownership is transferred
        */

		IMPORT_C CSIPClientTransaction*
            FetchRegistrationsL(CSIPRequestElements* aElements);
            
        // Not in real SIP API:
        IMPORT_C void SetState(TState aState);    

		//TODO:SHOULD THIS NEW FUNCTION BE INCLUDED INTO STUB???
		/**
		* Gets Iap-id used for this connection
		* @return Iap-id
		*/
		IMPORT_C TUint32 IapId() const;
		
		
		/**
        * Gets SIP server handle
        * @return handle to SIP server, or 0-pointer if SIP server does not
        *	exist any more
        */
        IMPORT_C CSIP* SIP();
  
		/**
        * Gets SIP server handle
        * @return handle to SIP server, or 0-pointer if SIP server does not
        *	exist any more
        */
        
        IMPORT_C const CSIP* SIP() const;

        /**
        * Gets the local IP address of this CSIPConnection 
        * that is used by the SIP implementation.
        * The user should re-new the call whenever the connection state changes
        * to the CSIPConnection::EActive in order to verify the correctness of
        * the previously retrieved local IP address. The local IP address might
        * be different as a result of the connection state change.
        * @pre State() == CSIPConnection::EActive
        * @param aAddr on return filled with local IP address
        * @leave a system-wide error code
        */ 	                         
	    IMPORT_C void GetLocalAddrL(TInetAddr& aAddr) const;

    private:
    
        CSIPConnection(CSIP& aSIP,
                       TUint32 aIapId/*,
                       MSIPConnectionObserver& aObserver*/);

        void ConstructL();
        
    private: // Data
    

        //The used CSIP instance. CSIPConnection doesn't own the CSIP.
        //If this pointer is NULL; it means application has deleted the
        //CSIP instance before deleting the CSIPConnection, and this
        //CSIPConnection is now useless.
        CSIP* iSIP;

        //IAP-id of this connection
        TUint32 iIapId;

        //Callback to upper layer
        //MSIPConnectionObserver& iObserver;
        
        
        // Not in real SIP API
        TState iState;        
	};

#endif

