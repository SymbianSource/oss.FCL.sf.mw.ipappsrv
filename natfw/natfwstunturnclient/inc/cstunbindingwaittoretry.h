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




#ifndef C_CSTUNBINDINGWAITTORETRY_H
#define C_CSTUNBINDINGWAITTORETRY_H

// INCLUDES
#include "cstunbindingstate.h"

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
 *  In this state, binding waits a while before retrying to send a Binding
 *  Request. This state is entered after a Binding Error Response with code
 *  5xx is received. 
 */
class CSTUNBindingWaitToRetry : public CSTUNBindingState
    {
    public: // Constructors and destructor

        /**
        * Constructor
        * @param aGetSharedSecret GetSharedSecret state
        */
        CSTUNBindingWaitToRetry( CSTUNBindingState& aGetSharedSecret );

          /**
        * Destructor
        */
        virtual ~CSTUNBindingWaitToRetry();

    public: // From CSTUNBindingState

        void TimerExpiredL( CBindingImplementation& aContext ) const;

        void SendRequestL( CBindingImplementation& aContext ) const;
        
    private:

        CSTUNBindingWaitToRetry();

        CSTUNBindingWaitToRetry( const CSTUNBindingWaitToRetry& aWaitToRetry );

    private: // Data

        //Next possible states
        CSTUNBindingState& iGetSharedSecret;
    };

#endif // C_CSTUNBINDINGWAITTORETRY_H
