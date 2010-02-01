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





#ifndef MNATFWUNSAFUDPSENDEROBSERVER_H
#define MNATFWUNSAFUDPSENDEROBSERVER_H

//  INCLUDES
#include <e32std.h>

// FORWARD DECLARATIONS


// CLASS DECLARATION
/**
* An interface for notifications about UNSAF messages sent to
* network using UDP.
*
*  @lib natfwunsafprotocols.lib
*/
class MNATFWUNSAFUdpSenderObserver
    {
    public:    // New functions

        /**
        * The UDP socket has succesfully completed sending a UNSAF message.
        */
        virtual void UNSAFUdpMessageSentL() = 0;

        /**
        * The UDP socket has failed sending a UNSAF message or
        * there was a leave in NATFWUNSAFUdpMessageSentL.
        * @param aError a reason for the failure.
        */
        virtual void UNSAFUdpMessageFailure(TInt aError) = 0;
    };

#endif // MNATFWUNSAFUDPSENDEROBSERVER_H


