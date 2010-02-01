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
#include "CMCEPoCSessionSettingsListContainer.h"

// ================= MEMBER FUNCTIONS =======================

/**
* Symbian OS 2nd phase constructor.  Creates a Window for the controls, which it contains.
* Constructs a label and adds it to the window, which it then activates.
* @param aRect The rectangle for this window
*/		
void CMCEPoCSessionSettingsListContainer::ConstructL(const TRect& aRect)
	{
	CreateWindowL();
	iSettingItemList = new CMCEPoCSessionSettingsItemList (iSettings);
    iSettingItemList->ConstructFromResourceL( R_POC_SESSION_SETTING_ITEM_LIST );
	SetRect(aRect);
	ActivateL();
	}
/**
* Symbian OS 2 phase constructor.
* Constructs the CSettingsListContainer using the NewLC method, popping
* the constructed object from the CleanupStack before returning it.
* 
* @param aRect The rectangle for this window
* @return The newly constructed CSettingsListContainer
*/
CMCEPoCSessionSettingsListContainer* CMCEPoCSessionSettingsListContainer::NewL(const TRect& aRect, TMCEPoCSessionParams& aSettings )
	{
	CMCEPoCSessionSettingsListContainer* self = CMCEPoCSessionSettingsListContainer::NewLC( aRect, aSettings );
	CleanupStack::Pop(self);
	return self;
	}

/**
* Symbian OS 2 phase constructor.
* Constructs the CSettingsListContainer using the constructor and ConstructL 
* method, leaving the constructed object on the CleanupStack before returning it.
* 
* @param aRect The rectangle for this window
* @return The newly constructed CSettingsListContainer
*/
CMCEPoCSessionSettingsListContainer* CMCEPoCSessionSettingsListContainer::NewLC(const TRect& aRect, TMCEPoCSessionParams& aSettings )
	{
	CMCEPoCSessionSettingsListContainer* self = new (ELeave) CMCEPoCSessionSettingsListContainer( aSettings );
	CleanupStack::PushL(self);
	self->ConstructL(aRect);
	return self;
	}

CMCEPoCSessionSettingsListContainer::CMCEPoCSessionSettingsListContainer(TMCEPoCSessionParams& aSettings):
                                        iSettings( aSettings )
                                    
	{
	
	}
/** 
* Destructor.  Frees up memory for the settings list.
*/
CMCEPoCSessionSettingsListContainer::~CMCEPoCSessionSettingsListContainer()
	{
	delete iSettingItemList;
	}

/**
* Called by the framework in compound controls	
* @return The number of controls in this CSettingsListContainer
*/
TInt CMCEPoCSessionSettingsListContainer::CountComponentControls() const
	{
	return 1; // return number of controls inside this container
	}

/**
* Called by the framework in compound controls	
* @param The index of the control to return
* @return The control for aIndex
*/
CCoeControl* CMCEPoCSessionSettingsListContainer::ComponentControl(TInt aIndex) const
	{
	switch (aIndex)
		{
		case 0:
			return iSettingItemList;
		default:
			return NULL;
		}
	}


/**
* Called by the framework whenever a key event occurs.	
* @param aKeyEvent the Key event which occured, e.g. select key pressed
* @param aType the type of Key event which occurred, e.g. key up, key down
* @return TKeyResponse EKeyWasNotConsumed if the key was not processed, EKeyWasConsumed if it was
*/
TKeyResponse CMCEPoCSessionSettingsListContainer::OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType)
	{
	if (iSettingItemList && IsVisible())
	    {
	        iSettingItemList->OfferKeyEventL (aKeyEvent, aType);    
	    }
	
	//In case the tabs has to be changed
	return EKeyWasNotConsumed;
	}

/**
* Asks the setting list to change the currently selected item
*/
void CMCEPoCSessionSettingsListContainer::ChangeSelectedItemL()
	{
	if (iSettingItemList)
		iSettingItemList->ChangeSelectedItemL();
	}


// End of File	
