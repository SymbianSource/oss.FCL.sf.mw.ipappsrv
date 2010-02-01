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




#ifndef CMCEPOCSESSIONSETTINGSLISTCONTAINER_H
#define CMCEPOCSESSIONSETTINGSLISTCONTAINER_H

// INCLUDES

// System includes
#include <coecntrl.h> // CCoeControl

//App
#include <MCETestUI.rsg>
#include "MCETestUI.hrh"

//App Qos
//#include "TMCEPoCSessionParams.h"
#include "CMCEPoCSessionSettingsListContainer.h"
#include "CMCEPoCSessionSettingsItemList.h"


// CLASS DECLARATION

/**
* CMCEPoCSessionSettingsListContainer class.
* Settings List container for Generic QoS
*/ 



class CMCEPoCSessionSettingsListContainer : public CCoeControl
	{
public: // Constructors and destructor

	static CMCEPoCSessionSettingsListContainer * NewL(const TRect& aRect, TMCEPoCSessionParams& aSettings );
	static CMCEPoCSessionSettingsListContainer * NewLC(const TRect& aRect, TMCEPoCSessionParams& aSettings );
	~CMCEPoCSessionSettingsListContainer ();

public: // members
	void ChangeSelectedItemL();

private: // constructor
    CMCEPoCSessionSettingsListContainer ( TMCEPoCSessionParams& aSettings );
	void ConstructL(const TRect& aRect);


private: // from CoeControl

	TInt CountComponentControls() const;
	CCoeControl* ComponentControl(TInt aIndex) const;
	TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);

private: //data

	CMCEPoCSessionSettingsItemList* iSettingItemList; 
	TMCEPoCSessionParams& iSettings;
	};

#endif	

// End of File
