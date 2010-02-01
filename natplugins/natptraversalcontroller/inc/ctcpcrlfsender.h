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

#ifndef CTCPCRLFSENDER_H
#define CTCPCRLFSENDER_H

//  INCLUDES
#include "ccrlfsender.h"

// FORWARD DECLARATIONS


// CLASS DECLARATION
/**
* A class for sending CRLF bursts to network using TCP socket.
*/
class CTcpCRLFSender : public CCRLFSender
    {
    public:    // Constructors and destructor

        CTcpCRLFSender(
            RSocket& aSocket,
            MNATBindingRefresherObserver& aObserver );

        ~CTcpCRLFSender();

    public: // From CCRLFSender

        TBool HasSocket( const RSocket& aSocket ) const;

    protected: // From CActive

        void DoCancel();

    protected: // From CCRLFSender

        void Send( const TDesC8& aData );

    private: // Data

        RSocket& iSocket;
    };

#endif // CTCPCRLFSENDER_H

// End of File
