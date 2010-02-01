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
* Description:  Implementation.
*
*/

#include "CTTCNResolverPlugin3.h"
#include <apacmdln.h>

// -----------------------------------------------------------------------------
// CTTCNResolverPlugin3::NewL
// -----------------------------------------------------------------------------
//
CTTCNResolverPlugin3* CTTCNResolverPlugin3::NewL()
	{
	CTTCNResolverPlugin3* self = new( ELeave ) CTTCNResolverPlugin3;
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	return self;
	}

// -----------------------------------------------------------------------------
// CTTCNResolverPlugin3::MyUid
// -----------------------------------------------------------------------------
//
TInt32 CTTCNResolverPlugin3::MyUid() const
    {
    return KTTCNResolverPluginUID3;
    }

