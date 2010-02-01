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
#include "UT_CNATFWUNSAFSendIndication.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>

//  INTERNAL INCLUDES
#include "natfwunsafsendindication.h"

// CONSTRUCTION
UT_CNATFWUNSAFSendIndication* UT_CNATFWUNSAFSendIndication::NewL()
    {
    UT_CNATFWUNSAFSendIndication* self = UT_CNATFWUNSAFSendIndication::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWUNSAFSendIndication* UT_CNATFWUNSAFSendIndication::NewLC()
    {
    UT_CNATFWUNSAFSendIndication* self = new( ELeave ) UT_CNATFWUNSAFSendIndication();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWUNSAFSendIndication::~UT_CNATFWUNSAFSendIndication()
    {
    }

// Default constructor
UT_CNATFWUNSAFSendIndication::UT_CNATFWUNSAFSendIndication()
    {
    }

// Second phase construct
void UT_CNATFWUNSAFSendIndication::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void UT_CNATFWUNSAFSendIndication::SetupL ()
    {
    //Fill TransactionID with dummy values
    _LIT8(KTaId, "............");
    iTaID = KTaId;

    for (TInt i=0; i < KMaxNATFWUNSAFTransactionIdLength; ++i)
        {
        iTaID[i] = 0x10 + i;
        }

    iMsg = CNATFWUNSAFSendIndication::NewL(iTaID);
    }

void UT_CNATFWUNSAFSendIndication::Teardown()
    {
    delete iMsg;
    iMsg = NULL;
    }

void UT_CNATFWUNSAFSendIndication::TestCreateL()
    {
    delete iMsg;
    iMsg = NULL;
    iMsg = static_cast<CNATFWUNSAFSendIndication*>
        (CNATFWUNSAFSendIndication::CreateL(iTaID));

    TNATFWUNSAFTransactionID taID = iMsg->TransactionID();

    EUNIT_ASSERT(iMsg->iTransactionID == iTaID);
    }

void UT_CNATFWUNSAFSendIndication::TestTypeL()
    {
    EUNIT_ASSERT(CNATFWUNSAFMessage::ESendIndication == iMsg->Type());
    }

void UT_CNATFWUNSAFSendIndication::TestValidateL()
    {
    EUNIT_ASSERT(iMsg->Validate());
    }

void UT_CNATFWUNSAFSendIndication::TestIsAllowedL()
    {
    EUNIT_ASSERT(iMsg->IsAllowed(CNATFWUNSAFAttribute::EData));
    EUNIT_ASSERT(iMsg->IsAllowed(CNATFWUNSAFAttribute::ERemoteAddress));

    EUNIT_ASSERT(!iMsg->IsAllowed(CNATFWUNSAFAttribute::ERequestedPortProps));
    EUNIT_ASSERT(!iMsg->IsAllowed(CNATFWUNSAFAttribute::EMessageIntegrity));
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
    UT_CNATFWUNSAFSendIndication,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "TestCreateL - test ",
    "CNATFWUNSAFSendIndication",
    "TestCreateL",
    "FUNCTIONALITY",
    SetupL, TestCreateL, Teardown)

EUNIT_TEST(
    "TestTypeL - test ",
    "CNATFWUNSAFSendIndication",
    "TestTypeL",
    "FUNCTIONALITY",
    SetupL, TestTypeL, Teardown)

EUNIT_TEST(
    "TestValidateL - test ",
    "CNATFWUNSAFSendIndication",
    "TestValidateL",
    "FUNCTIONALITY",
    SetupL, TestValidateL, Teardown)

EUNIT_TEST(
    "TestIsAllowedL - test ",
    "CNATFWUNSAFSendIndication",
    "TestIsAllowedL",
    "FUNCTIONALITY",
    SetupL,TestIsAllowedL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
