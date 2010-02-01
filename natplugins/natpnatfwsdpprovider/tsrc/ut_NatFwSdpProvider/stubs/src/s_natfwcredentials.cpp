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

#include "natfwcredentials.h"
#include "nspdefs.h"

// ======== MEMBER FUNCTIONS ========
// ---------------------------------------------------------------------------
// CNATFWCredentials::CNATFWCredentials
// ---------------------------------------------------------------------------
//    
CNATFWCredentials::CNATFWCredentials()
    {
    NSPLOG_STR( "(STUB)CNATFWCredentials::CNATFWCredentials" )
    }


// ---------------------------------------------------------------------------
// CNATFWCredentials::ConstructL
// ---------------------------------------------------------------------------
//        
void CNATFWCredentials::ConstructL( )
    {
    NSPLOG_STR( "(STUB)CNATFWCredentials::ConstructL" )
    
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
    NSPLOG_STR( "(STUB)CNATFWCredentials::ConstructL (copy)" )
    
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
CNATFWCredentials* CNATFWCredentials::NewL()
    {
    NSPLOG_STR( "(STUB)CNATFWCredentials::NewL" )
    
    CNATFWCredentials* self = CNATFWCredentials::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNATFWCredentials::NewL
// ---------------------------------------------------------------------------
//
CNATFWCredentials* CNATFWCredentials::NewL(
        const CNATFWCredentials& aCredentials )
    {
    NSPLOG_STR( "(STUB)CNATFWCredentials::NewL (copy)" )
    
    CNATFWCredentials* self = CNATFWCredentials::NewLC( aCredentials );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CNATFWCredentials::NewLC
// ---------------------------------------------------------------------------
//    
CNATFWCredentials* CNATFWCredentials::NewLC(
        const CNATFWCredentials& aCredentials )
    {
    NSPLOG_STR( "(STUB)CNATFWCredentials::NewLC (copy)" )
    
    CNATFWCredentials* self = new( ELeave ) CNATFWCredentials();
    CleanupStack::PushL( self );
    self->ConstructL( aCredentials );
    return self;
    }


// ---------------------------------------------------------------------------
// CNATFWCredentials::NewLC
// ---------------------------------------------------------------------------
//    
CNATFWCredentials* CNATFWCredentials::NewLC()
    {
    NSPLOG_STR( "(STUB)CNATFWCredentials::NewLC" )
    
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
    NSPLOG_STR( "(STUB)CNATFWCredentials::~CNATFWCredentials" )
    
    iUsername.Close();
    iPassword.Close();
    }
    

// ---------------------------------------------------------------------------
// CNATFWCredentials::SetStreamCollectionId
// ---------------------------------------------------------------------------
//    
void CNATFWCredentials::SetStreamCollectionId( 
        TUint aStreamCollectionId )
    {
    NSPLOG_UINT( "(STUB)CNATFWCredentials::SetStreamCollectionId", aStreamCollectionId )
    
    iStreamCollectionId = aStreamCollectionId;
    }


// ---------------------------------------------------------------------------
// CNATFWCredentials::StreamCollectionId
// ---------------------------------------------------------------------------
//    
TUint CNATFWCredentials::StreamCollectionId() const
    {
    NSPLOG_UINT( "(STUB)CNATFWCredentials::StreamCollectionId", iStreamCollectionId )
    
    return iStreamCollectionId;
    }


// ---------------------------------------------------------------------------
// CNATFWCredentials::SetStreamId
// ---------------------------------------------------------------------------
//
void CNATFWCredentials::SetStreamId( TUint aStreamId )
    {
    NSPLOG_UINT( "(STUB)CNATFWCredentials::SetStreamId", aStreamId )
    
    iStreamId = aStreamId;
    }


// ---------------------------------------------------------------------------
// CNATFWCredentials::StreamId
// ---------------------------------------------------------------------------
//    
TUint CNATFWCredentials::StreamId() const
    {
    NSPLOG_UINT( "(STUB)CNATFWCredentials::StreamId", iStreamId )
    
    return iStreamId;
    }


// ---------------------------------------------------------------------------
// CNATFWCredentials::SetDirection
// ---------------------------------------------------------------------------
//
void CNATFWCredentials::SetDirection( 
        TUtilizationDirection aDirection )
    {
    NSPLOG_UINT( "(STUB)CNATFWCredentials::SetDirection", aDirection )
    
    iDirection = aDirection;
    }


// ---------------------------------------------------------------------------
// CNATFWCredentials::Direction
// ---------------------------------------------------------------------------
//
CNATFWCredentials::TUtilizationDirection 
        CNATFWCredentials::Direction() const
    {
    NSPLOG_UINT( "(STUB)CNATFWCredentials::Direction", iDirection )
    
    return iDirection;
    }


// ---------------------------------------------------------------------------
// CNATFWCredentials::SetUsernameL
// ---------------------------------------------------------------------------
//    
void CNATFWCredentials::SetUsernameL( const TDesC8& aUsername )
    {
    NSPLOG8_STR2( "(STUB)CNATFWCredentials::SetUsernameL", aUsername )
    
    iUsername.Close();
    iUsername.Assign( aUsername.AllocL() );
    }


// ---------------------------------------------------------------------------
// CNATFWCredentials::Username
// ---------------------------------------------------------------------------
//    
const TDesC8& CNATFWCredentials::Username() const
    {
    NSPLOG8_STR2( "(STUB)CNATFWCredentials::Username", iUsername )
    
    return iUsername;
    }


// ---------------------------------------------------------------------------
// CNATFWCredentials::SetPasswordL
// ---------------------------------------------------------------------------
//    
void CNATFWCredentials::SetPasswordL( const TDesC8& aPassword )
    {
    NSPLOG8_STR2( "(STUB)CNATFWCredentials::SetPasswordL", aPassword )
    
    iPassword.Close();
    iPassword.Assign( aPassword.AllocL() );
    }


// ---------------------------------------------------------------------------
// CNATFWCredentials::Password
// ---------------------------------------------------------------------------
//    
const TDesC8& CNATFWCredentials::Password() const
    {
    NSPLOG8_STR2( "(STUB)CNATFWCredentials::Password", iPassword )
    
    return iPassword;
    }


// ---------------------------------------------------------------------------
// CNATFWCredentials::Compare
// Implements TIdentityRelation.
// ---------------------------------------------------------------------------
//
TBool CNATFWCredentials::Compare( 
        const CNATFWCredentials& aCredential1,
        const CNATFWCredentials& aCredential2 )
    {
    return ( aCredential1 == aCredential2 );
    }


// ---------------------------------------------------------------------------
// CNATFWCredentials::CopyL
// ---------------------------------------------------------------------------
//
void CNATFWCredentials::CopyL( 
        const CNATFWCredentials& aCredentials)
    {
    NSPLOG_STR( "(STUB)CNATFWCredentials::CopyL" )
    
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
TBool CNATFWCredentials::operator==( 
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
TBool CNATFWCredentials::operator!=( 
        const CNATFWCredentials& aCredentials ) const
    {
    return !(
        iStreamCollectionId == aCredentials.StreamCollectionId()
        && iStreamId == aCredentials.StreamId()
        && iDirection == aCredentials.Direction()
        && iUsername == aCredentials.Username()
        && iPassword == aCredentials.Password() );
    }
