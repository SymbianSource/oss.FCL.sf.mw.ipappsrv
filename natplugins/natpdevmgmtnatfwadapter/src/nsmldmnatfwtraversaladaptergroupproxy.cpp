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
#include "nsmldmnatfwtraversaladapter.h"

const TUint KNSmlDMNATFWAdapterImplUid    = 0x1020E58F;

// ---------------------------------------------------------------------------
// TImplementationProxy ImplementationTable[]
// Needed because of ECOM architecture
// ---------------------------------------------------------------------------
//
const TImplementationProxy ImplementationTable[] = 
    {
    IMPLEMENTATION_PROXY_ENTRY( KNSmlDMNATFWAdapterImplUid, 
        CNSmlDmNATFWTraversalAdapter::NewL )
    };

// ---------------------------------------------------------------------------
// ImplementationGroupProxy()
// Needed because of ECOM architecture
// ---------------------------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy( 
    TInt& aTableCount )
    {
    aTableCount = sizeof( ImplementationTable ) / 
        sizeof( TImplementationProxy );
    return ImplementationTable;
    }
  
