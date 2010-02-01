/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Device Management NAT-FW Traversal IAP specific settings
*
*/


// INCLUDE FILES
#include    <e32base.h>
#include    "natfwiapentry.h"
#include    "natfwdefaults.h"

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CNATFWIAPEntry::NewL
//
// ---------------------------------------------------------------------------
//
CNATFWIAPEntry* CNATFWIAPEntry::NewL()
    {
    CNATFWIAPEntry* self = NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CNATFWIAPEntry::NewLC
//
// ---------------------------------------------------------------------------
//
CNATFWIAPEntry* CNATFWIAPEntry::NewLC()
    {
    CNATFWIAPEntry* self = new (ELeave) CNATFWIAPEntry();
    CleanupStack::PushL( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CNATFWIAPEntry::CNATFWIAPEntry
//
// ---------------------------------------------------------------------------
//
CNATFWIAPEntry::CNATFWIAPEntry()
    {
    iIntervalUDP = KDefaultUDPRefreshInterval;
    iIntervalTCP = KDefaultTCPRefreshInterval;
    iSTUNRetransmitTimer = KDefaultSTUNRetransmitTimer;
    }

// ---------------------------------------------------------------------------
// CNATFWIAPEntry::~CNATFWIAPEntry
//
// ---------------------------------------------------------------------------
//
CNATFWIAPEntry::~CNATFWIAPEntry()
    {
    }

// ---------------------------------------------------------------------------
// CNATFWIAPEntry::SetIAPKey
//
// ---------------------------------------------------------------------------
//
void CNATFWIAPEntry::SetIAPKey( TUint32 aIAPKey )
    {
    iIAPKey = aIAPKey;
    }
    
// ---------------------------------------------------------------------------
// CNATFWIAPEntry::SetIAPId
//
// ---------------------------------------------------------------------------
//
void CNATFWIAPEntry::SetIAPId( TUint32 aIAPId )
    {
    iIAPId = aIAPId;
    }
    
// ---------------------------------------------------------------------------
// CNATFWIAPEntry::SetIntervalUDP
//
// ---------------------------------------------------------------------------
//
void CNATFWIAPEntry::SetIntervalUDP( TInt aIntervalUDP )
    {
    iIntervalUDP = aIntervalUDP;
    }
    
// ---------------------------------------------------------------------------
// CNATFWIAPEntry::SetIntervalTCP
//
// ---------------------------------------------------------------------------
//
void CNATFWIAPEntry::SetIntervalTCP( TInt aIntervalTCP )
    {
    iIntervalTCP = aIntervalTCP;
    }
    
// ---------------------------------------------------------------------------
// CNATFWIAPEntry::SetSTUNRetransmitTimer
//
// ---------------------------------------------------------------------------
//
void CNATFWIAPEntry::SetSTUNRetransmitTimer( TInt aSTUNRetransmitTimer )
    {
    iSTUNRetransmitTimer = aSTUNRetransmitTimer;
    }

// ---------------------------------------------------------------------------
// CNATFWIAPEntry::GetIAPKey
//
// ---------------------------------------------------------------------------
//
TUint32 CNATFWIAPEntry::GetIAPKey() const
    {
    return iIAPKey;
    }
    
// ---------------------------------------------------------------------------
// CNATFWIAPEntry::GetIAPId
//
// ---------------------------------------------------------------------------
//
TUint32 CNATFWIAPEntry::GetIAPId() const
    {
    return iIAPId;
    }
    
// ---------------------------------------------------------------------------
// CNATFWIAPEntry::GetIntervalUDP
//
// ---------------------------------------------------------------------------
//
TInt CNATFWIAPEntry::GetIntervalUDP() const
    {
    return iIntervalUDP;
    }
    
// ---------------------------------------------------------------------------
// CNATFWIAPEntry::GetIntervalTCP
//
// ---------------------------------------------------------------------------
//
TInt CNATFWIAPEntry::GetIntervalTCP() const
    {
    return iIntervalTCP;
    }
    
// ---------------------------------------------------------------------------
// CNATFWIAPEntry::GetSTUNRetransmitTimer
//
// ---------------------------------------------------------------------------
//
TInt CNATFWIAPEntry::GetSTUNRetransmitTimer() const
    {
    return iSTUNRetransmitTimer;
    }

//  End of File  
