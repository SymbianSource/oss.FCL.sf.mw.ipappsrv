/*
* Copyright (c) 2004-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Provides a comfort noise generator class
*
*/

// INCLUDE FILES
#include <mmfformat.h>
#include "sendstatemachine.h"
#include "msrppayloadformatdefs.h"

// MACROS

// ============================= LOCAL FUNCTIONS ===============================

// RECOMMENDED default time out value
const TInt KDefaultTimeoutValue = 300000;
// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSendStateMachine::CSendStateMachine
// C++ default constructor can NOT contain any code, that
// might leave
// -----------------------------------------------------------------------------
//
CSendStateMachine::CSendStateMachine( MSendStateObserver* aClient ) :
    iClient( aClient )
    {
    DP_MSRP_WRITE( "CSendStateMachine::CSendStateMachine" );
    iTimeOutValue = KDefaultTimeoutValue;
    }

// -----------------------------------------------------------------------------
// CSendStateMachine::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSendStateMachine::ConstructL( )
    {
    DP_MSRP_WRITE( "CSendStateMachine::ConstructL" );
    
    // iTimer.CreateLocal();	
    User::LeaveIfNull( iClient );
    iPeriodic = CPeriodic::NewL(0);
    }


// -----------------------------------------------------------------------------
// CSendStateMachine::NewL
// Static constructor.
// -----------------------------------------------------------------------------
//
CSendStateMachine* CSendStateMachine::NewL( 
       MSendStateObserver* aClient )
    {
    DP_MSRP_WRITE( "CSendStateMachine::NewL" );
    
    CSendStateMachine* self = 
            new ( ELeave ) CSendStateMachine( aClient );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// ------------------------------------------------------------------------------
// CSendStateMachine::~CSendStateMachine
// Destructor
// -----------------------------------------------------------------------------
//
CSendStateMachine::~CSendStateMachine( )
    {
    DP_MSRP_WRITE( "CSendStateMachine::~CSendStateMachine" );
    
      if (iPeriodic)
      {	 
         iPeriodic->Cancel (); 
         delete iPeriodic;
      }

    iClient = NULL;
    }


// -----------------------------------------------------------------------------
// CSendStateMachine::SetTimeOut
// SetTimeOut for periodic timer
// -----------------------------------------------------------------------------
//
void CSendStateMachine::SetTimeOut(TTimeIntervalMicroSeconds32 aValue)
{
    DP_MSRP_WRITE( "CSendStateMachine::SetTimeOut" );
	if (!iPeriodic->IsActive () )
	{
		iTimeOutValue = aValue;
	}
}
     

// -----------------------------------------------------------------------------
// CSendStateMachine::Start
// Starts the PeriodicTimer
// -----------------------------------------------------------------------------
//     
void CSendStateMachine::Start () 
{
    DP_MSRP_WRITE( "CSendStateMachine::Start" );
    if (!iPeriodic->IsActive () )
        {
        iPeriodic->Start(iTimeOutValue,iTimeOutValue,TCallBack(IdleCallBackL, this));
        } 
   
}


// -----------------------------------------------------------------------------
// CSendStateMachine::Cancel
// Cancels the PeriodicTimer
// -----------------------------------------------------------------------------
//
void CSendStateMachine::Cancel ()
{
    DP_MSRP_WRITE( "CSendStateMachine::Cancel" );
  iPeriodic->Cancel();	
}


// -----------------------------------------------------------------------------
// CSendStateMachine::IdleCallBackL
// IdleCallBackL is the callback function that actually calls
// back clients TimerExpiredL function
// -----------------------------------------------------------------------------
//
TInt CSendStateMachine::IdleCallBackL (TAny* aPtr)
{
    DP_MSRP_WRITE( "CSendStateMachine::IdleCallBackL" );
    CSendStateMachine* me = ((CSendStateMachine*)aPtr);
    me->iClient->TimerExpiredL();
    return ETrue;
}

// End of line

