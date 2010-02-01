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




#ifndef __UT_CAMRCOMMONUTILITY_H__
#define __UT_CAMRCOMMONUTILITY_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CAmrCommonUtility;

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
NONSHARABLE_CLASS( UT_CAmrCommonUtility )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CAmrCommonUtility* NewL();
        static UT_CAmrCommonUtility* NewLC();
        /**
         * Destructor
         */
        ~UT_CAmrCommonUtility();

    private:    // Constructors and destructors

        UT_CAmrCommonUtility();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CAmrCommonUtility_FrameInfoL();
        
        
         void UT_CAmrCommonUtility_SeekSyncL();
        
        
         void UT_CAmrCommonUtility_FrameSizeL();
        
        
         void UT_CAmrCommonUtility_FrameInfoWbL();
        
        
         void UT_CAmrCommonUtility_SeekSyncWbL();
        
        
         void UT_CAmrCommonUtility_FrameSizeWbL();

        
         void UT_CAmrCommonUtility_FrameHeaderSizeL();
        

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        CAmrCommonUtility* iUtil;
    };

#endif      //  __UT_CAMRCOMMONUTILITY_H__

// End of file
