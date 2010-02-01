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




#ifndef CMCESTATECONFIRMATIONREQUIRED_H
#define CMCESTATECONFIRMATIONREQUIRED_H

#include "mcestate.h"


/**
*  Represents MCE server state Confirmation Required
*  State transition is performed beacuse of following
*  trigger events:
*    - EMceUpdate
*/
class CMceStateConfirmationRequired : public CMceState,
                            public MMCEStateTransitionHandler
	{
	
public: // Constructors and destructor

	/**
	* Consructor
	*/
	CMceStateConfirmationRequired ();

	/**
	* Destructor.
	*/
	~CMceStateConfirmationRequired();

public: // from CMceState

	/**
	* Checks if state accepts event
	* @param aEvent atransition event
	*/
    TBool DoAcceptL( TMceStateTransitionEvent& aEvent );
    
	/**
	* Reruns transition handler
	* @return transition handler
	*/
    inline MMCEStateTransitionHandler& TransitionHandler() 
        {
        return *this;
        }

public: // from MMCEStateTransitionHandler

	/**
	* Hanldes state transition message and executes
	* necessary actions
	* @param aEvent atransition event
	*/
    void EntryL( TMceStateTransitionEvent& aEvent );

	/**
	* Resolves state to which current state exit
	* @param aEvent atransition event
	*/
    void ExitL( TMceStateTransitionEvent& aEvent );
    
private: //handler routines
    
    /**
    * Handles entry of EMceUpdate
    * @param aEvent atransition event
    */
    void EntryUpdateL( TMceStateTransitionEvent& aEvent );
    
    /**
    * Handles entry of EMceMediaReserved
    * @param aEvent atransition event
    */
    void EntryMediaUpdatedL( TMceStateTransitionEvent& aEvent );
 
 	};

#endif

// End of File
