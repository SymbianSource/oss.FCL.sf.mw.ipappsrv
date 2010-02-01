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

#include "MCETestUIEngineDefaultValues.h"

#include "CMCETestUIEngine.h"
#include "CMCETestUIEngineEvent.h"

#include "TMCETestUIEngineCmdTerminateEvent.h"
//#include "TMCETestUIEngineCmdUpdateSession.h"
#include "TMCETestUIEngineCmdDeleteEvent.h"
#include "TMCETestUIEngineCmdCreateEvent.h"


// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CMCETestUIEngineEvent::CMCETestUIEngineEvent
// -----------------------------------------------------------------------------
//
CMCETestUIEngineEvent::CMCETestUIEngineEvent( 
                CMCETestUIEngine& aEngine )
    : iEngine( aEngine )
    {
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineEvent::BaseConstructL()
// -----------------------------------------------------------------------------
//
void CMCETestUIEngineEvent::BaseConstructL()
    {
    }


// -----------------------------------------------------------------------------
// CMCETestUIEngineEvent::~CMCETestUIEngineEvent
// -----------------------------------------------------------------------------
//
EXPORT_C CMCETestUIEngineEvent::~CMCETestUIEngineEvent()
    {
    iCommands.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineEvent::Event
// -----------------------------------------------------------------------------
//
CMceEvent& CMCETestUIEngineEvent::Event()
    {
    return *iEvent;
    }


// -----------------------------------------------------------------------------
// CMCETestUIEngineEvent::GetCommands
// -----------------------------------------------------------------------------
//
const RPointerArray<TMCETestUIEngineCmdBase>& 
            CMCETestUIEngineEvent::GetCommandsL()
    {
    User::LeaveIfError( iEvent ? KErrNone : KErrNotReady );

    TInt state = iEvent->State();
    

	switch( state ) 
		{
		case CMceEvent::EIdle: 
		    {	
		      
		    break;
		    }
		case CMceEvent::EPending: 
		    {	
		     
		    break;
		    }    
	   	case CMceEvent::EActive:
		    {		         
		    iCommands.Append( new (ELeave) 
		        TMCETestUIEngineCmdTerminateEvent( iEngine, *this ) );
		   	iCommands.Append( new (ELeave) 
		        TMCETestUIEngineCmdCreateReferEvent( iEngine, *this ) );
		       
		    break;
		    }
		case CMceEvent::ETerminated: 
		    {	
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
// CMCETestUIEngineEvent::State
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& CMCETestUIEngineEvent::State() 
    {
    if ( !iEvent ) 
        {
        User::Panic( _L16("CMCETestUIEngine"), 0 );
        }
        
    TInt state = iEvent->State();

	switch( state ) 
		{
		case CMceEvent::EIdle: 
		    return KEventStateIdle; 
		    break;
		case CMceEvent::EPending: 
		    return KEventStatePending; 
		    break;
		case CMceEvent::EActive: 
		    return KEventStateActive; 
		    break;
		case CMceEvent::ETerminated: 
		    return KEventStateTerminated; 
		    break;
		default: 
		    return KEventStateUnknown; 
		    break;
		}    
    }

