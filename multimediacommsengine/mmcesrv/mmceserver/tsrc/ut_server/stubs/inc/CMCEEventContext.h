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
* Description:    Base class for event contexts. Contains methods for
*                creating the state array and controlling current state.
*
*/





#ifndef CMCEEVENTCONTEXT_H
#define CMCEEVENTCONTEXT_H

//  INCLUDES
#include <e32base.h>
#include "mcecomevent.h"
#include "mcecomeventconstants.h"

// FORWARD DECLARATIONS

class CMceComEvent;

// CLASS DECLARATION

/**
*  Base class for event contexts.
*
*  @lib 
*  @since Series 60 3.0
*/
class CMceEventContext : public CBase 
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        
        static CMceEventContext* NewL( CMceComEvent& aEvent );
        
        /**
        * Destructor.
        */
        ~CMceEventContext();

    public: // New functions
        
        /**
        * Returns the current state of the context
        * @since Series 60 3.0
        * @param None.
        * @return TMceEventStateIndex the current state, index to it.
        */
        TMceEventStateIndex CurrentState();
        
        /**
        * C++ default constructor.
        */
        CMceEventContext();  
          
    public:    // Data

        // Current state
        TMceEventStateIndex iCurrentState;
    };

#endif      // CMCEEVENTCONTEXT_H
            
// End of File
