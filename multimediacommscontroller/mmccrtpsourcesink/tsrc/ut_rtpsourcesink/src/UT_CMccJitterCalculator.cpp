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
#include "UT_CMccJitterCalculator.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "MccRtpInterface.h"

// CONSTRUCTION
UT_CMccJitterCalculator* UT_CMccJitterCalculator::NewL()
    {
    UT_CMccJitterCalculator* self = UT_CMccJitterCalculator::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMccJitterCalculator* UT_CMccJitterCalculator::NewLC()
    {
    UT_CMccJitterCalculator* self = new( ELeave ) UT_CMccJitterCalculator();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccJitterCalculator::~UT_CMccJitterCalculator()
    {
    }

// Default constructor
UT_CMccJitterCalculator::UT_CMccJitterCalculator()
    {
    }

// Second phase construct
void UT_CMccJitterCalculator::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

// Observer impl
void UT_CMccJitterCalculator::SendJitterEvent( TMccRtpEventData aEvent, TInt aError )
    {
    iLastEvent = aEvent;
    iLastError = aError;
    }
    
//  METHODS



void UT_CMccJitterCalculator::SetupL(  )
    {
    iCalculator = CMccJitterCalculator::NewL( *this );
    } 

void UT_CMccJitterCalculator::Teardown(  )
    {
    delete iCalculator;
    }

void UT_CMccJitterCalculator::UT_CMccJitterCalculator_StartObservingL()
    {
    // Test delete of inactive calculator
    delete iCalculator;
    iCalculator = NULL;
    
    iCalculator = CMccJitterCalculator::NewL( *this );
    
    // Jitter reports
    iCalculator->iReportType = EMccJitterReport;
    EUNIT_ASSERT_EQUALS( iCalculator->StartObserving(), KErrNone );
    EUNIT_ASSERT_EQUALS( iCalculator->iJitterObsOn, ETrue );
    
    // Packet loss reports
    iCalculator->iReportType = EMccPacketLossReport;
    EUNIT_ASSERT_EQUALS( iCalculator->StartObserving(), KErrNone );
    EUNIT_ASSERT_EQUALS( iCalculator->iFrameLossObsOn, ETrue );
    
    // All reports
    iCalculator->iFrameLossObsOn = EFalse;
    iCalculator->iJitterObsOn = EFalse;
    iCalculator->iMediaQualityObservingStarted = EFalse;
    iCalculator->iReportType = EMccQualityReportAll;
    EUNIT_ASSERT_EQUALS( iCalculator->StartObserving(), KErrNone );
    EUNIT_ASSERT_EQUALS( iCalculator->iJitterObsOn, ETrue );
    EUNIT_ASSERT_EQUALS( iCalculator->iFrameLossObsOn, ETrue );
    EUNIT_ASSERT_EQUALS( iCalculator->iMediaQualityObservingStarted, ETrue );
   
    // Not supported reports
    iCalculator->iReportType = 998;
    EUNIT_ASSERT_EQUALS( iCalculator->StartObserving(), KErrArgument );
    }

void UT_CMccJitterCalculator::UT_CMccJitterCalculator_JitterObservingL()
    {
    iCalculator->iHomeTime.HomeTime();
    
    // Not active
    iCalculator->JitterObserving();
    
    iCalculator->iPacketsReceived = 5;
    iCalculator->iJitterLevelFromClient = 10000;
    iCalculator->iCurJitter = 10000;
    iCalculator->iJitterObsOn = ETrue;
    
    // Time based reporting   
    // 
    iCalculator->iReportIntervalType = EMccQualityTimeBased;
    iCalculator->iCurTime.HomeTime();
    
    // Not reported as not enough time elapsed
    iCalculator->iReportInterval = 100000;
    iCalculator->JitterObserving();
    EUNIT_ASSERT_EQUALS( iLastEvent.iPacketsReceived, 0 );
    
    // Enough time elapsed but not over jitter level defined by client
    iCalculator->iReportInterval = 0;
    iCalculator->JitterObserving();
    EUNIT_ASSERT_EQUALS( iLastEvent.iPacketsReceived, 0 );
    
    // Jitter level defined by client exceeded
    iCalculator->iJitterLevelFromClient = 10;
    iCalculator->JitterObserving();
    EUNIT_ASSERT_EQUALS( iLastEvent.iPacketsReceived, 5 ); // just making sure that callback was called
    
    // Quality based reporting
    //
    iCalculator->iReportIntervalType = EMccQualityPacketBased;
    iLastEvent.iPacketsReceived = 0;
    iCalculator->iReportInterval = 2;
    iCalculator->iJitterObsOn = ETrue;
    
    // Not the packet when calculation is done
    iCalculator->iReceivedPacketCounter = 0;
    iCalculator->JitterObserving();
    EUNIT_ASSERT_EQUALS( iCalculator->iReceivedPacketCounter, 1 );
    
    // Now the interval is reached
    iCalculator->JitterObserving();
    EUNIT_ASSERT_EQUALS( iLastEvent.iPacketsReceived, 5 ); // just making sure that callback was called
    EUNIT_ASSERT_EQUALS( iCalculator->iReceivedPacketCounter, 0 ); // counter is zeroed
    
    // Other reporting
    iCalculator->iReportIntervalType = 998;
    iLastEvent.iPacketsReceived = 0;
    iCalculator->iJitterObsOn = ETrue;
    iCalculator->JitterObserving();
    EUNIT_ASSERT_EQUALS( iLastEvent.iPacketsReceived, 5 ); // just making sure that callback was called
    }

void UT_CMccJitterCalculator::UT_CMccJitterCalculator_RtpPacketReceivedL()
    {
    // TBD: real assert for values
    
    const TInt KTestConversion = 8000;
    
    // First packet
    const TInt firstPacketTimestamp = 1000;
    iCalculator->RtpPacketReceived( firstPacketTimestamp, EFalse );
    EUNIT_ASSERT( iCalculator->iPrevPacketTransit != 0 );
    
    // Second packet (fake that it is the one when reporting)
    iCalculator->iPacketsReceived = 15; // ( KJitterReportInterval - 1 )
    const TInt secondPacketTimestamp = 3000;
    iCalculator->RtpPacketReceived( secondPacketTimestamp, EFalse );
    EUNIT_ASSERT( iCalculator->iCurJitter != 0 );
    EUNIT_ASSERT_EQUALS( (TInt)iLastEvent.iPacketsReceived, 16 ); // just making sure that callback was called
    }
    
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMccJitterCalculator,
    "Add test suite description here.",
    "UNIT" )
    
EUNIT_TEST(
    "StartObserving - test ",
    "CMccJitterCalculator",
    "StartObserving",
    "FUNCTIONALITY",
    SetupL, UT_CMccJitterCalculator_StartObservingL, Teardown)

EUNIT_TEST(
    "JitterObserving - test ",
    "CMccJitterCalculator",
    "JitterObserving",
    "FUNCTIONALITY",
    SetupL, UT_CMccJitterCalculator_JitterObservingL, Teardown)
    
EUNIT_TEST(
    "RtpPacketReceived - test ",
    "CMccJitterCalculator",
    "RtpPacketReceived",
    "FUNCTIONALITY",
    SetupL, UT_CMccJitterCalculator_RtpPacketReceivedL, Teardown)
    
EUNIT_END_TEST_TABLE

//  END OF FILE
