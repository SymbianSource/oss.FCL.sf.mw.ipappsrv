/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef __UT_CMCESRVSTREAM_H__
#define __UT_CMCESRVSTREAM_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include "MceMediaManagerTestHelper.h"

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS

class CMccControllerStub;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMceSrvStream : public CEUnitTestSuiteClass
    {
public:     // Constructors and destructors

    static UT_CMceSrvStream* NewL();
    static UT_CMceSrvStream* NewLC();
    ~UT_CMceSrvStream();

private:    // Constructors and destructors

    UT_CMceSrvStream();
    void ConstructL();

private:    // EUNIT methods

     void SetupL();
     void Setup2L();
     void Teardown();

private:    // Test methods
    

    void UT_CMceSrvStream_DecodeLL();
    void UT_CMceSrvStream_EventReceived_Static_L();
    void UT_CMceSrvStream_SessionIdL();
    void UT_CMceSrvStream_LinkIdL();
    void UT_CMceSrvStream_SetLinkIdL();
    void UT_CMceSrvStream_IapIdL();
    void UT_CMceSrvStream_LocalMediaPortL();
    void UT_CMceSrvStream_EventReceivedL();
    void UT_CMceSrvStream_CleanupL();
    void UT_CMceSrvStream_IsEqualL();
    void UT_CMceSrvStream_MergeL();
    void UT_CMceSrvStream_IsMccPreparedL();
    void UT_CMceSrvStream_StateL();
    void UT_CMceSrvStream_InvalidateL();
    void UT_CMceSrvStream_IsAdoptedL();
    void UT_CMceSrvStream_SetLocalMediaPortL();
    void UT_CMceSrvStream_IdL();
    void UT_CMceSrvStream_StreamTypeL();
    void UT_CMceSrvStream_DirectionL();
    void UT_CMceSrvStream_IsMergedL();
    void UT_CMceSrvStream_DataL();
    void UT_CMceSrvStream_RemoteIpAddressL();
    void UT_CMceSrvStream_CodecL();
    void UT_CMceSrvStream_SourceL();
    void UT_CMceSrvStream_SinkL();
    void UT_CMceSrvStream_MccStreamTypeL();
    void UT_CMceSrvStream_LinkTypeL();
    void UT_CMceSrvStream_ConsumesL();
    void UT_CMceSrvStream_PrepareLL();
    void UT_CMceSrvStream_StartLL();
    void UT_CMceSrvStream_SynchronizeLL();
    void UT_CMceSrvStream_StopL();
    void UT_CMceSrvStream_ValidateL();
    void UT_CMceSrvStream_UnMergeL();
    void UT_CMceSrvStream_RequireSignallingL();
    void UT_CMceSrvStream_RequireSignalling_1L();
    void UT_CMceSrvStream_MccPrepareCalledLL();
    void UT_CMceSrvStream_UseDefaultStartupSequenceL();


private:    // Data

    CMceSrvStream* iStream;
    
	CMceMediaManager* iManager;
	CMceComSession* iSession;
	CMceServerStub* iServer;
	CMccControllerStub* iMcc;
	

	EUNIT_DECLARE_TEST_TABLE; 
		
    };


#endif      //  __UT_CMCESRVSTREAM_H__

// End of file
