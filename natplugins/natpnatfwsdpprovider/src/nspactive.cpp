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
* Description:  Async callback implementation.
*
*/

#include <sdpdocument.h>
#include "nspsessionobserver.h"
#include "nspactive.h"
#include "nspcontroller.h"
#include "nspevents.h"
#include "nspsession.h"
#include "nspdefs.h"

// ======== MEMBER FUNCTIONS ========
// ---------------------------------------------------------------------------
// CNSPActive::CNSPActive
// ---------------------------------------------------------------------------
//
CNSPActive::CNSPActive( CNSPController& aController,
        TUint aSessionId, TUint aStreamId, TUint aTransactionId,
        MNATFWConnectivityObserver::TNATFWConnectivityEvent aEvent,
        CSdpDocument* aDocument )
     : CActive( EPriorityStandard ),
       iController( aController ),
       iSessionId( aSessionId ),
       iStreamId( aStreamId ),
       iTransactionId( aTransactionId ),
       iEvent( aEvent ),
       iDocument( aDocument )
    {
    CActiveScheduler::Add( this );
    }


// ---------------------------------------------------------------------------
// CNSPAsyncCallback::ConstructL
// ---------------------------------------------------------------------------
//
void CNSPActive::ConstructL( TUint aTimerInMicroSeconds )
    {
	User::LeaveIfError( iTimer.CreateLocal() );
    TTimeIntervalMicroSeconds32 interval( aTimerInMicroSeconds );
    iTimer.After( iStatus, interval );
    SetActive();
    }


// ---------------------------------------------------------------------------
// CNSPActive::NewL
// ---------------------------------------------------------------------------
//
CNSPActive* CNSPActive::NewL( CNSPController& aController,
            TUint aSessionId, TUint aStreamId, TUint aTransactionId,
            MNATFWConnectivityObserver::TNATFWConnectivityEvent aEvent,
            TUint aTimerInMicroSeconds, CSdpDocument* aDocument )
    {
    CNSPActive* self = CNSPActive::NewLC( aController,
            aSessionId, aStreamId, aTransactionId,
            aEvent, aTimerInMicroSeconds, aDocument );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNSPActive::NewLC
// ---------------------------------------------------------------------------
//
CNSPActive* CNSPActive::NewLC( CNSPController& aController,
            TUint aSessionId, TUint aStreamId, TUint aTransactionId,
            MNATFWConnectivityObserver::TNATFWConnectivityEvent aEvent,
            TUint aTimerInMicroSeconds, CSdpDocument* aDocument )
    {
    CNSPActive* self = new ( ELeave ) CNSPActive( aController,
            aSessionId, aStreamId, aTransactionId, aEvent, aDocument );
    CleanupStack::PushL( self );
    self->ConstructL( aTimerInMicroSeconds );
    return self;
    }


// ---------------------------------------------------------------------------
// CNSPActive::~CNSPActive
// ---------------------------------------------------------------------------
//
CNSPActive::~CNSPActive()
    {
    if ( IsActive() )
        {
        CActive::Cancel();
        delete iDocument;
        iDocument = NULL;
        }
    
    iTimer.Close();
    }


// ---------------------------------------------------------------------------
// CNSPActive::RunL
// ---------------------------------------------------------------------------
//
void CNSPActive::RunL()
    {
    NSPLOG_STR( "CNSPActive::RunL()" )
    
    const TInt index = iController.FindSession( iSessionId );
    const TInt activeIndex = iController.FindActiveObject( iTransactionId );
    
    if ( KErrNotFound != activeIndex )
        {
        iController.iActiveObjects.Remove( activeIndex );
        iController.iActiveObjects.Compress();
        }
    
    if ( KErrNotFound != index )
        {
        if ( iDocument )
            {
            // UPDATE
            MNSPSessionObserver& observer =
                    iController.iSessionArray[index]->SessionObserver();
            observer.UpdateSdp( iSessionId, iDocument );
            }
        else
            {
            // NOTIFY
            TEventReturnStatus status = iController.iSessionArray[index]->EventOccured(
            		iStreamId, iEvent, KErrTimedOut, NULL );
            iController.Callback( *iController.iSessionArray[index], status );
            }
        }
    else
        {
        NSPLOG_STR( "CNSPActive::RunL(), Session not found!!" )
        NSPDEBUG_PANIC( KErrNotFound );
        }
    
    delete this;
    }


// ---------------------------------------------------------------------------
// CNSPActive::DoCancel
// ---------------------------------------------------------------------------
//
void CNSPActive::DoCancel()
    {
    NSPLOG_STR( "CNSPActive::DoCancel()" )
	iTimer.Cancel();
    }


// ---------------------------------------------------------------------------
// CNSPAsyncCallback::RunError
// ---------------------------------------------------------------------------
//
TInt CNSPActive::RunError( TInt /*aError*/ )
    {
    NSPLOG_STR( "CNSPActive::RunError(), Should not come here!!" )
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CNSPActive::SessionId
// ---------------------------------------------------------------------------
//
TUint CNSPActive::SessionId() const
    {
    return iSessionId;
    }


// ---------------------------------------------------------------------------
// CNSPActive::TransactionId
// ---------------------------------------------------------------------------
//
TUint CNSPActive::TransactionId() const
    {
    return iTransactionId;
    }


// end of file
