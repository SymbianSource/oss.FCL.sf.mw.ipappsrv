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

#ifndef __CTCBACKLIGHT_H__
#define __CTCBACKLIGHT_H__

//  INCLUDES
#include <e32base.h>

//  CLASS DEFINITION
/**
 * CTcBacklight implements a simple Active Object that keeps the LCD backlight
 * always on.
 */
class CTcBacklight
    : public CActive
    {
    public: // Constructors and destructor

		/**
		 * Static constructor.
		 *
		 * @return An initialised instance of this class.
		 */
		static CTcBacklight* NewL( );

		/// Destructor
		~CTcBacklight();

    private: // Constructors

		/// Default constructor.
		CTcBacklight();

		/// 2nd phase constructor.
        void ConstructL();

	protected: // from CActive

		void DoCancel();
		void RunL();

	private: // New methods

		/// Start timer
		void Start();

	private: // Data

		/// Backlight timer. Owned.
		RTimer iTimer;

    };

#endif // __CTCBACKLIGHT_H__
