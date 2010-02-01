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

#ifndef CUDPCRLFSENDER_H
#define CUDPCRLFSENDER_H

//  INCLUDES
#include "ccrlfsender.h"
#include "nattraversalsocketuser.h"

// FORWARD DECLARATIONS
class MNATTraversalSocketManager;

// CLASS DECLARATION
/**
* A class for sending CRLF bursts to network using UDP socket.
*/
class CUdpCRLFSender :
    public CCRLFSender,
    public MNATTraversalSocketUser
    {
    public:    // Constructors and destructor

        CUdpCRLFSender(
            RSocket& aSocket,
            const TInetAddr& aAddr,
            MNATBindingRefresherObserver& aObserver,
            MNATTraversalSocketManager& aSocketManager );

        ~CUdpCRLFSender();

    public: // From CCRLFSender

        TBool PersistentConnectionRequired() const;

        TBool HasSocket( const RSocket& aSocket ) const;

    public: // From MNATTraversalSocketUser

        const RSocket& Socket() const;

        void SendingAllowed();

        TBool IsSending() const;

        void CancelSending();

    protected: // From CActive

        void DoCancel();

    protected: // From CCRLFSender

        void Send( const TDesC8& aData );

        TBool ReadyToSend();

        void SendingCompleted();

    private: // Data

        RSocket& iSocket;
        TInetAddr iAddr;
        MNATTraversalSocketManager& iSocketManager;
        TBool iAllowedToSend;
    };

#endif // CUDPCRLFSENDER_H

// End of File
