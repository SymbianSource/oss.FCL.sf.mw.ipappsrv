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




#ifndef __UT_CMccAnySource_H__
#define __UT_CMccAnySource_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMccAnySource;
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
NONSHARABLE_CLASS( UT_CMccAnySource )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccAnySource* NewL();
        static UT_CMccAnySource* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccAnySource();

    private:    // Constructors and destructors

        UT_CMccAnySource();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

         void SetupL();
        
         void Teardown();
            
         void UT_CMccAnySource_SourceDataTypeCodeL();
        
        
         void UT_CMccAnySource_SetSourceDataTypeCodeL();
        
        
         void UT_CMccAnySource_BufferEmptiedLL();
        
        
         void UT_CMccAnySource_CanCreateSourceBufferL();
        
        
         void UT_CMccAnySource_CreateSourceBufferLL();
        
        
         void UT_CMccAnySource_SourceThreadLogonL();
        
        
         void UT_CMccAnySource_SourceThreadLogoffL();
        
        
         void UT_CMccAnySource_SourcePrimeLL();
        
        
         void UT_CMccAnySource_SourcePlayLL();
        
        
         void UT_CMccAnySource_SourcePauseLL();
        
        
         void UT_CMccAnySource_SourceStopLL();
        
        
         void UT_CMccAnySource_FillBufferLL();
        
        
         void UT_CMccAnySource_NegotiateSourceLL();
        

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 

        CMccAnySource*  iAnySource;
        CMccTestEventHandler* iEventHandler;


    };

#endif      //  __UT_CMccAnySource_H__

// End of file
