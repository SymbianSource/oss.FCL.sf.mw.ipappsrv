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

#ifndef __MTCTESTCONTEXT_H__
#define __MTCTESTCONTEXT_H__

// INCLUDES
#include <e32std.h>

// FORWARD DECLARATIONS
class CTcCTRLCodec;
class CTcRegistry;
class TTcIdentifier;

// CLASS DEFINITION
/**
 * MTcTestContext defines interface for test context implementations.
 */
class MTcTestContext
	{
	public: // Constructors and destructors

		/// Virtual destructor. Allows deletion through this interface.
		virtual ~MTcTestContext() {};

	public: // Abstract methods

		/**
		 * Called to execute a CTRL request (accessed using the CTcCTRLCodec)
		 */
		virtual void ExecuteL() = 0;

		/**
		 * Called to execute a CTRL RESET request.
		 *
		 * @param aId Request identifier.
		 */
		virtual void ResetL( const TTcIdentifier& aId ) = 0;

		/**
		 * @return Reference to an initialized CTRL codec containing the
		 * 		   current CTLR request under processing.
		 */
		virtual CTcCTRLCodec& Codec() = 0;

		/**
		 * @return Reference to object registry. To be used for storing
		 * 		   objects beyond the scope of ExecuteL()
		 * 		   (e.g. for the lifetime of the testcase)
		 */
		virtual CTcRegistry& Registry() = 0;

	};

#endif // __MTCTESTCONTEXT_H__
