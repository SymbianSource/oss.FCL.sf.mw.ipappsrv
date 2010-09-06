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




#ifndef CMSRPCALLBACKMGROBSERVER_H
#define CMSRPCALLBACKMGROBSERVER_H

#include "mccmsrpconnobserver.h"
#include "mccmsrpdataobserver.h"
#include "MMSRPSessionObserver.h"
//#include "mmfcontrollerframework.h"
#include <mmf/common/mmfcontrollerframework.h>
#include <mmccevents.h>

/**
*  callback function for received msrp packets. 
*
*  @lib mccmsrpsourcesink.dll
*/
class CMsrpCallbackMgrObserver : public CBase, public MMSRPSessionObserver
    {
	
	public:
		static CMsrpCallbackMgrObserver* NewL(MAsyncEventHandler& aEventHandler);
		
    public:
        
        /**
         * A MSRP message has been received from the network.
         * This function is called when the MSRP message was received from the network
         * the ownership is transferred
         * @param aMessage the received message
         * @param aStatus, KErrNone with complete message, KErrCancel if terminated
         */
        virtual void IncomingMessage( HBufC8* aMessageContent, TInt aStatus );

        /**
         * A MSRP REPORT message has been received from the network
         * This function is called to indicate to the client that a recipient
         * wishes to indicate a range of bytes as received. Alternative the
         * recipient might use REPORT to indicate an error. In case of error the
         * MSRP stops sending the message. However, it is client's responsibility
         * how the handle the information
         * @param aStatus status code from the REPORT message
         * @param aRangeStart start of byte range received
         * @param aRangeEnd end of byte range received
         * @param aTotal Total number of bytes in the message
         */
        virtual void IncomingReport( TInt aStatus, TInt aRangeStart,
            TInt aRangeEnd, TInt aTotal );

        /**
         * The SendMessage status of the message sending
         * The possible status code are defined by CMSRPConnection::TMSRPErrorCodes
         * or system wide error codes
         * @param aStatus status of the operation
         * @param aMessageId message identifier of the sent message
         */
        virtual void SendResult( TInt aStatus, const TDesC8& aMessageid );

        /**
         * If listen port request fails this is called
         * @param aStatus error code of the operation
         */
        virtual void ListenPortFailure( TInt aStatus );

        /**
         * When ConnectL function is used to establish a connection to remote
         * user, this is used to return the connection status. After ConnectL
         * the client must wait for this callback before sending or listening
         * messages. This is called also if the SendMessageL method is called
         * directly before establishing a connection
         * @param aStatus system-wide error code
         */
        virtual void ConnectStatus( TInt aStatus );
        
        /* Notifies that the entire file has been sent successfully */
        void SendFileNotification(TBool aStatus) ;
        
        /* Notifies that the entire file has been received successfully */
        void ReceiveFileNotification(TBool status) ;
        /**
        * File data transfer progress. returns the progress of data transfer
        * @param aTransferred number of bytes transferred
        * @param aTotal Total amount of bytes to be transferred
        */
        void FileSendProgress( TInt aTransferred, TInt aTotal ) ;

        /**
        * File receive progress, number of bytes received from incoming data
        * @param aReceived number of bytes received
        * @param aTotal Total amount of bytes to be received
        */
       void FileReceiveProgress( TInt aReceived, TInt aTotal );
		
		public:
		
		MMsrpConnectionObserver *iConnObserver;
		MMsrpDataObserver *iDataObserver;
		
		void RegisterConnectionObserver (MMsrpConnectionObserver& aObserver);
		void RegisterDataObserver (MMsrpDataObserver& aObserver);
		
		void ConstructL();
		
		private:
		
		CMsrpCallbackMgrObserver (MAsyncEventHandler& aEventHandler);
		TMccEvent  iMccEvent;
		
        // Eventhandler for errors
        MAsyncEventHandler& iEventHandler;
    };
	
#endif // End of MMSRPCALLBACKMGROBSERVER.H
