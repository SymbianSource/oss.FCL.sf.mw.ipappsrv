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



#ifndef UT_CNATTRAVERSALPENDINGREQUEST_H
#define UT_CNATTRAVERSALPENDINGREQUEST_H

#include "MNATTraversalPendingRequestContainer.h"
#include "sipnattraversalrequestobserver.h"

#include <digia/eunit/ceunittestsuiteclass.h>

#include <e32def.h>
#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class.
 *
 */
NONSHARABLE_CLASS( UT_CNATTraversalPendingRequest ) :
    public CEUnitTestSuiteClass,
    public MNATTraversalPendingRequestContainer,
    public MSIPNATTraversalRequestObserver
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CNATTraversalPendingRequest* NewL();
        static UT_CNATTraversalPendingRequest* NewLC();

        /**
         * Destructor
         */
        ~UT_CNATTraversalPendingRequest();

    private:    // Constructors and destructors

        UT_CNATTraversalPendingRequest();
        void ConstructL();

    public: // From MNATTraversalPendingRequestContainer

        void RequestCompleted( TUint32 aRequestId );

    public: // MSIPNATTraversalRequestObserver

        void RequestComplete(
            TUint32 aRequestId,
            TInt aCompletionCode );

        void PublicAddrResolved(
            TUint32 aRequestId,
            const TInetAddr& aPublicAddr );

    private: // New Methods

        void SetupL();
        void Teardown();
        void TestL();
        //static MTest* suiteL();

    private: // Data

        EUNIT_DECLARE_TEST_TABLE;

        TBool iRequestCompletedCalled;
        TUint32 iLastRequestId;
    };

#endif // UT_CNATTRAVERSALPENDINGREQUEST_H

// End of File
