/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef CNATTRAVERSALCONTROLLER_H
#define CNATTRAVERSALCONTROLLER_H

// INCLUDES
#include "natfwstunclientobserver.h"
#include "sipnattraversalcontroller.h"
#include "mnattraversalpendingrequestcontainer.h"
#include "tnattraversalpendingrequest.h"
#include "nattraversalsocketmanager.h"

// FORWARD DECLARATIONS
class CNATTraversalSession;
class CNATTraversalConfig;
class MNATTraversalSocketUser;

// CLASS DEFINITION
/**
* Default implementation of NAT Traversal Controller ECOM plug-in.
*/
class CNATTraversalController :
    public CSIPNATTraversalController,
    public MNATTraversalPendingRequestContainer,
    public MSTUNClientObserver,
    public MNATTraversalSocketManager
    {
    public: // Constructors and destructor

        static CNATTraversalController* NewL(TAny* aInitParams);

        ~CNATTraversalController ();

    public: // From CSIPNATTraversalController

        TUint32 ResolvePublicAddrL(
            TUint32 aIapId,
            RConnection& aConnection,
            const TInetAddr& aLocalAddr,
            const TDesC8& aDomain,
            RSocket& aSocket,
            const TInetAddr& aNextHopAddr,
            MSIPNATBindingObserver* aBindingObserver,
            MSIPNATTraversalRequestObserver& aRequestObserver );

        void Cancel( TUint32 aRequestId );

        TBool RefreshNATBindingL(
            RSocket& aSocket,
            const MSIPNATBindingObserver* aBindingObserver );

        TBool RefreshNATBindingL(
            CSecureSocket& aSecureSocket,
            const MSIPNATBindingObserver* aBindingObserver );

        void UpdateNextHop(
            const TInetAddr& aNextHopAddr,
            const MSIPNATBindingObserver* aBindingObserver );

        void SocketIdle(
            TBool aIdle,
            RSocket& aSocket );

        void SocketIdle(
            TBool aIdle,
            CSecureSocket& aSecureSocket );

        void DataReceivedL(
            const TDesC8& aData,
            const RSocket& aUdpSocket,
            TBool& aHandled );

        void FreeResources( TUint32 aIapId );

        void FreeResources( MSIPNATBindingObserver& aBindingObserver );

    public: // From MNATTraversalPendingRequestContainer

        void RequestCompleted( TUint32 aRequestId );

    public: // From MSTUNClientObserver

        void STUNClientInitCompleted( const CSTUNClient& aClient,
                                      TInt aCompletionCode );

        void STUNBindingEventOccurredL( TSTUNBindingEvent aEvent,
                                        const CBinding& aBinding );

        void STUNBindingErrorOccurred( const CBinding& aBinding,
                                       TInt aError );

    public: // From MNATTraveralSocketManager

        TBool AddToSendingQueueL( MNATTraversalSocketUser& aUser );

        void SendingCompleted( MNATTraversalSocketUser& aUser );

    private: // Constructors

        CNATTraversalController( RSocketServ& aSocketServ );
        void ConstructL();

    private: // New functions

        TUint32 NextRequestId();

        void CreateBindingL(
            TUint32 aRequestId,
            CNATTraversalConfig* aConfig,
            RConnection& aConnection,
            const TInetAddr& aLocalAddr,
            RSocket& aUdpSocket,
            const TInetAddr& aNextHopAddr,
            MSIPNATBindingObserver& aBindingObserver,
            MSIPNATTraversalRequestObserver& aRequestObserver,
            TBool& aAsyncInitiated );

        TInt FindSessionIndex(
            TUint32 aIapId,
            const TDesC8& aDomain ) const;

        TBool BindingExists(
            const MSIPNATBindingObserver* aObserver ) const;

        static void DetachConfigFromSession(TAny* aSession);

        void HandleNextFromSocketUserQueue();

    private: // Data

        RSocketServ& iSocketServ;
        CDeltaTimer* iDeltaTimer;
        RArray<TNATTraversalPendingRequest> iPendingRequests;
        TLinearOrder<TNATTraversalPendingRequest> iRequestOrder;
        TUint32 iRequestIdCounter;
        RPointerArray<CNATTraversalSession> iSessions;
        RPointerArray<MNATTraversalSocketUser> iSocketUserQueue;

    friend class UT_CNATTraversalController;
    };

#endif // CNATTRAVERSALCONTROLLER_H
