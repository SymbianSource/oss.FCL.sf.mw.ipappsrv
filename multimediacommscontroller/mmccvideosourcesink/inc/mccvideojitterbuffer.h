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




#ifndef MCCVIDEOJITTERBUFFER_H
#define MCCVIDEOJITTERBUFFER_H

// INCLUDES
#include <e32base.h>
#include "rtpdef.h"
#include "rtpheader.h"

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class CXPSPacketSink;

// CONSTANTS

const TInt KMccJitterBufferDefaultMaxSize = 1200;
const TInt KMccJitterBufferDefaultPlayThreshold = 600;
const TInt KMccJitterBufferDefaultLowLimit = 200;
const TInt KMccJitterBufferDefaultHighLimit = 900;

// CLASS DECLARATION

/**
* Callback interface for buffer user.
*/ 
class MMccVideoJitterBufferObserver
    {
    public:
        virtual void ErrorOccured( TInt aError ) = 0;
    };
    
/**
* Buffer for queueuing video packets before posting them towards Helix.
* Buffer is used to fight against network abnormalities (e.g. bursts).
*
*/
class CMccVideoJitterBuffer : public CTimer
    {

public:

    /**
    * Buffering status
    */        
    enum TMccPacketBufferingStatus
            {
            EBuffering,
            EPlayingStarted,
            EPlaying
            };
            

    /**
    * Storing data of single packet
    */
    class CMccVideoPacket : public CBase
        {
        public:
        
            static CMccVideoPacket* NewLC( TUint aStreamId, 
                                           const TRtpRecvHeader& aHeaderInfo, 
                                           const TDesC8& aPayloadData,
                                           TBool aImportantData );
            ~CMccVideoPacket();
            
            TUint StreamId();
            
            TRtpRecvHeader& HeaderInfo();
    
            const TDesC8& PayloadData();
            
            static TInt InsertInSeqNumOrder( const CMccVideoPacket& aPacket1, 
                                             const CMccVideoPacket& aPacket2 );
            
            TBool DroppingAllowed() const;
            
        private:
        
            CMccVideoPacket( TUint aStreamId, 
                             const TRtpRecvHeader& aHeaderInfo,
                             TBool aImportantData );
            
            void ConstructL( const TDesC8& aPayloadData );
            
        private:
        
            TUint iStreamId;
            
            TRtpRecvHeader iHeaderInfo;
            
            TBool iImportantData;
            
            HBufC8* iPayloadData;
            
        };
        
    /**
     * Two-phased constructor.
     * @param aObserver
     * @param aPacketSink
     * @param aInactivityTimeout in milliseconds
     */
    static CMccVideoJitterBuffer* NewL( MMccVideoJitterBufferObserver& aObserver, 
                                        CXPSPacketSink& aPacketSink,
                                        TUint aInactivityTimeoutInMs );

    /**
     * Two-phased constructor.
     * @param aObserver
     * @param aPacketSink
     * @param aInactivityTimeout in milliseconds
     */
    static CMccVideoJitterBuffer* NewLC( MMccVideoJitterBufferObserver& aObserver,
                                         CXPSPacketSink& aPacketSink,
                                         TUint aInactivityTimeoutInMs );

    /**
     * Destructor.
     */
    ~CMccVideoJitterBuffer();
    
    /**
    * Configure behavior of jitter buffer
    * @param aLowLimitMs
    * @param aHighLimitMs
    * @param aPlayThresholdMs
    * @param aMaxSizeMs
    * @param aFrameRateFps
    */
    void ConfigureL( TUint aLowLimitMs, 
                     TUint aHighLimitMs, 
                     TUint aPlayThresholdMs, 
                     TUint aMaxSizeMs, 
                     TUint aFrameRateFps );
    
    /**
    * Enqueu packet
    * @param aStreamId
    * @param aHeaderInfo
    * @param aPayloadData
    * @param aImportantData, ETrue if packet cannot be dropped
    * @return buffering status
    */
    TMccPacketBufferingStatus EnqueueL( TUint aStreamId, 
                    const TRtpRecvHeader& aHeaderInfo, 
                    const TDesC8& aPayloadData,
                    TBool aImportantData );
    
    /**
    * Start/resume packet posting
    */               
    void Play();
    
    /**
    * Pause packet posting
    */
    void Pause();
    
    /**
    * Get play threshold in milliseconds
    * @return play threshold in milliseconds
    */
    TInt PlayThresholdInMs();

protected: // from base class CActive

    void RunL();

private:

    CMccVideoJitterBuffer( MMccVideoJitterBufferObserver& aObserver,
                           CXPSPacketSink& aPacketSink,
                           TUint aInactivityTimeoutInMs );

    void ConstructL();
    
    void Start();
    
    TBool PlayThresholdExceeded();
    
    TTimeIntervalMicroSeconds32 CheckCurrentInterval();
    
    void UpdateFrameCount( CMccVideoPacket& aPacket, TBool aIsAdded );
    
    TBool PostFirstFrame();
    
    void MakeRoom();
    
    TBool MaxSizeExceeded();
    
    void CheckPostingInactivity( TInt64 aTimeFromPreviousFrame );
    
    CMccVideoJitterBuffer::TMccPacketBufferingStatus PlayingStatus();
    
    void DoRtpHeaderModify( TRtpRecvHeader& aRecvHeader );
    
private: // data

    MMccVideoJitterBufferObserver& iObserver;
    CXPSPacketSink& iPacketSink;
    TUint iInactivityTimeoutInMicroSecs;
    
    RPointerArray<CMccVideoPacket> iQueue;

	TUint iLowLimitMicroSecs;
    TUint iHighLimitMicroSecs;
    TUint iPlayThresholdMicroSecs;
    TUint iMaxSizeMicroSecs;
    TUint iFrameRate;
    
    TTimeIntervalMicroSeconds32 iCurrentInterval;
    TTimeIntervalMicroSeconds32 iNormalInterval;
    TTimeIntervalMicroSeconds32 iLowLimitInterval;
    TTimeIntervalMicroSeconds32 iHighLimitInterval;
    
    TUint iFrameCount;
    
    TBool iPlayThresholdExeeced;
    
    TBool iPlaybackStarted;
    
    TUint16 iSeqNum;
    
    TUint32 iCurrentRealtimeTimestamp;
    TUint32 iPreviousReceivedTimestamp;
    TTime iPreviousTime;
    
    private:
    	#ifdef TEST_EUNIT
			friend class UT_CMccVideoJitterBuffer;
			friend class UT_CMccVideoSinkUser;
   		#endif   	
    };
    


#endif  // MCCVIDEOJITTERBUFFER_H

