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




#ifndef __UT_CMCCRESOURCECONTAINER_H__
#define __UT_CMCCRESOURCECONTAINER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMccResourceContainer;
class CMccResourceItem;
class CMccRtpDataSink;
class CMccRtpDataSource;

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
NONSHARABLE_CLASS( UT_CMccResourceContainer )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccResourceContainer* NewL();
        static UT_CMccResourceContainer* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccResourceContainer();

    private:    // Constructors and destructors

        UT_CMccResourceContainer();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CMccResourceContainer_AddResourceItemLL();
        
        
         void UT_CMccResourceContainer_FindResourceItemL();
        
        
         void UT_CMccResourceContainer_StreamIdL();
        
        
         void UT_CMccResourceContainer_SetResourceStateLL();
                 
         void UT_CMccResourceContainer_SetResourceStateL2L();

         void UT_CMccResourceContainer_SetResourceStateL3L();
         
         void UT_CMccResourceContainer_SetResourceStateL4L();

         void UT_CMccResourceContainer_IsStandByL();
        
        
         void UT_CMccResourceContainer_SetIgnoreEventL();
        
        
         void UT_CMccResourceContainer_IgnoreEventL();
        
        
         void UT_CMccResourceContainer_FetchEndPointIdL();
        
        
         void UT_CMccResourceContainer_SetEnableRtcpL();
        
        
         void UT_CMccResourceContainer_EnableRtcpL();
        
        
         void UT_CMccResourceContainer_HandleEndpointRemovalL();
        

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        RPointerArray<CMccResourceItem> iItems;
        CMccResourceContainer* iContainer;
        CMccRtpDataSink* iSinkStub;
        CMccRtpDataSource* iSourceStub;
        CMccResourceItem* iOwnItem1;
        CMccResourceItem* iOwnItem2;

    };

#endif      //  __UT_CMCCRESOURCECONTAINER_H__

// End of file
