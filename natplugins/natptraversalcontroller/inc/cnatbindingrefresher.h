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

#ifndef CNATBINDINGREFRESHER_H
#define CNATBINDINGREFRESHER_H

//  INCLUDES
#include <e32base.h>
#include <es_sock.h>
#include <securesocket.h>

// FORWARD DECLARATIONS
class MNATBindingRefresherObserver;

// CLASS DECLARATION
/**
* A base class for refreshing NAT bindings.
*/
class CNATBindingRefresher : public CBase
    {
    public:  // Constructors and destructor

        virtual ~CNATBindingRefresher();

    public: // New functions

        virtual TBool PersistentConnectionRequired() const;

        virtual TBool HasSocket( const RSocket& aSocket ) const = 0;

        virtual TBool HasSocket( const CSecureSocket& aSocket ) const;

        void SetRefresh( TBool aRefresh );

        void ContinueRefreshing();

        TBool IsRefreshed() const;

    public: // A callback for CDeltaTimer

        static TInt RefreshTimerExpired( TAny* aPtr );

    protected: // Constructors

        CNATBindingRefresher(
            CDeltaTimer& aDeltaTimer,
            MNATBindingRefresherObserver& aObserver );

    protected: // New functions

        virtual TInt RefreshInterval() = 0;

        virtual void RefreshTimerExpiredL() = 0;

        virtual void CancelRefresh() = 0;

    private: // New functions

        void StartTimer();
        void RefreshNow();

    protected: // Data

        MNATBindingRefresherObserver& iObserver;

    private: // Data

        CDeltaTimer& iDeltaTimer;
        TCallBack iDeltaTimerCallBack;
        TDeltaTimerEntry iDeltaTimerEntry;
        TBool iIsRefreshed;
        TBool iTimerRunning;
    };

#endif // CNATBINDINGREFRESHER_H

// End of File
