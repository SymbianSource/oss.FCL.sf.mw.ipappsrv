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




#ifndef CSIPSERVERTRANSACTION_H
#define CSIPSERVERTRANSACTION_H

// INCLUDES
#include "siptransactionbase.h"

// FORWARD DECLARATIONS
class CSIPRequestElements;
class CSIPRefresh;
class MSIPResponseSender;
class CSIPConnection;
class CSIPResponseElements;


#ifdef CPPUNIT_TEST
#undef IMPORT_C
#define IMPORT_C
#endif


/**
*  Class for managing SIP server transactions.
*  It provides services for creating, ending
*  and getting SIP transaction parameters.
*  Client cannot instantiate this class.
*  
*  @lib sipclient
*/
class CSIPServerTransaction: public CSIPTransactionBase
	{
	public:

        /**
        * Destructor
		*/

		IMPORT_C ~CSIPServerTransaction();

    public: // Not in real SIP API
    
 		IMPORT_C static CSIPServerTransaction* NewL();

        IMPORT_C void SetRequestElements(CSIPRequestElements* aElements);


	public:
		
        /**
		* Sends response and ends the transaction;
		* The user must not send 100 Trying response. Leaves on failure.
        * @pre aElements!=0
        * @pre State()==ETrying || State()==EProceeding
		* @pre CSIPConnection::State()==EActive
		* @pre ResponseAllowed()==ETrue		
		* @param aElements contains Status Code, optional Reason Phrase and
        *   optional SIP message headers and body. Ownership is transferred.
		* @leave KErrArgument if aElements == NULL
		* @leave KErrGeneral if ResponseAllowed() == EFalse
		*/

		IMPORT_C void SendResponseL(CSIPResponseElements *aElements);


	    /**
		* Gets the request elements
		* @return Request elements. Ownership is not transferred.
		*/

		IMPORT_C const CSIPRequestElements* RequestElements() const;


        /**
		* Checks if the response sending is allowed for this server
        * transaction. For instance response is not allowed in ACK transaction.
		* @return ETrue if response sending is allowed, EFalse otherwise
		*/

		IMPORT_C TBool ResponseAllowed() const;

        /**
		* Sets a new respose sender, replacing the current sender.
		*
        * @param aSender Response sender to be used from now on. Ownership is
        *   	 transferred.		
		*/

        void SetResponseSender(MSIPResponseSender* aSender);


        /**
	    * Obtains the associated CSIPConnection instance. If connection can't
        * be accessed anymore, this function leaves.
        *	
	    * @return CSIPConnection
	    */

        CSIPConnection& SIPConnectionL();


        /**
		* Update the MTransactionAssociation to point to aAssociation.
		*
        * @param aAssociation Associated object		
		*/

        void ReAssociateL(MTransactionAssociation& aAssociation);


        /**
		* Removes, but does not delete, request elements from
        * aServerTransaction.
        *
		* @param aServerTransaction Server transaction from which response
        *   elements are removed. Ownership is not transferred.
		*/

        static void DetachRequestElements(TAny* aServerTransaction);

	public:

        /**
		* Creates a server transaction.
		*
        * @param aRequestId RequestId for the transaction to use
        * @param aAssociation Object with which the transaction is associated
        * @param aElements Request elements. Ownership is transferred.
        * @return New object. Ownership is transferred.
		*/

		static CSIPServerTransaction*
            NewL(TUint32 aRequestId,
                 MTransactionAssociation& aAssociation,
                 CSIPRequestElements* aElements);


        /**
		* Creates a server transaction and pushes it to cleanup stack.
		*
        * @param aRequestId RequestId for the transaction to use
        * @param aAssociation Object with which the transaction is associated
        * @param aElements Request elements. Ownership is transferred.
        * @return New object. Ownership is transferred.
		*/

		static CSIPServerTransaction*
            NewLC(TUint32 aRequestId,
                  MTransactionAssociation& aAssociation,
                  CSIPRequestElements* aElements);

	private:
		CSIPServerTransaction();


        /**
		* Second phase constructor.
		*        
        * @param aElements Request elements. Ownership is transferred.        
		*/
        void ConstructL(CSIPRequestElements* aElements);


        CSIPRequestElements* iRequestElements;
        CSIPResponseElements* iElements;
        //This interface is used for sending the response.
        //CSIPServerTransaction owns iResponseSender.
        MSIPResponseSender* iResponseSender;

        __DECLARE_TEST;

#ifdef CPPUNIT_TEST	
	    friend class CSIPServerTransaction_Test;
        friend class CSIP_Test;
#endif
	};

#endif
