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


#ifndef UT_CNATBINDINGSTUNANDCRLFREFRESHER_H
#define UT_CNATBINDINGSTUNANDCRLFREFRESHER_H

#include "MNATBindingRefresherObserver.h"
#include "nattraversalsocketmanager.h"

#include <es_sock.h>
#include "natfwstunclientobserver.h"

#include <digia/eunit/ceunittestsuiteclass.h>

#include <e32def.h>
#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif

class CSTUNClient;
class CSTUNBinding;
class CNATBindingSTUNAndCRLFRefresher;
class CNATBinding;


NONSHARABLE_CLASS(UT_CNATBindingSTUNAndCRLFRefresher) :
    public CEUnitTestSuiteClass,
    public MNATBindingRefresherObserver,
    public MSTUNClientObserver,
    public MNATTraversalSocketManager
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CNATBindingSTUNAndCRLFRefresher* NewL();
        static UT_CNATBindingSTUNAndCRLFRefresher* NewLC();

        /**
         * Destructor
         */
        ~UT_CNATBindingSTUNAndCRLFRefresher();

    private:    // Constructors and destructors

        UT_CNATBindingSTUNAndCRLFRefresher();
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
        void TestNATBindingRefreshCompleteL();
        void TestRefreshTimerExpiredL();
        void TestCancelRefreshL();

    private: // Data

        EUNIT_DECLARE_TEST_TABLE;

        CDeltaTimer* iDeltaTimer;
        RSocketServ iSocketServ;
        RConnection iConnection;
        RSocket iSocket;
        CSTUNClient* iSTUNClient;
        CSTUNBinding* iSTUNBinding;
        CNATBindingSTUNAndCRLFRefresher* iRefresher;
        TInt iLastCompletionCode;
        CNATBinding* iNATBinding;
        TBool iSendingNotAllowed;
    };

#endif // UT_CNATBINDINGSTUNANDCRLFREFRESHER_H

// End of File
