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




#ifndef C_CSTUNBINDINGACTIVE_H
#define C_CSTUNBINDINGACTIVE_H

// INCLUDES
#include "cstunbindingstate.h"

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
 *  In this state, the binding is active and has a public address mapping
 *
 *  @lib ?library
 *  @since S60 ?S60_version *** for example, S60 v3.0
 */
class CSTUNBindingActive : public CSTUNBindingState
    {
    public: // Constructors and destructor

        /**
        * Constructor
        * @param aGetSharedSecret GetSharedSecret state
        */
        CSTUNBindingActive( CSTUNBindingState& aGetSharedSecret );

          /**
        * Destructor
        */
        virtual ~CSTUNBindingActive();

    public: // From CSTUNBindingState

        void SendRequestL( CBindingImplementation& aContext ) const;

    private:

        CSTUNBindingActive();

        CSTUNBindingActive( const CSTUNBindingActive& aBindingActive );

    private: // Data

        //Next possible states
        CSTUNBindingState& iGetSharedSecret;
    };

#endif // C_CSTUNBINDINGACTIVE_H
