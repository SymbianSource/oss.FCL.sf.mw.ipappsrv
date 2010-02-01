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



#ifndef __CTcMCEFloorControlObserver_H__
#define __CTcMCEFloorControlObserver_H__


// INCLUDES
#include "MCEFCMsgObserver.h"

// FORWARD DECLARATIONS
class CTcMCEContext;
class CTcMCEReceiveQueue;
class CTcMCEReceived;	

// CLASS DEFINITION
/**
 * CTcMCEFloorControlObserver observes all events defined in 
 * MCE Floor Control API.
 */

class CTcMCEFloorControlObserver
	: public CBase,
	  public MMceFcMsgObserver	  
	{
	
	public:	// Constructors and destructor

		/**
		 * Static constructor.
		 *
		 * @param aContext Reference to test context object.
		 * @return An initialized instance of this class.
		 */
		static CTcMCEFloorControlObserver* NewL( CTcMCEContext& aContext );

		/// Destructor
		~CTcMCEFloorControlObserver();

	private:

		/**
		 * Constructor.
		 *
		 * @param aContext Reference to test context object.
		 */
		CTcMCEFloorControlObserver( CTcMCEContext& aContext );

		/// Default constructor. Not implemented.
		CTcMCEFloorControlObserver();

		/// 2nd phase constructor
		void ConstructL();

	public: // new functions
	
		/**
		 * Returns the first item off the Floor control receive queue (FIFO). 
		 * If no items are present, the function will wait for aTimeout seconds
		 * and then try fetching again.
		 *
		 * @param aTimeOut Time in seconds waited for items to arrive.
		 */
		CTcMCEReceived& ReceivedFCItemL( TInt aTimeout );

	public:	// from MMceFcMsgObserver
	
        void FCMsgReceived(CMceSession& aSession, HBufC8* aMessage);

        void FCMsgErrorOccurred(CMceSession& aSession, TInt aError);		

	private: // owned data

		CTcMCEReceiveQueue* iFCQueue;	    

	private: // data

		/// Reference to the test context. Not owned.
		CTcMCEContext& iContext;
		


	};

#endif // __CTcMCEFloorControlObserver_H__
