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

#ifndef __CTCAPPLAUNCHER_H__
#define __CTCAPPLAUNCHER_H__

//  INCLUDES
#include <e32base.h>

//  CLASS DEFINITION
/**
 * CTcAppLauncher is used to relaunch terminated applications.
 * It is also able to dismiss any error note dialogs from the UI.
 */
class CTcAppLauncher
    : public CActive
    {
    public: // Constructors and destructor

		/**
		 * Static constructor. Leaves pointer to cleanup stack.
		 *
		 * @param aAppName Full application dll name - for relaunching.
		 * @param aDismissDialog ETrue if there is an error dialog that should
		 *						 be dismissed.
		 * @param aOwner Reference to the owner array.
		 * @return An initialised instance of this class.
		 */
		static CTcAppLauncher* NewLC( const TDesC& aAppName,
									  TBool aDismissDialog,
									  CArrayPtr< CTcAppLauncher >& aOwner );

		/// Destructor
		~CTcAppLauncher();

    private: // Constructors

		/**
		 * Constructor.
		 *
		 * @param aAppName Full application dll name - for relaunching.
		 * @param aDismissDialog ETrue if there is an error dialog that should
		 *						 be dismissed.
		 * @param aOwner Reference to the owner array.
		 */
        CTcAppLauncher( const TDesC& aAppName,
					    TBool aDismissDialog,
						CArrayPtr< CTcAppLauncher >& aOwner );

		/// Default constructor. Not implemented.
		CTcAppLauncher();

		/// 2nd phase constructor.
        void ConstructL();

	protected: // from CActive

		void DoCancel();
		void RunL();

	private: // Data

		/// Launch timer. Owned.
		RTimer iTimer;

		/// Client application name and path. Owned.
		TFileName iAppName;

		/// Reference to the owner array.
		CArrayPtr< CTcAppLauncher >& iOwner;

		/// ETrue if there is an error dialog that should be dismissed.
		TBool iDismissDialog;

    };

#endif // __CTCAPPLAUNCHER_H__
