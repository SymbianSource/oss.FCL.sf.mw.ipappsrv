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
* Description:    
*
*/



// INCLUDE FILES
#include "mccresourcecontainer.h"
#include "mccresourceitem.h"
#include "mcccontrollerlogs.h"
#include "mccinternaldef.h"
#include "mccresourceiterators.h"

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccResourceContainer::NewLC
// -----------------------------------------------------------------------------
//
CMccResourceContainer* CMccResourceContainer::NewLC(
    const TMccResourceParams& aParams,
    RPointerArray<CMccResourceItem>& aAllEndpoints )
    {
    CMccResourceContainer* self = 
        new ( ELeave ) CMccResourceContainer( aParams, aAllEndpoints );
    CleanupStack::PushL( self );
    return self;
    }
    
// -----------------------------------------------------------------------------
// CMccResourceContainer::NewL
// -----------------------------------------------------------------------------
//
CMccResourceContainer* CMccResourceContainer::NewL(
    const TMccResourceParams& aParams,
    RPointerArray<CMccResourceItem>& aAllEndpoints )
    {
    CMccResourceContainer* self = 
        CMccResourceContainer::NewLC( aParams, aAllEndpoints );
    CleanupStack::Pop( self );
    return self;
    }

    
// -----------------------------------------------------------------------------
// CMccResourceContainer::~CMccResourceContainer
// -----------------------------------------------------------------------------
//       
CMccResourceContainer::~CMccResourceContainer()
    {
    HandleEndpointRemoval();
        
    iOwnEndpoints.Reset();
    iOwnEndpoints.Close();
    }
    
// -----------------------------------------------------------------------------
// CMccResourceContainer::AddResourceItemL
// -----------------------------------------------------------------------------
//
void CMccResourceContainer::AddResourceItemL( 
    const TMccResourceParams& aUser,
    CMccResourceItem* aItem )
    {
    __ASSERT_ALWAYS( aItem, User::Leave( KErrArgument ) );
    iOwnEndpoints.AppendL( aItem );
    TInt err = aItem->IncreaseRefCount( aUser );
    if ( err )
        {
        // Couldn't increase reference count, cannot use the resource
        iOwnEndpoints.Remove( iOwnEndpoints.Count() - 1 );
        User::Leave( err );
        }
    }

// -----------------------------------------------------------------------------
// CMccResourceContainer::FindResourceItem
// -----------------------------------------------------------------------------
//   
CMccResourceItem* CMccResourceContainer::FindResourceItem( TUint32 aEndpointId )
    {
    TMccResourceItemIterator iterator( iOwnEndpoints, aEndpointId );
    CMccResourceItem* item = NULL;
    
    iterator.Next( item );
    
    return item;
    }

// -----------------------------------------------------------------------------
// CMccResourceContainer::LinkId
// -----------------------------------------------------------------------------
//
TUint32 CMccResourceContainer::LinkId() const
    {
    return iParams.iLinkId;
    }
    
// -----------------------------------------------------------------------------
// CMccResourceContainer::StreamId
// -----------------------------------------------------------------------------
//
TUint32 CMccResourceContainer::StreamId() const
    {
    return iParams.iStreamId;
    }

// -----------------------------------------------------------------------------
// CMccResourceContainer::SetResourceStateL
// -----------------------------------------------------------------------------
//    
void CMccResourceContainer::SetResourceStateL( 
    TUint32 aEndpointId, 
    MMccResources::TResourceState aState,
    TBool& aControlNetworkResources,
    RArray<TUint32>& aControlledEndpoints,
    RArray<TUint32>& aUncontrolledEndpoints )
    { 
    __CONTROLLER_INT1( "CMccResourceContainer::SetResourceStateL endpoint:", 
                       aEndpointId ) 
    __CONTROLLER_INT1( "CMccResourceContainer::SetResourceStateL state:", 
                       aState )
                       
    aControlNetworkResources = EFalse;
    
    if ( MCC_CONTROL_ALL_ENDPOINTS( aEndpointId ) )
        {
        aControlNetworkResources = ETrue;
        
        TMccResourceItemIterator iterator( iOwnEndpoints );
        CMccResourceItem* item = NULL;
        while ( iterator.Next( item ) )
            {
            if ( item->SetResourceStateL( iParams.iStreamId, 
                                          aState, 
                                          StandbyControl(),
                                          DtmfControl() ) )
                {
                aControlledEndpoints.AppendL( item->EndpointId() );
                }
            else
                {
                aUncontrolledEndpoints.AppendL( item->EndpointId() );
                }
            }
        }
    else
        {
        CMccResourceItem* item = FindResourceItem( aEndpointId );
        __ASSERT_ALWAYS( item, User::Leave( KErrNotFound ) );
    
        // Network resources must be always prepared first
        //
        aControlNetworkResources = item->IsNetworkResource();
      
        TMccResourceItemIterator iterator( iOwnEndpoints );
        CMccResourceItem* comparedItem = NULL;
        while ( iterator.Next( comparedItem ) )
            {
            if ( comparedItem != item && 
                 aState == MMccResources::EPrepared && 
                 !aControlNetworkResources )
                {
                if ( comparedItem->IsNetworkResource() &&
                     !comparedItem->IsResourceActive() )
                    {
                    // Network resources are not yet active
                    User::Leave( KErrNotReady );
                    }
                }
            }
        
        if ( item->SetResourceStateL( iParams.iStreamId, 
                                      aState, 
                                      StandbyControl(),
                                      DtmfControl() ) )
            {
            aControlledEndpoints.AppendL( item->EndpointId() );
            }
        else
            {
            aUncontrolledEndpoints.AppendL( item->EndpointId() );
            }
        }
        
    __CONTROLLER( "CMccResourceContainer::SetResourceStateL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccResourceContainer::SetEnableRtcp
// -----------------------------------------------------------------------------
//   
void CMccResourceContainer::SetEnableRtcp( TBool aEnableRtcp )
    {
    iEnableRtcp = aEnableRtcp;
    }

// -----------------------------------------------------------------------------
// CMccResourceContainer::EnableRtcp
// -----------------------------------------------------------------------------
//   
TBool CMccResourceContainer::EnableRtcp() const
    {
    return iEnableRtcp;
    }

// -----------------------------------------------------------------------------
// CMccResourceContainer::HandleEndpointRemoval
// -----------------------------------------------------------------------------
//
void CMccResourceContainer::HandleEndpointRemoval( TUint32 aEndpointId )
    { 
    __CONTROLLER( "CMccResourceContainer::HandleEndpointRemoval" )
    
    // If called with zero endpoint, all owned endpoints are "removed"
    TInt lastIndex( iOwnEndpoints.Count() - 1 );
    for ( TInt i = lastIndex; i >= 0; i-- )
        {
        if ( !aEndpointId || iOwnEndpoints[ i ]->EndpointId() == aEndpointId )
            {
            if ( iOwnEndpoints[ i ]->DecreaseRefCount( iParams ) )
                {
                DeleteEndpoint( iOwnEndpoints[ i ]->EndpointId() );
                }
                
            iOwnEndpoints.Remove( i );
            }
        }
        
    __CONTROLLER( "CMccResourceContainer::HandleEndpointRemoval, exit" )
    }
          
// -----------------------------------------------------------------------------
// CMccResourceContainer::DeleteEndpoint
// -----------------------------------------------------------------------------
// 
void CMccResourceContainer::DeleteEndpoint( TUint32 aEndpointId )
    {
    __CONTROLLER( "CMccResourceContainer::DeleteEndpoint" )
    
    TInt lastIndex( iAllEndpoints.Count() - 1 );
    for ( TInt i = lastIndex; i >= 0; i-- )
        {
        if ( iAllEndpoints[ i ]->EndpointId() == aEndpointId )
            {
            delete iAllEndpoints[ i ];
            iAllEndpoints.Remove( i );
            return;
            }
        } 
        
    __CONTROLLER( "CMccResourceContainer::DeleteEndpoint, exit" )
    }
    
// -----------------------------------------------------------------------------
// CMccResourceContainer::CMccResourceContainer
// -----------------------------------------------------------------------------
//      
CMccResourceContainer::CMccResourceContainer(
    const TMccResourceParams& aParams,
    RPointerArray<CMccResourceItem>& aAllEndpoints ) :
    iParams( aParams ),
    iAllEndpoints( aAllEndpoints )
    {
    }

// -----------------------------------------------------------------------------
// CMccResourceContainer::IsStandBy
// -----------------------------------------------------------------------------
//  
TBool CMccResourceContainer::IsStandBy()
	{
	// Do not considered resource as standby resource if there's no
	// standby resource item present
	TMccResourceItemIterator iterator( iOwnEndpoints, KUidMmfAudioOutput );
    CMccResourceItem* standbyEndpoint = NULL;
    iterator.Next( standbyEndpoint );
	return ( standbyEndpoint && iParams.iIsStandby );	
	}

// -----------------------------------------------------------------------------
// CMccResourceContainer::SetStandbyControl
// -----------------------------------------------------------------------------
//
void CMccResourceContainer::SetStandbyControl( TBool aStandbyControl )
	{
	iStandbyControl = aStandbyControl;
	}
        
// -----------------------------------------------------------------------------
// CMccResourceContainer::StandbyControl
// -----------------------------------------------------------------------------
//      
TBool CMccResourceContainer::StandbyControl() const
	{
	return iStandbyControl;	
	}

// -----------------------------------------------------------------------------
// CMccResourceContainer::DtmfControl
// -----------------------------------------------------------------------------
//
TBool CMccResourceContainer::DtmfControl() const
    {
    return ( iParams.iStreamType == KMccDtmfStream );
    }
    
// -----------------------------------------------------------------------------
// CMccResourceContainer::FetchStandbyEndpointL
// -----------------------------------------------------------------------------
//  
CMccResourceItem& CMccResourceContainer::FetchStandbyEndpointL()
	{
	TMccResourceItemIterator iterator( iOwnEndpoints, KUidMmfAudioOutput );
    CMccResourceItem* standbyEndpoint = NULL;
    iterator.Next( standbyEndpoint );
        
    __ASSERT_ALWAYS( standbyEndpoint, User::Leave( KErrNotFound ) );

    return *standbyEndpoint;
	}


