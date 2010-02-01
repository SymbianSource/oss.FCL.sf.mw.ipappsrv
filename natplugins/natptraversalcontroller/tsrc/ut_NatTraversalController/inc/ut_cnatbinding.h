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


#ifndef UT_CNATBINDING_H
#define UT_CNATBINDING_H

#include "sipnattraversalrequestobserver.h"
#include "sipnatbindingobserver.h"
#include "nattraversalsocketmanager.h"

#include "natfwstunclientobserver.h"
#include <es_sock.h>
#include <in_sock.h>

#include <digia/eunit/ceunittestsuiteclass.h>

#include <e32def.h>
#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif

class CSTUNClient;
class CNATBinding;

NONSHARABLE_CLASS(UT_CNATBinding) :
    public CEUnitTestSuiteClass,
    public MSTUNClientObserver,
    public MSIPNATTraversalRequestObserver,
    public MSIPNATBindingObserver,
    public MNATTraversalSocketManager
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CNATBinding* NewL();
        static UT_CNATBinding* NewLC();

        /**
         * Destructor
         */
        ~UT_CNATBinding();

    private:    // Constructors and destructors

        UT_CNATBinding();
        void ConstructL();

    public: // From MSTUNClientObserver

        void STUNClientInitCompleted( const CSTUNClient& aClient,
                                      TInt aCompletionCode );

        void STUNBindingEventOccurredL( TSTUNBindingEvent aEvent,
                                        const CBinding& aBinding );

        void STUNBindingErrorOccurred( const CBinding& aBinding,
                                       TInt aError );

    public: // MSIPNATTraversalRequestObserver

        void RequestComplete(
            TUint32 aRequestId,
            TInt aCompletionCode );

        void PublicAddrResolved(
            TUint32 aRequestId,
            const TInetAddr& aPublicAddr );

    public: // MSIPNATBindingObserver

        void FlowFailure( TInt aError );

    public: // From MNATTraversalSocketManager

        TBool AddToSendingQueueL( MNATTraversalSocketUser& aUser );

        void SendingCompleted( MNATTraversalSocketUser& aUser );

    private: // New Methods

        void SetupL();
        void TeardownL();
        void TestBasicSettersAndGettersL();
        void TestSendSTUNRequestL();
        void TestPublicAddrL();
        void TestBindingFailedL();
        void TestHandleDataL();
        void TestNATBindingRefreshCompleteL();

    private: // Data

        EUNIT_DECLARE_TEST_TABLE;

        TInetAddr iLocalAddr;
        CDeltaTimer* iDeltaTimer;
        RSocketServ iSocketServ;
        RConnection iConnection;
        RSocket iSocket;
        CSTUNClient* iSTUNClient;
        CNATBinding* iBinding;
        TBool iRequestCompleteCalled;
        TInt iRequestCompleteCode;
        TBool iPublicAddrResolvedCalled;
        TInt iLastFlowFailure;
        TBool iSendingNotAllowed;
        TBool iSendingCompletedCalled;
    };

#endif // UT_CNATBINDING_H

// End of File
