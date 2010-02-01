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

#include <centralrepository.h>

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
void CSettings::ConstructBaseL( TUid aCRUid )
    {
    iRepository = CRepository::NewL( aCRUid );
    iDepository = CDataDepository::NewL( iRepository );
    }


// -----------------------------------------------------------------------------
// CSettings::CreateNewTableKeyL
// -----------------------------------------------------------------------------
//
EXPORT_C TUint32 CSettings::CreateNewTableKeyL( TUint aTableMask, TUint32 aFieldMask ) const
    {
    return iDepository->CreateNewTableKeyL( aTableMask, aFieldMask );
    }


// -----------------------------------------------------------------------------
// CSettings::StoreL
// -----------------------------------------------------------------------------
//
EXPORT_C void CSettings::StoreL( const TUint32 aKey, const TInt aData )
    {
    iDepository->StoreL( aKey, aData );
    }


// -----------------------------------------------------------------------------
// CSettings::StoreL
// -----------------------------------------------------------------------------
//
EXPORT_C void CSettings::StoreL( const TUint32 aKey, const TDesC8& aData )
    {
    iDepository->StoreL( aKey, aData );
    }
    
    
// -----------------------------------------------------------------------------
// CSettings::Read(
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CSettings::Read( const TUint32 aKey, TInt& aData )
    {
    return iDepository->Read( aKey, aData );
    }


// -----------------------------------------------------------------------------
// CSettings::ReadL
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CSettings::ReadL( const TUint32 aKey, HBufC8** aData )
    {
    return iDepository->ReadL( aKey, aData );
    }
    
    
// -----------------------------------------------------------------------------
// CSettings::EraseL
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CSettings::EraseL( TUint32 aTableMask, TUint32 aFieldTypeMask )
    {
    return iDepository->EraseL( aTableMask, aFieldTypeMask );
    }

