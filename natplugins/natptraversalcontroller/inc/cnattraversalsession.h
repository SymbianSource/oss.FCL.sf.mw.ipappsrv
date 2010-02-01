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

#ifndef CNATTRAVERSALSESSION_H
#define CNATTRAVERSALSESSION_H

//  INCLUDES
#include <e32base.h>
#include <es_sock.h>
#include <in_sock.h>
#include <securesocket.h>
#include <uri8.h>
#include <stringpool.h>
#include "natfwstunclient.h"
#include "natfwstunbinding.h"

// FORWARD DECLARATIONS
//class CSIPHeaderBase;
class MSTUNClientObserver;
class MSIPNATBindingObserver;
class MSIPNATTraversalRequestObserver;
class MNATTraversalSocketManager;
class CNATTraversalConfig;
class CNATBinding;
class CNATBindingRefresher;

// CLASS DECLARATION
/**
* NAT Traversal session
*/
class CNATTraversalSession : public CBase
    {
    public:  // Constructors and destructor

        static CNATTraversalSession* NewL(
            CDeltaTimer& aDeltaTimer,
            RSocketServ& aSocketServ,
            RConnection& aConnection,
            const TInetAddr& aLocalAddr,
            const TInetAddr& aNextHopAddr,
            CNATTraversalConfig* aConfig,
            MSTUNClientObserver& aObserver,
            MNATTraversalSocketManager& aSocketManager );

        static CNATTraversalSession* NewLC(
            CDeltaTimer& aDeltaTimer,
            RSocketServ& aSocketServ,
            RConnection& aConnection,
            const TInetAddr& aLocalAddr,
            const TInetAddr& aNextHopAddr,
            CNATTraversalConfig* aConfig,
            MSTUNClientObserver& aObserver,
            MNATTraversalSocketManager& aSocketManager );

        ~CNATTraversalSession();

    public: // New functions

        TBool HasSTUNClient(const CSTUNClient& aClient) const;

        const TDesC8& Domain() const;

        TUint32 IapId() const;

        void UpdateNextHop(const TInetAddr& aNextHopAddr);

        TBool HasBindings() const;

        TBool HasBinding(const MSIPNATBindingObserver* aObserver) const;

        void InitCompletedL();

        void InitFailed( TInt aError );

        TBool AddressResolvedL( const CBinding& aBinding );

        TBool BindingFailed(
            const CBinding& aBinding,
            TInt aError );

        CNATBinding& CreateNATBindingL(
            TUint32 aRequestId,
            RSocket& aSocket,
            MSIPNATBindingObserver& aBindingObserver,
            MSIPNATTraversalRequestObserver& aRequestObserver,
            TBool& aAsyncCallInitiated );

        void RefreshNATBindingL(
            RSocket& aSocket,
            const MSIPNATBindingObserver* aBindingObserver,
            TBool& aMaintainPersistentConnection,
            TBool& aHandled );

        void RefreshNATBindingL(
            CSecureSocket& aSecureSocket,
            const MSIPNATBindingObserver* aBindingObserver,
            TBool& aMaintainPersistentConnection,
            TBool& aHandled );

        void SocketIdle(
            TBool aIdle,
            RSocket& aSocket );

        void SocketIdle(
            TBool aIdle,
            CSecureSocket& aSocket );

        void DataReceivedL(
            const TDesC8& aData,
            const RSocket& aSocket,
            TBool& aHandled );

        TBool RemoveBinding( TUint32 aRequestId );

        TBool RemoveBinding( const MSIPNATBindingObserver& aObserver );

        void DetachConfig();

    private: // Second phase constructors

        CNATTraversalSession(
            const TInetAddr& aLocalAddr,
            const TInetAddr& aNextHopAddr,
            CDeltaTimer& aDeltaTimer,
            MNATTraversalSocketManager& aSocketManager );

        void ConstructL(
            RSocketServ& aSocketServ,
            RConnection& aConnection,
            CNATTraversalConfig* aConfig,
            MSTUNClientObserver& aObserver );

    private: // New functions

        TUint Protocol( RSocket& aSocket ) const;

        TInt RefreshInterval( TUint aProtocol ) const;

        TInt FindBindingIndex(
            const MSIPNATBindingObserver* aBindingObserver ) const;

        void CreateRefresherL(
            CNATBinding& aBinding,
            RSocket& aSocket,
            TBool& aMaintainPersistentConnection );

        template<class T> void SocketIdleImpl( TBool aIdle, T& aSocket );

        TBool IsBehindNAT(const CNATBinding& aBinding) const;

        TBool IsPrivateAddress(const TInetAddr& aAddr) const;

    private: // Data

        TInetAddr iLocalAddr;
        TInetAddr iNextHopAddr;
        CDeltaTimer& iDeltaTimer;
        CNATTraversalConfig* iConfig;
        CSTUNClient* iSTUNClient;
        MNATTraversalSocketManager& iSocketManager;
        RPointerArray<CNATBinding> iBindings;

    friend class UT_CNATTraversalSession;
    friend class UT_CNATTraversalController;
    };

#endif // CNATTRAVERSALSESSION_H

// End of File
