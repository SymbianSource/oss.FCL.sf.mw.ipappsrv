/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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



#include <e32std.h>
#include <ecom/ImplementationProxy.h>

#include "CTBCPTest.h"


// Map the interface UIDs to implementation factory functions
const TImplementationProxy ImplementationTable[] = 
	{
		IMPLEMENTATION_PROXY_ENTRY(0x10009DB3,	CTBCPTest::NewL),
		
	};




// -----------------------------------------------------------------------------
// DLL entry
// -----------------------------------------------------------------------------
//
 // Bullseye reguires following entry point
#ifdef _BullseyeCoverage
GLDEF_C int __stdcall __E32Dll(void*, unsigned, void*)
	{
	return 0;
	}
#else
#ifndef EKA2
// Starting point of the DLL (Removed in EKA2) 
GLDEF_C TInt E32Dll(TDllReason)
	{
	return(KErrNone);
	}
#endif
#endif // _BullseyeCoverage

// -----------------------------------------------------------------------------
// ImplementationGroupProxy
// Exported proxy for instantiation method resolution
// -----------------------------------------------------------------------------
//

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);

	return ImplementationTable;
	}




