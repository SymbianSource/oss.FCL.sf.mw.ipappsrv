/*
* Copyright (c) 2004-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Provides a downlink stream for symmetric subthread
*
*/



#ifndef MCCSYMDLSTREAM_H
#define MCCSYMDLSTREAM_H

//  INCLUDES
#include "mccsymstreambase.h"
#include "mccrtpdatasource.h"
#include "mccjitterbufferobserver.h"

// FORWARD DECLARATIONS
class CMccJitterBuffer;
class CMMFFormatDecode;
class CMccAudioRouter;
class MMccResources;

// CLASS DECLARATION

/**
*  Mcc Symmetric subthread downlink stream
*
*  @lib Mccsubthreads.lib
*  @since Series 60 3.0
*/
NONSHARABLE_CLASS( CMccSymDlStream ) : public CMccSymStreamBase, public MJitterBufferObserver
    {
    public:  // Constructors and destructor
        
        /**
        * Static constructor
        */
        static CMccSymDlStream* NewLC( TUint32 aMccStreamId, 
                MAsyncEventHandler* aEventhandler,  
                MMccResources* aMccResources, 
                CMccRtpManager* aManager,
                TInt aStreamType,
                CMccRtpMediaClock& aClock );
        
        /**
        * Destructor.
        */
        virtual ~CMccSymDlStream();

    public: // Functions from base classes
    
        /**
        * From CMccSymStreamBase
        */
        void SetPrioritySettingsL( const TMMFPrioritySettings& aPriority );
        
        /**
        * From CMccSymStreamBase
        */
        void AddSinkAndSourceL( MDataSink* aDatasink, 
                MDataSource* aDatasource );
        
        /**
        * From CMccSymStreamBase
        */      
        void LoadCodecL( const TMccCodecInfo& aCodecInfo,
                         const TDesC8& aFmtp );

        /**
        * From CMccSymStreamBase
        */
        void PrimeL( const TUint32 aEndpointId );
        
        /**
        * From CMccSymStreamBase
        */
        void PlayL( const TUint32 aEndpointId,
        			TBool aStreamPaused, 
                    TBool aEnableRtcp );
        
        /**
        * From CMccSymStreamBase
        */
        void PauseL( const TUint32 aEndpointId,
        			 TBool aEnableRtcp );
        
        /**
        * From CMccSymStreamBase
        */
        void ResumeL( const TUint32 aEndpointId,
        			  TBool aEnableRtcp );
        
        /**
        * From CMccSymStreamBase
        */
        void StopL( const TUint32 aEndpointId );

        /**
        * From CMccSymStreamBase
        */
        void SetBalanceL( TInt aLeftbal, TInt aRightbal );
        
        /**
        * From CMccSymStreamBase
        */
        void GetBalanceL( TInt& aLeftbal, TInt& aRightbal );
        
        /**
        * From CMccSymStreamBase
        */ 
        void GetSSRCL( TUint32& aSSRCValue );

        /**
        * From CMccSymStreamBase
        */
        void ResetCountersL();
        
        /**
        * From CMccSymStreamBase
        */
        void SendMediaSignalL( const TMccEvent& aEvent );
        
        /**
        * Sends RTCP receiver report.
        * @param none
        * @return One of the standard system-wide error codes.
        */
        void SendRTCPReceiverReportL();
        
        /** 
        * Starts inactivity timer for a stream
        * @param aTimeoutTime - [input] timeout value in milliseconds
        * @return One of the standard system-wide error codes.
        */
        void StartInactivityTimerL( TUint32 aTimeoutTime );

        /**
        * Stops inactivity timer for a stream 
        * @return One of the standard system-wide error codes.
        */
        void StopInactivityTimerL();
        
        /**
        * Checks if stream is uplink 
        * @return EFalse for downlink stream.
        */
        TBool IsUplink();

        /**
        * Sets stream to standby mode. 
        */
        void StandByL();
        
        /**
        * Set audio routing.
        */
        void SetAudioRouteL( TUint32 aRoutingDestination );
        
        /**
        * Get current audio routing destination.
        */
        void GetAudioRouteL( TUint32& aRoutingDestination );
        
    	/**
         * From MJitterBufferObserver
         */
        void JitterBufferError( TInt aError );
        
        /**
         * From MJitterBufferObserver
         */
        void DynamicBufferChangeRequest( TInt aBufferSize );
        
        /**
         * Bind context into stream.
         * @since Series 60 3.2
         * @param aEndpointId The endpoint id
         * @param aContextParams Crypto context
         * @return void
         */
        void BindContextIntoStreamL( TUint32 aEndpointId,
                                     const TMccCryptoContext& aContextParams );
        
    private:
        
        /**
        * C++ default constructor.
        */
        CMccSymDlStream( TUint32 aMccStreamId, 
                         MAsyncEventHandler* aEventhandler, 
                         MMccResources* aMccResources, 
                         CMccRtpManager* aManager, 
                         TInt aStreamType,
                         CMccRtpMediaClock& aClock );
        
        /**
        * 2nd phase constructor
        */
        void ConstructL();

    protected: // Functions from base classes
    
        MDataSink* MultiplexerSink();

    private:    // New functions
    
        /**
        * Creates the payload format decoder
        * @since Series 60 3.0
        * @param None
        * @return void
        */
        void CreatePayloadFormatDecoderL();
        
        /**
         * Worker function for G.711 dynamic buffering
         * @since S60 v3.2
         * @param aFrameSize New frametime in milliseconds
         * @return void
         */
        void DoDynamicFrameSizeChangeL( TInt aFrameTime );
        
        /**
         * Worker function for updating codec parameters
         * @since S60 v3.2
         * @param aOriginal Original codec settings
         * @param aNew New codec settings
         * @return void
         */
        void UpdateCodecInformationL( const TMccCodecInfo& aOriginal,
            const TMccCodecInfo& aNew );
                
    private:    // Data
    
        // Jitterbuffer
        CMccJitterBuffer* iJitterBuffer;
        
        // Payload format decoder
        CMMFFormatDecode* iFormatDecode;

        // Payload format decoder for redundancy format
        CMMFFormatDecode* iRedFormatDecode;
        
        // Audio router
        CMccAudioRouter* iAudioRouter;
        
        TBool iFirstResumeDone;
        
    private:
        #ifdef EUNIT_TEST
            friend class UT_CMccSymDlStream;
        #endif

    };

#endif      // MCCSYMDLSTREAM_H

// End of File
