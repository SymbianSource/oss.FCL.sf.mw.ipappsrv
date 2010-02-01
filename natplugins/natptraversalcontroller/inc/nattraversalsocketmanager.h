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


#ifndef MNATTRAVERSALSOCKETMANAGER_H
#define MNATTRAVERSALSOCKETMANAGER_H

// FORWARD DECLARATIONS
class MNATTraversalSocketUser;

// CLASS DECLARATION
/**
* An interface for asking for a permission to use a socket for sending.
*
*  @lib NATTraversalController.lib
*/
class MNATTraversalSocketManager
    {
public:

    virtual TBool AddToSendingQueueL( MNATTraversalSocketUser& aUser ) = 0;

    virtual void SendingCompleted( MNATTraversalSocketUser& aUser ) = 0;
    };

#endif // MNATTRAVERSALSOCKETMANAGER_H

// End of File
