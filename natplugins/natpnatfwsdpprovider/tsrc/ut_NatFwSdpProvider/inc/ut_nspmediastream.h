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


#ifndef __UT_CNSPMEDIASTREAM_H__
#define __UT_CNSPMEDIASTREAM_H__

class CRepository;
class CNSPController;
class CNSPSession;
class CNSPMediaStream;
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
NONSHARABLE_CLASS( UT_CNSPMediaStream )
     : public CEUnitTestSuiteClass,
       public MNSPSessionObserver
    {
public: // Constructors and destructors

    static UT_CNSPMediaStream* NewL();
    static UT_CNSPMediaStream* NewLC();
    ~UT_CNSPMediaStream();


public: // From MNSPSessionObserver

    void Initialized( TUint aSessionId );
    void OfferReady( TUint aSessionId, CSdpDocument* aDocument );
    void AnswerReady( TUint aSessionId, CSdpDocument* aDocument );
    void UpdateSdp( TUint aSessionId, CSdpDocument* aDocument );
    void ErrorOccurred( TUint aSessionId, TInt aError );
    void IcmpErrorOccurred( TUint aSessionId, TInt aError );

private: // Constructors and destructors

    UT_CNSPMediaStream();
    void ConstructL();


private: // New methods

	// Auxiliary
	TDesC8& RTCPAddr( CSdpMediaField& aMediaField );
	TUint RTCPPort(  CSdpMediaField& aMediaField );
    // Setup & Teardown
    void SetupL();
    void Teardown();
    
    // Method tests
    void UT_NewL();
    void UT_ModifyStunConnL();
    void UT_ModifyIceConnL();
    void UT_ModifyLocalConnL();
    void UT_MediaConnInfoL();

private: // Data

	RBuf8 iRtcpAddr;
	
	TNSPStorage* iStorage; // not own
    CRepository* iCenrep; // own
    TUint32 iCurrentDomainKey;
    
    TUint iSessionId;
    CNSPController* iController; // own
    CNSPSession* iSession; // not own
    CNSPMediaStream* iStream; // own
    
    EUNIT_DECLARE_TEST_TABLE;
    };

#endif      //  __UT_CNSPMEDIASTREAM_H__

// End of file
