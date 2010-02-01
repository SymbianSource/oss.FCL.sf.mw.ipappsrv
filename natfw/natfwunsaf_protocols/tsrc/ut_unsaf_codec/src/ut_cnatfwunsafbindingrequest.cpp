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
#include "UT_CNATFWUNSAFBindingRequest.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <in_sock.h>

//  INTERNAL INCLUDES
#include "natfwunsafbindingrequest.h"

// CONSTRUCTION
UT_CNATFWUNSAFBindingRequest* UT_CNATFWUNSAFBindingRequest::NewL()
    {
    UT_CNATFWUNSAFBindingRequest* self = UT_CNATFWUNSAFBindingRequest::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWUNSAFBindingRequest* UT_CNATFWUNSAFBindingRequest::NewLC()
    {
    UT_CNATFWUNSAFBindingRequest* self = new( ELeave ) UT_CNATFWUNSAFBindingRequest();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWUNSAFBindingRequest::~UT_CNATFWUNSAFBindingRequest()
    {
    }

// Default constructor
UT_CNATFWUNSAFBindingRequest::UT_CNATFWUNSAFBindingRequest()
    {
    }

// Second phase construct
void UT_CNATFWUNSAFBindingRequest::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS
void UT_CNATFWUNSAFBindingRequest::SetupL ()
    {
    //Fill TransactionID with dummy values
    _LIT8(KTaId, "............");
    iTaID = KTaId;

    for (TInt i=0; i < KMaxNATFWUNSAFTransactionIdLength; ++i)
        {
        iTaID[i] = 0x10 + i;
        }

    iMsg = CNATFWUNSAFBindingRequest::NewL(iTaID);
    }

void UT_CNATFWUNSAFBindingRequest::Teardown()
    {
    delete iMsg;
    iMsg = NULL;
    }

void UT_CNATFWUNSAFBindingRequest::TestCreateL()
    {
    delete iMsg;
    iMsg = NULL;
    iMsg = static_cast<CNATFWUNSAFBindingRequest*>
        (CNATFWUNSAFBindingRequest::CreateL(iTaID));

    TNATFWUNSAFTransactionID taID = iMsg->TransactionID();

    EUNIT_ASSERT(iMsg->iTransactionID == iTaID);
    }

void UT_CNATFWUNSAFBindingRequest::TestTypeL()
    {
    EUNIT_ASSERT(CNATFWUNSAFMessage::EBindingRequest == iMsg->Type());
    }

void UT_CNATFWUNSAFBindingRequest::TestValidateL()
    {
    EUNIT_ASSERT(iMsg->Validate());
    }

void UT_CNATFWUNSAFBindingRequest::TestIsAllowedL()
    {
    EUNIT_ASSERT(iMsg->IsAllowed(CNATFWUNSAFAttribute::EChangeRequest));
    EUNIT_ASSERT(iMsg->IsAllowed(CNATFWUNSAFAttribute::EUsername));
    EUNIT_ASSERT(iMsg->IsAllowed(CNATFWUNSAFAttribute::EMessageIntegrity));

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
    UT_CNATFWUNSAFBindingRequest,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "TestCreateL - test ",
    "CNATFWUNSAFBindingRequest",
    "TestCreateL",
    "FUNCTIONALITY",
    SetupL, TestCreateL, Teardown)

EUNIT_TEST(
    "TestTypeL - test ",
    "CNATFWUNSAFBindingRequest",
    "TestTypeL",
    "FUNCTIONALITY",
    SetupL, TestTypeL, Teardown)

EUNIT_TEST(
    "TestValidateL - test ",
    "CNATFWUNSAFBindingRequest",
    "TestValidateL",
    "FUNCTIONALITY",
    SetupL, TestValidateL, Teardown)

EUNIT_TEST(
    "TestIsAllowedL - test ",
    "CNATFWUNSAFBindingRequest",
    "TestIsAllowedL",
    "FUNCTIONALITY",
    SetupL,TestIsAllowedL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
