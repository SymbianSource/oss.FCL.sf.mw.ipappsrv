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
* Description:  Holds transiently one TURN server item.
*
*/


#include <e32base.h>

#include "turnserveritem.h"
#include "natfwdefaults.h"
#include "wpnatfwdebug.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CWPTurnServerItem::CWPTurnServerItem
// ---------------------------------------------------------------------------
//
CWPTurnServerItem::CWPTurnServerItem()
    {
    }

// ---------------------------------------------------------------------------
// CWPTurnServerItem::ConstructL
// ---------------------------------------------------------------------------
//
void CWPTurnServerItem::ConstructL()
    {
    DBG_PRINT( "CWPTurnServerItem::ConstructL - begin" );
    // Set default values.
    iTurnSrvAddr  = HBufC8::NewL( 0 );
    iTurnSrvPort  = KDefaultSTUNPort;
    iTurnUsername = HBufC8::NewL( 0 );
    iTurnPassword = HBufC8::NewL( 0 );
    DBG_PRINT( "CWPTurnServerItem::ConstructL - end" );
    }

// ---------------------------------------------------------------------------
// CWPTurnServerItem::NewL
// ---------------------------------------------------------------------------
//
CWPTurnServerItem* CWPTurnServerItem::NewL()
    {
    CWPTurnServerItem* self = CWPTurnServerItem::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CWPTurnServerItem::NewLC
// ---------------------------------------------------------------------------
//
CWPTurnServerItem* CWPTurnServerItem::NewLC()
    {
    CWPTurnServerItem* self = new( ELeave ) CWPTurnServerItem;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CWPTurnServerItem::~CWPTurnServerItem
// ---------------------------------------------------------------------------
//
CWPTurnServerItem::~CWPTurnServerItem()
    {
    DBG_PRINT( "CWPTurnServerItem::~CWPTurnServerItem - begin" );
    delete iTurnSrvAddr;
    delete iTurnUsername;
    delete iTurnPassword;
    DBG_PRINT( "CWPTurnServerItem::~CWPTurnServerItem - end" );
    }

// ---------------------------------------------------------------------------
// CWPTurnServerItem::SetTurnSrvAddrL
// ---------------------------------------------------------------------------
//
void CWPTurnServerItem::SetTurnSrvAddrL( const TDesC8& aTurnSrvAddr )
    {
    delete iTurnSrvAddr;
    iTurnSrvAddr = NULL;
    iTurnSrvAddr = aTurnSrvAddr.AllocL();
    }

// ---------------------------------------------------------------------------
// CWPTurnServerItem::SetTurnSrvPort
// ---------------------------------------------------------------------------
//
void CWPTurnServerItem::SetTurnSrvPort( TInt aTurnSrvPort )
    {
    iTurnSrvPort = aTurnSrvPort;
    }

// ---------------------------------------------------------------------------
// CWPTurnServerItem::SetTurnUsernameL
// ---------------------------------------------------------------------------
//
void CWPTurnServerItem::SetTurnUsernameL( const TDesC8& aUsername )
    {
    delete iTurnUsername;
    iTurnUsername = NULL;
    iTurnUsername = aUsername.AllocL();
    }

// ---------------------------------------------------------------------------
// CWPTurnServerItem::SetTurnPasswordL
// ---------------------------------------------------------------------------
//
void CWPTurnServerItem::SetTurnPasswordL( const TDesC8& aPassword )
    {
    delete iTurnPassword;
    iTurnPassword = NULL;
    iTurnPassword = aPassword.AllocL();
    }

// ---------------------------------------------------------------------------
// CWPTurnServerItem::TurnSrvAddr
// ---------------------------------------------------------------------------
//
const TDesC8& CWPTurnServerItem::TurnSrvAddr() const
    {
    return *iTurnSrvAddr;
    }

// ---------------------------------------------------------------------------
// CWPTurnServerItem::TurnSrvPort
// ---------------------------------------------------------------------------
//
TInt CWPTurnServerItem::TurnSrvPort() const
    {
    return iTurnSrvPort;
    }

// ---------------------------------------------------------------------------
// CWPTurnServerItem::TurnUsername
// ---------------------------------------------------------------------------
//
const TDesC8& CWPTurnServerItem::TurnUsername() const
    {
    return *iTurnUsername;
    }

// ---------------------------------------------------------------------------
// CWPTurnServerItem::TurnPassword
// ---------------------------------------------------------------------------
//
const TDesC8& CWPTurnServerItem::TurnPassword() const
    {
    return *iTurnPassword;
    }

