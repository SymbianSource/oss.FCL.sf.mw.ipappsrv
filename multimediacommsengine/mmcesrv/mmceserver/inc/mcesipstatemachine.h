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




#ifndef CMCESIPSTATEMACHINE_H
#define CMCESIPSTATEMACHINE_H

#include <e32base.h>
#include "mcesrv.h"
#include "mcestate.h"
#include "mceclientserver.h"

//  FORWARD DECLARATION
class CMceSipSession;
class TMceStateTransitionEvent;

// CLASS DECLARATION


/**
*  MCE SIP state machine
*
*  @lib 
*  @since
*/
class CMceSipStateMachine : public CBase
	{
	
public: // Constructors and destructor

	/**
	* Two-phased constructor.
	*/
	static CMceSipStateMachine* NewL ();

	/**
	* Two-phased constructor.
	*/
	static CMceSipStateMachine* NewLC ();

	/**
	* Destructor.
	*/
	~CMceSipStateMachine();

public: //New


	/**
	* returns ETrue, if event will be consumed
	* @param aEvent the event
	* @return ETrue, if event will be consumed
	*/
    TBool Consumes( TMceStateTransitionEvent& aEvent );

	/**
	* return a state by index
	* @param aStateIndex state index
	* @return the state
	*/
    CMceState& State( TMceStateIndex aStateIndex );
    
	/**
	* processes message
	* @param aSession, SIP session.
	* @param aEvent event
	*/
    void ProcessL( TMceStateTransitionEvent& aEvent );
    

public: // New

    
private:

	CMceSipStateMachine ();

	void ConstructL ();

private: // Data


    RPointerArray<CMceState> iStates;
    
	//definitions for unit testing
	MCESRV_UT_DEFINITIONS

	};

#endif

// End of File
