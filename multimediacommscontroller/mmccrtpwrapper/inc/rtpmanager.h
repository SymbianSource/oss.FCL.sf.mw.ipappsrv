// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// This file provides the information required for building the whole of a rtpwrapper.
// 
//

/**
 @file
*/

#ifndef __RTP_MNGER_H__
#define __RTP_MNGER_H__

#include <e32std.h>
#include <in_sock.h>
#include <es_sock.h>

#include "rtpapi.h"
#include <rtp.h>

#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <rtpsession_internal.h>
#endif

#include <srtpsession.h>
#include <rtpprepostprocessingcallback.h>


const TInt KDefaultStartRtpAddress  = 8000;
const TInt KDefaultMaxRxSz          = 1250;
const TUint KRtpDefaultBandWidth    = 64000;
 
// Minimum bandwidth in bps
const TUint KRtpMinimumBandWidth    = 8000;
 
// RTCP minimum interval (ms) (default is 5 seconds)
const TUint KRtpDefaultRtcpInterval = 5000;
//Unspecified PayloadType
const TInt KRtpPayloadTypeUnspecified = -1;

const TInt KSrtpMaxAuthTagLength = 20;
const TInt KSrtpMaxMKILen = 10;

class CRtpManager;

/* Node representing a pending RTP request.
 * When an Asynchronous send is requested the details are stored in this node
 * and then put in to a list. The head is scheduled for transmission 
 */
class TRtpSendReqNode
	{
	public:
	TSglQueLink iLink;
	RRtpSendPacket iPktToSend;
	TRequestStatus* iStatus;
	};

/* Note this 'T' Class. It donot own the Resources.
 * It is the responsibility of the creator to make sure
 * the resources are properly freed. By putting the resources
 * in this class the ownership is not transfered */
NONSHARABLE_CLASS(CRtpSessionInfo) : public CBase, public MPrePostProcessingCallback
	{
	public:
	TUint32 iKey; 
	TInt iPortNumber; /* The RTP Port number used by this Session */
	
	#ifndef SYMBIAN_ENABLE_SPLIT_HEADERS
	RRtpSession iRtpSession; /* Session */
	#else
	RRtpSession_Internal iRtpSession; /* Session */
	#endif	
	
	CSRTPSession* iSRTPSession;
	TInt    iPayloadType; /* The Payload type for this SendStream */
	RRtpSendSource iRtpSendSource;
	CRtpManager* iRtpManager; /* To find our way back from callbacks */
	MRtpObserver *iRtpObserver;
	TBool         iRtcpEnabled;
	MRtcpObserver *iRtcpObserver;
	TBool iFirstPacketSent;

	/* A session can have only one Payload type */
	TUint32 iSamplingRate;
	MNonRTPDataObserver *iNonRtpDataObserver;
	TInetAddr iRemoteAddress;
	TInetAddr iRemoteRTCPAddress;
	/* TODO CleanThisUp!!! List of pending requests */
	TSglQue<TRtpSendReqNode> *iPendingReqQue;
	
	TInt PreRtpProcessing(TDes8& aRTPPacket);
	void PostRtpProcessing(TDes8& aRTPPacket);
	TInt PreRtcpProcessing(TDes8& aRTPPacket);
	void PostRtcpProcessing(TDes8& aRTPPacket);
	
	static CRtpSessionInfo* NewL();
	void ConstructL();
	
	private:
	CRtpSessionInfo():iFirstPacketSent(EFalse),iPayloadType(KRtpPayloadTypeUnspecified)
		{
		}

	};

/* Stores the Information regarding a Particular RTP recv Stream
 * belonging to a session */
class TRtpStreamInfo
	{
public:
	enum TStreamInfoState
		{
		/* Pre-Created->created before any new source was
		 * detected. When a new Source is finally detected
		 * this ID will be used. The state will change to
		 * eStreamReadyToUse */
		eStreamPreCreated,
		/* Post Created stream: A new source was detected
		 * but there were no pre created stream to assign this
		 * stream to. When the appilcation calls CreateReceiveStream
		 * then this stream will be picked up and its ID will be
		 * returned( if the payload type matches). */
		eStreamPostCreated,
		/* ID is given to the Application and the RRtpReceiveSource
		 * is valid */
		eStreamReadyToUse
		};

	TUint32 iKey;
	TUint32 iSessionKey; /* The Session to which this Stream belongs to */
	TInt    iPayloadType;
	TBool   iIsRecvStream;
	TStreamInfoState  iState; 
	RRtpReceiveSource iRecvSource; 
	CRtpManager* iRtpManager; /* To find our way back from callbacks */
	};

/* This class ByPasses all the function calls to Symbian RTP Stack */
class CRtpManager: public CBase
	    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
         static CRtpManager* NewL( MRtpErrNotify& aErrNotify );

        /**
        * Destructor.
        */
        virtual ~CRtpManager();

    public: 

        /**
        * Open and initialize the CRtpAPI object.
        * @param aSdesInfo - [input] SDES (Source Description)
        *   of the local participant (CName, Username, etc.)
        * @param aRtpPacketDll - [input] Path+file name of RTP packet
        *   formatting DLL to use; NULL to use standard RTP packet format
        * @param aRConnPtr - [input] Network connection to use;
        *   NULL to use default network connection.
        * @return KErrNone if successful; system wide error code otherwise
        */
         TInt OpenL( const TRtpSdesParams& aSdesInfo,
                             const TDesC* aRtpPacketDll,
                             const RSocketServ* aSocketServPtr,
                             const RConnection* aConnPtr );

        /**
        * Open and initialize the CRtpAPI object.
        * @param aIapId - [input] IAP ID. If -1, no IAP selection dialog
        *   will pop up. Instead, the default IAP will be used.
        * @return KErrNone if successful; system wide error code otherwise
        */
         TInt StartConnection( TInt aIapId = -1 );

        /**
        * Open and initialize the CRtpAPI object in asynchronous mode
        * @param aStatus - [output] On completion, will contain a status code:
        *   KErrNone if successful; system wide error code otherwise.
        * @param aIapId - [input] IAP ID. If -1, no IAP selection dialog
        *   will pop up. Instead, the default IAP will be used.
        * @return KErrNone if successful; system wide error code otherwise
        */
         TInt StartConnection( TRequestStatus& aStatus,
                                       TInt aIapId = -1 );

        /**
        * Cancels asynchoronous start of connection.
        * Any open requests will be completed with KErrCancel.
        * @return None
        */
         void CancelStart(); 

        /**
        * Close the CRtpAPI object.
        * @param None
        * @return None
        */
         void Close();

        /**
        * Set SDES (Source Description) information of the local participant.
        * Participant is defined as an application program and
        * only one local participant is allowed.
        * @param aSdesInfo - [input] SDES of the local participant
        *   (CName, Username, etc.).
        * @return None
        */
         void SetLocalSdes( const TRtpSdesParams& aSdesInfo );

        /**
        * Get the local IP address
        * @param None
        * @return local ip address.
        */
         TInetAddr& GetLocalIPAddressL();

        /**
        * Create a new RTP Session and return the Session ID as well as the
        * local port number assigned for RTP. RTP uses an even port number and
        * RTCP, if enabled, uses the next higher (odd) port number.
        * @param aSessionParams - [input] Session parameters
        * @param aPort - [input] Local port number to use for RTP;
        *                               0 to use default port number
        *                     - [output] port number assigned for RTP
        * @param aEnableRtcp - [input] ETrue to enable RTCP; EFalse to disable
        * @param aRtcpParams - [input] RTCP parameters to use;
        *    NULL to use default RTCP parameters
        * @return Identifier (ID) of RTP Session created if successful;
        *    KNullId otherwise
        */
         TRtpId CreateSessionL(
                            const TCreateSessionParams& aSessionParams,
                            TUint& aPort,
                            TBool aEnableRtcp,
                            const TRtcpParams* aRtcpParams );
        /**
        * Create a new, secure RTP Session and return the Session ID as well as the
        * local port number assigned for RTP. RTP uses an even port number and
        * RTCP, if enabled, uses the next higher (odd) port number.
        * User is expected to create corresponding SRTP session prior to calling this
        * function. 
        * @param aSessionParams - [input] Session parameters
        * @param aPort - [input] Local port number to use for RTP;
        *                               0 to use default port number
        *                     - [output] port number assigned for RTP
        * @param aEnableRtcp - [input] ETrue to enable RTCP; EFalse to disable
        * @param aRtcpParams - [input] RTCP parameters to use;
        *    NULL to use default RTCP parameters
        * @param aSession - [input] SRTP session to be used by this RTP session.
        * @return Identifier (ID) of RTP Session created if successful;
        *    KNullId otherwise
        */

         TRtpId CreateSessionL( 
	                const TCreateSessionParams& aSessionParams, 
	                TUint& aPort, 
	                TBool aEnableRtcp, 
	                const TRtcpParams* aRtcpParams, 
	                CSRTPSession& aSession); 
                            

        /**
        * Start an RTP Session. If enabled, RTCP associated with the given
        * session is also started.
        * @param aSessionId - [input] Session ID
        * @return KErrNone if successful; system wide error code otherwise
        */
         TInt StartSession( TRtpId aSessionId );

        /**
        * Close an RTP Session.
        * @param aSessionId - [input] Session ID
        * @return None
        */
         void CloseSession( TRtpId aSessionId );

        /**
        * Set remote IP address and port number to RTP Session.
        * Port number for RTP must be an even number and the corresponding
        * RTCP, if enabled, will be set to use the next higher (odd) port.
        * @param aSessionId - [input] RTP Session ID
        * @param aRemoteAddr - [input] Remote IP address & port number for RTP
        * @return KErrNone if successful; system wide error code otherwise
        */
         TInt SetRemoteAddress( TRtpId aSessionId,
                                        const TInetAddr& aRemoteAddr );

        /**
        * Set remote RTCP IP address and port number to RTP Session accoring to RFC 3605
        * i.e. this should be used if RTCP port is different than RTP port + 1
        * @param TRtpId aSessionId - [input] RTP Session ID
        * @param TInetAddr& aRemoteAddr - [input] Remote IP address & port
        *                                 number for RTP
        * @return TInt - KErrNone if successful; KErrNotFound if invalid session id;
        *				 KErrNotSupported if RTCP disabled; system wide error code otherwise
        */
         TInt SetRemoteRtcpAddress( TRtpId aSessionId, const TInetAddr& aRemoteRtcpAddr );
        
        /**
        * Create a Receive stream for an RTP Session and return the stream
        * ID which is unique for all RTP Sessions.
        * @param aSessionId - [input] RTP Session ID
        * @param aParams - [input] Receive stream parameters
        * @return Stream ID if successful; KNullId otherwise
        */
         TRtpId CreateReceiveStreamL( TRtpId aSessionId,
                            const TRcvStreamParams& aParams );

        /**
        * Create a Transmit stream for an RTP Session and return the stream
        * ID which is unique for all RTP Sessions. SSRC value assigned for the
        * very first Transmit stream will be the same as the default SSRC
        * value reserved internally by CreateSession function. This function
        * is normally used to create a transmit stream where SSRC value is
        * randomly generated.
        * @param aSessionId - [input] RTP Session ID
        * @param aParams - [input] Transmit stream parameters
        * @param aSSRC - [output] SSRC value assigned
        * @return Stream ID if successful; KNullId otherwise
        */
         TRtpId CreateTransmitStreamL( TRtpId aSessionId,
                            const TTranStreamParams& aParams,
                            TRtpSSRC& aSSRC );

        /**
        * Create a Transmit stream, with a given SSRC value, for an RTP Session
        * and return the stream ID which is unique for all RTP Sessions. This
        * extended function is used for a special case where a specific SSRC
        * value needs to be associated with the transmit stream being created,
        * e.g. for retransmission purpose.
        * @param aSessionId - [input] RTP Session ID
        * @param aParams - [input] Transmit stream parameters
        * @param aSsrc - [input] SSRC value to use
        * @return Stream ID if successful; KNullId otherwise
        */
         TRtpId CreateTransmitStreamExtL( TRtpId aSessionId,
                            const TTranStreamParams& aParams,
                            const TRtpSSRC aSSRC );

        /**
        * Close a Transmit or Receive stream.
        * @param TRtpId aStreamId - [input] ID of stream to close
        * @return None
        */
         void CloseStream( TRtpId aStreamId );

        /**
        * Register a callback object for receiving RTP data packets from an RTP
        * Session. Only one receiver callback object is allowed to be
        * registered for one Session.
        * MRtpObserver::RtpPacketReceived function from aRtpObserver object is
        * called when an RTP data packet is received.
        * @param aSessionId - [input] RTP Session ID
        * @param aRtpObserver - [input] Callback object to receive RTP packets
        * @return KErrNone if successful; system wide error code otherwise
        */
         TInt RegisterRtpObserver( TRtpId aSessionId,
                                           MRtpObserver& aRtpObserver );

        /**
        * Unregister RTP observer callback object associated with an RTP
        * session.
        * @param aSessionId - [input] RTP Session ID
        * @return None
        */
         void UnregisterRtpObserver( TRtpId aSessionId );

        /**
        * Sets/resets the observer for the non-RTP data.
        * Only one receiver callback object is allowed to be
        * registered for one Session.
        * MNonRTPDataObserver::NonRTPDataReceived or
        * MNonRTPDataObserver::NonRTCPDataReceived from 
        * aNonRTPDataObserver object is
        * called when a non-RTP data packet is received.
        * @param aNonRTPDataObserver an observer, 0-pointer resets the observer
        * @return KErrNotFound if non-existing session-ID provided, system-wide error code otherwise. KErrNone if successfull.
        */
         TInt SetNonRTPDataObserver( TRtpId aSessionId, 
                                    MNonRTPDataObserver* aNonRTPDataObserver );

        /**
        * Send an RTP data packet in a Transmit stream synchronously.
        * @param aTranStreamId - [input] ID of Transmit stream
        * @param aHeaderInfo - [input] Header information of RTP data packet
        * @param aPayloadData - [input] Payload data of RTP data packet
        * @return KErrNone if successful; system wide error code otherwise
        */
         TInt SendRtpPacket( TRtpId aTranStreamId,
                                     const TRtpSendHeader& aHeaderInfo,
                                     const TDesC8& aPayloadData );

        /**
        * Send an RTP data packet in a Transmit stream asynchronously.
        * @param TRtpId aTranStreamId - [input] ID of Transmit stream
        * @param aHeaderInfo - [input] Header information
        *                                      of RTP data packet to send
        * @param aPayloadData - [input] Payload data of RTP data packet to send
        * @param aStatus - [output] On completion, will contain a status code:
        *    KErrNone if successful; system wide error code otherwise
        * @return KErrNone if successful; system wide error code otherwise
        */
         TInt SendRtpPacket( TRtpId aTranStreamId,
                                     const TRtpSendHeader& aHeaderInfo,
                                     const TDesC8& aPayloadData,
                                     TRequestStatus& aStatus );

        /**
        * Send an RTP data packet asynchronously, with a given sequence number,
        * in a Transmit stream mainly for retransmission purpose.
        * @param aTranStreamId - [input] ID of Transmit stream
        * @param aSequenceNum - [input] Sequence number to use
        * @param aHeaderInfo - [input] Header information of RTP data packet
        * @param aPayloadData - [input] Payload data of RTP data packet to send
        * @param aStatus - [output] On completion, will contain a status code:
        *    KErrNone if successful; system wide error code otherwise
        * @return KErrNone if successful; system wide error code otherwise
        */
         TInt SendRtpPacket( TRtpId aTranStreamId,
                                     TRtpSequence aSequenceNum,
                                     const TRtpSendHeader& aHeaderInfo,
                                     const TDesC8& aPayloadData,
                                     TRequestStatus& aStatus );


        /**
        * Send a non-RTP (control) data packet asynchronously
        * @param aSessionId - [input] RTP Session ID
        * @param aUseRTPSocket - [input] use RTP/RTCP socket
        * @param aData - [input] non-RTP data packet to send
        * @param aStatus - [output] On completion, will contain a status code:
        *    KErrNone if successful; system wide error code otherwise
        * @return KErrNone if successful; system wide error code otherwise
        */
         void SendDataL( TRtpId aSessionId,
                                TBool aUseRTPSocket,
                                const TDesC8& aData,
                                TRequestStatus& aStatus );

        /**
        * Cancel an outstanding SendRtpPacket() operation.
        * @param TRtpId aSessionId - [input] RTP Session ID
        * @return None
        */
         void CancelSend( TRtpId aSessionId );

        /**
        * Register a callback object for receiving RTCP packets associated with
        * a given RTP Session. Only one observer callback object is allowed to
        * be registered. One of aRtcpObserver object's callback functions is
        * called when an RTCP packet of that type is received.
        * @param aSessionId - [input] RTP Session ID
        * @param aObserver - [input] Callback object to receive RTCP packets
        * @return KErrNone if successful; system wide error code otherwise.
        */
         TInt RegisterRtcpObserver( TRtpId aSessionId,
                                            MRtcpObserver& aRtcpObserver );

        /**
        * Unregister RTCP observer callback object associated with an RTP
        * session.
        * @param aSessionId - [input] RTP Session ID
        * @return None
        */
         void UnregisterRtcpObserver( TRtpId aSessionId );

        /**
        * Send an RTCP BYE packet for a Transmit stream.
        * @param aTranStreamId - [input] Transmit stream ID
        * @param aReason - [input] Reason for leaving
        * @return KErrNone if successful; system wide error code otherwise
        */
         TInt SendRtcpByePacket( TRtpId aTranStreamId,
                                         const TDesC8& aReason );

        /**
        * Send an RTCP APP packet for a Transmit stream.
        * @param TRtpId aTranStreamId - [input] Transmit stream ID
        * @param TRtcpApp aApp - [input] APP packet to send
        * @return KErrNone if successful; system wide error code otherwise
        */
         TInt SendRtcpAppPacket( TRtpId aTranStreamId,
                                         const TRtcpApp& aApp );

        /** 
        * Send an RTCP RR packet for a Reception stream.
        * @param TRtpId aRcvStreamId - [input] Reception stream ID
        * @return TInt - KErrNone if successful; system wide error code otherwise
        */
         TInt SendRtcpRrPacket( TRtpId aRcvStreamId );
        
        /** 
        * Send an RTCP SR packet for a Transmit stream.
        * @param TRtpId aTranStreamId - [input] Transmit stream ID
        * @return TInt - KErrNone if successful; system wide error code otherwise
        */
         TInt SendRtcpSrPacket( TRtpId aTranStreamId );

		/** 
        * Suspend RTCP sending on/off, calculations will continue. 
        * @param TRtpId aSessionId - [input] RTP Session ID
        * @param TBool aAutoSending - [input] Auto sending flag. 
        *							  ETrue:  RTCP sending will be scheduled normally
        *							  EFalse: RTCP sending will be suspended
        * @return TInt - KErrNone if successful; KErrNotFound if invalid session id;
        *				 KErrNotSupported if RTCP disabled;  system wide error code otherwise				
        */
         TInt SuspendRtcpSending( TRtpId aSessionId, TBool aAutoSending );
        
		/** 
        * Gets the status of automatic RTCP sending.
        * @param TRtpId aSessionId - [input] RTP Session ID
        * @param TBool aAutoSending - [output] ETrue: RTCP auto sending is on 
        *				  					   EFalse: RTCP auto sending is off
 		* @return TInt - KErrNone if successful; KErrNotFound if invalid session id;
        *				 KErrNotSupported if RTCP disabled;  system wide error code otherwise
        */
         TInt IsRtcpSendingSuspended( TRtpId aSessionId, TBool& aAutoSending );

        /**
        * Get the session ID of a stream, which belongs to that session.
        * @param aStreamId - [input] ID of a stream
        * @return Identifier (ID) of RTP Session if found; KNullId otherwise
        */
         TRtpId GetSessionId( TRtpId aStreamId );

        /**
        * Get address of Socket object used by a given RTP Session
        * to send/receive RTP data packets.
        * @param aSessionId - [input] RTP Session ID
        * @return Address of Socket object used if successful; NULL otherwise
        */
         RSocket* GetRtpSocket( TRtpId aSessionId );

        /**
        * Get address of Socket object used by a given RTP Session
        * to send/receive RTCP control packets.
        * @param aSessionId - [input] RTP Session ID
        * @return Address of Socket object used if successful; NULL otherwise
        */
         RSocket* GetRtcpSocket( TRtpId aSessionId );

        /**
        * Retrieve statistical information for a stream
        * based on the reports from RTCP SR & RR packets.
        * @param aStreamId - [input] ID of stream
        * @param aStat - [output] Statistical information
        * @return KErrNone if successful; system wide error code otherwise
        */
         TInt GetStreamStatistics( TRtpId aStreamId,
                                           TRtpPeerStat& aStat );

        /**
        * Get sampling rate setting for a payload type.
        * @param aPayloadType - [input] Payload type
        * @return Sampling rate in Hz; 0 if payload type setting not found
        */
         TUint32 GetSamplingRate( TUint8 aPayloadType );

        /**
        * Set sampling rate for a payload type.
        * @param aPayloadType - [input] Payload type
        * @param aSamplingRate - [input] Sampling rate in Hz
        * @return KErrNone if successful; system wide error code otherwise
        */
         TInt SetSamplingRate( TUint8 aPayloadType,
                                       TUint32 aSamplingRate );

        /**
        * Set RTCP parameters for a given RTP Session.
        * This function does nothing if RTCP was not enabled previously.
        * @param aSessionId - [input] RTP Session ID
        * @param aRtcpParams - [input]  RTCP parameters to use
        * @return KErrNone if successful; system wide error code otherwise
        */
         TInt SetRtcpParameters( TRtpId aSessionId,
                                         const TRtcpParams& aRtcpParams );

        /**
        * Synchronous custom command interface for future extensions.
        * Input/output data and return value are defined by each custom command
        * @param aFunction - [input] Function ID
        * @param aInputData1 - [input] Input data #1; NULL if not used
        * @param aInputData2 - [input] Input data #2; NULL if not used
        * @param aOutputData - [output] Output data; NULL if not used
        * @return Defined by the function
        */
         TInt CustomCommandSync( TInt aFunction,
                                         const TDesC8& aInputData1,
                                         const TDesC8& aInputData2,
                                         TDes8& aOutputData );

        /**
        * ASynchronous custom command interface for future extensions.
        * Input/output data and return value are defined by each custom command
        * @param aFunction - [input] Function ID
        * @param aInputData1 - [input] Input data #1; NULL if not used
        * @param aInputData2 - [input] Input data #2; NULL if not used
        * @param aOutputData - [output] Output data; NULL if not used
        * @param aStatus - [output] On completion, will contain a status code:
        *    KErrNone if successful; system wide error code otherwise
        * @return Defined by the function
        */
         TInt CustomCommandAsync( TInt aFunction,
                                          const TDesC8& aInputData1,
                                          const TDesC8& aInputData2,
                                          TDes8& aOutputData,
                                          TRequestStatus& aStatus );
    private:
        /**
        * C++ default constructor.
        */
        CRtpManager();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL( MRtpErrNotify& aErrNotify );

        /* Returns the Session Index or KErrnotFound if Session cannot be found */
        TInt FindSession(TRtpId aSessionId);
		
        /* Returns the session Index or leaves if not found */
        TInt FindSessionL(TRtpId aSessionId);
        
        /* Returns the stream Index or KErrnotFound if stream cannot be found */
        TInt FindStream(TRtpId aStreamId);
        
        /* Returns the stream Index or leaves if not found */
        TInt FindStreamL(TRtpId aStreamId);
		
		void RegisterEventsOnSessionL(CRtpSessionInfo *aSessionInfo);
		void RegisterEventsOnRtpRcvSourceL(RRtpReceiveSource &aRcvSrc, TRtpStreamInfo *apStrmInfo);
		void HandleNonRtpDataL(CRtpSessionInfo *apSessionInfo);
		void HandleNonRtcpDataL(CRtpSessionInfo *apSessionInfo);
		
		TInt GetDefaultIapIdL();
		
		void MakeACnameL(TDes8 &aCname);
		
		TInt SendRtpDataFromPendingQueue(CRtpSessionInfo *aSessInfo);
		/* Callback Handlers */
		static void RtpSessionLevelCB(CRtpSessionInfo* apSessionInfo, const TRtpEvent& aEvent);
		static void RtpStreamLevelCB(TRtpStreamInfo* apSessionInfo, const TRtpEvent& aEvent);
		
    private:    // Data
       TBool     iOwnsSocketServer;
       TBool     iOwnsConnection;
       TInetAddr iSockAddr;
       MRtpErrNotify *iRtpErrNotify; 
	   RSocketServ   *iSocketServ;
	   RConnection   *iConnection;
	   TRtpSdesParams iSdesInfo;   //This needs to be looked into.
	   RPointerArray<CRtpSessionInfo> iRtpSessionArr;
	   RArray<TRtpStreamInfo> iRtpStreamsArr;
	   TInt   iIapId;
    private:
    #ifdef __RTPWRAPPER_UNITTEST__
        friend class CRtpManager_GetLocalIPAddress;
        friend class CRtpManager_CreateTransmitStream;
        friend class CRtpManager_CreateReceiveStream;
        friend class CRtpManager_SendData;
        friend class CRtpManager_CancelSend;
        friend class CRtpManager_SendRtcpByePacket;
        friend class CRtpManager_SendRtcpSrPacket;
    #endif
    #ifdef EUNIT_TESTING
	  friend class UT_CRtpAPI;
	#endif     	    
};

/* Loging Macros */
// Logging

// Logging
#ifdef _DEBUG

    #ifdef LOG_TO_DEBUGOUT
    #include <e32debug.h>
        #define __RTP_LOG_STMT(stmt)    stmt
        #define LOG_FUNC_ENTRY(text) RDebug::Print(_L("-->>"L##text));
        #define LOG_FUNC_EXIT(text) RDebug::Print(_L("<<--"L##text));
        #define __RTP_LOG(text) RDebug::Print(_L(text));
        #define __RTP_LOG1(text, a) RDebug::Print(_L(text), a);
        #define __RTP_LOG2(text, a, b) RDebug::Print(_L(text), a, b);

    #else
    #include <flogger.h>
        #define __RTP_LOG_STMT(stmt)    stmt
        #define KRtpWrapperLogDir   _L("rtp")
        #define KRtpWrapperLogFileName _L("rtpcorelog.txt")
        #define LOG_FUNC_ENTRY(text) RFileLogger::WriteFormat(KRtpWrapperLogDir, KRtpWrapperLogFileName, EFileLoggingModeAppend,_L("-->>"L##text));
        #define LOG_FUNC_EXIT(text) RFileLogger::WriteFormat(KRtpWrapperLogDir, KRtpWrapperLogFileName, EFileLoggingModeAppend,_L("<<--"L##text));
        #define __RTP_LOG(text) RFileLogger::WriteFormat(KRtpWrapperLogDir, KRtpWrapperLogFileName, EFileLoggingModeAppend, _L(text));
        #define __RTP_LOG1(text, a) RFileLogger::WriteFormat(KRtpWrapperLogDir, KRtpWrapperLogFileName, EFileLoggingModeAppend, _L(text), a);
        #define __RTP_LOG2(text, a, b) RFileLogger::WriteFormat(KRtpWrapperLogDir, KRtpWrapperLogFileName, EFileLoggingModeAppend, _L(text), a, b);
    #endif
    
#else
    #ifndef __RTP_LOG_LOG
    #include <flogger.h>
        #define LOG_FUNC_ENTRY(text) 
        #define LOG_FUNC_EXIT(text) 
        #define __RTP_LOG_STMT(stmt)
        #define __RTP_LOGNoInd(text)
        #define __RTP_LOG(text)
        #define __RTP_LOG1(text, a)
        #define __RTP_LOG2(text, a, b)
    #endif
    
#endif


#endif
