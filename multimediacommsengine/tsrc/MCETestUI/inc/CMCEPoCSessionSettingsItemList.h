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




#ifndef CMCEPOCSESSIONSETTINGSITEMLIST_H
#define CMCEPOCSESSIONSETTINGSITEMLIST_H

// INCLUDES

// System includes
#include <aknsettingitemlist.h> // CAknSettingItemList

// User includes
#include <MCETestUI.rsg>
#include "MCETestUI.hrh"


//Qos
//#include "TMCEPoCSessionParams.h"

// CLASS DECLARATION
class TMCEPoCSessionParams
	{
public:
	TBuf<200>	iPocRecipentAddress;
	};
/**
* CMCEPoCSessionSettingsItemList class.
* Settings Item List for Generic QoS
*/ 
class CMCEPoCSessionSettingsItemList : public CAknSettingItemList
	{
public: // Constructors and destructor
	CMCEPoCSessionSettingsItemList(TMCEPoCSessionParams& aSettings);
	
public: // from CAknSettingItemList
	CAknSettingItem* CreateSettingItemL(TInt identifier);
	void EditItemL (TInt aIndex, TBool aCalledFromMenu);
	
public:// members
	void ChangeSelectedItemL ();
	
private: // from CAknSettingItemList
	void SizeChanged();

private: // member data
	TMCEPoCSessionParams& iSettings;
	
	};

#endif	// #ifndef CMCEPOCSESSIONSETTINGSITEMLIST_H

// End of File
