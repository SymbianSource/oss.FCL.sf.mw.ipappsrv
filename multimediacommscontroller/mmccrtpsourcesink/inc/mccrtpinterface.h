/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    RTP Source/Sink Base class
*
*/



#ifndef MCCRTPINTERFACE_H
#define MCCRTPINTERFACE_H

//  INCLUDES
#include <msrtprekeyingobserver.h>

#include <mmf/common/mmfutilities.h>
#include "rtpdef.h"
#include "mccinternalcodecs.h"
#include "mmccevents.h"
#include "mccinternalevents.h"
#include "mccexpirationhandler.h"
#include "mccinternaldef.h"

// CONSTANTS
const TUint KDefSampleRate = 8000;

// DATA TYPES

enum TMccStandbyActionType
    {
    EForceStandby,
    EActivateStandby,
    EDeactivateStandby
    };

enum TMccStandbyState
    {
    EActivating,
    EActive,
    EDeactivating,
    EInactive,
    ETurnedOff
    };
    
// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class CRtpAPI;
class MRtpObserver;
class MRtcpObserver;
class MAsyncEventHandler;
class TMccEvent;
class MMccRtpKeepalive;
class CMccRtpMediaClock;
class CSRTPSession;
class TMccCryptoContext;
class CSRTPStream;
class CSRTPCryptoContext;

// CLASS DECLARATION

/**
* Class to pass the session related parameters to the RTP source/sink.
* 
* @lib Mccrtpsourcesink.dll
* @since Series 60 3.0
*/
class TMccRtpSessionParams
    {
    public:
        
        inline TMccRtpSessionParams() : iRtpAPI( NULL ), iSessionId( KNullId ),
            iEnableRTCP( ETrue ), iRtpKeepalive( NULL ), iSecSession( NULL ) { }

        inline TMccRtpSessionParams( CRtpAPI* aRtpAPI, TRtpId aSessionId, 
            TBool aEnableRTCP, MMccRtpKeepalive* aRtpKeepalive, CSRTPSession* aSecSession ) :
            iRtpAPI( aRtpAPI ),
            iSessionId( aSessionId ), 
            iEnableRTCP( aEnableRTCP ),
            iRtpKeepalive( aRtpKeepalive ),
            iSecSession( aSecSession )
            { }

    public: // data

        // RTP API to use
        CRtpAPI* iRtpAPI;

        // The RTP session where source/sink operates
        TRtpId iSessionId;

        // Whether to enable RTCP for the session
        TBool iEnableRTCP;
        
        MMccRtpKeepalive* iRtpKeepalive;
        
        /**
         * Pointer to SRTP session
         * Not own.
         */
        CSRTPSession* iSecSession;
    };


 /**
* Class to contain rtp source/sink user information.
* 
* @lib Mccrtpsourcesink.dll
* @since Series 60 3.0
*/   
 class TMccRtpUser
    {
    public:
    
        TMccRtpUser( MAsyncEventHandler* aEventHandler ) : 
            iEventHandler( aEventHandler ),
            iStartedOnce( EFalse ),
            iPayloadType( KMccPTNotDefined ),
            iTimerId( KMaxTUint ),
            iStandbyState( EInactive )
            {
            }
            
    public:
    
        MAsyncEventHandler* iEventHandler;
        TBool iStartedOnce;
        TUint iPayloadType;
        TMccTimerId iTimerId;
        TMccStandbyState iStandbyState;
    };
    
     
/**
* Common interface for CMccRtpDataSource and -DataSink.
*
* @lib MccRtpSourceSink.dll
* @since Series 60 3.0
*/
class MMccRtpInterface : public MSRTPReKeyingObserver
    {

    public: // New functions
        
        /**
        * Set session level parameters.
        * @since Series 60 3.0
        * @param aParams Session level parameters
        * @return None
        */        
        virtual void SetSessionParamsL( const TMccRtpSessionParams& aParams );
        
        /**
        * Sends media level events
        * @since Series 60 3.0
        * @param aEvent Event to be sent
        * @return void
        */
        virtual void SendMediaSignallingL( const TMccEvent& aEvent ) = 0;
        
        /**
        * Configures the source/sink
        * @since Series 60 3.0
        * @param aConfig Configuration
        * @return void
        */
        virtual void ConfigureL( const TDesC8& aConfig, CMccRtpMediaClock* aRtpMediaClock );
        
        /**
        * Sends RTCP receiver report to uplink if possible.
        * @param aStreamId - [input] Stream ID identifying stream
        * @return One of the standard system-wide error codes.
        */
        virtual TInt SendRTCPReceiverReport();

        /**
        * Sends RTCP sender report to uplink if possible.
        * @param aStreamId - [input] Stream ID identifying stream
        * @return One of the standard system-wide error codes.
        */
        virtual TInt SendRTCPSenderReport();
        
        /**
        * Sends Non-RTCP data
        * @param aData - [input] Data to send
        * @return One of the standard system-wide error codes.
        */
        virtual void SendRTCPDataL( const TDesC8& aData );

        /** 
        * Starts inactivity timer for a stream
        * @param aTimeoutTime - [input] timeout value in milliseconds
        * @return One of the standard system-wide error codes.
        */
        virtual void StartInactivityTimerL( TUint32 aTimeoutTime );

        /**
        * Stops inactivity timer for a stream 
        * @return One of the standard system-wide error codes.
        */
        virtual void StopInactivityTimerL();
        
        /**
        */
        virtual TInt StandBy( TMccStandbyActionType aActionType, TUint aPayloadType );
        
        virtual TBool HandleBySsrc( const TRtpSSRC& aSsrc );
        
        virtual TBool HandleByStreamId( const TRtpId& aStreamId );
        
        virtual TRtpId RtpStreamId() = 0;
    
        virtual TBool IsSink() const;
        
        virtual TBool IsSending() const;
        
        virtual void DoCreateSrtpStreamL() = 0;
        
        /**
         * Create SRTP crypto context. /
         * Refresh exists crypto context.
         * @since Series 60 3.2
         * @param TMccCryptoContext Crypto context params.
         * @return void
         */
        virtual void ApplyCryptoContextL( const TMccCryptoContext& aContextParams );
       
        /**
         * Remove crypto context.
         * @since Series 60 3.2
         * @return Error code.
         */                                     
        virtual TInt RemoveCryptoContext(); 

    protected:  // New functions
            
        /**
        * Source/Sink states
        */
        enum TSourceSinkState
            {
            ERtpStateConstructed = 0,  // Pseudo state, not in working thread.
            ERtpStateStopped = 1,      // RTCP packets are handled.
            ERtpStatePrimed = 2,       // RTCP packets are handled.
            ERtpStatePlaying = 3,      // RTP packest are received/transmitted.
            ERtpStatePaused = 4       // RTCP packets are handled.
            };

        /**
        * C++ default constructor.
        */
        inline MMccRtpInterface() : iRtpAPI( NULL ), iSessionID( KNullId ),
            iState( ERtpStateConstructed ), iEnableRtcp( EFalse ), 
            iEventHandler( NULL ), iCodecInfo(), iMccStreamId( 0 ), 
            iMccEvent( TMccEvent() ), iRtpKeepalive( NULL ), iContext( NULL), 
            iSecSession( NULL ), iSrtpStream( NULL ), iRtpMediaClock( NULL ),
            iContextId( 0 ),
			iRtpKeepaliveUpdatePending( ETrue )
            { }
            
            
        /**
        * Checks the given buffer if it is supported. Leaves with KErrNoMemory
        * if the supplied buffer pointer is NULL. Leaves with KErrNotSupported
        * if the buffer is not supported.
        * @since Series 60 3.0
        * @param aBuffer Buffer to be checked.
        * @return void
        */
        void CheckBufferSupportL( CMMFBuffer* aBuffer ) const;
        
        /**
        * Creates transmit or receive stream.
        * @since Series 60 3.0
        * @param None
        * @return Stream ID
        */
        virtual void DoCreateStreamL() = 0;
        
        /**
        * Sets current state. Leaves with KErrNotReady if state transition is
        * not acceptable.
        * @since Series 60 3.0
        * @param aState A new state
        * @return void
        */
        void SetStateL( TSourceSinkState aState );
        
        /**
        * Returns current state.
        * @since Series 60 3.0
        * @param None
        * @return Current state
        */        
        inline TSourceSinkState State() const
            {
            return iState;
            }
        
        void CloseStreams();
        
        void SendJitterEventToClient( 	MAsyncEventHandler* aEventHandler,
                                        TUid aEventOriginator,
                                        TMccInternalEventType aInternalEventType,
                                        TMccEventType aEventType,
                                        TUint32 aEndpointId,
                                        TInt aError = KErrNone, 
                                        TUint64 aJitterEstimate = 0,
                                        TUint32 aPacketsReceived = 0,
                                        TInt64 aPrevTransTime = 0,
                                        TUint64 aTriggeredJitterLevel = 0,
        								TInt aPacketLoss = 0,
        								TInt aTriggeredPacketLoss = 0 );
        
        void SendInternalRtpEventToClient( MAsyncEventHandler* aEventHandler,
                                           TUid aEventOriginator,
                                           TMccInternalEventType aInternalEventType,
                                           TMccEventType aEventType,
                                           TUint32 aEndpointId,
                                           TInt aError = KErrNone, 
                                           TUint64 aJitterEstimate = 0,
                                           TUint32 aPacketsReceived = 0,
                                           TInt64 aPrevTransTime = 0,
                                           TUint64 aTriggeredJitterLevel = 0 );
        
        void SendInternalRtpEventToAllClients( RArray<TMccRtpUser>& aUsers,
                                               TUid aEventOriginator,
                                               TMccInternalEventType aInternalEventType,
                                               TMccEventType aEventType,
                                               TUint32 aEndpointId,
                                               TInt aError = KErrNone, 
                                               TUint64 aJitterEstimate = 0,
                                               TUint32 aPacketsReceived = 0,
                                               TInt64 aPrevTransTime = 0,
                                               TUint64 aTriggeredJitterLevel = 0 );
        
        void ClearMccEvent();
        
        /**
         * Start sending keepalive packets.
         *
         * @since Series 60 3.2
         * @param aRtpMediaClock rtp media clock instance
         * @return void
         */  
        void StartKeepaliveL( CMccRtpMediaClock& aRtpMediaClock );
        
        
        /**
         * Stop sending keepalive packets.
         *
         * @since Series 60 3.2
         * @return void
         */  
        void StopKeepalive();
        
        /**
         * Reset keepalive timer.
         *
         * @since Series 60 3.2
         * @return void
         */  
        void ResetKeepaliveTimer();
        
        /**
         * Send secure related event to client.
         *
         * @since Series 60 3.2
         * @param aEventHandler Event handler.
         * @param aEventOriginator Event originator.
         * @param aInternalEventType Event type.
         * @param aEndpointId Endpoint id.
         * @return void
         */   
        void SendSecureRtpEventToClient(  MAsyncEventHandler* aEventHandler,
                                          TUid aEventOriginator,
                                          TMccInternalEventType aInternalEventType,
                                          TMccEventType aEventType,
                                          TUint32 aEndpointId );
                                   
        /**
         * FROM SRTP API
         *
         * This function is called by SRTP when a master key is stale and
         * needs to be refreshed. 
         */
        virtual void SRTPMasterKeyStaleEvent(const CSRTPStream& aStream) =  0;
        
        /**
         *  FROM SRTP API
         *
         * This function is called by SRTP Stream initiated with 
         * CSRTPSession  when a master key is stale and
         * needs to be refreshed.
         */ 
        virtual void SRTPMasterKeyStaleEvent(const CSRTPSession& aSession ) = 0;

            
    private:    // New functions

    public:     // Data
    
    protected:  // Data
        
        // RTP stack API reference
        CRtpAPI* iRtpAPI;
        
        // Session ID where media stream belongs to
        TRtpId iSessionID;
        
        // Source/Sink state ( Stopped, Primed, Playing )
        TSourceSinkState iState;
        
        // RTCP Enabled / Disabled
        TBool iEnableRtcp;
        
        // Eventhandler
        MAsyncEventHandler* iEventHandler;
        
        // Codec information
        TMccCodecInfo iCodecInfo;
        
        // Mcc stream identifier
        TUint32 iMccStreamId;
        
        TMccEvent iMccEvent;
        
        MMccRtpKeepalive* iRtpKeepalive;

        /**
         * Rtp media clock instance
         * Not own.
         */
        CMccRtpMediaClock* iRtpMediaClock; 
       
        /*
         * Srtp crypto context. 
         * Own. / Not own.
         */
        CSRTPCryptoContext* iContext;
       
        /**
         * Pointer to SRTP session
         * Not own.
         */
        CSRTPSession* iSecSession;
        
        /*
         * Pointer to SRTP stream
         * Own. /Not Own.
         */
        CSRTPStream* iSrtpStream;
        
        /*
         * Context identifier
         */
         TUint32 iContextId;
       
	   /*
         * Indicates need for setting media clock for keepalivetimer. 
         */
		TBool iRtpKeepaliveUpdatePending;	 
		 
    private:    // Friend classes
    
        #ifdef TEST_EUNIT
            friend class UT_CMccRtpInterface;
        #endif
    };

#endif      // MCCRTPINTERFACE_H
            
// End of File
