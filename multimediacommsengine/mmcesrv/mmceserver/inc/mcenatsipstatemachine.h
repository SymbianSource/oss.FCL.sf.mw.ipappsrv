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



#ifndef CMCENATSIPSTATEMACHINE_H
#define CMCENATSIPSTATEMACHINE_H

#include <e32base.h>
#include "mcesrv.h"
#include "mcenatstate.h"
#include "mceclientserver.h"
#include "mcedefs.h"

//  FORWARD DECLARATION
class TMceNatStateTransitionEvent;

/**
*  MCE SIP state machine
*
*  @lib 
*  @since
*/
class CMceNatSipStateMachine : public CBase
	{
public: // Constructors and destructor

	/**
	* Two-phased constructor.
	*/
	static CMceNatSipStateMachine* NewL();

	/**
	* Two-phased constructor.
	*/
	static CMceNatSipStateMachine* NewLC();

	/**
	* Destructor.
	*/
	~CMceNatSipStateMachine();


public: //New

	/**
	* return a state by index
	* @param aNewState next state
	*/
    void NewStateL( TMceNatStateTransitionEvent& aEvent,
                    TMceNatStateIndex aNewState );

	/**
	* processes message
	* @param aEvent event
	*/
    void ProcessL( TMceNatStateTransitionEvent& aEvent );

	/**
	* checks if event is accepted by current state.
	* @param aEvent event
	*/
    TBool Accept( TMceNatStateTransitionEvent& aEvent );

	/**
	* returns the current state index.
	* @return state index, if not found KErrNotFound returned
	*/
    TInt State() const;
    
	/**
	* Sets current state as Idle.
	*/
    void ErrorOccurred();
    
private:

	CMceNatSipStateMachine();
    
	void ConstructL();


private: // Data

    // Current state index
    TMceNatStateIndex iCurrentState;
    
    // Array containing all state - objects.
    RPointerArray<CMceNatState> iStates;
    
	//definitions for unit testing
	MCESRV_UT_DEFINITIONS
	
	};

#endif // CMCENATSIPSTATEMACHINE_H

// End of File
