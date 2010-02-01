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

#ifndef __CTCAPPUI_H__
#define __CTCAPPUI_H__

// INCLUDES
#include <bldvariant.hrh>
#ifdef __SERIES60_
#include <aknappui.h>
#else
#include <eikappui.h>
#endif
#include "MTcBearerObserver.h"
#include "MTcTestObserver.h"
#include "TTcSettings.h"

// FORWARD DECLARATIONS
class CErrorUI;
class CTcBacklight;
class CTcBearer;
class CTcContainer;
class CTcTestRunner;
class CTcWatcher;

// CLASS DEFINITION
/**
 * CTcAppUi implements the AppUi class required by the Application Framework.
 * Its main task is to handle user originated events. It is also responsible
 * for owning the test case runner CTcTestRunner and launching the settings
 * dialog CTcSettingsDialog.
 */
class CTcAppUi
#ifdef __SERIES60_
	: public CAknAppUi,
#else
	: public CEikAppUi,
#endif
	  public MTcBearerObserver,
	  public MTcTestObserver
	{
	public:	// Constructors and destructor

		/// Default constructor
		CTcAppUi();

		/// Destructor
		~CTcAppUi();

		/// 2nd phase constructor
		void ConstructL();

	public: // From CEikAppUi

		void HandleCommandL( TInt aCommand );
		void DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane );

	public: // From MTcBearerObserver

		void BearerCompletion( MTcBearerObserver::TOperation aOp,
							   TInt aStatus );

	public: // From MTcTestObserver

		void NotifyDisconnect();
		void NotifyStatusChange();

	private: // New methods

		/**
		 * Handles the ECmdConnect command. Creates and connects the chosen
		 * Bearer (TCP / BT )
		 */
		void CmdConnectL();

		/**
		 * Handles the ECmdDisconnect command. Deletes both Test Runner and
		 * Bearer, closing any network connections.
		 */
		void CmdDisconnect();

		/**
		 * Handles the ECmdSettings command. Constructs and displays
		 * the settings dialog CTcSettingsDialog.
		 */
		void CmdSettingsL();

		/**
		 * Handles the ECmdLocalAddress command. Displays
		 * the local device address in a popup note.
		 */
		void CmdLocalAddressL();

		/**
		 * Change Status label text in the application main view.
		 *
		 * @param aText New text for the label
		 */
		void SetStatus( const TDesC8& aText );

        /**
		 * Reads text from resource array.
		 *
		 */
        HBufC* ReadFromResourceArrayLC( TInt aResourceId, TInt aSettingIndex );
        
		/**
		 * Shows a global error note using CErrorUI if aStatus =! KErrNone
		 *
		 * @param aStatus System-wide error code, or KErrNone
		 */
		void ShowErrorNote( TInt aStatus );

	private: // Data

		/// Application view control. Owned.
		CTcContainer* iContainer;

		/// Bearer implemenatation object. Owned.
		CTcBearer* iBearer;

		/// Settings container. Owned.
		TTcSettings iSettings;

		/// Test runner. Owned.
		CTcTestRunner* iRunner;

#ifdef __SERIES60_

		/// Nokia Error UI. Owned.
		CErrorUI* iErrorUi;
#endif

		/// Watcher client wrapper. Owned.
		CTcWatcher* iWatcher;

		/// Backlight keep-alive object. Owned.
		CTcBacklight* iBacklight;

	};

#endif // __CTCAPPUI_H__
