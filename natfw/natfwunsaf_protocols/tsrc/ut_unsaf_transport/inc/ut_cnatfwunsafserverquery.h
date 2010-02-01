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




#ifndef __UT_CNATFWUNSAFSERVERQUERY_H__
#define __UT_CNATFWUNSAFSERVERQUERY_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <in_sock.h>

//  FORWARD DECLARATIONS
class UT_CNATFWUNSAFServerResolverObserverStub;
class CNATFWUNSAFQueryData;
class CNATFWUNSAFHostResolver;
class CNATFWUNSAFServerQuery;
class CNATFWUNSAFQueryConditionBase;

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
NONSHARABLE_CLASS( UT_CNATFWUNSAFServerQuery )
     : public CEUnitTestSuiteClass
    {

    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CNATFWUNSAFServerQuery* NewL();
        static UT_CNATFWUNSAFServerQuery* NewLC();
        /**
         * Destructor
         */
        ~UT_CNATFWUNSAFServerQuery();

    private:    // Constructors and destructors

        UT_CNATFWUNSAFServerQuery();
        void ConstructL();

    private:    // New methods

        void SetupL();
        void Teardown();
        void NewLTestL();
        void QueryTestL();
        void HandleQueryResultLTestL();
        void ServerResolverObserverTestL();
        void QueryDataTestL();
        void SetQueryConditionLTestL();

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE;

        CNATFWUNSAFQueryConditionBase*    iQueryCondition;
        UT_CNATFWUNSAFServerResolverObserverStub* iObserver;
        RSocketServ            iServer;
        RConnection         iConnection;
        TRequestStatus      iStatus;
        CNATFWUNSAFHostResolver*   iHostResolver;
        CNATFWUNSAFServerQuery*    iServerQuery;
        RArray<TInetAddr>* iResultArray;
    };

#endif      //  __UT_CNATFWUNSAFSERVERQUERY_H__

// End of file

