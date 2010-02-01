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




#ifndef __UT_CILBCPAYLOADFORMATREAD_H__
#define __UT_CILBCPAYLOADFORMATREAD_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include <mmf/common/mmfcontroller.h>
#include <mmf/server/mmfformat.h>
#include <e32base.h>
#include "rtpapi.h"
#include "mccdef.h" 
#include "mccinternaldef.h" // MMccEventHandler definition  
#include "mccinternalcodecs.h"

//  FORWARD DECLARATIONS
class CIlbcPayloadFormatRead;
class MDataSource;
class CMccDlDataPath;
class CMccRtpDataSource;


//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CIlbcPayloadFormatRead
     : public CEUnitTestSuiteClass, public MAsyncEventHandler
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CIlbcPayloadFormatRead* NewL();
        static UT_CIlbcPayloadFormatRead* NewLC();
        /**
         * Destructor
         */
        ~UT_CIlbcPayloadFormatRead();

    private:    // Constructors and destructors

        UT_CIlbcPayloadFormatRead();
        void ConstructL();
        CIlbcPayloadFormatRead* iRead;

    private:    // New methods

         void SetupAL();
         
         void SetupL();
         
         void Teardown();
         
        
         void UT_CIlbcPayloadFormatRead_NewLL();
        
        
         void UT_CIlbcPayloadFormatRead_LastDlPacketCountL();
               
         
         void UT_CIlbcPayloadFormatRead_SendDataToSinkLL();
        
        
         void UT_CIlbcPayloadFormatRead_FillSinkBufferL();
        
        
         void UT_CIlbcPayloadFormatRead_FillSourceBufferLL();
        
        
         void UT_CIlbcPayloadFormatRead_StreamsL();
        
        
         void UT_CIlbcPayloadFormatRead_FrameTimeIntervalL();
        
        
         void UT_CIlbcPayloadFormatRead_DurationL();
        
        
         void UT_CIlbcPayloadFormatRead_NumChannelsL();
        
        
         void UT_CIlbcPayloadFormatRead_SampleRateL();
        
        
         void UT_CIlbcPayloadFormatRead_SetSampleRateL();
        
        
         void UT_CIlbcPayloadFormatRead_SourceDataTypeCodeL();
        
        
         void UT_CIlbcPayloadFormatRead_SetSourceDataTypeCodeL();
        
        
         void UT_CIlbcPayloadFormatRead_CreateSourceBufferLL();
        
        
         void UT_CIlbcPayloadFormatRead_CreateSourceBufferL_1L();
        
        
         void UT_CIlbcPayloadFormatRead_FillBufferLL();
        
        
         void UT_CIlbcPayloadFormatRead_SourceThreadLogonL();
        
        
         void UT_CIlbcPayloadFormatRead_SourceThreadLogoffL();
        
        
         void UT_CIlbcPayloadFormatRead_NegotiateSourceLL();
        
        
         void UT_CIlbcPayloadFormatRead_SourcePrimeLL();
        
        
         void UT_CIlbcPayloadFormatRead_SourcePlayLL();
        
        
         void UT_CIlbcPayloadFormatRead_SourcePauseLL();
        
        
         void UT_CIlbcPayloadFormatRead_SourceStopLL();
        
        
         void UT_CIlbcPayloadFormatRead_BufferFilledLL();
        
         
         void UT_CIlbcPayloadFormatRead_SinkDataTypeCodeL();
         
         void UT_CIlbcPayloadFormatRead_ConfigurePayloadFormatLL();
         
         void UT_CIlbcPayloadFormatRead_UpdateConfigurationL();

       
        
    // VIRTUAALIT
    
         virtual TInt SendEventToClient( const TMMFEvent& /*aEvent*/ ) { return KErrNone; }
         
         virtual TInt SendMccEventToClient( TMccEvent& /*aEvent*/ ) { return KErrNone; }
         
         virtual void StateChange( TInt /*aState*/, TUint32 /*aLink*/ ) {}
         
         virtual void ErrorOccured( TInt /*aError*/, 
                                    TUint32 /*aSessionId*/,
                                    TUint32 /*aLinkId*/,
            	                    TUint32 /*aStreamId*/, 
            	                    TUint32 /*aEndpointId*/ ) {}
	
    
    
    private:    // Data

		EUNIT_DECLARE_TEST_TABLE; 
		
		TBool iAlloc;
		
    	MDataSource*            iRtpDataSourceStub;
	    CMccDlDataPath*         iDLDataPathStub;
		CMMFDataBuffer*   iBuffi;

        TMediaId iMediaId;
		TFourCC iFourCC;
		MAsyncEventHandler* aEventH;		
		TMccCodecInfo iCodec;
		MDataSink *iConsumer;
    };

#endif      //  __UT_CILBCPAYLOADFORMATREAD_H__

// End of file
