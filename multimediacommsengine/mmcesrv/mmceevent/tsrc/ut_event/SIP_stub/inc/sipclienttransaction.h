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





#ifndef CSIPCLIENTTRANSACTION_H
#define CSIPCLIENTTRANSACTION_H


// INCLUDES
#include "siptransactionbase.h"

// FORWARD DECLARATIONS
class CSIPRefresh;


#ifdef CPPUNIT_TEST
#undef IMPORT_C
#define IMPORT_C
#endif


/**
*
*  Class for managing SIP client transactions.
*  It provides services for ending and getting the SIP client transaction
*  parameters. Some client transactions can also be canceled.
*
*  The user of the class cannot instante this class.
*  @lib sipclient
*/
class CSIPClientTransaction: public CSIPTransactionBase
	{
	public:

        /**
        * Destructor
		*/

		IMPORT_C ~CSIPClientTransaction();

	public:

        /**
        * Gets response elements of the most recent response.
        * @return Response elements. Ownership isn't transferred.
        */

		IMPORT_C const CSIPResponseElements* ResponseElements() const;


        /**
		* Cancels client transaction i.e. creates a CANCEL request
		* and sends to the remote UA;
		* leaves on failure.
		* @pre State()==EProceeding
		* @pre CancelAllowed()==ETrue
		* @pre Connection().State()==EActive
		* @return SIP CANCEL transaction, ownership is transferred.
		* @leave KErrSIPInvalidTransactionState if canceling is not possible
		*	at all, or because the transaction is not in a proper state or the
        *   transaction is not related to a dialog.
		* @leave KErrSIPResourceNotAvailable if a required SIP Client API
		*	object has been deleted
		*/

		IMPORT_C CSIPClientTransaction* CancelL();


		/**
		* Gets the associated refresh with the transaction.
		* If the refresh is associated with the transaction,
		* transaction will be refreshed at defined interval.
		* @return associated refresh or 0-pointer if there's no
		*         associated refresh. Ownership isn't transferred.
		*/

		IMPORT_C const CSIPRefresh* Refresh() const;


        /**
		* Checks if the client transaction is such that it can be cancelled.
        * This does no check whether the transaction is currently in such a
        * state that the canceling can be done now.
        *
		* @return ETrue if client transaction can be cancelled;
		*         EFalse otherwise
		*/

		IMPORT_C TBool CancelAllowed() const;

        /**
		* Gets the associated refresh with the transaction and allows
        * modification fo the refresh. This method is for internal use only.
        *
		* @return associated refresh or 0-pointer if there's no
		*         associated refresh. Ownership isn't transferred.
		*/

        CSIPRefresh* Refresh();


        /**
	    * Clears the association from ClientTransaction to CSIPRefresh
	    */

        void RemoveRefresh();
        
    public: // Not in real SIP API
    
 		IMPORT_C static CSIPClientTransaction* NewL();

        IMPORT_C void SetResponseElements(CSIPResponseElements* aE);    

        
	public:

        /**
		* Instantiates a CSIPClientTransaction object, leaves on failure.
        *
        * @param aType Identifies the transaction type
        * @param aRequestId RequestId obtained from SIP client
        * @param aAssociation Object to which the transaction is associated
        *   with.
        * @param aRefresh If transaction is refreshed, this points to a
        *   CSIPRefresh, otherwise this is NULL. Ownership is not transferred.
		* @return SIP client transaction, ownership is transferred.
		*/

		static CSIPClientTransaction*
            NewL(RStringF aType,
                 TUint32 aRequestId,
                 MTransactionAssociation& aAssociation,
                 CSIPRefresh* aRefresh=0);

        /**
		* Instantiates a CSIPClientTransaction object and pushes it into
        * CleanupStack, leaves on failure.
        *
        * @param aType Identifies the transaction type
        * @param aRequestId RequestId obtained from SIP client
        * @param aAssociation Object to which the transaction is associated
        *   with.
        * @param aRefresh If transaction is refreshed, this points to a
        *   CSIPRefresh, otherwise this is NULL. Ownership is not transferred.
		* @return SIP client transaction, ownership is transferred.
		*/

		static CSIPClientTransaction*
            NewLC(RStringF aType,
                  TUint32 aRequestId,
                  MTransactionAssociation& aAssociation,
                  CSIPRefresh* aRefresh=0);

    protected:
		CSIPClientTransaction();

    private:
        //NULL if this transaction is not refreshed.
        //CSIPClientTransaction does not own the refresh object.
        CSIPRefresh* iRefresh;
        
        CSIPResponseElements* iE;

#ifdef CPPUNIT_TEST
	    friend class CSIP_Test;
#endif
	};

#endif
