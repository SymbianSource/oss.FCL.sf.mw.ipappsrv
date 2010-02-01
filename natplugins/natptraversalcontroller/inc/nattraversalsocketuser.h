/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef MNATTRAVERSALSOCKETUSER_H
#define MNATTRAVERSALSOCKETUSER_H

// FORWARD DECLARATIONS
#include <e32std.h>
#include <es_sock.h>

// CLASS DECLARATION
/**
* An interface for giving a permission to use a socket for sending.
*
*  @lib NATTraversalController.lib
*/
class MNATTraversalSocketUser
    {
public:

    virtual const RSocket& Socket() const = 0;

    virtual void SendingAllowed() = 0;

    virtual TBool IsSending() const = 0;

    virtual void CancelSending() = 0;
    };

#endif // MNATTRAVERSALSOCKETUSER_H

// End of File
