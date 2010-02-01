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
* Description:    Represents ICE valid list.
*
*/




#ifndef C_ICEVALIDLIST_H
#define C_ICEVALIDLIST_H

#include <e32base.h>
#include "natfwinternaldefs.h"    //UNIT_TEST

class CNATFWCandidatePair;

/**
 *  Represents valid list for a stream collection.
 *
 *  @lib icecandidatehandler.lib
 *  @since S60 v3.2
 */
class CIceValidList : public CBase
    {

    UNIT_TEST( UT_CIceValidList )
    
public:

    /**
     * Two-phased constructor.
     * @param aCollectionId     Identifier for the stream collection for which
     *                          this valid list is created.
     */
    static CIceValidList* NewL( TUint aCollectionId );
    
    /**
     * Two-phased constructor.
     * @param aCollectionId     Identifier for the stream collection for which
     *                          this valid list is created.
     */
    static CIceValidList* NewLC( TUint aCollectionId );
    
    /**
     * Destructor.
     */
    virtual ~CIceValidList();

    /**
     * Returns streamcollection identifier.
     *
     * @since   S60 v3.2
     * return   Streamcollection identifier
     */
    TUint StreamCollectionId() const;
    
    /**
     * Adds valid candidate pair for a stream collection.
     *
     * @since   S60 v3.2
     */
    void AddValidPairL( const CNATFWCandidatePair& aPair );
    
    /**
     * Returns ETrue if specified pair is in the valid list.
     *
     * @since   S60 v3.2
     * @param   aPair           The pair to search in valid list
     * return   ETrue if valid list has specified pair
     */
    TBool HasPair( const CNATFWCandidatePair& aPair ) const;
    
    /**
     * Returns ETrue if all specified media components have candidate 
     * pair in the valid list.
     *
     * @since   S60 v3.2
     * @param   aComponentIds   Media component identifiers
     * return   ETrue if all components have valid pair
     */
    TBool HasPairForComponents( RArray<TUint>& aComponentIds ) const;
    
    /**
     * Sets pair nominated.
     *
     * @since   S60 v3.2
     * @param   aPair           The candidate pair
     */
    void SetPairNominatedL( const CNATFWCandidatePair& aPair );
    
    /**
     * Returns ETrue if all specified media components have nominated 
     * candidate pair.
     *
     * @since   S60 v3.2
     * @param   aComponentIds   Media component identifiers
     * return   ETrue if all components have nominated pair
     */
    TBool NominatedPairsExist( RArray<TUint>& aComponentIds ) const;

    /**
     * Returns highest priority nominated pair for specified media component.
     *
     * @since   S60 v3.2
     * @param   aComponentId    Media component identifier
     * return   Selected pair or NULL if not found
     */
    const CNATFWCandidatePair* SelectedPair( TUint aComponentId ) const;
    
    /**
     * Returns highest priority pair for requested media component.
     *
     * @since   S60 v3.2
     * @param   aComponentId    Media component identifier
     * return   Highest priority pair
     */
    const CNATFWCandidatePair* HighestPriorityPair( TUint aComponentId ) const;
    
    /**
     * Flushes valid list due to ICE restart for a media stream.
     *
     * @since   S60 v3.2
     */
    void FlushValidList();
    
private:

    CIceValidList( TUint aCollectionId );

private: // data

    /**
     * Stream collection identifier.
     */
    TUint iCollectionId;
    
    /**
     * Valid list for a stream collection.
     */
    RPointerArray<CNATFWCandidatePair> iValidList;

    };

#endif // C_ICEVALIDLIST_H
