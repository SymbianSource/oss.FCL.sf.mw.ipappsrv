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
* Description:    MccJitterCalculator calculates jitter level and compares given
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
    TBool aMarker )
    {
    #ifdef TRACE_JITCALC
        RDebug::Print( _L("CMccJitterCalculator::RtpPacketReceived IN") );
    #endif
    
    #ifdef VOIP_RECEIVED_RTP_PACKET_INFO_ENABLED
        VoipTrace("%x %x %d %d %d", MCC_TRACE, MCC_RECEIVED_RTP_PACKET_INFO,
        aHeaderInfo.iSeqNum, aHeaderInfo.iTimestamp, iPacketsReceived );
    #endif
    
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
        
        TMccRtpEventData event( ( iCurJitter >> KNum4 ), 
                                iPacketsReceived, 
                                iPrevPacketTransit,
                                KNumZero );
        
        iObserver->SendJitterEvent( event, KErrNone );
        }
    
    JitterObserving();
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
    }

// -----------------------------------------------------------------------------
// CMccJitterCalculator::JitterObserving
// Trigger controller. Compares calculated and defined values.
// private method
// -----------------------------------------------------------------------------
//  
void CMccJitterCalculator::JitterObserving()
    {
    if ( iJitterObsOn )
        {
        #ifdef TRACE_JITCALC
            RDebug::Print( _L("CMccJitterCalculator::JitterObserving Jitter level observing ON") );
            RDebug::Print( _L("CMccJitterCalculator::JitterObserving jitterlevel %u ms compared to jitter estimate %Lu ms"), 
                iJitterLevelFromClient, ( iCurJitter / KConversionFactor ) );
        #endif
        
        // Normalize iCurJitter
        TUint64 modJitter = iCurJitter >> KNum4;
            
        if ( EMccQualityTimeBased == iReportIntervalType )
            {
            // change microsecs to millisecs
            TTimeIntervalMicroSeconds getTime = 
                iCurTime.MicroSecondsFrom( iHomeTime );
            
            #ifdef TRACE_JITCALC
                RDebug::Print( _L("CMccJitterCalculator::JitterObserving getTime = %Ld"), getTime.Int64() );
            #endif
            
            if ( static_cast<TInt64>( iReportInterval ) <= 
               ( getTime.Int64() / KConversionFactor ) )
                {
                // compare clients jitter level to current level
                if ( static_cast<TUint64>( iJitterLevelFromClient ) < 
                    modJitter )
                    {
                    #ifdef TRACE_JITCALC
                        RDebug::Print ( _L("CMccJitterCalculator::JitterObserving jitterlevel %u ms compared to jitter estimate %Lu ms"), 
                            iJitterLevelFromClient, modJitter );
                    #endif
                    
                    TMccRtpEventData event( ( iCurJitter >> KNum4 ), 
                                            iPacketsReceived, 
                                            iPrevPacketTransit,
                                            modJitter );
                    
                    // Informs client via event and cancels jitter observing
                    iObserver->SendJitterEvent( event, KErrNone );
                    CancelObserving();
                    }
                
                // Initialize hometime again
                iHomeTime.HomeTime();
                
                #ifdef TRACE_JITCALC       
                    RDebug::Print ( _L("CMccJitterCalculator::JitterObserving Initialize iHomeTime = %Ld"), iHomeTime.Int64() );
                #endif
            
                }           
            }
        else if ( EMccQualityPacketBased == iReportIntervalType )
            {
            iReceivedPacketCounter++;
            
            #ifdef TRACE_JITCALC
                RDebug::Print( _L("CMccJitterCalculator::JitterObserving iReceivedPacketCounter: %u"), iReceivedPacketCounter );
            #endif
            
            if ( iReportInterval == iReceivedPacketCounter )
                {
                // compare clients jitter level to current level
                if ( iJitterLevelFromClient < modJitter )
                    {
                    #ifdef TRACE_JITCALC
                        RDebug::Print( _L("CMccJitterCalculator::JitterObserving jitterlevel %u ms compared to jitter estimate %Lu ms"), 
                            iJitterLevelFromClient, modJitter );
                    #endif
                    
                    TMccRtpEventData event( ( iCurJitter >> KNum4 ), 
                                            iPacketsReceived, 
                                            iPrevPacketTransit,
                                            modJitter );
                    
                    // Informs client via event and cancels jitter observing
                    iObserver->SendJitterEvent( event, KErrNone );
                    CancelObserving();
                    }
                  
                iReceivedPacketCounter = KNumZero;
                }
            }
         else
            {
            #ifdef TRACE_JITCALC
                RDebug::Print( _L("CMccJitterCalculator::JitterObserving Report type is not valid!") );
            #endif
            
            TMccRtpEventData event( ( iCurJitter >> KNum4 ), 
                                      iPacketsReceived, 
                                      iPrevPacketTransit,
                                      KNumZero );
            
            iObserver->SendJitterEvent( event, KErrArgument );
            CancelObserving();
            }
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
    
//  End of File  
