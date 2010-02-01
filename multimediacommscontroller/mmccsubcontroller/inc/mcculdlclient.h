/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Provides interface for UL/DL subthread clients
*
*/




#ifndef MCCULDLCLIENT_H
#define MCCULDLCLIENT_H

//  INCLUDES
#include <mmf/common/mmfbase.h>
#include <mmf/common/mmfutilities.h>
#include "rtpapi.h"
#include "rtpdef.h"
#include "mmccinterfacedef.h"
#include "mccinternaldef.h"

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class CMccSubThreadClientBase;
class CMccUlSubThreadClient;
class CMccDlSubThreadClient;
class MDataSource;
class MDataSink;
class CMccDTMFClient;
class CMccSymSubthreadClient;
class MMccEventHandler;
class TMccCodecInfo;
class TMccEvent;
class MMccResources;
class CMccRtpMediaClock;
class TMccCryptoContext;

// CLASS DECLARATION

/**
* Mcc Uplink/Downlink client interface
*
* @lib Mccsubthreads.lib
* @since Series 60 3.0
*/
class CMccUlDlClient : public CBase, public MMccEventHandler
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        IMPORT_C static CMccUlDlClient* NewL( MMccEventHandler* aMccEventHandler,
                                              MMccResources* aMccResources,
                                              TUint32 aSessionId );
        /**
        * Destructor.
        */
        IMPORT_C virtual ~CMccUlDlClient();        
	                               
    public: // New functions
        
        
		IMPORT_C static void GetCodecDefaultsL( const TUid aType,
												TDes8& aParam,
												TMccCodecInfo& aCodec );
												
        /**
        * Gets the gain of current stream
        * @since Series 60 3.0
        * @param [input] aLinkId Link location in the array
        * @param [input] aStreamId ID of the stream
        * @param [output] aGain Return value of the current gain
        * @return TInt System wide error code   
        */
        IMPORT_C TInt GetGainL( const TUint32 aLinkId,
                                const TUint32 aStreamId,
                                TInt& aGain );
        /**
        * Sets the gain for ul client
        * @since Series 60 3.0
        * @param [input] aGain The wanted gain value 
        * @return TInt System wide error code
        */
        IMPORT_C TInt SetGainL( TInt aGain );
        /**
        * Returns the maximum gain of current stream
        * @since Series 60 3.0
        * @param [input] aLinkId Link location in the array
        * @param [input] aStreamId ID of the stream
        * @param [output] aMaxGain Value of the maximum gain
        * @return System wide error code   
        */
        IMPORT_C void MaxGainL( const TUint32 aLinkId,
        						const TUint32 aStreamId,
                                TInt& aMaxGain );
        
        /**
        * Sets the volume for current stream
        * @since Series 60 3.0
        * @param [input] aVolume Value for the current volume
        * @return System wide error code   
        */
        IMPORT_C TInt SetVolumeL( TInt aVolume );

        /**
        * Returns the maximum volume for current stream
        * @since Series 60 3.0
        * @param [input] aLinkId Link location in the array
        * @param [input] aStreamId ID of the stream
        * @param [output] aMaxVolume Return value of the maximum volume
        * @return TInt System wide error code   
        */
        IMPORT_C void MaxVolumeL( const TUint32 aLinkId,
                				  const TUint32 aStreamId,
                                  TInt& aMaxVolume );                         
        /**
        * Returns the used codec
        * @since Series 60 3.0
        * @param [input] aLinkId Link location in the array
        * @param [input] aStreamId ID of the stream
        * @param [output] aCodec Codec information struct containing codec info
        * @return void   
        */
        IMPORT_C void GetCodecL( const TUint32 aLinkId,
                                 const TUint32 aStreamId,
                                 TMccCodecInfo& aCodec );

        /**
        * Returns the fmtp 
        * @since Series 60 3.0
        * @param [input] aLinkId Link location in the array
        * @param [input] aStreamId ID of the stream
        * @return the fmtp string for the link
        */
        IMPORT_C void GetFmtpAttrL( const TUint32 aLinkId,
                                    const TUint32 aStreamId,
                                    TDes8& aFmtp );
        
        /**
        * Sets the stream priority
        * @since Series 60 3.0
        * @param [input] aLinkId Link location in the array
        * @param [input] aStreamId ID of the stream
        * @param [input] aPrioritySettings An object that holds the
        *                audio client's priority and device state settings.
        * @return TInt System wide error code
        */
        IMPORT_C TInt SetPriorityL( const TUint32 aLinkId,
                                    const TUint32 aStreamId,
                                    TMMFPrioritySettings aPrioritySettings );
        /**
        * Opens the ul or dl client subthread
        * @since Series 60 3.0
        * @param [input] aLinkId Link ID
        * @param [input] aStreamId unique id for the stream
        * @param [input] aStreamType The desired stream type
        * @param [input] aFourCC the fourCC code of the desired codec
        * @param [input] aDataSource Data source of the client
        * @param [input] aSink Data sink of the client
        * @param [input] aSettings MMF settings
        * @leave System-wide error code if unsuccessful
        * @return void
        */
        IMPORT_C void OpenL( const TUint32 aLinkIdId,
                             const TUint32 aStreamId, 
                             TInt aStreamType,
                             TFourCC aFourCC,
                             MDataSource* aDataSource,
                             MDataSink* aSink,
                             TMMFPrioritySettings aSettings );
                
        /**
        * Prepares ul/dl streams
        * @since Series 60 3.0
        * @param [input] aLinkId Link location in the array
        * @param [input] aStreamId ID of the stream
        * @leave System wide error code if unsuccessful
        * @return None
        */
        IMPORT_C void PrepareL( const TUint32 aLinkId,
                                const TUint32 aStreamId,
                                const TUint32 aEndpointId );

        /**
        * Plays ul/dl streams
        * @since Series 60 3.0
        * @param [input] aLinkId Link location in the array
        * @param [input] aStreamId ID of the stream
        * @leave System wide error code if unsuccessful
        * @return None
        */
        IMPORT_C void PlayL( const TUint32 aLinkId,
                             const TUint32 aStreamId,
							 const TUint32 aEndpointId,
                             const TBool aStreamPaused,
                             const TBool aEnableRTCP );

        /**
        * Pauses ul/dl streams
        * @since Series 60 3.0
        * @param [input] aLinkId Link location in the array
        * @param [input] aStreamId ID of the stream
        * @param [input] aEnableRTCP ETrue if automatic RTCP sending should be
        *   enabled when the stream is paused
        * @leave System wide error code if unsuccessful
        * @return None
        */
        IMPORT_C void PauseL( const TUint32 aLinkId,
                              const TUint32 aStreamId,
                              const TUint32 aEndpointId,
                              const TBool aEnableRTCP );

        /**
        * Stops ul/dl streams
        * @since Series 60 3.0 
        * @param [input] aLinkId Link location in the array
        * @param [input] aStreamId ID of the stream
        * @return TInt System wide error code
        */
        IMPORT_C void StopL( const TUint32 aLinkId,
                             const TUint32 aStreamId,
                             const TUint32 aEndpointId );

        /**
        * Resumes ul/dl streams
        * @since Series 60 3.0 
        * @param [input] aLinkId Link location in the array
        * @param [input] aStreamId ID of the stream
        * @param [input] aEnableRTCP ETrue if automatic RTCP sending should be
        *   enabled when the stream is paused
        * @leave System wide error code if unsuccessful
        * @return None
        */
        IMPORT_C void ResumeL( const TUint32 aLinkId,
                               const TUint32 aStreamId,
                               const TUint32 aEndpointId,
                               const TBool aEnableRTCP );

        /**
        * Checks if link contais stream
        * @since Series 60 3.0 
        * @param [input] aLinkId Link location in the array
        * @leave System wide error code if unsuccessful
        * @return ETrue, if streams exists
        */
        IMPORT_C TBool StreamsExistsL( const TUint32 aLinkId );

        /**
        * Returns the play balance
        * @since Series 60 3.0 
        * @param [input] aLinkId Link location in the array
        * @param [output] aLeftBalance Value for the left balance
        * @param [output] aRightBalance Value for the right balance
        * @return System wide error code
        */
        IMPORT_C TInt GetPlayBalanceL( const TUint32 aLinkId,
                                       const TUint32 aStreamId,
                                       TInt& aLeftBalance,
                                       TInt& aRightBalance );

        /**
        * Returns the record balance
        * @since Series 60 3.0 
        * @param [input] aLinkId Link location in the array
        * @param [input] aStreamId ID of the stream
        * @param [output] aLeftBalance Value for the left balance
        * @param [output] aRightBalance Value for the right balance
        * @return System wide error code
        */
        IMPORT_C TInt GetRecordBalanceL( const TUint32 aLinkId,
                                         const TUint32 aStreamId,
                                         TInt& aLeftBalance,
                                         TInt& aRightBalance );

        /**
        * Sets the record balance
        * @since Series 60 3.0 
        * @param [input] aLinkId Link location in the array
        * @param [input] aStreamId ID of the stream
        * @param [input] aLeftBalance Value for the left balance
        * @param [input] aRightBalance Value for the right balance
        * @leave System wide error code
        */
        IMPORT_C void SetRecordBalanceL( const TUint32 aLinkId, 
                                         const TUint32 aStreamId, 
                                         TInt aLeftBalance,
                                         TInt aRightBalance );

        /**
        * Sets the play balance
        * @since Series 60 3.0 
        * @param [input] aLinkId Link location in the array
        * @param [input] aStreamId ID of the stream
        * @param [input] aLeftBalance Value for the left balance
        * @param [input] aRightBalance Value for the right balance
        * @leave System wide error code
        */
        IMPORT_C void SetPlayBalanceL( const TUint32 aLinkId, 
                                       const TUint32 aStreamId, 
                                       TInt aLeftBalance, 
                                       TInt aRightBalance );  
        
        /**
        * Closes ul/dl streams
        * @since Series 60 3.0 
        * @param [input] aLinkId ID of link that contains stream
        * @param [input] aStreamId ID of stream to close
        * @return void
        */
        IMPORT_C void CloseL( const TUint32 aLinkIdId,
                              const TUint32 aStreamId );
      
        /**
        * Returns the session ID 
        * @since Series 60 3.0
        * @param None
        * @return Value which represents the session ID
        */
        IMPORT_C TUint32 GetSessionId();

        /**
        * Sets the remote address for aStream
        * @since Series 60 3.0
        * @param [input] aRemAddr Denotes the remote IP address
        * @param [input] aLinkId Link location in the array
        * @leave System wide error code if unsuccessful
        * @return None
        */
        IMPORT_C void SetRemoteAddressL( TInetAddr aRemAddr,
                                         const TUint32 aLinkId );
                                         
        /**
        * Sets the remote rtcp address for aStream
        * @since Series 60 3.0
        * @param [input] aRemAddr Denotes the remote rtcp IP address
        * @param [input] aLinkId Link location in the array
        * @leave System wide error code if unsuccessful
        * @return None
        */
        IMPORT_C void SetRemoteRtcpAddrL( TInetAddr aRemAddr,
                                         const TUint32 aLinkId );                                

        /**
        * Creates link
        * @since Series 60 3.0
        * @param [input] aLinkId ID of the new link (UL or DL client)
        * @param [input] aType   Type of the link to be created
        * @return void
        */
        IMPORT_C void CreateLinkL( TUint32& aLinkId,
                                   TInt aType );

        /**
        * Initializes link
        * @since Series 60 3.0
        * @param [input/output] aStatus Status of the request
        * @param [input] aLinkId ID of the link
        * @param [input] aIapId IAP ID
        * @return void
        */
        IMPORT_C void InitializeLinkL( TRequestStatus& aStatus,
                                       TUint32 aLinkId,
                                       TInt aIapId );

        /**
        * Creates the RTP session
        * @since Series 60 3.0
        * @param [input] aLinkId
        * @param [input/output] aPort    
        * @param [input] aIpTOS           
        * @param [input] aMediaSignaling 
        * @return void
        */
        IMPORT_C void CreateRtpSessionL( TUint32 aLinkId,
                                         TUint aPort,
                                         TInt aIpTOS,
                                         TInt aMediaSignaling );

        /**
        * Closes ul/dl links
        * @since Series 60 3.0 
        * @param [input] aLinkId ID of link to close
        * @return System wide error code
        */
        IMPORT_C TInt CloseLinkL( TUint32 aLinkIdId );

                                      
        /**
        * Sends media signals to uplink if possible
        * @since Series 60 3.0
        * @param aStreamId Stream into which the signal should be sent
        * @param aEvent The event to be sent
        * @return void
        */
        IMPORT_C void SendMediaSignalL( const TMccEvent& aEvent );
        
        /**
        * Sets the codec information for specific stream
        * @since Series 60 3.0
        * @param [input] aLink Link for which the codec information is set
        * @param [input] aStreamId ID of the stream
        * @param aCodecInfo Codec information for the stream
        * @return void
        */
        IMPORT_C void SetCodecInformationL( const TUint32 aLinkId, 
                const TUint32 aStreamId, 
                const TMccCodecInfo& aCodecInfo,
                const TDesC8& aFmtp );
                
        /**
        * Get the supported bitrates from the codec of the specified stream
        * @since Series 60 3.0
        * @param aLinkId - [input] link ID
        * @param aStreamId - [input] Stream ID identifying stream
        * @param aBitrates Array for the bitrates
        * @leave System wide error code if unsuccessful
        * @return None
        */
        IMPORT_C void GetSupportedBitratesL( const TUint32 aLinkIdId, 
                                             const TUint32 aStreamId,
                                             RArray<TUint>& aBitrates );
        
       /**
        * Get syncrinization source from the specified stream
        * @since Series 60 3.0
        * @param aLinkId - [input] link ID
        * @param aStreamId - [input] Stream ID identifying stream
        * @param aaSSRCValue - [input] SSRC value to be retturn
        * @leave System wide error code if unsuccessful
        * @return none.
        */
        IMPORT_C void GetSSRCL( const TUint32 aLinkId, 
                                const TUint32 aStreamId, 
                                TUint32&      aSSRCValue);
        
        /**
        * Sends RTCP receiver report to uplink if possible.
        * @param aLinkId - [input] link ID
        * @param aStreamId - [input] Stream ID identifying stream
        * @leave System wide error code if unsuccessful
        * @return None
        */
        IMPORT_C void SendRTCPReceiverReportL( const TUint32 aLinkIdId, 
                                               const TUint32 aStreamId );

        /**
        * Sends RTCP sender report to uplink if possible.
        * @param aLinkId - [input] link ID
        * @param aStreamId - [input] Stream ID identifying stream
        * @leave System wide error code if unsuccessful
        * @return None
        */
        IMPORT_C void SendRTCPSenderReportL( const TUint32 aLinkIdId,
                                             const TUint32 aStreamId );


        /**
        * Sends RTCP data to uplink if possible.
        * @param aLinkId - [input] link ID
        * @param aStreamId - [input] Stream ID identifying stream
        * @param aData - [input] Non-RTCP data
        * @leave System wide error code if unsuccessful
        * @return None
        */
        IMPORT_C void SendRTCPDataL( const TUint32 aLinkId,
                                     const TUint32 aStreamId,
                                     const TDesC8& aData );
        /**
        * Starts inactivity timer for a stream in a given session.
        * @param aLinkId - [input] link ID
        * @param aStreamId - [input] Stream ID identifying stream
        * @param aTimeoutTime - [input] timeout value in milliseconds
        * @leave System wide error code if unsuccessful
        * @return None
        */
        IMPORT_C void StartInactivityTimerL( const TUint32 aLinkIdId, 
                                             const TUint32 aStreamId,
                                             TUint32 aTimeoutTime );

        /**
        * Stops inactivity timer for a stream in a given session.
        * @param aLinkId - [input] link ID
        * @param aStreamId - [input] Stream ID identifying stream
        * @leave System wide error code if unsuccessful
        * @return None
        */
        IMPORT_C void StopInactivityTimerL( const TUint32 aLinkIdId,
                                            const TUint32 aStreamId );
                                      	  
        /**
        * Sets audio route for a stream in a given session.
        * @param aLinkId - [input] link ID
        * @param aStreamId - [input] Stream ID identifying stream
        * @param aRoutingDestination - [input] routing destination
        * @leave System wide error code if unsuccessful
        * @return None
        */
        IMPORT_C void SetAudioRouteL( TUint32 aLinkId, 
                                      TUint32 aStreamId,
                                      TUint32 aRoutingDestination );
        
        
        /**
        * Sets audio route for a stream in a given session.
        * @param aLinkId - [input] link ID
        * @param aStreamId - [input] Stream ID identifying stream
        * @param aRoutingDestination - [output] routing destination
        * @leave System wide error code if unsuccessful
        * @return None
        */
        IMPORT_C void GetAudioRouteL( TUint32 aLinkId, 
                                      TUint32 aStreamId,
                                      TUint32& aRoutingDestination );
                                      
        /**
        * Generates a random stream identifier
        * @since Series 60 3.0
        * @param None
        * @return TInt New stream ID
        */
        IMPORT_C TUint32 GenerateStreamId();
        
        /**
        * Unuses ul/dl stream
        * @since Series 60 3.0 
        * @param [input] aLinkId Link location in the array
        * @param [input] aStreamId ID of the stream
        * @return TInt System wide error code
        */
        IMPORT_C void UnuseL( const TUint32 aLinkId,
                              const TUint32 aStreamId );

        /**
        * Resolves local IP addresses and ports
        * @since Series 60 3.2
        * @param [input/output] aClientData Client data
        * @return void
        */
        IMPORT_C void GetLocalIpAddressesL( TMccCreateLink& aClientData );


        /**
         * Bind context into stream.
         * @since Series 60 3.2
         * @param aLinkId The link id
         * @param aStreamId The stream id
         * @param aEndpointId The endpoint id
         * @param aCryptoContext Crypto context
         * @return void
         */	
        IMPORT_C void BindContextIntoStreamL(  TUint32 aLinkId,
                                               TUint32 aStreamId,
                                               TUint32 aEndpointId,
                                               const TMccCryptoContext& aCryptoContext );

        /**
         * Remove context.
         * @since Series 60 3.2
         * @param aLinkId The link id
         * @param aStreamId The stream id
         * @param aEndpointId The endpoint id
         * @return void
         */	
        IMPORT_C void RemoveContextL( TUint32 aLinkId,
                                      TUint32 aStreamId,
                                      TUint32 aEndpointId );

        /**
        * Generic value setter.
        * @param [input] aParam identifier of the param to be set
        * @param [input] aLinkId Link location in the array
        * @param [input] aStreamId ID of the stream
        * @param [input] aEndpointId ID of the endpoint
        * @param [input] aVal value to be set
        */                      
        IMPORT_C void SetParameterL( TUint32 aParam,
                                     TUint32 aLinkId,
                                     TUint32 aStreamId,
                                     TUint32 aEndpointId,
                                     const TDesC8& aVal );
         
        /**
        * Generic value getter.
        * @param [input] aParam identifier of the param to be get
        * @param [input] aLinkId Link location in the array
        * @param [input] aStreamId ID of the stream
        * @param [input] aEndpointId ID of the endpoint
        * @param [input] aVal value fill be set to this
        */                      
        IMPORT_C void GetParameterL( TUint32 aParam,
                                     TUint32 aLinkId,
                                     TUint32 aStreamId,
                                     TUint32 aEndpointId,
                                     TDesC8& aVal );
                                                                  
    public: // From MMccEventHandler
        
        virtual TInt SendMccEventToClient( TMccEvent& aEvent );

	    virtual void StateChange( TInt aState, TUint32 aLinkId );

	    virtual void ErrorOccured( TInt aError, 
	                               TUint32 aSessionId, 
	                               TUint32 aLinkId,
	                               TUint32 aStreamId, 
	                               TUint32 aEndpointId );

    protected:  // New functions
       

    protected:  // Functions from base classes
        
    private:    // New functions
    
        /**
        * Method for finding correct streams
        * @since Series 60 3.0 
        * @param [input] aLinkId Link location in the array
        * @return TInt System wide error code
        */
        TInt FindLinkL( const TUint32 aLinkId );

        /**
        * Stores the link (pointer to clients)
        * @since Series 60 3.0 
        * @param [input] aClient client object
        * @return TInt System wide error code 
        */
        TInt StoreLink( CMccSubThreadClientBase* aClient );
        
    private:    // Constructors

        /**
        * C++ default constructor.
        */
        CMccUlDlClient( MMccEventHandler* aMccEventHandler,
                        MMccResources* aMccResources,
                        TUint32 aSessionId );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    public:     // Data
        
    protected:  // Data

    private:    // Data    
    
        // array for storing ul & dl client pointers to streams 
        RPointerArray<CMccSubThreadClientBase> iClientArray;

        // member variable for storing session id
        TUint32 iSessionId;

        // Variable for storing event error code
        TInt iErrorCode;

        // Datasink of the subthread
        MDataSink* iDatasink;

        // Datasource of the subthread
        MDataSource* iDatasource;
        
        // Event handler
        MMccEventHandler* iMccEventHandler;
        
        // Resources
        MMccResources* iMccResources;
        
        // Rtp media clock
        CMccRtpMediaClock* iRtpMediaClock;
                
    public:     // Friend classes
       
    protected:  // Friend classes
       
    private:    // Friend classes
       

    };

#endif      // MCCULDLCLIENT_H   
            
// End of File



