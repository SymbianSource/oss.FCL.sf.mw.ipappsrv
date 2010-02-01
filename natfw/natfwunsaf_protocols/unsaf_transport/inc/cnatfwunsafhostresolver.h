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



#ifndef CNATFWUNSAFHOSTRESOLVER_H
#define CNATFWUNSAFHOSTRESOLVER_H

//INCLUDES
#include <e32base.h>
#include <in_sock.h>
#include "mnatfwunsafhostresolver.h"
#include "natfwinternaldefs.h"

// FORWARD DECLARATIONS
class CNATFWUNSAFRequestQueue;
class CNATFWUNSAFServerQuery;
class MNATFWUNSAFServerResolverObserver;

// CLASS DEFINITION
/**
 * CNATFWUNSAFHostResolver implements a
 */
class CNATFWUNSAFHostResolver : public CActive, public MNATFWUNSAFHostResolver
    {
    public:// Constructors and destructor

        /**
         * Creates a new CNATFWUNSAFHostResolver instance
         * @param aServer for server session
         * @param aConnection for connection
         * @returns a new instance of CNATFWUNSAFHostResolver
         */
        static CNATFWUNSAFHostResolver* NewL( RSocketServ& aServer,
                                          RConnection& aConnection );
        /**
         * Creates a new CNATFWUNSAFHostResolver instance
         * and pushes it to CleanupStack
         * @param aServer for server session
         * @param aConnection for connection
         * @returns a new instance of CNATFWUNSAFHostResolver
         */
        static CNATFWUNSAFHostResolver* NewLC( RSocketServ& aServer,
                                        RConnection& aConnection );

        /**
         * Destructor.
         */
        ~CNATFWUNSAFHostResolver();

    public:// From MNATFWUNSAFHostResolver

        RHostResolver& Resolver();

        TRequestStatus& RequestStatus();

        void SetIPListFailed( TBool aFailed );

    public:

        void GetByQueryL( CNATFWUNSAFServerQuery* aQuery );

        void CancelResolving(
            const MNATFWUNSAFServerResolverObserver* aObserver );

        void RunL();

        TInt RunError( TInt aError );

        void DoCancel();

    private:// Constructors and destructor

        void ConstructL( RSocketServ& aServer, RConnection& aConnection );

        CNATFWUNSAFHostResolver();

    private:

        void ResolveNext();

        void NextQuery();

    private:// Data

        CNATFWUNSAFRequestQueue* iRequestQueue;

        CNATFWUNSAFServerQuery* iQuery;

        RHostResolver iResolver;

        TBool iObserverCancel;

        TBool iFailed;

    private: // For testing purposes

        UNIT_TEST(UT_CNATFWUNSAFHostResolver)
    };

#endif // end of CNATFWUNSAFHOSTRESOLVER_H


