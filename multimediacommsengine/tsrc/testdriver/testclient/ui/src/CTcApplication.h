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

#ifndef __CTCAPPLICATIONN_H__
#define __CTCAPPLICATIONN_H__

// INCLUDES
#include <eikapp.h>

// CLASS DEFINITION
/**
 * CTcApplication implements the Application class required
 * by the Application Framework.
 * Its main task is to create the application document.
 */
class CTcApplication
	: public CEikApplication
	{
	public: // Constructors and destructor

		/// Default constructor.
		CTcApplication();

		/// Destructor.
		~CTcApplication();

	public:  // from CEikApplication

		TUid AppDllUid() const;

	protected: // from CEikApplication

		CApaDocument* CreateDocumentL();

	};

#endif // __CTCAPPLICATIONN_H__
