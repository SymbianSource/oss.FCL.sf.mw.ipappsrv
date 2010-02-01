/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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




#include "TNATServerSettings.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// TNATServerSettings::TNATServerSettings
// ---------------------------------------------------------------------------
//
EXPORT_C TNATServerSettings::TNATServerSettings( 
    TUint aPort,
    TUint aTCPTimeout,
    TUint aUDPTimeout,
    TUint aConnectivityCheckTimeout,
    TBool aCRLFEnabled ) :
    iAddress( 0 ),
    iUsername( 0 ),
    iPassword( 0 ),
    iPort( aPort),
    iTCPTimeout( aTCPTimeout ),
    iUDPTimeout( aUDPTimeout ),
    iConnectivityCheckTimeout( aConnectivityCheckTimeout),
    iCRLFRefreshEnabled( aCRLFEnabled )
    {
    }


// ---------------------------------------------------------------------------
// TNATServerSettings::SetAddressL
// ---------------------------------------------------------------------------
//
EXPORT_C void TNATServerSettings::SetAddressL( const TDesC8& aAddress )
    {
    __ASSERT_ALWAYS( aAddress.Length() < KNATFWMaxAddressLength,
        User::Leave( KErrOverflow ) );
    iAddress.Copy( aAddress );
    }


// ---------------------------------------------------------------------------
// TNATServerSettings::SetAddressL
// ---------------------------------------------------------------------------
//
EXPORT_C void TNATServerSettings::SetUsernameL( const TDesC8& aUsername )
    {
    __ASSERT_ALWAYS( aUsername.Length() < KNATFWMaxUsernameLength,
        User::Leave( KErrOverflow ) );
    iUsername.Copy( aUsername );
    }
    
    
// ---------------------------------------------------------------------------
// TNATServerSettings::SetAddressL
// ---------------------------------------------------------------------------
//
EXPORT_C void TNATServerSettings::SetPasswordL( const TDesC8& aPassword )
    {
    __ASSERT_ALWAYS( aPassword.Length() < KNATFWMaxPasswordLength,
        User::Leave( KErrOverflow ) );
    iPassword.Copy( aPassword );
    }
    
    
// ---------------------------------------------------------------------------
// TNATServerSettings::Address
// ---------------------------------------------------------------------------
//
EXPORT_C const TDesC8& TNATServerSettings::Address() const
    {
    return iAddress;
    }


// ---------------------------------------------------------------------------
// TNATServerSettings::Port
// ---------------------------------------------------------------------------
//  
EXPORT_C TUint TNATServerSettings::Port() const
    {
    return iPort;
    }


// ---------------------------------------------------------------------------
// TNATServerSettings::TCPTimeout
// ---------------------------------------------------------------------------
//
EXPORT_C TUint TNATServerSettings::TCPTimeout() const
    {
    return iTCPTimeout;
    }
     
            
// ---------------------------------------------------------------------------
// TNATServerSettings::UDPTimeout
// ---------------------------------------------------------------------------
//
EXPORT_C TUint TNATServerSettings::UDPTimeout() const
    {
    return iUDPTimeout;
    }


// ---------------------------------------------------------------------------
// TNATServerSettings::ConnectivityCheckTimeout
// ---------------------------------------------------------------------------
//
EXPORT_C TUint TNATServerSettings::ConnectivityCheckTimeout() const
    {
    return iConnectivityCheckTimeout;
    }
      
               
// ---------------------------------------------------------------------------
// TNATServerSettings::CRLFRefreshEnabled
// ---------------------------------------------------------------------------
//
EXPORT_C TBool TNATServerSettings::CRLFRefreshEnabled() const
    {
    return iCRLFRefreshEnabled;
    }

    
// ---------------------------------------------------------------------------
// TNATServerSettings::Username
// ---------------------------------------------------------------------------
//  
EXPORT_C const TDesC8& TNATServerSettings::Username() const
    {
    return iUsername;
    }


// ---------------------------------------------------------------------------
// TNATServerSettings::Password
// ---------------------------------------------------------------------------
//  
EXPORT_C const TDesC8& TNATServerSettings::Password() const
    {
    return iPassword;
    }
    
//End of file

