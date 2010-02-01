/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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



// INCLUDE FILES
#include "mccqoscontroller.h"
#include "mccrateadaptationobserver.h"
#include "mmccevents.h"
#include "mccresources.h"
#include "mccdatasource.h"
#include "mccdatasink.h"
#include "QosControllerLog.h"
#include "mccinternalevents.h"

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS
const TInt KPluginMicroSecInSec = 1;
// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccQosController::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMccQosController* CMccQosController::NewL( 
    MMccRateAdaptationObserver& aObserver,
    MMccResources& aResources )
    {
    __TRACE_MCC_QOS_CONTROLLER1("CMccQosController::NewL");
    CMccQosController* self = 
        new ( ELeave ) CMccQosController( aObserver, aResources );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMccQosController::~CMccQosController
// -----------------------------------------------------------------------------
//        
EXPORT_C CMccQosController::~CMccQosController()
    {
    __TRACE_MCC_QOS_CONTROLLER1("CMccQosController::~CMccQosController");
    if ( iDeltaTimer )
        {
        iDeltaTimer->Remove( iDeltaTimerEntry );
        }
    delete iDeltaTimer;
    
    iIcmpErrors.Reset();
    iIcmpErrors.Close();
    
    __TRACE_MCC_QOS_CONTROLLER1("CMccQosController::~CMccQosController end");
    }

// -----------------------------------------------------------------------------
// CMccQosController::EventReceived
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CMccQosController::EventReceived( const TMccEvent& aEvent )
    {
    __TRACE_MCC_QOS_CONTROLLER1("CMccQosController::EventReceived");
    
    // Check if interesting event and then do calculations.
    // If adaptation is needed, make async brake and continue with it once
    // dummy async completes. QosController has also responsibility of
    // icmp error handling (random errors are filtered etc.).
    
    TBool ignoreEvent( EFalse );
    
    if ( aEvent.iEventCategory == KMccEventCategoryRtcp )
    	{
		__TRACE_MCC_QOS_CONTROLLER3(
		    "CMccQosController::EventReceived TMccEvent:iSessionId=", 
		    aEvent.iSessionId);
		__TRACE_MCC_QOS_CONTROLLER3(
		    "CMccQosController::EventReceived TMccEvent:iLinkId=", 
		    aEvent.iLinkId);
		__TRACE_MCC_QOS_CONTROLLER3(
		    "CMccQosController::EventReceived TMccEvent:iStreamId=", 
		    aEvent.iStreamId);
		__TRACE_MCC_QOS_CONTROLLER3(
		    "CMccQosController::EventReceived TMccEvent:iEndpointId=", 
		    aEvent.iEndpointId);
		__TRACE_MCC_QOS_CONTROLLER3(
		    "CMccQosController::EventReceived TMccEvent:iErrorCode=", 
		    aEvent.iErrorCode);

		const TMccRtcpEventData& eventdata = 
    		(*reinterpret_cast<const TMccRtcpEventDataPackage*>( 
                &aEvent.iEventData ))(); 
                      
    	if ( eventdata.iRtcpPacketType == KRtcpRrPacket )
            {
            // RR received
    
            iStreamStat = eventdata.iStats;
			__TRACE_MCC_QOS_CONTROLLER3(
			    "CMccQosController::EventReceived TRtpPeerStat:iNumPacketsSent=", 
			    iStreamStat.iNumPacketsSent);
			__TRACE_MCC_QOS_CONTROLLER3(
			    "CMccQosController::EventReceived TRtpPeerStat:iCumNumOctetsSent=", 
			    iStreamStat.iCumNumOctetsSent);
			__TRACE_MCC_QOS_CONTROLLER3(
			    "CMccQosController::EventReceived TRtpPeerStat:RoundTripDelay=", 
			    iStreamStat.iRoundTripDelay);
  			__TRACE_MCC_QOS_CONTROLLER3(
  			    "CMccQosController::EventReceived TRtpPeerStat:iTxBandwidth=", 
  			    iStreamStat.iTxBandwidth);
 			__TRACE_MCC_QOS_CONTROLLER3(
 			    "CMccQosController::EventReceived TRtpPeerStat:iCumNumPacketsLost=", 
 			    iStreamStat.iCumNumPacketsLost);
 			__TRACE_MCC_QOS_CONTROLLER3(
 			    "CMccQosController::EventReceived TRtpPeerStat:iFractionLost=", 
 			    iStreamStat.iFractionLost);
 			__TRACE_MCC_QOS_CONTROLLER3(
 			    "CMccQosController::EventReceived TRtpPeerStat:iArrivalJitter=", 
 			    iStreamStat.iArrivalJitter);
  			__TRACE_MCC_QOS_CONTROLLER3(
  			    "CMccQosController::EventReceived TRtpPeerStat:iRxBandwidth=", 
  			    iStreamStat.iRxBandwidth);
   			__TRACE_MCC_QOS_CONTROLLER3(
   			    "CMccQosController::EventReceived TRtpPeerStat:iChannelBufferSize=", 
   			    iStreamStat.iChannelBufferSize);
  			__TRACE_MCC_QOS_CONTROLLER3(
  			    "CMccQosController::EventReceived TRtpPeerStat:iNTPTimeStampSec=", 
  			    iStreamStat.iNTPTimeStampSec);   			
  			__TRACE_MCC_QOS_CONTROLLER3(
  			    "CMccQosController::EventReceived TRtpPeerStat:iNTPTimeStampFrac=", 
  			    iStreamStat.iNTPTimeStampFrac);
  			
            DoAdaptationCalculations();
            }
        }
    else if ( aEvent.iEventCategory == KMccEventCategoryRtp && 
              aEvent.iEventType == KMccStreamError &&
              aEvent.iErrorCode == KErrHostUnreach )
        {
        ignoreEvent = HandleIcmpError( aEvent );
        }
    else
        {
        // NOP
        }
    
    __TRACE_MCC_QOS_CONTROLLER1("CMccQosController::EventReceived end");   
     
    return ignoreEvent;
    }
    
//-----------------------------------------------------------------------------
// CMccQosController::DoAdaptationCalculations  
// -----------------------------------------------------------------------------
//  
void CMccQosController::DoAdaptationCalculations()
	{
	__TRACE_MCC_QOS_CONTROLLER1("CMccQosController::DoAdaptationCalculations");

	#ifdef RATECONTROL
	
	ClearRateEventData();
	
	__TRACE_MCC_QOS_CONTROLLER3("CMccQosController::RoundTripTime", 
	                            iRoundTripTime );
	__TRACE_MCC_QOS_CONTROLLER3("CMccQosController::RoundTripDelay", 
	                            iStreamStat.iRoundTripDelay );
		
	if ( iRoundTripTime )
		{
		// Temporary change will not affect immediately so much (kind of mean value)
		const TReal q = 0.9;
		iRoundTripTime = 
		    TInt ( q * iRoundTripTime + (1.0 - q) * iStreamStat.iRoundTripDelay );
		__TRACE_MCC_QOS_CONTROLLER3("CMccQosController::New RoundTripTime=", 
		                            iRoundTripTime );
		}
	else
		{
		iRoundTripTime = iStreamStat.iRoundTripDelay; 
		}
	
	// Store some amount of previous round trip delay values in order to
	// calculate average value of certain period	
    iRoundTripDelays[ iEventCounter % KMccShortTermCalculation ] = iStreamStat.iRoundTripDelay;
    
    iEventCounter++;
    
    TBool adaptedAtThisRound( EFalse );
	
	if ( iStreamStat.iFractionLost > iThresholdValues.iFractionLostHigh )
	    {
	    // Immediately drop bitrate if significant number of packets got lost
	    __TRACE_MCC_QOS_CONTROLLER1(
	        "CMccQosController::DoAdaptationCalculations, fractions lost, decrease");
	    iRateEventData.iRateAdaptationAdvice = ( 1.0 - KAdaptation );
	    DoAdaptation();
	    adaptedAtThisRound = ETrue;
	    }	 
    
    if ( !( iEventCounter % KMccShortTermCalculation ) )
        {
        // If long term calculation resulted in change, we are interested 
        // how it affected to average round trip delay
        __TRACE_MCC_QOS_CONTROLLER1(
            "CMccQosController::DoAdaptationCalculations, short term");
            
        TInt roundTripDelayOfThisPeriod = CalculateDelayOfShortTermPeriod();
        
        __TRACE_MCC_QOS_CONTROLLER3(
            "CMccQosController::DoAdaptationCalculations, avg delay of this period", 
            roundTripDelayOfThisPeriod );
        
        if ( !adaptedAtThisRound )
            {
            TInt comparisonDelay = GetShortTermComparisonDelay();
            
            __TRACE_MCC_QOS_CONTROLLER3(
                "CMccQosController::DoAdaptationCalculations, comparison delay", 
                comparisonDelay );
            
            if ( roundTripDelayOfThisPeriod - comparisonDelay > KRoundTripTimeLimit )
                {
                __TRACE_MCC_QOS_CONTROLLER1(
                    "CMccQosController::DoAdaptationCalculations, decrease");
                iRateEventData.iRateAdaptationAdvice = 1.0 - KAdaptation;
                DoAdaptation();
                adaptedAtThisRound = ETrue;
                }
            else if ( comparisonDelay - roundTripDelayOfThisPeriod > KRoundTripTimeLimit )
                {
                __TRACE_MCC_QOS_CONTROLLER1(
                    "CMccQosController::DoAdaptationCalculations, increase");
                iRateEventData.iRateAdaptationAdvice = 1.0 + KAdaptation;
                DoAdaptation();
                adaptedAtThisRound = ETrue;
                }
            else
                {
                }
            }
            
        iRoundTripDelayOfPreviousShortPeriod = roundTripDelayOfThisPeriod;
        }
	
	if ( !( iEventCounter % KMccLongTermCalculation ) )
	    {
	    __TRACE_MCC_QOS_CONTROLLER1(
	        "CMccQosController::DoAdaptationCalculations, long term");
	    
	    if ( !adaptedAtThisRound && !iAdapted )
	        {
	        // If adaptation wasn't needed within previous long term period, try to
	        // increase bitrate. Remember round trip time of this period so that
	        // we can see what kind of impact the increasing of bitrate has. 
	        __TRACE_MCC_QOS_CONTROLLER1(
	            "CMccQosController::DoAdaptationCalculations, try to increase");
	        iRateEventData.iRateAdaptationAdvice = ( 1.0 + KAdaptationMore );
	        DoAdaptation();
	        adaptedAtThisRound = ETrue;
	        }
	        
	    iAdapted = EFalse;
	    }
			
	#endif //RATECONTROL	
	__TRACE_MCC_QOS_CONTROLLER1("CMccQosController::DoAdaptationCalculations end");
    }
    
// -----------------------------------------------------------------------------
// CMccQosController::FillRateAdaptationEventL
// -----------------------------------------------------------------------------
//      
void CMccQosController::FillRateAdaptationEvent()
	{
	__TRACE_MCC_QOS_CONTROLLER1("CMccQosController::FillRateAdaptationEvent");
	iEvent = TMccEvent();
	TMccRateAdaptationEventDataPackage temp(iRateEventData);
	iEvent.iEventData.Copy( temp );	 
	__TRACE_MCC_QOS_CONTROLLER1("CMccQosController::FillRateAdaptationEvent end");
	}
	
// -----------------------------------------------------------------------------
// CMccQosController::ClearRateEventData
// -----------------------------------------------------------------------------
//      
void CMccQosController::ClearRateEventData()
	{
	__TRACE_MCC_QOS_CONTROLLER1("CMccQosController::ClearRateEventData");	
	iRateEventData = TMccRateAdaptationEventData();
	__TRACE_MCC_QOS_CONTROLLER1("CMccQosController::ClearRateEventData end");	
	}
	
// -----------------------------------------------------------------------------
// CMccQosController::StartTimerForAsync
// -----------------------------------------------------------------------------
//
void CMccQosController::StartTimerForAsync ()
	{
	__TRACE_MCC_QOS_CONTROLLER1("CMccQosController::StartTimerForAsync");
	iDeltaTimer->Remove(iDeltaTimerEntry);
	TTimeIntervalMicroSeconds32 interval(KPluginMicroSecInSec);
	iDeltaTimer->Queue(interval, iDeltaTimerEntry);
	__TRACE_MCC_QOS_CONTROLLER1("CMccQosController::StartTimerForAsync end");	
	}	
 
// -----------------------------------------------------------------------------
// CMccQosController::AsyncTimerExpired
// -----------------------------------------------------------------------------
//
TInt CMccQosController::AsyncTimerExpired(TAny* aPtr)
	{
	__TRACE_MCC_QOS_CONTROLLER1("CMccQosController::AsyncTimerExpired");

	CMccQosController* self = reinterpret_cast<CMccQosController*>(aPtr);
	self ->GetSinkSources();
	
	__TRACE_MCC_QOS_CONTROLLER1("CMccQosController::AsyncTimerExpired end");
  	return ETrue;
	} 
    
// -----------------------------------------------------------------------------
// CMccQosController::GetSinkSources
// -----------------------------------------------------------------------------
//
void CMccQosController::GetSinkSources()
	{
	__TRACE_MCC_QOS_CONTROLLER1("CMccQosController::GetSinkSources");
    iBitRateChanged = EFalse;
    const RPointerArray<MDataSource>& tempSources = iResources.Sources( ETrue );
  	FillRateAdaptationEvent();
  	TInt result = 0;
  	for ( TInt i = 0; i < tempSources.Count(); i++ )
        {
        iResultEvent = TMccEvent();
		__TRACE_MCC_QOS_CONTROLLER3("CMccQosController::GetSinkSources Sources", i);

        CMccDataSource* item = static_cast<CMccDataSource*> (tempSources[ i ]); 
        iResultEvent = TMccEvent(); 	
        result = item->RateAdaptationRequest( iEvent, iResultEvent );
        CheckRateChangeResult( result );
        }
  	const RPointerArray<MDataSink>& tempSinks = iResources.Sinks( ETrue );
  	for ( TInt j = 0; j < tempSinks.Count(); j++ )
        {
        iResultEvent = TMccEvent();
		__TRACE_MCC_QOS_CONTROLLER3("CMccQosController::GetSinkSources Sinks", j);        
        CMccDataSink* item = static_cast<CMccDataSink*> (tempSinks[ j ]);
        result = item->RateAdaptationRequest( iEvent, iResultEvent );
        CheckRateChangeResult( result );
        }
    if ( !iBitRateChanged )
    	{
    	iMainObserver.RateAdaptationAlert( iEvent,
    				 MMccRateAdaptationObserver::ERateAdaptationNotPossible );
    	}
	__TRACE_MCC_QOS_CONTROLLER1("CMccQosController::GetSinkSources END"); 
	}    
    
// -----------------------------------------------------------------------------
// CMccQosController::CheckRateChangeResult
// -----------------------------------------------------------------------------
// 
void CMccQosController::CheckRateChangeResult( TInt aValue )
    {
	__TRACE_MCC_QOS_CONTROLLER1("CMccQosController::CheckRateChangeResult");    
    if ( aValue == KErrNone )
    	{
		TMccRateAdaptationEventDataPackage resultdatapkg;
		resultdatapkg.Copy(iResultEvent.iEventData);
		TMccRateAdaptationEventData resultdata = resultdatapkg();
        if ( resultdata.iBitrateOriginal != resultdata.iBitrateModified )
        	{
        	__TRACE_MCC_QOS_CONTROLLER1("CMccQosController::BitRateChanged ETRUE");
        	iBitRateChanged = ETrue;
        	}
    	}
	__TRACE_MCC_QOS_CONTROLLER1("CMccQosController::CheckRateChangeResult end");
    }

// -----------------------------------------------------------------------------
// CMccQosController::HandleIcmpError
// -----------------------------------------------------------------------------
// 
TBool CMccQosController::HandleIcmpError( const TMccEvent& aEvent )
    {
    __TRACE_MCC_QOS_CONTROLLER1("CMccQosController::HandleIcmpError");   
    
    // By default error is ignored. Only if there has been certain amount
    // of errors within certain time window, error is reported.
    //
    TBool ignoreError( ETrue );
    
    // Do first cleanup of all entries which have exceeded their monitoring
    // timewindow.
    TTime currentTime;
    currentTime.HomeTime();
    TMccQosControllerIcmpError cleanupIcmpError;
    cleanupIcmpError.iErrorTimeWindowBeginning = currentTime;
    TIdentityRelation<TMccQosControllerIcmpError> 
        cleanupComparison( IcmpErrorCleanup );
    TInt index = iIcmpErrors.Find( cleanupIcmpError, cleanupComparison );
    while ( index != KErrNotFound )
        {
        iIcmpErrors.Remove( index );
        index = iIcmpErrors.Find( cleanupIcmpError, cleanupComparison );
        }
    
    __TRACE_MCC_QOS_CONTROLLER1("CMccQosController::HandleIcmpError, cleanup done");  
    
    TIdentityRelation<TMccQosControllerIcmpError> comparison( IcmpErrorMatch );
    TMccQosControllerIcmpError icmpError;
    icmpError.iLinkId = aEvent.iLinkId;
    index = iIcmpErrors.Find( icmpError, comparison );
    if ( index != KErrNotFound )
        {
        TMccQosControllerIcmpError& existingIcmpError = iIcmpErrors[ index ];
        existingIcmpError.iErrorCount++;
        if ( existingIcmpError.iErrorCount >= KMccIcmpErrorThreshold )
            {
            // Report the error
            ignoreError = EFalse;
            iIcmpErrors.Remove( index );
            }
        }
    else
        {
        icmpError.iErrorCount = KMccFirstIcmpError;
        icmpError.iErrorTimeWindowBeginning = currentTime;
        
        // It does not matter if entry cannot be added because of low memory
        iIcmpErrors.Append( icmpError );
        }
    
    __TRACE_MCC_QOS_CONTROLLER3("CMccQosController::HandleIcmpError ignore:", 
                                ignoreError );  
       
    return ignoreError;
    }

// -----------------------------------------------------------------------------
// CMccQosController::IcmpErrorMatch
// -----------------------------------------------------------------------------
//     
TBool CMccQosController::IcmpErrorMatch( 
    const TMccQosControllerIcmpError& aError1, 
    const TMccQosControllerIcmpError& aError2 )
    {
    // First argument is the search term
    return ( aError1.iLinkId == aError2.iLinkId );
    }

// -----------------------------------------------------------------------------
// CMccQosController::IcmpErrorCleanup
// -----------------------------------------------------------------------------
//     
TBool CMccQosController::IcmpErrorCleanup( 
    const TMccQosControllerIcmpError& aError1, 
    const TMccQosControllerIcmpError& aError2 )
    {
    // First argument is the search term
    TInt64 timeWindow = 
        aError1.iErrorTimeWindowBeginning.MicroSecondsFrom( 
            aError2.iErrorTimeWindowBeginning ).Int64();
    return ( timeWindow > KMccIcmpErrorTimeWindow );
    }
       
// -----------------------------------------------------------------------------
// CMccQosController::CMccQosController
// -----------------------------------------------------------------------------
//      
CMccQosController::CMccQosController(
    MMccRateAdaptationObserver& aObserver,
    MMccResources& aResources ) :
    iMainObserver( aObserver ),
    iResources( aResources ),
    iDeltaTimerCallBack(AsyncTimerExpired, this),
    iBitRateChanged(EFalse)
    {
    __TRACE_MCC_QOS_CONTROLLER1("CMccQosController::CMccQosController");
    iDeltaTimerEntry.Set(iDeltaTimerCallBack);
    }

// -----------------------------------------------------------------------------
// CMccQosController::ConstructL
// -----------------------------------------------------------------------------
// 
void CMccQosController::ConstructL()
    {
	__TRACE_MCC_QOS_CONTROLLER1("CMccQosController::ConstructL");    
    iDeltaTimer = CDeltaTimer::NewL(CActive::EPriorityStandard);
    iEventCounter = 0;
    #ifdef RATECONTROL
    //rate control threshold values

    iThresholdValues.iFractionLostHigh = 30;
    iThresholdValues.iFractionLostLow = 0;

	iRoundTripTime = 0;
	
	iAdapted = EFalse;

	#endif //RATECONTROL
    }

// -----------------------------------------------------------------------------
// CMccQosController::DoAdaptation
// -----------------------------------------------------------------------------
// 
void CMccQosController::DoAdaptation()
    {
    StartTimerForAsync();
    iAdapted = ETrue;
    }

// -----------------------------------------------------------------------------
// CMccQosController::GetShortTermComparisonDelay
// -----------------------------------------------------------------------------
// 
TInt CMccQosController::GetShortTermComparisonDelay()
    {    
    if ( iRoundTripDelayOfPreviousShortPeriod )
        {
        return iRoundTripDelayOfPreviousShortPeriod;
        }
        
    return iRoundTripTime;
    }

// -----------------------------------------------------------------------------
// CMccQosController::CalculateDelayOfShortTermPeriod
// -----------------------------------------------------------------------------
//
TInt CMccQosController::CalculateDelayOfShortTermPeriod()
    {
    TInt roundTripDelayOfThisPeriod( 0 );
    for ( TInt i = 0; i < KMccShortTermCalculation; i++ )
        {
        roundTripDelayOfThisPeriod += iRoundTripDelays[ i ];
        }
    return ( roundTripDelayOfThisPeriod / KMccShortTermCalculation );
    }
    
// End of file

