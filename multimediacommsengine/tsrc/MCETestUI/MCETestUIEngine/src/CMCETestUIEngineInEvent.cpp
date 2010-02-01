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




// MCE API

#include <MCEManager.h>
#include <MCEEvent.h>
#include <MCEInEvent.h>
#include "MCETestUIEngineDefaultValues.h"

#include "CMCETestUIEngine.h"
#include "CMCETestUIEngineEvent.h"
#include "CMCETestUIEngineInEvent.h"

#include "TMCETestUIEngineCmdAcceptEvent.h"
#include "TMCETestUIEngineCmdRejectEvent.h"
#include "TMCETestUIEngineCmdNotifyEvent.h"
#include "TMCETestUIEngineCmdDeleteEvent.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMCETestUIEngineInEvent::NewL()
// -----------------------------------------------------------------------------
//
CMCETestUIEngineInEvent* CMCETestUIEngineInEvent::NewL(
            CMCETestUIEngine& aEngine,
            CMceInEvent* aInEvent ) 
    {
    CMCETestUIEngineInEvent* self = 
        new (ELeave) CMCETestUIEngineInEvent( aEngine, aInEvent );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;  
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineInEvent::CMCETestUIEngineInEvent
// -----------------------------------------------------------------------------
//
CMCETestUIEngineInEvent::CMCETestUIEngineInEvent(
            CMCETestUIEngine& aEngine,
            CMceInEvent* aInEvent )
    :CMCETestUIEngineEvent( aEngine ),
     iInEvent( aInEvent ),
     iId( 0 )
    {
    }
     

// -----------------------------------------------------------------------------
// CMCETestUIEngineInEvent::ConstructL()
// -----------------------------------------------------------------------------
//
void CMCETestUIEngineInEvent::ConstructL()
    {
  //  User::LeaveIfError( iInEvent ? KErrNone : KErrArgument );
     
    BaseConstructL();
    iEvent = iInEvent;
    
       
    // Get identity
    iId = iEngine.NextInEventId();
    }


// -----------------------------------------------------------------------------
// CMCETestUIEngineInEvent::~CMCETestUIEngineInEvent
// -----------------------------------------------------------------------------
//
EXPORT_C CMCETestUIEngineInEvent::~CMCETestUIEngineInEvent()
    {
    delete iInEvent;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineInEvent::GetCommands
// -----------------------------------------------------------------------------
//
const RPointerArray<TMCETestUIEngineCmdBase>& 
            CMCETestUIEngineInEvent::GetCommandsL()
    {
    iCommands.ResetAndDestroy();

    // Get base class commands
    CMCETestUIEngineEvent::GetCommandsL();

    // Get own commands
    TInt state = iInEvent->State();

	switch( state ) 
		{
		case CMceEvent::EIdle: 
		    {
		    iCommands.Append( new (ELeave) TMCETestUIEngineCmdAcceptEvent( 
                                iEngine, *this ) );
			iCommands.Append( new (ELeave) TMCETestUIEngineCmdRejectEvent( 
                                iEngine, *this ) );
			iCommands.Append( new (ELeave) TMCETestUIEngineCmdNotifyEvent( 
                                iEngine, *this ) );  
            iCommands.Append( new (ELeave) 
		        TMCETestUIEngineCmdDeleteEvent( iEngine, *this ) );	                                                               		            
		    break;
		    }
	   	case CMceEvent::EPending: 
	   		{
	   		iCommands.Append( new (ELeave) TMCETestUIEngineCmdAcceptEvent( 
                                iEngine, *this ) );
			iCommands.Append( new (ELeave) TMCETestUIEngineCmdRejectEvent( 
                                iEngine, *this ) );
			iCommands.Append( new (ELeave) TMCETestUIEngineCmdNotifyEvent( 
                                iEngine, *this ) );  
            iCommands.Append( new (ELeave) 
		        TMCETestUIEngineCmdDeleteEvent( iEngine, *this ) );	  
		    break;   
	   		}
	   	case CMceEvent::EActive:
		    {
		    iCommands.Append( new (ELeave) TMCETestUIEngineCmdNotifyEvent( 
                                iEngine, *this ) );  		         
		    iCommands.Append( new (ELeave) 
		        TMCETestUIEngineCmdDeleteEvent( iEngine, *this ) );	    
		    break;
		    }
		default:
		    {		     
		    break;
		    }
		}  
		  
    return iCommands; 
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineInEvent::Id
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMCETestUIEngineInEvent::Id() const
    {
    return iId;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineInEvent::InSession
// -----------------------------------------------------------------------------
//
CMceInEvent& CMCETestUIEngineInEvent::InEvent()
    {
    return *iInEvent;
    }



