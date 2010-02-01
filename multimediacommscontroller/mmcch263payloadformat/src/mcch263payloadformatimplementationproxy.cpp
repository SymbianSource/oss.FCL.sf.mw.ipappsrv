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
#include <e32std.h>
#include <ecom/implementationproxy.h>
#include "mccuids.hrh"
#include "mcch263payloadformatwrite.h"

// CONSTANTS

// Exported proxy for instantiation method resolution
// Define the interface UIDs
#ifndef EKA2
const TImplementationProxy ImplementationTable[] = 
    {
        { { KImplUidH263PayloadFormatEncode },    CMccH263PayloadFormatWrite::NewL }
    };
#else

// Disabled PC-Lint warning for "suspicious typecast" caused by Symbian's
// ECom declarations (IMPLEMENTATION_PROXY_ENTRY that follows)
/*lint -e611 */

const TImplementationProxy ImplementationTable[] = 
    {
    IMPLEMENTATION_PROXY_ENTRY( KImplUidH263PayloadFormatEncode, CMccH263PayloadFormatWrite::NewL )
    };
#endif

// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// ImplementationGroupProxy
//
// Function to return the implementation proxy table
// -----------------------------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy( TInt& aTableCount )
    {
    aTableCount = sizeof( ImplementationTable ) / sizeof( TImplementationProxy );

    return ImplementationTable;
    }

// -----------------------------------------------------------------------------
// E32Dll
//
// DLL entry point
// -----------------------------------------------------------------------------
//
#ifndef EKA2

GLDEF_C TInt E32Dll ( TDllReason /*aReason*/ )
    {
    return ( KErrNone );
    }
    
#endif

// End of file
