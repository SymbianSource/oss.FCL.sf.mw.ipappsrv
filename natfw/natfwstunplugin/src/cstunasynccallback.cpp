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
#include "cstunasynccallback.h"


// ---------------------------------------------------------------------------
// CStunAsyncCallback::NewL
// ---------------------------------------------------------------------------
//
CStunAsyncCallback* CStunAsyncCallback::NewL( 
    const CNATFWPluginApi& aStunPlugin,
    MNATFWPluginObserver& aObserver )
    {
    CStunAsyncCallback* self 
        = new (ELeave) CStunAsyncCallback( aStunPlugin, aObserver );
    CActiveScheduler::Add( self );
    self->WaitForRequestsL();
    return self;
    }


// ---------------------------------------------------------------------------
// CStunAsyncCallback::CStunAsyncCallback
// ---------------------------------------------------------------------------
//
CStunAsyncCallback::CStunAsyncCallback( 
    const CNATFWPluginApi& aStunPlugin, 
    MNATFWPluginObserver& aObserver ) 
    :
    CActive( EPriorityStandard ),
    iStunPlugin( aStunPlugin ),
    iObserver( aObserver ),
    iPendingCallbacks( _FOFF( TStunPluginCallbackInfo, iLink ) )
    {
    }


// ---------------------------------------------------------------------------
// CStunAsyncCallback::CStunAsyncCallback
// Dummy implementation. Default constructor is declared private and not used.
// ---------------------------------------------------------------------------
//
CStunAsyncCallback::CStunAsyncCallback() :
    CActive( EPriorityStandard ),
    iStunPlugin( *( CNATFWPluginApi* )0x1 ),
    iObserver( *( MNATFWPluginObserver* )0x1 )
    {
    }


// ---------------------------------------------------------------------------
// CStunAsyncCallback::CStunAsyncCallback
// Dummy implementation, as copy constructor is declared private and not used.
// ---------------------------------------------------------------------------
//
CStunAsyncCallback::CStunAsyncCallback( 
    const CStunAsyncCallback& aAsyncCallback ) :
    CActive( EPriorityStandard ),
    iStunPlugin( *( CNATFWPluginApi* )0x1 ),
    iObserver( aAsyncCallback.iObserver )
    {    
    }


// ---------------------------------------------------------------------------
// CStunAsyncCallback::~CStunAsyncCallback
// ---------------------------------------------------------------------------
//
CStunAsyncCallback::~CStunAsyncCallback()
    {
    Cancel();

    TStunPluginCallbackInfo* callback = iPendingCallbacks.First();
    while ( iPendingCallbacks.IsFirst( callback ) &&
            !iPendingCallbacks.IsEmpty() )
        {
        iPendingCallbacks.Remove( *callback );
        delete callback;
        callback = iPendingCallbacks.First();
        }
    }


// ---------------------------------------------------------------------------
// CStunAsyncCallback::DoCancel
// ---------------------------------------------------------------------------
//
void CStunAsyncCallback::DoCancel()
    {
    if ( iStatus == KRequestPending )
        {
        TRequestStatus* status = &iStatus;    
        User::RequestComplete( status, KErrCancel );
        }
    }


// ---------------------------------------------------------------------------
// CStunAsyncCallback::RunL
// ---------------------------------------------------------------------------
//
void CStunAsyncCallback::RunL()
    {
    WaitForRequestsL();
    ExecuteFirstCallback();
    }


// ---------------------------------------------------------------------------
// CStunAsyncCallback::RunError
// Currently RunL cannot leave.
// ---------------------------------------------------------------------------
//
TInt CStunAsyncCallback::RunError( TInt aError )
    {
    if ( aError == KErrNoMemory )
        {
        return aError;
        }
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CStunAsyncCallback::MakeCallbackL
// ---------------------------------------------------------------------------
//
void CStunAsyncCallback::MakeCallbackL(
    TStunPluginCallbackInfo::TFunction aFunction,
    TUint aStreamId,
    TInt aErrorCode,
    TAny* aEventData )
    {    
    TStunPluginCallbackInfo* callback = 
        new ( ELeave ) TStunPluginCallbackInfo( iStunPlugin,
                                                iObserver,
                                                aFunction,
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
// CStunAsyncCallback::CancelCallback
// ---------------------------------------------------------------------------
//
void CStunAsyncCallback::CancelCallback( TUint aStreamId )
    {
    TSglQueIter<TStunPluginCallbackInfo> iter( iPendingCallbacks );

    for ( TStunPluginCallbackInfo* callback = iter++; 
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
// CStunAsyncCallback::ExecuteFirstCallback
// Handle one callback request and wait for the next one. If there are more
// callbacks, complete the asynchronous request so that RunL will soon be 
// called again. Don't call more than one callback, since upper layer 
// might've deleted STUN plugin, causing CStunAsyncCallback to be deleted too.
// For the same reason nothing is done after executing the callback.
// ---------------------------------------------------------------------------
//    
void CStunAsyncCallback::ExecuteFirstCallback()
    {
    TStunPluginCallbackInfo* callback = iPendingCallbacks.First();
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
// CStunAsyncCallback::WaitForRequestsL
// ---------------------------------------------------------------------------
//
void CStunAsyncCallback::WaitForRequestsL()
    {
    __ASSERT_ALWAYS( !IsActive(), User::Leave( KErrNotReady ) );

    iStatus = KRequestPending;
    SetActive();
    }


// ---------------------------------------------------------------------------
// CStunAsyncCallback::Wakeup
// ---------------------------------------------------------------------------
//
void CStunAsyncCallback::Wakeup()
    {
    TRequestStatus* status = &iStatus;
    User::RequestComplete( status, KErrNone );
    }
