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




#include "mcecssessionreceiver.h"
#include "mcecsserveritc.h"
#include "mcecsreceiveitem.h"

// -----------------------------------------------------------------------------
// CMceCsSessionReceiver::NewL
// -----------------------------------------------------------------------------
//
CMceCsSessionReceiver* CMceCsSessionReceiver::NewL( TMceManagerType aType, 
                                                    CMceCsServerITC& aItc)
	{
    CMceCsSessionReceiver* self = CMceCsSessionReceiver::NewLC( aType, aItc );
    CleanupStack::Pop( self );
    return self;
	}

// -----------------------------------------------------------------------------
// CMceCsSessionReceiver::NewLC
// -----------------------------------------------------------------------------
//
CMceCsSessionReceiver* CMceCsSessionReceiver::NewLC( TMceManagerType aType,     
                                                     CMceCsServerITC& aItc)
	{
    CMceCsSessionReceiver* self = 
        new (ELeave) CMceCsSessionReceiver( aType, aItc );
    CleanupStack::PushL ( self );
    self->ConstructL();
    return self;
	}

// -----------------------------------------------------------------------------
// CMceCsSessionReceiver::CMceCsSessionReceiver
// -----------------------------------------------------------------------------
//
CMceCsSessionReceiver::CMceCsSessionReceiver( TMceManagerType aType, 
                                              CMceCsServerITC& aItc ):
	CMceCsReceiverBase( aItc ),
	iType( aType )
	{
	}

// -----------------------------------------------------------------------------
// CMceCsSessionReceiver::Type
// -----------------------------------------------------------------------------
//
TMceManagerType CMceCsSessionReceiver::Type() const
	{
	return iType;
	}

// -----------------------------------------------------------------------------
// CMceCsSessionReceiver::~CMceCsSessionReceiver
// -----------------------------------------------------------------------------
//
CMceCsSessionReceiver::~CMceCsSessionReceiver()
	{
	}
	
