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
#include "UT_cmcecsreceivequeue.h"
#include "ut_cmcecsreceiverbase.h"
#include "ut_cmcecssession.h"
#include "ut_cmcecssubsession.h"
#include "ut_cmcesipsession.h"
#include "ut_cmcesipevent.h"
#include "ut_cmcesipconnection.h"
#include "ut_cmcesipmanager.h"
#include "ut_cmceclientresolver.h"
#include "UT_TMceActionSet.h"
#include "ut_cmcestateidle.h"
#include "ut_cmcestateclientestablishing.h"
#include "ut_cmcestateoffering.h"
#include "ut_cmcestateestablished.h"
#include "ut_cmcestateserverestablishing.h"
#include "ut_cmcestateserveroffering.h"
#include "ut_cmcestateupdating.h"
#include "ut_cmcestateupdated.h"
#include "ut_cmcestateanswering.h"
#include "ut_cmcestateterminating.h"
#include "ut_cmcestatecanceled.h"
#include "ut_cmcestateerror.h"
#include "ut_cmcestateconfirming.h"
#include "ut_cmcestateclientreserving.h"
#include "ut_cmcestateacknowledgementrequired.h"
#include "ut_cmcestateconfirmationrequired.h"
#include "ut_cmcestateserverreserving.h"
#include "ut_cmcestateserverinitializing.h"
#include "ut_mCEUsesCases.h"
#include "ut_cmcesipdefaultdata.h"
#include "ut_cmcesipdefaults.h"
#include "ut_mcesip.h"
#include "ut_cmcedictionary.h"
#include "ut_cmcesipextensions.h"
#include "ut_cmcereliablesender.h"
#include "ut_cmcelocaladdrresolver.h"
#include "UT_CMCENatSipSession.h"
#include "UT_CMCENatUseCases.h"
#include "ut_cmcenatstatecreateofferrequested.h"
#include "ut_cmcenatstatecreateoffer.h"
#include "ut_cmcenatstatecreateanswer.h"
#include "ut_cmcenatstatedecodeanswer.h"
#include "ut_cmcenatstatedecodeoffer.h"
#include "ut_cmcenatstatewait.h"
#include "ut_cmcenatstateconnected.h"
#include "ut_cmcenatstateidle.h"
#include "UT_CNSPPluginStub.h"
#include "UT_CMceState.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/CEUnitTestSuite.h>




/**
 * Test suite factory function.
 */
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    CEUnitTestSuite* rootSuite = CEUnitTestSuite::NewLC(_L("Wizard generated"));

    rootSuite->AddL( UT_CMceCsSubSession::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMceCsReceiveQueue::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMceCsReceiverBase::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMceCsSession::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMceSipSession::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMceSipEvent::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMceSipConnection::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMceSipManager::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMceClientResolver::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_TMceActionSet::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMceStateIdle::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMceStateClientEstablishing::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMceStateOffering::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMceStateEstablished::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMceStateServerEstablishing::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMceStateUpdating::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMceStateUpdated::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMceStateAnswering::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMceStateTerminating::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMceStateCanceled::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMceStateError::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMceStateConfirming::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMceStateClientReserving::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMceStateAcknowledgementRequired::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMceStateServerReserving::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMceStateServerOffering::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMceStateConfirmationRequired::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMceStateServerInitializing::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_MCEUsesCases::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMceSipDefaultData::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMceSipDefaults::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_MCESIP::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMceDictionary::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMceSipExtensions::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMceReliableSender::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMceLocalAddrResolver::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMceNatSipSession::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMCENatUseCases::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMceNatStateCreateOfferRequested::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMceNatStateCreateOffer::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMceNatStateCreateAnswer::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMceNatStateDecodeOffer::NewLC() );
    CleanupStack::Pop();

	rootSuite->AddL( UT_CMceNatStateDecodeAnswer::NewLC() );
    CleanupStack::Pop();

	rootSuite->AddL( UT_CMceNatStateWait::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMceNatStateConnected::NewLC() );
    CleanupStack::Pop();
	
    rootSuite->AddL( UT_CMceNatStateIdle::NewLC() );
    CleanupStack::Pop();
    
	rootSuite->AddL( UT_CNSPPluginStub::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMceState::NewLC() );
    CleanupStack::Pop();
    
    CleanupStack::Pop(); // rootSuite

    return rootSuite;    
    }

#ifdef _BullseyeCoverage
GLDEF_C int __stdcall __E32Dll(void*, unsigned, void*)
	{
	return 0;
	}
#endif

//  END OF FILE
