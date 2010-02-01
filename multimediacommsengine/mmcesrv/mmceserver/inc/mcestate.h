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




#ifndef CMCESTATE_H
#define CMCESTATE_H

#include <e32base.h>

//  FORWARD DECLARATION
#include "mceevents.h"
// CLASS DECLARATION

//state indexes
typedef TUint TMceStateIndex;

const TMceStateIndex KMceStateIdle = 0;
const TMceStateIndex KMceStateClientEstablishing = 1;
const TMceStateIndex KMceStateOffering = 2;
const TMceStateIndex KMceStateEstablished = 3;
const TMceStateIndex KMceStateTerminating = 4;
const TMceStateIndex KMceStateTerminated = 5;

const TMceStateIndex KMceStateServerEstablishing = 6;
const TMceStateIndex KMceStateUpdating = 7;
const TMceStateIndex KMceStateUpdated = 8;
const TMceStateIndex KMceStateAnswering = 9;

const TUint KMceStateError = 10;
const TUint KMceStateCanceled = 11;

const TMceStateIndex KMceStateConfirming = 12;
const TMceStateIndex KMceStateClientReserving = 13;
const TMceStateIndex KMceStateAcknowledgementRequired = 14;
const TMceStateIndex KMceStateServerReserving = 15;
const TMceStateIndex KMceStateConfirmationRequired = 16;
const TMceStateIndex KMceStateServerOffering = 17;
const TMceStateIndex KMceStateServerInitializing = 18;



/**
*  MCE SIP state transition handler
*
*  @lib 
*  @since
*/
class MMCEStateTransitionHandler
	{

public:

	/**
	* Hanldes state transition message and executes
	* necessary actions
	* @param aEvent atransition event
	*/
    virtual void EntryL( TMceStateTransitionEvent& aEvent ) = 0;

	/**
	* Resolves state to which current state exit
	* @param aEvent atransition event
	*/
    virtual void ExitL( TMceStateTransitionEvent& aEvent ) = 0;
	
	};
	

/**
*  MCE SIP state machine
*
*  @lib 
*  @since
*/
class CMceState : public CBase
	{
	
public: // Constructors and destructor

    
	/**
	* Destructor.
	*/
	~CMceState();

public: // New


	/**
	* return Id of state
	* @return Id of state
	*/
    TMceStateIndex Id();
    
	/**
	* Checks if state accepts event
	* @param aEvent atransition event
	* @return ETrue, if event is accepted
	*/
    virtual TBool AcceptL( TMceStateTransitionEvent& aEvent );

	/**
	* Checks if state accepts event
	* @param aEvent atransition event
	* @return ETrue, if event is accepted
	*/
    virtual TBool DoAcceptL( TMceStateTransitionEvent& aEvent ) = 0;

	/**
	* Reruns transition handler
	* @return transition handler
	*/
    virtual MMCEStateTransitionHandler& TransitionHandler() = 0;
			 
	/**
	* Performed when entering to state
	* @param aEvent atransition event
	*/
    virtual void StateEntry( TMceStateTransitionEvent& aEvent );

	/**
	* Performed when exiting from satte
	* @param aEvent atransition event
	*/
    virtual void StateExitL( TMceStateTransitionEvent& aEvent );
    
	/**
	* Performed when receiving ICMP  Error
	* @param aEvent atransition event
	*/
    virtual void MediaErrorOccured( TMceStateTransitionEvent& aEvent );

	
	/**
	* Checks if state accepts event
	* @param aEvent atransition event
	* @return ETrue, if event is accepted
	*/
    virtual TBool IsExtensionRequestEvent( TMceStateTransitionEvent& aEvent );
    
    
	/**
	* Handle Non Target Refresh Request
	* @param aEvent atransition event
	*/
    virtual void HandleExtensionRequestEventL( TMceStateTransitionEvent& aEvent );

	
	/**
	* Send Non Target Refresh Request 
	* @param aEvent atransition event
	*/
    virtual void EntrySendExtensionRequestL( TMceStateTransitionEvent& aEvent );
    
    
   	/**
	* Send Response to the Non Target Refresh Request
	* @param aEvent atransition event
	*/
    virtual void EntrySendExtensionResponseL( TMceStateTransitionEvent& aEvent );
    

	/**
	* Receive Non Target Refresh Request
	* @param aEvent atransition event
	*/
    virtual void EntryReceiveExtensionRequestL( TMceStateTransitionEvent& aEvent );
    
    
   	/**
	* Receive Response for Non Target Refresh Request
	* @param aEvent atransition event
	*/
    virtual void EntryReceiveExtensionResponseL( TMceStateTransitionEvent& aEvent );

    
    
protected:

	CMceState ( TMceStateIndex aId );

private:

    TMceStateIndex iId;
    
	};

#endif

// End of File
