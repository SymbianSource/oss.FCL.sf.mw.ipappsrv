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
* Description:    
*
*/




#include "cstuncredentials.h"
#include "stunturnclientlogs.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSTUNCredentials::NewL
// ---------------------------------------------------------------------------
//
CSTUNCredentials* CSTUNCredentials::NewL( const TDesC8& aUsername,
                                          const TDesC8& aPassword )
    {
    CSTUNCredentials* self = new ( ELeave ) CSTUNCredentials();
    CleanupStack::PushL( self );
    self->ConstructL( aUsername, aPassword );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CSTUNCredentials::CSTUNCredentials
// ---------------------------------------------------------------------------
//
CSTUNCredentials::CSTUNCredentials()
    {
    }

// ---------------------------------------------------------------------------
// CSTUNCredentials::CSTUNCredentials
// ---------------------------------------------------------------------------
//
CSTUNCredentials::CSTUNCredentials( const CSTUNCredentials& /*aCredentials*/ ) :
    CBase()
    {
    }

// ---------------------------------------------------------------------------
// CSTUNCredentials::ConstructL
// ---------------------------------------------------------------------------
//
void CSTUNCredentials::ConstructL( const TDesC8& aUsername,
                                   const TDesC8& aPassword )
    {
    __STUNTURNCLIENT( "CSTUNCredentials::ConstructL" )
    
    if ( aUsername.Length() > 0 && aPassword.Length() > 0 )
        {        
        iUsername = aUsername.AllocL();
        iPassword = aPassword.AllocL();
            
        __STUNTURNCLIENT_STR8( "username: ", *iUsername )
        __STUNTURNCLIENT_STR8( "password: ", *iPassword )
        }
    
    __STUNTURNCLIENT( "CSTUNCredentials::ConstructL End" )
    }


// ---------------------------------------------------------------------------
// CSTUNCredentials::~CSTUNCredentials
// ---------------------------------------------------------------------------
//
CSTUNCredentials::~CSTUNCredentials()
    {
    delete iUsername;
    delete iPassword;
    }

// -----------------------------------------------------------------------------
// CSTUNCredentials::Username
// -----------------------------------------------------------------------------
//
const TDesC8& CSTUNCredentials::Username() const
    {    
    return *iUsername;    
    }

// -----------------------------------------------------------------------------
// CSTUNCredentials::Password
// -----------------------------------------------------------------------------
//
const TDesC8& CSTUNCredentials::Password() const
    {
    return *iPassword;    
    }

// -----------------------------------------------------------------------------
// CSTUNCredentials::Compare
// -----------------------------------------------------------------------------
//
TBool CSTUNCredentials::Compare( const TDesC8& aUsername,
                                 const TDesC8& aPassword ) const
    {
    return aUsername.Compare( Username() ) == 0 &&
           aPassword.Compare( Password() ) == 0;
    }
