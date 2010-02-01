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




#ifndef __UT_CMCENATUSECASES_H__
#define __UT_CMCENATUSECASES_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "mceclientserver.h"

//  FORWARD DECLARATIONS
class CMceServerCore;
class CMceCsSession;
class CMceNatSipSession;
class CMCETls;


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
NONSHARABLE_CLASS( UT_CMCENatUseCases )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMCENatUseCases* NewL();
        static UT_CMCENatUseCases* NewLC();
        
        /**
         * Destructor
         */
        ~UT_CMCENatUseCases();

    private:    // Constructors and destructors

        UT_CMCENatUseCases();
        void ConstructL();

    private:    // New methods

        void SetupL();
        void Teardown();
        
        TInt& NatPluginManagerReturnStatusL();
        
        void UT_MOcall_success();
        void UT_MTcall_success();

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE;
        
        TUint iNatSessionId;    
	    TMceIds iIds;
	    
        CMceServerCore* iServerCore;
        CMceCsSession* iSession;
        CMceNatSipSession* iNatSipSession;
    
    private: //not Owned
    	    
        CMCETls* iStorage;

    };

#endif      //  __UT_CMCENATUSECASES_H__

// End of file
