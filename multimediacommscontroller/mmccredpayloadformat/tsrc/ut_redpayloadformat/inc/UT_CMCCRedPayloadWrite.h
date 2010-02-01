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




#ifndef __UT_CMccREDPAYLOADWRITE_H__
#define __UT_CMccREDPAYLOADWRITE_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include <mmf/common/mmfcontroller.h>
#include <mmf/server/mmfformat.h>
#include <e32base.h>
#include "rtpapi.h"


//  FORWARD DECLARATIONS
class CMccRedPayloadWrite;
class MDataSink;
class CMccRtpDataSink;
class CMccRedEncoder;
class CMccRedDecoder;
class CMccRtpMediaClock;

// CONSTANTS
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
class UT_CMccRedPayloadWrite
     : public CEUnitTestSuiteClass, public MAsyncEventHandler
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccRedPayloadWrite* NewL();
        static UT_CMccRedPayloadWrite* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccRedPayloadWrite();

    private:    // Constructors and destructors

        UT_CMccRedPayloadWrite();
        void ConstructL();

	public: 	// From observer interface
		
		

    private:    // New methods

         void SetupL();
         
         void SetupAL();
        
         void Teardown();
         
         void UT_CMccRedPayloadWrite_NewLL();
        
         void UT_CMccRedPayloadWrite_SetPayloadTypesL();
        
        
         void UT_CMccRedPayloadWrite_ConfigurePayloadFormatLL();


         void UT_CMccRedPayloadWrite_EmptyBufferLL();
        
        
         void UT_CMccRedPayloadWrite_FrameTimeIntervalL();
        
        
         void UT_CMccRedPayloadWrite_DurationL();
        
        
         void UT_CMccRedPayloadWrite_CreateSinkBufferLL();
        
        
         void UT_CMccRedPayloadWrite_SinkDataTypeCodeL();
        
        
         void UT_CMccRedPayloadWrite_SetSinkDataTypeCodeL();
        
        
         void UT_CMccRedPayloadWrite_SinkThreadLogonL();
        
        
         void UT_CMccRedPayloadWrite_SinkThreadLogoffL();
        
        
         void UT_CMccRedPayloadWrite_SinkPrimeLL();
        
        
         void UT_CMccRedPayloadWrite_SinkPlayLL();
        
        
         void UT_CMccRedPayloadWrite_SinkPauseLL();
        
        
         void UT_CMccRedPayloadWrite_SinkStopLL();
       
        
         void UT_CMccRedPayloadWrite_BufferEmptiedLL();
        

// VIRTUALS

         virtual TInt SendEventToClient( const TMMFEvent& /*aEvent*/ ) { return KErrNone; }
    
    private:    // Data

        TBool iAllocTest;
        CMccRedPayloadWrite* iWrite;
        MDataSink* iDataSinkStub;
        CMMFDataBuffer* iBuffer;
        MDataSource* iDataSourceStub;
        CMccRtpMediaClock* iRtpMediaClock;

		EUNIT_DECLARE_TEST_TABLE; 
		
    };

#endif      //  __UT_CMccREDPAYLOADWRITE_H__

// End of file
