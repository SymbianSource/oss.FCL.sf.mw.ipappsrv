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
#include "UT_CNATFWUNSAFSetActiveDestinationRequest.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "natfwunsafsetactivedestinationrequest.h"

// CONSTRUCTION
UT_CNATFWUNSAFSetActiveDestinationRequest* UT_CNATFWUNSAFSetActiveDestinationRequest::NewL()
    {
    UT_CNATFWUNSAFSetActiveDestinationRequest* self = UT_CNATFWUNSAFSetActiveDestinationRequest::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWUNSAFSetActiveDestinationRequest* UT_CNATFWUNSAFSetActiveDestinationRequest::NewLC()
    {
    UT_CNATFWUNSAFSetActiveDestinationRequest* self = new( ELeave ) UT_CNATFWUNSAFSetActiveDestinationRequest();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWUNSAFSetActiveDestinationRequest::~UT_CNATFWUNSAFSetActiveDestinationRequest()
    {
    }

// Default constructor
UT_CNATFWUNSAFSetActiveDestinationRequest::UT_CNATFWUNSAFSetActiveDestinationRequest()
    {
    }

// Second phase construct
void UT_CNATFWUNSAFSetActiveDestinationRequest::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void UT_CNATFWUNSAFSetActiveDestinationRequest::SetupL ()
    {
    //Fill TransactionID with dummy values
    _LIT8(KTaId, "............");
    iTaID = KTaId;

    for (TInt i=0; i < KMaxNATFWUNSAFTransactionIdLength; ++i)
        {
        iTaID[i] = 0x10 + i;
        }

    iMsg = CNATFWUNSAFSetActiveDestinationRequest::NewL(iTaID);
    }

void UT_CNATFWUNSAFSetActiveDestinationRequest::Teardown()
    {
    delete iMsg;
    iMsg = NULL;
    }

void UT_CNATFWUNSAFSetActiveDestinationRequest::TestCreateL()
    {
    delete iMsg;
    iMsg = NULL;
    iMsg = static_cast<CNATFWUNSAFSetActiveDestinationRequest*>
        (CNATFWUNSAFSetActiveDestinationRequest::CreateL(iTaID));

    TNATFWUNSAFTransactionID taID = iMsg->TransactionID();

    EUNIT_ASSERT(iMsg->iTransactionID == iTaID);
    }

void UT_CNATFWUNSAFSetActiveDestinationRequest::TestTypeL()
    {
    EUNIT_ASSERT(CNATFWUNSAFMessage::ESetActiveDestinationRequest == iMsg->Type());
    }

void UT_CNATFWUNSAFSetActiveDestinationRequest::TestValidateL()
    {
    EUNIT_ASSERT(iMsg->Validate());
    }

void UT_CNATFWUNSAFSetActiveDestinationRequest::TestIsAllowedL()
    {
    EUNIT_ASSERT(iMsg->IsAllowed(CNATFWUNSAFAttribute::ERemoteAddress));
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
    UT_CNATFWUNSAFSetActiveDestinationRequest,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "TestCreateL - test ",
    "CNATFWUNSAFSetActiveDestinationRequest",
    "TestCreateL",
    "FUNCTIONALITY",
    SetupL, TestCreateL, Teardown)

EUNIT_TEST(
    "TestTypeL - test ",
    "CNATFWUNSAFSetActiveDestinationRequest",
    "TestTypeL",
    "FUNCTIONALITY",
    SetupL, TestTypeL, Teardown)

EUNIT_TEST(
    "TestValidateL - test ",
    "CNATFWUNSAFSetActiveDestinationRequest",
    "TestValidateL",
    "FUNCTIONALITY",
    SetupL, TestValidateL, Teardown)

EUNIT_TEST(
    "TestIsAllowedL - test ",
    "CNATFWUNSAFSetActiveDestinationRequest",
    "TestIsAllowedL",
    "FUNCTIONALITY",
    SetupL,TestIsAllowedL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
