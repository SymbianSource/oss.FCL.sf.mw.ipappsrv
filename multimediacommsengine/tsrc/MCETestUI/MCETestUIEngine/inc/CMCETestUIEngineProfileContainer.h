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
* Description:    See class definition below.
*
*/



#ifndef __CMCETESTUIENGINEPROFILECONTAINER_H__
#define __CMCETESTUIENGINEPROFILECONTAINER_H__

// INCLUDES
#include <sipprofileregistryobserver.h>
#include <sipconnectionobserver.h>
#include <sipprofileregistry.h>

#include "CMCETestUIEngine.h"


// FORWARD DECLARATIONS
class CSIP;



// CLASS DEFINITION
/**
 * CMCETestUIEngineProfileContainer implements a container for multiple 
 * profiles and control whether they are in use or not.
 * Its task is also to observe profile registry events and pass the information
 * via CMCETestUIEngine to user interface.
 */
class CMCETestUIEngineProfileContainer
	: public CBase,
      public MSIPProfileRegistryObserver,
      public MSIPConnectionObserver
	{
	
	public:	// Constructors and destructor

		/**
		 * Static constructor.
		 */
		static CMCETestUIEngineProfileContainer* NewL( 
		            CMCETestUIEngine& aEngine, CSIP& aSip );

    	/**
    	* Destructor.
    	*/
		virtual ~CMCETestUIEngineProfileContainer();

	private:

        /**
        * C++ first phase constructor.
        */		
        CMCETestUIEngineProfileContainer( CMCETestUIEngine& aEngine, 
		                                  CSIP& aSip );

        /**
        * C++ default constructor. Not implemented.
        */		
        CMCETestUIEngineProfileContainer();

    	/**
        * Symbian second-phase constructor
        */
		void ConstructL();

	public: // From MSIPProfileRegistryObserver
	
        void ProfileRegistryEventOccurred(TUint32 aProfileId, TEvent aEvent);
        void ProfileRegistryErrorOccurred(TUint32 aProfileId, TInt aError);

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

    public : // new methods

		/**
		 * @return Reference to profile registry user interface.
		 */
        inline CSIPProfileRegistry& ProfileRegistry()
				{ return *iProfileRegistry; };

		/**
		 * Gets available profiles that are not in use yet.
		 * 
		 * @return Reference to array of unused available profiles
		 */ 
        const RPointerArray<CMCETestUIEngineProfile>& AvailableProfiles();

		/**
		 * Gets profiles that are allready in use.
		 * 
		 * @return Reference to array of used profiles
		 */          
        const RPointerArray<CMCETestUIEngineProfile>& UsedProfiles();

		/**
		 * @return ETrue if profile given as parameter is in use
		 */   
        TBool IsProfileInUse( CMCETestUIEngineProfile& aProfile );

		/**
		 * Marks profile given as parameter to be in use.
		 */           
        void TakeProfileIntoUseL( CMCETestUIEngineProfile& aProfile );

        /**
         * Gets default profile
         * 
         * @return Wrapper for default profile
         */
        CMCETestUIEngineProfile& GetDefaultProfileL();

	private: // data

		/// SIP stack. Not Owned.
		CSIP& iSip;

		/// User interface to profile registry. Owned.
		CSIPProfileRegistry* iProfileRegistry;
		
		/// Array of profiles retrieved from profile registry. Owned
		RPointerArray< CMCETestUIEngineProfile > iProfiles;

		/// Array of profiles taken to use. Owned.
		RPointerArray< CMCETestUIEngineProfile > iUsedProfiles;
	    
        /// Reference to UI Engine
        CMCETestUIEngine& iEngine;

	};

#endif // __CMCETESTUIENGINEPROFILECONTAINER_H__
