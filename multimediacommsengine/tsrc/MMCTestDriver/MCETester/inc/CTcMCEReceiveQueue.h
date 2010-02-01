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



#ifndef __CTcMCEReceiveQueue_H__
#define __CTcMCEReceiveQueue_H__

// INCLUDES
#include <e32base.h>
#include <badesca.h>
#include "MCEConstants.h"

// FORWARD DECLARATIONS
class CTcMCEContext;
class CTcMCEReceived;

// CLASS DEFINITION
/**
 *
 */
class CTcMCEReceiveQueue: public CBase
	{
	public:	// Constructors and destructor

		static CTcMCEReceiveQueue* NewL( CTcMCEContext& aContext );

		/// Destructor
		~CTcMCEReceiveQueue();

	public: // New methods
		
		void QueueReceivedL( CTcMCEReceived* aItem );
		CTcMCEReceived& ReceivedItemL( TInt aTimeout );	

	private:
	
		/// Default constructor
		CTcMCEReceiveQueue( CTcMCEContext& aContext );
		void ConstructL();

		/**
		 * CDeltaTimer callback. Called when the timer entry expires.
		 *
		 * @param aSelf Pointer to self
		 * @return KErrNone
		 */
		static TInt ReceiveTimeout( TAny* aSelf );		
		
	private: // Owned data

		/// Active scheduler wrapper for async waiting. Owned.
		CActiveSchedulerWait iActiveWait;
		
		/// Array of received items. Owned.
		CArrayFixSeg< TBuf8< KTcMaxObjectName > > iReceiveQueue;
	
		/// Timeout timer. Owned.
		CDeltaTimer* iTimer;
		
		/// Timeout entry. Owned.
		TDeltaTimerEntry iReceiveTimeout;

	private: // Not owned data

		/// Reference to the test context. Not owned.
		CTcMCEContext& iContext;		


	};

#endif // __CTcMCEReceiveQueue_H__
