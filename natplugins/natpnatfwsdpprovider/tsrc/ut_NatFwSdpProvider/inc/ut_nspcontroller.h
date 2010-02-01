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


#ifndef __UT_CNSPCONTROLLER_H__
#define __UT_CNSPCONTROLLER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include "nspsessionobserver.h"

// FORWARD DECLARATIONS
class CRepository;
class CNSPController;
class TNSPStorage;

#include <e32def.h>
#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif

NONSHARABLE_CLASS( UT_CNSPController )
     : public CEUnitTestSuiteClass,
       public MNSPSessionObserver
    {
public: // Constructors and destructors

    static UT_CNSPController* NewL();
    static UT_CNSPController* NewLC();
    ~UT_CNSPController();


private: // Constructors and destructors

    UT_CNSPController();
    void ConstructL();

public: // From MNSPSessionObserver

    void Initialized( TUint aSessionId );
    void OfferReady( TUint aSessionId, CSdpDocument* aDocument );
    void AnswerReady( TUint aSessionId, CSdpDocument* aDocument );
    void UpdateSdp( TUint aSessionId, CSdpDocument* aDocument );
    void ErrorOccurred( TUint aSessionId, TInt aError );
    void IcmpErrorOccurred( TUint aSessionId, TInt aError );


private: // New methods

    void SetupL();
    void Teardown();
    
    void UT_CreateUniqueId();
    void UT_GenerateUsernameL();
    void UT_GeneratePasswordL();
    void UT_NewSessionL();
    void UT_FindSessionObjectL();
    void UT_RandomByteL();
    void UT_UseIceL();
    void UT_ReplaceArrayL();
    void UT_IsIceL();
    void UT_EventOccuredL();


private: // Data

	TNSPStorage* iStorage;
    CRepository* iCenrep;
    TUint32 iCurrentDomainKey;
    TUint iReturnedSessionId;
    TUint iReturnedError;
    
    CNSPController* iController;
    
    EUNIT_DECLARE_TEST_TABLE;
    
    };

#endif      //  __UT_CNSPCONTROLLER_H__

// End of file
