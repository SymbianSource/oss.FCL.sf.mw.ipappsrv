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




#ifndef UT_CMCCRESOURCEITERATORS_H
#define UT_CMCCRESOURCEITERATORS_H

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMccRtpDataSink;
class CMccRtpDataSource;
class CMccResourceItem;
class CMccResourceContainer;

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
NONSHARABLE_CLASS( UT_CMccResourceIterators )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccResourceIterators* NewL();
        static UT_CMccResourceIterators* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccResourceIterators();

    private:    // Constructors and destructors

        UT_CMccResourceIterators();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CMccResourceIterators_IterateContainersL();
        
         void UT_CMccResourceIterators_IterateResourceItemsL();       

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        CMccRtpDataSink* iSinkStub;
        CMccRtpDataSource* iSourceStub;
        
        RPointerArray<CMccResourceItem> iItems;
        RPointerArray<CMccResourceContainer> iContainers;


    };

#endif      //  UT_CMCCRESOURCEITERATORS_H

// End of file
