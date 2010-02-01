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
* Description:    Implementation.
*
*/



#include "CTcMCEFactory.h"
#include "EComImplementationProxy.h"

const TImplementationProxy ImplementationTable [] =
    {
	//Implementation UID and pointer to instantiation method
    IMPLEMENTATION_PROXY_ENTRY( 0x10203355, CTcMCEFactory::NewL )
    };
    
#ifndef EKA2
// Starting point of the DLL (Removed in EKA2) 
GLDEF_C TInt E32Dll(TDllReason)
	{
	return(KErrNone);
	}
#endif
    
// -----------------------------------------------------------------------------
// ImplementationGroupProxy
// -----------------------------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(ImplementationTable)/sizeof(TImplementationProxy);
    return ImplementationTable;
    }
