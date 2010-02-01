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




#ifndef CSIPTRANSACTIONBASE_H
#define CSIPTRANSACTIONBASE_H

//  INCLUDES
#include "mcesrv.h"
#include <e32base.h>
#include <stringpool.h>

// FORWARD DECLARATIONS
class CSIPResponseElements;
class MTransactionAssociation;


#ifdef CPPUNIT_TEST
#undef IMPORT_C
#define IMPORT_C
#endif

/**
*  Base class for managing SIP transactions.
*  It provides services for quering transaction
*  type and its state.
*
*  This class is an abstract class and cannot be
*  instantiated.
*  @lib sipclient
*/
class CSIPTransactionBase: public CBase
	{
	public:
		/** SIP transaction state */
		enum TState
			{
			/** Trying state */
			ETrying,
			/** Calling state */
			ECalling,
			/** Proceeding state */
			EProceeding,
			/** Completed state */
			ECompleted,
			/** Confirmed state */
			EConfirmed,
			/** Terminated state */
			ETerminated
			};

	public:

        /**
        * Destructor
		*/

		virtual ~CSIPTransactionBase();
		
		
	public:
        /**
        * Gets the SIP transaction type
        * @return SIP transaction type
        */

		IMPORT_C RStringF Type() const;


		/**
		* Gets SIP transaction state
		* @return SIP transaction state
		*/

		IMPORT_C TState StateL() const;


		/**
		* Checks the if the actual object
		* is of type CSIPClientTransaction.
		* @return ETrue if object is of type CSIPClientTransaction and
		*         EFalse otherwise
		*/

		IMPORT_C TBool IsSIPClientTransaction() const;


		/**
		* Compares this object to another object also having
		* CSIPTransactionBase base class
		* The function has to be implemented in each of the sub-classes.
		* @param aTransaction a CSIPTransactionBase object to compare
		* @return ETrue if the objects are equal otherwise EFalse
		*/

        IMPORT_C TBool
            operator==(const CSIPTransactionBase& aTransaction) const;

        /**
		* Obtains the RequestId of the transaction.
		*
        * @return RequestId
		*/

        TUint32 RequestId() const;


        /**
		* Clears the MTransactionAssociation. After this the object can't be
        * used anymore and it is expected that user will delete it soon.
        *
        * @param aAssociation Object requesting the detach
		*/

        virtual void Detach(const MTransactionAssociation& aAssociation);


        /**
		* Changes the transaction state.
        *
        * @param aNextState State into which transaction moves
		*/

        void ChangeState(CSIPTransactionBase::TState aNextState);


        /**
        * Determines whether this transaction has an effect on the associated
        * dialog's state.
        *
		* @return ETrue if transaction has an effect on the dialog's state,
        *   EFalse otherwise.
		*/

        TBool AffectsDialogState() const;


        /**
		* Sets this transaction to affect the dialog state.
		*/

        void SetAffectsDialogState();


        /**
		* Determines whether the transaction type is a target refresh request.
        *
        * @param aType Type of transaction
        * @return ETrue If the transaction is a target refresh request, EFalse
        *   otherwise.
		*/

        static TBool IsTargetRefresh(RStringF aType);


        /**
		* Stores response elements. Depending on the status code, transaction
        * may enter another state.
        *
        * @param aElements Response elements, ownership is transferred.
		*/

        void SetResponseElements(CSIPResponseElements* aElements);
        
        /** utility
        */
        void SetState( TState aState );
       

    protected:
        CSIPTransactionBase( RStringF aType, TBool aIsCliTrx );

        void ConstructL();


        /**
		* Checks that iAssociation is available (not NULL). If iAssociation is
        * NULL, it means user has deleted a resource needed by
        * CSIPTransactionBase, and this function leaves.
		*/

        void CheckAssociationL() const;


        /**
		* Gets response elements.
        *
		* @return Response elements. Ownership isn't transferred.
		*/

        const CSIPResponseElements* ResponseElements() const;


        //RequestId received from SIP client        
        TUint32 iRequestId;

        //Every transaction is associated to exactly one other object:
        //CSIP, CSIPConnection, CSIPRegistrationBinding or CSIPDialogAssocBase
        MTransactionAssociation* iAssociation;

    private:
		RStringF iType;

        //ETrue is the transaction is a client transaction, EFalse otherwise
        TBool iIsClientTransaction;

        //Current transaction state
        TState iState;

        //ETrue if the transaction has an effect on the dialog state in case
        //the transaction is associated with a dialog.
        //EFalse otherwise.
        TBool iAffectsDialogState;

        //SIP response elements
        CSIPResponseElements* iResponseElements;

    	//definitions for unit testing
    	MCESRV_UT_DEFINITIONS

        __DECLARE_TEST;
    };

#endif
