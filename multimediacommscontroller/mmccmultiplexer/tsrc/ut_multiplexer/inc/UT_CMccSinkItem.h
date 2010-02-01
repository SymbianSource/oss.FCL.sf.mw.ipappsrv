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




#ifndef __UT_CMCCSINKITEM_H__
#define __UT_CMCCSINKITEM_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMccSinkItem;
class CMccRtpDataSink;
class CMccRtpDataSource;
class CAmrPayloadFormatRead;
class MDataSink;
class CMccTestEventHandler;
class CMccDlDataPath;
class CMMFDataBuffer;

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
NONSHARABLE_CLASS( UT_CMccSinkItem )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccSinkItem* NewL();
        static UT_CMccSinkItem* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccSinkItem();

    private:    // Constructors and destructors

        UT_CMccSinkItem();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

         void SetupL();
         
         void Setup2L();
        
         void Teardown();
         
         void Teardown2();
        
         void UT_CMccSinkItem_SinkL();
                  
         void UT_CMccSinkItem_Sink2L();
  
         void UT_CMccSinkItem_CopyBufferL();
         
         void UT_CMccSinkItem_IsInternalDecoderL();

         void UT_CMccSinkItem_BufferFilledLL();
        
         void UT_CMccSinkItem_BufferFilledL2L();

         void UT_CMccSinkItem_BufferFilledL3L();

         void UT_CMccSinkItem_FillBufferRequestLL();
        
         void UT_CMccSinkItem_StoreBufferL();
         
         void UT_CMccSinkItem_GetStoredBufferL();
         
         void UT_CMccSinkItem_MatchL();
        
         void UT_CMccSinkItem_Match2L();

         void UT_CMccSinkItem_SourceThreadLogonL();
        
        
         void UT_CMccSinkItem_SendEventToClientL();
        
        
         void UT_CMccSinkItem_MediaTypeL();
        
        
         void UT_CMccSinkItem_SetSourceModeL();
        
        
         void UT_CMccSinkItem_PassAllRequestsToSourceL();
        
        
         void UT_CMccSinkItem_ResolveMediaTypeL();
         
         void UT_CMccSinkItem_RegisterPayloadTypesLL( );
         
         void UT_CMccSinkItem_RegisteredPayloadTypesL();
        

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        
        CAmrPayloadFormatRead* iRead;
        CMccRtpDataSink*       iRtpDataSinkStub;
        CMccRtpDataSource*     iRtpDataSourceStub;
        CMccSinkItem*          iSinkitem;
        CMccTestEventHandler*  iEventHandler;
        CMccDlDataPath*        iDLDataPathStub;

    };

#endif      //  __UT_CMCCSINKITEM_H__

// End of file
