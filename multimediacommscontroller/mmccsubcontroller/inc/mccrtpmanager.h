/*
* Copyright (c) 2004-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    RTP session handling 
*
*/




#ifndef MCCRTPMANAGER_H
#define MCCRTPMANAGER_H

//  INCLUDES
#include "rtpapi.h"
#include "mccqoseventobserver.h"

// FORWARD DECLARATIONS
class MAsyncEventHandler;
class MDataSink;
class MDataSource;
class TMccCodecInfo;
class CMccRtcpReceiver;
class MMccResources;
class CMccRtpKeepaliveContainer;
class TMccCreateLink;
class CSRTPSession;
class CMccQosHandler;


// CLASS DECLARATION

/**
*  Mcc RTP Manager implements Mcc RTP API handling.
*
*  @since Series 60 3.0
*/
NONSHARABLE_CLASS( CMccRtpManager ) : public CBase, public MRtpErrNotify,
	public MMccQosEventObserver
	{
	public:	// Constructors and destructor

		/**
        * Two-phased constructor.
        */
        static CMccRtpManager* NewL( MAsyncEventHandler& aHandler, 
                                     MMccResources& aMccResources,
                                     TUint32 aMccSessionId );

		/**
        * Destructor.
        */
        virtual ~CMccRtpManager();

	public:	// New functions

        /**
        * Create RTP session.
        * @since Series 60 3.0
        * @param aRemoteAdd	- [input] Remote IP-address
        * @param aPort
        * - [input] Local UDP/IP port number proposed to use 
        * - [output] Local UDP/IP port number allocated
		* @param aEnableRTCP - [input] Enable RTCP handling for the session
        * @param aSecure - [input]   Indicates is secure session to be created
        * @param aIncreaseDefaultSocketSize - [input] ETrue if default socket size
        *           of RTP stack should be increased.
        * @return void
        */
        void CreateSessionL( TUint& aPort, 
                             TBool aEnableRTCP, 
                             TBool aSecure, 
                             TBool aIncreaseDefaultSocketSize = EFalse );

		/**
        * Close RTP session.
        * @since Series 60 3.0
        * @param None
        * @return None
        */
        void CloseSession();

        /**
        * Get RTPApi reference.
        * @since Series 60 3.0
        * @param None
        * @return pointer to RtpApi
        */
        CRtpAPI* GetRtpReferenceL();

        /**
        * Sets remote address.
        * @since Series 60 3.0
		* @param aRemoteAddress	- [input] Remote IP-address
        * @return KErrNone if successful
        */
        TInt SetRemoteAddress( const TInetAddr& aRemoteAddress );

		/**
        * Sets remote address.
        * @since Series 60 3.0
		* @param aRemoteAddress	- [input] Remote IP-address
        * @return KErrNone if successful
        */
        TInt SetRemoteRtcpAddr( const TInetAddr& aRemoteAddress );
        
        /**
        * Sets TOS flag, must be called before StartSession
        * @since Series 60 3.0
        * @param aIpTOS	- [input] TOS
        * @return void
        */
        void SetIpTOS( TInt aIpTOS );

        /**
        * Starts RTP session.
        * @since Series 60 3.0
        * @param None
        * @return void
        */
        void StartSessionL();

        /**
        * Initializes RTP session.
        * @since Series 60 3.0
        * @param aStatus - Async status
        * @param aIapId	- [input] Iap ID
        * @return KErrNone if successful
        */
        void InitializeL( TRequestStatus& aStatus, TInt aIapId );
		
        /**
        * Returns the session ID for this RTP manager object
        * @since Series 60 3.0
        * @param None
        * @return TRtpId Session Identifier
        */
        TRtpId SessionId() const;
        
        /**
        * Creates a new transmit stream
        * @since Series 60 3.0
        * @param aSink RTP Datasink
        * @param aCodecInfo Codec information
        * @return void
        */
        TUint32 CreateTransmitStreamL( MDataSink& aSink, 
                const TMccCodecInfo& aCodecInfo );
        
        /**
        * Creates a new receive stream
        * @since Series 60 3.0
        * @param MDataSource RTP datasource
        * @param aCodecInfo Codec information
        * @retun void
        */ 
        TUint32 CreateReceiveStreamL( MDataSource& aSource, 
                const TMccCodecInfo& aCodecInfo );

        /**
        * Sets RTCP sending and receiving.
        * @param aSend [input] ETrue if automatic RTCP sending and 
        *        RTCP receiving is enabled
        * @return KErrNone if successful
        */
        TInt SetRTCPSendReceive( TBool aEnableRtcp );
        
        /**
        * Sets id of the link where the rtp manager belongs.
        */
        void SetLinkId( TUint32 aLinkId );
        
        /**
        * Updates rtp manager.
        */
        void UpdateL( const TMccCodecInfo* aCodecInfo = 0 );

        /**
         * Resolves local IP address and ports from RTP API.
         * For NATFW support.
         * @since Series 60 3.2
         * @param aClientData Client data
         * @return void
         */
        void GetLocalIpAddressesL( TMccCreateLink& aClientData );

        /*
        * Sets RTP CName.
        */
        void SetCNameL( const TDesC8& aCName );

        /**
        * Returns the MCC session ID
        * @since Series 60 3.0
        * @param None
        * @return TRtpId Session Identifier
        */
        TUint32 MccSessionId() const;
        
    public: // Functions from base classes

        /**
        * From MRtpErrNotify Error call back.
        */
        void ErrorNotify( TInt aErrCode );
        
                                 
        /**
         * From MMccQosEventObserver
         */        
        void QoSParamEvent( TInt aError, TUint32 aEvent );

        /**
         * From MMccQosEventObserver
         */
        void QoSConnEvent( TInt aError, TUint32 aEvent );

    private:

        /**
        * C++ default constructor.
        */
        CMccRtpManager( MAsyncEventHandler& aHandler,
                        MMccResources& aMccResources,
                        TUint32 aSessionId );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
        * Remove RTP session from SessionArray.
        * @since Series 60 3.0
        * @param aStreamId - [input] RTP stream ID
        * @return None
        */
        void RemoveStreamFromArray( const TRtpId& aStreamId );

        /**
        * Check Session Exists.
        * @since Series 60 3.0
        * @param aStreamId - [input] RTP stream ID
        * @return TBool
        */
        TBool CheckStreamExists( const TRtpId& aStreamId ) const;

        /**
        * Check RTP Api is open.
        * @since Series 60 3.0
        * @param None
        * @return TBool
        */
        TBool IsOpen() const;

    private:    // Data

        // Session ID RTP manager handles one RTP session
        TRtpId iRtpSessionId;
        
        // Stream Array. One RTP session may have many RTP streams
        RArray<TRtpId> iStreamArray;

        // RTPApi instance
        CRtpAPI* iRtpApi;
        
        // Eventhandler for errors
        MAsyncEventHandler& iEventHandler;
        
        MMccResources& iMccResources;
        
        // TOS bit setting
        TInt iIpTOS;
        
        // Source description
        TRtpSdesParams iRtpSdes;
        
        // Local port
        TUint iLocalPort;
        
        // RTCP On/Off status
        TBool iRtcpEnabled;
        
        // Remote address for this session
        TInetAddr iRemoteAddress;
        
        TInetAddr iRemoteRtcpAddr;
        
        CMccRtcpReceiver* iRtcpReceiver;
        
        CMccRtpKeepaliveContainer* iRtpKeepaliveMechanism;
        
        // SocketServ
        RSocketServ iSocketServ;
        
        // Connection
        RConnection iConnection;
        
        TUint32 iLinkId;
        
        // MCC session ID
        TUint32 iMccSessionId;
        
        TRtpId iDummyRcvStreamId;
        
        /**
         * Pointer to SRTP session
         * Own.
         */
        CSRTPSession* iSecSession;
        
        /**
         * Pointer to QoS Plugin
         * Own.
         */
        CMccQosHandler* iQosHandler;

        HBufC8* iCName;
	#ifdef EUNIT_TEST
        friend class UT_CMccRtpManager;
        friend class UT_CMccSymSubthreadClient;
    #endif

	};

#endif	// MCCRTPMANAGER_H

// End of File
