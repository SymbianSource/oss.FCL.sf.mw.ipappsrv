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

#ifndef __TTCIDENTIFIER_H__
#define __TTCIDENTIFIER_H__

// INCLUDES
#include <e32std.h>

// CLASS DEFINITION
/**
 * TTcIdentifier implements a thin container for request/response identifiers.
 * An identifier is of form "function" or "container.function".
 *
 * Please note that TTcIdentifier does not own nor copy any data.
 * The lifetime of any TTcIdentifier instances must be shorter than
 * the descriptors given as constructor arguments.
 */
class TTcIdentifier
	{
	public: // Constructors and destructors

		/**
		 * Constructor. Assigns both iContainer and iFunction to KNullDesC8
		 */
		IMPORT_C TTcIdentifier();

		/**
		 * Constructor. Assigns iContainer to KNullDesC8
		 * and iFunction to aFunction.
		 *
		 * @param aFunction Function name
		 */
		IMPORT_C TTcIdentifier( const TDesC8& aFunction );

		/**
		 * Constructor. Assigns iContainer to aContainer
		 * and iFunction to aFunction.
		 *
		 * @param aContainer Container name
		 * @param aFunction Function name
		 */
		IMPORT_C TTcIdentifier( const TDesC8& aContainer,
								const TDesC8& aFunction );

		/// Destructor.
		IMPORT_C ~TTcIdentifier();

	public: // Data

		/// Pointer descriptor to container name.
		TPtrC8 iContainer;

		/// Pointer descriptor to function name.
		TPtrC8 iFunction;

	};

#endif // __TTCIDENTIFIER_H__
