/*
* Copyright (c) 2004-2006 Nokia Corporation and/or its subsidiary(-ies).
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
*                resolution for the AMR RTP payload format plugin.
*
*/




// INCLUDE FILES
#include <ecom/implementationproxy.h>
#include "amrpayloadformatread.h"
#include "amrpayloadformatwrite.h"
#include "mccuids.hrh"

// CONSTANTS

// Disabled PC-Lint warning for "suspicious typecast" caused by Symbian's
// ECom declarations (IMPLEMENTATION_PROXY_ENTRY that follows)
/*lint -e611 */

// Exported proxy for instantiation method resolution
// Define the interface UIDs
const TImplementationProxy ImplementationTable[] = 
    {
    IMPLEMENTATION_PROXY_ENTRY( KImplUidAmrPayloadFormatDecode,
            CAmrPayloadFormatRead::NewL ),
    IMPLEMENTATION_PROXY_ENTRY( KImplUidAmrPayloadFormatEncode,
            CAmrPayloadFormatWrite::NewL )
    };


// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// ImplementationGroupProxy
// Function to return the implementation proxy table
// -----------------------------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy( TInt& aTableCount )
    {
    aTableCount = sizeof( ImplementationTable ) / sizeof( TImplementationProxy );
    return ImplementationTable;
    };

