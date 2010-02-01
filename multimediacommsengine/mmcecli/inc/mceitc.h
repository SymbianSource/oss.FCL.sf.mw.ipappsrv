/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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





#ifndef MMCE_ITC_H
#define MMCE_ITC_H

#include <e32std.h>
#include "mceclientserver.h"


class MMceItc
	{
public:

    virtual ~MMceItc () { }

    virtual TInt Send ( TMceItcFunctions aFunction, TIpcArgs& aArgs) = 0;

    virtual void Receive (TIpcArgs& aArgs, TRequestStatus& aStatus) = 0;

    virtual TInt Receive (TIpcArgs& aArgs, TPtr8 aContext, TPtr8 aContent ) = 0;
    
    virtual void CancelReceive ( TIpcArgs& aArgs ) = 0;
	};

#endif // end of MMCE_ITC_H

// End of File
