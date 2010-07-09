/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  MccJitterCalculator calculates jitter level and compares given
*                triggers.
*
*/




// INCLUDE FILES
#include "mccinternaldef.h"
#include "mccrtpdatasource.h"
#include "mccjittercalculator.h"
#include "mccinternalevents.h"

#ifdef VOIP_TRACE_ENABLED
#include <voip_trace.h>
#endif

#ifdef _DEBUG
#define TRACE_JITCALC
#include <e32svr.h>
#endif

// CONSTANTS

// Packet interval for jitter/jitterbuffer stat reports
const TUint KJitterReportInterval = 16;
// Conversion factor from uSecs to mSecs
const TInt KConversionFactor = 1000;
// Samplerate in kHz, currently only 8kHz supported
const TUint KSampleRate = 8;

const TInt KNum4( 4 );
const TInt KNum8( 8 );
const TInt KNumZero( 0 );
const TInt KNumHundred( 100 );

const TUint32 KRtpFractationAccuracy = 5000;
// Specified period of the moving average
const TUint32 KDivider = 250;
// Specified the max calculate count of package loss
const TUint32 KMaxCalCount = 200;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccJitterCalculator::CMccJitterCalculator
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMccJitterCalculator::CMccJitterCalculator( MRtpJitterObserver& aObserver ) : 
    iObserver( &aObserver )
    {
    }

// -----------------------------------------------------------------------------
// CMccJitterCalculator::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
#ifdef FTD_ENABLED
void CMccJitterCalculator::ConstructL()
    {
    TInt err = iStreamStatsQueue.OpenGlobal( KMccStreamStats, EOwnerProcess );
    if ( KErrNone != err )
        {
        User::Leave( err );
        }
    
    err = iJBufStatsQueue.OpenGlobal( KMccJBufferStats, EOwnerProcess );
    if ( KErrNone != err )
        {
        iStreamStatsQueue.Close();
        User::Leave( err );
        }
    }
#endif

// -----------------------------------------------------------------------------
// CMccJitterCalculator::NewL
// Static constructor.
// -----------------------------------------------------------------------------
//
CMccJitterCalculator* CMccJitterCalculator::NewL( MRtpJitterObserver& aObserver )
    {
    CMccJitterCalculator* self = 
            new ( ELeave ) CMccJitterCalculator( aObserver );
    
    #ifdef FTD_ENABLED
    CleanupStack::PushL ( self );
    self->ConstructL ();
    CleanupStack::Pop( self );
    #endif
    
    return self;
    }


// -----------------------------------------------------------------------------
// CMccJitterCalculator::~CMccJitterCalculator
// Destructor.
// -----------------------------------------------------------------------------
//
CMccJitterCalculator::~CMccJitterCalculator()
    {
    #ifdef TRACE_JITCALC
        RDebug::Print( _L("CMccJitterCalculator::~CMccJitterCalculator IN") );
    #endif
    
    if ( IsObserving() )
    	{
        CancelObserving();               
        }

    iObserver = NULL;
    
    #ifdef FTD_ENABLED
    iStreamStatsQueue.Close();
    iJBufStatsQueue.Close();
    #endif
    
    #ifdef TRACE_JITCALC
        RDebug::Print( _L("CMccJitterCalculator::~CMccJitterCalculator OUT") );
    #endif
    }

// -----------------------------------------------------------------------------
// CMccJitterCalculator::RtpPacketReceived
// Calculates jitter values
// -----------------------------------------------------------------------------
//
void CMccJitterCalculator::RtpPacketReceived( const TUint32 aTimeStamp, 
    TBool aMarker, TUint16 aSeqNum )
    {
    #ifdef TRACE_JITCALC
        RDebug::Print( _L("CMccJitterCalculator::RtpPacketReceived IN") );
    #endif
    
    #ifdef VOIP_RECEIVED_RTP_PACKET_INFO_ENABLED
        VoipTrace("%x %x %d %d %d", MCC_TRACE, MCC_RECEIVED_RTP_PACKET_INFO,
        aHeaderInfo.iSeqNum, aHeaderInfo.iTimestamp, iPacketsReceived );
    #endif
          
    TBool calculatePkgLoss = ETrue;  
    if ( EMccQualityPacketBased == iReportIntervalType || iFrameLossObsOn )
        {
        // Save current packet sequence number
	    // The SeqNum come from RTP package. It maybe start from a random number between
        // 0 to 65535. iStartingSeqNum should be reset 
        // while iCurrentSeqNum was reset (by remote packages)
    	iCurrentSeqNum = aSeqNum;
		if( iStartingSeqNum == KNumZero ||
			iStartingSeqNum > iCurrentSeqNum )
			{
			iStartingSeqNum = aSeqNum;
			if ( iStartingSeqNum > 0 )
				{
			    iStartingSeqNum--;
				}
			else
				{
                #ifdef TRACE_JITCALC
                RDebug::Print( _L("Received SeqNum is 0, don't calculate lost package") );
                #endif
			    calculatePkgLoss = EFalse;
				}
			}
        iReceivedPacketCounter++; 
		}
    
    // Jitter calculating based on RFC3550 Appendix A.8, see www.ietf.org
    if ( aMarker || !iPacketsReceived )
        {
        // A new talkspurt, redo calculations Marker bit is set after silent
        // period and indicates that new speech data is received
        iPrevPacketArrival.HomeTime();
        TInt64 pcktTrans = iPrevPacketArrival.Int64() / TInt64( KConversionFactor );
        
        // Convert timestamp to milliseconds and calculate transit time   
        iPrevPacketTransit = 
                pcktTrans - static_cast<TInt64>( ( aTimeStamp / KSampleRate ) );
        
        // Reset jitter and packet counter for new talkspurt
        iCurJitter = KNumZero;
        iPacketsReceived = KNumZero;
        }
    else
        {
        // Just another packet in the talkspurt
        TTime newPacket;
        newPacket.HomeTime();
        
        // Time of Arrival in milliseconds
        TInt64 curPacketTOA = ( newPacket.Int64() / TInt64( KConversionFactor ) );
        
        // Convert timestamp to milliseconds and calculate transit time
        TInt64 curPacketTransit 
            = curPacketTOA - static_cast<TInt64>( ( aTimeStamp / KSampleRate ) );
        
        TInt64 diff = curPacketTransit - iPrevPacketTransit;
        iPrevPacketTransit = curPacketTransit;
        diff = Abs( diff );

        // Jitter is floating mean value over 16 packets last received
        iCurJitter +=  diff - ( ( iCurJitter + KNum8 ) >> KNum4 );
        
        #ifdef TRACE_JITCALC
            RDebug::Print( _L("CMccJitterCalculator::RtpPacketReceived CUR_TOA: %Ld"), curPacketTOA );
            RDebug::Print( _L("CMccJitterCalculator::RtpPacketReceived CUR_TRANS: %Ld"), curPacketTransit );
            RDebug::Print( _L("CMccJitterCalculator::RtpPacketReceived CUR_DIFF: %Ld"), diff );
            RDebug::Print( _L("CMccJitterCalculator::RtpPacketReceived CUR_JITT: %Lu"), iCurJitter );
            RDebug::Print( _L("CMccJitterCalculator::RtpPacketReceived SC_CUR_JITT: %Lu"), ( iCurJitter >> KNum4 ) );
        #endif
        }
    
    iPacketsReceived++;
    iCurTime.HomeTime();
    
    #ifdef TRACE_JITCALC
        RDebug::Print ( _L("CMccJitterCalculator::RtpPacketReceived newPacket = %Ld"), iCurTime.Int64() );
    #endif
    
    #ifdef FTD_ENABLED
    TMccStreamStats stats;
    stats.iPacketsReceived = iPacketsReceived;
    stats.SetFieldUpdatedFlag( EPacketsReceived );
    iStreamStatsQueue.Send( stats );
    
    TMccJBufferStats jStats;
    jStats.iJitterEstimate = iCurJitter;
    iJBufStatsQueue.Send( jStats );
    #endif 
    
    // Report jitter if we have enough received packets and jitter
    if ( !( iPacketsReceived % KJitterReportInterval ) && iCurJitter )
        {
        #ifdef TRACE_JITCALC
            RDebug::Print( _L("CMccJitterCalculator::RtpPacketReceived SENDING RCV: %d"), iPacketsReceived );
        #endif
        
        TMccRtpEventDataExtended event( ( iCurJitter >> KNum4 ), 
                                iPacketsReceived, 
                                iPrevPacketTransit,
                                KNumZero,
                                KNumZero,
                                KNumZero );
        
        iObserver->SendJitterEvent( event, KErrNone );
        }
    
    if ( calculatePkgLoss )	
    	{
        JitterObserving();
    	}    
    }

// ---------------------------------------------------------------------------
// CMccJitterCalculator::StartObserving
// Switch to deside report type
// ---------------------------------------------------------------------------
//  
TInt CMccJitterCalculator::StartObserving()
    {
    #ifdef TRACE_JITCALC
        RDebug::Print( _L("CMccJitterCalculator::StartObserving IN") );
    #endif
    
    TInt err( KErrNone );
    iHomeTime.HomeTime();
    
    #ifdef TRACE_JITCALC
        RDebug::Print ( _L("CMccJitterCalculator::StartObserving HomeTime = %Ld"), iHomeTime.Int64()  );
        RDebug::Print( _L("CMccJitterCalculator::StartObserving iReportType: %d"), iReportType );
    #endif

    switch( iReportType )
        {
        case EMccJitterReport:
            #ifdef TRACE_JITCALC
                RDebug::Print( _L("CMccJitterCalculator::StartObserving  iReportType selected EMccJitterReport"));
            #endif
            iJitterObsOn = ETrue;                
            iMediaQualityObservingStarted = ETrue;                
            break;
            
        case EMccPacketLossReport:
            #ifdef TRACE_JITCALC
                RDebug::Print( _L("CMccJitterCalculator::StartObserving  iReportType selected EMccPacketLossReport"));
            #endif
            iFrameLossObsOn = ETrue;
            iMediaQualityObservingStarted = ETrue;
            break;
            
        case EMccQualityReportAll:
            #ifdef TRACE_JITCALC
                RDebug::Print( _L("CMccJitterCalculator::StartObserving  iReportType selected EMccQualityReportAll"));
            #endif
            iJitterObsOn = ETrue;
            iFrameLossObsOn = ETrue;
            iMediaQualityObservingStarted = ETrue;
            break;

        default:
            // if report type is not set
            #ifdef TRACE_JITCALC
                RDebug::Print( _L("CMccJitterCalculator::StartObserving  iReportType selected default"));
            #endif
            
            err = KErrArgument;
            break;
        }
    
    return err;
    }

// ---------------------------------------------------------------------------
// CMccJitterCalculator::CancelObserving
// Cancel is called by client
// ---------------------------------------------------------------------------
//  
void CMccJitterCalculator::CancelObserving()
    {
    #ifdef TRACE_JITCALC
        RDebug::Print( _L("CMccJitterCalculator::CancelObserving IN") );
    #endif
    
    iJitterObsOn = EFalse;
    iFrameLossObsOn = EFalse;
    iMediaQualityObservingStarted = EFalse;
    iStartingSeqNum = KNumZero;
    }

// ---------------------------------------------------------------------------
// CMccJitterCalculator::SetMediaConfigsL
// Open aMessage and read media configs
// ---------------------------------------------------------------------------
//     
void CMccJitterCalculator::SetMediaConfigsL( const TMMFMessage& aMessage )
    {
    #ifdef TRACE_JITCALC
        RDebug::Print( _L("CMccJitterCalculator::SetMediaConfigs IN") );
    #endif
    
    TMMFMessage* tmp = new ( ELeave ) TMMFMessage( aMessage );
    CleanupStack::PushL( tmp );
    
    TMccMediaQualConfBuf configPckg;
    tmp->ReadData1FromClientL( configPckg );

    iMediaConf = configPckg();
        
    // Read data from client
    iJitterLevelFromClient = iMediaConf.iJitterLevel;
    iPacketLossFromClient = iMediaConf.iPacketLoss;
    iReportType = iMediaConf.iReportType;
    iReportIntervalType = iMediaConf.iReportIntervalType;
    iReportInterval = iMediaConf.iReportInterval;
        
    #ifdef TRACE_JITCALC
        RDebug::Print( _L("CMccJitterCalculator::SetMediaConfigsL SESSION %d"), iMediaConf.iSessionId );
        RDebug::Print( _L("CMccJitterCalculator::SetMediaConfigsL JITTER LEVEL %d"), iMediaConf.iJitterLevel );
        RDebug::Print( _L("CMccJitterCalculator::SetMediaConfigsL PACKET LOSS %d"), iMediaConf.iPacketLoss ); 
        RDebug::Print( _L("CMccJitterCalculator::SetMediaConfigsL REPORT TYPE %d"), iMediaConf.iReportType );
        RDebug::Print( _L("CMccJitterCalculator::SetMediaConfigsL REPORT INTERVAL TYPE %d"), iMediaConf.iReportIntervalType );
        RDebug::Print( _L("CMccJitterCalculator::SetMediaConfigsL REPORT INTERVAL %d"), iMediaConf.iReportInterval );
    #endif
        
    CleanupStack::PopAndDestroy( tmp );
    }

// -----------------------------------------------------------------------------
// CMccJitterCalculator::ResetCounters
// Set counters ready to receive packets
// -----------------------------------------------------------------------------
//
void CMccJitterCalculator::ResetCounters()
    {
    #ifdef TRACE_JITCALC
        RDebug::Print( _L("CMccJitterCalculator::ResetCounters()") );
    #endif
        
    iPacketsReceived = KNumZero;
    iCurJitter = KNumZero;
    iPrevPacketTransit = KNumZero;
    iCurrentSeqNum = KNumZero;
	iStartingSeqNum = KNumZero;
	iReceivedPacketCounter = KNumZero;
    }

// -----------------------------------------------------------------------------
// CMccJitterCalculator::JitterObserving
// Trigger controller. Compares calculated and defined values.
// private method
// -----------------------------------------------------------------------------
//  
void CMccJitterCalculator::JitterObserving()
    {
    #ifdef TRACE_JITCALC
        RDebug::Print( _L("CMccJitterCalculator::JitterObserving IN") );
    #endif
    
    TBool resetValuesForNextReport = EFalse;
    TBool cancelObs = EFalse;
    
    if ( iJitterObsOn )
        {
        #ifdef TRACE_JITCALC
            RDebug::Print( _L("CMccJitterCalculator::JitterObserving Jitter level observing ON") );
            RDebug::Print( _L("CMccJitterCalculator::JitterObserving jitterlevel %u ms compared to jitter estimate %Lu ms"), 
                iJitterLevelFromClient, ( iCurJitter / KConversionFactor ) );
        #endif
        
        CheckJitter( resetValuesForNextReport, cancelObs );
        }
        	
	if ( iFrameLossObsOn )
        {
        #ifdef TRACE_JITCALC
            RDebug::Print( _L("CMccJitterCalculator::JitterObserving Frame Loss observing ON") );
        #endif
        
        CheckPacketLoss( resetValuesForNextReport, cancelObs );
        }
 
    if( resetValuesForNextReport ) 
		{
        // Initialize hometime again
        iHomeTime.HomeTime();
        #ifdef TRACE_JITCALC       
            RDebug::Print ( _L("CMccJitterCalculator::JitterObserving Initialize iHomeTime = %Ld"), iHomeTime.Int64() );
        #endif
		}
	if( cancelObs )
		{
		CancelObserving();	
		}
    }
    
// ---------------------------------------------------------------------------
// CMccJitterCalculator::IsObserving
// To check if observing is ON
// ---------------------------------------------------------------------------
//      
TBool CMccJitterCalculator::IsObserving() const
    {
    return iMediaQualityObservingStarted;
    }
    
// -----------------------------------------------------------------------------
// CMccJitterCalculator::CountPacketLossPercentage
// Counts packet loss percentage from iStartingSeqNum, iCurrentSeqNum
// 		and iReceivedPacketCounter
// private method
// -----------------------------------------------------------------------------
//  
TInt CMccJitterCalculator::CountPacketLossPercentage()
	{
    TUint32 numberOfPacketsExpected( 0 );
    numberOfPacketsExpected = iCurrentSeqNum - iStartingSeqNum;
        
    if ( ( iPrevExpectedPackets - iPrevPacketsReceived ) ==
            ( numberOfPacketsExpected - iReceivedPacketCounter ) )
        {
        // no packet  lost - inorder
        iPrevFerValue = CalculateFer( iPrevFerValue, EFalse, ETrue );
        }
    else if ( ( iPrevPacketsReceived < iReceivedPacketCounter ) &&
                ( iPrevExpectedPackets == numberOfPacketsExpected ) )
        {
        // no packet  lost - late packet - remove FER
        iPrevFerValue = CalculateFer( iPrevFerValue, ETrue, EFalse );                               
        }
    else if ( numberOfPacketsExpected > iPrevExpectedPackets )
        {

        // packet  lost - add FER by number of packets lost.
        // expected = 5, prev_expected 3 => diff 2 but loss is ONE thats why - 1
        TUint32 maxCalcount = 
				 numberOfPacketsExpected - iPrevExpectedPackets - 1  < KMaxCalCount ?
				 numberOfPacketsExpected - iPrevExpectedPackets - 1 : KMaxCalCount;
        for ( TUint32 i = 0; i < maxCalcount; i++ )
           {
           iPrevFerValue = CalculateFer( iPrevFerValue, ETrue, ETrue );
           }
        }
    
    iPrevExpectedPackets = numberOfPacketsExpected; 
    iPrevPacketsReceived = iReceivedPacketCounter;
    
    TInt percentage( iPrevFerValue / KRtpFractationAccuracy ); 
    return percentage;
	}    
    
// -----------------------------------------------------------------------------
// CMccJitterCalculator::CheckJitter
// Checks if current jitter level exceeds the jitter level given by the client,
// 		and if exceeded, sends the report to client 
// private method
// -----------------------------------------------------------------------------
//      
void CMccJitterCalculator::CheckJitter( TBool& aReportIntervalReached, 
										TBool& aReportSent )
	{
	#ifdef TRACE_JITCALC
        RDebug::Print( _L("CMccJitterCalculator::CheckJitter IN") );
    #endif
    
	// Normalize iCurJitter
    TUint64 modJitter = iCurJitter >> KNum4;
        
    if ( EMccQualityTimeBased == iReportIntervalType )
        {
        // change microsecs to millisecs
        TTimeIntervalMicroSeconds getTime = 
            iCurTime.MicroSecondsFrom( iHomeTime );
        
        #ifdef TRACE_JITCALC
            RDebug::Print( _L("CMccJitterCalculator::CheckJitter getTime = %Ld"), getTime.Int64() );
        #endif
        
        if ( static_cast<TInt64>( iReportInterval ) <= 
           ( getTime.Int64() / KConversionFactor ) )
            {
            // compare clients jitter level to current level
            if ( static_cast<TUint64>( iJitterLevelFromClient ) < 
                modJitter )
                {
                #ifdef TRACE_JITCALC
                    RDebug::Print ( _L("CMccJitterCalculator::CheckJitter jitterlevel %u ms compared to jitter estimate %Lu ms"), 
                        iJitterLevelFromClient, modJitter );
                #endif
                
                TMccRtpEventDataExtended event( ( iCurJitter >> KNum4 ), 
                                        	iPacketsReceived, 
                                        	iPrevPacketTransit,
                                        	modJitter,
                                        	KNumZero,
                                        	KNumZero );
                
                // Informs client via event and cancels jitter observing
                iObserver->SendJitterEvent( event, KErrNone );
                aReportSent = ETrue; 
                }
            aReportIntervalReached = ETrue;
            }           
        }
    else if ( EMccQualityPacketBased == iReportIntervalType )
        {
        #ifdef TRACE_JITCALC
            RDebug::Print( _L("CMccJitterCalculator::CheckJitter iReceivedPacketCounter: %u"), iReceivedPacketCounter );
        #endif
        
        if ( iReportInterval == iReceivedPacketCounter )
            {
            // compare clients jitter level to current level
            if ( iJitterLevelFromClient < modJitter )
                {
                #ifdef TRACE_JITCALC
                    RDebug::Print( _L("CMccJitterCalculator::CheckJitter jitterlevel %u ms compared to jitter estimate %Lu ms"), 
                        iJitterLevelFromClient, modJitter );
                #endif
                
                TMccRtpEventDataExtended event( ( iCurJitter >> KNum4 ), 
                                        	iPacketsReceived, 
                                        	iPrevPacketTransit,
                                        	modJitter,
                                        	KNumZero,
                                        	KNumZero );
                
                // Informs client via event and cancels jitter observing
                iObserver->SendJitterEvent( event, KErrNone );
                aReportSent = ETrue; 
                }  
            aReportIntervalReached = ETrue;
            }
        }
     else
        {
        #ifdef TRACE_JITCALC
            RDebug::Print( _L("CMccJitterCalculator::CheckJitter Report type is not valid!") );
        #endif
        
        TMccRtpEventDataExtended event( ( iCurJitter >> KNum4 ), 
                                  	iPacketsReceived, 
                                  	iPrevPacketTransit,
                                  	KNumZero,
                                  	KNumZero,
                                  	KNumZero );
        
        iObserver->SendJitterEvent( event, KErrArgument );
        aReportSent = ETrue; 
        }
	
	}
    
// -----------------------------------------------------------------------------
// CMccJitterCalculator::CheckPacketLoss
// Checks if current packet loss level exceeds the packet loss level 
// 		given by the client, and if exceeded, sends the report to client 
// private method
// -----------------------------------------------------------------------------
//      
void CMccJitterCalculator::CheckPacketLoss( TBool& aReportIntervalReached, 
											TBool& aReportSent )
	{
	#ifdef TRACE_JITCALC
        RDebug::Print( _L("CMccJitterCalculator::CheckPacketLoss IN") );
    #endif

    TInt packetLossPercentage = CountPacketLossPercentage();
  
    if ( EMccQualityTimeBased == iReportIntervalType )
	    {
	    // change microsecs to millisecs
	    TTimeIntervalMicroSeconds getTime = 
	        iCurTime.MicroSecondsFrom( iHomeTime );
	    
	    #ifdef TRACE_JITCALC
	        RDebug::Print( _L("CMccJitterCalculator::CheckPacketLoss getTime = %Ld"), getTime.Int64() );
	    #endif
	    
	    if ( static_cast<TInt64>( iReportInterval ) <= 
	       ( getTime.Int64() / KConversionFactor ) )
	        {
			// If the packet loss percentage was exceeded
			if( iPacketLossFromClient < packetLossPercentage ) 
	            {
	            #ifdef TRACE_JITCALC
	                RDebug::Print( _L("CMccJitterCalculator::CheckPacketLoss Packet Loss From Client %u compared to Current Packet Loss %u "), 
	                    iPacketLossFromClient, packetLossPercentage );
	            #endif
	            
	            TMccRtpEventDataExtended event( ( iCurJitter >> KNum4 ), 
	                                    iPacketsReceived, 
	                                    iPrevPacketTransit,
	                                    KNumZero,
										packetLossPercentage,
										packetLossPercentage );
	            
	            // Informs client via event and cancels jitter observing
	            iObserver->SendJitterEvent( event, KErrNone );
	            aReportSent = ETrue; 
	   			}
	        aReportIntervalReached = ETrue; 
	        }
	    }      
	else if ( EMccQualityPacketBased == iReportIntervalType )
	    {
	    
	    #ifdef TRACE_JITCALC
		RDebug::Print( _L("CMccJitterCalculator::CheckPacketLoss EMccQualityPacketBased == iReportIntervalType") );
	    RDebug::Print( _L("CMccJitterCalculator::CheckPacketLoss iReceivedPacketCounter: %u"), iReceivedPacketCounter );
	    #endif
	    
	    if ( iReportInterval == iReceivedPacketCounter )  
	        {
			// If the packet loss percentage was exceeded
			if( iPacketLossFromClient < packetLossPercentage ) 
	            {
	            #ifdef TRACE_JITCALC
	                RDebug::Print( _L("CMccJitterCalculator::CheckPacketLoss Packet Loss From Client %u compared to Current Packet Loss %u "), 
	                    iPacketLossFromClient, packetLossPercentage );
	            #endif
	            
	            TMccRtpEventDataExtended event( ( iCurJitter >> KNum4 ), 
	                                    iPacketsReceived, 
	                                    iPrevPacketTransit,
	                                    KNumZero,
										packetLossPercentage,
										packetLossPercentage );
	            
	            // Informs client via event and cancels jitter observing
	            iObserver->SendJitterEvent( event, KErrNone );
	            aReportSent = ETrue; 
	   			}   
	        aReportIntervalReached = ETrue; 
	        }
	    }
	 else
	    {
	    #ifdef TRACE_JITCALC
	        RDebug::Print( _L("CMccJitterCalculator::CheckPacketLoss Report type is not valid!") );
	    #endif
	    
	    TMccRtpEventDataExtended event( ( iCurJitter >> KNum4 ), 
	                              iPacketsReceived, 
	                              iPrevPacketTransit,
	                              KNumZero,
	                              KNumZero,
								  KNumZero );
	    
	    iObserver->SendJitterEvent( event, KErrArgument );
	    aReportSent = ETrue; 
	    }
	}   

// -----------------------------------------------------------------------------
// CMccJitterCalculator::CalculateFer
// Calculates a new FER value
// -----------------------------------------------------------------------------
//      
TUint32 CMccJitterCalculator::CalculateFer( TUint32 aPrevValue,
    TBool aAdd, TBool aFlag )
    {
    TUint32 ret = 0;
    TInt64 temp_add = 0;

    temp_add = aAdd * KNumHundred * KRtpFractationAccuracy;

    TInt64 diff = temp_add - (TInt64)aPrevValue;
    TInt32 tempValue = static_cast<TInt32>( diff / KDivider );
    
    if ( aFlag )
        {
        ret =  TUint32( aPrevValue + tempValue );
        }
    else
        {
        if ( aPrevValue > tempValue )
            {
            ret = TUint32( aPrevValue - tempValue );
            }
        else
            {
            // removed FER would have gone over thus zero
            ret = 0;
            }
        }
    
    return ret;
    }

//  End of File  
