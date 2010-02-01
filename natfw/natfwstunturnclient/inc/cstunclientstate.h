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




#ifndef C_CSTUNCLIENTSTATE_H
#define C_CSTUNCLIENTSTATE_H

// INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class CSTUNClientImplementation;
class CBinding;

// CLASS DECLARATION

/**
 *  Base class for CSTUNClientImplementation's states.
 *
 *  @lib ?library
 *  @since S60 ?S60_version *** for example, S60 v3.0
 */
class CSTUNClientState : public CBase
    {
    public:
          /**
        * Destructor
        */
        virtual ~CSTUNClientState();

        /**
        * Resolving STUN server address has succeeded.
        * @param aContext STUN client context
        * @param aObtainSharedSecret ETrue if a shared secret must be obtained
        *        EFalse otherwise
        */
        virtual void
            ResolvingCompletedL( CSTUNClientImplementation& aContext,
                                 TBool aObtainSharedSecret ) const;

        /**
        * Resolving STUN server address has failed.
        * @param aContext STUN client context
        * @param aError The reason for the failure.
        */
        virtual void ResolvingFailed( CSTUNClientImplementation& aContext,
                                      TInt aError ) const;        
        
        /**
        * A new shared secret has been succesfully obtained.
        * @param aContext STUN client context
        */
        virtual void
            SharedSecretReceivedL( CSTUNClientImplementation& aContext ) const;
        
        /**
        * Error occurred when trying to obtain a shared secret.
        * @param aContext STUN client context
        * @param aError The reason for the failure.
        */
        virtual void SharedSecretErrorL( CSTUNClientImplementation& aContext,
                                         TInt aError ) const;

        /**
        * Binding requests a shared secret.
        * @param aContext STUN client context
        * @param aBinding Binding asking for the shared secret.
        */
        virtual void ObtainSharedSecretL( CSTUNClientImplementation& aContext,
                                           CBinding& aBinding ) const;

        /**
         * A STUN server has rejected certain username and password.
         * @param aBinding Binding that was using the username and password.
         * @param aUsername Username that was rejected
         * @param aPassword Password that was rejected
         * @return ETrue if the Binding should retry, EFalse if binding should
         *           terminate.
         */
        virtual TBool
            SharedSecretRejectedL( CSTUNClientImplementation& aContext,
                                   const CBinding& aBinding,
                                   const TDesC8& aUsername,
                                      const TDesC8& aPassword ) const;

    protected:

        CSTUNClientState();

    private:

        CSTUNClientState( const CSTUNClientState& aState );
    };

#endif // C_CSTUNCLIENTSTATE_H
