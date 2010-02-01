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
#include "UT_CNATFWUNSAFConnectRequest.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "natfwunsafconnectrequest.h"

// CONSTRUCTION
UT_CNATFWUNSAFConnectRequest* UT_CNATFWUNSAFConnectRequest::NewL()
    {
    UT_CNATFWUNSAFConnectRequest* self = UT_CNATFWUNSAFConnectRequest::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWUNSAFConnectRequest* UT_CNATFWUNSAFConnectRequest::NewLC()
    {
    UT_CNATFWUNSAFConnectRequest* self = new( ELeave ) UT_CNATFWUNSAFConnectRequest();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWUNSAFConnectRequest::~UT_CNATFWUNSAFConnectRequest()
    {
    }

// Default constructor
UT_CNATFWUNSAFConnectRequest::UT_CNATFWUNSAFConnectRequest()
    {
    }

// Second phase construct
void UT_CNATFWUNSAFConnectRequest::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void UT_CNATFWUNSAFConnectRequest::SetupL ()
    {
    //Fill TransactionID with dummy values
    _LIT8(KTaId, "............");
    iTaID = KTaId;

    for (TInt i=0; i < KMaxNATFWUNSAFTransactionIdLength; ++i)
        {
        iTaID[i] = 0x10 + i;
        }

    iMsg = CNATFWUNSAFConnectRequest::NewL(iTaID);
    }

void UT_CNATFWUNSAFConnectRequest::Teardown()
    {
    delete iMsg;
    iMsg = NULL;
    }

void UT_CNATFWUNSAFConnectRequest::TestCreateL()
    {
    delete iMsg;
    iMsg = NULL;
    iMsg = static_cast<CNATFWUNSAFConnectRequest*>
        (CNATFWUNSAFConnectRequest::CreateL(iTaID));

    TNATFWUNSAFTransactionID taID = iMsg->TransactionID();

    EUNIT_ASSERT(iMsg->iTransactionID == iTaID);
    }

void UT_CNATFWUNSAFConnectRequest::TestTypeL()
    {
    EUNIT_ASSERT(CNATFWUNSAFMessage::EConnectRequest == iMsg->Type());
    }

void UT_CNATFWUNSAFConnectRequest::TestValidateL()
    {
    EUNIT_ASSERT(iMsg->Validate());
    }

void UT_CNATFWUNSAFConnectRequest::TestIsAllowedL()
    {
    EUNIT_ASSERT(iMsg->IsAllowed(CNATFWUNSAFAttribute::ERemoteAddress));
    EUNIT_ASSERT(iMsg->IsAllowed(CNATFWUNSAFAttribute::EDestinationAddress));
    EUNIT_ASSERT(iMsg->IsAllowed(CNATFWUNSAFAttribute::EMessageIntegrity));

    EUNIT_ASSERT(!iMsg->IsAllowed(CNATFWUNSAFAttribute::ERequestedPortProps));
    EUNIT_ASSERT(!iMsg->IsAllowed(CNATFWUNSAFAttribute::EBandwidth));
    EUNIT_ASSERT(!iMsg->IsAllowed(CNATFWUNSAFAttribute::ELifetime));
    EUNIT_ASSERT(!iMsg->IsAllowed(CNATFWUNSAFAttribute::EMappedAddress));
    EUNIT_ASSERT(!iMsg->IsAllowed(CNATFWUNSAFAttribute::ESourceAddress));
    EUNIT_ASSERT(!iMsg->IsAllowed(CNATFWUNSAFAttribute::EChangedAddress));
    EUNIT_ASSERT(!iMsg->IsAllowed(CNATFWUNSAFAttribute::EPassword));
    EUNIT_ASSERT(!iMsg->IsAllowed(CNATFWUNSAFAttribute::EErrorCode));
    EUNIT_ASSERT(!iMsg->IsAllowed(CNATFWUNSAFAttribute::EUnknownAttributes));
    EUNIT_ASSERT(!iMsg->IsAllowed(CNATFWUNSAFAttribute::EReflectedFrom));
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWUNSAFConnectRequest,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "TestCreateL - test ",
    "CNATFWUNSAFConnectRequest",
    "TestCreateL",
    "FUNCTIONALITY",
    SetupL, TestCreateL, Teardown)

EUNIT_TEST(
    "TestTypeL - test ",
    "CNATFWUNSAFConnectRequest",
    "TestTypeL",
    "FUNCTIONALITY",
    SetupL, TestTypeL, Teardown)

EUNIT_TEST(
    "TestValidateL - test ",
    "CNATFWUNSAFConnectRequest",
    "TestValidateL",
    "FUNCTIONALITY",
    SetupL, TestValidateL, Teardown)

EUNIT_TEST(
    "TestIsAllowedL - test ",
    "CNATFWUNSAFConnectRequest",
    "TestIsAllowedL",
    "FUNCTIONALITY",
    SetupL,TestIsAllowedL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
