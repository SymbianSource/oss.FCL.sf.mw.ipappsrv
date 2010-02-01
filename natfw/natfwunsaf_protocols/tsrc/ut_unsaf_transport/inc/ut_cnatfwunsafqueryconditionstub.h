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




#ifndef UT_CNATFWUNSAFQUERYCONDITIONSTUB_H
#define UT_CNATFWUNSAFQUERYCONDITIONSTUB_H

#include <e32base.h>
#include "cnatfwunsafqueryconditionbase.h"

class CNATFWUNSAFQueryBase;
class MNATFWUNSAFServerQuery;

class UT_CNATFWUNSAFQueryConditionStub : public CNATFWUNSAFQueryConditionBase
    {
public:

    static UT_CNATFWUNSAFQueryConditionStub* NewL(
        MNATFWUNSAFServerQuery& aServerQuery );

    CNATFWUNSAFQueryBase& QueryBase ();

    CNATFWUNSAFQueryConditionBase* HandleQueryResultL(TInt aStatus);

    ~UT_CNATFWUNSAFQueryConditionStub();

private:
    void ConstructL();

    UT_CNATFWUNSAFQueryConditionStub(
        MNATFWUNSAFServerQuery& aServerQuery );

    CNATFWUNSAFQueryBase* iQueryBase;
    MNATFWUNSAFServerQuery& iServerQuery;

    };

#endif // end of UT_CNATFWUNSAFQUERYCONDITIONSTUB_H

// End of File
