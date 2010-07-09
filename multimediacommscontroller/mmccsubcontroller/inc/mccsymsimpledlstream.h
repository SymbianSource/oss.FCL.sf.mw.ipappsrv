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



#ifndef MCCSYMSIMPLEDLSTREAM_H
#define MCCSYMSIMPLEDLSTREAM_H

//  INCLUDES
#include "mccsymstreambase.h"
#include "mccrtpdatasource.h"
#include "mccjitterbufferobserver.h"

// FORWARD DECLARATIONS
class CMMFFormatDecode;
class MMccResources;

// CLASS DECLARATION

/**
*  MCP Symmetric subthread simple downlink stream
*
*  @lib Mccsubthreads.lib
*  @since Series 60 3.0
*/
NONSHARABLE_CLASS( CMccSymSimpleDlStream ) : public CMccSymStreamBase, public MJitterBufferObserver
    {
    public:  // Constructors and destructor
        
        /**
        * Static constructor
        */
        static CMccSymSimpleDlStream* NewLC( TUint32 aMccStreamId, 
                                             MAsyncEventHandler* aEventhandler,  
                                             MMccResources* aMccResources,
                                             CMccRtpManager* aManager, 
                                             TFourCC aFourCC, 
                                             TInt aStreamType,
                                             CMccRtpMediaClock& aClock );
        
        /**
        * Destructor.
        */
        virtual ~CMccSymSimpleDlStream();

    public: // New functions

		void CreatePayloadFormatDecoderL();

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
        
        void UpdateCodecInformationL( TMccCodecInfo& aCodecInfo );

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
         * From MJitterBufferObserver
         */
        void JitterBufferError( TInt aError );
        
        /**
         * From MJitterBufferObserver
         */
        void DynamicBufferChangeRequest( TInt aBufferSize );

    private:
        
        /**
        * C++ default constructor.
        */
        CMccSymSimpleDlStream( TUint32 aMccStreamId, 
                               MAsyncEventHandler* aEventhandler, 
                               MMccResources* aMccResources,
                               CMccRtpManager* aManager, 
                               TFourCC aFourCC, 
                               TInt aStreamType,
                               CMccRtpMediaClock& aClock );
        
        /**
        * 2nd phase constructor
        */
        void ConstructL();

            
    protected: // Functions from base classes
    
        MDataSink* MultiplexerSink();
        
    private:    // Data
    
    	TFourCC iFourCC;
    
        CMMFFormatDecode* iFormatDecode;
    private: // friend class
        #ifdef TEST_EUNIT
        friend class UT_CMccSymSimpleDlStream;
        #endif
    };

#endif      // MCCSYMSIMPLEDLSTREAM_H

// End of File
