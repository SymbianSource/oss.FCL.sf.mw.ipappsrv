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
#include "Mcccompletecallback.h"
#include "Mcculdatapath.h"
#include "Mccdldatapath.h"

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
    
    }

// -----------------------------------------------------------------------------
// CCompleteCallback::~CCompleteCallback
// Destructor
// -----------------------------------------------------------------------------
CCompleteCallback::~CCompleteCallback()
    {
    
    }

// -----------------------------------------------------------------------------
// CCompleteCallback::SignalDataPathComplete
// Signals that datapath is completed
// -----------------------------------------------------------------------------
void CCompleteCallback::SignalDataPathComplete( TInt /*aDataPathError*/ )
    {
    
    }

// -----------------------------------------------------------------------------
// CCompleteCallback::ActiveStatus
// Sets the object active and returns it's status
// -----------------------------------------------------------------------------
TRequestStatus& CCompleteCallback::ActiveStatus()
    {
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

    }

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
