/*
* Copyright (c) 2006 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  NAT-FW Traversal access point specific settings
*
*/


// INCLUDE FILES
#include    <e32base.h>
#include    "natfwiapspecificitem.h"
#include    "wpnatfwdebug.h"

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CNATFWIAPSpecificItem::NewL
//
// ---------------------------------------------------------------------------
//
CNATFWIAPSpecificItem* CNATFWIAPSpecificItem::NewL()
    {
    CNATFWIAPSpecificItem* self = NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CNATFWIAPSpecificItem::NewLC
// 
// ---------------------------------------------------------------------------
//
CNATFWIAPSpecificItem* CNATFWIAPSpecificItem::NewLC()
    {
    CNATFWIAPSpecificItem* self = new (ELeave) CNATFWIAPSpecificItem();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

//-----------------------------------------------------------------------------
// CNATFWIAPSpecificItem::ConstructL()
//
//-----------------------------------------------------------------------------
//
void CNATFWIAPSpecificItem::ConstructL()
    {
    iNapDefName = HBufC8::NewL( 0 );
    }
    
// Destructor
//-----------------------------------------------------------------------------
// CNATFWIAPSpecificItem::~CNATFWIAPSpecificItem()
//
//-----------------------------------------------------------------------------
//
CNATFWIAPSpecificItem::~CNATFWIAPSpecificItem()
    {
    delete iNapDefName;
    }

// ---------------------------------------------------------------------------
// CNATFWIAPSpecificItem::NapDefName
//
// ---------------------------------------------------------------------------
//
const TDesC8& CNATFWIAPSpecificItem::NapDefName() const
    {
    return *iNapDefName;
    }

// ---------------------------------------------------------------------------
// CNATFWIAPSpecificItem::NapDef
//
// ---------------------------------------------------------------------------
//
CWPCharacteristic* CNATFWIAPSpecificItem::NapDef()
    {
    return iNapDef;
    }

// ---------------------------------------------------------------------------
// CNATFWIAPSpecificItem::SetNapDefNameL
//
// ---------------------------------------------------------------------------
//
void CNATFWIAPSpecificItem::SetNapDefNameL( const TDesC8& aToNapId )
    {
    delete iNapDefName;
    iNapDefName = NULL;
    iNapDefName = aToNapId.AllocL();
    }

// ---------------------------------------------------------------------------
// CNATFWIAPSpecificItem::SetNapDefNameL
//
// ---------------------------------------------------------------------------
//
void CNATFWIAPSpecificItem::SetNapDef( CWPCharacteristic* aNapDef )
    {
    iNapDef = aNapDef;
    }

// ---------------------------------------------------------------------------
// CNATFWIAPSpecificItem::SetIAPKey
//
// ---------------------------------------------------------------------------
//
void CNATFWIAPSpecificItem::SetIAPKey( TUint32 aIAPKey )
    {
    iIAPKey = aIAPKey;
    }

// ---------------------------------------------------------------------------
// CNATFWIAPSpecificItem::SetIAPId
//
// ---------------------------------------------------------------------------
//
void CNATFWIAPSpecificItem::SetIAPId( TUint32 aIAPId )
    {
    iIAPId = aIAPId;
    }

// ---------------------------------------------------------------------------
// CNATFWIAPSpecificItem::SetIntervalUDP
//
// ---------------------------------------------------------------------------
//
void CNATFWIAPSpecificItem::SetIntervalUDP( TInt aIntervalUDP )
    {
    iIntervalUDP = aIntervalUDP;
    }

// ---------------------------------------------------------------------------
// CNATFWIAPSpecificItem::SetIntervalTCP
//
// ---------------------------------------------------------------------------
//
void CNATFWIAPSpecificItem::SetIntervalTCP( TInt aIntervalTCP )
    {
    iIntervalTCP = aIntervalTCP;
    }

// ---------------------------------------------------------------------------
// CNATFWIAPSpecificItem::SetSTUNRetransmitTimer
//
// ---------------------------------------------------------------------------
//
void CNATFWIAPSpecificItem::SetSTUNRetransmitTimer( TInt aSTUNRetransmitTimer )
    {
    iSTUNRetransmitTimer = aSTUNRetransmitTimer;
    }

// ---------------------------------------------------------------------------
// CNATFWIAPSpecificItem::GetIAPKey
//
// ---------------------------------------------------------------------------
//
TUint32 CNATFWIAPSpecificItem::GetIAPKey() const
    {
    return iIAPKey;
    }

// ---------------------------------------------------------------------------
// CNATFWIAPSpecificItem::GetIAPId
//
// ---------------------------------------------------------------------------
//
TUint32 CNATFWIAPSpecificItem::GetIAPId() const
    {
    return iIAPId;
    }

// ---------------------------------------------------------------------------
// CNATFWIAPSpecificItem::GetIntervalUDP
//
// ---------------------------------------------------------------------------
//
TInt CNATFWIAPSpecificItem::GetIntervalUDP() const
    {
    return iIntervalUDP;
    }

// ---------------------------------------------------------------------------
// CNATFWIAPSpecificItem::GetIntervalTCP
//
// ---------------------------------------------------------------------------
//
TInt CNATFWIAPSpecificItem::GetIntervalTCP() const
    {
    return iIntervalTCP;
    }

// ---------------------------------------------------------------------------
// CNATFWIAPSpecificItem::GetSTUNRetransmitTimer
//
// ---------------------------------------------------------------------------
//
TInt CNATFWIAPSpecificItem::GetSTUNRetransmitTimer() const
    {
    return iSTUNRetransmitTimer;
    }

//  End of File  
