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
* Description:    Class is used for passing events.
*
*/





#ifndef MMCCEVENTS_H
#define MMCCEVENTS_H

//  INCLUDES
#include <e32std.h>
#include <rtpapi.h>

// CONSTANTS

typedef TInt TMccEventCategory;
typedef TInt TMccEventType;
typedef TInt TMccRtcpPacketType;
typedef TInt TMccDtmfEventType;

const TInt KMccMaxEventDataLength = 1024;
const TInt KMccMaxDtmfStringLength = 100;

const TMccEventCategory KMccEventCategoryStream = 1;
const TMccEventCategory KMccEventCategoryRtcp = 2;
const TMccEventCategory KMccEventCategoryRtp = 3;
const TMccEventCategory KMccEventCategoryDtmf = 4;
const TMccEventCategory KMccEventCategoryTranscode = 5;

const TMccEventType KMccEventNone = 1;
const TMccEventType KMccStreamPrepared = 2;
const TMccEventType KMccStreamStarted = 3;
const TMccEventType KMccStreamPaused = 4;
const TMccEventType KMccStreamResumed = 5;
const TMccEventType KMccStreamStopped = 6;    
const TMccEventType KMccStreamClosed = 7;
const TMccEventType KMccInactivityEvent = 8;
const TMccEventType KMccActivityEvent = 9;
const TMccEventType KMccStreamPlaying = 10;
const TMccEventType KMccStreamBuffering = 11;
const TMccEventType KMccStreamIdle = 12;
const TMccEventType KMccStreamMMFEvent = 13;
const TMccEventType KMccStreamError = 14;
const TMccEventType KMccRtcpControl = 15;
const TMccEventType KMccRtcpReceived = 16;
const TMccEventType KMccDtmfControl = 17;
const TMccEventType KMccDtmfReceived = 18;
const TMccEventType KMccResourceReserved = 19;
const TMccEventType KMccResourceReleased = 20;
const TMccEventType KMccResourceNotAvailable = 21;
const TMccEventType KMccTranscodeInProgress = 22;
const TMccEventType KMccTranscodeCompleted = 23;
const TMccEventType KMccAudioRoutingChanged = 24;
const TMccEventType KMccLinkCreated = 25;
const TMccEventType KMccTranscodeCancelled = 26;
const TMccEventType KMccUnknownMediaReceived = 27;
const TMccEventType KMccMediaQualityStatus = 28;
const TMccEventType KMccMasterKeyStaled = 29;

const TMccRtcpPacketType KRtcpPacketUndefined = 0;
const TMccRtcpPacketType KRtcpSdesPacket = 1;
const TMccRtcpPacketType KRtcpByePacket = 2;
const TMccRtcpPacketType KRtcpAppPacket = 3;
const TMccRtcpPacketType KRtcpSrPacket = 4;
const TMccRtcpPacketType KRtcpRrPacket = 5;
const TMccRtcpPacketType KRtcpAnyPacket = 6;
     
const TMccDtmfEventType KMccDtmfUnknown = 0;
const TMccDtmfEventType KMccDtmfSigStartTone = 1;
const TMccDtmfEventType KMccDtmfSigStopTone = 2;
const TMccDtmfEventType KMccDtmfSigSendString = 3;
const TMccDtmfEventType KMccDtmfSigContinueSending = 4;
const TMccDtmfEventType KMccDtmfSigCancelSending = 5;
const TMccDtmfEventType KMccDtmfSendingComplete = 6;
const TMccDtmfEventType KMccDtmfManualStart = 7;
const TMccDtmfEventType KMccDtmfManualStop = 8;
const TMccDtmfEventType KMccDtmfManualAbort = 9;
const TMccDtmfEventType KMccDtmfSequenceStart = 10;
const TMccDtmfEventType KMccDtmfSequenceStop = 11;
const TMccDtmfEventType KMccDtmfSequenceAbort = 12;
const TMccDtmfEventType KMccDtmfStopInDtmfString = 13;

// MACROS

// DATA TYPES
        
// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION

// Disabling PC-lint warnings 1554 and 1555, for getting rid of them otherwise
// might not be possible
/*lint -e1554 -e1555*/


/**
* MCC Event
*
*  @lib mmccinterface.dll
*/
class TMccEvent 
{
public: // Constructors

    /**
    * Default constructor
    */
    inline TMccEvent() :
        iSessionId( 0 ), iLinkId( 0 ), iStreamId( 0 ), iEndpointId( 0 ),
        iEventCategory( 0 ), iEventType( KMccEventNone ), iErrorCode( KErrNone ),
        iEventNumData( 0 ), iEventData( KNullDesC8 ), 
        iReserved( 0 ), iReserved2( NULL )
        {
        }
    
    /**
    * Parametrized constructor
    */
    inline TMccEvent( TUint32 aSessionId, TUint32 aLinkId,
                      TUint32 aStreamId, TUint32 aEndpointId, TMccEventCategory aCategory, 
                      TMccEventType aEvent, TInt aError, const TDesC8& aData ) : 
        iSessionId( aSessionId ), iLinkId( aLinkId ), iStreamId( aStreamId ),
        iEndpointId( aEndpointId ), iEventCategory( aCategory ), iEventType( aEvent ),
        iErrorCode( aError ), iEventNumData( 0 ), iEventData( aData ),
        iReserved( 0 ), iReserved2( NULL )
        {
        }
        
    /**
    * Copy constructor
    */
    inline TMccEvent( const TMccEvent& aEvent ):
        iSessionId( aEvent.iSessionId ), iLinkId( aEvent.iLinkId ),
        iStreamId( aEvent.iStreamId ), iEndpointId( aEvent.iEndpointId ),
        iEventCategory( aEvent.iEventCategory ), iEventType( aEvent.iEventType ),
        iErrorCode( aEvent.iErrorCode ), iEventNumData(aEvent.iEventNumData),
        iEventData( aEvent.iEventData ), 
        iReserved( aEvent.iReserved ), iReserved2( aEvent.iReserved2 )
        {
        }
        
                
    inline TMccEvent& operator=( 
        const TMccEvent& aEvent )
        {
        if ( &aEvent != this )
            {
            iSessionId = aEvent.iSessionId;
            iLinkId = aEvent.iLinkId;
            iStreamId = aEvent.iStreamId;
            iEndpointId = aEvent.iEndpointId; 
            iEventCategory = aEvent.iEventCategory;
            iEventType = aEvent.iEventType;
            iErrorCode = aEvent.iErrorCode;
       	    iEventNumData = aEvent.iEventNumData;
            iEventData.Copy( aEvent.iEventData );
            iReserved = aEvent.iReserved;
            iReserved2 = aEvent.iReserved2;
            }
        return *this;
        }
        
public: // Data

    // Session identifier
    TUint32 iSessionId;

    // Link identifier
    TUint32 iLinkId;

    // Stream identifier
    TUint32 iStreamId;

    // Endpoint (source/sink) identifier
    TUint32 iEndpointId;
    
    // Event category
    TMccEventCategory iEventCategory;

    // Event type
    TMccEventType iEventType;
    
    // Error code for event
    TInt iErrorCode;
    
    // Numeric data for event
    TUint32 iEventNumData;

    // Event data
    TBuf8<KMccMaxEventDataLength> iEventData;

    // Reserved for future use
    TUint32 iReserved;
    
    // Reserved for future use
    TAny* iReserved2;

};

typedef TPckgBuf<TMccEvent> TMccEventPackage;


/**
* The class TMccRtcpEventData implements the RTCP signalling in the MCC.
*/

const TInt KMccMaxRtcpPacketDataLength = KMccMaxEventDataLength - 
                                         TInt(sizeof( TMccRtcpPacketType )) - 
                                         TInt(sizeof( TUint32 )) - 
                                         TInt(sizeof( TTimeStamps )) - 
                                         TInt(sizeof( TRtpPeerStat )) -
                                         /* TPckgBuf overhead */
                                         TInt(sizeof( TMccEventPackage ))
                                         + TInt(sizeof( TMccEvent ));

/**
* Rtp SourceSink event data
*/
class TMccRtpEventData
    {
    public: 
    
        /**
        * Default constructor
        */
        inline TMccRtpEventData() :
            iJitterEstimate( 0 ), 
            iPacketsReceived( 0 ), 
            iPrevTransTime( 0 ),
            iTriggeredJitterLevel( 0 )
            {   
            }
    
        /**
        * Copy constructor
        */
        inline TMccRtpEventData( const TMccRtpEventData& aEvent ) :
            iJitterEstimate( aEvent.iJitterEstimate ),
            iPacketsReceived( aEvent.iPacketsReceived ), 
            iPrevTransTime( aEvent.iPrevTransTime ),
            iTriggeredJitterLevel( aEvent.iTriggeredJitterLevel )
            {
            }
        
        /**
         * parametrized constructor
         */
        inline TMccRtpEventData( TInt64 aEstimate, 
                                 TUint32 aPcksRec, 
                                 TInt64 aPrevTransTime, 
                                 TUint64 aTriggeredJitterLevel ) :
            iJitterEstimate( aEstimate ),
            iPacketsReceived( aPcksRec ), 
            iPrevTransTime( aPrevTransTime ),
            iTriggeredJitterLevel( aTriggeredJitterLevel )
            {
            }
        
    public: // Data
        
        // Current jitter estimate
        TUint64 iJitterEstimate;
        
        // Number of received packets
        TUint32 iPacketsReceived;
        
        // Previous packet transit time
        TInt64 iPrevTransTime;
        
        // Triggered jitter level
        TUint64 iTriggeredJitterLevel;
    };
    
typedef TPckgBuf<TMccRtpEventData> TMccRtpEventDataPackage;

class TMccRtcpEventData
    {
    public: // Constructors
    
        /**
        * Constructor
        */
        inline TMccRtcpEventData() :
            iRtcpPacketData( KNullDesC8 ),
            iRtcpPacketType( KRtcpPacketUndefined ),
            iSsrc( 0 )
            {
            	iStats.iNumPacketsSent = 0;
            	iStats.iCumNumOctetsSent = 0;
            	iStats.iRoundTripDelay = 0;
            	iStats.iTxBandwidth = 0;
            	iStats.iCumNumPacketsLost = 0;
            	iStats.iFractionLost = 0;
            	iStats.iArrivalJitter = 0;
            	iStats.iRxBandwidth = 0;
            	iStats.iChannelBufferSize = 0;
            	iStats.iNTPTimeStampSec = 0;
            	iStats.iNTPTimeStampFrac = 0;
            	iStats.iTimeStamp = 0;
            	iTimeStamps.iNTPTimeStampSec = 0;
            	iTimeStamps.iNTPTimeStampFrac = 0;
            	iTimeStamps.iTimeStamp = 0;
            }

        /**
        * Copy constructor
        */
        inline TMccRtcpEventData( const TMccRtcpEventData& aEventData ) :
            iRtcpPacketData( aEventData.iRtcpPacketData ),
            iRtcpPacketType( aEventData.iRtcpPacketType ),
            iSsrc( aEventData.iSsrc ),
            iTimeStamps( aEventData.iTimeStamps ),
            iStats( aEventData.iStats )
            {
            }
    
    public: // Data
    
        // RTCP packet data
        TBuf8<KMccMaxRtcpPacketDataLength> iRtcpPacketData;
        
        // Type of the RTCP data
        TMccRtcpPacketType iRtcpPacketType;
        
        // Synchronization source of RTCP packet
        TUint32 iSsrc;   
        
        TTimeStamps iTimeStamps;
        
        TRtpPeerStat iStats;
    };
    
typedef TPckgBuf<TMccRtcpEventData> TMccRtcpEventDataPackage;

/**
* The class TMccDtmfEventData implements the DTMF signalling in the MCC.
*/
class TMccDtmfEventData
    {
    public: // Constructors
        
        /**
        * Constructor
        */
        inline TMccDtmfEventData() : 
            iDtmfEventType( KMccDtmfUnknown ),
            iContinue( EFalse ),
            iDtmfString( NULL ),
            iTimeStamp( 0 ),
            iSeqNum( 0 )
            {
            }    
            
        /**
        * Copy constructor
        */
        inline TMccDtmfEventData( const TMccDtmfEventData& aEventData ) : 
            iDtmfEventType( aEventData.iDtmfEventType ),
            iContinue( aEventData.iContinue ),
            iDtmfString( aEventData.iDtmfString ),
            iTimeStamp( aEventData.iTimeStamp ),
            iSeqNum( aEventData.iSeqNum )
            {
            }      
            
    public: // Data

        // Encapsulates type of the event
        TMccDtmfEventType iDtmfEventType;

        // Indicates is Dtmf sending continued after stop
        TBool iContinue;
        
        // Dtmf string to send
        TBuf8<KMccMaxDtmfStringLength> iDtmfString;
        
        // For internal usage
        TUint32 iTimeStamp;
        
        // For internal usage
        TUint32 iSeqNum;
        
    };
    
typedef TPckgBuf<TMccDtmfEventData> TMccDtmfEventDataPackage;


/**
* Rtp SourceSink event data, extended version
*/
class TMccRtpEventDataExtended : public TMccRtpEventData
    {
    public: 
    
        /**
        * Default constructor
        */
        inline TMccRtpEventDataExtended() :
            TMccRtpEventData(),
  			iPacketLoss( 0 ),
			iTriggeredPacketLoss( 0 )
            {   
            }
    
        /**
        * Copy constructor
        */           
        inline TMccRtpEventDataExtended( const TMccRtpEventDataExtended& aEvent ) :
            TMccRtpEventData( aEvent ),
            iPacketLoss( aEvent.iPacketLoss ),
			iTriggeredPacketLoss( aEvent.iTriggeredPacketLoss )
            {
            }
        
        
        /**
         * parametrized constructor
         */
        inline TMccRtpEventDataExtended( TInt64 aEstimate, 
                                 TUint32 aPcksRec, 
                                 TInt64 aPrevTransTime, 
                                 TUint64 aTriggeredJitterLevel,
                                 TInt aPacketLoss,
								 TInt aTriggeredPacketLoss ) :
            TMccRtpEventData(	aEstimate, 
            					aPcksRec, 
            					aPrevTransTime, 
            					aTriggeredJitterLevel ),
            iPacketLoss( aPacketLoss ),
			iTriggeredPacketLoss( aTriggeredPacketLoss )
            {
            }
        
    public: // Data
        
        // Current packet loss percentage
		TInt iPacketLoss;
		
		// Triggered packet loss percentage
		TInt iTriggeredPacketLoss;

    };
    
typedef TPckgBuf<TMccRtpEventDataExtended> TMccRtpEventDataExtendedPackage; 
      
#endif      // MMCCEVENTS_H   
            
// End of File
