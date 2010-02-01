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

#ifndef __MTCBEAREROBSERVER_H__
#define __MTCBEAREROBSERVER_H__

// INCLUDES
#include <e32std.h>

// CLASS DEFINITION
/**
 * MTcBearerObserver defines an interface for receiving bearer related
 * events such as transfer completion notifications.
 * Users of MTcBearerManager should implement this interface.
 */
class MTcBearerObserver
	{
	public: // Enumerations

		/// Identifies completed operations
		enum TOperation
			{
			/// Unknown operation
			EUnknown,
			/// The connection reached listening state
			EListen,
			// 
			EConnecting,
			/// The connection reached connected state
			EConnect,
			/// The connection finished a send operation
			ESend,
			/// The connection finished a receive operation
			EReceive
			};

	protected:	// Constructors and destructor

		/// Virtual destructor. Prohibit deletion through this interface.
		virtual ~MTcBearerObserver() {}

	public: // Abstract methods

		/**
		 * Called by an observer connection object after a state transition.
		 *
		 * @param aOp Operation that completed
		 * @param aStatus System-wide error code or KErrNone
		 */
		virtual void BearerCompletion( MTcBearerObserver::TOperation aOp,
									   TInt aStatus ) = 0;

	};

#endif // __MTCBEAREROBSERVER_H__
