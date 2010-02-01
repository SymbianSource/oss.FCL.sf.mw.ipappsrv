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

#ifndef __CTCSETTINGSDIALOG_H__
#define __CTCSETTINGSDIALOG_H__

// INCLUDES
#ifdef __SERIES60_
#include <aknform.h>
#include <aknlists.h>
#include <aknsettingitemlist.h>
#include <eiklbo.h>
#else
#include <eikdialg.h>
#endif


// FORWARD DECLARATIONS
class TTcSettings;
class CTcIAPManager;

// CLASS DEFINITION
/**
 * CTcSettingsDialog implements a UI dialog component for changing
 * the application settings (e.g. connection type and log file name).
 *
 */
class CTcSettingsDialog

#ifdef __SERIES60_
	: public CAknDialog,
	  public MEikListBoxObserver
#else
	: public CEikDialog
#endif
	{
	public: // Constructors and destructor

		/**
		 * Constructor.
		 *
		 * @param aSettings Reference to the application settings container.
		 */
		CTcSettingsDialog( TTcSettings& aSettings );

		/// Destructor
		~CTcSettingsDialog();

		/// 2nd phase costructor. Must be public in dialogs.
		void ConstructL();

	private: // Constructors and destructor

		// Default constructor, not implemented
		CTcSettingsDialog();

	protected: // from CEikDialog

		void PreLayoutDynInitL();
		TBool OkToExitL( TInt aKeycode );

#ifdef __SERIES60_
  public: // from MEikListBoxObserver

		void HandleListBoxEventL( CEikListBox* aListBox,
								  TListBoxEvent aEventType );

	public: // From MEikCommandObserver

		void ProcessCommandL( TInt aCommandId );

	public: // From CCoeControl

		TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent,
									 TEventCode aType );

 	private:

		/**
		 * This is an internal helper function used for changing
		 * the application title pane text to "Settings" for the
		 * lifetime of this dialog.
		 *
		 * @param aResourceId Resource file identifier for the title text.
		 *					  aResourceId=-1 sets the title back to default.
		 */
		void SetTitleL( TInt aResourceId ) const;

		/// @return A pointer to the settings item listbox owned by the dialog
		CAknSettingStyleListBox* ListBoxL() const;
#endif
		
	private: // Data

		/// Reference to the application settings container. Not owned.
		TTcSettings& iSettings;

		/// IAP Manager from TestClientNet. Owned.
		CTcIAPManager* iIAPManager;
		
#ifdef __SERIES60_
		/// Setting item array. Owned.
		CAknSettingItemArray iSettingItems;
		TBuf<40> iRemoteAddrText;
#endif

	};

#endif	// __CTCSETTINGSDIALOG_H__
