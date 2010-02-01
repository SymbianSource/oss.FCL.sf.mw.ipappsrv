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




#include <e32base.h>
#include <ecom/implementationproxy.h>
#include "mccuids.hrh"
#include "mccanysource.h"
#include "mccanysink.h"

// Disabled PC-Lint warning for "suspicious typecast" caused by Symbian's
// ECom declarations (IMPLEMENTATION_PROXY_ENTRY that follows)
/*lint -e611 */

const TImplementationProxy ImplementationTable[] = 
    {
        IMPLEMENTATION_PROXY_ENTRY( KImplUidMccAnySource, CMccAnySource::NewSourceL ),
        IMPLEMENTATION_PROXY_ENTRY( KImplUidMccAnySink, CMccAnySink::NewSinkL )
    };

// -----------------------------------------------------------------------------
// ImplementationGroupProxy()
// -----------------------------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy( TInt& aTableCount )
    {
    aTableCount = sizeof( ImplementationTable ) / sizeof( TImplementationProxy );
    return ImplementationTable;
    }

