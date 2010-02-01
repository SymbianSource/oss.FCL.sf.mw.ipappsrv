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




#ifndef __UT_CNATFWUNSAFA_AAAASRVDOMAIN_H__
#define __UT_CNATFWUNSAFA_AAAASRVDOMAIN_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <in_sock.h>

//  FORWARD DECLARATIONS
class UT_CNATFWUNSAFServerResolverObserverStub;
class CNATFWUNSAFQueryData;
class CNATFWUNSAFHostResolver;
class CNATFWUNSAFA_AAAASRVDomain;
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
NONSHARABLE_CLASS( UT_CNATFWUNSAFA_AAAASRVDomain )
     : public CEUnitTestSuiteClass
    {

    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CNATFWUNSAFA_AAAASRVDomain* NewL();
        static UT_CNATFWUNSAFA_AAAASRVDomain* NewLC();
        /**
         * Destructor
         */
        ~UT_CNATFWUNSAFA_AAAASRVDomain();

    private:    // Constructors and destructors

        UT_CNATFWUNSAFA_AAAASRVDomain();
        void ConstructL();

    private:    // New methods

        void SetupL();
        void Teardown();
        void QueryDataNewLTestL ();
        void HandleQueryResultLTestL ();

    private:

        EUNIT_DECLARE_TEST_TABLE;

        CNATFWUNSAFA_AAAASRVDomain*    iQueryCondition;
        UT_CNATFWUNSAFServerResolverObserverStub* iObserver;
        RSocketServ            iServer;
        RConnection         iConnection;
        TRequestStatus      iStatus;
        CNATFWUNSAFHostResolver*   iHostResolver;
        CNATFWUNSAFServerQuery*       iServerQuery;
        RArray<TInetAddr>* iResultArray;

    };

#endif      //  __UT_CNATFWUNSAFA_AAAASRVDOMAIN_H__

// End of file

