/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef MCCRESOURCEITERATORS_H
#define MCCRESOURCEITERATORS_H

#include <e32std.h>

class CMccResourceContainer;
class CMccResourceItem;
class TMccResourceParams;

enum TMccIteratorMatchType
    {
    ExactMatch,
    ExactReverseMatch    
    };
        
        
/**
 * Resource container iterator.
 *
 * @lib 
 */
class TMccResourceContainerIterator
    {
    public: // Constructors & Destructor

        TMccResourceContainerIterator( const RPointerArray<CMccResourceContainer>& aContainers );

        TMccResourceContainerIterator( const RPointerArray<CMccResourceContainer>& aContainers,
                                       TMccResourceParams& aResourceParams );
        
        TMccResourceContainerIterator( const RPointerArray<CMccResourceContainer>& aContainers,
                                       TUint32 aEndpointId );
        
    public: // new functions
                            
         TBool Next( CMccResourceContainer*& aCandidate,
                     TMccIteratorMatchType aMatchType = ExactMatch );
        
         void Reset();
         
         TInt Current();
         
         TBool IsEof(); 
         
         TInt Delete( RPointerArray<CMccResourceContainer>& aContainers );

    private: // NOT owned data

        const RPointerArray<CMccResourceContainer>& iContainers;

        TInt iCurrentIndex;
        
        TMccResourceParams* iResourceParams;
        
        TUint32 iEndpointId;
    
    };

/**
 * Resource item iterator.
 *
 * @lib 
 */
class TMccResourceItemIterator
    {
    public: // Constructors & Destructor

        TMccResourceItemIterator( 
            const RPointerArray<CMccResourceItem>& aItems,
            TBool aOnlyInternals = EFalse,
            TBool aOnlySinks = EFalse,
            TBool aOnlySources = EFalse );

        TMccResourceItemIterator( 
            const RPointerArray<CMccResourceItem>& aItems, 
            TUid aUid );
            
        TMccResourceItemIterator( 
            const RPointerArray<CMccResourceItem>& aItems,
            TUid aUid,
            TMccResourceParams& aResourceParams );
        
        TMccResourceItemIterator( 
            const RPointerArray<CMccResourceItem>& aItems,
            TMccResourceParams& aResourceParams );
        
        TMccResourceItemIterator( 
            const RPointerArray<CMccResourceItem>& aItems,
            TUint32 aEndpointId );
        
    public: // new functions
                            
         TBool Next( CMccResourceItem*& aCandidate,
                     TMccIteratorMatchType aMatchType = ExactMatch );
        
         void Reset();
         
         TInt Current();
         
         TBool IsEof(); 

    private: // NOT owned data

        const RPointerArray<CMccResourceItem>& iItems;

        TInt iCurrentIndex;
        
        TBool iOnlyInternals;
        
        TBool iOnlySinks;
        
        TBool iOnlySources;
        
        TUid iUid;
        
        TMccResourceParams* iResourceParams;
        
        TUint32 iEndpointId;
    
    };

#endif //MCCRESOURCEITERATORS_H

