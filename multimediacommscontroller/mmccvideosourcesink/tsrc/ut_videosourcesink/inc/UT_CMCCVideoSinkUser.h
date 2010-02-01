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




#ifndef __UT_CMccVideoSinkUser_H__
#define __UT_CMccVideoSinkUser_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <videoplayer.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CXPSPacketSink;
class CMccVideoSinkUser;

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
NONSHARABLE_CLASS( UT_CMccVideoSinkUser )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccVideoSinkUser* NewL();
        static UT_CMccVideoSinkUser* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccVideoSinkUser();

    private:    // Constructors and destructors

        UT_CMccVideoSinkUser();
        void ConstructL();

    private: // Setup

         void SetupL();
        
         void Teardown();
    
    private: // Tests
        
         void UT_CMccVideoSinkUser_EnqueueLL();
         
         void UT_CMccVideoSinkUser_SetFrameRateLL();
         
         void UT_CMccVideoSinkUser_GetPrerollL();
         
         void UT_CMccVideoSinkUser_GetActualPrerollL();
         
         void UT_CMccVideoSinkUser_PlayL();
         
         void UT_CMccVideoSinkUser_PauseL();
         
         void UT_CMccVideoSinkUser_IsResetNeededRealTimeModeL();
         
    private: // Helpers
    
        void MakeRealTimeUserL( CMccVideoSinkUser& aUser );     

    private:    // Data

        CXPSPacketSink* iPacketSink;
        CMccVideoSinkUser* iUser;
         		
        EUNIT_DECLARE_TEST_TABLE; 


    };

#endif      //  __UT_CMccVideoSinkUser_H__

// End of file
