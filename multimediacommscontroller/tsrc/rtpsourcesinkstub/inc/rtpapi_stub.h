/*
* Copyright (c) 2004-2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    API (Application Programming Interface) header file
*              : for RTP (Real-time Transport Protocol) library.
*
*/




#ifndef __RTPAPI_H
#define __RTPAPI_H

//  INCLUDES
#include "rtpdef.h"
#include "SrtpSession.h"

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class CRtpManager;

// CLASS DECLARATION

/**
*  API (Application Programming Interface) class for RTP (Real-time Transport
*  Protocol) library.
*
*  @lib RtpService.dll
*/
class CRtpAPI : public CBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        IMPORT_C static CRtpAPI* NewL( MRtpErrNotify& aErrNotify );

        /**
        * Destructor.
        */
        virtual ~CRtpAPI();

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
        IMPORT_C TInt OpenL( const TRtpSdesParams& aSdesInfo,
                             const TDesC* aRtpPacketDll,
                             const RSocketServ* aSocketServPtr,
                             const RConnection* aConnPtr );

        /**
        * Open and initialize the CRtpAPI object.
        * @param aIapId - [input] IAP ID. If -1, no IAP selection dialog
        *   will pop up. Instead, the default IAP will be used.
        * @return KErrNone if successful; system wide error code otherwise
        */
        IMPORT_C TInt StartConnection( TInt aIapId = -1 );

        /**
        * Open and initialize the CRtpAPI object in asynchronous mode
        * @param aStatus - [output] On completion, will contain a status code:
        *   KErrNone if successful; system wide error code otherwise.
        * @param aIapId - [input] IAP ID. If -1, no IAP selection dialog
        *   will pop up. Instead, the default IAP will be used.
        * @return KErrNone if successful; system wide error code otherwise
        */
        IMPORT_C TInt StartConnection( TRequestStatus& aStatus,
                                       TInt aIapId = -1 );

        /**
        * Cancels asynchoronous start of connection.
        * Any open requests will be completed with KErrCancel.
        * @return None
        */
        IMPORT_C void CancelStart(); 

        /**
        * Close the CRtpAPI object.
        * @param None
        * @return None
        */
        IMPORT_C void Close();

        /**
        * Set SDES (Source Description) information of the local participant.
        * Participant is defined as an application program and
        * only one local participant is allowed.
        * @param aSdesInfo - [input] SDES of the local participant
        *   (CName, Username, etc.).
        * @return None
        */
        IMPORT_C void SetLocalSdes( const TRtpSdesParams& aSdesInfo );

        /**
        * Get the local IP address
        * @param None
        * @return local ip address.
        */
        IMPORT_C TInetAddr& GetLocalIPAddressL();

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
        IMPORT_C TRtpId CreateSessionL(
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

        IMPORT_C TRtpId CreateSessionL( 
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
        IMPORT_C TInt StartSession( TRtpId aSessionId );

        /**
        * Close an RTP Session.
        * @param aSessionId - [input] Session ID
        * @return None
        */
        IMPORT_C void CloseSession( TRtpId aSessionId );

        /**
        * Set remote IP address and port number to RTP Session.
        * Port number for RTP must be an even number and the corresponding
        * RTCP, if enabled, will be set to use the next higher (odd) port.
        * @param aSessionId - [input] RTP Session ID
        * @param aRemoteAddr - [input] Remote IP address & port number for RTP
        * @return KErrNone if successful; system wide error code otherwise
        */
        IMPORT_C TInt SetRemoteAddress( TRtpId aSessionId,
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
        IMPORT_C TInt SetRemoteRtcpAddress( TRtpId aSessionId, const TInetAddr& aRemoteRtcpAddr );
        
        /**
        * Create a Receive stream for an RTP Session and return the stream
        * ID which is unique for all RTP Sessions.
        * @param aSessionId - [input] RTP Session ID
        * @param aParams - [input] Receive stream parameters
        * @return Stream ID if successful; KNullId otherwise
        */
        IMPORT_C TRtpId CreateReceiveStreamL( TRtpId aSessionId,
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
        IMPORT_C TRtpId CreateTransmitStreamL( TRtpId aSessionId,
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
        IMPORT_C TRtpId CreateTransmitStreamExtL( TRtpId aSessionId,
                            const TTranStreamParams& aParams,
                            const TRtpSSRC aSSRC );

        /**
        * Close a Transmit or Receive stream.
        * @param TRtpId aStreamId - [input] ID of stream to close
        * @return None
        */
        IMPORT_C void CloseStream( TRtpId aStreamId );

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
        IMPORT_C TInt RegisterRtpObserver( TRtpId aSessionId,
                                           MRtpObserver& aRtpObserver );

        /**
        * Unregister RTP observer callback object associated with an RTP
        * session.
        * @param aSessionId - [input] RTP Session ID
        * @return None
        */
        IMPORT_C void UnregisterRtpObserver( TRtpId aSessionId );

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
        IMPORT_C TInt SetNonRTPDataObserver( TRtpId aSessionId, 
                                    MNonRTPDataObserver* aNonRTPDataObserver );

        /**
        * Send an RTP data packet in a Transmit stream synchronously.
        * @param aTranStreamId - [input] ID of Transmit stream
        * @param aHeaderInfo - [input] Header information of RTP data packet
        * @param aPayloadData - [input] Payload data of RTP data packet
        * @return KErrNone if successful; system wide error code otherwise
        */
        IMPORT_C TInt SendRtpPacket( TRtpId aTranStreamId,
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
        IMPORT_C TInt SendRtpPacket( TRtpId aTranStreamId,
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
        IMPORT_C TInt SendRtpPacket( TRtpId aTranStreamId,
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
        IMPORT_C void SendDataL( TRtpId aSessionId,
                                TBool aUseRTPSocket,
                                const TDesC8& aData,
                                TRequestStatus& aStatus );

        /**
        * Cancel an outstanding SendRtpPacket() operation.
        * @param TRtpId aSessionId - [input] RTP Session ID
        * @return None
        */
        IMPORT_C void CancelSend( TRtpId aSessionId );

        /**
        * Register a callback object for receiving RTCP packets associated with
        * a given RTP Session. Only one observer callback object is allowed to
        * be registered. One of aRtcpObserver object's callback functions is
        * called when an RTCP packet of that type is received.
        * @param aSessionId - [input] RTP Session ID
        * @param aObserver - [input] Callback object to receive RTCP packets
        * @return KErrNone if successful; system wide error code otherwise.
        */
        IMPORT_C TInt RegisterRtcpObserver( TRtpId aSessionId,
                                            MRtcpObserver& aRtcpObserver );

        /**
        * Unregister RTCP observer callback object associated with an RTP
        * session.
        * @param aSessionId - [input] RTP Session ID
        * @return None
        */
        IMPORT_C void UnregisterRtcpObserver( TRtpId aSessionId );

        /**
        * Send an RTCP BYE packet for a Transmit stream.
        * @param aTranStreamId - [input] Transmit stream ID
        * @param aReason - [input] Reason for leaving
        * @return KErrNone if successful; system wide error code otherwise
        */
        IMPORT_C TInt SendRtcpByePacket( TRtpId aTranStreamId,
                                         const TDesC8& aReason );

        /**
        * Send an RTCP APP packet for a Transmit stream.
        * @param TRtpId aTranStreamId - [input] Transmit stream ID
        * @param TRtcpApp aApp - [input] APP packet to send
        * @return KErrNone if successful; system wide error code otherwise
        */
        IMPORT_C TInt SendRtcpAppPacket( TRtpId aTranStreamId,
                                         const TRtcpApp& aApp );

        /** 
        * Send an RTCP RR packet for a Reception stream.
        * @param TRtpId aRcvStreamId - [input] Reception stream ID
        * @return TInt - KErrNone if successful; system wide error code otherwise
        */
        IMPORT_C TInt SendRtcpRrPacket( TRtpId aRcvStreamId );
        
        /** 
        * Send an RTCP SR packet for a Transmit stream.
        * @param TRtpId aTranStreamId - [input] Transmit stream ID
        * @return TInt - KErrNone if successful; system wide error code otherwise
        */
        IMPORT_C TInt SendRtcpSrPacket( TRtpId aTranStreamId );

		/** 
        * Suspend RTCP sending on/off, calculations will continue. 
        * @param TRtpId aSessionId - [input] RTP Session ID
        * @param TBool aAutoSending - [input] Auto sending flag. 
        *							  ETrue:  RTCP sending will be scheduled normally
        *							  EFalse: RTCP sending will be suspended
        * @return TInt - KErrNone if successful; KErrNotFound if invalid session id;
        *				 KErrNotSupported if RTCP disabled;  system wide error code otherwise				
        */
        IMPORT_C TInt SuspendRtcpSending( TRtpId aSessionId, TBool aAutoSending );
        
		/** 
        * Gets the status of automatic RTCP sending.
        * @param TRtpId aSessionId - [input] RTP Session ID
        * @param TBool aAutoSending - [output] ETrue: RTCP auto sending is on 
        *				  					   EFalse: RTCP auto sending is off
 		* @return TInt - KErrNone if successful; KErrNotFound if invalid session id;
        *				 KErrNotSupported if RTCP disabled;  system wide error code otherwise
        */
        IMPORT_C TInt IsRtcpSendingSuspended( TRtpId aSessionId, TBool& aAutoSending );

        /**
        * Get the session ID of a stream, which belongs to that session.
        * @param aStreamId - [input] ID of a stream
        * @return Identifier (ID) of RTP Session if found; KNullId otherwise
        */
        IMPORT_C TRtpId GetSessionId( TRtpId aStreamId );

        /**
        * Get address of Socket object used by a given RTP Session
        * to send/receive RTP data packets.
        * @param aSessionId - [input] RTP Session ID
        * @return Address of Socket object used if successful; NULL otherwise
        */
        IMPORT_C RSocket* GetRtpSocket( TRtpId aSessionId );

        /**
        * Get address of Socket object used by a given RTP Session
        * to send/receive RTCP control packets.
        * @param aSessionId - [input] RTP Session ID
        * @return Address of Socket object used if successful; NULL otherwise
        */
        IMPORT_C RSocket* GetRtcpSocket( TRtpId aSessionId );

        /**
        * Retrieve statistical information for a stream
        * based on the reports from RTCP SR & RR packets.
        * @param aStreamId - [input] ID of stream
        * @param aStat - [output] Statistical information
        * @return KErrNone if successful; system wide error code otherwise
        */
        IMPORT_C TInt GetStreamStatistics( TRtpId aStreamId,
                                           TRtpPeerStat& aStat );

        /**
        * Get sampling rate setting for a payload type.
        * @param aPayloadType - [input] Payload type
        * @return Sampling rate in Hz; 0 if payload type setting not found
        */
        IMPORT_C TUint32 GetSamplingRate( TUint8 aPayloadType );

        /**
        * Set sampling rate for a payload type.
        * @param aPayloadType - [input] Payload type
        * @param aSamplingRate - [input] Sampling rate in Hz
        * @return KErrNone if successful; system wide error code otherwise
        */
        IMPORT_C TInt SetSamplingRate( TUint8 aPayloadType,
                                       TUint32 aSamplingRate );

        /**
        * Set RTCP parameters for a given RTP Session.
        * This function does nothing if RTCP was not enabled previously.
        * @param aSessionId - [input] RTP Session ID
        * @param aRtcpParams - [input]  RTCP parameters to use
        * @return KErrNone if successful; system wide error code otherwise
        */
        IMPORT_C TInt SetRtcpParameters( TRtpId aSessionId,
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
        IMPORT_C TInt CustomCommandSync( TInt aFunction,
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
        IMPORT_C TInt CustomCommandAsync( TInt aFunction,
                                          const TDesC8& aInputData1,
                                          const TDesC8& aInputData2,
                                          TDes8& aOutputData,
                                          TRequestStatus& aStatus );

        /**
        * Version of RtpService.dll
        * @return Version of current RtpService.dll
        */
        IMPORT_C TVersion Version() const;


    private:
        /**
        * C++ default constructor.
        */
        CRtpAPI();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL( MRtpErrNotify& aErrNotify );


    private:    // Data
        // RTP Manager
        CRtpManager* iManager;
        
        TInetAddr iAdressi;

        // Reserved for future extensions
        TAny* iReserved1;
        TAny* iReserved2;
        TInt iReserved3;
        TInt iReserved4;
    private:    
    #ifdef EUNIT_TESTING
	  friend class UT_CRtpAPI;
	#endif     	    
    };

#endif //__RTPAPI_H
