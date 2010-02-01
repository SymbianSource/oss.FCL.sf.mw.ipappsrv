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




#ifndef __UT_CMCCRESOURCEITEM_H__
#define __UT_CMCCRESOURCEITEM_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
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
NONSHARABLE_CLASS( UT_CMccResourceItem )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccResourceItem* NewL();
        static UT_CMccResourceItem* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccResourceItem();

    private:    // Constructors and destructors

        UT_CMccResourceItem();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CMccResourceItem_IncreaseRefCountL();
        
        
         void UT_CMccResourceItem_DecreaseRefCountL();
        
        
         void UT_CMccResourceItem_RefCountL();
        
        
         void UT_CMccResourceItem_EndpointIdL();
        
        
         void UT_CMccResourceItem_IsNetworkResourceL();
        
        
         void UT_CMccResourceItem_IsResourceActiveL();
        
        
         void UT_CMccResourceItem_IsSharedResourceL();
        
        
         void UT_CMccResourceItem_SetResourceStateLL();
        
        
         void UT_CMccResourceItem_IsSinkL();
        
        
         void UT_CMccResourceItem_IsSourceL();
        
        
         void UT_CMccResourceItem_IsInternalL();
        
        
         void UT_CMccResourceItem_SourceL();
        
        
         void UT_CMccResourceItem_SinkL();
        
        
         void UT_CMccResourceItem_MultiplexerLL();
        
        
         void UT_CMccResourceItem_GetMultiplexerLL();
        
        
         void UT_CMccResourceItem_UserSessionMatchL();

         
         void UT_CMccResourceItem_UserMatchL();
        
        
         void UT_CMccResourceItem_UserMatchNotStrictL();
        
        
         void UT_CMccResourceItem_UsersInfoLL();
        
        
         void UT_CMccResourceItem_CurrentStateL();
        
        
         void UT_CMccResourceItem_MatchL();
         
         void UT_CMccResourceItem_MatchSessionL();

         void UT_CMccResourceItem_IsCurrentUserL();
        
         void UT_CMccResourceItem_NonSharableResourceL();
         
         void UT_CMccResourceItem_StandbyStateChangeL();
         
         void UT_CMccResourceItem_NeedsCameraL();

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        CMccRtpDataSink* iSinkStub;
        CMccRtpDataSource* iSourceStub;
        CMccResourceItem* iItem;


    };

#endif      //  __UT_CMCCRESOURCEITEM_H__

// End of file
