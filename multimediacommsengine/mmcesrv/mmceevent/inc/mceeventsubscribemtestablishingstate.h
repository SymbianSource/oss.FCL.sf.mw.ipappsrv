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
* Description:    MT subscribe is received and waiting for action from client.
*
*/





#ifndef CMCEEVENTSUBSCRIBEMTESTABLISHINGSTATE_H
#define CMCEEVENTSUBSCRIBEMTESTABLISHINGSTATE_H

// INCLUDES
#include    "mceeventstatebase.h"

// CLASS DECLARATION

/**
*  MT subscribe is received and waiting for action from client.
*
*  @lib 
*  @since
*/
class CMceEventSubscribeMtEstablishingState : public CMceEventStateBase
    {
    public:  // Constructors and destructor

        /**
        * C++ default constructor.
        */
        CMceEventSubscribeMtEstablishingState();
        
        /**
        * Destructor.
        */
        ~CMceEventSubscribeMtEstablishingState();
        
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

    private :
    	friend class UT_CMceEventSubscribeMtEstablishingState;
    };

#endif      // CMCEEVENTSUBSCRIBEMTESTABLISHINGSTATE_H
            
// End of File
