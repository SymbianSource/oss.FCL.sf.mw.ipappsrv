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

#ifndef __MTCTESTOBSERVER_H__
#define __MTCTESTOBSERVER_H__

// INCLUDES
#include <e32std.h>

// CLASS DEFINITION
/**
 * MTcTestObserver defines an interface for observing test execution
 * in CTcTestRunner.
 * @sa CTcTestRunner
 */
class MTcTestObserver
	{
	protected: // Constructors and destructors

		/// Virtual destructor. Prohibit deletion through this interface.
		virtual ~MTcTestObserver() {};

	public: // Abstract methods

		/**
		 * Notifies that the CTRL connection has been disconnected unexpectedly
		 */
		virtual void NotifyDisconnect() = 0;

		/**
		 * Notifies of TestCore status changes.
		 */
		virtual void NotifyStatusChange() = 0;

	};

#endif // __MTCTESTOBSERVER_H__
