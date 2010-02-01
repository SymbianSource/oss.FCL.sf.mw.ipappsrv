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
* Description:    Holds various session related data.
*
*/





#ifndef CNATFWCANDIDATESTORAGE_H
#define CNATFWCANDIDATESTORAGE_H

#include <e32base.h>
#include "natfwcredentials.h"
#include "natfwconnectivityapidefs.h"
#include "natfwcandidate.h"
#include "natfwinternaldefs.h"    //UNIT_TEST

class CIceStreamCollection;
class MNATFWPluginObserver;
class TInetAddr;

/**
 *  Stores all session related data like candidates and credentials.
 *
 *  Eliminates redundant candidates and generates foundations for them.
 *
 *  @lib icecandidatehandler.dll
 *  @since S60 v3.2
 */
class CIceSessionData : public CBase
    {

    UNIT_TEST( UT_CIceSessionData )
    
public:

    /**
     * Two-phased constructor.
     * @param aDomain                   Domain used
     * @param aIapId                    Internet access point identifier
     */
    static CIceSessionData* NewL( 
        const TDesC8& aDomain, TUint32 aIapId );

    /**
     * Two-phased constructor.
     * @param aDomain                   Domain used
     * @param aIapId                    Internet access point identifier
     */
    static CIceSessionData* NewLC( 
        const TDesC8& aDomain, TUint32 aIapId );

    /**
     * Destructor.
     */
    virtual ~CIceSessionData();

   /**
     * Adds peer reflexive candidate.
     *
     * @since   S60 v3.2
     * @param   aPeerCandidate          Peer reflexive candidate
     * @param   aIsRemoteCand           Whether candidate is remote one
     */
    void AddPeerReflexiveCandidateL( CNATFWCandidate* aPeerCandidate, 
        TBool aIsRemoteCand );

    /**
     * Finds local candidate based on transport address info.
     *
     * @since   S60 v3.2
     * @param   aTransportAddr          Transport address
     * @return  Found candidate or NULL if not found
     */
    const CNATFWCandidate* FindLocalCandidate( 
        const TInetAddr& aTransportAddr ) const;

    /**
     * Finds local candidate.
     *
     * @since   S60 v3.2
     * @param   aStreamCollectionId     Stream collection identifier
     * @param   aComponentId            Component identifier
     * @param   aType                   Candidate type to found
     * @return  Found candidate or NULL if not found
     */
    const CNATFWCandidate* FindLocalCandidate( 
        TUint aStreamCollectionId,
        TUint aComponentId,
        CNATFWCandidate::TCandidateType aType ) const;

    /**
     * Finds remote candidate based on transport address info.
     *
     * @since   S60 v3.2
     * @param   aTransportAddr          Transport address
     * @return  Found candidate or NULL if not found 
     */
    const CNATFWCandidate* FindRemoteCandidate( 
        const TInetAddr& aTransportAddr ) const;

    /**
     * Gets local candidates.
     *
     * @since S60 v3.2
     * @param aStreamCollectionId       Stream collection identifier
     * @param aStreamId                 Stream identifier
     * @param aLocalCandidates          Found candidates. Caller is 
     *      responsible of deleting the contents of the array.
     */
    void GetLocalCandidates(
        const TUint aStreamCollectionId,
        const TUint aStreamId,
        RPointerArray<CNATFWCandidate>& aLocalCandidates );

    /**
     * Gets local candidates.
     *
     * @since S60 v3.2
     * @param aStreamCollectionId       Stream collection identifier
     * @param aLocalCandidates          Found candidates. Caller is 
     *      responsible of deleting the contents of the array.
     */
    void GetLocalCandidates(
        const TUint aStreamCollectionId,
        RPointerArray<CNATFWCandidate>& aLocalCandidates );
    
    /**
     * Gets local candidates.
     *
     * @since S60 v3.2
     * @param aLocalCandidates          Found candidates. Caller is 
     *      responsible of deleting the contents of the array.
     */
    void GetLocalCandidates(
        RPointerArray<CNATFWCandidate>& aLocalCandidates );
    
    /**
     * Deletes stream collection.
     *
     * @since   S60 3.2
     * @param   aStreamCollectionId     The ID identifying collection
     */
    void DeleteCollection( TUint aStreamCollectionId );
                    
    /**
     * Adds new credentials for stream collection.
     *
     * @since   S60 3.2
     * @param   aCredentials         Credentials for stream collection
     */
    void AddCredentialsL( 
        const CNATFWCredentials& aCredentials );

    /**
     * Returns all credentials used in a session.
     *
     * @since   S60 3.2
     * @return  Credentials list
     */
    const RPointerArray<CNATFWCredentials>& Credentials() const;
    
    /**
     * Returns credentials for specified stream and utilization direction.
     *
     * @since   S60 3.2
     * @param   aStreamId               The stream identifier
     * @param   aDirection              The utilization direction
     * @return  Credentials
     */
    const CNATFWCredentials* Credentials( 
        TUint aStreamId,
        CNATFWCredentials::TUtilizationDirection aDirection ) const;

    /**
     * Adds a remote candidate.
     *
     * @since   S60 3.2
     * @param   aRemoteCandidate        The candidate to add
     */
    void AddRemoteCandidateL( CNATFWCandidate* aRemoteCandidate );
    
    /**
     * Gets remote candidates.
     *
     * @since   S60 3.2
     * @param   aStreamCollectionId     Returned candidates have this stream
     *          collection id
     * @param   aComponentId            Returned candidates have this component id
     * @param   aRemoteCandidates candidates are returned in this array.
     *          Caller is responsible of deleting the contents of the
     *          array.
     */
    void GetRemoteCandidatesL( 
        TUint aStreamCollectionId,
        TUint aComponentId,
        RPointerArray<CNATFWCandidate>& aRemoteCandidates ); 
                    
    /**
     * Adds remote candidates.
     *
     * @param   aRemoteCandidates       Remote candidates to add
     */
    void AddRemoteCandidatesL( 
        RPointerArray<CNATFWCandidate>& aRemoteCandidates );

    /**
     * Gets remote candidates.
     *
     * @since   S60 3.2
     * @param   aStreamCollectionId     Collection where to find candidates
     * @param   aRemoteCandidates       Returned candidates.
     *          Caller is responsible of deleting the contents of the
     *          array.
     */
    void GetRemoteCandidatesL( const TUint aStreamCollectionId,
        RPointerArray<CNATFWCandidate>& aRemoteCandidates );

    /**
     * Adds a local candidate.
     *
     * @since  S60 3.2
     * @param  aLocalCandidate          The candidate to add
     * @return ETrue if candidate was redundant
     */
    TBool AddLocalCandidateL( CNATFWCandidate* aLocalCandidate );
    
    /**
     * Gets identifiers for all stream collections in a session.
     * 
     * @since   S60 3.2
     * @param   aStreamCollIds          Collection ids returned
     */                                        
    void GetStreamCollectionIdsL( 
        RArray<TUint>& aStreamCollIds ) const;
    
    /**
     * Creates a new stream collection.
     *
     * @since   S60 3.2
     * @param   aStreamCollectionId     The identifier for new collection
     */
    void CreateCollectionL( TUint aStreamCollectionId );
     
    /**
     * Returns collection identifier.
     *
     * @since   S60 3.2
     * @param   aStreamCollectionId     The identifier for collection
     */
    CIceStreamCollection* CollectionL( TUint aStreamCollectionId );

    /**
     * Finds collection where specified media component belongs.
     *
     * @since   S60 3.2
     * @param   aStreamId               The media component
     * @param   aCollectionId           On return collection id
     * @return  System wide error code
     */
    TInt GetCollectionIdForComponent( 
        TUint aStreamId, TUint& aCollectionId );
    
    /**
     * Cleanups all data related to the specified stream.
     *
     * @since   S60 v3.2
     * @param   aStreamId               The stream identifier
     */
    void CleanupStreamData( TUint aStreamId );
     
    /**
     * Removes all local candidates in a session.
     *
     * @since   S60 v3.2
     */
    void RemoveLocalCandidates();

    /**
     * Removes all remote candidates in a session.
     *
     * @since   S60 v3.2
     */
    void RemoveRemoteCandidates();

    /**
     * Sets new role.
     *
     * @since   S60 v3.2
     * @param   aRole                   The role
     */    
    void SetRole( TNATFWIceRole aRole );

    /**
     * Returns current role.
     *
     * @since   S60 v3.2
     * @return  Current role
     */
    TNATFWIceRole Role() const;
    
    /**
     * Sets new tie-breaker value.
     *
     * @since   S60 v3.2
     * @param   aTieBreaker             The tie-breaker value
     */
    void SetTieBreaker( TUint64 aTieBreaker );

    /**
     * Returns current tie-breaker value.
     *
     * @since   S60 v3.2
     * @return  Current tie-breaker
     */
    TUint64 TieBreaker() const;
    
    /**
     * Returns domain.
     *
     * @since   S60 v3.2
     * @return  Domain
     */
    const TDesC8& Domain() const;
    
    /**
     * Returns internet access point identifier.
     *
     * @since   S60 v3.2
     * @return  Internet access point identifier
     */
    TUint32 IapId() const;

    /**
     * Sets value of the TA timer in milliseconds.
     *
     * @since   S60 v3.2
     * @param   aTaTimerValue           The value for the timer
     */
    void SetTaTimerValue( TUint aTaTimerValue );

    /**
     * Returns value of the TA timer in milliseconds.
     *
     * @since   S60 v3.2
     * @return  TA timer value
     */
    TUint TaTimerValue() const;
    
private:
        

    CIceSessionData( TUint32 aIapId );

    void ConstructL( const TDesC8& aDomain );

    TBool IsRedundantCandidate( const CNATFWCandidate& aLocalCandidate,
        const RPointerArray<CNATFWCandidate>& aCandidateArray ) const;

    void AssignFoundationForCandidateL( CNATFWCandidate& aLocalCandidate );

    CIceStreamCollection* FindStreamCollectionById( 
        TUint aCollectionId );

    const TDesC8& GenerateRandomIdentifier();
    
    void UpdateUserNamesL( CNATFWCredentials& aCredentials );

private: // data

    /**
     * Tie-breaker used for role conflict determination in the session.
     */
    TUint64 iTieBreaker;
    
    /**
     * The current role.
     */
    TNATFWIceRole iRole;
    
    /**
     * Domain in which session is created.
     * Own.
     */
    HBufC8* iDomain;
    
    /**
     * Internet access point identifier.
     */
    TUint32 iIapId;
    
    /**
     * Stream collections.
     * Own.
     */
    RPointerArray<CIceStreamCollection> iStreamCollections;
    
    /**
     * Credentials.
     * Own.
     */
    RPointerArray<CNATFWCredentials> iCredentialsList;
    
    /**
     * Local candidates resolved so far.
     * Own.
     */
    RPointerArray<CNATFWCandidate> iLocalCandidateList;
    
    /**
     * Remote candidates known.
     * Own.
     */
    RPointerArray<CNATFWCandidate> iRemoteCandidateList;
    
    /**
     * Temporary variable for credentials generation.
     * Own.
     */
    RBuf8 iTempIdentifier;

    /**
     * Value for the TA timer.
     */
    TUint iTaTimerValue;
    };


#endif // CNATFWCANDIDATESTORAGE_H
