/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __CTCSIPHTTPDIGESTCONTAINER_H__
#define __CTCSIPHTTPDIGESTCONTAINER_H__

// INCLUDES
#include <e32base.h>
#include <badesca.h>

#include <siphttpdigestchallengeobserver.h>
#include <siphttpdigestchallengeobserver2.h>
#include <siphttpdigest.h>

// FORWARD DECLARATIONS
class CTcSIPContext;
class CSIP;

// CLASS DEFINITION
/**
 * CTcSIPHttpDigestContainer implements a container for a single TSIPHttpDigest
 * object. Its task is also to observe and queue httpdigest challenges
 */
class CTcSIPHttpDigestContainer
	: public CBase,
	  public MSIPHttpDigestChallengeObserver,
	  public MSIPHttpDigestChallengeObserver2
	{
	public:	// Constructors and destructor

		/**
		 * Static constructor.
		 *
		 * @param aContext Reference to test context object.
		 * @param aIAPName Internet Access Point name to be used.
		 * @return An initialized instance of this class.
		 */
		static CTcSIPHttpDigestContainer* NewL( CTcSIPContext& aContext,
		                                        CSIP& aSip );

		/// Destructor
		~CTcSIPHttpDigestContainer();

	private:

		/**
		 * Constructor.
		 *
		 * @param aContext Reference to test context object.
		 * @param aSip
		 */
		CTcSIPHttpDigestContainer( CTcSIPContext& aContext );

       /**
        * Second phase constructor
        *
        * @param aSip
        */
        void ConstructL( CSIP& aSip );
        
		/// Default constructor. Not implemented.
		CTcSIPHttpDigestContainer();

	public: // From MSIPHttpDigestChallengeObserver

        void ChallengeReceived( const TDesC8& aRealm );

    public: //From MSIPHttpDigestChallengeObserver2

        void ChallengeReceived( const CSIPClientTransaction& aTransaction );
        
        void ChallengeReceived( const CSIPRefresh& aRefresh );

	public: // New methods

		/**
		 * Returns the first challenge off the received challenges queue (FIFO). 
		 * items are present, the function will wait for aTimeout seconds
		 * and then try fetching again. Leaves with error if still unavailable
		 * to fetch 
		 *
		 * @param aTimeOut Time in seconds waited for items to arrive.
		 * @return challenge which ownership is transferred
		 */
		HBufC8* ReceivedChallengeL( TInt aTimeout );

		/// @return Reference to the contained TSIPHttpDigest object.
		inline CSIPHttpDigest& HttpDigest() { return *iSipHttpDigest; }

    private:
    
	   /**
		 * Add the specified realm to the received challenges queue.
		 * The item is copied to the internal array.
		 *
		 * @param aRealm Referece to a received challenge.
		 */
		void QueueReceivedChallengeL( const TDesC8& aRealm );
		
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
		
		/// Timeout timer. Owned.
		CDeltaTimer* iTimer;
		
		/// Timeout entry. Owned.
		TDeltaTimerEntry iReceiveTimeout;

		/// Reference to the test context. Not owned.
		CTcSIPContext& iContext;

		/// Array of received challenges. Owned.
		RPointerArray<HBufC8> iReceivedChallengesQueue;

		/// SIP HttpDigest. Owned.
		CSIPHttpDigest* iSipHttpDigest;
	};

#endif // __CTCSIPHTTPDIGESTCONTAINER_H__
