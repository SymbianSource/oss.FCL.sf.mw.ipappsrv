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



#ifndef CNATFWUNSAFA_AAAAORIGDOMAIN_H
#define CNATFWUNSAFA_AAAAORIGDOMAIN_H

//INCLUDES
#include "cnatfwunsafqueryconditionbase.h"
#include "natfwinternaldefs.h"

class CNATFWUNSAFQueryBase;
class MNATFWUNSAFServerQuery;

// CLASS DEFINITION
/**
 * CNATFWUNSAFA_AAAAOrigDomain implements a
 */
NONSHARABLE_CLASS( CNATFWUNSAFA_AAAAOrigDomain ):
    public CNATFWUNSAFQueryConditionBase
    {
    public:// Constructors and destructor

        /**
         * Creates a new CNATFWUNSAFA_AAAAOrigDomain instance
         * @param aServerQuery an MNATFWUNSAFServerQuery reference
         * @returns a new instance of CNATFWUNSAFA_AAAAOrigDomain
         */
        static CNATFWUNSAFA_AAAAOrigDomain* NewL(
            MNATFWUNSAFServerQuery& aServerQuery );

        /**
         * Destructor.
         */
        ~CNATFWUNSAFA_AAAAOrigDomain();

    public:

        CNATFWUNSAFQueryBase& QueryBase();

        CNATFWUNSAFQueryConditionBase* HandleQueryResultL( TInt aStatus );

    private:// Constructors and destructor

        void ConstructL();

        CNATFWUNSAFA_AAAAOrigDomain( MNATFWUNSAFServerQuery& aServerQuery );

    private:// Data

        CNATFWUNSAFQueryBase* iQueryBase;

        MNATFWUNSAFServerQuery& iServerQuery;

    private: // For testing purposes

        UNIT_TEST(UT_CNATFWUNSAFA_AAAAOrigDomain)
    };
#endif // end of CNATFWUNSAFA_AAAAORIGDOMAIN_H


