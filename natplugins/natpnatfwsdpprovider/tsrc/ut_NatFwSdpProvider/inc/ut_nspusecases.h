/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef __UT_CNSPUSECASES_H__
#define __UT_CNSPUSECASES_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include "nspsessionobserver.h"

//  INTERNAL INCLUDES
#include "nspcontrollerif.h"

//  FORWARD DECLARATIONS
class CSdpDocument;
class CSdpMediaField;
class CNSPController;
class CNSPSession;
class CRepository;
class TNSPStorage;

#include <e32def.h>
#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
NONSHARABLE_CLASS( UT_CNSPUseCases )
     : public CEUnitTestSuiteClass,
       public MNSPSessionObserver
    {
public: // Enumerations

    /* Async calls done to plugin client */
    enum TNSP_UT_LastAsyncCall
        {
        ENSP_UT_None,
        ENSP_UT_Initialized,
        ENSP_UT_OfferReady,
        ENSP_UT_AnswerReady,
        ENSP_UT_UpdateSdp,
        ENSP_UT_ErrorOccurred,
        ENSP_UT_IcmpErrorOccurred
        };
    
public: // Constructors and destructors

    static UT_CNSPUseCases* NewL();
    static UT_CNSPUseCases* NewLC();
    ~UT_CNSPUseCases();

private: // Constructors and destructors

    UT_CNSPUseCases();    
    void ConstructL();

public: // From MNSPSessionObserver

    void Initialized( TUint aSessionId );
    void OfferReady( TUint aSessionId, CSdpDocument* aDocument );
    void AnswerReady( TUint aSessionId, CSdpDocument* aDocument );
    void UpdateSdp( TUint aSessionId, CSdpDocument* aDocument );
    void ErrorOccurred( TUint aSessionId, TInt aError );
    void IcmpErrorOccurred( TUint aSessionId, TInt aError );

private: // New methods

    // Setup & Teardown
    void SetupStunL();
    void SetupIceL();
    void Teardown();
    
    // Auxiliary
    TDesC8& RTCPAddr( CSdpMediaField& aMediaField );
    TUint RTCPPort( CSdpMediaField& aMediaField );
    
    // Use cases
    void CreateStunOfferLL();
    void ResolveStunLL();
    void DecodeStunAnswerLL();
    void StunUpdateLL();
    void StunRejectMTL();
    void StunRejectMOL();
    void StunCreateOfferModConnFieldL();
    void StunResolveModConnFieldL();
    void StunDecodeAnswerModConnFieldL();
    void StunMtHoldLL();
    void StunMtOldwayHoldLL();
    void StunMoAnswerUpdateLL();
    void RemoteAddressUpdateLL();
    void SyncClosingL();
    void AsyncClosing_ConnectedL();
    void CreateIceOfferLL();
    void ResolveIceLL();
    void DecodeIceAnswerLL();
    void IceUpdateLL();
    void IceMtHoldLL();
    void TestMTMismatchL();
    void TestMOMismatchL();
    void TestLocalUpdateL();
    void TestTimerLL();
    void TestRtcpIcmpIgnoringL();
    void FetchCandFailL();
    void CreatePullModeOfferL();
    void Reslove_ConnectedL();

private: // Data

    RBuf8 iRtcpAddr;
    
    TNSPStorage* iStorage;
    CRepository* iCenrep;
    TUint32 iCurrentDomainKey;
    
    TUint iSessionId;
    CNSPController* iController;
    CNSPSession* iSession;
    CSdpDocument* iDocument;
    
    TNSP_UT_LastAsyncCall iLastCall;
    TInt  iLastError;
    TBool iUseInternalSchedulerLoop;
    
    EUNIT_DECLARE_TEST_TABLE;
    };

#endif      //  __UT_CNSPUSECASES_H__

// End of file
