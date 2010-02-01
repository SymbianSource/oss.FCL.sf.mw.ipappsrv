/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Asynchronous subthread event handler
*
*/




// INCLUDE FILES
#include "MmccEvents.h"
#include "Mcctesteventhandler.h"
#include "mccinternaldef.h"
#include "mccinternalevents.h"


// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS
const TUid KAmrPayloadUid = { KDllUidAmrPayloadFormat };
const TUint KMaxSignalsInQueue( 20 );

// MACROS

// LOCAL CONSTANTS AND MACROS
// Panic text in debug builds
#ifdef _DEBUG
_LIT( KTxtMccEventHandler, "MccEventHandler" );
#endif
    
// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccTestEventHandler::CMccTestEventHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
CMccTestEventHandler::CMccTestEventHandler()
    {
    };

// -----------------------------------------------------------------------------
// CMccTestEventHandler::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
void CMccTestEventHandler::ConstructL()
    {   
    };

// -----------------------------------------------------------------------------
// CMccTestEventHandler::NewL
// Static constructor.
// -----------------------------------------------------------------------------
CMccTestEventHandler* CMccTestEventHandler::NewL()
    {
    CMccTestEventHandler* self = new ( ELeave ) CMccTestEventHandler();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    
    return self;
    };

// -----------------------------------------------------------------------------
// CMccTestEventHandler::~CMccTestEventHandler
// Destructor
// -----------------------------------------------------------------------------  
CMccTestEventHandler::~CMccTestEventHandler()
    {
    };

// -----------------------------------------------------------------------------
// CMccTestEventHandler::ChangeState
// Changes the state of the eventhandler by using async request for itself
// -----------------------------------------------------------------------------
void CMccTestEventHandler::ChangeState( const TMccEventHandlerState& /*aState*/ )
    {
    };

// -----------------------------------------------------------------------------
// CMccTestEventHandler::CompleteClientRequest
// Completes a client request
// -----------------------------------------------------------------------------
void CMccTestEventHandler::CompleteClientRequest()
    {
    };


// -----------------------------------------------------------------------------
// CMccTestEventHandler::SendEventToClient
// -----------------------------------------------------------------------------
TInt CMccTestEventHandler::SendEventToClient( const TMMFEvent& aEvent )
    {
    TMccEvent* mccEventPtr = 0;
	if ( IS_MCC_EVENT( aEvent ) )
	    {
	    mccEventPtr = 
	        reinterpret_cast<const TMccInternalEvent&>( aEvent ).iMccEvent;                                     
	    }
	
	if ( mccEventPtr )
	    {
	    iLastEvent = *mccEventPtr;
	    }
	else
	    {
	    iLastEvent = TMccEvent();
	    }

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccTestEventHandler::SendMccEventToClient
// Handles a Mcc specific event
// -----------------------------------------------------------------------------
TInt CMccTestEventHandler::SendMccEventToClient( TMccEvent& /*aEvent*/ )
    {
    return KErrNone;
    };
    

// -----------------------------------------------------------------------------
// CMccTestEventHandler::StateChange
// -----------------------------------------------------------------------------       
void CMccTestEventHandler::StateChange( TInt /*aState*/, TUint32 /*aLink*/ )
    {
    }

// -----------------------------------------------------------------------------
// CMccTestEventHandler::ErrorOccured
// -----------------------------------------------------------------------------
void CMccTestEventHandler::ErrorOccured( TInt /*aError*/, TUint32 /*aSessionId*/, 
        TUint32 /*aLink*/, TUint32 /*aStreamId*/, TUint32 /*aEndpointId*/ )
    {
    }

// -----------------------------------------------------------------------------
// CMccTestEventHandler::ErrorNotify
// -----------------------------------------------------------------------------
void CMccTestEventHandler::ErrorNotify( TInt /*aErrCode*/ )
    {
    }
    
// -----------------------------------------------------------------------------
// CMccTestEventHandler::HandleAmrEvent
// Handles received AMR CMR event
// -----------------------------------------------------------------------------
//
void CMccTestEventHandler::HandleAmrEvent()
    {
    }

	
// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File
