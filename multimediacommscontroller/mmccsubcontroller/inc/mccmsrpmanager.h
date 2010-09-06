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
* Description:    Provides DTMF support
*
*/

#ifndef MCCMSRPMANAGER_H
#define MCCMSRPMANAGER_H

//  INCLUDES
//MSRP Headers
#include <CMSRP.h>
#include <CMSRPSession.h>
#include <MsrpCommon.h>
#include <MMSRPSessionObserver.h>
#include "msrpcallbackmgrobserver.h"
#include "mccqoseventobserver.h"
#include "mccmsrpconnobserver.h"

// FORWARD DECLARATIONS
class MAsyncEventHandler;
class MDataSink;
class MDataSource;
class TMccCodecInfo;
class MMccResources;
class TMccCreateLink;


// CLASS DECLARATION

/**
*  Mcc MSRP Manager implements Mcc MSRP API handling.
*
*  @since Series 60 3.0
*/
NONSHARABLE_CLASS( CMccMsrpManager ) : public CBase, public MMsrpConnectionObserver
	{
	public:	// Constructors and destructor

		/**
        * Two-phased constructor.
        */
        static CMccMsrpManager* NewL( MAsyncEventHandler& aHandler, 
                MMccResources& aMccResources, TUid aUID);

		/**
        * Destructor.
        */
        virtual ~CMccMsrpManager();
        
        /**
        * Sets id of the link where the msrp manager belongs.
        */
        void SetLinkId( TUint32 aLinkId );
        
	public: // functions from MMSRPSessionObserver
	    
        /**
         * When ConnectL function is used to establish a connection to remote
         * user, this is used to return the connection status. After ConnectL
         * the client must wait for this callback before sending or listening
         * messages. This is called also if the SendMessageL method is called
         * directly before establishing a connection
         * @param aStatus system-wide error code
         */
        void ConnectStatus( TInt aStatus );
        
        /* Notifies that the entire file has been sent successfully */
        void SendFileNotification(TBool aStatus);
                        
                /* Notifies that the entire file has been received successfully */
        void ReceiveFileNotification(TBool status);
	    
	    //Connection Status
	    TInt iConnectStatus;
  

	public:	// functions from CMSRPSession.h
	    
	     
	    // Returns the local MSRP path that is used in the "a" line
	    // during SDP negotiation.
	    TDes8& GetLocalMSRPPath();
	    
	    void SetLocalMSRPPath(TDesC8& aMsrpPath );
	    
	    
	    // Set the various parameters related to an MSRP session
	    // eg: The Success and Failure report headers sent in messages
	    TInt SetSessionParams( CMSRPSessionParams& aSessionParams );
	    
	    // === Post SDP Negotiation; Connection Setup.
	        
	    TInt Connect( const TDesC8& aRemoteMsrpPath );
	    
	    TInt Listen ( const TDesC8& aRemoteMsrpPath );
	    

	public : // New functions from CMSRP
        
        CMSRPSession* CreateMSRPSessionL( MMSRPSessionObserver& aObserver, 
                                                       const TUint aIapId ); 
        
        /**
        * Establishes MSRP connection with remote end
        * @since Series 60 3.0
        * @param aRemoteMsrpPath - [input] Remote Msrp Path
        */
        TInt SetRemoteMsrpPathL( const TDes8& aRemoteMsrpPath, const TDes8& aConnStatus );
        
        
        /**
        * Creates a new transmit stream
        * @since Series 60 3.0
        * @param aSink MSRP Datasink
        * @param aCodecInfo Codec information
        * @return void
        */
        TUint32 CreateTransmitStreamL( MDataSink& aSink, 
            const TMccCodecInfo& aCodecInfo );
        
        /**
        * Creates a new receive stream
        * @since Series 60 3.0
        * @param MDataSource MSRP datasource
        * @param aCodecInfo Codec information
        * @retun void
        */ 
        TUint32 CreateReceiveStreamL( MDataSource& aSource, 
                const TMccCodecInfo& aCodecInfo );
        
    public:
        

        /**
         * Extracts local IP address and ports from MSRP LocalPath.
         * @param aClientData Client data
         * @return void
         */
        void GetLocalIpAddressesL( TMccCreateLink& aClientData );
        
        
        /**
         * Pointer to MSRP session
         * Own.
         */
        CMSRPSession* iMsrpSession;
        
        /**
         * Pointer to Msrp call back manager
         * Own.
         */
        CMsrpCallbackMgrObserver* iMsrpObserver;
        
        void SetFileShareAttrbs(HBufC16* aFileName, TInt aFileSize, HBufC8* aFileType,
                TBool aFTProgressNotification);
        
        CMSRPSession* GetMsrpSession();
        
        /*
         * Sets CMSRP Object. The ownership is not transferred
         */
        void SetMsrpObject(CMSRP* aMsrpObject);
        
    private:

        /**
        * C++ default constructor.
        */
        CMccMsrpManager( MAsyncEventHandler& aHandler,
                MMccResources& aMccResources );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL(TUid aUID);

    private:    // Data

        // Not owned. Owned by CMccUIDLLink
        CMSRP* iMsrp;
        
        TUint32 iLinkId;
        
        TBool ifirstTime;
        
        
        // Eventhandler for errors
        MAsyncEventHandler& iEventHandler;
        
        MMccResources& iMccResources;
        MMSRPSessionObserver* iMsrpSessionOb;
        
        TBuf8<150> iLocalMsrpPath;
        HBufC16*   iFileName;
        TInt    iFileSize; 
        HBufC8*  iFileType;
        TBool   iFileShare;
        TBool iFileSendRequested;
        TBool iFileReceiveRequested;
        TBool iIsRegisteredForConnOb;
        TBool iFTProgressNotification;
        TInt  iRefCountForMSRPSession;
	};

#endif	// MCCMSRPMANAGER_H

// End of File
