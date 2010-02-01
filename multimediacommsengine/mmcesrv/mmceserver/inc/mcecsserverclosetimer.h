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




#ifndef CMCECSSERVERCLOSETIMER_H
#define CMCECSSERVERCLOSETIMER_H

//  INCLUDES
#include <e32base.h>

// CLASS DECLARATION

/**
*  Timer to stop MCE server when it becomes inactive.
*
*  @lib 
*  @since
*/
class CMceCsServerCloseTimer: public CActive
    {
	public: // Constructors and destructor
    
		/**
		* Two-phased constructor.
		*/
		static CMceCsServerCloseTimer* NewL ();

		/**
		* Two-phased constructor.
		*/
		static CMceCsServerCloseTimer* NewLC ();

		/**
		* Destructor.
		*/
		~CMceCsServerCloseTimer ();

	public: // New

		/**
		* Stops server after given timeout
		* @param aMilliSeconds, timer timeout value in milliseconds.
		*/
		void StopActiveSchedulerAfter (TUint aMilliSeconds);

		/**
		* Called after timeout.
		*/
		void RunL ();

		/**
		* Cancels timer.
		*/
		void DoCancel ();

	private:

		CMceCsServerCloseTimer ();

		void ConstructL ();

	private:

		RTimer iTimer;
    };

#endif

// End of File
