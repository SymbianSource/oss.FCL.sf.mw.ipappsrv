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
#include "UT_CNATFWUNSAFBindingResponse.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <in_sock.h>

//  INTERNAL INCLUDES
#include "natfwunsafbindingresponse.h"
#include "natfwunsafmappedaddressattribute.h"

// CONSTRUCTION
UT_CNATFWUNSAFBindingResponse* UT_CNATFWUNSAFBindingResponse::NewL()
    {
    UT_CNATFWUNSAFBindingResponse* self = UT_CNATFWUNSAFBindingResponse::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWUNSAFBindingResponse* UT_CNATFWUNSAFBindingResponse::NewLC()
    {
    UT_CNATFWUNSAFBindingResponse* self = new( ELeave ) UT_CNATFWUNSAFBindingResponse();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWUNSAFBindingResponse::~UT_CNATFWUNSAFBindingResponse()
    {
    }

// Default constructor
UT_CNATFWUNSAFBindingResponse::UT_CNATFWUNSAFBindingResponse()
    {
    }

// Second phase construct
void UT_CNATFWUNSAFBindingResponse::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void UT_CNATFWUNSAFBindingResponse::SetupL ()
    {
    //Fill TransactionID with dummy values
    _LIT8(KTaId, "............");
    iTaID = KTaId;

    for (TInt i=0; i < KMaxNATFWUNSAFTransactionIdLength; ++i)
        {
        iTaID[i] = 0x10 + i;
        }

    iMsg = CNATFWUNSAFBindingResponse::NewL(iTaID);
    //Add the required MAPPED-ADDRESS attribute
    TInetAddr addr(KInetAddrAny);
    CNATFWUNSAFMappedAddressAttribute* attr =
        CNATFWUNSAFMappedAddressAttribute::NewL(addr);
    iMsg->AddAttributeL(attr);
    }

void UT_CNATFWUNSAFBindingResponse::Teardown()
    {
    delete iMsg;
    iMsg = NULL;
    }

void UT_CNATFWUNSAFBindingResponse::TestCreateL()
    {
    delete iMsg;
    iMsg = NULL;
    iMsg = static_cast<CNATFWUNSAFBindingResponse*>
        (CNATFWUNSAFBindingResponse::CreateL(iTaID));

    TNATFWUNSAFTransactionID taID = iMsg->TransactionID();

    EUNIT_ASSERT(iMsg->iTransactionID == iTaID);
    }

void UT_CNATFWUNSAFBindingResponse::TestTypeL()
    {
    EUNIT_ASSERT(CNATFWUNSAFMessage::EBindingResponse == iMsg->Type());
    }

void UT_CNATFWUNSAFBindingResponse::TestValidateL()
    {
    EUNIT_ASSERT(iMsg->Validate());
    }

void UT_CNATFWUNSAFBindingResponse::TestIsAllowedL()
    {
    EUNIT_ASSERT(iMsg->IsAllowed(CNATFWUNSAFAttribute::EMappedAddress));
    EUNIT_ASSERT(iMsg->IsAllowed(CNATFWUNSAFAttribute::ESourceAddress));
    EUNIT_ASSERT(iMsg->IsAllowed(CNATFWUNSAFAttribute::EChangedAddress));
    EUNIT_ASSERT(iMsg->IsAllowed(CNATFWUNSAFAttribute::EMessageIntegrity));
    EUNIT_ASSERT(iMsg->IsAllowed(CNATFWUNSAFAttribute::EReflectedFrom));
    EUNIT_ASSERT(iMsg->IsAllowed(CNATFWUNSAFAttribute::EXorMappedAddress));
    EUNIT_ASSERT(iMsg->IsAllowed(CNATFWUNSAFAttribute::EServer));

    EUNIT_ASSERT(!iMsg->IsAllowed(CNATFWUNSAFAttribute::ERemoteAddress));
    EUNIT_ASSERT(!iMsg->IsAllowed(CNATFWUNSAFAttribute::ERelayAddress));
    EUNIT_ASSERT(!iMsg->IsAllowed(CNATFWUNSAFAttribute::EDestinationAddress));
    EUNIT_ASSERT(!iMsg->IsAllowed(CNATFWUNSAFAttribute::EPassword));
    EUNIT_ASSERT(!iMsg->IsAllowed(CNATFWUNSAFAttribute::EErrorCode));
    EUNIT_ASSERT(!iMsg->IsAllowed(CNATFWUNSAFAttribute::EUnknownAttributes));
    EUNIT_ASSERT(!iMsg->IsAllowed(CNATFWUNSAFAttribute::ETimerVal));
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWUNSAFBindingResponse,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "TestCreateL - test ",
    "CNATFWUNSAFBindingResponse",
    "TestCreateL",
    "FUNCTIONALITY",
    SetupL, TestCreateL, Teardown)

EUNIT_TEST(
    "TestTypeL - test ",
    "CNATFWUNSAFBindingResponse",
    "TestTypeL",
    "FUNCTIONALITY",
    SetupL, TestTypeL, Teardown)

EUNIT_TEST(
    "TestValidateL - test ",
    "CNATFWUNSAFBindingResponse",
    "TestValidateL",
    "FUNCTIONALITY",
    SetupL, TestValidateL, Teardown)

EUNIT_TEST(
    "TestIsAllowedL - test ",
    "CNATFWUNSAFBindingResponse",
    "TestIsAllowedL",
    "FUNCTIONALITY",
    SetupL,TestIsAllowedL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
