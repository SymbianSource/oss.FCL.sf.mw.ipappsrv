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
#include "mccresourcepool.h"
#include "mccresourcecontainer.h"
#include "mccresourceitem.h"
#include "mccinternaldef.h"
#include "mcccontrollerlogs.h"
#include "mccinternalevents.h"
#include "mccrtpdatasource.h"
#include "mccrtpdatasink.h"
#include "mcccamerahandler.h"
#include "mccvideosource.h"
#include "mccresourceiterators.h"
#include "mcctimermanager.h"

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
// CMccResourcePool::NewL
// -----------------------------------------------------------------------------
//
CMccResourcePool* CMccResourcePool::NewL()
    {
    CMccResourcePool* self = new ( ELeave ) CMccResourcePool;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::~CMccResourcePool
// -----------------------------------------------------------------------------
//        
CMccResourcePool::~CMccResourcePool()
    {
    iTempSources.Reset();
    iTempSinks.Reset();
    iResources.ResetAndDestroy();
    iResources.Close();
    iEndpoints.ResetAndDestroy();
    iEndpoints.Close();
    
    delete iCameraHandler;
    delete iTimerManager;
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::ReuseL
// -----------------------------------------------------------------------------
//    
void CMccResourcePool::ReuseL( 
    TUint32 aEndpointId )
    {
    CMccResourceItem* item = FindResourceItem( aEndpointId );
    __ASSERT_ALWAYS( item, User::Leave( KErrNotFound ) );
    
    if ( item->IsSource() )
        {
        iSourceCandidate = item->Source();
        iSourceCandidateEndpointId = aEndpointId;
        }
    else
        {
        iSinkCandidate = item->Sink();
        iSinkCandidateEndpointId = aEndpointId;
        }
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::ReferenceCountL
// -----------------------------------------------------------------------------
//    
void CMccResourcePool::ReferenceCountL( TUint32 aEndpointId, TInt& aRefCount )
    {
    CMccResourceItem* item = FindResourceItem( aEndpointId );
    __ASSERT_ALWAYS( item, User::Leave( KErrNotFound ) );
    
    aRefCount = item->RefCount();
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::SourceCandidate
// -----------------------------------------------------------------------------
//   
MDataSource* CMccResourcePool::SourceCandidate()
    {
    return iSourceCandidate;
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::SinkCandidate
// -----------------------------------------------------------------------------
//      
MDataSink* CMccResourcePool::SinkCandidate()
    {
    return iSinkCandidate;
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::SetEnableRtcpL
// -----------------------------------------------------------------------------
//   
void CMccResourcePool::SetEnableRtcpL( TUint32 aStreamId, TBool aEnableRtcp )
    {
    CMccResourceContainer* container = FindResourceContainer( aStreamId );
    __ASSERT_ALWAYS( container, User::Leave( KErrNotFound ) );
    
    container->SetEnableRtcp( aEnableRtcp );
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::EnableRtcp
// -----------------------------------------------------------------------------
//   
TBool CMccResourcePool::EnableRtcp( TUint32 aStreamId )
    {
    TBool enableRtcp( EFalse );
    CMccResourceContainer* container = FindResourceContainer( aStreamId );
    if ( container )
        {
        enableRtcp = container->EnableRtcp();
        }
    return enableRtcp;
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::CameraHandlerL
// -----------------------------------------------------------------------------
//
CMccCameraHandler& CMccResourcePool::CameraHandlerL( 
    TBool aCurrentCamera, 
    TInt aCameraIndex )
    {
    __ASSERT_ALWAYS( aCurrentCamera || 
                     iCameraHandler->CameraIndex() == aCameraIndex,
                     User::Leave( KErrNotReady ) );
    
    return *iCameraHandler;
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::CreateNewCameraHandlerL
// -----------------------------------------------------------------------------
//
CMccCameraHandler* CMccResourcePool::CreateNewCameraHandlerL( TInt aCameraIndex )
    {
    return CMccCameraHandler::NewL( *this, aCameraIndex );
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::UpdateResourceL
// -----------------------------------------------------------------------------
//
void CMccResourcePool::UpdateResourceL( TUint32 aEndpointId, const TDesC8& aVal )
    {
    __CONTROLLER( "CMccResourcePool::UpdateResourceL" )
    
    CMccResourceItem* item = FindResourceItem( aEndpointId );
    __ASSERT_ALWAYS( item, User::Leave( KErrNotFound ) );
    __ASSERT_ALWAYS( item->IsInternal(), User::Leave( KErrNotSupported ) );
    
    if ( item->IsSink() )
        {
        static_cast<CMccDataSink*>( item->Sink() )->UpdateL( aVal );
        }
    else
        {
        static_cast<CMccDataSource*>( item->Source() )->UpdateL( aVal );
        }
    }
    
// -----------------------------------------------------------------------------
// CMccResourcePool::ReserveSinkL
// -----------------------------------------------------------------------------
//
void CMccResourcePool::ReserveSinkL( MDataSink* aSink, TUint32 aEndpointId )
    {
    __ASSERT_ALWAYS( aSink, User::Leave( KErrArgument ) );
    iSinkCandidate = aSink;
    iSinkCandidateEndpointId = aEndpointId;
    
    if ( MCC_INTERNAL_ENDPOINT( aSink->DataSinkType().iUid ) )
        {
        static_cast<CMccDataSink*>( aSink )->SetResources( this );
        }
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::ReserveSourceL
// -----------------------------------------------------------------------------
//
void CMccResourcePool::ReserveSourceL( MDataSource* aSource, TUint32 aEndpointId )
    {
    __ASSERT_ALWAYS( aSource, User::Leave( KErrArgument ) );
    iSourceCandidate = aSource;
    iSourceCandidateEndpointId = aEndpointId;
    
    if ( MCC_INTERNAL_ENDPOINT( aSource->DataSourceType().iUid ) )
        {
        static_cast<CMccDataSource*>( aSource )->SetResources( this );
        }
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::PrepareEndpointRemovalL
// -----------------------------------------------------------------------------
//
void CMccResourcePool::PrepareEndpointRemovalL( 
    TUint32 aEndpointId, 
    RArray<TMccResourceParams>& aAffectedStreams )
    {
    __CONTROLLER( "CMccResourcePool::PrepareEndpointRemovalL" )
    
    CMccResourceItem* item = FindResourceItem( aEndpointId );
    __ASSERT_ALWAYS( item, User::Leave( KErrNotFound ) );
    
    item->UsersInfoL( aAffectedStreams );
    
    __CONTROLLER( "CMccResourcePool::PrepareEndpointRemovalL, exit" )
    }
    
// -----------------------------------------------------------------------------
// CMccResourcePool::RemoveEndpointL
// -----------------------------------------------------------------------------
//
void CMccResourcePool::RemoveEndpointL( TUint32 aEndpointId )
    {
    __CONTROLLER( "CMccResourcePool::RemoveEndpointL" )
    
    CMccResourceItem* item = FindResourceItem( aEndpointId );
    __ASSERT_ALWAYS( item, User::Leave( KErrNotFound ) );

    for ( TInt i = 0; i < iResources.Count(); i++ )
        {
        iResources[ i ]->HandleEndpointRemoval( aEndpointId );
        }
    
    // Possibly release camera resources
    HandleCameraResourcesL();
    
    // Possibly clear candidate
    //
    
    if ( aEndpointId == iSinkCandidateEndpointId )
        {
        iSinkCandidateEndpointId = 0;
        iSinkCandidate = 0;
        }
        
    if ( aEndpointId == iSourceCandidateEndpointId )
        {
        iSourceCandidateEndpointId = 0;
        iSourceCandidate = 0;
        }
        
    __CONTROLLER( "CMccResourcePool::RemoveEndpointL, exit" )
    }
    
// -----------------------------------------------------------------------------
// CMccResourcePool::ReserveResourcesL
// -----------------------------------------------------------------------------
//      
void CMccResourcePool::ReserveResourcesL( const TMccResourceParams& aParams )
    {
    __CONTROLLER( "CMccResourcePool::ReserveResourcesL" )
    
    __ASSERT_ALWAYS( iSinkCandidate && iSourceCandidate, 
                     User::Leave( KErrNotReady ) );
                   
    CMccResourceContainer* container = FindResourceContainer( aParams.iStreamId );
    __ASSERT_ALWAYS( !container, User::Leave( KErrAlreadyExists ) );
    
    CheckStandbyPropertiesL( aParams, *iSinkCandidate, *iSourceCandidate );
    
    CheckResourceAvailabilityL( *iSinkCandidate, *iSourceCandidate );
    
    CMccResourceItem* sinkItem = FindResourceItem( iSinkCandidateEndpointId );
    if ( !sinkItem )
        {
        sinkItem = CMccResourceItem::NewLC( aParams.iStreamId,
                                            iSinkCandidate, 
                                            0,
                                            iSinkCandidateEndpointId );
                                            
        iEndpoints.AppendL( sinkItem );
        CleanupStack::Pop( sinkItem );
        }
    
    CMccResourceItem* sourceItem = FindResourceItem( iSourceCandidateEndpointId );
    if ( !sourceItem )
        {
        sourceItem = CMccResourceItem::NewLC( aParams.iStreamId,
                                              0,
                                              iSourceCandidate, 
                                              iSourceCandidateEndpointId );
                                              
        iEndpoints.AppendL( sourceItem );
        CleanupStack::Pop( sourceItem );
        
        if ( iSourceCandidate->DataSourceType() == KMccVideoSourceUid )
            {
            static_cast<CMccVideoSource*>( iSourceCandidate )->SetCameraHandler( *iCameraHandler );
            }
        }
    
    container = CMccResourceContainer::NewL( aParams, iEndpoints );  
    CleanupStack::PushL( container ); 
    iResources.AppendL( container );
    CleanupStack::Pop( container );

    container->AddResourceItemL( aParams, sourceItem );
 
    container->AddResourceItemL( aParams, sinkItem );
    
    ClearCandidates();
    
    __CONTROLLER( "CMccResourcePool::ReserveResourcesL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::FreeResources
// -----------------------------------------------------------------------------
//     
void CMccResourcePool::FreeResources( TUint32 aStreamId )
    {
    __CONTROLLER( "CMccResourcePool::FreeResources" )
    
    TMccResourceParams params( 0, 0, aStreamId, 0, EFalse, 0 );
    TMccResourceContainerIterator iterator( iResources, params );
    CMccResourceContainer* container = NULL;
    
    // Only one entry per stream id
    if ( iterator.Next( container ) )
        {
        iterator.Delete( iResources );
        }
    
    __CONTROLLER( "CMccResourcePool::FreeResources, exit" )
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::IsResourceUsageAllowed
// -----------------------------------------------------------------------------
//                               
TBool CMccResourcePool::IsResourceUsageAllowed( TUint32 /*aStreamId*/ )
    {
    return ETrue;
    }                    

// -----------------------------------------------------------------------------
// CMccResourcePool::SetResourceStateL
// -----------------------------------------------------------------------------
//                                    
void CMccResourcePool::SetResourceStateL( 
    TUint32 aStreamId,
    TUint32 aEndpointId, 
    TResourceState aState,
    TBool& aControlNetworkResources,
    RArray<TUint32>& aControlledEndpoints,
    RArray<TUint32>& aUncontrolledEndpoints )
    {
    __CONTROLLER( "CMccResourcePool::SetResourceStateL" )
    
    CMccResourceContainer* container = FindResourceContainer( aStreamId );
    __ASSERT_ALWAYS( container, User::Leave( KErrNotFound ) );
    
    container->SetResourceStateL( aEndpointId, 
                                  aState, 
                                  aControlNetworkResources,
                                  aControlledEndpoints,
                                  aUncontrolledEndpoints );

    __CONTROLLER( "CMccResourcePool::SetResourceStateL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::ResourceStateSetL
// -----------------------------------------------------------------------------
//    
void CMccResourcePool::ResourceStateSetL()
    {
    // Possibly reserve or release camera resources
    HandleCameraResourcesL();
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::EventReceived
// Original event is possibly modified to contain standby stream control
// information. Additional control event may be returned as well when
// pausing existing active standby stream and activating other one.
// -----------------------------------------------------------------------------
//                                             
TInt CMccResourcePool::EventReceived( 
    TMccEvent& aEvent, 
    TBool& aDiscardEvent,
    TBool& aEffectiveStandby,
    TMccEvent** aAdditionalEvent )
    {
    TRAPD( err, *aAdditionalEvent = 
        StandbyLogicL( aEvent, aDiscardEvent, aEffectiveStandby ) )
    return err;
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::Sinks
// -----------------------------------------------------------------------------
//
const RPointerArray<MDataSink>& CMccResourcePool::Sinks( TBool aOnlyInternals )
    {
    iTempSinks.Reset();
    
    TMccResourceItemIterator iterator( iEndpoints, aOnlyInternals, ETrue, EFalse );
    CMccResourceItem* item = NULL;
    
    while( iterator.Next( item ) )
        {
        iTempSinks.Append( item->Sink() );
        }

    return iTempSinks;
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::Sources
// -----------------------------------------------------------------------------
//        
const RPointerArray<MDataSource>& CMccResourcePool::Sources( TBool aOnlyInternals )
    {
    iTempSources.Reset();
    
    TMccResourceItemIterator iterator( iEndpoints, aOnlyInternals, EFalse, ETrue );
    CMccResourceItem* item = NULL;
    
    while( iterator.Next( item ) )
        {
        iTempSources.Append( item->Source() );
        }
        
    return iTempSources;
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::MultiplexerL
// -----------------------------------------------------------------------------
//      
CMccMultiplexer* CMccResourcePool::MultiplexerL( MDataSource* aSource )
    {
    CMccResourceItem* item = FindResourceItem( MCC_ENDPOINT_ID( aSource ) );
    __ASSERT_ALWAYS( item, User::Leave( KErrNotFound ) );
    
    return item->MultiplexerL();
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::FindResource
// -----------------------------------------------------------------------------
//    
TInt CMccResourcePool::FindResource( 
    TUint32 aLinkId,
    TUint32 aEndpointId, 
    TUid aResourceUid, 
    MDataSource** aSource, 
    MDataSink** aSink )
    {
    if ( !aLinkId )
        {
        CMccResourceItem* item = FindResourceItem( aEndpointId );
        if ( item )
            {
            return GetEndpoint( *item, aResourceUid, aSource, aSink );
            }
        }
    else
        {
        // If link id is defined, search for any endpoint of given type which
        // is used in the link.
        
        TMccResourceParams params( 0, aLinkId, 0, 0, EFalse, 0 );
        
        TMccResourceItemIterator iterator( iEndpoints, params );
        CMccResourceItem* item = NULL;
    
        while( iterator.Next( item ) )
            {
            if ( GetEndpoint( *item, aResourceUid, aSource, aSink ) == KErrNone )
                {
                return KErrNone;
                }
            }
        }
    return KErrNotFound;
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::FindNetworkResourceByRtpStreamId
// -----------------------------------------------------------------------------
//   
TInt CMccResourcePool::FindNetworkResourceByRtpStreamId( 
    const TUint32 aMccSessionId,
    const TRtpId& aRtpStreamId,
    MDataSource** aSource, 
    MDataSink** aSink )
    {
    TBool found( EFalse );
    
    TMccResourceParams params( aMccSessionId, 0, 0, 0, EFalse, 0 );
    TMccResourceItemIterator iterator( iEndpoints, KMccRtpSourceUid, params );
    CMccResourceItem* item = NULL;
    
    while ( iterator.Next( item ) )
        {
        CMccRtpDataSource* rtpSource = 
                static_cast<CMccRtpDataSource*>( item->Source() );
                
        if ( rtpSource->HandleByStreamId( aRtpStreamId ) )
            {
            *aSource = item->Source();
            found = ETrue;
            }
        }
    
    TMccResourceItemIterator iterator2( iEndpoints, KMccRtpSinkUid, params );
    item = NULL;
    
    while ( !found && iterator2.Next( item ) )
        {
        CMccRtpDataSink* rtpSink = 
                static_cast<CMccRtpDataSink*>( item->Sink() );
                
        if ( rtpSink->HandleByStreamId( aRtpStreamId ) )
            {
            *aSink = item->Sink();
            found = ETrue;
            }
        }
        
    return ( found ? KErrNone : KErrNotFound );
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::FindNetworkResourceByRtpSsrc
// -----------------------------------------------------------------------------
//   
TInt CMccResourcePool::FindNetworkResourceByRtpSsrc( 
    const TRtpSSRC& aRtpSsrc,
    MDataSource** aSource, 
    MDataSink** aSink )
    {
    TBool found( EFalse );
    
    TMccResourceItemIterator iterator( iEndpoints, KMccRtpSourceUid );
    CMccResourceItem* item = NULL;
    
    while ( iterator.Next( item ) )
        {
        CMccRtpDataSource* rtpSource = 
                static_cast<CMccRtpDataSource*>( item->Source() );
                
        if ( rtpSource->HandleBySsrc( aRtpSsrc ) )
            {
            *aSource = item->Source();
            found = ETrue;
            }
        }
    
    TMccResourceItemIterator iterator2( iEndpoints, KMccRtpSinkUid );
    item = NULL;
    
    while ( !found && iterator2.Next( item ) )
        {
        CMccRtpDataSink* rtpSink = 
                static_cast<CMccRtpDataSink*>( item->Sink() );
                
        if ( rtpSink->HandleBySsrc( aRtpSsrc ) )
            {
            *aSink = item->Sink();
            found = ETrue;
            }
        }
    
    return ( found ? KErrNone : KErrNotFound );
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::TimerManager
// -----------------------------------------------------------------------------
// 
MMccTimerManager& CMccResourcePool::TimerManager()
    {
    return *iTimerManager;
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::StoreKeyFrameInfoL
// -----------------------------------------------------------------------------
//
void CMccResourcePool::StoreKeyFrameInfoL( TUint32 aEndpointId, CMMFBuffer& aKeyFrame )
    {
    CMccResourceItem* item = FindResourceItem( aEndpointId );
    __ASSERT_ALWAYS( item, User::Leave( KErrNotFound ) );
    item->StoreKeyFrameInfoL( aKeyFrame );
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::IsKeyFrame
// -----------------------------------------------------------------------------
//
TBool CMccResourcePool::IsKeyFrame( TUint32 aEndpointId, CMMFBuffer& aBuffer )
    {
    // First find associated resource containers, some of their endpoints
    // may contain keyframe info
    TMccResourceContainerIterator iterator( iResources, aEndpointId );
    CMccResourceContainer* container = NULL;
        
    while( iterator.Next( container ) )
        {
        TMccResourceParams params2( 0, 0, container->StreamId(), 0, EFalse, 0 );
        TMccResourceItemIterator endpointIterator( iEndpoints, params2 );
        CMccResourceItem* item = NULL;
        while ( endpointIterator.Next( item ) )
            {
            if ( item->IsKeyFrame( aBuffer ) )
                {
                return ETrue;
                }
            }
        }
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::EnableViewFinderL
// -----------------------------------------------------------------------------
// 
void CMccResourcePool::EnableViewFinderL( TMccVideoSinkSetting& aSetting )
    {
    __CONTROLLER( "CMccResourcePool::EnableViewFinderL" )
    
    CameraHandlerL().EnableViewFinderL( aSetting );

    HandleCameraResourcesL();
    
    __CONTROLLER( "CMccResourcePool::EnableViewFinderL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::DisableViewFinderL
// -----------------------------------------------------------------------------
// 
void CMccResourcePool::DisableViewFinderL()
    {
    __CONTROLLER( "CMccResourcePool::DisableViewFinderL" )
    
    CameraHandlerL().DisableViewFinderL();
    
    HandleCameraResourcesL();
    
    __CONTROLLER( "CMccResourcePool::DisableViewFinderL, exit" )
    }
        
// -----------------------------------------------------------------------------
// CMccResourcePool::FindResourceContainer
// -----------------------------------------------------------------------------
//    
CMccResourceContainer* CMccResourcePool::FindResourceContainer( 
    TUint32 aStreamId )
    {
    TMccResourceParams params( 0, 0, aStreamId, 0, EFalse, 0 );
    TMccResourceContainerIterator iterator( iResources, params );
    CMccResourceContainer* container = NULL;
    
    iterator.Next( container );

    return container;
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::FindResourceItem
// -----------------------------------------------------------------------------
//    
CMccResourceItem* CMccResourcePool::FindResourceItem( TUint32 aEndpointId )
    {
    TMccResourceItemIterator iterator( iEndpoints, aEndpointId );
    CMccResourceItem* item = NULL;
    
    iterator.Next( item );

    return item;
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::ClearCandidates
// -----------------------------------------------------------------------------
//    
void CMccResourcePool::ClearCandidates()
    {
    iSourceCandidate = 0;
    iSourceCandidateEndpointId = 0;
    iSinkCandidate = 0;
    iSinkCandidateEndpointId = 0;
    }
    
// -----------------------------------------------------------------------------
// CMccResourcePool::HandleCameraResourcesL
// -----------------------------------------------------------------------------
// 
void CMccResourcePool::HandleCameraResourcesL()
    {
    // If any video source is active, camera resources are needed.
    //
    
    CMccVideoSource* needsCamera = 0;
    
    TMccResourceItemIterator iterator( iEndpoints, KMccVideoSourceUid );
    CMccResourceItem* item = NULL;
    
    while ( iterator.Next( item ) && !needsCamera )
        {
        needsCamera = item->NeedsCamera( *iCameraHandler ) ?
                static_cast<CMccVideoSource*>( item->Source() ) : NULL;
        }
   
    if ( needsCamera )
        {
        // The first videosource can define the camera used
        iCameraHandler->ReserveResourcesL( 
            static_cast<TInt>( needsCamera->CameraIndex() ) );
        }
   else
        {
        iCameraHandler->ReleaseResources();
        }
   }

// -----------------------------------------------------------------------------
// CMccResourcePool::GetEndpoint
// -----------------------------------------------------------------------------
//
TInt CMccResourcePool::GetEndpoint( 
    CMccResourceItem& aItem,
    TUid aResourceUid,
    MDataSource** aSource, 
    MDataSink** aSink )
    {   
    if ( aItem.IsSource() && 
       ( aResourceUid == KNullUid || 
         aItem.Source()->DataSourceType() == aResourceUid ) )
        {
        *aSource = aItem.Source();
        return KErrNone;
        }
    
    if ( aItem.IsSink() && 
       ( aResourceUid == KNullUid ||
         aItem.Sink()->DataSinkType() == aResourceUid ) )
        {
        *aSink = aItem.Sink();
        return KErrNone;
        }
    return KErrNotFound;
    } 

// -----------------------------------------------------------------------------
// CMccResourcePool::CheckStandbyPropertiesL
// -----------------------------------------------------------------------------
//     
void CMccResourcePool::CheckStandbyPropertiesL( 
    const TMccResourceParams& aParams, 
    MDataSink& aSink, 
    MDataSource& aSource )
    {
    __CONTROLLER( "CMccResourcePool::CheckStandbyPropertiesL" )
    
    if ( aParams.iIsStandby )
        {
        // Standby is allowed only for audio downlink
        __ASSERT_ALWAYS( aSink.DataSinkType() == KUidMmfAudioOutput &&
                         aSource.DataSourceType() == KMccRtpSourceUid,
                         User::Leave( KErrArgument ) );   
        }
    
    __CONTROLLER( "CMccResourcePool::CheckStandbyPropertiesL, exit" )
    }
    
// -----------------------------------------------------------------------------
// CMccResourcePool::CheckResourceAvailabilityL
// -----------------------------------------------------------------------------
//     
void CMccResourcePool::CheckResourceAvailabilityL( 
    MDataSink& /*aSink*/, 
    MDataSource& aSource )
    {
    __CONTROLLER( "CMccResourcePool::CheckResourceAvailabilityL" )
    
    // Only one instance of the media recorder is allowed, having more
    // causes some serious system unstability. 
    // TBD: multiple videosource instances could be allowed if "resource not
    // available", "resource released", etc. events would be passed to the 
    // client and only one active videosource would be allowed. 
    if ( aSource.DataSourceType() == KMccVideoSourceUid )
        {
        TMccResourceItemIterator iterator( iEndpoints, KMccVideoSourceUid );
        CMccResourceItem* item = NULL;
    
        while ( iterator.Next( item ) )
            {
            __ASSERT_ALWAYS( item->Source() == &aSource, 
                             User::Leave( KErrAccessDenied ) );
            }
        }
    
    __CONTROLLER( "CMccResourcePool::CheckResourceAvailabilityL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::StandbyLogicL
// -----------------------------------------------------------------------------
//    
TMccEvent* CMccResourcePool::StandbyLogicL( 
    TMccEvent& aEvent, 
    TBool& aDiscardEvent,
    TBool& aEffectiveStandby )
    {
    TMccEvent* additionalControl = NULL;
    
    CMccResourceContainer* container = 
	    	FindResourceContainer( aEvent.iStreamId );

    if ( container && container->IsStandBy() )
		{
		__CONTROLLER( "CMccResourcePool::StandbyLogicL" )
		
		if ( container->StandbyControl() && aEvent.iEventType != KMccStreamIdle )	
			{
			// If standby mechanism is controlling the stream, event is discarded
			aDiscardEvent = ETrue;
		    container->SetStandbyControl( EFalse );
			}
		else if ( aEvent.iEventType == KMccStandbyInactivityEvent )
		    {
		    StandbyInactivityL( *container, aEvent, aDiscardEvent, aEffectiveStandby );
		    }
		else if ( aEvent.iEventType == KMccStandbyActivityEvent )
		    {
		    additionalControl =
		        StandbyActivityL( *container, aEvent, aDiscardEvent, aEffectiveStandby );
		    }
		else
		    {
		    // NOP
		    }  
    
		__CONTROLLER( "CMccResourcePool::StandbyLogicL, exit" )
		}
		
    return additionalControl;	
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::StandbyInactivityL
// -----------------------------------------------------------------------------
//    
void CMccResourcePool::StandbyInactivityL( 
    CMccResourceContainer& aContainer,
    TMccEvent& aEvent, 
    TBool& aDiscardEvent,
    TBool& aEffectiveStandby )
    {
	__CONTROLLER( "CMccResourcePool::StandbyInactivityL" )
    
    // If already inactive, no need to do anything
    CMccResourceItem& standbyResource = aContainer.FetchStandbyEndpointL();
    if ( standbyResource.CurrentState() == MMccResources::EStarted )
        {
        __CONTROLLER( "CMccResourcePool::StandbyInactivityL, effective" )
        
        aContainer.SetStandbyControl( ETrue );
        aEvent.iEndpointId = standbyResource.EndpointId();
        aEffectiveStandby = ETrue;
        }
	aDiscardEvent = ETrue;
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::StandbyActivityL
// -----------------------------------------------------------------------------
// 
TMccEvent* CMccResourcePool::StandbyActivityL(
    CMccResourceContainer& aContainer,
    TMccEvent& aEvent, 
    TBool& aDiscardEvent,
    TBool& aEffectiveStandby )
    {
    __CONTROLLER( "CMccResourcePool::StandbyActivityL" )
    
    TMccEvent* additionalControl = NULL;
		    	
    // If already active, no need to do anything
    CMccResourceItem& standbyResource = aContainer.FetchStandbyEndpointL();
    if ( standbyResource.CurrentState() == MMccResources::EPrepared ||
         standbyResource.CurrentState() == MMccResources::EResourcePaused )
        {
        __CONTROLLER( "CMccResourcePool::StandbyActivityL, effective" )
        
        // There might be other active standby stream within the link,
        // that has to be paused.
        TMccResourceParams params( 0, aEvent.iLinkId, 0, 0, EFalse, 0 );
        TMccResourceContainerIterator iterator( iResources, params );
        CMccResourceContainer* assocContainer = NULL;
        while ( iterator.Next( assocContainer ) && !additionalControl )
            {
            if ( assocContainer->IsStandBy() && 
                 assocContainer->FetchStandbyEndpointL().CurrentState() == 
                    MMccResources::EStarted )
                {
                __CONTROLLER( "CMccResourcePool::StandbyActivityL, other stream has to be paused" )
                
                additionalControl = new ( ELeave ) TMccEvent( aEvent );
                additionalControl->iEventType = KMccStandbyInactivityEvent;
                additionalControl->iStreamId = assocContainer->StreamId();
                additionalControl->iEndpointId = 
                    assocContainer->FetchStandbyEndpointL().EndpointId();
                assocContainer->SetStandbyControl( ETrue );
                }
            }
        
		aContainer.SetStandbyControl( ETrue );
		aEvent.iEndpointId = standbyResource.EndpointId(); 
		aEffectiveStandby = ETrue;
        }
	aDiscardEvent = ETrue;
	
	return additionalControl;
    }
// -----------------------------------------------------------------------------
// CMccResourcePool::CMccResourcePool
// -----------------------------------------------------------------------------
//      
CMccResourcePool::CMccResourcePool()
    {
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::ConstructL
// -----------------------------------------------------------------------------
// 
void CMccResourcePool::ConstructL()
    {
    iTimerManager = CMccTimerManager::NewL();
    iCameraHandler = CMccCameraHandler::NewL( *this );
    }

