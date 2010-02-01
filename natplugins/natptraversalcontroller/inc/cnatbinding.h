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

#ifndef CNATBINDING_H
#define CNATBINDING_H

//  INCLUDES
#include <in_sock.h>
#include <e32base.h>
#include "mnatbindingrefresherobserver.h"
#include "nattraversalsocketuser.h"

// FORWARD DECLARATIONS
class CSTUNBinding;
class MSIPNATTraversalRequestObserver;
class MSIPNATBindingObserver;
class MNATTraversalSocketManager;
class CNATBindingRefresher;

// CLASS DECLARATION
/**
* NAT binding.
*/
class CNATBinding : public CBase,
                    public MNATBindingRefresherObserver,
                    public MNATTraversalSocketUser
    {
    public:  // Constructors and destructor

        static CNATBinding* NewL(
            TUint32 aInitialRequestId,
            RSocket& aSTUNSocket,
            const TInetAddr& aLocalAddr,
            MSIPNATTraversalRequestObserver& aInitialRequestObserver,
            MSIPNATBindingObserver& aBindingObserver,
            MNATTraversalSocketManager& aSocketManager);

        static CNATBinding* NewLC(
            TUint32 aInitialRequestId,
            RSocket& aSTUNSocket,
            const TInetAddr& aLocalAddr,
            MSIPNATTraversalRequestObserver& aInitialRequestObserver,
            MSIPNATBindingObserver& aBindingObserver,
            MNATTraversalSocketManager& aSocketManager);

        ~CNATBinding();

    public: // New functions

        TBool PersistentConnectionRequired() const;

        RSocket& STUNSocket();

        void SetBinding(CSTUNBinding* aBinding);

        CSTUNBinding* Binding();

        void SendSTUNRequestL();

        void SetBindingFailed(TInt aError);

        TBool SetPublicAddr(const TInetAddr& aPublicAddr);

        void SetRefresher(CNATBindingRefresher* aRefresher);

        CNATBindingRefresher* Refresher();

        TUint32 InitialRequestId() const;

        MSIPNATBindingObserver& NATBindingObserver();

        TBool AddressResolved() const;

        const TInetAddr& PublicAddr() const;

        void HandleDataL(const TDesC8& aData,
                         const RSocket& aSocket,
                         TBool& aConsumed);

        void ContinueRefreshing();

    public: // From MNATBindingRefresherObserver

        void NATBindingRefreshComplete(TInt aCompletionCode);

    public: // MNATTraversalSocketUser

        const RSocket& Socket() const;

        void SendingAllowed();

        TBool IsSending() const;

        void CancelSending();

    private: // Constructors

        CNATBinding(
            TUint32 aInitialRequestId,
            RSocket& aSocket,
            const TInetAddr& aLocalAddr,
            MSIPNATTraversalRequestObserver& aInitialRequestObserver,
            MSIPNATBindingObserver& aBindingObserver,
            MNATTraversalSocketManager& aSocketManager);

    private: // New functions

        void SendingCompleted();

    private: // Data

        TUint32 iInitialReqId;
        RSocket& iSTUNSocket;
        MSIPNATTraversalRequestObserver& iInitialReqObserver;
        MSIPNATBindingObserver& iBindingObserver;
        MNATTraversalSocketManager& iSocketManager;
        TInetAddr iPublicAddr;
        TBool iAddressResolved;
        CSTUNBinding* iBinding; // owned
        CNATBindingRefresher* iRefresher; // owned
        TBool iIsSending;

    };

#endif // CNATBINDING_H

// End of File
