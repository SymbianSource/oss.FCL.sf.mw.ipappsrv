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




#ifndef __UT_CMccREDPAYLOADREAD_H__
#define __UT_CMccREDPAYLOADREAD_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <mmf/common/mmfcontroller.h>
#include <mmf/server/mmfformat.h>
#include <e32base.h>
#include "rtpapi.h"

//  INTERNAL INCLUDES

//  FORWARD DECLARATIONS
class CMccRedPayloadRead;
class MDataSource;
class CMccRtpDataSource;
class TRtpRecvHeader;

const TInt KILBCFrameSize20Ms = 38;
const TUint8 KILBCTestVector20Ms[KILBCFrameSize20Ms] = 
    {
    0xFC, 0x7F, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x20,
    0xFC, 0x7F, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x20,
    0xFC, 0x7F, 0xFF, 0xFF, 0xFF, 0xFD, 0xF7, 0x48, 
    0x59, 0xD4, 0xCC, 0x80, 0xFC, 0x7F, 0xFF, 0xFF, 
    0xFF, 0xFD, 0xF7, 0x48, 0x59, 0xD4
    };
    
const TInt KComfortNoiseSize = 16;
const TUint8 KComfortNoise10Ms[KComfortNoiseSize] =
    {
    0xFC, 0x7F, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x20,
    0xFC, 0x7F, 0xFF, 0xFF, 0xFF, 0xFD, 0xF7, 0x48
    };


//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMccRedPayloadRead : public CEUnitTestSuiteClass, public MAsyncEventHandler
 
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccRedPayloadRead* NewL();
        static UT_CMccRedPayloadRead* NewLC();
        
        /**
         * Destructor
         */
        ~UT_CMccRedPayloadRead();

    private:    // Constructors and destructors

        UT_CMccRedPayloadRead();
        void ConstructL();

	public: 	// From MAsyncEventHandler
	
        TInt SendEventToClient( const TMMFEvent& /*aEvent*/ ) { return KErrNone; }
		
    private:    // New methods

         void SetupAL();
         
         void Setup2L();
         
         void Setup2AL();
         
         void SetupL();
         
         void Teardown();
        
         void UT_CMccRedPayloadRead_SetPayloadTypesL();
        
        
         void UT_CMccRedPayloadRead_BufferFilledLL();
         
         
         void UT_CMccRedPayloadRead_ConfigurePayloadFormatLL(); // tälle ceissi?
        
        
         void UT_CMccRedPayloadRead_StreamsL();
        
        
         void UT_CMccRedPayloadRead_FrameTimeIntervalL();
        
        
         void UT_CMccRedPayloadRead_DurationL();
        
        
         void UT_CMccRedPayloadRead_SourceDataTypeCodeL();
        
        
         void UT_CMccRedPayloadRead_SetSourceDataTypeCodeL();
        
        
         void UT_CMccRedPayloadRead_CanCreateSourceBufferL();
        
        
         void UT_CMccRedPayloadRead_CreateSourceBufferLL();
        
        
         void UT_CMccRedPayloadRead_FillBufferLL();
        
        
         void UT_CMccRedPayloadRead_SourceThreadLogonL();
        
        
         void UT_CMccRedPayloadRead_SourceThreadLogoffL();
        
        
         void UT_CMccRedPayloadRead_SourcePrimeLL();
        
        
         void UT_CMccRedPayloadRead_SourcePlayLL();
        
        
         void UT_CMccRedPayloadRead_SourcePauseLL();
        
        
         void UT_CMccRedPayloadRead_SourceStopLL();
        
        
         void UT_CMccRedPayloadRead_NegotiateSourceLL();
        
        
         void UT_CMccRedPayloadRead_SinkDataTypeCodeL();
        
    private:    // Data
        
		EUNIT_DECLARE_TEST_TABLE; 
		
		TBool iAllocTest;
		TFourCC iFourCC;
		CMMFDataBuffer* iDataBuf;
		MDataSource* iDataSourceStub;
		MDataSink* iDataSinkStub;
        CMccRedPayloadRead* iRead;
    
    };

#endif      //  __UT_CMccREDPAYLOADREAD_H__

// End of file
