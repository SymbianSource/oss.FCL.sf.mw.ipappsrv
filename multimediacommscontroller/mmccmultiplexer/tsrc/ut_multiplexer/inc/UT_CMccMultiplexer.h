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




#ifndef __UT_CMCCMULTIPLEXER_H__
#define __UT_CMCCMULTIPLEXER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMccMultiplexer;
class CAmrPayloadFormatRead;
class CMccSinkItem;
class CMccDlDataPath;
class CMccTestEventHandler;
class MDataSource;

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
NONSHARABLE_CLASS( UT_CMccMultiplexer )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccMultiplexer* NewL();
        static UT_CMccMultiplexer* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccMultiplexer();

    private:    // Constructors and destructors

        UT_CMccMultiplexer();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

         void SetupL();
         
         void Setup2L();
        
         void Teardown();
         
         void UT_CMccMultiplexer_PassAllRequestsToSource();

         void UT_CMccMultiplexer_AddSinkL();

         void UT_CMccMultiplexer_FindSinkItemL();
    
         void UT_CMccMultiplexer_FindSinkItemIndex();
        
         void UT_CMccMultiplexer_IsPrimarySink();
        
         void UT_CMccMultiplexer_DoBufferFilledL();
                                      
        
         void UT_CMccMultiplexer_SetMultiplexerState();
        
         void UT_CMccMultiplexer_SendStreamEventToClient();
        
         void UT_CMccMultiplexer_UpdateSinkModes();
        
        
         void UT_CMccMultiplexer_IsSupportedMediaType();
        
         void UT_CMccMultiplexer_SendEventToAllClients();
        
         void UT_CMccMultiplexer_SourceThreadLogonL();
        
        
         void UT_CMccMultiplexer_SourceThreadLogoffL();
         
         void UT_CMccMultiplexer_SourceThreadLogoff2L();
        
         void UT_CMccMultiplexer_SetCurrentUserLL();
         
         
         void UT_CMccMultiplexer_RegClientForPayloadsLL();
        
        
         void UT_CMccMultiplexer_StreamsL();
        
        
         void UT_CMccMultiplexer_FrameTimeIntervalL();
        
        
         void UT_CMccMultiplexer_DurationL();
        
        
         void UT_CMccMultiplexer_CanCreateSourceBufferL();
        
        
         void UT_CMccMultiplexer_CreateSourceBufferLL();
         
         void UT_CMccMultiplexer_SuggestSourceBufferSizeL();
        
        
         void UT_CMccMultiplexer_DataSourceTypeL();
        
        
         void UT_CMccMultiplexer_FillBufferLL();
         
         void UT_CMccMultiplexer_FillBufferL2L();
        
        
         void UT_CMccMultiplexer_SourcePrimeLL();
        
        
         void UT_CMccMultiplexer_SourcePlayLL();
        
        
         void UT_CMccMultiplexer_SourcePauseLL();
        
        
         void UT_CMccMultiplexer_SourceStopLL();
        
        
         void UT_CMccMultiplexer_SourceDataTypeCodeL();
        
        
         void UT_CMccMultiplexer_SetSourceDataTypeCodeL();
        
      
        
         void UT_CMccMultiplexer_NegotiateSourceLL();
        
        
         void UT_CMccMultiplexer_BufferFilledLL();
        
        
         void UT_CMccMultiplexer_ConfigurePayloadFormatLL();
        
        
         void UT_CMccMultiplexer_SetPayloadSizeL();        
        
         void UT_CMccMultiplexer_SendEventToClientL();
        

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
                
        MDataSource*     iRtpDataSourceStub;
        CMccMultiplexer*       iMultiplexer;
        CAmrPayloadFormatRead* iRead;
        CMccSinkItem*          iSinkItem;
        CMccDlDataPath*        iDLDataPathStub;
        CMccTestEventHandler*  iEventHandler;
        MDataSource*     iFileSource;


    };

#endif      //  __UT_CMCCMULTIPLEXER_H__

// End of file
