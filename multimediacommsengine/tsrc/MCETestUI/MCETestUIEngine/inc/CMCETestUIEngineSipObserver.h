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
* Description:    See class definition below.
*
*/



#ifndef __CMCETESTUIENGINESIPOBSERVER_H__
#define __CMCETESTUIENGINESIPOBSERVER_H__

// INCLUDES
#include "CMCETestUiEngine.h"
#include "SIPObserver.h"
#include <sipconnectionobserver.h>

// FORWARD DECLARATIONS

class CSIPServerTransaction;

// CLASS DEFINITION
/**
 * CMCETestUIEngineSipObserver is a dummy class hiding the MSIPObserver 
 * dummies. Id there is not going to be any other use of class, one solution
 * would be to remove the class and include MSIPObserver dummies to 
 * CMCETestUIEngine.
 */
class CMCETestUIEngineSipObserver
	: public MSIPObserver,
	  public MSIPConnectionObserver
	{
	public:	// Constructors and destructor

		/**
		 * Static constructor.
		 *
		 * @return An initialized instance of this class.
		 */
		static CMCETestUIEngineSipObserver* NewL();

    	/**
    	* Destructor.
    	*/
		~CMCETestUIEngineSipObserver();
    
					
	public: // from MSIPObserver
	
		void IncomingRequest(TUint32 aIapId,
					         CSIPServerTransaction* aTransaction);

		void TimedOut(CSIPServerTransaction& aTransaction);
	public: // From MSIPConnectionObserver, all dummies

		void IncomingRequest (CSIPServerTransaction* aTransaction);
		void IncomingRequest (CSIPServerTransaction* aTransaction,
					          CSIPDialog& aDialog);
		void IncomingResponse (CSIPClientTransaction& aTransaction);
		void IncomingResponse (CSIPClientTransaction& aTransaction,
							   CSIPDialogAssocBase& aDialogAssoc);
		void IncomingResponse (CSIPClientTransaction& aTransaction,
							   CSIPInviteDialogAssoc* aDialogAssoc);
		void IncomingResponse (CSIPClientTransaction& aTransaction,
                               CSIPRegistrationBinding& aRegistration);
		void ErrorOccured (TInt aError,
                                   CSIPTransactionBase& aTransaction);
		void ErrorOccured (TInt aError,
				                   CSIPClientTransaction& aTransaction,
					               CSIPRegistrationBinding& aRegistration);
		void ErrorOccured (
					TInt aError,
				    CSIPTransactionBase& aTransaction,
					CSIPDialogAssocBase& aDialogAssoc);
		void ErrorOccured (TInt aError, CSIPRefresh& aSIPRefresh);
		void ErrorOccured (TInt aError,					
					               CSIPRegistrationBinding& aRegistration);
		void ErrorOccured (TInt aError,			
					               CSIPDialogAssocBase& aDialogAssoc);
        void InviteCompleted (CSIPClientTransaction& aTransaction);
        void InviteCanceled (CSIPServerTransaction& aTransaction);
		void ConnectionStateChanged (CSIPConnection::TState aState);	
		
		CSIP* GetSipInstance();
					
	private:

        /**
        * C++ default constructor.
        */        
        CMCETestUIEngineSipObserver();

		/**
		 * Symbian 2nd phase constructor
		 */
		void ConstructL();		
		
		CSIP* iSip;
		
	};

#endif // __CMCETESTUIENGINESIPOBSERVER_H__
