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
#include "mccresourcepool_STUB.h"
#include "mccmultiplexer.h"
#include "mcctimermanager.h"
#include <mmf/server/mmfbuffer.h>

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
// CMccResourcePoolStub::NewL
// -----------------------------------------------------------------------------
//
CMccResourcePoolStub* CMccResourcePoolStub::NewL()
    {
    CMccResourcePoolStub* self = new ( ELeave ) CMccResourcePoolStub;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMccResourcePoolStub::~CMccResourcePoolStub
// -----------------------------------------------------------------------------
//        
CMccResourcePoolStub::~CMccResourcePoolStub()
    {
    delete iMultiplexer;
    delete iTimerManager;
    }

// -----------------------------------------------------------------------------
// CMccResourcePoolStub::ReserveSinkL
// -----------------------------------------------------------------------------
//
void CMccResourcePoolStub::ReserveSinkL( MDataSink* /*aSink*/, TUint32 /*aEndpointId*/ )
    {
    }

// -----------------------------------------------------------------------------
// CMccResourcePoolStub::ReserveSourceL
// -----------------------------------------------------------------------------
//
void CMccResourcePoolStub::ReserveSourceL( MDataSource* /*aSource*/, TUint32 /*aEndpointId*/ )
    {
    }

// -----------------------------------------------------------------------------
// CMccResourcePoolStub::PrepareEndpointRemovalL
// -----------------------------------------------------------------------------
//
void CMccResourcePoolStub::PrepareEndpointRemovalL( 
    TUint32 /*aEndpointId*/, 
    RArray<TMccResourceParams>& /*aAffectedStreams*/ )
    {
    }

// -----------------------------------------------------------------------------
// CMccResourcePoolStub::RemoveEndpointL
// -----------------------------------------------------------------------------
//        
void CMccResourcePoolStub::RemoveEndpointL( TUint32 /*aEndpointId*/ )
    {
    }

// -----------------------------------------------------------------------------
// CMccResourcePoolStub::ReserveResourcesL
// -----------------------------------------------------------------------------
//      
void CMccResourcePoolStub::ReserveResourcesL( 
     const TMccResourceParams& /*aParams*/ )
    {
    }

// -----------------------------------------------------------------------------
// CMccResourcePoolStub::FreeResources
// -----------------------------------------------------------------------------
//      
void CMccResourcePoolStub::FreeResources( TUint32 /*aStreamId*/ )
    {
    }

// -----------------------------------------------------------------------------
// CMccResourcePoolStub::IsResourceUsageAllowed
// -----------------------------------------------------------------------------
//                               
TBool CMccResourcePoolStub::IsResourceUsageAllowed( TUint32 /*aStreamId*/ )
    {
    return ETrue;
    }                    

// -----------------------------------------------------------------------------
// CMccResourcePoolStub::SetResourceStateL
// -----------------------------------------------------------------------------
//                                    
void CMccResourcePoolStub::SetResourceStateL( 
    TUint32 /*aStreamId*/,
    TUint32 aEndpointId, 
    MMccResources::TResourceState /*aState*/,
    TBool& aControlNetworkResources,
    RArray<TUint32>& aControlledEndpoints,
    RArray<TUint32>& aUncontrolledEndpoints )
    {
    if ( iControlled )
        {
        aControlNetworkResources = ETrue;
        aControlledEndpoints.AppendL( aEndpointId );
        }
    else
        {
        aControlNetworkResources = EFalse;
        aUncontrolledEndpoints.AppendL( aEndpointId );
        }
    }

// -----------------------------------------------------------------------------
// CMccResourcePoolStub::ResourceStateSetL
// -----------------------------------------------------------------------------
//     
void CMccResourcePoolStub::ResourceStateSetL()
    {
    }

// -----------------------------------------------------------------------------
// CMccResourcePoolStub::EventReceived
// -----------------------------------------------------------------------------
//                                             

TInt CMccResourcePoolStub::EventReceived( 
    TMccEvent& /*aEvent*/, 
    TBool& aDiscardEvent,
    TBool& aEffectiveStandby,
    TMccEvent** /*aAdditionalEvent*/ )
    {
    aDiscardEvent = EFalse;
    aEffectiveStandby = EFalse;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccResourcePoolStub::Sinks
// -----------------------------------------------------------------------------
//
const RPointerArray<MDataSink>& CMccResourcePoolStub::Sinks( TBool /*aOnlyInternals*/ )
    {
    return iTempSinks;
    }

// -----------------------------------------------------------------------------
// CMccResourcePoolStub::Sources
// -----------------------------------------------------------------------------
//        
const RPointerArray<MDataSource>& CMccResourcePoolStub::Sources( TBool /*aOnlyInternals*/ )
    {
    return iTempSources;
    }

// -----------------------------------------------------------------------------
// CMccResourcePoolStub::MultiplexerL
// -----------------------------------------------------------------------------
//   
CMccMultiplexer* CMccResourcePoolStub::MultiplexerL( MDataSource* aSource )
    {
    if ( !iMultiplexer )
        {
        iMultiplexer = CMccMultiplexer::NewL( aSource );
        }
    return iMultiplexer;
    }

// -----------------------------------------------------------------------------
// CMccResourcePoolStub::FindResource
// -----------------------------------------------------------------------------
//
TInt CMccResourcePoolStub::FindResource( TUint32 /*aLinkId*/,
                   TUint32 /*aEndpointId*/, 
                   TUid /*aResourceUid*/, 
                   MDataSource** /*aSource*/, 
                   MDataSink** /*aSink*/ )
    {
    return KErrNotFound;
    }

// -----------------------------------------------------------------------------
// CMccResourcePoolStub::FindNetworkResourceByRtpStreamId
// -----------------------------------------------------------------------------
//    
TInt CMccResourcePoolStub::FindNetworkResourceByRtpStreamId( 
                                  const TUint32 aMccSessionId,
                                  const TRtpId& /*aRtpStreamId*/,
                                  MDataSource** /*aSource*/, 
                                  MDataSink** /*aSink*/ )
    {
    return KErrNotFound;
    }

// -----------------------------------------------------------------------------
// CMccResourcePoolStub::FindNetworkResourceByRtpSsrc
// -----------------------------------------------------------------------------
//
TInt CMccResourcePoolStub::FindNetworkResourceByRtpSsrc( 
                                  const TRtpSSRC& /*aRtpSsrc*/,
                                  MDataSource** /*aSource*/, 
                                  MDataSink** /*aSink*/ )
    {
    return KErrNotFound;
    }

// -----------------------------------------------------------------------------
// CMccResourcePoolStub::StoreKeyFrameInfoL
// -----------------------------------------------------------------------------
//  
void CMccResourcePoolStub::StoreKeyFrameInfoL( TUint32 /*aEndpointId*/, CMMFBuffer& aKeyFrame )
    {
    iKeyFrameTimeStamp = aKeyFrame.TimeToPlay().Int64();
    }
                
// -----------------------------------------------------------------------------
// CMccResourcePoolStub::IsKeyFrame
// -----------------------------------------------------------------------------
//  
TBool CMccResourcePoolStub::IsKeyFrame( TUint32 aEndpointId, CMMFBuffer& aBuffer )
    {
    return ( iKeyFrameTimeStamp == aBuffer.TimeToPlay().Int64() );
    }

// -----------------------------------------------------------------------------
// CMccResourcePoolStub::EnableViewFinderL
// -----------------------------------------------------------------------------
// 
void CMccResourcePoolStub::EnableViewFinderL( TMccVideoSinkSetting& /*aSetting*/ )
    {
    
    }

// -----------------------------------------------------------------------------
// CMccResourcePoolStub::DisableViewFinderL
// -----------------------------------------------------------------------------
// 
void CMccResourcePoolStub::DisableViewFinderL()
    {
    
    }
        
// -----------------------------------------------------------------------------
// CMccResourcePoolStub::CMccResourcePoolStub
// -----------------------------------------------------------------------------
//      
CMccResourcePoolStub::CMccResourcePoolStub() :
    iControlled( ETrue )
    {
    }

// -----------------------------------------------------------------------------
// CMccResourcePoolStub::ConstructL
// -----------------------------------------------------------------------------
// 
void CMccResourcePoolStub::ConstructL()
    {
    iTimerManager = CMccTimerManager::NewL();
    }

// -----------------------------------------------------------------------------
// CMccResourcePoolStub::ConstructL
// -----------------------------------------------------------------------------
//
MMccTimerManager& CMccResourcePoolStub::TimerManager()
    {
    return *iTimerManager;
    }

