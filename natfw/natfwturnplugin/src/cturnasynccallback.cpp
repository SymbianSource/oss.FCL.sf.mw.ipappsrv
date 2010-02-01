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




#include "mnatfwpluginobserver.h"
#include "cturnasynccallback.h"


// ---------------------------------------------------------------------------
// CTurnAsyncCallback::NewL
// ---------------------------------------------------------------------------
//
CTurnAsyncCallback* CTurnAsyncCallback::NewL(
    const CNATFWPluginApi& aTurnPlugin,
    MNATFWPluginObserver& aObserver )
    {
    CTurnAsyncCallback* self 
        = new (ELeave) CTurnAsyncCallback( aTurnPlugin, aObserver );
    CActiveScheduler::Add( self );
    self->WaitForRequestsL();
    return self;
    }

// ---------------------------------------------------------------------------
// CTurnAsyncCallback::CTurnAsyncCallback
// ---------------------------------------------------------------------------
//
CTurnAsyncCallback::CTurnAsyncCallback(
    const CNATFWPluginApi& aTurnPlugin, 
    MNATFWPluginObserver& aObserver )
    :
    CActive( EPriorityStandard ),
    iTurnPlugin( aTurnPlugin ),
    iObserver( aObserver ),
    iPendingCallbacks( _FOFF( TTurnPluginCallbackInfo, iLink ) )
    {
    }

// ---------------------------------------------------------------------------
// CTurnAsyncCallback::CTurnAsyncCallback
// Dummy implementation. Default constructor is declared private and not used.
// ---------------------------------------------------------------------------
//
CTurnAsyncCallback::CTurnAsyncCallback() :
    CActive( EPriorityStandard ),
    iTurnPlugin( *( CNATFWPluginApi* )0x1 ),
    iObserver( *( MNATFWPluginObserver* )0x1 )
    {
    }

// ---------------------------------------------------------------------------
// CTurnAsyncCallback::CTurnAsyncCallback
// Dummy implementation, as copy constructor is declared private and not used.
// ---------------------------------------------------------------------------
//
CTurnAsyncCallback::CTurnAsyncCallback(
    const CTurnAsyncCallback& aAsyncCallback ) :
    CActive( EPriorityStandard ),
    iTurnPlugin( *( CNATFWPluginApi* )0x1 ),
    iObserver( aAsyncCallback.iObserver )
    {
    }

// ---------------------------------------------------------------------------
// CTurnAsyncCallback::~CTurnAsyncCallback
// ---------------------------------------------------------------------------
//
CTurnAsyncCallback::~CTurnAsyncCallback()
    {
    Cancel();

    TTurnPluginCallbackInfo* callback = iPendingCallbacks.First();
    while ( iPendingCallbacks.IsFirst( callback ) &&
            !iPendingCallbacks.IsEmpty() )
        {
        iPendingCallbacks.Remove( *callback );
        delete callback;
        callback = iPendingCallbacks.First();
        }
    }

// ---------------------------------------------------------------------------
// CTurnAsyncCallback::DoCancel
// ---------------------------------------------------------------------------
//
void CTurnAsyncCallback::DoCancel()
    {
    if ( iStatus == KRequestPending )
        {
        TRequestStatus* status = &iStatus;
        User::RequestComplete( status, KErrCancel );
        }
    }

// ---------------------------------------------------------------------------
// CTurnAsyncCallback::RunL
// ---------------------------------------------------------------------------
//
void CTurnAsyncCallback::RunL()
    {
    WaitForRequestsL();
    ExecuteFirstCallback();
    }

// ---------------------------------------------------------------------------
// CTurnAsyncCallback::RunError
// Currently RunL cannot leave.
// ---------------------------------------------------------------------------
//
TInt CTurnAsyncCallback::RunError( TInt aError )
    {
    if ( aError == KErrNoMemory )
        {
        return aError;
        }
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CTurnAsyncCallback::MakeCallbackL
// ---------------------------------------------------------------------------
//
void CTurnAsyncCallback::MakeCallbackL(
    TTurnPluginCallbackInfo::TFunction aFunction,
    TUint aStreamId,
    TInt aErrorCode,
    TAny* aEventData )
    {
    TTurnPluginCallbackInfo* callback =
        new ( ELeave ) TTurnPluginCallbackInfo( iTurnPlugin,
                                                iObserver,
                                                aFunction,
                                                aStreamId,
                                                aErrorCode,
                                                aEventData );
    CleanupStack::PushL( callback );
    __ASSERT_ALWAYS( callback->Validate(), User::Leave( KErrArgument ) );
    CleanupStack::Pop( callback );

    // CActive::iActive won't tell whether User::RequestComplete has already
    // been called, so iStatus is inspected to see whether a previous call
    // to AddDeleteRequestL has already called User::RequestComplete.

    if ( iStatus == KRequestPending )
        {
        Wakeup();
        }

    iPendingCallbacks.AddLast( *callback );
    }

// ---------------------------------------------------------------------------
// CTurnAsyncCallback::CancelCallback
// ---------------------------------------------------------------------------
//
void CTurnAsyncCallback::CancelCallback( TUint aStreamId )
    {
    TSglQueIter<TTurnPluginCallbackInfo> iter( iPendingCallbacks );

    for ( TTurnPluginCallbackInfo* callback = iter++;
          callback; callback = iter++ )
        {
        if ( callback->iStreamId == aStreamId )
            {
            iPendingCallbacks.Remove( *callback );
            delete callback;
            }
        }
    }

// ---------------------------------------------------------------------------
// CTurnAsyncCallback::ExecuteFirstCallback
// Handle one callback request and wait for the next one. If there are more
// callbacks, complete the asynchronous request so that RunL will soon be 
// called again. Don't call more than one callback, since upper layer
// might've deleted STUN plugin, causing CTurnAsyncCallback to be deleted too.
// For the same reason nothing is done after executing the callback.
// ---------------------------------------------------------------------------
//    
void CTurnAsyncCallback::ExecuteFirstCallback()
    {
    TTurnPluginCallbackInfo* callback = iPendingCallbacks.First();
    if ( iPendingCallbacks.IsFirst( callback ) &&
         !iPendingCallbacks.IsEmpty() )
        {
        iPendingCallbacks.Remove( *callback );

        if ( !iPendingCallbacks.IsEmpty() )
            {
            Wakeup();
            }

        callback->Execute();
        delete callback;
        }
    }

// ---------------------------------------------------------------------------
// CTurnAsyncCallback::WaitForRequestsL
// ---------------------------------------------------------------------------
//
void CTurnAsyncCallback::WaitForRequestsL()
    {
    __ASSERT_ALWAYS( !IsActive(), User::Leave( KErrNotReady ) );

    iStatus = KRequestPending;
    SetActive();
    }

// ---------------------------------------------------------------------------
// CTurnAsyncCallback::Wakeup
// ---------------------------------------------------------------------------
//
void CTurnAsyncCallback::Wakeup()
    {
    TRequestStatus* status = &iStatus;
    User::RequestComplete( status, KErrNone );
    }

// End of file
