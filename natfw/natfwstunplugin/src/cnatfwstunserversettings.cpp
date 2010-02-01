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
* Description:    Stores STUN server related data for plug-in's internal use
*
*/




#include "cnatfwstunserversettings.h"


// ---------------------------------------------------------------------------
// CStunServerSettings::CStunServerSettings
// ---------------------------------------------------------------------------
//
CStunServerSettings::CStunServerSettings()
    {
    }


// ---------------------------------------------------------------------------
// CStunServerSettings::NewL
// ---------------------------------------------------------------------------
//
CStunServerSettings* CStunServerSettings::NewL()
    {
    CStunServerSettings* self = CStunServerSettings::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CStunServerSettings::NewLC
// ---------------------------------------------------------------------------
//
CStunServerSettings* CStunServerSettings::NewLC()
    {
    CStunServerSettings* self=new ( ELeave ) CStunServerSettings();
    CleanupStack::PushL( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CStunServerSettings::CStunServerSettings
// ---------------------------------------------------------------------------
//
CStunServerSettings::~CStunServerSettings()
    {
    delete iAddress;
    delete iUsername;
    delete iPassword;
    }


// ---------------------------------------------------------------------------
// CStunServerSettings::SetUsernameL
// ---------------------------------------------------------------------------
//
void CStunServerSettings::SetAddressL( const TDesC8& aAddress )
    {
    delete iAddress;
    iAddress = NULL;
    iAddress = aAddress.AllocL();
    }

 
// ---------------------------------------------------------------------------
// CStunServerSettings::Address
// ---------------------------------------------------------------------------
//
const TDesC8& CStunServerSettings::Address() const
    {
    return *iAddress;
    }   


 // ---------------------------------------------------------------------------
// CStunServerSettings::SetPort
// ---------------------------------------------------------------------------
//
void CStunServerSettings::SetPort( TUint aPort )
    {
    iPort = aPort;
    }


 // ---------------------------------------------------------------------------
// CStunServerSettings::Port
// ---------------------------------------------------------------------------
//
TUint CStunServerSettings::Port() const
    {
    return iPort;
    }

 
// ---------------------------------------------------------------------------
// CStunServerSettings::SetUsernameL
// ---------------------------------------------------------------------------
//
void CStunServerSettings::SetUsernameL( const TDesC8& aUsername )
    {
    delete iUsername;
    iUsername = NULL;
    iUsername = aUsername.AllocL();
    }

 
 // ---------------------------------------------------------------------------
// CStunServerSettings::Username
// ---------------------------------------------------------------------------
//
const HBufC8* CStunServerSettings::Username() const
    {
    return iUsername;
    }   


// ---------------------------------------------------------------------------
// CStunServerSettings::SetPasswordL
// ---------------------------------------------------------------------------
//
void CStunServerSettings::SetPasswordL( const TDesC8& aPassword )
    {
    delete iPassword;
    iPassword = NULL;
    iPassword = aPassword.AllocL();
    }

 
// ---------------------------------------------------------------------------
// CStunServerSettings::Password
// ---------------------------------------------------------------------------
//
const HBufC8* CStunServerSettings::Password() const
    {
    return iPassword;
    }   
