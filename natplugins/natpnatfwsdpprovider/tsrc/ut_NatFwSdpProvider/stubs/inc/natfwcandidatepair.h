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
* Description:  Candidate pair
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
    static CNATFWCandidatePair* NewL();

    /**
     * Two-phased constructor.
     */
    static CNATFWCandidatePair* NewLC();
    
    /**
     * Two-phased copy constructor.
     * @param   aPair               The pair to copy
     */
     static CNATFWCandidatePair* NewL(
        const CNATFWCandidatePair& aPair );

    /**
     * Two-phased copy constructor.
     * @param   aPair               The pair to copy
     */
     static CNATFWCandidatePair* NewLC(
        const CNATFWCandidatePair& aPair );
    
    /**
     * Two-phased constructor.
     * @param   aLocalCand          Local candidate
     * @param   aRemoteCand         Remote candidate
     */
     static CNATFWCandidatePair* NewL(
        const CNATFWCandidate& aLocalCand,
        const CNATFWCandidate& aRemoteCand );
    
    /**
     * Two-phased constructor.
     * @param   aLocalCand          Local candidate
     * @param   aRemoteCand         Remote candidate
     */
     static CNATFWCandidatePair* NewLC(
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
    const TDesC8& Foundation() const;

    /**
     * Sets local candidate.
     *
     * @since   S60 v3.2
     * @param   aCandidate          Local candidate to set
     */
    void SetLocalCandidateL( const CNATFWCandidate& aCandidate );

    /**
     * Gets local candidate.
     *
     * @since   S60 v3.2
     * @return  Local candidate
     */
    const CNATFWCandidate& LocalCandidate() const;

    /**
     * Sets remote candidate.
     *
     * @since   S60 v3.2
     * @param   aCandidate          Remote candidate
     */
    void SetRemoteCandidateL( const CNATFWCandidate& aCandidate );

    /**
     * Gets remote candidate.
     *
     * @since   S60 v3.2
     * @return  Remote candidate
     */
    const CNATFWCandidate& RemoteCandidate() const;

    /**
     * Sets pair priority.
     *
     * @since   S60 v3.2
     * @param   aPriority new pair priority
     */
    void SetPriority( const TUint64 aPriority );

    /**
     * Gets pair priority.
     *
     * @since   S60 v3.2
     * @return  Pair priority
     */
    TUint64 Priority() const;

    /**
     * Sets the candidate pair as selected.
     *
     * @since   S60 v3.2
     * @param   aSelected           ETrue if selected
     */
    void SetSelected( TBool aSelected );
    
    /**
     * Gets pair selected status (true/false).
     *
     * @since   S60 v3.2
     * @return  Whether candidate pair is selected by ICE algorithm
     */
    TBool Selected() const;
    
    /**
     * Implements TLinearOrder (descending order) using priority 
     * in comparison.
     *
     * @since   S60 v3.2
     */
    static TInt PriorityOrder( 
        const CNATFWCandidatePair& aA, const CNATFWCandidatePair& aB );
    
    /**
     * Implements TIdentityRelation regarding all fields.
     *
     * @since   S60 v3.2
     * @param   aItem1              The first item for comparison
     * @param   aItem2              The second item for comparison
     * @return  ETrue if candidate pairs match
     */
    static TBool Compare( 
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
    static TBool MatchAddresses( 
        const CNATFWCandidatePair& aItem1, 
        const CNATFWCandidatePair& aItem2 );
    
    /**
     * Makes a copy of candidate pair.
     *
     * @since   S60 v3.2
     * @param   aPair               The pair to copy
     */
    void CopyL( const CNATFWCandidatePair& aPair );

    TBool operator==(const CNATFWCandidatePair& aCandidate) const;
    
    TBool operator!=(const CNATFWCandidatePair& aCandidate) const;
    
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
