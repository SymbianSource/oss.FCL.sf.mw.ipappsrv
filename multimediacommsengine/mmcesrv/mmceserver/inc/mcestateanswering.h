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




#ifndef CMCESTATEANSWERING_H
#define CMCESTATEANSWERING_H

#include "mcestate.h"

/**
*  Represents MCE server state Answering
*  State transition is performed beacuse of following
*  trigger events:
*    - EMceAck
*
*/
class CMceStateAnswering : public CMceState,
                           public MMCEStateTransitionHandler
	{
	
public: // Constructors and destructor

	/**
	* Consructor
	*/
	CMceStateAnswering ();

	/**
	* Destructor.
	*/
	~CMceStateAnswering();

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
        
	/**
	* Performed when entering to state
	* @param aEvent atransition event
	*/
    void StateEntry( TMceStateTransitionEvent& aEvent );
        

public: // from MMCEStateTransitionHandler

	/**
	* Hanldes state transition message and executes
	* necessary actions
	* @param aEvent atransition event
	*/
    void EntryL( TMceStateTransitionEvent& aEvent );
    
    /**
	* Handles entry of EMceBye
	* @param aEvent transition event
	*/
    void EntryByeL( TMceStateTransitionEvent& aEvent );

	/**
	* Resolves state to which current state exit
	* @param aEvent atransition event
	*/
    void ExitL( TMceStateTransitionEvent& aEvent );

	};

#endif

// End of File
