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




#ifndef CMCESTATEESTABLISHED_H
#define CMCESTATEESTABLISHED_H

#include "mcestate.h"

//  FORWARD DECLARATION

// CLASS DECLARATION

/**
*  MCE SIP state machine
*
*  @lib 
*  @since
*/
class CMceStateEstablished : public CMceState,
                             public MMCEStateTransitionHandler
	{
	
public: // Constructors and destructor

	/**
	* Consructor
	*/
	CMceStateEstablished ();

	/**
	* Destructor.
	*/
	~CMceStateEstablished();

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
    
    /**
	* Performed when receiving MediaError
	* @param aEvent atransition event
	*/
    void MediaErrorOccured( TMceStateTransitionEvent& aEvent );
        

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


private://handler routines


	/**
	* Handles entry of EMceItcUpdate
	* @param aEvent transition event
	*/
    void EntryUpdateL( TMceStateTransitionEvent& aEvent );

    /**
    * Handles entry of EMceUpdate
    * @param aEvent transition event
    */
    void EntryInUpdateL( TMceStateTransitionEvent& aEvent );

	/**
	* Handles entry of EMceInvite
	* @param aEvent transition event
	*/
    void EntryInviteL( TMceStateTransitionEvent& aEvent );

	/**
	* Handles entry of EMceItcTerminateSession
	* @param aEvent transition event
	*/
    void EntryTerminateSessionL( TMceStateTransitionEvent& aEvent );

	/**
	* Handles entry of EMceBye
	* @param aEvent transition event
	*/
    void EntryByeL( TMceStateTransitionEvent& aEvent );

	/**
	* Handles entry of EMceItcEnable, EMceItcDisable,
	* EMceItcSendRR, EMceItcSendSR, EMceMediaPaused, EMceMediaResumed,
	* EMceMediaInactivityTimeout
	* @param aEvent transition event
	*/
    void EntryMediaL( TMceStateTransitionEvent& aEvent );
	
	};

#endif

// End of File
