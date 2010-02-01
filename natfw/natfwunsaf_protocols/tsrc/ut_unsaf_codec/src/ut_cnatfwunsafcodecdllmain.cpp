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
#include "UT_CNATFWUNSAFAllocateRequest.h"
#include "UT_CNATFWUNSAFAlternateServerAttribute.h"
#include "UT_CNATFWUNSAFBandwidthAttribute.h"
#include "UT_CNATFWUNSAFBindingRequest.h"
#include "UT_CNATFWUNSAFBindingResponse.h"
#include "UT_CNATFWUNSAFChangedAddressAttribute.h"
#include "UT_CNATFWUNSAFChangeRequestAttribute.h"
#include "UT_CNATFWUNSAFConnectRequest.h"
#include "UT_CNATFWUNSAFDataAttribute.h"
#include "UT_CNATFWUNSAFDestinationAddressAttribute.h"
#include "UT_CNATFWUNSAFErrorCodeAttribute.h"
#include "UT_CNATFWUNSAFFingerPrintAttribute.h"
#include "UT_CNATFWUNSAFIceControlledAttribute.h"
#include "UT_CNATFWUNSAFIceControllingAttribute.h"
#include "UT_CNATFWUNSAFLifetimeAttribute.h"
#include "UT_CNATFWUNSAFMagicCookieAttribute.h"
#include "UT_CNATFWUNSAFMappedAddressAttribute.h"
#include "UT_CNATFWUNSAFMessage.h"
#include "UT_CNATFWUNSAFMessageFactory.h"
#include "UT_CNATFWUNSAFMessageIntegrityAttribute.h"
#include "UT_CNATFWUNSAFNonceAttribute.h"
#include "UT_CNATFWUNSAFPasswordAttribute.h"
#include "UT_CNATFWUNSAFPriorityAttribute.h"
#include "UT_CNATFWUNSAFRealmAttribute.h"
#include "UT_CNATFWUNSAFReflectedFromAttribute.h"
#include "UT_CNATFWUNSAFRelayAddressAttribute.h"
#include "UT_CNATFWUNSAFRemoteAddressAttribute.h"
#include "UT_CNATFWUNSAFRequestedIpAttribute.h"
#include "UT_CNATFWUNSAFRequestedPortPropsAttribute.h"
#include "UT_CNATFWUNSAFRequestedTransportAttribute.h"
#include "UT_CNATFWUNSAFResponseAddressAttribute.h"
#include "UT_CNATFWUNSAFSendIndication.h"
#include "UT_CNATFWUNSAFServerAttribute.h"
#include "UT_CNATFWUNSAFSetActiveDestinationRequest.h"
#include "UT_CNATFWUNSAFSourceAddressAttribute.h"
#include "UT_CNATFWUNSAFTcpRelayPacket.h"
#include "UT_CNATFWUNSAFTcpRelayPacketFactory.h"
#include "UT_CNATFWUNSAFTimerValAttribute.h"
#include "UT_CNATFWUNSAFUnknownAttributesAttribute.h"
#include "UT_CNATFWUNSAFUseCandidateAttribute.h"
#include "UT_CNATFWUNSAFUsernameAttribute.h"
#include "UT_CNATFWUNSAFXorMappedAddressAttribute.h"
#include "UT_CNATFWUNSAFXorOnlyAttribute.h"


//  EXTERNAL INCLUDES
#include <digia/eunit/CEUnitTestSuite.h>

#define _DEBUG_

/**
 * Test suite factory function.
 */
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    CEUnitTestSuite* rootSuite = CEUnitTestSuite::NewLC(_L("NATFW UNSAF Codec unit tests"));

    rootSuite->AddL( UT_CNATFWUNSAFAllocateRequest::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWUNSAFAlternateServerAttribute::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWUNSAFBandwidthAttribute::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWUNSAFBindingRequest::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWUNSAFBindingResponse::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWUNSAFChangedAddressAttribute::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWUNSAFChangeRequestAttribute::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWUNSAFConnectRequest::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWUNSAFDataAttribute::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWUNSAFDestinationAddressAttribute::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWUNSAFErrorCodeAttribute::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWUNSAFFingerprintAttribute::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CNATFWUNSAFIceControlledAttribute::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CNATFWUNSAFIceControllingAttribute::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWUNSAFLifetimeAttribute::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWUNSAFMappedAddressAttribute::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWUNSAFMagicCookieAttribute::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWUNSAFMessage::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWUNSAFMessageFactory::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWUNSAFMessageIntegrityAttribute::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWUNSAFNonceAttribute::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWUNSAFPasswordAttribute::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWUNSAFPriorityAttribute::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWUNSAFRealmAttribute::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWUNSAFReflectedFromAttribute::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWUNSAFRelayAddressAttribute::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWUNSAFRemoteAddressAttribute::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWUNSAFRequestedIpAttribute::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWUNSAFRequestedPortPropsAttribute::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWUNSAFRequestedTransportAttribute::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWUNSAFResponseAddressAttribute::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWUNSAFSendIndication::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWUNSAFServerAttribute::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWUNSAFSetActiveDestinationRequest::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWUNSAFSourceAddressAttribute::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWUNSAFTcpRelayPacket::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWUNSAFTcpRelayPacketFactory::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWUNSAFTimerValAttribute::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWUNSAFUnknownAttributesAttribute::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWUNSAFUseCandidateAttribute::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWUNSAFUsernameAttribute::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWUNSAFXorMappedAddressAttribute::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWUNSAFXorOnlyAttribute::NewLC() );
    CleanupStack::Pop();

    CleanupStack::Pop( rootSuite );
    return rootSuite;
    }

#ifndef __SECURE_API__
/**
 * Standard Symbian DLL entry point function.
 */
GLDEF_C TInt E32Dll(TDllReason)
    {
    return KErrNone;
    }
#endif

//  END OF FILE
