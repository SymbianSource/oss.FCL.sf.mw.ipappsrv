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
* Description:    Performs connectivity check.
*
*/




#ifndef C_CICECONNECTIVITYCHECK_H
#define C_CICECONNECTIVITYCHECK_H

#include <e32base.h>
#include "ciceconnectionhandler.h"
#include "miceconncheckobserver.h"

class MIceConnCheckListener;
class CNATFWCandidatePair;
class CNATFWCandidate;
class TInetAddr;
class CIceSessionData;

/**
 *  Performs periodic and triggered end-to-end connectivity checks.
 *
 *  Connectivity check consists of STUN-request sending and response
 *  processing.
 *
 *  @lib icecandidatehandler
 *  @since S60 v3.2
 */
class CIceConnectivityCheck 
    : 
    public CBase,
    public MIceConnCheckObserver
    {

    UNIT_TEST( UT_CIceConnectivityCheck )
    UNIT_TEST( UT_CIceCheckList )

public:

    /**  ICE check states */
    enum TIceCheckState
        {
        EIceCheckWaiting        = 1,
        EIceCheckInProgress     = 2,
        EIceCheckSucceed        = 4,
        EIceCheckFailed         = 8,
        EIceCheckFrozen         = 16
        };

    /** ICE check types */
    enum TIceCheckType
        {
        EIceCheckTypePeriodic   = 1,
        EIceCheckTypeTriggered  = 2,
        EIceCheckTypeNomination = 3
        };

    /**
     * Two-phased constructor.
     * @param   aClient             The client issuing check
     * @param   aLocalCand          The local candidate
     * @param   aRemoteCand         The remote candidate
     * @param   aConnHandler        The connection handler
     * @param   aSessionData        The session data
     */
    static CIceConnectivityCheck* NewL( 
        MIceConnCheckListener& aClient,
        const CNATFWCandidate& aLocalCand,
        const CNATFWCandidate& aRemoteCand,
        CIceConnectionHandler& aConnHandler,
        CIceSessionData& aSessionData );

    /**
     * Two-phased constructor.
     * @param   aClient             The client issuing check
     * @param   aLocalCand          The local candidate
     * @param   aRemoteCand         The remote candidate
     * @param   aConnHandler        The connection handler
     * @param   aSessionData        The session data
     */
    static CIceConnectivityCheck* NewLC(
        MIceConnCheckListener& aClient,
        const CNATFWCandidate& aLocalCand,
        const CNATFWCandidate& aRemoteCand,
        CIceConnectionHandler& aConnHandler,
        CIceSessionData& aSessionData );
    
    /** 
     * Destructor.
     */
    virtual ~CIceConnectivityCheck();
    
    /**
     * Compare priorities of two connectivity checks.
     *
     * @since   S60 v3.2
     * @param   aA         The first check for comparison
     * @param   aB         The second check for comparison
     */
    static TInt ComparePriorities( 
        const CIceConnectivityCheck& aA, 
        const CIceConnectivityCheck& aB );

    /**
     * Implements TIdentityRelation regarding transport addresses.
     *
     * @since   S60 v3.2
     * @param   aItem1              The first item for comparison
     * @param   aItem2              The second item for comparison
     * @return  ETrue if checks match
     */
    static TBool MatchAddresses( 
        const CIceConnectivityCheck& aItem1, 
        const CIceConnectivityCheck& aItem2 );

    /** 
     * Initializes the check. After that check is ready to be performed.
     *
     * @since   S60 v3.2
     */
    void Initialize();
    
    /**
     * Executes connectivity check.
     * 
     * @since   S60 v3.2
     * @param   aType           Connectivity check type
     * @param   aPriority       The priority to be included in the check
     * @param   aRtoValue       The retransmission timeout value in ms
     */
    void PerformConnCheckL( TIceCheckType aType, TUint aPriority, 
        TUint aRtoValue );
    
    /** 
     * Resends earlier performed connectivity check in order to nominate pair.
     *
     * @since   S60 v3.2
     * @param   aRtoValue       The retransmission timeout value in ms
     */
    void PerformNominationL( TUint aRtoValue );
    
    /** 
     * Return whether check is nominated.
     *
     * @since   S60 v3.2
     * @return  ETrue if check is nominated
     */
    TBool Nominated() const;
    
    /** 
     * Set information from the remote check received for the same pair.
     * This information is needed for valid pair construction.
     *
     * @since   S60 v3.2
     * @param   aPriority       Priority of the remote check
     * @param   aFavored        Favored status from the remote check
     */
    void SetRemoteCheckInfo( TUint aPriority, TBool aFavored );
    
    /** 
     * Cancels the check.
     *
     * @since   S60 v3.2
     */
    void Cancel();
    
    /** 
     * Return stream identifier where this check is done.
     *
     * @since   S60 v3.2
     * @return  Stream identifier
     */
    TUint StreamId() const;
    
    /** 
     * Return identifier of the component for which this check is for.
     *
     * @since   S60 v3.2
     * @return  Component identifier
     */
    TUint ComponentId() const;
    
    /**
     * Returns stream collection id of the check.
     * 
     * @since   S60 v3.2
     * @return  Stream collection identifier
     */
    TUint StreamCollectionId() const;
    
    /** 
     * Return current state of the connectivity check.
     *
     * @since   S60 v3.2
     * @return  Current state
     */
    TIceCheckState State() const;
    
    /** 
     * Return candidate pair validated with this check.
     *
     * @since   S60 v3.2
     * @return  Validated candidate pair
     */
    const CNATFWCandidatePair* ValidatedPair() const;
    
    /** 
     * Returns candidate pair for which check is created.
     *
     * @since   S60 v3.2
     * @return  Candidate pair
     */
    const CNATFWCandidatePair& CandidatePair() const;

// From base class MIceConnectionObserver

    void ConnCheckCompletedL(
        TInt aCompletionCode,
        const TInetAddr& aLocalAddr,
        const TInetAddr& aPeerAddr,
        const TInetAddr& aMappedAddr );

private:

    CIceConnectivityCheck( 
        MIceConnCheckListener& aClient,
        CIceConnectionHandler& aConnHandler,
        CIceSessionData& aSessionData );

    void ConstructL( 
        const CNATFWCandidate& aLocalCand,
        const CNATFWCandidate& aRemoteCand );
    
    CNATFWCandidatePair* CreateValidatedPairL( 
        const TInetAddr& aMappedAddr );
    
private: // data

    /**
     * Current state of the ICE check.
     */
    TIceCheckState iState;
    
    /**
     * Type of the connectivity check.
     */
    TIceCheckType iType;
    
    /**
     * Priority used with binding request.
     */
    TUint iPriority;
    
    /**
     * Priority from check which triggered this check.
     */
    TUint iRemotePriority;
    
    /** 
     * Indicates is this pair favored by remote party.
     */
    TBool iRemoteFavored;
    
    /**
     * Whether check is nominated.
     */
    TBool iIsNominated;
    
    /**
     * Connection used for connectivity checks.
     */
    TUint iConnectionId;
    
    /**
     * Indicates that resend has been issued.
     */
    TBool iResendInProgress;
    
    /**
     * Client notified about a result of the check.
     * Not own.
     */
    MIceConnCheckListener& iClient;
    
    /**
     * The connection handler.
     * Not own.
     */
    CIceConnectionHandler& iConnHandler;
    
    /**
     * The session data.
     * Not own.
     */
    CIceSessionData& iSessionData;
    
    /**
     * Validated candidate pair.
     * Own.
     */
    CNATFWCandidatePair* iValidatedPair;
    
    /**
     * Pair to test for connectivity.
     * Own.
     */
    CNATFWCandidatePair* iCandidatePair;
    
    };

#endif // C_CICECONNECTIVITYCHECK_H
