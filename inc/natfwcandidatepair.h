/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Candidate pair
*
*/




#ifndef CNATFWCANDIDATEPAIR_H
#define CNATFWCANDIDATEPAIR_H

#include <e32base.h>
#include "natfwconnectivityapidefs.h"

class CNATFWCandidate;
class CNATFWCredentials;

/**
 *  A pairing containing a local candidate and a remote candidate.
 *  
 *  Used with the ICE protocol when making NAT connectivity checks.
 *  
 *  @lib  natconfw.lib
 *  @since S60 v3.2
 */
class CNATFWCandidatePair : public CBase
    {

public:

    /**
     * Two-phased constructor.
     */
    IMPORT_C static CNATFWCandidatePair* NewL();

    /**
     * Two-phased constructor.
     */
    IMPORT_C static CNATFWCandidatePair* NewLC();
    
    /**
     * Two-phased copy constructor.
     * @param   aPair               The pair to copy
     */
    IMPORT_C  static CNATFWCandidatePair* NewL(
        const CNATFWCandidatePair& aPair );

    /**
     * Two-phased copy constructor.
     * @param   aPair               The pair to copy
     */
    IMPORT_C  static CNATFWCandidatePair* NewLC(
        const CNATFWCandidatePair& aPair );
    
    /**
     * Two-phased constructor.
     * @param   aLocalCand          Local candidate
     * @param   aRemoteCand         Remote candidate
     */
    IMPORT_C  static CNATFWCandidatePair* NewL(
        const CNATFWCandidate& aLocalCand,
        const CNATFWCandidate& aRemoteCand );
    
    /**
     * Two-phased constructor.
     * @param   aLocalCand          Local candidate
     * @param   aRemoteCand         Remote candidate
     */
    IMPORT_C  static CNATFWCandidatePair* NewLC(
        const CNATFWCandidate& aLocalCand,
        const CNATFWCandidate& aRemoteCand );
    
    /**
     * Destructor.
     */
    virtual ~CNATFWCandidatePair();

    /**
     * Returns foundation of the candidate pair.
     *
     * @since   S60 v3.2
     * @return  Foundation of candidate pair
     */
    IMPORT_C const TDesC8& Foundation() const;

    /**
     * Sets local candidate.
     *
     * @since   S60 v3.2
     * @param   aCandidate          Local candidate to set
     */
    IMPORT_C void SetLocalCandidateL( const CNATFWCandidate& aCandidate );

    /**
     * Gets local candidate.
     *
     * @since   S60 v3.2
     * @return  Local candidate
     */
    IMPORT_C const CNATFWCandidate& LocalCandidate() const;

    /**
     * Sets remote candidate.
     *
     * @since   S60 v3.2
     * @param   aCandidate          Remote candidate
     */
    IMPORT_C void SetRemoteCandidateL( const CNATFWCandidate& aCandidate );

    /**
     * Gets remote candidate.
     *
     * @since   S60 v3.2
     * @return  Remote candidate
     */
    IMPORT_C const CNATFWCandidate& RemoteCandidate() const;

    /**
     * Sets pair priority.
     *
     * @since   S60 v3.2
     * @param   aPriority new pair priority
     */
    IMPORT_C void SetPriority( const TUint64 aPriority );

    /**
     * Gets pair priority.
     *
     * @since   S60 v3.2
     * @return  Pair priority
     */
    IMPORT_C TUint64 Priority() const;

    /**
     * Sets the candidate pair as selected.
     *
     * @since   S60 v3.2
     * @param   aSelected           ETrue if selected
     */
    IMPORT_C void SetSelected( TBool aSelected );
    
    /**
     * Gets pair selected status (true/false).
     *
     * @since   S60 v3.2
     * @return  Whether candidate pair is selected by ICE algorithm
     */
    IMPORT_C TBool Selected() const;
    
    /**
     * Implements TLinearOrder (descending order) using priority 
     * in comparison.
     *
     * @since   S60 v3.2
     */
    IMPORT_C static TInt PriorityOrder( 
        const CNATFWCandidatePair& aA, const CNATFWCandidatePair& aB );
    
    /**
     * Implements TIdentityRelation regarding all fields.
     *
     * @since   S60 v3.2
     * @param   aItem1              The first item for comparison
     * @param   aItem2              The second item for comparison
     * @return  ETrue if candidate pairs match
     */
    IMPORT_C static TBool Compare( 
        const CNATFWCandidatePair& aItem1, 
        const CNATFWCandidatePair& aItem2 );
    
    /**
     * Implements TIdentityRelation regarding transport addresses.
     *
     * @since   S60 v3.2
     * @param   aItem1              The first item for comparison
     * @param   aItem2              The second item for comparison
     * @return  ETrue if candidate pairs match
     */
    IMPORT_C static TBool MatchAddresses( 
        const CNATFWCandidatePair& aItem1, 
        const CNATFWCandidatePair& aItem2 );
    
    /**
     * Makes a copy of candidate pair.
     *
     * @since   S60 v3.2
     * @param   aPair               The pair to copy
     */
    IMPORT_C void CopyL( const CNATFWCandidatePair& aPair );

    IMPORT_C TBool operator==(const CNATFWCandidatePair& aCandidate) const;
    
    IMPORT_C TBool operator!=(const CNATFWCandidatePair& aCandidate) const;
    
private:

    CNATFWCandidatePair();
    
    void ConstructL();
    
    void ConstructL( const CNATFWCandidatePair& aPair );
    
    void ConstructL(
        const CNATFWCandidate& aLocalCand,
        const CNATFWCandidate& aRemoteCand );
    
    void UpdateFoundationL();
    
protected: // data

    /**
     * Priority of candidate pair
     */
    TUint64 iPairPriority;

    /**
     * Foundation of candidate pair
     * Own.
     */
    RBuf8 iFoundation;

    /**
     * Local candidate
     * Own.
     */
    CNATFWCandidate* iLocalCand;

    /**
     * Remote candidate
     * Own.
     */
    CNATFWCandidate* iRemoteCand;

    /**
     * Indicates whether the pair is selected
     */
    TBool iSelected;
    
    };


#endif // CNATFWCANDIDATEPAIR_H
