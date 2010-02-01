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
* Description:  See class definition below.
*
*/

#ifndef __CTCSIPCLIENTDISCOVERYCONTAINER_H__
#define __CTCSIPCLIENTDISCOVERYCONTAINER_H__

// INCLUDES
#include <e32base.h>
#include <badesca.h>

#include <sipclientdiscoveryobserver.h>

// FORWARD DECLARATIONS
class CTcSIPContext;
class CSIPClientDiscovery;


// ENUMERATIONS
enum TTcClientDiscoveryEventType
	{
	ETcClientDiscoveryEventUnset,
	ETcClientDiscoveryChannelResolved,
	ETcClientDiscoveryClientNotFound,
	ETcClientDiscoveryError
	};
	
	
// CLASS DEFINITIONS

/**
 * Simple container for received ClientDiscovery events
 */
class TTcClientDiscoveryEvent
    {
    public:	// Constructors and destructor

		/// Default constructor
		TTcClientDiscoveryEvent() : iType( ETcClientDiscoveryEventUnset ),
		                            iChannel( TUid::Uid( 0 ) ),
		                            iRequestId( 0 ), 
		                            iError( KErrNone ) { }

		/// Destructor
		~TTcClientDiscoveryEvent() { }
		
	public: // Data

        /// Event type
        TTcClientDiscoveryEventType iType;
        
		/// Uid of channel event was concerning
		TUid iChannel;

		/// Id of request event was concerning
		TUint32 iRequestId;

		/// System-wide or ClientDiscovery specific error code
		TInt iError;

    };

/**
 * CTcSIPClientDiscoveryContainer uses SIP ClientDiscovery API
 */
class CTcSIPClientDiscoveryContainer
	: public CBase,
	  public MSIPClientDiscoveryObserver
	{
	public:	// Constructors and destructor

		/**
		 * Static constructor.
		 *
		 * @param aContext Reference to test context object.
		 * @param aUid Uid of ClientDiscovery API user
		 * @return An initialized instance of this class.
		 */
		static CTcSIPClientDiscoveryContainer* NewL( CTcSIPContext& aContext, TUid aUid );

		/// Destructor
		~CTcSIPClientDiscoveryContainer();

	private:

		/**
		 * Constructor.
		 *
		 * @param aContext Reference to test context object.
		 */
		CTcSIPClientDiscoveryContainer( CTcSIPContext& aContext, TUid aUid );

       /**
        * Second phase constructor
        *
        */
        void ConstructL();
        
		/// Default constructor. Not implemented.
		CTcSIPClientDiscoveryContainer();
		

	public: // From MSIPClientDiscoveryObserver

        void ChannelResolvedL( TUid aChannel, TUint32 aRequestId );

		void ClientNotFoundL( TUint32 aRequestId,
							  TUint aStatusCode, 
							  RStringF aReasonPhrase, 
							  RPointerArray<CSIPHeaderBase> aHeaders,
						      HBufC8* aContent );

	    void ErrorOccurred( TInt aError, TUint32 aRequestId );		

	public: // New methods

		TTcClientDiscoveryEvent ReceivedClientDiscoveryEventL( TInt aTimeout );
		

		/// @return Reference to the contained CSIPClientDiscovery object.
		inline CSIPClientDiscovery& ClientDiscovery() { return *iSipClientDiscovery; }


    private:
    
	   /**
		 * Add the received event to the received events queue.
		 * The item is copied to the internal array.
		 *
		 * @param aClientDiscoveryEvent Referece to received event
		 */
		void QueueReceivedEventL( TTcClientDiscoveryEvent& aClientDiscoveryEvent );
		
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
		CArrayFixSeg< TTcClientDiscoveryEvent > iReceivedEventsQueue;

		/// SIP ClientDiscovery. Owned.
		CSIPClientDiscovery* iSipClientDiscovery;
		
		/// Uid of ClientDiscovery user
		TUid iUid;
	};

#endif // __CTCSIPCLIENTDISCOVERYCONTAINER_H__
