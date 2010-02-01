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




#ifndef __UT_CMccASYNCEVENTHANDLER_H__
#define __UT_CMccASYNCEVENTHANDLER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMccAsyncEventHandler;

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
NONSHARABLE_CLASS( UT_CMccAsyncEventHandler )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccAsyncEventHandler* NewL();
        static UT_CMccAsyncEventHandler* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccAsyncEventHandler();

    private:    // Constructors and destructors

        UT_CMccAsyncEventHandler();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CMccAsyncEventHandler_EventReceiveRequestLL();
        
        
         void UT_CMccAsyncEventHandler_CancelEventReceiveRequestLL();
        
        
         void UT_CMccAsyncEventHandler_SendEventToClientL();
        
        
         void UT_CMccAsyncEventHandler_SendMccEventToClientL();
        
        
         void UT_CMccAsyncEventHandler_SendMediaSignallingToClientL();
        
         void UT_CMccAsyncEventHandler_ReconstructMediaSignalL();
    
         void UT_CMccAsyncEventHandler_HandleAmrEventL();

         void UT_CMccAsyncEventHandler_HandleRtpSinkEventL();

         void UT_CMccAsyncEventHandler_HandleRtpSourceEventL();

         void UT_CMccAsyncEventHandler_HandleJitterEventL();

         void UT_CMccAsyncEventHandler_HandleMmfEventL();

         void UT_CMccAsyncEventHandler_CompleteClientRequestL();


    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        RMessage2 iMessage;
        CMccAsyncEventHandler* iHandler;
    };

#endif      //  __UT_CMccASYNCEVENTHANDLER_H__

// End of file
