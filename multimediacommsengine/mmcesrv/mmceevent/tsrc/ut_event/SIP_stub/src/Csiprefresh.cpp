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



#include "siprefresh.h"


EXPORT_C CSIPRefresh::~CSIPRefresh()
	{
	}


CSIPRefresh::CSIPRefresh()
	{
	}


EXPORT_C CSIPRefresh* CSIPRefresh::NewL()
	{
	return new (ELeave) CSIPRefresh;
	}


EXPORT_C CSIPRefresh* CSIPRefresh::NewLC()
	{
	CSIPRefresh* self = NewL();
	CleanupStack::PushL(self);
	return self;
	}


EXPORT_C void CSIPRefresh::SetIntervalL(TUint /*aInterval*/)
    {
    // Simulate leaving in low memory conditions
    delete HBufC8::NewL(100);
    }