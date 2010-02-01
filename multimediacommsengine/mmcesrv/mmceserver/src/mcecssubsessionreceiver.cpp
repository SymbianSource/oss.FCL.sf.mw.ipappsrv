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




#include "mcecssubsessionreceiver.h"
#include "mcecsserveritc.h"

class CMceCsServerITC;

// -----------------------------------------------------------------------------
// CMceCsSubSessionReceiver::NewL
// -----------------------------------------------------------------------------
//
CMceCsSubSessionReceiver* CMceCsSubSessionReceiver::NewL (CMceCsServerITC& aItc)
	{
    CMceCsSubSessionReceiver* self = CMceCsSubSessionReceiver::NewLC(aItc);
    CleanupStack::Pop(self);
    return self;
	}

// -----------------------------------------------------------------------------
// CMceCsSubSessionReceiver::NewLC
// -----------------------------------------------------------------------------
//
CMceCsSubSessionReceiver* CMceCsSubSessionReceiver::NewLC (CMceCsServerITC& aItc)
	{
    CMceCsSubSessionReceiver* self = new (ELeave) CMceCsSubSessionReceiver(aItc);
    CleanupStack::PushL (self);
    self->ConstructL ();
    return self;
	}


// -----------------------------------------------------------------------------
// CMceCsSubSessionReceiver::CMceCsSubSessionReceiver
// -----------------------------------------------------------------------------
//
CMceCsSubSessionReceiver::CMceCsSubSessionReceiver(CMceCsServerITC& aItc):
	CMceCsReceiverBase(aItc)
	{
	}

// -----------------------------------------------------------------------------
// CMceCsSubSessionReceiver::~CMceCsSubSessionReceiver
// -----------------------------------------------------------------------------
//
CMceCsSubSessionReceiver::~CMceCsSubSessionReceiver()
	{
	}

