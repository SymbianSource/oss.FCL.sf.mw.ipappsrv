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




#ifndef __UT_CMceNatStateIdle_H__
#define __UT_CMceNatStateIdle_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "mceclientserver.h"
#include "mcenatevents.h"

//  FORWARD DECLARATIONS
class CMceSipSession;
class CMceCsSession;
class CMceNatSipSession;
class CMceSipStateMachine;
class CMceCsSession;
class CMceServerCore;
class CMCETls;
class CMceNatState;


//  CLASS DEFINITION
class UT_CMceNatStateIdle
     : public CEUnitTestSuiteClass
    {
    public:    	// Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMceNatStateIdle* NewL();
        static UT_CMceNatStateIdle* NewLC();

        /**
         * Destructor
         */
        ~UT_CMceNatStateIdle();

    private:    // Constructors

        UT_CMceNatStateIdle();
        void ConstructL();

    private:    // New methods

        void SetupL();
        void Teardown();        
        
        void UT_CMceNatStateIdle_AcceptLL();
        void UT_CMceNatStateIdle_EntryLL();
        void UT_CMceNatStateIdle_ExitLL();
        void UT_CMceNatStateIdle_Exit2LL();
        void UT_CMceNatStateIdle_Exit2_1LL();
                
        TBool DoesAccept( TMceNatEventCode aEventCode );
        
                        

    private:    // Data

    	EUNIT_DECLARE_TEST_TABLE; 
    		
    	TMceIds iIds;
    	TUint iNatSessionId;    		
        CMceServerCore* iServerCore;
        CMceCsSession* iSession;
        CMceSipSession* iSipSession;
        CMceNatSipSession* iNatSession;
    
    private: // Not owned

        CMCETls* iStorage;
		CMceNatState* iState;
    };

#endif      //  __UT_CMceNatStateIdle_H__

// End of file
