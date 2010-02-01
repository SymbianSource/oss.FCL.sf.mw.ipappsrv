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
* Description:  See class definition below.
*
*/

#ifndef TCMDSETUNSAFTIMERS_H
#define TCMDSETUNSAFTIMERS_H

// INCLUDES
#include "TCmdUNSAFTimers.h"

// CLASS DEFINITION
/**
 * Command class responsible for setting the UNSAF timer values.
 */
class TCmdSetUNSAFTimers
	: public TCmdUNSAFTimers
	{
	public:	// Constructors and destructor

		/**
		 * Constructor.
		 *
		 * @param aContext SIP Test case context
		 */
		TCmdSetUNSAFTimers( MTcTestContext& aContext ) :
			TCmdUNSAFTimers( aContext ) {}

	public: // From TTcSIPCommandBase

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
		 * @param aContext SIP Test case context
		 * @return An initialized instance of this class.
		 */
		static TTcCommandBase* CreateL( MTcTestContext& aContext );

	private: // New functions, for internal use		

		/**
		 * Extract the parameters.
		 *		 
		 * @return ETrue At least one parameter was specified, write them to
		 *		   repository.
		 *		   EFalse No parameters were given, indicating the configuration
		 *		   should be removed from repository.
		 */
		TBool ReadInputParametersL( TInt& aBindingRequestInterval,
							  	    TInt& aUdpKeepAliveInterval,
							  	    TInt& aTcpKeepAliveInterval ) const;

		void SetValuesToRepositoryL( CRepository& aRepository,
									 TInt aStatus,
									 TUint32 aKey,
									 TInt aBindingRequestInterval,
									 TInt aUdpKeepAliveInterval,
									 TInt aTcpKeepAliveInterval );

		void WriteIapParamsL( CRepository& aRepository,
							  TUint32 aKey,
							  TInt aBindingRequestInterval,
							  TInt aUdpKeepAliveInterval,
							  TInt aTcpKeepAliveInterval );		

		void WriteDomainParamsL( CRepository& aRepository, 
							     TUint32 aKey,
							     TInt aUdpKeepAliveInterval,
							     TInt aTcpKeepAliveInterval );
	};

#endif // TCMDSETUNSAFTIMERS_H
