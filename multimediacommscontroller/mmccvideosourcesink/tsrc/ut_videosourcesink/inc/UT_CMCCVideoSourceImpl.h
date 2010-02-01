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




#ifndef __UT_CMccVIDEOSOURCEIMPL_H__
#define __UT_CMccVIDEOSOURCEIMPL_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMccVideoSourceImpl;
class CMMFDataBuffer;
class CMccUlDataPath;
class CCMRMediaBuffer;
class CMccCameraHandlerStub;
class CMccResourcePoolStub;
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
NONSHARABLE_CLASS( UT_CMccVideoSourceImpl )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccVideoSourceImpl* NewL();
        static UT_CMccVideoSourceImpl* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccVideoSourceImpl();

    private:    // Constructors and destructors

        UT_CMccVideoSourceImpl();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

         void SetupL();
         
         void Setup2L();
        
         void Teardown();
        
         void UT_CMccVideoSourceImpl_NewLL();
        
        
         void UT_CMccVideoSourceImpl_CMccVideoSourceImplL();
        
        
         void UT_CMccVideoSourceImpl_SetCameraHandleL();
        
        
         void UT_CMccVideoSourceImpl_SetVideoCodecLL();
        
        
         void UT_CMccVideoSourceImpl_GetVideoCodecLL();
        
        
         void UT_CMccVideoSourceImpl_GetSupportedVideoCodecsLL();
        
        
         void UT_CMccVideoSourceImpl_SetAudioCodecLL();
        
        
         void UT_CMccVideoSourceImpl_GetAudioCodecLL();
        
        
         void UT_CMccVideoSourceImpl_GetSupportedAudioCodecsLL();
        
        
         void UT_CMccVideoSourceImpl_SetAudioEnabledLL();
        
        
         void UT_CMccVideoSourceImpl_AudioEnabledLL();
        
        
         void UT_CMccVideoSourceImpl_SetGainLL();
        
        
         void UT_CMccVideoSourceImpl_GainLL();
        
        
         void UT_CMccVideoSourceImpl_MaxGainLL();
        
        
         void UT_CMccVideoSourceImpl_SourceDataTypeCodeL();
        
        
         void UT_CMccVideoSourceImpl_SetSourceDataTypeCodeL();
        
        
         void UT_CMccVideoSourceImpl_BufferEmptiedLL();
        
        
         void UT_CMccVideoSourceImpl_CanCreateSourceBufferL();
        
        
         void UT_CMccVideoSourceImpl_CreateSourceBufferLL();
        
        
         void UT_CMccVideoSourceImpl_SourceThreadLogonL();
        
        
         void UT_CMccVideoSourceImpl_SourceThreadLogoffL();
        
        
         void UT_CMccVideoSourceImpl_SourcePrimeLL();
        
         void UT_CMccVideoSourceImpl_SourcePrimeL2L();
         
         void UT_CMccVideoSourceImpl_SourcePrimeL3L();

         void UT_CMccVideoSourceImpl_SourcePrimeL4L();
        
        
         void UT_CMccVideoSourceImpl_SourcePlayLL();
        
        
         void UT_CMccVideoSourceImpl_SourcePauseLL();
        
        
         void UT_CMccVideoSourceImpl_SourceStopLL();
        
        
         void UT_CMccVideoSourceImpl_FillBufferLL();
        
        
         void UT_CMccVideoSourceImpl_NegotiateSourceLL();
        
        
         void UT_CMccVideoSourceImpl_WriteBufferLL();
        
        
         void UT_CMccVideoSourceImpl_SetVideoFrameSizeL();
        
        
         void UT_CMccVideoSourceImpl_SetAverageVideoBitRateL();
        
        
         void UT_CMccVideoSourceImpl_SetMaxVideoBitRateL();
        
        
         void UT_CMccVideoSourceImpl_SetAverageAudioBitRateL();
        
        
         void UT_CMccVideoSourceImpl_MmroPrepareCompleteL();
        
        
         void UT_CMccVideoSourceImpl_MmroStateChangeL();
        
        
         void UT_CMccVideoSourceImpl_MmroTemporaryErrorL();
        
        
         void UT_CMccVideoSourceImpl_MmroFatalErrorL();
        
         void UT_CMccVideoSourceImpl_CameraReadyL();
         
         void UT_CMccVideoSourceImpl_IsStoppedL();

         void UT_CMccVideoSourceImpl_RateAdaptationRequestL();
         
         void UT_CMccVideoSourceImpl_CallBackTestL();
         
         void UT_CMccVideoSourceImpl_GetConfigKeyTestL();
         
         void UT_CMccVideoSourceImpl_SetConfigKeyTestL();
    
    private: // Helpers
    
        void AddConfKeyMarkerL( TDes8& aBuf, TInt aStartIndex );     
        
    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        
        CMccVideoSourceImpl* iSourceImpl;
        CMMFDataBuffer*      iBuffer;
        CCMRMediaBuffer*     iMediaBuffer;
        CMccUlDataPath*      iULDataPathStub;
        CMccCameraHandlerStub* iCameraHandler;
        CMccResourcePoolStub* iMccResources;
        CMccTestEventHandler* iEventHandler;
    };

#endif      //  __UT_CMccVIDEOSOURCEIMPL_H__

// End of file
