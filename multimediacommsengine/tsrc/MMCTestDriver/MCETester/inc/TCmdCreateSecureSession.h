/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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



#ifndef __TCmdCreateSecureSession_H__
#define __TCmdCreateSecureSession_H__

// INCLUDES
#include "TTcMceCommandBase.h"
#include "TCmdCreateSession.h"

// FORWARD DECLARATIONS
class CMceOutSession;

// CLASS DEFINITION
/**
 * Command class responsible for "Establish" functionality.
 */
class TCmdCreateSecureSession
	: public TCmdCreateSession 
	{
	public:	// Constructors and destructor

		/**
		 * Constructor.
		 *
		 * @param aContext SIP Test case context
		 */
		TCmdCreateSecureSession( MTcTestContext& aContext )
							  : TCmdCreateSession( aContext ) {};

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
		
		CMceSession* CreateSessionL( CMceManager& aManager,
                        			 CSIPProfile& aProfile,
                        			 const TDesC8& aRecipient,
									 HBufC8* aOriginator  );
											 
		CMceSession* CreateSessionL( CMceEvent& aEvent );
		CMceSession* CreateSessionL( CMceRefer& aRefer );

	};

#endif // __TCmdCreateSecureSession_H__
