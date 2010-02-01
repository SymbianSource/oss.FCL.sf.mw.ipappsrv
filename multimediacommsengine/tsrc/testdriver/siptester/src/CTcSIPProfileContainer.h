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
* Description:  See class definition below.
*
*/

#ifndef __CTCSIPPROFILECONTAINER_H__
#define __CTCSIPPROFILECONTAINER_H__

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

#include "TTcSIPReceived.h"
#include "TTcProfileReceived.h"

#include "sipconnection.h"

// FORWARD DECLARATIONS
class CTcSIPContext;

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
	    
class CTcSIPManagedProfile : public CBase
	    {
	public:
	
	    /**
	     * Create CTcSIPProfile item which is used for storing both
	     * profile and its id
	     *
	     * @param aManagedProfile, ownership is taken
	     * @param aManagedProfileId
	     */
	    CTcSIPManagedProfile( CSIPManagedProfile* aManagedProfile, TInt aManagedProfileId );
	        
	    /**
	     * Destructor
         */
         ~CTcSIPManagedProfile();
            
        /**
         * Returns pointer to owned profile
         *
         * @return reference to CSIPManagedProfile
         */
        CSIPManagedProfile& Profile();
            
        TInt ProfileId();
            
    private:
        
        CSIPManagedProfile* iManagedProfile;
        TInt iManagedProfileId; 
	    };

// CLASS DEFINITION
/**
 * CTcSIPProfileContainer implements a container for multiple registry. 
 * Its task is also to observe and queue SIP notifications for
 * future use.
 */
class CTcSIPProfileContainer
	: public CBase,
      public MSIPProfileRegistryObserver
	{
	

	
	
	public:	// Constructors and destructor

		/**
		 * Static constructor.
		 *
		 * @param aContext Reference to test context object.
		 * @param aSip 
		 * @return An initialized instance of this class.
		 */
		static CTcSIPProfileContainer* NewL( CTcSIPContext& aContext,
											 CSIP& aSip );

		/// Destructor
		virtual ~CTcSIPProfileContainer();

	private:

		/**
		 * Constructor.
		 *
		 * @param aContext Reference to test context object.
		 * @param aIAPName Internet Access Point name to be used.
		 */
		CTcSIPProfileContainer( CTcSIPContext& aContext,
								CSIP& aSip );

		/// Default constructor. Not implementer.
		CTcSIPProfileContainer();

		/// 2nd phase constructor
		void ConstructL();

	public: // From MSIPProfileRegistryObserver
	
        void ProfileRegistryEventOccurred(TUint32 aProfileId, TEvent aEvent);
        void ProfileRegistryErrorOccurred(TUint32 aProfileId, TInt aError);

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
        inline CSIPManagedProfileRegistry& ManagedProfileRegistry()
				{ return *iManagedProfileRegistry; };

		/**
		 * @return Reference to Profile Registry User interface.
		 */
		inline CSIPProfileRegistry& ProfileRegistry()
				{ return *iProfileRegistry; };
				
				
		/**
		 * Store managed profile, cleanupstack is used internally so caller shouldn't use
		 * Cleanupstack::PushL for aManagedProfile before calling the function
		 *
		 * @param aManagedProfile, ownership is taken
		 */		
	    void AddManagedProfileL( CSIPManagedProfile* aManagedProfile );
	    
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
		 * Returns managed profile by its id from managed profiles array.
		 * If managed profile doesnt's exist already in managed profiles array,
		 * new instance will be created through managed profile registry and 
		 * created managed profile will be stored to managed profiles array
		 *
		 * @return reference to managed profile
         */
         CSIPManagedProfile& GetManagedProfileByIdL( TInt aManagedProfileId );
         
        /**
		 * Returns default profile as managed profile.
		 *
		 * @return reference to managed profile
         */
         CSIPManagedProfile& GetDefaultProfileL();
         
         /**
          * Deletes stored profile.
          *
          * @param aProfileId
          */
         void DeleteProfileById( TInt aProfileId );
         
         /**
          * Deletes stored managed profile.
          *
          * @param aManagedProfileId
          */
         void DeleteManagedProfileById( TInt aManagedProfileId );
         
	private: // New methods

		/**
		 * CDeltaTimer callback. Called when the timer entry expires.
		 *
		 * @param aSelf Pointer to self
		 * @return KErrNone
		 */
		static TInt ReceiveTimeout( TAny* aSelf );

	private: // data

		/// Active scheduler wrapper for async waiting. Owned.
		CActiveSchedulerWait iActiveWait;

		/// Reference to the test contextg. Not owned.
		CTcSIPContext& iContext;

		/// Timeout timer. Owned.
		CDeltaTimer* iTimer;

		/// Timeout entry. Owned.
		TDeltaTimerEntry iReceiveTimeout;

		/// SIP stack. Not Owned.
		CSIP& iSip;

		/// Admin interface to profile registry. Owned.
		CSIPManagedProfileRegistry* iManagedProfileRegistry;

		/// User interface to profile registry. Owned.
		CSIPProfileRegistry* iProfileRegistry;

		/// Array of received items. Owned.
		CArrayFixSeg< TTcProfileReceived > iReceiveQueue;
		
		/// Array of profiles retrieved from ProfileRegistry
		RPointerArray< CTcSIPProfile > iProfiles;
		
		/// Array of managed profiles retrieved profiles from ManagedProfileRegistry
		RPointerArray< CTcSIPManagedProfile > iManagedProfiles;
	    

	};

#endif // __CTCSIPPROFILECONTAINER_H__
