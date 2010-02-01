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




#ifndef C_CSTUNBINDINGINIT_H
#define C_CSTUNBINDINGINIT_H

// INCLUDES
#include "cstunbindingstate.h"

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
 *  In this state, the binding has been created, but no Binding Request has yet
 *  been sent.
 *
 *  @lib ?library
 *  @since S60 ?S60_version *** for example, S60 v3.0
 */
class CSTUNBindingInit : public CSTUNBindingState
    {
    public: // Constructors and destructor

        /**
        * Constructor
        * @param aGetSharedSecret GetSharedSecret state
        */
        CSTUNBindingInit( CSTUNBindingState& aGetSharedSecret );

          /**
        * Destructor
        */
        virtual ~CSTUNBindingInit();

    public: // From CSTUNBindingState

        void SendRequestL( CBindingImplementation& aContext ) const;
    
    private:

        CSTUNBindingInit();

        CSTUNBindingInit( const CSTUNBindingInit& aInit );

    private: // Data

        //Next possible states
        CSTUNBindingState& iGetSharedSecret;
    };

#endif // C_CSTUNBINDINGINIT_H
