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




#ifndef __UT_CMCEVIDEOSDPCODEC_H__
#define __UT_CMCEVIDEOSDPCODEC_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include "MceMediaManagerTestHelper.h"

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS

class CMceVideoSdpCodec;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMceVideoSdpCodec : public CEUnitTestSuiteClass
    {
public:     // Constructors and destructors

    static UT_CMceVideoSdpCodec* NewL();
    static UT_CMceVideoSdpCodec* NewLC();
    ~UT_CMceVideoSdpCodec();

private:    // Constructors and destructors

    UT_CMceVideoSdpCodec();
    void ConstructL();

private:    // EUNIT methods

     void SetupL();
     void Teardown();

private:    // Test methods
    
    void UT_CMceVideoSdpCodec_EncodesL();
    void UT_CMceVideoSdpCodec_CodecsLL();
    void UT_CMceVideoSdpCodec_EncodeRtpmapAttributeLCL();
    void UT_CMceVideoSdpCodec_EncodeMediaAttributesLL();
    void UT_CMceVideoSdpCodec_DecodeMediaAttributesLL();
    void UT_CMceVideoSdpCodec_CreateStreamLCL();
    void UT_CMceVideoSdpCodec_UpdateStreamLL();
    void UT_CMceVideoSdpCodec_CreateCodecLCL();
    void UT_CMceVideoSdpCodec_DecodeSessionMediaAttributesLL();


private:    // Data

    CMceVideoSdpCodec* iSdpCodec;
    
	CMceMediaManager* iManager;
	CMceComSession* iSession;
	CMceServerStub* iServer;

	EUNIT_DECLARE_TEST_TABLE; 
		
    };


#endif      //  __UT_CMCEVIDEOSDPCODEC_H__

// End of file
