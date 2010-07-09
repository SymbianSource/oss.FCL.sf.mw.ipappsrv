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




#ifndef MCCJITTERCALCULATOR_H
#define MCCJITTERCALCULATOR_H

//  INCLUDES
#include <mmf/common/mmfcontrollerframework.h>
#include <mmccinterfacedef.h>

#ifdef FTD_ENABLED
#include <e32msgqueue.h>
#include <mccstreamstats.h>
#include <mccjbufferstats.h>
#endif


// CLASS DECLARATION

/**
*  Observer for jitter calculation event notification
*
*  @lib      MccRtpSourceSink.dll
*  @since    Series 60 3.2
*/
class MRtpJitterObserver
    {
    public:
          virtual void SendJitterEvent( TMccRtpEventDataExtended aEvent, TInt aError ) = 0;
    };


/**
* Class which implements jittercalculations for received RTP packets
* 
* @lib Mccrtpsourcesink.dll
* @since Series 60 3.0
*/
class CMccJitterCalculator : public CBase
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CMccJitterCalculator* NewL( MRtpJitterObserver& aObserver );
        
        /**
        * Destructor.
        */
        virtual ~CMccJitterCalculator( );

    public: // New functions
    
        /**
        * Starts media quality observing
        * @since    Series 60 3.0
        * @return   Error code
        */
        TInt StartObserving();
        
        /**
        * Cancels media quality observing
        * @since    Series 60 3.0
        * @return   void
        */
        void CancelObserving();
        
        /**
        * Handles received packets and calculates jitter level
        * @since    Series 60 3.0
        * @param    "aHeaderInfo"
        *           RTP header from received packet
        * @return   void
        */
        void RtpPacketReceived( const TUint32 aTimeStamp, TBool aMarker, TUint16 aSeqNum );
        
        /**
        * Set media configurations
        * @since    Series 60 3.0
        * @param    "aMessage"
        *           message from client
        * @return   void
        */
        void SetMediaConfigsL( const TMMFMessage& aMessage );
        
        /**
        * Reset local counters
        * @since    Series 60 3.0
        * @return   void
        */
        void ResetCounters( );
        
        /**
        * Check the observing status
        * @since    Series 60 3.0
        * @return   iMediaQualityObservingStarted
        */
        TBool IsObserving() const;
        
    private:
        /**
        * C++ default constructor.
        */
        CMccJitterCalculator( MRtpJitterObserver& aObserver );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
        
        /**
        * Jitter level observing routines
        * @since    Series 60 3.0
        * @return   void
        */
        void JitterObserving();
        
        /**
        * Counts packet loss percentage from iStartingSeqNum, iCurrentSeqNum
        * 		and iReceivedPacketCounter
        * @return   Counted packet loss percentage
        */
        TInt CountPacketLossPercentage();
        
        /**
        * Checks if current jitter level exceeds the jitter level given by the client,
        * 		and if exceeded, sends the report to client 
        * @param    "aReportIntervalReached"
        *           Changed to ETrue if the report interval was reached
        * @param    "aReportSent"
        *           Changed to ETrue if the jitter report was sent
        * @return   void
        */
        void CheckJitter( TBool& aReportIntervalReached, TBool& aReportSent );
        
        /**
        * Checks if current packet loss level exceeds the packet loss level 
        * 		given by the client, and if exceeded, sends the report to client 
        * @param    "aReportIntervalReached"
        *           Changed to ETrue if the report interval was reached
        * @param    "aReportSent"
        *           Changed to ETrue if the packet loss report was sent
        * @return   void
        */
        void CheckPacketLoss( TBool& aReportIntervalReached, TBool& aReportSent );

        /**
        * Calculates a new FER value 
        * @param    aPrevValue Previous FER value
        * @param    aAdd Value to add( ETrue/EFalse)
        * @param    aFlag To add (ETrue) or remove (EFalse)
        * @return   new FER value
        */    
        TUint32 CalculateFer( TUint32 aPrevValue, TBool aAdd, TBool aFlag );

    private:   
        
        // Number of packets received
        TUint32 iPacketsReceived;

        // Current jitter estimate
        TUint64 iCurJitter;

        // Holds the previous packet arrival time
        TTime iPrevPacketArrival;
        
        // Holds the previous packets transit time
        TInt64 iPrevPacketTransit;
                
        // Indicates reference jitter level from client
        TUint iJitterLevelFromClient;
        
        // Indicates current report type
        TInt iReportType;
        
        // Indicates report interval type
        TInt iReportIntervalType;
        
        // Indicates report interval
        TInt iReportInterval;
        
        // Flags for indicating jitter level observing state and frame loss rate
        TBool iJitterObsOn;
        TBool iFrameLossObsOn;

        // Indicates observing state. Refer to IsObserving-method
        TBool iMediaQualityObservingStarted;
        
        // Indicates interval time
        TTime iIntervalTime;
        
        // Indicates packet loss rate from client
        TInt iPacketLossFromClient;
        
        // Indicates amount of received packets
        TUint32 iReceivedPacketCounter;
        
        // Media config buffer is filled with config data
        TMccMediaQualityConfig iMediaConf;
        
        // Indicates home time
        TTime iHomeTime;
        
        // Indicates current time
        TTime iCurTime;
        
        /**
         * jitter observer
         * Own.
         */
        MRtpJitterObserver* iObserver;
        
        // Current packet sequence number
 		TUint16 iCurrentSeqNum;
 		
 		// Packet sequence number where to start counting the packet loss
 		TUint16 iStartingSeqNum;

        // Holds the previous number of packets expected
        TUint32 iPrevExpectedPackets;
        
        // Holds the previous number of packets received
        TUint32 iPrevPacketsReceived;
        
        // Holds the previous FER value
        TUint32 iPrevFerValue;

        #ifdef FTD_ENABLED

        // Message queue carrying stream statistics
        RMsgQueue<TMccStreamStats> iStreamStatsQueue;
        
        // Message queue carrying jitter buffer statistics
        RMsgQueue<TMccJBufferStats> iJBufStatsQueue;

        #endif
        
        #ifdef TEST_EUNIT
        public:
            friend class UT_CMccRtpDataSource;
            friend class UT_CMccJitterCalculator;
        #endif
    };

#endif      // MCCJITTERCALCULATOR_H   
            
// End of File
