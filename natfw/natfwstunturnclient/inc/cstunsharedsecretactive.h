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




#ifndef C_CSTUNSHAREDSECRETACTIVE_H
#define C_CSTUNSHAREDSECRETACTIVE_H

// INCLUDES
#include <e32base.h>
#include "cstunsharedsecretstate.h"

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
 *  The shared secret has been obtained and can be used.
 */
class CSTUNSharedSecretActive : public CSTUNSharedSecretState
    {
    public: // Constructors and destructor

        /**
        * Destructor
        */
        CSTUNSharedSecretActive()
            {
            }

          /**
        * Destructor
        */
        ~CSTUNSharedSecretActive()
            {
            }
    
    private:

        /**
        * Copy Destructor has dummy implementation, as it is declared private
        * and not used.
        */
        CSTUNSharedSecretActive( const CSTUNSharedSecretActive& /*aActive*/ ) :
            CSTUNSharedSecretState()
            {
            }
    };

#endif // C_CSTUNSHAREDSECRETACTIVE_H
