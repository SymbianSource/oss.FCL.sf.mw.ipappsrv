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




#ifndef __UT_CMccFILEAUDIO_H__
#define __UT_CMccFILEAUDIO_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <3gplibrary/mp4lib.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMccFileAudio;
class CMccFileVideo;
class CMccFileSourceImpl;
class CMMFDataBuffer;
class CMccUlDataPath;
class CMccUlDataPath;
class CMccFrameItem;

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
NONSHARABLE_CLASS( UT_CMccFileAudio )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccFileAudio* NewL();
        static UT_CMccFileAudio* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccFileAudio();

    private:    // Constructors and destructors

        UT_CMccFileAudio();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

         void SetupL();
         void Setup2L();
         void Setup3L();
         void Teardown();
        
         void UT_CMccFileAudio_NewLL();
        
        
         void UT_CMccFileAudio_SetPositionLL();
        
        
         void UT_CMccFileAudio_TypeL();
        
        
         void UT_CMccFileAudio_PositionLL();
        
        
         void UT_CMccFileAudio_DurationLL();
        
        
         void UT_CMccFileAudio_AudioBitRateLL();
        
        
         void UT_CMccFileAudio_VideoFrameSizeLL();
        
        
         void UT_CMccFileAudio_VideoFrameRateLL();
        
        
         void UT_CMccFileAudio_StartTimerL();
        
        
         void UT_CMccFileAudio_StopTimerL();
        
        
         void UT_CMccFileAudio_GetFourCCL();
        
        
         void UT_CMccFileAudio_ResetTimeL();
        
        
         void UT_CMccFileAudio_TickAudioLL();
        
        
         void UT_CMccFileAudio_ReadFrameLL();
         
         void UT_CMccFileAudio_ReadFrameL2L();
        
         void UT_CMccFileAudio_ParseUpdateAudioDescriptionsL();
        
         void UT_CMccFileAudio_GetFrameLengthL();

         void UT_CMccFileAudio_SetStartTimeL();
         
         void UT_CMccFileAudio_IsPausedL();
         
         void UT_CMccFileSink_GetTimeStampL();
         
         void UT_CMccFileAudio_FillBufferL();

         void UT_CMccFileAudio_WriteBufferL();

         void UT_CMccFileAudio_TimingCorrectionL();
         
         void UT_CMccFileAudio_ConfigKeyL();
    
    private:
         
         static CMccFrameItem* CreateFrameItemLC( TInt aSize );
         
    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        
        CMccFileAudio* iAudio;
        CMccFileVideo* iVideo;
        MP4Handle   iMP4Handle;
        CMccFileSourceImpl* iFileSourceImpl;
        CMMFDataBuffer*     iBuffer;
        CMccUlDataPath*  iAudioULDPStub;
        CMccUlDataPath*  iVideoULDPStub;
        

    };

#endif      //  __UT_CMccFILEAUDIO_H__

// End of file
