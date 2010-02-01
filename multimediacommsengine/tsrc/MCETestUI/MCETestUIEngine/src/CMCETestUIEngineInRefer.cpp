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
#include <MCEInRefer.h>
#include "MCETestUIEngineDefaultValues.h"

#include "CMCETestUIEngine.h"
#include "CMCETestUIEngineRefer.h"
#include "CMCETestUIEngineInRefer.h"
#include "TMCETestUIEngineCmdAcceptEvent.h"
#include "TMCETestUIEngineCmdRejectEvent.h"
#include "TMCETestUIEngineCmdDeleteEvent.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMCETestUIEngineInRefer::NewL()
// -----------------------------------------------------------------------------
//
CMCETestUIEngineInRefer* CMCETestUIEngineInRefer::NewL(
            CMCETestUIEngine& aEngine,
            CMceInRefer* aInRefer ) 
    {
    CMCETestUIEngineInRefer* self = 
        new (ELeave) CMCETestUIEngineInRefer( aEngine, aInRefer );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;  
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineInRefer::CMCETestUIEngineInRefer
// -----------------------------------------------------------------------------
//
CMCETestUIEngineInRefer::CMCETestUIEngineInRefer(
            CMCETestUIEngine& aEngine,
            CMceInRefer* aInRefer )
    :CMCETestUIEngineRefer( aEngine ),
     iInRefer( aInRefer ),
     iId( 0 )
    {
    }
     

// -----------------------------------------------------------------------------
// CMCETestUIEngineInRefer::ConstructL()
// -----------------------------------------------------------------------------
//
void CMCETestUIEngineInRefer::ConstructL()
    {
    User::LeaveIfError( iInRefer ? KErrNone : KErrArgument );
     
    BaseConstructL();
    iRefer = iInRefer;
    
       
    // Get identity
    iId = iEngine.NextInReferId();
    }


// -----------------------------------------------------------------------------
// CMCETestUIEngineInRefer::~CMCETestUIEngineInRefer
// -----------------------------------------------------------------------------
//
EXPORT_C CMCETestUIEngineInRefer::~CMCETestUIEngineInRefer()
    {
    delete iInRefer;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineInEvent::GetCommands
// -----------------------------------------------------------------------------
//
const RPointerArray<TMCETestUIEngineCmdBase>& 
            CMCETestUIEngineInRefer::GetCommandsL()
    {
    iCommands.ResetAndDestroy();

    // Get base class commands
    CMCETestUIEngineRefer::GetCommandsL();

    // Get own commands
    TInt state = iInRefer->State();

	switch( state ) 
		{
		case CMceRefer::EIdle: 
		    {
		  
            iCommands.Append( new (ELeave) 
		        TMCETestUIEngineCmdAcceptRefer( iEngine, *this ) );
		    iCommands.Append( new (ELeave) 
		        TMCETestUIEngineCmdRejectRefer( iEngine, *this ) );	  
		    iCommands.Append( new (ELeave) 
		        TMCETestUIEngineCmdRespondRefer( iEngine, *this ) );
		 		                                                                 		            
		    break;
		    }
	   	case CMceRefer::EPending: 
	   		{
	   	/*	 
		  */  break;   
	   		}
	   	case CMceRefer::EAccepted:
		    {		         
		    
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
// CMCETestUIEngineInRefer::Id
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMCETestUIEngineInRefer::Id() const
    {
    return iId;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineInRefer::InSession
// -----------------------------------------------------------------------------
//
CMceInRefer& CMCETestUIEngineInRefer::InRefer()
    {
    return *iInRefer;
    }



