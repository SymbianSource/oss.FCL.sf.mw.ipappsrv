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




#ifndef __UT_CMCESDPCODEC_H__
#define __UT_CMCESDPCODEC_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include "MceMediaManagerTestHelper.h"

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS

class CMceSdpCodec;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMceSdpCodec : public CEUnitTestSuiteClass
    {
public:     // Constructors and destructors

    static UT_CMceSdpCodec* NewL();
    static UT_CMceSdpCodec* NewLC();
    ~UT_CMceSdpCodec();

private:    // Constructors and destructors

    UT_CMceSdpCodec();
    void ConstructL();

private:    // EUNIT methods

     void SetupL();
     void Teardown();

private:    // Test methods
    
    void UT_CMceSdpCodec_MediaL();
    void UT_CMceSdpCodec_DecodesL();
    void UT_CMceSdpCodec_AddCapabilityLL();
    void UT_CMceSdpCodec_IsSupportedL();
    void UT_CMceSdpCodec_PrepareForEncodeLL();
    void UT_CMceSdpCodec_EncodeRtpMapFieldsLL();

    void UT_CMceSdpCodec_DirectionL();
    void UT_CMceSdpCodec_ConvertDesToUintLL();
    void UT_CMceSdpCodec_ConvertDesToIntLL();
    void UT_CMceSdpCodec_ConvertTRealToDesLCL();
    void UT_CMceSdpCodec_ConvertDesToRealLL();
    void UT_CMceSdpCodec_IsPayloadTypeSupportedL();
    void UT_CMceSdpCodec_AddCapabilityPayloadTypeLL();	


private:    // Data

    CMceSdpCodec* iSdpCodec;
    
	CMceMediaManager* iManager;
	CMceComSession* iSession;
	CMceServerStub* iServer;

	EUNIT_DECLARE_TEST_TABLE; 
		
    };


#endif      //  __UT_CMCESDPCODEC_H__

// End of file
