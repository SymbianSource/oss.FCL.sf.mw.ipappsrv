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




//  CLASS HEADER
#include "UT_CCompleteCallback.h"
#include "UT_CMccAsyncEventHandler.h"
#include "UT_CMccRtpManager.h"
#include "UT_CMccSymDlStream.h"
#include "UT_CMccSymProxySession.h"
#include "UT_CMccSymSubthreadClient.h"
#include "UT_CMccSymUlStream.h"
#include "UT_CMccCodecConfigurator.h"
#include "UT_CMccDataPathBase.h"
#include "UT_CMccDlDataPath.h"
#include "UT_CMccSubThreadShutdownTimer.h"
#include "UT_CMccUlDataPath.h"
#include "UT_CMccUlDlClient.h"
#include "UT_CMccSymSimpleDlStream.h"
#include "UT_CMccRtcpReceiver.h"
#include "UT_CMccDtmfDlStream.h"
#include "UT_CMccDtmfUlStream.h"
#include "UT_CMccRtpKeepalive.h"
#include "UT_CMccQosHandler.h"
#include "UT_CMccQosSocketHandler.h"
#include "UT_CMccQosEventMonitor.h"


//  EXTERNAL INCLUDES
#include <digia/eunit/CEUnitTestSuite.h>

/**
 * Test suite factory function.
 */
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    CEUnitTestSuite* rootSuite = CEUnitTestSuite::NewLC(_L("Wizard generated"));

    rootSuite->AddL( UT_CCompleteCallback::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMccRtpManager::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMccSymDlStream::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMccSymSubthreadClient::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMccSymUlStream::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMccCodecConfigurator::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMccDataPathBase::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMccDlDataPath::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMccUlDataPath::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMccUlDlClient::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMccSymSimpleDlStream::NewLC() );
    CleanupStack::Pop();
     
    rootSuite->AddL( UT_CMccRtcpReceiver::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMccDtmfDlStream::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMccDtmfUlStream::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMccRtpKeepalive::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMccQosHandler::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMccQosSocketHandler::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMccQosEventMonitor::NewLC() );
    CleanupStack::Pop(); 
    
    CleanupStack::Pop(); // rootSuite

    return rootSuite;
    }

//  END OF FILE






