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
* Description:    
*
*/



// INCLUDE FILES
#include "mcccamerahandler_STUB.h"

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
// CMccCameraHandlerStub::NewL
// -----------------------------------------------------------------------------
//
CMccCameraHandlerStub* CMccCameraHandlerStub::NewL()
    {
    CMccCameraHandlerStub* self = new ( ELeave ) CMccCameraHandlerStub;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMccCameraHandlerStub::~CMccCameraHandlerStub
// -----------------------------------------------------------------------------
//        
CMccCameraHandlerStub::~CMccCameraHandlerStub()
    {
    }

// -----------------------------------------------------------------------------
// CMccCameraHandlerStub::Handle
// -----------------------------------------------------------------------------
//
TUint32 CMccCameraHandlerStub::Handle()
    {
    return 0;
    }

// -----------------------------------------------------------------------------
// CMccCameraHandlerStub::IsCameraReady
// -----------------------------------------------------------------------------
//        
TBool CMccCameraHandlerStub::IsCameraReady()
    {
    return iIsCameraReady;
    }

// -----------------------------------------------------------------------------
// CMccCameraHandlerStub::MonitorCameraReservation
// -----------------------------------------------------------------------------
//        
TInt CMccCameraHandlerStub::MonitorCameraReservation( 
    MMccCameraHandlerObserver& /*aObserver*/, 
    TBool aForce )
    {
    iForce = aForce;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccCameraHandlerStub::StopCameraMonitoring
// -----------------------------------------------------------------------------
// 
TInt CMccCameraHandlerStub::StopCameraMonitoring( 
    MMccCameraHandlerObserver& /*aObserver*/, 
    TBool aForce )
    {
    iForce = aForce;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccCameraHandlerStub::CMccCameraHandlerStub
// -----------------------------------------------------------------------------
//      
CMccCameraHandlerStub::CMccCameraHandlerStub() : iIsCameraReady( ETrue )
    {
    }

// -----------------------------------------------------------------------------
// CMccCameraHandlerStub::ConstructL
// -----------------------------------------------------------------------------
// 
void CMccCameraHandlerStub::ConstructL()
    {
    }

