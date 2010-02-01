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
#include "UT_CMccQosController.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>

#ifdef EUNIT_ALLOC_TEST_ON
#include <digia/eunit/CEunitAllocTestCaseDecorator.h>  //for using EUNIT_ALLOC_TEST
#endif // end of EUNIT_ALLOC_TEST_ON

//#define _DEBUG
//#endif
//  INTERNAL INCLUDES
#include "MccQosController.h"
#include "mccrateadaptationobserver_stub.h"
#include "mccresourcepool.h"
#include "mccrtpdatasource.h"
#include "mccrtpdatasink.h"
#include "mccuids.hrh"


const TUid KRtpDataSource =  { KImplUidRtpDataSource };
const TUid KRtpDataSink =  { KImplUidRtpDataSink };

// CONSTRUCTION
UT_CMccQosController* UT_CMccQosController::NewL()
    {
    UT_CMccQosController* self = new( ELeave ) UT_CMccQosController();
    CleanupStack::PushL( self );

    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// Destructor (virtual by CBase)
UT_CMccQosController::~UT_CMccQosController()
    {
    }

// Default constructor
UT_CMccQosController::UT_CMccQosController()
    {
    }

// Second phase construct
void UT_CMccQosController::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMccQosController::SetupL(  )
    {
    iRtpDataSinkStub = static_cast< CMccRtpDataSink* >( 
        CMccRtpDataSink::NewSinkL( KRtpDataSink, KNullDesC8 ) );

    iRtpDataSourceStub = static_cast< CMccRtpDataSource* >(
        CMccRtpDataSource::NewSourceL( KRtpDataSource, KNullDesC8 ) );
    
    iObserver = CMMccRateAdaptationObserverStub::NewL();
   
    iObserver->iResourcePool->iTempSources.Append(iRtpDataSourceStub);
    iObserver->iResourcePool->iTempSinks.Append(iRtpDataSinkStub);
    
    /*
    iObserver->iResourcePool->ReserveSinkL( iRtpDataSinkStub, 1 );
    iObserver->iResourcePool->ReserveSourceL(iRtpDataSourceStub, 2);
    TMccResourceParams param(0,1,1,0,EFalse);

    EUNIT_ASSERT_NO_LEAVE(iObserver->iResourcePool->ReserveResourcesL(param));
    */
    iQosController = CMccQosController::NewL(*iObserver, *iObserver->iResourcePool );
    }

void UT_CMccQosController::Teardown(  )
    {
    delete iQosController;    
    delete iObserver;
	delete iRtpDataSourceStub;
	delete iRtpDataSinkStub;

    }

void UT_CMccQosController::UT_CMccQosController_NewLL(  )
    {
    delete iQosController;
    iQosController = NULL;
    iQosController = CMccQosController::NewL(*iObserver, *iObserver->iResourcePool );
   
    }

void UT_CMccQosController::UT_CMccQosController_EventReceivedL(  )
    {
    // if ( aEvent.iEventCategory == KMccEventCategoryRtcp )
    //     	if ( eventdata.iRtcpPacketType == KRtcpRrPacket )
    TMccEvent event = TMccEvent();
    event.iEventCategory = KMccEventCategoryRtcp;
    event.iEventType = KMccRtcpReceived;
    TMccRtcpEventData eventratedata = TMccRtcpEventData();
    TRtpPeerStat stats = TRtpPeerStat();
    stats.iRoundTripDelay = 10;
    eventratedata.iStats = stats;
    event.iEventData.Copy ( TMccRtcpEventDataPackage ( eventratedata ) );
    
    eventratedata.iRtcpPacketType = KRtcpRrPacket;
    EUNIT_ASSERT_NO_LEAVE(iQosController->EventReceived( event ));
      
    // if ( aEvent.iEventCategory == KMccEventCategoryRtcp )
    //     	if !( eventdata.iRtcpPacketType == KRtcpRrPacket )
    eventratedata.iRtcpPacketType = KRtcpAnyPacket;
    EUNIT_ASSERT_NO_LEAVE(iQosController->EventReceived( event ));
    
    // !if ( aEvent.iEventCategory == KMccEventCategoryRtcp )
    //     	if ( eventdata.iRtcpPacketType == KRtcpRrPacket )
    event.iEventCategory = KMccEventCategoryStream;
    eventratedata.iRtcpPacketType = KRtcpRrPacket;
    EUNIT_ASSERT_NO_LEAVE(iQosController->EventReceived( event ));

    // !if ( aEvent.iEventCategory == KMccEventCategoryRtcp )
    //     	!if ( eventdata.iRtcpPacketType == KRtcpRrPacket )
    event.iEventCategory = KMccEventCategoryRtcp;
    eventratedata.iRtcpPacketType = KRtcpAnyPacket;
    EUNIT_ASSERT_NO_LEAVE(iQosController->EventReceived( event ));
    }

void UT_CMccQosController::UT_CMccQosController_ConstructLL(  )
    {
    delete iQosController;
    iQosController = NULL;
    iQosController = CMccQosController::NewL(*iObserver, *iObserver->iResourcePool );
  
    }

void UT_CMccQosController::UT_CMccQosController_FillRateAdaptationEventLL(  )
    {
    iQosController->iRateEventData.iRateAdaptationAdvice = 0.9;
    iQosController->iRateEventData.iFramerateOriginal = 0.8;
    iQosController->iRateEventData.iFramerateModified = 0.7;
    iQosController->iRateEventData.iBitrateOriginal = 100;
    iQosController->iRateEventData.iBitrateModified = 200;
    	 
    iQosController->FillRateAdaptationEvent();
    }
    
void UT_CMccQosController::UT_CMccQosController_CheckRateChangeResultL()
    {
  
    TMccRateAdaptationEventData eventdata;
    eventdata.iBitrateModified = 128000;
    eventdata.iBitrateOriginal = 64000;
    TMccRateAdaptationEventDataPackage eventdatapackage(eventdata);
    iQosController->iResultEvent.iEventData.Copy( eventdatapackage );
    EUNIT_ASSERT_NO_LEAVE(iQosController->CheckRateChangeResult(KErrNone));
    EUNIT_ASSERT(iQosController->iBitRateChanged);
    
    eventdata.iBitrateModified = 128000;
    eventdata.iBitrateOriginal = 128000;
    eventdatapackage = eventdata;
    iQosController->iResultEvent.iEventData.Copy( eventdatapackage );
    iQosController->iBitRateChanged = EFalse;
    EUNIT_ASSERT_NO_LEAVE(iQosController->CheckRateChangeResult(KErrNone));
    EUNIT_ASSERT(!iQosController->iBitRateChanged);
        
    EUNIT_ASSERT_NO_LEAVE(iQosController->CheckRateChangeResult(KErrGeneral));
    EUNIT_ASSERT(!iQosController->iBitRateChanged);

    }

void UT_CMccQosController::UT_CMCCQosController_GetSinkSourceL()
    {
    iQosController->iRateEventData.iRateAdaptationAdvice = 0.9;
    iQosController->iRateEventData.iFramerateOriginal = 0.8;
    iQosController->iRateEventData.iFramerateModified = 0.7;
    iQosController->iRateEventData.iBitrateOriginal = 100;
    iQosController->iRateEventData.iBitrateModified = 200;
    
    EUNIT_ASSERT_NO_LEAVE( iQosController->GetSinkSources() );
    }



void UT_CMccQosController::UT_CMccQosController_ClearRateEventDataL(  )
    {
 	iQosController->ClearRateEventData();
 	}

void UT_CMccQosController::UT_CMccQosController_StartTimerForAsyncL(  )
    {
    iQosController->StartTimerForAsync();
    }

void UT_CMccQosController::UT_CMccQosController_AsyncTimerExpiredL(  )
    {
 //   EUNIT_ASSERT( iQosController->AsyncTimerExpired(reinterpret_cast<TAny*>(iQosController)) );
    }

void UT_CMccQosController::UT_CMccQosController_DoAdaptationCalculationsLL(  )
    {
    //  if ( iStreamStat.iFractionLost > iThresholdValues.iFractionLostHigh )
    iQosController->iStreamStat.iFractionLost = 60;
    iQosController->iStreamStat.iRoundTripDelay = 7000;
    iQosController->DoAdaptationCalculations();
    EUNIT_ASSERT( iQosController->iRateEventData.iRateAdaptationAdvice == 1.0 - KAdaptation );   
    
    // Non handled event
    iQosController->iStreamStat.iFractionLost = 0;
    iQosController->iStreamStat.iRoundTripDelay = 6500;
    TInt previousRoundTripTime = iQosController->iRoundTripTime;
    iQosController->DoAdaptationCalculations();
    EUNIT_ASSERT( iQosController->iRateEventData.iRateAdaptationAdvice == 1.0 );   
    EUNIT_ASSERT( iQosController->iRoundTripTime != previousRoundTripTime );  
    
    // Short term check, change not needed
    TInt stepsToShortTerm = KMccShortTermCalculation - iQosController->iEventCounter - 1;
    for ( TInt i = 0; i < stepsToShortTerm - 1; i++ )
        {
        iQosController->DoAdaptationCalculations();
        }
    iQosController->DoAdaptationCalculations();
    EUNIT_ASSERT( iQosController->iRateEventData.iRateAdaptationAdvice == 1.0 );   
    
    // Short term check, increase needed
    iQosController->iStreamStat.iRoundTripDelay = 1000;
    stepsToShortTerm = 2 * KMccShortTermCalculation - iQosController->iEventCounter;
    for ( TInt i = 0; i < stepsToShortTerm - 1; i++ )
        {
        iQosController->DoAdaptationCalculations();
        }
    iQosController->DoAdaptationCalculations();
    EUNIT_ASSERT( iQosController->iRateEventData.iRateAdaptationAdvice == 1.0 + KAdaptation );   
    
    // Short term check, decrease needed
    iQosController->iStreamStat.iRoundTripDelay = 6000;
    stepsToShortTerm = 3 * KMccShortTermCalculation - iQosController->iEventCounter;
    for ( TInt i = 0; i < stepsToShortTerm - 1; i++ )
        {
        iQosController->DoAdaptationCalculations();
        }
    iQosController->DoAdaptationCalculations();
    EUNIT_ASSERT( iQosController->iRateEventData.iRateAdaptationAdvice == 1.0 - KAdaptation );   
    
    // Long term check, change not needed
    iQosController->iStreamStat.iRoundTripDelay = 2500;
    TInt stepsTolongTerm = 2 * KMccLongTermCalculation - iQosController->iEventCounter;
    for ( TInt i = 0; i < stepsTolongTerm - 1; i++ )
        {
        iQosController->DoAdaptationCalculations();
        }
    iQosController->iAdapted = ETrue;
    iQosController->DoAdaptationCalculations();
    EUNIT_ASSERT( iQosController->iRateEventData.iRateAdaptationAdvice == 1.0 );   
    
    // Long term check, increasing
    stepsTolongTerm = 3 * KMccLongTermCalculation - iQosController->iEventCounter;
    for ( TInt i = 0; i < stepsTolongTerm - 1; i++ )
        {
        iQosController->DoAdaptationCalculations();
        }
    iQosController->iAdapted = EFalse;
    iQosController->DoAdaptationCalculations();
    EUNIT_ASSERT( iQosController->iRateEventData.iRateAdaptationAdvice == 1.0 + KAdaptationMore );  

    // Short term check after long term change, increase was bad, need to decrease back
    iQosController->iStreamStat.iRoundTripDelay = 7000;
    stepsToShortTerm = 3 * KMccLongTermCalculation + KMccShortTermCalculation - iQosController->iEventCounter;
    for ( TInt i = 0; i < stepsToShortTerm - 1; i++ )
        {
        iQosController->DoAdaptationCalculations();
        }    
    iQosController->DoAdaptationCalculations();
    EUNIT_ASSERT( iQosController->iRateEventData.iRateAdaptationAdvice == 1.0 - KAdaptation );   
    
    }

void UT_CMccQosController::UT_CMccQosController_IcmpErrorHandlingL()
    {
   
    TMccEvent event;
    event.iLinkId = 1;
    event.iEventCategory = KMccEventCategoryRtp;
    event.iEventType = KMccStreamError;
    event.iErrorCode = KErrHostUnreach;
    EUNIT_ASSERT_EQUALS( iQosController->EventReceived( event ), ETrue );
    
#ifndef EUNIT_ALLOC_TEST_ON

    // Not useful to test when alloc failure is on as icmp error handling
    // is just silently disabled if not enough memory
     
    EUNIT_ASSERT_EQUALS( iQosController->iIcmpErrors.Count(), 1 );
    EUNIT_ASSERT_EQUALS( iQosController->iIcmpErrors[ 0 ].iErrorCount, 1 );
    
    TMccEvent event2;
    event2.iLinkId = 2;
    event2.iEventCategory = KMccEventCategoryRtp;
    event2.iEventType = KMccStreamError;
    event2.iErrorCode = KErrHostUnreach;
    
    EUNIT_ASSERT_EQUALS( iQosController->EventReceived( event2 ), ETrue );
    EUNIT_ASSERT_EQUALS( iQosController->iIcmpErrors.Count(), 2 );
    
    // Reach reporting threshold
    for ( TInt i = 0; i < KMccIcmpErrorThreshold - 2; i++ )
        {
        EUNIT_ASSERT_EQUALS( iQosController->EventReceived( event ), ETrue );
        EUNIT_ASSERT_EQUALS( iQosController->iIcmpErrors.Count(), 2 );
        EUNIT_ASSERT_EQUALS( iQosController->iIcmpErrors[ 0 ].iErrorCount, i + 2 );
        EUNIT_ASSERT_EQUALS( iQosController->iIcmpErrors[ 1 ].iErrorCount, 1 );
        }
    EUNIT_ASSERT_EQUALS( iQosController->EventReceived( event ), EFalse );
    EUNIT_ASSERT_EQUALS( iQosController->iIcmpErrors.Count(), 1 );
    EUNIT_ASSERT_EQUALS( iQosController->iIcmpErrors[ 0 ].iErrorCount, 1 );
    
    // Reach timewindow exceeding
    User::After( KMccIcmpErrorTimeWindow );
    EUNIT_ASSERT_EQUALS( iQosController->EventReceived( event2 ), ETrue );
    EUNIT_ASSERT_EQUALS( iQosController->iIcmpErrors.Count(), 1 );
    EUNIT_ASSERT_EQUALS( iQosController->iIcmpErrors[ 0 ].iErrorCount, 1 );
    
#endif
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccQosController,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "UT_CMccQosController",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CMccQosController_NewLL, Teardown)
 
EUNIT_TEST(
    "ConstructL - test ",
    "UT_CMccQosController",
    "ConstructL",
    "FUNCTIONALITY",
    SetupL, UT_CMccQosController_ConstructLL, Teardown)

EUNIT_TEST(
    "FillRateAdaptationEventL - test ",
    "UT_CMccQosController",
    "FillRateAdaptationEventL",
    "FUNCTIONALITY",
    SetupL, UT_CMccQosController_FillRateAdaptationEventLL, Teardown)
   
EUNIT_TEST(
    "ClearRateEventDataL - test ",
    "UT_CMccQosController",
    "ClearRateEventDataL",
    "FUNCTIONALITY",
    SetupL, UT_CMccQosController_ClearRateEventDataL, Teardown)

EUNIT_TEST(
    "StartTimerForAsyncL - test ",
    "UT_CMccQosController",
    "StartTimerForAsyncL",
    "FUNCTIONALITY",
    SetupL, UT_CMccQosController_StartTimerForAsyncL, Teardown)


EUNIT_TEST(
    "AsyncTimerExpiredL - test ",
    "UT_CMccQosController",
    "AsyncTimerExpiredL",
    "FUNCTIONALITY",
    SetupL, UT_CMccQosController_AsyncTimerExpiredL, Teardown)
    
EUNIT_TEST(
    "CheckRateChangeResultL - test ",
    "UT_CMccQosController",
    "CheckRateChangeResultL",
    "FUNCTIONALITY",
    SetupL, UT_CMccQosController_CheckRateChangeResultL, Teardown)
    
EUNIT_TEST(
    "GetSinkSourceL - test ",
    "UT_CMccQosController",
    "GetSinkSourceL",
    "FUNCTIONALITY",
    SetupL, UT_CMCCQosController_GetSinkSourceL, Teardown)
    
    
EUNIT_TEST(
    "EventReceivedL - test ",
    "UT_CMccQosController",
    "EventReceivedL",
    "FUNCTIONALITY",
    SetupL, UT_CMccQosController_EventReceivedL, Teardown)


EUNIT_TEST(
    "DoAdaptationCalculationsL - test ",
    "UT_CMccQosController",
    "DoAdaptationCalculationsL",
    "FUNCTIONALITY",
    SetupL, UT_CMccQosController_DoAdaptationCalculationsLL, Teardown)

EUNIT_TEST(
    "IcmpErrorHandlingL - test ",
    "UT_CMccQosController",
    "IcmpErrorHandlingL",
    "FUNCTIONALITY",
    SetupL, UT_CMccQosController_IcmpErrorHandlingL, Teardown)
    

EUNIT_END_TEST_TABLE

//  END OF FILE
