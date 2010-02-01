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




#ifndef C_STUNASYNCCALLBACK_H
#define C_STUNASYNCCALLBACK_H

// INCLUDES
#include <e32base.h>
#include "tstunplugincallbackinfo.h"

// FORWARD DECLARATIONS
class MNATFWPluginObserver;


// CLASS DECLARATION
/**
 * CStunAsyncCallback is an active object used to guarantee that when the
 * function calls to the MNATFWPluginObserver are made, the upper layer 
 * can delete any STUN plugin related object it owns.
 */
class CStunAsyncCallback : public CActive
    {
    
    friend class UT_CNATFWStunConnectionHandler;
    
public: // Constructor and destructor

    /**
     * Two-phased constructor.
     * @param aStunPlugin The STUN plugin instance
     * @param aObserver Observer implementing the callback functions
     */
    static CStunAsyncCallback* NewL( 
        const CNATFWPluginApi& aStunPlugin, 
        MNATFWPluginObserver& aObserver );

    /**     
     * Frees the resources of CStunAsyncCallback. Any issued callback 
     * request that have not yet been executed, are discarded.
     *
     * @since   s60 3.2
     */
    ~CStunAsyncCallback();

public: // New functions

    /**
     * Orders a specified callback function to be called, and provides all
     * necessary parameters.
     *
     * @since   s60 3.2     
     * @param   aFunction Identifies the function of MNATFWPluginObserver to 
     *          use
     * @param   aStreamId   Stream ID
     * @param   aErrorCode  Error reason
     * @param   aEventData  Event data
     */
    void MakeCallbackL( TStunPluginCallbackInfo::TFunction aFunction,
                        TUint aStreamId,
                        TInt aErrorCode,
                        TAny* aEventData );

    /**
     * Removes all pending callbacks, which have the specified stream id as a
     * parameter.
     *
     * @since   s60 3.2
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

    CStunAsyncCallback();

    CStunAsyncCallback( const CNATFWPluginApi& aStunPlugin, 
        MNATFWPluginObserver& aObserver );

    CStunAsyncCallback( const CStunAsyncCallback& aAsyncCallback );

private: // New functions, for internal use

    /**
     * Starts waiting for MakeCallbackL events.
     *
     * @since   s60 3.2
     */
    void WaitForRequestsL();
    
    /**
     * Wakes system up.
     *
     * @since   s60 3.2
     */
    void Wakeup();

    /**
     * Calls the oldest callback entry, if any.
     *
     * @since   s60 3.2
     */
    void ExecuteFirstCallback();
    
private: // Data

    /**
     * STUN plugin instance
     */
    const CNATFWPluginApi& iStunPlugin;

    /**
     * Implements the callback functions
     */
    MNATFWPluginObserver& iObserver;

    /**
     * Callback requests that have not yet been executed
     */
    TSglQue<TStunPluginCallbackInfo> iPendingCallbacks;
    };

#endif // C_STUNASYNCCALLBACK_H

