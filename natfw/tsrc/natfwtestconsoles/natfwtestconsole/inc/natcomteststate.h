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




#ifndef NATCOMTESTSTATE_H
#define NATCOMTESTSTATE_H

#include <e32cmn.h>

enum TNatComState
    {
    EEnd = 0,
    ECreateSession,
    ELoadPlugin,
    ECreateRtpStream,
    ECreateRtcpStream,
    EFetchRtpCandidates,
    EFetchRtcpCandidates,
    EFetchRtpCandidate,
    EFetchRtcpCandidate,
    EStoreLocalCandidates,
    ESetIdentification,  
    ESetOperationMode,
    EReadRemoteCandidates,
    EPause,
    EConnectivityChecks,
    ECreateRtpWrapper,
    ECreateRtcpWrapper,
    EPrepareMccLink,
    ESetOutgoingAddr,
    ESetWrapperIncomingAddr,
    ECreateMediaUpStream,
    ECreateMediaDownStream,
    EStartMediaUpStream,
    EStartMediaDownStream,
    ESetRtpSendingState,
    ESetRtpSendingStateIce,
    ESetRtpReceivingState,
    ESetRtcpSendingState,
    ESetRtcpSendingStateIce,
    ESetRtcpReceivingState,
    EPassiveRtpReceivingState,
    EPassiveRtpSendingState,
    EPassiveRtcpReceivingState,
    EPassiveRtcpSendingState,
    EPassiveRtpSendingStateIce,
    EPassiveRtcpSendingStateIce,
    ERunning,
    ETeardown,
    EError,
    EReady
    };

class TNatComTestState
    {
public:
    TNatComTestState();

    void SetSequence( const RArray<TNatComState> aStateSequence );
    
    TNatComState Next();
    
    TNatComState Current();
    
    ~TNatComTestState();
    
    void SetTestType( TInt aTestType );
    
    TInt TestType();
    
private:
    TInt iCurrentIndex;
    RArray<TNatComState> iStateSequence;
    TInt iComTestType;
    };
    
#endif
