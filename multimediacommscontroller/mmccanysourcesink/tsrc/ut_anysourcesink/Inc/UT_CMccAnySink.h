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




#ifndef __UT_CMccAnySink_H__
#define __UT_CMccAnySink_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMccAnySink;
class CMccTestEventHandler;

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
NONSHARABLE_CLASS( UT_CMccAnySink )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccAnySink* NewL();
        static UT_CMccAnySink* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccAnySink();

    private:    // Constructors and destructors

        UT_CMccAnySink();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
        
         void UT_CMccAnySink_SinkDataTypeCodeL();
        
        
         void UT_CMccAnySink_SetSinkDataTypeCodeL();
        
        
         void UT_CMccAnySink_BufferEmptiedLL();
        
        
         void UT_CMccAnySink_CanCreateSinkBufferL();
        
        
         void UT_CMccAnySink_CreateSinkBufferLL();
        
        
         void UT_CMccAnySink_SinkThreadLogonL();
        
        
         void UT_CMccAnySink_SinkThreadLogoffL();
        
        
         void UT_CMccAnySink_SinkPrimeLL();
        
        
         void UT_CMccAnySink_SinkPlayLL();
        
        
         void UT_CMccAnySink_SinkPauseLL();
        
        
         void UT_CMccAnySink_SinkStopLL();
        
        
         void UT_CMccAnySink_EmptyBufferLL();
         
        
         void UT_CMccAnySink_BufferFilledLL();
         

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        
        CMccAnySink* iAnySink;
		CMccTestEventHandler* iEventHandler;
    };

#endif      //  __UT_CMccAnySink_H__

// End of file
