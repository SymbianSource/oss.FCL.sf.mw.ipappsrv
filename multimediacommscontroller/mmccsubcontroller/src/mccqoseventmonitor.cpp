/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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




// INCLUDES
#include "mccqoseventmonitor.h"
#include "mccqoseventobserver.h"
#include "mccqoslogging.h"

// Constants for subconnection events. Need to define them here, because
// we cannot include cs_subconevents.h as that leads to unfrozen exports
// in this DLL. This will be fixed later.
const TUint32 KSubConGenericEventParamsGranted      =  0x1;
const TUint32 KSubConGenericEventDataClientJoined   =  0x2;
const TUint32 KSubConGenericEventDataClientLeft     =  0x4;
const TUint32 KSubConGenericEventSubConDown         =  0x8;
const TUint32 KSubConGenericEventParamsChanged      = 0x10;
const TUint32 KSubConGenericEventParamsRejected     = 0x20;

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CMccQosEventMonitor::CMccQosEventMonitor
// C++ default constructor can NOT contain any code, that might leave.
// ---------------------------------------------------------------------------
//
CMccQosEventMonitor::CMccQosEventMonitor( MMccQosEventObserver& aObserver, 
    RSubConnection& aSubConnection ) : CActive( EPriorityStandard ),
    iEventObserver( aObserver ), iSubConnection( aSubConnection )
    {
    CActiveScheduler::Add( this );
    }


// -----------------------------------------------------------------------------
// CMccQosEventMonitor::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMccQosEventMonitor* CMccQosEventMonitor::NewL( MMccQosEventObserver& 
    aObserver, RSubConnection& aSubConnection )
    {
    __QOSLOG( "CMccQosEventMonitor::NewL" )

    CMccQosEventMonitor* self( 
        CMccQosEventMonitor::NewLC( aObserver, aSubConnection ) );
    
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CMccQosEventMonitor::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMccQosEventMonitor* CMccQosEventMonitor::NewLC( MMccQosEventObserver& 
    aObserver, RSubConnection& aSubConnection )
    {
    __QOSLOG( "CMccQosEventMonitor::NewLC" )

    CMccQosEventMonitor* self = new ( ELeave ) CMccQosEventMonitor( aObserver,
        aSubConnection );
        
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CMccQosEventMonitor::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMccQosEventMonitor::ConstructL()
    {
    __QOSLOG( "CMccQosEventMonitor::ConstructL" )
    
    StartListenL();
    }
    
// -----------------------------------------------------------------------------
// CMccQosEventMonitor::~CMccQosEventMonitor
// Destructor
// -----------------------------------------------------------------------------
//
CMccQosEventMonitor::~CMccQosEventMonitor()
    {
    __QOSLOG("CMccQosEventMonitor::~CMccQosEventMonitor")

    // Calls DoCancel() if active
    Cancel();
    }


// -----------------------------------------------------------------------------
// CMccQosEventMonitor::StartListen()
// Sets active object active and issues listening
// -----------------------------------------------------------------------------
//
void CMccQosEventMonitor::StartListenL()
    {
    __QOSLOG_INT1( "CMccQosEventMonitor::StartListen iLastError: ", iLastError )
    __QOSLOG_INT1( "CMccQosEventMonitor::StartListen IsActive: ", IsActive() )
    
    if ( !IsActive() )
        {
        if ( KErrInUse != iLastError || 
             KErrCancel != iLastError || 
             KErrAbort != iLastError )
            {
            __QOSLOG( "CMccQosEventMonitor::StartListen -> EventNotification")
            
            iSubConnection.EventNotification( iQoSNotifBuffer,
                EFalse, iStatus );
            SetActive();
            }
        }
    else
        {
        __QOSLOG( "CMccQosEventMonitor::StartListen KErrNotReady")
        
        User::Leave( KErrNotReady );
        }
    }


// -----------------------------------------------------------------------------
// CMccQosEventMonitor::DoCancel
// Cancel any outstanding operation.
// -----------------------------------------------------------------------------
//
void CMccQosEventMonitor::DoCancel()
    {
    __QOSLOG( "CMccQosEventMonitor::DoCancel" )

    iSubConnection.CancelEventNotification();
    }


// -----------------------------------------------------------------------------
// CMccQosEventMonitor::RunL
// Handle completion of asynchronous requests to RSubConnection
// -----------------------------------------------------------------------------
//
void CMccQosEventMonitor::RunL()
    {
    iLastError = iStatus.Int();
    const TUint32 notifyId( iQoSNotifBuffer.Id() );
    
    StartListenL();
    
    __QOSLOG_INT1( "CMccQosEventMonitor::RunL iLastError: ", iLastError )
    
#ifdef __QOSLOG
    
    switch( notifyId )
        {
        // Parameters related
        case KSubConGenericEventParamsGranted:
            __QOSLOG( "CMccQosEventMonitor::RunL GRANTED" )
            break;

        case KSubConGenericEventParamsChanged:
            __QOSLOG( "CMccQosEventMonitor::RunL CHANGED" )
            break;

        case KSubConGenericEventParamsRejected:
            __QOSLOG( "CMccQosEventMonitor::RunL REJECTED" )
            break;
            
        case KSubConGenericEventDataClientJoined:
            __QOSLOG( "CMccQosEventMonitor::RunL DATACLIENT JOINED" )
            break;
            
        case KSubConGenericEventDataClientLeft:
            __QOSLOG( "CMccQosEventMonitor::RunL DATACLIENT LEFT" )
            break;
            
        case KSubConGenericEventSubConDown:
            __QOSLOG( "CMccQosEventMonitor::RunL SUBCON DOWN" )
            break;
            
        default:
            __QOSLOG_INT1("CMccQosEventMonitor::RunL UNKNOWN: ",
                iQoSNotifBuffer.Id() )
            break;
        }
#endif
    
    iEventObserver.QoSParamEvent( iLastError, notifyId );
    }

// -----------------------------------------------------------------------------
// CMccQosEventMonitor::RunError
// -----------------------------------------------------------------------------
//
TInt CMccQosEventMonitor::RunError( TInt aError )
    {
    __QOSLOG_INT1( "CMccQosEventMonitor::RunError aError: ", aError )
    
    if ( KErrNoMemory == aError )
        {
        return aError;
        }
    else
        {
        #ifdef _DEBUG

        return aError;
        
        #else // UREL

        return KErrNone;
        
        #endif // _DEBUG
        }
    }

