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
* Description:    
*
*/




#include "natfwcredentials.h"
#include "natfwclientlogs.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CNATFWCredentials::CNATFWCredentials
// ---------------------------------------------------------------------------
//    
CNATFWCredentials::CNATFWCredentials()
    {
    __NATFWCLIENT("CNATFWCredentials::CNATFWCredentials" )
    }


// ---------------------------------------------------------------------------
// CNATFWCredentials::ConstructL
// ---------------------------------------------------------------------------
//        
void CNATFWCredentials::ConstructL( )
    {
    __NATFWCLIENT("CNATFWCredentials::ConstructL" )
    
    iUsername.Assign( KNullDesC8().AllocL() );
    iPassword.Assign( KNullDesC8().AllocL() );
    }


// ---------------------------------------------------------------------------
// CNATFWCredentials::ConstructL
// ---------------------------------------------------------------------------
//        
void CNATFWCredentials::ConstructL(
        const CNATFWCredentials& aCredentials)
    {
    __NATFWCLIENT("CNATFWCredentials::ConstructL (copy)" )
    
    iDirection = aCredentials.Direction();
    iStreamCollectionId = aCredentials.StreamCollectionId();
    iStreamId = aCredentials.StreamId();
    
    iUsername.Assign( aCredentials.Username().AllocL() );
    iPassword.Assign( aCredentials.Password().AllocL() );
    }


// ---------------------------------------------------------------------------
// CNATFWCredentials::NewL
// ---------------------------------------------------------------------------
//
EXPORT_C CNATFWCredentials* CNATFWCredentials::NewL()
    {
    __NATFWCLIENT("CNATFWCredentials::NewL")
    
    CNATFWCredentials* self = CNATFWCredentials::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNATFWCredentials::NewL
// ---------------------------------------------------------------------------
//
EXPORT_C CNATFWCredentials* CNATFWCredentials::NewL(
        const CNATFWCredentials& aCredentials )
    {
    __NATFWCLIENT("CNATFWCredentials::NewL (copy)")
    
    CNATFWCredentials* self = CNATFWCredentials::NewLC( aCredentials );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CNATFWCredentials::NewLC
// ---------------------------------------------------------------------------
//    
EXPORT_C CNATFWCredentials* CNATFWCredentials::NewLC(
        const CNATFWCredentials& aCredentials )
    {
    __NATFWCLIENT("CNATFWCredentials::NewLC (copy)")
    
    CNATFWCredentials* self = new( ELeave ) CNATFWCredentials();
    CleanupStack::PushL( self );
    self->ConstructL( aCredentials );
    return self;
    }


// ---------------------------------------------------------------------------
// CNATFWCredentials::NewLC
// ---------------------------------------------------------------------------
//    
EXPORT_C CNATFWCredentials* CNATFWCredentials::NewLC()
    {
    __NATFWCLIENT("CNATFWCredentials::NewLC")
    
    CNATFWCredentials* self = new( ELeave ) CNATFWCredentials();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CNATFWCredentials::~CNATFWCredentials
// ---------------------------------------------------------------------------
//        
CNATFWCredentials::~CNATFWCredentials()
    {
    __NATFWCLIENT("CNATFWCredentials::~CNATFWCredentials")
    
    iUsername.Close();
    iPassword.Close();
    }
    

// ---------------------------------------------------------------------------
// CNATFWCredentials::SetStreamCollectionId
// ---------------------------------------------------------------------------
//    
EXPORT_C void CNATFWCredentials::SetStreamCollectionId( 
        TUint aStreamCollectionId )
    {
    __NATFWCLIENT_INT1(
        "CNATFWCredentials::SetStreamCollectionId", aStreamCollectionId )
    
    iStreamCollectionId = aStreamCollectionId;
    }


// ---------------------------------------------------------------------------
// CNATFWCredentials::StreamCollectionId
// ---------------------------------------------------------------------------
//    
EXPORT_C TUint CNATFWCredentials::StreamCollectionId() const
    {
    __NATFWCLIENT_INT1(
        "CNATFWCredentials::StreamCollectionId", iStreamCollectionId )
    
    return iStreamCollectionId;
    }


// ---------------------------------------------------------------------------
// CNATFWCredentials::SetStreamId
// ---------------------------------------------------------------------------
//
EXPORT_C void CNATFWCredentials::SetStreamId( TUint aStreamId )
    {
    __NATFWCLIENT_INT1( "CNATFWCredentials::SetStreamId", aStreamId )
    
    iStreamId = aStreamId;
    }


// ---------------------------------------------------------------------------
// CNATFWCredentials::StreamId
// ---------------------------------------------------------------------------
//    
EXPORT_C TUint CNATFWCredentials::StreamId() const
    {
    __NATFWCLIENT_INT1( "CNATFWCredentials::StreamId", iStreamId )
    
    return iStreamId;
    }


// ---------------------------------------------------------------------------
// CNATFWCredentials::SetDirection
// ---------------------------------------------------------------------------
//
EXPORT_C void CNATFWCredentials::SetDirection( 
        TUtilizationDirection aDirection )
    {
    __NATFWCLIENT_INT1("CNATFWCredentials::SetDirection", aDirection )
    
    iDirection = aDirection;
    }


// ---------------------------------------------------------------------------
// CNATFWCredentials::Direction
// ---------------------------------------------------------------------------
//
EXPORT_C CNATFWCredentials::TUtilizationDirection 
        CNATFWCredentials::Direction() const
    {
    __NATFWCLIENT_INT1("CNATFWCredentials::Direction", iDirection )
    
    return iDirection;
    }


// ---------------------------------------------------------------------------
// CNATFWCredentials::SetUsernameL
// ---------------------------------------------------------------------------
//    
EXPORT_C void CNATFWCredentials::SetUsernameL( const TDesC8& aUsername )
    {
    __NATFWCLIENT_STR8("CNATFWCredentials::SetUsernameL", aUsername )
    
    iUsername.Close();
    iUsername.Assign( aUsername.AllocL() );
    }


// ---------------------------------------------------------------------------
// CNATFWCredentials::Username
// ---------------------------------------------------------------------------
//    
EXPORT_C const TDesC8& CNATFWCredentials::Username() const
    {
    __NATFWCLIENT_STR8("CNATFWCredentials::Username", iUsername )
    
    return iUsername;
    }


// ---------------------------------------------------------------------------
// CNATFWCredentials::SetPasswordL
// ---------------------------------------------------------------------------
//    
EXPORT_C void CNATFWCredentials::SetPasswordL( const TDesC8& aPassword )
    {
    __NATFWCLIENT_STR8("CNATFWCredentials::SetPasswordL", aPassword )
    
    iPassword.Close();
    iPassword.Assign( aPassword.AllocL() );
    }


// ---------------------------------------------------------------------------
// CNATFWCredentials::Password
// ---------------------------------------------------------------------------
//    
EXPORT_C const TDesC8& CNATFWCredentials::Password() const
    {
    __NATFWCLIENT_STR8("CNATFWCredentials::Password", iPassword )
    
    return iPassword;
    }


// ---------------------------------------------------------------------------
// CNATFWCredentials::Compare
// Implements TIdentityRelation.
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CNATFWCredentials::Compare( 
        const CNATFWCredentials& aCredential1,
        const CNATFWCredentials& aCredential2 )
    {
    return ( aCredential1 == aCredential2 );
    }


// ---------------------------------------------------------------------------
// CNATFWCredentials::CopyL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNATFWCredentials::CopyL( 
        const CNATFWCredentials& aCredentials)
    {
    __NATFWCLIENT("CNATFWCredentials::CopyL" )
    
    iDirection = aCredentials.Direction();
    iStreamCollectionId = aCredentials.StreamCollectionId();
    
    iPassword.Close();
    iPassword.Assign( aCredentials.Password().AllocL() );
    
    iUsername.Close();
    iUsername.Assign( aCredentials.Username().AllocL() );
    }


// ---------------------------------------------------------------------------
// CNATFWCredentials::operator==
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CNATFWCredentials::operator==( 
        const CNATFWCredentials& aCredentials ) const
    {
    return (
        iStreamCollectionId == aCredentials.StreamCollectionId()
        && iStreamId == aCredentials.StreamId()
        && iDirection == aCredentials.Direction()
        && iUsername == aCredentials.Username()
        && iPassword == aCredentials.Password() );
    }
        

// ---------------------------------------------------------------------------
// CNATFWCredentials::operator!=
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CNATFWCredentials::operator!=( 
        const CNATFWCredentials& aCredentials ) const
    {
    return !(
        iStreamCollectionId == aCredentials.StreamCollectionId()
        && iStreamId == aCredentials.StreamId()
        && iDirection == aCredentials.Direction()
        && iUsername == aCredentials.Username()
        && iPassword == aCredentials.Password() );
    }
