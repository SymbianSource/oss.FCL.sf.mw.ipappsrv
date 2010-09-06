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
* Description:    Provides a comfort noise generator class
*
*/


#include <e32base.h>
#include <ImplementationProxy.h>
#include "mccuids.hrh"
#include "mccmsrpsource.h"
#include "mccmsrpsink.h"

const TImplementationProxy ImplementationTable[] = 
    {
        IMPLEMENTATION_PROXY_ENTRY( KImplUidMccMsrpSource, CMccMsrpSource::NewSourceL ),
        IMPLEMENTATION_PROXY_ENTRY( KImplUidMccMsrpSink, CMccMsrpSink::NewSinkL )
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

