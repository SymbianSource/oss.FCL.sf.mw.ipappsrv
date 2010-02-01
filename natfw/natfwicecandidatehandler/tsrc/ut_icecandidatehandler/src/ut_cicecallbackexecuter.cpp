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
#include "UT_CIceCallBackExecuter.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "cicecallbackexecuter.h"

//  CONSTANTS
const TInt KTestCallBackEnum = 1;

// CONSTRUCTION
UT_CIceCallBackExecuter* UT_CIceCallBackExecuter::NewL()
    {
    UT_CIceCallBackExecuter* self = UT_CIceCallBackExecuter::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CIceCallBackExecuter* UT_CIceCallBackExecuter::NewLC()
    {
    UT_CIceCallBackExecuter* self = new( ELeave ) UT_CIceCallBackExecuter();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CIceCallBackExecuter::~UT_CIceCallBackExecuter()
    {
    }

// Default constructor
UT_CIceCallBackExecuter::UT_CIceCallBackExecuter()
    {
    }

// Second phase construct
void UT_CIceCallBackExecuter::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CIceCallBackExecuter::SetupL(  )
    {
    iExecuter = CIceCallBackExecuter::NewL( CActive::EPriorityStandard );
    }

void UT_CIceCallBackExecuter::Teardown(  )
    {
    delete iExecuter;
    iExecuter = NULL;
    }

void UT_CIceCallBackExecuter::UT_CICECallBackExecuter_AddCallBackLL(  )
    {
    TIceCallBack callback( HandleCallBack, *this, KTestCallBackEnum, 0 );
    iExecuter->AddCallBackL( callback );
    iExecuter->AddCallBackL( callback );
    iExecuter->AddCallBackL( callback );
    CActiveScheduler::Start();
    CActiveScheduler::Start();
    CActiveScheduler::Start();
    
    iExecuter->AddCallBackL( callback );
    iExecuter->AddCallBackL( callback );
    iExecuter->Cancel();
    }


void UT_CIceCallBackExecuter::HandleCallBack( CBase& aObject, TInt aCallBackType )
    {
    UT_CIceCallBackExecuter& object( 
        static_cast<UT_CIceCallBackExecuter&>( aObject ) );
    
    object.DoHandleCallBack( aCallBackType );
    }

void UT_CIceCallBackExecuter::DoHandleCallBack( TInt aCallBackType )
    {
    EUNIT_ASSERT( KTestCallBackEnum == aCallBackType );
    CActiveScheduler::Stop();
    }
    
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CIceCallBackExecuter,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "AddCallBackL - test ",
    "CIceCallBackExecuter",
    "AddCallBackL",
    "FUNCTIONALITY",
    SetupL, UT_CICECallBackExecuter_AddCallBackLL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
