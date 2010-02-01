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
* Description:    See class definition below.
*
*/



#ifndef __TCmdGetRtpEvent_H__
#define __TCmdGetRtpEvent_H__

// INCLUDES
#include "TTcMCECommandBase.h"

// CLASS DEFINITION
/**
 * Command class responsible for "GetRtpEvent" functionality.
 */
class TCmdGetRtpEvent
	: public TTcMceCommandBase
	{
	public:	// Constructors and destructor

		/**
		 * Constructor.
		 *
		 * @param aContext MCE Test case context
		 */
		TCmdGetRtpEvent( MTcTestContext& aContext )
					: TTcMceCommandBase( aContext ) {};

	public: // From TTcMCECommandBase

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

#endif // __TCmdGetRtpEvent_H__