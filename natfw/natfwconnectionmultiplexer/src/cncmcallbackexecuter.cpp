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
* Description:    Executes client specified callbacks.
*
*/




#include "cncmcallbackexecuter.h"
#include "ncmconnectionmultiplexerlogs.h"

// ======== MEMBER FUNCTIONS ========


// ---------------------------------------------------------------------------
// CNcmCallBackExecuter::CNcmCallBackExecuter
// ---------------------------------------------------------------------------
//
CNcmCallBackExecuter::CNcmCallBackExecuter() : CActive( EPriorityStandard )
    {
    CActiveScheduler::Add( this );
    }


// ---------------------------------------------------------------------------
// CNcmCallBackExecuter::NewL
// ---------------------------------------------------------------------------
//
CNcmCallBackExecuter* CNcmCallBackExecuter::NewL()
    {
    CNcmCallBackExecuter* self = CNcmCallBackExecuter::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNcmCallBackExecuter::NewLC
// ---------------------------------------------------------------------------
//
CNcmCallBackExecuter* CNcmCallBackExecuter::NewLC()
    {
    CNcmCallBackExecuter* self 
        = new( ELeave ) CNcmCallBackExecuter();
    CleanupStack::PushL( self );
    self->WaitForRequestsL();
    return self;
    }


// ---------------------------------------------------------------------------
// CNcmCallBackExecuter::~CNcmCallBackExecuter
// ---------------------------------------------------------------------------
//
CNcmCallBackExecuter::~CNcmCallBackExecuter()
    {
    Cancel();
    iCallBacks.Close();
    }


// ---------------------------------------------------------------------------
// CNcmCallBackExecuter::AddCallBackL
// ---------------------------------------------------------------------------
//
void CNcmCallBackExecuter::AddCallBackL( const TNcmCallBack& aCallBack )
    {
    __CONNECTIONMULTIPLEXER( "CNcmCallBackExecuter::AddCallBackL" )
    
    if ( iStatus == KRequestPending )
        {
        Wakeup();
        }
        
    iCallBacks.AppendL( aCallBack );
    }


// ---------------------------------------------------------------------------
// CNcmCallBackExecuter::ExecuteFirstCallback
// Handle one callback request and wait for the next one. If there are more
// callbacks, complete the asynchronous request so that RunL will soon be 
// called again.
// ---------------------------------------------------------------------------
//    
void CNcmCallBackExecuter::ExecuteFirstCallback()
    {
    TNcmCallBack* callback = &iCallBacks[0];
    callback->CallBack();

    iCallBacks.Remove( 0 );
      
    if ( iCallBacks.Count() )
        {
        Wakeup();
        }
    }


// ---------------------------------------------------------------------------
// CNcmCallBackExecuter::WaitForRequestsL
// ---------------------------------------------------------------------------
//
void CNcmCallBackExecuter::WaitForRequestsL()
    {
    __ASSERT_ALWAYS( !IsActive(), User::Leave( KErrNotReady ) );

    iStatus = KRequestPending;
    SetActive();
    }


// ---------------------------------------------------------------------------
// CNcmCallBackExecuter::Wakeup
// ---------------------------------------------------------------------------
//
void CNcmCallBackExecuter::Wakeup()
    {
    TRequestStatus* status = &iStatus;
    User::RequestComplete( status, KErrNone );
    }
    

// ---------------------------------------------------------------------------
// From class CActive.
// ---------------------------------------------------------------------------
//
void CNcmCallBackExecuter::RunL()
    {
    __CONNECTIONMULTIPLEXER( "CNcmCallBackExecuter::RunL" )
    __ASSERT_ALWAYS( iCallBacks.Count(), User::Leave( KErrNotFound ) );
    
    WaitForRequestsL();
    ExecuteFirstCallback();
    }


// ---------------------------------------------------------------------------
// From class CActive.
// ---------------------------------------------------------------------------
//
void CNcmCallBackExecuter::DoCancel()
    {
    __CONNECTIONMULTIPLEXER( "CNcmCallBackExecuter::DoCancel" )
    
    iCallBacks.Reset();
    
    if ( iStatus == KRequestPending )
        {
        TRequestStatus* status = &iStatus;    
        User::RequestComplete( status, KErrCancel );
        }
    }
