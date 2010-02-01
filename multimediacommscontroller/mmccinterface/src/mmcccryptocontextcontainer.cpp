/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Class is used for configuring secure RTP.
*
*/




#include <mmf/common/mmfcontrollerframework.h>
#include <e32math.h>

#include "mmccinterfacelogs.h"
#include "mmcccryptocontextcontainer.h"
#include "mmcccryptocontext.h"

// ======== MEMBER FUNCTIONS ========

    
// ---------------------------------------------------------------------------
// CMccCryptoContextContainer::ConstructL
// ---------------------------------------------------------------------------
//
void CMccCryptoContextContainer::ConstructL( )
    {
    __INTERFACE( "CMccCryptoContextContainer::ConstructL" )
    }
    
// ---------------------------------------------------------------------------
// CMccCryptoContextContainer::NewL()
// ---------------------------------------------------------------------------
//
CMccCryptoContextContainer* CMccCryptoContextContainer::NewL( )
    {
    CMccCryptoContextContainer* self = new(ELeave) CMccCryptoContextContainer();
    CleanupStack::PushL( self );
    self->ConstructL( );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CMccCryptoContextContainer::~CMccCryptoContextContainer
// ---------------------------------------------------------------------------
//
CMccCryptoContextContainer::~CMccCryptoContextContainer()
    {
    iContextArray.Close();
    }

// ---------------------------------------------------------------------------
// CMccCryptoContextContainer::CreateContext
// ---------------------------------------------------------------------------
//
TInt CMccCryptoContextContainer::CreateContext( const TMccSrtpMasterKey& aMasterKey,
                                                const TMccSrtpMasterSalt& aSaltKey,
    					                        TUint32& aContextId,
    				                            const TMccSrtpCryptoParams& aCryptoParams )
    {
    __INTERFACE( "CMccCryptoContextContainer::CreateContext" )

    TUint32 contextId( GenerateContextId() );
    
    TMccCryptoContext cryptoContext( contextId,
                                     aMasterKey,
                                     aSaltKey,
                                     aCryptoParams );   
                                                    
                                                                  
    TInt error( cryptoContext.ValidateContext() );
    
    if ( KErrNone == error )
        {
        error = iContextArray.Append( cryptoContext );
        aContextId = contextId;
        }

    return error;
    }

// ---------------------------------------------------------------------------
// CMccCryptoContextContainer::RemoveContext
// ---------------------------------------------------------------------------
//
TInt CMccCryptoContextContainer::RemoveContext( TUint32 aContextId )
    {
    __INTERFACE( "CMccContextContainer::RemoveContext" )
    
    TInt contextId = FindContext( aContextId );
    
    if ( KErrNotFound != contextId )
        {
        iContextArray.Remove( contextId );
        return KErrNone;
        }
                       
    return KErrNotFound;   
    }

// ---------------------------------------------------------------------------
// CMccCryptoContextContainer::UpdateContextL
// ---------------------------------------------------------------------------
//
TInt CMccCryptoContextContainer::UpdateContext( TUint32 aContextId,
                                                TMccCryptoContext& aContext )
    {
    __INTERFACE( "CMccContextContainer::UpdateContext" )
    
    TInt contextId = FindContext( aContextId );
    
    if ( KErrNotFound != contextId )
        {
        iContextArray[ contextId ].UpdateContext( aContext );
            
        return KErrNone;
        }
    return KErrNotFound;
    }
 
// ---------------------------------------------------------------------------
// CMccCryptoContextContainer::GetContext
// ---------------------------------------------------------------------------
//
TInt CMccCryptoContextContainer::GetContext( TUint32 aContextId,
                                             TMccCryptoContext* &aContext )
    {
    __INTERFACE( "CMccContextContainer::GetContext" )
    
    TInt contextId = FindContext( aContextId );
    
    if ( KErrNotFound != contextId )
        {
        aContext = &iContextArray[ contextId ];
        return KErrNone;
        }

    return KErrNotFound;
    }

// -----------------------------------------------------------------------------
// CMccCryptoContextContainer::GenerateContextId
// -----------------------------------------------------------------------------
//
TUint32 CMccCryptoContextContainer::GenerateContextId()
    {
	__INTERFACE( "CMccContextContainer::GenerateContextId" )
    TUint32 id( 0 );
        
    do 
        {
        id = static_cast<TUint32>( Math::Random() );
        } while ( KErrNotFound != FindContext( id ) );

    return id;
    }

// ---------------------------------------------------------------------------
// CMccCryptoContextContainer::FindContext
// ---------------------------------------------------------------------------
//
TInt CMccCryptoContextContainer::FindContext( TUint32 aContextId )
    {
    TInt count( iContextArray.Count() );
    
    for ( TInt i = 0; i < count; i++ )
        {
        if ( iContextArray[ i ].ContextId() == aContextId )
            {
            return i;
            }
        }
    return KErrNotFound;
    }
//  End of File
