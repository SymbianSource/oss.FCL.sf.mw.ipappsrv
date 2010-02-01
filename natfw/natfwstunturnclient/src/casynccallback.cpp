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




#include "natfwstunclientobserver.h"
#include "casynccallback.h"
#include "stunassert.h"

// -----------------------------------------------------------------------------
// CAsyncCallback::NewL
// -----------------------------------------------------------------------------
//
CAsyncCallback* CAsyncCallback::NewL( MSTUNClientObserver& aObserver )
    {
    CAsyncCallback* self = new (ELeave) CAsyncCallback( aObserver );
    CActiveScheduler::Add( self );
    self->WaitForRequests();
    return self;
    }

// -----------------------------------------------------------------------------
// CAsyncCallback::CAsyncCallback
// -----------------------------------------------------------------------------
//
CAsyncCallback::CAsyncCallback( MSTUNClientObserver& aObserver ) :
    CActive( EPriorityStandard ),
    iObserver( aObserver ),
    iPendingCallbacks( _FOFF( TSTUNCallbackInfo, iLink ) )
    {
    }

// ---------------------------------------------------------------------------
// CAsyncCallback::CAsyncCallback
// Dummy implementation. Default constructor is declared private and not used.
// ---------------------------------------------------------------------------
//
CAsyncCallback::CAsyncCallback() :
    CActive( EPriorityStandard ),
    iObserver( *( MSTUNClientObserver* )0x1 )
    {
    }

// ---------------------------------------------------------------------------
// CAsyncCallback::CAsyncCallback
// Dummy implementation, as copy constructor is declared private and not used.
// ---------------------------------------------------------------------------
//
CAsyncCallback::CAsyncCallback( const CAsyncCallback& aAsyncCallback ) :
    CActive( EPriorityStandard ),
    iObserver( aAsyncCallback.iObserver )
    {    
    }

// -----------------------------------------------------------------------------
// CAsyncCallback::~CAsyncCallback
// -----------------------------------------------------------------------------
//
CAsyncCallback::~CAsyncCallback()
    {
    Cancel();

    TSTUNCallbackInfo* callback = iPendingCallbacks.First();
    while ( iPendingCallbacks.IsFirst( callback ) &&
            !iPendingCallbacks.IsEmpty() )
        {
        iPendingCallbacks.Remove( *callback );
        delete callback;
        callback = iPendingCallbacks.First();
        }
    }

// -----------------------------------------------------------------------------
// CAsyncCallback::DoCancel
// -----------------------------------------------------------------------------
//
void CAsyncCallback::DoCancel()
    {
    if ( iStatus == KRequestPending )
        {
        TRequestStatus* status = &iStatus;    
        User::RequestComplete( status, KErrCancel );
        }
    }

// -----------------------------------------------------------------------------
// CAsyncCallback::RunL
// -----------------------------------------------------------------------------
//
void CAsyncCallback::RunL()
    {
    WaitForRequests();
    ExecuteFirstCallback();
    }

// -----------------------------------------------------------------------------
// CAsyncCallback::RunError
// Currently RunL cannot leave.
// -----------------------------------------------------------------------------
//
TInt CAsyncCallback::RunError( TInt aError )
    {
    if ( aError == KErrNoMemory )
        {
        return aError;
        }
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CAsyncCallback::MakeCallbackL
// -----------------------------------------------------------------------------
//
void CAsyncCallback::MakeCallbackL( TSTUNCallbackInfo::TFunction aFunction,
                                    const CBinding* aBinding,
                                    TInt aErrorCode,
                                    const CSTUNClient* aClient )
    {    
    TSTUNCallbackInfo* callback = new ( ELeave ) TSTUNCallbackInfo( aFunction,
                                                                    aBinding,
                                                                    aErrorCode,
                                                                    aClient );
    CleanupStack::PushL( callback );
    __STUN_ASSERT_L( callback->Validate(), KErrArgument );
    CleanupStack::Pop( callback );

    //CActive::iActive won't tell whether User::RequestComplete has already
    //been called, so iStatus is inspected to see whether a previous call to
    //AddDeleteRequestL has already called User::RequestComplete.
    if ( iStatus == KRequestPending )
        {
        Wakeup();
        }

    iPendingCallbacks.AddLast( *callback );
    }

// -----------------------------------------------------------------------------
// CAsyncCallback::CancelCallback
// -----------------------------------------------------------------------------
//
void CAsyncCallback::CancelCallback( const CBinding& aBinding )
    {
    TSglQueIter<TSTUNCallbackInfo> iter( iPendingCallbacks );

    for ( TSTUNCallbackInfo* callback = iter++; callback; callback = iter++ )
        {
        if ( callback->iBinding == &aBinding )
            {
            iPendingCallbacks.Remove( *callback );
            delete callback;
            }
        }
    }

// -----------------------------------------------------------------------------
// CAsyncCallback::ExecuteFirstCallback
// Handle one callback request and wait for the next one. If there are more
// callbacks, complete the asynchronous request so that RunL will soon be called
// again. Don't call more than one callback, since upper layer might've deleted
// STUN client, causing CAsyncCallback to be deleted too. For the same reason
// nothing is done after executing the callback.
// -----------------------------------------------------------------------------
//    
void CAsyncCallback::ExecuteFirstCallback()
    {
    TSTUNCallbackInfo* callback = iPendingCallbacks.First();
    if ( iPendingCallbacks.IsFirst( callback ) &&
         !iPendingCallbacks.IsEmpty() )
        {
        iPendingCallbacks.Remove( *callback );
        
        if ( !iPendingCallbacks.IsEmpty() )
            {
            Wakeup();
            }

        callback->Execute( iObserver );
        delete callback;
        }
    }

// -----------------------------------------------------------------------------
// CAsyncCallback::WaitForRequests
// -----------------------------------------------------------------------------
//
void CAsyncCallback::WaitForRequests()
    {
    __STUN_ASSERT_RETURN( !IsActive(), KErrNotReady );

    iStatus = KRequestPending;
    SetActive();
    }

// -----------------------------------------------------------------------------
// CAsyncCallback::Wakeup
// -----------------------------------------------------------------------------
//
void CAsyncCallback::Wakeup()
    {
    TRequestStatus* status = &iStatus;
    User::RequestComplete( status, KErrNone );
    }
