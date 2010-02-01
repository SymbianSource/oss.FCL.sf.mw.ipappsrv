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


#ifndef CNATBINDINGSTUNREFRESHERTEST_H
#define CNATBINDINGSTUNREFRESHERTEST_H

#include "MNATBindingRefresherObserver.h"
#include "nattraversalsocketmanager.h"

#include <es_sock.h>
#include <digia/eunit/ceunittestsuiteclass.h>
#include "natfwstunclientobserver.h"


#include <e32def.h>
#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif

class CSTUNClient;
class CSTUNBinding;
class CNATBindingSTUNRefresher;
class CNATBinding;


//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class.
 *
 */

NONSHARABLE_CLASS( UT_CNATBindingSTUNRefresher ) :
    public CEUnitTestSuiteClass,
    public MNATBindingRefresherObserver,
    public MSTUNClientObserver,
    public MNATTraversalSocketManager
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CNATBindingSTUNRefresher* NewL();
        static UT_CNATBindingSTUNRefresher* NewLC();

        /**
         * Destructor
         */
        ~UT_CNATBindingSTUNRefresher();

    private:    // Constructors and destructors

        UT_CNATBindingSTUNRefresher();
        void ConstructL();

    public: // From MNATBindingRefresherObserver

        void NATBindingRefreshComplete( TInt aCompletionCode );

    public: // From MSTUNClientObserver

        void STUNClientInitCompleted( const CSTUNClient& aClient,
                                      TInt aCompletionCode );

        void STUNBindingEventOccurredL( TSTUNBindingEvent aEvent,
                                        const CBinding& aBinding );

        void STUNBindingErrorOccurred( const CBinding& aBinding,
                                       TInt aError );

    public: // From MNATTraversalSocketManager

        TBool AddToSendingQueueL( MNATTraversalSocketUser& aUser );

        void SendingCompleted( MNATTraversalSocketUser& aUser );

    private: // New Methods

        void SetupL();
        void Teardown();
        void TestRefreshIntervalL();
        void TestHasSocketL();
        void TestSetRefreshL();
        void TestContinueRefreshingL();
        void TestRefreshTimerExpiredL();

    private: // Data

        EUNIT_DECLARE_TEST_TABLE;

        CDeltaTimer* iDeltaTimer;
        RSocketServ iSocketServ;
        RConnection iConnection;
        RSocket iSocket;
        CSTUNClient* iSTUNClient;
        CSTUNBinding* iSTUNBinding;
        CNATBindingSTUNRefresher* iRefresher;
        CNATBinding* iNATBinding;
    };

#endif // UT_CNATBINDINGSTUNREFRESHER_H

// End of File
