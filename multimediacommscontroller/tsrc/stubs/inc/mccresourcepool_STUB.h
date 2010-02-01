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




#ifndef MCCRESOURCEPOOLSTUB_H
#define MCCRESOURCEPOOLSTUB_H

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
class CMccMultiplexer;
class CMccTimerManager;

// CLASS DECLARATION

/**
*
*/
class CMccResourcePoolStub : public CBase, public MMccResources
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CMccResourcePoolStub* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CMccResourcePoolStub();

    public: // New functions

    public: // Functions from MMccResources
    
        void ReserveSinkL( MDataSink* aSink, TUint32 aEndpointId );
        
        void ReserveSourceL( MDataSource* aSource, TUint32 aEndpointId );
        
        void PrepareEndpointRemovalL( TUint32 aEndpointId, 
                                      RArray<TMccResourceParams>& aAffectedStreams );
        
        void RemoveEndpointL( TUint32 aEndpointId );
        
        void ReserveResourcesL( const TMccResourceParams& aParams );
                                 
        void FreeResources( TUint32 aStreamId );
        
        TBool IsResourceUsageAllowed( TUint32 aStreamId );                         
                                     
        void SetResourceStateL( TUint32 aStreamId,
                                TUint32 aEndpointId, 
                                MMccResources::TResourceState aState,
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

        /**
        * C++ default constructor.
        */
        CMccResourcePoolStub();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    public:     // Data
    
        TBool iControlled;
        
        TInt64 iKeyFrameTimeStamp;
    
    protected:  // Data

    private:    // Data
    
        RPointerArray<MDataSink> iTempSinks;
        
        RPointerArray<MDataSource> iTempSources;
        
        CMccMultiplexer* iMultiplexer;
        
        CMccTimerManager* iTimerManager;
        
    public:     // Friend classes

    protected:  // Friend classes

    private:    // Friend classes

    };

#endif      // MCCRESOURCEPOOLSTUB_H   
            
// End of File
