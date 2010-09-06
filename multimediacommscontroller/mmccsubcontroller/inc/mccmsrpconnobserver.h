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

#ifndef MMSRPCONNECTIONOBSERVER_H
#define MMSRPCONNECTIONOBSERVER_H

#include <e32base.h>

/**
*  callback function for received msrp packets. 
*
*  @lib mccmsrpsourcesink.dll
*/
class MMsrpConnectionObserver
{
public :

        /**
         * When ConnectL function is used to establish a connection to remote
         * user, this is used to return the connection status. After ConnectL
         * the client must wait for this callback before sending or listening
         * messages. This is called also if the SendMessageL method is called
         * directly before establishing a connection
         * @param aStatus system-wide error code
         */
        virtual void ConnectStatus( TInt aStatus ) = 0;
        
        /* Notifies that the entire file has been sent successfully */
        virtual void SendFileNotification(TBool aStatus) = 0 ;
                
        /* Notifies that the entire file has been received successfully */
        virtual void ReceiveFileNotification(TBool status) = 0;
        /**
        * File data transfer progress. returns the progress of data transfer
        * @param aTransferred number of bytes transferred
        * @param aTotal Total amount of bytes to be transferred
        */
       // virtual void FileSendProgress( TInt aTransferred, TInt aTotal ) = 0 ;

        /**
        * File receive progress, number of bytes received from incoming data
        * @param aReceived number of bytes received
        * @param aTotal Total amount of bytes to be received
        */
       // virtual void FileReceiveProgress( TInt aReceived, TInt aTotal ) = 0;

};

#endif MMSRPCONNECTIONOBSERVER.H
