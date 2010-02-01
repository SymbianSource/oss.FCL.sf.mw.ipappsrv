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

#ifndef CNATBINDINGSTUNANDCRLFREFRESHER_H
#define CNATBINDINGSTUNANDCRLFREFRESHER_H

//  INCLUDES
#include "cnatbindingstunrefresher.h"
#include "mnatbindingrefresherobserver.h"

// FORWARD DECLARATIONS
class CUdpCRLFSender;
class MNATTraversalSocketManager;

// CLASS DECLARATION
/**
* A class for STUN based NAT binding refresh.
*/
class CNATBindingSTUNAndCRLFRefresher : public CNATBindingSTUNRefresher,
                                        public MNATBindingRefresherObserver
    {
    public:  // Constructors and destructor

        static CNATBindingSTUNAndCRLFRefresher* NewL(
            CDeltaTimer& aDeltaTimer,
            TInt aRefreshInterval,
            MNATBindingRefresherObserver& aObserver,
            CNATBinding& aNATBinding,
            RSocket& aSocket,
            const TInetAddr& aAddr,
            MNATTraversalSocketManager& aSocketManager );

        static CNATBindingSTUNAndCRLFRefresher* NewLC(
            CDeltaTimer& aDeltaTimer,
            TInt aRefreshInterval,
            MNATBindingRefresherObserver& aObserver,
            CNATBinding& aNATBinding,
            RSocket& aSocket,
            const TInetAddr& aAddr,
            MNATTraversalSocketManager& aSocketManager );

        ~CNATBindingSTUNAndCRLFRefresher();

    public: // From MNATBindingRefresherObserver

        void NATBindingRefreshComplete( TInt aCompletionCode );

    protected: // From CNATBindingRefresher

        void RefreshTimerExpiredL();

        void CancelRefresh();

    private: // Constructors

        CNATBindingSTUNAndCRLFRefresher(
            CDeltaTimer& aDeltaTimer,
            TInt aRefreshInterval,
            MNATBindingRefresherObserver& aObserver,
            CNATBinding& aNATBinding );

         void ConstructL(
            RSocket& aSocket,
            const TInetAddr& aAddr,
            MNATTraversalSocketManager& aSocketManager );

    private: // Data

        CUdpCRLFSender* iCRLFSender;
    };

#endif // CNATBINDINGSTUNANDCRLFREFRESHER_H

// End of File
