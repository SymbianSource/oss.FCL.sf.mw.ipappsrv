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




#ifndef __UT_CMccVIDEOSOURCE_H__
#define __UT_CMccVIDEOSOURCE_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMccVideoSource;
class CMMFDataBuffer;
class CMccUlDataPath;
class CMccCameraHandlerStub;
class CMccTestEventHandler;

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
NONSHARABLE_CLASS( UT_CMccVideoSource )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccVideoSource* NewL();
        static UT_CMccVideoSource* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccVideoSource();

    private:    // Constructors and destructors

        UT_CMccVideoSource();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CMccVideoSource_NewSourceLL();
        
        
         void UT_CMccVideoSource_ConstructSourceLL();
        
        
         void UT_CMccVideoSource_CMccVideoSourceL();
        
        
         void UT_CMccVideoSource_SetCameraHandleL();
        
        
         void UT_CMccVideoSource_SetVideoCodecLL();
        
        
         void UT_CMccVideoSource_GetVideoCodecLL();
        
        
         void UT_CMccVideoSource_GetSupportedVideoCodecsLL();
        
        
         void UT_CMccVideoSource_SetAudioCodecLL();
        
        
         void UT_CMccVideoSource_GetAudioCodecLL();
        
        
         void UT_CMccVideoSource_GetSupportedAudioCodecsLL();
        
        
         void UT_CMccVideoSource_SetAudioEnabledLL();
        
        
         void UT_CMccVideoSource_AudioEnabledLL();
        
        
         void UT_CMccVideoSource_SetGainLL();
        
        
         void UT_CMccVideoSource_GainLL();
        
        
         void UT_CMccVideoSource_MaxGainLL();
        
        
         void UT_CMccVideoSource_SourceDataTypeCodeL();
        
        
         void UT_CMccVideoSource_SetSourceDataTypeCodeL();
        
        
         void UT_CMccVideoSource_BufferEmptiedLL();
        
        
         void UT_CMccVideoSource_CanCreateSourceBufferL();
        
        
         void UT_CMccVideoSource_CreateSourceBufferLL();
        
        
         void UT_CMccVideoSource_SourceThreadLogonL();
        
        
         void UT_CMccVideoSource_SourceThreadLogoffL();
        
        
         void UT_CMccVideoSource_SourcePrimeLL();
         
         void UT_CMccVideoSource_SourcePrimeL2L();
        
        
         void UT_CMccVideoSource_SourcePlayLL();
        
        
         void UT_CMccVideoSource_SourcePauseLL();
        
        
         void UT_CMccVideoSource_SourceStopLL();
        
        
         void UT_CMccVideoSource_FillBufferLL();
        
        
         void UT_CMccVideoSource_NegotiateSourceLL();
        
         void UT_CMccVideoSource_UpdateL();
         
         void UT_CMccVideoSource_CameraIndexL();
         
         void UT_CMccVideoSource_GetParameterL();
         
         void UT_CMccVideoSource_SetParameterL();

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        
        CMccVideoSource* iVideoSource;
        CMMFDataBuffer*  iBuffer;
        CMccUlDataPath*  iULDataPathStub;
        CMccCameraHandlerStub* iCameraHandler;
        CMccTestEventHandler* iEventHandler;


    };

#endif      //  __UT_CMccVIDEOSOURCE_H__

// End of file
