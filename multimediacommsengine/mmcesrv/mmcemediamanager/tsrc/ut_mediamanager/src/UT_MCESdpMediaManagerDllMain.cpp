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
#include "UT_cmcesdpsession.h"
#include "UT_cmcemediamanager.h"
#include "UT_tmcesrvstreamiterator.h"
#include "UT_tmcemediaidle.h"
#include "UT_tmceofferingmedia.h"
#include "UT_tmceansweringmedia.h"
#include "UT_tmceansweringmediaupdate.h"
#include "UT_tmcemedianegotiated.h"
#include "UT_tmcepreparingoffererstreams.h"
#include "UT_tmcestartingoffererreceivestreams.h"
#include "UT_tmcestartingofferersendstreams.h"
#include "UT_tmcestartinganswererstreams.h"
#include "UT_tmcepreparinganswererstreams.h"
#include "UT_cmcesrvstream.h"
#include "UT_cmcesrvsink.h"
#include "UT_cmcesrvsource.h"
#include "UT_cmceadoptedsrvstream.h"
#include "UT_cmcesdpcodec.h"
#include "UT_cmcemediasdpcodec.h"
#include "UT_cmceaudiosdpcodec.h"
#include "UT_cmcevideosdpcodec.h"
#include "UT_tmcepreconditions.h"
#include "UT_CMceSecureMediaSession.h"
#include "UT_CMceSecureDesStream.h"
#include "UT_TMceSecurePreconditions.h"
#include "UT_CMceNatPluginManager.h"


//  EXTERNAL INCLUDES
#include <digia/eunit/CEUnitTestSuite.h>





/**
 * Test suite factory function.
 */
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    CEUnitTestSuite* rootSuite = CEUnitTestSuite::NewLC(_L("Wizard generated"));

    rootSuite->AddL( UT_CMceSdpSession::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMceSdpCodec::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMceMediaSdpCodec::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMceAudioSdpCodec::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMceVideoSdpCodec::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMceMediaManager::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_TMceSrvStreamIterator::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_TMceMediaIdle::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_TMceOfferingMedia::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_TMceAnsweringMedia::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_TMceAnsweringMediaUpdate::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_TMceMediaNegotiated::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_TMcePreparingOffererStreams::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_TMceStartingOffererReceiveStreams::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_TMceStartingOffererSendStreams::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_TMcePreparingAnswererStreams::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_TMceStartingAnswererStreams::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMceSrvSink::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMceSrvSource::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMceSrvStream::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMceAdoptedSrvStream::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_TMceSegmentedPreconditions::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMceSecureDesStream::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMceSecureMediaSession::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_TMceSecurePreconditions::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMceNatPluginManager::NewLC() );
    CleanupStack::Pop();
    
    CleanupStack::Pop(); // rootSuite

return rootSuite;
    }

/**
 * Standard Symbian DLL entry point function.
 */


//  END OF FILE
