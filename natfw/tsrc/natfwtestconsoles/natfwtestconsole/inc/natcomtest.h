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
* Description:    ICE Communication tests
*
*/




#ifndef NATCOMTEST_H
#define NATCOMTEST_H

#include <e32base.h>

#include "natfwconnectivityapidefs.h"
#include "mnatfwconnectivityobserver.h"
#include "natcomteststate.h"

class CNATFWConnectivityApi;
class MNatComTestObserver;
class CNATFWCredentials;
class CNATFWCandidatePair;
class CNATFWCandidate;
class CConsoleBase;
class CMccMediaStream;
class CMccLink;
class CMccSession;
class MNATFWSocketMediaConnWrapper;
class TInetAddr;
class CDesC8Array;

enum TComTestType
    {
    EIceComTest = 0,
    EStunComTest,
    ETurnComTest
    };


class CNatComTest :
    public CActive,
    public MNATFWConnectivityObserver
    {
public:

    static CNatComTest* NewL( const TDesC8& aDomain, TUint32 aIapId,
        MNatComTestObserver& aObserver );
        
    static CNatComTest* NewLC( const TDesC8& aDomain, TUint32 aIapId,
        MNatComTestObserver& aObserver );
        
    virtual ~CNatComTest();
   
    TInt StartComTest( TUint aProtocol, TComTestType aTestType,
        TBool aUseRtcp );
    
    TInt OnlyFetchCandidates( TUint aProtocol );
    
    TInt StartUsingIce( TUint aProtocol,
        TNATFWIceRole aRole, TBool aUseRtcp = EFalse );
    
    TInt StoreLocalCondidates( TUint aProtocol, TBool aUseRtcp = EFalse );
        
    TInt StoreLocalCandidate( TUint aProtocol,
            TComTestType aTestType, TBool aUseRtcp );
    
    void StopL();
    
protected:

    TInt Start( TUint aProtocol, TNATFWIceRole aRole );
    
// from CActive

    void RunL();
    
    void DoCancel();
    
// from base class MNATFWConnectivityObserver

    void EventOccured( TUint aSessionId, TUint aStreamId,
        TNATFWConnectivityEvent aEvent, TInt aError, TAny* aEventData );

private:

    CNatComTest( TUint32 aIapId, MNatComTestObserver& aObserver );
    
    void ConstructL( const TDesC8& aDomain );
    
    void ActiveRequestCompleted( TInt aError );
    
    const TInetAddr RemoteCandidateAddrByStreamIdL( TUint aStreamId );
    
    void GetCandidatesAssosiatedToL( TInt aStreamCollectionId,
        TInt aComponentId, const RPointerArray<CNATFWCandidate>& aSource,
        RPointerArray<CNATFWCandidate>& aTarget );
    
    void GetCandidatesAssosiatedToL( TInt aMediaComponentId,
        const RPointerArray<CNATFWCandidate>& aSource,
        RPointerArray<CNATFWCandidate>& aTarget );
    
    const CNATFWCandidate& IceLocalCandidateByStreamIdL( TUint aStreamId );
    
    const CNATFWCandidate& LocalCandidateByStreamIdL( TUint aStreamId );
    
    void SetToCandidatesL( TUint aSessionId, TInt aStreamId, 
        RPointerArray<CNATFWCandidate>& aTarget );
    
    void SetTeardownSequence( TInt aTestType, TBool aUseRtcp );
    
    
    void CreateSession();
    void LoadPlugin();
    void CreateRtpStream();
    void CreateRtcpStream();
    void SetIdentification();
    void FetchRtpCandidates();
    void FetchRtcpCandidates();
    void FetchRtpCandidate();
    void FetchRtcpCandidate();
    void SetOperationMode();
    void PerformConnectivityChecks();
    void CreateRtpWrapper();
    void CreateRtcpWrapper();
    void PrepareMccLink();
    void PrepareMccLinkWithRtcp();
    void SetWrapperIncomingAddr();
    void SetReceivingState( MNATFWSocketMediaConnWrapper& aWrapper,
        TUint aStreamId, TNATFWStreamingState aStreamingState );
    void SetSendingState( MNATFWSocketMediaConnWrapper& aWrapper,
        TUint aStreamId, TNATFWStreamingState aStreamingState,
        TInt aTestType, TBool aRtcp );
    void CreateMediaUpStream();
    void CreateMediaDownStream();
    void StartMediaUpStream();
    void StartMediaDownStream();
    TInt SetSendingStatus( TUint aStreamId,
        TNATFWStreamingState aState, const TInetAddr& aDestAddr );
    TInt SetReceivingStatus( TUint aStreamId, TNATFWStreamingState aState );
    void TeardownL();
    
private: // data
 
    TUint32 iIapId;
    MNatComTestObserver& iObserver;
    TUint iProtocol;
    CNATFWConnectivityApi* iNat;
    HBufC8* iDomain;
    TUint iSessionId;
    TUint iRtpStreamId;
    TUint iRtcpStreamId;
    RPointerArray<CNATFWCandidate> iLocalCandidates;
    RPointerArray<CNATFWCandidatePair> iCandidatePairs;
    TNATFWIceRole iMode;
    CConsoleBase* iConsole;
    TBool iOnlyCandidates;
    
    CMccSession* iSession;
    TInt32 iLinkId;
    TInt32 iUpStreamId;
    TInt32 iDownStreamId;
    
    TNatComState iState;
    TNatComTestState iStateSequence;
    TNatComTestState iTeardownSequence;
    
    MNATFWSocketMediaConnWrapper* iRtpMediaWrapper;
    MNATFWSocketMediaConnWrapper* iRtcpMediaWrapper;
    
    RArray<TInt> iStreamIds;
    CDesC8Array* iPlugins;
    };
#endif // NATCOMTEST_H
