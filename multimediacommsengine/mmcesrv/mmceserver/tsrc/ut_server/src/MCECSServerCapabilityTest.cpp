/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "MceCsServerCapabilityTest.h"
#include <cppunit/TestCaller.h>
#include <cppunit/TestSuite.h>
#include "mceclientserver.h"
	

#define CAP_CHECK(err,aIpcFunc) \
    assertTIntsEqualL(err,iServerCaller->Send(aIpcFunc));

	
#ifndef EKA2
// Starting point of the DLL (Removed in EKA2) 
GLDEF_C TInt E32Dll(TDllReason)
	{
	return(KErrNone);
	}
#endif

// Exactly one exported function returning the suite of 
// test functions for the test runner in the framework (Always needed).
//
EXPORT_C MTest* CreateTestL ()
	{
	return CMceCsServerCapabilityTest::suiteL();
	}


void CMceCsServerCapabilityTest::setUpL()
	{
    iServerCaller = CMceCsServerCaller::NewL();
	iServerCaller->ConnectL();
	}
	

void CMceCsServerCapabilityTest::tearDown()
	{
	delete iServerCaller;
	}

void CMceCsServerCapabilityTest::TestCapsPassAllL()
    {
    TInt err = KErrNone;
    
	CAP_CHECK(err,EMceItcSetAppUid)             
	CAP_CHECK(err,EMceItcReceive)               
	CAP_CHECK(err,EMceItcClientReadyToReceive)  
	CAP_CHECK(err,EMceItcCancel)                
    CAP_CHECK(err,EMceItcClientCancelReceive)   
    CAP_CHECK(err,EMceItcInitFC)                
    CAP_CHECK(err,EMceItcUpdateFC)              
    CAP_CHECK(err,EMceItcFCGetMediaLines)
    CAP_CHECK(err,EMceItcFCSetMediaLines)
    CAP_CHECK(err,EMceItcGetSupportedAudioCodecs)
    CAP_CHECK(err,EMceItcVolume)
    CAP_CHECK(err,EMceItcMaxVolume)
    CAP_CHECK(err,EMceItcGain)
    CAP_CHECK(err,EMceItcMaxGain)
	CAP_CHECK(err,EMceItcEnableInactivityTimer)
	CAP_CHECK(err,EMceItcDisableInactivityTimer)
	CAP_CHECK(err,EMceItcGetSession)
	// New Added (FileInfo)
	CAP_CHECK(err,EMceItcFileInfo)
    CAP_CHECK(err,EMceItcFilePosition)
    CAP_CHECK(err,EMceItcSetFileInfo)
    CAP_CHECK(err,EMceItcSetFilePosition)
    }


void CMceCsServerCapabilityTest::TestWriteDeviceDataCapsL()
    {
    TInt err = KErrPermissionDenied;
    #if (defined(MCE_WDD_CAPS) || defined(MCE_ALLMCE_CAPS))
        err = KErrNone;
    #endif
   
    CAP_CHECK(err,EMceItcSetVolume)
    CAP_CHECK(err,EMceItcSetGain)
    }
	
	
void CMceCsServerCapabilityTest::TestNetworkServicesCapsL()
    {
    TInt err = KErrPermissionDenied;
    #if (defined(MCE_NWS_CAPS) || defined(MCE_ALLMCE_CAPS))
        err = KErrNone;
    #endif
 
    CAP_CHECK(err,EMceItcCreateSession)
    CAP_CHECK(err,EMceItcDeleteSession) 
    CAP_CHECK(err,EMceItcSetDefaultHeaders)
	CAP_CHECK(err,EMceItcGetDefaultHeaders)
	CAP_CHECK(err,EMceItcEstablishSession)
	CAP_CHECK(err,EMceItcTerminateSession)
	CAP_CHECK(err,EMceItcAcceptSession)
	CAP_CHECK(err,EMceItcRejectSession)
	CAP_CHECK(err,EMceItcUpdate)
	CAP_CHECK(err,EMceItcRing)
	CAP_CHECK(err,EMceItcEnable)
	CAP_CHECK(err,EMceItcDisable)
    CAP_CHECK(err,EMceItcSendRR)
    CAP_CHECK(err,EMceItcSendSR)
    CAP_CHECK(err,EMceItcSendFCMsg)
    CAP_CHECK(err,EMceItcSubscribe)
    CAP_CHECK(err,EMceItcSubscribeUpdate)
    CAP_CHECK(err,EMceItcAcceptEvent)
    CAP_CHECK(err,EMceItcRejectEvent)
    CAP_CHECK(err,EMceItcRespondEvent) 
    CAP_CHECK(err,EMceItcTerminateEvent)
    CAP_CHECK(err,EMceItcNotify)
    CAP_CHECK(err,EMceItcReferEvent) 
    }
    

void CMceCsServerCapabilityTest::TestMultimediaCapsL()
    {
    TInt err = KErrPermissionDenied;
    #if (defined(MCE_NWS_CAPS) || defined(MCE_ALLMCE_CAPS))
        err = KErrNone;
    #endif
    
    CAP_CHECK(err,EMceItcCameraInfo)
	CAP_CHECK(err,EMceItcCamerasAvailable)
	CAP_CHECK(err,EMceItcZoomFactor)
    CAP_CHECK(err,EMceItcDigitalZoomFactor)
    CAP_CHECK(err,EMceItcContrast)
	CAP_CHECK(err,EMceItcBrightness)
	CAP_CHECK(err,EMceItcExposure)
    CAP_CHECK(err,EMceItcWhiteBalance)
    }
    
   

MTest* CMceCsServerCapabilityTest::suiteL ()
	{
    CTestSuite *suite = CTestSuite::NewL(_L8("MCE Server PlatSec Test"));	
					
    suite->addTestL(
		CTestCaller<CMceCsServerCapabilityTest>::NewL(
			_L8("CMceCsServerCapabilityTest::TestCapsPassAllL"),
			&TestCapsPassAllL));

    suite->addTestL(
		CTestCaller<CMceCsServerCapabilityTest>::NewL(
			_L8("CMceCsServerCapabilityTest::TestWriteDeviceDataCapsL"),
			&TestWriteDeviceDataCapsL));
			
    suite->addTestL(
		CTestCaller<CMceCsServerCapabilityTest>::NewL(
			_L8("CMceCsServerCapabilityTest::TestNetworkServicesCapsL"),
			&TestNetworkServicesCapsL));	
			
    suite->addTestL(
		CTestCaller<CMceCsServerCapabilityTest>::NewL(
			_L8("CMceCsServerCapabilityTest::TestMultimediaCapsL"),
			&TestMultimediaCapsL));	
    
    return suite;	
	}
	
// End of File
