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

#ifndef __UT_NSPINTERFACE_H__
#define __UT_NSPINTERFACE_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include "nspsessionobserver.h"

//  INTERNAL INCLUDES
#include <e32def.h>
#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif

class CNSPInterface;
class CRepository;
class TNSPStorage;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
NONSHARABLE_CLASS( UT_CNSPInterface )
     : public CEUnitTestSuiteClass,
       public MNSPSessionObserver
    {
public: // Constructors and destructors

    static UT_CNSPInterface* NewL();
    static UT_CNSPInterface* NewLC();
    ~UT_CNSPInterface();

private: // Constructors and destructors

    UT_CNSPInterface();
    void ConstructL();

public: // From MNSPSessionObserver

    void Initialized( TUint aSessionId );
    void OfferReady( TUint aSessionId, CSdpDocument* aOffer );
    void AnswerReady( TUint aSessionId, CSdpDocument* aAnswer );
    void UpdateSdp( TUint aSessionId, CSdpDocument* aOffer );
    void ErrorOccurred( TUint aSessionId, TInt aError );
    void IcmpErrorOccurred( TUint aSessionId, TInt aError );

private: // New methods

    // Setup & Teardown
    void SetupL();
    void Teardown();
    
    void UT_NewLL();
    void UT_SetSessionParam();
    void UT_GetSessionParam();
    
private: // Data

	TNSPStorage* iStorage;
    CRepository* iCenrep;
    TUint32 iCurrentDomainKey;
    
    CNSPInterface* iInterface; // own
    TUint iSessionId;
    
    EUNIT_DECLARE_TEST_TABLE;
    };

#endif      //  __UT_NSPINTERFACE_H__

// End of file
