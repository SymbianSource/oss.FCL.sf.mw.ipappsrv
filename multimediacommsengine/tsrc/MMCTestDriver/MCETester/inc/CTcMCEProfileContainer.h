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



#ifndef __CTcMCEProfileContainer_H__
#define __CTcMCEProfileContainer_H__

// INCLUDES
#include "CTcContextBase.h"
#include "CTcIAPManager.h"
#include <sipobserver.h>
#include <sipprofileregistryobserver.h>
#include <sipmanagedprofileregistry.h>
#include <sipprofileregistry.h>

#include <sipconnection.h>
#include <sipconnectionobserver.h>
#include <cdblen.h>

//#include "TTcSIPReceived.h"
#include "TTcProfileReceived.h"

#include "sipconnection.h"

// FORWARD DECLARATIONS
class CTcMCEContext;

// HELPER CLASSES
class CTcSIPProfile : public CBase
	    {
	public:
	
	    /**
	     * Create CTcSIPProfile item which is used for storing both
	     * profile and its id
	     *
	     * @param aProfile, ownership is taken
	     * @param aProfileId
	     */
	    CTcSIPProfile( CSIPProfile* aProfile, TInt aProfileId );
	        
	    /**
	     * Destructor
         */
         ~CTcSIPProfile();
            
        /**
         * Returns pointer to owned profile
         *
         * @return reference to CSIPProfile
         */
        CSIPProfile& Profile();
        
        TInt ProfileId();
            
    private:
        
        CSIPProfile* iProfile;
        TInt iProfileId; 
	    };
	

// CLASS DEFINITION
/**
 * CTcMCEProfileContainer implements a container for multiple registry. 
 * Its task is also to observe and queue SIP notifications for
 * future use.
 */
class CTcMCEProfileContainer
	: public CBase,
      public MSIPProfileRegistryObserver,
      public MSIPConnectionObserver
	{
	

	
	
	public:	// Constructors and destructor

		/**
		 * Static constructor.
		 *
		 * @param aContext Reference to test context object.
		 * @param aSip 
		 * @return An initialized instance of this class.
		 */
		static CTcMCEProfileContainer* NewL( CTcMCEContext& aContext,
											 CSIP& aSip );

		/// Destructor
		virtual ~CTcMCEProfileContainer();

	private:

		/**
		 * Constructor.
		 *
		 * @param aContext Reference to test context object.
		 * @param aIAPName Internet Access Point name to be used.
		 */
		CTcMCEProfileContainer( CTcMCEContext& aContext,
								CSIP& aSip );

		/// Default constructor. Not implementer.
		CTcMCEProfileContainer();

		/// 2nd phase constructor
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
		 * Returns the first item off the receive queue (FIFO). If no
		 * items are present, the function will wait for aTimeout seconds
		 * and then try fetching again.
		 *
		 * @param aTimeOut Time in seconds waited for items to arrive.
		 */
		TTcProfileReceived ReceivedProfileItemL( TInt aTimeout );

	    /**
		 * Add the specified item to the received items queue.
		 * The item is copied to the internal array.
		 *
		 * @param aItem Referece to a received object.
		 */
		void QueueProfileReceived( TTcProfileReceived& aItem );

		/**
		 * @return Reference to Profile Registry Admin interface.
		 */
        //inline CSIPManagedProfileRegistry& ManagedProfileRegistry()
		//		{ return *iManagedProfileRegistry; };

		/**
		 * @return Reference to Profile Registry User interface.
		 */
		inline CSIPProfileRegistry& ProfileRegistry()
				{ return *iProfileRegistry; };
				
				
		/**
		 * Store profile, cleanupstack is used internally so caller shouldn't use
		 * Cleanupstack::PushL for aProfile before calling the function
		 *
		 * @param aProfile, ownership is taken
		 */		
	    void AddProfileL( CSIPProfile* aProfile );
	    
		/**
		 * Returns profile by its id from profiles array.
		 * If profile doesnt's exist already in profiles array, new instance
		 * will be created through profile registry and created profile will
		 * be stored to profiles array
		 *
		 * @return reference to profile
         */
         CSIPProfile& GetProfileByIdL( TInt aProfileId );
         
        /**
		 * Registers profile and waits until registration succeeds.
		 *
		 * @param aProfileId, a profile to be registered
		 * @return reference to registered profile
         */ 
		CSIPProfile& RegisterProfileL(TInt aProfileId);
         
         /**
		 * Deletes profile 
		 *
		 * @param aProfileId, a profile to be registered
		 * @return KErrNone if deleting profile is successful
         */ 
		 TInt DisableProfileL(TInt aProfileId);
        
        /**
		 * Returns default profile.
		 *
		 * @return reference to default profile
         */
         CSIPProfile& GetDefaultProfileL();
         
	private: // New methods

		/**
		 * CDeltaTimer callback. Called when the timer entry expires.
		 *
		 * @param aSelf Pointer to self
		 * @return KErrNone
		 */
		//static TInt ReceiveTimeout( TAny* aSelf );

	private: // data

		/// Active scheduler wrapper for async waiting. Owned.
		//CActiveSchedulerWait iActiveWait;

		/// Reference to the test context. Not owned.
		CTcMCEContext& iContext;

		/// Timeout timer. Owned.
		//CDeltaTimer* iTimer;

		/// Timeout entry. Owned.
		//TDeltaTimerEntry iReceiveTimeout;

		/// SIP stack. Not Owned.
		CSIP& iSip;

		/// User interface to profile registry. Owned.
		CSIPProfileRegistry* iProfileRegistry;

		/// Array of received items. Owned.
		CArrayFixSeg< TTcProfileReceived > iReceiveQueue;
		
		/// Array of profiles retrieved from ProfileRegistry
		RPointerArray< CTcSIPProfile > iProfiles;
	
	    CActiveSchedulerWait iWait;

	};

#endif // __CTcMCEProfileContainer_H__
