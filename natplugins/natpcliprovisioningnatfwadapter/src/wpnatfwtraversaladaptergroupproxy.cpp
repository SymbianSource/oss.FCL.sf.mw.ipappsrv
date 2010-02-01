/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  NAT-FW Traversal adapter ImplementationGroupProxy
*
*/


// INCLUDE FILES
#include <e32std.h>
#include <ecom/implementationproxy.h>
#include "wpnatfwtraversaladapteruids.h"
#include "natfwtraversaladapter.h"

// CONSTANTS
const TImplementationProxy KImplementationTable[] = 
    {
	IMPLEMENTATION_PROXY_ENTRY( KProvisioningNATFWTraversalAdapterUid, 
	                            CNATFWTraversalAdapter::NewL )
    };


// ============================= LOCAL FUNCTIONS ==============================

// ============================ MEMBER FUNCTIONS ==============================

// ========================== OTHER EXPORTED FUNCTIONS ========================
// ----------------------------------------------------------------------------
// ImplementationGroupProxy
// Returns: TImplementationProxy*: pointer to TImplementationProxy
// ----------------------------------------------------------------------------

EXPORT_C const TImplementationProxy* ImplementationGroupProxy( 
    TInt& aTableCount )
    {
    aTableCount = sizeof( KImplementationTable ) / 
        sizeof( TImplementationProxy );
    return KImplementationTable;
    }
//  End of File
