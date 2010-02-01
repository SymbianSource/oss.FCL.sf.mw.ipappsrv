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




#ifndef __UT_CMCESTATESERVERINITIALIZING_H__
#define __UT_CMCESTATESERVERINITIALIZING_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "mceevents.h"

//  FORWARD DECLARATIONS
class CMceSipSession;
class CMceNatSipSession;
class CMceCsSession;
class CMceServerCore;
class CMCETls;
class CMceStateServerInitializing;


//  CLASS DEFINITION
class UT_CMceStateServerInitializing
     : public CEUnitTestSuiteClass
    {
    public:    	// Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMceStateServerInitializing* NewL();
        static UT_CMceStateServerInitializing* NewLC();

        /**
         * Destructor
         */
        ~UT_CMceStateServerInitializing();

    private:    // Constructors

        UT_CMceStateServerInitializing();
        void ConstructL();

    private:    // New methods

        void SetupL();
        void Teardown();        
        
        void UT_CMceStateServerInitializing_DoAcceptLL();
        void UT_CMceStateServerInitializing_EntryLL();
        void UT_CMceStateServerInitializing_ExitLL();

		TBool IsAcceptedL( enum TMceSipEventCode aEventCode );

    private:    // Data

    	EUNIT_DECLARE_TEST_TABLE; 
    		
    	TMceIds iIds;
        CMceServerCore* iServerCore;
        CMceCsSession* iSession;
        CMceSipSession* iSipSession;
        CMceNatSipSession* iNatSession;
    
    private: //not Owned

        CMCETls* iStorage;
		CMceStateServerInitializing* iState;
    };

#endif      //  __UT_CMCESTATESERVERINITIALIZING_H__

// End of file
