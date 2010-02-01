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
* Description:    Waits for sink buffer flushing in datapaths
*
*/




// INCLUDE FILES
#include <mmf/common/mmfcontroller.h>
#include "mcccompletecallback.h"
#include "mcculdatapath.h"
#include "mccdldatapath.h"

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCompleteCallback::CCompleteCallback
// Constructor
// -----------------------------------------------------------------------------
CCompleteCallback::CCompleteCallback( MAsyncEventHandler& aEventhandler, 
        TBool aWaitForSink ) : CActive( EPriorityNormal ), 
        iEventhandler( aEventhandler ),
        iWaitForSink( aWaitForSink )
    {
    CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
// CCompleteCallback::~CCompleteCallback
// Destructor
// -----------------------------------------------------------------------------
CCompleteCallback::~CCompleteCallback()
    {
    Cancel();
    }

// -----------------------------------------------------------------------------
// CCompleteCallback::SignalDataPathComplete
// Signals that datapath is completed
// -----------------------------------------------------------------------------
void CCompleteCallback::SignalDataPathComplete( TInt aDataPathError )
    {
    iDataPathComplete = ETrue;
    iDataPathError = aDataPathError;
    if ( !IsActive() )
        {
        // Signal ourselves to run with the given completion code
        TRequestStatus* status = &ActiveStatus();
        User::RequestComplete( status, KErrNone );
        }
    }

// -----------------------------------------------------------------------------
// CCompleteCallback::ActiveStatus
// Sets the object active and returns it's status
// -----------------------------------------------------------------------------
TRequestStatus& CCompleteCallback::ActiveStatus()
    {
    SetActive();
    return iStatus;
    }

// -----------------------------------------------------------------------------
// CCompleteCallback::DoCancel
// Cancel worker functions
// -----------------------------------------------------------------------------
void CCompleteCallback::DoCancel()
    {
    }

// -----------------------------------------------------------------------------
// CCompleteCallback::RunL
// Active object event handling
// -----------------------------------------------------------------------------
void CCompleteCallback::RunL()
    {
    if ( iWaitForSink )
        {
        if ( iDataPathComplete && iSinkComplete )
            {
            iDataPathComplete = EFalse;
            iSinkComplete = EFalse;
            // If we have to wait for the sink to complete, always use the sink error
            TMMFEvent event( KMMFEventCategoryPlaybackComplete, iSinkError );
            iEventhandler.SendEventToClient( event );
            }
        }
    else if ( iDataPathComplete )
        {
        iDataPathComplete = EFalse;
        iSinkComplete = EFalse;        
        TMMFEvent event( KMMFEventCategoryPlaybackComplete, iSinkError );
        iEventhandler.SendEventToClient( event );
        }
    else
        {
        // RunL activated for someunknown reason, so leave
        User::Leave( KErrUnknown );
        }
    };

// -----------------------------------------------------------------------------
// CCompleteCallback::RunError
// Active object event handling
// -----------------------------------------------------------------------------
TInt CCompleteCallback::RunError( TInt /*aError*/ )
	{
	return KErrNone;
	}

// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File  
