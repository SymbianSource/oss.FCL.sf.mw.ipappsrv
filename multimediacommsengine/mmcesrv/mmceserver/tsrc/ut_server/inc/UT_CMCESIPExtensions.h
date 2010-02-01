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




#ifndef __UT_CMceSipEXTENSIONS_H__
#define __UT_CMceSipEXTENSIONS_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES

//  FORWARD DECLARATIONS
class CMceSipExtensions;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMceSipExtensions
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMceSipExtensions* NewL();
        static UT_CMceSipExtensions* NewLC();
        /**
         * Destructor
         */
        ~UT_CMceSipExtensions();
        
    private:    // Constructors and destructors

        UT_CMceSipExtensions();
        void ConstructL();

    private:    // New methods

        void SetupL();

        void Teardown();
        
        void UT_CMceSipExtensions_LocalL();
        
        void UT_CMceSipExtensions_RemoteL();
         
        void UT_CMceSipExtensions_SetLocalL();
        
        void UT_CMceSipExtensions_SetRemoteL();
        
        void UT_CMceSipExtensions_LevelLL();
        
        void UT_CMceSipExtensions_UpdateLL();
        
        void UT_CMceSipExtensions_UpdateL2L();
        
        void UT_CMceSipExtensions_RemoteIMSRel5L();
        
        void UT_CMceSipExtensions_UpdateL();
        
        
    private:    // Data

        CMceSipExtensions* iExtensions;
        
		EUNIT_DECLARE_TEST_TABLE; 
		
    };

#endif      //  __UT_CMceSipEXTENSIONS_H__

// End of file
