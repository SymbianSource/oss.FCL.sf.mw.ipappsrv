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




#ifndef MCCVIDEOSINKUSER_H
#define MCCVIDEOSINKUSER_H

// INCLUDES
#include <e32base.h>
#include "rtpdef.h"
#include "mccinternalcodecs.h"
#include "mccvideojitterbuffer.h"

// N times average timestamp difference in timestamps between subsequent packets
// is allowed
const TInt KMccTimeStampDifferenceTreshold = 20;

// Number of subsequent timestamps stored
const TInt KMccTimeStampArraySize = 4;

// Minimum size for Helix PacketSink queue
const TInt KMccMinXPSQueueSize = 1000;

// Limits for sequence number based timestamp difference threshold
const TInt KMccSeqNumDifferenceThresholdMin = 10;
const TInt KMccSeqNumDifferenceThresholdMax = 100;

// Mininum payload buffer size
const TInt KMccVideoSinkMinPayloadBufSize = 4096;

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class CXPSPacketSink;

// CLASS DECLARATION

class TMccTimeStampEntry
    {
public:

    inline TMccTimeStampEntry();
    
    inline TMccTimeStampEntry( TInt64 aTime, TUint16 aSeq );
        
    TInt64 iTime;
    TUint16 iSeq;
    };

/**
* Stores details of single videosink user. Is used also to detect
* incorrect RTP timestamp usage. If other end is detected
* to be misbehaving, timestamps are tried to be corrected as otherwise
* video player is not able to playback the stream. 
*
*  @lib 
*  @since 
*/
class CMccVideoSinkUser : public CBase, public MMccVideoJitterBufferObserver
    {
    public: // Enumerations
    
        enum TMccPacketOverflowState
            {
            ENormal,
            EOccured,
            ERecovered
            };
            
        enum TMccTimeStampModifyMode
            {
            EModifyInit,
            EModifyRequiredMonitoring,
            EModifyRequired,
            EModifyNotRequiredMonitoring,
            EModifyNotRequired
            };
    
    public: // Functions

        /**
        * Constructor
        */
        static CMccVideoSinkUser* NewL( MAsyncEventHandler* aAsyncEventHandler, 
                                        TUid aMediaType,
                                        TUint aStreamId,
                                        TReal aFrameRate,
                                        CXPSPacketSink& aPacketSink );
                                         
        /**
        * Constructor
        */
        static CMccVideoSinkUser* NewLC( MAsyncEventHandler* aAsyncEventHandler, 
                                         TUid aMediaType,
                                         TUint aStreamId,
                                         TReal aFrameRate,
                                         CXPSPacketSink& aPacketSink );
        
        ~CMccVideoSinkUser();
        
        /**
        * Enqueue packet
        * @param aHeader
        * @param aPayloadData
        */
        CMccVideoJitterBuffer::TMccPacketBufferingStatus 
            EnqueueL( const TRtpRecvHeader& aHeaderInfo, 
                      const TDesC8& aPayloadData );
    
        /**
        * Set identifier values. E.g. can be used if this class is used
        * as dummy search parameter.
        * @param aAsyncEventHandler
        * @param aMediaType
        * @param aStreamId
        */
        void Set( MAsyncEventHandler* aAsyncEventHandler, 
                  TUid aMediaType,
                  TUint aStreamId );
        
        /**
        * Set framerate for the user
        * @param aFrameRate
        */
        void SetFrameRateL( TReal aFrameRate );
        
        /**
        * Get preroll value for the user
        * @return preroll value
        */
        TInt GetPreroll();
        
        /**
        * Get actual preroll value for the user
        * @return preroll value
        */
        TInt GetActualPreroll();
        
        /**
        * User play
        */
        void Play();
        
        /**
        * User pause
        */
        void Pause();
                           
        TBool IsQueueSizeDefined() const;
            
        TUint QueueSize() const;
            
        TMccPacketOverflowState PacketOverflowState() const;

        void SetPacketOverflow( TMccPacketOverflowState aState );
            
        void AddTimeStamp( TInt64 aTimeStamp, TUint16 aSeq ); 
        
        void Reset( TBool aFullReset );
        
        void CalculateAverageTimeStampDifference();
        
        TBool IsResetNeeded( TRtpRecvHeader& aHeaderInfo );
        
        TBool IsResetNeeded() const;
        
        TBool IsModifyNeeded( TRtpRecvHeader& aHeaderInfo );
    
        void SetAllowFrame( TBool aAllowFrame );
            
        TBool AllowFrame() const;
        
        /**
        * Let user know whether there's other users
        * @param aMultipleMediaTypes
        */
        void SetMultipleMediaTypesL( TBool aMultipleMediaTypes );
        
        /**
        * Set codec info for the user
        * @param aCodecInfo
        */
        void SetCodecInfoL( const TMccCodecInfo& aCodecInfo );

        TMccCodecInfo& CodecInfo();
        
        TInt GetPayloadSize();
    
    public: // From MMccVideoJitterBufferObserver
    
        void ErrorOccured( TInt aError );
        
    private:
    
        TInt NumTimeStamps() const;
        
        void RemoveFirstTimeStamp();
        
        void RemoveAllTimeStamps();
        
        TBool DoModify( TRtpRecvHeader& aHeaderInfo );
        
        TInt SeqNumDifToLastSeqNum( TUint16 aSeq );
        
       
        void DoModifyCheckInit( const TRtpRecvHeader& aHeaderInfo );
        
        void DoModifyCheckMonitoring( const TRtpRecvHeader& aHeaderInfo );
        
        void SetModifyingState( TMccTimeStampModifyMode aModifyMode );
        
        TBool MonitoringModify();
        
        TBool ModifyRequired();
        
        TUint32 GetTimestampDifferenceToPrevious( TUint32 aCurrentTimestamp );
        
        void UpdateFirstTimestamp( TUint32 aTimestamp );
        
        TUint32 DifferenceThreshold( TInt aSeqNumDif ) const;
        
        void CheckDenyFramesMaxPeriod();
        
        void DecideMonitoringContinuation( TMccTimeStampModifyMode aNextState );
        
        TBool IsResetNeededNormalMode( TRtpRecvHeader& aHeaderInfo );
    
        TBool IsResetNeededRealTimeMode( TRtpRecvHeader& aHeaderInfo );
        
        TBool RealTimeUser();
        
        void CheckErrorL( TInt aError );
        
        TBool CodecSpecificDataHandling( const TDesC8& aPayloadData );
    
    private:
    
        CMccVideoSinkUser( MAsyncEventHandler* aAsyncEventHandler, 
                           TUid aMediaType,
                           TUint aStreamId,
                           TReal aFrameRate,
                           CXPSPacketSink& aPacketSink );
                                                   
    public: // Data
    
        MAsyncEventHandler* iAsyncEventHandler;
        TUid iMediaType;
        TUint iStreamId;
        TMccTimeStampEntry iTimeStamps[ KMccTimeStampArraySize ];
        TBool iStartedOnce;
        TReal iFrameRate;
        TMccCodecInfo iCodecInfo;
        
    private:
    
        CXPSPacketSink& iPacketSink;  
        
        TMccPacketOverflowState iPacketOverflowState;
        TInt iNumTimeStamps;
        TUint32 iAverageTimeStampDifference;
        TBool iResetNeeded;
        TMccTimeStampModifyMode iModifyMode;
        
        TTime iFirstModifyCheck;
        TUint32 iFirstModifyCheckTimestamp;
        TUint32 iFirstTimestamp;
        
        TBool iAllowFrame;
        TTime iDenyFramesStarted;
        
        TBool iMultipleMediaTypes;
        
        TUint32 iPreviousPacketOriginalTimeStamp;
        TUint32 iPreviousPacketModifiedTimeStamp;
        TTime iPreviousPacketTime;
        
        TUint32 iCurrentMonitoringPeriod;
        
        TReal iTimestampIncreaseFactorDown;
        TReal iTimestampIncreaseFactorUp;
        
        CMccVideoJitterBuffer* iJitterBuf;
        
        TInt iPayloadSize;
    
    private:
    	#ifdef TEST_EUNIT
			friend class UT_CMccVideoSinkImpl;
			friend class UT_CMccVideoSinkUser;	
   		#endif    
    };

#endif  // MCCVIDEOSINKUSER_H

