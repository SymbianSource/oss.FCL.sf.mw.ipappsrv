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
#include "UT_CMccrtpmediaclock.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <e32debug.h>
#include <e32std.h>

//  INTERNAL INCLUDES
#include "mccrtpmediaclock.h"

//  CONSTANTS
const TUint Ksampling8 = 8000;
const TUint Ksampling16 = 16000;
const TInt KhwFt10 = 10;
const TInt KhwFt20 = 20;
const TInt KhwFt40 = 40;


// CONSTRUCTION
UT_CMccrtpmediaclock* UT_CMccrtpmediaclock::NewL()
    {
    UT_CMccrtpmediaclock* self = UT_CMccrtpmediaclock::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccrtpmediaclock* UT_CMccrtpmediaclock::NewLC()
    {
    UT_CMccrtpmediaclock* self = new( ELeave ) UT_CMccrtpmediaclock();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccrtpmediaclock::~UT_CMccrtpmediaclock()
    {
    }

// Default constructor
UT_CMccrtpmediaclock::UT_CMccrtpmediaclock()
    {
    }

// Second phase construct
void UT_CMccrtpmediaclock::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS
void UT_CMccrtpmediaclock::SetupL()
    {
    iClient = CMccRtpMediaClock::NewL();
    }

void UT_CMccrtpmediaclock::Teardown()
    {
    delete iClient;
    }

void UT_CMccrtpmediaclock::UT_CMccrtpmediaclock_RegisterMediaFormatL()
    {
    TUint32 key = iClient->RegisterMediaFormat( Ksampling8, KhwFt10 );
    EUNIT_ASSERT( key );
    RDebug::Print( _L("key = %d"), key );
    }

void UT_CMccrtpmediaclock::UT_CMccrtpmediaclock_UnregisterMediaFormatL()
    {
    TUint32 key = 0;
    iClient->UnregisterMediaFormat( key );
    key = iClient->RegisterMediaFormat( Ksampling8, KhwFt10 );
    iClient->UnregisterMediaFormat( key );
    RArray<TUint32> keys;
    CleanupClosePushL( keys );
    for( TInt k = 0; k < 100; k++ )
        {
        TUint32 ck = iClient->RegisterMediaFormat( Ksampling8, KhwFt10 );
        keys.Append( ck );
        }
    
    while( keys.Count() )
        {
        iClient->UnregisterMediaFormat( keys[keys.Count() - 1] );
        keys.Remove( keys.Count() - 1 );
        }
    
    CleanupStack::PopAndDestroy( &keys );
    }

void UT_CMccrtpmediaclock::UT_CMccrtpmediaclock_GetTimeStampL()
    {
    TUint32 timestamp(0);
    TUint32 key_not_reg = iClient->GetTimeStamp( 1, timestamp );
    EUNIT_ASSERT_EQUALS( key_not_reg, KErrNotFound );
    
    TUint32 key = iClient->RegisterMediaFormat( Ksampling8, KhwFt10 );
    
    TInt err = iClient->GetTimeStamp( key, timestamp );
    if ( err == KErrNotFound )
    	{
    	// RegisterMediaFormat may fail silently in case of running out of memory
    	User::Leave( KErrNoMemory );
    	}
    EUNIT_ASSERT_EQUALS( KErrNone, err );
    }

void UT_CMccrtpmediaclock::UT_CMccrtpmediaclock_LatestConsumedTSL()
    {
    TUint32 timestamp(0);
    iClient->GetLatestConsumedTS( timestamp );
    
    EUNIT_ASSERT( ETrue );
    }

void UT_CMccrtpmediaclock::TestMultiClientL()
    {
    TUint32 timestamp1(0), timestamp2(0), timestamp3(0), timestamp4(0);
    TInt err1(0), err2(0), err3(0), err4(0);
    TUint32 key1 = iClient->RegisterMediaFormat( Ksampling8, KhwFt10 );
    TUint32 key2 = iClient->RegisterMediaFormat( Ksampling8, KhwFt20 );
    TUint32 key3 = iClient->RegisterMediaFormat( Ksampling16, KhwFt20 );
    TUint32 key4 = iClient->RegisterMediaFormat( Ksampling8, KhwFt40 );
    
    // do some keep alive before starting media clock
    RDebug::Print( _L("***TEST PREAUDIO KEEPALIVE***") );
    iClient->GetLatestConsumedTS( timestamp1 );

    // wait for 10ms
    User::After(10000);
    // ask keep alive timestamp again (should be the same)
    iClient->GetLatestConsumedTS( timestamp1 );
    User::After( 10000 );
    // ask keep alive timestamp again (should be the same)
    iClient->GetLatestConsumedTS( timestamp2 );
    
    EUNIT_ASSERT_EQUALS( timestamp1, timestamp2 );
    
    RDebug::Print( _L("***TEST TIME STAMP INCREMENT***") );
    User::After( 20000 );
    timestamp2 = 0;
    err2 = iClient->GetTimeStamp( key2, timestamp2 );
    if ( err2 == KErrNotFound )
    	{
    	// RegisterMediaFormat may fail silently in case of running out of memory
    	User::Leave( KErrNoMemory );
    	}

    if ( !err2 )
        {
        EUNIT_ASSERT( timestamp2 );
        }
    else
        {
        RDebug::Print( _L("error2 %d"), err2 );
        EUNIT_ASSERT( EFalse );
        }
    
    User::After( 20000 );
    err2 = iClient->GetTimeStamp( key2, timestamp2 );
    
    if ( err2 )
        {
        RDebug::Print( _L("error2 %d"), err2 );
        EUNIT_ASSERT( EFalse );
        }
    
    User::After( 10000 );
    err4 = iClient->GetTimeStamp( key4, timestamp4 );
    
    if ( err4 )
        {
        RDebug::Print( _L("error4 %d"), err4 );
        EUNIT_ASSERT( EFalse );
        }

    User::After( 40000 );
    err3 = iClient->GetTimeStamp( key4, timestamp4 );    
    if ( err4 )
        {
        RDebug::Print( _L("error4 %d"), err4 );
        EUNIT_ASSERT( EFalse );
        }    
        
    RDebug::Print( _L("***TEST KEEP ALIVE (CALL ON HOLD)***") );
    
    User::After( 20000 );
    iClient->GetLatestConsumedTS( timestamp2 );
    
    RDebug::Print( _L("keep alive ongoing, timestamp does not increase") );
    EUNIT_ASSERT_EQUALS( timestamp2, timestamp4 );

    
    User::After( 20000 );
    iClient->GetLatestConsumedTS( timestamp2 );

    RDebug::Print( _L("keep alive ongoing, timestamp does not increase") );
    EUNIT_ASSERT_EQUALS( timestamp2, timestamp4 );
        
    User::After( 20000 );
    iClient->GetLatestConsumedTS( timestamp2 );
    
    RDebug::Print( _L("keep alive ongoing, timestamp does not increase") );
    EUNIT_ASSERT_EQUALS( timestamp2, timestamp4 );
    
    RDebug::Print( _L("***TEST TIMESTAMPS (UNHOLD)***") );
    
    // User::After( 40000 );
    err4 = iClient->GetTimeStamp( key4, timestamp4 );
    if ( err4 )
        {
        EUNIT_ASSERT( EFalse );
        }
        
    User::After( 40000 );
    err4 = iClient->GetTimeStamp( key4, timestamp4 );
    if ( err4 )
        {
        EUNIT_ASSERT( EFalse );
        }
        
    User::After( 40000 );
    err4 = iClient->GetTimeStamp( key4, timestamp4 );
    if ( err4 )
        {
        EUNIT_ASSERT( EFalse );
        }
    
    iClient->UnregisterMediaFormat( key2 );
    iClient->UnregisterMediaFormat( key4 );
    iClient->UnregisterMediaFormat( key3 );
    iClient->UnregisterMediaFormat( key1 );
    }

void UT_CMccrtpmediaclock::TestRunL()   
    {
    // generate many stamps in a row
    TUint32 timestamp1(0), timestamp2(0);
    TInt err1( KErrNone );
    TUint32 key1 = iClient->RegisterMediaFormat( Ksampling8, KhwFt40 );
    TUint32 key2 = iClient->RegisterMediaFormat( Ksampling8, KhwFt20 );

    err1 = iClient->GetTimeStamp( key1, timestamp1 );
    if ( err1 == KErrNotFound )
    	{
    	// RegisterMediaFormat may fail silently in case of running out of memory
    	User::Leave( KErrNoMemory );
    	}

    RDebug::Print( _L("***TEST RUN***") );
    
    for ( TInt i = 0; i < 20; i++ )
        {
        RDebug::Print( _L("timestamp increment...") );
        User::After( 5000 );
        err1 = iClient->GetTimeStamp( key1, timestamp1 );
        EUNIT_ASSERT_EQUALS( err1, KErrNone );
        }
    
    for ( TInt i = 0; i < 20; i++ )
        {
        RDebug::Print( _L("hold (keep alive)...") );
        User::After( 10000 );
        iClient->GetLatestConsumedTS( timestamp2 );
        EUNIT_ASSERT_EQUALS( timestamp2, timestamp1 );
        }
    
    for ( TInt i = 0; i < 20; i++ )
        {
        RDebug::Print( _L("continue timestamp increment...") );
        User::After( 5000 );
        err1 = iClient->GetTimeStamp( key1, timestamp1 );
        EUNIT_ASSERT_EQUALS( err1, KErrNone );
        }
    }
    
//  TEST TABLE
EUNIT_BEGIN_TEST_TABLE(
    UT_CMccrtpmediaclock,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "RegisterMediaFormat - test ",
    "CMccrtpmediaclock",
    "RegisterMediaFormat",
    "FUNCTIONALITY",
    SetupL, UT_CMccrtpmediaclock_RegisterMediaFormatL, Teardown)

EUNIT_TEST(
    "UnregisterMediaFormat - test ",
    "CMccrtpmediaclock",
    "UnregisterMediaFormat",
    "FUNCTIONALITY",
    SetupL, UT_CMccrtpmediaclock_UnregisterMediaFormatL, Teardown)

EUNIT_TEST(
    "GetTimeStamp - test ",
    "CMccrtpmediaclock",
    "GetTimeStamp",
    "FUNCTIONALITY",
    SetupL, UT_CMccrtpmediaclock_GetTimeStampL, Teardown)

EUNIT_TEST(
    "LatestConsumedTS - test ",
    "CMccrtpmediaclock",
    "LatestConsumedTS",
    "FUNCTIONALITY",
    SetupL, UT_CMccrtpmediaclock_LatestConsumedTSL, Teardown)

EUNIT_TEST(
    "TestMultiClient - test ",
    "CMccrtpmediaclock",
    "TestMultiClient",
    "FUNCTIONALITY",
    SetupL, TestMultiClientL, Teardown)

EUNIT_TEST(
    "TestRun - test ",
    "CMccrtpmediaclock",
    "TestRun",
    "FUNCTIONALITY",
    SetupL, TestRunL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
