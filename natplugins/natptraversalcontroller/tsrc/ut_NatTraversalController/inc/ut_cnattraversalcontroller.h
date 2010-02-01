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


#ifndef UT_CNATTRAVERSALCONTROLLER_H
#define UT_CNATTRAVERSALCONTROLLER_H

#include <es_sock.h>
#include <in_sock.h>

#include <digia/eunit/ceunittestsuiteclass.h>

#include "sipnattraversalrequestobserver.h"
#include "sipnatbindingobserver.h"
#include "nattraversalsocketuser.h"

#include <digia/eunit/ceunittestsuiteclass.h>

#include <e32def.h>
#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif

class CNATTraversalController;


NONSHARABLE_CLASS(UT_CNATTraversalController) :
    public CEUnitTestSuiteClass,
    public MSIPNATTraversalRequestObserver,
    public MSIPNATBindingObserver,
    public MNATTraversalSocketUser
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CNATTraversalController* NewL();
        static UT_CNATTraversalController* NewLC();

        /**
         * Destructor
         */
        ~UT_CNATTraversalController();

    private:    // Constructors and destructors

        UT_CNATTraversalController();
        void ConstructL();

    public: // MSIPNATTraversalRequestObserver

        void RequestComplete(
            TUint32 aRequestId,
            TInt aCompletionCode );

        void PublicAddrResolved(
            TUint32 aRequestId,
            const TInetAddr& aPublicAddr );

    public: // MSIPNATBindingObserver

        void FlowFailure( TInt aError );

    public: // MNATTraversalSocketUser

        const RSocket& Socket() const;
        void SendingAllowed();
        TBool IsSending() const;
        void CancelSending();

    private: // New Methods

        void SetupL();
        void Teardown();

        void TestHandleNextFromSocketUserQueueL();
        void TestNextRequestIdL();
        void TestCreateBindingL();
        void TestFindSessionIndexL();
        void TestHandleRequestL();
        void TestCancelL();
        void TestRefreshNATBindingL();
        void TestRefreshTlsNATBindingL();
        void TestUpdateNextHopL();
        void TestSocketIdleL();
        void TestTlsSocketIdleL();
        void TestDataReceivedL();
        void TestFreeResourcesL();
        void TestRequestCompletedL();
        void TestSTUNClientInitCompletedL();
        void TestSTUNBindingEventOccurredL();
        void TestSTUNBindingErrorOccurredL();
        void TestSTUNBindingErrorOccurred2L();
        void TestAddToSendingQueueL();
        void TestSendingCompletedL();

    private: // Data

        EUNIT_DECLARE_TEST_TABLE;

        RSocketServ iSocketServ;
        RConnection iConnection;
        RSocket iSocket;
        TInetAddr iLocalAddr;
        TInetAddr iNextHopAddr;
        CNATTraversalController* iController;
        TUint32 iRequestId;
        TBool iIsSending;
        TInt iRequestCompletionCode;
    };

#endif // UT_CNATTRAVERSALCONTROLLER_H

// End of File
