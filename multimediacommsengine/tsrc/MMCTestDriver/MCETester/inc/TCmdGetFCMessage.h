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



#ifndef __TCmdGetFCMessage_H__
#define __TCmdGetFCMessage_H__

// INCLUDES
#include "TTcMceCommandBase.h"

enum TGetMessageCommand
{
	EErrorCode,
	EIncomingSession,
	ESessionState,
	ERegistrationStatus,
	ERegistrationError
};

// CLASS DEFINITION
/**
 * Command class responsible for retrieving Floor Control messages.
 */
class TCmdGetFCMessage
	: public TTcMceCommandBase
	{
	public:	// Constructors and destructor

		/**
		 * Constructor.
		 *
		 * @param aContext SIP Test case context
		 */
		TCmdGetFCMessage( MTcTestContext& aContext )
							  : TTcMceCommandBase( aContext ) {};

	public: 

		void ExecuteL();

	public: // New methods

		/**
		 * Static function for matching the name of this command to
		 * a function identifier.
		 *
		 * @param aId An initialized identifier containing a function name.
		 * @return ETrue if this command matches the specified name.
		 */
		static TBool Match( const TTcIdentifier& aId );

		/**
		 * Static fuction for creating an instance of this class
		 *
		 * @param aContext MCE Test case context
		 * @return An initialized instance of this class.
		 */
		static TTcCommandBase* CreateL( MTcTestContext& aContext );

	};

#endif // __TCmdGetFCMessage_H__
