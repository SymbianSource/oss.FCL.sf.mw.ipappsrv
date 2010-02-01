/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    This file contains the exported proxy for instantiation method
*                resolution for the Redundancy RTP payload format plugin.
*
*/




// INCLUDE FILES
#include <e32std.h>
#include <ecom/implementationproxy.h>
#include "mccredpayloadread.h"
#include "mccredpayloadwrite.h"
#include "mccuids.hrh"

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// Disabled PC-Lint warning for "suspicious typecast" caused by Symbian's
// ECom declarations (IMPLEMENTATION_PROXY_ENTRY that follows)
/*lint -e611 */

const TImplementationProxy ImplementationTable[] = 
    {
    IMPLEMENTATION_PROXY_ENTRY( KImplUidRedPayloadFormatDecode,
        CMccRedPayloadRead::NewL ),
    IMPLEMENTATION_PROXY_ENTRY( KImplUidRedPayloadFormatEncode,
        CMccRedPayloadWrite::NewL )
    };


// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ==============================

// ============================ MEMBER FUNCTIONS ==============================

// ========================== OTHER EXPORTED FUNCTIONS ========================

// ----------------------------------------------------------------------------
// ImplementationGroupProxy
//
// Function to return the implementation proxy table
// ----------------------------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(ImplementationTable) / sizeof( TImplementationProxy );

    return ImplementationTable;
    }

//  End of File
