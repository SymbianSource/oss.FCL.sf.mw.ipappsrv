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
* Description:    Initial state for MO refer.
*
*/





#ifndef CMCEEVENTREFERMOIDLESTATE_H
#define CMCEEVENTREFERMOIDLESTATE_H

// INCLUDES
#include    "mceeventstatebase.h"

// CLASS DECLARATION

/**
*  Initial state for MO refer.
*
*  @lib 
*  @since
*/
class CMceEventReferMoIdleState : public CMceEventStateBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        CMceEventReferMoIdleState();

        /**
        * Destructor.
        */
        ~CMceEventReferMoIdleState();
        
    protected:  // Functions from base classes
       
        /**
        * From CMceEventStateBase Apply concrete state.
        * This function handles the action from the client.
        */
        void HandleL( CMceComEvent& aEvent );
        
        /**
        * From CMceEventStateBase Apply concrete state.
        * This function handles the request received from the SIP stack.
        * @param aEvent, a reference to CMceComEvent
        */
		void HandleReceiveRequestL( CMceComEvent& aEvent );
           
        /**
        * From CMceEventStateBase Apply concrete state.
        * This function handles the response or error received from the 
        * SIP stack.
        * @param aEvent, a reference to CMceComEvent
        */
        void HandleResponseReceivedL( CMceComEvent& aEvent );

        
    private://EUNIT
        friend class UT_CMceEventReferMoIdleState;
    };

#endif      // CMCEEVENTREFERMOIDLESTATE_H
            
// End of File
