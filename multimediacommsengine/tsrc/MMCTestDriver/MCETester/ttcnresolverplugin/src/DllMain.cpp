/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Implementation
*
*/



#include <e32std.h>
#include <e32svr.h>
#include <e32base.h>
#include <ecom/ImplementationProxy.h>
#include "CTTCNResolverPlugin1.h"

// Map the interface UIDs to implementation factory functions
const TImplementationProxy Implementations[] =
    {
    IMPLEMENTATION_PROXY_ENTRY( KTTCNResolverPluginUID1,
		                        CTTCNResolverPlugin1::NewL )
    };


// -----------------------------------------------------------------------------
// ImplementationGroupProxy
//
// Exported proxy for instantiation method resolution
// -----------------------------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy( TInt& aCount )
	{
	aCount = sizeof( Implementations ) / sizeof( TImplementationProxy );
	return Implementations;
	}

// -----------------------------------------------------------------------------
// E32Dll (Removed in EKA2) 
// -----------------------------------------------------------------------------
//
#ifndef EKA2
GLDEF_C TInt E32Dll(TDllReason)
	{
	return(KErrNone);
	}
#endif

