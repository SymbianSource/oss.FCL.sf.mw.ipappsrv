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


#ifndef UT_CNATBINDINGCRLFREFRESHER_H
#define UT_CNATBINDINGCRLFREFRESHER_H

#include "MNATBindingRefresherObserver.h"

#include <digia/eunit/ceunittestsuiteclass.h>

#include <e32def.h>
#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif

class CCRLFSender;
class CNATBindingCRLFRefresher;


NONSHARABLE_CLASS(UT_CNATBindingCRLFRefresher) :
    public CEUnitTestSuiteClass,
    public MNATBindingRefresherObserver
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CNATBindingCRLFRefresher* NewL();
        static UT_CNATBindingCRLFRefresher* NewLC();

        /**
         * Destructor
         */
        ~UT_CNATBindingCRLFRefresher();

    private:    // Constructors and destructors

        UT_CNATBindingCRLFRefresher();
        void ConstructL();

    public: // From MNATBindingRefresherObserver

        void NATBindingRefreshComplete( TInt aCompletionCode );

    private: // New Methods

        void SetupL();
        void Teardown();
        void TestSetRefreshL();
        void TestRefreshTimerExpiredL();

    private: // Data

        EUNIT_DECLARE_TEST_TABLE;

        CDeltaTimer* iDeltaTimer;
        CCRLFSender* iSender;
        CNATBindingCRLFRefresher* iRefresher;
    };

#endif // UT_CNATBINDINGCRLFREFRESHER_H

// End of File
