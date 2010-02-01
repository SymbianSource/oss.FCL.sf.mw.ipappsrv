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




#ifndef CMCESTATECONFIRMING_H
#define CMCESTATECONFIRMING_H

#include "mcestate.h"


/**
*  Represents MCE server state Confirming
*  State transition is performed beacuse of following
*  trigger events:
*    - EMceResponse
*    - EMceProvisionalResponse
*    - EMceErrorResponse
*    - EMceItcCancel
*/
class CMceStateConfirming : public CMceState,
                            public MMCEStateTransitionHandler
	{
	
public: // Constructors and destructor

	/**
	* Consructor
	*/
	CMceStateConfirming ();

	/**
	* Destructor.
	*/
	~CMceStateConfirming();

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
    * Handles entry of EMceResponse
    * @param aEvent atransition event
    */
    void EntryResponseL( TMceStateTransitionEvent& aEvent );
    
    /**
    * Handles entry of EMceProvisionalResponse
    * @param aEvent atransition event
    */
    void EntryProvisionalResponseL( TMceStateTransitionEvent& aEvent );
    
    /**
    * Handles entry of EMceRedirectionResponse and EMceErrorResponse
    * @param aEvent atransition event
    */
    void EntryErrorResponseL( TMceStateTransitionEvent& aEvent );
    
    /**
    * Handles entry of EMceItcCancel
    * @param aEvent atransition event
    */
    void EntryCancelL( TMceStateTransitionEvent& aEvent );
    
    /**
    * Handles entry of EMceMediaUpdated
    * @param aEvent atransition event
    */
    void EntryMediaUpdatedL( TMceStateTransitionEvent& aEvent );
    
    /**
    * Handles exit of EMceMediaUpdated
    * @param aEvent atransition event
    */
    void ExitMediaUpdatedL( TMceStateTransitionEvent& aEvent );
    
    };

#endif

// End of File
