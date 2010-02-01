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
* Description:  Implementation.
*
*/

#include <aknlists.h>
#include <aknsettingitemlist.h>
#include <akntitle.h>

#include "CTcIAPManager.h"
#include "CTcSettingsDialog.h"

#include "TTcSettings.h"
#include "TestClient.hrh"
#include <TestClient.rsg>

CTcSettingsDialog::CTcSettingsDialog( TTcSettings& aSettings )
	: iSettings( aSettings ),
	  iSettingItems( 2, EFalse, 1 )	// granularity, isnumered, 1st ordinal
	{
	}

CTcSettingsDialog::~CTcSettingsDialog()
	{
	// Delete setting items
	iSettingItems.ResetAndDestroy();
	// Set title pane text back to default, ignore any errors
	TRAP_IGNORE( SetTitleL( -1 ) )
	delete iIAPManager;
	}

void CTcSettingsDialog::ConstructL()
	{
	// Use base class to construct our menubar
	CAknDialog::ConstructL( R_TC_SETTINGS_MENUBAR );
	iIAPManager = CTcIAPManager::NewL();
	// Make sure there's at least one IAP
	if( iIAPManager->Count() == 0 )
		{
		User::Leave( KErrNotSupported );
		}
	}

void CTcSettingsDialog::PreLayoutDynInitL()
	{
	TInt id( 0 );
 	HBufC* itemTitle;
	CAknSettingItem* settingItem;

	// Create and initialize Bearer setting item from the resource file.
	CAknEnumeratedTextPopupSettingItem* textSettingItem =
								new( ELeave ) CAknEnumeratedTextPopupSettingItem( 0, iSettings.iBearerType );
	CleanupStack::PushL( textSettingItem );
	itemTitle = iCoeEnv->AllocReadResourceLC( R_TC_SETTINGS_MODE_TITLE );
	textSettingItem->ConstructL( EFalse, id++, *itemTitle, NULL, R_TC_SETTINGS_MODE_PAGE, EAknCtPopupField, 0, R_TC_SETTINGS_MODE_TEXTS );
	CleanupStack::PopAndDestroy(); // itemTitle
	iSettingItems.AppendL( textSettingItem );
	CleanupStack::Pop(); // textSettingItem
	
	
	// Create and initialize remote tcp address setting item from the resource file.
	iSettings.iTCPRemoteAddr.Output( iRemoteAddrText );
	settingItem = new( ELeave ) CAknTextSettingItem( 0, iRemoteAddrText );
	CleanupStack::PushL( settingItem );
	_LIT(KEmptyItemText, "None");
	settingItem->SetEmptyItemTextL(KEmptyItemText);
	itemTitle = iCoeEnv->AllocReadResourceLC( R_TC_SETTINGS_TCPREMOTEADDR_TITLE );
	settingItem->ConstructL( EFalse, id++, *itemTitle, NULL, R_TC_SETTINGS_TCPREMOTEADDR_PAGE, -1 );
	CleanupStack::PopAndDestroy(); // itemTitle
	iSettingItems.AppendL( settingItem );
	CleanupStack::Pop(); // settingitem

	// Create and initialize TCP port setting item from the resource file.
	settingItem = new( ELeave ) CAknIntegerEdwinSettingItem( 0, iSettings.iTCPPort );
	CleanupStack::PushL( settingItem );
	itemTitle = iCoeEnv->AllocReadResourceLC( R_TC_SETTINGS_TCPPORT_TITLE );
	settingItem->ConstructL( EFalse, id++, *itemTitle, NULL, R_TC_SETTINGS_TCPPORT_PAGE, EEikCtEdwin );
	CleanupStack::PopAndDestroy(); // itemTitle
	iSettingItems.AppendL( settingItem );
	CleanupStack::Pop(); // settingitem

	// Create and initialize IAP name setting item from the resource file.
	TUint32 tmpId( iSettings.iIAPId );
	iSettings.iIAPId = 0;
	textSettingItem = new( ELeave ) CAknEnumeratedTextPopupSettingItem( 0, iSettings.iIAPId );
	CleanupStack::PushL( textSettingItem );
	itemTitle = iCoeEnv->AllocReadResourceLC( R_TC_SETTINGS_IAPNAME_TITLE );
	textSettingItem->ConstructL( EFalse, id++, *itemTitle, NULL, R_TC_SETTINGS_IAPNAME_PAGE, EAknCtPopupField, 0, R_TC_SETTINGS_IAPNAME_TEXTS );
	CleanupStack::PopAndDestroy(); // itemTitle
	iSettingItems.AppendL( textSettingItem );
	CleanupStack::Pop(); // textSettingItem

	// Get text array from setting item
	CArrayPtr< CAknEnumeratedText >* texts = textSettingItem->EnumeratedTextArray();
	CAknEnumeratedText* dummy = texts->At( 0 );
	texts->Delete( 0 );
	delete dummy;

	// Populate text array from IAP Manager
	TInt count = iIAPManager->Count();
	for( TInt i = 0; i < count; i++ )
		{
		// Read IAP id and name
		TInt iapId = iIAPManager->Id( i );
		TBuf< 64 > text16;
		text16.Copy( iIAPManager->Name( i ) );
		HBufC* text = text16.AllocLC();

		// Create enumerated text item
		CAknEnumeratedText* enumeratedText = new( ELeave ) CAknEnumeratedText( iapId, text );
		CleanupStack::Pop( text );

		// Store text item to the array
		CleanupStack::PushL( enumeratedText );
		texts->AppendL( enumeratedText );
		CleanupStack::Pop( enumeratedText );
		}

	// Make sure iIAPId is a valid one
	// (the setting list item won't work otherwise);
	// Somebody might have removed our previously selected IAP
	// - in that case the first IAP is selected instead.
	iSettings.iIAPId = iIAPManager->Id( 0 );
	for( TInt j = 0; j < count; j++ )
		{
		if( tmpId == iIAPManager->Id( j ) )
			{
			iSettings.iIAPId = tmpId;
			break;
			}
		}

	textSettingItem->LoadL();

    // Create and initialize CSY name setting item from the resource file.
    textSettingItem = new( ELeave ) CAknEnumeratedTextPopupSettingItem( 0, iSettings.iCSYName );
	CleanupStack::PushL( textSettingItem );
	itemTitle = iCoeEnv->AllocReadResourceLC( R_TC_SETTINGS_CSYNAME_TITLE );
	textSettingItem->ConstructL( EFalse, id++, *itemTitle, NULL, R_TC_SETTINGS_MODE_PAGE, EAknCtPopupField, 0, R_TC_SETTINGS_CSYNAME_TEXTS );
	CleanupStack::PopAndDestroy(); // itemTitle
	iSettingItems.AppendL( textSettingItem );
	CleanupStack::Pop(); // textSettingItem
	
	// Create and initialize Comm port setting item from the resource file.
    textSettingItem = new( ELeave ) CAknEnumeratedTextPopupSettingItem( 0, iSettings.iCommPort );
	CleanupStack::PushL( textSettingItem );
	itemTitle = iCoeEnv->AllocReadResourceLC( R_TC_SETTINGS_CSYNAME_TITLE );
	textSettingItem->ConstructL( EFalse, id++, *itemTitle, NULL, R_TC_SETTINGS_COMMPORT_PAGE, EAknCtPopupField, 0, R_TC_SETTINGS_COMMPORT_TEXTS );
	CleanupStack::PopAndDestroy(); // itemTitle
	iSettingItems.AppendL( textSettingItem );
	CleanupStack::Pop(); // textSettingItem
	
	// Create and initialize Baudrate setting item from the resource file.
	settingItem = new( ELeave ) CAknIntegerEdwinSettingItem( 0, iSettings.iBaudrate );
	CleanupStack::PushL( settingItem );
	itemTitle = iCoeEnv->AllocReadResourceLC( R_TC_SETTINGS_BAUDRATE_TITLE );
	settingItem->ConstructL( EFalse, id++, *itemTitle, NULL, R_TC_SETTINGS_BAUDRATE_PAGE, EEikCtEdwin );
	CleanupStack::PopAndDestroy(); // itemTitle
	iSettingItems.AppendL( settingItem );
	CleanupStack::Pop(); // settingitem


	// Create and initialize AutoConnect setting item from the resource file.
	settingItem = new( ELeave ) CAknBinaryPopupSettingItem( 0, iSettings.iAutoConnect );
	CleanupStack::PushL( settingItem );
	itemTitle = iCoeEnv->AllocReadResourceLC( R_TC_SETTINGS_AUTOCONNECT_TITLE );
	settingItem->ConstructL( EFalse, id++, *itemTitle, NULL, R_TC_SETTINGS_AUTOCONNECT_PAGE, EAknCtPopupField, 0, R_TC_SETTINGS_AUTOCONNECT_TEXTS );
	CleanupStack::PopAndDestroy(); // itemTitle
	iSettingItems.AppendL( settingItem );
	CleanupStack::Pop(); // settingitem

	// Give the setting item list to the listbox, retain ownership
	CAknSettingStyleListBox* lb = ListBoxL();
	CTextListBoxModel* model = lb->Model();
	model->SetItemTextArray( &iSettingItems );
	model->SetOwnershipType( ELbmDoesNotOwnItemArray );
	iSettingItems.RecalculateVisibleIndicesL();

	// Register for listbox notifications
	lb->SetListBoxObserver( this );

	// Create scroll indicators
	lb->CreateScrollBarFrameL();
	lb->ScrollBarFrame()->SetScrollBarVisibilityL( CEikScrollBarFrame::EOff,
												   CEikScrollBarFrame::EAuto );
	// Update listbox and scroll indicators
	lb->HandleItemAdditionL();
	lb->UpdateScrollBarsL();

	// Now that we're fully constructed, change the title pane text
	SetTitleL( R_TC_SETTINGS_DLG_TITLE );
	}

TBool CTcSettingsDialog::OkToExitL( TInt aKeycode )
	{
	
	if( aKeycode == EAknSoftkeyOptions )
		{
		DisplayMenuL();
		return EFalse;
		}
	else
		{
		// Save remote address to settings 
		// or show error note if address is invalid
		if (KErrNone != iSettings.iTCPRemoteAddr.Input(iRemoteAddrText))
				{
				_LIT(KInvalidAddressText, "Invalid remote IP");
				CEikonEnv::Static()->AlertWin(KInvalidAddressText);
				return EFalse;
				}
		
		// Save settings to file
		TInt count = iSettingItems.Count();
		for( TInt i = 0; i < count; i++ )
				{
				iSettingItems.At( i )->StoreL();
				}
		iSettings.Store();
		}
	return ETrue;
	}

void CTcSettingsDialog::HandleListBoxEventL( CEikListBox* aListBox,
											 TListBoxEvent aEventType )
	{
	// Edit in popup mode if ProcessCommandL( ECmdChange ) has requested so
	TBool isPopUp( EFalse );
	if( aEventType == EEventEditingStarted )
		{
		isPopUp = ETrue;
		}

	// Start editing (if there's an item to edit :-)
	TInt index( aListBox->CurrentItemIndex() );
	if( index >=0 )	// index is -1 if there are no items in the list
		{
        iSettingItems.At( index )->EditItemL( isPopUp );
        aListBox->DrawItem( index );
		}
	}

void CTcSettingsDialog::SetTitleL( TInt aResourceId ) const
	{
	// Get pointer to the title pane
	CAknTitlePane* title =
		static_cast< CAknTitlePane* >(
			CEikonEnv::Static()
				->AppUiFactory()
					->StatusPane()
						->ControlL( TUid::Uid( EEikStatusPaneUidTitle ) ) );
	// Make sure we have a valid pointer
	User::LeaveIfNull( title );

	// Reset the text back to default or set a new one
	if( aResourceId == -1 )
		{
		title->SetTextToDefaultL();
		}
	else
		{
		// Read the title text from resource file and put it to the title pane
		HBufC* titleText = iCoeEnv->AllocReadResourceLC( aResourceId );
		title->SetTextL( *titleText );
		CleanupStack::PopAndDestroy(); // titleText
		}
	}

void CTcSettingsDialog::ProcessCommandL( TInt aCommandId )
	{
	// Close the popup menu now.
	HideMenu();

	switch( aCommandId )
		{
		case ECmdChange:
			{
			HandleListBoxEventL( ListBoxL(), EEventEditingStarted );
			break;
			}
		case EEikCmdExit:
			{
			TryExitL( EAknSoftkeyExit );
			break;
			}
		default:
			{
			// Forward to application UI by default
			CEikonEnv::Static()->EikAppUi()->HandleCommandL( aCommandId );
			}
		}
	}

TKeyResponse CTcSettingsDialog::OfferKeyEventL( const TKeyEvent& aKeyEvent,
												TEventCode aType )
	{
	
	// Make settings permanent on each keypress,
	// i.e. store values from setting items to TTcSettings
	TInt count = iSettingItems.Count();
	for( TInt i = 0; i < count; i++ )
		{
		iSettingItems.At( i )->StoreL();
		}

	// Save to file. This is done to ensure that settings are not lost
	// if the power is lost unexpectedly.
	iSettings.Store();

	// Forward to the base class
	return CAknDialog::OfferKeyEventL( aKeyEvent, aType );
	}

CAknSettingStyleListBox* CTcSettingsDialog::ListBoxL() const
	{
	CAknSettingStyleListBox* lb =
		static_cast< CAknSettingStyleListBox* >( Control( ECtrlSettingsList ) );
	User::LeaveIfNull( lb );
	return lb;
	}
