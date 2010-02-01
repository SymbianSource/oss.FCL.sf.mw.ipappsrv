/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   
*
*/





#ifndef MNATFWUNSAFTLSTRANSPORTOBSERVER_H
#define MNATFWUNSAFTLSTRANSPORTOBSERVER_H

//  INCLUDES
#include <e32std.h>

// FORWARD DECLARATIONS
class CNATFWUNSAFMessage;

// CLASS DECLARATION
/**
* An interface for notifications about TLS connection statuses and
* incoming UNSAF messages.
*
*  @lib natfwunsafprotocols.lib
*/
class MNATFWUNSAFTlsTransportObserver
    {
    public:    // New functions

        /**
        * TLS connection successfully established and
        * it is ready to send and receive UNSAF messages.
        */
        virtual void TlsConnectedL() = 0;

        /**
        * An UNSAF message has been received from the TLS connection.
        * @param aMessage incoming message, the ownership is transferred.
        */
        virtual void IncomingMessageL( CNATFWUNSAFMessage* aMessage ) = 0;

        /**
        * TLS connection establishment has failed or
        * sending or receiving using the connection has failed or
        * there was a leave in TlsConnectedL or IncomigMessageL.
        * @param aError a reason for the failure.
        */
        virtual void ErrorOccured( TInt aError ) = 0;
    };

#endif // MNATFWUNSAFTLSTRANSPORTOBSERVER_H


