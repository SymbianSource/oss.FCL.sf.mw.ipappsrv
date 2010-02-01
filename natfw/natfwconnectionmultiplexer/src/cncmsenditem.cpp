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
* Description:    Holds a data packet and sender observer for buffering if socket is busy.
*
*/




#include "cncmsenditem.h"


// ---------------------------------------------------------------------------
// CNcmSendItem::CNcmSendItem
// ---------------------------------------------------------------------------
//
CNcmSendItem::CNcmSendItem(
    MNcmSenderObserver* aObserver ) :
    iObserver( aObserver )
    {
    }


// ---------------------------------------------------------------------------
// CNcmSendItem::NewL
// ---------------------------------------------------------------------------
//
CNcmSendItem* CNcmSendItem::NewL( const TDesC8& aData,
    MNcmSenderObserver* aSenderObserver )
    {
    CNcmSendItem* self = CNcmSendItem::NewLC( aData, aSenderObserver );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNcmSendItem::NewLC
// ---------------------------------------------------------------------------
//
CNcmSendItem* CNcmSendItem::NewLC( const TDesC8& aData,
    MNcmSenderObserver* aSenderObserver )
    {
    CNcmSendItem* self=new ( ELeave ) CNcmSendItem( aSenderObserver );
    CleanupStack::PushL( self );
    self->ConstructL( aData );
    return self;
    }


// ---------------------------------------------------------------------------
// CNcmSendItem::ConstructL
// ---------------------------------------------------------------------------
//
void CNcmSendItem::ConstructL( const TDesC8& aData )
    {
    iData = aData.AllocL();
    }


// ---------------------------------------------------------------------------
// CNcmSendItem::~CNcmSendItem
// ---------------------------------------------------------------------------
//
CNcmSendItem::~CNcmSendItem()
    {
    delete iData;
    iObserver = NULL;
    }


// ---------------------------------------------------------------------------
// CNcmSendItem::Data
// ---------------------------------------------------------------------------
//
const TDesC8& CNcmSendItem::Data() const
    {
    return *iData;
    }

    
// ---------------------------------------------------------------------------
// CNcmSendItem::Observer
// ---------------------------------------------------------------------------
//
MNcmSenderObserver* CNcmSendItem::Observer()
    {
    return iObserver;
    }


// ---------------------------------------------------------------------------
// CNcmSendItem::SetSendInProgress
// ---------------------------------------------------------------------------
//
void CNcmSendItem::SetSendInProgress( TBool aSendInProgress )
    {
    iSendInProgress = aSendInProgress;
    }


// ---------------------------------------------------------------------------
// CNcmSendItem::SendInProgress
// ---------------------------------------------------------------------------
//
TBool CNcmSendItem::SendInProgress() const
    {
    return iSendInProgress;
    }
