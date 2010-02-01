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





#ifndef MNATFWUNSAFMSGASSEMBLEROBSERVER_H
#define MNATFWUNSAFMSGASSEMBLEROBSERVER_H

//  INCLUDES
#include <e32std.h>

// FORWARD DECLARATIONS
class CNATFWUNSAFMessage;

// CLASS DECLARATION
/**
* An interface for notifications about UNSAF message assembling.
*
*  @lib natfwunsafprotocols.lib
*/
class MNATFWUNSAFMsgAssemblerObserver
    {
    public:    // New functions

        /**
        * Assembled complete UNSAF message
        * @param aMessage assembled message, the ownership is transferred.
        * @param aStatus possible error can be informed with the param
        */
        virtual void NATFWUNSAFMsgCompleteL( CNATFWUNSAFMessage* aMessage,
                                        TInt aStatus ) = 0;

    };

#endif // MNATFWUNSAFMSGASSEMBLEROBSERVER_H


