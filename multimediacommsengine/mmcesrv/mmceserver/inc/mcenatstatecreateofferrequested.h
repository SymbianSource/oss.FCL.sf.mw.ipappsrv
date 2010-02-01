/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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



#ifndef CMCENATSTATECREATEOFFERREQUESTED_H
#define CMCENATSTATECREATEOFFERREQUESTED_H

#include "mcenatstate.h"
#include "mcestate.h"

class TMceNatStateTransitionEvent;

/**
* Media has been updated, and now waiting NAT plugin to initialize itself.
*/
class CMceNatStateCreateOfferRequested : public CMceNatState,
                                 		 public MMceNatStateTransitionHandler
	{	
public: // Constructors and destructor

	/**
	* Consructor
	*/
	CMceNatStateCreateOfferRequested();
    
	/**
	* Destructor.
	*/
	~CMceNatStateCreateOfferRequested();

public: // from CMceState

	/**
	* Checks if state accepts event
	* @param aEvent Transition event
	*/
    TBool Accept( TMceNatStateTransitionEvent& aEvent );
    
	/**
	* Retuns transition handler
	* @return Transition handler
	*/
    inline MMceNatStateTransitionHandler& TransitionHandler() 
        {
        return *this;
        }

public: // from MMceNatStateTransitionHandler

	/**
	* Handles state transition message and executes
	* necessary actions
	* @param aEvent Transition event
	*/
    void EntryL( TMceNatStateTransitionEvent& aEvent );

	/**
	* Resolves state to which current state exit
	* @param aEvent Transition event
	*/
    void ExitL( TMceNatStateTransitionEvent& aEvent );

private: // New functions

	};

#endif // CMCENATSTATECREATEOFFERREQUESTED_H

// End of File
