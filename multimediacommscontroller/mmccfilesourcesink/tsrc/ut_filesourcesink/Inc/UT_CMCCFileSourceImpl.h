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




#ifndef __UT_CMccFILESOURCEIMPL_H__
#define __UT_CMccFILESOURCEIMPL_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMccFileSourceImpl;
class CMMFDataBuffer;
class CMccUlDataPath;

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
NONSHARABLE_CLASS( UT_CMccFileSourceImpl )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccFileSourceImpl* NewL();
        static UT_CMccFileSourceImpl* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccFileSourceImpl();

    private:    // Constructors and destructors

        UT_CMccFileSourceImpl();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

         void SetupL();
         
         void Setup2L();

         void Teardown();
        
         void UT_CMccFileSourceImpl_NewLL();
        
        
         void UT_CMccFileSourceImpl_CMccFileSourceImplL();
        
        
         void UT_CMccFileSourceImpl_AddDataSinkL();
        
        
         void UT_CMccFileSourceImpl_OpenFileLL();
        
        
         void UT_CMccFileSourceImpl_CloseFileLL();
        
        
         void UT_CMccFileSourceImpl_SetPositionLL();
        
        
         void UT_CMccFileSourceImpl_VideoTypeLL();
        
        
         void UT_CMccFileSourceImpl_AudioTypeLL();
        
        
         void UT_CMccFileSourceImpl_VideoFrameRateLL();
        
        
         void UT_CMccFileSourceImpl_AudioBitRateLL();
        
        
         void UT_CMccFileSourceImpl_DurationLL();
        
        
         void UT_CMccFileSourceImpl_VideoFrameSizeLL();
        
        
         void UT_CMccFileSourceImpl_VideoBitRateLL();
        
        
         void UT_CMccFileSourceImpl_PositionLL();
        
        
         void UT_CMccFileSourceImpl_SourceDataTypeCodeL();
        
        
         void UT_CMccFileSourceImpl_SetSourceDataTypeCodeL();
        
        
         void UT_CMccFileSourceImpl_SourcePrimeLL();
        
        
         void UT_CMccFileSourceImpl_SourcePlayLL();
        
         void UT_CMccFileSourceImpl_SourcePlayL2L();
        
         void UT_CMccFileSourceImpl_SourcePauseLL();
        
        
         void UT_CMccFileSourceImpl_SourceStopLL();
        
        
         void UT_CMccFileSourceImpl_FillBufferLL();
         
         void UT_CMccFileSourceImpl_PauseAudioL();
         
         void UT_CMccFileSourceImpl_PauseVideoL();
        
         void UT_CMccFileSourceImpl_SourceThreadLogonL();

         void UT_CMccFileSourceImpl_SourceThreadLogOffL();
         
         void UT_CMccFileSourceImpl_MilliToMicroSecondsL();

         void UT_CMccFileSourceImpl_SetDelayAfterStopL();

         void UT_CMccFileSourceImpl_EofTimerExpiredLL();
         
         void UT_CMccFileSourceImpl_GetConfigKeyLL();
         
         void UT_CMccFileSourceImpl_GetConfigKeyL2L();
         
    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        
        CMMFDataBuffer*     iBuffer;
        CMccFileSourceImpl* iSourceImp;
        CMccUlDataPath*  iAudioULDPStub;
        CMccUlDataPath*  iVideoULDPStub;


    };

#endif      //  __UT_CMccFILESOURCEIMPL_H__

// End of file
