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




#ifndef __UT_CMCECLIENTRESOLVER_H__
#define __UT_CMCECLIENTRESOLVER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "mceclientserver.h"
#include "mceclientresolver.h"

//  FORWARD DECLARATIONS
class CMceClientResolver;
class CMceServerCore;
class CMCETls;
class CMceCsSession;
class CSIPServerTransaction;
class CMceSipConnection;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMceClientResolver
     : public CEUnitTestSuiteClass,
       public MMceClientResolverObserver
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMceClientResolver* NewL();
        static UT_CMceClientResolver* NewLC();
        /**
         * Destructor
         */
        ~UT_CMceClientResolver();
        
    public:
    
    void IncomingResolvedRequest( TUid aClientUid,
                                          CMceSipConnection& aConnection,
                                          CSIPServerTransaction* aRequest );
            

    private:    // Constructors and destructors

        UT_CMceClientResolver();
        void ConstructL();

    private:    // New methods

        void SetupL();

        void Teardown();
         
        void UT_CMceClientResolver_RegisterClientLL();

        void UT_CMceClientResolver_DeregisterClientL();

        void UT_CMceClientResolver_ResolveIncomingRequestLL();
        
        void UT_CMceClientResolver_ResolveIncomingRequestL_1L();

    private:    // Data

		EUNIT_DECLARE_TEST_TABLE; 
		
        CMceClientResolver* iClientResolver;
	    
	    
		
    };

#endif      //  __UT_CMCECLIENTRESOLVER_H__

// End of file
