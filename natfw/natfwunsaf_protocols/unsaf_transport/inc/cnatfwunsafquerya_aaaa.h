/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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



#ifndef CNATFWUNSAFQUERYA_AAAA_H
#define CNATFWUNSAFQUERYA_AAAA_H

//INCLUDES
#include <dns_qry.h>
#include "cnatfwunsafquerybase.h"
#include "natfwinternaldefs.h"

// CLASS DEFINITION
/**
 * CNATFWUNSAFQueryA_AAAA implements a
 */
NONSHARABLE_CLASS( CNATFWUNSAFQueryA_AAAA ): public CNATFWUNSAFQueryBase
    {
    public:

        static CNATFWUNSAFQueryA_AAAA* NewL( const TDesC8& aTarget );

        ~CNATFWUNSAFQueryA_AAAA();

    public:

        TInetAddr QueryResultBuf();

        void SetTargetL( const TDesC8& aTarget );

        void Query( MNATFWUNSAFHostResolver& aResolver );

        TBool QueryNext( MNATFWUNSAFHostResolver& aResolver );

        void HandleQueryResultL( MNATFWUNSAFHostResolver& aResolver );

    private:// Constructors and destructor

        void ConstructL( const TDesC8& aTarget );

        CNATFWUNSAFQueryA_AAAA();

    private:// Data

        HBufC* iTarget;

        TInetAddr iResult;

        TNameEntry iNResult;

    private: // For testing purposes

        UNIT_TEST(UT_CNATFWUNSAFQueryA_AAAA)

        UNIT_TEST(UT_CNATFWUNSAFQueryA_AAAAStub)

        UNIT_TEST(UT_CNATFWUNSAFA_AAAAOrigDomain)
    };
#endif // end of CNATFWUNSAFQUERYA_AAAA_H


