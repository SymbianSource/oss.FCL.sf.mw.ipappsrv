/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Iap
*
*/




#ifndef T_IAP_H
#define T_IAP_H

#include <e32std.h>

class TIap
{
public:
    TIap( TUint32 aIapId, const TDesC& aIapName );
    TUint32 IapId();
    const TDesC& IapName();
    static TInt ResolveIapL( RArray<TIap>& aIapArray );
    
private:
    TUint32 iIapId; 
    TBuf<40> iIapName;
};

#endif