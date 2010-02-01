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



#include "settings.h"

#include <unsafprotocolscrkeys.h>

#include "datadepository.h"


// -----------------------------------------------------------------------------
// CSettings
// -----------------------------------------------------------------------------
// 
CSettings::CSettings()
    {
    }
 
 
// -----------------------------------------------------------------------------
// ~CSettings
// -----------------------------------------------------------------------------
//
CSettings::~CSettings( )
    {
    delete iDepository;
    delete iRepository;
    }

// -----------------------------------------------------------------------------
// CSettings::ConstructBaseL
// -----------------------------------------------------------------------------
//  
void CSettings::ConstructBaseL()
    {
    iRepository = CRepository::NewL( KCRUidUNSAFProtocols );
    iDepository = CDataDepository::NewL( iRepository );
    }


// -----------------------------------------------------------------------------
// CSettings::CreateNewTableKeyL
// -----------------------------------------------------------------------------
//
TUint32 CSettings::CreateNewTableKeyL( TUint aTableMask, TUint32 aFieldMask ) const
    {
    return iDepository->CreateNewTableKeyL( aTableMask, aFieldMask );
    }


// -----------------------------------------------------------------------------
// CSettings::StoreL
// -----------------------------------------------------------------------------
//
void CSettings::StoreL( const TUint32 aKey, const TInt aData )
    {
    iDepository->StoreL( aKey, aData );
    }


// -----------------------------------------------------------------------------
// CSettings::StoreL
// -----------------------------------------------------------------------------
//
void CSettings::StoreL( const TUint32 aKey, const TDesC8& aData )
    {
    iDepository->StoreL( aKey, aData );
    }
    
    
// -----------------------------------------------------------------------------
// CSettings::Read(
// -----------------------------------------------------------------------------
//
TInt CSettings::Read( const TUint32 aKey, TInt& aData )
    {
    return iDepository->Read( aKey, aData );
    }


// -----------------------------------------------------------------------------
// CSettings::ReadL
// -----------------------------------------------------------------------------
//
TInt CSettings::ReadL( const TUint32 aKey, HBufC8** aData )
    {
    return iDepository->ReadL( aKey, aData );
    }
    
// -----------------------------------------------------------------------------
// CSettings::ReadDomainSettingsKeyL
// -----------------------------------------------------------------------------
//
TInt CSettings::ReadDomainSettingsKeyL( const TDesC8& aDomain, 
    TUint32& aDomainKey ) const
    {
    RArray<TUint32> keys;
    CleanupClosePushL( keys );
    
    TInt err = iRepository->FindEqL( KUNSAFProtocolsDomainMask,
        KUNSAFProtocolsFieldTypeMask, aDomain, keys );
    TInt count = keys.Count();
    
    if ( KErrNone == err && 1 == count )
        {
        aDomainKey = KUNSAFProtocolsDomainMask ^ keys[0];
        }
    if ( count > 1 )
        {
        err = KErrCorrupt;
        }
    CleanupStack::PopAndDestroy( &keys );
    return err;
    }

// -----------------------------------------------------------------------------
// CSettings::EraseL
// -----------------------------------------------------------------------------
//
TInt CSettings::EraseL( TUint32 aTableMask, TUint32 aFieldTypeMask )
    {
    return iDepository->EraseL( aTableMask, aFieldTypeMask );
    }




