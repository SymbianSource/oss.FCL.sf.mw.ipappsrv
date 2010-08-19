/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    RTP Datasource
*
*/




#ifndef MCCRTPDATASOURCE_H
#define MCCRTPDATASOURCE_H

//  INCLUDES
#include "rtpheader.h"
#include "mccdatasource.h"
#include "mccrtpinterface.h"
#include "mccdef.h"
#include "mccjittercalculator.h"

// CONSTANTS

// UIDs 

// MACROS

// DATA TYPES
    
// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class MDataSink;
class CMccTimerManager;

// CLASS DECLARATION


/**
*  ECom-plugin which receives and forwards RTP/RTCP packets.
*
*  @lib      MccRtpSourceSink.dll
*  @since    Series 60 3.0
*/
class CMccRtpDataSource : public CMccDataSource,
                          public MMccRtpInterface,
                          public MRtpObserver,
                          public MRtpJitterObserver,
                          public MMccExpirationHandler
    {

    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static MDataSource* NewSourceL( TUid aImplementationUid, 
                                        const TDesC8& aInitData );
        
        /**
        * Destructor.
        */
        virtual ~CMccRtpDataSource();

    public: // New functions
        
        /**
         * Registers payload types, which should pass RTP-source validation.
         *
         * @since    Series 60 3.2
         * @param    aPayloadTypes  Payload types to accept in validation
         * @return   void
         */
        virtual void RegisterPayloadTypesL( const RArray<TUint>& aPayloadTypes );
        
        /**
         * Unregisters payload types to be accepted.
         *
         * @since    Series 60 3.2
         * @param    aPayloadTypes      Payload types to unregister
         * @return   void
         */
        virtual void UnRegisterPayloadTypes( const RArray<TUint>& aPayloadTypes );
    
    public: // From MRtpObserver

        /**
        * From MRtpObserver.
        */
        void RtpPacketReceived( TRtpId aStreamId, 
                                const TRtpRecvHeader& aHeaderInfo, 
                                const TDesC8& aPayloadData );
    
    public: // From MMccRtpInterface
        
        /**
        * Starts inactivity timer for a stream in a given session.
        * @param aTimeoutTime - [input] timeout value in milliseconds
        * @return One of the standard system-wide error codes.
        */
        void StartInactivityTimerL( TUint32 aTimeoutTime );

        /**
        * Stops inactivity timer for a stream in a given session.
        * @return One of the standard system-wide error codes.
        */
        void StopInactivityTimerL( );

        TInt StandBy( TMccStandbyActionType aActionType, TUint aPayloadType );
        
        void SendMediaSignallingL( const TMccEvent& aEvent );
        
        TRtpId RtpStreamId();
        
        void DoCreateSrtpStreamL();
        
    public: // From MDataSource

        /**
        * From MDataSource. Returns the data type as a fourCC code of the data source.
        *
        * @since    Series 60 3.0
        * @param    "aMediaId"
        *           This identifies the type of media eg audio or video and the stream id.
        *           Only KUidMediaTypeAudio -mediatype is supported.
        * @return    The fourCC code identifying the source datatype for the specified aMediaId.
        */
        TFourCC SourceDataTypeCode( TMediaId /*aMediaId*/ );

        /**
        * From MDataSource. Sets the data type as a fourCC code for the data source
        *
        * @since    Series 60 3.0
        * @param    "aSourceFourCC"
        *            This specifies the data type as a fourCC code to set the source to.
        *
        * @param    "aMediaId"
        *            This identifies the type of media eg audio or video and the stream id.
        *           Only KUidMediaTypeAudio -mediatype is supported.
        *
        * @return    KErrNone if successful, KErrNotSupported if the source does not support having
        *            it's data type set, otherwise a system wide error code.
        */
        TInt SetSourceDataTypeCode( TFourCC aCodec, 
                                    TMediaId aMedia );

        /**
        * From MDataSource. Not used. Leaves with KErrNotSupported.
        * 
        * @since    Series 60 3.0
        * @param    "aBuffer"
        *            The buffer that has been emptied by a data sink and is now available for reuse
        * @return   None
        */
        void BufferEmptiedL( CMMFBuffer* aBuffer );
        
        /**
        * From MDataSource. Method to indicate whether the data source can create a buffer.
        *
        * @since    Series 60 3.0
        * 
        * @return    EFalse
        */
        TBool CanCreateSourceBuffer();
        
        /**
        * From MDataSource. Not used. Leaves with KErrNotSupported.
        *
        * @since    Series 60 3.0
        * @param    "aMediaId"
        *           This identifies the type of media eg audio or video and the stream id.
        *           This parameter is required in cases where the source can supply data
        *           of more than one media type and/or multiple strams of data eg a multimedia file.
        *
        * @param    "aReference"
        *           This must be written to by the method to indicate whether the created buffer is
        *           a 'reference' buffer.  A 'reference' buffer is a buffer that is owned by the source
        *           and should be used in preference to the sink buffer provided the sink buffer
        *           is also not a reference buffer.
        * .  
        * @return   The created buffer
        */
        CMMFBuffer* CreateSourceBufferL( TMediaId, 
                                         TBool& aReference );
        
        /**
        * From MDataSource. Method to 'logon' the data source to the same thread that source will be supplying data in.
        *
        * Jitterbuffer for incoming packets and downlink timeout timer are created here.
        * 
        * @param    "aEventHandler"
        *           This is an MAsyncEventHandler to handle asynchronous events that occur during the
        *           transfer of multimedia data.  The event handler must be in the same thread as the data transfer
        *           thread - hence the reason it is passed in the SourceThreadLogon as opposed to say the constructor.
        *
        *
        * @return   KErrNone if successful, otherwise a system wide error code.
        */
        TInt SourceThreadLogon( MAsyncEventHandler& aEventHandler );
        
        /**
        * From MDataSource. Method to 'logoff' the data source from the same thread that source supplies data in.
        *
        * Jitterbuffer for incoming packets and downlink timeout timer are deleted. Datasource deletes the 
        * stream(s) and unregisters itself as an observer for the RTP session for incoming RTP and RTCP packets.
        *
        * @return None
        */
        void SourceThreadLogoff();

        /**
        * From MDataSource. Method to 'prime' the data source.
        *
        * Datasource is set to primed state.
        *
        * @since    Series 60 3.0
        * @return   None
        */
        void SourcePrimeL();
        
        /**
        * From MDataSource. Method to 'play' the data source.
        *
        * Downlink timeout timer is started and datasource is set to playing state.
        *
        * @since    Series 60 3.0
        * @return   None
        */
        void SourcePlayL();
        
        /**
        * From MDataSource. Method to 'pause' the data source.
        *
        * Downlink timeout timer is stopped and datasource is set to paused state.
        *
        * @since    Series 60 3.0
        * @return   None
        */
        void SourcePauseL();
        
        /**
        * From MDataSource. Method to 'stop' the data source.
        *
        * Downlink timeout timer is stopped and datasource is set to stopped state.
        *
        * @since    Series 60 3.0
        * @return   None
        */
        void SourceStopL();

        /**
        * From MDataSource. Method called by a MDataSink to request the data source to fill aBuffer with data.
        *
        * Datasource gets packet from jitterbuffer and forwards it to the payloadformatdecoder
        * for further processing. Datasource works synchronously; consumers BufferFilledL
        * is called when RTP packet is got succesfully from jitterbuffer.
        * 
        * @since    Series 60 3.0
        * @param    "aBuffer"
        *           The buffer that needs filling with data
        *
        * @param    "aConsumer" 
        *           The data sink that consumes the data. The data source needs this to make the BufferFilledL
        *           callback on aConsumer when the data source has completed filling the aBuffer.
        *
        * @param    "aMediaId"
        *           This identifies the type of media eg audio or video and the stream id.
        *           This parameter is required in cases where the source can supply data
        *           of more than one media type and/or multiple strams of data eg a multimedia file.
        * @return   None
        */
        void FillBufferL( CMMFBuffer* aBuffer,
                          MDataSink* aConsumer,
                          TMediaId aMediaId );
                          
        /**
        * From MDataSource
        */
        void NegotiateSourceL( MDataSink& aDataSink );
        
        /**
         * From MDataSource
         * Used for custom command receiving.
         * @since    Series 60 3.2
         * @param    aMessage    The message specifying the custom command
         * @return   void
         */
    	void SourceCustomCommand( TMMFMessage& aMessage );
    
    public: // From MRtpJitterObserver
        /**
		 * @since	Series 60 3.2
         * Send jitter event through mcc rtp interface
         */
        void SendJitterEvent( TMccRtpEventDataExtended aEvent, TInt aError );

        
    protected:  // Functions from base classes
        
        /**
        * From MDataSource. Method to perform any source construction dependant on the source construction
        * initialisation data aInitData.
        *
        * @since    Series 60 3.0
        * @param    "aInitData"
        *            Source specific initialisation data required for source construction
        * @return   None
        */
        void ConstructSourceL( const TDesC8& aInitData );

    protected:  // From CMccRtpInterface
    
        /**
        * From CMccRtpInterface
        */
        void DoCreateStreamL();
        
         /**
         * FROM SRTP API
         *
         * This function is called by SRTP when a master key is stale and
         * needs to be refreshed. 
         */
        void SRTPMasterKeyStaleEvent(const CSRTPStream& aStream);
        
        /**
         *  FROM SRTP API
         *
         * This function is called by SRTP Stream initiated with 
         * CSRTPSession  when a master key is stale and
         * needs to be refreshed.
         */ 
        void SRTPMasterKeyStaleEvent(const CSRTPSession& aSession );

    private: // From MMccExpirationHandler
    
        void TimerExpiredL( TMccTimerId aTimerId, TAny* aTimerParam );
        
    private:
        
        /**
        * C++ default constructor.
        */
        CMccRtpDataSource();
        
        /**
         * @since   Series 60 3.0
         *
         * @param   aHeaderInfo     RTP header
         * @param   aPayloadData    RTP packet data
         * @return  void
         */
        void PlayoutRtpPacketL( const TRtpRecvHeader& aHeaderInfo, 
            const TDesC8& aPayloadData );
        
        /**
         * Validates the received RTP packet that it is not a hostile packet.
         * @since   Series 60 3.0
         * @param   aStreamId       ID of the RTP stream
         * @param   aHeader         RTP header
         * @param   aData           RTP packet data
         * @return  User entry for currently received payload type
         */
        TMccRtpUser* ValidatePacketL( const TRtpId aStreamId, 
                const TRtpRecvHeader& aHeader, const TDesC8& aData );
    
        void SendStreamEventToClient( TMccEventType aEventType, 
                                      TInt aError = KErrNone,
                                      TUint32 aTargetPayloadType = KMccPTNotDefined );                               
                                      
        void RtpPacketReceivedL( TRtpId aStreamId, 
                                const TRtpRecvHeader& aHeaderInfo, 
                                const TDesC8& aPayloadData );

        /**
         * Handler method for custom commands.
         * @since    Series 60 3.2
         * @param    aMessage    The message specifying the custom command
         * @return   void
         */
    	void SourceCustomCommandL( TMMFMessage& aMessage );
    	
    	TBool InactivityTimerActive() const;
    	
    	TBool StandbyEnabled( TMccRtpUser* aUser ) const;
    	
    	void StandByL( TMccStandbyActionType aActionType, TUint aPayloadType );
    	
    	void StartRtpStandByTimerL( TMccRtpUser* aUser );
    	
    	void StopRtpStandByTimers();
    	
    	void ResetStandBy();
    	
    	void HandleStandByL( TMccRtpUser* aUser );    
  
        void DoStandByDecision( TMccRtpUser* aUser );
    
    	TMccRtpUser* FindUserEntryByPayloadType( TUint aPayloadType );
    	
    	TMccRtpUser* FindUserEntryByTimerId( TMccTimerId aTimerId );
    	
    	TInt FindUserEntryIndex( TMccRtpUser* aUser );
    	
    	static TBool RtpUserMatch( const TMccRtpUser& aUser1, 
    	                           const TMccRtpUser& aUser2 );
    

    private:    // Data

        // The client who requested "FillBuffer"
        MDataSink* iFillBufferRequester;
                
        // Buffer received from RTP stack
        CMMFDataBuffer* iBufferToFill;

        // Indicates that first audio packet has been received 
        TBool iDlStreamStarted;
        
        // Number of packets received
        TUint32 iPacketsReceived;
        
        // Timeout interval of the inactivity timer
        TUint32 iTimeoutTime;
        
        TUint32 iStandByTimerValue;
        
        // Stream ID for media packets
        TRtpId iRtpStreamId;
        
        /**
         * Tracks encoding changes for unknown media report logic.
         */
        TUint8 iCurRecvPayloadType; 

        /**
         * Instance of jitter calculator
         * Own.
         */
        CMccJitterCalculator* iJitCalc;
        
        TMccTimerId iInactivityTimerId;
        
        CMccTimerManager* iTimer;
        
        // Registered users for source
        RArray<TMccRtpUser> iUsers;
        
        //flag for secure key expired
        TBool iSecureKeyExpired;

    private:    // Friend classes     

    #ifdef TEST_EUNIT
    public:
        friend class UT_CMccRtpDataSource;
    #endif
    
    };

#endif      // MCCRTPDATASOURCE_H   
            
// End of File
