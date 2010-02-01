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




#ifndef __UT_CNATFWUNSAFHOSTRESOLVER_H__
#define __UT_CNATFWUNSAFHOSTRESOLVER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <in_sock.h>

//  FORWARD DECLARATIONS
class UT_CNATFWUNSAFServerResolverObserverStub;
class CNATFWUNSAFQueryData;
class CNATFWUNSAFHostResolver;
class CNATFWUNSAFA_AAAAOrigDomain;
class CNATFWUNSAFServerQuery;

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
NONSHARABLE_CLASS( UT_CNATFWUNSAFHostResolver )
     : public CEUnitTestSuiteClass
    {

    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CNATFWUNSAFHostResolver* NewL();
        static UT_CNATFWUNSAFHostResolver* NewLC();
        /**
         * Destructor
         */
        ~UT_CNATFWUNSAFHostResolver();

    private:    // Constructors and destructors

        UT_CNATFWUNSAFHostResolver();
        void ConstructL();

    private:    // New methods

        void SetupL();
        void Teardown();
        void NewLTestL ();
        void GetByQueryTestL ();
        void RunErrorTestL ();
        void CancelGetByURITestL ();
        void RunLTestL();

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE;

        CNATFWUNSAFA_AAAAOrigDomain*    iQueryCondition;
        UT_CNATFWUNSAFServerResolverObserverStub* iObserver;
        UT_CNATFWUNSAFServerResolverObserverStub* iObserver2;

        RSocketServ            iServer;
        RConnection         iConnection;
        TRequestStatus      iStatus;
        CNATFWUNSAFHostResolver*   iHostResolver;
        CNATFWUNSAFServerQuery*       iServerQuery;
        CNATFWUNSAFServerQuery*       iServerQuery2;
        RArray<TInetAddr>* iResultArray;
    };

#endif      //  __UT_CNATFWUNSAFHOSTRESOLVER_H__

// End of file
