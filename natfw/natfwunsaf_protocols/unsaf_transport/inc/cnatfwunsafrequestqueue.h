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



#ifndef CUNSAFREQUESTQUEUE_H
#define CUNSAFREQUESTQUEUE_H

//INCLUDES
#include <e32base.h>
#include <e32std.h>
#include "natfwinternaldefs.h"

// FORWARD DECLARATIONS
class CNATFWUNSAFServerQuery;
class MNATFWUNSAFServerResolverObserver;

// CLASS DEFINITION
/**
 * CUNSAFRequestQueue implements a
 */
NONSHARABLE_CLASS( CNATFWUNSAFRequestQueue ): public CBase
    {
    public:// Constructors and destructor

        /**
         * Creates a new CUNSAFRequestQueue instance
         * @returns a new instance of CUNSAFRequestQueue
         */
        static CNATFWUNSAFRequestQueue* NewL();

        /**
         * Creates a new CUNSAFRequestQueue instance
         * and pushes it to CleanupStack
         * @returns a new instance of CUNSAFRequestQueue
         */
        static CNATFWUNSAFRequestQueue* NewLC();

        /**
         * Destructor.
         */
        ~CNATFWUNSAFRequestQueue();

    public:

        void Add( CNATFWUNSAFServerQuery& aQuery );

        CNATFWUNSAFServerQuery* NextQuery();

        void CancelQuery( const MNATFWUNSAFServerResolverObserver* aObserver );

    private:// Constructors and destructor

        CNATFWUNSAFRequestQueue();

    private:// data

        TSglQue<CNATFWUNSAFServerQuery> iList;

        TSglQueIter<CNATFWUNSAFServerQuery> iListIter;

    private: // For testing purposes

        UNIT_TEST(CUNSAFHostResolverTest)
    };

#endif // end of CUNSAFREQUESTQUEUE_H


