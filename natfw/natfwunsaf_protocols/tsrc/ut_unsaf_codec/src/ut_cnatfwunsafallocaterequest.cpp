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

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "natfwunsafallocaterequest.h"

// CONSTRUCTION
UT_CNATFWUNSAFAllocateRequest* UT_CNATFWUNSAFAllocateRequest::NewL()
    {
    UT_CNATFWUNSAFAllocateRequest* self = UT_CNATFWUNSAFAllocateRequest::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWUNSAFAllocateRequest* UT_CNATFWUNSAFAllocateRequest::NewLC()
    {
    UT_CNATFWUNSAFAllocateRequest* self = new( ELeave ) UT_CNATFWUNSAFAllocateRequest();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWUNSAFAllocateRequest::~UT_CNATFWUNSAFAllocateRequest()
    {
    }

// Default constructor
UT_CNATFWUNSAFAllocateRequest::UT_CNATFWUNSAFAllocateRequest()
    {
    }

// Second phase construct
void UT_CNATFWUNSAFAllocateRequest::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void UT_CNATFWUNSAFAllocateRequest::SetupL ()
    {
    //Fill TransactionID with dummy values
    _LIT8(KTaId, "............");
    iTaID = KTaId;

    for (TInt i=0; i < KMaxNATFWUNSAFTransactionIdLength; ++i)
        {
        iTaID[i] = 0x10 + i;
        }

    iMsg = CNATFWUNSAFAllocateRequest::NewL(iTaID);
    }

void UT_CNATFWUNSAFAllocateRequest::Teardown()
    {
    delete iMsg;
    iMsg = NULL;
    }

void UT_CNATFWUNSAFAllocateRequest::TestCreateL()
    {
    delete iMsg;
    iMsg = NULL;
    iMsg = static_cast<CNATFWUNSAFAllocateRequest*>
        (CNATFWUNSAFAllocateRequest::CreateL(iTaID));

    TNATFWUNSAFTransactionID taID = iMsg->TransactionID();

    EUNIT_ASSERT(iMsg->iTransactionID == iTaID);
    }

void UT_CNATFWUNSAFAllocateRequest::TestTypeL()
    {
    EUNIT_ASSERT(CNATFWUNSAFMessage::EAllocateRequest == iMsg->Type());
    }

void UT_CNATFWUNSAFAllocateRequest::TestValidateL()
    {
    EUNIT_ASSERT(iMsg->Validate());
    }

void UT_CNATFWUNSAFAllocateRequest::TestIsAllowedL()
    {
    EUNIT_ASSERT(iMsg->IsAllowed(CNATFWUNSAFAttribute::ERequestedTransport));
    EUNIT_ASSERT(iMsg->IsAllowed(CNATFWUNSAFAttribute::ERequestedIp));
    EUNIT_ASSERT(iMsg->IsAllowed(CNATFWUNSAFAttribute::ERequestedPortProps));
    EUNIT_ASSERT(iMsg->IsAllowed(CNATFWUNSAFAttribute::EMessageIntegrity));
    EUNIT_ASSERT(iMsg->IsAllowed(CNATFWUNSAFAttribute::EBandwidth));
    EUNIT_ASSERT(iMsg->IsAllowed(CNATFWUNSAFAttribute::ELifetime));

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
    UT_CNATFWUNSAFAllocateRequest,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "TestCreateL - test ",
    "CNATFWUNSAFAllocateRequest",
    "TestCreateL",
    "FUNCTIONALITY",
    SetupL, TestCreateL, Teardown)

EUNIT_TEST(
    "TestTypeL - test ",
    "CNATFWUNSAFAllocateRequest",
    "TestTypeL",
    "FUNCTIONALITY",
    SetupL, TestTypeL, Teardown)

EUNIT_TEST(
    "TestValidateL - test ",
    "CNATFWUNSAFAllocateRequest",
    "TestValidateL",
    "FUNCTIONALITY",
    SetupL, TestValidateL, Teardown)

EUNIT_TEST(
    "TestIsAllowedL - test ",
    "CNATFWUNSAFAllocateRequest",
    "TestIsAllowedL",
    "FUNCTIONALITY",
    SetupL,TestIsAllowedL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
