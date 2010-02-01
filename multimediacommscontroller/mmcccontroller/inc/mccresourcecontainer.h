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




#ifndef MCCRESOURCECONTAINER_H
#define MCCRESOURCECONTAINER_H

//  INCLUDES
#include <e32base.h>

#include "mccresources.h"

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class CMccResourceItem;

// CLASS DECLARATION

/**
*
*/
class CMccResourceContainer : public CBase
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CMccResourceContainer* NewLC( const TMccResourceParams& aParams,
                                             RPointerArray<CMccResourceItem>& aAllEndpoints );
        
        /**
        * Two-phased constructor.
        */
        static CMccResourceContainer* NewL( const TMccResourceParams& aParams,
                                            RPointerArray<CMccResourceItem>& aAllEndpoints  );
        
        /**
        * Destructor.
        */
        virtual ~CMccResourceContainer();
        
    public: // New functions
    
        void AddResourceItemL( const TMccResourceParams& aUser, CMccResourceItem* aItem );
    
        CMccResourceItem* FindResourceItem( TUint32 aEndpointId );
        
        TUint32 LinkId() const;
        
        TUint32 StreamId() const;
        
        void SetResourceStateL( TUint32 aEndpointId, 
                                MMccResources::TResourceState aState,
                                TBool& aControlNetworkResources,
                                RArray<TUint32>& aControlledEndpoints,
                                RArray<TUint32>& aUncontrolledEndpoints );
        
        TBool IsStandBy();
        
        void SetStandbyControl( TBool aStandbyControl );
        
        TBool StandbyControl() const;
        
        TBool DtmfControl() const;
        
        CMccResourceItem& FetchStandbyEndpointL();
        
        void SetEnableRtcp( TBool aEnableRtcp );
        
        TBool EnableRtcp() const;
        
        void HandleEndpointRemoval( TUint32 aEndpointId = 0 );

    public: // Functions from base classes
              
    protected:  // New functions

    protected:  // Functions from base classes

    private:
    
        CMccResourceContainer( const TMccResourceParams& aParams,
                               RPointerArray<CMccResourceItem>& aAllEndpoints );


    private:
    
        void DeleteEndpoint( TUint32 aEndpointId );
        
    public:     // Data
    
    protected:  // Data

    private:    // Data
    
        TMccResourceParams iParams;
        
        TBool iStandbyControl;
        
        TBool iStreamInUserInititatedMode;
    
        RPointerArray<CMccResourceItem> iOwnEndpoints;
        
        RPointerArray<CMccResourceItem>& iAllEndpoints;
        
        TBool iEnableRtcp;
        
    public:     // Friend classes

    protected:  // Friend classes

    private:    // Friend classes
    #ifdef TEST_EUNIT
        friend class UT_CMccResourceContainer;
    #endif

    };

#endif      // MCCRESOURCECONTAINER_H   
            
// End of File
