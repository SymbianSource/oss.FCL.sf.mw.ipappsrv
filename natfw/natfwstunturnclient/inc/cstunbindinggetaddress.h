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




#ifndef C_CSTUNBINDINGGETADDRESS_H
#define C_CSTUNBINDINGGETADDRESS_H

// INCLUDES
#include "cstunbindingstate.h"

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
 *  In this state, the binding the transaction to complete and return a
 *  public address.
 *
 *  @lib ?library
 *  @since S60 ?S60_version *** for example, S60 v3.0
 */
class CSTUNBindingGetAddress : public CSTUNBindingState
    {
    public: // Constructors and destructor

        /**
        * Constructor        
        */
        CSTUNBindingGetAddress();

          /**
        * Destructor
        */
        virtual ~CSTUNBindingGetAddress();

    public: // From CSTUNBindingState

        void PublicAddressReceivedL( CBindingImplementation& aContext,
                                     const TInetAddr& aPublicAddress ) const;
        
        // Overloaded for stun relay usage
        void PublicAddressReceivedL( CBindingImplementation& aContext,
                                     const TInetAddr& aReflexiveAddr,
                                     const TInetAddr& aRelayAddr ) const;
                                       

        void TransactionError( CBindingImplementation& aContext,
                               TInt aError );

        void SendRequestL( CBindingImplementation& aContext ) const;
        

    public: // New functions
    
        /**
        * Set the neighbour states.
        * @param aGetSharedSecret GetSharedSecret state
        * @param aWaitToRetry WaitToRetry state
        * @param aActive Active state
        */
        void SetNeighbourStates( CSTUNBindingState& aGetSharedSecret,
                                 CSTUNBindingState& aWaitToRetry,
                                 CSTUNBindingState& aActive );

    private:

        CSTUNBindingGetAddress( const CSTUNBindingGetAddress& aGetAddress );

    private: // Data

        //Next possible states, not owned
        CSTUNBindingState* iActive;
        CSTUNBindingState* iWaitToRetry;
        CSTUNBindingState* iGetSharedSecret;
        
        /**
         * Error buffer to keep record of occurred binding errors.
         */
        RArray<TInt> iErrorBuffer;
    };

#endif // C_CSTUNBINDINGGETADDRESS_H
