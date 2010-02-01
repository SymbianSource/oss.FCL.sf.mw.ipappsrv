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



#ifndef CNATFWUNSAFA_AAAASRVDOMAIN_H
#define CNATFWUNSAFA_AAAASRVDOMAIN_H

//INCLUDES
#include "cnatfwunsafqueryconditionbase.h"
#include "natfwinternaldefs.h"

// FORWARD DECLARATIONS
class CNATFWUNSAFQuerySrv;
class MNATFWUNSAFServerQuery;
class CNATFWUNSAFQueryBase;

// CLASS DEFINITION
/**
 * CNATFWUNSAFA_AAAASRVDomain implements a
 */
NONSHARABLE_CLASS( CNATFWUNSAFA_AAAASRVDomain ):
    public CNATFWUNSAFQueryConditionBase
    {
    public:// Constructors and destructor

        static CNATFWUNSAFA_AAAASRVDomain* NewL(
            MNATFWUNSAFServerQuery& aServerQuery,
            CNATFWUNSAFQueryBase* aSrvQuery );

        /**
         * Destructor.
         */
        ~CNATFWUNSAFA_AAAASRVDomain();

    public:

        CNATFWUNSAFQueryBase& QueryBase();

        CNATFWUNSAFQueryConditionBase* HandleQueryResultL( TInt aStatus );

    private:// Constructors and destructor

        void ConstructL( CNATFWUNSAFQueryBase* aSrvQuery );

        CNATFWUNSAFA_AAAASRVDomain( MNATFWUNSAFServerQuery& aServerQuery );

    private:// Data

        CNATFWUNSAFQueryBase* iQueryBase;

        CNATFWUNSAFQueryBase* iSrvQuery;

        MNATFWUNSAFServerQuery& iServerQuery;

    private: // For testing purposes

        UNIT_TEST(UT_CNATFWUNSAFA_AAAASRVDomain)
    };
#endif // end of CNATFWUNSAFA_AAAASRVDOMAIN_H


