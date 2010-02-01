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




#ifndef C_TURNASYNCCALLBACK_H
#define C_TURNASYNCCALLBACK_H

// INCLUDES
#include <e32base.h>
#include "tturnplugincallbackinfo.h"

// FORWARD DECLARATIONS
class MNATFWPluginObserver;


// CLASS DECLARATION
/**
 * CTurnAsyncCallback is an active object used to guarantee that when the
 * function calls to the MNATFWPluginObserver are made, the upper layer
 * can delete any TURN Plug-in related object it owns.
 */
class CTurnAsyncCallback : public CActive
    {

    friend class UT_CNATFWTurnConnectionHandler;

public: // Constructor and destructor

    /**
     * Two-phased constructor.
     * @param aTurnPlugin The TURN Plug-in instance
     * @param aObserver Observer implementing the callback functions
     */
    static CTurnAsyncCallback* NewL(
        const CNATFWPluginApi& aTurnPlugin,
        MNATFWPluginObserver& aObserver );

    /**
     * Frees the resources of CTurnAsyncCallback. Any issued callback
     * request that have not yet been executed, are discarded.
     *
     * @since   S60 3.2
     */
    ~CTurnAsyncCallback();

public: // New functions

    /**
     * Orders a specified callback function to be called, and provides all
     * necessary parameters.
     *
     * @since   S60 3.2
     * @param   aFunction Identifies the function of MNATFWPluginObserver
     *          to use
     * @param   aStreamId   Stream ID
     * @param   aErrorCode  Error reason
     * @param   aEventData  Event data
     */
    void MakeCallbackL( TTurnPluginCallbackInfo::TFunction aFunction,
                        TUint aStreamId,
                        TInt aErrorCode,
                        TAny* aEventData );

    /**
     * Removes all pending callbacks, which have the specified stream ID
     * as a parameter.
     *
     * @since   S60 3.2
     * @param   aStreamId   Stream ID
     */
    void CancelCallback( TUint aStreamId );

protected: // From CActive

    /**
     * from CActive
     */
    void DoCancel();

    /**
     * from CActive
     */
    void RunL();

    /**
     * from CActive
     */
    TInt RunError( TInt aError );

private: // Constructors

    CTurnAsyncCallback();

    CTurnAsyncCallback( const CNATFWPluginApi& aTurnPlugin,
        MNATFWPluginObserver& aObserver );

    CTurnAsyncCallback( const CTurnAsyncCallback& aAsyncCallback );

private: // New functions, for internal use

    /**
     * Starts waiting for MakeCallbackL events.
     *
     * @since   S60 3.2
     */
    void WaitForRequestsL();

    /**
     * Wakes system up.
     *
     * @since   S60 3.2
     */
    void Wakeup();

    /**
     * Calls the oldest callback entry, if any.
     *
     * @since   S60 3.2
     */
    void ExecuteFirstCallback();

private: // Data

    /**
     * TURN Plug-in instance
     */
    const CNATFWPluginApi& iTurnPlugin;

    /**
     * Implements the callback functions
     */
    MNATFWPluginObserver& iObserver;

    /**
     * Callback requests that have not yet been executed
     */
    TSglQue<TTurnPluginCallbackInfo> iPendingCallbacks;
    };

#endif // C_TURNASYNCCALLBACK_H
