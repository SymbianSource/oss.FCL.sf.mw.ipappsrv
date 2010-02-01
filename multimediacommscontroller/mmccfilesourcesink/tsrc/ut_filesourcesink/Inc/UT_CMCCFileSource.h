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




#ifndef __UT_CMccFILESOURCE_H__
#define __UT_CMccFILESOURCE_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMccFileSource;
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
NONSHARABLE_CLASS( UT_CMccFileSource )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccFileSource* NewL();
        static UT_CMccFileSource* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccFileSource();

    private:    // Constructors and destructors

        UT_CMccFileSource();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

         void SetupL();
         
         void Setup2L();
        
         void Teardown();
        
         void UT_CMccFileSource_NewSourceLL();
        
        
         void UT_CMccFileSource_ConstructSourceLL();
        
        
         void UT_CMccFileSource_CMccFileSourceL();
        
        
         void UT_CMccFileSource_OpenFileLL();
        
        
         void UT_CMccFileSource_CloseFileLL();
        
        
         void UT_CMccFileSource_SetPositionLL();
        
        
         void UT_CMccFileSource_VideoFrameRateLL();
        
        
         void UT_CMccFileSource_AudioBitRateLL();
        
        
         void UT_CMccFileSource_DurationLL();
        
        
         void UT_CMccFileSource_VideoFrameSizeLL();
        
        
         void UT_CMccFileSource_VideoBitRateLL();
        
        
         void UT_CMccFileSource_PositionLL();
        
        
         void UT_CMccFileSource_SourceDataTypeCodeL();
        
        
         void UT_CMccFileSource_SetSourceDataTypeCodeL();
        
        
         void UT_CMccFileSource_BufferEmptiedLL();
        
        
         void UT_CMccFileSource_CanCreateSourceBufferL();
        
        
         void UT_CMccFileSource_CreateSourceBufferLL();
        
        
         void UT_CMccFileSource_SourceThreadLogonL();
        
        
         void UT_CMccFileSource_SourceThreadLogoffL();
        
        
         void UT_CMccFileSource_SourcePrimeLL();
        
        
         void UT_CMccFileSource_SourcePlayLL();
        
        
         void UT_CMccFileSource_SourcePauseLL();
        
        
         void UT_CMccFileSource_SourceStopLL();
        
        
         void UT_CMccFileSource_FillBufferLL();
        
        
         void UT_CMccFileSource_NegotiateSourceLL();
         
         void UT_CMccFileSource_SetParameterLL();
         
         void UT_CMccFileSource_GetParameterLL();
         
         void UT_CMccFileSource_ImplNotReadyL();
        

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 

        CMMFDataBuffer*  iBuffer;
        CMccUlDataPath*  iAudioULDPStub;
        CMccUlDataPath*  iVideoULDPStub;
        CMccFileSource*  iFileSource;


    };

#endif      //  __UT_CMccFILESOURCE_H__

// End of file
