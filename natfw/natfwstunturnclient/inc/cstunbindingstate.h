/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef C_CSTUNBINDINGSTATE_H
#define C_CSTUNBINDINGSTATE_H

// INCLUDES
#include <e32base.h>
#include <in_sock.h>

// FORWARD DECLARATIONS
class CBindingImplementation;

// CLASS DECLARATION

/**
 *  Base class for CSTUNBinding's states. 
 */
class CSTUNBindingState : public CBase
    {
    public:
          /**
        * Destructor
        */
        virtual ~CSTUNBindingState();

        /**
        * Shared secret is available.
        * @param aContext Binding instance who received the event
        * @param aUsername Username
        * @param aPassword Password
        */
        virtual void
            SharedSecretObtainedL( CBindingImplementation& aContext,
                                   const TDesC8& aUsername,
                                   const TDesC8& aPassword ) const;

        /**
        * Transaction received a Binding Response, and there was no error.
        * @param aContext Binding instance who received the event
        * @param aPublicAddress Public address to use
        */
        virtual void
            PublicAddressReceivedL( CBindingImplementation& aContext,
                                    const TInetAddr& aPublicAddress ) const;

        /**
         * Transaction received a Binding Response, and there was no error.
         *
         * @since   s60 3.2
         * @param   aContext Binding instance who received the event
         * @param   aReflexiveAddr  Public address to use
         * @param   aRelayAddr      Stun RELAY address to use
         */
        virtual void
            PublicAddressReceivedL( CBindingImplementation& aContext,
                                    const TInetAddr& aReflexiveAddr,
                                    const TInetAddr& aRelayAddr ) const;
    
        /**
        * Transaction ended unsuccessfully.
        * @param aContext Binding instance who received the event
        * @param aError Reason for the failure
        */
        virtual void TransactionError( CBindingImplementation& aContext,
                                       TInt aError );

        virtual void
            TimerExpiredL( CBindingImplementation& aContext ) const;

        /**
        * Refreshes the binding causing a new Binding Request to be sent to the
        * STUN server
        * @param aContext Binding instance who received the event
        */
        virtual void SendRequestL( CBindingImplementation& aContext ) const;
        

        /**
        * Cancels a previously issued call to SendRequestL or AllocateRequestL.
        */
        virtual void
            CancelRequest( CBindingImplementation& aContext ) const;

    protected:

        CSTUNBindingState();

    private:

        CSTUNBindingState( const CSTUNBindingState& aState );
    };

#endif // C_CSTUNBINDINGSTATE_H
