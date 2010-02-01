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
* Description:  Holds transiently one STUN server item.
*
*/


#include <e32base.h>

#include "stunserveritem.h"
#include "natfwdefaults.h"
#include "wpnatfwdebug.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CWPStunServerItem::CWPStunServerItem
// ---------------------------------------------------------------------------
//
CWPStunServerItem::CWPStunServerItem()
    {
    }

// ---------------------------------------------------------------------------
// CWPStunServerItem::ConstructL
// ---------------------------------------------------------------------------
//
void CWPStunServerItem::ConstructL()
    {
    DBG_PRINT( "CWPStunServerItem::ConstructL - begin" );
    // Set default values.
    iStunSrvAddr   = HBufC8::NewL( 0 );
    iStunSrvPort   = KDefaultSTUNPort;
    iStunUsername  = HBufC8::NewL( 0 );
    iStunPassword  = HBufC8::NewL( 0 );
    DBG_PRINT( "CWPStunServerItem::ConstructL - end" );
    }

// ---------------------------------------------------------------------------
// CWPStunServerItem::NewL
// ---------------------------------------------------------------------------
//
CWPStunServerItem* CWPStunServerItem::NewL()
    {
    CWPStunServerItem* self = CWPStunServerItem::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CWPStunServerItem::NewLC
// ---------------------------------------------------------------------------
//
CWPStunServerItem* CWPStunServerItem::NewLC()
    {
    CWPStunServerItem* self = new( ELeave ) CWPStunServerItem;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CWPStunServerItem::~CWPStunServerItem
// ---------------------------------------------------------------------------
//
CWPStunServerItem::~CWPStunServerItem()
    {
    DBG_PRINT( "CWPStunServerItem::~CWPStunServerItem - begin" );
    delete iStunSrvAddr;
    delete iStunUsername;
    delete iStunPassword;
    DBG_PRINT( "CWPStunServerItem::~CWPStunServerItem - end" );
    }

// ---------------------------------------------------------------------------
// CWPStunServerItem::SetStunSrvAddrL
// ---------------------------------------------------------------------------
//
void CWPStunServerItem::SetStunSrvAddrL( const TDesC8& aStunSrvAddr )
    {
    delete iStunSrvAddr;
    iStunSrvAddr = NULL;
    iStunSrvAddr = aStunSrvAddr.AllocL();
    }

// ---------------------------------------------------------------------------
// CWPStunServerItem::SetStunSrvPort
// ---------------------------------------------------------------------------
//
void CWPStunServerItem::SetStunSrvPort( TInt aStunSrvPort )
    {
    iStunSrvPort = aStunSrvPort;
    }

// ---------------------------------------------------------------------------
// CWPStunServerItem::SetStunUsernameL
// ---------------------------------------------------------------------------
//
void CWPStunServerItem::SetStunUsernameL( const TDesC8& aStunUsername )
    {
    delete iStunUsername;
    iStunUsername = NULL;
    iStunUsername = aStunUsername.AllocL();
    }

// ---------------------------------------------------------------------------
// CWPStunServerItem::SetStunPasswordL
// ---------------------------------------------------------------------------
//
void CWPStunServerItem::SetStunPasswordL( const TDesC8& aStunPassword )
    {
    delete iStunPassword;
    iStunPassword = NULL;
    iStunPassword = aStunPassword.AllocL();
    }

// ---------------------------------------------------------------------------
// CWPStunServerItem::StunSrvAddr
// ---------------------------------------------------------------------------
//
const TDesC8& CWPStunServerItem::StunSrvAddr() const
    {
    return *iStunSrvAddr;
    }

// ---------------------------------------------------------------------------
// CWPStunServerItem::StunSrvPort
// ---------------------------------------------------------------------------
//
TInt CWPStunServerItem::StunSrvPort() const
    {
    return iStunSrvPort;
    }

// ---------------------------------------------------------------------------
// CWPStunServerItem::StunUsername
// ---------------------------------------------------------------------------
//
const TDesC8& CWPStunServerItem::StunUsername() const
    {
    return *iStunUsername;
    }

// ---------------------------------------------------------------------------
// CWPStunServerItem::StunPassword
// ---------------------------------------------------------------------------
//
const TDesC8& CWPStunServerItem::StunPassword() const
    {
    return *iStunPassword;
    }

