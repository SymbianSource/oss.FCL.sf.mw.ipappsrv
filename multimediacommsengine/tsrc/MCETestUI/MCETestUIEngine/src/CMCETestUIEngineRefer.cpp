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
#include <MCERefer.h>
#include "MCETestUIEngineDefaultValues.h"
#include "CMCETestUIEngine.h"
#include "CMCETestUIEngineRefer.h"

//#include "TMCETestUIEngineCmdTerminateEvent.h"
//#include "TMCETestUIEngineCmdUpdateSession.h"
#include "TMCETestUIEngineCmdDeleteEvent.h"
#include "TMCETestUIEngineCmdCreateEvent.h"


// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CMCETestUIEngineRefer::CMCETestUIEngineRefer
// -----------------------------------------------------------------------------
//
CMCETestUIEngineRefer::CMCETestUIEngineRefer( 
                CMCETestUIEngine& aEngine )
    : iEngine( aEngine )
    {
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineRefer::BaseConstructL()
// -----------------------------------------------------------------------------
//
void CMCETestUIEngineRefer::BaseConstructL()
    {
    }


// -----------------------------------------------------------------------------
// CMCETestUIEngineRefer::~CMCETestUIEngineRefer
// -----------------------------------------------------------------------------
//
EXPORT_C CMCETestUIEngineRefer::~CMCETestUIEngineRefer()
    {
    iCommands.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineRefer::Event
// -----------------------------------------------------------------------------
//
EXPORT_C CMceRefer& CMCETestUIEngineRefer::Refer()
    {
    return *iRefer;
    }


// -----------------------------------------------------------------------------
// CMCETestUIEngineRefer::GetCommands
// -----------------------------------------------------------------------------
//
const RPointerArray<TMCETestUIEngineCmdBase>& 
            CMCETestUIEngineRefer::GetCommandsL()
    {
    User::LeaveIfError( iRefer ? KErrNone : KErrNotReady );

    TInt state = iRefer->State();
    

	switch( state ) 
		{
		case CMceRefer::EIdle: 
		    {	
		    
		    iCommands.Append( new (ELeave) 
		        TMCETestUIEngineCmdDeleteRefer( iEngine, *this ) );  
		    break;
		    }
		case CMceRefer::EPending: 
		    {	
			iCommands.Append( new (ELeave) 
		        TMCETestUIEngineCmdDeleteRefer( iEngine, *this ) );	     
		    break;
		    }    
	   	case CMceRefer::EAccepted:
		    {	
		    iCommands.Append( new (ELeave) 
		        TMCETestUIEngineCmdCreateSessionFromRefer( iEngine, *this ) );  
		    break;	         
		    iCommands.Append( new (ELeave) 
		        TMCETestUIEngineCmdDeleteRefer( iEngine, *this ) );
		    	    
		    break;
		    }
		case CMceRefer::ETerminated: 
		    {	
			iCommands.Append( new (ELeave) 
		        TMCETestUIEngineCmdDeleteRefer( iEngine, *this ) );       
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
// CMCETestUIEngineRefer::State
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& CMCETestUIEngineRefer::State() 
    {
    if ( !iRefer ) 
        {
        User::Panic( _L16("CMCETestUIEngine"), 0 );
        }
        
    TInt state = iRefer->State();

	switch( state ) 
		{
		case CMceRefer::EIdle: 
		    return KReferStateIdle; 
		    break;
		case CMceRefer::EPending: 
		    return KReferStatePending; 
		    break;
		case CMceRefer::EAccepted: 
		    return KReferStateAccepted; 
		    break;
		case CMceRefer::ETerminated: 
		    return KReferStateTerminated; 
		    break;
		default: 
		    return KReferStateUnknown; 
		    break;
		}    
    }

