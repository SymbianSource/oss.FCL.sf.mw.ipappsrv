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




#ifndef MCCRESOURCEITEM_H
#define MCCRESOURCEITEM_H

//  INCLUDES
#include <e32base.h>

#include "mccresources.h"

// CONSTANTS
const TInt KMccMaxNumKeyFrames = 5;

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class MDataSink;
class MDataSource;
class CMccMultiplexer;
class CMMFBuffer;
class CMccCameraHandler;

// CLASS DECLARATION

/**
*
*/
class CMccResourceItem : public CBase
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CMccResourceItem* NewLC( TUint32 aCurrentUserStreamId,
                                        MDataSink* aSink, 
                                        MDataSource* aSource, 
                                        TUint32 aEndpointId,
                                        TBool aIsMultiplexer = EFalse );
        
        /**
        * Two-phased constructor.
        */
        static CMccResourceItem* NewL( TUint32 aCurrentUserStreamId,
                                       MDataSink* aSink, 
                                       MDataSource* aSource, 
                                       TUint32 aEndpointId,
                                       TBool aIsMultiplexer = EFalse );
        /**
        * Destructor.
        */
        virtual ~CMccResourceItem();
        
    public: // New functions
    
        TInt IncreaseRefCount( const TMccResourceParams& aUser );
        
        TBool DecreaseRefCount( const TMccResourceParams& aUser ); 
        
        TInt RefCount() const;
    
        TUint32 EndpointId() const;
        
        TBool IsNetworkResource() const;
        
        TBool IsResourceActive() const;
        
        TBool IsSharedResource() const;
        
        TBool NeedsCamera( CMccCameraHandler& aCameraHandler );
        
        TBool SetResourceStateL( TUint32 aStreamId, 
                                 MMccResources::TResourceState aState,
                                 TBool aStandbyControl,
                                 TBool aDtmfControl );
                                 
        TBool IsSink() const;
        
        TBool IsSource() const;
        
        TBool IsInternal() const;
        
        MDataSource* Source();
        
        MDataSink* Sink();
        
        CMccMultiplexer* MultiplexerL();
        
        CMccMultiplexer* GetMultiplexerL();
        
        static TBool UserSessionMatch( const TMccResourceParams& aUser1, 
                                       const TMccResourceParams& aUser2 );
        
        static TBool UserMatch( const TMccResourceParams& aUser1, 
                                const TMccResourceParams& aUser2 );
                                
        static TBool UserMatchNotStrict( const TMccResourceParams& aUser1, 
                                         const TMccResourceParams& aUser2 );
        
        static TBool UserMatchActive( const TMccResourceParams& aUser1, 
                                      const TMccResourceParams& aUser2 );
                                                        
        void UsersInfoL( RArray<TMccResourceParams>& aUsers );
        
        MMccResources::TResourceState CurrentState() const;
        
        TBool Match( const TMccResourceParams& aUser ) const;
        
        TBool MatchSession( const TUint32 aSessionId ) const;

        void StoreKeyFrameInfoL( CMMFBuffer& aKeyFrame );
                
        TBool IsKeyFrame( CMMFBuffer& aBuffer );
                                

    public: // Functions from base classes
              
    protected:  // New functions

    protected:  // Functions from base classes
    
    private:
    
        TBool IsCurrentUser( TUint32 aStreamId ) const;
    
        TBool NonSharableResource( TUid aUid ) const;
        
        TBool IsInternal( TUint32 aUid ) const;
        
        TBool IsStandbyResource() const;
        
        void SpecialStateChangeL( TUint32 aStreamId,
                                  MMccResources::TResourceState aState,
                                  TBool aStandbyControl,
                                  TBool aDtmfControl,
                                  TBool& aEffectiveStateChange );
                                  
        TBool UserStopL( TUint32 aStreamId );

    private:
    
        CMccResourceItem( TUint32 aCurrentUserStreamId, 
                          TUint32 aEndpointId,
                          TBool aIsMultiplexer );
        
        void ConstructL( MDataSink* aSink, MDataSource* aSource );

    public:     // Data
    
    protected:  // Data
        
    private: // Internal class
        
        class TMccKeyFrameInfo
            {
        public:
            TUint iSeqNum;
            TInt64 iTimestamp;
            TUint iSize;
            };

    private:    // Data
    
        RArray<TMccResourceParams> iUsers;
        
        TUint32 iCurrentUserStreamId;

        MDataSink* iSink;

        MDataSource* iSource;
        
        TUint32 iEndpointId;
        
        MMccResources::TResourceState iState;
        
        CMccResourceItem* iMultiplexerItem;
        
        CMccMultiplexer* iMultiplexer;
        
        TBool iIsMultiplexer;
        
        TBool iPausedByUser;
        
        RArray<TMccKeyFrameInfo> iKeyFrameInfo;
        
    public:     // Friend classes

    protected:  // Friend classes

    private:    // Friend classes
        #ifdef TEST_EUNIT
            friend class UT_CMccResourceItem;
            friend class UT_CMccResourceContainer;
            friend class UT_CMccResourcePool;
        #endif
    };

#endif      // MCCRESOURCEITEM_H   
            
// End of File
