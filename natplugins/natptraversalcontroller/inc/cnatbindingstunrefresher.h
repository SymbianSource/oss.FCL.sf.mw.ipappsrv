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

#ifndef CNATBINDINGSTUNREFRESHER_H
#define CNATBINDINGSTUNREFRESHER_H

//  INCLUDES
#include "cnatbindingrefresher.h"

// FORWARD DECLARATIONS
class CNATBinding;

// CLASS DECLARATION
/**
* A class for STUN based NAT binding refresh.
*/
class CNATBindingSTUNRefresher : public CNATBindingRefresher
    {
    public:  // Constructors and destructor

        CNATBindingSTUNRefresher(
            CDeltaTimer& aDeltaTimer,
            TInt aRefreshInterval,
            MNATBindingRefresherObserver& aObserver,
            CNATBinding& aNATBinding );

        ~CNATBindingSTUNRefresher();

    public: // From CNATBindingRefresher

        TBool HasSocket(const RSocket& aSocket) const;

    protected: // From CNATBindingRefresher

        TInt RefreshInterval();

        virtual void RefreshTimerExpiredL();

        virtual void CancelRefresh();

    protected: // Data

        CNATBinding& iNATBinding;

    private: // Data

        TInt iRefreshInterval;
        TInt64 iRandomSeed;

    friend class UT_CNATBindingSTUNRefresher;
    };

#endif // CNATBINDINGSTUNREFRESHER_H

// End of File
