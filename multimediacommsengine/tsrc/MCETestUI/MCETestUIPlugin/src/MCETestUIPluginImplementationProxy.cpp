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
* Description:    MCETestUIPluginImplementationProxy implementation
*
*/





// INCLUDE FILES
#include <e32base.h>
#include "MCETestUIPlugin.h"
#include <e32svr.h>

// MACROS
#define SSC_MCETestUIPLUGIN_DEBUG_PRINT1( a )   RDebug::Print( a )

// CONSTANTS

// Exported proxy for instantiation method resolution
// Use the interface implementation UID and Implementation factory method
// as a pair for ECom instantiation.
const TImplementationProxy Implementations[] = 
	{
	IMPLEMENTATION_PROXY_ENTRY(	0x10281F9C, CMCETestUIPlugin::NewL )
	};


// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// ImplementationGroupProxy
// Function to return the implementation proxy table 
// -----------------------------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy( TInt& aTableCount )
    {
    SSC_MCETestUIPLUGIN_DEBUG_PRINT1( _L("MCETestUIPlugin ImplementationGroupProxy"));
    aTableCount = sizeof( Implementations ) / sizeof( TImplementationProxy );

    return Implementations;
    }

// -----------------------------------------------------------------------------
// E32Dll
// DLL entry point
// -----------------------------------------------------------------------------
//
#ifndef EKA2
GLDEF_C TInt E32Dll ( TDllReason /*aReason*/ )
    {
    return ( KErrNone );
    }
#endif
