/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef CSIP_H
#define CSIP_H

//  INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class CDesC8Array;
class MSIPObserver;
class CSIPClient;
class CSIPClientObserver;
class CSIPConnection;
class CRegistrationState;
class CUnregistered;
class CRegistering;
class CRegistered;
class CUnregistering;
class CDialogState;
class CDialogTrying;
class CDialogEarly;
class CDialogConfirmed;
class CDialogTerminated;
class CSIPRequestElements;
class CPendingTransaction;


#ifdef CPPUNIT_TEST
#undef IMPORT_C
#define IMPORT_C
#endif


// CLASS DECLARATION

/**
*  Class for connecting to SIP server.
*
*  This class provides the connection to the SIP server and
*  provides functions for quering supported SIP security mechanisms and
*  the support of signalling compression (SigComp).
* 
*  Note that the implementation uses strings defined in the SIP Codec
*  string pool. During instantiation the class will open SIP codec string pool 
*  for getting static strings and getting and setting dynamic strings.
*  The user can create only one instance of this class. (a singleton class).
*
*  @lib sipclient.lib
*/

class CSIP :
    public CBase
	{
    public:
    
        /** SIP specific errors */
//	    enum TError
//		    {
//		    /** SIP message was malformed */
//		    EMalformedMessage = -17700,
 //           /** Registrar responded with a different AOR (in the To-header) */
//		    ERegistrarRespondedWithDifferentAOR = -17701, 
//		    /** Registrar modified Contact-header */
//		    ERegistrarRespondedWithDifferentSipAddressInContact = -17702,
//		    /** Registrar responded without contact header */
//		    ERegistrarRespondedWithoutContact = -17703,
//		    /** Request pending**/
//		    ERequestPending = -17704,
//		    /** The other end responded with expires value zero */
//		    EResponseWithExpiresZero = -17705,
//		    /** The other end responded with a different tag in To-header */
//		    EResponseWithDifferentRemoteTag = -17706,
//		    /** Transport port is already in use */
//		    EPortAlreadyInUse = -17707,
//		    /** The action can't be performed in the current transaction
 //            ** state */
//		    EInvalidTransactionState = -17708,
 //           /** Not allowed in dialogs current state */
 //           EInvalidDialogState = -17709,
//            /** Dialogs creation request invalid */
//            EInvalidDialogCreationRequest = -17710,
 //           /** Dialogs creation response invalid */
//            EInvalidDialogCreationResponse = -17711,
//            /** Response in dialogs invalid */
 //           EInvalidResponseInDialogs = -17712,
 //           /** Request in dialogs invalid */
 //           EInvalidRequestInDialogs = -17713,	
//		    /** Overlapping request pending in a dialog */
//		    EOverlappingDialogRequestPending = -17714,
//            /** Sending a SIP message failed. For example ICMP error occured */
 //           ETransportFailure = -17715,
//            /** No ACK was received after sending a 2xx response */
//            ENoAckReceived = -17716,
 //           /** Not allowed in registration's current state */
 //           EInvalidRegistrationState = -17717,
 //           /** The contact given did not contain user part */
//            EInvalidContact = -17718,
 //           /** Object can't access a resource, since that has been deleted by
 //               user. User is expected to delete this object as it can no
 //               longer be used. */
//            EResourceNotAvailable = -17719,
//            /** DNS query for the remote address failed */
//            EResolvingFailure = -17720,
//            /** Authentication with a server failed */
//            EForbidden = -17721,
//			/** Maximum number of allowed SigComp compartments exceeded */
//			EMaxCompartmentsInUse = -17722,
//            /** Refresh initiated client transaction was terminated with
 //               a 3xx, 4xx, 5xx or 6xx response.*/
 //           ETerminatedWithResponse = -17723,
 //           /** Sending a request to an outbound proxy has failed */ 
 //           EOutboundProxyFailed = -17724
 //           };
//
    public:  // Constructors and destructor
        
	    /**
        * Two-phased constructor.
		* @param aUid an unique id that identifies client
		* @param aObserver a observer for asynchronous event
        * @return New object, ownership is transferred
        */

		IMPORT_C static CSIP* NewL(const TUid& aUid,
                                   MSIPObserver& aObserver);


	    /**
        * Two-phased constructor.
		* Constructs an object and adds the pointer to the cleanup stack.
		* @param aUid an unique id that identifies client
		* @param aObserver a observer for asynchronous event
        * @return New object, ownership is transferred
        */

		IMPORT_C static CSIP* NewLC(const TUid& aUid,
                                    MSIPObserver& aObserver);


        /**
        * Destructor.               
        */

		IMPORT_C ~CSIP();

    public: // New functions

		/**
		* Gets all supported security mechanisms
		* @return supported SIP security mechanisms names (e.g. digest);
        *   the ownership is transferred
		*/

		IMPORT_C CDesC8Array* SupportedSecurityMechanismsL() const;


		/**
		* Checks if the signalling compression (SigComp) is supported.
        * Leaves on failure with a system wide error code.
		* @return ETrue if supported, EFalse otherwise
		*/

		IMPORT_C TBool IsSigCompSupportedL() const;
		

		/**
		* Checks if the user has a connection created for given IAP-id
		* @param aIapId IAP-id
 		* @return sip connection or 0-pointer if connection does not exist.
        *   Ownership is not transferred.
		*/

		IMPORT_C CSIPConnection* Connection(TUint32 aIapId) const;


        /**
		* Obtains the used CSIPClient instance
        *
 		* @return SIPClient
		*/

        CSIPClient& SIPClient();
        

        /**
		* Adds a new connection into CSIP
        *
        * @pre aConnection!=NULL
		* @param aConnection Connection to add. Ownership isn't transferred.
        * @param aIapId IAP-id 		
        * @leave KErrArgument if aConnection == NULL and release build is made 
        * @panic KErrArgument if aConnection == NULL and debug build is made
		*/

        void AddConnectionL(CSIPConnection* aConnection, TUint32 aIapId);


        /**
		* Detach a connection from CSIP
        *
        * @pre aConnection!=NULL
		* @param aConnection Connection to remove. Ownership isn't transferred.
		*/        

        void RemoveConnection(const CSIPConnection* aConnection);

        
        /**
		* Obtains the initial state for CSIPRegistrations
        *
		* @return Initial state of registration state machine. Ownership isn't
        *   transferred.
		*/

        CRegistrationState* InitialRegisterState();

        
        /**
		* Obtains the initial state for CSIPDialog
        *
		* @return Initial state of dialog state machine. Ownership isn't
        *   transferred.
		*/

        CDialogState* InitialDialogState();


        /**
		* Request has been received from network.
        *
        * @param aIapId IAP-id for which the user should create a
        *   CSIPConnection
        * @param aElements Some SIP headers of the received request. Ownership
        *   is transferred.
        * @param aRequestId RequestId associated with the received request
		*/

        void IncomingRequestL(TUint32 aIapId,
                              CSIPRequestElements* aElements,
                              TUint32 aRequestId);

        /**
	     * The transcation related to the received SIP request timed out and it
         * has been destroyed. This happens if the user fails to create a SIP
         * connection and does not send an appropriate SIP response.
         *
         * @param aRequestId RequestId of the request
	     */

        void TimedOut(TUint32 aRequestId);

    private:
        CSIP(MSIPObserver& aObserver);

        void ConstructL(const TUid& aUid);


        //Application's UID
        TUid iUid;

        //Callback to the application
        MSIPObserver& iObserver;

        //SIP client
        CSIPClient* iSIPClient;

        //Observer receiving events from SIP client
        CSIPClientObserver* iSIPClientObserver;

        //Existing connections, CSIP doesn't own them
        RPointerArray<CSIPConnection> iConnections;

        //When a request comes from network with an IAP-id for which there is
        //no CSIPConnection instance, the created ServerTransaction is stored
        //here. In case TimeOut occurs, CSIP can give the pointer to the
        //associated ServerTransaction to application.
        //When application creates a CSIPConnection, all ServerTransactions
        //which have the same IAP-id are removed from this array.
        RPointerArray<CPendingTransaction> iPendingServerTransactions;


        //Registration states. These are owned by CSIP and are shared by all
        //registration instances.
        CUnregistered* iUnregistered;
        CRegistering* iRegistering;
        CRegistered* iRegistered;
        CUnregistering* iUnregistering;        

        //Dialog states. These are owned by CSIP and are shared by all dialog
        //instances. There is no proceeding state.
        CDialogTrying* iDialogTrying;        
        CDialogEarly* iDialogEarly;
        CDialogConfirmed* iDialogConfirmed;
        CDialogTerminated* iDialogTerminated;

#ifdef CPPUNIT_TEST	
	    friend class CSIP_Test;
        friend class CSIPRegistrationBinding_Test;
        friend class CSIPInviteDialogAssoc_Test;
        friend class CSIPSubscribeDialogAssoc_Test;
        friend class CSIPReferDialogAssoc_Test;
#endif
		};

#endif
