/*
* Copyright (c) 2004-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Provides a comfort noise generator class
*
*/


#ifndef MCCMSRPDATAOBSERVER_H
#define MCCMSRPDATAOBSERVER_H

#include <e32base.h>

/**
*  callback function for received msrp packets. 
*
*  @lib mccmsrpsourcesink.dll
*/
class MMsrpDataObserver
{
public :

        /**
         * A MSRP message has been received from the network.
         * This function is called when the MSRP message was received from the network
         * the ownership is transferred
         * @param aMessage the received message
         * @param aStatus, KErrNone with complete message, KErrCancel if terminated
         */
        virtual void IncomingMessage( HBufC8* aMessageContent, TInt aStatus ) = 0;

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
            TInt aRangeEnd, TInt aTotal ) = 0;

        /**
         * The SendMessage status of the message sending
         * The possible status code are defined by CMSRPConnection::TMSRPErrorCodes
         * or system wide error codes
         * @param aStatus status of the operation
         * @param aMessageId message identifier of the sent message
         */
        virtual void SendResult( TInt aStatus, const TDesC8& aMessageid ) = 0;

        /**
         * If listen port request fails this is called
         * @param aStatus error code of the operation
         */
        virtual void ListenPortFailure( TInt aStatus ) = 0;

};

#endif //MCCMSRPDATAOBSERVER.H
