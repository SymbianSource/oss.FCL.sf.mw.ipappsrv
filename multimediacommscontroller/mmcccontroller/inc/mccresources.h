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




#ifndef MCCRESOURCES_H
#define MCCRESOURCES_H

//  INCLUDES
#include <e32base.h>
#include "rtpdef.h"

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class MDataSink;
class MDataSource;
class TMccEvent;
class CMccMultiplexer;
class MMccTimerManager;
class CMMFBuffer;
class TMccVideoSinkSetting;

// CLASS DECLARATION

class TMccResourceParams
    {
public:

    TMccResourceParams( TUint32 aSessionId, 
                        TUint32 aLinkId, 
                        TUint32 aStreamId,
                        TUint32 aIapId,
                        TBool aIsStandby,
                        TInt aStreamType ) :
        iSessionId( aSessionId ),
        iLinkId( aLinkId ),
        iStreamId( aStreamId ),
        iIapId( aIapId ),
        iIsStandby( aIsStandby ),
        iStreamType( aStreamType ),
        iIsStopped( EFalse )
        {
        }
        
public:

    TUint32 iSessionId;
    TUint32 iLinkId;
    TUint32 iStreamId; 
    TUint32 iIapId;
    TBool iIsStandby; 
    TInt iStreamType;
    TBool iIsStopped;
      
    };
    
/**
*
*/
class MMccResources
    {
    public:
    
        enum TResourceState
            {
            EConstructed,
            EPrepared,
            EStarted,
            EResourcePaused,
            EResourceStopped
            };
    
    public:
    
        /**
        * Stores sink as candidate for next stream resource reservation.
        * @param aSink
        * @param aEndpointId id of the sink
        */
        virtual void ReserveSinkL( MDataSink* aSink, TUint32 aEndpointId ) = 0;
        
        /**
        * Stores source as candidate for next stream resource reservation.
        * @param aSource
        * @param aEndpointId id of the sink
        */
        virtual void ReserveSourceL( MDataSource* aSource, TUint32 aEndpointId ) = 0;
        
        /**
        * Prepares resources for removing certain endpoint.
        * @param aEndpointId id of the endpoint to be removed
        * @param aAffectedStreams information about streams which are using
        *        the endpoint which is in removal process
        */
        virtual void PrepareEndpointRemovalL( TUint32 aEndpointId, 
                                      RArray<TMccResourceParams>& aAffectedStreams ) = 0;
        
        /**
        * Removes endpoint from resources.
        * @param aEndpointId
        */
        virtual void RemoveEndpointL( TUint32 aEndpointId ) = 0;
        
        /**
        * Reserves some stream specific resources if needed. May depend
        * on state of the stream (e.g. camera resources are needed when live video
        * upstream is in playing state).
        * @param aParams resource parameters
        */                              
        virtual void ReserveResourcesL( const TMccResourceParams& aParams ) = 0;
        
        /**
        * Free resources of certain stream.
        * @param aStreamId
        */                                
        virtual void FreeResources( TUint32 aStreamId ) = 0;
         
        /**
        * Checks whether stream can use resources it needs.
        * @param aStreamId
        * @return ETrue if resources can be used
        */                         
        virtual TBool IsResourceUsageAllowed( TUint32 aStreamId ) = 0;                         
         
        /**
        * Sets resource state.
        * @param aStreamId
        * @param aEndpointId
        * @param aState
        * @param aControlNetworkResources on return tells whether resource
        *        needs network resources
        * @param aControlledEndpoint on return contains list of endpoint
        *        ids of which changed state because of this command
        * @param aUncontrolledEndpoints on return contains list of endpoint
        *        ids of which didn't change state although command was pointed
        *        for those. 
        */                                    
        virtual void SetResourceStateL( TUint32 aStreamId,
                                        TUint32 aEndpointId, 
                                        TResourceState aState,
                                        TBool& aControlNetworkResources,
                                        RArray<TUint32>& aControlledEndpoints,
                                        RArray<TUint32>& aUncontrolledEndpoints ) = 0;
        
        /**
        * Should be called by user after state change has been completed
        * successfully.
        */                                    
        virtual void ResourceStateSetL() = 0;
        
        /**
        * Check if resources are affected becuase of event.
        * @param aDiscardEvent on return ETrue if event should be discarded
        * @param aEffectiveStandby on return ETrue if event was effective
        *        standby event
        * @param aAdditionalEvent on return contains NULL or additional event
        *        which should be handled as well. Ownership of event is
        *        transferred.
        * @return error code
        */
        virtual TInt EventReceived( TMccEvent& aEvent, 
                                    TBool& aDiscardEvent,
                                    TBool& aEffectiveStandby,
                                    TMccEvent** aAdditionalEvent ) = 0;
        
        /**
        * Get sinks.
        * @param aOnlyInternals ETrue if only mcc internal sinks are fetched.
        * @return list of sinks
        */
        virtual const RPointerArray<MDataSink>& Sinks( TBool aOnlyInternals ) = 0;
        
        /**
        * Get sources.
        * @param aOnlyInternals ETrue if only mcc internal sources are fetched.
        * @return list of sources
        */
        virtual const RPointerArray<MDataSource>& Sources( TBool aOnlyInternals ) = 0;
        
        /**
        * Get multiplexer for specific source
        * @param aSource
        * @return NULL or multiplexer, ownership is not transferred
        */
        virtual CMccMultiplexer* MultiplexerL( MDataSource* aSource ) = 0;
        
        /**
        * Find resource
        * @param aLinkId search term
        * @param aEndpointId search term
        * @param aResourceUid search term
        * @param aSource on return NULL or source, ownership is not transferred
        * @param aSink on return NULL or sink, ownership is not transferred
        * @return KErrNone if resource was found
        */
        virtual TInt FindResource( TUint32 aLinkId,
                                   TUint32 aEndpointId, 
                                   TUid aResourceUid, 
                                   MDataSource** aSource, 
                                   MDataSink** aSink ) = 0;
        
        /**
        * Find network resource based on rtp stream id
        * @param aMccSessionId MCC session id
        * @param aRtpStreamId search term
        * @param aSource on return NULL or source, ownership is not transferred
        * @param aSink on return NULL or sink, ownership is not transferred
        * @return KErrNone if resource was found
        */                           
        virtual TInt FindNetworkResourceByRtpStreamId( 
                                  const TUint32 aMccSessionId,
                                  const TRtpId& aRtpStreamId,
                                  MDataSource** aSource, 
                                  MDataSink** aSink ) = 0;
        
        /**
        * Find network resource based on rtp ssrc
        * @param aRtpSsrc search term
        * @param aSource on return NULL or source, ownership is not transferred
        * @param aSink on return NULL or sink, ownership is not transferred
        * @return KErrNone if resource was found
        */        
        virtual TInt FindNetworkResourceByRtpSsrc( 
                                  const TRtpSSRC& aRtpSsrc,
                                  MDataSource** aSource, 
                                  MDataSink** aSink ) = 0;
        
        /**
        * Timer services.
        * @return timer
        */                          
        virtual MMccTimerManager& TimerManager() = 0;
        
        /**
        * Store information about keyframe, can be helpful when some other endpoint
        * needs to know whether some buffer is keyframe as that information cannot
        * be carried in CMMFBuffer.
        * @param aEndpointId
        * @param aKeyFrame
        */  
        virtual void StoreKeyFrameInfoL( TUint32 aEndpointId, CMMFBuffer& aKeyFrame ) = 0;
        
        /**
        * Check whether buffer is keyframe.
        * @param aEndpointId
        * @param aBuffer
        * @return ETrue if buffer contains keyframe
        */
        virtual TBool IsKeyFrame( TUint32 aEndpointId, CMMFBuffer& aBuffer ) = 0;
        
        /**
        * Enables viewfinder
        */
        virtual void EnableViewFinderL( TMccVideoSinkSetting& aSetting ) = 0;
        
        /**
        * Disables viewfinder
        */
        virtual void DisableViewFinderL() = 0;

    };

#endif      // MCCRESOURCES_H   
            
// End of File
