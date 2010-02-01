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
* Description:    Event is terminated.
*
*/





#ifndef CMCEEVENTSUBSCRIBETERMINATEDSTATE_H
#define CMCEEVENTSUBSCRIBETERMINATEDSTATE_H

// INCLUDES
#include    "mceeventstatebase.h"

// CLASS DECLARATION

/**
*  Event is terminated.
*
*  @lib 
*  @since
*/
class CMceEventSubscribeTerminatedState : public CMceEventStateBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        CMceEventSubscribeTerminatedState();

        /**
        * Destructor.
        */
        ~CMceEventSubscribeTerminatedState();
        
    protected:  // Functions from base classes
       
        /**
        * From CMceEventStateBase Apply concrete state.
        * This function handles the action from the client.
        * @param aEvent, a reference to CMceComEvent
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

    private:
    	friend class UT_CMceEventSubscribeTerminatedState;
    };

#endif      // CMCEEVENTSUBSCRIBETERMINATEDSTATE_H
            
// End of File
