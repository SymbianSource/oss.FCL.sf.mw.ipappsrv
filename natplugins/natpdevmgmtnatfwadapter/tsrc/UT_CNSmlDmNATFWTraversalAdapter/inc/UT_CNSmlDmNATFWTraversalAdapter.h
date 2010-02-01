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


#ifndef __UT_CNSMLDMNATFWTRAVERSALADAPTER_H__
#define __UT_CNSMLDMNATFWTRAVERSALADAPTER_H__

//  EXTERNAL INCLUDES

#include <digia/eunit/ceunittestsuiteclass.h>
#include <nsmldmmodule.h>
#include "smldmadapter.h"

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CNSmlDmNATFWTraversalAdapter;
class CNATFWDomainEntry;
class CNATFWIAPEntry;
class CSmlDmCallback_stub;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CNSmlDmNATFWTraversalAdapter
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CNSmlDmNATFWTraversalAdapter* NewL();
        static UT_CNSmlDmNATFWTraversalAdapter* NewLC();
        /**
         * Destructor
         */
        ~UT_CNSmlDmNATFWTraversalAdapter();

    private:    // Constructors and destructors

        UT_CNSmlDmNATFWTraversalAdapter();
        void ConstructL();
        

    private:    // New methods

         void SetupL();
         
         void Setup2L();
        
         void Teardown();
         
         void CNSmlDmNATFWTraversalAdapter_DDFVersionLL(  );
         
         void CNSmlDmNATFWTraversalAdapter_AddNodeObjectLL(  );
         
         void CNSmlDmNATFWTraversalAdapter_UpdateLeafObjectLL(  );
         
         void CNSmlDmNATFWTraversalAdapter_UpdateLeafObjectOtherLL();
         
         void CNSmlDmNATFWTraversalAdapter_FetchLeafObjectLL(  );
         
         void CNSmlDmNATFWTraversalAdapter_FetchLeafObjectOtherLL(  );
        
         void CNSmlDmNATFWTraversalAdapter_ChildURIListLL(  );
        
         void CNSmlDmNATFWTraversalAdapter_DeleteNodeObjectLL(  );
         
         void CNSmlDmNATFWTraversalAdapter_DesToIntL(  );
         
         void CNSmlDmNATFWTraversalAdapter_RemoveLastURISegL(  );
         
         void CNSmlDmNATFWTraversalAdapter_NumOfURISegsL(  );
         
         void CNSmlDmNATFWTraversalAdapter_LastURISegL(  );
         
         
         void CNSmlDmNATFWTraversalAdapter_OtherFunctionsLL();
        
        
         void CNSmlDmNATFWTraversalAdapter_NewLCL();
        
        
         void CNSmlDmNATFWTraversalAdapter_ConstructLL();
        
         inline void DmTaPrint( TInt aStatus );
         
         inline void DmTaPrint( const TDesC8& aText );

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        
        CNSmlDmNATFWTraversalAdapter* iNSmlDmNATFWTraversalAdapter;
        
        CSmlDmCallback_stub* iDmCallback;
        
        HBufC8* iLuid1;
        HBufC8* iLuid2;
        HBufC8* iLuid3;
        
        TBool iAllocDecorator;

    };

#endif      //  __UT_CNSMLDMNATFWTRAVERSALADAPTER_H__

// End of file
