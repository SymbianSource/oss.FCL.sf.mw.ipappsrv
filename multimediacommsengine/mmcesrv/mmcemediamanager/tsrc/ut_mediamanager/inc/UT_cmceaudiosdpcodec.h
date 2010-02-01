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




#ifndef __UT_CMCEAUDIOSDPCODEC_H__
#define __UT_CMCEAUDIOSDPCODEC_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include "MceMediaManagerTestHelper.h"

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS

class CMceAudioSdpCodec;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMceAudioSdpCodec : public CEUnitTestSuiteClass
    {
public:     // Constructors and destructors

    static UT_CMceAudioSdpCodec* NewL();
    static UT_CMceAudioSdpCodec* NewLC();
    ~UT_CMceAudioSdpCodec();

private:    // Constructors and destructors

    UT_CMceAudioSdpCodec();
    void ConstructL();

private:    // EUNIT methods

     void SetupL();
     void Teardown();

private:    // Test methods
    

    void UT_CMceAudioSdpCodec_EncodesL();
    void UT_CMceAudioSdpCodec_CodecsLL();
    void UT_CMceAudioSdpCodec_EncodeRtpmapAttributeLCL();
    void UT_CMceAudioSdpCodec_EncodeMediaAttributesLL();
    void UT_CMceAudioSdpCodec_DecodeMediaAttributesLL();
    void UT_CMceAudioSdpCodec_CreateStreamLCL();
    void UT_CMceAudioSdpCodec_UpdateStreamLL();
    void UT_CMceAudioSdpCodec_CreateCodecLCL();
    void UT_CMceAudioSdpCodec_DecodeSessionMediaAttributesLL();
    

private:    // Data

    CMceAudioSdpCodec* iSdpCodec;
    
	CMceMediaManager* iManager;
	CMceComSession* iSession;
	CMceServerStub* iServer;

	EUNIT_DECLARE_TEST_TABLE; 
		
    };


#endif      //  __UT_CMCEAUDIOSDPCODEC_H__

// End of file
