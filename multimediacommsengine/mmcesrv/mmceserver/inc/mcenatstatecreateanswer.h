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



#ifndef CMCENATSTATECREATEANSWER_H
#define CMCENATSTATECREATEANSWER_H

#include "mcenatstate.h"
#include "mcestate.h"
/**
* CREATEANSWER
*
*
*/
class CMceNatStateCreateAnswer : public CMceNatState,
                                 public MMceNatStateTransitionHandler
	{	
public: // Constructors and destructor

	/**
	* Consructor
	*/
	CMceNatStateCreateAnswer();
    
	/**
	* Destructor.
	*/
	~CMceNatStateCreateAnswer();

public: // from CMceState

	/**
	* Checks if state accepts event
	* @param aEvent atransition event
	*/
    TBool Accept( TMceNatStateTransitionEvent& aEvent );
    
	/**
	* Returns transition handler
	* @return transition handler
	*/
    inline MMceNatStateTransitionHandler& TransitionHandler() 
        {
        return *this;
        }

public: // from MMCEStateTransitionHandler

	/**
	* Hanldes state transition message and executes
	* necessary actions
	* @param aEvent atransition event
	*/
    void EntryL( TMceNatStateTransitionEvent& aEvent );

	/**
	* Resolves state to which current state exit
	* @param aEvent atransition event
	*/
    void ExitL( TMceNatStateTransitionEvent& aEvent );

private: // New functions

    void ExitErrorL( TMceNatStateTransitionEvent& aEvent );

    void ExitHandleInviteL( TMceNatStateTransitionEvent& aEvent );
    
    void ExitDefaultL( TMceNatStateTransitionEvent& aEvent );

	};

#endif // CMCENATSTATECREATEANSWER_H

// End of File
