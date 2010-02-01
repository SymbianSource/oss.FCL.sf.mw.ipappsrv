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




#include "CMCETestUIApp.h"
#include "CMCETestUIDoc.h"
#include <aknapp.h>


//             The function is called by the UI framework to ask for the
//             application's UID. The returned value is defined by the
//             constant KUidMCEtestui and must match the second value
//             defined in the project definition file.
// 
TUid CMCETestUIApp::AppDllUid() const
	{
	return KUidMCETestUI;
	}

//             This function is called by the UI framework at
//             application start-up. It creates an instance of the
//             document class.
//
CApaDocument* CMCETestUIApp::CreateDocumentL()
	{
	return CMCETestUIDoc::NewL( *this );
	}