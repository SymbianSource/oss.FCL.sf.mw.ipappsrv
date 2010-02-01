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
* Description:    Provides an symmetric subthread client definition
*
*/




#ifndef MCCSYMSUBTHREADCLIENT_H
#define MCCSYMSUBTHREADCLIENT_H

//  INCLUDES
#include <e32base.h>
#include <mmf/server/mmfdatasink.h>
#include <mmf/server/mmfdatasource.h>
#include "mccsubthreadclientbase.h"
#include "mccinternaldef.h"


// FORWARD DECLARATIONS
class CMccRtpManager;
class CMccSymStreamBase;
class TMccEvent;
class MMccResources;
class CMccRtpMediaClock;

// CLASS DECLARATION

/**
*  Mcc Symmetric Subthread Client 
*  
*  @lib Mccsubthreads.lib
*  @since Series 60 3.0
*/

NONSHARABLE_CLASS( CMccSymSubthreadClient ): public CMccSubThreadClientBase,
    public MAsyncEventHandler
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CMccSymSubthreadClient* NewL( MMccEventHandler* aObserver,
                                             MMccResources* aMccResources,
                                             TInt aLinkType,
                                             TUint32 aMccSessionId );
        
        /**
        * Destructor.
        */
        virtual ~CMccSymSubthreadClient();
		
    public: // Functions from MAsyncEventHandler
        
		TInt SendEventToClient( const TMMFEvent& aEvent );

    public: // Functions from CMccSubThreadClientBase

        void OpenL( TInt aStreamType,
                    TFourCC aFourCC,
                    MDataSource* aDataSource, 
                    MDataSink* aSink,
                    const TUint32 aStreamId,
                    TMMFPrioritySettings aSettings );
        
        void SetBalanceL( const TUint32 aStreamId, 
                          TInt aLeftBalance, 
                          TInt aRightBalance );

        void GetBalanceL( const TUint32 aStreamId, 
                          TInt& aLeftBalance,
                          TInt& aRightBalance );

        void PrepareL( const TUint32 aStreamId,
        			   const TUint32 aEndpointId );

        void PlayL( const TUint32 aStreamId,
					const TUint32 aEndpointId,
                    TBool aStreamPaused,
                    TBool aEnableRTCP );

        void StopL( const TUint32 aStreamId,
        			const TUint32 aEndpointId );
        
        void PauseL( const TUint32 aStreamId,
        			 const TUint32 aEndpointId,
                     TBool aEnableRTCP );
        void ResumeL( const TUint32 aStreamId,
        			  const TUint32 aEndpointId,
                      TBool aEnableRTCP );

        void CloseL( const TUint32 aStreamId );

        void CloseL();

        TBool StreamsExistsL();
   
        void SetVolumeL( TInt aVolume );

        void SetGainL( TInt aGain );

        TInt MaxVolumeL( const TUint32 aStreamId);
  
        TInt MaxGainL( const TUint32 aStreamId );

        TInt GetVolumeL( const TUint32 aStreamId );

        TInt GetGainL( const TUint32 aStreamId );

        void SendMediaSignalL( const TMccEvent& aEvent );
        
        /**
        * Sends RTCP receiver report to uplink if possible.
        * @param aStreamId - [input] Stream ID identifying stream
        * @return void
        */
        void SendRTCPReceiverReportL( const TUint32 aStreamId );

        /**
        * Sends RTCP sender report to uplink if possible.
        * @param aStreamId - [input] Stream ID identifying stream
        * @return void
        */
        void SendRTCPSenderReportL( const TUint32 aStreamId );
        
        /**
        * Sends Non-RTCP data to uplink if possible.
        * @param aStreamId - [input] Stream ID identifying stream
        * @param aData - [input] Data to send
        * @return void
        */
        void SendRTCPDataL( const TUint32 aStreamId,
                            const TDesC8& aData );

       /**
        * Starts inactivity timer for a stream in a given session.
        * @param aStreamId - [input] Stream ID identifying stream
        * @param aTimeoutTime - [input] timeout value in milliseconds
        * @return One of the standard system-wide error codes.
        */
        void StartInactivityTimerL( const TUint32 aStreamId,
                                    TUint32 aTimeoutTime );

        /**
        * Stops inactivity timer for a stream in a given session.
        * @param aStreamId - [input] Stream ID identifying stream
        * @return One of the standard system-wide error codes.
        */
        void StopInactivityTimerL(  const TUint32 aStreamId );
        
        /**
        * Get the supported bitrates from the codec of the specified stream
        * @since Series 60 3.0
        * @param aStreamId - [input] Stream ID identifying stream
        * @param aBitrates Array for the bitrates
        * @leave System wide error code if unsuccessful
        * @return None
        */
        void GetSupportedBitratesL( const TUint32 aStreamId,
                                            RArray<TUint>& aBitrates );
        /**
        * @See CMccSubThreadClientBase
        *
        */                                                 
        void GetSSRCL( const TUint32 aStreamId, TUint32& aSSRCValue );
                                           
        MDataSink* DataSink( const TUint32 aStreamId );

        MDataSource* DataSource( const TUint32 aStreamId );
        
        /**
        * Get the supported bitrates from the codec of the specified stream
        * @since Series 60 3.0
        * @param aStreamId - [input] Stream ID identifying stream
        * @param aFmtp [output] The FMTP attribute
        * @return None
        */
        void GetFmtpAttrL( const TUint32 aStreamId, TDes8& aFmtp );
        	
		void SetRemoteAddressL( TInetAddr aRemAddr );
		
		void SetRemoteRtcpAddrL( TInetAddr aRemAddr );
		
		/**
        * 2nd step to create link
        * @since Series 60 3.0
        * @param [input/output] aStatus
        * @param [input] aIapId
        * @return void
        */
		void InitializeLinkL( TRequestStatus& aStatus, TInt aIapId );
		
		/**
        * 3rd step to create link. Creates a RTP session in the subthread
        * @since Series 60 3.0
        * @param [input/output] aPort
        * @param [input] aEnableRTCP
        * @param [input] TOS bit settings
        * @param [input] Reference to RTP Media Clock instance
        * @return void
        */	
		void CreateRtpSessionL( TUint aPort, 
        	                    TBool aEnableRTCP, 
        	                    TInt aIpTos,
        	                    CMccRtpMediaClock& aClock );
        
        void SetAudioRouteL( TUint32 aStreamId,
                             TUint32 aRoutingDestination );
        
        void GetAudioRouteL( TUint32 aStreamId,
                             TUint32& aRoutingDestination );
                             
        void UnuseL( TUint32 aStreamId );
        
        /**
        * Resolves and returns local IP addresses and ports
        * @since Series 60 3.2
        * @param [input/output] aClientData Client data
        * @return void
        */
        void GetLocalIpAddressesL( TMccCreateLink& aClientData );

        void SetParameterL( TUint32 aParam,
                            TUint32 aStreamId,
                            TUint32 aEndpointId,
                            const TDesC8& aVal );
         
        void GetParameterL( TUint32 aParam,
                            TUint32 aStreamId,
                            TUint32 aEndpointId,
                            TDesC8& aVal );
        

        /**
         * Bind context into stream.
         * @since Series 60 3.2
         * @param aStreamId The stream id
         * @param aEndpointId The endpoint id
         * @param aContextParams Crypto context
         * @return void
         */		
        void BindContextIntoStreamL( TUint32 aStreamId, 
                                     TUint32 aEndpointId,
                                     const TMccCryptoContext& aContextParams );
                                     
        /**
         * Remove context.
         * @since Series 60 3.2
         * @param aStreamId The stream id
         * @param aEndpointId The endpoint id

         * @return void
         */		
        void RemoveContextL( TUint32 aStreamId, 
                             TUint32 aEndpointId );
        
    private:  // Private construction

        /**
        * C++ default constructor.
        */
        CMccSymSubthreadClient( MMccEventHandler* aObserver,
                                MMccResources* aMccResources,
                                TInt aLinkType,
                                TUint32 aMccSessionId );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    
    private:  // New functions
    
		void DoMccEvent( TMccEvent& aEvent, const TMMFEvent& aMMFEvent );
		
		TInt HandleAmrEvent( const TMMFEvent& aEvent,
		                     const TMccEvent& aMccEvent );
		
    private:    // Data
        		        
        // Priority settings
        TMMFPrioritySettings iPrioritySettingsData;
        
        RArray<TUint32> iUnusedStreams;

        // Rtp media clock instance
        CMccRtpMediaClock* iRtpMediaClock;
                        
        #ifdef TEST_EUNIT
        friend class UT_CMccSymSubthreadClient;
        #endif

    };

#endif      // MCCSYMSUBTHREADCLIENT_H  

// End of File

