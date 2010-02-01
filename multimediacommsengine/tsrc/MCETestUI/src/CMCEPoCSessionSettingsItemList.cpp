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
* Description:  
*
*/




// INCLUDE FILES

// Class include
#include "CMCEPoCSessionSettingsItemList.h"


// ================= MEMBER FUNCTIONS =======================

/**
* Constructor.	Takes a copy of the reference to aDifficultyLevel
* @param aDifficultyLevel the level of difficulty which will be set
*/	
CMCEPoCSessionSettingsItemList::CMCEPoCSessionSettingsItemList (TMCEPoCSessionParams& aSettings)
: iSettings (aSettings)
	{
	}

/**
*	
* Called by framework when the view size is changed.  Resizes the
* setting list accordingly.
*
*/
void CMCEPoCSessionSettingsItemList::SizeChanged()
	{
	if (ListBox()) 
		{
		ListBox()->SetRect(Rect());
		}

	}


/**
*	
* Creates the actual setting items for the list, passing
* ownership of them to the calling class.  Each setting
* item has a piece of member data which it sets values in.
*/
CAknSettingItem* CMCEPoCSessionSettingsItemList::CreateSettingItemL(TInt aIdentifier)
	{
	CAknSettingItem* settingItem = NULL;
	
	switch (aIdentifier)
		{
		case EPoCRecipentAddress:
		  	settingItem = new (ELeave) CAknEnumeratedTextPopupSettingItem( aIdentifier,static_cast<TInt>( iSettings.iPocRecipentAddress );
			break;
	/*	case EDownLinkMaximumBurstSize:
			settingItem = new (ELeave) CAknIntegerEdwinSettingItem( aIdentifier, iSettings.iDownLinkMaximumBurstSize );
			break;
		case EUpLinkMaximumBurstSize:
			settingItem = new (ELeave) CAknIntegerEdwinSettingItem( aIdentifier, iSettings.iUpLinkMaximumBurstSize );
			break;
	    case EDownLinkAveragePacketSize:
        	settingItem = new (ELeave) CAknIntegerEdwinSettingItem( aIdentifier, iSettings.iDownLinkAveragePacketSize );
			break;
	    case EUpLinkAveragePacketSize:
        	settingItem = new (ELeave) CAknIntegerEdwinSettingItem( aIdentifier, iSettings.iUpLinkAveragePacketSize );
			break;
	    case EDownLinkMaximumPacketSize:
        	settingItem = new (ELeave) CAknIntegerEdwinSettingItem( aIdentifier, iSettings.iDownLinkMaximumPacketSize );
			break;
	    case EUpLinkMaximumPacketSize:
        	settingItem = new (ELeave) CAknIntegerEdwinSettingItem( aIdentifier, iSettings.iUpLinkMaximumPacketSize );
			break;
		case EDownLinkDelay:
        	settingItem = new (ELeave) CAknIntegerEdwinSettingItem( aIdentifier, iSettings.iDownLinkDelay );
			break;
	   	case EUpLinkDelay:
        	settingItem = new (ELeave) CAknIntegerEdwinSettingItem( aIdentifier, iSettings.iUpLinkDelay );
			break;
	    case EDownLinkDelayVariation:
	       	settingItem = new (ELeave) CAknIntegerEdwinSettingItem( aIdentifier, iSettings.iDownLinkDelayVariation );
			break;
	    case EUpLinkDelayVariation:
	       	settingItem = new (ELeave) CAknIntegerEdwinSettingItem( aIdentifier, iSettings.iUpLinkDelayVariation );
			break;
		case EDownLinkPriority:
		    settingItem = new (ELeave) CAknIntegerEdwinSettingItem( aIdentifier, iSettings.iDownLinkPriority );
			break;
		case EUpLinkPriority:
	        settingItem = new (ELeave) CAknIntegerEdwinSettingItem( aIdentifier, iSettings.iUpLinkPriority );
			break;
	*/	}
	
	return settingItem; // passing ownership
	}

/**
* Causes the edit page for the currently selected setting
* item to be displayed
*/
void CMCEPoCSessionSettingsItemList::ChangeSelectedItemL()
	{
	EditItemL(ListBox()->CurrentItemIndex(), ETrue);
	}

/**
* Called by the framework whenever an item is selected. 
* Causes the edit page for the currently selected setting item to be displayed and stores
* any changes made.
*/
void CMCEPoCSessionSettingsItemList::EditItemL (TInt aIndex, TBool aCalledFromMenu)
	{
	CAknSettingItemList::EditItemL(aIndex, aCalledFromMenu);
	(*SettingItemArray())[aIndex]->StoreL();

	}
