/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef TMCESRVSTREAMITERATOR_H
#define TMCESRVSTREAMITERATOR_H

#include <e32std.h>

#include "mcesrvstream.h"
#include "mcesrv.h"
class CMceComMediaStream;
class CMceComEndpoint;

const TBool KMceComUseProxyMatch = ETrue;
const TBool KMceComNoProxyMatch = EFalse;

/**
 * 
 *
 * @lib 
 */
class TMceSrvStreamIterator
    {


public: // enums

    enum TDirection
        {
        EZero,
        EAny,
        ESendOrReceive,
        ESend,
        EReceive,
        ESendOrLocal,
        EReceiveOrLocal,
        ELocal
        };
    

    enum TStreamStateMatchType
        {
        ExactMatch,
        ExactReverseMatch,
        GreaterOrEqualMatch,
        GreaterOrEqualReverseMatch      
        };
        
public: // Constructors & Destructor


    /**
     * C++ default constructor.
     */
    TMceSrvStreamIterator( const RPointerArray<CMceSrvStream>& aStreams );

    /**
     * C++ default constructor.
     */
    TMceSrvStreamIterator( const RPointerArray<CMceSrvStream>& aStreams,
                           TDirection aDirection );

    /**
     * C++ default constructor.
     */
    TMceSrvStreamIterator( const RPointerArray<CMceSrvStream>& aStreams,
                           const CMceComMediaStream& aStream );

    /**
     * C++ default constructor.
     */
    TMceSrvStreamIterator( const RPointerArray<CMceSrvStream>& aStreams,
                           const CMceComEndpoint& aEndpoint,
                           TBool aUseProxyMatch = KMceComNoProxyMatch,
                           TDirection aDirection = EZero,
                           const CMceComCodec* aCodec = NULL );

    /**
     * C++ default constructor.
     */
    TMceSrvStreamIterator( const RPointerArray<CMceSrvStream>& aStreams,
                           const CMceComCodec& aCodec );


    
    
public: // new functions

    //next based on filter
    TBool Next( CMceSrvStream*& aCandidate,
                TStreamStateMatchType aMatchType = ExactMatch );

    //next based on filter and matches recipient
    TBool Next( CMceSrvStream*& aCandidate,
                TMceMccComEvent& aRecipient,
                TStreamStateMatchType aMatchType = ExactMatch );

    //next based on filter and matches clone
    TBool Next( CMceSrvStream*& aCandidate,
                CMceSrvStream& aClone,
                TStreamStateMatchType aMatchType = ExactMatch );

    

    //iterates intersection of this and aSet
    //return value is intersected stream from this set 
    //and aIntersectCandidate is intersected stream from aSet
    TBool NextIntersect( CMceSrvStream*& aCandidate,
                          TMceSrvStreamIterator& aSet,
                          CMceSrvStream*& aIntersectCandidate );

    //iterates intersection of this and aSet
    TBool NextComplement( CMceSrvStream*& aCandidate,
                           TMceSrvStreamIterator& aSet );
                                  


    //next based on filter and aState, if aNegation is ETrue, which all
    //streams which are NOT in state aState
     TBool Next( CMceSrvStream*& aCandidate,
                CMceSrvStream::TState aState, 
                TStreamStateMatchType aMatchType = ExactMatch );
    
    void Reset();
    TInt Current();
    TBool IsEof();
    TInt Remove( RPointerArray<CMceSrvStream>& aStreams );    
    TDirection Direction();
    
    TBool DoDirectionMatch( CMceSrvStream& aSrvStream );

private: // NOT owned data


    const RPointerArray<CMceSrvStream>& iStreams;
    TDirection iDirection;
    const CMceComMediaStream* iStream;
    const CMceComEndpoint* iEndpoint;
    const CMceComCodec* iCodec;

    TInt iCurrentIndex;
    
    TBool iUseProxyMatch;
    
	//definitions for unit testing
	MCEMM_UT_DEFINITIONS
    
    };



#endif //TMCESRVSTREAMITERATOR_H
