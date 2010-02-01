/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
#include "e32std.h"

static TTime sCurrentTime = 0;

void TTime::HomeTime()
    {
    iTime = sCurrentTime.Int64();
    }

TInt TTime::DayNoInYear(TTime aStartDate) const
    {
    sCurrentTime = aStartDate;
    return 1;
    }