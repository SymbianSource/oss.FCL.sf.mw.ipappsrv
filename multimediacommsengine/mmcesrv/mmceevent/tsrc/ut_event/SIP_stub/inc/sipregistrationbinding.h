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




#ifndef CSIPREGISTRATIONBINDING_H
#define CSIPREGISTRATIONBINDING_H

// INCLUDES
#include "sipregistrationcontext.h"
#include "_sipcodecdefs.h"

// FORWARD DECLARATIONS
class CUri8;
class CSIPToHeader;
class CSIPFromHeader;
class CSIPContactHeader;
class CSIPRouteHeader;
class CSIPMessageElements;
class CSIPConnection;
class CSIPRefresh;
class CSIPClientTransaction;
class CRegBindingImplementation;
class CSIPConnection;

// CLASS DECLARATION

/**
*  Class for managing SIP registration binding.
*  Class provides functions for creating and updating SIP registration bindings.
*
*  @lib sipclient.lib
*/
class CSIPRegistrationBinding :
    public CBase,
    public MSIPRegistrationContext    
	{
    public: // Constructors and destructor

        /**
        * Two-phased constructor.
		* @pre aAor != 0 &&		
		*      aContact != 0
        * @pre "Expires" parameter in aContact must be >= KSIPMinExpirationValue
		* @param aConnection a SIP connection to be used
		* @param aAor contains user's address-of-record to register;
		*        the ownership is transfered
		* @param aContact contact(s) to be registered. The "expires" parameter
		*        indicates how long the client would like the binding to be
        *        valid.
		*        "expires" parameter value must be >= KSIPMinExpirationValue; 
		*		 the ownership is transfered
 		* @param aRefresh if set, created binding will be refreshed;
		*        the ownership is transfered
		* @param aOutboundProxy an outbound proxy address;
		*        the ownership is transfered
		* @param aRemoteUri if set, will be used in request-uri construction;
		*        the ownership is transfered
        * @param aFrom From header, the ownership is transferred
        * @return New object, ownership is transferred.
        * @leave KErrArgument if aAor == 0, aContact == 0 or if "Expires"
        *	parameter in aContact is less than KSIPMinExpirationValue
		*/
		IMPORT_C static CSIPRegistrationBinding*
            NewL(CSIPConnection& aConnection,
			     CSIPToHeader* aAor,
			     CSIPContactHeader* aContact,
 			     CSIPRefresh* aRefresh=0,
			     CSIPRouteHeader* aOutboundProxy=0,
			     CUri8* aRemoteUri=0,
                 CSIPFromHeader* aFrom=0);

		/**
        * Two-phased constructor.
		* @pre aAor != 0 &&
		*      aContact != 0
        * @pre "Expires" parameter in aContact must be >= KSIPMinExpirationValue
		* @param aConnection a SIP connection to be used
		* @param aAor contains user's address-of-record to register;
		*        the ownership is transfered		
		* @param aContact contact to be registered; The "expires" parameter
		*        indicates how long the client would like the binding to be
        *        valid.
        *        "expires" parameter value must be >= KSIPMinExpirationValue;
		*		 the ownership is transfered
		* @param aRefresh if set, created binding will be refreshed;
		*        the ownership is transfered
		* @param aOutboundProxy an outbound proxy address;
		*        the ownership is transfered
		* @param aRemoteUri if set, will be used in request-uri construction;
		*        the ownership is transfered
        * @param aFrom From header, the ownership is transferred
        * @return New object, ownership is transferred.
        * @leave KErrArgument if aAor == 0, aContact == 0 or if "Expires"
        *	parameter in aContact is less than KSIPMinExpirationValue
		*/
		IMPORT_C static CSIPRegistrationBinding*
            NewLC(CSIPConnection& aConnection,
			      CSIPToHeader* aAor,
			      CSIPContactHeader* aContact,
			      CSIPRefresh* aRefresh=0,
			      CSIPRouteHeader* aOutboundProxy=0,
			      CUri8* aRemoteUri=0,
                  CSIPFromHeader* aFrom=0);

		/**
		* Destructor
		*/
		IMPORT_C ~CSIPRegistrationBinding();

	public: //From MSIPRegistrationContext:

        /**
        * Checks if the registration context can be
        * used for creating SIP messages/dialogs
        * @return ETrue if the registration binding exists; EFalse
        *         otherwise
        */
        IMPORT_C TBool IsContextActive() const;

        /**
        * Gets the context id
        * @return context id
        * For internal use only
        */        
        IMPORT_C TUint32 ContextId() const;               
        
    public: // New functions
        /**
		* Creates SIP REGISTER request, sends it to the given destination.
		* Received 2xx response will create at least one registration
		* binding.
		* @pre Connection().State()==EActive
		* @param aElements contains user SIP headers and content; the ownership
		*        is taken
        * @return SIP REGISTER transaction; the ownership is transfered
        * @leave KErrSIPMalformedMessage if aElements contains contact or
        *		 expires headers
        * @leave KErrSIPInvalidRegistrationState if already registered or
        *	registering is in progress or deregistering is in progress.
        */
		IMPORT_C CSIPClientTransaction*
            RegisterL(CSIPMessageElements* aElements=0);

        /**
		* Creates SIP (de)REGISTER request, sends it to the given destination.
		* This function will remove registration binding.
		* Any SIP response (2xx - 6xx) to this request removes the binding.
		* Note that deregistration can be done then the 2xx is received to the
		* initial REGISTER transaction and there is no on-going registration
		* updates.
		* @pre Connection().State()==EActive
		* @pre IsContextActive()==ETrue
		* @param aElements contains user SIP headers and content; the ownership
		*        is transfered. If set, user SIP headers must not contain any 
		*        Contact-header nor Expires-header
        * @return SIP REGISTER transaction; the ownership is transfered
        * @leave KErrSIPInvalidRegistrationState if IsContextActive()==EFalse
        */
		IMPORT_C CSIPClientTransaction*
            DeregisterL(CSIPMessageElements* aElements=0);

		/**
		* Updates the registration binding. Note that update can be done
		* then the 2xx is received to the REGISTER transaction that relates
		* to the initial registration or to update.
		* @pre aElements!=0
		* @pre Connection().State()==EActive
		* @pre IsContextActive()==ETrue
		* @param aElements contains user SIP header and content, must not
        *        contain any Contact-header nor Expires-header; 
		*        the ownership is transferred.
        * @return SIP REGISTER transaction; the ownership is transfered
        * @leave KErrArgument if aElements == NULL
        * @leave KErrSIPInvalidRegistrationState if IsContextActive()==EFalse
		*/
		IMPORT_C CSIPClientTransaction* UpdateL(CSIPMessageElements* aElements);

		/**
		* Updates the regisration binding. Note that update can be done
		* then the 2xx is received to the REGISTER transaction that relates
		* to the initial registration or to update.
		* @pre aExpirationValue >= KSIPMinExpirationValue
		* @pre Connection().State()==EActive
		* @pre IsContextActive()==ETrue
		* @param aExpirationValue a new expiration value in seconds
		* @param aElements contains user SIP headers and content; must not
        *        contain any Contact-header nor Expires-header;
        *        the ownership is transferred.
        * @return SIP REGISTER transaction; the ownership is transfered
        * @leave KErrSIPInvalidRegistrationState if IsContextActive()==EFalse
		*/
		IMPORT_C CSIPClientTransaction*
            UpdateL(TUint aExpirationValue, CSIPMessageElements* aElements=0);

        /**
		* Gets the SIP connection used for the registration.
		* @return SIP connection used for the registration, or 0-pointer if the
        *   	  connection has been deleted. Ownership is not transferred.
		*/
        IMPORT_C CSIPConnection* Connection();

        /**
		* Gets the SIP connection used for the registration.
		* @return SIP connection used for the registration, or 0-pointer if the
        *   	  connection has been deleted. Ownership is not transferred.
		*/
        IMPORT_C const CSIPConnection* Connection() const;

		/**
		* Sets/resets outbound proxy.
		* If a 0-pointer is given the outbound proxy will be reset.
		* @param aOutboundProxy an outbound proxy. Ownership is transferred.
		* @leave KErrSIPResourceNotAvailable if a required SIP Client API
		*	object has been deleted
		*/
		IMPORT_C void SetOutboundProxyL(CSIPRouteHeader* aOutboundProxy);

		/**
		* Gets the address-of-record (AOR) used for creating binding
		* @return address-of-record (AOR)
		*/		
		IMPORT_C const CSIPToHeader& Aor() const;

		/**
		* Gets Contact header associated with the registration binding
		* @return Contact header
		*/
		IMPORT_C const CSIPContactHeader& ContactHeader() const;

		/**
		* Gets Contact header associated with the registration binding
		* @return Contact header
		*/
		IMPORT_C CSIPContactHeader& ContactHeader();
		
		/**
		* Gets used outbound proxy
		* @return outbound proxy or a 0-pointer if not set. Ownership is not
        *   transferred.
		*/
		IMPORT_C const CSIPRouteHeader* OutboundProxy() const;

        /**
		* Gets associated refresh in case the user has requested the refresh of
        * the registration. Note that client cannot use update or terminate
        * functions defined in the CSIPRefresh class. 
		* @return Associated refresh or a 0-pointer if the user has not
        *   requested a refresh of registration binding. Ownership is not
        *	transferred.
		*/
		IMPORT_C const CSIPRefresh* SIPRefresh() const;

       /**
		* Gets associated refresh in case the user has requested the refresh of
        * the registration. Note that client cannot use update or terminate
        * functions defined in the CSIPRefresh class. 
		* @return Associated refresh or a 0-pointer if the user has not
        *  requested a refresh of registration binding. Ownership is not
        *  transferred.
		*/
		IMPORT_C CSIPRefresh* SIPRefresh();
		
        /**
		* Compares this object to another object
		* @param aRegistration CSIPRegistrationBinding object to compare
		* @return ETrue if the objects are equal, otherwise EFalse
		*/
        IMPORT_C TBool
        	operator==(const CSIPRegistrationBinding& aRegistration) const;


    public: // Not in real SIP API:
        
        IMPORT_C void SetLeave(TBool aValue);
        IMPORT_C void SetContext(TBool aValue);


	public: // New functions, for internal use
		/**
		* Returns the implementation instance.
		*
 		* @return CRegBindingImplementation
		*/
		CRegBindingImplementation& Implementation();

	private: // Constructors
		CSIPRegistrationBinding();

        void ConstructL(CSIPConnection& aConnection,
        			    CSIPToHeader* aAor,
						CSIPContactHeader* aContacts,                        
						CSIPRefresh* aRefresh,
						CSIPRouteHeader* aOutboundProxy,
						CUri8* aRemoteUri,
                        CSIPFromHeader* aFrom);        

	private: // Data
		CRegBindingImplementation* iImplementation;
		
	private: // Data not in real SIP API
        CSIPMessageElements* iE;
        CSIPRouteHeader* iP;
        TBool iLeave;
        TBool iContext;		
		CSIPContactHeader* iContact;
		CSIPRefresh* iRefresh;
		
	private: // For testing purposes		        

		UNIT_TEST(CSIP_Test)
        UNIT_TEST(CSIPRegistrationBinding_Test)
        UNIT_TEST(CSIPInviteDialogAssoc_Test)
        UNIT_TEST(CSIPSubscribeDialogAssoc_Test)
        UNIT_TEST(CSIPReferDialogAssoc_Test)

		__DECLARE_TEST;
	};

#endif
