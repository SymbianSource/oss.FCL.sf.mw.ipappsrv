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

#ifndef CCRLFSENDER_H
#define CCRLFSENDER_H

//  INCLUDES
#include <e32base.h>
#include <es_sock.h>
#include <securesocket.h>

// FORWARD DECLARATIONS
class MNATBindingRefresherObserver;


// CLASS DECLARATION
/**
* A base class for sending CRLF bursts to network.
*/
class CCRLFSender : public CActive
    {
    public:    // Constructors and destructor

        virtual ~CCRLFSender();

    public: // New functions

        virtual TBool PersistentConnectionRequired() const;

        virtual TBool HasSocket( const RSocket& aSocket ) const;

        virtual TBool HasSocket( const CSecureSocket& aSocket ) const;

        void Send();

    private: // From CActive

        void RunL();

    protected: // Constructors

        CCRLFSender( MNATBindingRefresherObserver& aObserver );

    protected: // New functions

        virtual void Send( const TDesC8& aData ) = 0;

        virtual TBool ReadyToSend();

        virtual void SendingCompleted();

    protected: // Data

        MNATBindingRefresherObserver& iObserver;
    };

#endif // CCRLFSENDER_H

// End of File
