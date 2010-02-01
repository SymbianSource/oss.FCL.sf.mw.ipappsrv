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


#ifndef __UT_NSPSESSION_H__
#define __UT_NSPSESSION_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include "nspsessionobserver.h"

//  INTERNAL INCLUDES
#include "nspcontrollerif.h"

//  FORWARD DECLARATIONS
class CSdpDocument;
class CNATFWCandidatePair;
class CNSPController;
class CNSPSession;
class CNSPWrapper;
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
NONSHARABLE_CLASS( UT_CNSPSession )
     : public CEUnitTestSuiteClass,
       public MNSPSessionObserver
    {
public:

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

    static UT_CNSPSession* NewL();
    static UT_CNSPSession* NewLC();
    ~UT_CNSPSession();

private: // Constructors and destructors

    UT_CNSPSession();
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
    void SetupL();
    void Teardown();
    
    // Method tests
    void UT_CreateOfferL();
    void UT_ResolveL();
    void UT_DecodeAnswerL();
    void UT_UpdateL();
    
private: // Data

	TNSPStorage* iStorage;
    CRepository* iCenrep;
    TUint32 iCurrentDomainKey;

    TUint iSessionId;
    CNSPController* iController;
    CNSPSession* iSession;    
    CSdpDocument* iDocument;
    
    TNSP_UT_LastAsyncCall iLastCall;
    TBool iUseInternalSchedulerLoop;
    
    EUNIT_DECLARE_TEST_TABLE;
    };

#endif      //  __UT_NSPSESSION_H__

// End of file
