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
* Description:  Implementation proxy
*
*/


// INCLUDES
#include <e32std.h>
#include <ecom/implementationproxy.h>
#include "nspinterface.h"

// LOCAL CONSTANTS AND MACROS
const TImplementationProxy ImplementationTable[] = 
    {
    IMPLEMENTATION_PROXY_ENTRY( 0x102832EF, CNSPInterface::NewL )
    };


// ========================== OTHER EXPORTED FUNCTIONS ========================

// ----------------------------------------------------------------------------
// ImplementationGroupProxy
//
// Function to return the implementation proxy table
// ----------------------------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy( 
                                                           TInt& aTableCount )
    {
    aTableCount = 
        ( sizeof( ImplementationTable ) / sizeof( TImplementationProxy ) );
    return ImplementationTable;
    }


// end of file
