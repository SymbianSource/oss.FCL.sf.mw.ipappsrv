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

#ifndef __MTCTYPED_H__
#define __MTCTYPED_H__

// INCLUDES
#include <e32std.h>

// CLASS DEFINITION
/**
 * MTcTyped defines an interface for querying type of objects used in
 * CTRL parameter lists.
 * @sa CTcParameterList
 */
class MTcTyped
	{
	public: // Enumerations

		/// CTRL types
		enum TParameterType
			{
			/// Unspecified type
			EUnknown,
			/// CTcNameValue
			ENameValue,
			/// CTcArray
			EArray,
			/// CTcStructure
			EStructure
			};

	public: // Constructors and destructors

		/// Virtual destructor. Allows deletion through this interface.
		virtual ~MTcTyped() {};

	public: // Abstract methods

		/// @retun Type of the derived object
		virtual MTcTyped::TParameterType ParameterType() const = 0;

		/// @return Name of the object instance
		virtual TPtrC8 Name() const = 0;

	};

#endif // __MTCTYPED_H__
