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




#ifndef __UT_CDTMFPAYLOADFORMATREAD_H__
#define __UT_CDTMFPAYLOADFORMATREAD_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES

#include <mmf/common/mmfcontroller.h>



#include <e32base.h>
#include "rtpheader.h"
#include "rtpapi.h"
#include "mccdef.h"  
//#include "dtmfpayloadformatdefs.h"
#include "mccinternalcodecs.h"


//  FORWARD DECLARATIONS
class CDTMFPayloadFormatRead;
class CDTMFPayloadDecoder;
class MDataSource;
class CMccRtpDataSource;
class CMccRtpManager;
class TRtpRecvHeader;
class TStreamDecoder;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CDTMFPayloadFormatRead : public CEUnitTestSuiteClass, 
                                  public MAsyncEventHandler
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CDTMFPayloadFormatRead* NewL();
        static UT_CDTMFPayloadFormatRead* NewLC();
        /**
         * Destructor
         */
        ~UT_CDTMFPayloadFormatRead();

    private:    // Constructors and destructors

        UT_CDTMFPayloadFormatRead();
        void ConstructL();
        CDTMFPayloadFormatRead* iRead;

	public: 	// From observer interface


    private:    // New methods
    
         void SetupL();
        
         void Teardown();
        
         void UT_CDTMFPayloadFormatRead_NewLL();
         
         void UT_CDTMFPayloadFormatRead_HandleError();
              
         void UT_CDTMFPayloadFormatRead_PayloadFormatL();
        
         void UT_CDTMFPayloadFormatRead_SetPayloadFormatL();
        
         void UT_CDTMFPayloadFormatRead_AddMMFAudioOutputLL();
        
         void UT_CDTMFPayloadFormatRead_BufferFilledLL();
        
         void UT_CDTMFPayloadFormatRead_CanCreateSourceBufferL();
        
         void UT_CDTMFPayloadFormatRead_StreamsL();
        
         void UT_CDTMFPayloadFormatRead_FrameTimeIntervalL();
        
         void UT_CDTMFPayloadFormatRead_DurationL();
        
         void UT_CDTMFPayloadFormatRead_FillBufferLL();
        
         void UT_CDTMFPayloadFormatRead_CreateSourceBufferLL();
        
         void UT_CDTMFPayloadFormatRead_SourceDataTypeCodeL();
        
         void UT_CDTMFPayloadFormatRead_SinkDataTypeCodeL();
         
         void UT_CDTMFPayloadFormatRead_SetSourcePrioritySettings();

         void UT_CDTMFPayloadFormatRead_SourcePrimeLL();
        
         void UT_CDTMFPayloadFormatRead_SourcePlayLL();
        
         void UT_CDTMFPayloadFormatRead_SourcePauseLL();
        
         void UT_CDTMFPayloadFormatRead_SourceStopLL();
        
         void UT_CDTMFPayloadFormatRead_SourceThreadLogonL();
        
         void UT_CDTMFPayloadFormatRead_SourceThreadLogoffL();
        
         void UT_CDTMFPayloadFormatRead_HandleDTMFEventsL();
         
         void UT_CDTMFPayloadFormatRead_HandleDTMFTones();
         
         void UT_CDTMFPayloadFormatRead_SendDataToSinkLL();
        
         void UT_CDTMFPayloadFormatRead_FillSinkBufferL();
        
         void UT_CDTMFPayloadFormatRead_FillSourceBufferLL();
             
         void UT_CDTMFPayloadFormatRead_MatoPrepareCompleteL();
    
         void UT_CDTMFPayloadFormatRead_MatoPlayCompleteL();
         
         void UT_CDTMFPayloadFormatRead_ConfigurePayloadFormatLL();
         
         void UT_CDTMFPayloadFormatRead_CreateClipBufferLL();
         
         void UT_CDTMFPayloadFormatRead_CompareFirstPacketRtpHeaders();
        
         // VIRTUALS
    
         TInt SendEventToClient(const TMMFEvent& /*aEvent*/) { return KErrNone; };

    private:    // Data

		EUNIT_DECLARE_TEST_TABLE;
	    
		MDataSource* iRtpDataSourceStub;
		
        TRtpRecvHeader iHeaderInfo;
		
		CMMFDescriptorBuffer* iBuffi;
		
		TFourCC iFourCC;
		
		MAsyncEventHandler* iEventH;
		
    	TMccCodecInfo iCodec;

    };

#endif      //  __UT_CDTMFPAYLOADFORMATREAD_H__

// End of file
