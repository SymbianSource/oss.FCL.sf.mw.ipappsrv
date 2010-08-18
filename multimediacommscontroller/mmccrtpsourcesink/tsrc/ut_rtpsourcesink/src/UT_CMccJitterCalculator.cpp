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
void UT_CMccJitterCalculator::SendJitterEvent( TMccRtpEventDataExtended aEvent, TInt aError )
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
    
    iCalculator->iStartingSeqNum = 80;
    iCalculator->iCurrentSeqNum = 100; 
    iCalculator->iReceivedPacketCounter = 20; // 100-80=20, no packets lost
    
    iCalculator->iPacketsReceived = 5;
    iCalculator->iJitterLevelFromClient = 10000;
    iCalculator->iCurJitter = 10000;
    iCalculator->iJitterObsOn = ETrue;
    iCalculator->iFrameLossObsOn = ETrue;
    
    // Time based reporting   
    // 
    iCalculator->iReportIntervalType = EMccQualityTimeBased;
    iCalculator->iCurTime.HomeTime();
    
    // Not reported as not enough time elapsed
    iCalculator->iReportInterval = 100000;
    iCalculator->JitterObserving();
    EUNIT_ASSERT_EQUALS( iLastEvent.iPacketsReceived, 0 );
    EUNIT_ASSERT_EQUALS( iLastEvent.iPacketLoss, 0 );
    
    // Enough time elapsed but not over jitter level defined by client
    iCalculator->iReportInterval = 0;
    iCalculator->JitterObserving();
    EUNIT_ASSERT_EQUALS( iLastEvent.iPacketsReceived, 0 );
    EUNIT_ASSERT_EQUALS( iLastEvent.iPacketLoss, 0 );
    
    // Jitter and packet loss level defined by client exceeded
    iCalculator->iStartingSeqNum = 100;
    iCalculator->iCurrentSeqNum = 125; 
    iCalculator->iPrevFerValue = 93000; // 9%

    iCalculator->iPrevExpectedPackets = 20;
    iCalculator->iReceivedPacketCounter = 20; // 5 packets is lost. 

    iCalculator->iJitterLevelFromClient = 10;
    User::After( 5000 );
    iCalculator->JitterObserving();
    EUNIT_ASSERT_EQUALS( iLastEvent.iPacketsReceived, 5 ); // just making sure that callback was called
    EUNIT_ASSERT_EQUALS( iLastEvent.iPacketLoss, 19 );
    
    // Quality based reporting
    //
    EUNIT_ASSERT_EQUALS( iCalculator->iReceivedPacketCounter, 20 ); // counter is not zeroed
    
    iCalculator->iReportIntervalType = EMccQualityPacketBased;
    iCalculator->iJitterObsOn = ETrue;
    iCalculator->iFrameLossObsOn = ETrue;
    iLastEvent.iPacketsReceived = 0;
    iLastEvent.iPacketLoss = 0;
    iCalculator->iReportInterval = 2;
    iCalculator->iPacketsReceived = 2;

    iCalculator->iStartingSeqNum = 120;
    iCalculator->iCurrentSeqNum = 121; 
    iCalculator->iReceivedPacketCounter = 1; 
    iCalculator->iCurJitter = 10000;
    iCalculator->iJitterLevelFromClient = 10000;
    
    iCalculator->JitterObserving();
    //Check that nothing was changed in iLastEvent:
    EUNIT_ASSERT_EQUALS( iLastEvent.iPacketsReceived, 0 );
    EUNIT_ASSERT_EQUALS( iLastEvent.iPacketLoss, 0 );
    
    // One packet more...
    iCalculator->iPrevFerValue = 0; // 0%
    iCalculator->iCurrentSeqNum++;
    iCalculator->iReceivedPacketCounter++; 
    iCalculator->JitterObserving();
    //Check that nothing was changed in iLastEvent:
    EUNIT_ASSERT_EQUALS( iLastEvent.iPacketsReceived, 0 );
    EUNIT_ASSERT_EQUALS( iLastEvent.iPacketLoss, 0 );
    
    //Do the packet loss, with reporting:
    iCalculator->iStartingSeqNum = 122;
    iCalculator->iCurrentSeqNum = 126; 
    iCalculator->iPrevFerValue = 98000; // 9%
    iCalculator->iPrevExpectedPackets = 1;
    
    iCalculator->iReceivedPacketCounter = 2; 
    iCalculator->iCurJitter = 10000;
    iCalculator->iJitterLevelFromClient = 10;
    iCalculator->JitterObserving();
    //Check that values were changed in iLastEvent:
    EUNIT_ASSERT_EQUALS( iLastEvent.iPacketsReceived, 2 );
    EUNIT_ASSERT_EQUALS( iLastEvent.iPacketLoss, 20 );
    
    // Other reporting
    iCalculator->iReportIntervalType = 998;
    iLastEvent.iPacketsReceived = 0;
    iCalculator->iJitterObsOn = ETrue;
    iCalculator->iFrameLossObsOn = ETrue;
    iCalculator->JitterObserving();
    EUNIT_ASSERT_EQUALS( iLastEvent.iPacketsReceived, 2 ); // just making sure that callback was called
    EUNIT_ASSERT_EQUALS( iLastError, KErrArgument ); 
    }

void UT_CMccJitterCalculator::UT_CMccJitterCalculator_RtpPacketReceivedL()
    {
    iCalculator->iReportIntervalType = EMccQualityPacketBased;
    // TBD: real assert for values
    
    const TInt KTestConversion = 8000;
    TUint testSeqNum = 222;
    iCalculator->iCurrentSeqNum = 0;
    iCalculator->iStartingSeqNum = 0;
    iCalculator->iReceivedPacketCounter = 0;
    
    // First packet
    const TInt PacketTimestamp1 = 1000;
    iCalculator->RtpPacketReceived( PacketTimestamp1, EFalse, testSeqNum );
    EUNIT_ASSERT( iCalculator->iPrevPacketTransit != 0 );
    EUNIT_ASSERT( iCalculator->iCurrentSeqNum == 222 );
    EUNIT_ASSERT( iCalculator->iStartingSeqNum == 221 );
    EUNIT_ASSERT( iCalculator->iReceivedPacketCounter == 1 );
    
    testSeqNum++; //Increase seqnum for next packet. 
    // Second packet (fake that it is the one when reporting)
    iCalculator->iPacketsReceived = 15; // ( KJitterReportInterval - 1 )
    const TInt PacketTimestamp2 = 3000;
    iCalculator->RtpPacketReceived( PacketTimestamp2, EFalse, testSeqNum );
    EUNIT_ASSERT( iCalculator->iCurJitter != 0 );
    EUNIT_ASSERT_EQUALS( (TInt)iLastEvent.iPacketsReceived, 16 ); // just making sure that callback was called
    EUNIT_ASSERT( iCalculator->iCurrentSeqNum == 223 );
    EUNIT_ASSERT( iCalculator->iStartingSeqNum == 221 );
    EUNIT_ASSERT( iCalculator->iReceivedPacketCounter == 2 );
    
    
    iCalculator->iReportIntervalType = EMccQualityTimeBased;
    iCalculator->iFrameLossObsOn = EFalse;
    testSeqNum++; //Increase seqnum for next packet. 

    const TInt packetTimestamp3 = 4000;
    iCalculator->RtpPacketReceived( packetTimestamp3, EFalse, testSeqNum );
    EUNIT_ASSERT( iCalculator->iCurJitter != 0 );
    EUNIT_ASSERT_EQUALS( (TInt)iLastEvent.iPacketsReceived, 16 ); // just making sure that callback wasn't called
    EUNIT_ASSERT( iCalculator->iCurrentSeqNum == 223 );
    EUNIT_ASSERT( iCalculator->iStartingSeqNum == 221 );
    EUNIT_ASSERT( iCalculator->iReceivedPacketCounter == 2 );
    
    // Sequence number start from 0
    iCalculator->iReportIntervalType = EMccQualityPacketBased;
    iCalculator->iFrameLossObsOn = ETrue;
    testSeqNum = 0;
    const TInt packetTimestamp4 = 5000;
    iCalculator->RtpPacketReceived( packetTimestamp4, EFalse, testSeqNum );
    EUNIT_ASSERT( iCalculator->iCurJitter != 0 );
    EUNIT_ASSERT_EQUALS( (TInt)iLastEvent.iPacketsReceived, 16 ); // just making sure that callback wasn't called
    EUNIT_ASSERT_EQUALS( iCalculator->iCurrentSeqNum, 0);
    EUNIT_ASSERT( iCalculator->iStartingSeqNum == 0 );
    EUNIT_ASSERT( iCalculator->iReceivedPacketCounter == 3 );
    
    // Sequence number start from 1
    testSeqNum = 1;
    const TInt packetTimestamp5 = 6000;
    iCalculator->RtpPacketReceived( packetTimestamp5, EFalse, testSeqNum );
    EUNIT_ASSERT( iCalculator->iCurJitter != 0 );
    EUNIT_ASSERT_EQUALS( (TInt)iLastEvent.iPacketsReceived, 16 ); // just making sure that callback wasn't called
    EUNIT_ASSERT_EQUALS( iCalculator->iCurrentSeqNum, 1);
    EUNIT_ASSERT( iCalculator->iStartingSeqNum == 0 );
    EUNIT_ASSERT( iCalculator->iReceivedPacketCounter == 4 );
    
    }
    

void UT_CMccJitterCalculator::UT_CMccJitterCalculator_CalculateFerL()
    {
    TUint32 returnValue( 0 );
    
    // packet  lost
    TUint32 prevValue( 0 );
    TBool valueToadd( ETrue );
    TBool flagToAdd( ETrue );
    returnValue = iCalculator->CalculateFer( prevValue, valueToadd, flagToAdd );
    EUNIT_ASSERT_EQUALS( returnValue, 2000 );
    
    // packet  lost
    // returnValue == 2000
    returnValue = iCalculator->CalculateFer( returnValue, valueToadd, flagToAdd );
    EUNIT_ASSERT_EQUALS( returnValue, 3992 );
    
    // no packet lost - late packet - remove FER
    // decrease fer
    flagToAdd = EFalse;
    // returnValue == 3996
    returnValue = iCalculator->CalculateFer( returnValue, valueToadd, flagToAdd );
    
    EUNIT_ASSERT_EQUALS( returnValue, 2008 );
    
    // no packet lost - late packet - remove FER
    // decrease fer
    flagToAdd = EFalse;
    // returnValue == 5
    returnValue = 5;
    returnValue = iCalculator->CalculateFer( returnValue, valueToadd, flagToAdd );
    
    EUNIT_ASSERT_EQUALS( returnValue, 0);
    }

void UT_CMccJitterCalculator::UT_CMccJitterCalculator_CountPacketLossPercentageL()
    {
    TInt percentage( 0 );
    
    iCalculator->iHomeTime.HomeTime();
    
    // Not active
    iCalculator->JitterObserving();
    
    iCalculator->iPrevFerValue = 0;
    iCalculator->iStartingSeqNum = 80;
    iCalculator->iCurrentSeqNum = 100;
    
    // no packets lost
    iCalculator->iPrevExpectedPackets = 20;
    iCalculator->iReceivedPacketCounter = 20;

    iCalculator->CountPacketLossPercentage();
    
    EUNIT_ASSERT_EQUALS( iCalculator->iPrevFerValue, 0 );
    EUNIT_ASSERT_EQUALS( iCalculator->iPrevExpectedPackets, 20 );
    EUNIT_ASSERT_EQUALS( iCalculator->iPrevPacketsReceived, 20 );
    
    
    // no packets lost, inorder
    iCalculator->iCurrentSeqNum = 102;  
    iCalculator->iPrevExpectedPackets = 22;

    iCalculator->CountPacketLossPercentage();
    
    EUNIT_ASSERT_EQUALS( iCalculator->iPrevFerValue, 0 );
    EUNIT_ASSERT_EQUALS( iCalculator->iPrevExpectedPackets, 22 );
    EUNIT_ASSERT_EQUALS( iCalculator->iPrevPacketsReceived, 20 );
    
	
    // packet  lost - add FER by number of packets lost.
    // 5 packets lost
    iCalculator->iPrevFerValue = 0;
    iCalculator->iStartingSeqNum = 80;
    iCalculator->iCurrentSeqNum = 105;

    iCalculator->iPrevExpectedPackets = 20;
    iCalculator->iReceivedPacketCounter = 20;

    percentage = iCalculator->CountPacketLossPercentage();
    
    EUNIT_ASSERT_EQUALS( percentage, 1 );
    EUNIT_ASSERT_EQUALS( iCalculator->iPrevFerValue, 7952 );
    EUNIT_ASSERT_EQUALS( iCalculator->iPrevExpectedPackets, 25 );
    EUNIT_ASSERT_EQUALS( iCalculator->iPrevPacketsReceived, 20 );
    }

void UT_CMccJitterCalculator::UT_CMccJitterCalculator_ResetCountersL()
    {
    
    iCalculator->iPacketsReceived = 20;
    iCalculator->iCurJitter = 200;
    iCalculator->iPrevPacketTransit = 100;
    iCalculator->iCurrentSeqNum = 25;
    iCalculator->iStartingSeqNum = 2;
    iCalculator->iReceivedPacketCounter = 30;

    iCalculator->ResetCounters();
    
    EUNIT_ASSERT_EQUALS( iCalculator->iPacketsReceived, 0 );
    EUNIT_ASSERT_EQUALS( iCalculator->iCurJitter, 0 );
    EUNIT_ASSERT_EQUALS( iCalculator->iPrevPacketTransit, 0 );
    EUNIT_ASSERT_EQUALS( iCalculator->iCurrentSeqNum, 0 );
    EUNIT_ASSERT_EQUALS( iCalculator->iStartingSeqNum, 0 );
    EUNIT_ASSERT_EQUALS( iCalculator->iReceivedPacketCounter, 0 );
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMccJitterCalculator,
    "CMccJitterCalculator",
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
    

EUNIT_TEST(
    "CalculateFer - test ",
    "CMccJitterCalculator",
    "CalculateFer",
    "FUNCTIONALITY",
    SetupL, UT_CMccJitterCalculator_CalculateFerL, Teardown)

EUNIT_TEST(
    "CountPacketLossPercentage - test ",
    "CMccJitterCalculator",
    "CountPacketLossPercentage",
    "FUNCTIONALITY",
    SetupL, UT_CMccJitterCalculator_CountPacketLossPercentageL, Teardown)
    
EUNIT_TEST(
    "ResetCounters - test ",
    "CMccJitterCalculator",
    "ResetCounters",
    "FUNCTIONALITY",
    SetupL, UT_CMccJitterCalculator_ResetCountersL, Teardown)
    
    
EUNIT_END_TEST_TABLE

//  END OF FILE
