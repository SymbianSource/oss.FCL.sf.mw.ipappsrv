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
#include "ut_cmcecomevent.h"
#include "ut_cmceeventcontext.h"
#include "ut_cmceeventstatebase.h"
#include "ut_cmceeventsubscribemoidlestate.h"
#include "ut_cmceeventsubscribemoestablishingstate.h"
#include "ut_cmceeventsubscribemoestablishedstate.h"
#include "ut_cmceeventsubscribemtestablishedstate.h"
#include "ut_cmceeventsubscribeterminatingstate.h"
#include "ut_cmceeventsubscribeterminatedstate.h"
#include "ut_cmceeventrefermoidlestate.h"
#include "ut_cmceeventsubscribemtidlestate.h"
#include "ut_cmceeventsubscribemtestablishingstate.h"
#include "ut_cmceeventrefermoestablishingstate.h"
#include "ut_cmceeventrefermoestablishedstate.h"
#include "ut_cmceeventrefermtestablishedstate.h"
#include "ut_cmceeventrefermtidlestate.h"
#include "ut_cmceeventrefermtestablishingstate.h"
#include "ut_cmceeventreferterminatingstate.h"
#include "ut_cmceeventreferterminatedstate.h"
#include "ut_mcesipeventhelper.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/CEUnitTestSuite.h>





/**
 * Test suite factory function.
 */
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    CEUnitTestSuite* rootSuite = CEUnitTestSuite::NewLC(_L("Wizard generated"));

    rootSuite->AddL( UT_CMceComEvent::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMceEventContext::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMceEventStateBase::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMceEventSubscribeMoIdleState::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMceEventSubscribeMoEstablishingState::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMceEventSubscribeMtEstablishedState::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMceEventSubscribeMoEstablishedState::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMceEventSubscribeTerminatingState::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMceEventSubscribeTerminatedState::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMceEventReferMoIdleState::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMceEventSubscribeMtIdleState::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMceEventSubscribeMtEstablishingState::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMceEventReferMoEstablishingState::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMceEventReferMtIdleState::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMceEventReferMtEstablishingState::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMceEventReferMtEstablishedState::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMceEventReferMoEstablishedState::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMceEventReferTerminatingState::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMceEventReferTerminatedState::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_MCESIPEventHelper::NewLC() );
    CleanupStack::Pop();

    CleanupStack::Pop(); // rootSuite

    return rootSuite;
    }

/**
 * Standard Symbian DLL entry point function.
 */
/*GLDEF_C TInt E32Dll(TDllReason)
    {
    return KErrNone;
    }
*/
//  END OF FILE
