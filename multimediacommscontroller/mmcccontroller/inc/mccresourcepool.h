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




#ifndef MCCRESOURCEPOOL_H
#define MCCRESOURCEPOOL_H

//  INCLUDES
#include <e32base.h>

#include "mccresources.h"

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class CMccResourceContainer;
class CMccResourceItem;
class MDataSink;
class MDataSource;
class CMccCameraHandler;
class CMccTimerManager;

// CLASS DECLARATION

/**
*
*/
class CMccResourcePool : public CBase, public MMccResources
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CMccResourcePool* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CMccResourcePool();

    public: // New functions
            
        void ReuseL( TUint32 aEndpointId );
        
        void ReferenceCountL( TUint32 aEndpointId, TInt& aRefCount );
        
        MDataSource* SourceCandidate();
        
        MDataSink* SinkCandidate();
        
        void SetEnableRtcpL( TUint32 aStreamId, TBool aEnableRtcp );
        
        TBool EnableRtcp( TUint32 aStreamId );
        
        /**
        * Gets existing camerahandler. 
        */
        CMccCameraHandler& CameraHandlerL( TBool aCurrentCamera = ETrue, 
                                           TInt aCameraIndex = 0 );

        /**
        * Gets new camerahandler. 
        * @return CMccCameraHandler; ownership is transferred
        */
        CMccCameraHandler* CreateNewCameraHandlerL( TInt aCameraIndex = 0 );
        
        void UpdateResourceL( TUint32 aEndpointId, const TDesC8& aVal );
                                           
    public: // Functions from MMccResources
    
        void ReserveSinkL( MDataSink* aSink, TUint32 aEndpointId );
        
        void ReserveSourceL( MDataSource* aSource, TUint32 aEndpointId );
        
        void PrepareEndpointRemovalL( 
                TUint32 aEndpointId, 
                RArray<TMccResourceParams>& aAffectedStreams );
        
        void RemoveEndpointL( TUint32 aEndpointId );
        
        void ReserveResourcesL( const TMccResourceParams& aParams );
                                
        void FreeResources( TUint32 aStreamId );
                                 
        TBool IsResourceUsageAllowed( TUint32 aStreamId );                         

        void SetUserInitiationModeL( TUint32 aStreamId, 
                                     TBool iIsUserInititated );
                                     
        void SetResourceStateL( TUint32 aStreamId, 
                                TUint32 aEndpointId, 
                                TResourceState aState,
                                TBool& aControlNetworkResources,
                                RArray<TUint32>& aControlledEndpoints,
                                RArray<TUint32>& aUncontrolledEndpoints );
                                             
        
        void ResourceStateSetL();
    
        TInt EventReceived( TMccEvent& aEvent, 
                            TBool& aDiscardEvent,
                            TBool& aEffectiveStandby,
                            TMccEvent** aAdditionalEvent );
        
        const RPointerArray<MDataSink>& Sinks( TBool aOnlyInternals );
        
        const RPointerArray<MDataSource>& Sources( TBool aOnlyInternals );
        
        CMccMultiplexer* MultiplexerL( MDataSource* aSource );
        
        TInt FindResource( TUint32 aLinkId,
                           TUint32 aEndpointId, 
                           TUid aResourceUid, 
                           MDataSource** aSource, 
                           MDataSink** aSink );

        TInt FindNetworkResourceByRtpStreamId( 
                                  const TUint32 aMccSessionId,
                                  const TRtpId& aRtpStreamId,
                                  MDataSource** aSource, 
                                  MDataSink** aSink );
                
        TInt FindNetworkResourceByRtpSsrc( 
                                  const TRtpSSRC& aRtpSsrc,
                                  MDataSource** aSource, 
                                  MDataSink** aSink );
       
        MMccTimerManager& TimerManager();
        
        void StoreKeyFrameInfoL( TUint32 aEndpointId, CMMFBuffer& aKeyFrame );
        
        TBool IsKeyFrame( TUint32 aEndpointId, CMMFBuffer& aBuffer );
        
        void EnableViewFinderL( TMccVideoSinkSetting& aSetting );
        
        void DisableViewFinderL();
                                   
    protected:  // New functions

    protected:  // Functions from base classes
    
    private:
    
        CMccResourceContainer* FindResourceContainer( TUint32 aStreamId );
        
        CMccResourceItem* FindResourceItem( TUint32 aEndpointId );
                
        void ClearCandidates();
                                                      
        void HandleCameraResourcesL();
        
        TInt GetEndpoint( CMccResourceItem& aItem,
                          TUid aResourceUid,
                          MDataSource** aSource, 
                          MDataSink** aSink );

        void CheckStandbyPropertiesL( const TMccResourceParams& aParams, 
                                      MDataSink& aSink, 
                                      MDataSource& aSource );
                                      
        void CheckResourceAvailabilityL( MDataSink& aSink, MDataSource& aSource );
        
        TMccEvent* StandbyLogicL( TMccEvent& aEvent, 
                                  TBool& aDiscardEvent,
                                  TBool& aEffectiveStandby );
    
        void StandbyInactivityL( CMccResourceContainer& aContainer,
                                 TMccEvent& aEvent, 
                                 TBool& aDiscardEvent,
                                 TBool& aEffectiveStandby );
                                 
        TMccEvent* StandbyActivityL( CMccResourceContainer& aContainer,
                                     TMccEvent& aEvent, 
                                     TBool& aDiscardEvent,
                                     TBool& aEffectiveStandby );
    
    private:

        /**
        * C++ default constructor.
        */
        CMccResourcePool();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    public:     // Data
    
    protected:  // Data

    private:    // Data
    
        // Not owned
        MDataSink* iSinkCandidate;
        
        TUint32 iSinkCandidateEndpointId;
        
        // Not owned
        MDataSource* iSourceCandidate;
        
        TUint32 iSourceCandidateEndpointId;
        
        RPointerArray<CMccResourceContainer> iResources;
        
        RPointerArray<CMccResourceItem> iEndpoints;
        
        RPointerArray<MDataSink> iTempSinks;
        
        RPointerArray<MDataSource> iTempSources;

        // Camera handle shared by all components
		CMccCameraHandler* iCameraHandler;
		
		CMccTimerManager* iTimerManager;
		
	private:    // Friend classes

        #ifdef EUNIT_TEST
            friend class UT_CMccController; 
            friend class UT_CMccResourcePool; 
            friend class UT_CMccQosController;
        #endif
		
        
    public:     // Friend classes

    protected:  // Friend classes

    private:    // Friend classes

    };

#endif      // MCCRESOURCEPOOL_H   
            
// End of File
