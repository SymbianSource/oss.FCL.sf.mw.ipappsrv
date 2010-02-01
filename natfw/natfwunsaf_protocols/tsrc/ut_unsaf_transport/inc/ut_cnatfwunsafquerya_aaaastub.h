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




#ifndef UT_CNATFWUNSAFQUERYA_AAAASTUB_H
#define UT_CNATFWUNSAFQUERYA_AAAASTUB_H

#include <e32base.h>
#include <dns_qry.h>

#include "cnatfwunsafquerybase.h"
#include "cnatfwunsafquerya_aaaa.h"

class UT_CNATFWUNSAFQueryA_AAAAStub : public CNATFWUNSAFQueryA_AAAA
    {
    public:

        static UT_CNATFWUNSAFQueryA_AAAAStub* NewL(const TDesC8& aTarget);

        ~UT_CNATFWUNSAFQueryA_AAAAStub();
        void Query( MNATFWUNSAFHostResolver& aResolver );
        TBool QueryNext ( MNATFWUNSAFHostResolver& aResolver );
    private:

        void ConstructL(const TDesC8& aTarget);

        UT_CNATFWUNSAFQueryA_AAAAStub();
        friend class UT_CNATFWUNSAFA_AAAAOrigDomain;
    };
#endif // end of UT_CNATFWUNSAFQUERYA_AAAASTUB_H

// End of File
