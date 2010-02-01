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




#ifndef UT_CNATFWUNSAFQUERYSRVSTUB_H
#define UT_CNATFWUNSAFQUERYSRVSTUB_H

#include <e32base.h>
#include <e32std.h>
#include <dns_qry.h>

#include "cnatfwunsafquerybase.h"
#include "cnatfwunsafquerysrv.h"

class UT_CNATFWUNSAFQuerySrvStub : public CNATFWUNSAFQuerySrv
    {
public:

    static UT_CNATFWUNSAFQuerySrvStub* NewL(const TDesC8& aTarget);

    static HBufC8* SetTargetProtocolL(const TDesC8& aTarget, TInt aProtocol);
    ~UT_CNATFWUNSAFQuerySrvStub();
    void Query( MNATFWUNSAFHostResolver& aResolver );
    void HandleQueryResultL ( MNATFWUNSAFHostResolver& aResolver );

private:
    void ConstructL(const TDesC8& aTarget);

    UT_CNATFWUNSAFQuerySrvStub();

    };

#endif // end of UT_CNATFWUNSAFQUERYSRVSTUB_H

// End of File
