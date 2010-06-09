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




#ifndef __UT_CMCEMEDIASDPCODEC_H__
#define __UT_CMCEMEDIASDPCODEC_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include "MceMediaManagerTestHelper.h"

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS

class CMceMediaSdpCodec;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMceMediaSdpCodec : public CEUnitTestSuiteClass
    {
public:     // Constructors and destructors

    static UT_CMceMediaSdpCodec* NewL();
    static UT_CMceMediaSdpCodec* NewLC();
    ~UT_CMceMediaSdpCodec();

private:    // Constructors and destructors

    UT_CMceMediaSdpCodec();
    void ConstructL();

private:    // EUNIT methods

     void SetupL();
     void Setup2L();
     void Teardown();

private:    // Test methods
    
    void UT_CMceMediaSdpCodec_EncodeMediaOfferLL();
    void UT_CMceMediaSdpCodec_DecodeMediaAnswerLL();
    void UT_CMceMediaSdpCodec_DecodeMediaOfferLL();
    void UT_CMceMediaSdpCodec_DecodeMediaUpdateLL();
    void UT_CMceMediaSdpCodec_EncodeMediaAnswerLL();
    void UT_CMceMediaSdpCodec_CleanAttributesL();
    void UT_CMceMediaSdpCodec_PrepareForDecodeLL();
    
    void UT_CMceMediaSdpCodec_EncodeClientAttributesLL();
    void UT_CMceMediaSdpCodec_DecodeClientAttributesLL();
    void UT_CMceMediaSdpCodec_DecodeRemoteRtcpFieldLL();
    void UT_CMceMediaSdpCodec_EncodelocalRtcpAttrLL();
    void UT_CMceMediaSdpCodec_TypeL();
    void UT_CMceMediaSdpCodec_DecodePreconditionsLL();
    void UT_CMceMediaSdpCodec_EncodePreconditionsLL();
    void UT_CMceMediaSdpCodec_EncodeFmtpAttributeLL();
    void UT_CMceMediaSdpCodec_DecodeFmtpLinesLL();
    void UT_CMceMediaSdpCodec_ValidateSdpL();
    void UT_CMceMediaSdpCodec_DecodeDirectionLL(); 
    void UT_CMceMediaSdpCodec_DecodeRtpmapLinesLL();


private:    // Data

    CMceMediaSdpCodec* iSdpCodec;
    
	CMceMediaManager* iManager;
	CMceComSession* iSession;
	CMceServerStub* iServer;

	EUNIT_DECLARE_TEST_TABLE; 
		
    };


#endif      //  __UT_CMCEMEDIASDPCODEC_H__

// End of file
