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



#ifndef CMCENATSTATE_H
#define CMCENATSTATE_H

#include <e32base.h>

class TMceNatStateTransitionEvent;

//state indexes
typedef TUint TMceNatStateIndex;

const TMceNatStateIndex KMceNatStateInitializing = 0;
const TMceNatStateIndex KMceNatStateIdle = 1;
const TMceNatStateIndex KMceNatStateCreateOffer = 2;
const TMceNatStateIndex KMceNatStateCreateAnswer = 3;
const TMceNatStateIndex KMceNatStateDecodeOffer = 4;
const TMceNatStateIndex KMceNatStateDecodeAnswer = 5;
const TMceNatStateIndex KMceNatStateWait = 6;
const TMceNatStateIndex KMceNatStateConnected = 7;
const TMceNatStateIndex KMceNatStateCreateOfferRequested = 8;

/**
*  MCE SIP state transition handler
*
*  @lib 
*  @since
*/
class MMceNatStateTransitionHandler
	{

public:

	/**
	* Hanldes state transition message and executes
	* necessary actions
	* @param aEvent atransition event
	*/
    virtual void EntryL( TMceNatStateTransitionEvent& aEvent ) = 0;

	/**
	* Resolves state to which current state exit
	* @param aEvent atransition event
	*/
    virtual void ExitL( TMceNatStateTransitionEvent& aEvent ) = 0;
	
	};

/**
*  MCE SIP state machine
*
*  @lib 
*  @since
*/
class CMceNatState : public CBase
	{
	
public: // Constructors and destructor
    
	/**
	* Destructor.
	*/
	virtual ~CMceNatState();


public: // New functions

	/**
	* Checks if state accepts event
	* @param aEvent atransition event
	*/
    virtual TBool Accept( TMceNatStateTransitionEvent& aEvent ) = 0;
    
	/**
	* Returns transition handler
	* @return transition handler
	*/
    virtual MMceNatStateTransitionHandler& TransitionHandler() = 0;


protected: // Constructors and destructor

	/**
	* Constructor
	*/
    CMceNatState();

	};

#endif // CMCENATSTATE_H

// End of File
