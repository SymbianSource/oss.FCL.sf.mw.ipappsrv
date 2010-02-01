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
* Description:    Stores server related data for plug-in's internal use
*
*/




#include "cnatfwturnserversettings.h"


// ---------------------------------------------------------------------------
// CTurnServerSettings::CTurnServerSettings
// ---------------------------------------------------------------------------
//
CTurnServerSettings::CTurnServerSettings()
    {
    }

// ---------------------------------------------------------------------------
// CTurnServerSettings::NewL
// ---------------------------------------------------------------------------
//
CTurnServerSettings* CTurnServerSettings::NewL()
    {
    CTurnServerSettings* self = CTurnServerSettings::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CTurnServerSettings::NewLC
// ---------------------------------------------------------------------------
//
CTurnServerSettings* CTurnServerSettings::NewLC()
    {
    CTurnServerSettings* self=new ( ELeave ) CTurnServerSettings();
    CleanupStack::PushL( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CTurnServerSettings::CTurnServerSettings
// ---------------------------------------------------------------------------
//
CTurnServerSettings::~CTurnServerSettings()
    {
    delete iAddress;
    delete iUsername;
    delete iPassword;
    }

// ---------------------------------------------------------------------------
// CTurnServerSettings::SetUsernameL
// ---------------------------------------------------------------------------
//
void CTurnServerSettings::SetAddressL( const TDesC8& aAddress )
    {
    delete iAddress;
    iAddress = NULL;
    iAddress = aAddress.AllocL();
    }
 
// ---------------------------------------------------------------------------
// CTurnServerSettings::Address
// ---------------------------------------------------------------------------
//
const TDesC8& CTurnServerSettings::Address() const
    {
    return *iAddress;
    }   

 // ---------------------------------------------------------------------------
// CTurnServerSettings::SetPort
// ---------------------------------------------------------------------------
//
void CTurnServerSettings::SetPort( TUint aPort )
    {
    iPort = aPort;
    }

 // ---------------------------------------------------------------------------
// CTurnServerSettings::Port
// ---------------------------------------------------------------------------
//
TUint CTurnServerSettings::Port() const
    {
    return iPort;
    }
 
// ---------------------------------------------------------------------------
// CTurnServerSettings::SetUsernameL
// ---------------------------------------------------------------------------
//
void CTurnServerSettings::SetUsernameL( const TDesC8& aUsername )
    {
    delete iUsername;
    iUsername = NULL;
    iUsername = aUsername.AllocL();
    }
 
 // ---------------------------------------------------------------------------
// CTurnServerSettings::Username
// ---------------------------------------------------------------------------
//
const HBufC8* CTurnServerSettings::Username() const
    {
    return iUsername;
    }   

// ---------------------------------------------------------------------------
// CTurnServerSettings::SetPasswordL
// ---------------------------------------------------------------------------
//
void CTurnServerSettings::SetPasswordL( const TDesC8& aPassword )
    {
    delete iPassword;
    iPassword = NULL;
    iPassword = aPassword.AllocL();
    }
 
 // ---------------------------------------------------------------------------
// CTurnServerSettings::Password
// ---------------------------------------------------------------------------
//
const HBufC8* CTurnServerSettings::Password() const
    {
    return iPassword;
    }   
