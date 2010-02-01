/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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


#include <ecom/implementationproxy.h>
#include "cnattraversalcontroller.h"


const TImplementationProxy ImplementationTable [] =
    {
    // Disable PC-Lint warning for "suspicious typecast" caused by
    ///Symbian's IMPLEMENTATION_PROXY_ENTRY declaration
    /*lint -e611 */
    IMPLEMENTATION_PROXY_ENTRY( 0x102747D1, CNATTraversalController::NewL )
    };

// -----------------------------------------------------------------------------
// ImplementationGroupProxy
// -----------------------------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(ImplementationTable)/sizeof(TImplementationProxy);
    return ImplementationTable;
    }
