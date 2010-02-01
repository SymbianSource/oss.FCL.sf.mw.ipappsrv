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
* Description:    Candidate store
*
*/




#ifndef NATCOMTESTOBSERVER_H
#define NATCOMTESTOBSERVER_H

#include <e32std.h>

class CNATFWCandidate;
class CNATFWCandidatePair; 

class MNatComTestObserver
    {
    
public:
    
    enum TNatComTestEvent
        {
        ESessionCreated         = 1,
        EStreamCreated          = 2,
        ESetIdentification      = 3,
        ELocalCandidateFound    = 4,
        EFetchingCompleted      = 5,
        ESetOperationMode       = 6,
        ENewCandPairFound       = 7,
        EConnChecksCompleted    = 8,
        ERunning                = 9,
        ETestError              = 10,
        ETestEnd                = 11
        };
        
    virtual void Notify( TNatComTestEvent aEvent, TInt aError ) = 0;

    virtual TInt LocalCandidates( const RPointerArray<CNATFWCandidate>& aLocalCandidates ) = 0;
    
    virtual TInt RemoteCandidates( RPointerArray<CNATFWCandidate>& aRemoteCandidates ) = 0;
    
    virtual void DoPause( TRequestStatus& aStatus ) = 0;
    
    virtual TInt CandidatePairs( const RPointerArray<CNATFWCandidatePair>& iCandidatePairs ) = 0;
    };

#endif // NATCOMTESTOBSERVER_H