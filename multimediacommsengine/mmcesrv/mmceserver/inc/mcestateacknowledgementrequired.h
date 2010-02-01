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




#ifndef CMCESTATEACKNOWLEDGEMENTREQUIRED_H
#define CMCESTATEACKNOWLEDGEMENTREQUIRED_H

#include "mcesrv.h"
#include "mcestate.h"


/**
*  Represents MCE server state Acknowledgement Required
*  State transition is performed beacuse of following
*  trigger events:
*    - EMcePrack
*/
class CMceStateAcknowledgementRequired : public CMceState,
                            public MMCEStateTransitionHandler
	{
	
public: // Constructors and destructor

	/**
	* Consructor
	*/
	CMceStateAcknowledgementRequired ();

	/**
	* Destructor.
	*/
	~CMceStateAcknowledgementRequired();

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
    * Handles entry of EMcePrack
    * @param aEvent atransition event
    */
    void EntryPrackL( TMceStateTransitionEvent& aEvent );
    
    /**
    * Handles entry of EMceMediaUpdated
    * @param aEvent atransition event
    */
    void EntryMediaUpdatedL( TMceStateTransitionEvent& aEvent );
    
    /**
    * Handles entry of EMcePrackReceived
    * @param aEvent atransition evet
    */
    void EntryNoPrackReceivedL( TMceStateTransitionEvent& aEvent );


    /**
    * Handles entry of EMceReliableFailed
    * @param aEvent atransition evet
    */
    void EntryReliableFailedL( TMceStateTransitionEvent& aEvent );
    
    /**
    * Handles too early session acceptance.
    * @param aEvent atransition event
    */
    void EntryEarlyAcceptL( TMceStateTransitionEvent& aEvent );
    
    /**
    * Changes state to updated.
    * @param aEvent atransition event
    */
    void ProceedToUpdatedL( TMceStateTransitionEvent& aEvent );


	// Definitions for unit testing
    MCESRV_UT_DEFINITIONS
	};

#endif

// End of File
