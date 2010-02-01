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
* Description:    
*
*/





#ifndef CMCEACTIVESCHEDULER_H
#define CMCEACTIVESCHEDULER_H

//  INCLUDES
#include <e32base.h>

// CLASS DECLARATION

/**
*  Scheduler for the MCE server.
*
*  @lib 
*  @since
*/
class CMceActiveScheduler : public CActiveScheduler
    {
	public: // Constructors and destructor

		/**
		* Default constructor
		*/
		CMceActiveScheduler ();

		/**
		* Destructor.
		*/
		~CMceActiveScheduler ();

	public: // From base class

		/**
		* Error occurred
		* @param aError error code
		*/
		void Error (TInt aError) const;
    };

#endif 

