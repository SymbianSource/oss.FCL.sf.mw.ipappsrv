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




#include <centralrepository.h>
#include <unsafprotocolscrkeys.h>
#include "cnatfwcenrephandler.h"
#include "natsettingslogs.h"

const TInt KAllowedDomainAndIapKeyCount = 1; 


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CNATFWCenRepHandler::CNATFWCenRepHandler
// ---------------------------------------------------------------------------
//
CNATFWCenRepHandler::CNATFWCenRepHandler()
    {
    }


// ---------------------------------------------------------------------------
// CNATFWCenRepHandler::ConstructL
// ---------------------------------------------------------------------------
//    
void CNATFWCenRepHandler::ConstructL( const TUid& aRepositoryUid )
    {
    iCenRep = CRepository::NewL( aRepositoryUid );
    }


// ---------------------------------------------------------------------------
// CNATFWCenRepHandler::NewL
// ---------------------------------------------------------------------------
//
CNATFWCenRepHandler* CNATFWCenRepHandler::NewL( const TUid& aRepositoryUid )
    {
    __NATSETTINGS( "CNATFWCenRepHandler::NewL" )
    
    CNATFWCenRepHandler* self =
        new( ELeave ) CNATFWCenRepHandler();
    CleanupStack::PushL( self );
    self->ConstructL( aRepositoryUid );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNATFWCenRepHandler::~CNATFWCenRepHandler
//----------------------------------------------------------------------------
//
CNATFWCenRepHandler::~CNATFWCenRepHandler()
    {
    __NATSETTINGS( "CNATFWCenRepHandler::~CNATFWNatSettingsImpl" )
    
    delete iCenRep;
    }


// ---------------------------------------------------------------------------
// CNATFWCenRepHandler::ReadDomainSettingsKey
// ---------------------------------------------------------------------------
//  
TInt CNATFWCenRepHandler::ReadDomainSettingsKey(
    const TDesC8& aDomain, TUint32& aDomainKey ) const
    {
    __NATSETTINGS( "CNATFWCenRepHandler::ReadDomainSettingsKeyL" )
    
    TInt err( 0 );
    RArray<TUint32> keys;
    
    TRAPD( systemErr, err = iCenRep->FindEqL( KUNSAFProtocolsDomainMask,
        KUNSAFProtocolsFieldTypeMask, aDomain, keys ) );
    
    if ( KErrNone != systemErr )
        {
        keys.Close();
        return systemErr;
        }
    
    TInt count = keys.Count();
    
    if ( KErrNone == err && KAllowedDomainAndIapKeyCount == count )
        {
        aDomainKey = KUNSAFProtocolsDomainMask ^ keys[0];
        }
    if ( count > KAllowedDomainAndIapKeyCount )
        {
        err = KErrCorrupt;
        }
    keys.Close();
    return err;
    }


// ---------------------------------------------------------------------------
// CNATFWCenRepHandler::ReadIapSettingsKey
// ---------------------------------------------------------------------------
//  
TInt CNATFWCenRepHandler::ReadIapSettingsKey(
    TInt aIapId, TUint32& aIapKey ) const
    {
    __NATSETTINGS( "CNATFWCenRepHandler::ReadIapSettingsKeyL" )
    
    TInt err( 0 );
    RArray<TUint32> keys;
    
    TRAPD( systemErr, err = iCenRep->FindEqL( KUNSAFProtocolsIAPIdMask,
        KUNSAFProtocolsFieldTypeMask, aIapId, keys ) );
    
    if ( KErrNone != systemErr )
        {
        keys.Close();
        return systemErr;
        }
    
    TInt count = keys.Count();
    
    if ( KErrNone == err && KAllowedDomainAndIapKeyCount == count )
        {
        aIapKey = KUNSAFProtocolsIAPIdMask ^ keys[0];
        }
    if ( count > KAllowedDomainAndIapKeyCount )
        {
        err = KErrCorrupt;
        }
    keys.Close();
    return err;
    }


// ---------------------------------------------------------------------------
// CNATFWCenRepHandler::FindServerKeys
// ---------------------------------------------------------------------------
//
TInt CNATFWCenRepHandler::FindServerKeys( TUint32 aPartialId,
    RArray<TUint32>& aFoundKeys ) const
    {
    const TUint32 KMaskForAllServerSettingsIds = 0xff000fff;
    
    TInt error( 0 );
    RArray<TUint32> serverKeys;
    
    TRAPD( systemErr, error = iCenRep->FindL( aPartialId,
        KMaskForAllServerSettingsIds, serverKeys ) );
    
    if ( KErrNone != systemErr )    
        {
        serverKeys.Close();
        return systemErr;
        }
    
    if ( KErrNone == error )
        {
        TInt count = serverKeys.Count();

        for ( TInt i( 0 ); i < count; i++ )
            {
            error = aFoundKeys.Append( serverKeys[i] & KUNSAFProtocolsSubKeyMask );
            }
        
        for ( TInt j( 0 ); j < aFoundKeys.Count(); j++ )
            {
            for ( TInt i( j + 1 ); i < aFoundKeys.Count(); )
                {
                if ( aFoundKeys[ i ] == aFoundKeys[ j ] )
                    {
                    aFoundKeys.Remove( i );
                    }
                else
                    {
                    i++;
                    }
                }
            }
        }
    serverKeys.Close();
    return error;
    }


// ---------------------------------------------------------------------------
// CNATFWCenRepHandler::CreateNewKeyL
// ---------------------------------------------------------------------------
//  
TUint32 CNATFWCenRepHandler::CreateNewKeyL(
    const TUint32 aSettingKeyMask, const TUint32 aField ) const
    {
    TUint32 newKey( 0 );
    RArray<TUint32> keys;
    CleanupClosePushL( keys );
    TInt err( iCenRep->FindL( aSettingKeyMask, aField, keys ) );
    TInt keyCount( keys.Count() );
    
    if ( KErrNotFound == err )
        {
        newKey = aField + 1;
        }
    else
        {
        User::LeaveIfError( err );
        if ( !keyCount )
            {
            newKey = aField + 1;
            }
        else
            {
            // Find the biggest key and increment it by one
            keys.SortUnsigned();
            TUint32 maxKey = aSettingKeyMask ^ keys[keyCount - 1];  
            newKey = aField + 1 + maxKey;
            }
        }
    newKey |= aField; 
    newKey ^= aField;
    CleanupStack::PopAndDestroy( &keys );
    return newKey;
    }


// ---------------------------------------------------------------------------
// CNATFWCenRepHandler::ReadL
// ---------------------------------------------------------------------------
//
HBufC8* CNATFWCenRepHandler::ReadL( const TUint32& aKey ) const
    {
    HBufC8* buffer = NULL;
    TBuf8<1> tmp;
    TInt actualLength( 0 );
    TInt status = iCenRep->Get( aKey, tmp, actualLength );
    
    if ( ( KErrNone == status || KErrOverflow == status ) )
        {
        if( actualLength )
            {
            buffer = HBufC8::NewLC( actualLength );
            TPtr8 ptr( buffer->Des() );
            User::LeaveIfError( iCenRep->Get( aKey, ptr ) );
            CleanupStack::Pop( buffer );
            }
        else
            {
            buffer = KNullDesC8().AllocL();
            }
        }
    return buffer;
    }


// ---------------------------------------------------------------------------
// CNATFWCenRepHandler::ReadBoolValue
// ---------------------------------------------------------------------------
//
TInt CNATFWCenRepHandler::ReadBoolValue(
    const TUint32& aKey, TBool& aKeyValue ) const
    {
    TInt err = KErrNotFound;
    TInt tmp( 0 );
    if ( KErrNone == iCenRep->Get( aKey, tmp ) )
        {
        aKeyValue = ( tmp != 0 );
        err = KErrNone;
        }
    return err;
    }


// ---------------------------------------------------------------------------
// CNATFWCenRepHandler::Read
// ---------------------------------------------------------------------------
//
TInt CNATFWCenRepHandler::Read(
    const TUint32& aKey, TUint& aKeyValue ) const
    {
    TInt err = KErrNotFound;
    TInt tmp;
    if ( KErrNone == iCenRep->Get( aKey, tmp ) )
        {
        aKeyValue = static_cast<TUint>( tmp );
        err = KErrNone;
        }
    return err;
    }


// ---------------------------------------------------------------------------
// CNATFWCenRepHandler::Read
// ---------------------------------------------------------------------------
//
TInt CNATFWCenRepHandler::Read(
    const TUint32& aKey, TInt& aKeyValue ) const
    {
    TInt readKeyValue( 0 );
    if ( KErrNone == iCenRep->Get( aKey, readKeyValue ) )
        {
        aKeyValue = readKeyValue;
        }
    else
        {
        return KErrNotFound;
        }
    return KErrNone;
    }
