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




#ifndef C_NATFWASYNCCALLBACK_H
#define C_NATFWASYNCCALLBACK_H

// INCLUDES
#include <e32base.h>
#include "natfwcallbackinfo.h"

// CLASS DECLARATION
/**
 * CNatFwAsyncCallback is an active object used to guarantee that when the
 * function calls to the MNATFWConnectivityObserver are made, the upper layer
 * can delete any plugin related object it owns.
 */
NONSHARABLE_CLASS( CNatFwAsyncCallback ) : public CActive
    {
public: // Constructor and destructor

    /**
     * Two-phased constructor.
     */
    static CNatFwAsyncCallback* NewL();

    /**
     * Frees the resources of CNatFwAsyncCallback. Any issued callback
     * request that have not yet been executed, are discarded.
     *
     * @since   s60 3.2
     */
    ~CNatFwAsyncCallback();

public: // New functions

    /**
     * Orders a specified callback function to be called, and provides all
     * necessary parameters.
     *
     * @since   s60 3.2
     * @param   aController Event registration controller
     * @param   aFunction   Identifies the function of
     *                      MNATFWConnectivityObserver to use
     * @param   aSessionId  Session ID
     * @param   aStreamId   Stream ID
     * @param   aErrorCode  Error reason
     * @return  void
     */
    void MakeCallbackL( MNATFWRegistrationController& aController,
                        CNatFwCallbackInfo::TFunction aFunction,
                        TUint aSessionId,
                        TUint aStreamId,
                        TInt aErrorCode,
                        TAny* aEventData );

    /**
     * Removes all pending callbacks, which have the specified
     * stream ID as a parameter.
     *
     * @since   s60 3.2
     * @param   aStreamId
     * @return  void
     */
    void CancelCallback( TUint aStreamId );

protected: // From CActive

    void DoCancel();

    void RunL();

    TInt RunError( TInt aError );

private: // Constructors

    CNatFwAsyncCallback();

private: // New functions, for internal use

    /**
     * Starts waiting for MakeCallbackL events.
     *
     * @since   s60 3.2
     * @return  void
     */
    void WaitForRequestsL();

    /**
     * Wakes system up.
     *
     * @since   s60 3.2
     * @return  void
     */
    void Wakeup();

    /**
     * Calls the oldest callback entry, if any.
     *
     * @since   s60 3.2
     * @return  void
     */
    void ExecuteFirstCallback();

private: // Data

    /**
     * Callback requests that have not yet been executed
     */
    TSglQue<CNatFwCallbackInfo> iPendingCallbacks;
    };

#endif // C_NATFWASYNCCALLBACK_H
