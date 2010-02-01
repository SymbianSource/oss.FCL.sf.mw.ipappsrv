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




#ifndef __UT_CMCCDTMFDLSTREAM_H__
#define __UT_CMCCDTMFDLSTREAM_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include "mccinternalcodecs.h"

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMccDtmfDlStream;
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
NONSHARABLE_CLASS( UT_CMccDtmfDlStream )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccDtmfDlStream* NewL();
        static UT_CMccDtmfDlStream* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccDtmfDlStream();

    private:    // Constructors and destructors

        UT_CMccDtmfDlStream();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CMccDtmfDlStream_NewLL();
        
        
         void UT_CMccDtmfDlStream_NewLCL();
        
        
         void UT_CMccDtmfDlStream_SetPrioritySettingsLL();
        
        
         void UT_CMccDtmfDlStream_AddSinkAndSourceLL();
        
        
         void UT_CMccDtmfDlStream_LoadCodecLL();
        
        
         void UT_CMccDtmfDlStream_PrimeLL();
        
        
         void UT_CMccDtmfDlStream_PlayLL();
        
        
         void UT_CMccDtmfDlStream_PauseLL();
        
        
         void UT_CMccDtmfDlStream_ResumeLL();
        
        
         void UT_CMccDtmfDlStream_StopLL();
        
        
         void UT_CMccDtmfDlStream_ResetCountersLL();
        
        
         void UT_CMccDtmfDlStream_SetBalanceLL();
        
        
         void UT_CMccDtmfDlStream_GetBalanceLL();
        
        
         void UT_CMccDtmfDlStream_SendMediaSignalLL();
        
        
         void UT_CMccDtmfDlStream_StartInactivityTimerLL();
        
        
         void UT_CMccDtmfDlStream_StopInactivityTimerLL();
        
        
         void UT_CMccDtmfDlStream_GetSSRCLL();
        
        
         void UT_CMccDtmfDlStream_IsUplinkL();
        
        
         void UT_CMccDtmfDlStream_StandByLL();
        

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        
        CMccTestEventHandler* iHandler;
        CMccRtpManager* iRtpMan;
        CMccDtmfDlStream* iStream;
        TUint32 iStreamId;
        
        MDataSource* iSourceStub;
        MDataSink* iSinkStub;
        
        CMccResourcePoolStub* iResources;
        CMccMultiplexer* iMultiPlexer;
        
        CMccRtpMediaClock* iRtpMediaClock;
        
        TMccCodecInfo iCInfo;
        
        TUint32 iMccSessionId;
    };

#endif      //  __UT_CMCCDTMFDLSTREAM_H__

// End of file
