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



#ifndef CNATFWUNSAFSERVERQUERY_H
#define CNATFWUNSAFSERVERQUERY_H

//INCLUDES
#include <e32base.h>
#include "mnatfwunsafserverquery.h"
#include "natfwinternaldefs.h"

// FORWARD DECLARATIONS
class CNATFWUNSAFQueryConditionBase;
class MNATFWUNSAFServerResolverObserver;
class CNATFWUNSAFQueryData;
class MNATFWUNSAFHostResolver;

// CLASS DEFINITION
/**
 * CNATFWUNSAFServerQuery implements a
 */
NONSHARABLE_CLASS( CNATFWUNSAFServerQuery ): public CBase,
    public MNATFWUNSAFServerQuery
    {
    public:// Constructors and destructor

        /**
         * Creates a new CNATFWUNSAFServerQuery instance
         * @param aObserver callback that will be used when resolving
         *        has been completed. The ownership is not transferred.
         * @param aQueryData for storing the query data.
         *        The ownership is transferred.
         * @returns a new instance of CNATFWUNSAFServerQuery
         */
        static CNATFWUNSAFServerQuery* NewL(
            MNATFWUNSAFServerResolverObserver* aObserver,
            CNATFWUNSAFQueryData* aQueryData );

        /**
         * Creates a new CNATFWUNSAFServerQuery instance
         * and pushes it to CleanupStack
         * @param aObserver callback that will be used when resolving
         *        has been completed. The ownership is not transferred.
         * @param aQueryData for storing the query data.
         *        The ownership is transferred.
         * @returns a new instance of CNATFWUNSAFServerQuery
         */
        static CNATFWUNSAFServerQuery* NewLC(
            MNATFWUNSAFServerResolverObserver* aObserver,
            CNATFWUNSAFQueryData* aQueryData );

        /**
         * Destructor.
         */
        ~CNATFWUNSAFServerQuery();

    public:// From MNATFWUNSAFServerQuery

        CNATFWUNSAFQueryData& QueryData();

    public:

        void Query( MNATFWUNSAFHostResolver& aResolver );

        TBool HandleQueryResultL( TInt aStatus );

        MNATFWUNSAFServerResolverObserver* ServerResolverObserver();

        void SetQueryConditionL(
            CNATFWUNSAFQueryConditionBase* aQueryCondition );

    public:// Data

        static const TInt iOffset;

        TSglQueLink iLink;

    private:// Constructors and destructor

        void ConstructL( MNATFWUNSAFServerResolverObserver* aObserver,
                          CNATFWUNSAFQueryData* aQueryData );

        CNATFWUNSAFServerQuery();

    private:// Data

        CNATFWUNSAFQueryConditionBase* iQueryCondition;

        //Not owned
        MNATFWUNSAFServerResolverObserver* iObserver;

        CNATFWUNSAFQueryData* iQueryData;

    private: // For testing purposes

        UNIT_TEST( CNATFWUNSAFServerQueryTest)

    };
#endif // end of CNATFWUNSAFSERVERQUERY_H


