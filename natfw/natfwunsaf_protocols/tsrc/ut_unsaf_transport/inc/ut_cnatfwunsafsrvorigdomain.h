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




#ifndef __UT_CNATFWUNSAFSRVORIGDOMAIN_H__
#define __UT_CNATFWUNSAFSRVORIGDOMAIN_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <in_sock.h>

//  FORWARD DECLARATIONS
class UT_CNATFWUNSAFServerResolverObserverStub;
class CNATFWUNSAFQueryData;
class CNATFWUNSAFHostResolver;
class CNATFWUNSAFSRVOrigDomain;
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
NONSHARABLE_CLASS( UT_CNATFWUNSAFSrvOrigDomain )
     : public CEUnitTestSuiteClass
    {

    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CNATFWUNSAFSrvOrigDomain* NewL();
        static UT_CNATFWUNSAFSrvOrigDomain* NewLC();
        /**
         * Destructor
         */
        ~UT_CNATFWUNSAFSrvOrigDomain();

    private:    // Constructors and destructors

        UT_CNATFWUNSAFSrvOrigDomain();
        void ConstructL();

    private:    // New methods

        void SetupL();
        void Teardown();
        void QueryDataNewLTestL ();
        void HandleQueryResultLTestL ();

    private:

        EUNIT_DECLARE_TEST_TABLE;

        CNATFWUNSAFSRVOrigDomain*    iQueryCondition;
        UT_CNATFWUNSAFServerResolverObserverStub* iObserver;
        RSocketServ            iServer;
        RConnection         iConnection;
        TRequestStatus      iStatus;
        CNATFWUNSAFHostResolver*   iHostResolver;
        CNATFWUNSAFServerQuery*       iServerQuery;
        RArray<TInetAddr>* iResultArray;
    };

#endif      //  __UT_CNATFWUNSAFSRVORIGDOMAIN_H__

// End of file

