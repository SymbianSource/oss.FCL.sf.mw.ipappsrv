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
//#include "mccmultiplexer.h"

#include "mccuids.hrh"
#include "mccdatasource.h"
#include "mccdatasink.h"
#include "QosControllerLog.h"

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
    /*
    iResources.ResetAndDestroy();
    iResources.Close();
    iEndpoints.ResetAndDestroy();
    iEndpoints.Close();
    
    delete iCameraHandler;
    */
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::ReuseL
// -----------------------------------------------------------------------------
//    
void CMccResourcePool::ReuseL( 
    TUint32 /*aEndpointId*/ )
    {
    /*
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
        */
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::ReferenceCountL
// -----------------------------------------------------------------------------
//    
void CMccResourcePool::ReferenceCountL( TUint32 /*aEndpointId*/, TInt& /*aRefCount*/ )
    {
    /*
    CMccResourceItem* item = FindResourceItem( aEndpointId );
    __ASSERT_ALWAYS( item, User::Leave( KErrNotFound ) );
    
    aRefCount = item->RefCount();
    */
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
void CMccResourcePool::SetEnableRtcpL( TUint32 /*aStreamId*/, TBool /*aEnableRtcp*/ )
    {
    /*
    CMccResourceContainer* container = FindResourceContainer( aStreamId );
    __ASSERT_ALWAYS( container, User::Leave( KErrNotFound ) );
    
    container->SetEnableRtcp( aEnableRtcp );
    */
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::EnableRtcp
// -----------------------------------------------------------------------------
//   
TBool CMccResourcePool::EnableRtcp( TUint32 /*aStreamId*/ )
    {
    
    TBool enableRtcp( EFalse );
    /*
    CMccResourceContainer* container = FindResourceContainer( aStreamId );
    if ( container )
        {
        enableRtcp = container->EnableRtcp();
        }
        */
    return enableRtcp;
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::CameraHandlerL
// -----------------------------------------------------------------------------
//
CMccCameraHandler& CMccResourcePool::CameraHandlerL( 
    TBool /*aCurrentCamera*/, 
    TInt /*aCameraIndex*/ )
    {
    /*
    __ASSERT_ALWAYS( aCurrentCamera || 
                     iCameraHandler->CameraIndex() == aCameraIndex,
                     User::Leave( KErrNotReady ) );
     */
    return *iCameraHandler;
   
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::CreateNewCameraHandlerL
// -----------------------------------------------------------------------------
//
CMccCameraHandler* CMccResourcePool::CreateNewCameraHandlerL( TInt /*aCameraIndex*/ )
    {
   // return CMccCameraHandler::NewL( aCameraIndex );
    return NULL;
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::UpdateResourceL
// -----------------------------------------------------------------------------
//
void CMccResourcePool::UpdateResourceL( TUint32 /*aEndpointId*/, const TDesC8& /*aVal*/ )
    {
    /*
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
        */
    }
    
// -----------------------------------------------------------------------------
// CMccResourcePool::ReserveSinkL
// -----------------------------------------------------------------------------
//
void CMccResourcePool::ReserveSinkL( MDataSink* /*aSink*/, TUint32 /*aEndpointId*/ )
    {
    /*
    __ASSERT_ALWAYS( aSink, User::Leave( KErrArgument ) );
    iSinkCandidate = aSink;
    iSinkCandidateEndpointId = aEndpointId;
    */
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::ReserveSourceL
// -----------------------------------------------------------------------------
//
void CMccResourcePool::ReserveSourceL( MDataSource* /*aSource*/, TUint32 /*aEndpointId*/ )
    {
    /*
    __ASSERT_ALWAYS( aSource, User::Leave( KErrArgument ) );
    iSourceCandidate = aSource;
    iSourceCandidateEndpointId = aEndpointId;
    */
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::PrepareEndpointRemovalL
// -----------------------------------------------------------------------------
//
void CMccResourcePool::PrepareEndpointRemovalL( 
    TUint32 /*aEndpointId*/, 
    RArray<TMccResourceParams>& /*aAffectedStreams*/ )
    {
    /*
    __CONTROLLER( "CMccResourcePool::PrepareEndpointRemovalL" )
    
    CMccResourceItem* item = FindResourceItem( aEndpointId );
    __ASSERT_ALWAYS( item, User::Leave( KErrNotFound ) );
    
    item->UsersInfoL( aAffectedStreams );
    
    __CONTROLLER( "CMccResourcePool::PrepareEndpointRemovalL, exit" )
    */
    }
    
// -----------------------------------------------------------------------------
// CMccResourcePool::RemoveEndpointL
// -----------------------------------------------------------------------------
//
void CMccResourcePool::RemoveEndpointL( TUint32 /*aEndpointId*/ )
    {
    /*
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
    */
    }
    
// -----------------------------------------------------------------------------
// CMccResourcePool::ReserveResourcesL
// -----------------------------------------------------------------------------
//      
void CMccResourcePool::ReserveResourcesL( const TMccResourceParams& /*aParams*/ )
    {
    /*
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
    */
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::FreeResources
// -----------------------------------------------------------------------------
//     
void CMccResourcePool::FreeResources( TUint32 /*aStreamId*/ )
    {
    /*
    __CONTROLLER( "CMccResourcePool::FreeResources" )
    
    // Only one entry per stream id
    TBool found( EFalse );
    for ( TInt i = 0; i < iResources.Count() && !found; i++ )
        {
        if ( iResources[ i ]->StreamId() == aStreamId )
            {
            delete iResources[ i ];
            iResources.Remove( i );
            iResources.Compress();
            found = ETrue;
            }
        }
        
    __CONTROLLER( "CMccResourcePool::FreeResources, exit" )
    */
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
// CMccResourcePool::SetUserInitiationModeL
// -----------------------------------------------------------------------------
//
void CMccResourcePool::SetUserInitiationModeL( 
    TUint32 /*aStreamId*/, 
    TBool /*iIsUserInititated*/ )
    {
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::SetResourceStateL
// -----------------------------------------------------------------------------
//                                    
void CMccResourcePool::SetResourceStateL( 
    TUint32 /*aStreamId*/,
    TUint32 /*aEndpointId*/, 
    TResourceState /*aState*/,
    TBool& /*aControlNetworkResources*/,
    RArray<TUint32>& /*aControlledEndpoints*/,
    RArray<TUint32>& /*aUncontrolledEndpoints*/ )
    {/*
    __CONTROLLER( "CMccResourcePool::SetResourceStateL" )
    
    CMccResourceContainer* container = FindResourceContainer( aStreamId );
    __ASSERT_ALWAYS( container, User::Leave( KErrNotFound ) );
    
    container->SetResourceStateL( aEndpointId, 
                                  aState, 
                                  aControlNetworkResources,
                                  aControlledEndpoints,
                                  aUncontrolledEndpoints );

    __CONTROLLER( "CMccResourcePool::SetResourceStateL, exit" )
    */
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::ResourceStateSetL
// -----------------------------------------------------------------------------
//    
void CMccResourcePool::ResourceStateSetL()
    {
    // Possibly reserve or release camera resources
   // HandleCameraResourcesL();
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::EventReceived
// -----------------------------------------------------------------------------
//                                             
TInt CMccResourcePool::EventReceived( TMccEvent& /*aEvent*/, 
                                      TBool& /*aDiscardEvent*/,
                                      TBool& /*aEffectiveStandby*/,
                                      TMccEvent** /*aAdditionalEvent*/ )
    {
    TInt ret = KErrNone;
/*
    CMccResourceContainer* container = 
	    	FindResourceContainer( aEvent.iStreamId );

    if ( container && container->IsStandBy() )
		{
		if ( container->StandbyControl() && aEvent.iEventType != KMccStreamIdle )	
			{
			// If standby mechanism is controlling the stream, event is discarded
			aDiscardEvent = ETrue;
		    container->SetStandbyControl( EFalse );
			}
		else if ( aEvent.iEventType == KMccStandbyInactivityEvent ||
		    aEvent.iEventType == KMccStandbyActivityEvent )
		    {		 
			container->SetStandbyControl( ETrue );
			aDiscardEvent = ETrue;
			endpointId = container->FetchEndPointId();   
		    }
		else
		    {
		    // NOP
		    }      
		}  
		*/  
    return ret;
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::Sinks
// -----------------------------------------------------------------------------
//
const RPointerArray<MDataSink>& CMccResourcePool::Sinks( TBool /*aOnlyInternals*/ )
    {
    /*
    iTempSinks.Reset();
    for ( TInt i = 0; i < iEndpoints.Count(); i++ )
        {
        CMccResourceItem* item = iEndpoints[ i ];
        if ( item->IsSink() &&
           ( !aOnlyInternals || item->IsInternal() ) )
            {
            iTempSinks.Append( item->Sink() ); 
            }
        }
        */
    return iTempSinks;
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::Sources
// -----------------------------------------------------------------------------
//        
const RPointerArray<MDataSource>& CMccResourcePool::Sources( TBool /*aOnlyInternals*/ )
    {
    /*
    iTempSources.Reset();
    for ( TInt i = 0; i < iEndpoints.Count(); i++ )
        {
        CMccResourceItem* item = iEndpoints[ i ];
        if ( item->IsSource() && 
           ( !aOnlyInternals || item->IsInternal() ) )
            {
            iTempSources.Append( item->Source() ); 
            }
        }
        */
    return iTempSources;
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::MultiplexerL
// -----------------------------------------------------------------------------
//      
CMccMultiplexer* CMccResourcePool::MultiplexerL( MDataSource* /*aSource*/ )
    {
    /*
    CMccResourceItem* item = FindResourceItem( MCC_ENDPOINT_ID( aSource ) );
    __ASSERT_ALWAYS( item, User::Leave( KErrNotFound ) );
    
    return item->MultiplexerL();
    */
    return NULL;
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::FindResource
// -----------------------------------------------------------------------------
//    
TInt CMccResourcePool::FindResource( 
    TUint32 /*aLinkId*/,
    TUint32 /*aEndpointId*/, 
    TUid /*aResourceUid*/, 
    MDataSource** /*aSource*/, 
    MDataSink** /*aSink*/ )
    {
    /*
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
        
        TMccResourceParams params( 0, aLinkId, 0, 0, EFalse );
        for ( TInt i = 0; i < iEndpoints.Count(); i++ )
            {
            if ( iEndpoints[ i ]->Match( params ) && 
                 GetEndpoint( *iEndpoints[ i ], aResourceUid, aSource, aSink ) == KErrNone )
                {
                return KErrNone;
                }
            }
        }
        */
    return KErrNotFound;
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::FindNetworkResourceByRtpStreamId
// -----------------------------------------------------------------------------
//   
TInt CMccResourcePool::FindNetworkResourceByRtpStreamId( 
    const TUint32 aMccSessionId,
    const TRtpId& /*aRtpStreamId*/,
    MDataSource** /*aSource*/, 
    MDataSink** /*aSink*/ )
    {
    TBool found( EFalse );/*
    for ( TInt i = 0; i < iEndpoints.Count() && !found; i++ )
        {
        CMccResourceItem* item = iEndpoints[ i ];
        if ( item->IsSource() && 
             item->Source()->DataSourceType() == KMccRtpSourceUid )
            {
            CMccRtpDataSource* rtpSource = 
                static_cast<CMccRtpDataSource*>( item->Source() );
                
            if ( rtpSource->HandleByStreamId( aRtpStreamId ) )
                {
                *aSource = item->Source();
                found = ETrue;
                }
            }
        else if ( item->IsSink() && 
                  item->Sink()->DataSinkType() == KMccRtpSinkUid )
            {
            CMccRtpDataSink* rtpSink = 
                static_cast<CMccRtpDataSink*>( item->Sink() );
                
            if ( rtpSink->HandleByStreamId( aRtpStreamId ) )
                {
                *aSink = item->Sink();
                found = ETrue;
                }
            }
        else
            {
            }
        }
        */
    return ( found ? KErrNone : KErrNotFound );
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::FindNetworkResourceByRtpSsrc
// -----------------------------------------------------------------------------
//   
TInt CMccResourcePool::FindNetworkResourceByRtpSsrc( 
    const TRtpSSRC& /*aRtpSsrc*/,
    MDataSource** /*aSource*/, 
    MDataSink** /*aSink*/ )
    {
    TBool found( EFalse );
    /*
    for ( TInt i = 0; i < iEndpoints.Count() && !found; i++ )
        {
        CMccResourceItem* item = iEndpoints[ i ];
        if ( item->IsSource() && 
             item->Source()->DataSourceType() == KMccRtpSourceUid )
            {
            CMccRtpDataSource* rtpSource = 
                static_cast<CMccRtpDataSource*>( item->Source() );
                
            if ( rtpSource->HandleBySsrc( aRtpSsrc ) )
                {
                *aSource = item->Source();
                found = ETrue;
                }
            }
        else if ( item->IsSink() && 
                  item->Sink()->DataSinkType() == KMccRtpSinkUid )
            {
            CMccRtpDataSink* rtpSink = 
                static_cast<CMccRtpDataSink*>( item->Sink() );
                
            if ( rtpSink->HandleBySsrc( aRtpSsrc ) )
                {
                *aSink = item->Sink();
                found = ETrue;
                }
            }
        else
            {
            }
        }
        */
    return ( found ? KErrNone : KErrNotFound );
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::StoreKeyFrameInfoL
// -----------------------------------------------------------------------------
//  
void CMccResourcePool::StoreKeyFrameInfoL( TUint32 /*aEndpointId*/, CMMFBuffer& /*aKeyFrame*/ )
    {
    }
                
// -----------------------------------------------------------------------------
// CMccResourcePool::IsKeyFrame
// -----------------------------------------------------------------------------
//  
TBool CMccResourcePool::IsKeyFrame( TUint32 /*aEndpointId*/, CMMFBuffer& /*aBuffer*/ )
    {
    return EFalse;
    }
	
// -----------------------------------------------------------------------------
// CMccResourcePool::FindResourceContainer
// -----------------------------------------------------------------------------
//    
CMccResourceContainer* CMccResourcePool::FindResourceContainer( 
    TUint32 /*aStreamId*/ )
    {
    /*
    for ( TInt i = 0; i < iResources.Count(); i++ )
        {
        if ( iResources[ i ]->StreamId() == aStreamId )
            {
            return iResources[ i ];
            }
        }
        */
    return 0;
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::FindResourceItem
// -----------------------------------------------------------------------------
//    
CMccResourceItem* CMccResourcePool::FindResourceItem( TUint32 /*aEndpointId*/ )
    {
    /*
    for ( TInt i = 0; i < iEndpoints.Count(); i++ )
        {
        if ( iEndpoints[ i ]->EndpointId() == aEndpointId )
            {
            return iEndpoints[ i ];
            }
        }
        */
    return 0;
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::ClearCandidates
// -----------------------------------------------------------------------------
//    
void CMccResourcePool::ClearCandidates()
    {/*
    iSourceCandidate = 0;
    iSourceCandidateEndpointId = 0;
    iSinkCandidate = 0;
    iSinkCandidateEndpointId = 0;
    */
    }
    
// -----------------------------------------------------------------------------
// CMccResourcePool::HandleCameraResourcesL
// -----------------------------------------------------------------------------
// 
void CMccResourcePool::HandleCameraResourcesL()
    {
    // If any video source is active, camera resources are needed.
    //
    /*
    CMccVideoSource* needsCamera = 0;
    for ( TInt i = 0; i < iEndpoints.Count() && !needsCamera; i++ )
        {
        if ( iEndpoints[ i ]->IsSource() && 
             iEndpoints[ i ]->Source()->DataSourceType() == KMccVideoSourceUid )
            {
            needsCamera = iEndpoints[ i ]->IsResourceActive() ?
                static_cast<CMccVideoSource*>( iEndpoints[ i ]->Source() ) : 0;
            }
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
        */
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::GetEndpoint
// -----------------------------------------------------------------------------
//
TInt CMccResourcePool::GetEndpoint( 
    CMccResourceItem& /*aItem*/,
    TUid /*aResourceUid*/,
    MDataSource** /*aSource*/, 
    MDataSink** /*aSink*/ )
    {  /* 
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
        }*/
    return KErrNotFound;
    } 

// -----------------------------------------------------------------------------
// CMccResourcePool::CheckStandbyPropertiesL
// -----------------------------------------------------------------------------
//     
void CMccResourcePool::CheckStandbyPropertiesL( 
    const TMccResourceParams& /*aParams*/, 
    MDataSink& /*aSink*/, 
    MDataSource& /*aSource*/ )
    {/*
    __CONTROLLER( "CMccResourcePool::CheckStandbyPropertiesL" )
    
    if ( aParams.iIsStandby )
        {
        // Standby is allowed only for audio downlink
        __ASSERT_ALWAYS( aSink.DataSinkType() == KUidMmfAudioOutput &&
                         aSource.DataSourceType() == KMccRtpSourceUid,
                         User::Leave( KErrArgument ) );   
        }
    
    __CONTROLLER( "CMccResourcePool::CheckStandbyPropertiesL, exit" )*/
    }
    
// -----------------------------------------------------------------------------
// CMccResourcePool::CheckResourceAvailabilityL
// -----------------------------------------------------------------------------
//     
void CMccResourcePool::CheckResourceAvailabilityL( 
    MDataSink& /*aSink*/, 
    MDataSource& /*aSource*/ )
    { /*
    __CONTROLLER( "CMccResourcePool::CheckResourceAvailabilityL" )
   
    // Only one instance of the media recorder is allowed, having more
    // causes some serious system unstability. 
    // TBD: multiple videosource instances could be allowed if "resource not
    // available", "resource released", etc. events would be passed to the 
    // client and only one active videosource would be allowed. 
    if ( aSource.DataSourceType() == KMccVideoSourceUid )
        {
        for ( TInt i = 0; i < iEndpoints.Count(); i++ )
            {
            if ( iEndpoints[ i ]->IsSource() && 
                 iEndpoints[ i ]->Source()->DataSourceType() == KMccVideoSourceUid )
                {
                __ASSERT_ALWAYS( iEndpoints[ i ]->Source() == &aSource, 
                                 User::Leave( KErrAccessDenied ) );
                }
            }
        }
    
    __CONTROLLER( "CMccResourcePool::CheckResourceAvailabilityL, exit" )
   */ 
   }

// -----------------------------------------------------------------------------
// CMccResourcePool::TimerManager
// -----------------------------------------------------------------------------
// 
MMccTimerManager& CMccResourcePool::TimerManager()
    {
    TInt k( 0 );
    return reinterpret_cast<MMccTimerManager&>( k );
    }
 
// -----------------------------------------------------------------------------
// CMccResourcePool::EnableViewFinderL
// -----------------------------------------------------------------------------
// 
void CMccResourcePool::EnableViewFinderL( TMccVideoSinkSetting& /*aSetting*/ )
    {
    
    }

// -----------------------------------------------------------------------------
// CMccResourcePool::DisableViewFinderL
// -----------------------------------------------------------------------------
// 
void CMccResourcePool::DisableViewFinderL()
    {
    
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
   // iCameraHandler = CMccCameraHandler::NewL();
    }

