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
* Description:    State for established MO refer.
*
*/





#ifndef CMCEEVENTREFERMOESTABLISHEDSTATE_H
#define CMCEEVENTREFERMOESTABLISHEDSTATE_H

// INCLUDES
#include    "mceeventstatebase.h"

// CLASS DECLARATION

/**
*  State for established MO refer.
*
*  @lib 
*  @since
*/
class CMceEventReferMoEstablishedState : public CMceEventStateBase
    {
    public:  // Constructors and destructor
        
       /**
        * C++ default constructor.
        */
        CMceEventReferMoEstablishedState();

        /**
        * Destructor.
        */
        ~CMceEventReferMoEstablishedState();
        
    protected:  // Functions from base classes
       
        /**
        * From CMceEventStateBase Apply concrete state.
        * This function handles the client initiated requests.
        */
        void HandleL( CMceComEvent& aEvent );
        
        /**
        * From CMceEventReferStateBase Apply concrete state.
        * This function handles the request received from the SIP stack.
        * @param aEvent, a reference to CMceComEvent
        */
        void HandleReceiveRequestL( CMceComEvent& aEvent );
        
        /**
        * From CMceEventReferStateBase Apply concrete state.
        * This function handles the response or error received from the 
        * SIP stack.
        * @param aEvent, a reference to CMceComEvent
        */
        void HandleResponseReceivedL( CMceComEvent& aEvent );

    private ://EUNIT
    	friend class UT_CMceEventReferMoEstablishedState;
    };

#endif      // CMCEEVENTREFERMOESTABLISHEDSTATE_H
            
// End of File
