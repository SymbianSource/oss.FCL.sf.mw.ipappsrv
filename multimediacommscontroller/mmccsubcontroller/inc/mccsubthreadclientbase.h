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
* Description:    Provides a baseclass for subthread clients to implement
*
*/




#ifndef MCCSUBTHREADCLIENTBASE_H
#define MCCSUBTHREADCLIENTBASE_H

//  INCLUDES
#include <e32base.h>
#include "mccinternalcodecs.h"

// CONSTANTS

// FORWARD DECLARATIONS
class MDataSource;
class MDataSink;
class MMccEventHandler;
class TMccEvent;
class CMccRtpManager;
class MMccResources;
class CMccRtpMediaClock;
class TMccCreateLink;
class CMccSymStreamBase;
class TMccCryptoContext;

/**
* Mcc Subthread Client baseclass
*
* @lib Mccsubthreads.lib
* @since Series 60 3.0
*/
NONSHARABLE_CLASS( CMccSubThreadClientBase ) : public CBase
    {
    public:  // Constructor

        /**
        * Destructor
        */
        virtual ~CMccSubThreadClientBase();

    public: // New functions

        /**
        * 1st step to create link asynchronously
        * @since Series 60 3.0
        * @param [input/output] aStatus
        * @param [input] aIapId
        * @return void
        */
        virtual void InitializeLinkL( TRequestStatus& aStatus,
                                      TInt aIapId );
        
        /**
        * 2nd step to create link. Creates a RTP session in the subthread
        * @since Series 60 3.0
        * @param [input/output] aPort Local port
        * @param [input] aEnableRTCP  Whether to enable RTCP for RTP session
        * @param [input] aIpTos       TOS bit settings
        * @param [input] aClock       Mediaclock for timestamp synchronization
        * @return void
        */
        virtual void CreateRtpSessionL( TUint aPort,
                                        TBool aEnableRTCP,
                                        TInt aIpTos,
                                        CMccRtpMediaClock& aClock );

        /**
        * Returns the codec info of the subthread client
        * @since Series 60 3.0
        * @param aStreamId The stream id
        * @param [output] aCodec The codec info reference where to write
        * @return void
        */
        void CodecInformationL( TUint32 aStreamId, TMccCodecInfo& aCodec );
        
        /**
        * Sets the codec information for this stream
        * @since Series 60 3.0
        * @param aStreamId Stream ID
        * @param aCodecInfo Codec information
        * @param aFmtp FMTP line associated with codec
        * @return void
        */
        void SetCodecInformationL( TUint32 aStreamId, 
                                   const TMccCodecInfo& aCodecInfo, 
                                   const TDesC8& aFmtp );

        /**
        * Sets the MMF priority settings for the subthread
        * @since Series 60 3.0
        * @param aStreamId The stream id
        * @param [input] aPrioritySettings The priority settings
        * @return void
        */
        void SetPrioritySettingsL( TUint32 aStreamId,
                                   const TMMFPrioritySettings& aPrioritySet );

         /**
        * Stores the link id number
        * @since Series 60 3.0
        * @param [input] aLinkId Link index number
        * @return void
        */
        void SetLinkId( const TUint32 aLinkId );

        /**
        * Returns the link index number
        * @since Series 60 3.0
        * @param None
        * @return TInt System wide error code
        */
        TUint32 GetLinkId() const;

        /**
        * Returns the MCC session id
        * @since Series 60 3.0
        * @param None
        * @return TInt System wide error code
        */
        TUint32 MccSessionId() const;

        /**
        * Returns current volume
        * @since Series 60 3.0
        * @param [input] Stream ID
        * @return TInt Current volume
        */
        virtual TInt GetVolumeL( const TUint32 aStreamId ) = 0;
        
        /**
        * Sets current volume 
        * @since Series 60 3.0
        * @param [input] New volume
        * @return void
        */
        virtual void SetVolumeL( TInt aVolume ) = 0;
        
        /**
        * Returns maximum volume 
        * @since Series 60 3.0
        * @param [input] Stream ID
        * @return TInt Max volume
        */
        virtual TInt MaxVolumeL( const TUint32 aStreamId ) = 0;
        
        /**
        * Returns current gain of uplink subthread
        * @since Series 60 3.0
        * @param [input] Stream ID
        * @return TInt Current gain
        */
        virtual TInt GetGainL( const TUint32 aStreamId ) = 0;
        
        /**
        * Sets the gain for uplink subthread
        * @since Series 60 3.0
        * @param [input] aGain Gain for uplink playback
        * @return void
        */
        virtual void SetGainL( TInt aGain ) = 0;

        /**
        * Returns the maximum gain for uplink subthread 
        * @since Series 60 3.0
        * @param [input] Stream ID
        * @param [output] aMaxGain Contains the audio HW maximum gain
        * @return void
        */
        virtual TInt MaxGainL( const TUint32 aStreamId ) = 0;

        /**
        * Opens & creates the subthread 
        * @since Series 60 3.0
        * @param [input] aStreamType The type of stream
        * @param [input] aFourCC the fourCC code of the desired codec
        * @param [input] aDatasource Datasource
        * @param [input] aDatasink Datasink
        * @param [input] aStreamId id of the new stream
        * @return TUint32
        */
        virtual void OpenL( TInt aStreamType, 
                            TFourCC aFourCC,
                            MDataSource* aDatasource, 
                            MDataSink* aDatasink,
                            const TUint32 aStreamID,
                            TMMFPrioritySettings aSettings ) = 0;

        /**
        * Sets the record/playback balance for ul/dl streams
        * @since Series 60 3.0
        * @param [input]  Stream ID
        * @param [input] aLeftBalance Left side balance for balance
        * @param [input] aRightBalance Right side balance for balance
        * @return void 
        */
        virtual void SetBalanceL( const TUint32 aStreamId, 
                                  TInt aLeftBalance,
                                  TInt aRightBalance ) = 0;

        /**
        * Returns the record/playback balance for ul/dl streams
        * @since Series 60 3.0
        * @param [input]  Stream ID
        * @param [output] aLeftBalance Return value of left side balance for
        *                 balance
        * @param [output] aRightBalance Return value of right side balance for
        *                 balance
        * @return void
        */
        virtual void GetBalanceL( const TUint32 aStreamId, 
                                  TInt& aLeftBalance,
                                  TInt& aRightBalance ) = 0;

        /**
        * Sets the remote address                       
        * @since Series 60 3.0
        * @param [input] aRemAddr Contains the remote host IP address
        * @return void
        */
        virtual void SetRemoteAddressL( TInetAddr aRemAddr );

		/**
        * Sets the remote rtcp address                       
        * @since Series 60 3.0
        * @param [input] aRemAddr Contains the remote rtcp host IP address
        * @return void
        */
        virtual void SetRemoteRtcpAddrL( TInetAddr aRemAddr );
        
        /**
        * Prepares the subthread 
        * @since Series 60 3.0
        * @param aStreamId Stream ID
        * @return void
        */
        virtual void PrepareL( const TUint32 aStreamId,
        					   const TUint32 aEndpointId ) = 0;

        /**
        * Starts a subthread
        * @since Series 60 3.0
        * @param aStreamId Stream ID
        * @return void
        */
        virtual void PlayL( const TUint32 aStreamId,
        					const TUint32 aEndpointId,
                            TBool aStreamPaused,
                            TBool aEnableRTCP ) = 0;

        /**
        * Stops a subthread
        * @since Series 60 3.0
        * @param aStreamId Stream ID
        * @return void
        */
        virtual void StopL( const TUint32 aStreamId,
        					const TUint32 aEndpointId ) = 0;

        /**
        * Pauses subthread record or playback
        * @since Series 60 3.0
        * @param aStreamId Stream ID
        * @return void
        */
        virtual void PauseL( const TUint32 aStreamId,
        					 const TUint32 aEndpointId,
                             TBool aEnableRTCP ) = 0;

        /**
        * Resumes a paused subthread
        * @since Series 60 3.0
        * @param aStreamId Stream ID
        * @return void
        */
        virtual void ResumeL( const TUint32 aStreamId,
        					  const TUint32 aEndpointId,
                              TBool aEnableRTCP ) = 0;

        /**
        * Closes the subthread clients
        * @since Series 60 3.0
        * @param aStreamId Stream ID
        * @return void
        */
        virtual void CloseL( const TUint32 aStreamId ) = 0;

        /**
        * Closes all subthread clients
        * @since Series 60 3.0
        * @param aStreamId Stream ID
        * @return void
        */
        virtual void CloseL();
        
        /**
        * Checks if link contains any streams
        * @since Series 60 3.0
        * @return ETrue, if streams exists
        */
        virtual TBool StreamsExistsL();

        /**
        * Sends media signals to this stream
        * @since Series 60 3.0
        * @param aEvent Media event to be sent
        * @return void
        */                             
        virtual void SendMediaSignalL( const TMccEvent& aEvent );
        
        /**
        * Sends RTCP receiver report to uplink if possible.
        * @param aStreamId - [input] Stream ID identifying stream
        * @return void
        */
        virtual void SendRTCPReceiverReportL( const TUint32 aStreamId );

        /**
        * Sends RTCP sender report to uplink if possible.
        * @param aStreamId - [input] Stream ID identifying stream
        * @return void
        */
        virtual void SendRTCPSenderReportL( const TUint32 aStreamId );

        /**
        * Sends Non-RTCP data to uplink if possible.
        * @param aStreamId - [input] Stream ID identifying stream
        * @param aData - [input] Data to send
        * @return void
        */
        virtual void SendRTCPDataL( const TUint32 aStreamId,
                                    const TDesC8& aData );
        
       /**
        * Starts inactivity timer for a stream in a given session.
        * @param aStreamId - [input] Stream ID identifying stream
        * @param aTimeoutTime - [input] timeout value in milliseconds
        * @return One of the standard system-wide error codes.
        */
        virtual void StartInactivityTimerL( const TUint32 aStreamId,
                                            TUint32 aTimeoutTime );

        /**
        * Stops inactivity timer for a stream in a given session.
        * @param aStreamId - [input] Stream ID identifying stream
        * @return One of the standard system-wide error codes.
        */
        virtual void StopInactivityTimerL( const TUint32 aStreamId );
        
        /**
        * Get the supported bitrates from the codec of the specified stream
        * @since Series 60 3.0
        * @param aStreamId - [input] Stream ID identifying stream
        * @param aBitrates Array for the bitrates
        * @leave System wide error code if unsuccessful
        * @return None
        */
        virtual void GetSupportedBitratesL( const TUint32 aStreamId,
                                            RArray<TUint>& aBitrates );
         
        /**
        * Get Syncronization source for the specified stream
        * @since Series 60 3.0
        * @param aStreamId - [input] Stream ID identifying stream
        * @param aSSRCValue - [input] a reference value of SSRC.
        * @leave System wide error code if unsuccessful
        * @return None.
        */  
                                       
        virtual void GetSSRCL( const TUint32 aStreamId,
                               TUint32& aSSRCValue);

        /**
        * Get the supported bitrates from the codec of the specified stream
        * @since Series 60 3.0
        * @param aStreamId - [input] Stream ID identifying stream
        * @return None
        */
        virtual void GetFmtpAttrL( const TUint32 aStreamId, TDes8& aFmtp );

        virtual inline TBool IsDtmf()
        	{
        	return iIsDTMF;
        	}

        virtual MDataSink* DataSink( const TUint32 aStreamId ) = 0;

        virtual MDataSource* DataSource( const TUint32 aStreamId ) = 0;                        
        
        virtual void SetAudioRouteL( TUint32 aStreamId,
                                     TUint32 aRoutingDestination ) = 0;
        
        virtual void GetAudioRouteL( TUint32 aStreamId,
                                     TUint32& aRoutingDestination ) = 0;
        
        virtual void UnuseL( TUint32 aStreamId ) = 0;

        /**
        * Resolves local IP addresses and ports
        * @since Series 60 3.2
        * @param [input/output] aClientData Client data
        * @return void
        */
        virtual void GetLocalIpAddressesL( TMccCreateLink& aClientData ) = 0;

        virtual void SetParameterL( TUint32 aParam,
                                    TUint32 aStreamId,
                                    TUint32 aEndpointId,
                                    const TDesC8& aVal ) = 0;
         
        virtual void GetParameterL( TUint32 aParam,
                                    TUint32 aStreamId,
                                    TUint32 aEndpointId,
                                    TDesC8& aVal ) = 0;

        /**
         * Bind context into stream.
         * @since Series 60 3.2
         * @param aStreamId The stream id
         * @param aEndpointId The endpoint id
         * @param aContextParams Crypto context
         * @return void
         */	
        virtual void BindContextIntoStreamL( TUint32 aStreamId,
                                             TUint32 aEndpointId,
                                             const TMccCryptoContext& aContextParams ) = 0;
                                             
        /**
         * Remove context.
         * @since Series 60 3.2
         * @param aStreamId The stream id
         * @param aEndpointId The endpoint id
         * @return void
         */	
        virtual void RemoveContextL( TUint32 aStreamId, 
                                     TUint32 aEndpointId ) = 0;
                                                        
    protected:  // Constructor

        /**
        * C++ constructor
        */
        inline CMccSubThreadClientBase( MMccEventHandler* aObserver,
                                        MMccResources* aMccResources,
                                        TInt aLinkType,
                                        TUint32 aSessionId ) :
                iDatasink( NULL ), 
                iDatasource( NULL ),
                iLinkType( aLinkType ), 
                iSessionId( aSessionId ),
                iObserver( aObserver ),
                iMccResources( aMccResources ),
                iSessionCreated( EFalse ), 
                iIsDTMF( EFalse )
                {   
                }
        
        
        /**
         * Helper functions for searching a stream with given
         * ID.
         */
        TInt FindStreamL( TUint32 aStreamId );
		TInt FindStream( TUint32 aStreamId );
        
    protected:  // Data

        CMccRtpManager* iRtpmanager;

        // Datasink of the subthread
        MDataSink* iDatasink;

        // Datasource of the subthread
        MDataSource* iDatasource;

        // Client observer
        MMccEventHandler* iObserver;
        
        MMccResources* iMccResources;

        // Priority settings
        TMMFPrioritySettings iPrioritySettings;
        
        // Flag for session creation
        TBool iSessionCreated;
        
        //Flag dtmf
        TBool iIsDTMF;
        
        /**
         * Link type
         */
        TInt iLinkType;
    
        // Id of this link
        TUint32 iLinkId;
        
        // Streams owned by the link
        RPointerArray<CMccSymStreamBase> iStreams;

        // MCC session id
        TUint32 iSessionId;
        
        #ifdef TEST_EUNIT
        friend class UT_CMccSymSubthreadClient;
        #endif
    };

#endif      // MCCSUBTHREADCLIENTBASE_H
            
// End of File
