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

#include <e32const.h>

#ifdef EKA2 //RD_APPS_TO_EXES 
#include <eikstart.h>
#endif

EXPORT_C CApaApplication* NewApplication()
	{
	return new CMCETestUIApp;
	}

GLDEF_C TInt E32Main()
	{
	return EikStart::RunApplication(NewApplication);
	}


