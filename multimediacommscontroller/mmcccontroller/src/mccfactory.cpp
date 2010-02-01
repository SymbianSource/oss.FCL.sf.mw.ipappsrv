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
* Description:    
*
*/




// INCLUDE FILES
#include "mcccontroller.h"
#include "mccuids.hrh"
#include <ecom/implementationproxy.h>

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
// DLL Entry point
// -----------------------------------------------------------------------------
//
#ifndef EKA2
GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
    {
    return(KErrNone);
    };
#endif

// -----------------------------------------------------------------------------
// Exported proxy for instantiation method resolution
// Define the interface UIDs
// -----------------------------------------------------------------------------
//
#ifndef EKA2
const TImplementationProxy ImplementationTable[] =
    {
        {{KImplUidMccController}, CMccController::NewL}
    };
#else

// Disabled PC-Lint warning for "suspicious typecast" caused by Symbian's
// ECom declarations (IMPLEMENTATION_PROXY_ENTRY that follows)
/*lint -e611 */

const TImplementationProxy ImplementationTable[] = 
    {
        IMPLEMENTATION_PROXY_ENTRY( KImplUidMccController, CMccController::NewL )
    };
#endif

// -----------------------------------------------------------------------------
// ImplementationGroupProxy()
// -----------------------------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
    return ImplementationTable;
    };
