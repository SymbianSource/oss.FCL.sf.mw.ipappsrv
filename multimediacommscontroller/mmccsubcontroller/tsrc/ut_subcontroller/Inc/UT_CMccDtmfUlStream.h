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




#ifndef __UT_CMCCDTMFULSTREAM_H__
#define __UT_CMCCDTMFULSTREAM_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include "mccinternalcodecs.h"

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMccDtmfUlStream;
class CMccRtpManager;
class CMccTestEventHandler;
class MDataSource;
class MDataSink;
class CMccResourcePoolStub;
class CMccMultiplexer;
class CMccRtpMediaClock;

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
NONSHARABLE_CLASS( UT_CMccDtmfUlStream )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccDtmfUlStream* NewL();
        static UT_CMccDtmfUlStream* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccDtmfUlStream();

    private:    // Constructors and destructors

        UT_CMccDtmfUlStream();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CMccDtmfUlStream_NewLL();
        
        
         void UT_CMccDtmfUlStream_NewLCL();
        
        
         void UT_CMccDtmfUlStream_SetPrioritySettingsLL();
        
        
         void UT_CMccDtmfUlStream_AddSinkAndSourceLL();
        
        
         void UT_CMccDtmfUlStream_LoadCodecLL();
        
        
         void UT_CMccDtmfUlStream_PrimeLL();
        
        
         void UT_CMccDtmfUlStream_PlayLL();
        
        
         void UT_CMccDtmfUlStream_PauseLL();
        
        
         void UT_CMccDtmfUlStream_ResumeLL();
        
        
         void UT_CMccDtmfUlStream_StopLL();
        
        
         void UT_CMccDtmfUlStream_ResetCountersLL();
        
        
         void UT_CMccDtmfUlStream_SetBalanceLL();
        
        
         void UT_CMccDtmfUlStream_GetBalanceLL();
        
        
         void UT_CMccDtmfUlStream_SendMediaSignalLL();
        
        
         void UT_CMccDtmfUlStream_StartInactivityTimerLL();
        
        
         void UT_CMccDtmfUlStream_StopInactivityTimerLL();
        
        
         void UT_CMccDtmfUlStream_GetSSRCLL();
        
        
         void UT_CMccDtmfUlStream_IsUplinkL();
        
        
         void UT_CMccDtmfUlStream_StandByLL();
        

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        
        CMccTestEventHandler* iHandler;
        CMccRtpManager* iRtpMan;
        CMccDtmfUlStream* iStream;
        TUint32 iStreamId;
        
        MDataSource* iSourceStub;
        MDataSink* iSinkStub;
        
        CMccResourcePoolStub* iResources;
        CMccMultiplexer* iMultiPlexer;
        
        CMccRtpMediaClock* iRtpMediaClock;
        
        TMccCodecInfo iCInfo;
        
        TUint32 iMccSessionId;
    };

#endif      //  __UT_CMCCDTMFULSTREAM_H__

// End of file
