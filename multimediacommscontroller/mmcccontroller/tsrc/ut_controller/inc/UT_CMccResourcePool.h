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




#ifndef __UT_CMCCRESOURCEPOOL_H__
#define __UT_CMCCRESOURCEPOOL_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMccResourcePool;
class CMccRtpDataSink;
class CMccRtpDataSource;
class CMccResourceItem;
class MDataSource;
class MDataSink;

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
NONSHARABLE_CLASS( UT_CMccResourcePool )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccResourcePool* NewL();
        static UT_CMccResourcePool* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccResourcePool();

    private:    // Constructors and destructors

        UT_CMccResourcePool();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

         void SetupL();
        
         void Teardown();
         
         void InitializeStandbyStreamL( TUint32 aStandbyStreamId );
        
         void UT_CMccResourcePool_ReuseLL();
        
        
         void UT_CMccResourcePool_ReferenceCountLL();
        
        
         void UT_CMccResourcePool_SourceCandidateL();
        
        
         void UT_CMccResourcePool_SinkCandidateL();
        
        
         void UT_CMccResourcePool_SetEnableRtcpLL();
        
        
         void UT_CMccResourcePool_EnableRtcpL();
        
        
         void UT_CMccResourcePool_CameraHandlerLL();
        
        
         void UT_CMccResourcePool_CreateNewCameraHandlerLL();
        
        
         void UT_CMccResourcePool_UpdateResourceLL();
        
        
         void UT_CMccResourcePool_ReserveSinkLL();
        
        
         void UT_CMccResourcePool_ReserveSourceLL();
        
        
         void UT_CMccResourcePool_PrepareEndpointRemovalLL();
        
        
         void UT_CMccResourcePool_RemoveEndpointLL();
        
        
         void UT_CMccResourcePool_ReserveResourcesLL();
        
        
         void UT_CMccResourcePool_FreeResourcesL();
        
        
         void UT_CMccResourcePool_IsResourceUsageAllowedL();
        
        
         void UT_CMccResourcePool_SetResourceStateLL();
        
        
         void UT_CMccResourcePool_ResourceStateSetLL();
        
        
         void UT_CMccResourcePool_EventReceivedL();
        
        
         void UT_CMccResourcePool_SinksL();
        
        
         void UT_CMccResourcePool_SourcesL();
        
        
         void UT_CMccResourcePool_MultiplexerLL();
        
        
         void UT_CMccResourcePool_FindResourceL();
        
        
         void UT_CMccResourcePool_FindNetworkResourceByRtpStreamIdL();
        
        
         void UT_CMccResourcePool_FindNetworkResourceByRtpSsrcL();
         
         void UT_CMccResourcePool_GetEndpointL();
         
         void UT_CMccResourcePool_StoreKeyFrameInfoL();
         
         void UT_CMccResourcePool_IsKeyFrameL();
         
         void UT_CMccResourcePool_EnableViewFinderL();
         
         void UT_CMccResourcePool_DisableViewFinderL();

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        CMccResourcePool* iResourcePool;
        CMccRtpDataSink* iSinkStub;
        CMccRtpDataSource* iSourceStub;
        CMccResourceItem* iOwnItem1;
        CMccResourceItem* iOwnItem2;
        RPointerArray<MDataSource> iStandbySources;
        RPointerArray<MDataSink> iStandbySinks;
        TUint32 iSinkStubEndpointId;
        TUint32 iSourceStubEndpointId;
		TUint32 iMccSessionId;
        

    };

#endif      //  __UT_CMCCRESOURCEPOOL_H__

// End of file
