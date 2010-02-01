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

#include "datadepository.h"


CDataDepository::CDataDepository( CRepository* aRepository )
    {
    iRepository = aRepository;
    }

CDataDepository::~CDataDepository( )
    {
    }

CDataDepository* CDataDepository::NewL( CRepository* aRepository )
    {
    CDataDepository* self = CDataDepository::NewLC( aRepository );
    CleanupStack::Pop(self);
    return self;
    }

CDataDepository* CDataDepository::NewLC( CRepository* aRepository )
    {
    CDataDepository* self = new( ELeave ) CDataDepository( aRepository );
    CleanupStack::PushL( self );
    //self->ConstructL( );
    return self;
    }


TUint32 CDataDepository::CreateNewTableKeyL( TUint aTableMask, TUint32 aFieldMask ) const
    {
    const TInt KReserved = 1;
    TUint32 newKey = 0;
    RArray< TUint32 > keys;
    CleanupClosePushL( keys );
    TInt err = iRepository->FindL( aTableMask,
                                   aFieldMask,
                                   keys );
    TInt keyCount = keys.Count( );
    
    if ( err == KErrNotFound )
        {
        newKey = aFieldMask + 1;
        }
    else
        {
        User::LeaveIfError( err );
        if ( keyCount == 0 )
            {
            newKey = aFieldMask + 1;
            }
        else
            {
            // Find the biggest key and increment it by one
            keys.SortUnsigned( );
            TUint32 maxKey = aTableMask ^ keys[ keyCount - 1 ];  
            newKey = aFieldMask + 1 + maxKey;
            }
        }
    newKey |= aFieldMask; 
    newKey ^= aFieldMask;
    
    User::LeaveIfError( iRepository->Create( aTableMask | newKey, KReserved ) );
    CleanupStack::PopAndDestroy( &keys );
    return newKey;
    }
    
void CDataDepository::StoreL( const TUint32 aKey, const TInt aData )
    {
    User::LeaveIfError(
        iRepository->StartTransaction(
            CRepository::EConcurrentReadWriteTransaction ) );
    iRepository->CleanupCancelTransactionPushL( );

    TInt tmp = 0;
    if ( iRepository->Get( aKey,tmp ) == KErrNone )
        {
        // Update existing value
        User::LeaveIfError( iRepository->Set( aKey, aData ) );
        }
    else
        {
        // Create new value
        User::LeaveIfError( iRepository->Create( aKey, aData ) );
        } 
    TUint32 dummy;
    User::LeaveIfError( iRepository->CommitTransaction( dummy ) );
    CleanupStack::Pop( ); // transaction
    }   
        
void CDataDepository::StoreL( const TUint32 aKey, const TDesC8& aData )
    {
    User::LeaveIfError(
            iRepository->StartTransaction(
                CRepository::EConcurrentReadWriteTransaction ) );
            
    iRepository->CleanupCancelTransactionPushL( );
    
    TBuf8< 1 > tmp;
    TInt err = iRepository->Get( aKey,tmp );
    if ( err == KErrNone || err == KErrOverflow )
        {
        // Update existing value
        User::LeaveIfError( iRepository->Set( aKey, aData ) );
        }
    else
        {
        // Create new value
        User::LeaveIfError( iRepository->Create( aKey, aData ) );
        }
    TUint32 dummy;
    User::LeaveIfError( iRepository->CommitTransaction( dummy ) );
    CleanupStack::Pop( ); // transaction
    }
            
TInt CDataDepository::Read( const TUint32 aKey, TInt& aData )
    {
    TInt err = KErrNotFound;
    if ( iRepository->Get( aKey, aData ) == KErrNone )
        {
        err = KErrNone;
        }
    return err;
    }
    
TInt CDataDepository::ReadL( const TUint32 aKey, HBufC8** aData )
    {
    TInt err = KErrNotFound;
    TBuf8< 1 > tmp;
    TInt actualLength = 0;
    TInt status = iRepository->Get( aKey,tmp,actualLength );
                
    if ( ( status == KErrNone || status == KErrOverflow) && ( actualLength > 0 ) )
        {
        err = KErrNone;
        HBufC8* buf = HBufC8::NewL( actualLength );
        CleanupStack::PushL( buf );
        TPtr8 ptr( buf->Des( ) );
        User::LeaveIfError( iRepository->Get( aKey,ptr ) );
        *aData = buf; 
        CleanupStack::Pop( buf );
        }
    else
        {
        *aData = KNullDesC8( ).AllocL( );
        }
    return err;    
    }       

TInt CDataDepository::EraseL( TUint32 aTableMask, TUint32 aFieldTypeMask )
    {
    //find KDomain names
    RArray< TUint32 > keys;
    CleanupClosePushL( keys );
    		
    TInt err = iRepository->FindL( aTableMask,
                                   aFieldTypeMask,
                                   keys );
    if( err == KErrNone ) 
        {
        TInt count = keys.Count( );
        for ( TInt i = 0; i < count; i++ )
            {
            //delete setting
            iRepository->Delete( keys[ i ] );	
            }
        }
    CleanupStack::PopAndDestroy( &keys );
    return err;
    }
