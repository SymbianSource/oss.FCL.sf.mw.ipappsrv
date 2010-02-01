/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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



#ifndef __CMCETESTUIAPP_H
#define __CMCETESTUIAPP_H (0x1)


#include <aknapp.h>
#include <MCETestUI.rsg>
#include <e32base.h>
#include "CMCETestUIDoc.h"

#include "MCETestUI.hrh"


const TUid KUidMCETestUI = { 0X10281F9A}; 
//const TUid KUidMCETestUI = { 0X102747D3 }; 

class CAknApplication;



class CMCETestUIApp : public CAknApplication

	{
private: 
	// Inherited from class CApaApplication
	CApaDocument* CreateDocumentL();
	TUid AppDllUid() const;
	};
	
#endif