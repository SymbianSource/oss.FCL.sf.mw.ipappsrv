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


#ifndef UT_CNATTRAVERSALSESSION_H
#define UT_CNATTRAVERSALSESSION_H

#include "sipnattraversalrequestobserver.h"
#include "sipnatbindingobserver.h"
#include "nattraversalsocketmanager.h"

#include <es_sock.h>
#include "natfwstunclientobserver.h"
#include "natfwbindingbase.h"

#include <digia/eunit/ceunittestsuiteclass.h>

#include <e32def.h>
#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif

class CNATTraversalSession;


NONSHARABLE_CLASS(UT_CNATTraversalSession) :
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
        static UT_CNATTraversalSession* NewL();
        static UT_CNATTraversalSession* NewLC();

        /**
         * Destructor
         */
        ~UT_CNATTraversalSession();

    private:    // Constructors and destructors

        UT_CNATTraversalSession();
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
        void Teardown();
        void TestConstructionL();
        void TestBasicSettersAndGettersL();
        void TestIsPrivateAddressL();
        void TestIsBehindNATL();
        void TestCreateRefresherL();
        void TestInitCompletedL();
        void TestInitFailedL();
        void TestAddressResolvedL();
        void TestBindingFailedL();
        void TestRefreshNATBindingL();
        void TestRefreshTlsNATBindingL();
        void TestSocketIdleL();
        void TestDataReceivedL();
        void TestRemoveBindingL();

		void PrepareCentralRepositoryL(const TDesC8& aDomain);

    private: // Data

        EUNIT_DECLARE_TEST_TABLE;

        CDeltaTimer* iDeltaTimer;
        RSocketServ iSocketServ;
        RConnection iConnection;
        RSocket iSocket;
        TInt iLastFlowFailure;
        CNATTraversalSession* iSession;
    };

#endif // UT_CNATTRAVERSALSESSION_H

// End of File
