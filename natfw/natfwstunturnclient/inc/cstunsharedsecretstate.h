/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef C_CSTUNSHAREDSECRETSTATE_H
#define C_CSTUNSHAREDSECRETSTATE_H

// INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class CNATFWUNSAFMessage;
class CSTUNSharedSecret;

// CLASS DECLARATION

/**
 *  Base class for CSTUNSharedSecret's states. 
 */
class CSTUNSharedSecretState : public CBase
    {
    public:

          /**
        * Destructor
        */
        virtual ~CSTUNSharedSecretState();

        /**
        * TLS connection successfully established.
        * @param aContext SharedSecret instance who received the event
        */
        virtual void TlsConnectedL( CSTUNSharedSecret& aContext ) const;

        /**
        * An UNSAF message has been received from the TLS connection.
        * @param aContext SharedSecret instance who received the event
        * @param aMessage incoming message, the ownership is transferred.
        */
        virtual void IncomingMessageL( CSTUNSharedSecret& aContext,
                                       CNATFWUNSAFMessage* aMessage ) const;

        /**
        * TLS connection establishment has failed or 
        * sending or receiving using the connection has failed or
        * there was a leave in TlsConnectedL or IncomingMessageL.
        * @param aContext SharedSecret instance who received the event
        * @param aError a reason for the failure.
        */
        virtual void ErrorOccured( CSTUNSharedSecret& aContext,
                                   TInt aError ) const;

        virtual void TimerExpiredL( CSTUNSharedSecret& aContext ) const;

    protected:

        CSTUNSharedSecretState();

    private:

        CSTUNSharedSecretState( const CSTUNSharedSecretState& aState );
    };

#endif // C_CSTUNSHAREDSECRETSTATE_H
