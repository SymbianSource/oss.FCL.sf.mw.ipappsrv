/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    QosController test
*
*/




// INCLUDE FILES
#include <mmf/common/mmfbase.h>
#include <ecam.h>

//#include "mccrateadaptationobserver.h"
#include "mccresourcepool.h"
#include "mccrateadaptationobserver_stub.h"
#include "QosControllerLog.h"

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
// CMMccRateAdaptationObserverStub::CMMccRateAdaptationObserverStub
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMMccRateAdaptationObserverStub::CMMccRateAdaptationObserverStub()
    {
    }

// -----------------------------------------------------------------------------
// CMMccRateAdaptationObserverStub::~CMMccRateAdaptationObserverStub
// Destructor
// -----------------------------------------------------------------------------
//
CMMccRateAdaptationObserverStub::~CMMccRateAdaptationObserverStub()
    {
    __TRACE_MCC_QOS_CONTROLLER1("CMMccRateAdaptationObserverStub::~CMMccRateAdaptationObserverStub");
    delete iResourcePool;
    }

// -----------------------------------------------------------------------------
// CMMccRateAdaptationObserverStub::NewL
// Static constructor.
// -----------------------------------------------------------------------------
//
CMMccRateAdaptationObserverStub* CMMccRateAdaptationObserverStub::NewL()
    {
     __TRACE_MCC_QOS_CONTROLLER1("CMMccRateAdaptationObserverStub::NewL");

    CMMccRateAdaptationObserverStub* self = new ( ELeave ) CMMccRateAdaptationObserverStub;
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    
    return self;
    }

// -----------------------------------------------------------------------------
// CMMccRateAdaptationObserverStub::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMMccRateAdaptationObserverStub::ConstructL()
    {     
    __TRACE_MCC_QOS_CONTROLLER1("CMMccRateAdaptationObserverStub::ConstructL");

    iResourcePool = CMccResourcePool::NewL();        
    }
// -----------------------------------------------------------------------------
// CMMccRateAdaptationObserverStub::RateAdaptationRequest
// -----------------------------------------------------------------------------
//    
TInt CMMccRateAdaptationObserverStub::RateAdaptationRequest( 
    const TMccEvent& /*aInputData*/, 
    TMccEvent& /*aOutputData*/ )
    {
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMMccRateAdaptationObserverStub::RateAdaptationAlert
// -----------------------------------------------------------------------------
//    
TInt CMMccRateAdaptationObserverStub::RateAdaptationAlert( 
    const TMccEvent& /*aAdaptationData*/,
    TAlertType /*aAlertType*/ )
    {
     __TRACE_MCC_QOS_CONTROLLER1("CMMccRateAdaptationObserverStub::RateAdaptationAlert");

    return KErrNone;
    }
// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File
