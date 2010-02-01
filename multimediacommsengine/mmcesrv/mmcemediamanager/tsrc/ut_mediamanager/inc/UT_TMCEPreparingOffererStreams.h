/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef __UT_TMCEPREPARINGOFFERERSTREAMS_H__
#define __UT_TMCEPREPARINGOFFERERSTREAMS_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include "MceMediaManagerTestHelper.h"

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class TMcePreparingStreams;
class CMceMediaManager;
class CMccControllerStub;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_TMcePreparingOffererStreams : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_TMcePreparingOffererStreams* NewL();
        static UT_TMcePreparingOffererStreams* NewLC();
        /**
         * Destructor
         */
        ~UT_TMcePreparingOffererStreams();

    private:    // Constructors and destructors

        UT_TMcePreparingOffererStreams();
        void ConstructL();



    private:    // New methods

         void SetupL();
        
         void Teardown();

        /**
        * Basic
        */
        void UT_TMcePreparingOffererStreams_BasicL();

        /**
        * Mcc
        *
        * void EntryL();
        *
        */
        void UT_TMcePreparingOffererStreams_EntryLL();

        /**
        * Mcc
        *
        * void StreamsPreparedL();
        *
        */
        void UT_TMcePreparingOffererStreams_StreamsPreparedLL();

        /**
        * Mcc
        *
        * void StreamsStartedL();
        *
        */
        void UT_TMcePreparingOffererStreams_StreamsStartedLL();


    private:    // Data


        TMcePreparingStreams* iState;
		CMceMediaManager* iManager;
		CMceComSession* iSession;
		CMceComSession* iInSession;
		CSdpDocument* iSdpDocument; 
		CMceServerStub* iServer;
		CMccControllerStub* iMcc;


		EUNIT_DECLARE_TEST_TABLE; 

		
    };

#endif      //  __UT_TMCEPREPARINGOFFERERSTREAMS_H__

// End of file
