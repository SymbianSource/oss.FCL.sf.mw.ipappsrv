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

#ifndef __TTCPROFILERECEIVED_H__
#define __TTCPROFILERECEIVED_H__

// INCLUDES
#include <e32std.h>

// ENUMERATIONS
enum TTcProfileEvent
	{
	ETcUnknown,
	ETcProfileCreated,
	ETcProfileUpdated,
	ETcProfileRegistrationStatusChanged,
	ETcProfileDestroyed,
	ETcProfileRegistryErrorOccurred
	};
	
enum TTcProfileRegistrationStatus
    {
    ETcStatusUnknown,
    ETcStatusUnregistered,
    ETcStatusRegistered 
    };

// CLASS DEFINITION
/**
 * TTcProfileReceived implements a container class for profile notification messages.
 */
class TTcProfileReceived
	{
	public:	// Constructors and destructor

		/// Default constructor
		TTcProfileReceived();

		/// Destructor
		~TTcProfileReceived();

	public: // Data

		/// Profile id in a profile registry
		TUint32 iProfileId;

		/// Profile agent event
		TTcProfileEvent iEvent;
		
		/// Profile registration status
		TTcProfileRegistrationStatus iRegStatus;

		/// System-wide or Profile specific error code
		TInt iError;

	};

#endif // __TTCPROFILERECEIVED_H__
