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
#include "UT_CNATFWIdentification.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
//  INTERNAL INCLUDES
#include "natfwcredentials.h"
#include "natfwunittestmacros.h"

const TUint KStreamCollectionId = 40;
const TUint KStreamId = 4;
_LIT8( KUserName, "UserName" );
_LIT8( KPassword, "Password" );


// CONSTRUCTION
UT_CNATFWIdentification* UT_CNATFWIdentification::NewL()
    {
    UT_CNATFWIdentification* self = UT_CNATFWIdentification::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWIdentification* UT_CNATFWIdentification::NewLC()
    {
    UT_CNATFWIdentification* self = new( ELeave ) UT_CNATFWIdentification();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWIdentification::~UT_CNATFWIdentification()
    {
    }

// Default constructor
UT_CNATFWIdentification::UT_CNATFWIdentification()
    {
    }

// Second phase construct
void UT_CNATFWIdentification::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CNATFWIdentification::SetupL(  )
    {
    iIdentifi = CNATFWCredentials::NewL();
    }

void UT_CNATFWIdentification::Teardown(  )
    {
    delete iIdentifi;
    iIdentifi = NULL;
    }

void UT_CNATFWIdentification::UT_CNATFWIdentification_NewLL(  )
    {
    CNATFWCredentials* id;
    CNATFWCredentials* id2;

    NATFW_EUNIT_ASSERT_NO_LEAVE( id = CNATFWCredentials::NewL() );
    CleanupStack::PushL( id );
    NATFW_EUNIT_ASSERT_NO_LEAVE( id2 = CNATFWCredentials::NewL( *id ) );
    
    CleanupStack::PopAndDestroy( id );
    delete id2;
    }

void UT_CNATFWIdentification::UT_CNATFWIdentification_SetStreamCollectionIdL(  )
    {
    iIdentifi->SetStreamCollectionId( KStreamCollectionId );
    EUNIT_ASSERT_EQUALS( KStreamCollectionId, iIdentifi->StreamCollectionId() );
    }

void UT_CNATFWIdentification::UT_CNATFWIdentification_SetStreamIdL(  )
    {
    iIdentifi->SetStreamId( KStreamId );
    EUNIT_ASSERT_EQUALS( KStreamId,  iIdentifi->StreamId() );
    }

void UT_CNATFWIdentification::UT_CNATFWIdentification_SetDirectionL(  )
    {
    iIdentifi->SetDirection( CNATFWCredentials::EOutbound );
    EUNIT_ASSERT_EQUALS( iIdentifi->Direction(), CNATFWCredentials::EOutbound );
    }

void UT_CNATFWIdentification::UT_CNATFWIdentification_SetUsernameLL(  )
    {
    NATFW_EUNIT_ASSERT_NO_LEAVE( iIdentifi->SetUsernameL( KUserName ) );
    EUNIT_ASSERT_EQUALS( KUserName(), iIdentifi->Username() );
    }

void UT_CNATFWIdentification::UT_CNATFWIdentification_SetPasswordLL(  )
    {
    NATFW_EUNIT_ASSERT_NO_LEAVE( iIdentifi->SetPasswordL( KPassword ) );
    EUNIT_ASSERT_EQUALS( KPassword(), iIdentifi->Password() );
    }

void UT_CNATFWIdentification::UT_CNATFWIdentification_CopyLL(  )
    {
    CNATFWCredentials* credentials1 = CNATFWCredentials::NewLC();
    TBuf8<1> usernameBuf1;
    usernameBuf1.Append( '1' );
    NATFW_EUNIT_ASSERT_NO_LEAVE(
        credentials1->SetUsernameL( usernameBuf1 ) );
    
    CNATFWCredentials* credentials2 = CNATFWCredentials::NewLC();
    TBuf8<1> usernameBuf2;
    usernameBuf2.Append( '2' );
    credentials2->SetUsernameL( usernameBuf2 );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( credentials1->CopyL( *credentials2 ) );
    EUNIT_ASSERT( *credentials1 == *credentials2 );
    
    CleanupStack::PopAndDestroy( credentials2 );
    CleanupStack::PopAndDestroy( credentials1 );
    }

void UT_CNATFWIdentification::UT_CNATFWIdentification_operatorsL(  )
    {
    CNATFWCredentials* id1 = CNATFWCredentials::NewLC();
        
    CNATFWCredentials* id2 = CNATFWCredentials::NewLC();
        
    EUNIT_ASSERT( *id1 == *id2 );
    EUNIT_ASSERT( !( *id2 != *id2 ) );
    
    CleanupStack::PopAndDestroy( id2 );
    CleanupStack::PopAndDestroy( id1 );
    }

void UT_CNATFWIdentification::UT_CNATFWIdentification_CompareL()
    {
    CNATFWCredentials* credentials1 = CNATFWCredentials::NewLC();
    TBuf8<1> usernameBuf1;
    usernameBuf1.Append( '1' );
    NATFW_EUNIT_ASSERT_NO_LEAVE( credentials1->SetUsernameL( usernameBuf1 ) );
    
    CNATFWCredentials* credentials2 = CNATFWCredentials::NewLC();
    TBuf8<1> usernameBuf2;
    usernameBuf2.Append( '2' );
    NATFW_EUNIT_ASSERT_NO_LEAVE( credentials2->SetUsernameL( usernameBuf2 ) );
    
    EUNIT_ASSERT( !credentials1->Compare( *credentials1, *credentials2 ) );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( credentials2->SetUsernameL( usernameBuf1 ) );
    
    EUNIT_ASSERT( credentials1->Compare( *credentials1, *credentials2 ) );
    
    CleanupStack::PopAndDestroy( credentials2 );
    CleanupStack::PopAndDestroy( credentials1 );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWIdentification,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CNATFWCredentials",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWIdentification_NewLL, Teardown)

EUNIT_TEST(
    "SetStreamCollectionId - test ",
    "CNATFWCredentials",
    "SetStreamCollectionId",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWIdentification_SetStreamCollectionIdL, Teardown)

EUNIT_TEST(
    "SetStreamId - test ",
    "CNATFWCredentials",
    "SetStreamId",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWIdentification_SetStreamIdL, Teardown)

EUNIT_TEST(
    "SetDirection - test ",
    "CNATFWCredentials",
    "SetDirection",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWIdentification_SetDirectionL, Teardown)

EUNIT_TEST(
    "SetUsernameL - test ",
    "CNATFWCredentials",
    "SetUsernameL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWIdentification_SetUsernameLL, Teardown)

EUNIT_TEST(
    "SetPasswordL - test ",
    "CNATFWCredentials",
    "SetPasswordL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWIdentification_SetPasswordLL, Teardown)

EUNIT_TEST(
    "CopyL - test ",
    "CNATFWCredentials",
    "CopyL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWIdentification_CopyLL, Teardown)

EUNIT_TEST(
    "operators - test ",
    "CNATFWCredentials",
    "operators",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWIdentification_operatorsL, Teardown)

EUNIT_TEST(
    "Compare - test ",
    "CNATFWCredentials",
    "Compare",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWIdentification_CompareL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
