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

#ifndef __MTCDEATHOBSERVER_H__
#define __MTCDEATHOBSERVER_H__

//  INCLUDES
#include <e32std.h>

// FORWARD DECLARATIONS

//  CLASS DEFINITION
/**
 * MTcDeathObserver defines a simple abstract interface for receiving
 * thread death notifications from CTcCoroner.
 */
class MTcDeathObserver
     {
    public: // Constructors and destructor

		/// Virtual destructor.
		virtual ~MTcDeathObserver() {};

    public: // Abstract methods

		/**
		 * Called when a client has died.
		 *
		 * @param aAppName Full application dll name - for relaunching.
		 * @param aDismissDialog ETrue if there is an error dialog that should
		 *						 be dismissed.
		 */
		virtual void Died( const TDesC& aAppName, TBool aDismissDialog ) = 0;

    };

#endif // __MTCDEATHOBSERVER_H__
