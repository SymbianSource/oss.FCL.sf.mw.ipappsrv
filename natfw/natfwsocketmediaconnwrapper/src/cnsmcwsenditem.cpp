/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Constructs a send buffer.
*
*/




#include "cnsmcwsenditem.h"

const TInt CNsmcwSendItem::iOffset = _FOFF( CNsmcwSendItem, iLink );

// ---------------------------------------------------------------------------
// CNsmcwSendItem::CNsmcwSendItem()
// ---------------------------------------------------------------------------
//
CNsmcwSendItem::CNsmcwSendItem()
    {
    }


// ---------------------------------------------------------------------------
// CNsmcwSendItem::ConstructL()
// ---------------------------------------------------------------------------
//
void CNsmcwSendItem::ConstructL( const TDesC8& aPacket )
    {
    iPacket = aPacket.AllocL();
    }


// ---------------------------------------------------------------------------
// CNsmcwSendItem::NewL()
// ---------------------------------------------------------------------------
//
CNsmcwSendItem* CNsmcwSendItem::NewL( const TDesC8& aPacket )
    {
    CNsmcwSendItem* self = CNsmcwSendItem::NewLC( aPacket );
    CleanupStack::Pop( self );
    return self;
    }

    
// ---------------------------------------------------------------------------
// CNsmcwSendItem::NewLC()
// ---------------------------------------------------------------------------
//
CNsmcwSendItem* CNsmcwSendItem::NewLC( const TDesC8& aPacket )
    {
    CNsmcwSendItem* self = new ( ELeave ) CNsmcwSendItem();
    CleanupStack::PushL( self );
    self->ConstructL( aPacket );
    return self;
    }


// ---------------------------------------------------------------------------
// CNsmcwSendItem::~CNsmcwSendItem()
// ---------------------------------------------------------------------------
//
CNsmcwSendItem::~CNsmcwSendItem()
    {
    delete iPacket;
    }


// ---------------------------------------------------------------------------
// CNsmcwSendItem::GetData()
// ---------------------------------------------------------------------------
//
const TDesC8& CNsmcwSendItem::GetData() const
    {
    return *iPacket;
    }
