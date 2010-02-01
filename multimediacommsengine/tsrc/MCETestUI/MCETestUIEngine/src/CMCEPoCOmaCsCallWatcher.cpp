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
* Description:    For monitoring CS call events from Publish & Subscribe
*
*/



// INCLUDE FILES
#include "CMCEPoCOmaCsCallWatcher.h"
#include "MCETestUIDebug.h"

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CMCEPoCOmaCSCallWatcher::CMCEPoCOmaCSCallWatcher()
// C++ constructor
// -----------------------------------------------------------------------------
//
CMCEPoCOmaCSCallWatcher::CMCEPoCOmaCSCallWatcher() :
    CActive( EPriorityStandard )
    {
    }

// -----------------------------------------------------------------------------
// CMCEPoCOmaCSCallWatcher::ConstructL()
// Symbian OS default constructor
// -----------------------------------------------------------------------------
//
void CMCEPoCOmaCSCallWatcher::ConstructL( MMcePoCOmaCSStateObserver* aObserver )
    {
    iCsCallNotify = aObserver;
    
    // Add this active object to the scheduler.
    CActiveScheduler::Add( this );
    // Attach to the property
    User::LeaveIfError( iProperty.Attach( KPSUidCtsyCallInformation, 
                                          KCTsyCallState) ); 
    }

// -----------------------------------------------------------------------------
// CMCEPoCOmaCSCallWatcher::NewL()
// Constructs a new entry with given values.
// -----------------------------------------------------------------------------
//
CMCEPoCOmaCSCallWatcher* CMCEPoCOmaCSCallWatcher::NewL( MMcePoCOmaCSStateObserver* aObserver )
    {
    CMCEPoCOmaCSCallWatcher* self = new (ELeave) CMCEPoCOmaCSCallWatcher();

    CleanupStack::PushL(self);
    
    self->ConstructL( aObserver );
    
    CleanupStack::Pop();
    
    return self;
    }

// -----------------------------------------------------------------------------
// CMCEPoCOmaCSCallWatcher::CMCEPoCOmaCSCallWatcher()
// Destructor
// -----------------------------------------------------------------------------
//
CMCEPoCOmaCSCallWatcher::~CMCEPoCOmaCSCallWatcher()
    {
    
    // Check if object is in active scheduler’s list of active objects
    if( IsAdded() ) 
        {
        Deque(); // Cancel outstanding request and remove from active scheduler’s list
        }

    // Close handle, call after Deque() !!!
    // DoCancel() will call iPropeerty.Cancel()
    iProperty.Close();
    
    // Clear RPointerArray to avoid memory problems
    //iCsCallNotifyList.Reset();
    }

// -----------------------------------------------------------------------------
// CMCEPoCOmaCSCallWatcher::Subscribe()
// Starts listening KPSUidCurrentCallValue event
// -----------------------------------------------------------------------------
//
TInt CMCEPoCOmaCSCallWatcher::Subscribe()
    {
    TInt error = KErrInUse;

    if( !IsActive() )
        { 
        MCE_TESTUI_DEBUG1( "Subscribed to call state monitoring" );
        // Request CS Call events
        iProperty.Subscribe( iStatus );
        SetActive();
        error = KErrNone;
        }
    else
        {
        }
                
    return error;
    }

// -----------------------------------------------------------------------------
// CMCEPoCOmaCSCallWatcher::StopSubscribe()
// Stops listening system agent events
// -----------------------------------------------------------------------------
//
void CMCEPoCOmaCSCallWatcher::StopSubscribe()
    {    
    Cancel();   // From CActive
    }

// -----------------------------------------------------------------------------
// CMCEPoCOmaCSCallWatcher::RunL()
// Call all subscribed clients with parameters
// -----------------------------------------------------------------------------
//
void CMCEPoCOmaCSCallWatcher::RunL()
    {
    TInt err( KErrNone );        
       
    // Check iStatus
    if( iStatus != KErrNone )
        {
        if( iStatus != KErrCancel )
            {
            Subscribe(); // Subscribe again
            }

        return; // There was some error, so don't notify this event to clients
        }
    
    // Subscribing again before getting the current value
    // improves the chance that concurrent update is not missed
    Subscribe(); 
    
    TInt leaveError( KErrNone );  

    // Get current value
    TInt value( KErrNone );
    iProperty.Get( value );
    
    MCE_TESTUI_DEBUG3("Call state changed to: ", value );
    // notify CS observers
    //for (nextIndex = 0; nextIndex < iCsCallNotifyList.Count(); nextIndex++ )
    //	{
    TRAP( err, iCsCallNotify->CsStatusChangedL(
                   TranslateToPoCCsState( (/*TPSTelephonyCallState*/TPSCTsyCallState)value ) ) );
    if( err == KErrNoMemory )
        {
        leaveError = err;
        }

        //}
    
    User::LeaveIfError( leaveError ); 
    }

// -----------------------------------------------------------------------------
// CMCEPoCOmaCSCallWatcher::RunError
// Handles errors
// -----------------------------------------------------------------------------
//
TInt CMCEPoCOmaCSCallWatcher::RunError( TInt aError )
    {
    if( aError == KErrNoMemory )
        {
        Subscribe(); // Continue subscribe from PubSub
        return KErrNoMemory;
        }
        
    return KErrNone;
    }
// -----------------------------------------------------------------------------
// CMCEPoCOmaCSCallWatcher::DoCancel()
// Cancels event subscribe
// -----------------------------------------------------------------------------
//
void CMCEPoCOmaCSCallWatcher::DoCancel()
    {    
    iProperty.Cancel();
    }

// -----------------------------------------------------------------------------
// CMCEPoCOmaCSCallWatcher::AddObserverL()
// For subscribing CS events
// -----------------------------------------------------------------------------
//
/*void CMCEPoCOmaCSCallWatcher::AddObserverL( const MPoCOmaCSStateObserver* aObserver )
    {
    iCsCallNotify = aObserver;
    
    Subscribe(); // Subscribe to PubSub events   
    
    }

// -----------------------------------------------------------------------------
// CMCEPoCOmaCSCallWatcher::RemoveObserver()
// Cancels CS call event listening
// -----------------------------------------------------------------------------
//
void CMCEPoCOmaCSCallWatcher::RemoveObserver( const MPoCOmaCSStateObserver* aObserver )
    {
    StopSubscribe(); // Stop subscribing PubSub events   
    }
*/
// -----------------------------------------------------------------------------
// CMCEPoCOmaCSCallWatcher::CsState()
// For having latest status of CS call
// -----------------------------------------------------------------------------
//
TPoCOmaCsState CMCEPoCOmaCSCallWatcher::CsState()
    {
    TInt value ( KErrNone );
    // Get attached value
    iProperty.Get ( value );

    TPoCOmaCsState currentCsState = TranslateToPoCCsState( (/*TPSTelephonyCallState*/ TPSCTsyCallState)value );

    return currentCsState;
    }

// -----------------------------------------------------------------------------
// CMCEPoCOmaCSCallWatcher::TranslateToPoCCsState()
// Translates TSACurrentCall state to TPoCCsState state
// -----------------------------------------------------------------------------
//
TPoCOmaCsState CMCEPoCOmaCSCallWatcher::TranslateToPoCCsState( 
    /*TPSTelephonyCallState*/TPSCTsyCallState aPSCsCallState )
    {
    TPoCOmaCsState poCCsState( EPoCOmaCallStateNone );
    
    switch( aPSCsCallState )
        {
    	case EPSCTsyCallStateUninitialized:
    	    poCCsState = EPoCOmaCallUninitialized;
    	    break;

        case EPSCTsyCallStateNone:
            poCCsState = EPoCOmaCallStateNone; 
            break;

        case EPSCTsyCallStateAlerting:
            poCCsState = EPoCOmaCallStateAlerting; 
            break;

        case EPSCTsyCallStateRinging:
          poCCsState = EPoCOmaCallStateRinging; 
          break;

        case EPSCTsyCallStateDialling:
            poCCsState = EPoCOmaCallStateDialling;
            break;

        case EPSCTsyCallStateAnswering:
            poCCsState = EPoCOmaCallStateAnswering; 
            break;

        case EPSCTsyCallStateDisconnecting:
            poCCsState = EPoCOmaCallStateDisconnecting; 
            break;

        case EPSCTsyCallStateConnected:
            poCCsState = EPoCOmaCallStateConnected;
            break;

        case EPSCTsyCallStateHold:
            poCCsState = EPoCOmaCallStateHold;
            break;

        default: 
            poCCsState = EPoCOmaCallStateNone; 
         }      
    
    return poCCsState;
    }
//  End of File  

