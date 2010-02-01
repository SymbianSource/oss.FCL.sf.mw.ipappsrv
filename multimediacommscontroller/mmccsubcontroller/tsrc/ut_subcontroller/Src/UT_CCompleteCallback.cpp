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
#include "UT_CCompleteCallback.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "mcccompletecallback.h"
#include "mccresourcepool_stub.h"

// CONSTRUCTION
UT_CCompleteCallback* UT_CCompleteCallback::NewL()
    {
    UT_CCompleteCallback* self = UT_CCompleteCallback::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CCompleteCallback* UT_CCompleteCallback::NewLC()
    {
    UT_CCompleteCallback* self = new( ELeave ) UT_CCompleteCallback();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CCompleteCallback::~UT_CCompleteCallback()
    {
    }

// Default constructor
UT_CCompleteCallback::UT_CCompleteCallback()
    {
    }

// Second phase construct
void UT_CCompleteCallback::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

TInt UT_CCompleteCallback::SendEventToClient( const TMMFEvent& /*aEvent*/ )
    {
    return KErrNone;
    }

void UT_CCompleteCallback::SetupL(  )
    {
    iResources = CMccResourcePoolStub::NewL();
    TMediaId media = KUidMediaTypeAudio;
    iDataPath = CMccUlDataPath::NewL( this, iResources, media );
    TBool waitForSink( EFalse );
    iCallback = new ( ELeave ) CCompleteCallback( *this, waitForSink );
    }

void UT_CCompleteCallback::Teardown(  )
    {
    if ( iCallback )
        {
        if ( iCallback->IsActive() )
            {
            iCallback->Cancel();
            }
        delete iCallback;
        iCallback = NULL;
        }
    if ( iDataPath )
        {
        if ( iDataPath->IsActive() )
            {
            iDataPath->Cancel();
            }
        delete iDataPath;
        iDataPath = NULL;
        }
    delete iResources;
    }

void UT_CCompleteCallback::UT_CCompleteCallback_CCompleteCallbackL(  )
    {
    TMediaId media = KUidMediaTypeAudio;
    CMccDlDataPath* dataPath = CMccDlDataPath::NewL( this, iResources, media );
    CleanupStack::PushL( dataPath );
    TBool waitForSink( EFalse );
    CCompleteCallback* callBack = new ( ELeave ) CCompleteCallback( *this, waitForSink );
    delete callBack;
    callBack = NULL;
    CleanupStack::PopAndDestroy( dataPath );
    }

void UT_CCompleteCallback::UT_CCompleteCallback_SignalDataPathCompleteL(  )
    {
    iCallback->SignalDataPathComplete( KErrNone );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CCompleteCallback,
    "CCompleteCallback",
    "UNIT" )

EUNIT_TEST(
    "CCompleteCallback - test ",
    "CCompleteCallback",
    "CCompleteCallback",
    "FUNCTIONALITY",
    SetupL, UT_CCompleteCallback_CCompleteCallbackL, Teardown)

EUNIT_TEST(
    "SignalDataPathComplete - test ",
    "CCompleteCallback",
    "SignalDataPathComplete",
    "FUNCTIONALITY",
    SetupL, UT_CCompleteCallback_SignalDataPathCompleteL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
