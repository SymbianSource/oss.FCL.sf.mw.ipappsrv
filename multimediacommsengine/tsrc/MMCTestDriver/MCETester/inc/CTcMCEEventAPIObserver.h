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



#ifndef __CTcMCEEventAPIObserver_H__
#define __CTcMCEEventAPIObserver_H__


// INCLUDES
#include <MCEEventObserver.h>
#include <MceInEventObserver.h>
#include <MCEReferObserver.h>
#include <MceInReferObserver.h>
#include "CTcMCEReceiveQueue.h"


// FORWARD DECLARATIONS
class CTcMCEContext;
class CMceEvent;
class CMceInEvent;
class CMceRefer;
class CMceInRefer;
class TMceTransactionDataContainer;
	

// CLASS DEFINITION
/**
 * CTcMCEEventAPIObserver observes all events defined in 
 * multimedia API.
 */

class CTcMCEEventAPIObserver
	: public CBase,
	  public MMceEventObserver,
	  public MMceInEventObserver,
	  public MMceReferObserver,
	  public MMceInReferObserver	  
	{
	
	public:	// Constructors and destructor

		/**
		 * Static constructor.
		 *
		 * @param aContext Reference to test context object.
		 * @return An initialized instance of this class.
		 */
		static CTcMCEEventAPIObserver* NewL( CTcMCEContext& aContext );

		/// Destructor
		~CTcMCEEventAPIObserver();

	private:

		/**
		 * Constructor.
		 *
		 * @param aContext Reference to test context object.
		 */
		CTcMCEEventAPIObserver( CTcMCEContext& aContext );

		/// Default constructor. Not implemented.
		CTcMCEEventAPIObserver();

		/// 2nd phase constructor
		void ConstructL();

	public: // new functions

		/**
		 * Returns the first item off the Event receive queue (FIFO). 
		 * If no items are present, the function will wait for aTimeout seconds
		 * and then try fetching again.
		 *
		 * @param aTimeOut Time in seconds waited for items to arrive.
		 */
		CTcMCEReceived& ReceivedEventItemL( TInt aTimeout );

    public: // from MMceEventObserver

        void EventStateChanged(
        			CMceEvent& aEvent,
    				TMceTransactionDataContainer* aContainer );

		void NotifyReceived(
					CMceEvent& aEvent,
    				TMceTransactionDataContainer* aContainer );	
    

        void EventConnectionStateChanged(
	                CMceEvent& aEvent,
	                TBool aActive );

        void Failed( CMceEvent& aEvent, TInt aError );

    public: // from MMceInEventObserver

        void IncomingEvent(
					CMceInEvent* aEvent,
    				TMceTransactionDataContainer* aContainer );
		
    public: // from MMceReferObserver
    
        void ReferStateChanged(
        			CMceRefer& aRefer,
    				TMceTransactionDataContainer* aContainer );

        void ReferConnectionStateChanged(
	                CMceRefer& aRefer,
	                TBool aActive );

        void Failed( CMceRefer& aRefer, TInt aError );
    
    public: // from MMceInReferObserver
       
    	void IncomingRefer(
					CMceInRefer* aRefer,
					const TDesC8& aReferTo,
    				TMceTransactionDataContainer* aContainer );
        

	private: // owned data
	
		CTcMCEReceiveQueue* iEventQueue;

	private: // data

		/// Reference to the test context.
		CTcMCEContext& iContext;	    

	};

#endif // __CTcMCEEventAPIObserver_H__
