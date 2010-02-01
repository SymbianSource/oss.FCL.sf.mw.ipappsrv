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

#ifndef TNATTRAVERSALPENDINGREQUEST_H
#define TNATTRAVERSALPENDINGREQUEST_H

// INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class MSIPNATTraversalRequestObserver;
class MNATTraversalPendingRequestContainer;


class TNATTraversalPendingRequest
    {
    public: // Constructors

        TNATTraversalPendingRequest(
            const TUint32& aRequestId,
            MSIPNATTraversalRequestObserver* aObserver,
            MNATTraversalPendingRequestContainer& aContainer);

    public: // New functions

        const TUint32& RequestId() const;
        TDeltaTimerEntry& DeltaTimerEntry();
        void CompleteRequest();

        static TInt TimerExpired(TAny* aPtr);

        static TInt RequestOrder(
            const TNATTraversalPendingRequest& aFirst,
            const TNATTraversalPendingRequest& aSecond);

    private: // New functions

        void RequestCompleted();

    private: // Data

        TUint32 iRequestId;
        MSIPNATTraversalRequestObserver* iObserver;
        MNATTraversalPendingRequestContainer& iContainer;
        TCallBack iDeltaTimerCallBack;
        TDeltaTimerEntry iDeltaTimerEntry;
    };

#endif // TNATTRAVERSALPENDINGREQUEST_H

// End of File
