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

#ifndef CNATBINDINGCRLFREFRESHER_H
#define CNATBINDINGCRLFREFRESHER_H

//  INCLUDES
#include "cnatbindingrefresher.h"

// FORWARD DECLARATIONS
class CCRLFSender;

// CLASS DECLARATION
/**
* A class for refreshing NAT bindings using CRLF bursts
* over connection oriented transports like TCP and TLS.
*/
class CNATBindingCRLFRefresher : public CNATBindingRefresher
    {
    public:  // Constructors and destructor

        CNATBindingCRLFRefresher(
            CDeltaTimer& aDeltaTimer,
            TInt aRefreshInterval,
            MNATBindingRefresherObserver& aObserver,
            CCRLFSender* aSender );

        ~CNATBindingCRLFRefresher();

    public: // From CNATBindingRefresher

        TBool PersistentConnectionRequired() const;

        TBool HasSocket(const RSocket& aSocket) const;

        TBool HasSocket(const CSecureSocket& aSocket) const;

    protected: // From CNATBindingRefresher

        TInt RefreshInterval();

        void RefreshTimerExpiredL();

        void CancelRefresh();

    private: // Data

        TInt iRefreshInterval;
        CCRLFSender* iSender; // owned
    };

#endif // CNATBINDINGCRLFREFRESHER_H

// End of File
