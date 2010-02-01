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




#include "cnatfwasynccallback.h"


// ---------------------------------------------------------------------------
// CNatFwAsyncCallback::NewL
// ---------------------------------------------------------------------------
//
CNatFwAsyncCallback* CNatFwAsyncCallback::NewL()
    {
    CNatFwAsyncCallback* self
        = new (ELeave) CNatFwAsyncCallback();
    CActiveScheduler::Add( self );
    self->WaitForRequestsL();
    return self;
    }


// ---------------------------------------------------------------------------
// CNatFwAsyncCallback::CNatFwAsyncCallback
// ---------------------------------------------------------------------------
//
CNatFwAsyncCallback::CNatFwAsyncCallback() :
    CActive( EPriorityStandard ),
    iPendingCallbacks( _FOFF( CNatFwCallbackInfo, iLink ) )
    {

    }


// ---------------------------------------------------------------------------
// CNatFwAsyncCallback::~CNatFwAsyncCallback
// ---------------------------------------------------------------------------
//
CNatFwAsyncCallback::~CNatFwAsyncCallback()
    {
    Cancel();

    CNatFwCallbackInfo* callback = iPendingCallbacks.First();
    while ( iPendingCallbacks.IsFirst( callback ) &&
            !iPendingCallbacks.IsEmpty() )
        {
        iPendingCallbacks.Remove( *callback );
        delete callback;
        callback = iPendingCallbacks.First();
        }
    }


// ---------------------------------------------------------------------------
// CNatFwAsyncCallback::DoCancel
// ---------------------------------------------------------------------------
//
void CNatFwAsyncCallback::DoCancel()
    {
    if ( iStatus == KRequestPending )
        {
        TRequestStatus* status = &iStatus;
        User::RequestComplete( status, KErrCancel );
        }
    }


// ---------------------------------------------------------------------------
// CNatFwAsyncCallback::RunL
// ---------------------------------------------------------------------------
//
void CNatFwAsyncCallback::RunL()
    {
    WaitForRequestsL();
    ExecuteFirstCallback();
    }


// ---------------------------------------------------------------------------
// CNatFwAsyncCallback::RunError
// Currently RunL cannot leave.
// ---------------------------------------------------------------------------
//
TInt CNatFwAsyncCallback::RunError( TInt aError )
    {
    if ( aError == KErrNoMemory )
        {
        return aError;
        }

    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CNatFwAsyncCallback::MakeCallbackL
// ---------------------------------------------------------------------------
//
void CNatFwAsyncCallback::MakeCallbackL(
    MNATFWRegistrationController& aController,
    CNatFwCallbackInfo::TFunction aFunction,
    TUint aSessionId,
    TUint aStreamId,
    TInt aErrorCode,
    TAny* aEventData )
    {
    CNatFwCallbackInfo* callback =
        new ( ELeave ) CNatFwCallbackInfo( aController,
                                           aFunction,
                                           aSessionId,
                                           aStreamId,
                                           aErrorCode,
                                           aEventData );
    CleanupStack::PushL( callback );
    __ASSERT_ALWAYS( callback->Validate(), User::Leave( KErrArgument ) );
    CleanupStack::Pop( callback );

    // CActive::iActive won't tell whether User::RequestComplete has already
    // been called, so iStatus is inspected to see whether a previous call to
    // AddDeleteRequestL has already called User::RequestComplete.

    if ( iStatus == KRequestPending )
        {
        Wakeup();
        }

    iPendingCallbacks.AddLast( *callback );
    }


// ---------------------------------------------------------------------------
// CNatFwAsyncCallback::CancelCallback
// ---------------------------------------------------------------------------
//
void CNatFwAsyncCallback::CancelCallback( TUint aStreamId )
    {
    TSglQueIter<CNatFwCallbackInfo> iter( iPendingCallbacks );

    for ( CNatFwCallbackInfo* callback = iter++; callback; callback = iter++ )
        {
        if ( callback->iStreamId == aStreamId )
            {
            iPendingCallbacks.Remove( *callback );
            delete callback;
            }
        }
    }


// ---------------------------------------------------------------------------
// CNatFwAsyncCallback::ExecuteFirstCallback
// Handle one callback request and wait for the next one. If there are more
// callbacks, complete the asynchronous request so that RunL will soon be
// called again.
// ---------------------------------------------------------------------------
//
void CNatFwAsyncCallback::ExecuteFirstCallback()
    {
    CNatFwCallbackInfo* callback = iPendingCallbacks.First();
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
// CNatFwAsyncCallback::WaitForRequestsL
// ---------------------------------------------------------------------------
//
void CNatFwAsyncCallback::WaitForRequestsL()
    {
    __ASSERT_ALWAYS( !IsActive(), User::Leave( KErrNotReady ) );

    iStatus = KRequestPending;
    SetActive();
    }


// ---------------------------------------------------------------------------
// CNatFwAsyncCallback::Wakeup
// ---------------------------------------------------------------------------
//
void CNatFwAsyncCallback::Wakeup()
    {
    TRequestStatus* status = &iStatus;
    User::RequestComplete( status, KErrNone );
    }
