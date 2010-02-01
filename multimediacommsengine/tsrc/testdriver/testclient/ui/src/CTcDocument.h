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

#ifndef __CTCDOCUMENT_H__
#define __CTCDOCUMENT_H__

// INCLUDES
#include <eikdoc.h>

// FORWARD DECLARATIONS
class CEikApplication;
class CTcAppUi;

// CLASS DEFINITION
/**
 * CTcDocument implements the Document class required
 * by the Application Framework.
 * Its main task is to create the application UI.
 */
class CTcDocument
	: public CEikDocument
	{
	public:	// Constructors and destructor

		/**
		 * Static constructor.
		 * @param aApp Reference to the application object creating
		 *			   this document.
		 * @return An initialized instance of this class
		 */
		static CTcDocument* NewL( CEikApplication& aApp );

		/// Destructor
		~CTcDocument();

	private: // Constructors and destructor

		/**
		 * Constructor.
		 * @param aApp Reference to the application object creating
		 *			   this document.
		 */
		CTcDocument( CEikApplication& aApp );

		/// Default constructor. Not implemented.
		CTcDocument();

		/// 2nd phase constructor
		void ConstructL();

	public: // from CEikDocument

		CEikAppUi* CreateAppUiL();

	};

#endif // __CTCDOCUMENT_H__
