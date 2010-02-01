/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef C_ASYNCCALLBACK_H
#define C_ASYNCCALLBACK_H

// INCLUDES
#include <e32base.h>
#include "tstuncallbackinfo.h"

// FORWARD DECLARATIONS
class MSTUNClientObserver;
class CBinding;

// CLASS DECLARATION
/*
 * CAsyncCallback is an active object used to guarantee that when the function
 * calls to the MSTUNClientObserver are made, the upper layer can delete any
 * STUN client related object it owns.
 */
class CAsyncCallback : public CActive
    {
public: // Constructor and destructor

    /**
     * Creates a new instance of CAsyncCallback.
     *
     * @param aObserver Observer implementing the callback functions
     * @return value New CAsyncCallback object. Ownership is transferred.
     */
    static CAsyncCallback* NewL( MSTUNClientObserver& aObserver );

    /**     
     * Frees the resources of CAsyncCallback. Any issued callback request that
     * have not yet been executed, are discarded.
     */
    ~CAsyncCallback();

public: // From CActive

    void DoCancel();

    void RunL();

    TInt RunError( TInt aError );

public: // New functions

    /**
     * Orders a specified callback function to be called, and provides all
     * necessary parameters.
     *     
     * @param aFunction Identifies the function of MSTUNClientObserver to use
     * @param aBinding STUN binding
     * @param aErrorCode Error reason
     * @param aClient STUN client     
     */
    void MakeCallbackL( TSTUNCallbackInfo::TFunction aFunction,
                        const CBinding* aBinding,
                        TInt aErrorCode = KErrNone,
                        const CSTUNClient* aClient = NULL );

    /**
     * Removes all pending callbacks, which have the specified binding as a
     * parameter.
     *
     * @param aBinding STUN binding     
     */
    void CancelCallback( const CBinding& aBinding );

    /**
     * Calls the oldest callback entry, if any.
     *
     * @param aBinding STUN binding     
     */
    void ExecuteFirstCallback();

private: // Constructors

    CAsyncCallback();

    CAsyncCallback( MSTUNClientObserver& aObserver );

    CAsyncCallback( const CAsyncCallback& aAsyncCallback );

private: // New functions, for internal use

    /**
     * Starts waiting for MakeCallbackL events.
     */
    void WaitForRequests();

    void Wakeup();

private: // Data

    // Implements the callback functions
    MSTUNClientObserver& iObserver;

    // Callback requests that have not yet been executed
    TSglQue<TSTUNCallbackInfo> iPendingCallbacks;

private: // For testing purposes

#ifdef TEST_EUNIT
    friend class CAsyncCallbackTest;
    friend class CSTUNClientTest;
#endif
    };

#endif // end of C_ASYNCCALLBACK_H

// End of File
