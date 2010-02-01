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


#ifndef __UT_CNSPMEDIASTREAMCOMPONENT_H__
#define __UT_CNSPMEDIASTREAMCOMPONENT_H__

class CRepository;
class CNSPController;
class CNSPSession;
class CNSPMediaStream;
class CNSPMediaStreamComponent;
class TNSPStorage;

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include "nspsessionobserver.h"

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
NONSHARABLE_CLASS( UT_CNSPMediaStreamComponent )
     : public CEUnitTestSuiteClass,
       public MNSPSessionObserver
    {
public: // Enumerations

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

    static UT_CNSPMediaStreamComponent* NewL();
    static UT_CNSPMediaStreamComponent* NewLC();
    ~UT_CNSPMediaStreamComponent();

private: // Constructors and destructors

    UT_CNSPMediaStreamComponent();
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
    void UT_ControlMediaLL();
    void UT_RtpIcmp_StateSendRecv();
    void UT_RtcpIcmp_StateSendRecv();

private: // Data

	TNSPStorage* iStorage; // not own
    CRepository* iCenrep; // own
    TUint32 iCurrentDomainKey;
    
    TUint iSessionId;
    CNSPController* iController; // own
    CNSPSession* iSession; // not own
    CNSPMediaStream* iStream; // own
    CNSPMediaStreamComponent* iStreamComponent; // not own
    
    TInt iLastError;
    TNSP_UT_LastAsyncCall iLastCall;
    TBool iUseInternalSchedulerLoop;
    
    EUNIT_DECLARE_TEST_TABLE;
    };

#endif      //  __UT_CNSPMEDIASTREAMCOMPONENT_H__

// End of file
