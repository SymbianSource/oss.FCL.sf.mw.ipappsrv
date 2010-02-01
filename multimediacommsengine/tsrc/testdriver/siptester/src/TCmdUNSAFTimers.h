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

#ifndef TCMDUNSAFTIMERS_H
#define TCMDUNSAFTIMERS_H

// INCLUDES
#include "TTcSIPCommandBase.h"

// FORWARD DECLARATIONS
class CRepository;

// CLASS DEFINITION
/**
 * Base class for UNSAF timer related commands.
 */
class TCmdUNSAFTimers
	: public TTcSIPCommandBase
	{
	public:	// Constructors and destructor

		/**
		 * Constructor.
		 *
		 * @param aContext SIP Test case context
		 */
		TCmdUNSAFTimers( MTcTestContext& aContext ) :
			TTcSIPCommandBase( aContext ) {};

	protected: // New methods

		TInt FindDomainKeyL( CRepository& aRepository,							
							 TPtrC8 aDomain,
							 TUint32& aKey ) const;

		TUint32 CreateNewKeyL( CRepository& aRepository,
							   TUint32 aKey,
							   TUint32 aTable ) const;

		void RemoveParameterL( CRepository& aRepository,
							   TUint32 aKey ) const;

		TInt ReadTimerValuesL( CRepository& aRepository,
							   TUint32& aKey,
						       TInt& aBindingRequestInterval,
						       TInt& aUdpKeepAliveInterval,
							   TInt& aTcpKeepAliveInterval ) const;

		void WriteL( CRepository& aRepository,
					 const TUint32& aKey,
					 TInt aKeyValue ) const;

		void WriteL( CRepository& aRepository,
				     const TUint32& aKey,
				     const TDesC8& aKeyValue ) const;
				     
	    void Read( CRepository& aRepository,
				   const TUint32& aKey,
			       TInt& aKeyValue ) const;

	private: // New functions, for internal use

		/**
		 * Reads the binding request retransmission interval and keepalive
		 * interval parameters of the given IAP-id, from central repository.
		 *
		 * @return KErrNone If successful, otherwise a system wide error code.
		 */
		TInt ReadIapTimersL( CRepository& aRepository,
							 TInt aIapId,
							 TUint32& aKey,
							 TInt& aBindingRequestInterval,
							 TInt& aUdpKeepAliveInterval,
							 TInt& aTcpKeepAliveInterval ) const;

		TInt FindIapKeyL( CRepository& aRepository,
					      TInt aIapId,
					      TUint32& aKey ) const;

		TInt ReadDomainTimersL( CRepository& aRepository,
						        TPtrC8 aDomain,
								TUint32& aKey,
								TInt& aUdpKeepAliveInterval,
								TInt& aTcpKeepAliveInterval ) const;
		
		TInt Read( CRepository& aRepository,
			       const TUint32& aKey,
			       TDes8& aKeyValue ) const;	
	};

#endif // TCMDUNSAFTIMERS_H
