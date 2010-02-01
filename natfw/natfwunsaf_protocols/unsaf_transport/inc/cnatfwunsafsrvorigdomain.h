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



#ifndef CNATFWUNSAFSRVORIGDOMAIN_H
#define CNATFWUNSAFSRVORIGDOMAIN_H

//INCLUDES
#include <e32base.h>
#include "cnatfwunsafqueryconditionbase.h"
#include "natfwinternaldefs.h"

// FORWARD DECLARATIONS
class CNATFWUNSAFQueryBase;
class MNATFWUNSAFServerQuery;

// CLASS DEFINITION
/**
 * CNATFWUNSAFSRVOrigDomain implements a
 */
NONSHARABLE_CLASS( CNATFWUNSAFSRVOrigDomain ):
    public CNATFWUNSAFQueryConditionBase
    {
    public:// Constructors and destructor

        static CNATFWUNSAFSRVOrigDomain* NewL(
            MNATFWUNSAFServerQuery& aServerQuery,
            TBool aFailIfNoSRVRecordsFound );

        /**
         * Destructor.
         */
        ~CNATFWUNSAFSRVOrigDomain();

    public:

        CNATFWUNSAFQueryBase& QueryBase ();

        CNATFWUNSAFQueryConditionBase* HandleQueryResultL( TInt aStatus );

    private:// Constructors and destructor

        void ConstructL();

        CNATFWUNSAFSRVOrigDomain( MNATFWUNSAFServerQuery& aServerQuery,
                             TBool aFailIfNoSRVRecordsFound );

    private:// Data

        CNATFWUNSAFQueryBase* iQueryBase;

        MNATFWUNSAFServerQuery& iServerQuery;

        TBool iFailIfNoSRVRecordsFound;

    private: // For testing purposes

        UNIT_TEST(UT_CNATFWUNSAFSrvOrigDomain)
    };
#endif // end of CNATFWUNSAFSRVORIGDOMAIN_H


