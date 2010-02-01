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
* Description:    Provides an abstraction of the MMF Controller
*
*/




#ifndef __MCC_INTERFACE_H__
#define __MCC_INTERFACE_H__

//  INCLUDES
#include <e32base.h>
#include <mmf/common/mmffourcc.h>
#include <mmf/common/mmfstandardcustomcommands.h>
#include <MmccNetworkSettings.h>
#include <MmccInterfaceDef.h>

// CONSTANTS
#define K_Mcc_VERSION TVersion( 2, 0, 0 )

// FORWARD DECLARATIONS
class CMccCodecInformation;
class CMccEvent;
class CMccCodecInformation;
class CMccCodecAMR;
class CMccCodecG711;
class CMccCodecILBC;
class TCameraInfo;
class CMccControllerStub;

// CONSTANTS

const TUid KUidMccFileSink		= { 0x102747D7 };
const TUid KUidMccFileSource    = { 0x102747D5 };
const TUid KUidMccRtpSource   	= { 0x1020740B };
const TUid KUidMccRtpSink   	= { 0x1020740C };
const TUid KUidMccVideoSource   = { 0x102747D6 };
const TUid KUidMccVideoSink   	= { 0x102747D8 };

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// CLASS DECLARATION

/**
*  Mcc subsystem client interface.
*
*  @lib MccInterface.lib
*/
class CMccInterface : public CBase
    {
    public:  // Constructors and destructor
        /**
        * Two-phased constructor.
        * @param aObserver A callback object to receive and handle
        *                    event notifications from Mcc subsystem.
        * @return CMccInterface* Pointer to the interface object created
        */
        IMPORT_C static CMccInterface* NewL( MMccCtrlObserver& aObserver );

        /**
        * Destructor.
        */
        virtual ~CMccInterface();

public: // New functions

    /**
    * Creates a new MCC session. The session is needed to create
    * links and streams into MCC. A single session may have any
    * number of links.
    * @param aSessionID ID of the new session if successful
    * @return KErrNone if success; otherwise system wide error code.
    */
    IMPORT_C TInt CreateSession( TUint32& aSessionId );

    /**
    * Closes MCC session. All links and streams of the session are also
    * closed.
    * @param aSessionId session ID
    * @return One of the standard system-wide error codes.
    */
    IMPORT_C TInt CloseSession( TUint32 aSessionId );

    /**
    * Creates a new MCC link with a remote participant. Link creates
    * new RTP session with given network settings.
    * @param aSessionId session ID
    * @param aLinkType Defines is stream Up- or Downlink
    * @param aLinkId ID of new link
    * @param aNetSettings network settings for the link
    * @return KErrNone if success; otherwise system wide error code.
    */
    IMPORT_C TInt CreateLink( TUint32 aSessionId,
                              TInt aLinkType,
                              TUint32& aLinkId,
                              TMccNetSettings& aNetSettings );

    /**
    * Closes MCC link. All streams of the link are also closed.
    * @param aSessionId session ID
    * @param aLinkId link ID
    * @return One of the standard system-wide error codes.
    */
    IMPORT_C TInt CloseLink( TUint32 aSessionId, TUint32 aLinkId );

    /**
    * Creates a stream into a MCC link. Constructs the datapath
    * for stream, but no media resources reserved yet.
    * @param aSessionId session where stream is created to
    * @param aLinkId link ID where stream is created to
    * @param aStreamId Stream ID for the stream created.
    * @param aStreamType type of the stream
    * @param aCodecInformation Codec information for the stream.
    * @return KErrNone if success; otherwise system wide error code.
    */
    IMPORT_C TInt CreateStream( TUint32 aSessionId, TUint32 aLinkId,
                                TUint32& aStreamId, TInt aStreamType,
                                CMccCodecInformation& aCodecInformation );

    /**
    * Prepares a stream so that it is ready to be started. Media resources
    * are reserved and RTP stream is created.
    * @param aSessionId session where stream belongs to
    * @param aLinkId link ID where stream belongs to
    * @param aStreamId ID identifying stream to be prepared
    * @param aEndpointId ID identifying endpoint of the stream to be prepared.
    *		 If value 0, both endpoints prepared.
    * @return One of the standard system-wide error codes.
    */
    IMPORT_C TInt PrepareStream( TUint32 aSessionId, TUint32 aLinkId, 
                                 TUint32 aStreamId, TUint32 aEndpointId = 0 );

    /**
    * Starts to send/receive media stream in a given session.
    * @param aSessionId session where stream belongs to
    * @param aLinkId link ID
    * @param aStreamId ID identifying stream to be started
    * @param aPaused ETrue, if stream paused after start
    * @param aEndpointId ID identifying endpoint of the stream to be started.
    *		 If value 0, both endpoints started.
    * @param aEnableRTCP ETrue, if RTCP sent.
    * @return One of the standard system-wide error codes.
    */
    IMPORT_C TInt StartStream( TUint32 aSessionId, TUint32 aLinkId, 
                               TUint32 aStreamId, TUint32 aEndpointId = 0,
                               TBool aPaused = EFalse, TBool aEnableRTCP = ETrue );

    /**
    * Pauses media stream in a given session. No RTP packets are 
    * sent/received anymore. However, RTCP packets may still be sent and
    * received.
    * @param aSessionId session where stream belongs to
    * @param aLinkId link ID
    * @param aStreamId ID identifying stream to be paused
    * @param aEndpointId ID identifying endpoint of the stream to be paused.
    *		 If value 0, both endpoints paused.
    * @param aEnableRTCP ETrue, if RTCP sent while paused.
    * @return One of the standard system-wide error codes.
    */
    IMPORT_C TInt PauseStream( TUint32 aSessionId, TUint32 aLinkId, 
                               TUint32 aStreamId, TUint32 aEndpointId = 0,
                               TBool aEnableRTCP = ETrue );

    /**
    * Resumes paused media stream in a given session.
    * @param aSessionId session where stream belongs to
    * @param aLinkId link ID
    * @param aStreamId Stream ID identifying stream to be resumed
    * @param aEndpointId ID identifying endpoint of the stream to be resumed.
    *		 If value 0, both endpoints resumed.
    * @param aEnableRTCP ETrue, if RTCP sent while playing.
    * @return One of the standard system-wide error codes.
    */
    IMPORT_C TInt ResumeStream( TUint32 aSessionId, TUint32 aLinkId, 
                                TUint32 aStreamId, TUint32 aEndpointId = 0,
                                TBool aEnableRTCP = ETrue );

    /**
    * Stops sending/receiving media stream in a given session.
    * @param aSessionId session where stream belongs to
    * @param aLinkId link ID
    * @param aStreamId Stream ID identifying stream to be stopped
    * @param aEndpointId ID identifying endpoint of the stream to be stopped.
    *		 If value 0, both endpoints stopped.
    * @return One of the standard system-wide error codes.
    */
    IMPORT_C TInt StopStream( TUint32 aSessionId, TUint32 aLinkId, 
                               TUint32 aStreamId, TUint32 aEndpointId = 0 );

    /**
    * Deletes media stream in a given session.
    * @param aSessionId  session where stream belongs to
    * @param aLinkId link ID
    * @param aStreamId  Stream ID identifying stream to be deleted
    * @return One of the standard system-wide error codes.
    */
    IMPORT_C TInt DeleteStream( TUint32 aSessionId, TUint32 aLinkId, 
                                TUint32 aStreamId  );

    /**
    * Starts inactivity timer for a stream in a given session.
    * If RTP data is not received in given tive, inactivy
    * event is sent to client. In case of timeout, inactivity
    * timer is stopped. Client may restart the timer is needed.
    * @param aSessionId session where stream belongs to
    * @param aLinkId link ID
    * @param aStreamId Stream ID identifying stream
    * @param aTimeoutTime timeout value in microseconds
    * @param aEndpointId ID identifying endpoint of the stream to be monitored.
    * @return One of the standard system-wide error codes.
    */
    IMPORT_C TInt StartInactivityTimer( TUint32 aSessionId,
                                        TUint32 aLinkId, TUint32 aStreamId,
                                        TUint32 aTimeoutTime, TUint32 aEndpointId = 0 );

    /**
    * Stops inactivity timer for a stream in a given session.
    * @param aSessionId session where stream belongs to
    * @param aLinkId link ID
    * @param aStreamId Stream ID identifying stream
    * @param aEndpointId ID identifying endpoint of the stream where monitoring stopped.
    * @return One of the standard system-wide error codes.
    */
    IMPORT_C TInt StopInactivityTimer( TUint32 aSessionId, TUint32 aLinkId,
                                       TUint32 aStreamId, TUint32 aEndpointId = 0 );

    /**
    * Set remote IP address and port number for a given session.
    * @param aSessionId session ID
    * @param aLinkId link ID
    * @param aRemoteAddr Remote address to set
    * @param aRemoteRtcpPort Remote RTCP port to set, if not RTP port +1
    * @return One of the standard system-wide error codes.
    */
    IMPORT_C TInt SetRemoteAddress( TUint32 aSessionId, TUint32 aLinkId,
                                    const TInetAddr& aRemoteAddr, TUint aRemoteRtcpPort = 0 );


    /**
    * Gets current codec used in a given stream.
    * @param aSessionId session ID
    * @param aLinkId Link ID
    * @param aStreamId Stream ID
    * @return Current codec used with the specified stream, ownership is transferred.
    */
    IMPORT_C CMccCodecInformation* CodecL( TUint32 aSessionId,
                                           TUint32 aLinkId,
                                           TUint32 aStreamId );

    /**
    * Sets codec to be used in a given stream.
    * @param aSessionId session ID
    * @param aLinkId Link ID
    * @param aStreamId Stream ID
    * @param aCodec Codec to be used
    * @return One of the standard system-wide error codes.
    */
    IMPORT_C TInt SetCodec( TUint32 aSessionId, TUint32 aLinkId,
                            TUint32 aStreamId,
                            CMccCodecInformation& aCodecInformation );
    
    
    /**
    * Get supported codecs by the system. Certain sinks and sources
    * however may have limitations on which codecs they support, hence
    * all these codecs may not allowed in all streams.
    * @param aCodecs An array to be filled with FourCC codes
    * @return One of the standard system-wide error codes.
    */
    TInt GetSupportedCodecs( RArray<TFourCC>& aCodecs );
                            
    /**
    * Returns array of supported codecs in the system with their default values.
    * Note that data that the aCapabilities contains before calling this function 
    * will be lost during the excecution of this function. Note also that the client
    * is responsible for the destruction of the objects contained in the
    * parameter array.
    * @param aCapabilities Codec capabilities
    * @return KErrNone if successful
    */
    IMPORT_C TInt GetCapabilities(
            RPointerArray<CMccCodecInformation>& aCapabilities ) const;

    /**
    * Returns array of supported codecs of specific sink or source. Note that data that
    * the aCapabilities contains before calling this function will be
    * lost during the excecution of this function. Note also that the client
    * is responsible for the destruction of the objects contained in the
    * parameter array.
    * @param aType, type identifier of source or sink
    * @param aStreamType type of the stream
    * @param aParam, source or sink specific param
    * @param aCapabilities Codec capabilities
    * @return KErrNone if successful
    */
    IMPORT_C TInt GetCapabilities( const TUid aType, TInt aStreamType, const TDesC8& aParam,
            RPointerArray<CMccCodecInformation>& aCapabilities );
         
    /**
    * Get Synchronization source identifier.
    * @param aSessionId session ID
    * @param aLinkId Link ID
    * @param aStreamId Stream ID
    * @param aEndpointId ID identifying endpoint of the stream.
    * @param aSSRC SSRC value
    * @return One of the standard system-wide error codes.
    */   
    IMPORT_C TInt GetSSRC ( TUint32 aSessionId,
                            TUint32 aLinkId,
                            TUint32 aStreamId,
                            TUint32 aEndpointId,
                            TUint32& aSSRC );

    /**
    * Sends media signal (e.g. DTMF,RTCP) to uplink if possible.
    * @param aEvent Event to be sent
    * @return void
    */
    IMPORT_C void SendMediaSignalL( const TMccEvent& aEvent );
    
    /**
    * Adds new sink to the session. User must later
    * call CreateStream to connect this sink to some stream.
    * @param aSinkType type identifier of the sink.
    * @param aParam initialization parameters of the sink.
    * @param aEndpointId idenfier of the created sink on return.
    * @return KErrNotFound, if sink type not found
    */
    IMPORT_C TInt AddDataSink( const TUid aSinkType, const TDesC8& aParam, 
    						   TUint32& aEndpointId );

    /**
    * Adds new source to the session. User must later
    * call CreateStream to connect this source to some stream.
    * @param aSourceType type identifier of the source.
    * @param aParam initialization parameters of the source.
    * @param aEndpointId idenfier of the created source on return.
    * @return KErrNotFound, if source type not found
    */
    IMPORT_C TInt AddDataSource( const TUid aSourceType, const TDesC8& aParam,
    						     TUint32& aEndpointId );
    
    /**
    * Checks if link already exists based on local port
    * @param aSessionId session ID
    * @param aLinkId ID of the link
    * @param aNetSettings network settings for the link
    * @return ETrue if found; otherwise EFalse.
    */
    IMPORT_C TBool LinkExists( TUint32 aSessionId,
                               TUint32& aLinkId,
                               const TMccNetSettings& aNetSettings );

    /**
    * Checks if link contains streams
    * @param aSessionId session ID
    * @param aLinkId ID of the link
    * @return ETrue if found; otherwise EFalse.
    */
    IMPORT_C TBool StreamsExists( TUint32 aSessionId,
                                  TUint32 aLinkId );

    /** 
    * Gets sink or source parameter
    * @param aSessionId session where sink or source belongs to
    * @param aLinkId link ID where sink or source belongs to
    * @param aStreamId stream ID where sink or source belongs to
    * @param aEndpointId endpoint ID identifying sink or source.
    * @param aParam a parameter to get
    * @param aVal on return will contain parameter value in TPkgBuf structure
    * @return KErrNotFound if parameter was not found, KErrNone otherwise
    */
    IMPORT_C TInt GetParameter( TUint32 aSessionId, TUint32 aLinkId, 
                                TUint32 aStreamId, TUint32 aEndpointId,
                                TUint32 aParam, TDes8& aVal );

    /** 
    * Sets sink or source parameter
    * @param aSessionId session where sink or source belongs to
    * @param aLinkId link ID where sink or source belongs to
    * @param aStreamId stream ID where sink or source belongs to
    * @param aEndpointId endpoint ID identifying sink or source.
    * @param aParam a parameter to get
    * @param aVal on return will contain parameter value in TPkgBuf structure
    * @return KErrNotFound if parameter was not found, KErrNone otherwise
    */
    IMPORT_C TInt SetParameter( TUint32 aSessionId, TUint32 aLinkId, 
                                TUint32 aStreamId, TUint32 aEndpointId,
                                TUint32 aParam, const TDesC8& aVal );

    /**
    * Updates sink of the stream.
    * @param aSessionId session where stream belongs to
    * @param aLinkId link ID
    * @param aStreamId ID identifying stream to be updated
    * @param aEndpointId ID identifying endpoint of the stream to be updated.
    * @param aParam initialization parameters of the sink.
    * @return KErrNotFound, if sink type not found
    */
    IMPORT_C TInt UpdateDataSink( TUint32 aSessionId, TUint32 aLinkId, 
                               TUint32 aStreamId, TUint32 aEndpointId,
                               const TDesC8& aParam );

    /**
    * Updates source of the stream. 
    * @param aSessionId session where stream belongs to
    * @param aLinkId link ID
    * @param aStreamId ID identifying stream to be updated
    * @param aEndpointId ID identifying endpoint of the stream to be updated.
    * @param aParam initialization parameters of the source.
    * @return KErrNotFound, if source type not found
    */
    IMPORT_C TInt UpdateDataSource( TUint32 aSessionId, TUint32 aLinkId, 
                                 TUint32 aStreamId, TUint32 aEndpointId,
                                 const TDesC8& aParam );

    /**
    * Removes sink from MCC.
    * @param aSessionId session where stream belongs to
    * @param aLinkId link ID
    * @param aStreamId ID identifying stream to be updated
    * @param aEndpointId ID identifying endpoint of the stream to be updated.
    * @return KErrNotFound, if sink type not found
    */
    IMPORT_C TInt RemoveDataSink( TUint32 aSessionId, TUint32 aLinkId, 
                                  TUint32 aStreamId, TUint32 aEndpointId );

    /**
    * Removes source from MCC. 
    * @param aSessionId session where stream belongs to
    * @param aLinkId link ID
    * @param aStreamId ID identifying stream to be updated
    * @param aEndpointId ID identifying endpoint of the stream to be updated.
    * @return KErrNotFound, if source type not found
    */
    IMPORT_C TInt RemoveDataSource( TUint32 aSessionId, TUint32 aLinkId, 
                                    TUint32 aStreamId, TUint32 aEndpointId );

    /**
    * Reuses sink or source of the stream. Reused sink or source may be
    * simultaneously shared by two or more stream. It is also possible,that only one stream 
    * can use it at a time. User must later call CreateStream to connect this sink or source
    * to some stream.
    * @param aSessionId session where stream belongs to
    * @param aLinkId link ID
    * @param aStreamId ID identifying stream to be updated
    * @param aEndpointId ID identifying endpoint of the stream to be reused.
    * @return KErrNotFound, if endpoint not found
    */
    IMPORT_C TInt Reuse( TUint32 aSessionId, TUint32 aLinkId, 
                         TUint32 aStreamId, TUint32 aEndpointId );

    /**
    * Get the reference count of given sink or source. 
    * @param aSessionId session where stream belongs to
    * @param aLinkId link ID
    * @param aStreamId ID identifying stream to be updated
    * @param aEndpointId ID identifying endpoint of the stream to be updated.
    * @param aCount number of references using this sink or source.
    * @return KErrNotFound, if endpoint not found
    */
    IMPORT_C TInt ReferenceCount( TUint32 aSessionId, TUint32 aLinkId, 
                                  TUint32 aStreamId, TUint32 aEndpointId,
                                  TUint& aCount );

    /**
    * Get the reference count of given sink or source. 
    * @param aSessionId session where stream belongs to
    * @param aBundle new bundle to be added in the session.
    * @param aBundleId identifier of the added bundle on return.
    * @return KErrNone if successful
    */
    IMPORT_C TInt AddBundle( TUint32 aSessionId, TMccStreamBundle& aBundle, TUint32& aBundleId );

    /**
    * Get the reference count of given sink or source. 
    * @param aSessionId session where stream belongs to
    * @param aBundleId identifier of the removed bundle.
    * @return KErrNotFound, if bundle not found
    */
    IMPORT_C TInt RemoveBundle( TUint32 aSessionId, TUint32 aBundleId );

    /**
    * Get the reference count of given sink or source. 
    * @param aSessionId session where stream belongs to
    * @param aBundles an array of bundles in given session.
    * @return KErrNone if successful
    */
    IMPORT_C TInt GetBundles( TUint32 aSessionId, RArray<TMccStreamBundle>& aBundles);

    /**
    * Transcodes media file in non-realtime. Progress of transcoding
    * can be monitored through MMccCtrlObserver::MccEventReceived.
    * @param aSourceFile source file for transcoding
    * @param aDestFile destination file for transcoding
    * @param aQuality requested quality for transcoding
    * @param aSessionId session identifier for transcoding
    * @param aVideoCodec requested video parameters for transcoding, overrides aQuality parameter.
    *		 Ownership is not transferred.	
    * @param aAudioCodec requested audio parameters for transcoding, overrides aQuality parameter.
    *		 Ownership is not transferred.	
    * @return ETrue if succesfull; otherwise EFalse.
    */
    IMPORT_C TInt TranscodeFile( const TDesC8& aSourceFile, const TDesC8& aDestFile,
    							 TUint32 aQuality, TUint32& aSessionId,  
    							 const CMccCodecInformation* aVideoCodec = 0,
    							 const CMccCodecInformation* aAudioCodec = 0 );

    /**
    * Cancels transcoding of media file.
    * @param aSessionId session identifier for transcoding
    * @return ETrue if succesfull; otherwise EFalse.
    */
    IMPORT_C TInt CancelTranscodeFile( TUint32 aSessionId );
                                     

    private:

        /**
        * C++ default constructor.
        */
        CMccInterface();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL( MMccCtrlObserver& aObserver );
   
   private:   

       /**
        * Open controller.
        * @since Series 60 3.0
        * @param aControllerUid Controller uid to open.
        * @return One of the standard system-wide error codes.
        */
        TInt OpenController( const TUid& aControllerUid );
        
        /**
        * Gets supported codecs from controller.
        * @since Series 60 3.0
        * @return One of the standard system-wide error codes.
        */
        void CapabilitiesL();

        /**
        * Gets supported bit rates from the codec of the specified stream
        * @since Series 60 3.0
        * @return One of the standard system-wide error codes.
        */
        TInt GetSupportedBitrates( TUint32 aSessionId,
                                   TUint32 aLinkId,
                                   TUint32 aStreamId,
                                   RArray<TUint>& aBitrates );

    private:    // Data
        // Reserved for future extensions
        TAny* iReserved1;
        TInt  iReserved2;    
    
    private:    // Data

        // Notifies about events happened in Mcc plugin
        //CMccEvent* iMccEvent;

        // Handle to the controller plugin for sending custom commands
        CMccControllerStub* iController;

        // Contains unique ID of controller interface
        //TMMFMessageDestinationPckg iMessageDest;

        // For easier access to Audio Play Device functionality
        //RMMFAudioPlayDeviceCustomCommands iAudioPlayDeviceCustomCommands;

        // For easier access to Audio Record Device functionality
        //RMMFAudioRecordDeviceCustomCommands iAudioRecordDeviceCustomCommands;
    
        // Array for codec capabilities
        RPointerArray<CMccCodecInformation> iCodecInformation;    
    
    private:
        #ifdef TEST_EUNIT
        friend class UT_CMccInterface;
        #endif
    };

#endif /* __Mcc_INTERFACE_H__ */

// End of File
